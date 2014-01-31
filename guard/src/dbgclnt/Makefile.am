CLIENT	= @guard_srcdir@/src/client
USEFUL	= @guard_srcdir@/src/useful
GRAM	= @guard_srcdir@/src/gram
DBGCOMM	= @guard_srcdir@/src/dbgcomm
MRNET	= @guard_srcdir@/mrnet
SDM	= @guard_srcdir@/sdm

# FIXME: CLIENT shouldn't be used here, but it is a hack until
# BUG 804428 is fixed...
AM_CFLAGS = -I$(USEFUL) -I$(GRAM) -I$(DBGCOMM) -I$(CLIENT)
AM_LIBTOOLFLAGS = --quiet

######################
# Determine protocol #
######################

# Check for MRNet
if MRN
AM_CFLAGS	+= -I$(MRNET) -I$(SDM)
MRN_PROTO	= dbgclnt_mrn.c dbg_mrn_fe.c mrn_topgen.c
endif

# Check for Socket
if SOCK
SOCK_PROTO	= dbgclnt_soc.c
endif

############################
# Determine debug frontend #
############################

# Check for gdb-mi
if GDBMI_BACKEND
FRONTEND	= dbgclnt_gdbmi.c
endif

##############################
# Build debug client library #
##############################

noinst_LIBRARIES	= libdbgclnt.a libdbgproto.a
libdbgclnt_a_SOURCES	= dbg_client.c dbgclnt_debugfe.c \
			  dbg_set_sync.c \
			  $(FRONTEND)
libdbgproto_a_SOURCES	= dbgclnt_proto.c proto_message.c \
			  dbgclnt_proxy.c dbgproxy.c dbgoutp.c \
			  $(MRN_PROTO) $(SOCK_PROTO)

if CRAY
# Cray ALPS does not need guard_starter
else
AM_CFLAGS += -DBINDIR=\"$(bindir)\"
bin_PROGRAMS 	= guard_starter
guard_starter_SOURCES	= guard_starter.c
guard_starter_LDADD	= $(USEFUL)/libuseful.a $(GRAM)/libgram.a $(DBGCOMM)/libdbg.a
guard_starter_LDFLAGS	= $(LDFLAGS)
endif
