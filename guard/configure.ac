#
# © 2010-2019 Cray Inc. All Rights Reserved
# Copyright (c) 1996-2010 by Guardsoft Pty Ltd.
#
# Unpublished Proprietary Information.
# This unpublished work is protected to trade secret, copyright and other laws.
# Except as permitted by contract or express written permission of Cray Inc.,
# no part of this work or its content may be used, reproduced or disclosed
# in any form.
#

AC_PREREQ([2.63])

AC_INIT([guard],[1.2],[andrewg@cray.com])
AC_COPYRIGHT([© 2010-2014 Cray Inc. All Rights Reserved.])

AC_CONFIG_SRCDIR([defs.h])
AC_CONFIG_AUX_DIR([config])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([config.h])

AC_MSG_NOTICE([Configuring Guard])

dnl Turn on Posix extensions
AC_USE_SYSTEM_EXTENSIONS
#AC_GNU_SOURCE

AM_INIT_AUTOMAKE([foreign -Wall])
LT_INIT

m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

#############################
### Check for --with tags ###
#############################
dnl Check for --with-editline	   
AC_ARG_WITH([editline], 
	    [AS_HELP_STRING([--with-editline=dir], 
			    [Use editline library. Optional dir argument to override system location.]
			    )
	    ],
	    [with_editline=$withval], 
	    [with_editline=no]
	    )

dnl Check for --with-aif
AC_ARG_WITH([aif], 
	    [AS_HELP_STRING([--with-aif=dir], 
			    [Use libaif install located in dir. Libaif is required to build guard.]
			    )
	    ], 
	    [with_aif=$withval], 
	    [with_aif=check]
	    )

dnl Check for --with-mpi
AC_ARG_WITH([mpi], 
	    [AS_HELP_STRING([--with-mpi], 
			    [Enable standard MPICH support in Guard.]
			    )
	    ], 
	    [with_mpirun=yes], 
	    [with_mpirun=no]
	    )

dnl Check for --with-ompi
AC_ARG_WITH([ompi], 
	    [AS_HELP_STRING([--with-ompi], 
			    [Enable open mpi support in Guard.]
			    )
	    ],
	    [with_ompi_info=yes],
	    [with_ompi_info=no]
	    )

dnl Check for --with-p4 (P4 Parallel Programming system from ANL)
AC_ARG_WITH([p4], 
	    [AS_HELP_STRING([--with-p4], 
			    [Enable ANL P4 Parallel Programming System support in Guard.]
			    )
	    ], 
	    [AC_DEFINE([P4], 
		       [1], 
		       [Enable P4 support in Guard.]
		       )
	    ], 
	    [])

dnl Check for --with-ibm
AC_ARG_WITH([ibm], 
	    [AS_HELP_STRING([--with-ibm], 
			    [Enable IBM POE support in Guard.]
			    )
	    ], 
	    [with_poe=yes],
	    [with_poe=no]
	    )

dnl Check for --with-cray
AC_ARG_WITH([cray], 
	    [AS_HELP_STRING([--with-cray], 
			    [Enable Cray APRUN support in Guard.]
			    )
	    ],
	    [with_cray=yes],
	    [with_cray=no]
	    )

dnl check for --with-cray sub-arguments
if test "x$with_cray" != xno; then
	dnl Check for --with-craytool-inc
	AC_ARG_WITH([craytool-inc],
		    [AS_HELP_STRING([--with-craytool-inc],
				    [Location of the craytool header files.]
				    )
		    ],
		    [CPPFLAGS="$CPPFLAGS -I$withval"],
		    [AC_MSG_WARN([Using default path to the craytool headers.])]
		    )

	dnl Check for --with-craytool-lib
	AC_ARG_WITH([craytool-lib],
		    [AS_HELP_STRING([--with-craytool-lib],
				    [Location of the craytool libraries.]
				    )
		    ],
		    [LDFLAGS="$LDFLAGS -L$withval"
		     AC_SUBST([craytool_path], ["$withval"])
		     libcraytool_path="$withval"
		    ],
		    [AC_MSG_WARN([Using default path to craytool libraries.])]
		    )
fi

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

