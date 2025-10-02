#!/bin/bash

#SBATCH -n 5
#SBATCH -t 220
#SBATCH -p umd-cscdr-cpu
#SBATCH -e %j.e
#SBATCH -o %j.o


bash analysis_2.sh
