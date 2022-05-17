#include <catch.hpp>

#include "merge_sort.h"
#include "merge_sort_v2.h"

TEST_CASE("MergeSort") {
    constexpr int N = 13;
    index_t in[N] = {13, 12,11,10,9,8,7,6,5,4,3,2,1};
    index_t other[N] = {0};

    merge_sort_v1::merge_sort<index_t>(in, other,  N);

    for(index_t i = 0; i < N; i++) {
        REQUIRE(in[i] == i + 1);
    }
}


TEST_CASE("13", "[MergeSort2]") {
     constexpr int N = 13;
    index_t in[N] = {13, 12,11,10,9,8,7,6,5,4,3,2,1};
    index_t other[N] = {0};

    merge_sort_v2::merge_sort<index_t>(in, other,  N);
    for(index_t i = 0; i < N; i++) {
        REQUIRE(in[i] == i + 1);
    }
}

TEST_CASE("12", "[MergeSort2]") {
     constexpr int N = 12;
    index_t in[N] = {12,11,10,9,8,7,6,5,4,3,2,1};
    index_t other[N] = {0};

    merge_sort_v2::merge_sort<index_t>(in, other,  N);
    for(index_t i = 0; i < N; i++) {
        REQUIRE(in[i] == i + 1);
    }
}

TEST_CASE("11", "[MergeSort2]") {
     constexpr int N = 11;
    index_t in[N] = {11,10,9,8,7,6,5,4,3,2,1};
    index_t other[N] = {0};

    merge_sort_v2::merge_sort<index_t>(in, other,  N);
    for(index_t i = 0; i < N; i++) {
        REQUIRE(in[i] == i + 1);
    }
}

TEST_CASE("10", "[MergeSort2]") {
     constexpr int N = 10;
    index_t in[N] = {10,9,8,7,6,5,4,3,2,1};
    index_t other[N] = {0};

    merge_sort_v2::merge_sort<index_t>(in, other,  N);
    for(index_t i = 0; i < N; i++) {
        REQUIRE(in[i] == i + 1);
    }
}

TEST_CASE("9", "[MergeSort2]") {
     constexpr int N = 9;
    index_t in[N] = {9,8,7,6,5,4,3,2,1};
    index_t other[N] = {0};

    merge_sort_v2::merge_sort<index_t>(in, other,  N);
    for(index_t i = 0; i < N; i++) {
        REQUIRE(in[i] == i + 1);
    }
}

TEST_CASE("8", "[MergeSort2]") {
     constexpr int N = 8;
    index_t in[N] = {8,7,6,5,4,3,2,1};
    index_t other[N] = {0};

    merge_sort_v2::merge_sort<index_t>(in, other,  N);
    for(index_t i = 0; i < N; i++) {
        REQUIRE(in[i] == i + 1);
    }
}
