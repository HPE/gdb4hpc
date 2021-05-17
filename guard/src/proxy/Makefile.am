USEFUL	= @guard_srcdir@/src/useful
GRAM	= @guard_srcdir@/src/gram
DBGCLNT = @guard_srcdir@/src/dbgclnt
DBGCOMM	= @guard_srcdir@/src/dbgcomm
MRNET	= @mrn_root@
SDM	= @guard_srcdir@/sdm

AM_CXXFLAGS = -DBINDIR=\"$(bindir)\" -I$(USEFUL) -I$(GRAM) -I$(DBGCLNT) -I$(DBGCOMM)
AM_LIBTOOLFLAGS = --quiet

#################################
# Check for optional components #
#################################

# Check for Cray libraries
if CRAY
CRAYTOOL_LIBS	= $(CRAY_CTI_FE_LIBS)
AM_CXXFLAGS += $(CRAY_CTI_FE_CFLAGS)
endif

# Check for relative debugging libraries
if WITH_RD
GUARD_RD_LIBS   = @rd_libs@
endif

# Check for MRNet libraries
if MRN
AM_CXXFLAGS       += -I$(MRNET) -I$(SDM)
MRN_LIBS        = -L$(MRNET)/lib -L$(SDM) -lmrnet -lbitset
endif

# Check for gdb-mi libraries
if GDBMI_BACKEND
MI_LIBS         = @mi_libs@
endif

SSH2_LIBS       = @ssh2_libs@

######################
# Build proxy daemon #
######################

libexec_PROGRAMS	= guard_proxy
guard_proxy_SOURCES	= proxy_daemon.cpp dbg_client_proxy.cpp
guard_proxy_LDFLAGS   	= $(LDFLAGS) -Wl,-E -Wl,-z,origin -Wl,-rpath,\$$ORIGIN/../lib -Wl,-rpath,\$$ORIGIN -Wl,-rpath,/opt/cray/pe/lib64 -Wl,-rpath,/opt/cray/lib64 -Wl,--enable-new-dtags
guard_proxy_LDADD	= $(DBGCLNT)/libdbgproto.a $(DBGCOMM)/libdbg.a $(USEFUL)/libuseful.a \
			  $(CRAYTOOL_LIBS) $(MRN_LIBS) $(SSH2_LIBS) -lpthread -laif -lm -lstdc++ -lstdc++fs
