#pragma once

#include "components/engine/structures/matrix/matrix.hpp"
#include "components/engine/structures/vector/vector.hpp"

namespace camera
{
    void resolve() noexcept;
    void refresh() noexcept;

    [[nodiscard]] bool ready() noexcept;

    [[nodiscard]] const sdk::matrix4x4& view_projection() noexcept;
    [[nodiscard]] const sdk::vector3&   position() noexcept;
    [[nodiscard]] sdk::vector2          viewport() noexcept;

    [[nodiscard]] bool world_to_screen(const sdk::vector3& world, sdk::vector2& out) noexcept;
}
