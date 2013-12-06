#!/bin/bash
#
# merge cray tool
# A script to automatically merge Cray ccdb repository commits to the monash
# guard related repositories.
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

################################################################################
#### EDITME: Change the following variable to the location where your       ####
#### local monash working copy is located. It takes a long time to checkout ####
#### the monash repository, so we expect the user to have all of the        ####
#### repositories sitting in a directory somewhere. There also tends to be  ####
#### conflicts in the merge process that need to be resolved by hand using  ####
#### svn resolved. Most of these are bogus conflicts. The user of this      ####
#### script is then expected to commit all of the necessary changes to the  ####
#### monash repository manually.                                            ####
####                                                                        ####
#### The expected layout of the specified directory is as                   ####
#### follows:                                                               ####
####                                                                        ####
#### specified_directory_root                                               ####
####           |-cray_specific (cray_specific/trunk)                        ####
####           |-guard (guard/trunk)                                        ####
####           |---guard-rd (guard-rd/trunk)                                ####
####           |-libaif (libaif/trunk)                                      ####
####           |-libmi (libmi/trunk)                                        ####
####                                                                        ####
################################################################################
local_WC=/cray/css/users/andrewg/repositories/monash_svn

# vars
user_name=`whoami`

# long url names
cray_repo_url=http://svn/svn/debugger-support/ccdb/trunk
cray_guard_url=http://svn/svn/debugger-support/ccdb/trunk/guard
cray_guard_rd_url=http://svn/svn/debugger-support/ccdb/trunk/guard/guard-rd
cray_libaif_url=http://svn/svn/debugger-support/ccdb/trunk/libaif
cray_libmi_url=http://svn/svn/debugger-support/ccdb/trunk/libmi
monash_cray_specific_url=http://svnpub.infotech.monash.edu.au/svn/guard/cray_specific/trunk
monash_guard_url=http://svnpub.infotech.monash.edu.au/svn/guard/guard/trunk
monash_guard_rd_url=http://svnpub.infotech.monash.edu.au/svn/guard/guard-rd/trunk
monash_libaif_url=http://svnpub.infotech.monash.edu.au/svn/guard/libaif/trunk
monash_libmi_url=http://svnpub.infotech.monash.edu.au/svn/guard/libmi/trunk

# paths
cray_WC=$local_WC/cray_specific
guard_WC=$local_WC/guard
guardrd_WC=$local_WC/guard/guard-rd
libaif_WC=$local_WC/libaif
libmi_WC=$local_WC/libmi

# booleans to keep track of changes being made
guard_change=false
guardrd_change=false
libaif_change=false
libmi_change=false

# ensure we have write permissions on the current directory
if [ ! -w $local_WC ]; then
	echo "ERROR: You do not have write permissions on local working directory."
	exit 1
fi

# update our working copy of the monash-svn
svn update $cray_WC
svn update $guard_WC
svn update $guardrd_WC
svn update $libaif_WC
svn update $libmi_WC
echo -e "Update of local monash trunk complete.\n\n"

# make sure the cray.rev file exists, we depend on it
if [ ! -e $cray_WC/cray.rev ]; then
	echo "ERROR: cray.rev file not found inside the local working copy."
	exit 1
fi

# find out the current merged revision of the cray repo
let cur_monash=`cat $cray_WC/cray.rev | sed -ne "s/^\([^#]\)/\1/p" | sed -ne "s/CUR_REV=//p"`
echo "Current monash guard trunk revision: $cur_monash"

# find out the current HEAD revision of the cray repo
# and the last changed revision of each cray repo
echo "Fetching svn info from the cray repository..."

# Main cray repo
info_cray=`svn info $cray_repo_url@HEAD`
# This sed command will replace the entire svn info string with the current revision number.
let cur_cray=`echo $info_cray | sed -e "s/.*[[:space:]]Revision: \([^[:space:]]*\).*/\1/"`
echo "Current cray ccdb repo revision: $cur_cray"

# guard trunk
info_guard=`svn info $cray_guard_url@HEAD`
# This sed command will replace the entire svn info string with the last changed revision number.
let cur_guard=`echo $info_guard | sed -e "s/.*[[:space:]]Last Changed Rev: \([^[:space:]]*\).*/\1/"`
echo "Current cray guard trunk revision: $cur_guard"

# guard-rd trunk
info_guardrd=`svn info $cray_guard_rd_url@HEAD`
let cur_guardrd=`echo $info_guardrd | sed -e "s/.*[[:space:]]Last Changed Rev: \([^[:space:]]*\).*/\1/"`
echo "Current cray guard-rd trunk revision: $cur_guardrd"

# libaif trunk
info_libaif=`svn info $cray_libaif_url@HEAD`
let cur_libaif=`echo $info_libaif | sed -e "s/.*[[:space:]]Last Changed Rev: \([^[:space:]]*\).*/\1/"`
echo "Current cray libaif trunk revision: $cur_libaif"

