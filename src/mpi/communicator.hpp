#pragma once

#include "sequential/grid.hpp"
#include "sequential/ant.hpp"
#include <vector>
#include <cstdint>

class GhostRowExchange {
public:
    GhostRowExchange(int rank, int nprocs, int cols);

    void exchangeRows(Grid& grid, int localRows);

private:
    int rank_, nprocs_, cols_;
    std::vector<uint8_t> topGhostBuffer_, bottomGhostBuffer_;
};

class AgentMigration {
public:
    AgentMigration(int rank, int nprocs, int gridSize);

    std::vector<Ant> getMigratingAnts(const std::vector<Ant>& ants);
    void sendAgents(const std::vector<Ant>& agents);
    std::vector<Ant> receiveAgents();

private:
    int rank_, nprocs_, gridSize_;
};
