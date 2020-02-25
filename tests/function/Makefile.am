# Copyright 2020 Cray Inc.

check_PROGRAMS = bd_dt/c/c_time.x bd_dt/c/c_type_test.x bd_dt/c/c_cmp_diff.x \
	bd_dt/C++/cpp_bd_test.x bd_dt/C++/cpp_cmp_diff.x bd_dt/C++/cpp_ds_test1.x \
	bd_dt/fortran/ftn_bd_dt.x bd_dt/fortran/ftn_cmp_diff.x


bd_dt_c_c_time_x_SOURCES = bd_dt/c/wtime.c bd_dt/c/mtest.c
bd_dt_c_c_type_test_x_SOURCES = bd_dt/c/c_type_test.c
bd_dt_c_c_cmp_diff_x_SOURCES = bd_dt/c/c_cmp_diff.c

bd_dt_C___cpp_bd_test_x_SOURCES = bd_dt/C++/cpp_bd_test.cpp
bd_dt_C___cpp_cmp_diff_x_SOURCES = bd_dt/C++/cpp_cmp_diff.cpp
bd_dt_C___cpp_ds_test1_x_SOURCES = bd_dt/C++/cpp_ds_test1.cpp

bd_dt_fortran_ftn_bd_dt_x_SOURCES = bd_dt/fortran/ftn_bd_dt.f90
bd_dt_fortran_ftn_cmp_diff_x_SOURCES = bd_dt/fortran/ftn_cmp_diff.f90

AM_CPPFLAGS = -I$(top_srcdir)/include
#AM_CFLAGS = -g -O0
#AM_CXXFLAGS = -g -O0
#AM_FCFLAGS = -g -O0






