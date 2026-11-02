#pragma once

#include <windows.h>

namespace core
{
    void init(HINSTANCE dll_handle) noexcept;
    void shutdown() noexcept;
    void request_unload() noexcept;
}
