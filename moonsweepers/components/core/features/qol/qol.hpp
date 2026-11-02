#pragma once

namespace features::qol
{
    void tick() noexcept;
    void draw_menu() noexcept;

    inline bool  tune_enabled   = false;
    inline float climb_speed    = 5.0f;
    inline float earth_gravity  = 9.81f;
}
