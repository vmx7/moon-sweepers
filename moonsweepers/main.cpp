#include <windows.h>

#include "components/core/core.hpp"

namespace
{
    DWORD WINAPI init_thread(LPVOID param) noexcept
    {
        core::init(static_cast<HINSTANCE>(param));
        return 0;
    }
}

BOOL WINAPI DllMain(HINSTANCE dll_handle, DWORD reason, LPVOID) noexcept
{
    if (reason == DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(dll_handle);
        const auto th = CreateThread(nullptr, 0, init_thread, dll_handle, 0, nullptr);
        if (th != nullptr)
        {
            CloseHandle(th);
        }
    }
    else if (reason == DLL_PROCESS_DETACH)
    {
        core::shutdown();
    }
    
    return TRUE;
}
