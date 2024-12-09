#include "random_graph_generator.hpp"
#include <random>
#include <unordered_set>
#include <utility>
#include <functional>

Graph generateRandomGraph(int num_nodes, int num_edges, int min_weight, int max_weight, int seed) {
    Graph g(num_nodes);
    std::mt19937 gen(seed);
    std::uniform_int_distribution<> node_dist(1, num_nodes);
    std::uniform_int_distribution<> weight_dist(min_weight, max_weight);

    auto pair_hash = [](const std::pair<int,int> &p){
        return std::hash<int>()(p.first) ^ (std::hash<int>()(p.second) << 1);
    };
    std::unordered_set<std::pair<int,int>, decltype(pair_hash)> edge_set(0, pair_hash);

    int attempts = 0;
    int max_attempts = num_edges * 10;

    while (g.numEdges() < num_edges && attempts < max_attempts) {
        int u = node_dist(gen);
        int v = node_dist(gen);
        if (u != v && edge_set.find({u,v}) == edge_set.end()) {
            int w = weight_dist(gen);
            g.addEdge(u, v, w);
            edge_set.insert({u,v});
        }
        attempts++;
    }

    return g;
}
