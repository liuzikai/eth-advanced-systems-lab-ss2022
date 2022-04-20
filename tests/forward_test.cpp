#include <catch.hpp>

#include "forward.h"
#include "common.h"

#include <set>
#include "triangles.h"

TEST_CASE("forward: neighbor container") {

    adjacency_graph_t *G = create_graph_from_file(INPUT_DIR "sample.txt");
    forward_neighbor_container_t *A = forward_create_neighbor_container(G);
    REQUIRE(A != nullptr);
    REQUIRE(A->n == 5);
    for (index_t i = 0; i < A->n; i++) {
        REQUIRE(A->adjacency[i].count == 0);
    }
    free_graph(G);
    forward_delete_neighbor_container(A);
}

TEST_CASE("forward: example graph") {
    triangles.clear();
    print_triangle_func_t original_print = print_triangle;
    print_triangle = add_triangle;


    adjacency_graph_t *G = create_graph_from_file(INPUT_DIR "sample_undirected.txt");
    forward_neighbor_container_t *A = forward_create_neighbor_container(G);
    for (int i = 0; i < 3; i++) {  // repeat
        REQUIRE(forward(G, A) == 3);
        REQUIRE(triangles == TriangleSet{{0, 1, 2},
                                         {0, 1, 3},
                                         {0, 3, 4}});
    }
    free_graph(G);
    forward_delete_neighbor_container(A);


    G = create_graph_from_file(INPUT_DIR "sample2.txt");
    A = forward_create_neighbor_container(G);
    REQUIRE(forward(G, A) == 5);
    REQUIRE(triangles == TriangleSet{{0, 1, 2},
                                     {0, 1, 3},
                                     {0, 3, 4},
                                     {0, 5, 6},
                                     {0, 7, 8}});
    free_graph(G);
    forward_delete_neighbor_container(A);

    print_triangle = original_print;
}
