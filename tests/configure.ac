# configure.ac script for gdb4hpc tests.
#
# Copyright 2020 Cray Inc.
#

AC_PREREQ([2.63])
AC_INIT([gdb4hpc_tests], [1.0])
AM_INIT_AUTOMAKE([subdir-objects foreign -Wall])

: ${CXXFLAGS="-g -O0"}
: ${CFLAGS="-g -O0"}
: ${FCFLAGS="-g -O0"}

AC_PROG_CXX([CC])
AC_PROG_CC([cc])
AC_PROG_FC
AC_COPYRIGHT([Copyright 2020 Cray Inc.])

AC_CONFIG_HEADERS([include/config.h])

AC_CONFIG_FILES([
    Makefile
    function/Makefile
    function/bd_dt/Makefile
    function/bd_dt/fortran/Makefile
    function/bd_dt/C++/Makefile
    function/bd_dt/c/Makefile
])


AC_OUTPUT
