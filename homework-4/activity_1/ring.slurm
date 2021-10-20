#!/bin/bash
#SBATCH --job-name=Ring_Bcast
#SBATCH --partition=exclusive
##SBATCH --account=ics632
#SBATCH --time=0-01:00:00 ## time format is DD-HH:MM:SS

#SBATCH --cpus-per-task=1
#SBATCH --nodes=1
#SBATCH --tasks-per-node=19

##SBATCH --constraint="ib_hdr"  # will allow a mix of HDR200 and HDR100 nodes
##SBATCH --constraint="ib_hdr100"  # explicitly only allow HDR100 nodes

#SBATCH --distribution="*:*:*"  # Set the distribution to defaults if doing sbatch from interactive session

#SBATCH --error=Ring_Bcast-%A.err ## %A - filled with jobid
#SBATCH --output=Ring_Bcast-%A.out ## %A - filled with jobid

## Useful for remote notification
##SBATCH --mail-type=BEGIN,END,FAIL,REQUEUE,TIME_LIMIT_80
##SBATCH --mail-user=user@test.org

## All options and environment variables found on schedMD site: http://slurm.schedmd.com/sbatch.html
## Intel MPI manual: https://software.intel.com/en-us/mpi-refman-lin-html
## Some Intel MPI Fabricenvironment variables:  https://software.intel.com/en-us/node/528821



# Configure the Intel MPI parameters
# dapl is supported by Intel 2018, but is deprecated in newer version of Intel
module load mpi/OpenMPI/2.1.2-GCC-6.4.0-2.28
export I_MPI_FABRICS=shm:dapl  
export I_MPI_PMI_LIBRARY=/lib64/libpmi.so

# Prepare and execute the MPI application
#module load lib/libfabric # required if you do not already have the intelchain loaded
mpicc -Ofast -o bcast_skeleton bcast_skeleton.c
#echo ${SLURM_NTASKS}
mpirun -n ${SLURM_NTASKS} ./bcast_skeleton default_bcast