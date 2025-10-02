#!/bin/bash

# Number of times to execute the script
total_executions=500

# Output file to store the data

output_file="multi_iterations.dat"

#echo "#N Error" >> $output_file
# Clear the output file if it exists (uncomment this if you want to start with a fresh file each time)
# > "$output_file"

parallel -j 100 "bash problem_1.sh >> $output_file" ::: $(seq "$total_executions")
#bash problem_1a1.sh >> $output_file ::: $(seq "$total_executions")

#oinclude <stdlib.h>

