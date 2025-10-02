#!/bin/bash

echo "method processes_or_threads time" > comparison.dat

# Serial baseline
echo "serial 1 3.200" >> comparison.dat

# OpenMP results (from your openmp_scaling.dat)
awk '{if(NR>1) print "openmp", $1, $2}' openmp_scaling.dat >> comparison.dat

# MPI results (from your manual tests)
echo "mpi 1 0.791" >> comparison.dat
echo "mpi 2 1.766" >> comparison.dat
echo "mpi 48 1.772" >> comparison.dat
echo "mpi 100 1.771" >> comparison.dat
echo "mpi 288 1.772" >> comparison.dat
