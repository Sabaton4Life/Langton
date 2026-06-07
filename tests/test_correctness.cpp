#include "sequential/simulator.hpp"
#include <iostream>

int main() {
    std::cout << "Running correctness test...\n";

    SequentialSimulator sim(100, 1000, 1);
    sim.run();

    std::cout << "Correctness test completed!\n";
    return 0;
}
