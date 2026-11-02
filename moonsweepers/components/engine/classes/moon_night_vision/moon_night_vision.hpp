#pragma once

namespace moon_night_vision
{
    void resolve() noexcept;

    [[nodiscard]] bool ready() noexcept;

    void set_amount(float amount) noexcept;
    [[nodiscard]] float amount() noexcept;
}
