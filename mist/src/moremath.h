#ifndef MOREMATH_H_
#define MOREMATH_H_

#include <cmath>

namespace mist
{

template <typename T> T lerp(T t, T a, T b)
{
    return a + t * (b - a);
}

template <typename T> int roundi(T x)
{
    return static_cast<int>(std::round(x));
}

template <typename T> auto clamp(const T &value, const T &lowerBound, const T &upperBound) -> T
{
    return std::min(upperBound, std::max(lowerBound, value));
}

template <typename T=double>
class LinearTransform
{
public:
    LinearTransform(T x0_, T x1, T y0_, T y1)
        : x0(x0_), y0(y0_), a((y1 - y0) / (x1 - x0))
    {
    }

    T operator()(T x) const { return y0 + (x - x0) * a; }

private:
    T x0, y0;
    T a;
};

} // namespace mist

#endif
