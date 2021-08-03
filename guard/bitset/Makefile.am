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

GUARD=@guard_srcdir@

AM_CXXFLAGS = -I$(GUARD)

if MRN
MRN_BITSETS = sparse_bitset.cpp bitset_convert.cpp
endif

if ZMQN
# when zmqnet is built, the lrle_bitset.h header is installed in the gdb4hpc include directory.
LRLE = @zmqnet_root@
ZMQN_BITSETS = lrunle_bitset.cpp
ZMQN_LDFLAG = -Wl,-rpath,$(LRLE)/lib
ZMQN_LIB = -L$(LRLE)/lib -lLrleBitset
AM_CXXFLAGS += -I$(LRLE)/include
endif

lib_LTLIBRARIES = libbitset.la

AM_LIBTOOLFLAGS = --quiet

libbitset_la_SOURCES 	= bitset.cpp dense_bitset.cpp $(MRN_BITSETS) $(ZMQN_BITSETS)
libbitset_la_LDFLAGS	= $(LDFLAGS) $(ZMQN_LDFLAG) -shared -avoid-version 
libbitset_la_LIBADD     = $(ZMQN_LIB)
