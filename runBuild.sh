#!/bin/bash
#
# runBuild.sh - Build steps for gdb4hpc
#
# Copyright 2010-2020 Hewlett Packard Enterprise Development LP.
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

top_level=$PWD

source ./external/cdst_build_library/build_lib

setup_modules

module load cray-cdst-support
check_exit_status

module load cray-cti-devel
check_exit_status

# Create install directory
source $top_level/release_versioning
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
mkdir -p $install_dir
check_exit_status

# ensure python3 exists
python_loc=$(command -v python3)
if [[ ! -x "$python_loc" ]]; then
    echo "python3 not found on system..."
    exit 1
fi

echo "############################################"
echo "#               Building libaif            #"
echo "############################################"

cd $top_level/libaif
check_exit_status

autoreconf -ifv
check_exit_status

CFLAGS="-g -O2 -Wall" ./configure --prefix=$install_dir
check_exit_status

make $cdst_j_flags
check_exit_status

make $cdst_j_flags install
check_exit_status

echo "############################################"
echo "#               Building libmi             #"
echo "############################################"

# ensure submodule is checked out
if [ ! -f $top_level/external/libmi/README ]
then
    echo "libmi submodule not checked out..."
    exit 1
fi

cd $top_level/external/libmi
check_exit_status

autoreconf -ifv
check_exit_status

CFLAGS="-g -O2 -Wall" ./configure --prefix=$install_dir
check_exit_status

make $cdst_j_flags
check_exit_status

make $cdst_j_flags install
check_exit_status

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

# ensure submodule is checked out
if [[ ! -f $top_level/external/ddt_gdb-8.1/README ]]; then
    echo "gdb submodule not checked out..."
    exit 1
fi

cd $top_level/external/ddt_gdb-8.1
check_exit_status

./configure --prefix=$install_dir \
            --with-gdb-datadir=$install_dir/share/gdb \
            --with-python=$python_loc \
            --disable-werror \
            --with-mmalloc \
            --without-babeltrace \
            --without-guile \
            --without-separate-debug-dir
check_exit_status

make $cdst_j_flags
check_exit_status

# gdb is part of binutils. We don't want all of binutils to install, only gdb.
cd $top_level/external/ddt_gdb-8.1/gdb
check_exit_status
make $cdst_j_flags install
check_exit_status

# deliver pretty-printers
cp -a $top_level/external/python_extras/gdb $install_dir/share/.
check_exit_status

# deliver gdb-guard to libexec dir
mkdir -p $install_dir/libexec
check_exit_status

cp -a $install_dir/bin/gdb $install_dir/libexec/gdb-guard
check_exit_status

arch=$(get_arch)
if [[ "$arch" == "$cdst_arch_aarch64" ]]; then
    echo "############################################"
    echo "#      aarch64, cudagdb not supported      #"
    echo "############################################"
else
    echo "############################################"
    echo "#          Building cuda gdb 10.1          #"
    echo "############################################"

    # we've already ensured python3 exists, just noting it's necessary here too.
    # ensure submodule is checked out
    if [[ ! -f $top_level/external/ddt_cuda-gdb-101/README ]]; then
        echo "cuda gdb 10.1 submodule not checked out..."
        exit 1
    fi

    cd $top_level/external/ddt_cuda-gdb-101
    check_exit_status

    cuda_install_dir=$install_dir/cuda_gdb_install
    ./configure --prefix=$cuda_install_dir \
                --with-gdb-datadir=$cuda_install_dir/share/cuda-gdb \
                --with-python=$python_loc \
                --program-prefix=cuda- \
                --enable-cuda \
                --enable-gdbserver=no \
                --enable-targets=x86_64-unknown-linux-gnu,arm-elf-linux-gnu,m68k-unknown-linux-gnu \
                CFLAGS="-Wno-shift-negative-value \
                    -Wno-unused-const-variable \
                    -I$top_level/external/pebuildenv/opt/cudatoolkit/10.1/include -g -O2" \
                LDFLAGS="-lpthread"
    check_exit_status

    make $cdst_j_flags
    check_exit_status

    # as in ddt_gdb-8.1, the cuda gdb build comes with LOTS of extra stuff: we don't
    # want all of cuda gdb to install, so we only install from the gdb directory.
    cd $top_level/external/ddt_cuda-gdb-101/gdb
    check_exit_status
    make $cdst_j_flags install
    check_exit_status

    # deliver cuda gdb to libexec dir
    cp -a $cuda_install_dir/bin/cuda-gdb $install_dir/libexec/cuda101-gdb-guard
    check_exit_status
fi

echo "############################################"
echo "#           Building uda plugin            #"
echo "############################################"

cd $top_level/external/gdb_uda
check_exit_status

CC=gcc make
check_exit_status

# deliver the gdb uda support library to libdir
cp -a $top_level/external/gdb_uda/plugin/uda-plugin-cray-upc.so $install_dir/lib
check_exit_status

echo "############################################"
echo "#              Building MRNet              #"
echo "############################################"

