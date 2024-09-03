# Packaging definitions
%global pkgversion           %(%{_sourcedir}/get_package_data --crayversion)
%global branch               %(%{_sourcedir}/get_package_data --branch)
# Supported Python Version
%global python_version       %(%{_sourcedir}/get_package_data --pythonversion)
%global pkgversion_separator -
%global copyright_date       %(date +%%Y)
%global copyright            Certain components, files or programs contained within this package or product are Copyright 2007-%{copyright_date} Hewlett Packard Enterprise Development LP.

# RPM build time
%global release_date %(date +%%B\\ %%Y)

%global product gdb4hpc
%global cray_product %{product}
%global cray_name %{cray_product}

# Path definitions
%global cray_prefix /opt/cray/pe

#FIXME: This should be relocatable
%global external_build_dir %{cray_prefix}/%{cray_product}/%{pkgversion}

%global tests_source_dir %{_topdir}/../tests
%global demo_source_dir  %{_topdir}/../extras/demos

# modulefile definitions
%global modulefile_name      %{cray_name}
%global module_template_name %{modulefile_name}.module.template

# set_default modulefile definitions
%global set_default_command       set_default
%global set_default_template      set_default_template
%global set_default_path          admin-pe/set_default_files

# lmod modulefiles
%global lmod_template_gdb4hpc     template_%{product}.lua

# release notes
%global release_notes_file release_notes.md

# yaml file
%global yaml_template yaml.template
%global removal_date  %(date '+%Y-%m-%d' -d "+5 years")

# attributions file
%global attributions_name ATTRIBUTIONS_gdb4hpc.txt

# cdst-support version
%global cdst_support_pkgversion_min %(%{_sourcedir}/get_package_data --cdstversionmin)
%global cdst_support_pkgversion_max %(%{_sourcedir}/get_package_data --cdstversionmax)

# cti version
%global cti_pkgversion_min %(%{_sourcedir}/get_package_data --ctiversionmin)
%global cti_pkgversion_max %(%{_sourcedir}/get_package_data --ctiversionmax)

# mrnet version
%global mrnet_pkgversion_min %(%{_sourcedir}/get_package_data --mrnetversionmin)
%global mrnet_pkgversion_max %(%{_sourcedir}/get_package_data --mrnetversionmax)

# zmqnet version
%global zmqnet_pkgversion_min %(%{_sourcedir}/get_package_data --zmqnetversionmin)
%global zmqnet_pkgversion_max %(%{_sourcedir}/get_package_data --zmqnetversionmax)

# Disable debug package
%global debug_package %{nil}
%global _build_id_links none

# System strip command may be too old, use current path
%global __strip strip

# Filter requires - these all come from the cdst-support rpm
# These should match what is excluded in the cdst-support rpm specfile!
%global privlibs             libboost.*
%global privlibs %{privlibs}|libmrnet.*
%global privlibs %{privlibs}|libxplat.*
%global privlibs %{privlibs}|libssh2
%global privlibs %{privlibs}|libarchive
%global privlibs %{privlibs}|libsymtabAPI
%global privlibs %{privlibs}|libpcontrol
%global privlibs %{privlibs}|libdynElf
%global privlibs %{privlibs}|libdynDwarf
%global privlibs %{privlibs}|libcommon
%global privlibs %{privlibs}|libxplat.*
%global privlibs %{privlibs}|libedit
%global privlibs %{privlibs}|libdw
%global privlibs %{privlibs}|libebl.*
%global privlibs %{privlibs}|libelf
%global privlibs %{privlibs}|libtbbmalloc*
%global privlibs %{privlibs}|libtbb.*
%global privlibs %{privlibs}|libLrleBitset.*
%global privlibs %{privlibs}|libcapnp-0.8-dev.*
%global privlibs %{privlibs}|libgdb4_dbgagg.*
%global privlibs %{privlibs}|libkj-0.8-dev*
%global privlibs %{privlibs}|libzmq.*
%global privlibs %{privlibs}|libzmqnet_libsodium.*
%global privlibs %{privlibs}|libzmqnet.*
%global privlibs %{privlibs}|libgdb4_zmqnetagg.*
%global privlibs %{privlibs}|libstdc++.*
%global privlibs %{privlibs}|libgdb4_unittest.*
%global privlibs %{privlibs}|libgdb4_pybe_*.*
%global __requires_exclude ^(%{privlibs})\\.so*
%global __provides_exclude ^(%{privlibs})\\.so*

# Dist tags for SuSE need to be manually set
%if 0%{?suse_version}
%if 0%{?sle_version} == 150400
%global dist .sles15sp4
%endif
%if 0%{?sle_version} == 150500
%global dist .sles15sp5
%endif
%if 0%{?sle_version} == 150600
%global dist .sles15sp6
%endif
%global OS_HW_TAG 7.0
%global OS_WB_TAG sles15
%endif

%if "%{_arch}" == "aarch64"
%global SYS_HW_TAG AARCH64
%global SYS_WB_TAG AARCH64
%endif
%if "%{_arch}" == "x86_64"
%global SYS_HW_TAG HARDWARE
%global SYS_WB_TAG WHITEBOX
%endif
%if 0%{?rhel} == 8
%global OS_HW_TAG el8
%global OS_WB_TAG el8
%endif

Summary:    Cray gdb4hpc parallel debugger
Name:       %{cray_name}-%{pkgversion}
# BUILD_METADATA is set by Jenkins
Version:    %(echo ${BUILD_METADATA})
%if "%{branch}" == "release"
Release:    %(echo ${BUILD_NUMBER})%{dist}
%else
Release:    1%{dist}
%endif
Prefix:     %{cray_prefix}
License:    HPE Proprietary
Vendor:     Hewlett Packard Enterprise Development LP.
Group:      Development/Tools/Debuggers
Provides:   %{cray_name} = %{pkgversion}
Requires:   set_default_3, cray-cdst-support >= %{cdst_support_pkgversion_min}, cray-cdst-support < %{cdst_support_pkgversion_max}, cray-cti >= %{cti_pkgversion_min}, cray-cti < %{cti_pkgversion_max}, cray-mrnet >= %{mrnet_pkgversion_min}, cray-mrnet < %{mrnet_pkgversion_max}, cray-zmqnet >= %{zmqnet_pkgversion_min}, cray-zmqnet < %{zmqnet_pkgversion_max}
Source0:    %{module_template_name}
Source2:    %{release_notes_file}
Source3:    %{attributions_name}
Source4:    %{lmod_template_gdb4hpc}
Source5:    %{yaml_template}

%description
gdb4hpc is a gdb based parallel debugger used to debug MPI/SHMEM/UPC/Fortran 2008 applications written in C/C++/Fortran.

%package -n %{cray_name}-tests-%{pkgversion}
Summary:    Cray gdb4hpc parallel debugger tests
Group:      Development/Tools/Debuggers
Provides:   %{cray_name}-tests = %{pkgversion}
Requires:   %{cray_name} = %{pkgversion}
%description -n %{cray_name}-tests-%{pkgversion}
Tests for Cray gdb4hpc parallel debugger

%prep
# Run q(uiet) with build directory name, c(reate) subdirectory, disable T(arball) unpacking
%setup -q -n %{name} -c -T
%build
# external build
%{__sed} 's|<VERSION>|%{pkgversion}|g;s|<RELEASE>|%{version}-%{release}|g;s|<COPYRIGHT>|%{copyright}|g;s|<NAME>|%{cray_name}|g;s|<CRAY_PREFIX>|%{cray_prefix}|g;s|<DIST>|%{dist}.%{_target_cpu}|g;s|<ARCH>|%{_target_cpu}|g;s|<ATTRIBUTIONS_FILE_PATH>|%{cray_prefix}/%{product}/%{pkgversion}/%{attributions_name}|g;s|<cdst_version_range>|>= %{cdst_support_pkgversion_min}, < %{cdst_support_pkgversion_max}|g;s|<cti_version_range>|%{cti_pkgversion_min}, < %{cti_pkgversion_max}|g;s|<mrnet_version_range>|>= %{mrnet_pkgversion_min}, < %{mrnet_pkgversion_max}|g;s|<zmqnet_version_range>|>= %{zmqnet_pkgversion_min}, < %{zmqnet_pkgversion_max}|g' %{SOURCE2} > ${RPM_BUILD_DIR}/%{release_notes_file}
%{__sed} 's|<COPYRIGHT>|%{copyright}|g' %{SOURCE3} > ${RPM_BUILD_DIR}/%{attributions_name}

%install
# copy files from external install
#
# Cray PE package root
%{__install} -d ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}

# Information files
%{__cp} -a ${RPM_BUILD_DIR}/%{release_notes_file} ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/%{release_notes_file}
%{__cp} -a ${RPM_BUILD_DIR}/%{attributions_name} ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/%{attributions_name}

