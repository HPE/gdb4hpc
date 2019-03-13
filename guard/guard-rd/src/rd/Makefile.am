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

AM_CXXFLAGS = -I$(DF) -I$(GUARD) -I$(USEFUL) -I$(GRAM) -I$(DBGCOMM) -I$(DBGCLNT) -I$(DBGSRV) -I$(CLIENT) -I$(SDM)

noinst_LIBRARIES = librd.a
librd_a_SOURCES = ainfo.cpp rd_cmd.cpp rd_help_dict.cpp cmd_graph.cpp disp.cpp \
		  hdf.cpp hdf5.cpp cmd_blockmap.cpp
librd_a_LIBS = $(libhdf) $(libhdf5)
