GUARD_RD_DIR = @rd_dir@
GUARD_DF_DIR = @df_dir@

ACLOCAL_AMFLAGS = -I m4

if MRN
MRNET_DIR = sdm mrnet filters filters/mrnet
endif

SUBDIRS =	$(GUARD_RD_DIR) $(GUARD_DF_DIR) $(MRNET_DIR) \
		src/useful src/gram src/dbgcomm src/dbgclnt \
		src/dbgsrv src/client src/proxy scripts
			
install-exec-hook: 
	find $(DESTDIR)$(libdir) -type f -name \*.la -delete
