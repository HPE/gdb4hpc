USEFUL	= @guard_srcdir@/src/useful
GRAM	= @guard_srcdir@/src/gram
MRNET	= @guard_srcdir@/mrnet
SDM	= @guard_srcdir@/sdm

AM_CXXFLAGS = -I$(USEFUL) -I$(GRAM)
AM_LIBTOOLFLAGS = --quiet

#################################
# Check for optional components #
#################################

# Check for Socket proto
if SOCK
SOCK_COMM       = dbg_soc.cpp
endif

# Check for MRNet
if MRN
AM_CXXFLAGS       += -I$(MRNET) -I$(SDM)
endif

##############################
# Build debug common library #
##############################

noinst_LIBRARIES = libdbg.a
libdbg_a_SOURCES = dbgbp.cpp dbgevent.cpp dbgformat.cpp AIFStream.cpp \
		   $(SOC_COMM)

