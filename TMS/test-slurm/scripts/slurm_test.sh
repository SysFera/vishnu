#!/bin/sh
# SBATCH -N4
#SBATCH -J myFristJob
#SBATCH -o myJob-%J.out
#SBATCH -e myJob-%J.err
#SBATCH -t 01:02:20
srun hostname
sleep 120
echo "Hello word!...."
