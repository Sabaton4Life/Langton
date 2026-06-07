#pragma once

class DomainPartition {
public:
    DomainPartition(int rank, int nprocs, int gridSize);

    int getRank() const { return rank_; }
    int getNprocs() const { return nprocs_; }
    int getGlobalSize() const { return globalSize_; }
    int getRowStart() const { return rowStart_; }
    int getRowEnd() const { return rowEnd_; }
    int getLocalRows() const { return rowEnd_ - rowStart_; }

private:
    int rank_, nprocs_;
    int globalSize_;
    int rowStart_, rowEnd_;
};
