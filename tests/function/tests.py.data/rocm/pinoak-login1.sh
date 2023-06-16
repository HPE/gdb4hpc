#!/bin/bash

PARTITION='-p bardpeak'

if [[ $1 == args ]]; then
    echo $PARTITION
    exit 0
fi


PE_ENV="${PE_ENV-CRAY}"

#module load PrgEnv-amd
module load rocm
module load craype-accel-amd-gfx90a


export OMP_DEBUG=enabled
export MV2_USE_CUDA=1

echo "in pea2k.sh" $*


if [[ $1 == make ]]; then
    echo  cc -gheterogeneous-dwarf -O0 -fopenmp -DUSE_MPI ./src/c/simple_gpu.c -o ./bin/${PE_ENV}/c/simple_gpu-rocm.x
    cc -gheterogeneous-dwarf -O0 -fopenmp -DUSE_MPI ./src/c/simple_gpu.c -o ./bin/${PE_ENV}/c/simple_gpu-rocm.x
    echo ftn -g --gheterogeneous-dwarf O0 -h omp ./src/fortran/ftn_omp.f90 -o ./bin/${PE_ENV}/fortran/ftn_omp-rocm.x
    ftn -g -gheterogeneous-dwarf -O0 -h omp ./src/fortran/ftn_omp.f90 -o ./bin/${PE_ENV}/fortran/ftn_omp-rocm.x
   exit 0
fi

if [[ $1 == run ]]; then
   echo run
   srun -n 1 $PARTITION ./bd_dt/c/simple_gpu-rocm.x
   exit 0
fi


if [[ $1 == launch ]]; then
   echo launch
   shift
   echo $*
   echo `which gdb4hpc`
   echo `which rocgdb`
   $*
   exit 0
fi


echo 'expecting make or run'
exit -1

