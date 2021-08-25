CLIENT	= @guard_srcdir@/src/client
USEFUL	= @guard_srcdir@/src/useful
GRAM	= @guard_srcdir@/src/gram
DBGCOMM	= @guard_srcdir@/src/dbgcomm
MRNET_ROOT	= @mrn_root@
BITSET  = @guard_srcdir@/bitset
ZMQNET_ROOT  = @zmqnet_root@

AM_CXXFLAGS = -I$(USEFUL) -I$(GRAM) -I$(DBGCOMM)
AM_LIBTOOLFLAGS = --quiet

######################
# Determine protocol #
######################

# Check for MRNet
if MRNET
AM_CXXFLAGS	+= -I$(MRNET_ROOT) -I$(BITSET)
MRN_PROTO	= dbgclnt_mrn.cpp dbg_mrn_network.cpp mrn_topgen.cpp
endif

if ZMQNET
AM_CXXFLAGS += -I$(ZMQNET_ROOT)/include -I$(BITSET)
ZMQNET_PROTO = dbgclnt_zmq.cpp dbg_zmq_network.cpp
endif

############################
# Determine debug frontend #
############################

# Check for gdb-mi
if GDBMI_BACKEND
FRONTEND	= dbgclnt_gdbmi.cpp
endif

if CRAY
AM_CXXFLAGS += $(CRAY_CTI_FE_CFLAGS)
endif

##############################
# Build debug client library #
##############################

noinst_LIBRARIES	= libdbgclnt.a libdbgproto.a
libdbgclnt_a_SOURCES	= dbg_client.cpp dbgclnt_debugfe.cpp\
						  $(FRONTEND)
libdbgproto_a_SOURCES	= dbgclnt_proto.cpp dbgclnt_proto_ctiship.cpp dbgclnt_network.cpp proto_message.cpp \
						  dbginvoke.cpp dbgclnt_proxy.cpp dbgproxy.cpp dbgoutp.cpp \
						  $(MRN_PROTO) $(ZMQNET_PROTO)

if CRAY
# Cray ALPS does not need guard_starter
else
AM_CXXFLAGS += -DBINDIR=\"$(bindir)\"
bin_PROGRAMS 	= guard_starter
guard_starter_SOURCES	= guard_starter.cpp
guard_starter_LDADD	= $(USEFUL)/libuseful.a $(GRAM)/libgram.a $(DBGCOMM)/libdbg.a -lpthread
guard_starter_LDFLAGS	= $(LDFLAGS)
endif
