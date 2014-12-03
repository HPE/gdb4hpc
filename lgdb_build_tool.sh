#!/bin/bash
#
# lgdb build tool
# A primer script to automagically build guard and all of its
# dependencies.
#
# ©2010-2014 Cray Inc.  All Rights Reserved.
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
# Build settings                                    #
# Modify these according to your build environment. #
#####################################################

# Build version
lgdb_build_version="2.4"

# Current directory name where dependencies are located
thirdparty_directory_name="third_party"
extras_directory_name="extras"

# Directory name where the final lgdb package will be placed
lgdb_package_directory_name="lgdb_install"

# Directory name that will be used as a "staging" directory, that is
# dependency packages will be installed to the staging directory and only things
# that are needed in the final lgdb package will be included in the lgdb package
# directory.
lgdb_staging_directory_name="lgdb_tmp"

# Directory name where rpms are extracted to
lgdb_externals_directory_name="external"

# Third party source tree names. 
mrnet_directory_name="mrnet-4.1.0a-2992b89"
gdb_directory_name="ddt_gdb-7.6.2"
cuda55_gdb_directory_name="ddt_cuda_gdb_55"
cuda65_gdb_directory_name="ddt_cuda_gdb_65"
uda_directory_name="gdb_uda"
editline_directory_name="editline"
libmi_directory_name="libmi"
libssh2_directory_name="libssh2"
libarchive_directory_name="libarchive"
python_directory_name="Python-2.6.2"

# Python extras name
python_extras_name="python_extras"
# File containing list of python dependences
python_depmod_name="python_files"
# Directory containing gdb pretty printers
python_gdb_name="gdb"
# Directory to install python dependencies to
python_inst_name="python2.6"

# extra python directory build by gdb
gdb_extra_python_path="gdb/data-directory/python"
# extra syscalls directory build by gdb
gdb_extra_syscalls_path="gdb/data-directory/syscalls"
# bad file name that we need to remove
gdb_bad_file="python/gdb/command/pdbx.py"

# libssh2 openssl path
openssl_path="/opt/cpkg/current/openssl/0.9.8k"

# base location of cudatoolkit rpms
cuda_rpm_dir="/cray/css/release/cray/build/xt/sles11sp3/x86_64/trunk-ari/working/latest/3rd-party/x86_64"
cuda55_basename="cray-cudatoolkit-5.5"
cuda65_basename="cray-cudatoolkit6.5-6.5"
cuda_inc_path="opt/nvidia/cudatoolkit/*/include"

# Current dependency source trees that are directly maintained in lgdb
aif_directory_name="libaif"
guard_directory_name="guard"
guard_rd_directory_name="guard-rd"

# External dependency source trees that are imported directly in lgdb
craytool_directory_name="craytool_interface"

# Demos directory name
demo_directory_name="demos"

# Docs directory name
docs_directory_name="docs"

# Current dependency dso revision information
libedit_dso="libedit.so"
libedit_major="libedit.so.0"
libedit_minor="libedit.so.0.0.41"

libcraytool_backend_dso="libcraytools_be.so"
libcraytool_backend_major="libcraytools_be.so.6"
libcraytool_backend_minor="libcraytools_be.so.6.0.0"

libcraytool_frontend_dso="libcraytools_fe.so"
libcraytool_frontend_major="libcraytools_fe.so.6"
libcraytool_frontend_minor="libcraytools_fe.so.6.0.0"

libssh2_dso="libssh2.so"
libssh2_major="libssh2.so.1"
libssh2_minor="libssh2.so.1.0.1"

libmrnet_dso="libmrnet.so"
libmrnet_major="libmrnet.so.4"
libmrnet_minor="libmrnet.so.4.1.0"

libxplat_dso="libxplat.so"
libxplat_major="libxplat.so.4"
libxplat_minor="libxplat.so.4.1.0"

########################################################
# Setup the build layout. The following variables are  #
# used to define where to find dependecies, where to   #
# place built sources, where to place the lgdb install #
# etc...                                               #
# I assume that the build tree is rooted at the PWD    #
# that I was called from.                              #
########################################################

###################################################
# The first bunch relate to the build environment #
###################################################

# Assume that we are based at PWD
cur_dir=$(pwd)
build_dir="$cur_dir" # Placeholder for a seperate build tree (not used)

# This defines where the dependency packages are located
thirdparty_dir="$cur_dir/$thirdparty_directory_name"
extras_dir="$cur_dir/$extras_directory_name"

# Define where to place the final lgdb install
inst_dir="$build_dir/$lgdb_package_directory_name"

# Define where to temporarily install dependency packages to
stage_dir="$build_dir/$lgdb_staging_directory_name"

# Define where rpms are extracted to
external_dir="$build_dir/$lgdb_externals_directory_name"

########################################################
# The following variables define source tree locations #
########################################################

# Define where to find the libaif source tree
aif_src="$cur_dir/$aif_directory_name"

# Define where to find the libmi source tree
libmi_src="$thirdparty_dir/$libmi_directory_name"

# Define where to find the libssh2 source tree
libssh2_src="$thirdparty_dir/$libssh2_directory_name"

# Define where to find the MRNet source tree
mrnet_src="$thirdparty_dir/$mrnet_directory_name"

# Define where to find the python source tree
python_src="$thirdparty_dir/$python_directory_name"
python_extras="$thirdparty_dir/$python_extras_name"
python_gdb="$python_extras/$python_gdb_name"
python_depmod="$python_extras/$python_depmod_name"

# Define where to find the gdb source tree
gdb_src="$thirdparty_dir/$gdb_directory_name"
gdb_python_dir="$gdb_src/$gdb_extra_python_path"
gdb_syscalls_dir="$gdb_src/$gdb_extra_syscalls_path"

# Define where to find the cuda-gdb source tree
cuda55_gdb_src="$thirdparty_dir/$cuda55_gdb_directory_name"
cuda65_gdb_src="$thirdparty_dir/$cuda65_gdb_directory_name"

# Define where to find the cuda-gdb external components
cuda55_ext="$external_dir/cuda55_base"
cuda65_ext="$external_dir/cuda65_base"

# Define where to find the gdb source tree
gdb_uda_src="$thirdparty_dir/$uda_directory_name"

# Define where to find the editline source tree
editline_src="$thirdparty_dir/$editline_directory_name"

# Define where to find the libarchive source tree
libarchive_src="$thirdparty_dir/$libarchive_directory_name"

# Define where to find the tools interface source tree
interface_src="$cur_dir/$craytool_directory_name"

# Define where to find the guard source tree
lgdb_src="$cur_dir/$guard_directory_name"

# Define where to find the guard-rd source tree
# Note that this typically resides inside of the guard source tree
lgdb_rd_src="$lgdb_src/$guard_rd_directory_name"

# Define where to find the module primer
module_primer="$extras_dir/modulefiles/cray-lgdb/2.0_primer"
module_version="2.4.0"

# Define where to find the demos
demo_dir="$extras_dir/$demo_directory_name"

# Define where to find the docs
docs_dir="$extras_dir/$docs_directory_name"

###############################
# Advanced settings CAREFUL!! #
###############################

# Define compiler options - careful here. Know what you're doing.
dbg_backend_args=""
libssh2_args=""
libssh2_opt_flag=" -I$openssl_path/include"
libssh2_ld_flag=" -L$openssl_path/lib"
libarchive_args="--disable-shared --with-pic --without-expat --without-xml2 --without-openssl --without-nettle --without-lzo2 --without-lzma --without-libiconv-prefix --without-iconv --without-lzmadec --without-bz2lib --without-zlib --disable-bsdtar --disable-bsdcpio --disable-rpath"
mrnet_args=""
python_args=""
gdb_args="--disable-rpath --without-separate-debug-dir --without-gdb-datadir --with-mmalloc --with-python=$stage_dir/bin/python"
cuda_gdb_args="--program-prefix=cuda- --enable-cuda --enable-targets=x86_64-apple-darwin,x86_64-unknown-linux-gnu,arm-elf-linux-gnu,m68k-unknown-linux-gnu --disable-rpath --without-separate-debug-dir --without-gdb-datadir --with-python=$stage_dir/bin/python"
perf_flag=""
be_perf_flag=""
valgrind_flag=""
dbg_flag=""
opt_flag="-g -O2 -Wall"

# Flags used later on
opt_ask_deps=n
opt_aif=n
opt_libmi=n
opt_libssh2=n
opt_mrnet=n
opt_python=n
opt_gdb=n
opt_cuda55_gdb=n
opt_cuda65_gdb=n
opt_gdb_uda=n
opt_editline=n
opt_libarchive=n
opt_interface=n
opt_lgdb=n
opt_cleanup=n
opt_advanced=n
prompt_user=1

function print_help {
echo "Usage: `basename $0` options (-ABCDEFGHIJKLMN | --all)"
echo "A: libaif"
echo "B: libmi"
echo "C: libssh2"
echo "D: MRNet"
echo "E: Python"
echo "F: gdb"
echo "G: cuda-gdb for CUDA_50"
echo "H: cuda-gdb for CUDA_55"
echo "I: gdb uda support"
echo "J: editline"
echo "K: libarchive"
echo "L: Tools Interface"
echo "M: lgdb"
echo "N: cleanup"
}


# Used to keep track of warnings and errors
((error=0))
((warning=0))
((comp_error=0))

##############################################
# Setup the build environment based on above #
##############################################

# Display welcome message
echo -e "\nlgdb build tool v$lgdb_build_version - By andrewg@cray.com"

# Remove the install directory tree if it already exists
if [ -d $inst_dir ]; then
	rm -rf $inst_dir
fi

# Create bindir, libdir, and libexec variables
bindir="$inst_dir/bin"
libdir="$inst_dir/lib"
libexecdir="$inst_dir/libexec"

# Create the install directory tree
mkdir $inst_dir
mkdir $bindir
mkdir $libdir
mkdir $libexecdir

# Create the stage directory if it doesn't already exist
if [[ ! -d $stage_dir ]]; then
	mkdir $stage_dir
	mkdir $stage_dir/bin
	mkdir $stage_dir/include
	mkdir $stage_dir/lib
else
	if [[ ! -d $stage_dir/bin ]]; then
		mkdir $stage_dir/bin
	fi
	if [[ ! -d $stage_dir/include ]]; then
		mkdir $stage_dir/include
	fi
	if [[ ! -d $stage_dir/lib ]]; then
		mkdir $stage_dir/lib
	fi
fi

# Create the external directory if it doesn't already exist
if [[ ! -d $external_dir ]]; then
	mkdir $external_dir
fi

