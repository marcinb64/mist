#ifndef POINT_H_
#define POINT_H_

#include <cmath>
#include <ostream>

namespace mist
{

template <class T> struct Point2 {
    T x = 0;
    T y = 0;

    auto operator==(const Point2<T> &other) const noexcept -> bool = default;

    template <class U> auto operator+=(const Point2<U> &v) noexcept -> Point2<T>
    {
        x += v.x;
        y += v.y;
        return *this;
    }

    template <class U> auto operator-=(const Point2<U> &v) noexcept -> Point2<T>
    {
        x -= v.x;
        y -= v.y;
        return *this;
    }

    template <class U> auto operator*=(U v) noexcept -> Point2<T>
    {
        x *= v;
        y *= v;
        return *this;
    }

    template <class U> auto operator/=(U v) noexcept -> Point2<T>
    {
        x /= v;
        y /= v;
        return *this;
    }

    template <class U> 
    inline auto operator<(const Point2<U> &b) const noexcept -> bool
    {
        return x < b.x || (x <= b.x && y < b.y);
    }

    template <class U> 
    inline auto operator>(const Point2<U> &b) const noexcept -> bool
    {
        return x > b.x || (x >= b.x && y > b.y);
    }

    [[nodiscard]] auto length() const noexcept -> double { return sqrt(x * x + y * y); }

    template <class U> explicit operator Point2<U>() const noexcept
    {
        return {static_cast<U>(x), static_cast<U>(y)};
    }

    template <class U> Point2<U> as() const noexcept
    {
        return {static_cast<U>(x), static_cast<U>(y)};
    }
};

template <class T, class U>
[[nodiscard]] inline Point2<T> operator+(const Point2<T> &a, const Point2<U> &b)
{
    return {a.x + b.x, a.y + b.y};
};

template <class T, class U>
[[nodiscard]] inline Point2<T> operator-(const Point2<T> &a, const Point2<U> &b)
{
    return {a.x - b.x, a.y - b.y};
};

template <class T, class U> [[nodiscard]] inline Point2<T> operator*(const Point2<T> &a, const U v)
{
    return {a.x * v, a.y * v};
};

template <class T, class U> [[nodiscard]] inline Point2<T> operator/(const Point2<T> &a, const U v)
{
    return {a.x / v, a.y / v};
};

using Point2i = Point2<int>;
using Point2f = Point2<float>;
using Point2d = Point2<double>;

[[nodiscard]] inline auto round(const Point2f &p) -> Point2i
{
    return Point2i {static_cast<int>(roundf(p.x)), static_cast<int>(roundf(p.y))};
}

[[nodiscard]] inline auto round(const Point2d &p) -> Point2i
{
    return mist::Point2i {static_cast<int>(::round(p.x)), static_cast<int>(::round(p.y))};
}

std::ostream &operator<<(std::ostream &os, const Point2i &p);

} // namespace mist

#endif
