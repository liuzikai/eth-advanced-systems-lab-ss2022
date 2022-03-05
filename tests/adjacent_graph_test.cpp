//
// Created by Zikai Liu on 3/5/22.
//

#include <catch.hpp>
#include "AdjacencyGraph.h"

TEST_CASE("AdjacentGraph: empty") {
    AdjacencyGraph G(0);
    REQUIRE(G.nodeCount() == 0);
}

TEST_CASE("AdjacentGraph: in-order node indices") {
    AdjacencyGraph G(2);
    REQUIRE(G.nodeCount() == 2);
    REQUIRE(G.edgeCount() == 0);
    G.addEdge(1, 0);  // AdjacencyGraph should swap the indices
    REQUIRE(G.edgeCount() == 1);
    REQUIRE(G.adjNodes(0).size() == 1);
    REQUIRE(G.adjNodes(0)[0] == 1);
    REQUIRE(G.adjNodes(1).empty());
}

// TODO: [Zikai 2022.03.05] unit test AdjacencyGraph(filename)?