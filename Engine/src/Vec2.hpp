#pragma once

#include "SFML/System/Vector2.hpp"
#include "imgui.h"

#include <cmath>
#include <cstdint>
#include <utility>

template <typename T> class Vec2
{
public:
    constexpr Vec2() = default;
    constexpr Vec2(const T& _x, const T& _y) : x{_x}, y{_y} {}

    T x{};
    T y{};

    // Conversions
    constexpr explicit Vec2(const sf::Vector2<T>& vec) : x{vec.x}, y{vec.y} {}

    template <typename U>
    constexpr explicit Vec2(const sf::Vector2<U>& rhs)
        : x{static_cast<U>(rhs.x)}, y{static_cast<U>(rhs.y)}
    {
    }

    template <typename U> constexpr explicit operator Vec2<U>() const
    {
        return Vec2<U>{static_cast<U>(x), static_cast<U>(y)};
    }

    constexpr operator sf::Vector2<T>() const { return {x, y}; }
    constexpr operator ImVec2() const { return {x, y}; }
    constexpr operator std::pair<T, T>() const { return {x, y}; }

    // Basic arimetic operators
    constexpr Vec2 operator+(const Vec2& rhs) const
    {
        return {x + rhs.x, y + rhs.y};
    }
    constexpr Vec2 operator-(const Vec2& rhs) const
    {
        return {x - rhs.x, y - rhs.y};
    }
    constexpr Vec2 operator*(const Vec2& rhs) const
    {
        return {x * rhs.x, y * rhs.y};
    }
    constexpr Vec2 operator/(const Vec2& rhs) const
    {
        return {x / rhs.x, y / rhs.y};
    }

    // Equality
    constexpr bool operator==(const Vec2& rhs) const
    {
        return x == rhs.x && y == rhs.y;
    }
    constexpr bool operator!=(const Vec2& rhs) const
    {
        return x != rhs.x && y != rhs.y;
    }

    // Compound assignment
    constexpr Vec2& operator+=(const Vec2& rhs)
    {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }
    constexpr Vec2& operator-=(const Vec2& rhs)
    {
        x -= rhs.x;
        y -= rhs.y;
        return *this;
    }
    constexpr Vec2& operator*=(const Vec2& rhs)
    {
        x *= rhs.x;
        y *= rhs.y;
        return *this;
    }
    constexpr Vec2& operator/=(const Vec2& rhs)
    {
        x /= rhs.x;
        y /= rhs.y;
        return *this;
    }

    // Scalar operations
    constexpr Vec2 operator*(const T& scalar) const
    {
        return {x * scalar, y * scalar};
    }
    constexpr Vec2& operator*=(const T& scalar)
    {
        x *= scalar;
        y *= scalar;
        return *this;
    }
    constexpr Vec2 operator/(const T& scalar) const
    {
        return {x / scalar, y / scalar};
    }
    constexpr Vec2& operator/=(const T& scalar)
    {
        x /= scalar;
        y /= scalar;
        return *this;
    }
    constexpr Vec2& operator+=(const T& scalar)
    {
        x += scalar;
        y += scalar;
        return *this;
    }
    constexpr Vec2& operator-=(const T& scalar)
    {
        x -= scalar;
        y -= scalar;
        return *this;
    }

    // Math
    constexpr float length() const
    {
        static_assert(std::is_floating_point_v<T>,
                      "Length is only supported for floating point types");

        return std::sqrt(std::pow(x, 2) + std::pow(y, 2));
    }

    constexpr Vec2 normalised() const
    {
        static_assert(
            std::is_floating_point_v<T>,
            "Normalisation is only supported for floating point types");

        auto len = length();
        if(len > 0)
        {
            return {x / len, y / len};
        }
        else
        {
            return {};
        }
    }
};

template <typename T> Vec2(T, T) -> Vec2<T>;

using Vec2f = Vec2<float>;
using Vec2u = Vec2<uint32_t>;
using Vec2i = Vec2<int>;
