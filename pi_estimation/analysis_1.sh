#!/bin/bash

    
for (( threads=70; threads<=100; threads=threads+10 )); do
    ./mcmc_parallel 1000000000000 $threads >> "mcmc_parallel.dat"
done
