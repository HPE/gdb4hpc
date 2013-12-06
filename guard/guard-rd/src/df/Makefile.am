GUARD	= @guardrd_srcdir@/@GUARD_SRC@
USEFUL	= $(GUARD)/src/useful
GRAM	= $(GUARD)/src/gram
DBGCOMM	= $(GUARD)/src/dbgcomm
DBGSRV	= $(GUARD)/src/dbgsrv
DBGCLNT	= $(GUARD)/src/dbgclnt
CLIENT	= $(GUARD)/src/client
RD	= @guardrd_srcdir@/src/rd
SDM	= $(GUARD)/sdm

AM_CFLAGS = -I$(RD) -I$(GUARD) -I$(USEFUL) -I$(GRAM) -I$(DBGCOMM) -I$(DBGCLNT) -I$(DBGSRV) -I$(CLIENT) -I$(SDM)

noinst_LIBRARIES = libdf.a
libdf_a_SOURCES = gvar.c build.c graph.c token.c node.c nodedef.c nodes.c df.c
