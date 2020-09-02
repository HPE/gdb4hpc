# Guard dejagnu testsuite configure script
# Copyright 2010, 2020 Hewlett Packard Enterprise Development LP.

# These coded instructions, statements, and computer programs contain
# unpublished proprietary information of Hewlett Packard Enterprise Development LP., and are protected
# by Federal copyright law.  They  may  not  be  disclosed  to  third
# parties  or copied or duplicated in any form, in whole or in part,
# without the prior written consent of Hewlett Packard Enterprise Development LP.

# Author: Andrew Gontarek (andrewg@cray.com)

AC_PREREQ([2.66])
AC_INIT([guard_testsuite], [1.0], [andrewg@cray.com])
AC_CONFIG_SRCDIR([boards/cray.exp])
AC_CONFIG_AUX_DIR([config-build])
AC_CONFIG_HEADERS([config.h])

AC_CANONICAL_BUILD
AC_CANONICAL_HOST
AC_CANONICAL_TARGET

AM_INIT_AUTOMAKE([foreign -Wall])

# check for programs
dnl we need to check for cc first, for some reason the documentation says it checks cc then gcc
dnl but I was getting gcc and then cc, which meant my mpi lib stuff was not being found.
AC_PROG_CC([cc])
AC_PROG_FC([ftn])
AC_PROG_CXX([CC])
AC_PROG_RANLIB

AC_CHECK_HEADER([mpi.h],
		[],
		[AC_MSG_ERROR([You must have system mpi includes to compile the test programs.])]
		)

AC_CONFIG_FILES([Makefile
		 example_progs/mpi_example/Makefile
		 example_progs/shallow_example/Makefile])
AC_OUTPUT
