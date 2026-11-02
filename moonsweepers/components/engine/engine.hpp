#pragma once

#include <cstdint>

#include "components/engine/il2cpp.hpp"
#include "utilities/utilities.hpp"

namespace engine
{
    [[nodiscard]] bool init() noexcept;
    void shutdown() noexcept;
    void tick() noexcept;

    [[nodiscard]] const mem::module_info& game_assembly() noexcept;
    [[nodiscard]] const mem::module_info& unity_player() noexcept;

    [[nodiscard]] il2cpp::il2cpp_object* local_walker() noexcept;
}
