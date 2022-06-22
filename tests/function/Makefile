# Copyright 2022 Hewlett Packard Enterprise Development LP.

# Needed until PE-41446 is fixed and released
# Set DOT to local working directory if not already set externally
DOT?=${PWD}

TESTS = \
   ${DOT}/bd_dt/c/c_time.x \
   ${DOT}/bd_dt/c/c_type_test.x \
   ${DOT}/bd_dt/c/c_cmp_diff.x \
   ${DOT}/bd_dt/c/c_dlopen.x \
   ${DOT}/bd_dt/c/c_segv.x \
   ${DOT}/bd_dt/c/c_list.x \
   ${DOT}/bd_dt/c/c_inc.x \
   ${DOT}/bd_dt/c/c_half_exit.x \
   ${DOT}/bd_dt/c/simple_gpu.x \
   ${DOT}/bd_dt/C++/cpp_bd_test.x \
   ${DOT}/bd_dt/C++/cpp_cmp_diff.x \
   ${DOT}/bd_dt/C++/cpp_ds_test1.x \
   ${DOT}/bd_dt/C++/cpp_arrays.x \
   ${DOT}/bd_dt/C++/cpp_decomp.x \
   ${DOT}/bd_dt/C++/cpp_decomp2.x \
   ${DOT}/bd_dt/C++/cpp_threads.x \
   ${DOT}/bd_dt/C++/cpp_parallel_threads.x \
   ${DOT}/bd_dt/C++/cpp_bt.x \
   ${DOT}/bd_dt/C++/cpp_bt_stress.x \
   ${DOT}/bd_dt/C++/cpp_arrays_print.x \
   ${DOT}/bd_dt/C++/cpp_catch.x \
   ${DOT}/bd_dt/fortran/ftn_cmp_diff.x \
   ${DOT}/bd_dt/fortran/ftn_arrays.x \
   ${DOT}/bd_dt/fortran/ftn_decomp.x \
   ${DOT}/bd_dt/fortran/ftn_bd_dt.x

# tests which require CTI development libraries to build
CTI_TESTS = \
	${DOT}/bd_dt/C++/cpp_attach.x

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

# Needed until PE-41446 is fixed and released
# Note that `make -C` can't be used since it will un-change the current directory away from /tmp
all:
	cd /tmp && make all-real -f ${DOT}/Makefile DOT=${DOT}

all-real: $(TESTS)
ifneq ($(CTI_PRESENT),yes)
	@echo -e "$(IN_YELLOW)CTI development libraries not found, skipping tests that require them."
	@echo -e "$(IN_YELLOW)To build these tests, load CTI development libraries such that pkg-config can find them."
	@echo -e "$(IN_YELLOW)Verify with 'pkg-config --cflags common_tools_fe'."
	@echo -ne "$(NO_COLOR)"
endif

