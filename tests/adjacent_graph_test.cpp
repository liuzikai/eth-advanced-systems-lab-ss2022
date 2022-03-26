#include <catch.hpp>

extern "C" {
#include "adjacency_graph.h"
}

#include <set>

static std::set<index_t> set_from_list(const adjacency_list_t &list) {
    return {list.neighbors, list.neighbors + list.count};
}

TEST_CASE("adjacency_graph: sample graph") {
    adjacency_graph_t *graph = create_graph_from_file(INPUT_DIR "sample.txt", nullptr);
    REQUIRE(graph->n == 5);
    REQUIRE(graph->adjacency[0].count == 4);
    REQUIRE(set_from_list(graph->adjacency[0]) == std::set<index_t>({1, 2, 3, 4}));
    REQUIRE(graph->adjacency[1].count == 2);
    REQUIRE(set_from_list(graph->adjacency[1]) == std::set<index_t>({2, 3}));
    REQUIRE(graph->adjacency[2].count == 0);
    REQUIRE(graph->adjacency[3].count == 1);
    REQUIRE(set_from_list(graph->adjacency[3]) == std::set<index_t>({4}));
    REQUIRE(graph->adjacency[4].count == 0);
    free_graph(graph);
}

TEST_CASE("adjacency_graph: accessory graph") {
    adjacency_graph_t *A = nullptr;
    adjacency_graph_t *graph = create_graph_from_file(INPUT_DIR "sample.txt", &A);
    REQUIRE(A != nullptr);
    REQUIRE(A->n == 5);
    for (index_t i = 0; i < A->n; i++) {
        REQUIRE(A->adjacency[i].count == 0);
    }
    free_graph(A);
    free_graph(graph);
}