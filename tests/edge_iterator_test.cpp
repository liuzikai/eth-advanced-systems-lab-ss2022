#include <catch.hpp>
#include "edge_iterator.h"
#include "common.h"
#include <set>
#include "triangle_lister.h"

TEST_CASE("edge_iterator: example graph") {  
    AdjacencyGraph<index_t> *graph = create_graph_from_file<index_t>(INPUT_DIR "sample_undirected.txt");
    auto triangle_listing = edge_iterator<index_t, TriangleListing::Collect>(graph);
    REQUIRE(triangle_listing.triangles.size() == 3);
    REQUIRE(triangle_listing.triangles == TriangleSet{{0, 1, 2}, {0, 1, 3}, {0, 3, 4}});
    free_graph(graph);
}