INCLUDE_MRN = @mrn_root@/include
INCLUDE_BITSET = @gdb4hpc_srcdir@/bitset
INCLUDE_DMR = @gdb4hpc_srcdir@/filters
LIB_BITSET = @gdb4hpc_srcdir@/bitset
LIB_DMR = @gdb4hpc_srcdir@/filters

AM_CPPFLAGS = -I$(INCLUDE_MRN) -I$(INCLUDE_BITSET) -I$(INCLUDE_DMR)
AM_LIBTOOLFLAGS = --quiet

lib_LTLIBRARIES		= libdbgagg.la
libdbgagg_la_SOURCES	= Aggregation.cpp
libdbgagg_la_CXXFLAGS	= $(CXXFLAGS) -fPIC
libdbgagg_la_LIBADD	= $(LIB_BITSET)/libbitset.la $(LIB_DMR)/libdmr.la
libdbgagg_la_LDFLAGS	= $(LDFLAGS) -shared -avoid-version -Wl,--enable-new-dtags
