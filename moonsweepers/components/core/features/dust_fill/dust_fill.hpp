#pragma once

#include <cstdint>

namespace features::dust_fill
{
    void tick() noexcept;
    void draw_menu() noexcept;

    inline bool           enabled    = false;
    inline float          target_kg  = 500.0f;
    inline std::uintptr_t slot_addr  = 0;
}
