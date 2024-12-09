cmake_minimum_required(VERSION 3.0)
project(VertexDisjointPaths CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(VertexDisjointPaths
    src/main.cpp
    src/graph.cpp
    src/random_graph_generator.cpp
    src/vertex_disjoint_paths.cpp
    src/bruteforce.cpp
    src/tests.cpp
)
