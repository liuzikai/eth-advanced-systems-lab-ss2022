//
// Created by Zikai Liu on 3/5/22.
//

#include <catch.hpp>
#include "AdjacencyGraph.h"
#include "Forward.h"

TEST_CASE("Forward: example graph") {
    // The example graph of Fig. 1, p. 5
    AdjacencyGraph G(5);
    G.addEdge(0, 1);
    G.addEdge(0, 2);
    G.addEdge(0, 3);
    G.addEdge(0, 4);
    G.addEdge(1, 2);
    G.addEdge(1, 3);
    G.addEdge(3, 4);
    REQUIRE(G.edgeCount() == 7);
    REQUIRE(forwardSolve(G) == 3);
}