# Libraries
%{__install} -d ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/lib
%{__cp} -a %{external_build_dir}/lib/libgdb4_unittest.so ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/lib
%{__cp} -a %{external_build_dir}/lib/libgdb4_dbgagg.so ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/lib
%{__cp} -a %{external_build_dir}/lib/libgdb4_zmqnetagg.so ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/lib

# The pybe libraries require debug info to work - the rpm packaging process will strip them
# by default. We could disable *all* stripping in the package, but another way to disable
# stripping for only select files is to disguise them as non-binary files.
%{__gzip} -c %{external_build_dir}/lib/libgdb4_pybe_3.6.so > ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/lib/libgdb4_pybe_3.6.gz
%{__gzip} -c %{external_build_dir}/lib/libgdb4_pybe_3.8.so > ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/lib/libgdb4_pybe_3.8.gz
%{__gzip} -c %{external_build_dir}/lib/libgdb4_pybe_3.9.so > ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/lib/libgdb4_pybe_3.9.gz
%{__gzip} -c %{external_build_dir}/lib/libgdb4_pybe_3.10.so > ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/lib/libgdb4_pybe_3.10.gz
%{__gzip} -c %{external_build_dir}/lib/libgdb4_pybe_3.11.so > ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/lib/libgdb4_pybe_3.11.gz
%{__gzip} -c %{external_build_dir}/lib/libgdb4_pybe_3.12.so > ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/lib/libgdb4_pybe_3.12.gz

# Libraries static
# pkg-config
# Headers

# libexec binaries
%{__install} -d ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/libexec
%{__cp} -a %{external_build_dir}/libexec/dbgsrv ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/libexec
%{__cp} -a %{external_build_dir}/libexec/dbgsrv_alps ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/libexec
%{__cp} -a %{external_build_dir}/libexec/gdb-guard ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/libexec

# Binaries
%{__install} -d ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/bin
%{__cp} -a %{external_build_dir}/bin/gdb4hpc ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/bin

