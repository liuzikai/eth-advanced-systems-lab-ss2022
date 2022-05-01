#include <catch.hpp>

#include "forward_hashed.h"
#include "common.h"

#include <set>
#include "triangle_lister.h"

TEST_CASE("forward_hashed: example graph") {
    AdjacencyGraph<index_t> *G = create_graph_from_file<index_t>(INPUT_DIR "sample_undirected.txt");
    ForwardHashedNeighborContainer<index_t> *A = forward_hashed_create_neighbor_container(G);
    for (int i = 0; i < 3; i++) {  // repeat
        auto lister = forward_hashed<index_t, TriangleListing::Collect>(G, A);
        REQUIRE(lister.triangles.size() == 3);
        REQUIRE(lister.triangles == TriangleSet{{0, 1, 2},
                                         {0, 1, 3},
                                         {0, 3, 4}});
    }
    free_graph(G);
    forward_hashed_delete_neighbor_container(A);

    G = create_graph_from_file<index_t>(INPUT_DIR "sample2.txt");
    A = forward_hashed_create_neighbor_container(G);
    auto lister = forward_hashed<index_t, TriangleListing::Collect>(G, A);
    REQUIRE(lister.triangles.size() == 5);
    REQUIRE(lister.triangles == TriangleSet{{0, 1, 2}, {0, 1, 3}, {0, 3, 4}, {0, 5, 6}, {0, 7, 8}});
    free_graph(G);
    forward_hashed_delete_neighbor_container(A);
}
