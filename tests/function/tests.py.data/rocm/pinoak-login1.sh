#!/bin/bash

PE_ENV="${PE_ENV-CRAY}"

#module load PrgEnv-amd
#module load craype-accel-amd-gfx90a

AFAR=/lus/scratch/afar/rocm-afar001-348
AFAR_CLANG=${AFAR}/bin/amdclang++
AFAR_ARGS="-fopenmp -fopenmp-targets=amdgcn-amd-amdhsa -Xopenmp-target=amdgcn-amd-amdhsa -march=gfx90a"
AFAR_LIBS="-L${CRAY_MPICH_BASEDIR}/amd/5.0/lib -L${CRAY_MPICH_BASEDIR}/../gtl/lib -L/opt/cray/pe/libsci/21.08.1.2/AMD/4.0/x86_64/lib -L/opt/cray/pe/dsmml/0.2.1/dsmml/lib ${CRAY_XPMEM_POST_LINK_OPTS}"
AFAR_LINK_OPTS="-Wl,--as-needed,-lsci_amd_mpi,--no-as-needed -Wl,--as-needed,-lsci_amd,--no-as-needed -ldl -Wl,--as-needed,-lmpi_amd,--no-as-needed -lmpi_gtl_hsa -Wl,--as-needed,-ldsmml,--no-as-needed -lxpmem"

#AFAR_LIBS="-L/opt/cray/pe/mpich/8.1.12.134/ofi/amd/4.4/lib -L/opt/cray/pe/mpich/8.1.12.134/gtl/lib -L/opt/cray/pe/libsci/21.08.1.2/AMD/4.0/x86_64/lib -L/opt/cray/pe/dsmml/0.2.1/dsmml//lib -L/opt/cray/xpmem/2.3.2-2.2_1.16__g9ea452c.shasta/lib64 -Wl,--as-needed,-lsci_amd_mpi,--no-as-needed -Wl,--as-needed,-lsci_amd,--no-as-needed -ldl -Wl,--as-needed,-lmpi_amd,--no-as-needed -lmpi_gtl_hsa -Wl,--as-needed,-ldsmml,--no-as-needed -lxpmem"
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$AFAR/hsa/lib:$AFAR/hip/lib

export PATH=$AFAR/bin:$PATH


export OMP_DEBUG=enabled
export MV2_USE_CUDA=1

echo "in pinoak.sh" $*

if [ x$1 == xmake ]; then
    echo ${AFAR_CLANG} -DUSE_MPI -g -ggdb -O0 ${AFAR_ARGS} -I${CRAY_MPICH_BASEDIR}/amd/5.0/include $AFAR_LIBS $AFAR_LINK_OPTS -o ./bin/${PE_ENV}/c/simple_gpu.x ./src/c/simple_gpu.c
    ${AFAR_CLANG} -DUSE_MPI -g -ggdb -O0 ${AFAR_ARGS} -I${CRAY_MPICH_BASEDIR}/amd/5.0/include $AFAR_LIBS $AFAR_LINK_OPTS -o ./bin/${PE_ENV}/c/simple_gpu.x ./src/c/simple_gpu.c
# -lxpmem
   exit 0
fi

if [ x$1 == xrun ]; then
   echo run
   srun -n 1 ./bd_dt/c/simple_gpu.x
   exit 0
fi

if [ x$1 == xlaunch ]; then
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

