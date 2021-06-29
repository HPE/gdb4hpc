USEFUL	= @guard_srcdir@/src/useful
DBGCOMM	= @guard_srcdir@/src/dbgcomm
DBGCLNT = @guard_srcdir@/src/dbgclnt
CLIENT	= @guard_srcdir@/src/client
BITSET	= @guard_srcdir@/bitset

AM_CFLAGS = -I$(USEFUL) -I$(DBGCOMM) -I$(DBGCLNT) -I$(CLIENT) -I$(BITSET)
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
