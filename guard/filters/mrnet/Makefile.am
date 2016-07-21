INCLUDE_MRN = @mrn_root@/include
INCLUDE_SDM = @guard_srcdir@/sdm
INCLUDE_DMR = @guard_srcdir@/filters
LIB_SDM = @guard_srcdir@/sdm
LIB_DMR = @guard_srcdir@/filters

AM_CPPFLAGS = -I$(INCLUDE_MRN) -I$(INCLUDE_SDM) -I$(INCLUDE_DMR)
AM_LIBTOOLFLAGS = --quiet

lib_LTLIBRARIES		= libdbgagg.la
libdbgagg_la_SOURCES	= Aggregation.cpp
libdbgagg_la_CXXFLAGS	= $(CXXFLAGS) -fPIC
libdbgagg_la_LIBADD	= $(LIB_SDM)/libbitset.la $(LIB_DMR)/libdmr.la
libdbgagg_la_LDFLAGS	= $(LDFLAGS) -shared -avoid-version --enable-new-dtags
