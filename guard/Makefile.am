GUARD_RD_DIR = @rd_dir@
GUARD_DF_DIR = @df_dir@

ACLOCAL_AMFLAGS = -I m4
AM_LIBTOOLFLAGS = --quiet

if MRN
MRNET_DIR = sdm filters filters/mrnet
endif

if ZMQN
ZMQNET_DIR = filters/zmqnet
endif

SUBDIRS =	$(GUARD_RD_DIR) $(GUARD_DF_DIR) $(MRNET_DIR) \
		src/useful src/gram src/dbgcomm $(ZMQNET_DIR) src/dbgclnt \
		src/dbgsrv src/client src/proxy scripts
			
install-exec-hook: 
	find $(DESTDIR)$(libdir) -type f -name \*.la -delete
