# Vertex-Disjoint Shortest Pair of Paths Project

## Overview
This project implements and compares two approaches for finding two vertex-disjoint shortest paths between a given source and target in a directed, weighted graph:

1. **Brute Force Method (Conceptual Baseline)**:
   - Enumerates possible paths and checks pairs for vertex-disjointness.
   - Feasible only for small graphs due to exponential complexity.

2. **Sophisticated Dijkstra-Based Method**:
   - Runs Dijkstra’s algorithm to find one shortest path.
   - Removes internal vertices of this path from the graph.
   - Runs Dijkstra’s again on the modified graph to find a second shortest path.
   - Achieves polynomial-time performance and scales to large graphs.

The code supports loading graphs from DIMACS format (e.g., `Rome99.gr`) or generating random graphs. Outputs can include DOT files for graph visualization and timing comparisons of performance.


### Key Components
- **`main.cpp`**: Entry point for selecting input mode and running algorithms or tests.
- **`vertex_disjoint_paths.cpp` / `vertex_disjoint_paths.hpp`**: Sophisticated algorithm implementation.
- **`bruteforce.cpp` / `bruteforce.hpp`**: Brute force approach for small graphs.
- **`tests.cpp` / `tests.hpp`**: Compares brute force and sophisticated methods.
- **`graph.cpp` / `graph.hpp`**: Graph data structure and utilities.
- **`random_graph_generator.cpp` / `random_graph_generator.hpp`**: Random graph generation.
- **`data/`**: Input datasets and generated output files.

## Building the Project
1. Ensure you have CMake and a C++17 or newer compiler.
2. From the project root directory:
   ```bash
   mkdir build
   cd build
   cmake ..
   make

## Running the Project

After building the project as described above, you can run the executable:

```bash
./build/VertexDisjointPaths

You’ll be prompted to select an input mode:

1: Generate a random graph (you specify nodes/edges).
2: Load Rome99.gr from data/.
3: Run brute force on a tiny random graph.
4: Run comparison tests between brute force and sophisticated on various sizes.
After choosing:

If generating a random graph, enter the number of nodes and edges.
If loading Rome99, just proceed.
You will then be asked for source and target nodes.
The program prints runtime info and found paths. If two vertex-disjoint paths are found, it prints their sequences and costs, and generates a graph.dot file.

