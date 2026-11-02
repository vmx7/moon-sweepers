#pragma once

#include "components/engine/il2cpp.hpp"

namespace moon_run_over
{
    void resolve() noexcept;

    [[nodiscard]] bool ready() noexcept;

    [[nodiscard]] il2cpp::il2cpp_object* local_walker() noexcept;
}
