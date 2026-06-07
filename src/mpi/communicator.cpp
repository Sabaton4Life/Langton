#include "mpi/communicator.hpp"
#include <mpi.h>
#include <algorithm>
#include <cstdint>

GhostRowExchange::GhostRowExchange(int rank, int nprocs, int cols)
    : rank_(rank), nprocs_(nprocs), cols_(cols),
      topGhostBuffer_(cols, 0), bottomGhostBuffer_(cols, 0) {}

void GhostRowExchange::exchangeRows(Grid& grid, int localRows) {
    int cols = grid.getCols();

    // Copy boundary rows to buffers
    for (int c = 0; c < cols; ++c) {
        topGhostBuffer_[c] = grid.get(0, c) ? 1 : 0;
        bottomGhostBuffer_[c] = grid.get(localRows - 1, c) ? 1 : 0;
    }

    // Exchange with neighbors using MPI_Sendrecv
    std::vector<uint8_t> topReceived(cols, 0), bottomReceived(cols, 0);

    if (rank_ > 0) {
        MPI_Sendrecv(topGhostBuffer_.data(), cols, MPI_UINT8_T, rank_ - 1, 0,
                     topReceived.data(), cols, MPI_UINT8_T, rank_ - 1, 1,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    if (rank_ < nprocs_ - 1) {
        MPI_Sendrecv(bottomGhostBuffer_.data(), cols, MPI_UINT8_T, rank_ + 1, 1,
                     bottomReceived.data(), cols, MPI_UINT8_T, rank_ + 1, 0,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    // Update ghost rows in grid (conceptually; actual grid access depends on implementation)
}

AgentMigration::AgentMigration(int rank, int nprocs, int gridSize)
    : rank_(rank), nprocs_(nprocs), gridSize_(gridSize) {}

std::vector<Ant> AgentMigration::getMigratingAnts(const std::vector<Ant>& ants) {
    std::vector<Ant> migrating;
    for (const auto& ant : ants) {
        if (ant.x < 0 || ant.x >= gridSize_ || ant.y < 0 || ant.y >= gridSize_) {
            migrating.push_back(ant);
        }
    }
    return migrating;
}

void AgentMigration::sendAgents(const std::vector<Ant>& agents) {
    // TODO: Implement actual MPI_Isend for agents
}

std::vector<Ant> AgentMigration::receiveAgents() {
    // TODO: Implement actual MPI_Probe + MPI_Recv for agents
    return std::vector<Ant>();
}
