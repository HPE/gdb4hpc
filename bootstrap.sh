#!/bin/bash

# Driver script for installing the prerequisites for gdb4hpc.
#
# sudo ./bootstrap.sh --prep
#   installs the build requirments from the OS
# install_dir=[prefix] ./boostrap.sh
#    will build is and install the package


set -e

install_dir="${install_dir=${HOME}/hpe-tools}"

nodeps=1
nogdb=
prep=
reconfigure=
for arg in "$@"; do
    if [ $arg == "--nodeps" ]; then
	nodeps=1
    elif [ $arg == "--nogdb" ]; then
	nogdb=1
    elif [ $arg == "--reconfigure" ]; then
	reconfigure="--reconfigure"
    elif [ $arg == "--prep" ]; then
	prep=1
    else
	echo "usage $0 [--prep|--nodeps|--nogdb|--reconfigure]"
	exit 1
    fi
done

if [ -n "$prep" ]; then
    if [ "$(whoami)" != "root" ]; then
	echo "--prep must be run as root"
	exit 1
    fi
    echo prepping
    if [ -n "$(command -v dnf)" ]; then
	# some of the requirements need epel packages
	# sudo dnf config-manager --add-repo=https://arti.hpc.amslabs.hpecorp.net:443/artifactory/mirror-epel/epel/9/Everything/x86_64/
	# needed by cti
	dnf install gcc-toolset-14 meson cmake libssh2-devel boost-devel dyninst-devel pandoc 
	# local to zmqnet
	dnf install libsodium-devel libuuid-devel
	# specific to gdb4hpc
	dnf install elfutils-devel ncurses ncurses-devel bison python3-devel
    elif [ -n "$(command -v zypper)" ]; then
	# needed by CTI
	zypper install gcc14-c++ meson cmake libssh2-devel libboost_headers1_66_0-devel
	# specific to zmqnet
	zypper install libsodium-devel libuuid-devel
	# specfic to gdb4hpc
	zypper install ncurses ncurses-devel bison python3-devel
    else
	echo "could find neither dnf nor zypper to install"
	exit 1
    fi
    exit 0
fi

if [ $(gcc --version | grep -c '\<14\.') != 1 ]; then
    source /opt/rh/gcc-toolset-14/enable
fi    


echo "############################################"
echo "#            Setup environment.            #"
echo "############################################"

top_level="$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
libdir="$install_dir/lib/gdb4hpc"
libexecdir="$install_dir/libexec/gdb4hpc"
cdst_j_flags="-j 4"

echo "Install dir: \"$install_dir\""

mkdir -p $install_dir

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
    if [[ ! -f $top_level/external/$binutils_gdb/README ]]; then
	echo "gdb submodule not checked out..."
	exit 1
    fi

    cd $top_level/external/$binutils_gdb

    # this module doesn't like to rebuild
    git clean -dfx
    
    ./configure --prefix=$install_dir \
		--with-gdb-datadir=$install_dir/share/gdb \
		--with-python=$(command -v python3) \
		--disable-werror \
		--with-mmalloc \
		--without-babeltrace \
		--without-guile \
		--without-separate-debug-dir \
		CFLAGS="-O2" CXXFLAGS="-O2 --std=c++17 -Wno-template-id-cdtor"

    make $cdst_j_flags

    # gdb is part of binutils. We don't want all of binutils to install, only gdb.
    cd $top_level/external/$binutils_gdb/gdb
    make $cdst_j_flags install

    # deliver gdb-guard to libexec dir
    mkdir -p $install_dir/libexec/gdb4hpc

    cp -a $install_dir/bin/gdb $install_dir/libexec/gdb4hpc/gdb-guard
fi

if [ -z "$nodeps" ]; then
    echo "############################################"
    echo "#             Building ompd plugin         #"
    echo "############################################"

    ompd_dir=$install_dir/share/ompd
    ompd_build=$ompd_dir/build

    mkdir -p $ompd_build

    mkdir -p $ompd_dir

    cd $top_level/external/openmp/libompd/gdb-plugin

    cp ../../runtime/src/include/omp-tools.h.var omp-tools.h

    if [ ! -d buildenv ]; then
        python3 -m venv buildenv
	source ./buildenv/bin/activate
	python3 -m pip install wheel --trusted-host https://arti.hpc.amslabs.hpecorp.net \
		--extra-index-url https://arti.hpc.amslabs.hpecorp.net:443/artifactory/api/pypi/dst-pip-master-local/simple \
		--index-url https://arti.hpc.amslabs.hpecorp.net:443/artifactory/api/pypi/pypi-remote/simple
    else
	source ./buildenv/bin/activate
    fi

    python3 ./setup.py -v bdist_wheel -b $ompd_build -d $ompd_build
    python3 ./setup.py clean --all
    rm -R ompd.egg-info
    python3 -m pip install -U -t $ompd_build/python-module --no-index  . .
    rm omp-tools.h

    cd $ompd_dir
    mkdir -p gdb
    cp $ompd_build/python-module/ompd/*.* gdb
    rm -R build

    deactivate
fi

echo "############################################"
echo "#             Building gdb4hpc             #"
echo "############################################"

cd $top_level

# we explicitly set CC/CXX here to their full paths so that the full paths are
# baked into the meson build configuration and meson doesn't use whatever gcc/g++ is
# in PATH on subsequent builds outside of runBuild.sh
CC=$(which gcc) CXX=$(which g++) PKG_CONFIG_PATH="$install_dir/lib/cti/pkgconfig:$install_dir/lib/zmqnet/pkgconfig:$PKG_CONFIG_PATH" \
  meson setup \
    -Dprefix=$install_dir \
    -Dlibdir=lib/gdb4hpc \
    -Ddebug=true \
    -Doptimization=2 \
    -Db_ndebug=true \
    $reconfigure \
    ./build

if [ -z "$reconfigure" ]; then

    meson compile --verbose -C build

    meson install -C build
fi   

echo "############################################"
echo "#              Done with build             #"
echo "############################################"

