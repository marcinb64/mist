#ifndef MATRIX_H_
#define MATRIX_H_

#include "Point.h"

#include <vector>

namespace mist
{

template <typename T> class Matrix
{
public:
    Matrix(int xSize_, int ySize_)
        : xSize(xSize_), ySize(ySize_), data(static_cast<size_t>(xSize_ * ySize_))
    {
    }
    Matrix(const Size &size)
        : xSize(size.x), ySize(size.y), data(static_cast<size_t>(size.x * size.y))
    {
    }

    [[nodiscard]] auto at(int x, int y) -> T & { return data.at(index(x, y)); }
    [[nodiscard]] auto at(int x, int y) const -> const T & { return data.at(index(x, y)); }
    [[nodiscard]] auto at(const Point2i &p) -> T & { return data.at(index(p)); }
    [[nodiscard]] auto at(const Point2i &p) const -> const T & { return data.at(index(p)); }

    [[nodiscard]] auto getXSize() const noexcept -> int { return xSize; }
    [[nodiscard]] auto getYSize() const noexcept -> int { return ySize; }
    [[nodiscard]] auto getSize() const noexcept -> Size { return Size {xSize, ySize}; }

    [[nodiscard]] auto contains(const Point2i &p) const -> bool
    {
        return p.x >= 0 && p.x < xSize && p.y >= 0 && p.y < ySize;
    }

    auto begin() { return data.begin(); }
    auto end() { return data.end(); }

    template <typename F> auto foreachKey(F func) const -> const Matrix &
    {
        int i = 0;
        for (auto y = 0; y < ySize; ++y) {
            for (auto x = 0; x < xSize; ++x) {
                func(Point2i {x, y});
            }
        }

        return *this;
    }

    template <typename F> auto foreachValue(F func) -> Matrix &
    {
        for (auto &i : data)
            func(i);

        return *this;
    }

    template <typename F> auto foreachValue(F func) const -> const Matrix &
    {
        for (const auto &i : data)
            func(i);

        return *this;
    }

    template <typename F> auto foreachKeyValue(F func) -> Matrix &
    {
        int i = 0;
        for (auto y = 0; y < ySize; ++y) {
            for (auto x = 0; x < xSize; ++x) {
                func(Point2i {x, y}, data[i++]);
            }
        }

        return *this;
    }

    template <typename F> auto foreachKeyValue(F func) const -> const Matrix &
    {
        int i = 0;
        for (auto y = 0; y < ySize; ++y) {
            for (auto x = 0; x < xSize; ++x) {
                func(Point2i {x, y}, data[i++]);
            }
        }

        return *this;
    }

    template <typename F> auto transform(F func) -> Matrix &
    {
        for (auto &i : data)
            i = func(i);

        return *this;
    }

    auto fill(const T &value) -> Matrix &
    {
        for (auto &i : data)
            i = value;

        return *this;
    }

    template <class G> auto generate(G generator) -> Matrix &
    {
        int i = 0;
        for (auto y = 0; y < ySize; ++y) {
            for (auto x = 0; x < xSize; ++x) {
                data[i++] = generator(Point2i {x, y});
            }
        }

        return *this;
    }

private:
    int            xSize;
    int            ySize;
    std::vector<T> data;

    auto index(int x, int y) const -> size_t { return static_cast<size_t>(y * xSize + x); }
    auto index(const Point2i &p) const -> size_t { return static_cast<size_t>(p.y * xSize + p.x); }
};

template <typename T> auto min(const Matrix<T> &m) -> T
{
    T          ret = m.at({0, 0});
    m.foreachValue([&](const T &v) {
        ret = std::min(ret, v);
    });
    return ret;
}

template <typename T> auto max(const Matrix<T> &m) -> T
{
    T          ret = m.at({0, 0});
    m.foreachValue([&](const T &v) {
        ret = std::max(ret, v);
    });
    return ret;
}

template <typename A, typename B, class F>
auto transformMatrix(const Matrix<A> &src, F func) -> Matrix<B>
{
    Matrix<B> out(src.getSize());
    for (auto y = 0; y < src.getYSize(); ++y) {
        for (auto x = 0; x < src.getXSize(); ++x) {
            out.at(x, y) = func(src.at(x, y));
        }
    }
    return out;
}

} // namespace mist

#endif
