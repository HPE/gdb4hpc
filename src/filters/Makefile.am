BITSET = @gdb4hpc_srcdir@/bitset

AM_LIBTOOLFLAGS = --quiet

noinst_LTLIBRARIES	= libdmr.la
libdmr_la_SOURCES	= dmr.cpp
libdmr_la_CXXFLAGS	= $(CXXFLAGS) -fPIC -I$(BITSET)
libdmr_la_LDFLAGS	= $(LDFLAGS) -static
