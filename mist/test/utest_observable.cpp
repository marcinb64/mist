#include "Value.h"

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

using namespace mist;

TEST_CASE("Value basic accessors", "[observable]")
{
    SECTION("Initializers")
    {
        Value<int> v2 {456};
        CHECK(v2.get() == 456);

        Value<int> v3(v2);
        CHECK(v3.get() == 456);

        Value<int> v4(Value<int>(789));
        CHECK(v4.get() == 789);
    }

    SECTION("Accessors")
    {
        Value<int> v1 {5};

        int implicitCast = v1;
        CHECK(implicitCast == 5);
        CHECK(v1 == 5);

        v1 = 10;
        CHECK(v1 == 10);
    }
}

TEST_CASE("Value observers", "[observable]")
{
    Value<int> value {0};

    SECTION("Observer")
    {
        Owner owner;
        int   observed = -1;

        value.watch(&owner, [&](const int &v) {
            observed = v;
        });
        CHECK(observed == 0);

        value = 5;
        CHECK(observed == 5);
    }
}

TEST_CASE("Explicit stop watching", "[observable]")
{
    Value<int> value {0};
    Owner      owner;
    int        observed = -1;

    value.watch(&owner, [&](const int &v) {
        observed = v;
    });

    value = 5;
    value.stopWatching(&owner);
    value = 6;
    CHECK(observed == 5);
}

TEST_CASE("Lifecycle", "[observable]")
{
    int observed = -1;

    SECTION("Stop watching when owner is destroyed")
    {
        Value<int> value {0};

        {
            Owner owner;
            value.watch(&owner, [&](const int &v) {
                observed = v;
            });
            value = 5;
        }

        value = 6;
        CHECK(observed == 5);
    }

    SECTION("Stop watching when value is destroyed")
    {
        Owner owner;

        {
            Value<int> value {0};
            value.watch(&owner, [&](const int &v) {
                observed = v;
            });
            value = 5;
        }

        CHECK(observed == 5);
    }
}
