//
// Updated by JZ on 20.03.22.
//

#include <catch.hpp>
extern "C" {
#include "edge_iterator.h"
#include "common.h"
}
#include <set>
#include "triangles.h"

// using Triangle = std::set<index_t>;
// using TriangleSet = std::set<Triangle>;
// TriangleSet triangles;
// extern "C" {
//     static void add_triangle(index_t u, index_t v, index_t w) {
//         triangles.insert({u, v, w});
//     }
// }

TEST_CASE("edge_iterator: example graph") {
    triangles.clear();
    print_triangle_func_t original_print = print_triangle;
    print_triangle = add_triangle;

    adjacency_graph_t *graph = create_graph_from_file(INPUT_DIR "sample.txt", NULL);
    REQUIRE(edge_iterator(graph) == 3);
    REQUIRE(triangles == TriangleSet{{0, 1, 2}, {0, 1, 3}, {0, 3, 4}});
    free_graph(graph);

    print_triangle = original_print;
}