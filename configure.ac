#
# Copyright 2010-2022 Hewlett Packard Enterprise Development LP. All Rights Reserved
# Copyright (c) 1996-2010 by Guardsoft Pty Ltd.
#
# Unpublished Proprietary Information.
# This unpublished work is protected to trade secret, copyright and other laws.
# Except as permitted by contract or express written permission of Hewlett Packard Enterprise Development LP.,
# no part of this work or its content may be used, reproduced or disclosed
# in any form.
#
AC_PREREQ([2.63])

AC_INIT([gdb4hpc], m4_esyscmd([source $PWD/release_versioning; echo $major.$minor | tr -d '\n']),[john.leroy.vogt@hpe.com])
AC_COPYRIGHT([Copyright 2010-2022 Hewlett Packard Enterprise Development LP.])

AC_CONFIG_SRCDIR([src/defs.h])
AC_CONFIG_AUX_DIR([src/config])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([src/config.h])

AC_MSG_NOTICE([Configuring gdb4hpc])

dnl Turn on Posix extensions
AC_USE_SYSTEM_EXTENSIONS

AM_INIT_AUTOMAKE([foreign -Wall])

dnl Pull inCray Specific m4 extensions
cray_INIT

m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

#############################
###  Define architecture  ###
#############################
AC_CANONICAL_HOST

AC_DEFINE([GDB4HPC_ARCH_X86_64], [1], [Architecture value])
AC_DEFINE([GDB4HPC_ARCH_AARCH64], [2], [Architecture value])
AC_DEFINE([GDB4HPC_ARCH_UNKNOWN], [3], [Architecture value])

AS_CASE([$host_cpu],
	[x86_64],  [AC_DEFINE([GDB4HPC_ARCH], [GDB4HPC_ARCH_X86_64], [Architecture])],
	[aarch64], [AC_DEFINE([GDB4HPC_ARCH], [GDB4HPC_ARCH_AARCH64], [Architecture])],
	# default
	           [AC_DEFINE([GDB4HPC_ARCH], [GDB4HPC_ARCH_UNKNOWN], [Architecture])]
)

#############################
### Check for --with tags ###
#############################


dnl Check for --with-cray
AC_ARG_WITH([cray],
	    [AS_HELP_STRING([--with-cray],
			    [Enable Cray APRUN support in gdb4hpc.]
			    )
	    ],
	    [with_cray=yes],
	    [with_cray=no]
	    )

dnl Check for --with-mrnet
AC_ARG_WITH([mrnet],
	    [AS_HELP_STRING([--with-mrnet],
			    [Enable MRNet support in gdb4hpc.]
			    )
	    ],
	    [mrnet_mode=yes
	     mrnet_root=$withval
	    ],
	    [mrnet_mode=no]
	    )

dnl Check for --with-zmqnet
AC_ARG_WITH([zmqnet],
	    [AS_HELP_STRING([--with-zmqnet],
			    [Enable zmqnet support in gdb4hpc.]
			    )
	    ],
	    [zmqnet_mode=yes
	     zmqnet_root=$withval
	    ],
	    [zmqnet_mode=no]
	    )

dnl Check for --with-doctest
dnl Note: this must always be passed. To disable doctest, provide this argument
dnl so that no headers get lost, and then also provide --disable-doctest.
AC_ARG_WITH([doctest],
            [AS_HELP_STRING([--with-doctest],
			    [Enable unit tests.]
			    )
	    ],
        [doctest_mode=yes
         doctest_root=$withval
	    ],
        [doctest_mode=no]
	    )

dnl Check for --disable-doctest
AC_ARG_ENABLE(doctest, "Controls compilation of unit tests in binary.",
    if test "x$enableval" != xyes; then
        AC_DEFINE([DOCTEST_CONFIG_DISABLE], [1], [If defined, all doctest material is removed from the binary.])
    fi,
    AC_DEFINE([DOCTEST_CONFIG_DISABLE], [1], [If defined, all doctest material is removed from the binary.])
)

