#pragma once

#include <cstdint>
#include <vector>

namespace features::dust_locator
{
    struct hit
    {
        std::uintptr_t addr;
        double         value;
    };

    void scan() noexcept;
    void draw_menu() noexcept;

    [[nodiscard]] const std::vector<hit>& results() noexcept;
}
