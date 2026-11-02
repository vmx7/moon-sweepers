#pragma once

#include <cstdint>
#include <vector>

#include "components/engine/enum/landmark_kind/landmark_kind.hpp"
#include "components/engine/il2cpp.hpp"
#include "components/engine/structures/vector/vector.hpp"

namespace moon_landmark
{
    struct marker
    {
        sdk::vector3       world;
        sdk::landmark_kind kind;
        std::uintptr_t     handle;
        std::int32_t       id;
        bool               raised;
        char               name[64];
    };

    void resolve() noexcept;

    [[nodiscard]] bool ready() noexcept;

    [[nodiscard]] std::vector<marker> snapshot() noexcept;
}
