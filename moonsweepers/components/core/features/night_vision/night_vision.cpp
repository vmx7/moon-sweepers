#include "night_vision.hpp"

#include "components/engine/classes/moon_night_vision/moon_night_vision.hpp"
#include "components/engine/classes/moon_walker/moon_walker.hpp"
#include "components/engine/engine.hpp"

#include "imgui/imgui.h"

namespace features::night_vision
{
    namespace
    {
        bool last_on_ = false;
    }

    void tick() noexcept
    {
        auto* walker = engine::local_walker();
        if (walker == nullptr)
        {
            last_on_ = false;

            return;
        }

        if (enabled)
        {
            moon_walker::set_night_vision_field(walker, true);
            if (moon_night_vision::ready())
            {
                moon_night_vision::set_amount(intensity);
            }
            last_on_ = true;
        }
        else if (last_on_)
        {
            moon_walker::set_night_vision_field(walker, false);
            if (moon_night_vision::ready())
            {
                moon_night_vision::set_amount(0.0f);
            }
            last_on_ = false;
        }
    }

    void draw_menu() noexcept
    {
        if (ImGui::CollapsingHeader("Night vision"))
        {
            ImGui::Checkbox("Enable##nv",      &enabled);
            ImGui::SliderFloat("Intensity",    &intensity, 0.1f, 3.0f, "%.2f");
        }
    }
}
