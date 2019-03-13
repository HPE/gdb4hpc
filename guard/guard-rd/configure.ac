#
# ©2010-2012 Cray Inc.  All Rights Reserved.
# Copyright (c) 1996-2010 by Guardsoft Pty Ltd.
#
# Unpublished Proprietary Information.
# This unpublished work is protected to trade secret, copyright and other laws.  
# Except as permitted by contract or express written permission of Cray Inc., 
# no part of this work or its content may be used, reproduced or disclosed in 
# any form.
#
# $HeadURL$
# $Date$
# $Rev$
# $Author$
#

AC_PREREQ([2.63])

AC_INIT([guard-rd],[1.2],[andrewg@cray.com])
AC_COPYRIGHT([© 2010-2013 Cray Inc. All Rights Reserved.])

AC_CONFIG_SRCDIR([src/df/df.cpp])
AC_CONFIG_AUX_DIR([config])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([config.h])

AC_MSG_NOTICE([Configuring Relative Debugging Support for Guard.])

dnl Turn on Posix extensions
AC_USE_SYSTEM_EXTENSIONS

AM_INIT_AUTOMAKE([foreign -Wall])
LT_INIT

m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

#############################
### Check for --with tags ###
#############################
dnl Check for --with-aif
AC_ARG_WITH([aif], 
	    [AS_HELP_STRING([--with-aif=dir], 
			    [use aif in dir]
			    )
	    ], 
	    [with_aif=$withval], 
	    [with_aif=check]
	    )

dnl Check for --with-guard (used for independent building of guard-rd)
AC_ARG_WITH([guard], 
	    [AS_HELP_STRING([--with-guard=dir],
			    [use Guard in dir]
			    )
	    ], 
	    [with_guard=$withval], 
	    [with_guard=check]
	    )

