#include "speed.hpp"

#include "components/engine/classes/moon_walker/moon_walker.hpp"
#include "components/engine/engine.hpp"
#include "components/engine/il2cpp.hpp"

#include "imgui/imgui.h"

namespace features::speed
{
    namespace
    {
        bool last_applied_ = false;

        void apply_single(il2cpp::il2cpp_object* walker,
                          il2cpp::il2cpp_method_info* method,
                          float value) noexcept
        {
            if (walker == nullptr || method == nullptr)
            {
                return;
            }
            void* args[] = { &value };
            il2cpp::il2cpp_object* exc = nullptr;
            (void)il2cpp::runtime_invoke(method, walker, args, &exc);
        }
    }

    void tick() noexcept
    {
        auto* walker = engine::local_walker();
        if (walker == nullptr || !moon_walker::ready())
        {
            last_applied_ = false;

            return;
        }

        if (enabled)
        {
            apply_single(walker, moon_walker::set_dev_speed_multiplier(),       move_multiplier);
            apply_single(walker, moon_walker::set_equipment_speed_multiplier(), equipment_speed);
            last_applied_ = true;
        }
        else if (last_applied_)
        {
            apply_single(walker, moon_walker::set_dev_speed_multiplier(),       1.0f);
            apply_single(walker, moon_walker::set_equipment_speed_multiplier(), 1.0f);
            last_applied_ = false;
        }
    }

    void draw_menu() noexcept
    {
        if (ImGui::CollapsingHeader("Speed"))
        {
            ImGui::Checkbox("Enable##speed", &enabled);
            ImGui::SliderFloat("Move multiplier",      &move_multiplier, 0.5f, 10.0f, "%.2fx");
            ImGui::SliderFloat("Equipment speed",      &equipment_speed, 0.5f, 10.0f, "%.2fx");
            ImGui::TextDisabled("DevSpeedMultiplier + EquipmentSpeedMultiplier");
        }
    }
}
