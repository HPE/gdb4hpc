#!/usr/bin/env python
content='''#%Module######################################################################
 #
 #       (C) COPYRIGHT CRAY INC.
 #       UNPUBLISHED PROPRIETARY INFORMATION.
 #       ALL RIGHTS RESERVED.
 #
 
 set GDB4HPC_LEVEL   <version> 
 set GDB4HPC_CURPATH  [install_dir]/gdb4hpc/$GDB4HPC_LEVEL
 
 setenv GDB4HPC_VERSION  $GDB4HPC_LEVEL
 setenv GDB4HPC_DIR $GDB4HPC_CURPATH
 setenv GDB4HPC_LIB_DIR $GDB4HPC_CURPATH/lib
 
 append-path	PE_PRODUCT_LIST   GDB4HPC
 prepend-path	PATH $GDB4HPC_CURPATH/bin
 prepend-path	MANPATH $GDB4HPC_CURPATH/man

 if { [ file exists ${GDB4HPC_CURPATH}/release_info ] } {
       set REL_INFO [ exec cat ${GDB4HPC_CURPATH}/release_info ]
    } else {
       set REL_INFO ""
    }


 proc ModulesHelp { } {
    global GDB4HPC_CURPATH
    global GDB4HPC_LEVEL
    global REL_INFO
    puts stderr "The modulefile defines the system paths and"
    puts stderr "variables for the product gdb4hpc."
    puts stderr "$REL_INFO"
    puts stderr "============================================="
    puts stderr "To re-display gdb4hpc/$GDB4HPC_LEVEL release information,"
    puts stderr "type:    less $GDB4HPC_CURPATH/release_info"
    puts stderr "==============================================\n"
    puts stderr ""
 }


module-whatis   "Loads the Cray Line Mode Parallel Debugger."

'''
