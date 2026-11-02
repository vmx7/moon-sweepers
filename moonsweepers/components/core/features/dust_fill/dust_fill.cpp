#include "dust_fill.hpp"

#include <cstdio>
#include <cstring>

#include <windows.h>

#include "imgui/imgui.h"

namespace features::dust_fill
{
    namespace
    {
        constexpr std::uintptr_t sig_mark_lo = 0x6A174DDD;
        constexpr std::uintptr_t sig_mark_hi = 0xFFFFFFFF;

        [[nodiscard]] bool read_double(std::uintptr_t addr, double& out) noexcept
        {
            if (addr == 0)
            {
                return false;
            }
            std::memcpy(&out, reinterpret_cast<const void*>(addr), sizeof(out));

            return true;
        }

        void write_double(std::uintptr_t addr, double v) noexcept
        {
            if (addr == 0)
            {
                return;
            }

            DWORD old = 0;
            VirtualProtect(reinterpret_cast<void*>(addr), sizeof(v), PAGE_READWRITE, &old);
            std::memcpy(reinterpret_cast<void*>(addr), &v, sizeof(v));
            VirtualProtect(reinterpret_cast<void*>(addr), sizeof(v), old, &old);
        }
    }

    void tick() noexcept
    {
        if (!enabled || slot_addr == 0)
        {
            return;
        }

        double now = 0.0;
        if (!read_double(slot_addr, now))
        {
            return;
        }

        if (now < target_kg * 0.9)
        {
            write_double(slot_addr, static_cast<double>(target_kg));
        }
    }

    void draw_menu() noexcept
    {
        if (ImGui::CollapsingHeader("Dust auto-fill"))
        {
            ImGui::Checkbox("Enable##dust", &enabled);
            ImGui::SliderFloat("Target kg", &target_kg, 10.0f, 10000.0f, "%.0f");

            char buf[32]{};
            std::snprintf(buf, sizeof(buf), "%llX",
                static_cast<unsigned long long>(slot_addr));
            if (ImGui::InputText("Slot address (hex)", buf, sizeof(buf),
                                 ImGuiInputTextFlags_CharsHexadecimal))
            {
                slot_addr = static_cast<std::uintptr_t>(_strtoui64(buf, nullptr, 16));
            }

            if (slot_addr != 0)
            {
                double current = 0.0;
                read_double(slot_addr, current);
                ImGui::Text("Current: %.3f kg", current);
            }
            else
            {
                ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f),
                    "Set slot address first (use the scanner).");
            }
        }
    }
}
