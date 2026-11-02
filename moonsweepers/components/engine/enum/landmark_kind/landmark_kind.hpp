#pragma once

#include <cstdint>

namespace sdk
{
    enum class landmark_kind : std::uint8_t
    {
        flag    = 0,
        site    = 1,
        unknown = 0xFF,
    };

    [[nodiscard]] constexpr const char* to_string(landmark_kind k) noexcept
    {
        switch (k)
        {
            case landmark_kind::flag: return "Flag";
            case landmark_kind::site: return "Site";
            default:                  return "Unknown";
        }
    }
}
