//
// Created by Zikai Liu on 3/5/22.
//

#include "AdjacencyGraph.h"
#include <vector>
#include <iostream>

/**
 * List triangles in the input graph using the forward algorithm (Algorithm 1, p.5)
 * @param G   Input graph. Pass-by-value since the adjacency lists need to be sorted.
 * @return    Number of triangles.
 * @note      AdjacencyGraph stores each edge {s, t} only once with s < t. The if statement is eliminated.
 */
Index forwardSolve(AdjacencyGraph G) {
    G.sortAdjNodes();  // according to sec. 4, the sorting is included in the execution time

    Index count = 0;

    std::vector<std::vector<Index>> A(G.nodeCount(), std::vector<Index>());
    // A will be constructed with increasing node Indices

    for (Index s = 0; s < G.nodeCount(); s++) {
        for (Index t : G.adjNodes(s)) {
            // AdjacentGraph guaranteed s < t, the if statement is eliminated
            assert(s < t && "Forward: s < t is not satisfied from AdjacentGraph");

            size_t i = 0, j = 0;
            const auto As = A[s];
            auto &At = A[t];
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

            At.emplace_back(s);
        }
    }

    return count;
}