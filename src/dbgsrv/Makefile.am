USEFUL	= @gdb4hpc_srcdir@/useful
GRAM	= @gdb4hpc_srcdir@/gram
DBGCOMM	= @gdb4hpc_srcdir@/dbgcomm
MRNET_ROOT	= @mrn_root@
BITSET  = @gdb4hpc_srcdir@/bitset
ZMQNET_ROOT  = @zmqnet_root@

AM_CXXFLAGS = -DBINDIR=\"$(bindir)\" -I$(USEFUL) -I$(GRAM) -I$(DBGCOMM)
AM_LIBTOOLFLAGS = --silent

######################
# Determine protocol #
######################

# Check for MRNet
if MRNET
AM_CXXFLAGS	+= -I$(MRNET_ROOT) -I$(BITSET)
MRN_LIBS	= -L$(MRNET_ROOT)/lib -lmrnet $(BITSET)/libbitset.la
MRN_SRV		= dbg_mrn_be.cpp dbgsrv_mrn.cpp mrn_topo.cpp
endif

# Check for ZMQNet
if ZMQNET
AM_CXXFLAGS += -I$(ZMQNET_ROOT)/include -I$(BITSET)
ZMQNET_LIBS  = -L$(ZMQNET_ROOT)/lib -lkj -lcapnp -lzmqnet $(BITSET)/libbitset.la
ZMQNET_SRV   = dbgsrv_zmq.cpp
endif

#####################
# Determine backend #
#####################

BACKEND		= dbgsrv_gdbmi.cpp dbgsrv_init.cpp dbgsrv_libmi2aif.cpp dbgsrv_util.cpp dbgsrv_p2p.cpp
MI_LIBS		= @mi_libs@

#################################
# Check for optional components #
#################################

# Check for Cray
if CRAY
CRAYTOOL_LIBS = $(CRAY_CTI_BE_LIBS)
AM_CXXFLAGS += $(CRAY_CTI_BE_CFLAGS)
CRAY_PROGS	= dbgsrv_alps

if MRNET
dbgsrv_alps_SOURCES 	= dbgsrv_alps_mrn.cpp
endif

dbgsrv_trampoline_SOURCES = dbgsrv_trampoline.c
dbgsrv_trampoline_LDFLAGS = -O0

dbgsrv_alps_LDADD	= $(USEFUL)/libuseful.a $(CRAYTOOL_LIBS)
dbgsrv_alps_LDFLAGS	= $(LDFLAGS) -Wl,-E -Wl,-z,origin -Wl,-rpath,\$$ORIGIN/../lib -Wl,-rpath,\$$ORIGIN -Wl,-rpath,/opt/cray/pe/lib64 -Wl,-rpath,/opt/cray/lib64 -Wl,--enable-new-dtags
endif

################
# Build dbgsrv #
################

libexec_PROGRAMS= dbgsrv $(CRAY_PROGS) dbgsrv_trampoline
dbgsrv_SOURCES	= dbgsrv.cpp dbgfn.cpp dbgsrv_proto.cpp dbgsrv_cmds.cpp \
		$(BACKEND) $(MRN_SRV) $(ZMQNET_SRV)
dbgsrv_LDADD	= $(USEFUL)/libuseful.a $(GRAM)/libgram.a $(DBGCOMM)/libdbg.a \
		$(MI_LIBS) $(MRN_LIBS) $(ZMQNET_LIBS) -lm $(CRAYTOOL_LIBS) -lstdc++ -lstdc++fs -lpthread
dbgsrv_LDFLAGS	= $(LDFLAGS) -Wl,-E -Wl,-z,origin -Wl,-rpath,\$$ORIGIN/../lib -Wl,-rpath,\$$ORIGIN -Wl,-rpath,/opt/cray/pe/lib64 -Wl,-rpath,/opt/cray/lib64 -Wl,--enable-new-dtags
