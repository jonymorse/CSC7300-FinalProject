#include "bruteforce.hpp"
#include <algorithm>
#include <limits>

// Helper function: find all paths from source to target using a backtracking DFS.
// WARNING: For non-trivial graphs, this will explode combinatorially!
static void allPathsDFS(const Graph &g, int current, int target, std::vector<bool> &visited, 
                        std::vector<int> &path, std::vector<std::vector<int>> &all_paths) {
    if (current == target) {
        all_paths.push_back(path);
        return;
    }

    const auto &adj = g.adjacencyList();
    for (auto &edge : adj[current]) {
        int v = edge.v;
        if (!visited[v]) {
            visited[v] = true;
            path.push_back(v);
            allPathsDFS(g, v, target, visited, path, all_paths);
            path.pop_back();
            visited[v] = false;
        }
    }
}

std::pair<std::vector<int>, std::vector<int>> bruteForceFindVertexDisjointPaths(const Graph &g, int source, int target) {
    int n = g.numNodes();
    if (source < 1 || source > n || target < 1 || target > n) {
        return {{}, {}};
    }

    // Find all paths from source to target
    std::vector<std::vector<int>> all_paths;
    {
        std::vector<bool> visited(n+1, false);
        visited[source] = true;
        std::vector<int> path;
        path.push_back(source);
        allPathsDFS(g, source, target, visited, path, all_paths);
    }

    // Now try all pairs of paths and check vertex-disjoint property
    // and keep track of best total cost
    auto calculateCost = [&](const std::vector<int> &p) {
        const auto &adj = g.adjacencyList();
        int cost = 0;
        for (size_t i = 0; i + 1 < p.size(); i++) {
            int u = p[i];
            int v = p[i+1];
            int w = std::numeric_limits<int>::max();
            for (auto &ed : adj[u]) {
                if (ed.v == v) {
                    w = ed.weight;
                    break;
                }
            }
            cost += w;
        }
        return cost;
    };

    int best_cost = std::numeric_limits<int>::max();
    std::pair<std::vector<int>, std::vector<int>> best_pair;

    for (size_t i = 0; i < all_paths.size(); i++) {
        for (size_t j = i+1; j < all_paths.size(); j++) {
            const auto &p1 = all_paths[i];
            const auto &p2 = all_paths[j];

            // Check vertex-disjoint: Except source and target, no vertex should appear in both
            std::vector<int> p1_internal(p1.begin()+1, p1.end()-1);
            std::vector<int> p2_internal(p2.begin()+1, p2.end()-1);

            // Sort or use a set for intersection check
            std::sort(p1_internal.begin(), p1_internal.end());
            std::sort(p2_internal.begin(), p2_internal.end());
            std::vector<int> intersection;
            std::set_intersection(p1_internal.begin(), p1_internal.end(), 
                                  p2_internal.begin(), p2_internal.end(), 
                                  std::back_inserter(intersection));
            if (!intersection.empty()) {
                // Not disjoint
                continue;
            }

            int cost1 = calculateCost(p1);
            int cost2 = calculateCost(p2);
            int total = cost1 + cost2;
            if (total < best_cost) {
                best_cost = total;
                best_pair = {p1, p2};
            }
        }
    }

    return best_pair;
}
