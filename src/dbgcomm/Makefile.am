USEFUL	= @gdb4hpc_srcdir@/useful
GRAM	= @gdb4hpc_srcdir@/gram
MRNET_ROOT	= @mrn_root@
SDM	    = @gdb4hpc_srcdir@/sdm
ZMQNET_ROOT  = @zmqnet_root@
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
AM_CXXFLAGS += -I$(ZMQNET_ROOT)/include

# generate dbg_zmq.capnp.c++ and dbg_zmq.capnp.h
# the capnp binary is built and installed when building zmqnet
dbg_zmq.capnp.c++: dbg_zmq.capnp
	PATH=$(prefix)/bin:$PATH capnp compile -oc++ dbg_zmq.capnp

ZMQNET_FILES = dbg_zmq.capnp.c++

clean-local:
	rm -f dbg_zmq.capnp.*
endif

##############################
# Build debug common library #
##############################

noinst_LIBRARIES = libdbg.a
libdbg_a_SOURCES = dbgbp.cpp dbgevent.cpp dbgformat.cpp AIFStream.cpp ais.cpp \
				   $(ZMQNET_FILES) socstream.cpp
