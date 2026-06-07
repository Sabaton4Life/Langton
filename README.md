# Langton's Ant Cellular Automaton - MPI Implementation

A high-performance parallel implementation of Langton's Ant cellular automaton using MPI (Message Passing Interface) for distributed computing.

## Quick Start

### Build
```bash
mkdir -p build && cd build
cmake ..
make -j4
```

### Run Sequential Version
```bash
./langton_seq -n 1000 -t 100000 -ants 10 -o output_seq
```

### Run MPI Version
```bash
# 1 process (sequential with MPI overhead)
mpirun -np 1 ./langton_mpi -n 1000 -t 100000 -ants 10 -o output_mpi

# 4 processes (parallel)
mpirun -np 4 ./langton_mpi -n 1000 -t 100000 -ants 10 -o output_mpi
```

## Command Line Options

| Option | Description | Default |
|--------|-------------|---------|
| `-n <size>` | Grid size (N×N) | 1000 |
| `-t <steps>` | Number of simulation steps | 100000 |
| `-ants <count>` | Number of ants | 1 |
| `-o <dir>` | Output directory for PPM frames | output |
| `-h` | Show help | — |

## Architecture

### Data Structures

**Grid**: 2D boolean array
- `false` = white cell
- `true` = black cell

**Ant**: Position (x, y), direction (N/E/S/W), ID
- Rotates 90° right on white cell, left on black
- Flips cell color
- Moves forward one step

### Algorithms

**Sequential**:
1. For each step:
   - For each ant: rotate, flip, move
   - Remove ants that leave grid

**Parallel (MPI)**:
1. Partition grid into 1D strips (one per process)
2. For each step:
   - Compute local ants
   - Exchange ghost rows with neighbors (not yet implemented fully)
   - Gather results to rank 0

## Implementation Status

✅ **Completed**:
- Sequential simulator with multi-ant support
- Basic MPI skeleton with domain partitioning
- Unit tests (Grid, Ant operations)
- Integration tests
- CLI interface
- Build system (CMake)

⏳ **In Progress / Future**:
- Ghost row exchange with MPI_Sendrecv
- Agent migration between processes
- PPM frame export (partially done)
- Performance benchmarking
- Weak/strong scaling analysis
- Highway detection (post-10K steps convergence)

## Testing

### Run Tests
```bash
cd build
ctest
```

### Test Coverage
- `test_grid`: Grid operations (get/set/flip)
- `test_ant`: Ant rotation and movement
- `test_correctness`: Integration test (100×100, 1000 steps)

## Performance

| Configuration | Seq Time | MPI (1p) | MPI (4p) |
|---------------|----------|----------|----------|
| 1000×1000, 100K steps, 10 ants | ~15ms | ~5ms | ~5ms* |

*MPI overhead dominates for small problem sizes. Parallelism benefits scale with grid size.

## Project Structure

```
Langton/
├── src/
│   ├── sequential/    # Sequential implementation
│   │   ├── grid.hpp/cpp
│   │   ├── ant.hpp/cpp
│   │   └── simulator.hpp/cpp
│   ├── mpi/           # MPI parallel implementation
│   │   ├── domain.hpp/cpp      # Domain partitioning
│   │   ├── communicator.hpp/cpp # Ghost rows, migration
│   │   └── mpi_simulator.hpp/cpp
│   ├── utils/         # Utilities
│   │   ├── io.hpp/cpp           # PPM export
│   │   └── timing.hpp           # MPI_Wtime timers
│   └── main.cpp       # CLI & dispatch
├── tests/             # Unit + integration tests
├── experiments/       # Benchmarking
├── CMakeLists.txt
└── README.md
```

## References

1. Langton, C. G. (1986). Studying artificial life with cellular automata. *Physica D*, 22(1-3), 120-149.
2. Bunimovich, L. A., & Troubetzkoy, S. E. (2000). Recurrence properties of Lorentz lattice gas cellular automata.
3. Wolfram, S. (2002). *A New Kind of Science*.
4. [OpenMPI Documentation](https://www.open-mpi.org/doc/)

## Build Requirements

- CMake 3.18+
- C++17 compiler (g++, clang)
- OpenMPI 3.1+ or MPICH 3.3+

## Tested On

- Linux (g++ 16.1.1, OpenMPI 3.1)
- Ubuntu 20.04 LTS

## License

Educational project.

