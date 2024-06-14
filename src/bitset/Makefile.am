#
# Makefile.am for the bitset components of gdb4hpc
#
# Copyright 2012,2021 Hewlett Packard Enterprise Development LP..
#
# Unpublished Proprietary Information.
# This unpublished work is protected to trade secret, copyright and other laws.
# Except as permitted by contract or express written permission of Hewlett Packard Enterprise Development LP.,
# no part of this work or its content may be used, reproduced or disclosed
# in any form.

GUARD=@gdb4hpc_srcdir@
UNITTEST=@gdb4hpc_srcdir@/unittest

AM_CXXFLAGS = -I$(GUARD) -I$(UNITTEST)

lib_LTLIBRARIES = libbitset.la

AM_LIBTOOLFLAGS = --quiet

libbitset_la_SOURCES 	= bitset.cpp dense_bitset.cpp
libbitset_la_LDFLAGS	= $(LDFLAGS) -shared -avoid-version
libbitset_la_LIBADD     = $(UNITTEST)/libgdb4_unittest.la

if MRNET
libbitset_la_SOURCES += sparse_bitset.cpp bitset_convert.cpp
endif

if ZMQNET
libbitset_la_SOURCES += lrunle_bitset.cpp
libbitset_la_CXXFLAGS = $(AM_CXXFLAGS) $(LRLEBITSET_CFLAGS)
libbitset_la_LIBADD += $(LRLEBITSET_LIBS)
endif

