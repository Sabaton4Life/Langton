#pragma once

#include "sequential/grid.hpp"
#include "sequential/ant.hpp"
#include "mpi/domain.hpp"
#include "mpi/communicator.hpp"
#include <vector>
#include <memory>

class MPISimulator {
public:
    MPISimulator(int gridSize, int numSteps, int numAnts);
    ~MPISimulator();

    void run();

private:
    int gridSize_, numSteps_, numAnts_;
    int rank_, nprocs_;

    std::unique_ptr<DomainPartition> partition_;
    std::unique_ptr<GhostRowExchange> ghostExchange_;
    std::unique_ptr<AgentMigration> agentMigration_;

    Grid gridLocal_;
    std::vector<Ant> ants_;

    void initializeAnts();
    void updateAnts();
    void collectGrid();
};
