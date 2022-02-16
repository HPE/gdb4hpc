USEFUL	= @gdb4hpc_srcdir@/useful
GRAM	= @gdb4hpc_srcdir@/gram
DBGCOMM	= @gdb4hpc_srcdir@/dbgcomm
DBGSRV	= @gdb4hpc_srcdir@/dbgsrv
DBGCLNT	= @gdb4hpc_srcdir@/dbgclnt
CLIENT	= @gdb4hpc_srcdir@/client
BITSET	= @gdb4hpc_srcdir@/bitset

AM_CXXFLAGS = -I@gdb4hpc_srcdir@ -I$(USEFUL) -I$(GRAM) -I$(DBGCOMM) -I$(DBGCLNT) -I$(DBGSRV) -I$(CLIENT) -I$(BITSET)

noinst_LIBRARIES = libdf.a
libdf_a_SOURCES = gvar.cpp build.cpp graph.cpp token.cpp node.cpp nodedef.cpp nodes.cpp df.cpp
