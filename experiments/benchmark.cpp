#include "sequential/simulator.hpp"
#include "utils/timing.hpp"
#include <iostream>

int main() {
    std::cout << "Benchmarking sequential simulator...\n";

    Timer t("Sequential benchmark");

    SequentialSimulator sim(500, 10000, 5);
    sim.run();

    t.print();

    return 0;
}
