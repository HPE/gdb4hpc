GUARD	= @guardrd_srcdir@/@GUARD_SRC@
USEFUL	= $(GUARD)/src/useful
GRAM	= $(GUARD)/src/gram
DBGCOMM	= $(GUARD)/src/dbgcomm
DBGSRV	= $(GUARD)/src/dbgsrv
DBGCLNT	= $(GUARD)/src/dbgclnt
CLIENT	= $(GUARD)/src/client
RD	= @guardrd_srcdir@/src/rd
BITSET	= $(GUARD)/bitset

AM_CXXFLAGS = -I$(RD) -I$(GUARD) -I$(USEFUL) -I$(GRAM) -I$(DBGCOMM) -I$(DBGCLNT) -I$(DBGSRV) -I$(CLIENT) -I$(BITSET)

noinst_LIBRARIES = libdf.a
libdf_a_SOURCES = gvar.cpp build.cpp graph.cpp token.cpp node.cpp nodedef.cpp nodes.cpp df.cpp
