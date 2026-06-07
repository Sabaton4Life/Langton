#include "mpi/communicator.hpp"
#include <mpi.h>
#include <algorithm>
#include <cstdint>

GhostRowExchange::GhostRowExchange(int rank, int nprocs, int cols)
    : rank_(rank), nprocs_(nprocs), cols_(cols),
      topGhostBuffer_(cols, 0), bottomGhostBuffer_(cols, 0),
      topReceived_(cols, 0), bottomReceived_(cols, 0) {}

void GhostRowExchange::exchangeRows(Grid& grid, int localRows) {
    int cols = grid.getCols();

    for (int c = 0; c < cols; ++c) {
        topGhostBuffer_[c] = grid.get(0, c) ? 1 : 0;
        bottomGhostBuffer_[c] = grid.get(localRows - 1, c) ? 1 : 0;
    }

    if (rank_ > 0) {
        MPI_Sendrecv(topGhostBuffer_.data(), cols, MPI_UINT8_T, rank_ - 1, 0,
                     topReceived_.data(), cols, MPI_UINT8_T, rank_ - 1, 1,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
    if (rank_ < nprocs_ - 1) {
        MPI_Sendrecv(bottomGhostBuffer_.data(), cols, MPI_UINT8_T, rank_ + 1, 1,
                     bottomReceived_.data(), cols, MPI_UINT8_T, rank_ + 1, 0,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    for (int c = 0; c < cols; ++c) {
        if (rank_ > 0) grid.set(0, c, topReceived_[c] != 0);
        if (rank_ < nprocs_ - 1) grid.set(localRows - 1, c, bottomReceived_[c] != 0);
    }
}

AgentMigration::AgentMigration(int rank, int nprocs, int gridSize, int rowStart, int rowEnd)
    : rank_(rank), nprocs_(nprocs), gridSize_(gridSize), rowStart_(rowStart), rowEnd_(rowEnd) {}

std::vector<Ant> AgentMigration::getMigratingAnts(const std::vector<Ant>& ants) {
    std::vector<Ant> migrating;
    for (const auto& ant : ants) {
        if (ant.y < rowStart_ || ant.y >= rowEnd_ || ant.x < 0 || ant.x >= gridSize_) {
            migrating.push_back(ant);
        }
    }
    return migrating;
}

void AgentMigration::sendAgents(const std::vector<Ant>& agents) {
    std::vector<Ant> toUp, toDown;

    for (const auto& ant : agents) {
        if (ant.y < rowStart_ && rank_ > 0) {
            toUp.push_back(ant);
        } else if (ant.y >= rowEnd_ && rank_ < nprocs_ - 1) {
            toDown.push_back(ant);
        }
    }

    // Trimitere către vecini (P2P este mult mai rapid decât Bcast în buclă)
    if (rank_ > 0) {
        int count = toUp.size();
        MPI_Send(&count, 1, MPI_INT, rank_ - 1, 10, MPI_COMM_WORLD);
        if (count > 0) 
            MPI_Send(toUp.data(), count * sizeof(Ant), MPI_BYTE, rank_ - 1, 11, MPI_COMM_WORLD);
    }
    if (rank_ < nprocs_ - 1) {
        int count = toDown.size();
        MPI_Send(&count, 1, MPI_INT, rank_ + 1, 20, MPI_COMM_WORLD);
        if (count > 0)
            MPI_Send(toDown.data(), count * sizeof(Ant), MPI_BYTE, rank_ + 1, 21, MPI_COMM_WORLD);
    }
}

std::vector<Ant> AgentMigration::receiveAgents() {
    std::vector<Ant> incoming;
    
    auto receiveFrom = [&](int source, int tagCount, int tagData) {
        int count = 0;
        MPI_Status status;
        // Verificăm dacă există mesaje de la vecini
        MPI_Recv(&count, 1, MPI_INT, source, tagCount, MPI_COMM_WORLD, &status);
        if (count > 0) {
            std::vector<Ant> buffer(count);
            MPI_Recv(buffer.data(), count * sizeof(Ant), MPI_BYTE, source, tagData, MPI_COMM_WORLD, &status);
            incoming.insert(incoming.end(), buffer.begin(), buffer.end());
        }
    };

    if (rank_ < nprocs_ - 1) receiveFrom(rank_ + 1, 10, 11); // Primim de la vecinul de jos (care a trimis "sus")
    if (rank_ > 0) receiveFrom(rank_ - 1, 20, 21);          // Primim de la vecinul de sus (care a trimis "jos")

    return incoming;
}
