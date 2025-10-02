#!/bin/bash

#SBATCH -n 120
#SBATCH -t 300
#SBATCH -p umd-cscdr-cpu
#SBATCH -e %j.e
#SBATCH -o %j.o


bash problem_2.sh
