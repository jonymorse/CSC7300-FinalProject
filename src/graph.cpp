#include "graph.hpp"
#include <fstream>
#include <iostream>

Graph::Graph(int num_nodes) : m_num_nodes(num_nodes) {
    m_adjacency.resize(num_nodes + 1); // indexing nodes from 1 to num_nodes
}

void Graph::addEdge(int u, int v, int weight) {
    m_edges.push_back({u, v, weight});
}

int Graph::numNodes() const {
    return m_num_nodes;
}

int Graph::numEdges() const {
    return (int)m_edges.size();
}

void Graph::saveDIMACS(const std::string &filename) const {
    std::ofstream out(filename);
    if(!out) {
        std::cerr << "Error opening file " << filename << "\n";
        return;
    }
    out << "p sp " << m_num_nodes << " " << numEdges() << "\n";
    for (auto &e : m_edges) {
        out << "a " << e.u << " " << e.v << " " << e.weight << "\n";
    }
}

void Graph::printInfo() const {
    std::cout << "Graph Information:\n";
    std::cout << "Number of nodes: " << m_num_nodes << "\n";
    std::cout << "Number of edges: " << numEdges() << "\n\n";
    std::cout << "Edge list:\n";
    for (auto &e : m_edges) {
        std::cout << "Edge from " << e.u << " to " << e.v 
                  << " with weight " << e.weight << "\n";
    }
}

void Graph::buildAdjacencyList() {
    // Clear existing adjacency (if any)
    for (auto &adj : m_adjacency) {
        adj.clear();
    }

    // Build adjacency from edges
    for (auto &e : m_edges) {
        m_adjacency[e.u].push_back(e);
    }
}

// If a path is provided, highlight those edges
void Graph::saveDOT(const std::string &filename, const std::vector<int> &path) const {
    std::ofstream dot(filename);
    if(!dot) {
        std::cerr << "Error opening file " << filename << "\n";
        return;
    }
    dot << "digraph G {\n";
    for (int i = 1; i <= m_num_nodes; i++) {
        dot << "    " << i << ";\n";
    }

    // Create a set of edges that are on the shortest path for highlighting
    std::vector<std::pair<int,int>> path_edges;
    for (size_t i = 0; i + 1 < path.size(); i++) {
        path_edges.push_back({path[i], path[i+1]});
    }

    // We'll highlight path edges in red
    for (auto &e : m_edges) {
        bool on_path = false;
        for (auto &pe : path_edges) {
            if (pe.first == e.u && pe.second == e.v) {
                on_path = true;
                break;
            }
        }

        if (on_path) {
            dot << "    " << e.u << " -> " << e.v << " [label=\"" << e.weight 
                << "\", color=\"red\", penwidth=2.0];\n";
        } else {
            dot << "    " << e.u << " -> " << e.v << " [label=\"" << e.weight << "\"];\n";
        }
    }
    dot << "}\n";
}


void Graph::saveDOTWithTwoPaths(const std::string &filename, 
                                const std::vector<int> &path1, 
                                const std::vector<int> &path2) const {
    std::ofstream dot(filename);
    if(!dot) {
        std::cerr << "Error opening file " << filename << "\n";
        return;
    }
    dot << "digraph G {\n";
    for (int i = 1; i <= m_num_nodes; i++) {
        dot << "    " << i << ";\n";
    }

    // Convert paths to sets of edges
    std::vector<std::pair<int,int>> p1_edges, p2_edges;
    for (size_t i = 0; i + 1 < path1.size(); i++) {
        p1_edges.push_back({path1[i], path1[i+1]});
    }
    for (size_t i = 0; i + 1 < path2.size(); i++) {
        p2_edges.push_back({path2[i], path2[i+1]});
    }

    auto is_in_path = [&](int u, int v, const std::vector<std::pair<int,int>> &edges) {
        for (auto &e : edges) {
            if (e.first == u && e.second == v) return true;
        }
        return false;
    };

    for (auto &e : m_edges) {
        bool on_p1 = is_in_path(e.u, e.v, p1_edges);
        bool on_p2 = is_in_path(e.u, e.v, p2_edges);

        std::string color = "";
        if (on_p1 && on_p2) {
            // Should never happen if truly vertex-disjoint, 
            // but let's just handle the logic
            color = "purple";
        } else if (on_p1) {
            color = "blue";
        } else if (on_p2) {
            color = "red";
        }

        if (!color.empty()) {
            dot << "    " << e.u << " -> " << e.v << " [label=\"" << e.weight 
                << "\", color=\"" << color << "\", penwidth=2.0];\n";
        } else {
            dot << "    " << e.u << " -> " << e.v << " [label=\"" << e.weight << "\"];\n";
        }
    }
    dot << "}\n";
}
