#!/bin/bash

#  Copyright 2020 Hewlett Packard Enterprise Development LP.

# backup method to build the binaries that need to be built on the
# testing machine.   Currently manual, this should get integrated into
# the test process sooner or later (or autotools will become universally
# available)

cc -I. -I./include  -g -O0 -c -o bd_dt/c/wtime.o bd_dt/c/wtime.c
cc -I. -I./include  -g -O0 -c -o bd_dt/c/mtest.o bd_dt/c/mtest.c
cc  -g -O0   -o bd_dt/c/c_time.x bd_dt/c/wtime.o bd_dt/c/mtest.o  
cc -I. -I./include  -g -O0 -c -o bd_dt/c/c_type_test.o bd_dt/c/c_type_test.c
cc  -g -O0   -o bd_dt/c/c_type_test.x bd_dt/c/c_type_test.o  
cc -I. -I./include  -g -O0 -c -o bd_dt/c/c_cmp_diff.o bd_dt/c/c_cmp_diff.c
cc  -g -O0   -o bd_dt/c/c_cmp_diff.x bd_dt/c/c_cmp_diff.o  
cc  bd_dt/c/c_dlopen_test.c -DBIN_PATH=\"/home/users/jvogt/dev/gdb4hpc/tests/function/bd_dt/c/\" -ldl -o bd_dt/c/c_dlopen.x
CC -I. -I./include  -g -O0 -c -o bd_dt/C++/cpp_bd_test.o bd_dt/C++/cpp_bd_test.cpp
CC  -g -O0   -o bd_dt/C++/cpp_bd_test.x bd_dt/C++/cpp_bd_test.o  
CC -I. -I./include  -g -O0 -c -o bd_dt/C++/cpp_cmp_diff.o bd_dt/C++/cpp_cmp_diff.cpp
CC  -g -O0   -o bd_dt/C++/cpp_cmp_diff.x bd_dt/C++/cpp_cmp_diff.o  
CC -I. -I./include  -g -O0 -c -o bd_dt/C++/cpp_ds_test1.o bd_dt/C++/cpp_ds_test1.cpp
CC  -g -O0   -o bd_dt/C++/cpp_ds_test1.x bd_dt/C++/cpp_ds_test1.o  
CC -I. -I./include  -g -O0 -c -o bd_dt/C++/cpp_arrays.o bd_dt/C++/cpp_arrays.cpp
CC  -g -O0   -o bd_dt/C++/cpp_arrays.x bd_dt/C++/cpp_arrays.o  
CC -I. -I./include  -g -O0 -c -o bd_dt/C++/cpp_decomp.o bd_dt/C++/cpp_decomp.cpp
CC  -g -O0   -o bd_dt/C++/cpp_decomp.x bd_dt/C++/cpp_decomp.o  
CC -I. -I./include  -g -O0 -c -o bd_dt/C++/cpp_decomp2.o bd_dt/C++/cpp_decomp2.cpp
CC  -g -O0   -o bd_dt/C++/cpp_decomp2.x bd_dt/C++/cpp_decomp2.o  
CC -I. -I./include  -g -O0 -c -o bd_dt/C++/cpp_threads.o bd_dt/C++/cpp_threads.cpp
CC  -g -O0   -o bd_dt/C++/cpp_threads.x bd_dt/C++/cpp_threads.o  
CC -I. -I./include  -g -O0 -c -o bd_dt/C++/cpp_parallel_threads.o bd_dt/C++/cpp_parallel_threads.cpp
CC  -g -O0   -o bd_dt/C++/cpp_parallel_threads.x bd_dt/C++/cpp_parallel_threads.o  
CC -I. -I./include  -g -O0 -c -o bd_dt/C++/cpp_bt.o bd_dt/C++/cpp_bt.cpp
CC  -g -O0   -o bd_dt/C++/cpp_bt.x bd_dt/C++/cpp_bt.o  
#CC -I. -I./include  -g -O0 -c -o bd_dt/C++/cpp_bt_stress.o bd_dt/C++/cpp_bt_stress.cpp
#CC  -g -O0   -o bd_dt/C++/cpp_bt_stress.x bd_dt/C++/cpp_bt_stress.o  
ftn  -g -O0 -c -o bd_dt/fortran/ftn_bd_dt.o bd_dt/fortran/ftn_bd_dt.f90
ftn  -g -O0   -o bd_dt/fortran/ftn_bd_dt.x bd_dt/fortran/ftn_bd_dt.o  
ftn  -g -O0 -c -o bd_dt/fortran/ftn_cmp_diff.o bd_dt/fortran/ftn_cmp_diff.f90
ftn  -g -O0   -o bd_dt/fortran/ftn_cmp_diff.x bd_dt/fortran/ftn_cmp_diff.o  
ftn  -g -O0 -c -o bd_dt/fortran/ftn_arrays.o bd_dt/fortran/ftn_arrays.f90
ftn  -g -O0   -o bd_dt/fortran/ftn_arrays.x bd_dt/fortran/ftn_arrays.o  
ftn  -g -O0 -c -o bd_dt/fortran/ftn_decomp.o bd_dt/fortran/ftn_decomp.f90
ftn  -g -O0   -o bd_dt/fortran/ftn_decomp.x bd_dt/fortran/ftn_decomp.o  
