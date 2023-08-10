#ifndef POINT_H_
#define POINT_H_

#include <cmath>
#include <ostream>

namespace mist
{

template <typename T> struct Point2 {
    T x = 0;
    T y = 0;

    auto operator==(const Point2<T> &other) const noexcept -> bool = default;

    auto operator+=(const Point2<T> &v) noexcept -> Point2<T>
    {
        x += v.x;
        y += v.y;
        return *this;
    }

    auto operator-=(const Point2<T> &v) noexcept -> Point2<T>
    {
        x -= v.x;
        y -= v.y;
        return *this;
    }

    auto operator*=(float v) noexcept -> Point2<T>
    {
        x *= v;
        y *= v;
        return *this;
    }

    auto operator/=(float v) noexcept -> Point2<T>
    {
        x /= v;
        y /= v;
        return *this;
    }

    inline auto operator<(const Point2<T> &b) const noexcept -> bool
    {
        return x < b.x || (x <= b.x && y < b.y);
    }

    inline auto operator>(const Point2<T> &b) const noexcept -> bool
    {
        return x > b.x || (x >= b.x && y > b.y);
    }

    [[nodiscard]] auto length() const noexcept -> float { return sqrtf(x * x + y * y); }
};

template <typename T>
[[nodiscard]] inline Point2<T> operator+(const Point2<T> &a, const Point2<T> &b)
{
    return {a.x + b.x, a.y + b.y};
};

template <typename T>
[[nodiscard]] inline Point2<T> operator-(const Point2<T> &a, const Point2<T> &b)
{
    return {a.x - b.x, a.y - b.y};
};

template <typename T> [[nodiscard]] inline Point2<T> operator*(const Point2<T> &a, const float v)
{
    return {a.x * v, a.y * v};
};

template <typename T> [[nodiscard]] inline Point2<T> operator/(const Point2<T> &a, const float v)
{
    return {a.x / v, a.y / v};
};

using Point2i = Point2<int>;
using Point2f = Point2<float>;
using Point2d = Point2<double>;
using Size = Point2<int>;

[[nodiscard]] inline auto round(const Point2f &p) -> Point2i
{
    return Point2i {static_cast<int>(roundf(p.x)), static_cast<int>(roundf(p.y))};
}

[[nodiscard]] inline auto point2f(const Point2i &p) -> Point2f
{
    return Point2f {static_cast<float>(p.x), static_cast<float>(p.y)};
}

std::ostream &operator<<(std::ostream &os, const Point2i &p);

} // namespace mist

#endif
