#pragma once

#include "components/engine/il2cpp.hpp"

namespace moon_brush
{
    void resolve() noexcept;

    [[nodiscard]] bool ready() noexcept;

    [[nodiscard]] il2cpp::il2cpp_field_info* field_cleaning_width() noexcept;
    [[nodiscard]] il2cpp::il2cpp_field_info* field_cleaning_depth() noexcept;
    [[nodiscard]] il2cpp::il2cpp_field_info* field_cleaning_edge_width() noexcept;
    [[nodiscard]] il2cpp::il2cpp_field_info* field_removal_rate() noexcept;
    [[nodiscard]] il2cpp::il2cpp_field_info* field_reach() noexcept;
    [[nodiscard]] il2cpp::il2cpp_field_info* field_stroke_interval() noexcept;
    [[nodiscard]] il2cpp::il2cpp_field_info* field_clear_in_one_pass() noexcept;
}