# Create dep_bindir, dep_includedir, and dep_libdir variables
dep_bindir="$stage_dir/bin"
dep_sbindir="$stage_dir/sbin"
dep_includedir="$stage_dir/include"
dep_libexecdir="$stage_dir/libexec"
dep_libdir="$stage_dir/lib"
dep_lib64dir="$stage_dir/lib64"

##############################################
# Ensure that a PrgEnv module is not loaded. #
##############################################

if [ -e /opt/modules/default/init/bash ]; then 
	source /opt/modules/default/init/bash
fi

if test "x`module list -l 2>&1 > /dev/null | grep 'PrgEnv'`" != "x"; then
	echo "!!!!!!!!!!WARNING!!!!!!!!!!"
	echo "A PrgEnv module has been detected as being loaded in the current"
	echo "module list. Running this script with a PrgEnv-xxx module loaded"
	echo "can result in undefined behavior."
	echo "!!!!!!!!!!WARNING!!!!!!!!!!"
	echo -n "Are you sure you want to continue?[c/q]: "
	while true; do
		read opt_undef
		if [[ $opt_undef == c || $opt_undef == q ]]; then
			break
		fi
		echo -ne "\tEnter c or q: "
	done
	if [[ $opt_undef == q ]]; then
		exit
	else
		echo -e "Continuing. This mode is not supported!\n"
	fi
fi

##################################
# Start asking for user options. #
##################################

if [ "$#" != "0" ] ; then
	#getops doesn't natively support long opts
	prompt_user=0
	if [ "$1" == "--help" ] ; then
		print_help
		exit 0
	fi
	if [ "$1" == "--all" ] ; then
		opt_aif=y
		opt_libmi=y
		opt_libssh2=y
		opt_mrnet=y
		opt_python=y
		opt_gdb=y
		opt_cuda55_gdb=y
		opt_cuda65_gdb=y
		opt_gdb_uda=y
		opt_editline=y
		opt_libarchive=y
		opt_interface=y
		opt_lgdb=y
		opt_cleanup=y
		opt_advanced=n
		#for release builds, we don't want to build with -g
		opt_flag="-O2 -Wall"
 	else
		while getopts "aAbBcCdDeEfFgGhHiIjJkKlLmM" Option
		do
			case $Option in
				a | A ) echo "Building A: libaif"
					opt_aif=y;;
				b | B ) echo "Building B: libmi"
					opt_libmi=y;;
				c | C ) echo "Building C: libssh2"
					opt_libssh2=y;;
				d | D ) echo "Building D: MRNet"
					opt_mrnet=y;;
				e | E ) echo "Building E: Python"
					opt_python=y;;
				f | F ) echo "Building E: gdb"
					opt_gdb=y;;
				g | G ) echo "Building F: cuda-gdb for CUDA_55"
					opt_cuda55_gdb=y;;
				h | H ) echo "Building G: cuda-gdb for CUDA_65"
					opt_cuda65_gdb=y;;
				i | I ) echo "Building H: gdb uda support"
					opt_gdb_uda=y;;
				j | J ) echo "Building I: editline"
					opt_editline=y;;
				k | K ) echo "Building J: libarchive"
					opt_libarchive=y;;
				l | L ) echo "Building K: tools interface"
					opt_interface=y;;
				m | M )	echo "Building L: lgdb"
					opt_lgdb=y;;
				n | N ) echo "Running M: cleanup"
					opt_cleanup=y;;
				? ) 	print_help
					exit 0;;
			esac
		done
		shift $(($OPTIND - 1))
	fi
fi
echo -e "Please choose from the following options.\n"

# Ask if we should ask to build dependencies
echo -n "A. Specify dependency builds?[y/n]: "
if [ $prompt_user -eq 1 ] ; then
	while true; do
		read opt_ask_deps
		if [[ $opt_ask_deps == y || $opt_ask_deps == n ]]; then
			break
		fi
		echo -ne "\tEnter y or n: "
	done
else
	echo "$opt_ask_deps"
fi

# Ask if we should build libaif.
echo -n "A1. Build libaif?[y/n]: "
if [[ $prompt_user == 1 && $opt_ask_deps == y ]] ; then
	while true; do
		read opt_aif
		if [[ $opt_aif == y || $opt_aif == n ]]; then
			break
		fi
		echo -ne "\tEnter y or n: "
	done
else
	echo "$opt_aif "
fi

# Process the libaif build option.
if [[ $opt_aif == n ]]; then
	# Check to see if libaif exists in the default staging directory
	if [ -e $dep_includedir/aif.h ] && [ -e $dep_libdir/libaif.a ]; then
		echo -e "\tUsing libaif in $stage_dir."
		path_aif=$stage_dir
	else
		# warn the user that we couldn't find libaif install in the stage dir
		echo -e "\tWARNING: libaif install not found in $stage_dir."
		if [ $prompt_user -eq 1 ] ; then
			echo -ne "\tAre you sure you want to continue?[c/q]: "
			while true; do
				read opt_undef
				if [[ $opt_undef == c || $opt_undef == q ]]; then
					break
				fi
				echo -ne "\tEnter c or q: "
			done
			if [[ $opt_undef == q ]]; then
				exit
			else
				echo -e "\tContinuing. Your build may fail!\n"
			fi
		fi
	fi
fi

# Ask if we should build libmi
echo -n "A2. Build libmi?[y/n]: "
if [[ $prompt_user == 1 && $opt_ask_deps == y ]] ; then
	while true; do
		read opt_libmi
		if [[ $opt_libmi == y || $opt_libmi == n ]]; then
			break
		fi
		echo -ne "\tEnter y or n: "
	done
else
	echo "$opt_libmi "
fi

# Process the libmi build option. If not, ensure that it exists in the system so that we can
# continue building
if [[ $opt_libmi == n ]]; then
	# Check to see if libmi exists in the default staging directory
	if [ -e $dep_libdir/libmi.so ] && [ -e $dep_includedir/MI.h ]; then
		echo -e "\tUsing libmi in $stage_dir."
		dbg_backend_args=$stage_dir
	else
		# warn the user that we couldn't find libaif install in the stage dir
		echo -e "\tWARNING: libmi install not found in $stage_dir."
		if [ $prompt_user -eq 1 ] ; then
			echo -ne "\tAre you sure you want to continue?[c/q]: "
			while true; do
				read opt_undef
				if [[ $opt_undef == c || $opt_undef == q ]]; then
					break
				fi
				echo -ne "\tEnter c or q: "
			done
			if [[ $opt_undef == q ]]; then
				exit
			else
				echo -e "\tContinuing. Your build may fail!\n"
			fi
		fi
	fi
fi

# Ask if we should build libssh2
echo -n "A3. Build libssh2?[y/n]: "
if [[ $prompt_user == 1 && $opt_ask_deps == y ]] ; then
	while true; do
		read opt_libssh2
		if [[ $opt_libssh2 == y || $opt_libssh2 == n ]]; then
			break
		fi
		echo -ne "\tEnter y or n: "
	done
else
	echo "$opt_libssh2 "
fi

# Process the libssh2 build option. If not, ensure that it exists in the system so that we can
# continue building
if [[ $opt_libssh2 == n ]]; then
	# Check to see if libssh2 exists in the default staging directory
	if [ -e $dep_libdir/libssh2.so ] && [ -e $dep_includedir/libssh2.h ]; then
		echo -e "\tUsing libssh2 in $stage_dir."
		libssh2_args=$stage_dir
	else
		# warn the user that we couldn't find libssh2 install in the stage dir
		echo -e "\tWARNING: libssh2 install not found in $stage_dir."
		if [ $prompt_user -eq 1 ] ; then
			echo -ne "\tAre you sure you want to continue?[c/q]: "
			while true; do
				read opt_undef
				if [[ $opt_undef == c || $opt_undef == q ]]; then
					break
				fi
				echo -ne "\tEnter c or q: "
			done
			if [[ $opt_undef == q ]]; then
				exit
			else
				echo -e "\tContinuing. Your build may fail!\n"
			fi
		fi
	fi
fi

# Ask if we should build MRNet
echo -n "A4. Build MRNet?[y/n]: "
if [[ $prompt_user == 1 && $opt_ask_deps == y ]] ; then
	while true; do
		read opt_mrnet
		if [[ $opt_mrnet == y || $opt_mrnet == n ]]; then
			break
		fi
		echo -ne "\tEnter y or n: "
	done
else
	echo "$opt_mrnet "
fi

# Process the MRNet build option.
if [[ $opt_mrnet == n ]]; then
	# Check to see if MRNet exists in the default staging directory
	if [ -e $dep_bindir/mrnet_commnode ]; then
		echo -e "\tUsing mrnet in $stage_dir."
	else
		# warn the user that we couldn't find MRNet install in the stage dir
		echo -e "\tWARNING: MRNet install not found in $stage_dir."
		if [ $prompt_user -eq 1 ] ; then
			echo -ne "\tAre you sure you want to continue?[c/q]: "
			while true; do
				read opt_undef
				if [[ $opt_undef == c || $opt_undef == q ]]; then
					break
				fi
				echo -ne "\tEnter c or q: "
			done
			if [[ $opt_undef == q ]]; then
				exit
			else
				echo -e "\tContinuing. Your build may fail!\n"
			fi
		fi
	fi
	
	# Check to see if the commnode wrapper exists in the extras dir
	if [ -e $thirdparty_dir/lgdb_mrnet_commnode_wrapper ]; then
		echo -e "\tUsing lgdb_mrnet_commnode_wrapper in $thirdparty_dir."
	else
		# warn the user that we couldn't find MRNet commnode wrapper in the extras dir
		echo -e "\tWARNING: lgdb_mrnet_commnode_wrapper not found in $thirdparty_dir."
		if [ $prompt_user -eq 1 ] ; then
			echo -ne "\tAre you sure you want to continue?[c/q]: "
			while true; do
				read opt_undef
				if [[ $opt_undef == c || $opt_undef == q ]]; then
					break
				fi
				echo -ne "\tEnter c or q: "
			done
			if [[ $opt_undef == q ]]; then
				exit
			else
				echo -e "\tContinuing. Your build may fail!\n"
			fi
		fi
	fi
fi

# Ask if we should build python.
echo -n "A5. Build python?[y/n]: "
if [[ $prompt_user == 1 && $opt_ask_deps == y ]] ; then
	while true; do
		read opt_python
		if [[ $opt_python == y || $opt_python == n ]]; then
			break
		fi
		echo -ne "\tEnter y or n: "
	done
else
	echo "$opt_python "
fi

