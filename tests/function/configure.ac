# configure.ac script for gdb4hpc tests.
#
# Copyright 2020 Hewlett Packard Enterprise Development LP.
#

AC_PREREQ([2.63])
AC_INIT([gdb4hpc_tests], [1.0])
AM_INIT_AUTOMAKE([subdir-objects foreign -Wall])

: ${CXXFLAGS="-g -O0"}
: ${CFLAGS="-g -O0"}
: ${FCFLAGS="-g -O0"}

AC_PROG_CXX([CC])
AC_PROG_CC([cc])
AC_PROG_FC([ftn])
AC_COPYRIGHT([Copyright 2020 Hewlett Packard Enterprise Development LP.])

AC_CONFIG_FILES([
    Makefile
    bd_dt/Makefile
    bd_dt/fortran/Makefile
    bd_dt/C++/Makefile
    bd_dt/c/Makefile
])

AC_OUTPUT
