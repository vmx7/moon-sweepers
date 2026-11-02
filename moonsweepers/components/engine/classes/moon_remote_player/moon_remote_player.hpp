#pragma once

#include <string>
#include <vector>

#include "components/engine/il2cpp.hpp"
#include "components/engine/structures/vector/vector.hpp"

namespace moon_remote_player
{
    struct snapshot
    {
        il2cpp::il2cpp_object* object;
        sdk::vector3           position;
        std::string            name;
    };

    void resolve() noexcept;

    [[nodiscard]] bool ready() noexcept;

    [[nodiscard]] std::vector<snapshot> find_all() noexcept;
}
