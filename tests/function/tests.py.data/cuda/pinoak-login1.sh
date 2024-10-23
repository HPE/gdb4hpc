#!/bin/bash

if [ "$1" == "make" ]; then
    # pinoak as of october 2024
    module load cce/17
    module load cuda/12
    module load craype-accel-nvidia80

    set -ex

    cc -DUSE_MPI -fopenmp -g -O0 ./src/c/simple_gpu.c -o ./bin/CRAY/cuda/simple_gpu-cuda.x -L"$CUDA_HOME"/lib64

    exit 0
fi

if [[ "$1" == "args" ]]; then
    echo "-p allgriz"
    exit 0
fi

echo 'other options not implemented'
exit -1
