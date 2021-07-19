ZMQNET  = @zmqnet_root@
DBGCOMM	= @guard_srcdir@/src/dbgcomm
BITSET = @guard_srcdir@/bitset

AM_CPPFLAGS = -I$(ZMQNET)/include -I$(DBGCOMM) -I$(BITSET)
AM_LIBTOOLFLAGS = --quiet

lib_LTLIBRARIES		     = libzmqnetagg.la
libzmqnetagg_la_SOURCES	 = zmqnetagg.cpp
libzmqnetagg_la_CXXFLAGS = $(CXXFLAGS) -fPIC
libzmqnetagg_la_LIBADD   = $(BITSET)/libbitset.la
libzmqnetagg_la_LDFLAGS	 = $(LDFLAGS) -shared
