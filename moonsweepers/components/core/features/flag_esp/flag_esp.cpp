#include "flag_esp.hpp"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <limits>
#include <vector>

#include "components/engine/classes/camera/camera.hpp"
#include "components/engine/classes/moon_landmark/moon_landmark.hpp"
#include "components/engine/classes/moon_walker/moon_walker.hpp"
#include "components/engine/classes/moon_world/moon_world.hpp"
#include "components/engine/engine.hpp"
#include "components/engine/enum/landmark_kind/landmark_kind.hpp"
#include "utilities/log/log.hpp"

namespace features::flag_esp
{
    namespace
    {
        std::vector<moon_landmark::marker> cached_;
        int                                selected_ = 0;

        [[nodiscard]] ImU32 color_for(sdk::landmark_kind k) noexcept
        {
            switch (k)
            {
                case sdk::landmark_kind::flag: return ImGui::ColorConvertFloat4ToU32(color_flag);
                case sdk::landmark_kind::site: return ImGui::ColorConvertFloat4ToU32(color_site);
                default:                       return ImGui::ColorConvertFloat4ToU32(color_other);
            }
        }

        void teleport_to(const moon_landmark::marker& m) noexcept
        {
            auto* w = engine::local_walker();
            if (w == nullptr)
            {
                lg::warn("esp", "teleport failed: no local walker");

                return;
            }
            moon_walker::teleport(w, m.world.x, m.world.y, m.world.z);
            lg::info("esp", "teleport to '{}' ({:.1f},{:.1f},{:.1f})",
                m.name[0] ? m.name : "<unnamed>",
                m.world.x, m.world.y, m.world.z);
        }

        void teleport_nearest_unraised() noexcept
        {
            if (cached_.empty())
            {
                return;
            }
            const auto cam = camera::position();
            std::size_t best = 0;
            float best_d2 = std::numeric_limits<float>::max();
            for (std::size_t i = 0; i < cached_.size(); ++i)
            {
                const auto& m = cached_[i];
                if (m.raised || m.id < 0)
                {
                    continue;
                }
                const float dx = m.world.x - cam.x;
                const float dy = m.world.y - cam.y;
                const float dz = m.world.z - cam.z;
                const float d2 = dx * dx + dy * dy + dz * dz;
                if (d2 < best_d2)
                {
                    best_d2 = d2;
                    best    = i;
                }
            }
            if (best_d2 < std::numeric_limits<float>::max())
            {
                teleport_to(cached_[best]);
            }
            else
            {
                lg::info("esp", "no unraised landmarks left");
            }
        }
    }

    void render() noexcept
    {
        if (!moon_landmark::ready() || !camera::ready())
        {
            return;
        }

        cached_ = moon_landmark::snapshot();

        if (tp_bind != ImGuiKey_None && ImGui::IsKeyPressed(tp_bind, false)
            && !ImGui::GetIO().WantCaptureKeyboard)
        {
            teleport_nearest_unraised();
        }

        if (!enabled)
        {
            return;
        }

        const auto cam = camera::position();

        auto* dl = ImGui::GetForegroundDrawList();

        for (const auto& m : cached_)
        {
            if (only_unraised && m.raised)
            {
                continue;
            }

            const float dx = m.world.x - cam.x;
            const float dy = m.world.y - cam.y;
            const float dz = m.world.z - cam.z;
            const float d  = std::sqrt(dx * dx + dy * dy + dz * dz);
            if (d > max_distance)
            {
                continue;
            }

            sdk::vector2 screen{};
            if (!camera::world_to_screen(m.world, screen))
            {
                continue;
            }

            const auto col = color_for(m.kind);

            dl->AddCircleFilled(ImVec2(screen.x, screen.y), 3.0f, col, 12);

            if (show_label)
            {
                const char* label = m.name[0] ? m.name : sdk::to_string(m.kind);
                dl->AddText(ImVec2(screen.x + 5.0f, screen.y - text_size), col, label);
            }
            if (show_distance)
            {
                char buf[32]{};
                std::snprintf(buf, sizeof(buf), "%.0f m", d);
                dl->AddText(ImVec2(screen.x + 5.0f, screen.y + 2.0f), col, buf);
            }
        }
    }

