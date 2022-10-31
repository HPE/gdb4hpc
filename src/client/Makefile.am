USEFUL	= @gdb4hpc_srcdir@/useful
GRAM	= @gdb4hpc_srcdir@/gram
DBGCLNT	= @gdb4hpc_srcdir@/dbgclnt
DBGCOMM	= @gdb4hpc_srcdir@/dbgcomm
MRNET_ROOT	= @mrn_root@
BITSET	= @gdb4hpc_srcdir@/bitset
SCRIPT	= @gdb4hpc_srcdir@/script
ZMQNET_ROOT  = @zmqnet_root@
UNITTEST     = @gdb4hpc_srcdir@/unittest

AM_CXXFLAGS = -DBINDIR=\"$(bindir)\" -I$(USEFUL) -I$(GRAM) -I$(DBGCLNT) -I$(DBGCOMM) \
			  -I$(BITSET) -I$(SCRIPT) -I$(UNITTEST)
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
GUARD_RD_LIBS	= $(SCRIPT)/libdf.a

# Check for MRNet libraries
if MRNET
AM_CXXFLAGS	+= -I$(MRNET_ROOT)
MRN_LIBS 	= -L$(MRNET_ROOT)/lib -lmrnet 
endif

# Check for ZMQNet libraries
if ZMQNET
AM_CXXFLAGS	+= -I$(ZMQNET_ROOT)/include
ZMQN_LIBS 	= -L$(ZMQNET_ROOT)/lib -lzmqnet -lkj -lcapnp
endif

MI_LIBS         = @mi_libs@
SSH2_LIBS		= @ssh2_libs@

# add editline compile flags
AM_CXXFLAGS		+= $(EDITLINE_CFLAGS)


###############
# Build guard #
###############

bin_PROGRAMS	= gdb4hpc
gdb4hpc_SOURCES	= main.cpp class_cmd.cpp cli_funcs.cpp cmd.cpp help_dict.cpp cmd_assign.cpp cmd_break.cpp cmd_checkpoint.cpp \
		  cmd_comp.cpp cmd_env.cpp cmd_iostat.cpp cmd_frame.cpp cmd_func.cpp cmd_go.cpp cmd_list.cpp cmd_print.cpp cmd_proc.cpp \
		  cmd_procset.cpp cmd_source.cpp cmd_step.cpp cmd_thread.cpp cmd_whatis.cpp cli_readline.cpp cli_socket.cpp cli_file.cpp \
		  cmd_passthru.cpp cmd_script.cpp cmd_blockmap.cpp compare_util.cpp mi_funcs.cpp
gdb4hpc_LDFLAGS	= $(LDFLAGS) -Wl,-E -Wl,-z,origin -Wl,-rpath,\$$ORIGIN/../lib \
				  -Wl,-rpath,\$$ORIGIN -Wl,-rpath,/opt/cray/pe/gcc-libs \
				  -Wl,--enable-new-dtags
gdb4hpc_LDADD	= $(UNITTEST)/libgdb4_unittest.la $(GUARD_RD_LIBS) $(GRAM)/libgram.a \
				  $(DBGCLNT)/libdbgclnt.a $(DBGCLNT)/libdbgproto.a $(DBGCOMM)/libdbg.a $(USEFUL)/libuseful.a \
				  $(EDITLINE_LIBS) $(CRAYTOOL_LIBS) $(MRN_LIBS) $(ZMQN_LIBS) $(MI_LIBS) \
				  $(SSH2_LIBS) $(BITSET)/libbitset.la -lm -lstdc++ -lstdc++fs -lpthread
