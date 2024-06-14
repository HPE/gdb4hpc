USEFUL	= @gdb4hpc_srcdir@/useful
GRAM	= @gdb4hpc_srcdir@/gram
MRNET_ROOT	= @mrn_root@
SDM	    = @gdb4hpc_srcdir@/sdm
UNITTEST = @gdb4hpc_srcdir@/unittest

AM_CXXFLAGS = -I$(USEFUL) -I$(GRAM) -I$(UNITTEST)
AM_LIBTOOLFLAGS = --quiet

#################################
# Check for optional components #
#################################

# Check for MRNet
if MRNET
AM_CXXFLAGS += -I$(MRNET_ROOT) -I$(SDM)
endif

if ZMQNET
AM_CXXFLAGS += $(ZMQNET_CFLAGS)

# generate dbg_zmq.capnp.c++ and dbg_zmq.capnp.h
# the capnp binary is built and installed when building zmqnet
dbg_zmq.capnp.c++: dbg_zmq.capnp
	PATH=$(ZMQNET_DEVEL_SUPPORT_DIR)/bin:$(PATH) capnp compile -oc++ dbg_zmq.capnp

ZMQNET_FILES = dbg_zmq.capnp.c++

clean-local:
	rm -f dbg_zmq.capnp.*
endif

##############################
# Build debug common library #
##############################

noinst_LIBRARIES = libdbg.a
libdbg_a_SOURCES = dbgbp.cpp dbgevent.cpp dbgformat.cpp AIFStream.cpp ais.cpp \
				   blockmap.cpp $(ZMQNET_FILES) socstream.cpp
libdbg_a_CXXFLAGS = $(AM_CXXFLAGS) $(CAPNP_CFLAGS)