# ddt-gdb Share
%{__install} -d ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share
%{__install} -d ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/gdb
%{__install} -d ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python
%{__install} -d ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/boost
%{__cp} -a %{external_build_dir}/share/gdb/python/boost/__init__.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/boost
%{__cp} -a %{external_build_dir}/share/gdb/python/boost/printers.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/boost
%{__install} -d ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb
%{__cp} -a %{external_build_dir}/share/gdb/python/gdb/FrameDecorator.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb
%{__cp} -a %{external_build_dir}/share/gdb/python/gdb/FrameIterator.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb
%{__cp} -a %{external_build_dir}/share/gdb/python/gdb/frames.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb
%{__cp} -a %{external_build_dir}/share/gdb/python/gdb/__init__.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb
%{__cp} -a %{external_build_dir}/share/gdb/python/gdb/printing.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb
%{__cp} -a %{external_build_dir}/share/gdb/python/gdb/prompt.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb
%{__cp} -a %{external_build_dir}/share/gdb/python/gdb/types.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb
%{__cp} -a %{external_build_dir}/share/gdb/python/gdb/unwinder.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb
%{__cp} -a %{external_build_dir}/share/gdb/python/gdb/xmethod.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb
%{__install} -d ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb/command
%{__cp} -a %{external_build_dir}/share/gdb/python/gdb/command/explore.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb/command
%{__cp} -a %{external_build_dir}/share/gdb/python/gdb/command/frame_filters.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb/command
%{__cp} -a %{external_build_dir}/share/gdb/python/gdb/command/__init__.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb/command
%{__cp} -a %{external_build_dir}/share/gdb/python/gdb/command/pretty_printers.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb/command
%{__cp} -a %{external_build_dir}/share/gdb/python/gdb/command/prompt.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb/command
%{__cp} -a %{external_build_dir}/share/gdb/python/gdb/command/type_printers.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb/command
%{__cp} -a %{external_build_dir}/share/gdb/python/gdb/command/unwinders.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb/command
%{__cp} -a %{external_build_dir}/share/gdb/python/gdb/command/xmethods.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb/command
%{__install} -d ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb/function
%{__cp} -a %{external_build_dir}/share/gdb/python/gdb/function/as_string.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb/function
%{__cp} -a %{external_build_dir}/share/gdb/python/gdb/function/caller_is.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb/function
%{__cp} -a %{external_build_dir}/share/gdb/python/gdb/function/__init__.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb/function
%{__cp} -a %{external_build_dir}/share/gdb/python/gdb/function/strfns.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb/function
%{__install} -d ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb/printer
%{__cp} -a %{external_build_dir}/share/gdb/python/gdb/printer/bound_registers.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb/printer
%{__cp} -a %{external_build_dir}/share/gdb/python/gdb/printer/__init__.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb/printer
%{__install} -d ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/gdb/syscalls
%{__install} -d ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/libstdcxx
%{__cp} -a %{external_build_dir}/share/gdb/python/libstdcxx/__init__.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/libstdcxx
%{__install} -d ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/libstdcxx/v6
%{__cp} -a %{external_build_dir}/share/gdb/python/libstdcxx/v6/__init__.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/libstdcxx/v6
%{__cp} -a %{external_build_dir}/share/gdb/python/libstdcxx/v6/printers.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/libstdcxx/v6
%{__cp} -a %{external_build_dir}/share/gdb/syscalls/*.xml ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/gdb/syscalls
%{__cp} -a %{external_build_dir}/share/gdb/syscalls/gdb-syscalls.dtd ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/gdb/syscalls
%{__install} -d ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/gdb/system-gdbinit
%{__cp} -a %{external_build_dir}/share/gdb/system-gdbinit/elinos.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/gdb/system-gdbinit
%{__cp} -a %{external_build_dir}/share/gdb/system-gdbinit/wrs-linux.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/gdb/system-gdbinit

# ompd gdb plugin
%{__install} -d ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/ompd
%{__install} -d ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/ompd/gdb
%{__cp} -a %{external_build_dir}/share/ompd/gdb/frame_filter.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/ompd/gdb/frame_filter.py
%{__cp} -a %{external_build_dir}/share/ompd/gdb/__init__.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/ompd/gdb/__init__.py
%{__cp} -a %{external_build_dir}/share/ompd/gdb/ompd_address_space.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/ompd/gdb/ompd_address_space.py
%{__cp} -a %{external_build_dir}/share/ompd/gdb/ompd_callbacks.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/ompd/gdb/ompd_callbacks.py
%{__cp} -a %{external_build_dir}/share/ompd/gdb/ompd_handles.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/ompd/gdb/ompd_handles.py
%{__cp} -a %{external_build_dir}/share/ompd/gdb/ompdModule.cpython-%{python_version}-%{_arch}-linux-gnu.so ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/ompd/gdb/ompdModule.cpython-%{python_version}-%{_arch}-linux-gnu.so
%{__cp} -a %{external_build_dir}/share/ompd/gdb/ompd.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/ompd/gdb/ompd.py

# gdb plugin for debugging the cpython interpreter
%{__install} -d ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/py-gdb
%{__cp} -a %{external_build_dir}/share/py-gdb/python-gdb.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/py-gdb/python-gdb.py

# manpage
%{__install} -d ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/man/man1
%{__cp} -a %{external_build_dir}/share/man/man1/gdb4hpc.1 ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/man/man1

# doc
%{__install} -d ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/doc
%{__cp} -a %{external_build_dir}/share/doc/tutorial.md ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/doc
%{__cp} -a %{external_build_dir}/share/doc/tutorial.html ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/doc
%{__cp} -a %{external_build_dir}/share/doc/getting-started.md ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/doc
%{__cp} -a %{external_build_dir}/share/doc/getting-started.html ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/doc
%{__cp} -a %{external_build_dir}/share/doc/raja-support.md ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/doc
%{__cp} -a %{external_build_dir}/share/doc/raja-support.html ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/doc
%{__cp} -a %{external_build_dir}/share/doc/kokkos-support.md ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/doc
%{__cp} -a %{external_build_dir}/share/doc/kokkos-support.html ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/doc
%{__cp} -a %{external_build_dir}/share/doc/handling-arrays.md ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/doc
%{__cp} -a %{external_build_dir}/share/doc/handling-arrays.html ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/doc
%{__cp} -a %{external_build_dir}/share/doc/vscode-plugin.md ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/doc
%{__cp} -a %{external_build_dir}/share/doc/vscode-plugin.html ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/doc
%{__cp} -a %{external_build_dir}/share/doc/python-extension.md ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/doc
%{__cp} -a %{external_build_dir}/share/doc/python-extension.html ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/doc
%{__cp} -a %{external_build_dir}/share/doc/python-mode.md ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/doc
%{__cp} -a %{external_build_dir}/share/doc/python-mode.html ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/doc
%{__cp} -a %{external_build_dir}/share/doc/help.md ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/doc

# modulefile
%{__install} -d ${RPM_BUILD_ROOT}/%{prefix}/modulefiles/%{modulefile_name}
%{__sed} 's|<PREFIX>|%{external_build_dir}|g;s|<CRAY_NAME>|%{cray_name}|g;s|<VERSION>|%{pkgversion}|g;s|<COPYRIGHT>|%{copyright}|g' %{SOURCE0} > ${RPM_BUILD_ROOT}/%{prefix}/modulefiles/%{modulefile_name}/%{pkgversion}

# demo files
%{__install} -d ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/demos
%{__cp} -a %{demo_source_dir}/* ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/demos

# lmod modulefile
%{__install} -d ${RPM_BUILD_ROOT}/%{prefix}/lmod/modulefiles/core/%{cray_name}
%{__sed} 's|\[@%PREFIX_PATH%@\]|%{prefix}|g;s|\[@%MODULE_VERSION%@\]|%{pkgversion}|g;s|<COPYRIGHT>|%{copyright}|g' %{SOURCE4} > ${RPM_BUILD_ROOT}/%{prefix}/lmod/modulefiles/core/%{cray_name}/%{pkgversion}.lua

# set_default template
%{__sed} 's|\[product_name\]|%{cray_product}|g;s|\[version_string\]|%{pkgversion}|g;s|\[install_dir\]|%{prefix}|g;s|\[module_dir\]|%{prefix}/modulefiles|g;s|\[module_name_list\]|%{modulefile_name}|g;s|\[lmod_dir_list\]|%{prefix}/lmod/modulefiles/core|g;s|\[lmod_name_list\]|%{modulefile_name}|g' %{_sourcedir}/set_default/%{set_default_template} > ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/%{set_default_command}_%{cray_name}_%{pkgversion}

# set_default into admin-pe
%{__install} -d ${RPM_BUILD_ROOT}/%{prefix}/%{set_default_path}
%{__install} -D ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/%{set_default_command}_%{cray_name}_%{pkgversion} ${RPM_BUILD_ROOT}/%{prefix}/%{set_default_path}/%{set_default_command}_%{cray_name}_%{pkgversion}

# yaml file
%{__mkdir} -p %{_rpmdir}/%{_arch}
%global start_rmLine %(sed -n /section-3/= %{SOURCE5})
%global end_rmLine %(sed -n /admin-pe/= %{SOURCE5} | tail -1)
%global tests_rpm_name %{cray_name}-tests-%{pkgversion}-%{version}-%{release}.%{_arch}
%global rpm_list %{tests_rpm_name}.rpm
%if 0%{?rhel} == 8
%if "%{branch}" == "release"
%{__sed} 's|<PRODUCT>|%{cray_name}|g;s|<VERSION>|%{pkgversion}|g;s|<BUILD_METADATA>|%{version}|g;s|<RELEASE>|%{release}|g;s|<ARCH>|%{_arch}|g;s|<REMOVAL_DATE>|%{removal_date}|g;s|<SYS_HW_TAG>|%{SYS_HW_TAG}|g;s|<SYS_WB_TAG>|%{SYS_WB_TAG}|g;s|<OS_HW_TAG>|%{OS_HW_TAG}|g;s|<OS_WB_TAG>|%{OS_WB_TAG}|g;s|<RPM_LST>|%{rpm_list}|g;s|<TESTS_RPM_NAME>|%{tests_rpm_name}|g;s|<SET_DEFAULT_VALUE>|1|g' %{SOURCE5} > %{_rpmdir}/%{_arch}/%{cray_name}-%{pkgversion}-%{version}-%{release}.%{_arch}.rpm.yaml
%else
%{__sed} '%{start_rmLine},%{end_rmLine}d;s|section-5|section-3|g;s|<PRODUCT>|%{cray_name}|g;s|<VERSION>|%{pkgversion}|g;s|<BUILD_METADATA>|%{version}|g;s|<RELEASE>|%{release}|g;s|<ARCH>|%{_arch}|g;s|<REMOVAL_DATE>|%{removal_date}|g;s|<SYS_HW_TAG>|%{SYS_HW_TAG}|g;s|<SYS_WB_TAG>|%{SYS_WB_TAG}|g;s|<OS_HW_TAG>|%{OS_HW_TAG}|g;s|<OS_WB_TAG>|%{OS_WB_TAG}|g;s|<RPM_LST>|%{rpm_list}|g;s|<TESTS_RPM_NAME>|%{tests_rpm_name}|g;s|<SET_DEFAULT_VALUE>|0|g' %{SOURCE5} > %{_rpmdir}/%{_arch}/%{cray_name}-%{pkgversion}-%{version}-%{release}.%{_arch}.rpm.yaml
%endif
%endif

# tests
%{__install} -d ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests
%{__install} -d ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/src
%{__install} -d ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/src/c
%{__install} -d ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/src/raja
%{__install} -d ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/src/C++
%{__install} -d ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/src/fortran
%{__install} -d ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/src/kokkos
%{__install} -d ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/src/python
%{__install} -d ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data
%{__install} -d ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/rocm
%{__install} -d ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/cuda
%{__install} -d ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/include

%{__install} -d ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/cdst_test
%{__cp} -a %{tests_source_dir}/function/cdst-test/cdst_test/README.md                   ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/cdst_test/README.md
%{__cp} -a %{tests_source_dir}/function/cdst-test/cdst_test/README_config.md            ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/cdst_test/README_config.md
%{__cp} -a %{tests_source_dir}/function/cdst-test/cdst_test/README_textcheck.md         ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/cdst_test/README_textcheck.md
%{__cp} -a %{tests_source_dir}/function/cdst-test/cdst_test/__init__.py                 ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/cdst_test/__init__.py
%{__cp} -a %{tests_source_dir}/function/cdst-test/cdst_test/requirements.txt            ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/cdst_test/requirements.txt
%{__cp} -a %{tests_source_dir}/function/cdst-test/cdst_test/artifactory.py              ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/cdst_test/artifactory.py
%{__cp} -a %{tests_source_dir}/function/cdst-test/cdst_test/bootstrap.py                ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/cdst_test/bootstrap.py
%{__cp} -a %{tests_source_dir}/function/cdst-test/cdst_test/config.py                   ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/cdst_test/config.py
%{__cp} -a %{tests_source_dir}/function/cdst-test/cdst_test/config_default.py           ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/cdst_test/config_default.py
%{__cp} -a %{tests_source_dir}/function/cdst-test/cdst_test/config_nodes.py             ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/cdst_test/config_nodes.py
%{__cp} -a %{tests_source_dir}/function/cdst-test/cdst_test/env.py                      ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/cdst_test/env.py
%{__cp} -a %{tests_source_dir}/function/cdst-test/cdst_test/files.py                    ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/cdst_test/files.py
%{__cp} -a %{tests_source_dir}/function/cdst-test/cdst_test/job_environment.py          ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/cdst_test/job_environment.py
%{__cp} -a %{tests_source_dir}/function/cdst-test/cdst_test/log.py                      ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/cdst_test/log.py
%{__cp} -a %{tests_source_dir}/function/cdst-test/cdst_test/modules.py                  ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/cdst_test/modules.py
%{__cp} -a %{tests_source_dir}/function/cdst-test/cdst_test/remote_test_tool_startup.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/cdst_test/remote_test_tool_startup.py
%{__cp} -a %{tests_source_dir}/function/cdst-test/cdst_test/remote_test_tool_ui.py      ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/cdst_test/remote_test_tool_ui.py
%{__cp} -a %{tests_source_dir}/function/cdst-test/cdst_test/ssh.py                      ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/cdst_test/ssh.py
%{__cp} -a %{tests_source_dir}/function/cdst-test/cdst_test/test_server.py              ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/cdst_test/test_server.py
%{__cp} -a %{tests_source_dir}/function/cdst-test/cdst_test/test_server_commands.py     ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/cdst_test/test_server_commands.py
%{__cp} -a %{tests_source_dir}/function/cdst-test/cdst_test/test_server_proto.py        ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/cdst_test/test_server_proto.py
%{__cp} -a %{tests_source_dir}/function/cdst-test/cdst_test/textcheck.py                ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/cdst_test/textcheck.py
%{__cp} -a %{tests_source_dir}/function/cdst-test/cdst_test/useful.py                   ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/cdst_test/useful.py

%{__install} -d ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/cdst_test/data
%{__cp} -a %{tests_source_dir}/function/cdst-test/cdst_test/data/hosts.yaml    ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/cdst_test/data/hosts.yaml
%{__cp} -a %{tests_source_dir}/function/cdst-test/cdst_test/data/products.yaml ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/cdst_test/data/products.yaml

%{__install} -d ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/cdst_test/scripts
%{__cp} -a %{tests_source_dir}/function/cdst-test/cdst_test/scripts/avo_config.py        ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/cdst_test/scripts/avo_config.py
%{__cp} -a %{tests_source_dir}/function/cdst-test/cdst_test/scripts/passwordedscp.expect ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/cdst_test/scripts/passwordedscp.expect
%{__cp} -a %{tests_source_dir}/function/cdst-test/cdst_test/scripts/passwordedssh.expect ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/cdst_test/scripts/passwordedssh.expect
%{__cp} -a %{tests_source_dir}/function/cdst-test/cdst_test/scripts/sshcopyid.expect     ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/cdst_test/scripts/sshcopyid.expect
%{__cp} -a %{tests_source_dir}/function/cdst-test/cdst_test/scripts/uai.expect           ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/cdst_test/scripts/uai.expect
%{__cp} -a %{tests_source_dir}/function/cdst-test/cdst_test/scripts/validate_ssh.sh      ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/cdst_test/scripts/validate_ssh.sh
%{__cp} -a %{tests_source_dir}/function/cdst-test/cdst_test/scripts/detect_wlm.sh        ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/cdst_test/scripts/detect_wlm.sh

%{__install} -d ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/cdst_test/src
%{__cp} -a %{tests_source_dir}/function/cdst-test/cdst_test/src/Makefile    ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/cdst_test/src/Makefile
%{__cp} -a %{tests_source_dir}/function/cdst-test/cdst_test/src/hello_mpi.c ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/cdst_test/src/hello_mpi.c

%{__cp} -a %{tests_source_dir}/function/cdst-test/test_tool.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/test_tool.py
%{__cp} -a %{tests_source_dir}/function/tests.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py
%{__cp} -a %{tests_source_dir}/function/libtests.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/libtests.py
%{__cp} -a %{tests_source_dir}/function/avocado_config.yaml ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/avocado_config.yaml
%{__cp} -a %{tests_source_dir}/function/avocado_config_fullsuite.yaml ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/avocado_config_fullsuite.yaml
%{__cp} -a %{tests_source_dir}/function/avocado_config_python.yaml ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/avocado_config_python.yaml
%{__cp} -a %{tests_source_dir}/function/test_tool_config.yaml ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/test_tool_config.yaml
%{__cp} -a %{tests_source_dir}/function/Makefile ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/Makefile
%{__cp} -a %{tests_source_dir}/function/README.md ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/README.md

%{__cp} -a %{tests_source_dir}/function/src/cti_launch.cpp ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/src/cti_launch.cpp

%{__cp} -a %{tests_source_dir}/function/src/c/c_cmp_diff.c ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/src/c/c_cmp_diff.c
%{__cp} -a %{tests_source_dir}/function/src/c/c_signal.c ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/src/c/c_signal.c
%{__cp} -a %{tests_source_dir}/function/src/c/c_type_pause.c ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/src/c/c_type_pause.c
%{__cp} -a %{tests_source_dir}/function/src/c/c_type_test.c ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/src/c/c_type_test.c
%{__cp} -a %{tests_source_dir}/function/src/c/mtest.c ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/src/c/mtest.c
%{__cp} -a %{tests_source_dir}/function/src/c/wtime.c ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/src/c/wtime.c
%{__cp} -a %{tests_source_dir}/function/src/c/c_dlopen_test.c ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/src/c/c_dlopen_test.c
%{__cp} -a %{tests_source_dir}/function/src/c/libdlopen_test.c ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/src/c/libdlopen_test.c
%{__cp} -a %{tests_source_dir}/function/src/c/c_segv.c ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/src/c/c_segv.c
%{__cp} -a %{tests_source_dir}/function/src/c/c_list.c ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/src/c/c_list.c
%{__cp} -a %{tests_source_dir}/function/src/c/c_half_exit.c ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/src/c/c_half_exit.c
%{__cp} -a %{tests_source_dir}/function/src/c/c_ompd.c ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/src/c/c_ompd.c
%{__cp} -a %{tests_source_dir}/function/src/c/c_inc.c ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/src/c/c_inc.c
%{__cp} -a %{tests_source_dir}/function/src/c/libfoo.c ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/src/c/libfoo.c
%{__cp} -a %{tests_source_dir}/function/src/c/libbar.c ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/src/c/libbar.c
%{__cp} -a %{tests_source_dir}/function/src/c/simple_gpu.c ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/src/c/simple_gpu.c
%{__cp} -a %{tests_source_dir}/function/src/c/c_shmem.c ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/src/c/c_shmem.c
%{__cp} -a %{tests_source_dir}/function/src/c/c_not_mpi.c ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/src/c/c_not_mpi.c
%{__cp} -a %{tests_source_dir}/function/src/c/c_nolib.c ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/src/c/c_nolib.c
%{__cp} -a %{tests_source_dir}/function/src/c/c_libnolib.c ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/src/c/c_libnolib.c

%{__cp} -a %{tests_source_dir}/function/src/C++/cpp_arrays.cpp ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/src/C++/cpp_arrays.cpp
%{__cp} -a %{tests_source_dir}/function/src/C++/cpp_arrays_print.cpp ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/src/C++/cpp_arrays_print.cpp
%{__cp} -a %{tests_source_dir}/function/src/C++/cpp_bd_test.cpp ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/src/C++/cpp_bd_test.cpp
%{__cp} -a %{tests_source_dir}/function/src/C++/cpp_bt.cpp ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/src/C++/cpp_bt.cpp
%{__cp} -a %{tests_source_dir}/function/src/C++/cpp_bt_stress.cpp ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/src/C++/cpp_bt_stress.cpp
%{__cp} -a %{tests_source_dir}/function/src/C++/cpp_catch.cpp ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/src/C++/cpp_catch.cpp
%{__cp} -a %{tests_source_dir}/function/src/C++/cpp_cmp_diff.cpp ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/src/C++/cpp_cmp_diff.cpp
%{__cp} -a %{tests_source_dir}/function/src/C++/cpp_decomp2.cpp ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/src/C++/cpp_decomp2.cpp
%{__cp} -a %{tests_source_dir}/function/src/C++/cpp_decomp.cpp ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/src/C++/cpp_decomp.cpp
%{__cp} -a %{tests_source_dir}/function/src/C++/cpp_ds_test1.cpp ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/src/C++/cpp_ds_test1.cpp
%{__cp} -a %{tests_source_dir}/function/src/C++/cpp_ds_test2.cpp ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/src/C++/cpp_ds_test2.cpp
%{__cp} -a %{tests_source_dir}/function/src/C++/cpp_parallel_threads.cpp ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/src/C++/cpp_parallel_threads.cpp
%{__cp} -a %{tests_source_dir}/function/src/C++/cpp_points.cpp ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/src/C++/cpp_points.cpp
%{__cp} -a %{tests_source_dir}/function/src/C++/cpp_threads.cpp ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/src/C++/cpp_threads.cpp
%{__cp} -a %{tests_source_dir}/function/src/C++/cpp_attach.cpp ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/src/C++/cpp_attach.cpp

%{__cp} -a %{tests_source_dir}/function/src/fortran/ftn_arrays.f90 ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/src/fortran/ftn_arrays.f90
%{__cp} -a %{tests_source_dir}/function/src/fortran/ftn_bd_dt.f90 ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/src/fortran/ftn_bd_dt.f90
%{__cp} -a %{tests_source_dir}/function/src/fortran/ftn_cmp_diff.f90 ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/src/fortran/ftn_cmp_diff.f90
%{__cp} -a %{tests_source_dir}/function/src/fortran/ftn_decomp.f90 ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/src/fortran/ftn_decomp.f90
%{__cp} -a %{tests_source_dir}/function/src/fortran/ftn_omp.f90 ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/src/fortran/ftn_omp.f90

%{__cp} -a %{tests_source_dir}/function/src/raja/view-layout_solution.cpp                         ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/src/raja/view-layout_solution.cpp
%{__cp} -a %{tests_source_dir}/function/src/raja/view-layout-decomp_solution.cpp                  ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/src/raja/view-layout-decomp_solution.cpp
%{__cp} -a %{tests_source_dir}/function/src/raja/kernel-matrix-transpose-local-array_solution.cpp ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/src/raja/kernel-matrix-transpose-local-array_solution.cpp
%{__cp} -a %{tests_source_dir}/function/src/raja/launch-matrix-transpose-local-array_solution.cpp ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/src/raja/launch-matrix-transpose-local-array_solution.cpp
%{__cp} -a %{tests_source_dir}/function/src/raja/reductions_solution.cpp                          ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/src/raja/reductions_solution.cpp

%{__cp} -a %{tests_source_dir}/function/src/kokkos/team_vector_loop.cpp ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/src/kokkos/team_vector_loop.cpp
%{__cp} -a %{tests_source_dir}/function/src/kokkos/Makefile ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/src/kokkos/Makefile

%{__cp} -a %{tests_source_dir}/function/src/python/sleep.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/src/python/sleep.py
%{__cp} -a %{tests_source_dir}/function/src/python/spin.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/src/python/spin.py

%{__cp} -a %{tests_source_dir}/function/tests.py.data/assert_scalars.in       ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/assert_scalars.in
%{__cp} -a %{tests_source_dir}/function/tests.py.data/assert_expressions.in   ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/assert_expressions.in
%{__cp} -a %{tests_source_dir}/function/tests.py.data/assert_arrays.in        ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/assert_arrays.in
%{__cp} -a %{tests_source_dir}/function/tests.py.data/assert_decomp.in        ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/assert_decomp.in
%{__cp} -a %{tests_source_dir}/function/tests.py.data/assert_deadlock.in      ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/assert_deadlock.in
%{__cp} -a %{tests_source_dir}/function/tests.py.data/assert_interrupted.in   ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/assert_interrupted.in
%{__cp} -a %{tests_source_dir}/function/tests.py.data/assert_errors.in        ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/assert_errors.in
%{__cp} -a %{tests_source_dir}/function/tests.py.data/assert_save.in          ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/assert_save.in
%{__cp} -a %{tests_source_dir}/function/tests.py.data/assert_stopstart.in     ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/assert_stopstart.in
%{__cp} -a %{tests_source_dir}/function/tests.py.data/assert_twice.in         ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/assert_twice.in
%{__cp} -a %{tests_source_dir}/function/tests.py.data/assert_invalidated.in   ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/assert_invalidated.in
%{__cp} -a %{tests_source_dir}/function/tests.py.data/command_timeout.in      ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/command_timeout.in
%{__cp} -a %{tests_source_dir}/function/tests.py.data/c_assign.in             ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_assign.in
%{__cp} -a %{tests_source_dir}/function/tests.py.data/c_assign_decomp.in      ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_assign_decomp.in
%{__cp} -a %{tests_source_dir}/function/tests.py.data/c_breakpoint.in         ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_breakpoint.in
%{__cp} -a %{tests_source_dir}/function/tests.py.data/c_breakpoint_pending.in ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_breakpoint_pending.in
%{__cp} -a %{tests_source_dir}/function/tests.py.data/c_breakpoint_save.in    ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_breakpoint_save.in
%{__cp} -a %{tests_source_dir}/function/tests.py.data/c_checkpoint0.in        ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_checkpoint0.in
%{__cp} -a %{tests_source_dir}/function/tests.py.data/c_checkpoint1.in        ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_checkpoint1.in
%{__cp} -a %{tests_source_dir}/function/tests.py.data/c_checkpoint2.in        ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_checkpoint2.in
%{__cp} -a %{tests_source_dir}/function/tests.py.data/c_cmp_false.in          ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_cmp_false.in
%{__cp} -a %{tests_source_dir}/function/tests.py.data/c_cmp_true.in           ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_cmp_true.in
%{__cp} -a %{tests_source_dir}/function/tests.py.data/c_commnode_fanout.in    ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_commnode_fanout.in
%{__cp} -a %{tests_source_dir}/function/tests.py.data/c_defset.in             ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_defset.in
%{__cp} -a %{tests_source_dir}/function/tests.py.data/c_finish.in             ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_finish.in
%{__cp} -a %{tests_source_dir}/function/tests.py.data/c_focus.in              ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_focus.in
%{__cp} -a %{tests_source_dir}/function/tests.py.data/c_frame.in              ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_frame.in
%{__cp} -a %{tests_source_dir}/function/tests.py.data/c_gdbmode.in            ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_gdbmode.in
%{__cp} -a %{tests_source_dir}/function/tests.py.data/c_help.in               ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_help.in
%{__cp} -a %{tests_source_dir}/function/tests.py.data/c_half_exit.in          ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_half_exit.in
%{__cp} -a %{tests_source_dir}/function/tests.py.data/c_ompd.in               ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_ompd.in
%{__cp} -a %{tests_source_dir}/function/tests.py.data/c_info.in               ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_info.in
%{__cp} -a %{tests_source_dir}/function/tests.py.data/c_kill.in               ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_kill.in
%{__cp} -a %{tests_source_dir}/function/tests.py.data/c_list.in               ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_list.in
%{__cp} -a %{tests_source_dir}/function/tests.py.data/c_next.in               ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_next.in
%{__cp} -a %{tests_source_dir}/function/tests.py.data/cpp_arrays_compare.in   ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/cpp_arrays_compare.in
%{__cp} -a %{tests_source_dir}/function/tests.py.data/cpp_arrays_chunk.in   ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/cpp_arrays_chunk.in
%{__cp} -a %{tests_source_dir}/function/tests.py.data/cpp_arrays_print.in     ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/cpp_arrays_print.in
%{__cp} -a %{tests_source_dir}/function/tests.py.data/cpp_bt.in               ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/cpp_bt.in
%{__cp} -a %{tests_source_dir}/function/tests.py.data/cpp_catch.in            ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/cpp_catch.in
%{__cp} -a %{tests_source_dir}/function/tests.py.data/cpp_decomp2.in          ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/cpp_decomp2.in
%{__cp} -a %{tests_source_dir}/function/tests.py.data/cpp_decomp.in           ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/cpp_decomp.in
%{__cp} -a %{tests_source_dir}/function/tests.py.data/cpp_thread_funcs.in     ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/cpp_thread_funcs.in
%{__cp} -a %{tests_source_dir}/function/tests.py.data/cpp_thread_focus.in     ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/cpp_thread_focus.in
%{__cp} -a %{tests_source_dir}/function/tests.py.data/cpp_attach.in           ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/cpp_attach.in
%{__cp} -a %{tests_source_dir}/function/tests.py.data/c_print.in              ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_print.in
%{__cp} -a %{tests_source_dir}/function/tests.py.data/c_release.in            ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_release.in
%{__cp} -a %{tests_source_dir}/function/tests.py.data/c_run.in                ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_run.in
%{__cp} -a %{tests_source_dir}/function/tests.py.data/c_run_not_mpi.in        ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_run_not_mpi.in
%{__cp} -a %{tests_source_dir}/function/tests.py.data/c_set_unset.in          ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_set_unset.in
%{__cp} -a %{tests_source_dir}/function/tests.py.data/c_show.in               ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_show.in
%{__cp} -a %{tests_source_dir}/function/tests.py.data/c_signal.in             ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_signal.in
%{__cp} -a %{tests_source_dir}/function/tests.py.data/c_sizeof.in             ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_sizeof.in
%{__cp} -a %{tests_source_dir}/function/tests.py.data/c_step.in               ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_step.in
%{__cp} -a %{tests_source_dir}/function/tests.py.data/c_up_down.in            ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_up_down.in
%{__cp} -a %{tests_source_dir}/function/tests.py.data/c_viewset.in            ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_viewset.in
%{__cp} -a %{tests_source_dir}/function/tests.py.data/c_whatis.in             ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_whatis.in
%{__cp} -a %{tests_source_dir}/function/tests.py.data/c_segv.in               ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_segv.in
%{__cp} -a %{tests_source_dir}/function/tests.py.data/c_shmem.in               ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_shmem.in
%{__cp} -a %{tests_source_dir}/function/tests.py.data/c_mpmd.in               ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_mpmd.in
%{__cp} -a %{tests_source_dir}/function/tests.py.data/c_nolib.in             ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_nolib.in
%{__cp} -a %{tests_source_dir}/function/tests.py.data/ftn_arrays.in           ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/ftn_arrays.in
%{__cp} -a %{tests_source_dir}/function/tests.py.data/ftn_decomp.in           ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/ftn_decomp.in
%{__cp} -a %{tests_source_dir}/function/tests.py.data/ftn_rocmGpu.in           ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/ftn_rocmGpu.in
%{__cp} -a %{tests_source_dir}/function/tests.py.data/cpp_ds1.in         ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/cpp_ds1.in
%{__cp} -a %{tests_source_dir}/function/tests.py.data/cpp_t1.in          ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/cpp_t1.in
%{__cp} -a %{tests_source_dir}/function/tests.py.data/cpp_t2.in          ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/cpp_t2.in
%{__cp} -a %{tests_source_dir}/function/tests.py.data/cpp_t3.in          ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/cpp_t3.in
%{__cp} -a %{tests_source_dir}/function/tests.py.data/ftn_t1.in          ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/ftn_t1.in
%{__cp} -a %{tests_source_dir}/function/tests.py.data/ftn_t2.in          ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/ftn_t2.in
%{__cp} -a %{tests_source_dir}/function/tests.py.data/ftn_t3.in          ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/ftn_t3.in
%{__cp} -a %{tests_source_dir}/function/tests.py.data/decomp_save.in          ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/decomp_save.in
%{__cp} -a %{tests_source_dir}/function/tests.py.data/sbatch.in               ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/sbatch.in
%{__cp} -a %{tests_source_dir}/function/tests.py.data/source.in               ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/source.in
%{__cp} -a %{tests_source_dir}/function/tests.py.data/sourceme.rc             ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/sourceme.rc
%{__cp} -a %{tests_source_dir}/function/tests.py.data/rocmGpu.in              ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/rocmGpu.in
%{__cp} -a %{tests_source_dir}/function/tests.py.data/cudaGpu.in              ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/cudaGpu.in
%{__cp} -a %{tests_source_dir}/function/tests.py.data/qsub.template           ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/qsub.template
%{__cp} -a %{tests_source_dir}/function/tests.py.data/rocm/pinoak-login1.sh   ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/rocm/pinoak-login1.sh
%{__cp} -a %{tests_source_dir}/function/tests.py.data/rocm/pea2k.sh   ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/rocm/pea2k.sh
%{__cp} -a %{tests_source_dir}/function/tests.py.data/cuda/pegasus.sh         ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/cuda/pegasus.sh
%{__cp} -a %{tests_source_dir}/function/tests.py.data/raja_views.in           ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/raja_views.in
%{__cp} -a %{tests_source_dir}/function/tests.py.data/raja_tile.in            ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/raja_tile.in
%{__cp} -a %{tests_source_dir}/function/tests.py.data/raja_decomp.in          ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/raja_decomp.in
%{__cp} -a %{tests_source_dir}/function/tests.py.data/raja_reductions.in      ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/raja_reductions.in
%{__cp} -a %{tests_source_dir}/function/tests.py.data/save_all.in             ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/save_all.in
%{__cp} -a %{tests_source_dir}/function/tests.py.data/kokkos.in      ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/kokkos.in
%{__cp} -a %{tests_source_dir}/function/tests.py.data/mi_basics_console_exec.in   ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/mi_basics_console_exec.in
%{__cp} -a %{tests_source_dir}/function/tests.py.data/mi_basics_errors.in         ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/mi_basics_errors.in
%{__cp} -a %{tests_source_dir}/function/tests.py.data/mi_basics_exec.in           ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/mi_basics_exec.in
%{__cp} -a %{tests_source_dir}/function/tests.py.data/mi_basics_fallback.in       ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/mi_basics_fallback.in
%{__cp} -a %{tests_source_dir}/function/tests.py.data/mi_basics_prefixes.in       ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/mi_basics_prefixes.in
%{__cp} -a %{tests_source_dir}/function/tests.py.data/mi_basics_prompt.in         ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/mi_basics_prompt.in
%{__cp} -a %{tests_source_dir}/function/tests.py.data/mi_basics_results.in        ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/mi_basics_results.in
%{__cp} -a %{tests_source_dir}/function/tests.py.data/mi_backtrace.in             ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/mi_backtrace.in
%{__cp} -a %{tests_source_dir}/function/tests.py.data/mi_ccdb_breakpoint.in       ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/mi_ccdb_breakpoint.in
%{__cp} -a %{tests_source_dir}/function/tests.py.data/mi_ccdb_compare.in          ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/mi_ccdb_compare.in
%{__cp} -a %{tests_source_dir}/function/tests.py.data/mi_ccdb_watch_mouse.in      ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/mi_ccdb_watch_mouse.in
%{__cp} -a %{tests_source_dir}/function/tests.py.data/mi_ccdb_step_next_finish.in ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/mi_ccdb_step_next_finish.in
%{__cp} -a %{tests_source_dir}/function/tests.py.data/mi_ccdb_assert.in           ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/mi_ccdb_assert.in
%{__cp} -a %{tests_source_dir}/function/tests.py.data/completions_nolaunch.in     ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/completions_nolaunch.in
%{__cp} -a %{tests_source_dir}/function/tests.py.data/completions_launch.in       ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/completions_launch.in
%{__cp} -a %{tests_source_dir}/function/tests.py.data/launch_from_path.in         ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/launch_from_path.in
%{__cp} -a %{tests_source_dir}/function/tests.py.data/positional_args.in          ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/positional_args.in
%{__cp} -a %{tests_source_dir}/function/tests.py.data/python_attach_detect_36.in               ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/python_attach_detect_36.in
%{__cp} -a %{tests_source_dir}/function/tests.py.data/python_attach_halt_bt_active_allstop.in  ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/python_attach_halt_bt_active_allstop.in
%{__cp} -a %{tests_source_dir}/function/tests.py.data/python_attach_halt_bt_dormant_allstop.in ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/python_attach_halt_bt_dormant_allstop.in
%{__cp} -a %{tests_source_dir}/function/tests.py.data/python_launch_break_bt_print.in          ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/python_launch_break_bt_print.in
%{__cp} -a %{tests_source_dir}/function/tests.py.data/python_launch_break_set.in               ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/python_launch_break_set.in
%{__cp} -a %{tests_source_dir}/function/tests.py.data/python_launch_halt_bt_active_allstop.in  ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/python_launch_halt_bt_active_allstop.in
%{__cp} -a %{tests_source_dir}/function/tests.py.data/python_launch_halt_bt_active_nonstop.in  ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/python_launch_halt_bt_active_nonstop.in
%{__cp} -a %{tests_source_dir}/function/tests.py.data/python_launch_halt_bt_dormant_allstop.in ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/python_launch_halt_bt_dormant_allstop.in
%{__cp} -a %{tests_source_dir}/function/tests.py.data/python_launch_halt_bt_dormant_nonstop.in ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/python_launch_halt_bt_dormant_nonstop.in
%{__cp} -a %{tests_source_dir}/function/tests.py.data/python_launch_print.in                   ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/python_launch_print.in
%{__cp} -a %{tests_source_dir}/function/tests.py.data/python_launch_step_next_finish.in        ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/python_launch_step_next_finish.in

%{__cp} -a %{tests_source_dir}/function/include/mpitest.h ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/include/mpitest.h
%{__cp} -a %{tests_source_dir}/function/include/mpitestconf.h ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/include/mpitestconf.h
%{__cp} -a %{tests_source_dir}/function/include/mpitestconf.h.in ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/include/mpitestconf.h.in
%{__cp} -a %{tests_source_dir}/function/include/mpitestcxx.h ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/include/mpitestcxx.h
%{__cp} -a %{tests_source_dir}/function/include/mpithreadtest.h ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/include/mpithreadtest.h
%{__cp} -a %{tests_source_dir}/function/include/rectangle.h ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/include/rectangle.h

%post
# Add Shasta configuration files for PE content projection
if [ "${RPM_INSTALL_PREFIX}" = "%{cray_prefix}" ]; then
    %{__mkdir_p} /etc/%{cray_prefix}/admin-pe/bindmount.conf.d/
    %{__mkdir_p} /etc/%{cray_prefix}/admin-pe/modulepaths.conf.d/
    echo "%{cray_prefix}/%{product}/" > /etc/%{cray_prefix}/admin-pe/bindmount.conf.d/%{cray_name}.conf
    echo "%{cray_prefix}/modulefiles/%{modulefile_name}" > /etc/%{cray_prefix}/admin-pe/modulepaths.conf.d/%{modulefile_name}.conf
    echo "%{cray_prefix}/lmod/modulefiles/core/%{cray_name}" >> /etc/%{cray_prefix}/admin-pe/modulepaths.conf.d/%{modulefile_name}.conf
fi

# Process relocations
if [ "${RPM_INSTALL_PREFIX}" != "%{prefix}" ]
then
    # tcl modulefile
    %{__sed} -i "s|^\([[:space:]]*set GDB4HPC_BASEDIR[[:space:]]*\)\(.*\)|\1${RPM_INSTALL_PREFIX}/%{cray_product}/%{pkgversion}|" ${RPM_INSTALL_PREFIX}/modulefiles/%{modulefile_name}/%{pkgversion}
    # lua modulefile
    %{__sed} -i "s|^\([[:space:]]*local INSTALL_ROOT[[:space:]]=*\)\(.*\)|\1= \"${RPM_INSTALL_PREFIX}\"|" ${RPM_INSTALL_PREFIX}/lmod/modulefiles/core/%{modulefile_name}/%{pkgversion}.lua
    # set default command
    %{__sed} -i "s|^\(export CRAY_inst_dir=\).*|\1${RPM_INSTALL_PREFIX}|" ${RPM_INSTALL_PREFIX}/%{cray_product}/%{pkgversion}/%{set_default_command}_%{cray_name}_%{pkgversion}
else
    # Only call set_default if we are not relocating the rpm and there's not already a default set unless someone passes in CRAY_INSTALL_DEFAULT=1
    if [ ${CRAY_INSTALL_DEFAULT:-0} -eq 1 ] || [ ! -f ${RPM_INSTALL_PREFIX}/modulefiles/%{cray_name}/.version ]
    then
        ${RPM_INSTALL_PREFIX}/%{set_default_path}/%{set_default_command}_%{cray_name}_%{pkgversion}
    fi

    # Don't want to set LD_LIBRARY_PATH if we are not relocating since rpath was set properly
    # tcl module
    %{__sed} -i "/^[[:space:]]*prepend-path[[:space:]]*LD_LIBRARY_PATH.*/d" ${RPM_INSTALL_PREFIX}/modulefiles/%{modulefile_name}/%{pkgversion}
    # lua module
    %{__sed} -i "/^prepend-path[[:space:]]*LD_LIBRARY_PATH.*/d" ${RPM_INSTALL_PREFIX}/lmod/modulefiles/core/%{cray_name}/%{pkgversion}.lua
