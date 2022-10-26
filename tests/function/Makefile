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
   ./bin/${PE_ENV}/c/simple_gpu.x \
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

# you shouldn't have to have cti devel installed to test gdb4hpc, so only build
# tests which require it if we happen to have it
CTI_PRESENT := $(shell pkg-config --exists common_tools_fe && echo yes)
CTI_INC      = $(shell pkg-config --cflags common_tools_fe)
CTI_LD       = $(shell pkg-config --libs common_tools_fe)

ifeq ($(CTI_PRESENT),yes)
	TESTS += $(CTI_TESTS)
endif

# ANSI escape codes
IN_YELLOW = \033[1;33m
NO_COLOR = \033[0m

all: dirs $(TESTS)
ifneq ($(CTI_PRESENT),yes)
	@echo -e "$(IN_YELLOW)CTI development libraries not found, skipping tests that require them."
	@echo -e "$(IN_YELLOW)To build these tests, load CTI development libraries such that pkg-config can find them."
	@echo -e "$(IN_YELLOW)Verify with 'pkg-config --cflags common_tools_fe'."
	@echo -ne "$(NO_COLOR)"
endif

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

./bin/${PE_ENV}/c/%.o : ./src/c/%.c
	cc -g -O0 -c -I./include $< -o $@

./bin/${PE_ENV}/C++/%.o : ./src/C++/%.cpp
	CC -g -O0 -c -I./include $< -o $@

./bin/${PE_ENV}/fortran/%.o : ./src/fortran/%.f90
	ftn -g -O0 -c $< -o $@

./bin/${PE_ENV}/c/c_time.x : ./bin/${PE_ENV}/c/wtime.o ./bin/${PE_ENV}/c/mtest.o
	cc -g -O0 $^ -o $@

./bin/${PE_ENV}/c/c_type_test.x : ./bin/${PE_ENV}/c/c_type_test.o
	cc -g -O0 $^ -o $@

./bin/${PE_ENV}/c/c_cmp_diff.x : ./bin/${PE_ENV}/c/c_cmp_diff.o
	cc -g -O0 $^ -o $@

./bin/${PE_ENV}/c/libdlopen_test.so : ./src/c/libdlopen_test.c
	cc -g -O0 -shared $^ -o $@

./bin/${PE_ENV}/c/c_dlopen.x :  ./src/c/c_dlopen_test.c ./bin/${PE_ENV}/c/libdlopen_test.so
	cc -g -O0 ./src/c/c_dlopen_test.c -DBIN_PATH=\"${PWD}/bin/${PE_ENV}/c\" -ldl -o $@

./bin/${PE_ENV}/c/c_segv.x : ./bin/${PE_ENV}/c/c_segv.o
	cc -g -O0 $^ -o $@

./bin/${PE_ENV}/c/c_list.x : ./bin/${PE_ENV}/c/c_list.o
	cc -g -O0 $^ -o $@

./bin/${PE_ENV}/c/c_inc.x : ./bin/${PE_ENV}/c/c_inc.o
	cc -g -O0 $^ -o $@

./bin/${PE_ENV}/c/c_half_exit.x : ./bin/${PE_ENV}/c/c_half_exit.o
	cc -g -O0 $^ -o $@

./bin/${PE_ENV}/C++/cpp_bd_test.x : ./bin/${PE_ENV}/C++/cpp_bd_test.o
	CC -g -O0 $^ -o $@

./bin/${PE_ENV}/C++/cpp_cmp_diff.x : ./bin/${PE_ENV}/C++/cpp_cmp_diff.o
	CC -g -O0 $^ -o $@

./bin/${PE_ENV}/C++/cpp_ds_test1.x : ./bin/${PE_ENV}/C++/cpp_ds_test1.o
	CC -g -O0 $^ -o $@

./bin/${PE_ENV}/C++/cpp_arrays.x : ./bin/${PE_ENV}/C++/cpp_arrays.o
	CC -g -O0 $^ -o $@

./bin/${PE_ENV}/C++/cpp_decomp.x : ./bin/${PE_ENV}/C++/cpp_decomp.o
	CC -g -O0 $^ -o $@

./bin/${PE_ENV}/C++/cpp_decomp2.x : ./bin/${PE_ENV}/C++/cpp_decomp2.o
	CC -g -O0 $^ -o $@

./bin/${PE_ENV}/C++/cpp_threads.x : ./bin/${PE_ENV}/C++/cpp_threads.o
	CC -g -O0 $^ -o $@

./bin/${PE_ENV}/C++/cpp_parallel_threads.x : ./bin/${PE_ENV}/C++/cpp_parallel_threads.o
	CC -g -O0 $^ -o $@

./bin/${PE_ENV}/C++/cpp_bt.x : ./bin/${PE_ENV}/C++/cpp_bt.o
	CC -g -O0 $^ -o $@

./bin/${PE_ENV}/C++/cpp_bt_stress.x : ./bin/${PE_ENV}/C++/cpp_bt_stress.o
	CC -g -O0 $^ -o $@

./bin/${PE_ENV}/C++/cpp_arrays_print.x : ./bin/${PE_ENV}/C++/cpp_arrays_print.o
	CC -g -O0 $^ -o $@

./bin/${PE_ENV}/c/libfoo.so : ./src/c/libfoo.c
	gcc -g -O0 -fPIC -shared $^ -o $@

./bin/${PE_ENV}/c/libbar.so : ./src/c/libbar.c
	gcc -g -O0 -fPIC -shared $^ -o $@

./bin/${PE_ENV}/C++/cpp_catch.x : ./src/C++/cpp_catch.cpp ./bin/${PE_ENV}/c/libfoo.so ./bin/${PE_ENV}/c/libbar.so
	CC -g -O0 -DBD_DT=\"./bin/${PE_ENV}\"s ./src/C++/cpp_catch.cpp -o $@ -ldl

./bin/${PE_ENV}/C++/cpp_attach.x : ./bin/${PE_ENV}/C++/cpp_attach.o ./bin/${PE_ENV}/cti_launch.x
	CC -g -O0 ./bin/${PE_ENV}/C++/cpp_attach.o -o $@

./bin/${PE_ENV}/fortran/ftn_cmp_diff.x : ./bin/${PE_ENV}/fortran/ftn_cmp_diff.o
	ftn -g -O0 $^ -o $@

./bin/${PE_ENV}/fortran/ftn_arrays.x : ./bin/${PE_ENV}/fortran/ftn_arrays.o
	ftn -g -O0 $^ -o $@

./bin/${PE_ENV}/fortran/ftn_decomp.x : ./bin/${PE_ENV}/fortran/ftn_decomp.o
	ftn -g -O0 $^ -o $@

./bin/${PE_ENV}/fortran/ftn_bd_dt.x : ./bin/${PE_ENV}/fortran/ftn_bd_dt.o
	ftn -g -O0 $^ -o $@

./bin/${PE_ENV}/cti_launch.o: ./src/cti_launch.cpp
	CC -g -O0 -c -I./include $(CTI_INC) $< -o $@

./bin/${PE_ENV}/cti_launch.x : ./bin/${PE_ENV}/cti_launch.o
	CC -g -O0 $^ -o $@ $(CTI_LD)

ROCM_SCRIPT = ./tests.py.data/rocm/$(HOSTNAME).sh 

./bin/${PE_ENV}/c/simple_gpu.x : ./src/c/simple_gpu.c $(wildcard $(ROCM_SCRIPT))
	if [ -f $(ROCM_SCRIPT)] ; then \
		$(ROCM_SCRIPT) make; \
	fi