dnl Put doctest.h into build directory. This simplifies what test writers have to #include
AC_CONFIG_LINKS([src/unittest/doctest.h:$doctest_root/doctest/doctest.h])

dnl Check for --with-gdbmi
AC_ARG_WITH([gdbmi],
	    [AS_HELP_STRING([--with-gdbmi],
			    [Enable GDB/MI backend support in gdb4hpc.]
			    )
	    ],
	    [mi_mode=yes
	     libmi_path=$withval
	    ],
	    [mi_mode=no]
	    )

dnl Check for --with-libssh2
AC_ARG_WITH([libssh2],
	    [AS_HELP_STRING([--with-libssh2],
			    [Specify path to libssh2 library.]
			    )
	    ],
	    [libssh2_path=$withval]
	    )

dnl Check for --with-debug
AC_ARG_WITH([debug],
	    [AS_HELP_STRING([--with-debug],
			    [Enable debug mode options.]
			    )
	    ],
	    [debug_mode=yes],
	    [debug_mode=no]
	    )

dnl Check for --with-valgrind
AC_ARG_WITH([valgrind],
	    [AS_HELP_STRING([--with-valgrind],
			    [Enable backend valgrind options.]
			    )
	    ],
	    [valgrind_mode=yes],
	    [valgrind_mode=no]
	    )

dnl Check for --with-python
AC_ARG_WITH([python],
	    [AS_HELP_STRING([--with-python],
			    [Enable python support in gdb4hpc.]
			    )
	    ],
	    [python_mode=yes],
	    [python_mode=no]
	    )


#########################
### Setup env options ###
#########################
dnl Set the directory path
AC_SUBST([gdb4hpc_srcdir], ["$(cd $srcdir/src && pwd)"])

dnl Check for non-default library paths
if test -d "/lib"; then
	LDFLAGS="$LDFLAGS -L/lib"
fi
if test -d "/lib64"; then
	LDFLAGS="$LDFLAGS -L/lib64"
fi

dnl Define --as-needed to the linker.
dnl This makes it so that only the dso's that we actually need are pulled in.
LDFLAGS="$LDFLAGS -Wl,--as-needed"

dnl Check for debug settings
if test "x$debug_mode" = xyes; then
	AC_DEFINE([DEBUG], [1], [Building gdb4hpc with debug mode.])
fi

dnl Check for valgrind settings
if test "x$valgrind_mode" = xyes; then
        AC_DEFINE([BE_VALGRIND], [1], [Building gdb4hpc with backend valgrind mode.])
fi

###########################
### Check for programs. ###
###########################
AM_PROG_AR
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_YACC
AC_PROG_SED


LT_INIT

AC_LANG([C++])
m4_include([m4/ax_cxx_compile_stdcxx.m4])
AX_CXX_COMPILE_STDCXX(20, noext, mandatory)

dnl Check for Cray's Parallel Environment
if test "x$with_cray" != xno; then
	dnl It would be more accurate for this do be AC_DEFINE([CTI]...) but there
    dnl are many many instances of #ifdef ALPS in gdb4hpc and converting that
	dnl would require a bit more work.
	AC_DEFINE([ALPS], [1], [Enable ALPS support in gdb4hpc.])
fi

##################################
### Check for DbgSrv protocols ###
##################################
dnl Check for mrnet
if test "x$mrnet_mode" = xyes; then
	AC_DEFINE([MRNET], [1], [Enable MRNet support in gdb4hpc.])
	AC_DEFINE_UNQUOTED([MRNET_ROOT_DIR], ["$mrnet_root"], [cray-mrnet default installation directory.])
fi

dnl Check for zmqnet
if test "x$zmqnet_mode" = xyes; then
	AC_DEFINE([ZMQNET], [1], [Enable zmqnet support in gdb4hpc.])
fi

############################
### Check for python.    ###
############################

if test "x$python_mode" = xyes; then
	AC_DEFINE([USE_PYTHON], [1], [Enable python support in gdb4hpc.])
	PKG_CHECK_MODULES([PYTHON3], [python3], [], [AC_MSG_ERROR([python3 package not found.])])
