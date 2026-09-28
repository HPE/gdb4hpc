#!/bin/bash
#
# runBuild.sh - Build steps for gdb4hpc
#
# Copyright 2010-2024 Hewlett Packard Enterprise Development LP.
#
# Unpublished Proprietary Information.
# This unpublished work is protected to trade secret, copyright and other laws.
# Except as permitted by contract or express written permission of Hewlett Packard Enterprise Development LP.,
# no part of this work or its content may be used, reproduced or disclosed
# in any form.
#

echo "############################################"
echo "#            Setup environment.            #"
echo "############################################"

# directory of this script
hpe="$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
top_level="$hpe/.."

source "$hpe/external/cdst_build_library/build_lib_gcc"

setup_modules

module load cray-dyninst
check_exit_status

module load cray-cti-devel
check_exit_status

module load cray-zmqnet-devel
check_exit_status

module list

# Create install directory
source "$top_level/release_versioning"
gdb4hpc_version=
branch_name=$(get_branch_info)
branch_type=$(echo "$branch_name" | cut -d'/' -f1)
if [ "$branch_type" != "release" ]; then
  gdb4hpc_version="$major.$minor.$revision.$build_number"
else
  gdb4hpc_version="$major.$minor.$revision"
fi
echo "gdb4hpc version: \"$gdb4hpc_version\""
install_dir="/opt/cray/pe/gdb4hpc/$gdb4hpc_version"
echo "Install dir: \"$install_dir\""

if [[ ! -z $BRANCH_NAME ]]; then
  echo "cleaning build env"
  rm -rf /opt/cray/pe/gdb4hpc

  # Ensure our DST working directory is usable (safe) by user other than owner
  echo "Setting git safe directory . . ." 
  git config --global --add safe.directory /home/jenkins
  check_exit_status
fi

mkdir -p $install_dir
check_exit_status

nodeps=
nogdb=

for arg in "$@"; do
    if [ $arg == "--nodeps" ]; then
	nodeps=1
    elif [ $arg == "--nogdb" ]; then
	nogdb=1
    fi
done

echo "############################################"
echo "#    Copying subprojects                   #"
echo "############################################"

"$hpe/wrap.sh" tinystl doctest pythoncapi-compat
check_exit_status

if [ -z "$nodeps" -a -z "$nogdb" ]; then

    echo "############################################"
    echo "#               Building gdb               #"
    echo "############################################"
    #TODO:  Both the gdb abd cuda gdb builds take longer than we need; however,
    #       because of a non-recursive configure, both require make to run from the
    #       top in order to generate the makefiles for the gdb directory.  This is
    #       an area we could signficiantly speed up the build, but we're waiting on
    #       the release process to be defined properly; at that juncture it may be
    #       possible that we replace the gdb backend builds here with a separate rpm
    #       of gdb backends included in the build prep.

    binutils_gdb=binutils-gdb

    # ensure submodule is checked out
    if [[ ! -f $hpe/external/$binutils_gdb/README ]]; then
	echo "gdb submodule not checked out..."
	exit 1
    fi

    cd "$hpe/external/$binutils_gdb"
    check_exit_status

    # this module doesn't like to rebuild
    git clean -dfx
    check_exit_status
    
    ./configure --prefix=$install_dir \
		--with-gdb-datadir=$install_dir/share/gdb \
		--with-python=$(command -v python3) \
		--disable-werror \
		--with-mmalloc \
		--without-babeltrace \
		--without-guile \
		--without-separate-debug-dir \
		CFLAGS="-O2 -I$CRAY_DYNINST_INSTALL_DIR/include" CXXFLAGS="-O2 --std=c++17 -Wno-template-id-cdtor"
    check_exit_status

    make $cdst_j_flags
    check_exit_status

    # gdb is part of binutils. We don't want all of binutils to install, only gdb.
    cd "$hpe/external/$binutils_gdb/gdb"
    check_exit_status
    make $cdst_j_flags install
    check_exit_status

    # deliver pretty-printers
    cp -a $top_level/subprojects/python_extras/gdb $install_dir/share/.
    check_exit_status

    # deliver gdb-guard to libexec dir
    mkdir -p $install_dir/libexec
    check_exit_status

    cp -a $install_dir/bin/gdb $install_dir/libexec/gdb-guard
    check_exit_status

fi

if [ -z "$nodeps"]; then
    echo "############################################"
    echo "#             Building ompd plugin         #"
    echo "############################################"

    ompd_dir=$install_dir/share/ompd
    ompd_build=$ompd_dir/build

    mkdir -p $ompd_build
    check_exit_status

    mkdir -p $ompd_dir

    cd "$hpe/external/openmp/libompd/gdb-plugin"
    check_exit_status

    cp ../../runtime/src/include/omp-tools.h.var omp-tools.h
    check_exit_status

    if [ ! -d buildenv ]; then
        python3 -m venv buildenv
	source ./buildenv/bin/activate
	python3 -m pip install wheel --trusted-host https://arti.hpc.amslabs.hpecorp.net \
		--extra-index-url https://arti.hpc.amslabs.hpecorp.net:443/artifactory/api/pypi/dst-pip-master-local/simple \
		--index-url https://arti.hpc.amslabs.hpecorp.net:443/artifactory/api/pypi/pypi-remote/simple
	check_exit_status
    else
	source ./buildenv/bin/activate
    fi
    check_exit_status

    python3 ./setup.py -v bdist_wheel -b $ompd_build -d $ompd_build
    check_exit_status
    python3 ./setup.py clean --all
    check_exit_status
    rm -R ompd.egg-info
    check_exit_status
    python3 -m pip install -U -t $ompd_build/python-module --no-index  . .
    check_exit_status
    rm omp-tools.h
    check_exit_status

    cd $ompd_dir
    check_exit_status
    mkdir -p gdb
    check_exit_status
    cp $ompd_build/python-module/ompd/*.* gdb
    check_exit_status
    rm -R build
    check_exit_status

    deactivate
fi

echo "############################################"
echo "#             Building gdb4hpc             #"
echo "############################################"

cd $top_level
check_exit_status

echo $PKG_CONFIG_PATH
pkg-config --cflags cray-dyninst

# we explicitly set CC/CXX here to their full paths so that the full paths are
# baked into the meson build configuration and meson doesn't use whatever gcc/g++ is
# in PATH on subsequent builds outside of runBuild.sh
CC=$(which gcc) CXX=$(which g++) \
  meson setup \
    -Dprefix=$install_dir \
    -Dlibdir=lib \
    -Ddebug=true \
    -Doptimization=2 \
    -Db_ndebug=true \
    ./build
check_exit_status

meson compile --verbose -C build
check_exit_status

meson install -C build
check_exit_status

# deliver manpage
#mkdir -p $install_dir/share/man/man1
#check_exit_status
#cp -a $top_level/gdb4hpc/doc/man/gdb4hpc.1 $install_dir/share/man/man1/.
#check_exit_status

# deliver doc
#mkdir -p $install_dir/share/doc
#check_exit_status
#cp -a $top_level/gdb4hpc/doc/guides/*.md $install_dir/share/doc/.
#check_exit_status
#cp -a $top_level/gdb4hpc/doc/guides/*.html $install_dir/share/doc/.
#check_exit_status

echo "############################################"
echo "#              Done with build             #"
echo "############################################"

exit_with_status
