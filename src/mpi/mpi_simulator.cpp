#include "mpi/simulator.hpp"
#include "utils/io.hpp"
#include "utils/timing.hpp"
#include <mpi.h>
#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <cstdint>

MPISimulator::MPISimulator(int gridSize, int numSteps, int numAnts)
    : gridSize_(gridSize), numSteps_(numSteps), numAnts_(numAnts),
      gridLocal_(1, gridSize) {
    MPI_Comm_rank(MPI_COMM_WORLD, &rank_);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs_);

    partition_ = std::make_unique<DomainPartition>(rank_, nprocs_, gridSize_);
    ghostExchange_ = std::make_unique<GhostRowExchange>(rank_, nprocs_, gridSize_);
    agentMigration_ = std::make_unique<AgentMigration>(rank_, nprocs_, gridSize_);

    int localRows = partition_->getLocalRows();
    gridLocal_ = Grid(localRows, gridSize_);

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

std::vector<Ant> MPISimulator::updateAnts() {
    std::vector<Ant> localAnts, migratingAnts;

    for (const auto& ant : ants_) {
        int globalRow = ant.y;
        int rowStart = partition_->getRowStart();
        int rowEnd = partition_->getRowEnd();

        if (globalRow >= rowStart && globalRow < rowEnd) {
            localAnts.push_back(ant);
        } else {
            migratingAnts.push_back(ant);
        }
    }

    for (auto& ant : localAnts) {
        int localRow = ant.y - partition_->getRowStart();
        bool isBlack = gridLocal_.get(localRow, ant.x);

        if (isBlack) ant.rotateLeft();
        else ant.rotateRight();

        gridLocal_.flip(localRow, ant.x);

        auto [nextX, nextY] = ant.getNextPosition();
        ant.x = nextX;
        ant.y = nextY;
    }

    ants_ = localAnts;
    return migratingAnts;
}

void MPISimulator::collectGrid() {
    // Simple gather to rank 0
    if (rank_ == 0) {
        Grid globalGrid(gridSize_, gridSize_);
        int rowStart = partition_->getRowStart();
        int localRows = partition_->getLocalRows();

        for (int r = 0; r < localRows; ++r) {
            for (int c = 0; c < gridSize_; ++c) {
                globalGrid.set(rowStart + r, c, gridLocal_.get(r, c));
            }
        }

        std::vector<uint8_t> recvBuf;
        for (int p = 1; p < nprocs_; ++p) {
            DomainPartition ppart(p, nprocs_, gridSize_);
            int prows = ppart.getLocalRows();
            recvBuf.resize(prows * gridSize_);

            MPI_Recv(recvBuf.data(), prows * gridSize_, MPI_UINT8_T, p, 0,
                    MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            for (int r = 0; r < prows; ++r) {
                for (int c = 0; c < gridSize_; ++c) {
                    globalGrid.set(ppart.getRowStart() + r, c,
                                  recvBuf[r * gridSize_ + c] != 0);
                }
            }
        }
    } else {
        std::vector<uint8_t> sendBuf;
        int localRows = partition_->getLocalRows();
        for (int r = 0; r < localRows; ++r) {
            for (int c = 0; c < gridSize_; ++c) {
                sendBuf.push_back(gridLocal_.get(r, c) ? 1 : 0);
            }
        }
        MPI_Send(sendBuf.data(), localRows * gridSize_, MPI_UINT8_T, 0, 0,
                MPI_COMM_WORLD);
    }
}

void MPISimulator::run() {
    if (rank_ == 0) {
        std::cout << "Running MPI simulation on " << nprocs_ << " processes\n";
        std::cout << "Grid: " << gridSize_ << "x" << gridSize_ << ", steps: "
                  << numSteps_ << ", ants: " << numAnts_ << "\n";
    }

    Timer timer("MPI simulation");

    for (int step = 0; step < numSteps_; ++step) {
        // 1. Sincronizare rânduri ghost (consistență la granițe)
        ghostExchange_->exchangeRows(gridLocal_, partition_->getLocalRows());

        // 2. Calcul local
        auto migrating = updateAnts();

        // 3. Migrare furnici între procese
        agentMigration_->sendAgents(migrating);
        auto incoming = agentMigration_->receiveAgents();
        
        // Adăugăm furnicile primite la lista locală
        ants_.insert(ants_.end(), incoming.begin(), incoming.end());

        if (step == 0 || step == 499 || step == 9999 || step == numSteps_ - 1) {
            if (rank_ == 0) {
                std::cout << "Step " << std::setw(6) << step << "\n";
            }
        }
    }

    if (rank_ == 0) {
        timer.print();
        std::cout << "MPI Simulation complete.\n";
    }
}
