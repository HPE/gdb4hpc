CLIENT	= @gdb4hpc_srcdir@/client
USEFUL	= @gdb4hpc_srcdir@/useful
GRAM	= @gdb4hpc_srcdir@/gram
DBGCOMM	= @gdb4hpc_srcdir@/dbgcomm
SCRIPT  = @gdb4hpc_srcdir@/script
MRNET_ROOT	= @mrn_root@
BITSET  = @gdb4hpc_srcdir@/bitset
ZMQNET_ROOT  = @zmqnet_root@

AM_CXXFLAGS = -I$(USEFUL) -I$(GRAM) -I$(DBGCOMM) -I$(SCRIPT)
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

FRONTEND	= dbgclnt_gdbmi.cpp

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
						  dbgclnt_breakpoints.cpp dbginvoke.cpp dbgclnt_proxy.cpp dbgproxy.cpp dbgoutp.cpp \
						  $(MRN_PROTO) $(ZMQNET_PROTO)
