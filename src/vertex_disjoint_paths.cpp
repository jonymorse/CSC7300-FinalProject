#include "vertex_disjoint_paths.hpp"
#include <queue>
#include <functional>
#include <limits>
#include <algorithm> // for std::find
#include <unordered_set>
// Dijkstra using a min-heap (priority_queue)
std::pair<std::vector<int>, std::vector<int>> dijkstra(const Graph &g, int source) {
    int n = g.numNodes();
    std::vector<int> dist(n+1, std::numeric_limits<int>::max());
    std::vector<int> pred(n+1, -1);
    dist[source] = 0;

    // min-heap of (distance, node)
    std::priority_queue<std::pair<int,int>, 
                        std::vector<std::pair<int,int>>,
                        std::greater<>> pq;
    pq.push({0, source});

    const auto &adj = g.adjacencyList();

    while (!pq.empty()) {
        auto [current_distance, u] = pq.top();
        pq.pop();

        if (current_distance > dist[u]) continue;

        // Explore neighbors
        for (auto &edge : adj[u]) {
            int v = edge.v;
            int w = edge.weight;
            int nd = current_distance + w;
            if (nd < dist[v]) {
                dist[v] = nd;
                pred[v] = u;
                pq.push({nd, v});
            }
        }
    }

    return {dist, pred};
}

std::vector<int> getShortestPath(const std::vector<int> &predecessors, int source, int target) {
    if (predecessors[target] == -1 && target != source) {
        // No path
        return {};
    }

    std::vector<int> path;
    int current = target;
    while (current != -1) {
        path.push_back(current);
        current = predecessors[current];
    }
    std::reverse(path.begin(), path.end());
    return path;
}
static Graph createModifiedGraph(const Graph &g, 
                                 const std::vector<int> &path1, 
                                 int source, int target) {
    // Internal vertices are all except the first and last in path1
    std::vector<int> internalVertices(path1.begin() + 1, path1.end() - 1);
    
    // Convert internal vertices to a set for quick lookup
    std::unordered_set<int> internalSet(internalVertices.begin(), internalVertices.end());

    // Edges on path1
    std::vector<std::pair<int,int>> path1_edges;
    for (size_t i = 0; i + 1 < path1.size(); i++) {
        path1_edges.push_back({path1[i], path1[i+1]});
    }

    // Create a new graph with same number of nodes
    Graph modified(g.numNodes());

    // Add edges from original if not removed
    // We'll rebuild adjacency after
    for (auto &e : g.allEdges()) {
        if (internalSet.find(e.u) != internalSet.end() || 
            internalSet.find(e.v) != internalSet.end()) {
            // Edge involves internal vertex, skip
            continue;
        }

        bool on_path1 = false;
        for (auto &p : path1_edges) {
            if (p.first == e.u && p.second == e.v) {
                on_path1 = true;
                break;
            }
        }

        if (!on_path1) {
            // This edge is allowed
            modified.addEdge(e.u, e.v, e.weight);
        }
    }

    modified.buildAdjacencyList();
    return modified;
}

std::pair<std::vector<int>, std::vector<int>> findVertexDisjointPaths(const Graph &g, int source, int target) {
    // First shortest path
    auto [dist1, pred1] = dijkstra(g, source);
    if (dist1[target] == std::numeric_limits<int>::max()) {
        // No path
        return {{}, {}};
    }

    std::vector<int> path1 = getShortestPath(pred1, source, target);
    if (path1.empty()) {
        return {{}, {}};
    }

    // Create modified graph removing internal vertices and path1 edges
    Graph modified = createModifiedGraph(g, path1, source, target);

    // Find second path
    auto [dist2, pred2] = dijkstra(modified, source);
    if (dist2[target] == std::numeric_limits<int>::max()) {
        // No second path
        return {{}, {}};
    }

    std::vector<int> path2 = getShortestPath(pred2, source, target);
    if (path2.empty()) {
        return {{}, {}};
    }

    // If somehow path2 is the same as path1, discard
    if (path1 == path2) {
        return {{}, {}};
    }

    return {path1, path2};
}

int calculatePathCost(const Graph &g, const std::vector<int> &path) {
    if (path.size() < 2) return 0;
    int cost = 0;
    // We can look up edge weights by adjacency or by scanning edges.
    // Let's use adjacency for efficiency.
    const auto &adj = g.adjacencyList();
    for (size_t i = 0; i + 1 < path.size(); i++) {
        int u = path[i];
        int v = path[i+1];
        // Find edge (u->v)
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
}
