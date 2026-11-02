#include "fly.hpp"

#include "components/engine/classes/moon_walker/moon_walker.hpp"
#include "components/engine/engine.hpp"
#include "components/engine/il2cpp.hpp"

#include "imgui/imgui.h"

namespace features::fly
{
    namespace
    {
        bool last_on_ = false;
    }

    void tick() noexcept
    {
        auto* walker = engine::local_walker();
        if (walker == nullptr || !moon_walker::ready())
        {
            last_on_ = false;

            return;
        }

        if (enabled)
        {
            if (auto* m = moon_walker::method_set_flying())
            {
                bool on = true;
                bool nc = noclip;
                void* args[] = { &on, &nc };
                il2cpp::il2cpp_object* exc = nullptr;
                (void)il2cpp::runtime_invoke(m, walker, args, &exc);
            }
            if (auto* m = moon_walker::set_fly_speed())
            {
                float v = speed;
                void* args[] = { &v };
                il2cpp::il2cpp_object* exc = nullptr;
                (void)il2cpp::runtime_invoke(m, walker, args, &exc);
            }
            last_on_ = true;
        }
        else if (last_on_)
        {
            if (auto* m = moon_walker::method_set_flying())
            {
                bool on = false;
                bool nc = false;
                void* args[] = { &on, &nc };
                il2cpp::il2cpp_object* exc = nullptr;
                (void)il2cpp::runtime_invoke(m, walker, args, &exc);
            }
            last_on_ = false;
        }
    }

    void draw_menu() noexcept
    {
        if (ImGui::CollapsingHeader("Fly"))
        {
            ImGui::Checkbox("Enable##fly", &enabled);
            ImGui::Checkbox("Noclip (phase through geometry)", &noclip);
            ImGui::SliderFloat("Fly speed", &speed, 1.0f, 200.0f, "%.1f");
        }
    }
}
