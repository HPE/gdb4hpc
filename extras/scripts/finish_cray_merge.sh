#!/bin/bash
#
# Finish cray merge tool
# A script to automatically update the cray.rev and monash.rev files
# in both the cray and monash repositories after finishing a manual
# commit to the monash repository.
#
# Note that this script should only be run when the user has finished
# committing the required changes to the monash repository that were
# created by running the merge cray script.
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

# vars
user_name=`whoami`

# long url names
monash_repo_url=http://svnpub.infotech.monash.edu.au/svn/guard
monash_cray_specific_url=http://svnpub.infotech.monash.edu.au/svn/guard/cray_specific/trunk
cray_repo_url=http://svn/svn/debugger-support/ccdb/trunk

# paths
cray_WC=cray_monashrev_trunk
monash_WC=monash_crayrev_trunk

# ensure we have write permissions on the current directory
if [ ! -w . ]; then
	echo "ERROR: You do not have write permissions on the current dir."
	exit 1
fi

echo -e "\nChecking out the monash.rev file from the Cray repository...\n"

# Checkout only the monash.rev file from the Cray repository
svn co $cray_repo_url $cray_WC --depth empty
svn up $cray_WC/monash.rev

if [ ! -e $cray_WC/monash.rev ]; then
	echo "ERROR: monash.rev file not found inside the Cray working copy."
	exit 1
fi

echo -e "\nCheckout of Cray monash.rev complete.\n"

echo -e "Checking out the cray.rev file from the Monash repository...\n"

# Checkout only the cray.rev file from the monash repository
svn co $monash_cray_specific_url $monash_WC --depth empty
svn up $monash_WC/cray.rev

if [ ! -e $monash_WC/cray.rev ]; then
	echo "ERROR: cray.rev file not found inside the Monash working copy."
	exit 1
fi

echo -e "\nCheckout of Monash cray.rev complete.\n"

echo "Gathering revision information about the Monash repository..."

# Find out the current revision stored in the monash.rev file
let monash_rev=`cat $cray_WC/monash.rev | sed -ne "s/^\([^#]\)/\1/p" | sed -ne "s/CUR_REV=//p"`
echo "CUR_REV stored in Cray's monash.rev file: $monash_rev"

# find out the current revision of the monash repo
info_monash=`svn info $monash_repo_url@HEAD`
# This sed command will replace the entire svn info string with the current revision number.
let cur_monash=`echo $info_monash | sed -e "s/.*[[:space:]]Revision: \([^[:space:]]*\).*/\1/"`
echo -e "Current Monash guard repo revision: $cur_monash\n"

if [ "$monash_rev" -lt "$cur_monash" ]; then
	echo -e "An update of the revision files is needed!\n"
	
	# Start by changing the revision numbers in the monash.rev file
	# Note that since this is fixing the merge, we do not want to update the
	# previous revision since no "real" changes to code occured. It was just
	# related to bringing the two repositories up to date with one another.
	#
	# Also, we want to incremement the CUR_REV by one in the monash.rev file
	# since commit's happen much more frequently to the cray debugger svn.
	# There is a race condition here where updating to the monash repo
	# between finishing the manual commit and running this script could cause
	# the monash.rev file to go out of sync. Since there is a big time difference
	# between AUS and US, we consider the chance of this happening remote.
	(( cur_monash++ ))
	cat $cray_WC/monash.rev | sed -e "s/\(CUR_REV=\)[[:digit:]]*/\1$cur_monash/" > $cray_WC/monash.rev2
	mv $cray_WC/monash.rev2 $cray_WC/monash.rev
	echo "monash.rev updated to CUR_REV $cur_monash."

	# Generate a conformat svn log message for the Cray repo
	echo "Subject: Auto fix of monash.rev" > ci.log
	echo "Author: $user_name" >> ci.log
	echo "Description: This is an automatic fix of monash.rev" >> ci.log
	echo "using the finish Cray merge script." >> ci.log

	# Now we need to commit our changes
	echo "Committing changes to the Cray repository."
	svn ci $cray_WC --file ci.log

	# Cleanup the log file
	rm ci.log

	# find out the current revision of the cray repo
	info_cray=`svn info $cray_repo_url@HEAD`
	# This sed command will replace the entire svn info string with the current revision number.
	let cur_cray=`echo $info_cray | sed -e "s/.*[[:space:]]Revision: \([^[:space:]]*\).*/\1/"`
	echo -e "\nCurrent Cray ccdb repo revision: $cur_cray"

	# We need to update the revision number in the cray.rev file. Similar to before, we do not
	# want to update the PRE_REV value.
	cat $monash_WC/cray.rev | sed -e "s/\(CUR_REV=\)[[:digit:]]*/\1$cur_cray/" > $monash_WC/cray.rev2
	mv $monash_WC/cray.rev2 $monash_WC/cray.rev
	echo "cray.rev updated to CUR_REV $cur_cray."

	# Now we need to commit our changes
	echo "Committing changes to the Monash repository."
	svn ci $monash_WC --message "Auto updated cray.rev."
else
	echo "Update of the revision files unnecessary."
fi

# Cleanup the local working copies
rm -rf $cray_WC
rm -rf $monash_WC

echo "Done..."

