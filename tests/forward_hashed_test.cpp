#include <catch.hpp>

#include "forward_hashed.h"
#include "common.h"

#include <set>
#include "triangles.h"

TEST_CASE("forward_hashed: example graph") {
    triangles.clear();
    print_triangle_func_t original_print = print_triangle;
    print_triangle = add_triangle;

    AdjacencyGraph<index_t> *G = create_graph_from_file<index_t>(INPUT_DIR "sample_undirected.txt");
    ForwardHashedNeighborContainer<index_t> *A = forward_hashed_create_neighbor_container(G);
    for (int i = 0; i < 3; i++) {  // repeat
        REQUIRE(forward_hashed(G, A) == 3);
        REQUIRE(triangles == TriangleSet{{0, 1, 2},
                                         {0, 1, 3},
                                         {0, 3, 4}});
    }
    free_graph(G);
    forward_hashed_delete_neighbor_container(A);

    G = create_graph_from_file<index_t>(INPUT_DIR "sample2.txt");
    A = forward_hashed_create_neighbor_container(G);
    REQUIRE(forward_hashed(G, A) == 5);
    REQUIRE(triangles == TriangleSet{{0, 1, 2}, {0, 1, 3}, {0, 3, 4}, {0, 5, 6}, {0, 7, 8}});
    free_graph(G);
    forward_hashed_delete_neighbor_container(A);

    print_triangle = original_print;
}
