#ifndef BRUTEFORCE_HPP
#define BRUTEFORCE_HPP

#include "graph.hpp"
#include <vector>
#include <utility>

// Returns a pair of paths (each a vector of ints) if found, or empty vectors if none.
// This brute force approach is only meant for very small graphs.
std::pair<std::vector<int>, std::vector<int>> bruteForceFindVertexDisjointPaths(const Graph &g, int source, int target);

#endif
