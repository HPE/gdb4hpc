#!/bin/bash
#
# clean merge tool
# A script to automatically cleanup after running the merge monash script.
# We do not want to cleanup while running the merge monash script in the 
# event that a conflict prevented us from commiting the changes to the
# Cray repository.
#
# ©2010-2012 Cray Inc.  All Rights Reserved.
#
# Unpublished Proprietary Information.
# This unpublished work is protected to trade secret, copyright and other laws.
# Except as permitted by contract or express written permission of Cray Inc.,
# no part of this work or its content may be used, reproduced or disclosed
# in any form.
#
# $HeadURL$
# $Date$
# $Rev$
# $Author$
#

# paths
cray_WC=cray_ccdb_trunk
monash_WC=monash_crayrev_trunk

# remove the checkin log
if [ -e ci.log ]; then
	rm -f ci.log
fi

# remove the cray working copy
if [ -d $cray_WC ]; then
	rm -rf $cray_WC
fi

# remove the monash working copy
if [ -d $monash_WC ]; then
	rm -rf $monash_WC
fi

echo "Done..."
