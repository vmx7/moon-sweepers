#include "dev_tools.hpp"

#include "components/engine/classes/camera/camera.hpp"
#include "components/engine/classes/moon_steam_lobby/moon_steam_lobby.hpp"
#include "components/engine/classes/moon_walker/moon_walker.hpp"
#include "components/engine/classes/moon_world/moon_world.hpp"
#include "components/engine/engine.hpp"
#include "utilities/log/log.hpp"

#include "imgui/imgui.h"

namespace features::dev_tools
{
    namespace
    {
        [[nodiscard]] il2cpp::il2cpp_object* world() noexcept
        {
            return moon_walker::world(engine::local_walker());
        }
    }

    void draw_menu() noexcept
    {
        if (!ImGui::CollapsingHeader("Dev Tools (Host only)"))
        {
            return;
        }

        auto* world_inst = world();
        auto* player     = moon_world::local_player();

        if (world_inst == nullptr || player == nullptr)
        {
            ImGui::TextDisabled("Waiting for local walker + MoonWorld...");

            return;
        }

        const bool host = moon_steam_lobby::is_host();
        if (host)
        {
            ImGui::TextColored(ImVec4(0.4f, 1.0f, 0.4f, 1.0f), "You are HOST - Dev* writes persist.");
        }
        else
        {
            ImGui::TextColored(ImVec4(1.0f, 0.6f, 0.3f, 1.0f),
                "You are CLIENT - host snapshot will overwrite Dev* writes.");
            if (ImGui::Button("Force IsHost = true (local only, risky)"))
            {
                moon_steam_lobby::set_is_host(true);
                lg::info("dev", "forced MoonSteamLobby.IsHost = true");
            }
        }

        ImGui::Separator();

        ImGui::InputFloat("Dollars##dev",   &money_amount,   100.0f, 10000.0f, "%.0f");
        if (ImGui::Button("DevAddMoney"))
        {
            moon_world::dev_add_money(world_inst, player, static_cast<double>(money_amount));
            lg::info("dev", "DevAddMoney {:.0f}", money_amount);
        }

        ImGui::InputFloat("Carried dust (kg)##dev", &dust_kg_amount, 10.0f, 100.0f, "%.1f");
        if (ImGui::Button("DevSetCarriedDust"))
        {
            moon_world::dev_set_carried_dust(world_inst, player, static_cast<double>(dust_kg_amount));
            lg::info("dev", "DevSetCarriedDust {:.1f} kg", dust_kg_amount);
        }

        ImGui::SliderInt("Rings##dev", &region_rings, 1, 20);
        if (ImGui::Button("DevCompleteRegions (around me)"))
        {
            const auto cam = camera::position();
            const auto n   = moon_world::dev_complete_regions(world_inst, cam, region_rings);
            lg::info("dev", "DevCompleteRegions rings={} completed={}", region_rings, n);
        }

        if (ImGui::Button("DevFinishAllButOne (around me)"))
        {
            const auto cam = camera::position();
            sdk::vector3 last{};
            const auto n = moon_world::dev_finish_all_but_one(world_inst, cam, last);
            lg::info("dev", "DevFinishAllButOne completed={} last=({:.1f},{:.1f},{:.1f})",
                n, last.x, last.y, last.z);
        }

        ImGui::SameLine();
        if (ImGui::Button("DevResetDust"))
        {
            const bool ok = moon_world::dev_reset_dust(world_inst);
            lg::info("dev", "DevResetDust ok={}", ok ? 1 : 0);
        }
    }
}
