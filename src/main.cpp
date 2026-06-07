#include <iostream>
#include <string>
#include <cstring>

#ifdef ENABLE_MPI
#include <mpi.h>
#include "mpi/simulator.hpp"
#else
#include "sequential/simulator.hpp"
#endif

void printUsage(const char* progName) {
    std::cout << "Usage: " << progName << " [OPTIONS]\n"
              << "Options:\n"
              << "  -n <size>      Grid size (default: 1000)\n"
              << "  -t <steps>     Number of steps (default: 100000)\n"
              << "  -ants <count>  Number of ants (default: 1)\n"
              << "  -o <dir>       Output directory (default: output)\n"
              << "  -h             Show this help\n";
}

int main(int argc, char* argv[]) {
#ifdef ENABLE_MPI
    MPI_Init(&argc, &argv);
#endif

    int gridSize = 1000;
    int numSteps = 100000;
    int numAnts = 1;
    std::string outputDir = "output";

    for (int i = 1; i < argc; ++i) {
        if (std::strcmp(argv[i], "-n") == 0 && i + 1 < argc) {
            gridSize = std::atoi(argv[++i]);
        } else if (std::strcmp(argv[i], "-t") == 0 && i + 1 < argc) {
            numSteps = std::atoi(argv[++i]);
        } else if (std::strcmp(argv[i], "-ants") == 0 && i + 1 < argc) {
            numAnts = std::atoi(argv[++i]);
        } else if (std::strcmp(argv[i], "-o") == 0 && i + 1 < argc) {
            outputDir = argv[++i];
        } else if (std::strcmp(argv[i], "-h") == 0) {
            printUsage(argv[0]);
#ifdef ENABLE_MPI
            MPI_Finalize();
#endif
            return 0;
        }
    }

#ifdef ENABLE_MPI
    MPISimulator sim(gridSize, numSteps, numAnts);
    sim.run();
    MPI_Finalize();
#else
    SequentialSimulator sim(gridSize, numSteps, numAnts);
    sim.run();
#endif

    return 0;
}
