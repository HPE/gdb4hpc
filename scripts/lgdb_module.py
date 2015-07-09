#!/usr/bin/env python
content='''#%Module######################################################################
 #
 #       (C) COPYRIGHT CRAY INC.
 #       UNPUBLISHED PROPRIETARY INFORMATION.
 #       ALL RIGHTS RESERVED.
 #
 
 set CRAY_LGDB_LEVEL   <version> 
 set CRAY_LGDB_CURPATH  [install_dir]/lgdb/$CRAY_LGDB_LEVEL
 
 setenv CRAY_LGDB_VERSION  $CRAY_LGDB_LEVEL
 setenv CRAY_LGDB_DIR $CRAY_LGDB_CURPATH
 setenv CRAY_LGDB_LIB_DIR $CRAY_LGDB_CURPATH/lib
 
 append-path	PE_PRODUCT_LIST   CRAY_LGDB
 prepend-path	PATH $CRAY_LGDB_CURPATH/bin
 prepend-path	MANPATH $CRAY_LGDB_CURPATH/man

 if { [ file exists ${CRAY_LGDB_CURPATH}/release_info ] } {
       set REL_INFO [ exec cat ${CRAY_LGDB_CURPATH}/release_info ]
    } else {
       set REL_INFO ""
    }


 proc ModulesHelp { } {
    global CRAY_LGDB_CURPATH
    global CRAY_LGDB_LEVEL
    global REL_INFO
    puts stderr "The modulefile defines the system paths and"
    puts stderr "variables for the product cray-lgdb."
    puts stderr "$REL_INFO"
    puts stderr "============================================="
    puts stderr "To re-display cray-lgdb/$CRAY_LGDB_LEVEL release information,"
    puts stderr "type:    less $CRAY_LGDB_CURPATH/release_info"
    puts stderr "==============================================\n"
    puts stderr ""
 }


module-whatis   "Loads the Cray Line Mode Parallel Debugger."

'''
