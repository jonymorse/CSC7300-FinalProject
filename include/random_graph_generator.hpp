#ifndef RANDOM_GRAPH_GENERATOR_HPP
#define RANDOM_GRAPH_GENERATOR_HPP

#include "graph.hpp"

Graph generateRandomGraph(int num_nodes, int num_edges, int min_weight = 1, int max_weight = 20, int seed = 42);

#endif
