#include "gravity_boots.hpp"

#include "components/engine/classes/moon_walker/moon_walker.hpp"
#include "components/engine/engine.hpp"
#include "components/engine/il2cpp.hpp"

#include "imgui/imgui.h"

namespace features::gravity_boots
{
    namespace
    {
        bool last_state_ = false;
    }

    void tick() noexcept
    {
        auto* walker = engine::local_walker();
        if (walker == nullptr || !moon_walker::ready())
        {
            return;
        }

        if (enabled == last_state_)
        {
            return;
        }

        if (auto* m = moon_walker::method_set_gravity_boots())
        {
            bool v = enabled;
            void* args[] = { &v };
            il2cpp::il2cpp_object* exc = nullptr;
            (void)il2cpp::runtime_invoke(m, walker, args, &exc);
            last_state_ = enabled;
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
