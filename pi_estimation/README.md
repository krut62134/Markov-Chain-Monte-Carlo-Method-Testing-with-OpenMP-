# Markov Chain Monte Carlo Methods with Parallel Computing

Implementation and performance analysis of Monte Carlo and MCMC algorithms using OpenMP and MPI.

## Projects

### 1. [π Estimation](./pi_estimation/)
Monte Carlo integration with OpenMP parallelization (Fall 2023 course project).

### 2. Bayesian MCMC Inference
Metropolis-Hastings sampler with parallel implementations for comparison.

---

## Bayesian MCMC

### Problem Statement
Inferring coin bias from observed flips using Bayesian inference. Given 7 heads in 10 flips, estimate the posterior distribution of the bias parameter θ.

- Prior: Beta(2,2)
- Likelihood: Binomial(n=10, p=θ)
- Analytical posterior: Beta(9,5)

The analytical solution lets us validate the MCMC implementation.

### Code Structure
```
beta_binomial_mcmc.c           # Serial baseline
beta_binomial_mcmc_parallel.c  # OpenMP version
beta_binomial_mcmc_mpi.c       # MPI version
```

### Compilation
```bash
gcc -o beta_binomial_mcmc beta_binomial_mcmc.c -lm
gcc -fopenmp -o beta_binomial_mcmc_parallel beta_binomial_mcmc_parallel.c -lm
mpicc -o beta_binomial_mcmc_mpi beta_binomial_mcmc_mpi.c -lm
```

### Usage
```bash
./beta_binomial_mcmc                          # Serial: 10M samples
./beta_binomial_mcmc_parallel 4 10000         # OpenMP: 4 chains, 10k samples each
ibrun -n 48 ./beta_binomial_mcmc_mpi 10000000 # MPI: 48 chains, 10M samples each
```

### Results

**Posterior Recovery**

![Posterior](./posterior_validation.png)

The sampler recovers the Beta(9,5) analytical posterior, confirming correctness.

**OpenMP vs MPI Performance**

![Comparison](./openmp_vs_mpi.png)

OpenMP maintains ~0.82s across 1-48 cores. MPI stays around 1.77s from 1-288 processes.

The chains are completely independent with no communication between them. This means:
- OpenMP works well since threads share memory efficiently
- MPI adds overhead (process initialization, barriers) that costs ~1 second
- Since computation per chain is ~0.8s, the MPI overhead kills any benefit

OpenMP is limited to single node (48 cores here). MPI would be necessary for larger systems, but this particular problem is too lightweight to benefit from distributed computing.

**Convergence Check**

![Traces](./trace_plots.png)

Gelman-Rubin statistic: R̂ = 1.0003 (should be < 1.1)

Four independent chains all converge to the same distribution. Acceptance rate ~0.22, close to the theoretical optimum of 0.234 for 1D problems.

### Performance Data

| Method | Cores | Time (10M samples) |
|--------|-------|-------------------|
| Serial | 1 | 3.20s |
| OpenMP | 48 | 0.82s |
| MPI | 288 | 1.77s |

### Scripts
- `test_openmp_scaling.sh` - Performance testing
- `validate.py` - Check posterior against analytical
- `plot_comparison.py` - Generate comparison plots
- `plot_traces.py` - Convergence diagnostics

### Notes

The MPI implementation works correctly but doesn't help here. For problems with heavier computation per chain or those requiring inter-process communication, MPI would make more sense. This is a case where understanding when not to use a tool matters.

The acceptance rate can be tuned by adjusting the proposal standard deviation. Currently using 0.9 which gives ~22% acceptance.

---

## Author

Krut Patel

## License

MIT - see LICENSE file
