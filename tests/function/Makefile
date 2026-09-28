# Copyright 2022-2023 Hewlett Packard Enterprise Development LP.

# Get $PE_ENV from environment to generate directories for output binaries.
# IF $PE_ENV isn't set, just use CRAY
PE_ENV ?= CRAY

TESTS = \
   ./bin/${PE_ENV}/c/c_not_mpi.x \
   ./bin/${PE_ENV}/c/c_time.x \
   ./bin/${PE_ENV}/c/c_type_test.x \
   ./bin/${PE_ENV}/c/c_cmp_diff.x \
   ./bin/${PE_ENV}/c/c_dlopen.x \
   ./bin/${PE_ENV}/c/c_segv.x \
   ./bin/${PE_ENV}/c/c_list.x \
   ./bin/${PE_ENV}/c/c_inc.x \
   ./bin/${PE_ENV}/c/c_half_exit.x \
   ./bin/${PE_ENV}/c/c_signal.x \
   ./bin/${PE_ENV}/c/c_nolib.x \
   ./bin/${PE_ENV}/C++/cpp_bd_test.x \
   ./bin/${PE_ENV}/C++/cpp_cmp_diff.x \
   ./bin/${PE_ENV}/C++/cpp_ds_test1.x \
   ./bin/${PE_ENV}/C++/cpp_arrays.x \
   ./bin/${PE_ENV}/C++/cpp_decomp.x \
   ./bin/${PE_ENV}/C++/cpp_decomp2.x \
   ./bin/${PE_ENV}/C++/cpp_threads.x \
   ./bin/${PE_ENV}/C++/cpp_parallel_threads.x \
   ./bin/${PE_ENV}/C++/cpp_bt.x \
   ./bin/${PE_ENV}/C++/cpp_bt_stress.x \
   ./bin/${PE_ENV}/C++/cpp_arrays_print.x \
   ./bin/${PE_ENV}/C++/cpp_catch.x \
   ./bin/${PE_ENV}/C++/c_moved.x \
   ./bin/${PE_ENV}/C++/cpp_points.x \
   ./bin/${PE_ENV}/fortran/ftn_cmp_diff.x \
   ./bin/${PE_ENV}/fortran/ftn_bd_dt.x

ifndef LOCALHOST
   TESTS := $(TESTS) \
      ./bin/${PE_ENV}/c/c_mpi_comms.x \
      ./bin/${PE_ENV}/fortran/ftn_arrays.x \
      ./bin/${PE_ENV}/fortran/ftn_decomp.x \
      make_ds



endif

# tests which require CTI development libraries to build
CTI_TESTS = \
	./bin/${PE_ENV}/C++/cpp_attach.x

# tests which require GPUs
GPU_TESTS = \
	./bin/CRAY/c/simple_gpu-rocm.x \
	./bin/CRAY/c/simple_gpu-cuda.x \
	./bin/CRAY/fortran/ftn_omp-rocm.x

RAJA_TESTS = \
	./bin/CRAY/raja/view-layout_solution.x \
	./bin/CRAY/raja/kernel-matrix-transpose-local-array_solution.x \
	./bin/CRAY/raja/launch-matrix-transpose-local-array_solution.x \
	./bin/CRAY/raja/reductions_solution.x \
	./bin/CRAY/raja/view-layout-decomp_solution.x

# simpleMPI will also build simpleMPI_error.x
CUDA_TESTS = \
	./bin/CRAY/cuda/$(HOSTNAME)_simpleMPI.x \
	./bin/CRAY/c/simple_gpu-cuda.x

# OpenMP OMPD tests
OMPD_TESTS = \
   ./bin/CRAY/c/c_ompd.x


# you shouldn't have to have cti devel installed to test gdb4hpc, so only build
# tests which require it if we happen to have it
CTI_INC      = -I${CTI_INSTALL_DIR}/include
CTI_LD       = -L${CTI_INSTALL_DIR}/lib -lcommontools_fe


ifneq (${CTI_INSTALL_DIR}no,no)
	TESTS += $(CTI_TESTS)
endif

ifndef LOCALHOST
ifeq ($(PE_ENV),CRAY)
	TESTS += $(GPU_TESTS) $(OMPD_TESTS)
