#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>
#include <string>

struct Edge {
    int u;
    int v;
    int weight;
};

class Graph {
public:
    Graph(int num_nodes);

    void addEdge(int u, int v, int weight);
    int numNodes() const;
    int numEdges() const;

    // Save graph in DIMACS format
    void saveDIMACS(const std::string &filename) const;

    // Print graph info to stdout
    void printInfo() const;

    // Save DOT file for visualization
    void saveDOT(const std::string &filename,
                 const std::vector<int> &path = {}) const;

    // New: A method to build adjacency list
    void buildAdjacencyList();

    // Get adjacency list: returns a const reference
    const std::vector<std::vector<Edge>>& adjacencyList() const {
        return m_adjacency;
    }
    void saveDOTWithTwoPaths(const std::string &filename, 
                         const std::vector<int> &path1, 
                         const std::vector<int> &path2) const;
    
    const std::vector<Edge>& allEdges() const { return m_edges; }


private:
    int m_num_nodes;
    std::vector<Edge> m_edges;
    std::vector<std::vector<Edge>> m_adjacency; // adjacency list by node
};

#endif
