#include "tests.hpp"
#include "random_graph_generator.hpp"
#include "vertex_disjoint_paths.hpp"
#include "bruteforce.hpp"
#include <iostream>
#include <chrono>
#include <vector>
#include <utility>

template <typename Func>
double measureTime(Func f) {
    auto start = std::chrono::high_resolution_clock::now();
    (void) f(); // Execute the function, discard result if we don't need it here
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end - start;
    return diff.count();
}

void runComparisonTests() {
    std::cout << "\nRunning Comparison Tests (Brute Force vs Sophisticated) on both small and large graphs.\n";
    std::cout << "Note: Running brute force on large graphs may be extremely slow!\n\n";

    // Configurations for testing:
    std::vector<std::pair<int,int>> configs = {
        // Small graphs where brute force is feasible
        {5, 10},
        {6, 15},
        {10, 30},

        // Medium to large graphs
        {50, 150},
        {100, 300},
        {200, 600},
        // You can go larger, but be warned brute force may never finish in reasonable time
        // {500, 1500},
        // {1000, 3000},
    };

    std::cout << "Nodes | Edges | BF Time(s) | Soph. Time(s) | Found 2 paths?\n";
    std::cout << "-------------------------------------------------------------\n";

    int source = 1, target = 5;

    for (auto &cfg : configs) {
        int num_nodes = cfg.first;
        int num_edges = cfg.second;

        // Generate graph
        Graph g = generateRandomGraph(num_nodes, num_edges);
        g.buildAdjacencyList();
        if (target > num_nodes) { target = num_nodes; }

        // Measure brute force
        std::cout << "Testing " << num_nodes << " nodes, " << num_edges << " edges...\n";
        std::cout << "Running brute force...\n";
        double bf_time = measureTime([&](){
            (void) bruteForceFindVertexDisjointPaths(g, source, target);
        });
        auto [p1_bf, p2_bf] = bruteForceFindVertexDisjointPaths(g, source, target);
        bool bf_found = (!p1_bf.empty() && !p2_bf.empty());

        std::cout << "Brute force done.\nRunning sophisticated approach...\n";

        // Measure sophisticated approach
        double soph_time = measureTime([&](){
            (void) findVertexDisjointPaths(g, source, target);
        });
        auto [p1_soph, p2_soph] = findVertexDisjointPaths(g, source, target);
        bool soph_found = (!p1_soph.empty() && !p2_soph.empty());

        std::string status;
        if (bf_found && soph_found) status = "Yes";
        else if (bf_found && !soph_found) status = "BF only";
        else if (!bf_found && soph_found) status = "Soph only";
        else status = "No";

        std::cout << num_nodes << "     | " << num_edges << "     | "
                  << bf_time << "    | "
                  << soph_time << "       | "
                  << status << "\n";
    }

    std::cout << "-------------------------------------------------------------\n";
    std::cout << "Note: On large graphs, brute force might take extremely long or appear to hang.\n"
              << "Consider stopping the program if it doesn't finish in a reasonable time.\n";
}
