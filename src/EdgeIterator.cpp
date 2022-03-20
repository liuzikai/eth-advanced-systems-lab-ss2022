//
// Created by JZ on 20.03.22.
//

#include "EdgeIterator.h"
#include "AdjacencyGraph.h"
#include <vector>
#include <iostream>


/**
 * List triangles in the input graph using the edge iterator algorithm (Algorithm 1, p.4)
 * @param G   Input graph. Pass-by-value since the adjacency lists need to be sorted.
 * @return    Number of triangles.
 * @note      AdjacencyGraph stores each edge {s, t} only once with s < t. The if statement is eliminated.
 */
Index edgeIteratorSolve(AdjacencyGraph G) {
    G.sortAdjNodes();  // according to sec. 4, the sorting is included in the execution time

    Index count = 0;

    // Iterate over the edges
    for (Index s = 0; s < G.nodeCount(); s++) {
        for (Index t : G.adjNodes(s)) {
            // AdjacentGraph guaranteed s < t, the if statement is eliminated
            assert(s < t && "Forward: s < t is not satisfied from AdjacentGraph");

            size_t i = 0, j = 0;
            const auto As = G.adjNodes(s);
            const auto At = G.adjNodes(t);
            while (i < As.size() && j < At.size()) {
                if (As[i] == At[j]) {
                    count++;
                    // TODO: [Zikai 2022.03.05] Listing is not implemented yet (use a shared infrastructure?)
                    i++;
                    j++;
                } else if (As[i] < At[j]) {
                    i++;
                } else {
                    j++;
                }
            }
        }
    }

    return count;
}
