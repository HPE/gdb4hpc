#!/bin/bash
#
# merge monash tool
# A script to automatically merge monash repository commits to the
# Cray ccdb source repository.
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
monash_guard_url=http://svnpub.infotech.monash.edu.au/svn/guard/guard/trunk
monash_guard_rd_url=http://svnpub.infotech.monash.edu.au/svn/guard/guard-rd/trunk
monash_libaif_url=http://svnpub.infotech.monash.edu.au/svn/guard/libaif/trunk
monash_libmi_url=http://svnpub.infotech.monash.edu.au/svn/guard/libmi/trunk
monash_cray_specific_url=http://svnpub.infotech.monash.edu.au/svn/guard/cray_specific/trunk
cray_repo_url=http://svn/svn/debugger-support/ccdb/trunk
cray_tags_url=http://svn/svn/debugger-support/ccdb/tags/Auto_merged

# paths
local_WC=cray_ccdb_trunk
monash_WC=monash_crayrev_trunk

# ensure we have write permissions on the current directory
if [ ! -w . ]; then
	echo "ERROR: You do not have write permissions on the current dir."
	exit 1
fi

# grab a working copy of the trunk from the cray-svn
svn co $cray_repo_url $local_WC
echo -e "Checkout of local working copy complete.\n\n"

# make sure the monash.rev file exists, we depend on it
if [ ! -e $local_WC/monash.rev ]; then
	echo "ERROR: monash.rev file not found inside the local working copy."
	exit 1
fi

# find out the current merged revision of the monash repo
let cur_cray=`cat $local_WC/monash.rev | sed -ne "s/^\([^#]\)/\1/p" | sed -ne "s/CUR_REV=//p"`
echo "Current cray ccdb trunk revision: $cur_cray"

# find out the current HEAD revision of the monash repo
# and the last changed revision of each monash repo
echo "Fetching svn info from the monash repository..."

# Main monash repo
info_monash=`svn info $monash_repo_url@HEAD`
# This sed command will replace the entire svn info string with the current revision number.
let cur_monash=`echo $info_monash | sed -e "s/.*[[:space:]]Revision: \([^[:space:]]*\).*/\1/"`
echo "Current monash guard repo revision: $cur_monash"

# guard trunk
info_guard=`svn info $monash_guard_url@HEAD`
# This sed command will replace the entire svn info string with the last changed revision number.
let cur_guard=`echo $info_guard | sed -e "s/.*[[:space:]]Last Changed Rev: \([^[:space:]]*\).*/\1/"`
echo "Current monash guard trunk revision: $cur_guard"

# guard-rd trunk
info_guardrd=`svn info $monash_guard_rd_url@HEAD`
let cur_guardrd=`echo $info_guardrd | sed -e "s/.*[[:space:]]Last Changed Rev: \([^[:space:]]*\).*/\1/"`
echo "Current monash guard-rd trunk revision: $cur_guardrd"

# libaif trunk
info_libaif=`svn info $monash_libaif_url@HEAD`
let cur_libaif=`echo $info_libaif | sed -e "s/.*[[:space:]]Last Changed Rev: \([^[:space:]]*\).*/\1/"`
echo "Current monash libaif trunk revision: $cur_libaif"

# libmi trunk
info_libmi=`svn info $monash_libmi_url@HEAD`
let cur_libmi=`echo $info_libmi | sed -e "s/.*[[:space:]]Last Changed Rev: \([^[:space:]]*\).*/\1/"`
echo "Current monash libmi trunk revision: $cur_libmi"

# cray specific trunk
info_crayspecific=`svn info $monash_cray_specific_url@HEAD`
let cur_crayspecific=`echo $info_crayspecific | sed -e "s/.*[[:space:]]Last Changed Rev: \([^[:space:]]*\).*/\1/"`
echo "Current monash cray specific trunk revision: $cur_crayspecific"

