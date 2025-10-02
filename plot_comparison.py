import numpy as np
import matplotlib.pyplot as plt

data = np.loadtxt('comparison.dat', dtype=str, skiprows=1)
methods = data[:, 0]
threads = data[:, 1].astype(int)
times = data[:, 2].astype(float)

openmp_mask = methods == 'openmp'
mpi_mask = methods == 'mpi'

plt.figure(figsize=(10, 6))
plt.semilogx(threads[openmp_mask], times[openmp_mask], 'o-', label='OpenMP (shared memory)', linewidth=2, markersize=8)
plt.semilogx(threads[mpi_mask], times[mpi_mask], 's-', label='MPI (distributed memory)', linewidth=2, markersize=8)
plt.axhline(y=0.82, color='gray', linestyle='--', alpha=0.5, label='Optimal time (~0.82s)')
plt.xlabel('Number of Cores/Processes', fontsize=12)
plt.ylabel('Computation Time (seconds)', fontsize=12)
plt.title('Parallelization Method Comparison: OpenMP vs MPI\nBayesian MCMC (10M samples per chain)', fontsize=14)
plt.legend(fontsize=11)
plt.grid(True, alpha=0.3)
plt.ylim(0.7, 1.9)
plt.savefig('openmp_vs_mpi.png', dpi=150, bbox_inches='tight')
