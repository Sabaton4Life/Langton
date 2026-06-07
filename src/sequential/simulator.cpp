#include "simulator.hpp"
#include "../utils/io.hpp"
#include <iostream>
#include <iomanip>
#include <map>
#include <algorithm>

SequentialSimulator::SequentialSimulator(int gridSize, int numSteps, int numAnts)
    : gridSize_(gridSize), numSteps_(numSteps), grid_(gridSize, gridSize) {
    initializeAnts();
    for (int i = 0; i < numAnts; ++i) {
        ants_.push_back(Ant(gridSize / 2, gridSize / 2, Direction::NORTH, i));
    }
}

void SequentialSimulator::initializeAnts() {
    ants_.clear();
}

void SequentialSimulator::updateAnts() {
    std::map<std::pair<int,int>, std::vector<int>> cellToAnts;

    // Step 1: Compute rotations and flips
    for (auto& ant : ants_) {
        bool isBlack = grid_.get(ant.y, ant.x);
        if (isBlack) ant.rotateLeft();
        else ant.rotateRight();
        grid_.flip(ant.y, ant.x);
    }

    // Step 2: Move ants (keep those in bounds)
    std::vector<Ant> newAnts;
    for (auto& ant : ants_) {
        auto [nextX, nextY] = ant.getNextPosition();
        if (nextX >= 0 && nextX < gridSize_ && nextY >= 0 && nextY < gridSize_) {
            ant.x = nextX;
            ant.y = nextY;
            newAnts.push_back(ant);
            cellToAnts[{ant.x, ant.y}].push_back(ant.id);
        }
    }

    // Step 3: Handle conflicts (if multiple ants on same cell, keep all - they interact dynamically)
    ants_ = newAnts;
}

void SequentialSimulator::handleConflicts() {
    // Conflicts are resolved during movement (deterministic order)
}

void SequentialSimulator::run() {
    std::cout << "Running sequential simulation: grid=" << gridSize_ << "x" << gridSize_
              << ", steps=" << numSteps_ << ", ants=" << ants_.size() << "\n";

    for (int step = 0; step < numSteps_; ++step) {
        updateAnts();

        if (step == 0 || step == 499 || step == 999 || step == 4999 ||
            step == 9999 || step == 49999 || step == numSteps_ - 1) {
            std::cout << "Step " << std::setw(6) << step << ": " << ants_.size() << " ants alive\n";
        }
    }

    std::cout << "Simulation complete. Final ants: " << ants_.size() << "\n";
}

void SequentialSimulator::exportFrame(int step, const std::string& outputDir) {
    std::string filename = outputDir + "/frame_" + std::to_string(step) + ".ppm";
    PPMExporter::exportBinary(grid_.data(), gridSize_, gridSize_, filename);
    std::cout << "Exported: " << filename << "\n";
}
