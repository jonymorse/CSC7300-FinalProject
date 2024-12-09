#ifndef TESTS_HPP
#define TESTS_HPP

#include "graph.hpp"

// Run experiments comparing brute force and the vertex-disjoint paths approach.
// This function will:
// 1) Generate small random graphs.
// 2) Run brute force and measure its time.
// 3) Run the sophisticated approach and measure its time.
// 4) Print the results.
void runComparisonTests();

#endif
