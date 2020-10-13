USEFUL	= @guard_srcdir@/src/useful
GRAM	= @guard_srcdir@/src/gram
DBGCLNT	= @guard_srcdir@/src/dbgclnt
DBGCOMM	= @guard_srcdir@/src/dbgcomm
MRNET	= @guard_srcdir@/mrnet
SDM	= @guard_srcdir@/sdm

AM_CXXFLAGS = -DBINDIR=\"$(bindir)\" -I$(USEFUL) -I$(GRAM) -I$(DBGCLNT) -I$(DBGCOMM)
AM_LIBTOOLFLAGS = --quiet

#################################
# Check for optional components #
#################################

# Check for Cray libraries
if CRAY
CRAYTOOL_LIBS	= $(CRAY_CTI_FE_LIBS)
AM_CXXFLAGS 	+= $(CRAY_CTI_FE_CFLAGS)
endif

# Check for relative debugging libraries
if WITH_RD
GUARD_RD_LIBS	= @rd_libs@
endif

# Check for MRNet libraries
if MRN
AM_CXXFLAGS		+= -I$(MRNET) -I$(SDM)
MRN_LIBS 		= -L$(MRNET) -L$(SDM) -lmrnet_c -lmrnet -lbitset
endif

# Check for gdb-mi libraries
if GDBMI_BACKEND
MI_LIBS         = @mi_libs@
endif

SSH2_LIBS		= @ssh2_libs@

# add editline compile flags
AM_CXXFLAGS		+= $(EDITLINE_CFLAGS)


###############
# Build guard #
###############

bin_PROGRAMS	= gdb4hpc
gdb4hpc_SOURCES	= main.cpp class_cmd.cpp cli_funcs.cpp cmd.cpp help_dict.cpp cmd_assign.cpp cmd_break.cpp cmd_comp.cpp \
		  cmd_env.cpp cmd_frame.cpp cmd_func.cpp cmd_go.cpp cmd_list.cpp cmd_print.cpp cmd_proc.cpp \
		  cmd_procset.cpp cmd_source.cpp cmd_step.cpp cmd_thread.cpp cmd_whatis.cpp cli_readline.cpp cli_socket.cpp cli_file.cpp \
		  cmd_passthru.cpp
gdb4hpc_LDFLAGS	= $(LDFLAGS) -Wl,-E -Wl,-z,origin -Wl,-rpath,\$$ORIGIN/../lib -Wl,-rpath,\$$ORIGIN -Wl,-rpath,/opt/cray/pe/gcc-libs -Wl,--enable-new-dtags
gdb4hpc_LDADD	= $(GUARD_RD_LIBS) $(GRAM)/libgram.a $(DBGCLNT)/libdbgclnt.a \
		  $(DBGCLNT)/libdbgproto.a $(DBGCOMM)/libdbg.a $(USEFUL)/libuseful.a \
		  $(EDITLINE_LIBS) $(CRAYTOOL_LIBS) $(MRN_LIBS) $(MI_LIBS) $(SSH2_LIBS) -laif -lm -lstdc++ -lstdc++fs -lpthread

