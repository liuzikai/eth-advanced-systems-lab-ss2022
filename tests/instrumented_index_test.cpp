#include <catch.hpp>

#include "instrumented_index.h"

TEST_CASE("InstrumentedIndex") {
    InstrumentedIndex i(0);
    InstrumentedIndex i2(0);
    REQUIRE(i2 == InstrumentedIndex(0));
    i++;

    OpCounter::ResetOpCount();
    REQUIRE(OpCounter::GetOpCount() == 0);

    i++;
    REQUIRE(OpCounter::GetOpCount() == 1);

    ++i;
    REQUIRE(OpCounter::GetOpCount() == 2);

    REQUIRE(i == InstrumentedIndex(3));
    REQUIRE(OpCounter::GetOpCount() == 3);

    REQUIRE((index_t) (i % /* increase */ InstrumentedIndex(2)) == /* no increase */ 1);
    REQUIRE(OpCounter::GetOpCount() == 4);

    REQUIRE((index_t) (i % /* increase */ InstrumentedIndex(5)) == /* no increase */ 3);
    REQUIRE(OpCounter::GetOpCount() == 5);

    REQUIRE(i2 == InstrumentedIndex(0));
    REQUIRE(OpCounter::GetOpCount() == 6);

    bool comp = i2 < i;
    REQUIRE(comp == true);
    REQUIRE(OpCounter::GetOpCount() == 7);

    REQUIRE(i2 == InstrumentedIndex(0));
    REQUIRE(!(i2 == i));
    REQUIRE(OpCounter::GetOpCount() == 9);
}
