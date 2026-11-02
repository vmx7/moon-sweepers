#pragma once

#include "components/engine/il2cpp.hpp"

namespace moon_player_visual
{
    void resolve() noexcept;

    [[nodiscard]] bool ready() noexcept;

    [[nodiscard]] il2cpp::il2cpp_object* brush(il2cpp::il2cpp_object* visual) noexcept;
}
