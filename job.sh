#!/bin/bash
#PBS -q optimist
#PBS -lnodes=1:ppn=12
#PBS -lwalltime=00:00:30
#PBS -V
#PBS -lpmem=2MB

cd $PBS_O_WORKDIR
mpirun -np 12 TMA4280-Exercise-4/a.out
 
exit 0
