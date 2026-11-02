#pragma once

namespace moon_steam_lobby
{
    void resolve() noexcept;

    [[nodiscard]] bool ready() noexcept;

    [[nodiscard]] bool is_host() noexcept;
    void set_is_host(bool v) noexcept;
}
