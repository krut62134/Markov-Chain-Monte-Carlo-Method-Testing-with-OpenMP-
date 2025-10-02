#!/bin/bash

#SBATCH -n 110
#SBATCH -t 120
#SBATCH -p umd-cscdr-cpu
#SBATCH -e %j.e
#SBATCH -o %j.o


bash analysis_1.sh
