AC_PREREQ([2.63])
AC_INIT([libaif],
	[1.0],
	[http://messagelab.monash.edu.au/Guard])
AC_CONFIG_SRCDIR([aif.h])
AC_CONFIG_AUX_DIR([config])
AC_CONFIG_HEADER([config.h])

AC_MSG_NOTICE([Configuring libaif])

AM_INIT_AUTOMAKE([foreign -Wall])

m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

# Checks for programs.
AC_PROG_CC
AC_PROG_RANLIB
AC_PROG_INSTALL

# Checks for libraries.

# Check endianness.
AC_C_BIGENDIAN

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

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([fcntl.h memory.h stdlib.h string.h sys/param.h unistd.h values.h strings.h sys/time.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_HEADER_TIME
AC_STRUCT_TM
AC_TYPE_INT32_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_FUNC_MEMCMP
AC_FUNC_VPRINTF
AC_CHECK_FUNCS([memmove memset strchr strdup strerror strrchr strstr strtol strtoul strdup])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
