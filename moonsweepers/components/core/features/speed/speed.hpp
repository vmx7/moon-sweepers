#pragma once

namespace features::speed
{
    void tick() noexcept;
    void draw_menu() noexcept;

    inline bool  enabled           = false;
    inline float move_multiplier   = 2.5f;
    inline float equipment_speed   = 2.5f;
}
