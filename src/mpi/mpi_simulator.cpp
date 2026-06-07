#include "mpi/simulator.hpp"
#include <mpi.h>
#include <iostream>
#include <iomanip>

MPISimulator::MPISimulator(int gridSize, int numSteps, int numAnts)
    : gridSize_(gridSize), numSteps_(numSteps), numAnts_(numAnts),
      gridLocal_(1, gridSize) {
    MPI_Comm_rank(MPI_COMM_WORLD, &rank_);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs_);

    partition_ = std::make_unique<DomainPartition>(rank_, nprocs_, gridSize_);
    ghostExchange_ = std::make_unique<GhostRowExchange>(rank_, nprocs_, gridSize_);
    agentMigration_ = std::make_unique<AgentMigration>(rank_, nprocs_, gridSize_);

    int localRows = partition_->getLocalRows();
    gridLocal_ = Grid(localRows + 2, gridSize_); // +2 for ghost rows

    initializeAnts();
}

MPISimulator::~MPISimulator() = default;

void MPISimulator::initializeAnts() {
    if (rank_ == 0) {
        for (int i = 0; i < numAnts_; ++i) {
            ants_.push_back(Ant(gridSize_ / 2, gridSize_ / 2, Direction::NORTH, i));
        }
    }
}

void MPISimulator::updateAnts() {
    for (auto& ant : ants_) {
        bool isBlack = gridLocal_.get(ant.y, ant.x);

        if (isBlack) {
            ant.rotateLeft();
        } else {
            ant.rotateRight();
        }

        gridLocal_.flip(ant.y, ant.x);

        auto [nextX, nextY] = ant.getNextPosition();
        ant.x = nextX;
        ant.y = nextY;
    }
}

void MPISimulator::collectGrid() {
    // TODO: Implement MPI_Gather
}

void MPISimulator::run() {
    if (rank_ == 0) {
        std::cout << "Running MPI simulation on " << nprocs_ << " processes\n";
    }

    for (int step = 0; step < numSteps_; ++step) {
        ghostExchange_->exchangeRows(gridLocal_, partition_->getLocalRows());
        updateAnts();

        if (step == 0 || step == 499 || step == 9999 || step == numSteps_ - 1) {
            if (rank_ == 0) {
                std::cout << "Step " << std::setw(6) << step << "\n";
            }
        }
    }
}
