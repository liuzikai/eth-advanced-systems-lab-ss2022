#ifndef TEAM02_GRAPH_GENERATION_H
#define TEAM02_GRAPH_GENERATION_H

#include "../src/common.h"
#include <string>
#include <ctime>

enum class GraphType {
    GERMAN_ROAD_NETWORK,
    ACTOR_MOVIE_GRAPH,
    COMP_SCIENCE_AUTHORS,
    GOOGLE_CONTEST,
    HELP_LITERATURE,
    ROUTER_NETWORK,
    WWW_NOTRE_DAME,
    US_PATENTS,
    GENERATED
};

enum class Density {
    NONE,
    SPARSE,
    DENSE
};

enum class HighDegreeNodeGeneration {
    NONE,
    NATURAL_LOG,
    SQRT
};

struct GraphDefinition {
    GraphType graphType;
    Density density;
    HighDegreeNodeGeneration hdng;
    std::string output_filename;
    index_t random_seed;
    index_t nodes;
    index_t edges;

    GraphDefinition(GraphType gT, std::string out = std::string("generated_graph.txt")) {
        graphType = gT;
        if(gT == GraphType::GENERATED) {
            density = Density::SPARSE;
            hdng = HighDegreeNodeGeneration::NONE;
            nodes = 100;
            edges = 1000;
        }
        output_filename = out;
    }
    GraphDefinition(GraphType gT, Density dense, HighDegreeNodeGeneration degree, index_t m, index_t n = 0, index_t seed = static_cast<index_t>(std::time(0)), std::string out = std::string("generated_graph.txt")) {
        graphType = gT;
        density = dense;
        hdng = degree;
        nodes = n;
        edges = m;
        random_seed = seed;
        output_filename = out;
    }
};


/**
 * Generate a graph and store to file.
 * @param graph_defintion  The metadata needed to generate the graph.
 */
void generate_graph(GraphDefinition graph_defintion);

#endif //TEAM02_GRAPH_GENERATION_H
