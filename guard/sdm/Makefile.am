#
# Makefile.am for the bitset components of ccdb
#
# Copyright 2012,2020 Hewlett Packard Enterprise Development LP..
#
# Unpublished Proprietary Information.
# This unpublished work is protected to trade secret, copyright and other laws.
# Except as permitted by contract or express written permission of Hewlett Packard Enterprise Development LP.,
# no part of this work or its content may be used, reproduced or disclosed
# in any form.

lib_LTLIBRARIES = libbitset.la

AM_LIBTOOLFLAGS = --quiet

libbitset_la_SOURCES 	= 	bitset.c compat.c conversion.c sparse_bitset.c \
				smart_bitset.c varstr.c
libbitset_la_CFLAGS	=	$(CFLAGS) -fPIC
libbitset_la_LDFLAGS	=	$(LDFLAGS) -shared -avoid-version
