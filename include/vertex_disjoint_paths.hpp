#ifndef UTIL_HPP
#define UTIL_HPP

#include <vector>
#include "graph.hpp"

// Existing declarations for dijkstra
std::pair<std::vector<int>, std::vector<int>> dijkstra(const Graph &g, int source);
std::vector<int> getShortestPath(const std::vector<int> &predecessors, int source, int target);

// New declarations
std::pair<std::vector<int>, std::vector<int>> findVertexDisjointPaths(const Graph &g, int source, int target);
int calculatePathCost(const Graph &g, const std::vector<int> &path);

#endif
