#pragma once

#include <cstddef>
#include <cstdint>
#include <string_view>

namespace mem
{
    struct module_info
    {
        std::uintptr_t base;
        std::size_t    size;

        [[nodiscard]] bool valid() const noexcept
        {
            return base != 0 && size != 0;
        }
    };

    [[nodiscard]] module_info module_of(std::wstring_view name) noexcept;

    [[nodiscard]] void* get_export(const module_info& mod, const char* name) noexcept;

    [[nodiscard]] bool wait_module(std::wstring_view name, std::uint32_t timeout_ms = 60000) noexcept;
}
