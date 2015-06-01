#!/bin/bash
#
# lgdb build tool
# A bootstrap script to automagically build guard and all of its dependencies
#
# Copyright 2010-2015 Cray Inc.  All Rights Reserved.
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
mrnet_directory_name="mrnet"
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
cuda_inc_path="opt/nvidia/cudatoolkit*/*/include"

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
libcraytool_backend_major="libcraytools_be.so.7"
libcraytool_backend_minor="libcraytools_be.so.7.0.0"

libcraytool_frontend_dso="libcraytools_fe.so"
libcraytool_frontend_major="libcraytools_fe.so.7"
libcraytool_frontend_minor="libcraytools_fe.so.7.0.0"

libcraytool_dlaunch="cti_dlaunch7.0"

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
cti_src="$cur_dir/$craytool_directory_name"

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
opt_cti=n
opt_lgdb=n
opt_cleanup=n
opt_advanced=n
prompt_user=1

opt_sub="A"
opt_sub_num=1

# Used to keep track of warnings and errors
((error=0))
((warning=0))
((comp_error=0))

########################
# Function definitions #
########################

function print_help {
	echo "Usage: `basename $0` options (ABCDEFGHIJKLMN | --all)"
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

function check_error {
	# check for errors in component
	if [[ "$comp_error" -ne "0" ]]; then
		((++error))
	fi
	# reset comp_error
	((comp_error=0))
}

function __ask_build {
	local opt_var
	if [[ $prompt_user == 1 ]]; then
		while true; do
			read opt_var
			if [[ $opt_var == y || $opt_var == Y || $opt_var == n || $opt_var == N ]]; then
				eval ${1}=\$opt_var
				break
			fi
			echo -ne "\tEnter y or n: "
		done
	else
		eval echo "\$${1}"
	fi
}

function ask_build {
	echo -n "${opt_sub}. ${1}?[y/n]: "
	__ask_build $2
}

function ask_sub_build {
	echo -ne "  ${opt_sub}${opt_sub_num}. ${1}?[y/n]: "
	__ask_build $2
	((++opt_sub_num))
}

function do_build {
	# run make
	echo -e "\nBuilding ${1}...\n"
	make --silent
	# run make install
	echo -e "Installing ${1}...\n"
	make --silent install
}

function do_clean {
	if [[ $1 == y ]]; then
		cd $2
		echo "Cleaning ${3}..."
		make --silent clean
		make --silent distclean
	fi
}

function copy_file {

	local base_name=$(basename $1)
	local dir_name=$(dirname $1)
	
	# ensure $2 exists
	if [[ ! -d $2 ]]; then
		echo "ERROR: $2 is not a directory. The $3 build failed."
		((++comp_error))
		return 1
	fi
	
	# copy $1 to $2, ensuring $1 exists
	if [[ -e $1 ]]; then
		# deliver to the install dir
		echo "Delivering $base_name to ${2}..."
		cp -r $1 $2
	else
		echo "ERROR: $base_name was not found in ${dir_name}. The $3 build failed."
		((++comp_error))
		return 1
	fi
	
	return 0
}

function check_file {
	
	local base_name=$(basename $1)
	local dir_name=$(dirname $1)

	if [[ ! -e $1 ]]; then
		echo "ERROR: $base_name not found in ${dir_name}. The $2 build failed."
		((++comp_error))
		return 1
	fi
	
	return 0
}

function rename_file {

	local base_name_1=$(basename $1)
	local base_name_2=$(basename $2)
	local dir_name_1=$(dirname $1)
	local dir_name_2=$(dirname $2)
	
	# ensure $2 exists
	if [[ ! -d $dir_name_2 ]]; then
		echo "ERROR: $dir_name_2 is not a directory. The $3 build failed."
		((++comp_error))
		return 1
	fi

	# rename $1 to $2
	if [[ -e $1 ]]; then
		echo "Renaming $base_name_1 to ${base_name_2}..."
		mv $1 $2
	else
		echo "ERROR: Could not find $base_name_1 in ${dir_name_1}. The $3 build failed."
		((++comp_error))
		return 1
	fi
	
	return 0
}

##############################################
# Setup the build environment based on above #
##############################################

# Display welcome message
echo -e "\nlgdb build tool v$lgdb_build_version - By andrewg@cray.com"

# Remove the install directory tree if it already exists
if [[ -d $inst_dir ]]; then
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

if [[ -e /opt/modules/default/init/bash ]]; then 
	source /opt/modules/default/init/bash
fi

if test "x`module list -l 2>&1 > /dev/null | grep 'PrgEnv'`" != "x"; then
	echo "!!!!!!!!!!ERROR!!!!!!!!!!"
	echo "A PrgEnv module has been detected as being loaded in the current"
	echo "module list. Running this script with a PrgEnv-xxx module loaded"
	echo "results in undefined behavior. Unload the module and try again."
	exit
fi

##################################
# Start asking for user options. #
##################################

if [[ "$#" != "0" ]] ; then
	#getops doesn't natively support long opts
	prompt_user=0
	if [[ "$1" == "--help" ]] ; then
		print_help
		exit 0
	fi
	if [[ "$1" == "--all" ]] ; then
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
		opt_cti=y
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
				f | F ) echo "Building F: gdb"
					opt_gdb=y;;
				g | G ) echo "Building G: cuda-gdb for CUDA_55"
					opt_cuda55_gdb=y;;
				h | H ) echo "Building H: cuda-gdb for CUDA_65"
					opt_cuda65_gdb=y;;
				i | I ) echo "Building I: gdb uda support"
					opt_gdb_uda=y;;
				j | J ) echo "Building J: editline"
					opt_editline=y;;
				k | K ) echo "Building K: libarchive"
					opt_libarchive=y;;
				l | L ) echo "Building L: tools interface"
					opt_cti=y;;
				m | M )	echo "Building M: lgdb"
					opt_lgdb=y;;
				n | N ) echo "Running N: cleanup"
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
opt_sub="A"
ask_build "Specify dependency builds" opt_ask_deps
if [[ $opt_ask_deps == y ]]; then
	opt_sub_num=1
	ask_sub_build "Build libaif" opt_aif
	ask_sub_build "Build libmi" opt_libmi
	ask_sub_build "Build libssh2" opt_libssh2
	ask_sub_build "Build MRNet" opt_mrnet
	ask_sub_build "Build python" opt_python
	ask_sub_build "Build gdb" opt_gdb
	ask_sub_build "Build cuda-gdb for CUDA_55" opt_cuda55_gdb
	ask_sub_build "Build cuda-gdb for CUDA_65" opt_cuda65_gdb
	ask_sub_build "Build gdb uda support" opt_gdb_uda
	ask_sub_build "Build editline" opt_editline
	ask_sub_build "Build libarchive" opt_libarchive
	ask_sub_build "Build tools interface" opt_cti
