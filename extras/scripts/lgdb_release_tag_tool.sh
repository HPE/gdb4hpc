#!/bin/bash
#
# Release tag tool
# A script to atuomagically create a release tag for ccdb/lgdb and all of
# its dependencies.
#
# ©2013-2014 Cray Inc.  All Rights Reserved.
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

#####################################################
# Repository settings                               #
# Modify these according to your repo environment.  #
#####################################################

# SVN repostiory root for the release branches
lgdb_RB_root="http://svn/svn/debugger-support/ccdb/branches/lgdb"
# externals RB are below
cti_RB_root="http://svn/svn/debugger-support/tools_interface/branches/RB-"

# SVN repository root for the release tags
lgdb_RT_root="http://svn/svn/debugger-support/ccdb/tags/lgdb_release"
# externals RT are below
cti_RT_root="http://svn/svn/debugger-support/tools_interface/tags/lgdb_release"

# Base directory names for external components
cti_dir_base="craytool_interface"

# Primer files for svn:externals properties
svnprop_1="svn-prop.template.1"
svnprop_2="svn-prop.template.2"

# Temporary repo checkout location
temprepo="tmp"

###############################
# Internal script variables   #
###############################

# Revision number variables
major=0
minor=0
rev=0
confirmed=n

cti_major=0
cti_minor=0

user_name=`whoami`

##################################
# Ask user for current rev.      #
##################################

while [[ $confirmed == n ]]; do
	# Ask for the major 
	echo -ne "\n1. Enter the major lgdb number: "
	read major
	# Ask for the minor
	echo -n "2. Enter the minor lgdb number: "
	read minor
	# Ask for the revision
	echo -n "3. Enter the revision lgdb number: "
	read rev

	# Ask for the cti major
	echo -n "4. Enter the current major cti number: "
	read cti_major
	# Ask for the cti minor
	echo -n "5. Enter the current minor cti number: "
	read cti_minor

	# Ask the user to confirm
	echo -e "\nCreating tags for lgdb release $major.$minor.$rev"
	echo -e "Using cti branch $cti_major.$cti_minor"
	echo -n "Is the entered release numbers correct?[y/n]: "
	while true; do
		read confirmed
		if [[ $confirmed == y || $confirmed == n ]]; then
			break
		fi
		echo -ne "\tEnter y or n: "
	done
done

# Create a conformant message for the cray svn commits
echo "Subject: Auto tag for lgdb $major.$minor.$rev release" > ci.log
echo "Author: $user_name" >> ci.log
echo "Description: This is an automatic tag for lgdb $major.$minor.$rev release" >> ci.log
echo "using the release tag tool." >> ci.log

# Create the release tag for the external components

# Create CTI release tag
echo -e "########################################################\n"
echo "Creating release tag for Cray Tools Interface in $cti_RT_root/$major.$minor.$rev/$cti_dir_base/"
echo "Creating tag directory $cti_RT_root/$major.$minor.$rev/"
svn mkdir $cti_RT_root/$major.$minor.$rev/ --file ci.log
echo "Creating tag from branch..."
svn copy $cti_RB_root$cti_major.$cti_minor/ $cti_RT_root/$major.$minor.$rev/$cti_dir_base/ --file ci.log

# Temporarily change the lgdb branch externals to point at the proper release tags

# Checkout empty top level lgdb branch
echo -e "########################################################\n"
echo "Changing svn:externals for lgdb branch"
echo "Checking out empty top level lgdb branch."
svn co $lgdb_RB_root/$major.$minor/ $temprepo --depth empty

# Backup original svn:externals prop
echo "Backing up current svn:externals to svn-prop.orig.1"
svn propget svn:externals $temprepo > svn-prop.orig.1

# Create a temp svn-prop file from template
echo "Changing svn:externals for top level lgdb branch"
cat $svnprop_1 | sed -e "s@%CTI%@$cti_RT_root/$major.$minor.$rev/$cti_dir_base/@" > svn-prop.tmp
svn propset svn:externals $temprepo --file svn-prop.tmp
svn ci $temprepo --file ci.log

# Cleanup the temp repo
rm -rf $temprepo
rm svn-prop.tmp

# Create the release tag of the lgdb release branch
echo -e "########################################################\n"
echo "Creating release tag for lgdb in $lgdb_RT_root/$major.$minor.$rev/"
svn copy $lgdb_RB_root/$major.$minor/ $lgdb_RT_root/$major.$minor.$rev/ --file ci.log

# Restore original svn:externals prop
echo -e "########################################################\n"
echo "Restore svn:externals for lgdb branch"

# Restore the top level branch
echo "Checking out empty top level lgdb branch."
svn co $lgdb_RB_root/$major.$minor/ $temprepo --depth empty

echo "Restoring svn:externals for top level lgdb branch"
svn propset svn:externals $temprepo --file svn-prop.orig.1
svn ci $temprepo --file ci.log

# Cleanup the temp repo
rm -rf $temprepo
rm svn-prop.orig.1
rm ci.log

echo "Done..."

