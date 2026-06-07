# Usage Guide - Langton's Ant Simulator

## Installation

```bash
cd /home/cafe/Desktop/Sesiune/AgPA/Langton
mkdir -p build && cd build
cmake ..
make -j4
```

Executables will be in `build/`:
- `langton_seq` — Sequential version
- `langton_mpi` — MPI parallel version
- `test_*` — Unit tests
- `benchmark` — Performance testing

## Running Simulations

### Basic Example: Sequential

**Small grid (fast)**:
```bash
./langton_seq -n 100 -t 1000 -ants 1
```

Output:
```
Running sequential simulation: grid=100x100, steps=1000, ants=1
Step      0: 1 ants alive
Step    499: 1 ants alive
Step    999: 1 ants alive
Simulation complete. Final ants: 1
```

**Large grid**:
```bash
./langton_seq -n 1000 -t 100000 -ants 10 -o /tmp/output
```

### MPI Examples

**Single process (for testing)**:
```bash
mpirun -np 1 ./langton_mpi -n 500 -t 50000 -ants 5
```

**Multiple processes (parallel)**:
```bash
# 2 processes
mpirun -np 2 ./langton_mpi -n 1000 -t 100000 -ants 10

# 4 processes (recommended for testing)
mpirun -np 4 ./langton_mpi -n 1000 -t 100000 -ants 10

# 8 processes (larger grids)
mpirun -np 8 ./langton_mpi -n 5000 -t 1000000 -ants 100
```

## Testing

### Run All Tests
```bash
cd build
ctest
```

### Run Individual Tests
```bash
# Grid operations
./tests/test_grid

# Ant movement
./tests/test_ant

# Integration test
./tests/test_correctness
```

### Benchmark
```bash
./experiments/benchmark
```

## Understanding Output

### Grid Size (-n)
Determines the N×N grid dimensions:
- `100` = 100×100 cells (10,000 total)
- `1000` = 1000×1000 cells (1,000,000 total)
- `5000` = 5000×5000 cells (25,000,000 total)

### Steps (-t)
Number of simulation steps:
- After ~500 steps: "simplicity phase" ends
- After ~500-10,000 steps: "chaos phase"
- After ~10,000 steps: "highway phase" begins (diagonal pattern)

### Ants (-ants)
Number of concurrent ants:
- Each starts at grid center (N/2, N/2)
- All face North initially
- Remove if they leave the grid boundary

### Output (-o)
Directory for PPM images (not yet integrated):
```bash
./langton_seq -n 100 -t 1000 -ants 1 -o /tmp/frames
ls /tmp/frames/  # (Currently empty - would contain frame_*.ppm)
```

## Performance Expectations

### Sequential
- 100×100, 1000 steps: ~1ms
- 1000×1000, 100K steps: ~15ms
- 5000×5000, 1M steps: ~1-2s

### MPI (Large Grids)
- Expected speedup with multiple processes
- Overhead dominates on small problems
- Benefits at 5000×5000+

## Troubleshooting

### Build Issues
```bash
# Clean and rebuild
rm -rf build
mkdir build && cd build
cmake ..
make clean && make
```

### MPI Not Found
```bash
# Check if MPI is installed
which mpirun

# If not installed:
# Ubuntu: sudo apt-get install libopenmpi-dev
# CentOS: sudo yum install openmpi-devel
```

### Too Many Ants Die Early
This is expected behavior. The ants' random walk causes them to eventually leave the bounded grid. To keep more ants alive longer, increase grid size or reduce total steps before they escape.

## Configuration Examples

### Minimal Test
```bash
./langton_seq -n 50 -t 100 -ants 1
```

### Standard Verification
```bash
./langton_seq -n 500 -t 11000 -ants 5
# Watch for highway phase starting around step 10000
```

### Parallel Stress Test
```bash
mpirun -np 4 ./langton_mpi -n 2000 -t 500000 -ants 50
```

### Production Run (Large Scale)
```bash
mpirun -np 8 ./langton_mpi -n 5000 -t 1000000 -ants 100
```

## What to Expect

**Phase 1 (0-500 steps)**: Regular symmetric patterns  
**Phase 2 (500-10,000 steps)**: Chaotic movement  
**Phase 3 (10,000+ steps)**: Periodic "highway" emerges (theoretical prediction)

The bounded grid means ants eventually escape. Toroidal wrap-around would allow indefinite behavior.

## Next Steps

- Try different grid sizes to observe scaling
- Run multiple tests to confirm determinism
- Compare sequential vs. MPI results
- Vary ant counts to see interaction effects