fi

# Ask if we should build lgdb
opt_sub="B"
ask_build "Build lgdb" opt_lgdb

# Ask if we should cleanup after build.
opt_sub="C"
ask_build "Cleanup build" opt_cleanup

# Ask if the user wants to use advanced mode
opt_sub="D"
ask_build "Advanced mode" opt_advanced
if [[ $opt_advanced == y ]]; then
	opt_sub_num=1
	ask_sub_build "lgdb debug mode" opt_debug
	# only allow optimization level if not using debug mode.
	if [[ $opt_debug == n ]]; then
		ask_sub_build "Specify optimization level" opt_optimization
		if [[ $opt_optimization == y ]]; then
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
		fi
	fi
	ask_sub_build "lgdb perf log mode" opt_perflog
	# only ask for backend perf mode if using perf log mode.
	if [[ $opt_perflog == y ]]; then
		ask_sub_build "Extended backend perf log mode (gathers extra data to lustre files)" opt_beperflog
	fi
	ask_sub_build "lgdb backend valgrind mode" opt_valgrind

	# Now process the sub selections

	# Process debug mode. This turns on debug printing options inside of lgdb. 
	if [[ $opt_debug == y ]]; then
		# This is the debug option for the call to configure
		dbg_flag="--with-debug"
		opt_flag="-g -O0 -Wall"
		mrnet_args="--enable-debug"
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

# libaif build
if [[ $opt_aif == y ]]; then
	echo -e "Starting libaif build.\n"
	# change directories
	cd $aif_src
	echo -e "Using libaif source in $aif_src.\n"
	# run aif configure
	echo "Configuring libaif..."
	set -x
	./configure --prefix=$stage_dir CFLAGS="$opt_flag"
	set +x
	do_build "libaif"
	echo -e "\nlibaif staged to $stage_dir.\n"
	# check to see if libaif.a was created
	check_file "$dep_libdir/libaif.a" "AIF"
	echo -e "########################################################\n"
fi

# Check for errors in component
check_error

