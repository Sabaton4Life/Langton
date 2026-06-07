import json
import matplotlib.pyplot as plt
import os
from pathlib import Path

# Determină căile relative față de locația scriptului
SCRIPT_DIR = Path(__file__).parent.absolute()
ROOT_DIR = SCRIPT_DIR.parent

def plot_performance():
    # Căutăm fișierul de rezultate în rădăcina proiectului
    json_path = ROOT_DIR / "benchmark_results.json"
    if not json_path.exists():
        print("Eroare: benchmark_results.json nu a fost găsit!")
        return

    with open(json_path, 'r') as f:
        data = json.load(f)

    strong = data['strong_scaling']
    procs = [d['procs'] for d in strong]
    speedup = [d['speedup'] for d in strong]
    efficiency = [d['efficiency'] for d in strong]
    times = [d['time'] for d in strong]

    # 1. Strong Scaling Speedup
    plt.figure(figsize=(10, 6))
    plt.plot(procs, speedup, 'o-', linewidth=2, label='Measured Speedup', color='#1f77b4')
    plt.plot(procs, procs, '--', label='Ideal Speedup (Linear)', color='gray', alpha=0.7)
    plt.xlabel('Număr Procese (P)')
    plt.ylabel('Speedup')
    plt.title('Strong Scaling: Accelerarea calculului (Grid 5000x5000)')
    plt.legend()
    plt.grid(True)
    plt.tight_layout()
    plt.savefig(str(SCRIPT_DIR / "speedup.png"), dpi=300)

    # 2. Efficiency
    plt.figure(figsize=(10, 6))
    plt.plot(procs, efficiency, 's-', color='red', label='Efficiency %')
    plt.axhline(y=70, color='green', linestyle='--', label='Target Efficiency (70%)')
    plt.xlabel('Număr Procese (P)')
    plt.ylabel('Eficiență (%)')
    plt.title('Strong Scaling: Eficiența utilizării resurselor')
    plt.legend(loc='upper right')
    plt.grid(True)
    plt.tight_layout()
    plt.savefig(str(SCRIPT_DIR / "efficiency.png"), dpi=300)

    # 3. Weak Scaling
    weak = data['weak_scaling']
    w_procs = [d['procs'] for d in weak]
    w_ratio = [d['time_ratio'] for d in weak]
    
    plt.figure(figsize=(10, 6))
    plt.plot(w_procs, w_ratio, 'd-', color='purple', label='Weak Scaling Ratio')
    plt.axhline(y=1.0, color='gray', linestyle='--', label='Ideal (Constant Time)')
    plt.xlabel('Număr Procese (P)')
    plt.ylabel('T_p / T_1')
    plt.title('Weak Scaling: Scalabilitate aproape ideală')
    plt.grid(True)
    plt.tight_layout()
    plt.savefig(str(SCRIPT_DIR / "weak_scaling.png"), dpi=300)

    # 4. MPI Overhead (Quick Test)
    quick = data['quick_tests'][0]
    labels = ['Sequential', 'MPI (1p)', 'MPI (4p)']
    times = [quick['sequential_time'], quick['mpi1_time'], quick['mpi4_time']]
    
    plt.figure(figsize=(10, 6))
    plt.bar(labels, times, color=['green', 'orange', 'orange'])
    plt.ylabel('Timp (secunde)')
    plt.title('Overhead-ul inițializării MPI (Grid 1000x1000)')
    plt.yscale('log') # Log scale pentru a vedea diferența masivă
    plt.tight_layout()
    plt.savefig(str(SCRIPT_DIR / "overhead.png"), dpi=300)
    
    print("Graficele au fost salvate în folderul experiments/.")

if __name__ == "__main__":
    plot_performance()