fi

%{__gzip} -d -k -N ${RPM_INSTALL_PREFIX}/%{cray_product}/%{pkgversion}/lib/libgdb4_pybe_3.6.gz
%{__gzip} -d -k -N ${RPM_INSTALL_PREFIX}/%{cray_product}/%{pkgversion}/lib/libgdb4_pybe_3.8.gz
%{__gzip} -d -k -N ${RPM_INSTALL_PREFIX}/%{cray_product}/%{pkgversion}/lib/libgdb4_pybe_3.9.gz
%{__gzip} -d -k -N ${RPM_INSTALL_PREFIX}/%{cray_product}/%{pkgversion}/lib/libgdb4_pybe_3.10.gz
%{__gzip} -d -k -N ${RPM_INSTALL_PREFIX}/%{cray_product}/%{pkgversion}/lib/libgdb4_pybe_3.11.gz
%{__gzip} -d -k -N ${RPM_INSTALL_PREFIX}/%{cray_product}/%{pkgversion}/lib/libgdb4_pybe_3.12.gz

%preun
default_link="${RPM_INSTALL_PREFIX}/%{cray_product}/default"

# Cleanup module .version if it points to this version
if [ -f ${RPM_INSTALL_PREFIX}/modulefiles/%{modulefile_name}/.version ]
then
  dotversion=`grep ModulesVersion ${RPM_INSTALL_PREFIX}/modulefiles/%{modulefile_name}/.version | cut -f2 -d'"'`

  if [ "$dotversion" == "%{pkgversion}" ]
  then
    %{__rm} -f ${RPM_INSTALL_PREFIX}/modulefiles/%{modulefile_name}/.version
    echo "Uninstalled version and .version file match = ${default_version}."
    echo "Removing %{modulefile_name} .version file."
    %{__rm} -f ${default_link}
  fi
