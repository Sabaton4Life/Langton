# Langton's Ant Cellular Automaton - MPI Implementation

A parallel implementation of Langton's Ant cellular automaton using MPI (Message Passing Interface).

## Project Structure

```
Langton/
├── src/
│   ├── sequential/    # Sequential version
│   ├── mpi/           # MPI parallel version
│   └── utils/         # Utilities (I/O, timing)
├── tests/             # Unit and integration tests
├── experiments/       # Benchmarking code
└── CMakeLists.txt     # Build configuration
```

## Building

```bash
mkdir build && cd build
cmake ..
make
```

## Running

### Sequential version
```bash
./langton_seq -n 1000 -t 100000 -ants 10 -o output
```

### MPI version
```bash
mpirun -np 4 ./langton_mpi -n 1000 -t 100000 -ants 10 -o output
```

## Command Line Options

- `-n <size>` — Grid size (default: 1000)
- `-t <steps>` — Number of simulation steps (default: 100000)
- `-ants <count>` — Number of ants (default: 1)
- `-o <dir>` — Output directory (default: output)
- `-h` — Show help

## Running Tests

```bash
cd build
ctest
```

## Implementation Phases

1. **Week 1** — Sequential version with multi-ant support
2. **Week 2** — MPI with domain partitioning and ghost rows
3. **Week 3** — Agent migration and optimizations
4. **Week 4** — Benchmarking and report

## References

- Langton, C. G. (1986). Studying artificial life with cellular automata.
- Bunimovich & Troubetzkoy (2000). Highway behavior in Langton's Ant.
