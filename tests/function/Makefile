
# Copyright 2020 Hewlett Packard Enterprise Development LP.

TESTS = \
   bd_dt/c/c_time.x \
   bd_dt/c/c_type_test.x \
   bd_dt/c/c_cmp_diff.x \
   bd_dt/c/c_dlopen.x \
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
   bd_dt/fortran/ftn_cmp_diff.x \
   bd_dt/fortran/ftn_arrays.x \
   bd_dt/fortran/ftn_decomp.x \
   bd_dt_fortran/ftn_bd_dt.x

all: $(TESTS)

clean:
	rm -f $(TESTS)
	rm -f */*/*.o
	rm -f */*/*.so

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
	cc bd_dt/c/c_dlopen_test.c -DBIN_PATH=\"$(PWD)/bd_dt/c/\" -ldl -o $@

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

bd_dt/C++/cpp_bt_stress.x :    bd_dt/C++/cpp_bt_stress.o
	CC -g -O0 $^ -o $@

bd_dt/fortran/ftn_cmp_diff.x : bd_dt/fortran/ftn_cmp_diff.o
	ftn -g -O0 $^ -o $@

bd_dt/fortran/ftn_arrays.x : bd_dt/fortran/ftn_arrays.o
	ftn -g -O0 $^ -o $@

bd_dt/fortran/ftn_decomp.x : bd_dt/fortran/ftn_decomp.o
	ftn -g -O0 $^ -o $@

bd_dt/fortran/ftn_bd_dt.x : bd_dt/fortran/ftn_bd_dt.o
	ftn -g -O0 $^ -o $@