endif
endif

ifdef LOCALHOST
MPI_INC = -I./localMPI
MOVED_MPI_INC = -I../../localMPI
MPI_COBJ = ./localMPI/cmpi.o
MPI_FOBJ = ./localMPI/mpi.o
CC = gcc
CXX = g++
FC     = gfortran
else
CC = cc
CXX = CC
FC = ftn
endif

# ANSI escape codes
IN_YELLOW = \033[1;33m
NO_COLOR = \033[0m

ifndef LOCALHOST
ADDITIONAL_TESTS = kokkosTest shmemTest
endif

all: dirs $(TESTS) $(ADDITIONAL_TESTS)

ifeq (${CTI_INSTALL_DIR}no,no)
	@echo -e "$(IN_YELLOW)CTI development libraries not found, skipping tests that require them."
	@echo -e "$(IN_YELLOW)To build these tests, load CTI development libraries such that pkg-config can find them."
	@echo -e "$(IN_YELLOW)Verify with 'pkg-config --cflags common_tools_fe'."
	@echo -ne "$(NO_COLOR)"
endif

CFLAGS = -g -O0 -I./include $(MPI_INC) -fPIC
CXXFLAGS = -g -O0 -I./include $(MPI_INC) -fPIC
FFLAGS = -g -O0 $(MPI_INC) -fPIC

# programming environments
gnu: all
amd: all
nvidia: all
intel: all

cuda: $(CUDA_TESTS)

dirs:
	mkdir -p ./bin/${PE_ENV}/c
	mkdir -p ./bin/${PE_ENV}/C++
	mkdir -p ./bin/${PE_ENV}/fortran
	mkdir -p ./bin/${PE_ENV}/kokkos
	mkdir -p ./bin/${PE_ENV}/cuda
	mkdir -p ./bin/${PE_ENV}/rust

clean:
	rm -rf ./bin
	rm -rf raja_installation*

./bin/${PE_ENV}/c/%.o : ./src/c/%.c
	$(CC) $(CFLAGS) -c $< -o $@

./bin/${PE_ENV}/C++/cpp_ds_test1.o : ./src/C++/cpp_ds_test1.cpp
	$(CXX) $(CXXFLAGS) -std=c++23 -c $< -o $@

./bin/${PE_ENV}/C++/%.o : ./src/C++/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

./bin/${PE_ENV}/fortran/%.o : ./src/fortran/%.f90
	$(FC) $(FFLAGS) -c $< -o $@

./bin/${PE_ENV}/c/c_not_mpi.x : ./bin/${PE_ENV}/c/c_not_mpi.o 
	$(CC) $(CFLAGS) $^ -o $@

./bin/${PE_ENV}/c/c_time.x : ./bin/${PE_ENV}/c/wtime.o ./bin/${PE_ENV}/c/mtest.o
	$(CC) $(CFLAGS) $^ -o $@

./bin/${PE_ENV}/c/c_type_test.x : ./bin/${PE_ENV}/c/c_type_test.o
	$(CC) $(CFLAGS) $^ -o $@

./bin/${PE_ENV}/c/c_cmp_diff.x : ./bin/${PE_ENV}/c/c_cmp_diff.o
	$(CC) $(CFLAGS) $^ -o $@

./bin/${PE_ENV}/c/libdlopen_test.so : ./src/c/libdlopen_test.c
	$(CC) $(CFLAGS) -shared $^ -o $@

./bin/${PE_ENV}/c/c_dlopen.x :  ./src/c/c_dlopen_test.c ./bin/${PE_ENV}/c/libdlopen_test.so
	$(CC) $(CFLAGS) ./src/c/c_dlopen_test.c -DBIN_PATH=\"${PWD}/bin/${PE_ENV}/c\" -ldl -o $@

./bin/${PE_ENV}/c/c_segv.x : ./bin/${PE_ENV}/c/c_segv.o
	$(CC) $(CFLAGS) $^ -o $@

./bin/${PE_ENV}/c/c_list.x : ./bin/${PE_ENV}/c/c_list.o
	$(CC) $(CFLAGS) $^ -o $@

./bin/${PE_ENV}/c/c_inc.x : ./bin/${PE_ENV}/c/c_inc.o
	$(CC) $(CFLAGS) $^ -o $@

./bin/${PE_ENV}/c/c_half_exit.x : ./bin/${PE_ENV}/c/c_half_exit.o
	$(CC) $(CFLAGS) $^ -o $@

./bin/${PE_ENV}/c/c_signal.x : ./bin/${PE_ENV}/c/c_signal.o
	$(CC) $(CFLAGS) $^ -o $@

./bin/${PE_ENV}/c/c_mpi_comms.x : ./bin/${PE_ENV}/c/c_mpi_comms.o
	$(CC) $(CFLAGS) $^ -o $@

./bin/${PE_ENV}/c/c_libnolib.so : ./src/c/c_libnolib.c
	$(CC) $(CFLAGS) -shared $^ -o $@

./bin/${PE_ENV}/c/c_nolib.x : ./bin/${PE_ENV}/c/c_nolib.o ./bin/${PE_ENV}/c/c_libnolib.so
	cp ./bin/${PE_ENV}/c/c_libnolib.so ./bin/${PE_ENV}/c/libnolib.so
	$(CC) $(CFLAGS) $^ -L./bin/${PE_ENV}/c -lnolib -o $@
	rm ./bin/${PE_ENV}/c/libnolib.so

./bin/${PE_ENV}/c/c_ompd.x : ./src/c/c_ompd.c
	$(CC) -fopenmp $(CFLAGS) $^ -o $@

shmemTest :
	$(MAKE) -i ./bin/${PE_ENV}/c/c_shmem.x 

./bin/${PE_ENV}/c/c_shmem.x : ./bin/${PE_ENV}/c/c_shmem.o
	$(CC) $(CFLAGS) $^ -o $@

./bin/${PE_ENV}/C++/cpp_bd_test.x : ./bin/${PE_ENV}/C++/cpp_bd_test.o
	$(CXX) $(CXXFLAGS) $^ -o $@

./bin/${PE_ENV}/C++/cpp_cmp_diff.x : ./bin/${PE_ENV}/C++/cpp_cmp_diff.o
	$(CXX) $(CXXFLAGS) $^ -o $@

./bin/${PE_ENV}/C++/cpp_ds_test1.x : ./bin/${PE_ENV}/C++/cpp_ds_test1.o
	$(CXX) $(CXXFLAGS) -std=c++23 $^ -o $@

./bin/${PE_ENV}/C++/cpp_arrays.x : ./bin/${PE_ENV}/C++/cpp_arrays.o
	$(CXX) $(CXXFLAGS) $^ -o $@

./bin/${PE_ENV}/C++/cpp_decomp.x : ./bin/${PE_ENV}/C++/cpp_decomp.o
	$(CXX) $(CXXFLAGS) $^ -o $@

./bin/${PE_ENV}/C++/cpp_decomp2.x : ./bin/${PE_ENV}/C++/cpp_decomp2.o
	$(CXX) $(CXXFLAGS) $^ -o $@

./bin/${PE_ENV}/C++/cpp_threads.x : ./bin/${PE_ENV}/C++/cpp_threads.o
	$(CXX) $(CXXFLAGS) $^ -o $@ -pthread

./bin/${PE_ENV}/C++/cpp_parallel_threads.x : ./bin/${PE_ENV}/C++/cpp_parallel_threads.o
	$(CXX) $(CXXFLAGS) $^ -o $@ -pthread

./bin/${PE_ENV}/C++/cpp_bt.x : ./bin/${PE_ENV}/C++/cpp_bt.o
	$(CXX) $(CXXFLAGS) $^ -o $@

./bin/${PE_ENV}/C++/cpp_bt_stress.x : ./bin/${PE_ENV}/C++/cpp_bt_stress.o
	$(CXX) $(CXXFLAGS) $^ -o $@

./bin/${PE_ENV}/C++/cpp_arrays_print.x : ./bin/${PE_ENV}/C++/cpp_arrays_print.o
	$(CXX) $(CXXFLAGS) $^ -o $@

./bin/${PE_ENV}/C++/cpp_points.x : ./bin/${PE_ENV}/C++/cpp_points.o
	$(CXX) $(CXXFLAGS) $^ -o $@

./bin/${PE_ENV}/C++/c_moved.x : ./src/c/c_list.c
	( cp src/c/c_list.c src/C++; cd src/C++; $(CC) $(CFLAGS) $(MOVED_MPI_INC) c_list.c -o ../../$@; rm c_list.c )

./bin/${PE_ENV}/c/libfoo.so : ./src/c/libfoo.c
	gcc -g -O0 -fPIC -shared $^ -o $@

./bin/${PE_ENV}/c/libbar.so : ./src/c/libbar.c
	gcc -g -O0 -fPIC -shared $^ -o $@

./bin/${PE_ENV}/C++/cpp_catch.x : ./src/C++/cpp_catch.cpp ./bin/${PE_ENV}/c/libfoo.so ./bin/${PE_ENV}/c/libbar.so
	$(CXX) $(CXXFLAGS) -DBD_DT=\"./bin/${PE_ENV}\"s ./src/C++/cpp_catch.cpp -o $@ -ldl

./bin/${PE_ENV}/C++/cpp_attach.x : ./bin/${PE_ENV}/C++/cpp_attach.o ./bin/${PE_ENV}/cti_launch.x
	$(CXX) $(CXXFLAGS) ./bin/${PE_ENV}/C++/cpp_attach.o -o $@

./bin/${PE_ENV}/fortran/ftn_cmp_diff.x : ./bin/${PE_ENV}/fortran/ftn_cmp_diff.o $(MPI_COBJ)
	$(FC) $(FFLAGS) $^ -o $@

./bin/${PE_ENV}/fortran/ftn_arrays.x : ./bin/${PE_ENV}/fortran/ftn_arrays.o $(MPI_FOBJ)
	$(FC) $(FFLAGS) $^ -o $@

./bin/${PE_ENV}/fortran/ftn_decomp.x : ./bin/${PE_ENV}/fortran/ftn_decomp.o $(MPI_FOBJ)
	$(FC) $(FFLAGS) $^ -o $@

./bin/${PE_ENV}/fortran/ftn_bd_dt.x : ./bin/${PE_ENV}/fortran/ftn_bd_dt.o $(MPI_COBJ)
	$(FC) $(FFLAGS) $^ -o $@

./bin/${PE_ENV}/cti_launch.o: ./src/cti_launch.cpp
	$(CXX) $(CXXFLAGS) -c -I./include $(CTI_INC) $< -o $@

./bin/${PE_ENV}/cti_launch.x : ./bin/${PE_ENV}/cti_launch.o
	$(CXX) $(CXXFLAGS) $^ -o $@ $(CTI_LD)

./bin/${PE_ENV}/raja/view-layout-decomp_solution.x: get_raja ./raja_installation/lib/libRAJA.a ./raja_installation/lib/libcamp.a ./src/raja/view-layout-decomp_solution.cpp
	mkdir -p ./bin/${PE_ENV}/raja
	$(CXX) $(CXXFLAGS) -I./raja_installation/include ./src/raja/view-layout-decomp_solution.cpp ./raja_installation/lib/libRAJA.a ./raja_installation/lib/libcamp.a -o $@

./bin/${PE_ENV}/raja/view-layout_solution.x: get_raja ./raja_installation/lib/libRAJA.a ./raja_installation/lib/libcamp.a ./src/raja/view-layout_solution.cpp
	mkdir -p ./bin/${PE_ENV}/raja
	$(CXX) $(CXXFLAGS) -I./raja_installation/include ./src/raja/view-layout_solution.cpp ./raja_installation/lib/libRAJA.a ./raja_installation/lib/libcamp.a -o $@

./bin/${PE_ENV}/raja/kernel-matrix-transpose-local-array_solution.x: get_raja ./raja_installation/lib/libRAJA.a ./raja_installation/lib/libcamp.a ./src/raja/kernel-matrix-transpose-local-array_solution.cpp
	mkdir -p ./bin/${PE_ENV}/raja
	$(CXX) $(CXXFLAGS) -I./raja_installation/include ./src/raja/kernel-matrix-transpose-local-array_solution.cpp ./raja_installation/lib/libRAJA.a ./raja_installation/lib/libcamp.a -o $@

./bin/${PE_ENV}/raja/launch-matrix-transpose-local-array_solution.x: get_raja ./raja_installation/lib/libRAJA.a ./raja_installation/lib/libcamp.a ./src/raja/launch-matrix-transpose-local-array_solution.cpp
	mkdir -p ./bin/${PE_ENV}/raja
	$(CXX) $(CXXFLAGS) -I./raja_installation/include ./src/raja/launch-matrix-transpose-local-array_solution.cpp ./raja_installation/lib/libRAJA.a ./raja_installation/lib/libcamp.a -o $@

./bin/${PE_ENV}/raja/reductions_solution.x: get_raja ./raja_installation/lib/libRAJA.a ./raja_installation/lib/libcamp.a ./src/raja/reductions_solution.cpp
	mkdir -p ./bin/${PE_ENV}/raja
	$(CXX) $(CXXFLAGS) -I./raja_installation/include ./src/raja/reductions_solution.cpp ./raja_installation/lib/libRAJA.a ./raja_installation/lib/libcamp.a -o $@

ROCM_SCRIPT = ./tests.py.data/rocm/$(HOSTNAME).sh
CUDA_SCRIPT = ./tests.py.data/cuda/$(HOSTNAME).sh

./bin/CRAY/c/simple_gpu-rocm.x : ./src/c/simple_gpu.c $(wildcard $(ROCM_SCRIPT))
	if [ -f $(ROCM_SCRIPT) ] ; then \
		$(ROCM_SCRIPT) make $< $@; \
	fi

./bin/CRAY/c/simple_gpu-cuda.x : ./src/c/simple_gpu.c $(wildcard $(CUDA_SCRIPT))
	if [ -f $(CUDA_SCRIPT) ] ; then \
		$(CUDA_SCRIPT) make $< $@; \
	fi

./bin/CRAY/fortran/ftn_omp-rocm.x : ./src/fortran/ftn_omp.f90 $(wildcard $(ROCM_SCRIPT))
	if [ -f $(ROCM_SCRIPT) ] ; then \
		$(ROCM_SCRIPT) make $< $@; \
	fi

CUDA_SIMPLEMPI_SCRIPT = ./tests.py.data/cuda/simpleMPI_$(HOSTNAME).sh

./bin/CRAY/cuda/$(HOSTNAME)_simpleMPI.x : dirs ./src/cuda/simpleMPI.cpp ./src/cuda/simpleMPI.h ./src/cuda/simpleMPI.cu
	if [ -f $(CUDA_SIMPLEMPI_SCRIPT) ] ; then \
		$(CUDA_SIMPLEMPI_SCRIPT) make $< $@; \
	fi

raja: $(RAJA_TESTS)

get_raja:
	if [ ! -e "./raja_installation" ]; then \
		wget "https://arti.hpc.amslabs.hpecorp.net/artifactory/pe-internal-third-party-generic-local/raja/raja_installation_$(shell uname -i).tar.gz" && \
		tar -xf "./raja_installation_$(shell uname -i).tar.gz"; \
	fi

kokkosTest:
	$(MAKE) -i kokkosCanFail

kokkosCanFail: ./bin/${PE_ENV}/kokkos/TeamVectorLoop.host

Kokkos:
	git clone --depth=1 --branch=master https://github.com/kokkos/kokkos.git Kokkos

Kokkos/kokkos: Kokkos
	mkdir Kokkos/kokkos

Kokkos/kokkos/Makefile: Kokkos/kokkos
	( cd Kokkos/kokkos; cmake -DCMAKE_CXX_COMPILER=CC .. )

Kokkos/kokkos/core/src/libcokkoscore.a: Kokkos/kokkos/Makefile
	$(MAKE) -C Kokkos/kokkos

./bin/${PE_ENV}/kokkos/TeamVectorLoop.host : src/kokkos/team_vector_loop.cpp Kokkos/kokkos/core/src/libcokkoscore.a
	$(MAKE) CXX=CC -C src/kokkos
	mv src/kokkos/TeamVectorLoop.host ./bin/${PE_ENV}/kokkos/TeamVectorLoop.host

make_ds : ./bin/${PE_ENV}/rust/rust_ds

./bin/${PE_ENV}/rust/rust_ds: ./src/rust/rust_ds.rs
	-rustc -g $< -o $@
