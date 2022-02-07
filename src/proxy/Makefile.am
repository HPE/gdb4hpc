USEFUL	= @gdb4hpc_srcdir@/useful
GRAM	= @gdb4hpc_srcdir@/gram
DBGCLNT = @gdb4hpc_srcdir@/dbgclnt
DBGCOMM	= @gdb4hpc_srcdir@/dbgcomm
MRNET_ROOT	= @mrn_root@
BITSET  = @gdb4hpc_srcdir@/bitset
ZMQNET_ROOT  = @zmqnet_root@

AM_CXXFLAGS = -DBINDIR=\"$(bindir)\" -I$(USEFUL) -I$(GRAM) -I$(DBGCLNT) -I$(DBGCOMM) -I$(BITSET)
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
GUARD_RD_LIBS   = @gdb4hpc_srcdir@/graph/libdf.a

# Check for MRNet libraries
if MRNET
AM_CXXFLAGS  += -I$(MRNET_ROOT)
MRN_LIBS     = -L$(MRNET_ROOT)/lib -lmrnet 
endif

# Check for ZMQNet libraries
if ZMQNET
AM_CXXFLAGS	 += -I$(ZMQNET_ROOT)/include
ZMQN_LIBS 	 = -L$(ZMQNET_ROOT)/lib -lzmqnet -lkj -lcapnp
endif

MI_LIBS         = @mi_libs@
SSH2_LIBS       = @ssh2_libs@

######################
# Build proxy daemon #
######################

libexec_PROGRAMS	= guard_proxy
guard_proxy_SOURCES	= proxy_daemon.cpp dbg_client_proxy.cpp
guard_proxy_LDFLAGS   	= $(LDFLAGS) -Wl,-E -Wl,-z,origin -Wl,-rpath,\$$ORIGIN/../lib -Wl,-rpath,\$$ORIGIN -Wl,-rpath,/opt/cray/pe/lib64 -Wl,-rpath,/opt/cray/lib64 -Wl,--enable-new-dtags
guard_proxy_LDADD	= $(DBGCLNT)/libdbgproto.a $(DBGCOMM)/libdbg.a $(USEFUL)/libuseful.a \
			  $(CRAYTOOL_LIBS) $(MRN_LIBS) $(ZMQN_LIBS) $(SSH2_LIBS) $(BITSET)/libbitset.la -lpthread -laif -lm -lstdc++ -lstdc++fs
