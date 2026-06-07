#!/usr/bin/env python3
"""
Benchmarking script for Langton's Ant simulator
Runs strong/weak scaling tests and generates performance metrics
"""

import subprocess
import time
import os
import sys
import json
from pathlib import Path

# Configuration
BUILD_DIR = "/home/cafe/Desktop/Sesiune/AgPA/Langton/build"
RESULTS_FILE = "/home/cafe/Desktop/Sesiune/AgPA/Langton/benchmark_results.json"

# Test configurations
STRONG_SCALING = [
    {"procs": 1, "grid": 5000, "steps": 10000, "ants": 10},
    {"procs": 2, "grid": 5000, "steps": 10000, "ants": 10},
    {"procs": 4, "grid": 5000, "steps": 10000, "ants": 10},
    {"procs": 8, "grid": 5000, "steps": 10000, "ants": 10},
]

WEAK_SCALING = [
    {"procs": 1, "grid": 500, "steps": 10000, "ants": 10},   # 500² = 250K cells
    {"procs": 2, "grid": 707, "steps": 10000, "ants": 10},   # ~354K cells per proc
    {"procs": 4, "grid": 1000, "steps": 10000, "ants": 10},  # 250K cells per proc
]

QUICK_TESTS = [
    {"procs": 1, "grid": 1000, "steps": 100000, "ants": 10},
    {"procs": 4, "grid": 1000, "steps": 100000, "ants": 10},
]

results = {
    "strong_scaling": [],
    "weak_scaling": [],
    "quick_tests": [],
}


def run_sequential(grid_size, num_steps, num_ants):
    """Run sequential simulator and return execution time"""
    cmd = [
        f"{BUILD_DIR}/langton_seq",
        "-n", str(grid_size),
        "-t", str(num_steps),
        "-ants", str(num_ants),
    ]

    start = time.time()
    result = subprocess.run(cmd, capture_output=True, text=True)
    elapsed = time.time() - start

    return elapsed, result.returncode == 0


def run_mpi(procs, grid_size, num_steps, num_ants):
    """Run MPI simulator and return execution time"""
    cmd = [
        "mpirun", "-np", str(procs),
        f"{BUILD_DIR}/langton_mpi",
        "-n", str(grid_size),
        "-t", str(num_steps),
        "-ants", str(num_ants),
    ]

    start = time.time()
    result = subprocess.run(cmd, capture_output=True, text=True)
    elapsed = time.time() - start

    return elapsed, result.returncode == 0


def calculate_metrics(t1, tp, p):
    """Calculate speedup and efficiency"""
    if tp == 0:
        return 0, 0
    speedup = t1 / tp
    efficiency = (speedup / p) * 100
    return speedup, efficiency


print("╔════════════════════════════════════════════════════════════╗")
print("║  Langton's Ant - Benchmarking Campaign                    ║")
print("╚════════════════════════════════════════════════════════════╝\n")

# STRONG SCALING
print("📊 STRONG SCALING TEST (Grid 5000×5000 fixed)\n")
print(f"{'Procs':<8} {'Grid':<10} {'Steps':<10} {'Time (s)':<12} {'Speedup':<12} {'Efficiency %':<12}")
print("-" * 74)

t1_strong = None
for test in STRONG_SCALING:
    procs = test["procs"]
    grid = test["grid"]
    steps = test["steps"]
    ants = test["ants"]

    elapsed, success = run_mpi(procs, grid, steps, ants)

    if procs == 1:
        t1_strong = elapsed

    speedup, efficiency = calculate_metrics(t1_strong, elapsed, procs)

    result = {
        "procs": procs,
        "grid": grid,
        "steps": steps,
        "ants": ants,
        "time": elapsed,
        "speedup": speedup,
        "efficiency": efficiency,
        "success": success,
    }
    results["strong_scaling"].append(result)

    print(f"{procs:<8} {grid:<10} {steps:<10} {elapsed:<12.4f} {speedup:<12.2f} {efficiency:<12.1f}%")

# WEAK SCALING
print("\n📈 WEAK SCALING TEST (N²/P ≈ constant)\n")
print(f"{'Procs':<8} {'Grid':<10} {'Cells/Proc':<15} {'Time (s)':<12} {'Ratio':<12}")
print("-" * 67)

t1_weak = None
for i, test in enumerate(WEAK_SCALING):
    procs = test["procs"]
    grid = test["grid"]
    steps = test["steps"]
    ants = test["ants"]
    cells_per_proc = (grid * grid) / procs

    elapsed, success = run_mpi(procs, grid, steps, ants)

    if procs == 1:
        t1_weak = elapsed

    time_ratio = elapsed / t1_weak if t1_weak else 1.0

    result = {
        "procs": procs,
        "grid": grid,
        "steps": steps,
        "ants": ants,
        "cells_per_proc": cells_per_proc,
        "time": elapsed,
        "time_ratio": time_ratio,
        "success": success,
    }
    results["weak_scaling"].append(result)

    print(f"{procs:<8} {grid:<10} {cells_per_proc:<15.0f} {elapsed:<12.4f} {time_ratio:<12.2f}x")

# QUICK SEQUENTIAL vs MPI
print("\n⚡ QUICK COMPARISON (1000×1000 grid, 100K steps)\n")
print(f"{'Mode':<15} {'Time (s)':<12} {'Speedup':<12}")
print("-" * 39)

# Sequential
seq_time, seq_ok = run_sequential(1000, 100000, 10)
print(f"{'Sequential':<15} {seq_time:<12.4f}")

# MPI 1 process
mpi1_time, mpi1_ok = run_mpi(1, 1000, 100000, 10)
mpi1_speedup = seq_time / mpi1_time if mpi1_time > 0 else 0
print(f"{'MPI (1 proc)':<15} {mpi1_time:<12.4f} {mpi1_speedup:<12.2f}x")

# MPI 4 processes
mpi4_time, mpi4_ok = run_mpi(4, 1000, 100000, 10)
mpi4_speedup = seq_time / mpi4_time if mpi4_time > 0 else 0
print(f"{'MPI (4 procs)':<15} {mpi4_time:<12.4f} {mpi4_speedup:<12.2f}x")

results["quick_tests"].append({
    "sequential_time": seq_time,
    "mpi1_time": mpi1_time,
    "mpi4_time": mpi4_time,
})

# Save results
with open(RESULTS_FILE, 'w') as f:
    json.dump(results, f, indent=2)

print(f"\n✅ Results saved to: {RESULTS_FILE}\n")

# Summary
print("╔════════════════════════════════════════════════════════════╗")
print("║  SUMMARY                                                   ║")
print("╚════════════════════════════════════════════════════════════╝\n")

if results["strong_scaling"]:
    print("Strong Scaling (5000×5000):")
    for r in results["strong_scaling"]:
        print(f"  P={r['procs']:2d}: {r['time']:.4f}s, Speedup={r['speedup']:.2f}x, Eff={r['efficiency']:.1f}%")

if results["weak_scaling"]:
    print("\nWeak Scaling (constant cells/proc):")
    for r in results["weak_scaling"]:
        print(f"  P={r['procs']:2d}: {r['time']:.4f}s (ratio={r['time_ratio']:.2f}x)")

print(f"\nSequential: {seq_time:.4f}s")
print(f"MPI (1p):   {mpi1_time:.4f}s")
print(f"MPI (4p):   {mpi4_time:.4f}s")