    void draw_menu() noexcept
    {
        if (!ImGui::CollapsingHeader("Flag / landmark ESP"))
        {
            return;
        }

        ImGui::Checkbox("Enable##flagesp",    &enabled);
        ImGui::Checkbox("Only unraised",      &only_unraised);
        ImGui::Checkbox("Show label",         &show_label);
        ImGui::Checkbox("Show distance",      &show_distance);
        ImGui::SliderFloat("Max distance (m)", &max_distance, 10.0f, 500000.0f, "%.0f");
        ImGui::SliderFloat("Text size",        &text_size,     8.0f, 32.0f,   "%.0f");

        ImGui::ColorEdit4("Flag color",  reinterpret_cast<float*>(&color_flag));
        ImGui::ColorEdit4("Site color",  reinterpret_cast<float*>(&color_site));
        ImGui::ColorEdit4("Other color", reinterpret_cast<float*>(&color_other));

        ImGui::Separator();
        std::size_t flag_raised = 0;
        std::size_t flag_total  = 0;
        std::size_t site_raised = 0;
        std::size_t site_total  = 0;
        for (const auto& m : cached_)
        {
            if (m.kind == sdk::landmark_kind::flag)
            {
                ++flag_total;
                if (m.raised) { ++flag_raised; }
            }
            else if (m.kind == sdk::landmark_kind::site)
            {
                ++site_total;
                if (m.raised) { ++site_raised; }
            }
        }
        ImGui::Text("Flags: %zu/%zu  Sites: %zu/%zu  (total %zu)",
            flag_raised, flag_total, site_raised, site_total, cached_.size());

        if (ImGui::Button("Teleport to nearest unraised"))
        {
            teleport_nearest_unraised();
        }
        ImGui::SameLine();
        if (ImGui::Button("Raise all unraised landmarks"))
        {
            auto* world_inst = moon_walker::world(engine::local_walker());
            auto* player     = moon_world::local_player();
            if (world_inst == nullptr || player == nullptr)
            {
                lg::warn("esp", "raise all: no world or local player");
            }
            else
            {
                int ok = 0;
                for (const auto& m : cached_)
                {
                    if (m.raised || m.id < 0)
                    {
                        continue;
                    }
                    if (moon_world::crew_raise_landmark(world_inst, player, m.id))
                    {
                        ++ok;
                    }
                }
                lg::info("esp", "raised {} landmarks", ok);
            }
        }

        const char* key_name = (tp_bind == ImGuiKey_None) ? "none" : ImGui::GetKeyName(tp_bind);
        char btn[64]{};
        std::snprintf(btn, sizeof(btn), "%s [%s]##tp_bind",
            tp_bind_rebinding ? "press key..." : "Bind TP key", key_name);
        if (ImGui::Button(btn))
        {
            tp_bind_rebinding = !tp_bind_rebinding;
        }
        ImGui::SameLine();
        if (ImGui::SmallButton("Clear##tp_bind"))
        {
            tp_bind           = ImGuiKey_None;
            tp_bind_rebinding = false;
        }
        if (tp_bind_rebinding)
        {
            for (int k = ImGuiKey_NamedKey_BEGIN; k < ImGuiKey_NamedKey_END; ++k)
            {
                const auto key = static_cast<ImGuiKey>(k);
                if (key == ImGuiKey_Insert || key == ImGuiKey_End)
                {
                    continue;
                }
                if (ImGui::IsKeyPressed(key, false))
                {
                    tp_bind           = key;
                    tp_bind_rebinding = false;
                    break;
                }
            }
        }

        if (!cached_.empty())
        {
            selected_ = std::clamp(selected_, 0, static_cast<int>(cached_.size()) - 1);
            const auto& current = cached_[static_cast<std::size_t>(selected_)];
            const char* preview = current.name[0] ? current.name : sdk::to_string(current.kind);

            if (ImGui::BeginCombo("Pick landmark", preview))
            {
                for (std::size_t i = 0; i < cached_.size(); ++i)
                {
                    const auto& m = cached_[i];
                    if (only_unraised && m.raised)
                    {
                        continue;
                    }
                    const char* label = m.name[0] ? m.name : sdk::to_string(m.kind);
                    char line[128]{};
                    std::snprintf(line, sizeof(line), "%s [%s]%s##%zu",
                        label, sdk::to_string(m.kind),
                        m.raised ? " (raised)" : "",
                        i);
                    const bool is_sel = (static_cast<int>(i) == selected_);
                    if (ImGui::Selectable(line, is_sel))
                    {
                        selected_ = static_cast<int>(i);
                    }
                    if (is_sel)
                    {
                        ImGui::SetItemDefaultFocus();
                    }
                }
                ImGui::EndCombo();
            }

            if (ImGui::Button("Teleport to selected"))
            {
                teleport_to(cached_[static_cast<std::size_t>(selected_)]);
            }
        }
    }
}
