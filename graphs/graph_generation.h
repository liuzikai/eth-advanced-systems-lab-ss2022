#ifndef TEAM02_GRAPH_GENERATION_H
#define TEAM02_GRAPH_GENERATION_H

#include "common.h"
#include "adjacency_graph.h"
#include <string>
#include <ctime>
#include <algorithm>

void convert_to_upper(std::string &s) {
    std::transform(s.begin(), s.end(), s.begin(), ::toupper);
}

enum class GraphType {
    GERMAN_ROAD_NETWORK,
    ACTOR_MOVIE_GRAPH,
    COMP_SCIENCE_AUTHORS,
    GOOGLE_CONTEST,
    HEP_LITERATURE,
    ROUTER_NETWORK,
    WWW_NOTRE_DAME,
    US_PATENTS,
    GENERATED
};

GraphType string_to_graphType(std::string s) {
    convert_to_upper(s);
    if (s == "GERMAN_ROAD_NETWORK") {
        return GraphType::GERMAN_ROAD_NETWORK;
    } else if (s == "ACTOR_MOVIE_GRAPH") {
        return GraphType::ACTOR_MOVIE_GRAPH;
    } else if (s == "COMP_SCIENCE_AUTHORS") {
        return GraphType::COMP_SCIENCE_AUTHORS;
    } else if (s == "GOOGLE_CONTEST") {
        return GraphType::GOOGLE_CONTEST;
    } else if (s == "HEP_LITERATURE") {
        return GraphType::HEP_LITERATURE;
    } else if (s == "ROUTER_NETWORK") {
        return GraphType::ROUTER_NETWORK;
    } else if (s == "WWW_NOTRE_DAME") {
        return GraphType::WWW_NOTRE_DAME;
    } else if (s == "US_PATENTS") {
        return GraphType::US_PATENTS;
    } else if (s == "GENERATED") {
        return GraphType::GENERATED;
    } else {
        throw std::invalid_argument("Invalid GraphType");
    }
}

enum class Density {
    NONE,
    SPARSE,
    EXIST,
    DENSE
};

Density string_to_density(std::string s) {
    convert_to_upper(s);
    if (s == "NONE") {
        return Density::NONE;
    } else if (s == "SPARSE") {
        return Density::SPARSE;
    } else if (s == "DENSE") {
        return Density::DENSE;
    } else if (s == "EXIST") {
        return Density::EXIST;
    } else {
        throw std::invalid_argument("Invalid Density");
    }
}

enum class HighDegreeNodeGeneration {
    NONE,
    NATURAL_LOG,
    SQRT
};

HighDegreeNodeGeneration string_to_highDegreeNodeGeneration(std::string s) {
    convert_to_upper(s);
    if (s == "NONE") {
        return HighDegreeNodeGeneration::NONE;
    } else if (s == "NATURAL_LOG") {
        return HighDegreeNodeGeneration::NATURAL_LOG;
    } else if (s == "SQRT") {
        return HighDegreeNodeGeneration::SQRT;
    } else {
        throw std::invalid_argument("Invalid HighDegreeNodeGeneration");
    }
}

struct GraphDefinition {
    GraphType graphType;
    Density density;
    HighDegreeNodeGeneration hdng;
    std::string output_filename;
    uint64_t random_seed;
    uint64_t nodes;
    uint64_t edges;
    bool shuffle_edges;

    GraphDefinition(GraphType gT = GraphType::GENERATED, std::string out = std::string("generated_graph.txt")) {
        graphType = gT;
        if(gT == GraphType::GENERATED) {
            density = Density::SPARSE;
            hdng = HighDegreeNodeGeneration::NONE;
            nodes = 100;
            edges = 1000;
            shuffle_edges = true;
        }
        output_filename = out;
    }
    GraphDefinition(GraphType gT, Density dense, HighDegreeNodeGeneration degree, uint64_t m, uint64_t n = 0, bool shuffle = true, uint64_t seed = static_cast<uint64_t>(std::time(0)), std::string out = std::string("generated_graph.txt")) {
        graphType = gT;
        density = dense;
        hdng = degree;
        nodes = n;
        edges = m;
        shuffle_edges = shuffle;
        random_seed = seed;
        output_filename = out;
    }
};


/**
 * Generate a graph and store to file.
 * @param graph_defintion  The metadata needed to generate the graph.
 */
void generate_graph(const GraphDefinition &graph_defintion);

#endif //TEAM02_GRAPH_GENERATION_H
