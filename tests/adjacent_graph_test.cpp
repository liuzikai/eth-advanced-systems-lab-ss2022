#include <catch.hpp>


#include "adjacency_graph.h"


#include <set>

static std::set<index_t> set_from_list(const adjacency_list_t &list) {
    return {list.neighbors, list.neighbors + list.count};
}

TEST_CASE("adjacency_graph: sample graph") {
    adjacency_graph_t *graph = create_graph_from_file(INPUT_DIR "sample.txt");
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