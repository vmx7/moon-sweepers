#pragma once

#include "imgui/imgui.h"

namespace features::flag_esp
{
    void render() noexcept;
    void draw_menu() noexcept;

    inline bool  enabled           = true;
    inline bool  show_label        = true;
    inline bool  show_distance     = true;
    inline bool  only_unraised     = true;
    inline float max_distance      = 50000.0f;
    inline float text_size         = 14.0f;

    inline ImGuiKey tp_bind        = ImGuiKey_T;
    inline bool     tp_bind_rebinding = false;

    inline ImVec4 color_flag      = { 1.00f, 0.95f, 0.20f, 1.00f };
    inline ImVec4 color_site      = { 0.30f, 0.85f, 1.00f, 1.00f };
    inline ImVec4 color_other     = { 1.00f, 1.00f, 1.00f, 1.00f };
}