fi

%postun
if [ $1 == 1 ]
then
  exit 0
fi

# If the install dir exists
if [[ -z `ls ${RPM_INSTALL_PREFIX}/%{cray_product}` ]]; then
  %{__rm} -rf ${RPM_INSTALL_PREFIX}/%{cray_product}
  if [ -f /etc%{prefix}/admin-pe/bindmount.conf.d/%{cray_product}.conf ]; then
    %{__rm} -rf /etc%{prefix}/admin-pe/bindmount.conf.d/%{cray_product}.conf
  fi
  if [ -f /etc%{prefix}/admin-pe/modulepaths.conf.d/%{cray_product}.conf ]; then
    %{__rm} -rf /etc%{prefix}/admin-pe/modulepaths.conf.d/%{cray_product}.conf
  fi
  if [ -d ${RPM_INSTALL_PREFIX}/lmod/modulefiles/core/%{cray_product} ]; then
    %{__rm} -rf ${RPM_INSTALL_PREFIX}/lmod/modulefiles/core/%{cray_product}
  fi
  if [ -d ${RPM_INSTALL_PREFIX}/modulefiles/%{cray_product} ]; then
    %{__rm} -rf ${RPM_INSTALL_PREFIX}/modulefiles/%{cray_product}
  fi
fi

