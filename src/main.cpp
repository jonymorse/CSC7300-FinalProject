#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <chrono> // For timing
#include "graph.hpp"
#include "random_graph_generator.hpp"
#include "vertex_disjoint_paths.hpp"
#include "bruteforce.hpp"
#include "tests.hpp"

// Function to load a DIMACS graph file into our Graph class
Graph loadDIMACSGraph(const std::string &filename) {
    std::ifstream infile(filename);
    if (!infile) {
        std::cerr << "Error opening file " << filename << "\n";
        return Graph(0);
    }

    std::string line;
    int num_nodes = 0;
    int num_edges = 0;

    // Parse the problem line (p sp N M)
    while (std::getline(infile, line)) {
        if (line.empty() || line[0] == 'c') {
            continue; // skip comment lines
        } else if (line[0] == 'p') {
            // Format: p sp <num_nodes> <num_edges>
            char p_label;
            std::string sp_label;
            std::istringstream iss(line);
            iss >> p_label >> sp_label >> num_nodes >> num_edges;
            break;
        }
    }

    Graph g(num_nodes);

    // Now read the edges
    while (std::getline(infile, line)) {
        if (line.empty() || line[0] == 'c') continue;
        if (line[0] == 'a') {
            // Format: a u v w
            char a_label;
            int u, v, w;
            std::istringstream iss(line);
            iss >> a_label >> u >> v >> w;
            g.addEdge(u, v, w);
        }
    }

    g.buildAdjacencyList();
    return g;
}

int main() {
    std::cout << "Select input mode:\n";
    std::cout << "1) Generate random graph\n";
    std::cout << "2) Load Rome99 graph from data/Rome99.gr\n";
    std::cout << "3) Run brute force on a tiny random graph\n";
    std::cout << "4) Run comparison tests (Brute Force vs Soph.)\n";

    std::cout << "Enter choice (1, 2, 3, or 4): ";

    int choice;
    std::cin >> choice;

    Graph g(0);

    // Start timing graph construction/loading
    auto start_time = std::chrono::high_resolution_clock::now();

    if (choice == 1) {
        // Ask user for graph parameters
        int num_nodes, num_edges;
        std::cout << "Enter number of nodes: ";
        std::cin >> num_nodes;
        std::cout << "Enter number of edges: ";
        std::cin >> num_edges;

        g = generateRandomGraph(num_nodes, num_edges);
        g.buildAdjacencyList();
        std::cout << "\nUsing random graph with " << num_nodes << " nodes and " << num_edges << " edges.\n";
    } else if (choice == 2) {
        // Load Rome99 graph
        std::string input_file = "data/Rome99.gr";
        g = loadDIMACSGraph(input_file);
        std::cout << "\nLoaded Rome99 graph from " << input_file << ".\n";
    } else if (choice == 3) {
        Graph tiny = generateRandomGraph(5, 10);
        tiny.buildAdjacencyList();
        tiny.printInfo();

        int src = 1, tgt = 5;
        auto [bf_p1, bf_p2] = bruteForceFindVertexDisjointPaths(tiny, src, tgt);
        if (!bf_p1.empty() && !bf_p2.empty()) {
            std::cout << "Brute force found two paths:\n";
            // Optional: print them if needed
        } else {
            std::cout << "No two vertex-disjoint paths found by brute force.\n";
        }

        // Now assign g = tiny so that the rest of the code uses this graph
        g = tiny;
    } else if (choice == 4) {
        // Call our new test function
        runComparisonTests();
        return 0; // After running tests, we can exit or continue as needed
    } else {
        std::cerr << "Invalid choice.\n";
        return 1;
    }

    // End timing for graph generation/loading
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> graph_load_duration = end_time - start_time;
    std::cout << "Graph construction/loading took " << graph_load_duration.count() << " seconds.\n";

    // Print graph info
    g.printInfo();

    // Ask for source and target
    int source, target;
    std::cout << "\nEnter source node: ";
    std::cin >> source;
    std::cout << "Enter target node: ";
    std::cin >> target;

    // Start timing for finding vertex-disjoint paths
    start_time = std::chrono::high_resolution_clock::now();
    auto [path1, path2] = findVertexDisjointPaths(g, source, target);
    end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> vd_paths_duration = end_time - start_time;
    std::cout << "Finding vertex-disjoint paths took " << vd_paths_duration.count() << " seconds.\n";

    if (!path1.empty() && !path2.empty()) {
        int cost1 = calculatePathCost(g, path1);
        int cost2 = calculatePathCost(g, path2);
        std::cout << "\nFound two vertex-disjoint paths:\n";
        std::cout << "Path 1 (Blue): ";
        for (size_t i = 0; i < path1.size(); i++) {
            std::cout << path1[i] << (i+1<path1.size() ? " -> " : "");
        }
        std::cout << "\nCost of Path 1: " << cost1 << "\n";

        std::cout << "Path 2 (Red): ";
        for (size_t i = 0; i < path2.size(); i++) {
            std::cout << path2[i] << (i+1<path2.size() ? " -> " : "");
        }
        std::cout << "\nCost of Path 2: " << cost2 << "\n";
        std::cout << "Total Cost: " << cost1 + cost2 << "\n";

        // Highlight both paths in the DOT file
        std::string output_dot = "data/graph.dot";
        g.saveDOTWithTwoPaths(output_dot, path1, path2);

        std::cout << "\nDOT file generated: " << output_dot << "\n";
        std::cout << "To visualize:\n";
        std::cout << "1) Run: dot -Tpng " << output_dot << " -o data/graph.png\n";
        std::cout << "2) open data/graph.png\n";

    } else {
        std::cout << "\nNo vertex-disjoint paths found from " << source << " to " << target << "\n";
    }

    // Optionally: Run multiple times or with different sizes, record output to file, etc.
    // You could also print results in a CSV format for easy import into a spreadsheet.

    return 0;
}
