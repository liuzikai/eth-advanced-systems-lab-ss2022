//
// Created by Zikai Liu on 3/5/22.
//

#include "AdjacencyGraph.h"
#include <iostream>
#include <fstream>
#include <algorithm>

// replace vector, class in C?

AdjacencyGraph::AdjacencyGraph(Index n) : n(n), m(0), adjNodes_(n, std::vector<Index>()) {}

void AdjacencyGraph::addEdge(Index u, Index v) {
    if (u > v) std::swap(u, v);
    adjNodes_[u].emplace_back(v);
    m++;
}

AdjacencyGraph::AdjacencyGraph(const std::string &filename) {
    std::ios::sync_with_stdio(false);
    std::ifstream fin(filename);

    fin >> n >> m;

    adjNodes_ = std::vector(n, std::vector<Index>());  // initialize empty vectors

    for (Index i = 0; i < m; i++) {
        Index u, v;
        fin >> u >> v;
        // implement swap?
        if (u > v) std::swap(u, v);
        adjNodes_[u].emplace_back(v);
    }

    fin.close();
}

void AdjacencyGraph::sortAdjNodes() {
    for (auto &v : adjNodes_) {
        // implement sort
        std::sort(v.begin(), v.end());
    }
}
