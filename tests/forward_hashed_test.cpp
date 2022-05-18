#include <catch.hpp>

#include "forward_hashed/forward_hashed_base.hpp"
#include "common.h"

#include <set>
#include "triangle_lister.h"

TEST_CASE("forward_hashed: example graph") {
    using namespace forward_hashed_base;
    AdjacencyGraph<index_t> *G = create_graph_from_file<index_t>(INPUT_DIR "sample_undirected.txt");
    ForwardHashedNeighborContainer<index_t> *A = forward_hashed_create_neighbor_container(G);
    forward_hashed_reset_neighbor_container(G, A);
    for (int i = 0; i < 3; i++) {  // repeat
        auto lister = forward_hashed<index_t, index_t, TriangleListing::Collect<index_t>>(G, A);
        REQUIRE(lister.triangles.size() == 3);
        REQUIRE(lister.triangles == TriangleListing::Collect<index_t>::TriangleSet{{0, 1, 2},
                                         {0, 1, 3},
                                         {0, 3, 4}});
    }
    free_graph(G);
    forward_hashed_delete_neighbor_container(A);

    G = create_graph_from_file<index_t>(INPUT_DIR "sample2.txt");
    A = forward_hashed_create_neighbor_container(G);
    auto lister = forward_hashed<index_t, index_t, TriangleListing::Collect<index_t>>(G, A);
    REQUIRE(lister.triangles.size() == 5);
    REQUIRE(lister.triangles == TriangleListing::Collect<index_t>::TriangleSet{{0, 1, 2}, {0, 1, 3}, {0, 3, 4}, {0, 5, 6}, {0, 7, 8}});
    free_graph(G);
    forward_hashed_delete_neighbor_container(A);
}

TEST_CASE("forward_hashed: WWW_NOTRE_DAME graph") {
    using namespace forward_hashed_base;
    std::string generate_graph = "./graph_generation -gt www_notre_dame -o notre.txt -shuffle_edges";
    std::string delete_graph = "rm notre.txt";
    std::ignore = system(generate_graph.c_str());
    AdjacencyGraph<index_t> *graph = create_graph_from_file<index_t>("notre.txt");
    ForwardHashedNeighborContainer<index_t> *A = forward_hashed_create_neighbor_container(graph);
    auto triangle_listing = forward_hashed<index_t, index_t, TriangleListing::Collect<index_t>>(graph, A);
    REQUIRE(triangle_listing.triangles.size() == 8910005);
    free_graph(graph);
    forward_hashed_delete_neighbor_container(A);
    std::ignore = system(delete_graph.c_str());
}

TEST_CASE("forward_hashed: US Patents graph") {
    using namespace forward_hashed_base;
    std::string generate_graph = "./graph_generation -gt us_patents -o us_patents.txt -shuffle_edges";
    std::string delete_graph = "rm us_patents.txt";
    std::ignore = system(generate_graph.c_str());
    AdjacencyGraph<index_t> *graph = create_graph_from_file<index_t>("us_patents.txt");
    ForwardHashedNeighborContainer<index_t> *A = forward_hashed_create_neighbor_container(graph);
    auto triangle_listing = forward_hashed<index_t, index_t, TriangleListing::Collect<index_t>>(graph, A);
    REQUIRE(triangle_listing.triangles.size() == 7515023);
    free_graph(graph);
    forward_hashed_delete_neighbor_container(A);
    std::ignore = system(delete_graph.c_str());
}