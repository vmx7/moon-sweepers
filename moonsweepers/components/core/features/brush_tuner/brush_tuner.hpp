#pragma once

namespace features::brush_tuner
{
    void tick() noexcept;
    void draw_menu() noexcept;

    inline bool  enabled           = false;
    inline float cleaning_width    = 15.0f;
    inline float cleaning_depth    = 15.0f;
    inline float cleaning_edge     = 2.0f;
    inline float reach             = 25.0f;
    inline float removal_rate      = 300.0f;
    inline float stroke_interval   = 0.05f;
    inline bool  clear_in_one_pass = false;
}
