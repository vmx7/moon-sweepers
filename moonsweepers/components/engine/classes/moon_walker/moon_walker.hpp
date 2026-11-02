#pragma once

#include "components/engine/il2cpp.hpp"

namespace moon_walker
{
    void resolve() noexcept;

    [[nodiscard]] bool ready() noexcept;

    [[nodiscard]] il2cpp::il2cpp_method_info* method_set_flying() noexcept;
    [[nodiscard]] il2cpp::il2cpp_method_info* method_set_night_vision() noexcept;
    [[nodiscard]] il2cpp::il2cpp_method_info* method_set_gravity_boots() noexcept;
    [[nodiscard]] il2cpp::il2cpp_method_info* method_unstick() noexcept;

    [[nodiscard]] il2cpp::il2cpp_method_info* set_fly_speed() noexcept;
    [[nodiscard]] il2cpp::il2cpp_method_info* set_dev_speed_multiplier() noexcept;
    [[nodiscard]] il2cpp::il2cpp_method_info* set_equipment_speed_multiplier() noexcept;

    [[nodiscard]] il2cpp::il2cpp_object* view_camera(il2cpp::il2cpp_object* walker) noexcept;
    [[nodiscard]] il2cpp::il2cpp_object* world(il2cpp::il2cpp_object* walker) noexcept;

    void teleport(il2cpp::il2cpp_object* walker, float x, float y, float z) noexcept;

    void set_night_vision_field(il2cpp::il2cpp_object* walker, bool on) noexcept;
    void set_gravity_boots_field(il2cpp::il2cpp_object* walker, bool on) noexcept;
}
