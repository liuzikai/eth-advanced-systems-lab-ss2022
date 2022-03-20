//
// Created by Zikai Liu on 3/5/22.
//

#include <catch.hpp>
extern "C" {
#include "forward.h"
#include "common.h"
}
#include <set>

using Triangle = std::set<index_t>;
using TriangleSet = std::set<Triangle>;
TriangleSet triangles;
extern "C" {
    static void add_triangle(index_t u, index_t v, index_t w) {
        triangles.insert({u, v, w});
    }
}

TEST_CASE("forward: example graph") {
    triangles.clear();
    print_triangle_func_t original_print = print_triangle;
    print_triangle = add_triangle;

    adjacency_graph_t *A = nullptr;
    adjacency_graph_t *graph = create_graph_from_file(INPUT_DIR "sample.txt", &A);
    REQUIRE(forward(graph, A->adjacency) == 3);
    REQUIRE(triangles == TriangleSet{{0, 1, 2}, {0, 1, 3}, {0, 3, 4}});
    free_graph(A);
    free_graph(graph);

    print_triangle = original_print;
}