#
# Makefile.am for the bitset components of ccdb
#
# ©2012 Cray Inc.  All Rights Reserved.
#
# Unpublished Proprietary Information.
# This unpublished work is protected to trade secret, copyright and other laws.
# Except as permitted by contract or express written permission of Cray Inc.,
# no part of this work or its content may be used, reproduced or disclosed
# in any form.

lib_LTLIBRARIES = libbitset.la

AM_LIBTOOLFLAGS = --quiet

libbitset_la_SOURCES 	= 	bitset.c compat.c conversion.c sparse_bitset.c \
							smart_bitset.c varstr.c
libbitset_la_CFLAGS		=	-fPIC
libbitset_la_LDFLAGS	=	-shared -avoid-version
