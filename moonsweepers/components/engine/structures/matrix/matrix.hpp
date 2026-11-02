#pragma once

#include "components/engine/structures/vector/vector.hpp"

namespace sdk
{
    struct matrix4x4
    {
        float m[4][4]{};

        [[nodiscard]] constexpr float at(int row, int col) const noexcept
        {
            return m[row][col];
        }
    };
}
