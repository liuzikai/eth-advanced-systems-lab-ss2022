#include <cstdlib>
#include <iostream>
#include <map>
#include <vector>
#include <ctime>
#include <fstream>
#include <string>
#include <algorithm>
#include <random>
#include <unordered_map>
#include <sstream>

#include "ArgParser.h"

#include "graph_generation.h"
#include "graph_generation_util.h"
#include "../src/common.h"

const static std::map<GraphType, std::string> graph_to_file = {
    {GraphType::GERMAN_ROAD_NETWORK, ""},
    {GraphType::ACTOR_MOVIE_GRAPH, ""},
    {GraphType::COMP_SCIENCE_AUTHORS, ""},
    {GraphType::GOOGLE_CONTEST, "../graphs/graph_data/google_contest.txt.gz"},
    {GraphType::HELP_LITERATURE, ""},
    {GraphType::ROUTER_NETWORK, ""},
    {GraphType::WWW_NOTRE_DAME, ""},
    {GraphType::US_PATENTS, "../graphs/graph_data/cit-Patents.txt.gz"}};

void generate_random_graph(const GraphDefinition &graph_definition, std::ofstream &outfile)
{
    std::vector<std::vector<uint64_t>> adjacency_matrix(graph_definition.nodes, std::vector<uint64_t>(graph_definition.nodes, 0));
    uint64_t src, dst, edges_left = graph_definition.edges;
    while (edges_left > 0)
    {
        src = std::rand() % graph_definition.nodes;
        dst = std::rand() % graph_definition.nodes;
        if (src >= dst || adjacency_matrix[src][dst])
            continue;
        adjacency_matrix[src][dst] = 1;
        adjacency_matrix[src][src] += 1;
        adjacency_matrix[dst][src] = 1;
        adjacency_matrix[dst][dst] += 1;
        edges_left--;
    }
    outfile << graph_definition.nodes << std::endl;
    std::default_random_engine gen(graph_definition.random_seed);
    for (uint64_t i = 0; i < graph_definition.nodes; i++)
    {
        outfile << adjacency_matrix[i][i] << " ";
        //get all indecies of non-zero elements in row i of adjacency_matrix
        std::vector<uint64_t> non_zero_indecies;
        adjacency_matrix[i][i] = 0;
        for (uint64_t j = 0; j < graph_definition.nodes; j++)
        {
            if (adjacency_matrix[i][j]) {
                non_zero_indecies.push_back(j);
            }
        }

        //shuffle non_zero_indecies if shuffle_edges is true
        if (graph_definition.shuffle_edges) {
            std::shuffle(non_zero_indecies.begin(), non_zero_indecies.end(), gen);
        }

        for (uint64_t j = 0; j < non_zero_indecies.size(); j++)
        {
            outfile << non_zero_indecies[j] << " ";
        }
        outfile << std::endl;
    }
}


uint64_t num_nodes_snap_stanford_ds(std::ifstream &infile) {
    std::string line;
    while (line.find("Nodes") == std::string::npos)
    {
        std::getline(infile, line);
    }
    //read in the number between "Nodes: " and "Edges: " in line
    uint64_t nodes = std::stoull(line.substr(line.find("Nodes: ") + 7, line.find(" Edges: ") - line.find("Nodes: ") - 7));
    return nodes;
}

void generate_graph_from_snap_stanford_ds(const GraphDefinition &graph_definition, std::ifstream &infile, std::ofstream &outfile)
{
    std::string line;

    uint64_t nodes = num_nodes_snap_stanford_ds(infile);

    std::vector<std::vector<uint64_t>> adjacency_list(nodes, std::vector<uint64_t>());

    //skip a line to get to the Data Section
    std::getline(infile, line);
    
    uint64_t edges = parse_edge_list(infile, adjacency_list);
    std::cout << "Nodes: " << nodes << " Edges: " << edges << std::endl;

    output_adjacency_list(outfile, adjacency_list, graph_definition.shuffle_edges, graph_definition.random_seed);
}

// Output Format:
// num_nodes
// for each node:
//  num_neighbours, neighbour 1, neighbour 2, etc.
void generate_graph(const GraphDefinition &graph_definition)
{
    //print the seed
    std::srand(graph_definition.random_seed);
    std::ofstream outfile(graph_definition.output_filename);
    //convert graphType to file name using graph_to_file map and the default value "" if the graphType is not in the map
    auto search = graph_to_file.find(graph_definition.graphType);
    std::string zipped_file_name = (search != graph_to_file.end()) ? search->second : "";

    if (graph_definition.graphType != GraphType::GENERATED && zipped_file_name.empty()) {
        std::cerr << "Graph type currently not supported." << std::endl;
        exit(1);
    }

    std::string file_name = unzip_file(zipped_file_name);
    // open file_name
    std::ifstream infile(file_name);

    std::cout << "Generating " << zipped_file_name << std::endl;

    switch (graph_definition.graphType)
    {
    case GraphType::GERMAN_ROAD_NETWORK:
        break;
    case GraphType::ACTOR_MOVIE_GRAPH:
        break;
    case GraphType::COMP_SCIENCE_AUTHORS:
        break;
    case GraphType::GOOGLE_CONTEST:
        generate_graph_from_snap_stanford_ds(graph_definition, infile, outfile);
        break;
    case GraphType::HELP_LITERATURE:
        break;
    case GraphType::ROUTER_NETWORK:
        break;
    case GraphType::WWW_NOTRE_DAME:
        break;
    case GraphType::US_PATENTS:
        generate_graph_from_snap_stanford_ds(graph_definition, infile, outfile);
        break;
    case GraphType::GENERATED:
        switch (graph_definition.density)
        {
        case Density::SPARSE:
            break;
        case Density::DENSE:
            break;
        case Density::NONE:
            generate_random_graph(graph_definition, outfile);
            break;
        }
        break;
    }
}

GraphDefinition parse_arguments(ArgParser &parser)
{
    GraphDefinition graph;

    graph.graphType = string_to_graphType(std::string{parser.getCmdOption("-gt").value_or("GENERATED")});
    graph.nodes = parser.getCmdOptionAsInt("-num_nodes").value_or(100);
    graph.edges = parser.getCmdOptionAsInt("-num_edges").value_or(1000);
    graph.density = string_to_density(std::string{parser.getCmdOption("-density").value_or("NONE")});
    graph.hdng = string_to_highDegreeNodeGeneration(std::string{parser.getCmdOption("-hdng").value_or("NONE")});
    graph.random_seed = parser.getCmdOptionAsInt("-seed").value_or(std::time(nullptr));
    graph.shuffle_edges = parser.cmdOptionExists("-shuffle_edges");
    graph.output_filename = parser.getCmdOption("-o").value_or("graph.txt");

    return graph;
}

int main(int argc, char *argv[])
{
    ArgParser parser(argc, argv);
    std::cout << "Generating graph..." << std::endl;
    GraphDefinition graph = parse_arguments(parser);
    generate_graph(graph);
    std::cout << "Graph generated." << std::endl;
}