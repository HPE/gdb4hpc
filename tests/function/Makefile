# Copyright 2022 Hewlett Packard Enterprise Development LP.

# Get $PE_ENV from environment to generate directories for output binaries.
# IF $PE_ENV isn't set, just use CRAY
PE_ENV ?= CRAY

TESTS = \
   ./bin/${PE_ENV}/c/c_time.x \
   ./bin/${PE_ENV}/c/c_type_test.x \
   ./bin/${PE_ENV}/c/c_cmp_diff.x \
   ./bin/${PE_ENV}/c/c_dlopen.x \
   ./bin/${PE_ENV}/c/c_segv.x \
   ./bin/${PE_ENV}/c/c_list.x \
   ./bin/${PE_ENV}/c/c_inc.x \
   ./bin/${PE_ENV}/c/c_half_exit.x \
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
   ./bin/${PE_ENV}/fortran/ftn_cmp_diff.x \
   ./bin/${PE_ENV}/fortran/ftn_arrays.x \
   ./bin/${PE_ENV}/fortran/ftn_decomp.x \
   ./bin/${PE_ENV}/fortran/ftn_bd_dt.x

# tests which require CTI development libraries to build
CTI_TESTS = \
	./bin/${PE_ENV}/C++/cpp_attach.x

# tests which require GPUs
GPU_TESTS = \
	./bin/CRAY/c/simple_gpu-rocm.x \
	./bin/CRAY/c/simple_gpu-cuda.x

RAJA_TESTS = \
	./bin/CRAY/raja/view-layout_solution.x \
	./bin/CRAY/raja/kernel-matrix-transpose-local-array_solution.x \
	./bin/CRAY/raja/launch-matrix-transpose-local-array_solution.x \
	./bin/CRAY/raja/reductions_solution.x

# you shouldn't have to have cti devel installed to test gdb4hpc, so only build
# tests which require it if we happen to have it
CTI_INC      = -I${CTI_INSTALL_DIR}/include
CTI_LD       = -L${CTI_INSTALL_DIR}/lib -lcommontools_fe


ifneq (${CTI_INSTALL_DIR}no,no)
	TESTS += $(CTI_TESTS)
endif

ifeq ($(PE_ENV),CRAY)
	TESTS += $(GPU_TESTS)
endif

ifdef LOCALHOST
MPI_INC = -I./localMPI
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

all: dirs $(TESTS)
ifeq (${CTI_INSTALL_DIR}no,no)
	@echo -e "$(IN_YELLOW)CTI development libraries not found, skipping tests that require them."
	@echo -e "$(IN_YELLOW)To build these tests, load CTI development libraries such that pkg-config can find them."
	@echo -e "$(IN_YELLOW)Verify with 'pkg-config --cflags common_tools_fe'."
	@echo -ne "$(NO_COLOR)"
endif

CFLAGS = -g -O0 -I./include $(MPI_INC)
CXXFLAGS = -g -O0 -I./include $(MPI_INC)
FFLAGS = -g -O0 $(MPI_INC)

gnu: all
amd: all
nvidia: all
intel: all

dirs:
	mkdir -p ./bin/${PE_ENV}/c
	mkdir -p ./bin/${PE_ENV}/C++
	mkdir -p ./bin/${PE_ENV}/fortran

clean:
	rm -rf ./bin
	rm -rf raja_installation*

./bin/${PE_ENV}/c/%.o : ./src/c/%.c
	$(CC) $(CFLAGS) -c $< -o $@

./bin/${PE_ENV}/C++/%.o : ./src/C++/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

./bin/${PE_ENV}/fortran/%.o : ./src/fortran/%.f90
	$(FC) $(FFLAGS) -c $< -o $@

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

./bin/${PE_ENV}/C++/cpp_bd_test.x : ./bin/${PE_ENV}/C++/cpp_bd_test.o
	$(CXX) $(CXXFLAGS) $^ -o $@

./bin/${PE_ENV}/C++/cpp_cmp_diff.x : ./bin/${PE_ENV}/C++/cpp_cmp_diff.o
	$(CXX) $(CXXFLAGS) $^ -o $@

./bin/${PE_ENV}/C++/cpp_ds_test1.x : ./bin/${PE_ENV}/C++/cpp_ds_test1.o
	$(CXX) $(CXXFLAGS) $^ -o $@

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

./bin/${PE_ENV}/raja/view-layout_solution.x: get_raja ./raja_installation/lib/libRAJA.a ./src/raja/view-layout_solution.cpp
	mkdir -p ./bin/${PE_ENV}/raja
	$(CXX) $(CXXFLAGS) -I./raja_installation/include ./src/raja/view-layout_solution.cpp ./raja_installation/lib/libRAJA.a -o $@

./bin/${PE_ENV}/raja/kernel-matrix-transpose-local-array_solution.x: get_raja ./raja_installation/lib/libRAJA.a ./src/raja/kernel-matrix-transpose-local-array_solution.cpp
	mkdir -p ./bin/${PE_ENV}/raja
	$(CXX) $(CXXFLAGS) -I./raja_installation/include ./src/raja/kernel-matrix-transpose-local-array_solution.cpp ./raja_installation/lib/libRAJA.a -o $@

./bin/${PE_ENV}/raja/launch-matrix-transpose-local-array_solution.x: get_raja ./raja_installation/lib/libRAJA.a ./src/raja/launch-matrix-transpose-local-array_solution.cpp
	mkdir -p ./bin/${PE_ENV}/raja
	$(CXX) $(CXXFLAGS) -I./raja_installation/include ./src/raja/launch-matrix-transpose-local-array_solution.cpp ./raja_installation/lib/libRAJA.a -o $@

./bin/${PE_ENV}/raja/reductions_solution.x: get_raja ./raja_installation/lib/libRAJA.a ./src/raja/reductions_solution.cpp
	mkdir -p ./bin/${PE_ENV}/raja
	$(CXX) $(CXXFLAGS) -I./raja_installation/include ./src/raja/reductions_solution.cpp ./raja_installation/lib/libRAJA.a -o $@

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

raja: $(RAJA_TESTS)

get_raja:
	if [ ! -e "./raja_installation" ]; then \
		wget https://arti.dev.cray.com:443/artifactory/pe-internal-third-party-generic-local/raja/raja_installation.tar.gz && \
		tar -xf ./raja_installation.tar.gz; \
	fi
