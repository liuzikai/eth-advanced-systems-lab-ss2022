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

TEST_CASE("edge_iterator: WWW_NOTRE_DAME graph") {
    std::string generate_graph = "./graph_generation -gt www_notre_dame -o notre.txt -shuffle_edges";
    std::string delete_graph = "rm notre.txt";
    std::ignore = system(generate_graph.c_str());
    AdjacencyGraph<index_t> *graph = create_graph_from_file<index_t>("notre.txt");
    auto triangle_listing = edge_iterator<index_t, TriangleListing::Collect>(graph);
    REQUIRE(triangle_listing.triangles.size() == 8910005);
    free_graph(graph);
    std::ignore = system(delete_graph.c_str());
}

TEST_CASE("edge_iterator: US Patents graph") {
    std::string generate_graph = "./graph_generation -gt us_patents -o us_patents.txt -shuffle_edges";
    std::string delete_graph = "rm us_patents.txt";
    std::ignore = system(generate_graph.c_str());
    AdjacencyGraph<index_t> *graph = create_graph_from_file<index_t>("us_patents.txt");
    auto triangle_listing = edge_iterator<index_t, TriangleListing::Collect>(graph);
    REQUIRE(triangle_listing.triangles.size() == 7515023);
    free_graph(graph);
    std::ignore = system(delete_graph.c_str());
}