fi


############################
### Check for libraries. ###
############################

dnl Check for libmi
if test "x$GDB" != xno && test "x$mi_mode" != xno; then

	if test -f "$libmi_path/libmi.so"; then
		echo "testing $libmi_path/libmi.so"
		LDFLAGS="$LDFLAGS -L$libmi_path"
		AC_CHECK_LIB([mi],
					[MIVarDelete],
					[AC_SUBST([mi_libs], ["-lmi"])],
					[AC_MSG_ERROR([You must have libmi.so to compile gdb4hpc.])]
					)
	else
		if test -f "$libmi_path/lib/libmi.so"; then
			LDFLAGS="$LDFLAGS -L$libmi_path/lib -lstdc++"
			AC_CHECK_LIB([mi],
						[MIVarDelete],
						[AC_SUBST([mi_libs], ["-lmi"])],
						[AC_MSG_ERROR([You must have libmi.so to compile gdb4hpc.])]
						)
		else
			echo "testing else path = $libmi_path"
			AC_CHECK_LIB([mi],
						[MIVarDelete],
						[AC_SUBST([mi_libs], ["-lmi"])],
						[AC_MSG_ERROR([You must have libmi.so to compile gdb4hpc.])]
						)
		fi
	fi

	AC_DEFINE([GDBMI], [1], [Enable libmi support in gdb4hpc.])
fi

dnl Check for libssh2
if test -f "$libssh2_path/libssh2.so"; then
	LDFLAGS="$LDFLAGS -L$libssh2_path"
	AC_CHECK_LIB(	[ssh2],
					[libssh2_init],
					[AC_SUBST([ssh2_libs], ["-lssh2"])],
					[AC_MSG_ERROR([You must have libssh2.so to compile gdb4hpc.])]
					)
else
	if test -f "$libssh2_path/lib/libssh2.so"; then
		LDFLAGS="$LDFLAGS -L$libssh2_path/lib"
		AC_CHECK_LIB(	[ssh2],
						[libssh2_init],
						[AC_SUBST([ssh2_libs], ["-lssh2"])],
						[AC_MSG_ERROR([You must have libssh2.so to compile gdb4hpc.])]
						)
	else
		AC_CHECK_LIB(	[ssh2],
						[libssh2_init],
						[AC_SUBST([ssh2_libs], ["-lssh2"])],
						[AC_MSG_ERROR([You must have libssh2.so to compile gdb4hpc.])]
						)
	fi
fi

dnl Conditional check for mrnet libs
if test "x$mrnet_mode" = xyes; then
	dnl Check for stdc++
	AC_CHECK_LIB([stdc++],
		     [main],
		     [],
		     [AC_MSG_ERROR([libstdc++ not found.])]
		     )

	dnl Check for the mrnet libs
	AC_MSG_CHECKING([for libmrnet.so])
	if test -f "$mrnet_root/lib/libmrnet.so"; then
		AC_MSG_RESULT([$mrnet_root/lib/libmrnet.so])
	else
		AC_MSG_RESULT([not found])
		AC_MSG_ERROR([libmrnet.so not found in given path.])
	fi

	AC_MSG_CHECKING([for libxplat.a])
	if test -f "$mrnet_root/lib/libxplat.so"; then
		AC_MSG_RESULT([$mrnet_root/lib/libxplat.so])
	else
		AC_MSG_RESULT([not found])
		AC_MSG_ERROR([libxplat.so not found in given path.])
	fi

	LDFLAGS="$LDFLAGS -L$mrnet_root/lib"

	dnl add the mrnet libs to MRN_LIBS
	AC_SUBST([mrn_root], ["$mrnet_root"])
fi