clean:
	rm -f $(TESTS)
	rm -f ${DOT}/*/*.{o,x,so}
	rm -f ${DOT}/*/*/*.{o,x,so}

%.o : %.c
	cc -g -O0 -c -I${DOT}/include $< -o $@

%.o : %.cpp
	CC -g -O0 -c -I${DOT}/include $< -o $@

%.o : %.f90
	ftn -g -O0 -c $< -o $@

${DOT}/bd_dt/c/c_time.x : ${DOT}/bd_dt/c/wtime.o ${DOT}/bd_dt/c/mtest.o
	cc -g -O0 $^ -o $@

${DOT}/bd_dt/c/c_type_test.x : ${DOT}/bd_dt/c/c_type_test.o
	cc -g -O0 $^ -o $@

${DOT}/bd_dt/c/c_cmp_diff.x : ${DOT}/bd_dt/c/c_cmp_diff.o
	cc -g -O0 $^ -o $@

${DOT}/bd_dt/c/libdlopen_test.so : ${DOT}/bd_dt/c/libdlopen_test.c
	cc -shared $^ -o $@

${DOT}/bd_dt/c/c_dlopen.x :  ${DOT}/bd_dt/c/c_dlopen_test.c ${DOT}/bd_dt/c/libdlopen_test.so
	cc -g -O0 ${DOT}/bd_dt/c/c_dlopen_test.c -DBIN_PATH=\"${DOT}/bd_dt/c/\" -ldl -o $@

${DOT}/bd_dt/c/c_segv.x : ${DOT}/bd_dt/c/c_segv.o
	cc -g -O0 $^ -o $@

${DOT}/bd_dt/c/c_list.x : ${DOT}/bd_dt/c/c_list.o
	cc -g -O0 $^ -o $@

${DOT}/bd_dt/c/c_inc.x : ${DOT}/bd_dt/c/c_inc.o
	cc -g -O0 $^ -o $@

${DOT}/bd_dt/c/c_half_exit.x : ${DOT}/bd_dt/c/c_half_exit.o
	cc -g -O0 $^ -o $@

${DOT}/bd_dt/C++/cpp_bd_test.x : ${DOT}/bd_dt/C++/cpp_bd_test.o
	CC -g -O0 $^ -o $@

${DOT}/bd_dt/C++/cpp_cmp_diff.x : ${DOT}/bd_dt/C++/cpp_cmp_diff.o
	CC -g -O0 $^ -o $@

${DOT}/bd_dt/C++/cpp_ds_test1.x : ${DOT}/bd_dt/C++/cpp_ds_test1.o
	CC -g -O0 $^ -o $@

${DOT}/bd_dt/C++/cpp_arrays.x : ${DOT}/bd_dt/C++/cpp_arrays.o
	CC -g -O0 $^ -o $@

${DOT}/bd_dt/C++/cpp_decomp.x : ${DOT}/bd_dt/C++/cpp_decomp.o
	CC -g -O0 $^ -o $@

${DOT}/bd_dt/C++/cpp_decomp2.x : ${DOT}/bd_dt/C++/cpp_decomp2.o
	CC -g -O0 $^ -o $@

${DOT}/bd_dt/C++/cpp_threads.x : ${DOT}/bd_dt/C++/cpp_threads.o
	CC -g -O0 $^ -o $@

${DOT}/bd_dt/C++/cpp_parallel_threads.x : ${DOT}/bd_dt/C++/cpp_parallel_threads.o
	CC -g -O0 $^ -o $@

${DOT}/bd_dt/C++/cpp_bt.x : ${DOT}/bd_dt/C++/cpp_bt.o
	CC -g -O0 $^ -o $@

${DOT}/bd_dt/C++/cpp_bt_stress.x : ${DOT}/bd_dt/C++/cpp_bt_stress.o
	CC -g -O0 $^ -o $@

${DOT}/bd_dt/C++/cpp_arrays_print.x : ${DOT}/bd_dt/C++/cpp_arrays_print.o
	CC -g -O0 $^ -o $@

${DOT}/bd_dt/c/libfoo.so : ${DOT}/bd_dt/c/libfoo.c
	gcc -g -O0 -fPIC -shared $^ -o $@

${DOT}/bd_dt/c/libbar.so : ${DOT}/bd_dt/c/libbar.c
	gcc -g -O0 -fPIC -shared $^ -o $@

${DOT}/bd_dt/C++/cpp_catch.x : ${DOT}/bd_dt/C++/cpp_catch.cpp ${DOT}/bd_dt/c/libfoo.so ${DOT}/bd_dt/c/libbar.so
	CC -g -O0 -DBD_DT=\"${DOT}/bd_dt\"s ${DOT}/bd_dt/C++/cpp_catch.cpp -o $@ -ldl

${DOT}/bd_dt/C++/cpp_attach.x : ${DOT}/bd_dt/C++/cpp_attach.o ${DOT}/bd_dt/cti_launch.x
	CC -g -O0 ${DOT}/bd_dt/C++/cpp_attach.o -o $@

${DOT}/bd_dt/fortran/ftn_cmp_diff.x : ${DOT}/bd_dt/fortran/ftn_cmp_diff.o
	ftn -g -O0 $^ -o $@

${DOT}/bd_dt/fortran/ftn_arrays.x : ${DOT}/bd_dt/fortran/ftn_arrays.o
	ftn -g -O0 $^ -o $@

${DOT}/bd_dt/fortran/ftn_decomp.x : ${DOT}/bd_dt/fortran/ftn_decomp.o
	ftn -g -O0 $^ -o $@

${DOT}/bd_dt/fortran/ftn_bd_dt.x : ${DOT}/bd_dt/fortran/ftn_bd_dt.o
	ftn -g -O0 $^ -o $@

${DOT}/bd_dt/cti_launch.o: ${DOT}/bd_dt/cti_launch.cpp
	CC -g -O0 -c -I./include $(CTI_INC) $< -o $@

${DOT}/bd_dt/cti_launch.x : ${DOT}/bd_dt/cti_launch.o
	CC -g -O0 $^ -o $@ $(CTI_LD)

ROCM_SCRIPT = ${DOT}/tests.py.data/rocm/$(HOSTNAME).sh 

${DOT}/bd_dt/c/simple_gpu.x : ${DOT}/bd_dt/c/simple_gpu.c $(wildcard $(ROCM_SCRIPT))
	if [ -f $(ROCM_SCRIPT)] ; then \
		$(ROCM_SCRIPT) make; \
	fi

