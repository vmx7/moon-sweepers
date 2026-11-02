#include "dust_locator.hpp"

#include <cmath>
#include <cstddef>
#include <cstdio>
#include <cstring>

#include <windows.h>

#include "components/core/features/dust_fill/dust_fill.hpp"
#include "utilities/log/log.hpp"

#include "imgui/imgui.h"

namespace features::dust_locator
{
    namespace
    {
        constexpr unsigned char signature[8] = {
            0xDD, 0x4D, 0x17, 0x6A, 0xFF, 0xFF, 0xFF, 0xFF
        };
        constexpr std::ptrdiff_t klass_ptr_off = 0x08;
        constexpr std::ptrdiff_t dust_off      = 0x10;

        std::vector<hit> results_;

        [[nodiscard]] bool addr_in_user_space(std::uintptr_t a) noexcept
        {
            return a >= 0x10000 && a <= 0x00007FFFFFFEFFFF;
        }

        [[nodiscard]] bool scan_region(std::uintptr_t base, std::size_t size) noexcept
        {
            const auto* bytes = reinterpret_cast<const unsigned char*>(base);
            if (size < sizeof(signature) + dust_off + sizeof(double))
            {
                return false;
            }

            for (std::size_t i = 0; i + sizeof(signature) + dust_off + sizeof(double) < size; ++i)
            {
                if (std::memcmp(bytes + i, signature, sizeof(signature)) != 0)
                {
                    continue;
                }

                const auto sig_ea = base + i;

                std::uintptr_t klass = 0;
                std::memcpy(&klass, bytes + i + klass_ptr_off, sizeof(klass));
                if (!addr_in_user_space(klass))
                {
                    continue;
                }
                if ((klass >> 28) != (sig_ea >> 28))
                {
                    continue;
                }

                double value = 0.0;
                std::memcpy(&value, bytes + i + dust_off, sizeof(value));
                if (!std::isfinite(value) || value < -1e-6 || value > 1e8)
                {
                    continue;
                }

                results_.push_back({ sig_ea + dust_off, value });
            }

            return true;
        }
    }

    void scan() noexcept
    {
        results_.clear();

        SYSTEM_INFO si{};
        GetSystemInfo(&si);

        std::uintptr_t cursor = reinterpret_cast<std::uintptr_t>(si.lpMinimumApplicationAddress);
        const auto      end   = reinterpret_cast<std::uintptr_t>(si.lpMaximumApplicationAddress);

        while (cursor < end)
        {
            MEMORY_BASIC_INFORMATION mbi{};
            if (VirtualQuery(reinterpret_cast<LPCVOID>(cursor), &mbi, sizeof(mbi)) == 0)
            {
                cursor += 0x10000;

                continue;
            }

            const auto base = reinterpret_cast<std::uintptr_t>(mbi.BaseAddress);
            const auto size = mbi.RegionSize;

            if (mbi.State == MEM_COMMIT &&
                (mbi.Protect == PAGE_READWRITE || mbi.Protect == PAGE_EXECUTE_READWRITE) &&
                mbi.Type == MEM_PRIVATE &&
                size <= 128ull * 1024 * 1024)
            {
                scan_region(base, size);
            }

            cursor = base + size;
            if (cursor <= base)
            {
                break;
            }
        }

        lg::info("locator", "scan done hits={}", results_.size());
    }

    const std::vector<hit>& results() noexcept
    {
        return results_;
    }

    void draw_menu() noexcept
    {
        if (ImGui::CollapsingHeader("Dust slot locator"))
        {
            if (ImGui::Button("Scan heap"))
            {
                scan();
            }
            ImGui::SameLine();
            ImGui::Text("hits: %zu", results_.size());

            if (ImGui::BeginTable("hits", 3,
                    ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollY,
                    ImVec2(0.0f, 180.0f)))
            {
                ImGui::TableSetupColumn("Address");
                ImGui::TableSetupColumn("Value");
                ImGui::TableSetupColumn("Pick");
                ImGui::TableHeadersRow();

                for (std::size_t i = 0; i < results_.size(); ++i)
                {
                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::Text("0x%llX", static_cast<unsigned long long>(results_[i].addr));
                    ImGui::TableNextColumn();
                    ImGui::Text("%.3f", results_[i].value);
                    ImGui::TableNextColumn();

                    char btn[32]{};
                    std::snprintf(btn, sizeof(btn), "Use##dl%zu", i);
                    if (ImGui::Button(btn))
                    {
                        dust_fill::slot_addr = results_[i].addr;
                    }
                }
                ImGui::EndTable();
            }
        }
    }
}
