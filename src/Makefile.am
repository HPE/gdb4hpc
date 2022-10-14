ACLOCAL_AMFLAGS = -I m4
AM_LIBTOOLFLAGS = --quiet

if MRNET
MRNET_DIR = filters/mrnet
endif

if ZMQNET
ZMQNET_DIR = filters/zmqnet
endif

SUBDIRS = unittest bitset filters script $(MRNET_DIR) \
		useful gram dbgcomm $(ZMQNET_DIR) dbgclnt \
		dbgsrv client

install-exec-hook: 
	find $(DESTDIR)$(libdir) -type f -name \*.la \! -name libLrleBitset.la -delete