# libmi build
if [[ $opt_libmi == y ]]; then
	echo -e "Starting libmi build.\n"
	# change directories
	cd $libmi_src
	echo -e "Using libmi source in $libmi_src.\n"
	# run libmi configure
	echo "Configuring libmi..."
	set -x
	./configure --prefix=$stage_dir CFLAGS="$opt_flag"
	set +x
	do_build "libmi"
	echo -e "\nlibmi staged to $stage_dir.\n"
fi

#libmi install
echo -e "Starting libmi install.\n"
# deliver libmi.so 
copy_file "${dep_libdir}/libmi.so" $libdir "libmi"
echo -e "########################################################\n"

# Check for errors in component
check_error

# libssh2 build
if [[ $opt_libssh2 == y ]]; then
	echo -e "Starting libssh2 build.\n"
	# change directories
	cd $libssh2_src
	echo -e "Using libssh2 source in $libssh2_src.\n"
	# run libssh2 configure
	echo "Configuring libssh2..."
	set -x
	./configure --prefix=$stage_dir CFLAGS="$opt_flag$libssh2_opt_flag" LDFLAGS="$libssh2_ld_flag"
	set +x
	do_build "libssh2"
	echo -e "\nlibssh2 staged to $stage_dir.\n"
fi

# libssh2 install
echo -e "Starting libssh2 install.\n"
# deliver libssh2.so
copy_file "${dep_libdir}/$libssh2_minor" $libdir "libssh2"
# create the symlinks
if [[ $? == 0 ]]; then
	echo "Creating libssh2 symlinks for $libssh2_dso and $libssh2_major..."
	cd $libdir
	ln -s $libssh2_minor $libssh2_dso
	ln -s $libssh2_minor $libssh2_major
fi
echo -e "########################################################\n"

# Check for errors in component
check_error

# python build
if [[ $opt_python == y ]]; then
	echo "Starting python build."
	# change directories
	cd $python_src
	echo -e "\nUsing python source $python_src.\n"
	# run python configure
	echo -e "Configuring python..."
	# This is a hack to get python to build. I found it in a python bug report.
	# It seems to work...
	export SVNVERSION="Unversioned directory"
	set -x
	./configure --prefix=$stage_dir "$python_args"
	set +x
	do_build "python"
	echo -e "\npython staged to $stage_dir.\n"
fi

