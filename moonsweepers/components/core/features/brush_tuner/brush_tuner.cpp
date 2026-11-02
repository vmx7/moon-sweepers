#include "brush_tuner.hpp"

#include <cstdint>

#include "components/engine/classes/moon_brush/moon_brush.hpp"
#include "components/engine/classes/moon_crew_session/moon_crew_session.hpp"
#include "components/engine/classes/moon_player_visual/moon_player_visual.hpp"
#include "components/engine/il2cpp.hpp"

#include "imgui/imgui.h"

namespace features::brush_tuner
{
    namespace
    {
        void write_field(il2cpp::il2cpp_object* obj,
                         il2cpp::il2cpp_field_info* field,
                         void* src) noexcept
        {
            if (obj == nullptr || field == nullptr)
            {
                return;
            }
            il2cpp::field_set_value(obj, field, src);
        }
    }

    void tick() noexcept
    {
        if (!enabled || !moon_brush::ready())
        {
            return;
        }

        auto* visual = moon_crew_session::local_visual();
        auto* brush  = moon_player_visual::brush(visual);
        if (brush == nullptr)
        {
            return;
        }

        write_field(brush, moon_brush::field_cleaning_width(),      &cleaning_width);
        write_field(brush, moon_brush::field_cleaning_depth(),      &cleaning_depth);
        write_field(brush, moon_brush::field_cleaning_edge_width(), &cleaning_edge);
        write_field(brush, moon_brush::field_reach(),               &reach);
        write_field(brush, moon_brush::field_removal_rate(),        &removal_rate);
        write_field(brush, moon_brush::field_stroke_interval(),     &stroke_interval);

        if (auto* f = moon_brush::field_clear_in_one_pass())
        {
            std::uint8_t b = clear_in_one_pass ? 1 : 0;
            il2cpp::field_set_value(brush, f, &b);
        }
    }

    void draw_menu() noexcept
    {
        if (ImGui::CollapsingHeader("Brush tuner"))
        {
            ImGui::Checkbox("Enable##brush",       &enabled);
            ImGui::SliderFloat("Cleaning width",   &cleaning_width,  0.5f, 50.0f,  "%.2f m");
            ImGui::SliderFloat("Cleaning depth",   &cleaning_depth,  0.5f, 50.0f,  "%.2f m");
            ImGui::SliderFloat("Cleaning edge",    &cleaning_edge,   0.0f, 20.0f,  "%.2f m");
            ImGui::SliderFloat("Reach",            &reach,           1.0f, 100.0f, "%.1f m");
            ImGui::SliderFloat("Removal rate",     &removal_rate,    1.0f, 2000.0f, "%.0f");
            ImGui::SliderFloat("Stroke interval",  &stroke_interval, 0.0f, 1.0f,   "%.3f s");
            ImGui::Checkbox("Clear in one pass",   &clear_in_one_pass);

            if (ImGui::SmallButton("Preset: Wide + smooth"))
            {
                cleaning_width    = 20.0f;
                cleaning_depth    = 20.0f;
                cleaning_edge     = 3.0f;
                reach             = 30.0f;
                removal_rate      = 400.0f;
                stroke_interval   = 0.05f;
                clear_in_one_pass = false;
            }
            ImGui::SameLine();
            if (ImGui::SmallButton("Preset: Vanilla"))
            {
                cleaning_width    = 2.0f;
                cleaning_depth    = 2.0f;
                cleaning_edge     = 0.5f;
                reach             = 5.0f;
                removal_rate      = 50.0f;
                stroke_interval   = 0.1f;
                clear_in_one_pass = false;
            }

            ImGui::TextDisabled("Lag? Keep removal<=1000, clear-in-one-pass OFF,");
            ImGui::TextDisabled("and stroke interval >= 0.05 s.");
        }
    }
}
