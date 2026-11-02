#pragma once

#include "components/engine/il2cpp.hpp"

namespace moon_crew_session
{
    void resolve() noexcept;

    [[nodiscard]] bool ready() noexcept;

    [[nodiscard]] il2cpp::il2cpp_object* instance() noexcept;
    [[nodiscard]] il2cpp::il2cpp_object* local_visual() noexcept;
}
