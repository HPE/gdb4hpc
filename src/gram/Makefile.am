USEFUL	= @gdb4hpc_srcdir@/useful
DBGCOMM	= @gdb4hpc_srcdir@/dbgcomm
DBGCLNT = @gdb4hpc_srcdir@/dbgclnt
CLIENT	= @gdb4hpc_srcdir@/client
BITSET	= @gdb4hpc_srcdir@/bitset
SCRIPT  = @gdb4hpc_srcdir@/script
UNITTEST = @gdb4hpc_srcdir@/unittest

AM_CFLAGS = -I$(USEFUL) -I$(DBGCOMM) -I$(DBGCLNT) -I$(CLIENT) -I$(BITSET) -I$(SCRIPT) -I$(UNITTEST)
AM_CXXFLAGS = $(AM_CFLAGS)
AM_LIBTOOLFLAGS = --quiet

# check for lex/yacc related files
BUILT_SOURCES	= expr.tab.cpp expr.tab.h
CLEANFILES	= $(BUILT_SOURCES) tokens

expr.tab.cpp expr.tab.h: expr.y
	$(YACC) -b expr -p expr_ -d -v expr.y -o expr.tab.cpp

noinst_LIBRARIES	= libgram.a
libgram_a_SOURCES	= gram.cpp val.cpp eval.cpp expr.tab.cpp \
                          blockmap_client.cpp  \
			  procset.cpp slice.cpp var.cpp 