# python install
echo -e "Starting python install.\n"
# Ensure the dependency modules list exists
check_file $python_depmod "python"
if [[ $? == 0 ]]; then
	# create the python install directory
	if [[ ! -d ${libdir}/${python_inst_name} ]]; then
		mkdir ${libdir}/${python_inst_name}
	fi
	
	# copy all of the required python modules
	while read py_mod; do
		copy_file "${dep_libdir}/${python_inst_name}/${py_mod}" "${libdir}/${python_inst_name}" "python"
		# If this was a directory, we need to ensure .pyc and .pyo files do not get installed
		if [[ -d ${libdir}/${python_inst_name}/${py_mod} ]]; then
			rm -f ${libdir}/${python_inst_name}/${py_mod}/*.pyc
			rm -f ${libdir}/${python_inst_name}/${py_mod}/*.pyo
		fi
	done <$python_depmod

	# copy python pretty printers for gdb
	copy_file $python_gdb $libdir "python"
fi
echo -e "########################################################\n"

# Check for errors in component
check_error

# gdb build
if [[ $opt_gdb == y ]]; then
	echo "Starting gdb build."
	# change directories
	cd $gdb_src
	echo -e "\nUsing gdb source $gdb_src.\n"
	# run gdb configure
	echo "Configuring gdb..."
	set -x
	./configure --prefix=$stage_dir $gdb_args CFLAGS="$opt_flag"
	set +x
	do_build "gdb"
	echo -e "\ngdb staged to $stage_dir.\n"
	echo -e "Starting post build ops.\n"
	# ensure the old staging location for gdb files is removed
	if [[ -d $dep_libdir/$python_gdb_name ]]; then
		rm -rf $dep_libdir/$python_gdb_name
	fi
	mkdir $dep_libdir/$python_gdb_name
	# save the built files into the stage_dir for later
	copy_file $gdb_python_dir "${dep_libdir}/${python_gdb_name}/" "gdb"
	copy_file $gdb_syscalls_dir "${dep_libdir}/${python_gdb_name}/" "gdb"
	# remove the bad python file that causes errors
	if [[ -e ${dep_libdir}/${python_gdb_name}/${gdb_bad_file} ]]; then
		rm ${dep_libdir}/${python_gdb_name}/${gdb_bad_file}
	fi
	# rename gdb to gdb-guard
	rename_file "${dep_bindir}/gdb" "${dep_bindir}/gdb-guard" "gdb"
fi

# gdb install
echo -e "Starting gdb install.\n"
# deliver the built python files and gdb-guard
copy_file "${dep_libdir}/${python_gdb_name}" $libdir "gdb"
copy_file "${dep_bindir}/gdb-guard" $libexecdir "gdb"
echo -e "########################################################\n"

# Check for errors in component
check_error

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
	echo -e "\nUsing cuda-gdb 55 source $cuda55_gdb_src."
	echo -e "Using cuda 55 headers in $cuda55_include_dir.\n"
	# run cuda-gdb configure
	echo "Configuring cuda-gdb..."
	set -x
	./configure --prefix=$stage_dir $cuda_gdb_args CFLAGS="-I$cuda55_include_dir $opt_flag" LDFLAGS="-lpthread"
	set +x
	do_build "cuda-gdb"
	echo -e "\ncuda-gdb staged to $stage_dir.\n"
	echo -e "Starting post build ops.\n"
	# rename cuda-gdb to cuda55-gdb-guard
	rename_file "${dep_bindir}/cuda-gdb" "${dep_bindir}/cuda55-gdb-guard" "cuda-gdb 55"
fi

# cuda-gdb 55 install
echo -e "Starting cuda-gdb 55 install.\n"
# Deliver the cuda55-gdb-guard binary
copy_file "${dep_bindir}/cuda55-gdb-guard" $libexecdir "cuda-gdb 55"
echo -e "########################################################\n"

# Check for errors in component
check_error

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
	echo -e "\nUsing cuda-gdb 65 source $cuda65_gdb_src."
	echo -e "Using cuda 65 headers in $cuda65_include_dir.\n"
	# run cuda-gdb confiugre
	echo "Configuring cuda-gdb..."
	set -x
	./configure --prefix=$stage_dir $cuda_gdb_args CFLAGS="-I$cuda65_include_dir $opt_flag" LDFLAGS="-lpthread"
	set +x
	do_build "cuda-gdb"
	echo -e "\ncuda-gdb staged to $stage_dir.\n"
	echo -e "Starting post build ops.\n"
	# rename cuda-gdb to cuda65-gdb-guard
	rename_file "${dep_bindir}/cuda-gdb" "${dep_bindir}/cuda65-gdb-guard" "cuda-gdb 55"
fi

# cuda-gdb 65 install
echo -e "Starting cuda-gdb 65 install.\n"
# Deliver the cuda65-gdb-guard binary
copy_file "${dep_bindir}/cuda65-gdb-guard" $libexecdir "cuda-gdb 65"
echo -e "########################################################\n"

# Check for errors in component
check_error

# gdb uda support library build
if [[ $opt_gdb_uda == y ]]; then
	echo -e "Starting gdb uda support library build.\n"
	# change directories
	cd $gdb_uda_src
	# run gdb uda library make
	make --silent
	# deliver the gdb uda support library to libdir
	copy_file "${gdb_uda_src}/plugin/uda-plugin-cray-upc.so" $dep_libdir "uda plugin"
fi

# gdb uda support library install
echo -e "Starting gdb uda support library install.\n"
copy_file "${dep_libdir}/uda-plugin-cray-upc.so" $libdir "uda plugin"
echo -e "########################################################\n"

# Check for errors in component
check_error

# editline build
if [[ $opt_editline == y ]]; then
	echo -e "Starting editline build.\n"
	# change directories
	cd $editline_src
	echo -e "\nUsing editline source $editline_src.\n"
	# run editline configure
	echo "Configuring editline..."
	set -x
	./configure --prefix=$stage_dir CFLAGS="$opt_flag"
	set +x
	do_build "editline"
	echo -e "\nEditline staged to $stage_dir.\n"
fi

# editline install
echo -e "Starting editline install.\n"
copy_file "${dep_libdir}/${libedit_minor}" $libdir "editline"
# create the symlinks
if [[ $? == 0 ]]; then
	echo "Creating editline symlinks for $libedit_dso and $libedit_major..."
	# cd to libdir to create the symlinks
	cd $libdir
	ln -s $libedit_minor $libedit_dso
	ln -s $libedit_minor $libedit_major
fi
echo -e "########################################################\n"

# Check for errors in component
check_error

# libarchive build
if [[ $opt_libarchive == y ]]; then
	echo -e "Starting libarchive build.\n"
	# change directories
	cd $libarchive_src
	echo -e "Using libarchive source in $libarchive_src.\n"
	# run libarchive configure
	echo "Configuring libarchive..."
	set -x
	./configure --prefix=$stage_dir $libarchive_args CFLAGS="$opt_flag"
	set +x
	do_build "libarchive"
	echo -e "\nlibarchive staged to $stage_dir.\n"
	# check to see if libarchive.a was created
	check_file "${dep_libdir}/libarchive.a" "libarchive"
	echo -e "########################################################\n"
fi

# Check for errors in component
check_error

# tools interface build
if [[ $opt_cti == y ]]; then
	echo -e "Starting tools interface build.\n" 
	# change directories 
	cd $cti_src 
	echo -e "\nUsing tools interface source $cti_src.\n" 
	# run tools interface configure 
	echo "Configuring tools interface..." 
	set -x
	./configure --prefix=$stage_dir --with-libarchive=$stage_dir --with-libmi=$stage_dir CFLAGS="$opt_flag"
	set +x
	do_build "tools interface"
	echo -e "\nTools interface staged to $stage_dir.\n" 
fi
	
# tools interface install
echo -e "Starting tools interface install.\n"
copy_file "${dep_libexecdir}/${libcraytool_dlaunch}" $libexecdir "tools interface"
copy_file "${dep_libexecdir}/cti_starter" $libexecdir "tools interface"
copy_file "${dep_libexecdir}/cti_slurm_util" $libexecdir "tools interface"
copy_file "${dep_libexecdir}/cti_overwatch" $libexecdir "tools interface"
copy_file "${dep_libdir}/libaudit.so" $libdir "tools interface"
copy_file "${dep_libdir}/${libcraytool_backend_minor}" $libdir "tools interface"
# create the symlinks
if [[ $? == 0 ]]; then
	echo "Creating libcraytool symlinks for $libcraytool_backend_dso and $libcraytool_backend_major..." 
	cd $libdir 
	ln -s $libcraytool_backend_minor $libcraytool_backend_dso 
	ln -s $libcraytool_backend_minor $libcraytool_backend_major
fi
copy_file "${dep_libdir}/${libcraytool_frontend_minor}" $libdir "tools interface"
if [[ $? == 0 ]]; then
	echo "Creating libcraytool symlinks for $libcraytool_frontend_dso and $libcraytool_frontend_major..."
	cd $libdir 
	ln -s $libcraytool_frontend_minor $libcraytool_frontend_dso
	ln -s $libcraytool_frontend_minor $libcraytool_frontend_major
fi
echo -e "########################################################\n"

# Check for errors in component
check_error

# mrnet build - This *must* happen after the cti library build now since mrnet
# depends on cti components.
if [[ $opt_mrnet == y ]]; then
	echo "Starting MRNet build."
	# change directories
	cd $mrnet_src
	echo -e "\nUsing MRNet source in $mrnet_src.\n"
	# run mrnet configure
	echo "Configuring MRNet..."
	set -x
	./configure --prefix=$stage_dir --with-startup=cray --enable-shared \
	--with-cti-inc=$dep_includedir --with-cti-lib=$dep_libdir \
	$mrnet_args CFLAGS="$opt_flag" CXXFLAGS="$opt_flag"
	set +x
	do_build "MRNet"
	echo -e "\nMRNet staged to $stage_dir\n"
	echo -e "Starting post build ops.\n"
	# deliver the boost headers included with mrnet source if not done already
	# we don't want to error on failure to copy, so we do things in place here
	if [[ ! -d $dep_includedir/boost ]]; then
		if [[ -d $mrnet_src/external/boost/include/boost ]]; then
			echo "Delivering boost headers to $dep_includedir..."
			cp -r $mrnet_src/external/boost/include/boost $dep_includedir
		else
			echo "WARNING: Could not find the boost include files. Build may be incomplete."
			echo "The system is probably already providing these for free."
		fi
	fi
	# Deliver the mrnet_config.h file
	copy_file "${mrnet_src}/build/x86_64-unknown-linux-gnu/mrnet_config.h" $dep_includedir "MRNet"
	# Deliver the xplat_config.h file
	copy_file "${mrnet_src}/build/x86_64-unknown-linux-gnu/xplat_config.h" $dep_includedir "MRNet"
fi

# mrnet install
echo -e "Starting MRNet install.\n"
# Deliver the mrnet binaries
copy_file "${dep_bindir}/mrnet_commnode" $libexecdir "MRNet"
# Deliver the mrnet dso's to libdir 
copy_file "${dep_libdir}/${libmrnet_minor}" $libdir "MRNet"
if [[ $? == 0 ]]; then
	echo "Creating mrnet symlinks for $libmrnet_dso and $libmrnet_major..." 
	cd $libdir
	ln -s $libmrnet_minor $libmrnet_dso
	ln -s $libmrnet_minor $libmrnet_major
fi 
# Deliver the xplat dso's to libdir 
copy_file "${dep_libdir}/${libxplat_minor}" $libdir "MRNet"
if [[ $? == 0 ]]; then
	echo "Creating xplat symlinks for $libxplat_dso and $libxplat_major..."
	cd $libdir
	ln -s $libxplat_minor $libxplat_dso
	ln -s $libxplat_minor $libxplat_major
fi
echo -e "########################################################\n"

# Check for errors in component
check_error

# lgdb build
if [[ $opt_lgdb == y ]]; then
	echo -e "Starting lgdb build.\n"
	# change directories
	echo -e "Using guard source in $lgdb_src.\n"
	cd $lgdb_src
	# run lgdb configure
	echo "Configuring lgdb..."
	set -x
	./configure --with-aif=$stage_dir --with-rd --with-mrnet=$stage_dir --with-cray \
	--with-craytool-inc=$dep_includedir --with-craytool-lib=$dep_libdir --prefix=$inst_dir \
	--with-editline=$stage_dir --with-gdbmi=$stage_dir --with-libssh2=$stage_dir \
	"$perf_flag" "$be_perf_flag" "$dbg_flag" "$valgrind_flag" CFLAGS="$opt_flag" \
	CXXFLAGS="$opt_flag"
	set +x
	do_build "lgdb"
	echo -e "\nlgdb installed to $inst_dir.\n"
	echo -e "Starting post build ops.\n"
	# change the name of the guard binary to lgdb
	rename_file "${bindir}/guard" "${bindir}/lgdb" "lgdb"
	# ensure the guard_proxy was built
	check_file "${libexecdir}/guard_proxy" "lgdb"
	echo -e "\nDelivering lgdb modulefile..."
	check_file $module_primer "lgdb"
	if [[ $? == 0 ]]; then
		if [[ ! -d $inst_dir/modulefiles ]]; then
			mkdir $inst_dir/modulefiles
		fi
		if [[ ! -d $inst_dir/modulefiles/cray-lgdb ]]; then
			mkdir $inst_dir/modulefiles/cray-lgdb
		fi
		# deliver the fixed modulefile. Sed, you amaze me.
		cat $module_primer | sed -e "s@%HOME%@$inst_dir@" | sed -e "s@%VERSION%@$module_version@" \
		> $inst_dir/modulefiles/cray-lgdb/$module_version.DEV
	fi
	copy_file $demo_dir ${inst_dir} "lgdb"
	copy_file $docs_dir ${inst_dir} "lgdb"
	echo -e "########################################################\n"
fi

# Check for errors in component
check_error

# cleanup
if [[ $opt_cleanup == y ]]; then
	echo -e "Starting build cleanup.\n"
	do_clean $opt_aif $aif_src "libaif"
	do_clean $opt_libmi $libmi_src "libmi"
	do_clean $opt_libssh2 $libssh2_src "libssh2"
	do_clean $opt_mrnet $mrnet_src "MRNet"
	do_clean $opt_python $python_src "python"
	do_clean $opt_gdb $gdb_src "gdb"
	do_clean $opt_cuda55_gdb $cuda55_gdb_src "cuda-gdb 55"
	do_clean $opt_cuda65_gdb $cuda65_gdb_src "cuda-gdb 65"
	do_clean $opt_gdb_uda $gdb_uda_src "gdb uda support library"
	do_clean $opt_editline $editline_src "editline"
	do_clean $opt_libarchive $libarchive_src "libarchive"
	do_clean $opt_cti $cti_src "tools interface"
	# make clean lgdb
	if [[ $opt_lgdb == y ]]; then
		echo "Cleaning lgdb..."
		cd $lgdb_rd_src
		make --silent clean
		cd $lgdb_src
		make --silent clean
		make --silent distclean
	fi
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