# first we check to see if something has been changed
if [ "$cur_cray" -lt "$cur_monash" ]; then
	# Something has changed, so we actually need to merge
	echo "A merge is required!"
	# Start off with generating a conformant svn log message
	echo "Subject: Auto merge of monash repo r$cur_monash" > ci.log
	echo "Author: $user_name" >> ci.log
	echo "Description: This is an automatic merge of monash repo r$cur_monash" >> ci.log
	echo "using the automatic merge script." >> ci.log						
	
	# Create a tag of the cray trunk
	svn copy $cray_repo_url $cray_tags_url/before_monash_$cur_monash --file ci.log
	echo "Created tag: $cray_tags_url/before_monash_$cur_monash"
	
	# compare each last rev number with the current cray rev
	# First check guard
	if [ "$cur_cray" -lt "$cur_guard" ]; then
		echo "Guard merge required."
		echo "Making merge..."
		# we need to merge the guard dir so lets make the merge
		svn merge $monash_guard_url@$cur_cray \
		$monash_guard_url@$cur_guard \
		$local_WC/guard
		echo "Running autoreconf on guard..."
		# run autoreconf
		autoreconf -iv $local_WC/guard
	fi
	# Next check guard-rd
	if [ "$cur_cray" -lt "$cur_guardrd" ]; then
		echo "Guard-rd merge required."
		echo "Making merge..."
		svn merge $monash_guard_rd_url@$cur_cray \
		$monash_guard_rd_url@$cur_guardrd \
		$local_WC/guard/guard-rd
		echo "Running autoreconf on guard-rd..."
		autoreconf -iv $local_WC/guard/guard-rd
	fi
	# Next check libaif
	if [ "$cur_cray" -lt "$cur_libaif" ]; then
		echo "Libaif merge required."
		echo "Making merge..."
		svn merge $monash_libaif_url@$cur_cray \
		$monash_libaif_url@$cur_libaif \
		$local_WC/libaif
		echo "Running autoreconf on libaif..."
		autoreconf -iv $local_WC/libaif
	fi
	# Check libmi
	if [ "$cur_cray" -lt "$cur_libmi" ]; then
		echo "Libmi merge required."
		echo "Making merge..."
		svn merge $monash_libmi_url@$cur_cray \
		$monash_libmi_url@$cur_libmi \
		$local_WC/libmi
	fi
	# Check Cray specific
	if [ "$cur_cray" -lt "$cur_crayspecific" ]; then
		echo "Cray specific merge required."
		echo "Making merge..."
		svn merge $monash_cray_specific_url@$cur_cray \
		$monash_cray_specific_url@$cur_crayspecific \
		$local_WC
	fi
	echo "Merge complete."

	# We need to change the revision numbers in the monash.rev file
	# First change the previous revision
	# This sed command looks for PRE_REV=xxx where xxx is the previous revision number
	# and changes xxx to the current monash.rev of the cray repo
	cat $local_WC/monash.rev | sed -e "s/\(PRE_REV=\)[[:digit:]]*/\1$cur_cray/" > $local_WC/rev1
	# We need to increment $cur_monash by one because we are going to also update the cray.rev inside
	# of the monash repository. Note that there is a horrible race condition here where if someone
	# from monash happens to be checking stuff in at the same time we are running this script, it
	# could break our revision numbers. However, due to the time differential lets just assume that
	# the chances of this happening are relatively low.
	let "new_monash = $cur_monash + 1"
	# Next change the current revision
	cat $local_WC/rev1 | sed -e "s/\(CUR_REV=\)[[:digit:]]*/\1$new_monash/" > $local_WC/monash.rev
	rm -f $local_WC/rev1
	echo "monash.rev updated to previous version $cur_cray and current version $cur_monash."
	
	# Now we need to commit our changes
	echo "Committing changes to the cray repository."
	svn ci $local_WC --file ci.log

	# Create a new tag of the trunk after the ci
	svn copy $cray_repo_url $cray_tags_url/after_monash_$cur_monash --file ci.log
	echo "Created tag: $cray_tags_url/after_monash_$cur_monash"

	# Checkout only the cray.rev file from the monash repository.
	svn co $monash_cray_specific_url $monash_WC --depth empty
	svn up $monash_WC/cray.rev

	if [ ! -e $monash_WC/cray.rev ]; then
		echo "ERROR: cray.rev file not found inside the monash working copy."
		exit 1
	fi

	echo "Checkout of Monash cray.rev complete."

	# New cray repo info
	info_cray=`svn info $cray_repo_url@HEAD`
	# This sed command will replace the entire svn info string with the current revision number
	let new_cray=`echo $info_cray | sed -e "s/.*[[:space:]]Revision: \([^[:space:]]*\).*/\1/"`
	echo "New cray ccdb repo revision: $new_cray"

	# We need to update the revision numbers in the cray.rev file to ignore the revisions created
	# by this script.
	# We only need to change the current revision since the commits made by this script doesn't 
	# actually change any of the files. It just makes sure both repositories are mirror images.
	cat $monash_WC/cray.rev | sed -e "s/\(CUR_REV=\)[[:digit:]]*/\1$new_cray/" > $monash_WC/cray.rev2
	mv $monash_WC/cray.rev2 $monash_WC/cray.rev
	echo "cray.rev updated to current version $new_cray."

	echo "Committing changes to the monash repository."
	svn ci $monash_WC --message "Auto updated cray.rev."
else
	echo "Merge unnecessary."
fi

echo "Done..."

