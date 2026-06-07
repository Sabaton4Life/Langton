#include "sequential/grid.hpp"
#include <cassert>
#include <iostream>

int main() {
    std::cout << "Testing Grid class...\n";

    Grid g(10, 10);

    // Test get/set
    g.set(5, 5, true);
    assert(g.get(5, 5) == true);
    assert(g.get(0, 0) == false);

    // Test flip
    g.flip(5, 5);
    assert(g.get(5, 5) == false);

    // Test boundaries
    assert(g.get(-1, 5) == false);
    assert(g.get(10, 10) == false);

    std::cout << "All grid tests passed!\n";
    return 0;
}
