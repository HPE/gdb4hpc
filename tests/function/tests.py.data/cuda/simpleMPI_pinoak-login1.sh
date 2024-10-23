#!/bin/bash

if [ "$1" == "make" ]; then
    # pinoak as of october 2024
    module load cce/17
    module load cuda/12

    set -ex

    function make_simpleMPI() {
        nvcc -g -G -O0 -c -ccbin=cc -gencode arch=compute_80,code=sm_80 -allow-unsupported-compiler ./src/cuda/"$1".cu -o ./bin/CRAY/cuda/pinoak-login1_"$1"_gpu.o
        CC -g -O0 -c ./src/cuda/"$1".cpp -o ./bin/CRAY/cuda/pinoak-login1_"$1"_cpu.o
        CC ./bin/CRAY/cuda/pinoak-login1_"$1"_gpu.o ./bin/CRAY/cuda/pinoak-login1_"$1"_cpu.o -o ./bin/CRAY/cuda/pinoak-login1_"$1".x -L"$CUDA_HOME"/lib64
    }

    make_simpleMPI "simpleMPI"
    make_simpleMPI "simpleMPI_error"

    exit 0
fi

if [[ "$1" == "args" ]]; then
    echo "-p allgriz"
    exit 0
fi

echo 'other options not implemented'
exit -1
