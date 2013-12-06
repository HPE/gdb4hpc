#!/bin/bash
#
# find_mod.sh
# A script to determine the loaded lgdb module.
#
# ©2013 Cray Inc.  All Rights Reserved.
#
# Unpublished Proprietary Information.
# This unpublished work is protected to trade secret, copyright and other laws.
# Except as permitted by contract or express written permission of Cray Inc.,
# no part of this work or its content may be used, reproduced or disclosed
# in any form.
#

. ${MODULESHOME}/init/sh

loaded_module=$(module list -l 2>&1 > /dev/null | grep 'cray-lgdb' | awk '{match($1,"cray-lgdb[/0-9DEV.]*",a)}END{print a[0]}')

if test "x$loaded_module" = "x"; then
	loaded_module=$(module list 2>&1 > /dev/null | grep 'cray-lgdb' | awk '{match($2,".*cray-lgdb[/0-9DEV.]*",a)}END{print a[0]}')
	if test "x$loaded_module" = "x"; then
		exit 1
	fi
	echo -n "$loaded_module"
	exit 0
fi

location=$(module show $loaded_module 2>&1 > /dev/null | grep /$loaded_module | awk '{match($1,".*/'$loaded_module'",a)}END{print a[0]}')

echo -n "$location"
