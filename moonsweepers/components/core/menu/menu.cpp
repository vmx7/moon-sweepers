#include "menu.hpp"

#include "components/core/features/brush_tuner/brush_tuner.hpp"
#include "components/core/features/flag_esp/flag_esp.hpp"
#include "components/core/features/fly/fly.hpp"
#include "components/core/features/gravity_boots/gravity_boots.hpp"
#include "components/core/features/night_vision/night_vision.hpp"
#include "components/core/features/speed/speed.hpp"

#include "imgui/imgui.h"

namespace menu
{
    namespace
    {
        bool open_ = true;
    }

    void render() noexcept
    {
        features::fly::tick();
        features::speed::tick();
        features::night_vision::tick();
        features::gravity_boots::tick();
        features::brush_tuner::tick();

        ImGui::SetNextWindowSize(ImVec2(420.0f, 600.0f), ImGuiCond_FirstUseEver);
        if (!ImGui::Begin("moonsweepers", &open_))
        {
            ImGui::End();

            return;
        }

        features::fly::draw_menu();
        features::speed::draw_menu();
        features::night_vision::draw_menu();
        features::gravity_boots::draw_menu();
        features::brush_tuner::draw_menu();
        features::flag_esp::draw_menu();

        ImGui::End();
    }
}
