#include <catch.hpp>

#include "merge_sort.h"

TEST_CASE("MergeSort") {
    constexpr int N = 13;
    index_t in[N] = {13, 12,11,10,9,8,7,6,5,4,3,2,1};
    index_t other[N] = {0};

    merge_sort<index_t>(in, other,  N);

    for(index_t i = 0; i < N; i++) {
        REQUIRE(in[i] == i + 1);
    }
}