# Process the python build option. If we do not build python, ensure that it
# exists so that we can continue building.
if [[ $opt_python == n ]]; then
	# Check to see if python exists in the dep_bindir
	if [ -e $dep_bindir/python ]; then
		echo -e "\tUsing python in $dep_bindir."
	else
		echo -e "\tWARNING: Could not find python in $dep_bindir."
		if [ $prompt_user -eq 1 ]; then
			echo -ne "\tAre you sure you want to continue?[c/q]: "
			while true; do
				read opt_undef
				if [[ $opt_undef == c || $opt_undef == q ]]; then
					break
				fi
				echo -ne "\tEnter c or q: "
			done
			if [[ $opt_undef == q ]]; then
				exit
			else
				echo -e "\tContinuing. Your build may fail!\n"
			fi
		fi
	fi
fi

# Ask if we should build gdb.
echo -n "A6. Build gdb?[y/n]: "
if [[ $prompt_user == 1 && $opt_ask_deps == y ]] ; then
	while true; do
		read opt_gdb
		if [[ $opt_gdb == y || $opt_gdb == n ]]; then
			break
		fi
		echo -ne "\tEnter y or n: "
	done
else
	echo "$opt_gdb "
fi

# Process the gdb build option. If we do not build gdb, ensure that it exists so
# that we can continue building.
if [[ $opt_gdb == n ]]; then
	# Check to see if gdb-guard exists in the dep_bindir
	if [ -e $dep_bindir/gdb-guard ]; then
		echo -e "\tUsing gdb-guard in $dep_bindir."
	else
		echo -e "\tWARNING: Could not find gdb-guard in $dep_bindir."
		if [ $prompt_user -eq 1 ] ; then 
			echo -ne "\tAre you sure you want to continue?[c/q]: " 
			while true; do 
				read opt_undef
				if [[ $opt_undef == c || $opt_undef == q ]]; then
					break
				fi
				echo -ne "\tEnter c or q: "
			done
			if [[ $opt_undef == q ]]; then
				exit
			else
				echo -e "\tContinuing. Your build may fail!\n"
			fi
		fi
	fi
fi

# Ask if we should build cuda-gdb for CUDA_55
echo -n "A7. Build cuda-gdb for CUDA_55?[y/n]: "
if [[ $prompt_user == 1 && $opt_ask_deps == y ]] ; then
	while true; do
		read opt_cuda55_gdb
		if [[ $opt_cuda55_gdb == y || $opt_cuda55_gdb == n ]]; then
			break;
		fi
		echo -ne "\tEnter y or n: "
	done
else
	echo "$opt_cuda55_gdb"
fi

# Process the cuda-gdb for CUDA_55 build option. If we do not build gdb, ensure that it
# exists so that we can continue building.
if [[ $opt_cuda55_gdb == n ]]; then
	# Check to see if cuda55-gdb-guard exists in the dep_bindir
	if [ -e $dep_bindir/cuda55-gdb-guard ]; then
		echo -e "\tUsing cuda55-gdb-guard in $dep_bindir."
	else
		echo -e "\tWARNING: Could not find cuda55-gdb-guard in $dep_bindir."
		if [ $prompt_user -eq 1 ]; then
			echo -ne "\tAre you sure you want to continue?[c/q]: "
			while true; do
				read opt_undef
				if [[ $opt_undef == c || $opt_undef == q ]]; then
					break
				fi
				echo -ne "\tEnter c or q: "
			done
			if [[ $opt_undef == q ]]; then
				exit
			else
				echo -e "\tContinuing. Your build may fail!\n"
			fi
		fi
	fi
fi

# Ask if we should build cuda-gdb for CUDA_65
echo -n "A8. Build cuda-gdb for CUDA_65?[y/n]: " 
if [[ $prompt_user == 1 && $opt_ask_deps == y ]] ; then 
	while true; do 
		read opt_cuda65_gdb 
		if [[ $opt_cuda65_gdb == y || $opt_cuda65_gdb == n ]]; then 
			break 
		fi 
		echo -ne "\tEnter y or n: " 
	done 
else 
	echo "$opt_cuda65_gdb " 
fi

# Process the cuda-gdb for CUDA_65 build option. If we do not build gdb, ensure that it
# exists so that we can continue building.
if [[ $opt_cuda65_gdb == n ]]; then
	# Check to see if cuda65-gdb-guard exists in the dep_bindir
	if [ -e $dep_bindir/cuda65-gdb-guard ]; then
		echo -e "\tUsing cuda65-gdb-guard in $dep_bindir."
	else
		echo -e "\tWARNING: Could not find cuda65-gdb-guard in $dep_bindir."
		if [ $prompt_user -eq 1 ]; then
			echo -ne "\tAre you sure you want to continue?[c/q]: "
			while true; do
				read opt_undef
				if [[ $opt_undef == c || $opt_undef == q ]]; then
					break
				fi
				echo -ne "\tEnter c or q: "
			done
			if [[ $opt_undef == q ]]; then
				exit
			else
				echo -e "\tContinuing. Your build may fail!\n"
			fi
		fi
	fi
fi

# Ask if we should build the gdb uda support library
echo -n "A9. Build gdb uda support?[y/n]: "
if [[ $prompt_user == 1 && $opt_ask_deps == y ]] ; then
	while true; do
		read opt_gdb_uda
		if [[ $opt_gdb_uda == y || $opt_gdb_uda == n ]]; then
			break
		fi
		echo -ne "\tEnter y or n: "
	done
else
	echo "$opt_gdb_uda"
fi

# Process the gdb uda support build option.
if [[ $opt_gdb_uda == n ]]; then
	# Check to see if gdb uda plugin library exists in dep_libdir
	if [ -e $dep_libdir/uda-plugin-cray-upc.so ]; then
		echo -e "\tUsing uda-plugin-cray-upc.so in $dep_libdir."
	else
		# uda plugin not found
		echo -e "\tWARNING: Could not find gdb uda plugin in $dep_libdir."
		if [ $prompt_user -eq 1 ] ; then
			echo -ne "\tAre you sure you want to continue?[c/q]: "
			while true; do
				read opt_undef
				if [[ $opt_undef == c || $opt_undef == q ]]; then
					break
				fi
				echo -ne "\tEnter c or q: "
			done
			if [[ $opt_undef == q ]]; then
				exit
			else
				echo -e "\tContinuing. Your build may fail!\n"
			fi
		fi
	fi
fi

# Ask if we should build editline.
echo -n "A10. Build editline?[y/n]: "
if [[ $prompt_user == 1 && $opt_ask_deps == y ]] ; then
	while true; do
		read opt_editline
		if [[ $opt_editline == y || $opt_editline == n ]]; then
			break
		fi
		echo -ne "\tEnter y or n: "
	done
else
	echo "$opt_editline "
fi

# Process the editline build option.
if [[ $opt_editline == n ]]; then
	# Check to see if libedit exists in the dep_libdir
	if [ -e $dep_libdir/$libedit_dso ]; then
		# Check to see if the libedit minor exists in the dep_libdir
		if [ -e $dep_libdir/$libedit_minor ]; then
			echo -e "\tUsing libedit in $dep_bindir."
		else
			# libedit_dso found, but libedit_minor not found
			echo -e "\tERROR: Found $libedit_dso, but could not find $libedit_minor in $dep_libdir."
			echo -e "\tThe version information in this script for libedit is probably out of date"
			echo -e "\tor the library binary is missing from $dep_libdir."
			echo -e "\tDefaulting to build editline library..."
			opt_editline="y"
		fi
	else
		echo -e "\tWARNING: $libedit_dso not found in $dep_libdir."
		if [ $prompt_user -eq 1 ] ; then
			echo -ne "\tAre you sure you want to continue?[c/q]: "
			while true; do
				read opt_undef
				if [[ $opt_undef == c || $opt_undef == q ]]; then
					break
				fi
				echo -ne "\tEnter c or q: "
			done
			if [[ $opt_undef == q ]]; then
				exit
			else
				echo -e "\tContinuing. Your build may fail!\n"
			fi
		fi
	fi
fi

# Ask if we should build libarchive
echo -n "A11. Build libarchive?[y/n]: "
if [[ $prompt_user == 1 && $opt_ask_deps == y ]] ; then
	while true; do
		read opt_libarchive
		if [[ $opt_libarchive == y || $opt_libarchive == n ]]; then
			break
		fi
		echo -ne "\tEnter y or n: "
	done
else
	echo "$opt_libarchive "
fi

# Process the libarchive build option.
if [[ $opt_libarchive == n ]]; then
	# Check to see if libarchive exists in the default staging directory
	if [ -e $dep_includedir/archive.h ] && [ -e $dep_libdir/libarchive.a ]; then
		echo -e "\tUsing libarchive in $stage_dir."
	else
		# warn the user that we couldn't find libarchive install in the stage dir
		echo -e "\tWARNING: libarchive not found in $stage_dir."
		if [ $prompt_user -eq 1 ] ; then
			echo -ne "\tAre you sure you want to continue?[c/q]: "
			while true; do
				read opt_undef
				if [[ $opt_undef == c || $opt_undef == q ]]; then
					break
				fi
				echo -ne "\tEnter c or q: "
			done
			if [[ $opt_undef == q ]]; then
				exit
			else
				echo -e "\tContinuing. Your build may fail!\n"
			fi
		fi
	fi
fi

# Ask if we should build the cray tools interface
echo -n "A12. Build tools interface?[y/n]: "
if [[ $prompt_user == 1 && $opt_ask_deps == y ]] ; then
	while true; do
		read opt_interface
		if [[ $opt_interface == y || $opt_interface == n ]]; then
			break
		fi
		echo -ne "\tEnter y or n: "
	done
else
	echo "$opt_interface "
fi

