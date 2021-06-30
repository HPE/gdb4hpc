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

if MRN
MRN_BITSETS = sparse_bitset.cpp bitset_convert.cpp
endif

if ZMQN
ZMQN_BITSETS = lrunle_bitset.cpp
ZMQN_BITSETS_LIBS = -lLrleBitset
# TODO: include this with gdb4hpc properly, don't steal the header from zmqnet.
# luckily for us, when zmqnet is built, the header is installed in the gdb4hpc
# include directory.
LRLE = @zmqnet_root@
endif

lib_LTLIBRARIES = libbitset.la

AM_LIBTOOLFLAGS = --quiet

libbitset_la_SOURCES 	= bitset.cpp dense_bitset.cpp $(MRN_BITSETS) $(ZMQN_BITSETS)
libbitset_la_CFLAGS	    = $(CFLAGS) -fPIC
libbitset_la_LDFLAGS	= $(LDFLAGS) -shared -avoid-version $(ZMQN_BITSETS_LIBS)
