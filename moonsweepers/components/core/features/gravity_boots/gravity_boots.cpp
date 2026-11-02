#include "gravity_boots.hpp"

#include "components/engine/classes/moon_walker/moon_walker.hpp"
#include "components/engine/engine.hpp"

#include "imgui/imgui.h"

namespace features::gravity_boots
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
            moon_walker::set_gravity_boots_field(walker, true);
            last_on_ = true;
        }
        else if (last_on_)
        {
            moon_walker::set_gravity_boots_field(walker, false);
            last_on_ = false;
        }
    }

    void draw_menu() noexcept
    {
        if (ImGui::CollapsingHeader("Gravity boots"))
        {
            ImGui::Checkbox("Enable##boots", &enabled);
        }
    }
}
