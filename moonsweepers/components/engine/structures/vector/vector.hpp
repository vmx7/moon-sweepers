#pragma once

#include <cmath>

namespace sdk
{
    struct vector2
    {
        float x{};
        float y{};
    };

    struct vector3
    {
        float x{};
        float y{};
        float z{};

        [[nodiscard]] constexpr vector3 operator+(const vector3& o) const noexcept
        {
            return { x + o.x, y + o.y, z + o.z };
        }

        [[nodiscard]] constexpr vector3 operator-(const vector3& o) const noexcept
        {
            return { x - o.x, y - o.y, z - o.z };
        }

        [[nodiscard]] constexpr vector3 operator*(float s) const noexcept
        {
            return { x * s, y * s, z * s };
        }

        [[nodiscard]] constexpr float dot(const vector3& o) const noexcept
        {
            return x * o.x + y * o.y + z * o.z;
        }

        [[nodiscard]] float length() const noexcept
        {
            return std::sqrt(dot(*this));
        }
    };

    struct vector4
    {
        float x{};
        float y{};
        float z{};
        float w{};
    };
}
