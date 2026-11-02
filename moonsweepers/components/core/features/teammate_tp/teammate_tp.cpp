#include "teammate_tp.hpp"

#include <cstdio>
#include <vector>

#include "components/engine/classes/moon_remote_player/moon_remote_player.hpp"
#include "components/engine/classes/moon_walker/moon_walker.hpp"
#include "components/engine/engine.hpp"
#include "utilities/log/log.hpp"

#include "imgui/imgui.h"

namespace features::teammate_tp
{
    namespace
    {
        std::vector<moon_remote_player::snapshot> cached_;
    }

    void draw_menu() noexcept
    {
        if (!ImGui::CollapsingHeader("Teleport to teammate"))
        {
            return;
        }

        if (!moon_remote_player::ready())
        {
            ImGui::TextDisabled("MoonRemotePlayer path not resolved");

            return;
        }

        if (ImGui::Button("Refresh teammates"))
        {
            cached_ = moon_remote_player::find_all();
            lg::info("tp", "found {} teammates", cached_.size());
        }

        if (cached_.empty())
        {
            ImGui::SameLine();
            ImGui::TextDisabled("click to scan");

            return;
        }

        for (std::size_t i = 0; i < cached_.size(); ++i)
        {
            const auto& t = cached_[i];
            char btn[96]{};
            std::snprintf(btn, sizeof(btn), "TP to %s ##%zu",
                t.name.empty() ? "<unnamed>" : t.name.c_str(), i);
            if (ImGui::Button(btn))
            {
                auto* w = engine::local_walker();
                if (w != nullptr)
                {
                    moon_walker::teleport(w, t.position.x, t.position.y, t.position.z);
                    lg::info("tp", "teleport to '{}' ({:.1f},{:.1f},{:.1f})",
                        t.name, t.position.x, t.position.y, t.position.z);
                }
            }
            ImGui::SameLine();
            ImGui::TextDisabled("(%.0f,%.0f,%.0f)",
                t.position.x, t.position.y, t.position.z);
        }
    }
}
