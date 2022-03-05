//
// Created by Zikai Liu on 3/5/22.
//

#ifndef TEAM02_ADJACENCYGRAPH_H
#define TEAM02_ADJACENCYGRAPH_H

#include "Common.h"
#include <string>
#include <vector>

class AdjacencyGraph {
public:

    /**
     * Construct a graph with n nodes and no edge
     * @param n   The number of nodes.
     */
    explicit AdjacencyGraph(Index n);

    void addEdge(Index u, Index v);

    /**
     * Construct a graph from an input file, in the format of n, m, lists of edges in {u, v} indexed from 0, separated
     * by spaces.
     * @param filename    The input file.
     */
    explicit AdjacencyGraph(const std::string &filename);

    Index nodeCount() const { return n; }

    Index edgeCount() const { return m; }

    const std::vector<Index> &adjNodes(Index node) const { return adjNodes_[node]; }

    void sortAdjNodes();

private:

    Index n, m;
    std::vector<std::vector<Index>> adjNodes_;

};


#endif //TEAM02_ADJACENCYGRAPH_H
