#!/bin/bash

echo "Testing serial baseline..."
./beta_binomial_mcmc | grep "Time:"

echo ""
echo "Testing OpenMP scaling (1-48 threads)..."
echo "threads time" > openmp_scaling.dat

for threads in 1 2 4 8 16 24 32 48; do
    export OMP_NUM_THREADS=$threads
    echo "Testing $threads threads..."
    time_output=$(./beta_binomial_mcmc_parallel $threads 10000000 | grep "Time:" | awk '{print $2}')
    echo "$threads $time_output" >> openmp_scaling.dat
    echo "$threads threads: $time_output seconds"
done

echo ""
echo "Results saved to openmp_scaling.dat"
