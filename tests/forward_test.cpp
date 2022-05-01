#include <catch.hpp>

#include "forward.h"
#include "common.h"

#include <set>

TEST_CASE("forward: neighbor container") {

    AdjacencyGraph<index_t> *G = create_graph_from_file<index_t>(INPUT_DIR "sample.txt");
    ForwardNeighborContainer<index_t> *A = forward_create_neighbor_container(G);
    REQUIRE(A != nullptr);
    REQUIRE(A->n == 5);
    for (index_t i = 0; i < A->n; i++) {
        REQUIRE(A->adjacency[i].count == 0);
    }
    free_graph(G);
    forward_delete_neighbor_container(A);
}

TEST_CASE("forward: example graph") {
    AdjacencyGraph<index_t> *G = create_graph_from_file<index_t>(INPUT_DIR "sample_undirected.txt");
    ForwardNeighborContainer<index_t> *A = forward_create_neighbor_container(G);
    for (int i = 0; i < 3; i++) {  // repeat
        auto lister = forward<index_t, TriangleListing::Collect>(G, A);
        REQUIRE(lister.triangles.size() == 3);
        REQUIRE(lister.triangles == TriangleSet{{0, 1, 2},
                                         {0, 1, 3},
                                         {0, 3, 4}});
    }
    free_graph(G);
    forward_delete_neighbor_container(A);


    G = create_graph_from_file<index_t>(INPUT_DIR "sample2.txt");
    A = forward_create_neighbor_container(G);
    auto lister = forward<index_t, TriangleListing::Collect>(G, A);
    REQUIRE(lister.triangles.size() == 5);
    REQUIRE(lister.triangles == TriangleSet{{0, 1, 2},
                                     {0, 1, 3},
                                     {0, 3, 4},
                                     {0, 5, 6},
                                     {0, 7, 8}});
    free_graph(G);
    forward_delete_neighbor_container(A);
}
