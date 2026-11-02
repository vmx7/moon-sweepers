#include "overlay.hpp"

#include <atomic>
#include <cstdint>

#include <d3d11.h>
#include <windows.h>

#include "imgui/backends/imgui_impl_dx11.h"
#include "imgui/backends/imgui_impl_win32.h"
#include "imgui/imgui.h"

#include "components/core/core.hpp"
#include "components/core/features/flag_esp/flag_esp.hpp"
#include "components/core/menu/menu.hpp"
#include "components/engine/engine.hpp"
#include "utilities/log/log.hpp"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND, UINT, WPARAM, LPARAM);

namespace core::overlay
{
    namespace
    {
        ID3D11Device*           device_          = nullptr;
        ID3D11DeviceContext*    context_         = nullptr;
        ID3D11RenderTargetView* rtv_             = nullptr;
        HWND                    hwnd_            = nullptr;
        WNDPROC                 original_wndproc_= nullptr;
        std::atomic<bool>       initialized_{ false };
        std::atomic<bool>       menu_open_{ true };

        LRESULT CALLBACK wndproc_hook(HWND hwnd, UINT msg, WPARAM w, LPARAM l)
        {
            if (initialized_.load(std::memory_order_acquire))
            {
                ImGui_ImplWin32_WndProcHandler(hwnd, msg, w, l);
                if (menu_open_.load(std::memory_order_acquire))
                {
                    const auto& io = ImGui::GetIO();
                    if (io.WantCaptureMouse && msg >= WM_MOUSEFIRST && msg <= WM_MOUSELAST)
                    {
                        return 1;
                    }
                    if (io.WantCaptureKeyboard && msg >= WM_KEYFIRST && msg <= WM_KEYLAST)
                    {
                        return 1;
                    }
                }
            }

            return CallWindowProcW(original_wndproc_, hwnd, msg, w, l);
        }
    }

    bool init(IDXGISwapChain* swap) noexcept
    {
        if (initialized_.load(std::memory_order_acquire))
        {
            return true;
        }
        if (swap == nullptr)
        {
            return false;
        }

        DXGI_SWAP_CHAIN_DESC desc{};
        if (FAILED(swap->GetDesc(&desc)))
        {
            lg::error("overlay", "GetDesc failed");

            return false;
        }
        hwnd_ = desc.OutputWindow;

        if (FAILED(swap->GetDevice(__uuidof(ID3D11Device), reinterpret_cast<void**>(&device_))))
        {
            lg::error("overlay", "GetDevice failed");

            return false;
        }
        device_->GetImmediateContext(&context_);

        ID3D11Texture2D* back_buffer = nullptr;
        if (FAILED(swap->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&back_buffer))))
        {
            lg::error("overlay", "GetBuffer failed");

            return false;
        }
        device_->CreateRenderTargetView(back_buffer, nullptr, &rtv_);
        back_buffer->Release();

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        auto& io = ImGui::GetIO();
        io.IniFilename = nullptr;
        io.LogFilename = nullptr;
        ImGui::StyleColorsDark();

        if (!ImGui_ImplWin32_Init(hwnd_))
        {
            lg::error("overlay", "ImGui_ImplWin32_Init failed");

            return false;
        }
        if (!ImGui_ImplDX11_Init(device_, context_))
        {
            lg::error("overlay", "ImGui_ImplDX11_Init failed");

            return false;
        }

        original_wndproc_ = reinterpret_cast<WNDPROC>(
            SetWindowLongPtrW(hwnd_, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(wndproc_hook)));

        initialized_.store(true, std::memory_order_release);
        lg::info("overlay", "imgui dx11+win32 up hwnd=0x{:X}",
            reinterpret_cast<std::uintptr_t>(hwnd_));

        return true;
    }

    void render(IDXGISwapChain*) noexcept
    {
        if (!initialized_.load(std::memory_order_acquire))
        {
            return;
        }

        engine::tick();

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        if (ImGui::IsKeyPressed(ImGuiKey_Insert, false))
        {
            menu_open_.store(!menu_open_.load(std::memory_order_acquire),
                             std::memory_order_release);
        }
        if (ImGui::IsKeyPressed(ImGuiKey_End, false))
        {
            core::request_unload();
        }

        features::flag_esp::render();

        if (menu_open_.load(std::memory_order_acquire))
        {
            menu::render();
        }

        ImGui::EndFrame();
        ImGui::Render();
        context_->OMSetRenderTargets(1, &rtv_, nullptr);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    }

    void shutdown() noexcept
    {
        if (!initialized_.load(std::memory_order_acquire))
        {
            return;
        }
        initialized_.store(false, std::memory_order_release);

        if (hwnd_ != nullptr && original_wndproc_ != nullptr)
        {
            SetWindowLongPtrW(hwnd_, GWLP_WNDPROC,
                              reinterpret_cast<LONG_PTR>(original_wndproc_));
        }

        ImGui_ImplDX11_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();

        if (rtv_ != nullptr)     { rtv_->Release();     rtv_     = nullptr; }
        if (context_ != nullptr) { context_->Release(); context_ = nullptr; }
        if (device_ != nullptr)  { device_->Release();  device_  = nullptr; }
    }
}
