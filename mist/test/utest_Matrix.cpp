#include "Matrix.h"
#include "Point.h"

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

using namespace mist;

TEST_CASE("Matrix accessors", "[utils]")
{
    Matrix<int> m(5, 10);

    m.at(0, 0) = 100;
    CHECK(m.at(0, 0) == 100);

    const Point2i p {4, 8};
    m.at(p) = 48;
    CHECK(m.at(p) == 48);
}

TEST_CASE("Matrix contains point", "[utils]")
{
    const Matrix<int> m(3, 9);

    CHECK_FALSE(m.contains(Point2i {-1, 0}));
    CHECK_FALSE(m.contains(Point2i {0, -1}));
    CHECK_FALSE(m.contains(Point2i {3, 0}));
    CHECK_FALSE(m.contains(Point2i {0, 9}));
    CHECK(m.contains(Point2i {0, 0}));
    CHECK(m.contains(Point2i {2, 8}));
}

TEST_CASE("Matrix foreach", "[utils]")
{
    Matrix<int> m(2, 3);
    m.fill(99);
    m.foreachValue([](int &i) {
        i = i + 1;
    });
    CHECK(m.at(0, 0) == 100);
    CHECK(m.at(1, 0) == 100);
    CHECK(m.at(0, 1) == 100);
    CHECK(m.at(1, 1) == 100);
    CHECK(m.at(0, 2) == 100);
    CHECK(m.at(1, 2) == 100);
}

TEST_CASE("Matrix const foreach", "[utils]")
{
    Matrix<int> m(2, 3);
    m.fill(0);
    m.at(1, 1) = 9;
    m.at(2, 0) = 9;

    int                countNines = 0;
    const Matrix<int> &cm = m;
    cm.foreachValue([&](int i) {
        if (i == 9) countNines++;
    });
    CHECK(countNines == 2);
}

TEST_CASE("Matrix transform", "[utils]")
{
    Matrix<int> m(2, 3);
    m.fill(99);
    m.transform([](int i) {
        return i + 1;
    });
    CHECK(m.at(0, 0) == 100);
    CHECK(m.at(1, 0) == 100);
    CHECK(m.at(0, 1) == 100);
    CHECK(m.at(1, 1) == 100);
    CHECK(m.at(0, 2) == 100);
    CHECK(m.at(1, 2) == 100);
}

TEST_CASE("Matrix min/max", "[utils]")
{
    Matrix<int> m(2, 3);
    m.fill(0);
    m.at(0, 0) = -5;
    m.at(1, 1) = 88;

    const Matrix<int> &cm = m;
    CHECK(min(cm) == -5);
    CHECK(max(cm) == 88);
}