dnl Check for --with-socket
AC_ARG_WITH([socket], 
	    [AS_HELP_STRING([--with-socket], 
			    [Enable legacy socket support in Guard.]
			    )
	    ], 
	    [socket_mode=yes], 
	    [socket_mode=no]
	    )

dnl Check for --with-idb
AC_ARG_WITH([idb], 
	    [AS_HELP_STRING([--with-idb], 
			    [Enable IDB backend support in Guard.]
			    )
	    ], 
	    [idb_mode=yes], 
	    [idb_mode=no]
	    )

dnl Check for --with-gdb
AC_ARG_WITH([gdb], 
	    [AS_HELP_STRING([--with-gdb], 
			    [Enable GDB regex backend support in Guard.]
			    )
	    ], 
	    [gdb_mode=yes], 
	    [gdb_mode=no]
	    )

dnl Check for --with-gdbmi
AC_ARG_WITH([gdbmi], 
	    [AS_HELP_STRING([--with-gdbmi], 
			    [Enable GDB/MI backend support in Guard.]
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

dnl Check for --with-rd
AC_ARG_WITH([rd], 
	    [AS_HELP_STRING([--with-rd], 
			   [include relative debugging support.]
			   )
	    ], 
	    [with_rd=yes],
	    [with_rd=no]
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
	    
dnl Check for --with-backend-perf
AC_ARG_WITH([backend-perf],
	    [AS_HELP_STRING([--with-backend-perf],
			    [Enable extra backend performance logging options with added overhead.]
			    )
	    ],
	    [be_perf_mode=yes],
	    [be_perf_mode=no]
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

#########################
### Setup env options ###
#########################
dnl Set the directory path
AC_SUBST([guard_srcdir], ["`pwd`"])

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
	AC_DEFINE([DEBUG], [1], [Building guard with debug mode.])
fi

dnl Check for perf settings
if test "x$perf_mode" = xyes; then
        AC_DEFINE([PERFLOG], [1], [Building guard with perf mode.])
fi

dnl Check for backend perf settings
if test "x$be_perf_mode" = xyes; then
	AC_DEFINE([BE_PERFLOG], [1], [Building guard with backend perf mode.])
fi

dnl Check for valgrind settings
if test "x$valgrind_mode" = xyes; then
        dnl Check for valgrind
        AC_PATH_PROG([VALGRIND], [valgrind], [no])
        if test "x$VALGRIND" = xno; then
                AC_MSG_ERROR([You must have valgrind to enable valgrind mode in GUARD.])
        fi
        AC_DEFINE([BE_VALGRIND], [1], [Building guard with backend valgrind mode.])
fi

##########################################
### Check for relative debugging build ###
##########################################
dnl Setup rd build environment
if test "x$with_rd" = xyes; then 
	AC_DEFINE([RD], [1], [Enable relative debugging support]) 
	AC_SUBST([rd_dir], ["$guard_srcdir/guard-rd/src/rd"]) 
	AC_SUBST([df_dir], ["$guard_srcdir/guard-rd/src/df"]) 
	AC_SUBST([rd_libs], ["$rd_dir/librd.a $df_dir/libdf.a"]) 
	CPPFLAGS="$CPPFLAGS -I$rd_dir" 
	CPPFLAGS="$CPPFLAGS -I$df_dir" 
fi

###########################
### Check for programs. ###
###########################
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_YACC
AC_PROG_SED

AC_LANG([C++])
m4_include([m4/ax_cxx_compile_stdcxx.m4])
AX_CXX_COMPILE_STDCXX(11, noext, mandatory)

dnl Check for GDB
AC_PATH_PROG([GDB_GUARD], [gdb-guard], [no])
if test "x$GDB_GUARD" != xno; then
	AC_DEFINE_UNQUOTED([GDB], ["$ac_cv_path_GDB_GUARD"], [Location of gdb command.])
	AC_DEFINE([GDB_WITH_AIF], [1], [Using gdb with AIF support.])
else
	AC_PATH_PROG([GDB], [gdb], [no])
	if test "x$GDB" != xno; then
		AC_DEFINE_UNQUOTED([GDB], ["$ac_cv_path_GDB"], [Location of gdb debugger.])
	fi
fi

dnl Check for idb
if test "x$idb_mode" != xno; then
        AC_PATH_PROG([IDB], [idb], [no])
        if test "x$IDB" != xno; then
	        AC_DEFINE_UNQUOTED([IDB], ["$ac_cv_path_IDB"], [Location of idb command.])
        fi
fi

dnl Check for rsh
AC_PATH_PROG([RSH], [rsh], [no])
if test "x$RSH" != xno; then
	AC_DEFINE_UNQUOTED([RSH], ["$ac_cv_path_RSH"], [Location of rsh command.])
fi

dnl Check for ssh
AC_PATH_PROG([SSH], [ssh], [no])
if test "x$SSH" != xno; then
	AC_DEFINE_UNQUOTED([SSH], ["$ac_cv_path_SSH"], [Location of ssh command.])
fi

dnl Check for bpsh
AC_PATH_PROG([BPSH], [bpsh], [no])
if test "x$BPSH" != xno; then
	AC_DEFINE([BPROC], [1],[Enable BPROC support in Guard.])
	AC_DEFINE_UNQUOTED([BPSH], ["$ac_cv_path_BPSH"], [Location of bpsh command.])
fi

dnl Check for runtest
dnl used to test guard
AC_PATH_PROG([DEJA_RUNTEST], [runtest], [no])
if test "x$DEJA_RUNTEST" != xno; then
	if test "x$DEJAGNU" = x; then
		DEJAGNU="$guard_srcdir/testsuite/global-config.exp"
	fi
	AC_SUBST([DEJAGNU])
	dnl doesn't work for now
	#AC_CONFIG_SUBDIRS([testsuite])
fi

dnl Check for xmlto
dnl used to build documentation (Redhat 7.3)
AC_PATH_PROG([XMLTO], [xmlto], [no])
if test "x$XMLTO" = xno; then
	XMLTO=":"
fi

dnl Check for tar
AC_PATH_PROGS([TAR], [gtar tar], [no])
if test "x$TAR" = xno; then
	AC_MSG_WARN([You must have GNU TAR to compile the tar.gz distribution of Guard.])
fi

##############################
### Check for MP Starters. ###
##############################
dnl Check for mpirun
if test "x$with_mpirun" != xno ; then 
	AC_PATH_PROG([MPIRUN], [mpirun], [no])
	if test "x$MPIRUN" != xno; then
		AC_DEFINE([MPICH], [1], [Enable MPICH support in Guard]) 
		AC_DEFINE_UNQUOTED([MPIRUN], ["$ac_cv_path_MPIRUN"], [Location of mpirun command.]) 
		AC_PATH_PROG([MPDTRACE], [mpdtrace], [no]) 
		if test "x$MPDTRACE" != xno; then
			AC_DEFINE([MPICH_MPD], [1], [Enable MPICH support in Guard with mpd]) 
			AC_DEFINE_UNQUOTED([MPDTRACE], ["$ac_cv_path_MPDTRACE"], [Location of mpdtrace command.]) 
		fi
	else
		AC_MSG_ERROR([You must have mpirun to enable mpich support in GUARD.])
	fi
fi

dnl Check for ompi_info 
if test "x$with_ompi_info" != xno; then
	AC_PATH_PROG([OMPI_INFO], [ompi_info], [no])
	if test "x$OMPI_INFO" != xno; then 
		AC_PATH_PROG(MPIRUN, mpirun, no) 
		if test "x$ac_cv_path_MPIRUN" != xno; then
			AC_DEFINE([OMPI], [1], [Enable Open MPI support in Guard.]) 
			AC_DEFINE_UNQUOTED([MPIRUN], ["$ac_cv_path_MPIRUN"], [Location of mpirun command.]) 
		fi 
	fi
fi

dnl Check for IBM's Parallel Environment
if test "x$with_poe" != xno; then
	AC_PATH_PROG([POE], [poe], [no])
	if test "x$POE" != xno; then 
		AC_DEFINE([AIX_POE], [1], [Enable POE support in Guard.]) 
		AC_DEFINE_UNQUOTED([POE], ["$ac_cv_path_POE"], [Location of poe command.]) 
	else
		AC_MSG_ERROR([You must have poe to enable ibm support in GUARD.])
	fi
fi

dnl Check for Cray's Parallel Environment
if test "x$with_cray" != xno; then
	dnl This now always works for build machines that lack the aprun binary
	AC_DEFINE([ALPS], [1], [Enable ALPS support in Guard.])
fi

##################################
### Check for DbgSrv protocols ###
##################################
dnl Check for mrnet
if test "x$mrnet_mode" = xyes; then
	AC_DEFINE([MRNET], [1], [Enable MRNet support in Guard.])
fi

dnl Check for socket
if test "x$socket_mode" = xyes; then
	AC_DEFINE([SOCKETS], [1], [Enable legacy socket support in Guard.])
fi

##############################
### Check for Dbg Backends ###
##############################
dnl Check for idb
if test "x$IDB" != xno && test "x$idb_mode" != xno; then
	AC_DEFINE([IDB_BACKEND], [1], [Enable IDB backend support in Guard.])
fi

dnl Check for gdb
if test "x$GDB" != xno && test "x$gdb_mode" != xno; then
	AC_DEFINE([GDB_BACKEND], [1], [Enable GDB backend support in Guard.])
fi

dnl Check for gdb/mi
if test "x$GDB" != xno && test "x$mi_mode" != xno; then
	AC_DEFINE([GDBMI_BACKEND], [1], [Enable GDB/MI backend support in Guard.])
fi

############################
### Check for libraries. ###
############################

dnl Check for editline libraries
if test "x$with_editline" != xno; then
        dnl We can use the editline library to provide readline wrappers. Note
        dnl that editline is under a BSD license whereas readline is GPL.
        dnl
	dnl check if the user provided a withval
	if test "x$with_editline" = x; then
		dnl default to the system location
		dnl first check if editline needs any deplibs
		AC_CHECK_FUNC([tgetstr],
			      [EDITLINE_DEPLIB=""],
			      [AC_CHECK_LIB([curses],
					    [tgetstr],
					    [EDITLINE_DEPLIB="-lcurses"],
					    [AC_CHECK_LIB([ncurses],
							  [tgetstr],
							  [EDITLINE_DEPLIB="-lncurses"],
							  [AC_CHECK_LIB([termcap],
									[tgetstr],
									[EDITLINE_DEPLIB="-ltermcap"],
									[AC_MSG_ERROR([No curses or termcap library found!])]
									)
							  ])
					    ])
			      ])
		dnl now try to check for editline
		AC_CHECK_LIB([edit],
			     [readline],
			     [AC_SUBST([editline_libs],
				       ["-ledit $EDITLINE_DEPLIB"]
				       )
			     ],
			     [AC_MSG_ERROR([Editline library not found in system path.])],
			     [$EDITLINE_DEPLIB]
			     )
	else
		dnl first add the given location to the LDFLAGS
		LDFLAGS="$LDFLAGS -L$with_editline/lib"
		dnl check to see if we need any deplibs
		AC_CHECK_FUNC([tgetstr],
			      [EDITLINE_DEPLIB=""],
			      [AC_CHECK_LIB([curses],
					    [tgetstr],
					    [EDITLINE_DEPLIB="-lcurses"],
					    [AC_CHECK_LIB([ncurses],
							  [tgetstr],
							  [EDITLINE_DEPLIB="-lncurses"],
							  [AC_CHECK_LIB([termcap],
								        [tgetstr],
								        [EDITLINE_DEPLIB="-ltermcap"],
								        [AC_MSG_ERROR([No curses or termcap library found!])]
								        )
							  ])
					    ])
			      ])
		dnl make sure that libedit.a exists
		AC_MSG_CHECKING([for libedit.a])
		if test -f "$with_editline/lib/libedit.a"; then
			AC_MSG_RESULT([$with_editline/lib/libeditline.a])
			AC_CHECK_LIB([edit],
				     [readline],
				     [],
				     [AC_MSG_ERROR([Editline library is missing readline()!])],
				     [$EDITLINE_DEPLIB]
				     )
		else
			AC_MSG_RESULT([not found])
			AC_MSG_ERROR([libedit.a not found in given path.])
		fi
		
		dnl if we get here, we have passed the checks.
		AC_SUBST([editline_libs], ["-ledit $EDITLINE_DEPLIB"])		
	fi
else
	dnl For now, editline support is required. This check is in place when
	dnl code is added to disable editline support. Note that GNU readline
	dnl is released under the gpl but editline is under a BSD license.
	AC_MSG_WARN([editline support is required to build guard.])
fi


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

dnl Check for libmi
if test "x$GDB" != xno && test "x$mi_mode" != xno; then 	
	if test -f "$libmi_path/libmi.so"; then 
		LDFLAGS="$LDFLAGS -L$libmi_path" 
		AC_CHECK_LIB([mi],
					[MIVarDelete],
					[AC_SUBST([mi_libs], ["-lmi"])],
					[AC_MSG_ERROR([You must have libmi.so to compile GUARD.])]
					)
	else	
		if test -f "$libmi_path/lib/libmi.so"; then
			LDFLAGS="$LDFLAGS -L$libmi_path/lib"
			AC_CHECK_LIB([mi],
						[MIVarDelete],
						[AC_SUBST([mi_libs], ["-lmi"])],
						[AC_MSG_ERROR([You must have libmi.so to compile GUARD.])]
						)
		else
			AC_CHECK_LIB([mi],
						[MIVarDelete],
						[AC_SUBST([mi_libs], ["-lmi"])],
						[AC_MSG_ERROR([You must have libmi.so to compile GUARD.])]
						)
		fi
	fi

	AC_DEFINE([GDBMI], [1], [Enable libmi support in Guard.])
fi

dnl Check for libssh2
if test -f "$libssh2_path/libssh2.so"; then 
	LDFLAGS="$LDFLAGS -L$libssh2_path" 
	AC_CHECK_LIB(	[ssh2],
					[libssh2_init],
					[AC_SUBST([ssh2_libs], ["-lssh2"])],
					[AC_MSG_ERROR([You must have libssh2.so to compile GUARD.])]
					)
else	
	if test -f "$libssh2_path/lib/libssh2.so"; then
		LDFLAGS="$LDFLAGS -L$libssh2_path/lib"
		AC_CHECK_LIB(	[ssh2],
						[libssh2_init],
						[AC_SUBST([ssh2_libs], ["-lssh2"])],
						[AC_MSG_ERROR([You must have libssh2.so to compile GUARD.])]
						)
	else
		AC_CHECK_LIB(	[ssh2],
						[libssh2_init],
						[AC_SUBST([ssh2_libs], ["-lssh2"])],
						[AC_MSG_ERROR([You must have libssh2.so to compile GUARD.])]
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
	AC_MSG_CHECKING([for libmrnet.a])
	if test -f "$mrnet_root/lib/libmrnet.a"; then
		AC_MSG_RESULT([$mrnet_root/lib/libmrnet.a])
	else
		AC_MSG_RESULT([not found])
		AC_MSG_ERROR([libmrnet.a not found in given path.])
	fi
	
	# NOTE: AC_CHECK_LIB doesn't handle c++ name mangling. So attempting to
	#       test for the mrnet stuff seems a bit futile. Just assume that 
	#       if we can find the .a that it is valid.
	
	AC_MSG_CHECKING([for libxplat.a])
	if test -f "$mrnet_root/lib/libxplat.a"; then
		AC_MSG_RESULT([$mrnet_root/lib/libxplat.a])
	else
		AC_MSG_RESULT([not found])
		AC_MSG_ERROR([libxplat.a not found in given path.])
	fi

	LDFLAGS="$LDFLAGS -L$mrnet_root/lib"
	
	dnl add the mrnet libs to MRN_LIBS
	AC_SUBST([mrn_root], ["$mrnet_root"])
fi

dnl Conditional check for CTI libs

dnl
dnl Check for pkg-config
dnl
PKG_PROG_PKG_CONFIG

PKG_CHECK_MODULES([CRAY_CTI_FE], [craytools_fe],[],[AC_MSG_ERROR([libcraytools_fe.so not found.])])
PKG_CHECK_MODULES([CRAY_CTI_BE], [craytools_be],[],[AC_MSG_ERROR([libcraytools_be.so not found.])])

###############################
### Check for header files. ###
###############################
AC_FUNC_ALLOCA
AC_HEADER_STDC
AC_CHECK_HEADERS([arpa/inet.h fcntl.h float.h inttypes.h libintl.h limits.h malloc.h memory.h netdb.h netinet/in.h stddef.h stdint.h stdlib.h string.h strings.h sys/param.h sys/socket.h sys/time.h unistd.h values.h wchar.h])

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


dnl Check for editline headers
if test "x$with_editline" != xno; then
        dnl We can use the editline library to provide readline wrappers. Note
        dnl that editline is under a BSD license whereas readline is GPL.
        dnl
        dnl check if the user provided a withval
        if test "x$with_editline" = x; then
		AC_CHECK_HEADER([editline/readline.h],
				[AC_DEFINE([HAVE_LIBEDITLINE], 
					   [1], 
					   [Define if you have libeditline]
					   )
				],
				[AC_MSG_ERROR([You must have editline/readline.h to enable editline support in GUARD.])]
				)
	else
		AC_MSG_CHECKING([for readline.h])
		if test -f "$with_editline/include/editline/readline.h"; then
			AC_MSG_RESULT([$with_editline/include/editline/readline.h])
		else
			AC_MSG_RESULT([not found])
			AC_MSG_ERROR([readline.h not found in given path.])
		fi

		dnl if we get here, we have passed the checks.
		CPPFLAGS="$CPPFLAGS -I$with_editline/include"

		AC_DEFINE([HAVE_LIBEDITLINE], [1], [Define if you have libeditline])
		AC_DEFINE([CUSTOM_EDITLINE], [1], [Define if you have libeditline source])
	fi
else
	dnl For now editline support is required.
	AC_MSG_ERROR([You must have editline/readline.h to build GUARD.])
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
AC_TYPE_UINT64_T
AC_TYPE_UINT32_T
AC_TYPE_UINT16_T
AC_TYPE_UINT8_T
AC_CHECK_TYPES([ptrdiff_t])
AC_STRUCT_TM
AC_HEADER_TIME
AC_HEADER_STDBOOL

AC_C_INLINE

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
		inet_ntoa memset mkfifo mkdir munmap pow regcomp select \
		setenv sigset sighold sigrelse socket sqrt strcasecmp \
		strchr strdup strerror strncasecmp strndup strpbrk \
		strrchr strstr strtol strtoul strtoull])

AC_SUBST(LIBS,"")

##############################
### Set AM characteristics ###
##############################
dnl Generic conds
AM_CONDITIONAL([CRAY], [test "x$with_cray" != xno])
AM_CONDITIONAL([WITH_RD], [test "x$with_rd" != xno])

dnl DbgSrv proto conds
AM_CONDITIONAL([MRN], [test "x$mrnet_mode" != xno])
AM_CONDITIONAL([SOCK], [test "x$socket_mode" != xno])

dnl Backend conds
AM_CONDITIONAL([GDB_BACKEND], [test "x$gdb_mode" != xno])
AM_CONDITIONAL([GDBMI_BACKEND], [test "x$mi_mode" != xno])
AM_CONDITIONAL([IDB_BACKEND], [test "x$idb_mode" != xno])

dnl Lib conds
AM_CONDITIONAL([HDF_LIBS], [test "x$mfhdf" = xyes || test "x$hdf5" = xyes])

################
### Finalize ###
################
dnl Conditional run of guard-rd configure.ac
if test "x$mrnet_mode" != xno; then
	AC_CONFIG_SUBDIRS([guard-rd])
fi

AC_CONFIG_FILES([Makefile
		 doc/Makefile
		 sdm/Makefile
		 scripts/Makefile
		 filters/Makefile
		 filters/mrnet/Makefile
		 filters/sci/Makefile
		 mrnet/Makefile
		 src/client/Makefile
		 src/dbgclnt/Makefile
		 src/dbgcomm/Makefile
		 src/dbgsrv/Makefile
		 src/gram/Makefile
		 src/proxy/Makefile
		 src/useful/Makefile])
AC_OUTPUT
