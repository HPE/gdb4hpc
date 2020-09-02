# Dejagnu testsuite makefile
# Copyright 2010, 2020 Hewlett Packard Enterprise Development LP.

# These coded instructions, statements, and computer programs contain
# unpublished proprietary information of Hewlett Packard Enterprise Development LP., and are protected
# by Federal copyright law.  They  may  not  be  disclosed  to  third
# parties  or copied or duplicated in any form, in whole or in part,
# without the prior written consent of Hewlett Packard Enterprise Development LP.  

# Author: Andrew Gontarek (andrewg@cray.com)

## Process this file with automake to generate Makefile.in

AUTOMAKE_OPTIONS 	= dejagnu
DEJATOOL 		= guard
SUBDIRS 		= example_progs/shallow_example \
	  		  example_progs/mpi_example
EXTRA_DIST 		= global-config.exp \
	     		  boards/cray.exp \
	     		  config/cray_stubs.exp \
	     		  guard.test/guard_ParallelInvoke.exp \
	     		  guard.test/guard_SimpleAssert.exp \
	     		  guard.test/guard_SimpleInvoke.exp \
	     		  lib/guard.exp \
	     		  lib/qsub_stubs.exp
RUNTESTDEFAULTFLAGS 	= --tool $$tool --target_board=cray 
