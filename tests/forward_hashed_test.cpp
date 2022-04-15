#include <catch.hpp>

#include "forward_hashed.h"
#include "common.h"

#include <set>
#include "triangles.h"

TEST_CASE("forward_hashed: example graph") {
    triangles.clear();
    print_triangle_func_t original_print = print_triangle;
    print_triangle = add_triangle;

    adjacency_graph_hashed_t *A = nullptr;
    adjacency_graph_t *graph = create_graph_from_file_hashed(INPUT_DIR "sample.txt", &A);
    REQUIRE(forward_hashed(graph, A->adjacency) == 3);
    REQUIRE(triangles == TriangleSet{{0, 1, 2}, {0, 1, 3}, {0, 3, 4}});
    free_graph_hashed(A);
    free_graph(graph);
    
    A = nullptr;
    graph = create_graph_from_file_hashed(INPUT_DIR "sample2.txt", &A);
    REQUIRE(forward_hashed(graph, A->adjacency) == 5);
    REQUIRE(triangles == TriangleSet{{0, 1, 2}, {0, 1, 3}, {0, 3, 4}, {0, 5, 6}, {0, 7, 8}});
    free_graph_hashed(A);
    free_graph(graph);

    print_triangle = original_print;
}
