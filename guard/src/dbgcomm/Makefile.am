USEFUL	= @guard_srcdir@/src/useful
GRAM	= @guard_srcdir@/src/gram
MRNET	= @guard_srcdir@/mrnet
SDM	= @guard_srcdir@/sdm

AM_CFLAGS = -I$(USEFUL) -I$(GRAM)

#################################
# Check for optional components #
#################################

# Check for RPC proto
if RPC
BUILT_SOURCES	= dbgproto_rpc.h dbgproto_xdr.c
CLEANFILES	= $(BUILT_SOURCES)
RPC_COMM	= dbgproto_xdr.c dbg_rpc.c

dbgproto_rpc.h: dbgproto_rpc.x
	        $(RPCGEN) -h $(RPCGENFLAGS) -o dbgproto_rpc.h dbgproto_rpc.x

dbgproto_xdr.c: dbgproto_rpc.x
	        $(RPCGEN) -c $(RPCGENFLAGS) -o dbgproto_xdr.c dbgproto_rpc.x
endif

# Check for Socket proto
if SOCK
SOCK_COMM       = dbg_soc.c
endif

# Check for MRNet
if MRN
AM_CFLAGS       += -I$(MRNET) -I$(SDM)
endif

##############################
# Build debug common library #
##############################

noinst_LIBRARIES = libdbg.a
libdbg_a_SOURCES = dbgbp.c dbgcmd.c dbgevent.c dbgformat.c \
		   $(RPC_COMM) $(SOC_COMM)

