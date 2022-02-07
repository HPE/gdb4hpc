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

AM_CXXFLAGS = -I$(GUARD)

if MRNET
MRNET_BITSETS = sparse_bitset.cpp bitset_convert.cpp
endif

if ZMQNET
# when zmqnet is built, the lrle_bitset.h header is installed in the gdb4hpc include directory.
LRLE = @zmqnet_root@
ZMQNET_BITSETS = lrunle_bitset.cpp
ZMQNET_LDFLAG = -Wl,-rpath,$(LRLE)/lib
ZMQNET_LIB = -L$(LRLE)/lib -lLrleBitset
AM_CXXFLAGS += -I$(LRLE)/include
endif

lib_LTLIBRARIES = libbitset.la

AM_LIBTOOLFLAGS = --quiet

libbitset_la_SOURCES 	= bitset.cpp dense_bitset.cpp $(MRNET_BITSETS) $(ZMQNET_BITSETS)
libbitset_la_LDFLAGS	= $(LDFLAGS) $(ZMQNET_LDFLAG) -shared -avoid-version 
libbitset_la_LIBADD     = $(ZMQNET_LIB)
