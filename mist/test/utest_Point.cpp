#include "Point.h"

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

using namespace mist;
using namespace Catch::Matchers;

TEST_CASE("Point2i")
{
    const Point2i p0;
    const Point2i p5 {5, 55};
    const Point2i pc(p5);

    CHECK((p0.x == 0 && p0.y == 0));
    CHECK((p5.x == 5 && p5.y == 55));
    CHECK((pc.x == 5 && pc.y == 55));
}

TEST_CASE("Point comparison")
{
    CHECK(Point2i {3, 10} < Point2i {9, 0});
    CHECK(Point2i {3, 10} < Point2i {3, 11});

    CHECK(Point2i {9, 0} > Point2i {3, 10});
    CHECK(Point2i {3, 11} > Point2i {3, 10});

    CHECK(Point2i {3, 11} == Point2i {3, 11});
    CHECK(Point2i {3, 11} != Point2i {4, 11});
}

TEST_CASE("Point2 rounding")
{
    SECTION("float -> int")
    {
        Point2f p1 {0.5001f, 2.444f};
        Point2i p2 = round(p1);

        CHECK(p2 == Point2i {1, 2});
    }

    SECTION("double -> int")
    {
        Point2d p1 {0.5001, 2.4444};
        Point2i p2 = round(p1);

        CHECK(p2 == Point2i {1, 2});
    }
}

TEST_CASE("Point2 casting")
{
    SECTION("int -> float")
    {
        Point2i p1 {1, 2};
        Point2f p2 = static_cast<Point2f>(p1);
        Point2f p3 = p1.as<float>();

        CHECK(p2 == Point2f {1.0f, 2.0f});
        CHECK(p3 == Point2f {1.0f, 2.0f});
    }

    SECTION("int -> double")
    {
        Point2i p1 {1, 2};
        Point2d p2 = static_cast<Point2d>(p1);
        Point2d p3 = p1.as<double>();

        CHECK(p2 == Point2d {1.0, 2.0});
        CHECK(p3 == Point2d {1.0f, 2.0f});
    }

    SECTION("double -> float")
    {
        Point2d p1 {1.8, 2.9};
        Point2f p2 = static_cast<Point2f>(p1);
        Point2f p3 = p1.as<float>();

        CHECK(p2 == Point2f {1.8f, 2.9f});
        CHECK(p3 == Point2f {1.8f, 2.9f});
    }
}

TEST_CASE("Point2 simple math")
{
    SECTION("int")
    {
        Point2i a {1, 2};
        Point2i b {10, 20};

        CHECK(a + b == Point2i {11, 22});
        CHECK(b - a == Point2i {9, 18});
        CHECK(a * 3 == Point2i {3, 6});
        CHECK(b / 2 == Point2i {5, 10});

        a *= 3;
        CHECK(a == Point2i {3, 6});
        b /= 2;
        CHECK(b == Point2i {5, 10});
    }

    SECTION("double")
    {
        Point2d a {1.1, 2.2};
        Point2d b {10.0, 20.0};

        CHECK(a + b == Point2d {11.1, 22.2});
        CHECK(b - a == Point2d {8.9, 17.8});
        auto r1 = a * 3;
        CHECK_THAT(r1.x, WithinAbs(3.3, 0.05));
        CHECK_THAT(r1.y, WithinAbs(6.6, 0.05));
        auto r2 = b / 2;
        CHECK_THAT(r2.x, WithinAbs(5.0, 0.05));
        CHECK_THAT(r2.y, WithinAbs(10.0, 0.05));
    }
}

TEST_CASE("Point2 length")
{
    Point2i p {5, 10};
    CHECK_THAT(p.length(), WithinAbs(11.18, 0.01));
}
