USEFUL	= @gdb4hpc_srcdir@/useful
DBGCOMM	= @gdb4hpc_srcdir@/dbgcomm
DBGCLNT = @gdb4hpc_srcdir@/dbgclnt
CLIENT	= @gdb4hpc_srcdir@/client
BITSET	= @gdb4hpc_srcdir@/bitset
GRAPH   = @gdb4hpc_srcdir@/graph

AM_CFLAGS = -I$(USEFUL) -I$(DBGCOMM) -I$(DBGCLNT) -I$(CLIENT) -I$(BITSET) -I$(GRAPH)
AM_CXXFLAGS = $(AM_CFLAGS)
AM_LIBTOOLFLAGS = --quiet

# check for lex/yacc related files
BUILT_SOURCES	= expr.y expr.tab.cpp expr.tab.h
CLEANFILES	= $(BUILT_SOURCES) tokens

expr.y : expr.y.in cmdtoken.h
	$(SED) -n "s/^#define[[:space:]]*\(CT_[[:graph:]]*\)[[:space:]]*\([[:digit:]]*\).*$$/s@%\1%@\2@/p" < cmdtoken.h > tokens
	$(SED) -f tokens < expr.y.in > expr.y

expr.tab.cpp expr.tab.h: expr.y
	$(YACC) -b expr -p expr_ -d -v expr.y -o expr.tab.cpp

noinst_LIBRARIES	= libgram.a
libgram_a_SOURCES	= assign.cpp blockmap.cpp blockmap_util.cpp cmdtoken.cpp expr.cpp expr.tab.cpp \
			  file.cpp func.cpp line.cpp perm.cpp procset.cpp \
			  slice.cpp val.cpp var.cpp
