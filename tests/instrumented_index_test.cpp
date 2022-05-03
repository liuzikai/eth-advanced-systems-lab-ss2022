#include <catch.hpp>

#include "instrumented_index.h"

TEST_CASE("InstrumentedIndex") {
    InstrumentedIndex i = 0;
    InstrumentedIndex i2 = 0;
    REQUIRE(i2 == InstrumentedIndex(0));
    i++;

    OpCounter::ResetOpCount();
    REQUIRE(OpCounter::GetOpCount() == 0);

    i++;
    REQUIRE(OpCounter::GetOpCount() == 1);

    ++i;
    REQUIRE(OpCounter::GetOpCount() == 2);
    REQUIRE(i == InstrumentedIndex(3));
    REQUIRE(i2 == InstrumentedIndex(0));

    bool comp = i2 < i;
    REQUIRE(comp == true);
    REQUIRE(OpCounter::GetOpCount() == 5);

    REQUIRE(i2 == InstrumentedIndex(0));
    REQUIRE( !(i2 == i));
    REQUIRE(OpCounter::GetOpCount() == 7);
}