# libmi trunk
info_libmi=`svn info $cray_libmi_url@HEAD`
let cur_libmi=`echo $info_libmi | sed -e "s/.*[[:space:]]Last Changed Rev: \([^[:space:]]*\).*/\1/"`
echo "Current cray libmi trunk revision: $cur_libmi"

# first we check to see if something has been changed
if [ "$cur_monash" -lt "$cur_cray" ]; then
	# Something has changed, so we actually need to merge
	echo "A merge is required!"

	# compare each last rev number with the current monash rev
	# First check guard-rd
	if [ "$cur_monash" -lt "$cur_guardrd" ]; then
		echo "Guard-rd merge required."
		echo "Making merge..."
		# we need to merge the guard-rd dir so lets make the merge
		svn merge $cray_guard_rd_url@$cur_monash \
		$cray_guard_rd_url@$cur_guardrd \
		$guardrd_WC
		# set the changed boolean to true
		guardrd_change=true
	fi

	# Next check guard
	if [ "$cur_monash" -lt "$cur_guard" ]; then
		echo "Guard merge required."
		echo "Making merge..."
		svn merge $cray_guard_url@$cur_monash \
		$cray_guard_url@$cur_guard \
		$guard_WC
		# set the changed boolean to true
		guard_change=true
	fi

	# Next check libaif
	if [ "$cur_monash" -lt "$cur_libaif" ]; then
		echo "Libaif merge required."
		echo "Making merge..."
		svn merge $cray_libaif_url@$cur_monash \
		$cray_libaif_url@$cur_libaif \
		$libaif_WC
		echo "Running autoreconf on libaif..."
		autoreconf -iv $libaif_WC
		# set the changed boolean to true
		libaif_change=true
	fi

	# Check libmi
	if [ "$cur_monash" -lt "$cur_libmi" ]; then
		echo "Libmi merge required."
		echo "Making merge..."
		svn merge $cray_libmi_url@$cur_monash \
		$cray_libmi_url@$cur_libmi \
		$libmi_WC
		echo "Running autoreconf on libmi..."
		autoreconf -iv $libmi_WC
		# set the changed boolean to true
		libmi_change=true
	fi

	# Finally merge any cray specific stuff not in the other trunks
	echo "Making final cray_specific merge..."
	svn merge $cray_repo_url@$cur_monash \
	$cray_repo_url@$cur_cray \
	$cray_WC

	echo "Merge complete."

	# We need to change the revision numbers in the cray.rev file
	# First change the previous revision
	# This sed command looks for PRE_REV=xxx where xxx is the previous revision number
	# and changes xxx to the current monash.rev of the cray repo
	cat $cray_WC/cray.rev | sed -e "s/\(PRE_REV=\)[[:digit:]]*/\1$cur_monash/" > $cray_WC/rev1
	# Next change the current revision
	cat $cray_WC/rev1 | sed -e "s/\(CUR_REV=\)[[:digit:]]*/\1$cur_cray/" > $cray_WC/cray.rev
	rm -f $cray_WC/rev1
	echo "cray.rev updated to previous version $cur_monash and current version $cur_cray."
	
	# Now we should automatically resolve the conflicts that we expect to always happen in the
	# cray_specific directory. On the Cray side of things, there is only one ccdb source 
	# repository that contains all of the source files. On the Monash side there are several
	# repositories that are split up. Whenever we merge in changes to the local monash cray_specific
	# repo, it complains about any other directory that was changed not existing. For instance if
	# we had changes inside the guard directory, the cray_specific repo would complain that the
	# directory guard does not exist, since it exists in its own source repository. The user will
	# most likely still have to resolve some dependencies by hand, but this should take care of all
	# the common ones that seem to always occur.
	if $guard_change ; then
		echo "Resolving $cray_WC/guard conflict."
		svn resolved $cray_WC/guard
	fi
	if $guardrd_change ; then
		echo "Resolving $guard_WC/guard-rd conflict."
		svn resolved $guard_WC/guard-rd
	fi
	if $libaif_change ; then
		echo "Resolving $cray_WC/libaif conflict."
		svn resolved $cray_WC/libaif
	fi
	if $libmi_change ; then
		echo "Resolving $cray_WC/libmi conflict."
		svn resolved $cray_WC/libmi
	fi

	echo "Done auto resolving conflicts."

	# This script does not do any commiting of changes.
	# We expect the developer to provide a verbose log message detailing the changes for
	# the monash repository

	echo -e "\n\nNote that nothing has actually been checked into the monash repository."
	echo "It is your responsibility to do the final svn ci commands for each repo."
	echo "Please provide a detailed log message recording the specific changes made to each repo."
else
	echo "Merge unnecessary."
fi

echo "Done..."

