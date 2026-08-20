#pragma once
#include <SFML/System/Vector2.hpp>
#include <cmath>

template <typename T>
class Vec2
{
public:
    T x = 0;
    T y = 0;

    Vec2() = default;

    Vec2(T xin, T yin)
        : x(xin),
          y(yin)
    {
    }

    Vec2(const sf::Vector2<T>& vec)
        : x(vec.x),
          y(vec.y)
    {
    }

    operator sf::Vector2<T>() const
    {
        return sf::Vector2<T>(x, y);
    }

    bool operator==(const Vec2& rhs) const
    {
        return rhs.x == x && rhs.y == y;
    }

    bool operator!=(const Vec2& rhs) const
    {
        return rhs.x != x || rhs.y != y;
    }

    Vec2 operator+(const Vec2& rhs) const
    {
        return Vec2(x + rhs.x, y + rhs.y);
    }

    Vec2 operator-(const Vec2& rhs) const
    {
        return Vec2(x - rhs.x, y - rhs.y);
    }

    Vec2 operator/(const T val) const
    {
        return Vec2(x / val, y / val);
    }

    Vec2 operator*(const T val) const
    {
        return Vec2(x * val, y * val);
    }

    Vec2 operator-() const
    {
        return Vec2(-x, -y);
    }

    void operator+=(const Vec2& rhs)
    {
        x += rhs.x;
        y += rhs.y;
    }

    void operator-=(const Vec2& rhs)
    {
        x -= rhs.x;
        y -= rhs.y;
    }

    void operator*=(const T val)
    {
        x *= val;
        y *= val;
    }

    void operator/=(const T val)
    {
        x /= val;
        y /= val;
    }

    float dist(const Vec2& rhs) const
    {
        return std::sqrt((rhs.x - x) * (rhs.x - x) + (rhs.y - y) * (rhs.y - y));
    }

    float length() const
    {
        return std::sqrt(x * x + y * y);
    }
};