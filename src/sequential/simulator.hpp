#pragma once

#include "grid.hpp"
#include "ant.hpp"
#include <vector>
#include <string>

class SequentialSimulator {
public:
    SequentialSimulator(int gridSize, int numSteps, int numAnts);

    void run();
    void exportFrame(int step, const std::string& outputDir);

    const Grid& getGrid() const { return grid_; }
    const std::vector<Ant>& getAnts() const { return ants_; }

private:
    int gridSize_;
    int numSteps_;
    Grid grid_;
    std::vector<Ant> ants_;

    void initializeAnts();
    void updateAnts();
    void handleConflicts();
};
