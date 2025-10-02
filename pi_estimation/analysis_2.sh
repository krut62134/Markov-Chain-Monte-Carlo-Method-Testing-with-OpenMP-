#!/bin/bash
  


# Loop through N values from 10^1 to 10^10
for (( i=1; i<=12; i++ )); do
    N=$((10**i))

        ./mcmc_parallel $N 1 >> "mcmc_serial.dat"
done
