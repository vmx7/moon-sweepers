#pragma once

namespace features::night_vision
{
    void tick() noexcept;
    void draw_menu() noexcept;

    inline bool enabled = false;
}
