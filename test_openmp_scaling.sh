#!/bin/bash

echo "Testing serial baseline..."
./beta_binomial_mcmc | grep "Time:"

echo ""
echo "Testing parallel scaling (2^0 to 2^8)..."
echo "threads,time" > scaling_results.csv

for i in {0..8}; do
    threads=$((2**i))
    export OMP_NUM_THREADS=$threads
    echo "Testing $threads threads (2^$i)..."
    time_output=$(./beta_binomial_mcmc_parallel $threads 10000000 | grep "Time:" | awk '{print $2}')
    echo "$threads,$time_output" >> scaling_results.csv
    echo "$threads threads: $time_output seconds"
done

echo ""
echo "Results saved to scaling_results.csv"
