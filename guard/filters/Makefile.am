INCLUDE_SDM = @guard_srcdir@/sdm

AM_CPPFLAGS = -I$(INCLUDE_SDM)
AM_LIBTOOLFLAGS = --quiet

noinst_LTLIBRARIES	= libdmr.la
libdmr_la_SOURCES	= dmr.cpp
libdmr_la_CXXFLAGS	= $(CXXFLAGS) -fPIC
libdmr_la_LDFLAGS	= $(LDFLAGS) -static