dnl Conditional check for zmqnet libs
if test "x$zmqnet_mode" = xyes; then
	AC_MSG_CHECKING([for libzmqnet.so])
	if test -f "$zmqnet_root/lib/libzmqnet.so"; then
		AC_MSG_RESULT([$zmqnet_root/lib/zmqnet.so])
	else
		AC_MSG_RESULT([not found])
		AC_MSG_ERROR([libzmqnet.so not found in given path.])
	fi

	LDFLAGS="$LDFLAGS -L$zmqnet_root/lib"

	dnl add the zmqnet substitution
	AC_SUBST([zmqnet_root], ["$zmqnet_root"])
fi

if test "x$doctest_mode" = xyes; then
    AC_SUBST([doctest_root], ["$doctest_root"])
fi

dnl Conditional check for pkg-config libs

dnl
dnl Check for pkg-config
dnl
PKG_PROG_PKG_CONFIG

PKG_CHECK_MODULES([CRAY_CTI_FE],        [common_tools_fe],[],[AC_MSG_ERROR([libcommontools_fe.so not found.])])
PKG_CHECK_MODULES([CRAY_CTI_BE],        [common_tools_be],[],[AC_MSG_ERROR([libcommontools_be.so not found.])])
PKG_CHECK_MODULES([EDITLINE], 		[libedit],[],[AC_MSG_ERROR([libedit.so not found.])])

###############################
### Check for header files. ###
###############################
AC_FUNC_ALLOCA
AC_HEADER_STDC
AC_CHECK_HEADERS([arpa/inet.h fcntl.h float.h inttypes.h libintl.h limits.h malloc.h memory.h netdb.h netinet/in.h stddef.h stdint.h stdlib.h string.h strings.h sys/ioctl.h sys/param.h sys/socket.h sys/time.h unistd.h values.h wchar.h])

dnl Merge with above
AC_CHECK_HEADERS([arpa/inet.h netdb.h netinet/in.h stdlib.h string.h strings.h sys/param.h sys/socket.h unistd.h])


dnl Check for libmi headers
if test "x$GDB" != xno && test "x$mi_mode" != xno; then
	AC_MSG_CHECKING([for MI.h])
	if test -f "$libmi_path/MI.h"; then
		CPPFLAGS="$CPPFLAGS -I$libmi_path"
		AC_MSG_RESULT([$libmi_path/MI.h])
	else
		if test -f "$libmi_path/include/MI.h"; then
			CPPFLAGS="$CPPFLAGS -I$libmi_path/include"
			AC_MSG_RESULT([$libmi_path/include/MI.h])
		else
			AC_MSG_RESULT([not found])
			AC_MSG_ERROR([MI.h not found. Please provide install location of libmi to --with-gdbmi])
		fi
	fi
fi

dnl Check for libssh2 headers
AC_MSG_CHECKING([for libssh2.h])
if test -f "$libssh2_path/libssh2.h"; then
	CPPFLAGS="$CPPFLAGS -I$libssh2_path"
	AC_MSG_RESULT([$libssh2_path/libssh2.h])
else
	if test -f "$libssh2_path/include/libssh2.h"; then
		CPPFLAGS="$CPPFLAGS -I$libssh2_path/include"
		AC_MSG_RESULT([$libssh2_path/include/libssh2.h])
	else
		AC_MSG_RESULT([not found])
		AC_MSG_ERROR([libssh2.h not found. Please provide install location of libssh2 to --with-libssh2])
	fi
fi

dnl Check for mrnet headers
if test "x$mrnet_mode" = xyes; then
        dnl Add the default location to the search paths
        dnl TODO: Fix this, its not actually checking anything...
        if test -d "$mrnet_root/include"; then
                if test -d "$mrnet_root/include/mrnet"; then
                        CPPFLAGS="$CPPFLAGS -I$mrnet_root/include/mrnet"
                fi
                if test -d "$mrnet_root/include/xplat"; then
                        CPPFLAGS="$CPPFLAGS -I$mrnet_root/include/xplat"
                fi
                if test -d "$mrnet_root/include/boost"; then
                        CPPFLAGS="$CPPFLAGS -I$mrnet_root/include/boost"
                fi
        fi
fi

