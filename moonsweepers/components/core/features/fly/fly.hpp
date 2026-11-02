#pragma once

namespace features::fly
{
    void tick() noexcept;
    void draw_menu() noexcept;

    inline bool  enabled = false;
    inline bool  noclip  = true;
    inline float speed   = 20.0f;
}
