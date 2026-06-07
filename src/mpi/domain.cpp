#include "mpi/domain.hpp"

DomainPartition::DomainPartition(int rank, int nprocs, int gridSize)
    : rank_(rank), nprocs_(nprocs), globalSize_(gridSize) {
    int rowsPerProc = gridSize / nprocs;
    int remainder = gridSize % nprocs;

    if (rank < remainder) {
        rowStart_ = rank * (rowsPerProc + 1);
        rowEnd_ = rowStart_ + rowsPerProc + 1;
    } else {
        rowStart_ = remainder * (rowsPerProc + 1) + (rank - remainder) * rowsPerProc;
        rowEnd_ = rowStart_ + rowsPerProc;
    }
}
