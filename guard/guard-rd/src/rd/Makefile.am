GUARD	= @guardrd_srcdir@/@GUARD_SRC@
USEFUL	= $(GUARD)/src/useful
GRAM	= $(GUARD)/src/gram
DBGCOMM	= $(GUARD)/src/dbgcomm
DBGCLNT = $(GUARD)/src/dbgclnt
DBGSRV	= $(GUARD)/src/dbgsrv
CLIENT	= $(GUARD)/src/client
DF	= @guardrd_srcdir@/src/df
SDM	= $(GUARD)/sdm
libhdf	= @HDF_LIB@
libhdf5	= @HDF5_LIB@

AM_CFLAGS = -I$(DF) -I$(GUARD) -I$(USEFUL) -I$(GRAM) -I$(DBGCOMM) -I$(DBGCLNT) -I$(DBGSRV) -I$(CLIENT) -I$(SDM)

noinst_LIBRARIES = librd.a
librd_a_SOURCES = ainfo.c rd_cmd.c rd_help_dict.c cmd_graph.c disp.c \
		  hdf.c hdf5.c cmd_blockmap.c
librd_a_LIBS = $(libhdf) $(libhdf5)
