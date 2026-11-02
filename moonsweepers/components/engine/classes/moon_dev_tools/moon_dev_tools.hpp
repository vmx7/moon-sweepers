#pragma once

namespace moon_dev_tools
{
    void resolve() noexcept;

    [[nodiscard]] bool ready() noexcept;

    [[nodiscard]] bool noclip() noexcept;
    void set_noclip(bool v) noexcept;

    [[nodiscard]] bool enabled() noexcept;

    bool patch_enabled_true() noexcept;
}