%files
%defattr(755, root, root)
%dir %{prefix}/%{cray_product}/%{pkgversion}
%dir %{prefix}/%{cray_product}/%{pkgversion}/lib
%dir %{prefix}/%{cray_product}/%{pkgversion}/libexec
%dir %{prefix}/%{cray_product}/%{pkgversion}/bin
%dir %{prefix}/%{cray_product}/%{pkgversion}/share
%dir %{prefix}/%{cray_product}/%{pkgversion}/share/man
%dir %{prefix}/%{cray_product}/%{pkgversion}/share/man/man1
%dir %{prefix}/%{cray_product}/%{pkgversion}/share/doc
%dir %{prefix}/%{cray_product}/%{pkgversion}/share/gdb
%dir %{prefix}/%{cray_product}/%{pkgversion}/share/gdb/syscalls
%dir %{prefix}/%{cray_product}/%{pkgversion}/share/gdb/system-gdbinit
%dir %{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python
%dir %{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/boost
%dir %{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb
%dir %{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb/command
%dir %{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb/function
%dir %{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb/printer
%dir %{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/libstdcxx/v6
%dir %{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/libstdcxx
%dir %{prefix}/%{cray_product}/%{pkgversion}/share/ompd
%dir %{prefix}/%{cray_product}/%{pkgversion}/share/ompd/gdb
%dir %{prefix}/%{cray_product}/%{pkgversion}/share/py-gdb
%dir %{prefix}/%{cray_product}/%{pkgversion}/demos
%dir %{prefix}/%{cray_product}/%{pkgversion}/demos/mpi_example
%dir %{prefix}/%{cray_product}/%{pkgversion}/demos/hpcc_demo
%dir %{prefix}/%{cray_product}/%{pkgversion}/demos/hpcc_demo/hpcc_scripts
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/%{release_notes_file}
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/%{attributions_name}
%attr(755, root, root) %{prefix}/%{cray_product}/%{pkgversion}/lib/libgdb4_unittest.so
%attr(755, root, root) %{prefix}/%{cray_product}/%{pkgversion}/lib/libgdb4_pybe_3.6.gz
%ghost %{prefix}/%{cray_product}/%{pkgversion}/lib/libgdb4_pybe_3.6.so
%attr(755, root, root) %{prefix}/%{cray_product}/%{pkgversion}/lib/libgdb4_pybe_3.8.gz
%ghost %{prefix}/%{cray_product}/%{pkgversion}/lib/libgdb4_pybe_3.8.so
%attr(755, root, root) %{prefix}/%{cray_product}/%{pkgversion}/lib/libgdb4_pybe_3.9.gz
%ghost %{prefix}/%{cray_product}/%{pkgversion}/lib/libgdb4_pybe_3.9.so
%attr(755, root, root) %{prefix}/%{cray_product}/%{pkgversion}/lib/libgdb4_pybe_3.10.gz
%ghost %{prefix}/%{cray_product}/%{pkgversion}/lib/libgdb4_pybe_3.10.so
%attr(755, root, root) %{prefix}/%{cray_product}/%{pkgversion}/lib/libgdb4_pybe_3.11.gz
%ghost %{prefix}/%{cray_product}/%{pkgversion}/lib/libgdb4_pybe_3.11.so
%attr(755, root, root) %{prefix}/%{cray_product}/%{pkgversion}/lib/libgdb4_pybe_3.12.gz
%ghost %{prefix}/%{cray_product}/%{pkgversion}/lib/libgdb4_pybe_3.12.so
%attr(755, root, root) %{prefix}/%{cray_product}/%{pkgversion}/lib/libgdb4_dbgagg.so
%attr(755, root, root) %{prefix}/%{cray_product}/%{pkgversion}/lib/libgdb4_zmqnetagg.so
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/demos/mpi_example/Makefile
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/demos/mpi_example/hello_mpi.f90
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/demos/mpi_example/hello_mpi.cpp
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/demos/mpi_example/hello_mpi.c
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/demos/hpcc_demo/README
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/demos/hpcc_demo/hpcc.source.tar.gz
%attr(755, root, root) %{prefix}/%{cray_product}/%{pkgversion}/demos/hpcc_demo/build_demo.sh
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/demos/hpcc_demo/hpccinf.txt
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/demos/hpcc_demo/hpcc_scripts/*.rc

%attr(755, root, root) %{prefix}/%{cray_product}/%{pkgversion}/libexec/dbgsrv
%attr(755, root, root) %{prefix}/%{cray_product}/%{pkgversion}/libexec/dbgsrv_alps
%attr(755, root, root) %{prefix}/%{cray_product}/%{pkgversion}/libexec/gdb-guard
%attr(755, root, root) %{prefix}/%{cray_product}/%{pkgversion}/bin/gdb4hpc
#gdb share
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/boost/__init__.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/boost/printers.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb/FrameDecorator.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb/FrameIterator.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb/frames.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb/__init__.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb/printing.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb/prompt.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb/types.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb/unwinder.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb/xmethod.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb/command/explore.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb/command/frame_filters.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb/command/__init__.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb/command/pretty_printers.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb/command/prompt.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb/command/type_printers.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb/command/unwinders.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb/command/xmethods.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb/function/as_string.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb/function/caller_is.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb/function/__init__.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb/function/strfns.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb/printer/bound_registers.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb/printer/__init__.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/libstdcxx/__init__.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/libstdcxx/v6/__init__.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/libstdcxx/v6/printers.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/gdb/syscalls/*.xml
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/gdb/syscalls/gdb-syscalls.dtd
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/gdb/system-gdbinit/elinos.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/gdb/system-gdbinit/wrs-linux.py
#ompd share
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/ompd/gdb/frame_filter.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/ompd/gdb/__init__.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/ompd/gdb/ompd_address_space.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/ompd/gdb/ompd_callbacks.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/ompd/gdb/ompd_handles.py
%attr(755, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/ompd/gdb/ompdModule.cpython-%{python_version}-%{_arch}-linux-gnu.so
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/ompd/gdb/ompd.py
#end share

%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/py-gdb/python-gdb.py

%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/man/man1/gdb4hpc.1
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/doc/getting-started.md
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/doc/getting-started.html
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/doc/tutorial.md
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/doc/tutorial.html
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/doc/raja-support.md
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/doc/raja-support.html
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/doc/handling-arrays.md
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/doc/handling-arrays.html
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/doc/kokkos-support.md
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/doc/kokkos-support.html
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/doc/vscode-plugin.md
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/doc/vscode-plugin.html
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/doc/python-extension.md
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/doc/python-extension.html
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/doc/python-mode.md
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/doc/python-mode.html
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/doc/help.md

%attr(755, root, root) %{prefix}/%{cray_product}/%{pkgversion}/%{set_default_command}_%{cray_product}_%{pkgversion}
%attr(755, root, root) %{prefix}/%{set_default_path}/%{set_default_command}_%{cray_product}_%{pkgversion}
%attr(755, root, root) %{prefix}/modulefiles/%{modulefile_name}/%{pkgversion}
%attr(644, root, root) %{prefix}/lmod/modulefiles/core/%{cray_name}/%{pkgversion}.lua

%files -n %{cray_name}-tests-%{pkgversion}
%dir %{prefix}/%{cray_product}/%{pkgversion}/tests
%dir %{prefix}/%{cray_product}/%{pkgversion}/tests/src
%dir %{prefix}/%{cray_product}/%{pkgversion}/tests/src/c
%dir %{prefix}/%{cray_product}/%{pkgversion}/tests/src/C++
%dir %{prefix}/%{cray_product}/%{pkgversion}/tests/src/fortran
%dir %{prefix}/%{cray_product}/%{pkgversion}/tests/src/raja
%dir %{prefix}/%{cray_product}/%{pkgversion}/tests/src/kokkos
%dir %{prefix}/%{cray_product}/%{pkgversion}/tests/src/python
%dir %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data
%dir %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/rocm
%dir %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/cuda
%dir %{prefix}/%{cray_product}/%{pkgversion}/tests/include

%dir %{prefix}/%{cray_product}/%{pkgversion}/tests/cdst_test
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/cdst_test/README.md
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/cdst_test/README_config.md
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/cdst_test/README_textcheck.md
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/cdst_test/__init__.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/cdst_test/requirements.txt
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/cdst_test/artifactory.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/cdst_test/bootstrap.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/cdst_test/config.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/cdst_test/config_default.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/cdst_test/config_nodes.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/cdst_test/env.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/cdst_test/files.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/cdst_test/job_environment.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/cdst_test/log.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/cdst_test/modules.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/cdst_test/remote_test_tool_startup.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/cdst_test/remote_test_tool_ui.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/cdst_test/ssh.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/cdst_test/test_server.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/cdst_test/test_server_commands.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/cdst_test/test_server_proto.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/cdst_test/textcheck.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/cdst_test/useful.py

%dir %{prefix}/%{cray_product}/%{pkgversion}/tests/cdst_test/data
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/cdst_test/data/hosts.yaml
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/cdst_test/data/products.yaml

%dir %{prefix}/%{cray_product}/%{pkgversion}/tests/cdst_test/scripts
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/cdst_test/scripts/avo_config.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/cdst_test/scripts/passwordedscp.expect
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/cdst_test/scripts/passwordedssh.expect
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/cdst_test/scripts/sshcopyid.expect
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/cdst_test/scripts/uai.expect
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/cdst_test/scripts/validate_ssh.sh
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/cdst_test/scripts/detect_wlm.sh

%dir %{prefix}/%{cray_product}/%{pkgversion}/tests/cdst_test/src
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/cdst_test/src/Makefile
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/cdst_test/src/hello_mpi.c

%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/README.md
%attr(755, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/test_tool.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/libtests.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/avocado_config.yaml
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/avocado_config_fullsuite.yaml
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/avocado_config_python.yaml
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/test_tool_config.yaml
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/Makefile

%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/src/cti_launch.cpp

%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/src/c/c_cmp_diff.c
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/src/c/c_type_pause.c
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/src/c/c_type_test.c
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/src/c/mtest.c
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/src/c/wtime.c
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/src/c/c_dlopen_test.c
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/src/c/libdlopen_test.c
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/src/c/c_segv.c
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/src/c/c_signal.c
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/src/c/c_list.c
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/src/c/c_half_exit.c
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/src/c/c_ompd.c
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/src/c/c_inc.c
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/src/c/libfoo.c
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/src/c/libbar.c
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/src/c/simple_gpu.c
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/src/c/c_shmem.c
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/src/c/c_not_mpi.c
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/src/c/c_nolib.c
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/src/c/c_libnolib.c

%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/src/C++/cpp_arrays.cpp
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/src/C++/cpp_arrays_print.cpp
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/src/C++/cpp_bd_test.cpp
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/src/C++/cpp_bt.cpp
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/src/C++/cpp_bt_stress.cpp
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/src/C++/cpp_catch.cpp
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/src/C++/cpp_cmp_diff.cpp
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/src/C++/cpp_decomp2.cpp
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/src/C++/cpp_decomp.cpp
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/src/C++/cpp_ds_test1.cpp
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/src/C++/cpp_ds_test2.cpp
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/src/C++/cpp_parallel_threads.cpp
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/src/C++/cpp_points.cpp
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/src/C++/cpp_threads.cpp
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/src/C++/cpp_attach.cpp

%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/src/fortran/ftn_arrays.f90
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/src/fortran/ftn_bd_dt.f90
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/src/fortran/ftn_cmp_diff.f90
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/src/fortran/ftn_decomp.f90
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/src/fortran/ftn_omp.f90

%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/src/raja/view-layout_solution.cpp
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/src/raja/view-layout-decomp_solution.cpp
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/src/raja/kernel-matrix-transpose-local-array_solution.cpp
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/src/raja/launch-matrix-transpose-local-array_solution.cpp
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/src/raja/reductions_solution.cpp

%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/src/python/sleep.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/src/python/spin.py

%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/assert_scalars.in
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/assert_expressions.in
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/assert_arrays.in
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/assert_decomp.in
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/assert_deadlock.in
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/assert_interrupted.in
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/assert_errors.in
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/assert_save.in
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/assert_stopstart.in
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/assert_twice.in
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/assert_invalidated.in
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/command_timeout.in
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_assign.in
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_assign_decomp.in
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_breakpoint.in
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_breakpoint_save.in
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_breakpoint_pending.in
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_checkpoint0.in
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_checkpoint1.in
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_checkpoint2.in
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_cmp_false.in
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_cmp_true.in
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_commnode_fanout.in
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_defset.in
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_finish.in
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_focus.in
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_frame.in
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_gdbmode.in
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_help.in
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_half_exit.in
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_ompd.in
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_info.in
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_kill.in
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_list.in
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_next.in
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/cpp_arrays_compare.in
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/cpp_arrays_chunk.in
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/cpp_arrays_print.in
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/cpp_bt.in
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/cpp_catch.in
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/cpp_decomp2.in
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/cpp_decomp.in
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/cpp_thread_funcs.in
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/cpp_thread_focus.in
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/cpp_attach.in
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_print.in
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_release.in
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_run.in
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_run_not_mpi.in
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_set_unset.in
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_show.in
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_sizeof.in
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_step.in
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_up_down.in
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_viewset.in
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_whatis.in
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_segv.in
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_shmem.in
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_mpmd.in
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_signal.in
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_nolib.in
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/ftn_arrays.in
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/ftn_decomp.in
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/ftn_rocmGpu.in
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/cpp_ds1.in
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/cpp_t1.in
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/cpp_t2.in
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/cpp_t3.in
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/ftn_t1.in
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/ftn_t2.in
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/ftn_t3.in
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/decomp_save.in
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/sbatch.in
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/source.in
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/sourceme.rc
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/rocmGpu.in
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/cudaGpu.in
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/qsub.template
%attr(755, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/rocm/pinoak-login1.sh
%attr(755, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/rocm/pea2k.sh
%attr(755, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/cuda/pegasus.sh
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/raja_views.in
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/raja_tile.in
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/raja_decomp.in
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/raja_reductions.in
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/save_all.in
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/mi_basics_console_exec.in
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/mi_basics_errors.in
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/mi_basics_exec.in
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/mi_basics_fallback.in
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/mi_basics_prefixes.in
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/mi_basics_prompt.in
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/mi_basics_results.in
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/mi_backtrace.in
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/mi_ccdb_breakpoint.in
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/mi_ccdb_compare.in
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/mi_ccdb_watch_mouse.in
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/mi_ccdb_step_next_finish.in
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/mi_ccdb_assert.in
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/completions_nolaunch.in
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/completions_launch.in
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/launch_from_path.in
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/positional_args.in
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/python_attach_detect_36.in
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/python_attach_halt_bt_active_allstop.in
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/python_attach_halt_bt_dormant_allstop.in
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/python_launch_break_bt_print.in
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/python_launch_break_set.in
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/python_launch_halt_bt_active_allstop.in
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/python_launch_halt_bt_active_nonstop.in
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/python_launch_halt_bt_dormant_allstop.in
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/python_launch_halt_bt_dormant_nonstop.in
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/python_launch_print.in
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/python_launch_step_next_finish.in

%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/src/kokkos/Makefile
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/src/kokkos/team_vector_loop.cpp
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/kokkos.in

%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/include/mpitest.h
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/include/mpitestconf.h
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/include/mpitestconf.h.in
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/include/mpitestcxx.h
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/include/mpithreadtest.h
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/include/rectangle.h
