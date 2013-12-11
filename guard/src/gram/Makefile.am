USEFUL	= @guard_srcdir@/src/useful
DBGCOMM	= @guard_srcdir@/src/dbgcomm
DBGCLNT = @guard_srcdir@/src/dbgclnt
CLIENT	= @guard_srcdir@/src/client
SDM	= @guard_srcdir@/sdm

AM_CFLAGS = -I$(USEFUL) -I$(DBGCOMM) -I$(DBGCLNT) -I$(CLIENT) -I$(SDM)
AM_LIBTOOLFLAGS = --quiet

# check for lex/yacc related files
BUILT_SOURCES	= expr.y expr.tab.c expr.tab.h
CLEANFILES	= $(BUILT_SOURCES) tokens

expr.y : expr.y.in cmdtoken.h
	$(SED) -n "s/^#define[[:space:]]*\(CT_[[:graph:]]*\)[[:space:]]*\([[:digit:]]*\).*$$/s@%\1%@\2@/p" < cmdtoken.h > tokens
	$(SED) -f tokens < expr.y.in > expr.y

expr.tab.c expr.tab.h: expr.y
	$(YACC) -b expr -p expr_ -d -v expr.y

noinst_LIBRARIES	= libgram.a
libgram_a_SOURCES	= assign.c blockmap.c blockmap_util.c cmdtoken.c expr.c expr.tab.c \
			  file.c func.c invoke.c line.c perm.c procset.c \
			  slice.c val.c var.c