dnl Check for zmqnet headers
if test "x$zmqnet_mode" = xyes; then
	AC_MSG_CHECKING([for zmqnet.hpp])
	if test -f "$zmqnet_root/include/zmqnet.hpp"; then
		AC_MSG_RESULT([$zmqnet_root/include/zmqnet.hpp])
	else
		AC_MSG_RESULT([not found])
		AC_MSG_ERROR([zmqnet.hpp not found. Please provide install location of zmqnet to --with-zmqnet=<zmqnet directory root>])
	fi
fi


#####################################################################
### Check for typedefs, structures, and compiler characteristics. ###
#####################################################################
AC_TYPE_UID_T
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_TYPE_INT64_T
AC_TYPE_INT32_T
AC_TYPE_INT16_T
AC_TYPE_INT8_T
AC_TYPE_MODE_T
AC_TYPE_UINT64_T
AC_TYPE_UINT32_T
AC_TYPE_UINT16_T
AC_TYPE_UINT8_T
AC_CHECK_TYPES([ptrdiff_t])
AC_STRUCT_TM
AC_HEADER_TIME
AC_HEADER_STDBOOL

# Check if compiler supports "long long" type.
AC_CHECK_SIZEOF(long long)
if test $ac_cv_sizeof_long_long -ne 0; then
	AC_DEFINE(CC_HAS_LONG_LONG,1,[define if system has long long type.])
fi

# Check if compiler supports "long double" type.
AC_CHECK_SIZEOF(long double)
if test $ac_cv_sizeof_long_double -ne 0; then
	# Check if "long double" is longer than "double".
	AC_CHECK_SIZEOF(double)
	if test $ac_cv_sizeof_long_double -gt $ac_cv_sizeof_double; then
		AC_DEFINE(HAVE_LONG_DOUBLE,1,[define if system has long double type.])
	fi
fi

####################################
### Check for library functions. ###
####################################
AC_FUNC_FORK
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_FUNC_STRTOD
AC_FUNC_MEMCMP
AC_FUNC_MMAP
AC_FUNC_VPRINTF
AC_FUNC_ERROR_AT_LINE
AC_CHECK_FUNCS([asprintf bzero dup2 floor gethostbyname gethostname gettimeofday \
		inet_ntoa localtime_r memset memmove mkfifo mkdir munmap pow realpath \
		regcomp select setenv sigset sighold sigrelse socket sqrt strcasecmp \
		strchr strdup strerror strncasecmp strndup strpbrk \
		strrchr strstr strtol strtoul strtoull])

dnl merge with above
AC_CHECK_FUNCS([asprintf clock_gettime gethostbyname memset socket strcasecmp strchr strdup strrchr strstr])

AC_SUBST(LIBS,"")

##############################
### Set AM characteristics ###
##############################
dnl Generic conds
AM_CONDITIONAL([CRAY], [test "x$with_cray" != xno])

dnl DbgSrv proto conds
AM_CONDITIONAL([MRNET], [test "x$mrnet_mode" != xno])
AM_CONDITIONAL([ZMQNET], [test "x$zmqnet_mode" != xno])
AM_CONDITIONAL([PYTHON], [test "x$python_mode" != xno])
AM_CONDITIONAL([DOCTEST], [test "x$doctest_mode" != xno])

################
### Finalize ###
################
dnl Conditional run of guard-rd configure.ac
if test "x$mrnet_mode" != xno -o "x$zmqnet_mode" != xno; then
	AC_CONFIG_SUBDIRS([src/guard-rd])
fi

AC_CONFIG_FILES([
	Makefile
	src/Makefile
	src/unittest/Makefile
	src/bitset/Makefile
	src/filters/Makefile
	src/filters/mrnet/Makefile
	src/filters/zmqnet/Makefile
	src/script/Makefile
	src/client/Makefile
	src/dbgclnt/Makefile
	src/dbgcomm/Makefile
	src/dbgsrv/Makefile
	src/gram/Makefile
	src/useful/Makefile
])
AC_OUTPUT