cti_pkgconfig_L_dir=$(pkg-config --libs-only-L common_tools_fe)
if [[ $? -ne 0 ]]; then
    echo "Could not find CTI in pkgconfig path. CTI may not be installed. Cannot continue with MRNet build."
    exit 1
fi

cti_pkgconfig_I_dir=$(pkg-config --cflags-only-I common_tools_fe)
if [[ $? -ne 0 ]]; then
    echo "Could not find CTI in pkgconfig path. CTI may not be installed. Cannot continue with MRNet build."
    exit 1
fi

cti_lib_directory=$(echo $cti_pkgconfig_L_dir | cut -c 3-)
cti_include_directory=$(echo $cti_pkgconfig_I_dir | cut -c 3-)

echo "cti_pkgconfig_L_dir:    $cti_pkgconfig_L_dir"
echo "cti_pkgconfig_I_dir:    $cti_pkgconfig_I_dir"
echo "cti_lib_directory:      $cti_lib_directory"
echo "cti_include_directory:  $cti_include_directory"

if [[ ! -d ${CRAY_CDST_SUPPORT_INSTALL_DIR} ]]; then
    echo "Could not find cray-cdst-support install path."
    exit 1
fi

# ensure submodule is checked out
if [[ ! -f $top_level/external/mrnet/README ]]; then
    echo "MRNet submodule not checked out..."
    exit 1
fi

cd $top_level/external/mrnet
check_exit_status

mrnet_cflags=$(pkg-config --cflags cray-cdst-support)
mrnet_cxxflags=${mrnet_cflags}
mrnet_ldflags="$(pkg-config --libs cray-cdst-support) "'-Wl,-rpath,\$$ORIGIN'
mrnet_soflags="-Wl,-rpath,/opt/cray/pe/gcc-libs"
mrnet_ctiflags=$(pkg-config --libs common_tools_be)
mrnet_flags="${mrnet_ldflags} ${mrnet_ctiflags}"

echo "mrnet_cflags:     $mrnet_cflags"
echo "mrnet_cxxflags:   $mrnet_cxxflags"
echo "mrnet_ldflags:    $mrnet_ldflags"
echo "mrnet_soflags:    $mrnet_soflags"
echo "mrnet_ctiflags:   $mrnet_ctiflags"

CFLAGS=${mrnet_cflags} CXXFLAGS=${mrnet_cxxflags} LDFLAGS=${mrnet_flags} SOFLAGS=${mrnet_soflags} ./configure --prefix=$install_dir --with-startup=cray-cti --enable-shared --with-craycti-inc=${cti_include_directory} --with-craycti-lib=${cti_lib_directory} --with-boost=${CRAY_CDST_SUPPORT_INSTALL_DIR}
check_exit_status

# parallel builds are broken with mrnet
make
check_exit_status

make install
check_exit_status

# deliver extra config files
cp -a $top_level/external/mrnet/build/*/xplat_config.h $install_dir/include/.
check_exit_status
cp -a $top_level/external/mrnet/build/*/mrnet_config.h $install_dir/include/.
check_exit_status

echo "############################################"
echo "#             Building zmqnet              #"
echo "############################################"

cd $top_level/external/zmqnet
check_exit_status

# hack: make sure that the zmqnet build script properly imports the build 
#       library for its own use.
unset CDST_capture_jenkins_build
unset CDST_cdst_config
unset CDST_check_exit
unset CDST_get_branch_info
unset CDST_get_os_info
unset CDST_install_common_pe

install_dir=$install_dir ./runBuildModule.sh
check_exit_status

echo "############################################"
echo "#             Building ompd plugin         #"
echo "############################################"

ompd_dir=$install_dir/share/ompd
ompd_build=$ompd_dir/build

mkdir -p $ompd_build
check_exit_status

mkdir -p $ompd_dir

cd $top_level/external/openmp/libompd/gdb-plugin
check_exit_status

cp ../../runtime/src/include/omp-tools.h.var omp-tools.h
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

echo "############################################"
echo "#             Building gdb4hpc             #"
echo "############################################"

cd $top_level/guard
check_exit_status

autoreconf -ifv
check_exit_status

PATH="$PATH:$install_dir/libexec" CFLAGS="-g -O2 -Wall" CXXFLAGS="-g -O2 -Wall" LDFLAGS=$(pkg-config --libs cray-cdst-support) ./configure --prefix=$install_dir --with-aif=$install_dir --with-rd --with-mrnet=$install_dir --with-gdbmi=$install_dir --with-libssh2=${CRAY_CDST_SUPPORT_INSTALL_DIR} --with-cray

check_exit_status

make $cdst_j_flags
check_exit_status

make $cdst_j_flags install
check_exit_status

# deliver manpage
mkdir -p $install_dir/share/man/man1
check_exit_status
cp -a $top_level/man/man1/gdb4hpc.1 $install_dir/share/man/man1/.
check_exit_status

echo "############################################"
echo "#              Done with build             #"
echo "############################################"

exit_with_status
