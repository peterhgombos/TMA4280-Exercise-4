#!/bin/bash
#PBS -q optimist
#PBS -lnodes=2:ppn=12
#PBS -lwalltime=00:00:30
#PBS -V
#PBS -lpmem=2MB
#PBS -A freecycle

cd $PBS_O_WORKDIR
mpirun -np 12 a.out > output
 
exit 0
