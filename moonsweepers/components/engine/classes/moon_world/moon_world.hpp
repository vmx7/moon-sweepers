#pragma once

#include <cstdint>

#include "components/engine/il2cpp.hpp"
#include "components/engine/structures/vector/vector.hpp"

namespace moon_world
{
    void resolve() noexcept;

    [[nodiscard]] bool ready() noexcept;

    [[nodiscard]] bool gentle_ground(il2cpp::il2cpp_object* world_instance,
                                     const sdk::vector3&    direction,
                                     sdk::vector3&          out) noexcept;

    [[nodiscard]] bool is_landmark_raised(il2cpp::il2cpp_object* world_instance,
                                          std::int32_t          id) noexcept;

    [[nodiscard]] il2cpp::il2cpp_object* local_player() noexcept;

    bool crew_raise_landmark(il2cpp::il2cpp_object* world_instance,
                             il2cpp::il2cpp_object* player_string,
                             std::int32_t          index) noexcept;
}
