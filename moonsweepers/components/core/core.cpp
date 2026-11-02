#include "core.hpp"

#include <atomic>

#include <windows.h>

#include "components/core/hook/hook.hpp"
#include "components/engine/engine.hpp"
#include "utilities/log/log.hpp"
#include "utilities/utilities.hpp"

namespace core
{
    namespace
    {
        std::atomic<bool> initialized{ false };
        std::atomic<bool> unload_requested{ false };
        HINSTANCE dll_handle_ = nullptr;

        DWORD WINAPI unload_thread(LPVOID param) noexcept
        {
            try
            {
                Sleep(120);
                hook::remove();
                Sleep(120);
                FreeLibraryAndExitThread(static_cast<HMODULE>(param), 0);
            }
            catch (...)
            {
                FreeLibraryAndExitThread(static_cast<HMODULE>(param), 1);
            }
            return 0;
        }
    }

    void init(HINSTANCE dll_handle) noexcept
    {
        if (initialized.exchange(true, std::memory_order_acq_rel))
        {
            return;
        }
        dll_handle_ = dll_handle;

        try
        {
            lg::info("core", "moonsweepers internal loaded dll=0x{:X}",
                reinterpret_cast<std::uintptr_t>(dll_handle));

            if (!mem::wait_module(L"GameAssembly.dll"))
            {
                lg::error("core", "GameAssembly.dll never appeared");
                return;
            }
            if (!mem::wait_module(L"UnityPlayer.dll"))
            {
                lg::error("core", "UnityPlayer.dll never appeared");
                return;
            }

            if (!engine::init())
            {
                lg::error("core", "engine init failed");
                return;
            }

            if (!hook::install())
            {
                lg::warn("core", "hook install failed overlay disabled");
            }

            lg::info("core", "init complete");
        }
        catch (...)
        {
            lg::error("core", "init crashed with c++ exception");
        }
    }

    void shutdown() noexcept
    {
        if (!initialized.load(std::memory_order_acquire))
        {
            return;
        }
        lg::info("core", "shutdown");
        hook::remove();
        engine::shutdown();
        initialized.store(false, std::memory_order_release);
    }

    void request_unload() noexcept
    {
        if (unload_requested.exchange(true, std::memory_order_acq_rel))
        {
            return;
        }
        lg::info("core", "unload requested");
        if (dll_handle_ == nullptr)
        {
            return;
        }
        const auto th = CreateThread(nullptr, 0, unload_thread, dll_handle_, 0, nullptr);
        if (th != nullptr)
        {
            CloseHandle(th);
        }
    }
}
