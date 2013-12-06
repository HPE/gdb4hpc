USEFUL	= @guard_srcdir@/src/useful
GRAM	= @guard_srcdir@/src/gram
DBGCLNT	= @guard_srcdir@/src/dbgclnt
DBGCOMM	= @guard_srcdir@/src/dbgcomm
MRNET	= @guard_srcdir@/mrnet
SDM	= @guard_srcdir@/sdm
EDITLINE_LIBS	= @editline_libs@

AM_CFLAGS = -DBINDIR=\"$(bindir)\" -I$(USEFUL) -I$(GRAM) -I$(DBGCLNT) -I$(DBGCOMM)

#################################
# Check for optional components #
#################################

# Check for Cray libraries
if CRAY
CRAYTOOL_LIBS	= @craytool_fe_libs@
endif

# Check for relative debugging libraries
if WITH_RD
GUARD_RD_LIBS	= @rd_libs@
endif

# Check for MRNet libraries
if MRN
AM_CFLAGS	+= -I$(MRNET) -I$(SDM)
MRN_LIBS 	= -L$(MRNET) -L$(SDM) -lmrnet_c -lmrnet -lbitset -lxplat
endif

# Check for gdb-mi libraries
if GDBMI_BACKEND
MI_LIBS         = @mi_libs@
endif

SSH2_LIBS	= @ssh2_libs@

###############
# Build guard #
###############

bin_PROGRAMS	= guard
guard_SOURCES	= main.c class_cmd.c cli_funcs.c cmd.c help_dict.c cmd_assign.c cmd_break.c cmd_comp.c \
		  cmd_env.c cmd_frame.c cmd_func.c cmd_go.c cmd_list.c cmd_print.c cmd_proc.c \
		  cmd_procset.c cmd_source.c cmd_step.c cmd_whatis.c cli_readline.c cli_socket.c cli_file.c \
		  cmd_passthru.c
guard_LDFLAGS	= $(LDFLAGS) -Wl,-E -Wl,-z,origin -Wl,-rpath,'$$ORIGIN'/../lib
guard_LDADD	= $(GUARD_RD_LIBS) $(GRAM)/libgram.a $(DBGCLNT)/libdbgclnt.a \
		  $(DBGCLNT)/libdbgproto.a $(DBGCOMM)/libdbg.a $(USEFUL)/libuseful.a \
		  $(EDITLINE_LIBS) $(CRAYTOOL_LIBS) $(MRN_LIBS) $(MI_LIBS) $(SSH2_LIBS) -laif

