ZMQNET_ROOT = @zmqnet_root@
DBGCOMM	= @gdb4hpc_srcdir@/dbgcomm
BITSET = @gdb4hpc_srcdir@/bitset

AM_CPPFLAGS = -I$(ZMQNET_ROOT)/include -I$(DBGCOMM) -I$(BITSET)
AM_LIBTOOLFLAGS = --quiet

lib_LTLIBRARIES		     = libzmqnetagg.la
libzmqnetagg_la_SOURCES	 = zmqnetagg.cpp
libzmqnetagg_la_CXXFLAGS = $(CXXFLAGS) -fPIC
libzmqnetagg_la_LIBADD   = $(BITSET)/libbitset.la
libzmqnetagg_la_LDFLAGS	 = $(LDFLAGS) -shared
