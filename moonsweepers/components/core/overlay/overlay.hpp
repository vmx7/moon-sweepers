#pragma once

struct IDXGISwapChain;

namespace core::overlay
{
    [[nodiscard]] bool init(IDXGISwapChain* swap) noexcept;
    void render(IDXGISwapChain* swap) noexcept;
    void shutdown() noexcept;
}
