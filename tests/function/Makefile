
# Copyright 2020 Hewlett Packard Enterprise Development LP.

TESTS = \
   bd_dt/c/c_time.x \
   bd_dt/c/c_type_test.x \
   bd_dt/c/c_cmp_diff.x \
   bd_dt/c/c_dlopen.x \
   bd_dt/c/c_segv.x \
   bd_dt/c/c_list.x \
   bd_dt/c/c_inc.x \
   bd_dt/c/c_half_exit.x \
   bd_dt/c/simple_gpu.x \
   bd_dt/C++/cpp_bd_test.x \
   bd_dt/C++/cpp_cmp_diff.x \
   bd_dt/C++/cpp_ds_test1.x \
   bd_dt/C++/cpp_arrays.x \
   bd_dt/C++/cpp_decomp.x \
   bd_dt/C++/cpp_decomp2.x \
   bd_dt/C++/cpp_threads.x \
   bd_dt/C++/cpp_parallel_threads.x \
   bd_dt/C++/cpp_bt.x \
   bd_dt/C++/cpp_bt_stress.x \
   bd_dt/C++/cpp_arrays_print.x \
   bd_dt/C++/cpp_catch.x \
   bd_dt/fortran/ftn_cmp_diff.x \
   bd_dt/fortran/ftn_arrays.x \
   bd_dt/fortran/ftn_decomp.x \
   bd_dt/fortran/ftn_bd_dt.x

# tests which require CTI development libraries to build
CTI_TESTS = \
	bd_dt/C++/cpp_attach.x

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

all: $(TESTS)
ifneq ($(CTI_PRESENT),yes)
	@echo -e "$(IN_YELLOW)CTI development libraries not found, skipping tests that require them."
	@echo -e "$(IN_YELLOW)To build these tests, load CTI development libraries such that pkg-config can find them."
	@echo -e "$(IN_YELLOW)Verify with 'pkg-config --cflags common_tools_fe'."
	@echo -ne "$(NO_COLOR)"
endif

clean:
	rm -f $(TESTS)
	rm -f */*.{o,x,so}
	rm -f */*/*.{o,x,so}

%.o : %.c
	cc -g -O0 -c -I./include $< -o $@

%.o : %.cpp
	CC -g -O0 -c -I./include $< -o $@

%.o : %.f90
	ftn -g -O0 -c $< -o $@

bd_dt/c/c_time.x : bd_dt/c/wtime.o bd_dt/c/mtest.o
	cc -g -O0 $^ -o $@

bd_dt/c/c_type_test.x : bd_dt/c/c_type_test.o
	cc -g -O0 $^ -o $@

bd_dt/c/c_cmp_diff.x : bd_dt/c/c_cmp_diff.o
	cc -g -O0 $^ -o $@

bd_dt/c/libdlopen_test.so : bd_dt/c/libdlopen_test.c
	cc -shared $^ -o $@

bd_dt/c/c_dlopen.x :  bd_dt/c/c_dlopen_test.c bd_dt/c/libdlopen_test.so
	cc -g -O0 bd_dt/c/c_dlopen_test.c -DBIN_PATH=\"$(PWD)/bd_dt/c/\" -ldl -o $@

bd_dt/c/c_segv.x : bd_dt/c/c_segv.o
	cc -g -O0 $^ -o $@

bd_dt/c/c_list.x : bd_dt/c/c_list.o
	cc -g -O0 $^ -o $@

bd_dt/c/c_inc.x : bd_dt/c/c_inc.o
	cc -g -O0 $^ -o $@

bd_dt/c/c_half_exit.x : bd_dt/c/c_half_exit.o
	cc -g -O0 $^ -o $@

bd_dt/C++/cpp_bd_test.x : bd_dt/C++/cpp_bd_test.o
	CC -g -O0 $^ -o $@

bd_dt/C++/cpp_cmp_diff.x : bd_dt/C++/cpp_cmp_diff.o
	CC -g -O0 $^ -o $@

bd_dt/C++/cpp_ds_test1.x : bd_dt/C++/cpp_ds_test1.o
	CC -g -O0 $^ -o $@

bd_dt/C++/cpp_arrays.x : bd_dt/C++/cpp_arrays.o
	CC -g -O0 $^ -o $@

bd_dt/C++/cpp_decomp.x : bd_dt/C++/cpp_decomp.o
	CC -g -O0 $^ -o $@

bd_dt/C++/cpp_decomp2.x : bd_dt/C++/cpp_decomp2.o
	CC -g -O0 $^ -o $@

bd_dt/C++/cpp_threads.x : bd_dt/C++/cpp_threads.o
	CC -g -O0 $^ -o $@

bd_dt/C++/cpp_parallel_threads.x : bd_dt/C++/cpp_parallel_threads.o
	CC -g -O0 $^ -o $@

bd_dt/C++/cpp_bt.x : bd_dt/C++/cpp_bt.o
	CC -g -O0 $^ -o $@

bd_dt/C++/cpp_bt_stress.x : bd_dt/C++/cpp_bt_stress.o
	CC -g -O0 $^ -o $@

bd_dt/C++/cpp_arrays_print.x : bd_dt/C++/cpp_arrays_print.o
	CC -g -O0 $^ -o $@

bd_dt/c/libfoo.so : bd_dt/c/libfoo.c
	gcc -g -O0 -fPIC -shared $^ -o $@

bd_dt/c/libbar.so : bd_dt/c/libbar.c
	gcc -g -O0 -fPIC -shared $^ -o $@

bd_dt/C++/cpp_catch.x : bd_dt/C++/cpp_catch.cpp bd_dt/c/libfoo.so bd_dt/c/libbar.so
	CC -g -O0 -DBD_DT=\"$(PWD)/bd_dt\"s bd_dt/C++/cpp_catch.cpp -o $@ -ldl

bd_dt/C++/cpp_attach.x : bd_dt/C++/cpp_attach.o bd_dt/cti_launch.x
	CC -g -O0 bd_dt/C++/cpp_attach.o -o $@

bd_dt/fortran/ftn_cmp_diff.x : bd_dt/fortran/ftn_cmp_diff.o
	ftn -g -O0 $^ -o $@

bd_dt/fortran/ftn_arrays.x : bd_dt/fortran/ftn_arrays.o
	ftn -g -O0 $^ -o $@

bd_dt/fortran/ftn_decomp.x : bd_dt/fortran/ftn_decomp.o
	ftn -g -O0 $^ -o $@

bd_dt/fortran/ftn_bd_dt.x : bd_dt/fortran/ftn_bd_dt.o
	ftn -g -O0 $^ -o $@

bd_dt/cti_launch.o: bd_dt/cti_launch.cpp
	CC -g -O0 -c -I./include $(CTI_INC) $< -o $@

bd_dt/cti_launch.x : bd_dt/cti_launch.o
	CC -g -O0 $^ -o $@ $(CTI_LD)

ROCM_SCRIPT = tests.py.data/rocm/$(HOSTNAME).sh 

bd_dt/c/simple_gpu.x : bd_dt/c/simple_gpu.c $(wildcard $(ROCM_SCRIPT))
	if [ -f $(ROCM_SCRIPT)] ; then \
		$(ROCM_SCRIPT) make; \
	fi

