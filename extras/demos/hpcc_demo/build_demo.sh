#!/bin/bash
#
# build_demo.sh
# A primer script to automagically build the ccdb demo.
#
# ©2013 Cray Inc.  All Rights Reserved.
#
# Unpublished Proprietary Information.
# This unpublished work is protected to trade secret, copyright and other laws.
# Except as permitted by contract or express written permission of Cray Inc.,
# no part of this work or its content may be used, reproduced or disclosed
# in any form.
#

#
# setup local variables
#
cur_dir=$(pwd)
build_dir="$cur_dir/hpcc-1.4.1"
# ensure this gets exported
export build_dir
bin_dir="$cur_dir"
src_dir="$build_dir/hpl/src/pgesv"

bug_src_file="HPL_pdupdateTT.c.broken"
orig_src_file="HPL_pdupdateTT.c.working"
src_file="HPL_pdupdateTT.c"
src_tarball="hpcc.source.tar.gz"

app="hpcc"
arch="CCLE"

# Name of each apps final binary
procs_1_bin_name="hpcc_broken"
procs_2_bin_name="hpcc_working"

#
# First ensure that the proper PrgEnv module is loaded.
#
echo -e "\n########################################################"
echo    "Verifying PrgEnv-cray module is currently loaded..."
echo -e "########################################################\n"

# Ensure the module file is sourced in properly.
if [ -e /etc/modules ] ; then
	source /etc/modules/bash
fi
if [ -d /sw/modules/init ] ; then
	source /sw/modules/init/bash
fi
if [ -d /opt/modules/default/init ] ; then
	source /opt/modules/default/init/bash
fi

loaded_PrgEnv_module=$(module list -l 2>&1 > /dev/null | grep 'PrgEnv' | awk '{match($1,"PrgEnv-[a-z]+",a)}END{print a[0]}')

if test "x$loaded_PrgEnv_module" = "x"; then
	echo "Loading module PrgEnv-cray."
	module load PrgEnv-cray
elif test "x$loaded_PrgEnv_module" != "xPrgEnv-cray"; then
	echo "Warning: Swapping $loaded_PrgEnv_module with PrgEnv-cray."
	module swap $loaded_PrgEnv_module PrgEnv-cray
else
	echo "Using loaded $loaded_PrgEnv_module."
fi

#
# Extract the source if it doesn't already exist
#
if test ! -e $build_dir; then
	echo -e "\n########################################################"
	echo    "Extracting source files..."
	echo -e "########################################################\n"
	echo "tar xvzf $src_tarball"
	tar xvzf $src_tarball
fi

#
# cd to the build directory and clean
#
echo -e "\n########################################################"
echo    "Ensuring clean build..."
echo -e "########################################################\n"
cd $build_dir
make clean arch=$arch
cd $build_dir/hpl
make clean_arch_all arch=$arch

#
# Remove the old binaries if they already exist
#
if test -e $bin_dir/$procs_1_bin_name; then
	echo "rm -f $bin_dir/$procs_1_bin_name"
	rm -f $bin_dir/$procs_1_bin_name
fi
if test -e $bin_dir/$procs_2_bin_name; then
	echo "rm -f $bin_dir/$procs_1_bin_name"
	rm -f $bin_dir/$procs_1_bin_name
fi

#
# Setup and build the bugged version
#
echo -e "\n########################################################"
echo    "Building bugged application..."
echo -e "########################################################\n"
cd $build_dir
cp $src_dir/$bug_src_file $src_dir/$src_file
make all arch=$arch
echo "mv $build_dir/$app $bin_dir/$procs_1_bin_name"
mv $build_dir/$app $bin_dir/$procs_1_bin_name
echo "chmod 777 $bin_dir/$procs_1_bin_name"
chmod 777 $bin_dir/$procs_1_bin_name


#
# Setup and build the fixed version
#
echo -e "\n########################################################"
echo    "Building working application..."
echo -e "########################################################\n"
cd $build_dir
cp $src_dir/$orig_src_file $src_dir/$src_file
make all arch=$arch
echo "mv $build_dir/$app $bin_dir/$procs_2_bin_name"
mv $build_dir/$app $bin_dir/$procs_2_bin_name
echo "chmod 777 $bin_dir/$procs_2_bin_name"
chmod 777 $bin_dir/$procs_2_bin_name

echo -e "\n########################################################"
echo    "Cleaning build..."
echo -e "########################################################\n"
cd $build_dir
make clean arch=$arch
cd $build_dir/hpl
make clean_arch_all arch=$arch

echo -e "\n########################################################"
echo    "Finished building the demo."
echo -e "########################################################\n"