# Process the tools interface build option.
if [[ $opt_interface == n ]]; then
	# Check to see if libcraytool_backend exists in the dep_libdir
	if [ -e $dep_libdir/$libcraytool_backend_dso ]; then
		# Check to see if the libcraytool_backend minor exists in the dep_libdir
		if [ -e $dep_libdir/$libcraytool_backend_minor ]; then
			echo -e "\tUsing libcraytool_backend in $dep_libdir."
		else
			# libcraytool_backend_dso found, but libcraytool_backend_minor not found
			echo -e "\tERROR: Found $libcraytool_backend_dso, but could not find $libcraytool_backend_minor in $dep_libdir."
			echo -e "\tThe version information in this script for libcraytool_backend is probably out of date"
			echo -e "\tor the library binary is missing from $dep_libdir."
			echo -e "\tDefaulting to build craytool library..."
			opt_interface="y"
		fi
	else
		echo -e "\tWARNING: $libcraytool_backend_dso not found in $dep_libdir."
		if [ $prompt_user -eq 1 ] ; then
			echo -ne "\tAre you sure you want to continue?[c/q]: "
			while true; do
				read opt_undef 
				if [[ $opt_undef == c || $opt_undef == q ]]; then 
					break 
				fi 
				echo -ne "\tEnter c or q: " 
			done 
			if [[ $opt_undef == q ]]; then 
				exit 
			else 
				echo -e "\tContinuing. Your build may fail!\n" 
			fi
		fi
	fi
	# Check to see if libcraytool_frontend exists in the dep_libdir 
	if [ -e $dep_libdir/$libcraytool_frontend_dso ]; then 
		# Check to see if the libcraytool_frontend minor exists in the dep_libdir 
		if [ -e $dep_libdir/$libcraytool_frontend_minor ]; then 
			echo -e "\tUsing libcraytool_frontend in $dep_libdir." 
		else 
			# libcraytool_frontend_dso found, but libcraytool_frontend_minor not found 
			echo -e "\tERROR: Found $libcraytool_frontend_dso, but could not find $libcraytool_frontend_minor in $dep_libdir." 
			echo -e "\tThe version information in this script for libcraytool_frontend is probably out of date" 
			echo -e "\tor the library binary is missing from $dep_libdir." 
			echo -e "\tDefaulting to build craytool library..." 
			opt_interface="y" 
		fi 
	else
		echo -e "\tWARNING: $libcraytool_frontend_dso not found in $dep_libdir." 
		if [ $prompt_user -eq 1 ] ; then 
			echo -ne "\tAre you sure you want to continue?[c/q]: " 
			while true; do 
				read opt_undef 
				if [[ $opt_undef == c || $opt_undef == q ]]; then 
					break 
				fi 
				echo -ne "\tEnter c or q: " 
			done 
			if [[ $opt_undef == q ]]; then 
				exit 
			else 
				echo -e "\tContinuing. Your build may fail!\n" 
			fi 
		fi 
	fi 
fi

# Ask if we should build lgdb
echo -n "B. Build lgdb?[y/n]: "
if [ $prompt_user -eq 1 ] ; then
	while true; do
		read opt_lgdb
		if [[ $opt_lgdb == y || $opt_lgdb == n ]]; then
			break
		fi
		echo -ne "\tEnter y or n: "
	done
else
	echo "$opt_lgdb "
fi

# Ask if we should cleanup after build.
echo -n "C. Cleanup build?[y/n]: "
if [ $prompt_user -eq 1 ] ; then
	while true; do
		read opt_cleanup
		if [[ $opt_cleanup == y || $opt_cleanup == n ]]; then
			break
		fi
		echo -ne "\tEnter y or n: "
	done
else
	echo "$opt_cleanup "
fi


# Ask if the user wants to use advanced mode
echo -n "D. Advanced mode?[y/n]: "
if [ $prompt_user -eq 1 ] ; then
	while true; do
		read opt_advanced
		if [[ $opt_advanced == y || $opt_advanced == n ]]; then
			break
		fi
		echo -ne "\tEnter y or n: "
	done
else
	echo "$opt_cleanup "
fi

###############################################################
# Process opt_advanced option. Advanced mode contains several #
# sub-options to choose from. Determine these options first   #
###############################################################

if [[ $opt_advanced == y ]]; then
	echo -n "D1. lgdb debug mode?[y/n]: "
	while true; do
		read opt_debug
		if [[ $opt_debug == y || $opt_debug == n ]]; then
			break
		fi
		echo -ne "\tEnter y or n: "
	done
	
	# only allow optimization level if not using debug mode.
	if [[ $opt_debug == n ]]; then
		echo -n "D2. Specify optimization level?[y/n]: "
		while true; do
			read opt_optimization
			if [[ $opt_optimization == y || $opt_optimization == n ]]; then
				break
			fi
			echo -ne "\tEnter y or n: "
		done
	fi
	
	echo -n "D3. lgdb perf log mode?[y/n]: "
	while true; do
		read opt_perflog
		if [[ $opt_perflog == y || $opt_perflog == n ]]; then
			break
		fi
		echo -ne "\tEnter y or n: "
	done

	# only ask for backend perf mode if using perf log mode.
	if [[ $opt_perflog == y ]]; then
		echo -n "D4. extended backend perf log mode (gathers extra data to lustre files)?[y/n]: "
		while true; do
			read opt_beperflog
			if [[ $opt_beperflog == y || $opt_beperflog == n ]]; then
				break
			fi
			echo -ne "\tEnter y or n: "
		done
	fi

	echo -n "D5. lgdb backend valgrind mode?[y/n]: "
	while true; do
		read opt_valgrind
		if [[ $opt_valgrind == y || $opt_valgrind == n ]]; then
			break
		fi
		echo -ne "\tEnter y or n: "
	done

	# Now process the sub-mode selections

	# Process debug mode. This turns on debug printing options inside of lgdb. 
	if [[ $opt_debug == y ]]; then
		# This is the debug option for the call to configure
		dbg_flag="--with-debug"
		opt_flag="-g -O0 -Wall"
		mrnet_args="--enable-debug"
	fi

	# Process optimization setting. This allows the user to specify custom optimization levels.
	if [[ $opt_optimization == y ]]; then
		# only allow this if debug mode is not on
		if [[ $opt_debug == n ]]; then
			echo -ne "\tSpecify optimization level (i.e. 0,1,2,3): "
			while true; do
				read opt_lvl
				if [[ $opt_lvl == 0 || $opt_lvl == 1 || $opt_lvl == 2 || $opt_lvl == 3 ]]; then
					break
				fi
				echo -ne "\tEnter 0, 1, 2, or 3: "
			done
			# Setup the cflag option for the call to configure
			opt_flag="-g -O$opt_lvl -Wall"
		else
			echo -ne "\tOptimization specification is not allowed with debug mode.\n"
		fi
	fi

	# Process perflog setting. This allows the user to turn on performance logging.
	if [[ $opt_perflog == y ]]; then
		# This is the perf log option for the call to configure
		perf_flag="--with-perf"
		# we only check for backend perflog mode if the user turned on perflog
		if [[ $opt_beperflog == y ]]; then
			# This is the backend perf log option for the call to configure
			be_perf_flag="--with-backend-perf"
		fi
	fi

	# Process valgrind setting. This allows the user to turn on backend valgrind usage.
	if [[ $opt_valgrind == y ]]; then
		# This is the valgrind option for the call to configure
		valgrind_flag="--with-valgrind"
	fi
fi

########################################################
# Done processing options. Prepare to start the build. #
########################################################

# go back to the base directory incase something above cd'ed us somewhere else.
cd $cur_dir

echo -e "\n################# Done parsing input ###################\n"

echo	"Using the following build/inst directories:"
echo -e	"\tInstall directory: $inst_dir"
echo -e	"\tStaging directory: $stage_dir"
echo -e	"\tThirdParty directory: $thirdparty_dir"
echo -e	"\tExtras directory: $extras_dir"

echo -e	"\n########################################################"
echo	"Starting build process..."
echo -e	"########################################################\n"

######################################################
# Start building lgdb, beginning  with dependencies. #
######################################################

# reset comp_error
((comp_error=0))

# libaif build
if [[ $opt_aif == y ]]; then
	echo -e "Starting AIF build.\n"
	# change directories
	cd $aif_src
	echo -e "Using AIF source in $aif_src.\n"
	# run aif configure
	echo "Configuring libaif..."
	echo -e "./configure --prefix=$stage_dir CFLAGS=$opt_flag\n"
	./configure --prefix=$stage_dir CFLAGS="$opt_flag"
	# run make
	echo -e "\nBuilding libaif...\n"
	make --silent
	# run make install
	echo -e "Installing libaif...\n"
	make --silent install
	echo -e "\nAIF installed to $stage_dir.\n"
	# check to see if libaif.a was created
	if [ ! -e $dep_libdir/libaif.a ]; then
		echo "ERROR: libaif.a not found in $dep_libdir. The AIF build failed."
		((++comp_error))
	fi
	# update path_aif to use our freshly build copy
	path_aif=$stage_dir
	# return home
	cd $cur_dir
	echo -e "########################################################\n"
fi

# check for errors in component
if [ "$comp_error" -ne "0" ]; then
	((++error))
fi

# reset comp_error
((comp_error=0))

# libmi build
if [[ $opt_libmi == y ]]; then
	echo -e "Starting libmi build.\n"
	# change directories
	cd $libmi_src
	echo -e "Using libmi source in $libmi_src.\n"
	# run libmi configure
	echo "Configuring libmi..."
	echo -e "./configure --prefix=$stage_dir CFLAGS=$opt_flag\n"
	./configure --prefix=$stage_dir CFLAGS="$opt_flag"
	# run make
	echo -e "\nBuilding libmi...\n"
	make --silent
	# run make install
	echo -e "Installing libmi...\n"
	make --silent install
	echo -e "\nlibmi installed to $stage_dir.\n"
	# deliver libmi.so 
	if [ -e $dep_libdir/libmi.so ]; then 
		echo "Delivering libmi.so to $libdir..." 
		cp $dep_libdir/libmi.so $libdir 
	else 
		echo "ERROR: libmi.so not found in $dep_libdir. The libmi build failed."
		((++comp_error)) 
	fi
	# update dbg_backend_args to use our freshly built copy
	dbg_backend_args=$stage_dir
	# return home
	cd $cur_dir
	echo -e "########################################################\n"
else 
	# We still need to deliver the libmi libraries for use in the final lgdb package 
	echo -e "Starting libmi install.\n"
	# deliver libmi.so
	if [ -e $dep_libdir/libmi.so ]; then
		echo "Delivering libmi.so to $libdir..."
		cp $dep_libdir/libmi.so $libdir
	else
		echo "ERROR: Could not find libmi.so in $dep_libdir. Manually add this to $inst_dir."
		((++comp_error))
	fi
	# return home
	cd $cur_dir
	echo -e "########################################################\n"
fi

# check for errors in component
if [ "$comp_error" -ne "0" ]; then
	((++error))
fi

# reset comp_error
((comp_error=0))

# libssh2 build
if [[ $opt_libssh2 == y ]]; then
	echo -e "Starting libssh2 build.\n"
	# change directories
	cd $libssh2_src
	echo -e "Using libssh2 source in $libssh2_src.\n"
	# run libssh2 configure
	echo "Configuring libssh2..."
	echo -e "./configure --prefix=$stage_dir CFLAGS=\"$opt_flag$libssh2_opt_flag\" LDFLAGS=\"$libssh2_ld_flag\"\n"
	./configure --prefix=$stage_dir CFLAGS="$opt_flag$libssh2_opt_flag" LDFLAGS="$libssh2_ld_flag"
	# run make
	echo -e "\nBuilding libssh2...\n"
	make --silent
	# run make install
	echo -e "Installing libssh2...\n"
	make --silent install
	echo -e "\nlibssh2 installed to $stage_dir.\n"
	# deliver libssh2.so
	if [ -e $dep_libdir/$libssh2_minor ]; then
		echo "Delivering $libssh2_minor to $libdir..."
		cp $dep_libdir/$libssh2_minor $libdir
		echo "Creating libssh2 symlinks for $libssh2_dso and $libssh2_major..."
		# cd to libdir to create the symlinks
		cd $libdir
		ln -s $libssh2_minor $libssh2_dso
		ln -s $libssh2_minor $libssh2_major
	else
		echo "ERROR: Could not find $libssh2_minor in $dep_libdir. The libssh2 build failed."
		((++comp_error))
	fi
	# update libssh2_args to use our freshly built copy
	libssh2_args=$stage_dir
	# return home
	cd $cur_dir
	echo -e "########################################################\n"
