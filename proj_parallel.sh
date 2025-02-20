#!/bin/bash
#SBATCH --nodes=1               # Use 1 node
#SBATCH --ntasks-per-node=5     # Use 5 tasks per node
#SBATCH --partition=compute2011
#SBATCH --exclusive
#SBATCH --job-name=my_mpi_job
#SBATCH --output=mpi_job_output_%j.txt

mpirun -np 5 ./proj_parallel           # Run with 5 processors
