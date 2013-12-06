USEFUL	= @guard_srcdir@/src/useful
GRAM	= @guard_srcdir@/src/gram
DBGCLNT = @guard_srcdir@/src/dbgclnt
DBGCOMM	= @guard_srcdir@/src/dbgcomm
MRNET	= @guard_srcdir@/mrnet
SDM	= @guard_srcdir@/sdm

AM_CFLAGS = -DBINDIR=\"$(bindir)\" -I$(USEFUL) -I$(GRAM) -I$(DBGCLNT) -I$(DBGCOMM)

#################################
# Check for optional components #
#################################

# Check for Cray libraries
if CRAY
CRAYTOOL_LIBS   = @craytool_fe_libs@
endif

# Check for relative debugging libraries
if WITH_RD
GUARD_RD_LIBS   = @rd_libs@
endif

# Check for MRNet libraries
if MRN
AM_CFLAGS       += -I$(MRNET) -I$(SDM)
MRN_LIBS        = -L$(MRNET) -L$(SDM) -lmrnet_c -lmrnet -lbitset -lxplat
endif

# Check for gdb-mi libraries
if GDBMI_BACKEND
MI_LIBS         = @mi_libs@
endif

SSH2_LIBS       = @ssh2_libs@

######################
# Build proxy daemon #
######################

bin_PROGRAMS		= guard_proxy
guard_proxy_SOURCES	= proxy_daemon.c dbg_client_proxy.c
guard_proxy_LDFLAGS   	= $(LDFLAGS) -Wl,-E -Wl,-z,origin -Wl,-rpath,'$$ORIGIN'/../lib
guard_proxy_LDADD	= $(DBGCLNT)/libdbgproto.a $(DBGCOMM)/libdbg.a $(USEFUL)/libuseful.a \
			  $(CRAYTOOL_LIBS) $(MRN_LIBS) $(SSH2_LIBS) -lpthread -laif
