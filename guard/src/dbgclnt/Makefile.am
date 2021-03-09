CLIENT	= @guard_srcdir@/src/client
USEFUL	= @guard_srcdir@/src/useful
GRAM	= @guard_srcdir@/src/gram
DBGCOMM	= @guard_srcdir@/src/dbgcomm
MRNET	= @guard_srcdir@/mrnet
SDM	= @guard_srcdir@/sdm

AM_CXXFLAGS = -I$(USEFUL) -I$(GRAM) -I$(DBGCOMM)
AM_LIBTOOLFLAGS = --quiet

######################
# Determine protocol #
######################

# Check for MRNet
if MRN
AM_CXXFLAGS	+= -I$(MRNET) -I$(SDM)
MRN_PROTO	= dbgclnt_mrn.cpp dbg_mrn_fe.cpp mrn_topgen.cpp
endif

# Check for Socket
if SOCK
SOCK_PROTO	= dbgclnt_soc.cpp
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
libdbgproto_a_SOURCES	= dbgclnt_proto.cpp proto_message.cpp dbginvoke.cpp \
						  dbgclnt_proxy.cpp dbgproxy.cpp dbgoutp.cpp \
						  $(MRN_PROTO) $(SOCK_PROTO)

if CRAY
# Cray ALPS does not need guard_starter
else
AM_CXXFLAGS += -DBINDIR=\"$(bindir)\"
bin_PROGRAMS 	= guard_starter
guard_starter_SOURCES	= guard_starter.cpp
guard_starter_LDADD	= $(USEFUL)/libuseful.a $(GRAM)/libgram.a $(DBGCOMM)/libdbg.a -lpthread
guard_starter_LDFLAGS	= $(LDFLAGS)
endif
