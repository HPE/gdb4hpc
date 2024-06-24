USEFUL	= @gdb4hpc_srcdir@/useful
GRAM	= @gdb4hpc_srcdir@/gram
DBGCOMM	= @gdb4hpc_srcdir@/dbgcomm
MRNET_ROOT	= @mrn_root@
BITSET  = @gdb4hpc_srcdir@/bitset
UNITTEST   = @gdb4hpc_srcdir@/unittest
TINYSTL_ROOT = @tinystl_root@

AM_CXXFLAGS = -DBINDIR=\"$(bindir)\" -I$(USEFUL) -I$(GRAM) -I$(DBGCOMM) -I$(UNITTEST)
AM_LIBTOOLFLAGS = --silent

######################
# Determine protocol #
######################

# Check for MRNet
if MRNET
AM_CXXFLAGS	+= -I$(MRNET_ROOT) -I$(BITSET)
MRN_LIBS	= -L$(MRNET_ROOT)/lib -lmrnet
MRN_SRV		= dbg_mrn_be.cpp dbgsrv_mrn.cpp mrn_topo.cpp
endif

# Check for ZMQNet
if ZMQNET
AM_CXXFLAGS += $(ZMQNET_CFLAGS) -I$(BITSET)
ZMQNET_SRV   = dbgsrv_zmq.cpp
endif

#####################
# Determine backend #
#####################

BACKEND		= dbgsrv_gdbmi.cpp dbgsrv_init.cpp dbgsrv_libmi2aif.cpp dbgsrv_typeparser.cpp dbgsrv_util.cpp dbgsrv_p2p.cpp
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

dbgsrv_alps_LDADD	= $(UNITTEST)/libgdb4_unittest.la $(CRAYTOOL_LIBS) \
					  $(DBGCOMM)/libdbg.a $(USEFUL)/libuseful.a 
dbgsrv_alps_LDFLAGS	= $(LDFLAGS) -Wl,-E -Wl,-z,origin -Wl,-rpath,\$$ORIGIN/../lib \
					  -Wl,-rpath,\$$ORIGIN -Wl,-rpath,/opt/cray/pe/lib64 \
					  -Wl,-rpath,/opt/cray/lib64 -Wl,--enable-new-dtags
endif

################
# Build dbgsrv #
################

# TODO write in python.md why this doesn't have libstd++
lib_LTLIBRARIES = libgdb4_pybe.la
libgdb4_pybe_la_SOURCES = pybe_entry.cpp pybe_stl.cpp
libgdb4_pybe_la_CXXFLAGS = -g -Og $(PYTHON3_CFLAGS) -I$(TINYSTL_ROOT)/include \
                           -fPIC -fvisibility=hidden -fno-rtti -fno-exceptions
libgdb4_pybe_la_LDFLAGS = -version-info 0:0:0 -nostdlib++ -Wl,--no-undefined \
	                        $(PYTHON3_LIBS) -pthread
# The following is necessary to avoid pulling in libstdc++
libgdb4_pybe_la_LINK = $(LIBTOOL) --tag=CC $(AM_LIBTOOLFLAGS) $(LIBTOOLFLAGS) \
              --mode=link $(CCLD) $(libgdb4_pybe_la_CFLAGS) $(CFLAGS) \
              $(libgdb4_pybe_la_LDFLAGS) $(LTLDFLAGS) -o $@

pybe_unittest_SOURCES = pybe_unittest.cpp
pybe_unittest_LDADD = $(UNITTEST)/libgdb4_unittest.la

libexec_PROGRAMS= dbgsrv $(CRAY_PROGS) dbgsrv_trampoline pybe_unittest
dbgsrv_SOURCES	= dbgsrv.cpp dbgfn.cpp dbgsrv_proto.cpp dbgsrv_cmds.cpp \
				  $(BACKEND) $(MRN_SRV) $(ZMQNET_SRV)
dbgsrv_LDADD	= $(UNITTEST)/libgdb4_unittest.la $(GRAM)/libgram.a $(DBGCOMM)/libdbg.a \
				  $(USEFUL)/libuseful.a $(MI_LIBS) $(MRN_LIBS) $(ZMQNET_LIBS) \
				  $(BITSET)/libbitset.la $(CRAYTOOL_LIBS) \
				  -lm -lstdc++ -lstdc++fs -lpthread
dbgsrv_LDFLAGS	= $(LDFLAGS) -Wl,-E -Wl,-z,origin -Wl,-rpath,\$$ORIGIN/../lib \
				  -Wl,-rpath,\$$ORIGIN -Wl,-rpath,/opt/cray/pe/lib64 \
				  -Wl,-rpath,/opt/cray/lib64 -Wl,--enable-new-dtags
