# Monte Carlo & MCMC π Estimation with OpenMP Parallelization

A high-performance computing project implementing Markov Chain Monte Carlo (MCMC) method for π approximation with OpenMP parallelization, including comprehensive error analysis and scaling tests.

## Overview

This project parallelizes the Monte Carlo π estimation algorithm using OpenMP and analyzes:
- Error convergence (E ∝ N^(-1/2))
- Strong scaling efficiency
- Weak scaling performance
- Thread count impact on accuracy and performance

## Project Structure

```
project/
├── mcmc.c                  # Serial MCMC implementation
├── mcmc_parallel.c         # OpenMP parallel implementation
├── mcmc_parallel_1.c       # Alternative parallel version (20 threads fixed)
├── analysis_1.sh           # Thread scaling tests (70-100 threads)
├── analysis_2.sh           # Serial baseline runs (N=10^1 to 10^12)
├── problem_1.sh            # Single iteration test
├── problem_2.sh            # Multiple iteration analysis (500 runs)
├── job.sh                  # SLURM job script (110 cores, 120 min)
├── job_2.sh                # SLURM job script (5 cores, 220 min)
├── job_3.sh                # SLURM job script (120 cores, 300 min)
└── Project.ipynb           # Analysis and visualization notebook
```

## Requirements

- C compiler with OpenMP support (gcc recommended)
- Python 3.x with numpy, matplotlib
- SLURM (for HPC cluster submission)
- GNU Parallel (for `problem_2.sh`)

## Compilation

```bash
# Serial version
gcc -o mcmc mcmc.c -lm

# Parallel version
gcc -fopenmp -o mcmc_parallel mcmc_parallel.c -lm
```

## Usage

### Single Run
```bash
./mcmc_parallel <N> <num_threads>
# Example: ./mcmc_parallel 1000000 10
```

Output format: `num_threads N pi_estimate error_estimate time`

### Batch Analysis
```bash
# Run scaling tests
bash analysis_1.sh  # Thread scaling
bash analysis_2.sh  # Problem size scaling
bash problem_2.sh   # Multiple iterations for statistical analysis
```

### HPC Cluster Submission
```bash
sbatch job.sh    # Thread scaling analysis
sbatch job_2.sh  # Serial baseline
sbatch job_3.sh  # Multiple iteration analysis
```

## Key Findings

### Error Analysis
- Both MCMC and standard Monte Carlo follow E ∝ N^(-1/2) convergence
- **Critical finding**: Error plateaus at ~10^(-6) beyond N=10^10 (numerical precision limit)
- Thread count has minimal impact on error estimates

### Performance Analysis
- **Optimal thread count**: 27 threads for this implementation
- Strong scaling: Near-linear speedup up to ~10 threads, then degrades
- Weak scaling: Maximum speedup ~30x regardless of problem size beyond optimal thread count
- Algorithm is not computationally intensive enough for efficient scaling beyond 30 threads

### Limitations Identified
1. π approximation is too simple for effective large-scale parallelization
2. Random number generation becomes bottleneck
3. Numerical precision limits accuracy beyond N=10^10

## Visualization

The Jupyter notebook (`Project.ipynb`) generates:
- N vs Error plots (log-log)
- Thread vs Error correlation
- Strong scaling analysis
- Weak scaling analysis
- N vs Time comparisons

## Proposed Improvements

1. **Random Number Generation**: Replace `rand_r()` with higher-quality PRNG
2. **Algorithm Complexity**: Test with more computationally intensive integrals
3. **Load Balancing**: Investigate dynamic scheduling strategies
4. **Numerical Precision**: Use arbitrary precision libraries for N > 10^10
5. **Memory Optimization**: Reduce overhead for better cache utilization

## Technical Details

### OpenMP Implementation
```c
#pragma omp parallel for reduction(+:inside_circle_count) private(seed)
```
- `reduction(+:inside_circle_count)`: Thread-safe accumulation
- `private(seed)`: Per-thread RNG seed to avoid race conditions

### MCMC Method
Points are sequentially generated in [-1,1]×[-1,1], forming an implicit Markov chain. Acceptance criterion: point inside unit circle (x²+y²≤1). π ≈ 4×(accepted/total).

## Performance Data

Representative results:
- N=10^6, 10 threads: ~0.5s, error ~10^(-3)
- N=10^9, 27 threads: ~15s, error ~10^(-5)
- N=10^12, 100 threads: Minimal speedup gain vs 27 threads

## Author

Krut Patel  
UMass Dartmouth - High Performance Computing Project

## License

Academic project - contact author for usage permissions.