else
	# We still need to deliver the libssh2 libraries for use in the final lgdb package
	echo -e "Starting libssh2 install.\n"
	# deliver libssh2.so
	if [ -e $dep_libdir/$libssh2_minor ]; then
		echo "Delivering $libssh2_minor to $libdir..."
		cp $dep_libdir/$libssh2_minor $libdir
		echo "Creating libssh2 symlinks for $libssh2_dso and $libssh2_major..."
		# cd to libdir to create the symlinks
		cd $libdir
		ln -s $libssh2_minor $libssh2_dso
		ln -s $libssh2_minor $libssh2_major
	else
		echo "ERROR: Could not find $libssh2_minor in $dep_libdir. Manually add this to $inst_dir."
		((++comp_error))
	fi
	# return home
	cd $cur_dir
	echo -e "########################################################\n"
fi

# check for errors in component
if [ "$comp_error" -ne "0" ]; then
	((++error))
fi

# reset comp_error
((comp_error=0))

function install_python {
	# Ensure the dependency modules list exists
	if [[ ! -e $python_depmod ]]; then
		echo "ERROR: Could not find python dependency list at $python_depmod. The python install failed."
		((++comp_error))
		return
	fi

	# create the python install directory
	if [[ ! -d $libdir/$python_inst_name ]]; then
		mkdir $libdir/$python_inst_name
	fi

	while read py_mod; do
		# Ensure the file exists in the dep_libdir
		if [[ -e $dep_libdir/$python_inst_name/$py_mod ]]; then
			echo "Delivering $py_mod to $libdir/$python_inst_name/$py_mod."
			cp -r $dep_libdir/$python_inst_name/$py_mod $libdir/$python_inst_name/$py_mod
			# If this is a directory, we need to ensure .pyc and .pyo files do not get installed
			if [[ -d $libdir/$python_inst_name/$py_mod ]]; then
				rm -f $libdir/$python_inst_name/$py_mod/*.pyc
				rm -f $libdir/$python_inst_name/$py_mod/*.pyo
			fi
		else
			echo "ERROR: Could not find $py_mod in $dep_libdir/$python_inst_name. The python install failed."
			((++comp_error))
		fi
	done <$python_depmod

	# copy python pretty printers for gdb
	if [[ ! -d $python_gdb ]]; then
		echo "ERROR: Could not find the python pretty printers at $python_gdb. The python install failed."
		((++comp_error))
		return
	fi
	echo "Delivering $python_gdb_name to $libdir/$python_gdb_name."
	cp -r $python_gdb $libdir/$python_gdb_name
}

# python build
if [[ $opt_python == y ]]; then
	echo "Starting python build."
	# change directories
	cd $python_src
	echo -e "\nUsing python source $python_src.\n"
	# run python configure
	echo -e "Configuring python..."
	echo -e "./configure --prefix=$stage_dir $python_args\n"
	# This is a hack to get python to build. I found it in a python bug report.
	# It seems to work...
	export SVNVERSION="Unversioned directory"
	./configure --prefix=$stage_dir "$python_args"
	# run python make
	echo -e "\nBuilding python...\n"
	make --silent
	# run python make install
	echo -e "\nInstalling python...\n"
	make --silent install
	echo -e "\npython installed to $stage_dir.\n"
	echo -e "Starting post build ops.\n"
	# Deliver the necessary modules to the python install location
	install_python
	# return home
	cd $cur_dir
	echo -e "########################################################\n"
else
	# We still need the python modules for use in the final lgdb package
	echo -e "Starting python install.\n"
	# Always use the python modules in the dep_libdir
	install_python
	echo -e "########################################################\n"
fi

# check for errors in component
if [ "$comp_error" -ne "0" ]; then
	((++error))
fi

# reset comp_error
((comp_error=0))

# gdb build
if [[ $opt_gdb == y ]]; then
	echo "Starting gdb build."
	# change directories
	cd $gdb_src
	echo -e "\nUsing gdb source $gdb_src.\n"
	# run gdb configure
	echo "Configuring gdb..."
	echo -e "./configure --prefix=$stage_dir $gdb_args CFLAGS=$opt_flag\n"
	./configure --prefix=$stage_dir $gdb_args CFLAGS="$opt_flag"
	# run gdb make
	echo -e "\nBuilding gdb...\n"
	make --silent
	# run gdb make install
	echo -e "\nInstalling gdb...\n"
	make --silent install
	echo -e "\ngdb installed to $stage_dir.\n"
	echo -e "Starting post build ops.\n"
	# ensure the old staging location for gdb files is removed
	if [[ -d $dep_libdir/$python_gdb_name ]]; then
		rm -rf $dep_libdir/$python_gdb_name
	fi
	mkdir $dep_libdir/$python_gdb_name
	# save the built files into the stage_dir for later
	if [[ -d $gdb_python_dir ]]; then
		echo "Delivering built python files to $dep_libdir/$python_gdb_name..."
		cp -r $gdb_python_dir $dep_libdir/$python_gdb_name/
	else
		echo "ERROR: Could not find built python files. The gdb build failed."
		((++comp_error))
	fi
	if [[ -d $gdb_syscalls_dir ]]; then
		echo "Delivering built syscall files to $dep_libdir/$python_gdb_name..."
		cp -r $gdb_syscalls_dir $dep_libdir/$python_gdb_name/
	else
		echo "ERROR: Could not find built syscall files. The gdb build failed."
		((++comp_error))
	fi
	# remove the bad python file that causes errors
	if [[ -e $dep_libdir/$python_gdb_name/$gdb_bad_file ]]; then
		rm $dep_libdir/$python_gdb_name/$gdb_bad_file
	fi
	# deliver the built files
	if [[ -d $dep_libdir/$python_gdb_name ]]; then
		echo "Delivering built gdb files to $libdir..."
		cp -r $dep_libdir/$python_gdb_name $libdir
	else
		echo "ERROR: Could not find built gdb files in $dep_libdir. The gdb build failed."
		((++comp_error))
	fi
	# rename gdb to gdb-guard and deliver
	if [[ -e $dep_bindir/gdb ]]; then
		echo "Renaming gdb to gdb-guard..."
		mv $dep_bindir/gdb $dep_bindir/gdb-guard
		# deliver gdb-guard to the install dir
		echo "Delivering gdb-guard to $libexecdir..."
		cp $dep_bindir/gdb-guard $libexecdir
	else
		echo "ERROR: Could not find gdb in $dep_bindir. The gdb build failed."
		((++comp_error))
	fi
	# return home
	cd $cur_dir
	echo -e "########################################################\n"
else
	# We still need to deliver gdb binaries for use in the final lgdb package
	echo -e "Starting gdb install.\n"
	# deliver the build files
	if [[ -d $dep_libdir/$python_gdb_name ]]; then
		echo "Delivering built gdb files to $libdir..."
		cp -r $dep_libdir/$python_gdb_name $libdir
	else
		echo "ERROR: Could not find built gdb files in $dep_libdir. Manually add this to $inst_dir."
		((++comp_error))
	fi
	# Always use the gdb in the dep_bindir 
	if [[ -e $dep_bindir/gdb-guard ]]; then
		# deliver gdb-guard to the install dir
		echo "Delivering gdb-guard to $libexecdir..."
		cp $dep_bindir/gdb-guard $libexecdir
	else
		echo "ERROR: gdb-guard was not found in $dep_bindir. Manually add this to $inst_dir."
		((++comp_error))
	fi
	echo -e "########################################################\n"
fi

# check for errors in component
if [ "$comp_error" -ne "0" ]; then
	((++error))
fi

# reset comp_error
((comp_error=0))

# cuda-gdb 55 build
if [[ $opt_cuda55_gdb == y ]]; then
	echo "Starting cuda-gdb 55 build."
	# Unpack the rpm if we haven't done so already
	if [[ ! -d $cuda55_ext ]]; then
		# find the cuda55 rpm
		cuda55_rpm=$(ls --reverse --sort=version $cuda_rpm_dir/$cuda55_basename[0-9\.\-]*.x86_64.rpm | grep -m 1 -E "$cuda55_basename[0-9\.\-]*")
		if [[ -e $cuda55_rpm ]]; then
			# rpm found, so extract it
			mkdir $cuda55_ext
			cd $cuda55_ext
			rpm2cpio $cuda55_rpm | cpio -idv
		else
			echo "ERROR: Could not find $cuda55_basename rpm in $cuda_rpm_dir. The cuda-gdb 55 build failed."
			((++comp_error))
		fi
	fi
	# Get the location of the include files
	cuda55_include_dir=$(cd $cuda55_ext/$cuda_inc_path; pwd)
	if [[ ! -d $cuda55_include_dir ]]; then
		echo "ERROR: Missing required $cuda55_basename headers. The cuda-gdb 55 build failed."
		((++comp_error))
	fi
	# change directories
	cd $cuda55_gdb_src
	echo -e "\nUsing cuda-gdb 55 source $cuda55_gdb_src.\n"
	# run cuda-gdb confiugre
	echo "Configuring cuda-gdb..."
	echo -e "./configure --prefix=$stage_dir $cuda_gdb_args CFLAGS=\"-I$cuda55_include_dir $opt_flag\" LDFLAGS=-lpthread\n"
	./configure --prefix=$stage_dir $cuda_gdb_args CFLAGS="-I$cuda55_include_dir $opt_flag" LDFLAGS='-lpthread'
	# run cuda-gdb make
	echo -e "\nBuilding cuda-gdb...\n"
	make --silent
	# run cuda-gdb make install
	echo -e "\nInstalling cuda-gdb...\n"
	make --silent install
	echo -e "\ncuda-gdb installed to $stage_dir.\n"
	echo -e "Starting post build ops.\n"
	# rename cuda-gdb to cuda55-gdb-guard and deliver
	if [ -e $dep_bindir/cuda-gdb ]; then
		echo "Renaming cuda-gdb to cuda55-gdb-guard..."
		mv $dep_bindir/cuda-gdb $dep_bindir/cuda55-gdb-guard
		# deliver cuda55-gdb-guard to the install dir
		echo "Delivering cuda55-gdb-guard to $libexecdir..."
		cp $dep_bindir/cuda55-gdb-guard $libexecdir
	else
		echo "ERROR: Could not find cuda-gdb in $dep_bindir. The cuda-gdb 55 build failed."
		((++comp_error))
	fi
	# return home
	cd $cur_dir
	echo -e "########################################################\n"
else
	# We still need to deliver gdb binaries for use in the final lgdb package
	echo -e "Starting cuda-gdb 55 install.\n"
	# Always use the cuda-gdb in dep_bindir
	if [[ -e $dep_bindir/cuda55-gdb-guard ]]; then
		# deliver cuda55-gdb-guard to install dir
		echo "Delivering cuda55-gdb-guard to $libexecdir..."
		cp $dep_bindir/cuda55-gdb-guard $libexecdir
	else
		echo "ERROR: cuda55-gdb-guard was not found in $dep_bindir. Manually add this to $inst_dir."
		((++comp_error))
	fi
	echo -e "########################################################\n"
fi

# check for errors in component
if [ "$comp_error" -ne "0" ]; then
	((++error))
fi

# reset comp_error
((comp_error=0))

# cuda-gdb 65 build
if [[ $opt_cuda65_gdb == y ]]; then
	echo "Starting cuda-gdb 65 build."
	# Unpack the rpm if we haven't done so already
	if [[ ! -d $cuda65_ext ]]; then
		# find the cuda65 rpm
		cuda65_rpm=$(ls --reverse --sort=version $cuda_rpm_dir/$cuda65_basename[0-9\.\-]*.x86_64.rpm | grep -m 1 -E "$cuda65_basename[0-9\.\-]*")
		if [[ -e $cuda65_rpm ]]; then
			# rpm found, so extract it
			mkdir $cuda65_ext
			cd $cuda65_ext
			rpm2cpio $cuda65_rpm | cpio -idv
		else
			echo "ERROR: Could not find $cuda65_basename rpm in $cuda_rpm_dir. The cuda-gdb 65 build failed."
			((++comp_error))
		fi
	fi
	# Get the location of the include files
	cuda65_include_dir=$(cd $cuda65_ext/$cuda_inc_path; pwd)
	if [[ ! -d $cuda65_include_dir ]]; then
		echo "ERROR: Missing required $cuda65_basename headers. The cuda-gdb 65 build failed."
		((++comp_error))
	fi
	# change directories
	cd $cuda65_gdb_src
	echo -e "\nUsing cuda-gdb 65 source $cuda65_gdb_src.\n"
	# run cuda-gdb confiugre
	echo "Configuring cuda-gdb..."
	echo -e "./configure --prefix=$stage_dir $cuda_gdb_args CFLAGS=\"-I$cuda65_include_dir $opt_flag\" LDFLAGS=-lpthread\n"
	./configure --prefix=$stage_dir $cuda_gdb_args CFLAGS="-I$cuda65_include_dir $opt_flag" LDFLAGS='-lpthread'
	# run cuda-gdb make
	echo -e "\nBuilding cuda-gdb...\n"
	make --silent
	# run cuda-gdb make install
	echo -e "\nInstalling cuda-gdb...\n"
	make --silent install
	echo -e "\ncuda-gdb installed to $stage_dir.\n"
	echo -e "Starting post build ops.\n"
	# rename cuda-gdb to cuda65-gdb-guard and deliver
	if [ -e $dep_bindir/cuda-gdb ]; then
		echo "Renaming cuda-gdb to cuda65-gdb-guard..."
		mv $dep_bindir/cuda-gdb $dep_bindir/cuda65-gdb-guard
		# deliver cuda65-gdb-guard to the install dir
		echo "Delivering cuda65-gdb-guard to $libexecdir..."
		cp $dep_bindir/cuda65-gdb-guard $libexecdir
	else
		echo "ERROR: Could not find cuda-gdb in $dep_bindir. The cuda-gdb 65 build failed."
		((++comp_error))
	fi
	# return home
	cd $cur_dir
	echo -e "########################################################\n"
else
# We still need to deliver gdb binaries for use in the final lgdb package
	echo -e "Starting cuda-gdb 65 install.\n"
	# Always use the cuda-gdb in dep_bindir
	if [[ -e $dep_bindir/cuda65-gdb-guard ]]; then
		# deliver cuda65-gdb-guard to install dir
		echo "Delivering cuda65-gdb-guard to $libexecdir..."
		cp $dep_bindir/cuda65-gdb-guard $libexecdir
	else
		echo "ERROR: cuda65-gdb-guard was not found in $dep_bindir. Manually add this to $inst_dir."
		((++comp_error))
	fi
	echo -e "########################################################\n"
fi

# check for errors in component
if [ "$comp_error" -ne "0" ]; then
	((++error))
fi

# reset comp_error
((comp_error=0))

# gdb uda support library build
if [[ $opt_gdb_uda == y ]]; then
	echo -e "Starting gdb uda support library build.\n"
	# change directories
	cd $gdb_uda_src
	# run gdb uda library make
	make --silent
	# deliver the gdb uda support library to libdir
	if [ -e plugin/uda-plugin-cray-upc.so ]; then
		# Silently deliver the plugin to the dep_libdir
		cp plugin/uda-plugin-cray-upc.so $dep_libdir
		echo "Delivering uda plugin to $libdir"
		cp plugin/uda-plugin-cray-upc.so $libdir
	else
		echo "ERROR: Could not find uda-plugin-cray-upc.so in $PWD/plugin. The uda plugin build failed."
		((++comp_error))
	fi
	# return home 
	cd $cur_dir 
	echo -e "########################################################\n"
else
	# We still need to deliver the editline libraries for use in the final lgdb package
	echo -e "Starting gdb uda support library install.\n"
	if [ -e $dep_libdir/uda-plugin-cray-upc.so ]; then
		echo "Delivering uda plugin to $libdir"
		cp $dep_libdir/uda-plugin-cray-upc.so $libdir
	else
		echo "ERROR: gdb uda plugin was not found in $dep_libdir. Manually add this to $inst_dir."
		((++comp_error))
	fi
fi

# check for errors in component
if [ "$comp_error" -ne "0" ]; then
	((++error))
fi

# reset comp_error
((comp_error=0))

# editline build
if [[ $opt_editline == y ]]; then
	echo -e "Starting editline build.\n"
	# change directories
	cd $editline_src
	echo -e "\nUsing editline source $editline_src.\n"
	# run editline configure
	echo "Configuring editline..."
	echo -e "./configure --prefix=$stage_dir CFLAGS=$opt_flag\n"
	./configure --prefix=$stage_dir CFLAGS="$opt_flag"
	# run editline make
	echo -e "\nBuilding editline...\n"
	make --silent
	# run editline make install
	echo -e "\nInstalling editline...\n"
	make --silent install
	echo -e "\nEditline installed to $stage_dir.\n"
	echo -e "Starting post build ops.\n"
	# deliver the editline dso's to libdir
	if [ -e $dep_libdir/$libedit_minor ]; then
		echo "Delivering $libedit_minor to $libdir..."
		cp $dep_libdir/$libedit_minor $libdir
		echo "Creating editline symlinks for $libedit_dso and $libedit_major..."
		# cd to libdir to create the symlinks
		cd $libdir
		ln -s $libedit_minor $libedit_dso
		ln -s $libedit_minor $libedit_major
	else
		echo "ERROR: Could not find $libedit_minor in $dep_libdir. The editline build failed."
		((++comp_error))
	fi
	# return home
	cd $cur_dir
	echo -e "########################################################\n"
else
# We still need to deliver the editline libraries for use in the final lgdb package
	echo -e "Starting editline install.\n"
	# deliver the editline dso's to libdir
	if [ -e $dep_libdir/$libedit_minor ]; then
		echo "Using editline package in $stage_dir."
		echo "Delivering $libedit_minor to $libdir..."
		cp $dep_libdir/$libedit_minor $libdir
		echo "Creating editline symlinks for $libedit_dso and $libedit_major..."
		# cd to libdir to create the symlinks
		cd $libdir
		ln -s $libedit_minor $libedit_dso
		ln -s $libedit_minor $libedit_major
		# return home
		cd $cur_dir
	else
		echo "ERROR: $libedit_minor was not found. Manually add this to $inst_dir."
		((++comp_error))
	fi
	echo -e "########################################################\n"
fi

# check for errors in component
if [ "$comp_error" -ne "0" ]; then
	((++error))
fi

# reset comp_error
((comp_error=0))

# libarchive build
if [[ $opt_libarchive == y ]]; then
	echo -e "Starting libarchive build.\n"
	# change directories
	cd $libarchive_src
	echo -e "Using libarchive source in $libarchive_src.\n"
	# run libarchive configure
	echo "Configuring libarchive..."
	echo -e "./configure --prefix=$stage_dir $libarchive_args CFLAGS=$opt_flag\n"
	./configure --prefix=$stage_dir $libarchive_args CFLAGS="$opt_flag"
	# run make
	echo -e "\nBuilding libarchive...\n"
	make --silent
	# run make install
	echo -e "Installing libarchive...\n"
	make --silent install
	echo -e "\nlibarchive installed to $stage_dir.\n"
	# check to see if libarchive.a was created
	if [ ! -e $dep_libdir/libarchive.a ]; then
		echo "ERROR: libarchive.a not found in $dep_libdir. The libarchive build failed."
		((++comp_error))
	fi
	# return home
	cd $cur_dir
	echo -e "########################################################\n"
fi

# check for errors in component
if [ "$comp_error" -ne "0" ]; then
	((++error))
fi

# reset comp_error
((comp_error=0))

# tools interface build
if [[ $opt_interface == y ]]; then
	echo -e "Starting tools interface build.\n" 
	# change directories 
	cd $interface_src 
	echo -e "\nUsing tools interface source $interface_src.\n" 
	# run tools interface configure 
	echo "Configuring tools interface..." 
	echo -e "./configure --prefix=$stage_dir --with-libarchive=$stage_dir --with-libmi=$stage_dir CFLAGS=$opt_flag\n" 
	./configure --prefix=$stage_dir --with-libarchive=$stage_dir --with-libmi=$stage_dir CFLAGS="$opt_flag"
	# run tools interface make 
	echo -e "\nBuilding tools interface...\n" 
	make --silent 
	# run tools interface make install 
	echo -e "\nInstalling tools interface...\n" 
	make --silent install 
	echo -e "\nTools interface installed to $stage_dir.\n" 
	echo -e "Starting post build ops.\n" 
	# deliver cti_dlaunch
	if [ -e $dep_libexecdir/cti_dlaunch ]; then 
		echo "Delivering cti_dlaunch to $libexecdir..."
		cp $dep_libexecdir/cti_dlaunch $libexecdir 
	else 
		echo "ERROR: Could not find cti_dlaunch in $dep_libexecdir. The tools interface build failed." 
		((++comp_error)) 
	fi
	# deliver cti_starter
	if [ -e $dep_libexecdir/cti_starter ]; then
		echo "Delivering cti_starter to $libexecdir..."
		cp $dep_libexecdir/cti_starter $libexecdir
	else
		echo "ERROR: Could not find cti_starter in $dep_libexecdir. The tools interface build failed."
		((++comp_error))
	fi
	# deliver cti_slurm_util
	if [ -e $dep_libexecdir/cti_slurm_util ]; then
		echo "Delivering cti_slurm_util to $libexecdir..."
		cp $dep_libexecdir/cti_slurm_util $libexecdir
	else
		echo "ERROR: Could not find cti_slurm_util in $dep_libexecdir. The tools interface build failed."
		((++comp_error))
	fi
	# deliver cti_overwatch
	if [ -e $dep_libexecdir/cti_overwatch ]; then
		echo "Delivering cti_overwatch to $libexecdir..."
		cp $dep_libexecdir/cti_overwatch $libexecdir
	else
		echo "ERROR: Could not find cti_overwatch in $dep_libexecdir. The tools interface build failed."
		((++comp_error))
	fi
	# deliver libaudit.so
	if [ -e $dep_libdir/libaudit.so ]; then
		echo "Delivering libaudit.so to $libdir..."
		cp $dep_libdir/libaudit.so $libdir
	else
		echo "ERROR: Could not find libaudit.so in $dep_libdir. The tools interface build failed."
		((++comp_error))
	fi
	# deliver the tool interface dso's to libdir 
	if [ -e $dep_libdir/$libcraytool_backend_minor ]; then 
		echo "Delivering $libcraytool_backend_minor to $libdir..." 
		cp $dep_libdir/$libcraytool_backend_minor $libdir 
		echo "Creating libcraytool symlinks for $libcraytool_backend_dso and $libcraytool_backend_major..." 
		# cd to libdir to create the symlinks 
		cd $libdir 
		ln -s $libcraytool_backend_minor $libcraytool_backend_dso
		ln -s $libcraytool_backend_minor $libcraytool_backend_major
	else 
		echo "ERROR: Could not find $libcraytool_backend_minor in $dep_libdir. The tools interface build failed." 
		((++comp_error)) 
	fi
	if [ -e $dep_libdir/$libcraytool_frontend_minor ]; then 
		echo "Delivering $libcraytool_frontend_minor to $libdir..." 
		cp $dep_libdir/$libcraytool_frontend_minor $libdir 
		echo "Creating libcraytool symlinks for $libcraytool_frontend_dso and $libcraytool_frontend_major..." 
		# cd to libdir to create the symlinks 
		cd $libdir 
		ln -s $libcraytool_frontend_minor $libcraytool_frontend_dso 
		ln -s $libcraytool_frontend_minor $libcraytool_frontend_major 
	else 
		echo "ERROR: Could not find $libcraytool_frontend_minor in $dep_libdir. The tools interface build failed." 
		((++comp_error)) 
	fi
	# return home 
	cd $cur_dir 
	echo -e "########################################################\n"
else
	# We still need to deliver the tools interface libraries for use in the final lgdb package
	echo -e "Starting tools interface install.\n"
	# deliver cti_dlaunch 
	if [ -e $dep_libexecdir/cti_dlaunch ]; then 
		echo "Delivering cti_dlaunch to $libexecdir..." 
		cp $dep_libexecdir/cti_dlaunch $libexecdir 
	else 
		echo "ERROR: Could not find cti_dlaunch in $dep_libexecdir. Manually add this to $inst_dir." 
		((++comp_error)) 
	fi 
	# deliver cti_starter
	if [ -e $dep_libexecdir/cti_starter ]; then
		echo "Delivering cti_starter to $libexecdir..."
		cp $dep_libexecdir/cti_starter $libexecdir
	else
		echo "ERROR: Could not find cti_starter in $dep_libexecdir. Manually add this to $inst_dir."
		((++comp_error))
	fi
	# deliver cti_slurm_util
	if [ -e $dep_libexecdir/cti_slurm_util ]; then
		echo "Delivering cti_slurm_util to $libexecdir..."
		cp $dep_libexecdir/cti_slurm_util $libexecdir
	else
		echo "ERROR: Could not find cti_slurm_util in $dep_libexecdir. Manually add this to $inst_dir."
		((++comp_error))
	fi
	# deliver cti_overwatch
	if [ -e $dep_libexecdir/cti_overwatch ]; then
		echo "Delivering cti_overwatch to $libexecdir..."
		cp $dep_libexecdir/cti_overwatch $libexecdir
	else
		echo "ERROR: Could not find cti_overwatch in $dep_libexecdir. Manually add this to $inst_dir."
		((++comp_error))
	fi
	# deliver libaudit.so 
	if [ -e $dep_libdir/libaudit.so ]; then 
		echo "Delivering libaudit.so to $libdir..." 
		cp $dep_libdir/libaudit.so $libdir 
	else 
		echo "ERROR: Could not find libaudit.so in $dep_libdir. Manually add this to $inst_dir." 
		((++comp_error)) 
	fi
	# deliver the tool interface dso's to libdir 
	if [ -e $dep_libdir/$libcraytool_backend_minor ]; then 
		echo "Delivering $libcraytool_backend_minor to $libdir..." 
		cp $dep_libdir/$libcraytool_backend_minor $libdir 
		echo "Creating libcraytool symlinks for $libcraytool_backend_dso and $libcraytool_backend_major..." 
		# cd to libdir to create the symlinks 
		cd $libdir 
		ln -s $libcraytool_backend_minor $libcraytool_backend_dso 
		ln -s $libcraytool_backend_minor $libcraytool_backend_major 
	else 
		echo "ERROR: Could not find $libcraytool_backend_minor in $dep_libdir. Manually add this to $inst_dir." 
		((++comp_error)) 
	fi 
	if [ -e $dep_libdir/$libcraytool_frontend_minor ]; then 
		echo "Delivering $libcraytool_frontend_minor to $libdir..." 
		cp $dep_libdir/$libcraytool_frontend_minor $libdir 
		echo "Creating libcraytool symlinks for $libcraytool_frontend_dso and $libcraytool_frontend_major..." 
		# cd to libdir to create the symlinks 
		cd $libdir 
		ln -s $libcraytool_frontend_minor $libcraytool_frontend_dso 
		ln -s $libcraytool_frontend_minor $libcraytool_frontend_major 
	else 
		echo "ERROR: Could not find $libcraytool_frontend_minor in $dep_libdir. Manually add this to $inst_dir." 
		((++comp_error)) 
	fi 
	# return home
	cd $cur_dir 
	echo -e "########################################################\n"
fi

# check for errors in component
if [ "$comp_error" -ne "0" ]; then
	((++error))
fi

# reset comp_error
((comp_error=0))

# mrnet build - This *must* happen after the cti library build now since mrnet
# depends on cti components.
if [[ $opt_mrnet == y ]]; then
	echo "Starting MRNet build."
	# change directories
	cd $mrnet_src
	echo -e "\nUsing MRNet source in $mrnet_src.\n"
	# run mrnet configure
	echo "Configuring MRNet..."
	echo -n "./configure --prefix=$stage_dir --with-startup=cray "
	echo -n "--with-cti-inc=$dep_includedir --with-cti-lib=$dep_libdir "
	echo -e "$mrnet_args CFLAGS=$opt_flag CXXFLAGS=$opt_flag\n"
	./configure --prefix=$stage_dir --with-startup=cray --enable-shared \
	--with-cti-inc=$dep_includedir --with-cti-lib=$dep_libdir \
	$mrnet_args CFLAGS="$opt_flag" CXXFLAGS="$opt_flag"
	# run mrnet make
	echo -e "\nBuilding MRNet...\n"
	make --silent
	# run mrnet make install
	echo -e "\nInstalling MRNet...\n"
	make --silent install
	echo -e "\nMRNet installed to $stage_dir\n"
	echo -e "Starting post build ops.\n"
	# deliver the boost headers included with mrnet source if not done already
	if [ ! -d $dep_includedir/boost ]; then
		if [ -d $mrnet_src/include/boost ]; then
			echo "Delivering boost headers to $dep_includedir..."
			cp -r $mrnet_src/include/boost $dep_includedir
		else
			echo "WARNING: Could not find the boost include files. Build may be incomplete."
			echo "The system is probably already providing these for free."
		fi
	fi
	# Deliver the mrnet binaries
	if [ -e $dep_bindir/mrnet_commnode ]; then
		echo "Delivering mrnet_commnode to $libexecdir..."
		cp $dep_bindir/mrnet_commnode $libexecdir
	else
		echo "ERROR: Could not find mrnet_commnode in $dep_bindir. MRNet build failed."
		((++comp_error))
	fi
	# Deliver the commnode wrapper
	if [ -f $thirdparty_dir/lgdb_mrnet_commnode_wrapper ]; then
		echo "Delivering lgdb_mrnet_commnode_wrapper to $libexecdir..."
		cp $thirdparty_dir/lgdb_mrnet_commnode_wrapper $libexecdir
	else
		echo "WARNING: Could not find lgdb_mrnet_commnode_wrapper. Build may be incomplete."
		((++warning))
	fi
	# Deliver the mrnet topology generator 
	if [ -e $dep_bindir/mrnet_topgen ]; then
		echo "Delivering mrnet_topgen to $libexecdir..."
		cp $dep_bindir/mrnet_topgen $libexecdir
	else
		echo "ERROR: Could not find mrnet_topgen in $dep_bindir. MRNet cannot create k-nomial tree."
		((++comp_error))
	fi
	# Deliver the mrnet dso's to libdir
	if [ -e $dep_libdir/$libmrnet_minor ]; then
		echo "Delivering $libmrnet_minor to $libdir..."
		cp $dep_libdir/$libmrnet_minor $libdir
		echo "Creating mrnet symlinks for $libmrnet_dso and $libmrnet_major..."
		# cd to libdir to create the symlinks
		cd $libdir
		ln -s $libmrnet_minor $libmrnet_dso
		ln -s $libmrnet_minor $libmrnet_major
	else
		echo "ERROR: Could not find $libmrnet_minor in $dep_libdir. The mrnet build failed."
		((++comp_error))
	fi
	# Deliver the xplat dso's to libdir
	if [ -e $dep_libdir/$libxplat_minor ]; then
		echo "Delivering $libxplat_minor to $libdir..." 
		cp $dep_libdir/$libxplat_minor $libdir 
		echo "Creating xplat symlinks for $libxplat_dso and $libxplat_major..." 
		# cd to libdir to create the symlinks 
		cd $libdir 
		ln -s $libxplat_minor $libxplat_dso 
		ln -s $libxplat_minor $libxplat_major 
	else 
		echo "ERROR: Could not find $libxplat_minor in $dep_libdir. The mrnet build failed." 
		((++comp_error)) 
	fi
	# Deliver the mrnet_config.h file
	if [ -e $mrnet_src/build/x86_64-unknown-linux-gnu/mrnet_config.h ]; then
		echo "Delivering mrnet_config.h to $dep_includedir..."
		cp $mrnet_src/build/x86_64-unknown-linux-gnu/mrnet_config.h $dep_includedir
	else
		echo "WARNING: Could not find mrnet_config.h in $mrnet_src/build/x86_64-unknown-linux-gnu."
		echo "Build may be incomplete."
		((++warning))
	fi
	# Deliver the xplat_config.h file
	if [ -e $mrnet_src/build/x86_64-unknown-linux-gnu/xplat_config.h ]; then
		echo "Delivering xplat_config.h to $dep_includedir..."
		cp $mrnet_src/build/x86_64-unknown-linux-gnu/xplat_config.h $dep_includedir
	else
		echo "WARNING: Could not find xplat_config.h in $mrnet_src/build/x86_64-unknown-linux-gnu."
		echo "Build may be incomplete."
		((++warning))
	fi
	# return home
	cd $cur_dir
	echo -e "########################################################\n"
else
# We still need to deliver MRNet binaries for use in the final lgdb package
	echo -e "Starting MRNet install.\n"
	echo -e "Using MRNet package in $stage_dir."
	# Deliver the mrnet binaries
	if [ -f $dep_bindir/mrnet_commnode ]; then
		echo "Delivering mrnet_commnode to $libexecdir..."
		cp $dep_bindir/mrnet_commnode $libexecdir
	else
		echo "ERROR: mrnet_commnode was not found. Manually add this to $inst_dir."
		((++comp_error))
	fi
	# Deliver the commnode wrapper
	if [ -f $thirdparty_dir/lgdb_mrnet_commnode_wrapper ]; then
		echo "Delivering lgdb_mrnet_commnode_wrapper to $libexecdir..."
		cp $thirdparty_dir/lgdb_mrnet_commnode_wrapper $libexecdir
	else
		echo "ERROR: lgdb_mrnet_commnode_wrapper was not found. Manually add this to $inst_dir."
		((++comp_error))
	fi
	# Deliver the mrnet dso's to libdir 
	if [ -e $dep_libdir/$libmrnet_minor ]; then 
		echo "Delivering $libmrnet_minor to $libdir..." 
		cp $dep_libdir/$libmrnet_minor $libdir 
		echo "Creating mrnet symlinks for $libmrnet_dso and $libmrnet_major..." 
		# cd to libdir to create the symlinks 
		cd $libdir 
		ln -s $libmrnet_minor $libmrnet_dso 
		ln -s $libmrnet_minor $libmrnet_major 
	else 
		echo "ERROR: Could not find $libmrnet_minor in $dep_libdir. Manually add this to $inst_dir." 
		((++comp_error)) 
	fi 
	# Deliver the xplat dso's to libdir 
	if [ -e $dep_libdir/$libxplat_minor ]; then 
		echo "Delivering $libxplat_minor to $libdir..." 
		cp $dep_libdir/$libxplat_minor $libdir 
		echo "Creating xplat symlinks for $libxplat_dso and $libxplat_major..." 
		# cd to libdir to create the symlinks 
		cd $libdir 
		ln -s $libxplat_minor $libxplat_dso 
		ln -s $libxplat_minor $libxplat_major 
	else 
		echo "ERROR: Could not find $libxplat_minor in $dep_libdir. Manually add this to $inst_dir." 
		((++comp_error)) 
	fi
	# return home
	cd $cur_dir
	echo -e "########################################################\n"
fi

# check for errors in component
if [ "$comp_error" -ne "0" ]; then
	((++error))
fi

# reset comp_error
((comp_error=0))

# lgdb build
if [[ $opt_lgdb == y ]]; then
	echo -e "Starting lgdb build.\n"
	# change directories
	echo -e "Using guard source in $lgdb_src.\n"
	cd $lgdb_src
	# run lgdb configure
	echo "Configuring lgdb..."
	echo -n "./configure --with-aif=$path_aif --with-rd --with-mrnet=$stage_dir --with-cray"
	echo -n "--with-craytool-inc=$dep_includedir --with-craytool-lib=$dep_libdir --prefix=$inst_dir "
	echo -n "--with-editline=$stage_dir --with-gdbmi=$dbg_backend_args --with-libssh2=$libssh2_args "
	echo -n ""$perf_flag" "$be_perf_flag" "$dbg_flag" "$valgrind_flag" CFLAGS=$opt_flag "
	echo -e "CXXFLAGS=$opt_flag\n"
	./configure --with-aif=$path_aif --with-rd --with-mrnet=$stage_dir --with-cray \
	--with-craytool-inc=$dep_includedir --with-craytool-lib=$dep_libdir --prefix=$inst_dir \
	--with-editline=$stage_dir --with-gdbmi=$dbg_backend_args --with-libssh2=$libssh2_args \
	"$perf_flag" "$be_perf_flag" "$dbg_flag" "$valgrind_flag" CFLAGS="$opt_flag" \
	CXXFLAGS="$opt_flag"
	# run lgdb make
	echo -e "\nBuilding lgdb...\n"
	make --silent
	# run lgdb make install
	echo -e "\nInstalling lgdb...\n"
	make --silent install
	echo -e "\nlgdb installed to $inst_dir.\n"
	echo -e "Starting post build ops.\n"
	# change the name of the guard binary to lgdb
	if [ -e $bindir/guard ]; then
		echo "Renaming guard to lgdb..."
		mv $bindir/guard $bindir/lgdb
	else
		echo "ERROR: guard not found in $bindir. The lgdb build failed."
		((++comp_error))
	fi
	# ensure the guard_proxy was built
	if [ ! -e $libexecdir/guard_proxy ]; then
		echo "ERROR: guard_proxy not found in $libexecdir. The lgdb build failed."
		((++comp_error))
	fi
	# deliver the modulefiles
	echo -e "\nDelivering lgdb modulefile..."
	if [ ! -e $module_primer ]; then
		echo -e "\tWARNING: Defined module primer not found. Modulefile will not be delivered."
		((++warning))
	else
		if [ ! -d $inst_dir/modulefiles ]; then
			mkdir $inst_dir/modulefiles
		fi
		if [ ! -d $inst_dir/modulefiles/cray-lgdb ]; then
			mkdir $inst_dir/modulefiles/cray-lgdb
		fi
		# deliver the fixed modulefile. Sed, you amaze me.
		cat $module_primer | sed -e "s@%HOME%@$inst_dir@" | sed -e "s@%VERSION%@$module_version@" \
		> $inst_dir/modulefiles/cray-lgdb/$module_version.DEV
	fi
	# deliver the demos
	if [ ! -d $demo_dir ]; then
		echo -e "\nWARNING: Defined demos directory not found. Demos will not be delivered."
		((++warning))
	else
		echo -e "\nDelivering demos..."
		cp -r $demo_dir $inst_dir/$demo_directory_name
	fi
	# deliver the docs
	if [ ! -d $docs_dir ]; then
		echo -e "\nWARNING: Defined docs directory not found. Docs will not be delivered."
		((++warning))
	else
		echo -e "\nDelivering docs..."
		cp -r $docs_dir $inst_dir/$docs_directory_name
	fi
	
	# return home
	cd $cur_dir
	echo -e "########################################################\n"
fi

# check for errors in component
if [ "$comp_error" -ne "0" ]; then
	((++error))
fi

# cleanup
if [[ $opt_cleanup == y ]]; then
	echo -e "Starting build cleanup.\n"
	# make clean libaif
	if [[ $opt_aif == y ]]; then
		cd $aif_src
		echo "Cleaning libaif..."
		make --silent clean
		make --silent distclean
	fi
	# make clean libmi
	if [[ $opt_libmi == y ]]; then
		cd $libmi_src
		echo "Cleaning libmi..."
		make --silent clean
		make --silent distclean
	fi
	# make clean libssh2
	if [[ $opt_libssh2 == y ]]; then
		cd $libssh2_src
		echo "Cleaning libssh2..."
		make --silent clean
		make --silent distclean
	fi
	# make clean mrnet
	if [[ $opt_mrnet == y ]]; then
		cd $mrnet_src
		echo "Cleaning MRNet..."
		make --silent clean
		make --silent distclean
	fi
	# make clean python
	if [[ $opt_python == y ]]; then
		cd $python_src
		echo "Cleaning python..."
		make --silent clean
		make --silent distclean
	fi
	# make clean gdb
	if [[ $opt_gdb == y ]]; then
		cd $gdb_src
		echo "Cleaning gdb..."
		make --silent clean
		make --silent distclean
	fi
	# make clean cuda-gdb 55
	if [[ $opt_cuda55_gdb == y ]]; then
		cd $cuda55_gdb_src
		echo "Cleaning cuda-gdb 55..."
		make --silent clean
		make --silent distclean
	fi
	# make clean cuda-gdb 65
	if [[ $opt_cuda65_gdb == y ]]; then
		cd $cuda65_gdb_src
		echo "Cleaning cuda-gdb 65..."
		make --silent clean
		make --silent distclean
	fi
	# make clean gdb uda support library
	if [[ $opt_gdb_uda == y ]]; then
		cd $gdb_uda_src
		echo "Cleaning gdb uda support library..."
		make --silent clean
	fi
	# make clean editline
	if [[ $opt_editline == y ]]; then
		echo "Cleaning editline..."
		cd $editline_src
		make --silent clean
		make --silent distclean
	fi
	# make clean libarchive
	if [[ $opt_libarchive == y ]]; then
		echo "Cleaning libarchive..."
		cd $libarchive_src
		make --silent clean
		make --silent distclean
	fi
	# make clean tools interface
	if [[ $opt_interface == y ]]; then
		echo "Cleaning tools interface..."
		cd $interface_src
		make --silent clean
		make --silent distclean
	fi
	# make clean lgdb
	if [[ $opt_lgdb == y ]]; then
		echo "Cleaning lgdb..."
		cd $lgdb_rd_src
		make --silent clean
		cd $lgdb_src
		make --silent clean
		make --silent distclean
	fi
	# return home
	cd $cur_dir
	echo -e "########################################################\n"
fi

echo "Done. The lgdb distribution was installed to $inst_dir."

if [ "$error" -eq "0" ] && [ "$warning" -eq "0" ]; then
	echo -e "Finished without warnings or errors.\n"
else
	echo "Finished with $warning warnings."
	echo -e "Finished with errors in $error components.\n"
	exit 1
fi

