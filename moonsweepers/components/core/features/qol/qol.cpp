#include "qol.hpp"

#include "components/engine/classes/moon_walker/moon_walker.hpp"
#include "components/engine/engine.hpp"
#include "utilities/log/log.hpp"

#include "imgui/imgui.h"

namespace features::qol
{
    void tick() noexcept
    {
        if (!tune_enabled)
        {
            return;
        }

        auto* walker = engine::local_walker();
        if (walker == nullptr)
        {
            return;
        }

        moon_walker::set_climb_speed(walker,   climb_speed);
        moon_walker::set_earth_gravity(walker, earth_gravity);
    }

    void draw_menu() noexcept
    {
        if (ImGui::CollapsingHeader("QoL"))
        {
            if (ImGui::Button("Unstick me"))
            {
                if (auto* w = engine::local_walker())
                {
                    moon_walker::call_unstick(w);
                    lg::info("qol", "Unstick called");
                }
            }

            ImGui::Checkbox("Tune climb + gravity",   &tune_enabled);
            ImGui::SliderFloat("Climb speed",         &climb_speed,   0.5f, 50.0f, "%.2f");
            ImGui::SliderFloat("Earth gravity (m/s2)", &earth_gravity, 0.0f, 30.0f, "%.2f");
            ImGui::TextDisabled("Earth gravity 0 = moon-grav everywhere, 9.81 = default indoors");
        }
    }
}