dnl Check for --with-mrnet
AC_ARG_WITH([mrnet], 
	    [AS_HELP_STRING([--with-mrnet],
			    [Enable MRNet support in Guard.]
			    )
	    ],
	    [mrnet_mode=yes 
	     mrnet_root=$withval 
	     ], 
	    [mrnet_mode=no]
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

dnl Check for --with-perf
AC_ARG_WITH([perf],
	    [AS_HELP_STRING([--with-perf],
			    [Enable performance logging.]
			    )
	    ],
	    [perf_mode=yes],
	    [perf_mode=no]
	    )

#########################
### Setup env options ###
#########################
dnl Set the directory path
AC_SUBST([guardrd_srcdir], ["`pwd`"])

dnl Check for debug settings
if test "x$debug_mode" = xyes; then 
	AC_DEFINE([DEBUG], [1], [Building guard with debug mode.])
fi
dnl Check for perf settings
if test "x$perf_mode" = xyes; then
        AC_DEFINE([PERFLOG], [1], [Building guard with perf mode.])
fi

dnl Ensure RD is defined
AC_DEFINE([RD], [1], [Enable relative debugging support])

############################
### Check for guard tree ###
############################
dnl Check for guard source tree
AS_IF([test "x$with_guard" = xcheck], 
      [AS_IF([test -d "$srcdir/../guard"], 
	     [GUARD_SRC=$srcdir/../guard], 
	     [AS_IF([test -d "$srcdir/../src"], 
		    [GUARD_SRC=$srcdir/..], 
		    [AC_MSG_ERROR([Guard source directory not found.])]
		    )
	     ])
      ], 
      [AS_IF([test -d "$with_guard/src/useful"], 
	     [GUARD_SRC=$with_guard], 
	     [AC_MSG_ERROR([Invalid guard directory.])]
	     )
      ])
AC_SUBST([GUARD_SRC])

###########################
### Check for programs. ###
###########################
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_LIBTOOL
AC_PROG_LN_S

AC_LANG([C++])
m4_include([m4/ax_cxx_compile_stdcxx.m4])
AX_CXX_COMPILE_STDCXX(11, noext, mandatory)

##################################
### Check for DbgSrv protocols ###
##################################
dnl Check for mrnet
if test "x$mrnet_mode" = xyes; then 
	AC_DEFINE([MRNET], [1], [Enable MRNet support in Guard.]) 
	AC_DEFINE([AGGMODE], [1], [Enable MRNet aggrigation support in Guard.]) 
fi 

############################
### Check for libraries. ###
############################

dnl Check for AIF
if test "x$with_aif" != xcheck; then 
	if test "x$with_aif" = x; then 
		AC_CHECK_LIB([aif], 
			     [NewAIF], 
			     [AC_SUBST([aif_libs], ["-laif"])], 
			     [AC_MSG_ERROR([You must have libaif to compile GUARD.])]
			     ) 
	else 
		AC_MSG_CHECKING([for libaif.a]) 
		if test -f "$with_aif/lib/libaif.a"; then 
			AC_MSG_RESULT([$with_aif/lib/libaif.a]) 
		else 
			AC_MSG_RESULT([not found]) 
			AC_MSG_ERROR([libaif.a not found in given path.]) 
		fi 
		
		LDFLAGS="$LDFLAGS -L$with_aif/lib" 

		AC_CHECK_LIB([aif], 
			     [NewAIF], 
			     [AC_SUBST([aif_libs], ["-laif"])], 
			     [AC_MSG_ERROR([libaif not found.])]
			     )
	fi
else
	AC_CHECK_LIB([aif], 
		     [NewAIF], 
		     [AC_SUBST([aif_libs], ["-laif"])], 
		     [AC_MSG_ERROR([You must have libaif to compile GUARD.])]
		     )
fi

###############################
### Check for header files. ###
###############################
AC_CHECK_HEADERS([arpa/inet.h netdb.h netinet/in.h stdlib.h string.h strings.h sys/param.h sys/socket.h unistd.h])

dnl Check for aif headers
if test "x$with_aif" != xcheck; then
	if test "x$with_aif" = x; then 
		dnl Error condition. Check default system location.  
		AC_CHECK_HEADER([aif.h], 
				[], 
				[AC_MSG_ERROR([You must have libaif to compile GUARD.])]
				) 
		AC_CHECK_HEADER([fds.h],
				[], 
				[AC_MSG_ERROR([You must have libaif to compile GUARD.])]
				) 
	else
		AC_MSG_CHECKING([for aif.h])
		if test -f "$with_aif/include/aif.h"; then
			AC_MSG_RESULT([$with_aif/include/aif.h]) 
		else
			AC_MSG_RESULT([not found])
			AC_MSG_ERROR([aif.h not found in given path.])
		fi
		
		AC_MSG_CHECKING([for fds.h]) 
		if test -f "$with_aif/include/fds.h"; then 
			AC_MSG_RESULT([$with_aif/include/fds.h]) 
		else 
			AC_MSG_RESULT([not found]) 
			AC_MSG_ERROR([fds.h not found in given path.]) 
		fi

		dnl if we get here, we have passed the checks.
		CPPFLAGS="$CPPFLAGS -I$with_aif/include"
	fi
else
	dnl Error condition. Check default system location.
	AC_CHECK_HEADER([aif.h], 
			[], 
			[AC_MSG_ERROR([You must have libaif to compile GUARD.])]
			) 
			
	AC_CHECK_HEADER([fds.h], 
			[], 
			[AC_MSG_ERROR([You must have libaif to compile GUARD.])]
			)
fi

#####################################################################
### Check for typedefs, structures, and compiler characteristics. ###
#####################################################################
AC_HEADER_STDBOOL
AC_HEADER_STDC
AC_TYPE_INT32_T

####################################
### Check for library functions. ###
####################################
AC_FUNC_FORK
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([asprintf clock_gettime gethostbyname memset socket strcasecmp strchr strdup strrchr strstr])

################
### Finalize ###
################
AC_CONFIG_FILES([Makefile
		 src/df/Makefile
		 src/rd/Makefile])

AC_OUTPUT
