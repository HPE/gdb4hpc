USEFUL	= @guard_srcdir@/src/useful
GRAM	= @guard_srcdir@/src/gram
DBGCOMM	= @guard_srcdir@/src/dbgcomm
MRNET	= @guard_srcdir@/mrnet
SDM	= @guard_srcdir@/sdm

AM_CFLAGS = -DBINDIR=\"$(bindir)\" -I$(USEFUL) -I$(GRAM) -I$(DBGCOMM)
AM_LIBTOOLFLAGS = --silent

######################
# Determine protocol #
######################

# Check for MRNet
if MRN
AM_CFLAGS	+= -I$(MRNET) -I$(SDM)
MRN_LIBS	= -L$(MRNET) -L$(SDM) -lmrnet_c -lbitset -lmrnet 
MRN_SRV		= dbg_mrn_be.c dbgsrv_mrn.c mrn_topo.c
endif

# Check for Socket
if SOCK
SOCK_SRV	= dbgsrv_soc.c
endif

#####################
# Determine backend #
#####################

# Check for gdb
if GDB_BACKEND
BACKEND		= dbgsrv_gdb.c dbgrexp.c dbgsrv_util.c dbgsrv_p2p.c
endif

# Check for gdb-mi
if GDBMI_BACKEND
BACKEND		= dbgsrv_gdbmi.c dbgsrv_libmi2aif.c dbgsrv_util.c dbgsrv_p2p.c
MI_LIBS		= @mi_libs@
endif

# Check for idb
if IDB_BACKEND
BACKEND		= dbgsrv_idb.c dbgrexp.c
endif

#################################
# Check for optional components #
#################################

# Check for Cray
if CRAY
CRAYTOOL_LIBS = $(CRAY_CTI_BE_LIBS)
AM_CFLAGS += $(CRAY_CTI_BE_CFLAGS)
CRAY_PROGS	= dbgsrv_alps

if MRN
dbgsrv_alps_SOURCES 	= dbgsrv_alps_mrn.c
endif

dbgsrv_alps_LDADD	= $(USEFUL)/libuseful.a $(CRAYTOOL_LIBS)
dbgsrv_alps_LDFLAGS	= $(LDFLAGS) -Wl,-E -Wl,-z,origin -Wl,-rpath,\$$ORIGIN/../lib -Wl,-rpath,\$$ORIGIN -Wl,--enable-new-dtags
endif

################
# Build dbgsrv #
################

libexec_PROGRAMS= dbgsrv $(CRAY_PROGS)
dbgsrv_SOURCES	= dbgsrv.c dbgfn.c dbgsrv_proto.c dbgsrv_cmds.c \
		$(BACKEND) $(MRN_SRV) $(SOCK_SRV)
dbgsrv_LDADD	= $(USEFUL)/libuseful.a $(GRAM)/libgram.a $(DBGCOMM)/libdbg.a \
		$(MI_LIBS) $(MRN_LIBS) -laif -lm $(CRAYTOOL_LIBS) -lstdc++
dbgsrv_LDFLAGS	= $(LDFLAGS) -Wl,-E -Wl,-z,origin -Wl,-rpath,\$$ORIGIN/../lib -Wl,-rpath,\$$ORIGIN -Wl,--enable-new-dtags

