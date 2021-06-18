USEFUL	= @guard_srcdir@/src/useful
GRAM	= @guard_srcdir@/src/gram
MRNET	= @mrn_root@
SDM	    = @guard_srcdir@/sdm
ZMQNET  = @zmqnet_root@

AM_CXXFLAGS = -I$(USEFUL) -I$(GRAM)
AM_LIBTOOLFLAGS = --quiet

#################################
# Check for optional components #
#################################

# Check for MRNet
if MRN
AM_CXXFLAGS       += -I$(MRNET) -I$(SDM)
endif

if ZMQN
AM_CXXFLAGS += -I$(ZMQNET)/include
# temporary hack: use the capnp libraries and include files in the zmqnet submodule
# the capnp binary is built and installed when building zmqnet

# generate dbg_zmq.capnp.c++ and dbg_zmq.capnp.h
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
libdbg_a_SOURCES = dbgbp.cpp dbgevent.cpp dbgformat.cpp AIFStream.cpp $(ZMQNET_FILES)
