#!/bin/bash

PE_ENV="${PE_ENV-CRAY}"

#module load PrgEnv-amd
module load craype-accel-amd-gfx908

export OMP_DEBUG=enabled
export MV2_USE_CUDA=1

echo "in pea2k.sh" $*


if [ x$1 == xmake ]; then
    echo  cc -gheterogeneous-dwarf -O0 -fopenmp -DUSE_MPI ./src/c/simple_gpu.c -o ./bin/${PE_ENV}/c/simple_gpu-rocm.x
    cc -gheterogeneous-dwarf -O0 -fopenmp -DUSE_MPI ./src/c/simple_gpu.c -o ./bin/${PE_ENV}/c/simple_gpu-rocm.x
    echo ftn -g --gheterogeneous-dwarf O0 -h omp ./src/fortran/ftn_omp.f90 -o ./bin/${PE_ENV}/fortran/ftn_omp-rocm.x
    ftn -g -gheterogeneous-dwarf -O0 -h omp ./src/fortran/ftn_omp.f90 -o ./bin/${PE_ENV}/fortran/ftn_omp-rocm.x
   exit 0
fi

if [ x$1 == xrun ]; then
   echo run
   srun -n 1 ./bd_dt/c/simple_gpu-rocm.x
   exit 0
fi

if [ x$1 == xlaunch ]; then
    batch=''
    nextIsBatch=0
    for opt in "$@"; do
	if [ $opt == '-b' ]; then
	    nextIsBatch=1
	elif [ $nextIsBatch == 1 ]; then
	    nextIsBatch=0
	    batch=$opt
	fi
    done

    if [ -n "$batch" ]; then
	sed -i '/^launch / s/$/ --launcher-args="-p mi100"/' $batch
    fi
	    
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

