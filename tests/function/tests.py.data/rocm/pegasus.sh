#!/bin/bash

module unload rocm
module load rocm/5.2.0

# 15.0.0.2 and 15.0.0.3 cause an unknown cfa error on the back side
module swap cce/15.0.0
module load craype-accel-amd-gfx906
export OMP_DEBUG=enabled
export MV2_USE_CUDA=1

if [ x$1 == xmake ]; then
    cc -DUSE_MPI -fopenmp -g -ggdb -O0 -o $3 $2
    exit 0
fi

if [ x$1 == xrun ]; then
    srun -n 1 -p amdMI60 ./bd_dt/c/simple_gpu.x 0
    exit 0
fi

if [ x$1 == xlaunch ]; then
    shift
    batch=''
    for arg in "$@"
    do
	if [ $arg == "-b" ]; then
	    batch="next"
	elif [ x$batch == xnext ]; then
	    batch=$arg
	fi
    done

    if [ x$batch != x ]; then
	sed -i 's/--launcher-args="/--launcher-args="-p amdMI60 /' $batch
    fi

    echo "running: $@"
    $@

    exit 0
fi

echo 'expecting make, run, or launch'
exit -1

