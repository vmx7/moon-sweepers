#pragma once

#include "components/engine/il2cpp.hpp"

namespace moon_dust_hand
{
    void resolve() noexcept;

    [[nodiscard]] bool ready() noexcept;

    [[nodiscard]] il2cpp::il2cpp_field_info* field_reach() noexcept;
    [[nodiscard]] il2cpp::il2cpp_field_info* field_inventory() noexcept;
}
