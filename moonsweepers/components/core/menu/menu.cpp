#include "menu.hpp"

#include "components/core/features/brush_tuner/brush_tuner.hpp"
#include "components/core/features/dev_tools/dev_tools.hpp"
#include "components/core/features/flag_esp/flag_esp.hpp"
#include "components/core/features/fly/fly.hpp"
#include "components/core/features/gravity_boots/gravity_boots.hpp"
#include "components/core/features/night_vision/night_vision.hpp"
#include "components/core/features/qol/qol.hpp"
#include "components/core/features/speed/speed.hpp"
#include "components/core/features/teammate_tp/teammate_tp.hpp"

#include "imgui/imgui.h"

namespace menu
{
    namespace
    {
        bool open_ = true;
    }

    void tick_features() noexcept
    {
        features::fly::tick();
        features::speed::tick();
        features::night_vision::tick();
        features::gravity_boots::tick();
        features::brush_tuner::tick();
        features::qol::tick();
    }

    void render() noexcept
    {
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
        features::qol::draw_menu();
        features::brush_tuner::draw_menu();
        features::flag_esp::draw_menu();
        features::teammate_tp::draw_menu();
        features::dev_tools::draw_menu();

        ImGui::End();
    }
}
