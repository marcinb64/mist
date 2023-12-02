#include "Point.h"
#include "moremath.h"

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

using namespace mist;
using namespace Catch::Matchers;

TEST_CASE("Linear transform - identity")
{
    constexpr auto identity = LinearTransform<double>::identity();
    CHECK_THAT(identity(3.456), WithinAbs(3.456, 0.001));
}

TEST_CASE("Linear transform")
{
    constexpr auto tr = LinearTransform<double>(-1.0, 1.0, 10.0, 20.0);
    CHECK_THAT(tr(-1.0), WithinAbs(10.0, 0.001));
    CHECK_THAT(tr(-0.5), WithinAbs(12.5, 0.001));
    CHECK_THAT(tr(0), WithinAbs(15.0, 0.001));
    CHECK_THAT(tr(0.5), WithinAbs(17.5, 0.001));
    CHECK_THAT(tr(1.0), WithinAbs(20.0, 0.001));
}

TEST_CASE("Linear transform 2D - identity")
{
    auto identity = LinearTransform2<double>::identity();
    auto r = identity(Point2d {3.456, 9.876});
    CHECK_THAT(r.x, WithinAbs(3.456, 0.001));
    CHECK_THAT(r.y, WithinAbs(9.876, 0.001));
}

TEST_CASE("Linear transform 2D")
{
    auto tr = LinearTransform2<double>({-1.0, -10.0}, {1.0, 10.0}, {30.0, 300.0}, {40.0, 400.0});

    auto p1 = tr(Point2d {-1.0, -10.0});
    CHECK_THAT(p1.x, WithinAbs(30.0, 0.001));
    CHECK_THAT(p1.y, WithinAbs(300, 0.001));

    auto p2 = tr(Point2d {-0.5, -5.0});
    CHECK_THAT(p2.x, WithinAbs(32.5, 0.001));
    CHECK_THAT(p2.y, WithinAbs(325, 0.001));
    
    auto p3 = tr(Point2d {0.0, 0.0});
    CHECK_THAT(p3.x, WithinAbs(35.0, 0.001));
    CHECK_THAT(p3.y, WithinAbs(350, 0.001));
}
