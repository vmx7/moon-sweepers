#include "hook.hpp"

#include <cstddef>

#include <d3d11.h>
#include <windows.h>

#include "MinHook.h"
#include "kiero/kiero.h"

#include "components/core/overlay/overlay.hpp"
#include "utilities/log/log.hpp"

namespace core::hook
{
    namespace
    {
        constexpr std::size_t idx_present = 8;

        using fn_present = HRESULT (STDMETHODCALLTYPE*)(IDXGISwapChain*, UINT, UINT);
        fn_present original_present_ = nullptr;
        bool       installed_        = false;

        HRESULT STDMETHODCALLTYPE present_hook(IDXGISwapChain* swap, UINT sync_interval, UINT flags)
        {
            static bool overlay_ready = false;
            static int  entries = 0;
            if (entries < 3)
            {
                ++entries;
                lg::info("hook", "present_hook called swap=0x{:X} #{}",
                    reinterpret_cast<std::uintptr_t>(swap), entries);
            }
            if (!overlay_ready)
            {
                overlay_ready = overlay::init(swap);
                if (!overlay_ready && entries <= 3)
                {
                    lg::warn("hook", "overlay::init returned false");
                }
            }
            if (overlay_ready)
            {
                overlay::render(swap);
            }

            return original_present_(swap, sync_interval, flags);
        }
    }

    bool install() noexcept
    {
        if (installed_)
        {
            return true;
        }
        if (MH_Initialize() != MH_OK)
        {
            lg::error("hook", "MH_Initialize failed");

            return false;
        }

        const auto s = kiero::init(kiero::RenderType::D3D11);
        if (s != kiero::Status::Success)
        {
            lg::error("hook", "kiero D3D11 init failed status={}", static_cast<int>(s));
            MH_Uninitialize();

            return false;
        }

        if (kiero::bind(idx_present,
                        reinterpret_cast<void**>(&original_present_),
                        reinterpret_cast<void*>(present_hook)) != kiero::Status::Success)
        {
            lg::error("hook", "kiero bind Present vtable[{}] failed", idx_present);
            kiero::shutdown();
            MH_Uninitialize();

            return false;
        }

        installed_ = true;
        lg::info("hook", "D3D11 Present hook installed");

        return true;
    }

    void remove() noexcept
    {
        if (!installed_)
        {
            return;
        }
        kiero::shutdown();
        MH_Uninitialize();
        overlay::shutdown();
        installed_ = false;
    }
}
