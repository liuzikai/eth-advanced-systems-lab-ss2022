#include <catch.hpp>

#include "forward/forward_base.hpp"
#include "common.h"

#include <set>

TEST_CASE("forward: neighbor container") {
    using namespace f0;
    AdjacencyGraph<index_t> *G = create_graph_from_file<index_t>(INPUT_DIR "sample.txt");
    ForwardNeighborContainer<index_t> *A = forward_create_neighbor_container(G);
    forward_reset_neighbor_container(G, A);
    REQUIRE(A != nullptr);
    REQUIRE(A->n == 5);
    for (index_t i = 0; i < A->n; i++) {
        REQUIRE(A->adjacency[i].count == 0);
    }
    free_graph(G);
    forward_delete_neighbor_container(A);
}

TEST_CASE("forward: example graph") {
    using namespace f0;
    AdjacencyGraph<index_t> *G = create_graph_from_file<index_t>(INPUT_DIR "sample_undirected.txt");
    ForwardNeighborContainer<index_t> *A = forward_create_neighbor_container(G);
    for (int i = 0; i < 3; i++) {  // repeat
        auto lister = forward<index_t, index_t , TriangleListing::SetCollect<index_t>>(G, A);
        REQUIRE(lister.triangles.size() == 3);
        REQUIRE(lister.triangles == TriangleListing::SetCollect<index_t>::TriangleSet{{0, 1, 2},
                                         {0, 1, 3},
                                         {0, 3, 4}});
    }
    free_graph(G);
    forward_delete_neighbor_container(A);


    G = create_graph_from_file<index_t>(INPUT_DIR "sample2.txt");
    A = forward_create_neighbor_container(G);
    auto lister = forward<index_t, index_t, TriangleListing::SetCollect<index_t>>(G, A);
    REQUIRE(lister.triangles.size() == 5);
    REQUIRE(lister.triangles == TriangleListing::SetCollect<index_t>::TriangleSet{{0, 1, 2},
                                     {0, 1, 3},
                                     {0, 3, 4},
                                     {0, 5, 6},
                                     {0, 7, 8}});
    free_graph(G);
    forward_delete_neighbor_container(A);
}

TEST_CASE("forward: WWW_NOTRE_DAME graph") {
    using namespace f0;
    std::string generate_graph = "./graph_generation -gt www_notre_dame -o notre.txt -shuffle_edges";
    std::string delete_graph = "rm notre.txt";
    std::ignore = system(generate_graph.c_str());
    AdjacencyGraph<index_t> *graph = create_graph_from_file<index_t>("notre.txt");
    ForwardNeighborContainer<index_t> *A = forward_create_neighbor_container(graph);
    auto triangle_listing = forward<index_t, index_t, TriangleListing::SetCollect<index_t>>(graph, A);
    REQUIRE(triangle_listing.triangles.size() == 8910005);
    free_graph(graph);
    forward_delete_neighbor_container(A);
    std::ignore = system(delete_graph.c_str());
}

TEST_CASE("forward: US Patents graph") {
    using namespace f0;
    std::string generate_graph = "./graph_generation -gt us_patents -o us_patents.txt -shuffle_edges";
    std::string delete_graph = "rm us_patents.txt";
    std::ignore = system(generate_graph.c_str());
    AdjacencyGraph<index_t> *graph = create_graph_from_file<index_t>("us_patents.txt");
    ForwardNeighborContainer<index_t> *A = forward_create_neighbor_container(graph);
    auto triangle_listing = forward<index_t, index_t, TriangleListing::SetCollect<index_t>>(graph, A);
    REQUIRE(triangle_listing.triangles.size() == 7515023);
    free_graph(graph);
    forward_delete_neighbor_container(A);
    std::ignore = system(delete_graph.c_str());
}