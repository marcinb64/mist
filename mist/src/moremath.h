#ifndef MOREMATH_H_
#define MOREMATH_H_

#include "Point.h"
#include <cmath>

namespace mist
{

template <typename T> constexpr T lerp(T t, T a, T b)
{
    return a + t * (b - a);
}

template <typename T> constexpr int roundi(T x)
{
    return static_cast<int>(std::round(x));
}

template <typename T>
constexpr auto clamp(const T &value, const T &lowerBound, const T &upperBound) -> T
{
    return std::min(upperBound, std::max(lowerBound, value));
}

template <typename T = double> class LinearTransform
{
public:
    static constexpr LinearTransform identity() { return LinearTransform {}; };

    constexpr LinearTransform() = default;

    constexpr LinearTransform(T x0_, T x1, T y0_, T y1) : x0(x0_), y0(y0_), a((y1 - y0) / (x1 - x0))
    {
    }

    constexpr T operator()(T x) const { return y0 + (x - x0) * a; }

private:
    T x0 {};
    T y0 {};
    T a {1};
};

template <typename T = double> struct LinearTransform2 {

    static constexpr LinearTransform2 identity() { return LinearTransform2 {}; };

    constexpr LinearTransform2() = default;

    constexpr LinearTransform2(const Point2<T> a0, const Point2<T> a1, const Point2<T> b0,
                               const Point2<T> b1)
        : xTransform {a0.x, a1.x, b0.x, b1.x}, yTransform {a0.y, a1.y, b0.y, b1.y}
    {
    }

    constexpr Point2<T> operator()(const Point2<T> p) { return {xTransform(p.x), yTransform(p.y)}; }

private:
    mist::LinearTransform<T> xTransform {0, 1, 0, 1};
    mist::LinearTransform<T> yTransform {0, 1, 0, 1};
};

} // namespace mist

#endif
