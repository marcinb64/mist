#ifndef MAPTOOLS_H_
#define MAPTOOLS_H_

#include "Matrix.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <limits>
#include <list>
#include <set>

namespace mist
{

template <typename T, class OutIterator>
auto addPointsNextTo(const Matrix<T> &m, const Point2i &p0, OutIterator inserter)
{
    static constexpr std::array plusMinusOneInCardinalDirs {Point2i {-1, 0}, Point2i {0, -1},
                                                            Point2i {1, 0}, Point2i {0, 1}};

    for (const auto &d : plusMinusOneInCardinalDirs) {
        const auto p = p0 + d;
        if (m.contains(p)) *inserter++ = p;
    }
}

/* -------------------------------------------------------------------------- */

class DiamondSquare
{
public:
    DiamondSquare(Matrix<double> &output_);

    auto setSeed(long seed_) -> DiamondSquare &;
    auto setRoughness(double roughness_) -> DiamondSquare &;
    auto setInitialRandomness(double initRand_) -> DiamondSquare &;
    auto build() -> void;

private:
    Matrix<double> &output;
    int             size;
    long            seed = 0;
    double          roughness = 1.0;
    double          initialRandomness = 0.6;

    auto diamond(const Point2i &p, int a) -> double;
    auto square(const Point2i &p, int a) -> double;
};

/* -------------------------------------------------------------------------- */

template <typename T> auto makeDistanceMatrix(int radius) -> Matrix<T>
{
    const auto           brushSize = 2 * radius + 1;
    const auto           center = Point2i {brushSize / 2, brushSize / 2};
    mist::Matrix<double> brush(brushSize, brushSize);

    brush.generate([=](const Point2i &p) {
        return (p - center).length();
    });

    return brush;
}

template <typename T> class MapBrush
{
public:
    MapBrush(Matrix<T> &map_, int radius)
        : map(map_), brush(mist::makeDistanceMatrix<T>(radius)), brushCenter(radius, radius)
    {
    }

    template <class List, class BrushFunc> auto atPoints(const List &points, BrushFunc func)
    {
        for (const auto &p0 : points) {
            brush.foreachKeyValue([&](const Point2i &d, const double r) {
                const auto p = p0 + (d - brushCenter);
                if (!map.contains(p)) return;

                func(p, r);
            });
        }
    }

private:
    Matrix<T>      &map;
    mist::Matrix<T> brush;
    Point2i         brushCenter;
};

/* -------------------------------------------------------------------------- */

template <typename T, class FillFunc>
auto floodFill(const Matrix<T> &map, const Point2i &origin, T maxDistance, T fillUpTo,
               FillFunc filler) -> void
{
    std::list<Point2i> frontier {origin};
    std::set<Point2i>  visited;

    while (!frontier.empty()) {
        const Point2i p0 = frontier.front();
        frontier.pop_front();

        if (map.at(p0) > fillUpTo) continue;
        if (visited.contains(p0)) continue;
        if (static_cast<T>((p0 - origin).length()) > maxDistance) continue;

        filler(p0);
        visited.emplace(p0);

        addPointsNextTo(map, p0, std::back_inserter(frontier));
    }
}

/* -------------------------------------------------------------------------- */

template <typename T> class AStar
{
private:
    static constexpr auto infinity = std::numeric_limits<T>::max();

public:
    AStar(const Matrix<T> &map_) : map(map_), cost(map_.getSize()) {}

    auto calculate(const Point2i &from) -> AStar &
    {
        static constexpr std::array plusMinusOneInCardinalDirs {Point2i {-1, 0}, Point2i {0, -1},
                                                                Point2i {1, 0}, Point2i {0, 1}};

        // offset all map values to make travel costs non-negative
        const auto offset = -std::min(static_cast<T>(0), min(map)) + 1;

        // Reset
        startPoint = from;
        cost.fill(infinity);
        cost.at(from) = 0;

        std::list<Point2i> frontier;
        frontier.emplace_back(from);

        // Explore
        while (!frontier.empty()) {
            const auto p0 = frontier.front();
            frontier.pop_front();

            for (const auto &d : plusMinusOneInCardinalDirs) {
                // expand to surrounding points
                const auto p = p0 + d;
                if (!map.contains(p) || p == from) continue;

                // map values above requested threshold block movement
                if (map.at(p) > blockValue) continue;

                // total cost to reach 'p' = total cost to 'p0' + cost to move through 'p'
                const auto candidateCost =
                    cost.at(p0) + std::pow(map.at(p) + offset, routeCostFactor);

                // update cost map if we found a better way to reach 'p', and keep expanding from
                // 'p'
                if (candidateCost < cost.at(p)) {
                    cost.at(p) = static_cast<T>(candidateCost);
                    frontier.emplace_back(p);
                }
            }
        }

        return *this;
    }

    [[nodiscard]] auto canReach(const Point2i &p) const -> bool { return cost.at(p) < infinity; }

    auto route(const Point2i &endPoint) -> std::list<Point2i>
    {
        static constexpr std::array plusMinusOneInCardinalDirs {Point2i {-1, 0}, Point2i {0, -1},
                                                                Point2i {1, 0}, Point2i {0, 1}};

        // Start from the end...
        std::list<Point2i> ret;
        Point2i            p0 = endPoint;

        // ...and follow the lowest cost until we reach the start
        while (p0 != startPoint) {
            ret.emplace_back(p0);

            // Search for lowest cost surrounding 'p0'
            Point2i bestPoint;
            T       bestValue = infinity;
            for (const auto &d : plusMinusOneInCardinalDirs) {
                const auto p = p0 + d;
                if (!map.contains(p)) continue;
                if (cost.at(p) < bestValue) {
                    bestPoint = p;
                    bestValue = cost.at(p);
                }
            }

            p0 = bestPoint;
        }

        ret.emplace_back(p0);
        return ret;
    }

    [[nodiscard]] auto getCost() const noexcept -> const Matrix<T> & { return cost; }

    auto setStepCostFactor(T a) -> AStar &
    {
        routeCostFactor = a;
        return *this;
    }

    auto setBlockValue(T blockValue_) -> AStar &
    {
        blockValue = blockValue_;
        return *this;
    }

private:
    const Matrix<T> &map;
    Matrix<T>        cost;
    T                blockValue {infinity};
    T                routeCostFactor {1};
    Point2i          startPoint;
};

/* -------------------------------------------------------------------------- */

template <typename T> auto calculateGradient(const Matrix<T> &src) -> Matrix<Point2<T>>
{
    Matrix<Point2<T>> grad(src.getSize());

    src.foreachKeyValue([&](const Point2i &p, T v) {
        Point2i x1 {p.x + 1, p.y};
        Point2i y1 {p.x, p.y + 1};

        const auto dx = (src.contains(x1) ? src.at(x1) : v) - v;
        const auto dy = (src.contains(y1) ? src.at(y1) : v) - v;

        grad.at(p) = {dx, dy};
    });

    return grad;
}

} // namespace mist

#endif
