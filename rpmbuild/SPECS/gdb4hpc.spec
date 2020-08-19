# Packaging definitions
%global pkgversion %(%{_sourcedir}/get_package_data --crayversion)
%global pkgversion_separator -
%global copyright Copyright 2007-2020 Cray Inc. All rights reserved.

# RPM build time
%global release_date %(date +%%B\\ %%Y)

%global product gdb4hpc
%global cray_product %{product}
%global cray_name %{cray_product}

# Path definitions
%global cray_prefix /opt/cray/pe
%global test_prefix /opt/cray/tests

#FIXME: This should be relocatable
%global external_build_dir %{cray_prefix}/%{cray_product}/%{pkgversion}

%global tests_source_dir %{_topdir}/../tests

# modulefile definitions
%global modulefile_name %{cray_name}
%global module_template_name %{modulefile_name}.module.template

# set_default modulefile definitions
%global set_default_command set_default
%global set_default_template_name %{set_default_command}.template
%global set_default_path admin-pe/set_default_files

# lmod modulefiles
%global lmod_template_gdb4hpc template_%{product}.lua

# release info
%global release_info_name release_info
%global release_info_template_name %{release_info_name}.template

# yaml file
%global yaml_template yaml.template
%global removal_date %(date '+%Y-%m-%d' -d "+5 years")

# attributions file
%global attributions_name ATTRIBUTIONS_gdb4hpc.txt

# cdst-support version
%global cdst_support_pkgversion_min %(%{_sourcedir}/get_package_data --cdstversionmin)
%global cdst_support_pkgversion_max %(%{_sourcedir}/get_package_data --cdstversionmax)

# cti version
%global cti_pkgversion_min %(%{_sourcedir}/get_package_data --ctiversionmin)
%global cti_pkgversion_max %(%{_sourcedir}/get_package_data --ctiversionmax)

# Disable debug package
%global debug_package %{nil}
%global _build_id_links none

# System strip command may be too old, use current path
%global __strip strip

# Filter requires - these all come from the cdst-support rpm
# These should match what is excluded in the cdst-support rpm specfile!
%global privlibs             libboost.*
%global privlibs %{privlibs}|libmrnet
%global privlibs %{privlibs}|libbitset
%global privlibs %{privlibs}|libmi
%global privlibs %{privlibs}|libssh2
%global privlibs %{privlibs}|libarchive
%global privlibs %{privlibs}|libsymtabAPI
%global privlibs %{privlibs}|libpcontrol
%global privlibs %{privlibs}|libdynElf
%global privlibs %{privlibs}|libdynDwarf
%global privlibs %{privlibs}|libcommon
%global privlibs %{privlibs}|libxplat
%global privlibs %{privlibs}|libedit
%global privlibs %{privlibs}|libdw
%global privlibs %{privlibs}|libebl.*
%global privlibs %{privlibs}|libelf
%global privlibs %{privlibs}|libtbbmalloc*
%global privlibs %{privlibs}|libtbb.*
%global privlibs %{privlibs}|librocm-dbgapi
%global __requires_exclude ^(%{privlibs})\\.so*

# Dist tags for SuSE need to be manually set
%if 0%{?suse_version}
%if 0%{?suse_version} == 1315
%global dist .sles12
%endif
%if 0%{?sle_version} == 120000
%global dist .sles12
%endif
%if 0%{?sle_version} == 120100
%global dist .sles12sp1
%endif
%if 0%{?sle_version} == 120200
%global dist .sles12sp2
%endif
%if 0%{?sle_version} == 120300
%global dist .sles12sp3
%endif
%if 0%{?sle_version} == 120400
%global dist .sles12sp4
%endif
%if 0%{?sle_version} == 150000
%global dist .sles15
%global OS_HW_TAG 7.0,7.1
%global OS_WB_TAG sles15 
%endif
%if 0%{?sle_version} == 150100
%global dist .sles15sp1
%global OS_HW_TAG 7.0,7.1
%global OS_WB_TAG sles15sp1
%endif
%endif

%if %{_arch} == aarch64
%global SYS_HW_TAG AARCH64
%global SYS_WB_TAG AARCH64
%endif
%if %{_arch} == x86_64
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
Release:    1%{dist}
Prefix:     %{cray_prefix}
License:    %{copyright}
Vendor:     Cray Inc.
Group:      Development/Tools/Debuggers
Provides:   %{cray_name} = %{pkgversion}
Requires:   set_default_2, cray-cdst-support >= %{cdst_support_pkgversion_min}, cray-cdst-support < %{cdst_support_pkgversion_max}, cray-cti >= %{cti_pkgversion_min}, cray-cti < %{cti_pkgversion_max}
Source0:    %{module_template_name}
Source1:    %{set_default_template_name}
Source2:    %{release_info_template_name}
Source3:    %{attributions_name}
Source4:    %{lmod_template_gdb4hpc}
Source5:    %{yaml_template}

%description
Cray gdb4hpc %{pkgversion}
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
%{__sed} 's|<RELEASE_DATE>|%{release_date}|g;s|<VERSION>|%{pkgversion}|g;s|<RELEASE>|%{version}-%{release}|g;s|<COPYRIGHT>|%{copyright}|g;s|<CRAY_NAME>|%{cray_name}|g;s|<CRAY_PREFIX>|%{cray_prefix}|g;s|<ARCH>|%{_target_cpu}|g' %{SOURCE2} > ${RPM_BUILD_DIR}/%{release_info_name}
%{__cp} -a %{SOURCE3} ${RPM_BUILD_DIR}/%{attributions_name}

%install
# copy files from external install
#
# Cray PE package root
%{__install} -d ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}
# Information files
%{__cp} -a ${RPM_BUILD_DIR}/%{release_info_name} ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/%{release_info_name}
%{__cp} -a ${RPM_BUILD_DIR}/%{attributions_name} ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/%{attributions_name}
# Libraries
%{__install} -d ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/lib
%{__cp} -a %{external_build_dir}/lib/libbitset.so ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/lib
%{__cp} -a %{external_build_dir}/lib/libdbgagg.so ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/lib
%{__cp} -a %{external_build_dir}/lib/libmi.so ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/lib
%{__cp} -a %{external_build_dir}/lib/libmrnet.so* ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/lib
%{__cp} -a %{external_build_dir}/lib/libxplat.so* ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/lib
%{__cp} -a %{external_build_dir}/lib/uda-plugin-cray-upc.so ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/lib
# Libraries static
# pkg-config
# Headers
# libexec binaries
%{__install} -d ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/libexec
%{__cp} -a %{external_build_dir}/libexec/dbgsrv ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/libexec
%{__cp} -a %{external_build_dir}/libexec/dbgsrv_alps ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/libexec
%{__cp} -a %{external_build_dir}/libexec/gdb-guard ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/libexec
%ifnarch aarch64
%{__cp} -a %{external_build_dir}/libexec/cuda101-gdb-guard ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/libexec
%endif
%{__cp} -a %{external_build_dir}/libexec/ROCgdb-guard ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/libexec
%{__cp} -a %{external_build_dir}/libexec/guard_proxy ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/libexec
%{__cp} -a %{external_build_dir}/libexec/start_proxy.sh ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/libexec
%{__cp} -a %{external_build_dir}/bin/mrnet_commnode ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/libexec
%{__cp} -a %{external_build_dir}/bin/mrnet_topgen ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/libexec
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
%{__cp} -a %{external_build_dir}/share/gdb/python/gdb/command/tsx.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb/command
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
%{__install} -d ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/libstdcxx
%{__cp} -a %{external_build_dir}/share/gdb/python/libstdcxx/__init__.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/libstdcxx
%{__install} -d ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/libstdcxx/v6
%{__cp} -a %{external_build_dir}/share/gdb/python/libstdcxx/v6/__init__.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/libstdcxx/v6
%{__cp} -a %{external_build_dir}/share/gdb/python/libstdcxx/v6/printers.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/libstdcxx/v6
%{__install} -d ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/gdb/syscalls
%{__cp} -a %{external_build_dir}/share/gdb/syscalls/*.xml ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/gdb/syscalls
%{__cp} -a %{external_build_dir}/share/gdb/syscalls/gdb-syscalls.dtd ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/gdb/syscalls
%{__install} -d ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/gdb/system-gdbinit
%{__cp} -a %{external_build_dir}/share/gdb/system-gdbinit/elinos.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/gdb/system-gdbinit
%{__cp} -a %{external_build_dir}/share/gdb/system-gdbinit/wrs-linux.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/gdb/system-gdbinit
# cuda-gdb Share
%ifnarch aarch64
%{__install} -d ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb
%{__install} -d ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python
%{__install} -d ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb
%{__cp} -a %{external_build_dir}/cuda_gdb_install/share/cuda-gdb/python/gdb/FrameDecorator.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb
%{__cp} -a %{external_build_dir}/cuda_gdb_install/share/cuda-gdb/python/gdb/FrameIterator.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb
%{__cp} -a %{external_build_dir}/cuda_gdb_install/share/cuda-gdb/python/gdb/frames.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb
%{__cp} -a %{external_build_dir}/cuda_gdb_install/share/cuda-gdb/python/gdb/__init__.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb
%{__cp} -a %{external_build_dir}/cuda_gdb_install/share/cuda-gdb/python/gdb/printing.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb
%{__cp} -a %{external_build_dir}/cuda_gdb_install/share/cuda-gdb/python/gdb/prompt.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb
%{__cp} -a %{external_build_dir}/cuda_gdb_install/share/cuda-gdb/python/gdb/types.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb
%{__cp} -a %{external_build_dir}/cuda_gdb_install/share/cuda-gdb/python/gdb/unwinder.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb
%{__cp} -a %{external_build_dir}/cuda_gdb_install/share/cuda-gdb/python/gdb/xmethod.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb
%{__install} -d ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb/command
%{__cp} -a %{external_build_dir}/cuda_gdb_install/share/cuda-gdb/python/gdb/command/explore.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb/command
%{__cp} -a %{external_build_dir}/cuda_gdb_install/share/cuda-gdb/python/gdb/command/frame_filters.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb/command
%{__cp} -a %{external_build_dir}/cuda_gdb_install/share/cuda-gdb/python/gdb/command/__init__.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb/command
%{__cp} -a %{external_build_dir}/cuda_gdb_install/share/cuda-gdb/python/gdb/command/pretty_printers.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb/command
%{__cp} -a %{external_build_dir}/cuda_gdb_install/share/cuda-gdb/python/gdb/command/prompt.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb/command
%{__cp} -a %{external_build_dir}/cuda_gdb_install/share/cuda-gdb/python/gdb/command/tsx.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb/command
%{__cp} -a %{external_build_dir}/cuda_gdb_install/share/cuda-gdb/python/gdb/command/type_printers.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb/command
%{__cp} -a %{external_build_dir}/cuda_gdb_install/share/cuda-gdb/python/gdb/command/unwinders.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb/command
%{__cp} -a %{external_build_dir}/cuda_gdb_install/share/cuda-gdb/python/gdb/command/xmethods.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb/command
%{__install} -d ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb/function
%{__cp} -a %{external_build_dir}/cuda_gdb_install/share/cuda-gdb/python/gdb/function/as_string.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb/function
%{__cp} -a %{external_build_dir}/cuda_gdb_install/share/cuda-gdb/python/gdb/function/caller_is.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb/function
%{__cp} -a %{external_build_dir}/cuda_gdb_install/share/cuda-gdb/python/gdb/function/__init__.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb/function
%{__cp} -a %{external_build_dir}/cuda_gdb_install/share/cuda-gdb/python/gdb/function/strfns.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb/function
%{__install} -d ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb/printer
%{__cp} -a %{external_build_dir}/cuda_gdb_install/share/cuda-gdb/python/gdb/printer/bound_registers.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb/printer
%{__cp} -a %{external_build_dir}/cuda_gdb_install/share/cuda-gdb/python/gdb/printer/__init__.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb/printer
%{__install} -d ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/syscalls
%{__cp} -a %{external_build_dir}/cuda_gdb_install/share/cuda-gdb/syscalls/*.xml ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/syscalls
%{__cp} -a %{external_build_dir}/cuda_gdb_install/share/cuda-gdb/syscalls/gdb-syscalls.dtd ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/syscalls
%{__install} -d ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/system-gdbinit
%{__cp} -a %{external_build_dir}/cuda_gdb_install/share/cuda-gdb/system-gdbinit/elinos.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/system-gdbinit
%{__cp} -a %{external_build_dir}/cuda_gdb_install/share/cuda-gdb/system-gdbinit/wrs-linux.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/system-gdbinit
# ROCgdb Share
%{__install} -d ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/ROCgdb
%{__install} -d ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/ROCgdb/python
%{__install} -d ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/ROCgdb/python/gdb
%{__cp} -a %{external_build_dir}/ROCgdb_install/gdb/data-directory/python/gdb/FrameDecorator.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/ROCgdb/python/gdb
%{__cp} -a %{external_build_dir}/ROCgdb_install/gdb/data-directory/python/gdb/FrameIterator.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/ROCgdb/python/gdb
%{__cp} -a %{external_build_dir}/ROCgdb_install/gdb/data-directory/python/gdb/frames.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/ROCgdb/python/gdb
%{__cp} -a %{external_build_dir}/ROCgdb_install/gdb/data-directory/python/gdb/__init__.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/ROCgdb/python/gdb
%{__cp} -a %{external_build_dir}/ROCgdb_install/gdb/data-directory/python/gdb/printing.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/ROCgdb/python/gdb
%{__cp} -a %{external_build_dir}/ROCgdb_install/gdb/data-directory/python/gdb/prompt.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/ROCgdb/python/gdb
%{__cp} -a %{external_build_dir}/ROCgdb_install/gdb/data-directory/python/gdb/types.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/ROCgdb/python/gdb
%{__cp} -a %{external_build_dir}/ROCgdb_install/gdb/data-directory/python/gdb/unwinder.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/ROCgdb/python/gdb
%{__cp} -a %{external_build_dir}/ROCgdb_install/gdb/data-directory/python/gdb/xmethod.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/ROCgdb/python/gdb
%{__install} -d ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/ROCgdb/python/gdb/command
%{__cp} -a %{external_build_dir}/ROCgdb_install/gdb/data-directory/python/gdb/command/explore.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/ROCgdb/python/gdb/command
%{__cp} -a %{external_build_dir}/ROCgdb_install/gdb/data-directory/python/gdb/command/frame_filters.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/ROCgdb/python/gdb/command
%{__cp} -a %{external_build_dir}/ROCgdb_install/gdb/data-directory/python/gdb/command/__init__.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/ROCgdb/python/gdb/command
%{__cp} -a %{external_build_dir}/ROCgdb_install/gdb/data-directory/python/gdb/command/pretty_printers.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/ROCgdb/python/gdb/command
%{__cp} -a %{external_build_dir}/ROCgdb_install/gdb/data-directory/python/gdb/command/prompt.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/ROCgdb/python/gdb/command
%{__cp} -a %{external_build_dir}/ROCgdb_install/gdb/data-directory/python/gdb/command/type_printers.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/ROCgdb/python/gdb/command
%{__cp} -a %{external_build_dir}/ROCgdb_install/gdb/data-directory/python/gdb/command/unwinders.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/ROCgdb/python/gdb/command
%{__cp} -a %{external_build_dir}/ROCgdb_install/gdb/data-directory/python/gdb/command/xmethods.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/ROCgdb/python/gdb/command
%{__install} -d ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/ROCgdb/python/gdb/function
%{__cp} -a %{external_build_dir}/ROCgdb_install/gdb/data-directory/python/gdb/function/as_string.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/ROCgdb/python/gdb/function
%{__cp} -a %{external_build_dir}/ROCgdb_install/gdb/data-directory/python/gdb/function/caller_is.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/ROCgdb/python/gdb/function
%{__cp} -a %{external_build_dir}/ROCgdb_install/gdb/data-directory/python/gdb/function/__init__.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/ROCgdb/python/gdb/function
%{__cp} -a %{external_build_dir}/ROCgdb_install/gdb/data-directory/python/gdb/function/strfns.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/ROCgdb/python/gdb/function
%{__install} -d ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/ROCgdb/python/gdb/printer
%{__cp} -a %{external_build_dir}/ROCgdb_install/gdb/data-directory/python/gdb/printer/bound_registers.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/ROCgdb/python/gdb/printer
%{__cp} -a %{external_build_dir}/ROCgdb_install/gdb/data-directory/python/gdb/printer/__init__.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/ROCgdb/python/gdb/printer
%{__install} -d ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/ROCgdb/syscalls
%{__cp} -a %{external_build_dir}/ROCgdb_install/gdb/data-directory/syscalls/*.xml ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/ROCgdb/syscalls
%{__cp} -a %{external_build_dir}/ROCgdb_install/gdb/data-directory/syscalls/gdb-syscalls.dtd ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/ROCgdb/syscalls
%{__install} -d ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/ROCgdb/system-gdbinit
%{__cp} -a %{external_build_dir}/ROCgdb_install/gdb/data-directory/system-gdbinit/elinos.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/ROCgdb/system-gdbinit
%{__cp} -a %{external_build_dir}/ROCgdb_install/gdb/data-directory/system-gdbinit/wrs-linux.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/ROCgdb/system-gdbinit
%endif
# manpage
%{__install} -d ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/man/man1
%{__cp} -a %{external_build_dir}/share/man/man1/gdb4hpc.1 ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/man/man1
# modulefile
%{__install} -d ${RPM_BUILD_ROOT}/%{prefix}/modulefiles/%{modulefile_name}
%{__sed} 's|<PREFIX>|%{external_build_dir}|g;s|<CRAY_NAME>|%{cray_name}|g;s|<VERSION>|%{pkgversion}|g' %{SOURCE0} > ${RPM_BUILD_ROOT}/%{prefix}/modulefiles/%{modulefile_name}/%{pkgversion}
# lmod modulefile
%{__install} -d ${RPM_BUILD_ROOT}/%{prefix}/lmod/modulefiles/core/%{cray_name}
%{__sed} 's|\[@%PREFIX_PATH%@\]|%{prefix}|g;s|\[@%MODULE_VERSION%@\]|%{pkgversion}|g' %{SOURCE4} > ${RPM_BUILD_ROOT}/%{prefix}/lmod/modulefiles/core/%{cray_name}/%{pkgversion}.lua
# set_default scripts
%{__sed} 's|<PREFIX>|%{prefix}|g;s|<CRAY_PRODUCT>|%{cray_product}|g;s|<VERSION>|%{pkgversion}|g;s|<MODULEFILE_NAME>|%{modulefile_name}|g' %{SOURCE1} > ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/%{set_default_command}_%{cray_product}_%{pkgversion}
# set_default into admin-pe
%{__install} -d ${RPM_BUILD_ROOT}/%{prefix}/%{set_default_path}
%{__install} -D ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/%{set_default_command}_%{cray_product}_%{pkgversion} ${RPM_BUILD_ROOT}/%{prefix}/%{set_default_path}/%{set_default_command}_%{cray_product}_%{pkgversion}
# yaml file
%{__mkdir} -p %{_rpmdir}/%{_arch}
%{__sed} 's|<PRODUCT>|%{cray_name}|g;s|<VERSION>|%{pkgversion}|g;s|<BUILD_METADATA>|%{version}|g;s|<RELEASE>|%{release}|g;s|<ARCH>|%{_arch}|g;s|<REMOVAL_DATE>|%{removal_date}|g;s|<SYS_HW_TAG>|%{SYS_HW_TAG}|g;s|<SYS_WB_TAG>|%{SYS_WB_TAG}|g;s|<OS_HW_TAG>|%{OS_HW_TAG}|g;s|<OS_WB_TAG>|%{OS_WB_TAG}|g' %{SOURCE5} > %{_rpmdir}/%{_arch}/%{cray_name}-%{pkgversion}-%{version}-%{release}.%{_arch}.rpm.yaml

# tests
%{__install} -d ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests
%{__install} -d ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/bd_dt
%{__install} -d ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/bd_dt/c
%{__install} -d ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/bd_dt/C++
%{__install} -d ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/bd_dt/fortran
%{__install} -d ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data
%{__install} -d ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/include

%{__cp} -a %{tests_source_dir}/function/test_tool_starter.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/test_tool_starter.py
%{__cp} -a %{tests_source_dir}/function/tests.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py
%{__cp} -a %{tests_source_dir}/function/avocado_config.yaml ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/avocado_config.yaml
%{__cp} -a %{tests_source_dir}/function/test_tool_config.yaml ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/test_tool_config.yaml
%{__cp} -a %{tests_source_dir}/function/Makefile.am ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/Makefile.am
%{__cp} -a %{tests_source_dir}/function/configure.ac ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/configure.ac
%{__cp} -a %{tests_source_dir}/function/README.md ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/README.md

%{__cp} -a %{tests_source_dir}/function/bd_dt/Makefile.am ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/bd_dt/Makefile.am

%{__cp} -a %{tests_source_dir}/function/bd_dt/c/Makefile.am ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/bd_dt/c/Makefile.am
%{__cp} -a %{tests_source_dir}/function/bd_dt/c/c_cmp_diff.c ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/bd_dt/c/c_cmp_diff.c
%{__cp} -a %{tests_source_dir}/function/bd_dt/c/c_type_pause.c ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/bd_dt/c/c_type_pause.c
%{__cp} -a %{tests_source_dir}/function/bd_dt/c/c_type_test.c ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/bd_dt/c/c_type_test.c
%{__cp} -a %{tests_source_dir}/function/bd_dt/c/mtest.c ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/bd_dt/c/mtest.c
%{__cp} -a %{tests_source_dir}/function/bd_dt/c/wtime.c ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/bd_dt/c/wtime.c
%{__cp} -a %{tests_source_dir}/function/bd_dt/c/c_dlopen_test.c ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/bd_dt/c/c_dlopen_test.c
%{__cp} -a %{tests_source_dir}/function/bd_dt/c/libdlopen_test.c ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/bd_dt/c/libdlopen_test.c

%{__cp} -a %{tests_source_dir}/function/bd_dt/C++/Makefile.am ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/bd_dt/C++/Makefile.am
%{__cp} -a %{tests_source_dir}/function/bd_dt/C++/cpp_arrays.cpp ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/bd_dt/C++/cpp_arrays.cpp
%{__cp} -a %{tests_source_dir}/function/bd_dt/C++/cpp_bd_test.cpp ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/bd_dt/C++/cpp_bd_test.cpp
%{__cp} -a %{tests_source_dir}/function/bd_dt/C++/cpp_bt.cpp ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/bd_dt/C++/cpp_bt.cpp
%{__cp} -a %{tests_source_dir}/function/bd_dt/C++/cpp_bt_stress.cpp ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/bd_dt/C++/cpp_bt_stress.cpp
%{__cp} -a %{tests_source_dir}/function/bd_dt/C++/cpp_cmp_diff.cpp ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/bd_dt/C++/cpp_cmp_diff.cpp
%{__cp} -a %{tests_source_dir}/function/bd_dt/C++/cpp_decomp2.cpp ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/bd_dt/C++/cpp_decomp2.cpp
%{__cp} -a %{tests_source_dir}/function/bd_dt/C++/cpp_decomp.cpp ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/bd_dt/C++/cpp_decomp.cpp
%{__cp} -a %{tests_source_dir}/function/bd_dt/C++/cpp_ds_test1.cpp ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/bd_dt/C++/cpp_ds_test1.cpp
%{__cp} -a %{tests_source_dir}/function/bd_dt/C++/cpp_ds_test2.cpp ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/bd_dt/C++/cpp_ds_test2.cpp
%{__cp} -a %{tests_source_dir}/function/bd_dt/C++/cpp_parallel_threads.cpp ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/bd_dt/C++/cpp_parallel_threads.cpp
%{__cp} -a %{tests_source_dir}/function/bd_dt/C++/cpp_threads.cpp ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/bd_dt/C++/cpp_threads.cpp

%{__cp} -a %{tests_source_dir}/function/bd_dt/fortran/Makefile.am ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/bd_dt/fortran/Makefile.am
%{__cp} -a %{tests_source_dir}/function/bd_dt/fortran/ftn_arrays.f90 ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/bd_dt/fortran/ftn_arrays.f90
%{__cp} -a %{tests_source_dir}/function/bd_dt/fortran/ftn_bd_dt.f90 ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/bd_dt/fortran/ftn_bd_dt.f90
%{__cp} -a %{tests_source_dir}/function/bd_dt/fortran/ftn_cmp_diff.f90 ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/bd_dt/fortran/ftn_cmp_diff.f90
%{__cp} -a %{tests_source_dir}/function/bd_dt/fortran/ftn_decomp.f90 ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/bd_dt/fortran/ftn_decomp.f90

%{__cp} -a %{tests_source_dir}/function/tests.py.data/c_assert_wdecomp.rc ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_assert_wdecomp.rc
%{__cp} -a %{tests_source_dir}/function/tests.py.data/c_assert_wdecomp.txt ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_assert_wdecomp.txt
%{__cp} -a %{tests_source_dir}/function/tests.py.data/c_assert_wodecomp.rc ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_assert_wodecomp.rc
%{__cp} -a %{tests_source_dir}/function/tests.py.data/c_assert_wodecomp.txt ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_assert_wodecomp.txt
%{__cp} -a %{tests_source_dir}/function/tests.py.data/c_assign.rc ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_assign.rc
%{__cp} -a %{tests_source_dir}/function/tests.py.data/c_assign.txt ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_assign.txt
%{__cp} -a %{tests_source_dir}/function/tests.py.data/c_breakpoint.rc ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_breakpoint.rc
%{__cp} -a %{tests_source_dir}/function/tests.py.data/c_breakpoint.txt ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_breakpoint.txt
%{__cp} -a %{tests_source_dir}/function/tests.py.data/c_breakpoint_pending.rc ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_breakpoint_pending.rc
%{__cp} -a %{tests_source_dir}/function/tests.py.data/c_breakpoint_pending.txt ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_breakpoint_pending.txt
%{__cp} -a %{tests_source_dir}/function/tests.py.data/c_cmp_false.rc ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_cmp_false.rc
%{__cp} -a %{tests_source_dir}/function/tests.py.data/c_cmp_false.txt ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_cmp_false.txt
%{__cp} -a %{tests_source_dir}/function/tests.py.data/c_cmp_true.rc ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_cmp_true.rc
%{__cp} -a %{tests_source_dir}/function/tests.py.data/c_cmp_true.txt ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_cmp_true.txt
%{__cp} -a %{tests_source_dir}/function/tests.py.data/c_defset.rc ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_defset.rc
%{__cp} -a %{tests_source_dir}/function/tests.py.data/c_defset.txt ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_defset.txt
%{__cp} -a %{tests_source_dir}/function/tests.py.data/c_finish.rc ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_finish.rc
%{__cp} -a %{tests_source_dir}/function/tests.py.data/c_finish.txt ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_finish.txt
%{__cp} -a %{tests_source_dir}/function/tests.py.data/c_focus.rc ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_focus.rc
%{__cp} -a %{tests_source_dir}/function/tests.py.data/c_focus.txt ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_focus.txt
%{__cp} -a %{tests_source_dir}/function/tests.py.data/c_frame.rc ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_frame.rc
%{__cp} -a %{tests_source_dir}/function/tests.py.data/c_frame.txt ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_frame.txt
%{__cp} -a %{tests_source_dir}/function/tests.py.data/c_gdbmode.rc ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_gdbmode.rc
%{__cp} -a %{tests_source_dir}/function/tests.py.data/c_gdbmode.txt ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_gdbmode.txt
%{__cp} -a %{tests_source_dir}/function/tests.py.data/c_help.rc ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_help.rc
%{__cp} -a %{tests_source_dir}/function/tests.py.data/c_help.txt ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_help.txt
%{__cp} -a %{tests_source_dir}/function/tests.py.data/c_info.rc ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_info.rc
%{__cp} -a %{tests_source_dir}/function/tests.py.data/c_info.txt ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_info.txt
%{__cp} -a %{tests_source_dir}/function/tests.py.data/c_kill.rc ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_kill.rc
%{__cp} -a %{tests_source_dir}/function/tests.py.data/c_kill.txt ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_kill.txt
%{__cp} -a %{tests_source_dir}/function/tests.py.data/c_list.rc ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_list.rc
%{__cp} -a %{tests_source_dir}/function/tests.py.data/c_list.txt ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_list.txt
%{__cp} -a %{tests_source_dir}/function/tests.py.data/c_next.rc ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_next.rc
%{__cp} -a %{tests_source_dir}/function/tests.py.data/c_next.txt ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_next.txt
%{__cp} -a %{tests_source_dir}/function/tests.py.data/cpp_arrays_compare.rc ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/cpp_arrays_compare.rc
%{__cp} -a %{tests_source_dir}/function/tests.py.data/cpp_arrays_compare.txt ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/cpp_arrays_compare.txt
%{__cp} -a %{tests_source_dir}/function/tests.py.data/cpp_bt.rc ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/cpp_bt.rc
%{__cp} -a %{tests_source_dir}/function/tests.py.data/cpp_bt.txt ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/cpp_bt.txt
%{__cp} -a %{tests_source_dir}/function/tests.py.data/cpp_decomp2.rc ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/cpp_decomp2.rc
%{__cp} -a %{tests_source_dir}/function/tests.py.data/cpp_decomp2.txt ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/cpp_decomp2.txt
%{__cp} -a %{tests_source_dir}/function/tests.py.data/cpp_decomp.rc ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/cpp_decomp.rc
%{__cp} -a %{tests_source_dir}/function/tests.py.data/cpp_decomp.txt ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/cpp_decomp.txt
%{__cp} -a %{tests_source_dir}/function/tests.py.data/cpp_thread_funcs.rc ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/cpp_thread_funcs.rc
%{__cp} -a %{tests_source_dir}/function/tests.py.data/cpp_thread_funcs.txt ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/cpp_thread_funcs.txt
%{__cp} -a %{tests_source_dir}/function/tests.py.data/cpp_thread_focus.rc ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/cpp_thread_focus.rc
%{__cp} -a %{tests_source_dir}/function/tests.py.data/cpp_thread_focus.txt ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/cpp_thread_focus.txt
%{__cp} -a %{tests_source_dir}/function/tests.py.data/c_print.rc ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_print.rc
%{__cp} -a %{tests_source_dir}/function/tests.py.data/c_print.txt ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_print.txt
%{__cp} -a %{tests_source_dir}/function/tests.py.data/c_release.rc ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_release.rc
%{__cp} -a %{tests_source_dir}/function/tests.py.data/c_release.txt ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_release.txt
%{__cp} -a %{tests_source_dir}/function/tests.py.data/c_set_unset.rc ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_set_unset.rc
%{__cp} -a %{tests_source_dir}/function/tests.py.data/c_set_unset.txt ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_set_unset.txt
%{__cp} -a %{tests_source_dir}/function/tests.py.data/c_show.rc ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_show.rc
%{__cp} -a %{tests_source_dir}/function/tests.py.data/c_show.txt ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_show.txt
%{__cp} -a %{tests_source_dir}/function/tests.py.data/c_sizeof.rc ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_sizeof.rc
%{__cp} -a %{tests_source_dir}/function/tests.py.data/c_sizeof.txt ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_sizeof.txt
%{__cp} -a %{tests_source_dir}/function/tests.py.data/c_step.rc ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_step.rc
%{__cp} -a %{tests_source_dir}/function/tests.py.data/c_step.txt ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_step.txt
%{__cp} -a %{tests_source_dir}/function/tests.py.data/c_up_down.rc ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_up_down.rc
%{__cp} -a %{tests_source_dir}/function/tests.py.data/c_up_down.txt ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_up_down.txt
%{__cp} -a %{tests_source_dir}/function/tests.py.data/c_viewset.rc ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_viewset.rc
%{__cp} -a %{tests_source_dir}/function/tests.py.data/c_viewset.txt ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_viewset.txt
%{__cp} -a %{tests_source_dir}/function/tests.py.data/c_whatis.rc ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_whatis.rc
%{__cp} -a %{tests_source_dir}/function/tests.py.data/c_whatis.txt ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_whatis.txt
%{__cp} -a %{tests_source_dir}/function/tests.py.data/ds1.txt ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/ds1.txt
%{__cp} -a %{tests_source_dir}/function/tests.py.data/ftn_arrays.rc ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/ftn_arrays.rc
%{__cp} -a %{tests_source_dir}/function/tests.py.data/ftn_arrays.txt ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/ftn_arrays.txt
%{__cp} -a %{tests_source_dir}/function/tests.py.data/ftn_decomp.rc ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/ftn_decomp.rc
%{__cp} -a %{tests_source_dir}/function/tests.py.data/ftn_decomp.txt ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/ftn_decomp.txt
%{__cp} -a %{tests_source_dir}/function/tests.py.data/lgdb_cpp_ds1.rc ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/lgdb_cpp_ds1.rc
%{__cp} -a %{tests_source_dir}/function/tests.py.data/lgdb_cpp_ds1.txt ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/lgdb_cpp_ds1.txt
%{__cp} -a %{tests_source_dir}/function/tests.py.data/lgdb_cpp_t1.rc ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/lgdb_cpp_t1.rc
%{__cp} -a %{tests_source_dir}/function/tests.py.data/lgdb_cpp_t1.txt ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/lgdb_cpp_t1.txt
%{__cp} -a %{tests_source_dir}/function/tests.py.data/lgdb_cpp_t2.rc ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/lgdb_cpp_t2.rc
%{__cp} -a %{tests_source_dir}/function/tests.py.data/lgdb_cpp_t2.txt ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/lgdb_cpp_t2.txt
%{__cp} -a %{tests_source_dir}/function/tests.py.data/lgdb_cpp_t3.rc ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/lgdb_cpp_t3.rc
%{__cp} -a %{tests_source_dir}/function/tests.py.data/lgdb_cpp_t3.txt ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/lgdb_cpp_t3.txt
%{__cp} -a %{tests_source_dir}/function/tests.py.data/lgdb_ftn_t1.rc ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/lgdb_ftn_t1.rc
%{__cp} -a %{tests_source_dir}/function/tests.py.data/lgdb_ftn_t1.txt ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/lgdb_ftn_t1.txt
%{__cp} -a %{tests_source_dir}/function/tests.py.data/lgdb_ftn_t2.rc ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/lgdb_ftn_t2.rc
%{__cp} -a %{tests_source_dir}/function/tests.py.data/lgdb_ftn_t2.txt ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/lgdb_ftn_t2.txt
%{__cp} -a %{tests_source_dir}/function/tests.py.data/lgdb_ftn_t3.rc ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/lgdb_ftn_t3.rc
%{__cp} -a %{tests_source_dir}/function/tests.py.data/lgdb_ftn_t3.txt ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/lgdb_ftn_t3.txt
%{__cp} -a %{tests_source_dir}/function/tests.py.data/lgdb_openacc_t1.rc ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/lgdb_openacc_t1.rc
%{__cp} -a %{tests_source_dir}/function/tests.py.data/lgdb_upc_t1.rc ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/lgdb_upc_t1.rc
%{__cp} -a %{tests_source_dir}/function/tests.py.data/open_acc_t1.txt ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/open_acc_t1.txt
%{__cp} -a %{tests_source_dir}/function/tests.py.data/test1.txt ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/test1.txt
%{__cp} -a %{tests_source_dir}/function/tests.py.data/test2.txt ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/test2.txt
%{__cp} -a %{tests_source_dir}/function/tests.py.data/upc.txt ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/upc.txt
%{__cp} -a %{tests_source_dir}/function/tests.py.data/qsub.template ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/qsub.template

%{__cp} -a %{tests_source_dir}/function/include/mpitest.h ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/include/mpitest.h
%{__cp} -a %{tests_source_dir}/function/include/mpitestconf.h ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/include/mpitestconf.h
%{__cp} -a %{tests_source_dir}/function/include/mpitestconf.h.in ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/include/mpitestconf.h.in
%{__cp} -a %{tests_source_dir}/function/include/mpitestcxx.h ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/include/mpitestcxx.h
%{__cp} -a %{tests_source_dir}/function/include/mpithreadtest.h ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/include/mpithreadtest.h
%{__cp} -a %{tests_source_dir}/function/include/rectangle.h ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/tests/include/rectangle.h

# yaml file
# since there is no modulefile in the tests rpm, remove related sections from the template
%global start_rmLine %(sed -n /section-3/= %{SOURCE5})
%global end_rmLine %(sed -n /admin-pe/= %{SOURCE5} | tail -1)
%{__sed} '%{start_rmLine},%{end_rmLine}d;s|section-5|section-3|g;s|<PRODUCT>|%{cray_name}-tests|g;s|<VERSION>|%{pkgversion}|g;s|<BUILD_METADATA>|%{version}|g;s|<RELEASE>|%{release}|g;s|<ARCH>|%{_arch}|g;s|<REMOVAL_DATE>|%{removal_date}|g;s|<SYS_HW_TAG>|%{SYS_HW_TAG}|g;s|<SYS_WB_TAG>|%{SYS_WB_TAG}|g;s|<OS_HW_TAG>|%{OS_HW_TAG}|g;s|<OS_WB_TAG>|%{OS_WB_TAG}|g;'/admin-pe'/d' %{SOURCE5} > %{_rpmdir}/%{_arch}/%{cray_name}-tests-%{pkgversion}-%{version}-%{release}.%{_arch}.rpm.yaml

%post
# Add Shasta configuration files for PE content projection
if [ "${RPM_INSTALL_PREFIX}" = "%{cray_prefix}" ]; then
    %{__mkdir_p} /etc/%{cray_prefix}/admin-pe/bindmount.conf.d/
    %{__mkdir_p} /etc/%{cray_prefix}/admin-pe/modulepaths.conf.d/
    echo "%{cray_prefix}/%{product}/%{pkgversion}" > /etc/%{cray_prefix}/admin-pe/bindmount.conf.d/%{cray_name}.conf
    echo "%{cray_prefix}/modulefiles/%{modulefile_name}" > /etc/%{cray_prefix}/admin-pe/modulepaths.conf.d/%{modulefile_name}.conf
    echo "%{cray_prefix}/lmod/modulefiles/core/%{cray_name}" >> /etc/%{cray_prefix}/admin-pe/modulepaths.conf.d/%{modulefile_name}.conf

    echo -e '#%Modulefile\r\nset  ModulesVersion "%{pkgversion}"' > %{cray_prefix}/lmod/modulefiles/core/%{cray_name}/.version
fi

# Process relocations
if [ "${RPM_INSTALL_PREFIX}" != "%{prefix}" ]
then
    # Modulefile
    %{__sed} -i "s|^\([[:space:]]*set GDB4HPC_BASEDIR[[:space:]]*\)\(.*\)|\1${RPM_INSTALL_PREFIX}/%{cray_product}/%{pkgversion}|" ${RPM_INSTALL_PREFIX}/modulefiles/%{modulefile_name}/%{pkgversion}
    # set default command
    %{__sed} -i "s|^\(export CRAY_inst_dir=\).*|\1${RPM_INSTALL_PREFIX}|" ${RPM_INSTALL_PREFIX}/%{cray_product}/%{pkgversion}/%{set_default_command}_%{cray_product}_%{pkgversion}
else
    ${RPM_INSTALL_PREFIX}/%{set_default_path}/%{set_default_command}_%{cray_product}_%{pkgversion}

    # Don't want to set LD_LIBRARY_PATH or GDB4HPC_INSTALL_DIR if we are not relocating since rpath was set properly
    # tcl module
    %{__sed} -i "/^[[:space:]]*prepend-path[[:space:]]*LD_LIBRARY_PATH.*/d" ${RPM_INSTALL_PREFIX}/modulefiles/%{modulefile_name}/%{pkgversion}
    %{__sed} -i "/^[[:space:]]*setenv[[:space:]]*GDB4HPC_INSTALL_DIR.*/d" ${RPM_INSTALL_PREFIX}/modulefiles/%{modulefile_name}/%{pkgversion}
    # lua module
    %{__sed} -i "/^prepend-path[[:space:]]*LD_LIBRARY_PATH.*/d" ${RPM_INSTALL_PREFIX}/lmod/modulefiles/core/%{cray_name}/%{pkgversion}.lua
    %{__sed} -i "/^setenv[[:space:]]*GDB4HPC_INSTALL_DIR.*/d" ${RPM_INSTALL_PREFIX}/lmod/modulefiles/core/%{cray_name}/%{pkgversion}.lua
fi

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
%ifnarch aarch64
%dir %{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb
%dir %{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python
%dir %{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb/command
%dir %{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb/function
%dir %{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb/printer
%dir %{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb
%dir %{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/syscalls
%dir %{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/system-gdbinit
%dir %{prefix}/%{cray_product}/%{pkgversion}/share/ROCgdb
%dir %{prefix}/%{cray_product}/%{pkgversion}/share/ROCgdb/python
%dir %{prefix}/%{cray_product}/%{pkgversion}/share/ROCgdb/python/gdb/command
%dir %{prefix}/%{cray_product}/%{pkgversion}/share/ROCgdb/python/gdb/function
%dir %{prefix}/%{cray_product}/%{pkgversion}/share/ROCgdb/python/gdb/printer
%dir %{prefix}/%{cray_product}/%{pkgversion}/share/ROCgdb/python/gdb
%dir %{prefix}/%{cray_product}/%{pkgversion}/share/ROCgdb/syscalls
%dir %{prefix}/%{cray_product}/%{pkgversion}/share/ROCgdb/system-gdbinit
%endif
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/%{release_info_name}
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/%{attributions_name}
%attr(755, root, root) %{prefix}/%{cray_product}/%{pkgversion}/lib/libbitset.so
%attr(755, root, root) %{prefix}/%{cray_product}/%{pkgversion}/lib/libdbgagg.so
%attr(755, root, root) %{prefix}/%{cray_product}/%{pkgversion}/lib/libmi.so
%{prefix}/%{cray_product}/%{pkgversion}/lib/libmrnet.so*
%{prefix}/%{cray_product}/%{pkgversion}/lib/libxplat.so*
%attr(755, root, root) %{prefix}/%{cray_product}/%{pkgversion}/lib/uda-plugin-cray-upc.so
%attr(755, root, root) %{prefix}/%{cray_product}/%{pkgversion}/libexec/dbgsrv
%attr(755, root, root) %{prefix}/%{cray_product}/%{pkgversion}/libexec/dbgsrv_alps
%attr(755, root, root) %{prefix}/%{cray_product}/%{pkgversion}/libexec/gdb-guard
%ifnarch aarch64
%attr(755, root, root) %{prefix}/%{cray_product}/%{pkgversion}/libexec/cuda101-gdb-guard
%attr(755, root, root) %{prefix}/%{cray_product}/%{pkgversion}/libexec/ROCgdb-guard
%endif
%attr(755, root, root) %{prefix}/%{cray_product}/%{pkgversion}/libexec/guard_proxy
%attr(755, root, root) %{prefix}/%{cray_product}/%{pkgversion}/libexec/start_proxy.sh
%attr(755, root, root) %{prefix}/%{cray_product}/%{pkgversion}/libexec/mrnet_commnode
%attr(755, root, root) %{prefix}/%{cray_product}/%{pkgversion}/libexec/mrnet_topgen
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
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb/command/tsx.py
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
#cuda-gdb share
%ifnarch aarch64
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb/FrameDecorator.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb/FrameIterator.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb/frames.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb/__init__.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb/printing.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb/prompt.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb/types.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb/unwinder.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb/xmethod.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb/command/explore.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb/command/frame_filters.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb/command/__init__.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb/command/pretty_printers.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb/command/prompt.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb/command/tsx.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb/command/type_printers.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb/command/unwinders.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb/command/xmethods.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb/function/as_string.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb/function/caller_is.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb/function/__init__.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb/function/strfns.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb/printer/bound_registers.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb/printer/__init__.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/syscalls/*.xml
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/syscalls/gdb-syscalls.dtd
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/system-gdbinit/elinos.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/system-gdbinit/wrs-linux.py
#ROCgdb share
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/ROCgdb/python/gdb/FrameDecorator.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/ROCgdb/python/gdb/FrameIterator.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/ROCgdb/python/gdb/frames.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/ROCgdb/python/gdb/__init__.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/ROCgdb/python/gdb/printing.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/ROCgdb/python/gdb/prompt.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/ROCgdb/python/gdb/types.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/ROCgdb/python/gdb/unwinder.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/ROCgdb/python/gdb/xmethod.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/ROCgdb/python/gdb/command/explore.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/ROCgdb/python/gdb/command/frame_filters.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/ROCgdb/python/gdb/command/__init__.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/ROCgdb/python/gdb/command/pretty_printers.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/ROCgdb/python/gdb/command/prompt.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/ROCgdb/python/gdb/command/type_printers.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/ROCgdb/python/gdb/command/unwinders.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/ROCgdb/python/gdb/command/xmethods.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/ROCgdb/python/gdb/function/as_string.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/ROCgdb/python/gdb/function/caller_is.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/ROCgdb/python/gdb/function/__init__.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/ROCgdb/python/gdb/function/strfns.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/ROCgdb/python/gdb/printer/bound_registers.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/ROCgdb/python/gdb/printer/__init__.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/ROCgdb/syscalls/*.xml
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/ROCgdb/syscalls/gdb-syscalls.dtd
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/ROCgdb/system-gdbinit/elinos.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/ROCgdb/system-gdbinit/wrs-linux.py
#end share
%endif
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/man/man1/gdb4hpc.1
%attr(755, root, root) %{prefix}/%{cray_product}/%{pkgversion}/%{set_default_command}_%{cray_product}_%{pkgversion}
%attr(755, root, root) %{prefix}/%{set_default_path}/%{set_default_command}_%{cray_product}_%{pkgversion}
%attr(755, root, root) %{prefix}/modulefiles/%{modulefile_name}/%{pkgversion}
%attr(644, root, root) %{prefix}/lmod/modulefiles/core/%{cray_name}/%{pkgversion}.lua

%files -n %{cray_name}-tests-%{pkgversion}
%defattr(755, root, root)
%dir %{prefix}/%{cray_product}/%{pkgversion}/tests
%dir %{prefix}/%{cray_product}/%{pkgversion}/tests/bd_dt
%dir %{prefix}/%{cray_product}/%{pkgversion}/tests/bd_dt/c
%dir %{prefix}/%{cray_product}/%{pkgversion}/tests/bd_dt/C++
%dir %{prefix}/%{cray_product}/%{pkgversion}/tests/bd_dt/fortran
%dir %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data
%dir %{prefix}/%{cray_product}/%{pkgversion}/tests/include

%attr(755, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/test_tool_starter.py
%attr(755, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/avocado_config.yaml
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/test_tool_config.yaml
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/Makefile.am
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/configure.ac
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/README.md

%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/bd_dt/Makefile.am

%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/bd_dt/c/Makefile.am
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/bd_dt/c/c_cmp_diff.c
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/bd_dt/c/c_type_pause.c
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/bd_dt/c/c_type_test.c
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/bd_dt/c/mtest.c
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/bd_dt/c/wtime.c
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/bd_dt/c/c_dlopen_test.c
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/bd_dt/c/libdlopen_test.c

%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/bd_dt/C++/Makefile.am
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/bd_dt/C++/cpp_arrays.cpp
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/bd_dt/C++/cpp_bd_test.cpp
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/bd_dt/C++/cpp_bt.cpp
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/bd_dt/C++/cpp_bt_stress.cpp
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/bd_dt/C++/cpp_cmp_diff.cpp
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/bd_dt/C++/cpp_decomp2.cpp
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/bd_dt/C++/cpp_decomp.cpp
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/bd_dt/C++/cpp_ds_test1.cpp
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/bd_dt/C++/cpp_ds_test2.cpp
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/bd_dt/C++/cpp_parallel_threads.cpp
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/bd_dt/C++/cpp_threads.cpp

%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/bd_dt/fortran/Makefile.am
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/bd_dt/fortran/ftn_arrays.f90
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/bd_dt/fortran/ftn_bd_dt.f90
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/bd_dt/fortran/ftn_cmp_diff.f90
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/bd_dt/fortran/ftn_decomp.f90

%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_assert_wdecomp.rc
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_assert_wdecomp.txt
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_assert_wodecomp.rc
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_assert_wodecomp.txt
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_assign.rc
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_assign.txt
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_breakpoint.rc
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_breakpoint.txt
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_breakpoint_pending.rc
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_breakpoint_pending.txt
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_cmp_false.rc
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_cmp_false.txt
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_cmp_true.rc
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_cmp_true.txt
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_defset.rc
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_defset.txt
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_finish.rc
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_finish.txt
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_focus.rc
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_focus.txt
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_frame.rc
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_frame.txt
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_gdbmode.rc
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_gdbmode.txt
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_help.rc
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_help.txt
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_info.rc
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_info.txt
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_kill.rc
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_kill.txt
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_list.rc
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_list.txt
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_next.rc
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_next.txt
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/cpp_arrays_compare.rc
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/cpp_arrays_compare.txt
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/cpp_bt.rc
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/cpp_bt.txt
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/cpp_decomp2.rc
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/cpp_decomp2.txt
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/cpp_decomp.rc
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/cpp_decomp.txt
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/cpp_thread_funcs.rc
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/cpp_thread_funcs.txt
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/cpp_thread_focus.rc
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/cpp_thread_focus.txt
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_print.rc
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_print.txt
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_release.rc
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_release.txt
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_set_unset.rc
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_set_unset.txt
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_show.rc
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_show.txt
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_sizeof.rc
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_sizeof.txt
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_step.rc
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_step.txt
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_up_down.rc
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_up_down.txt
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_viewset.rc
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_viewset.txt
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_whatis.rc
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/c_whatis.txt
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/ds1.txt
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/ftn_arrays.rc
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/ftn_arrays.txt
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/ftn_decomp.rc
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/ftn_decomp.txt
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/lgdb_cpp_ds1.rc
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/lgdb_cpp_ds1.txt
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/lgdb_cpp_t1.rc
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/lgdb_cpp_t1.txt
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/lgdb_cpp_t2.rc
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/lgdb_cpp_t2.txt
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/lgdb_cpp_t3.rc
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/lgdb_cpp_t3.txt
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/lgdb_ftn_t1.rc
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/lgdb_ftn_t1.txt
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/lgdb_ftn_t2.rc
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/lgdb_ftn_t2.txt
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/lgdb_ftn_t3.rc
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/lgdb_ftn_t3.txt
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/lgdb_openacc_t1.rc
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/lgdb_upc_t1.rc
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/open_acc_t1.txt
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/test1.txt
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/test2.txt
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/upc.txt
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/tests.py.data/qsub.template

%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/include/mpitest.h
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/include/mpitestconf.h
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/include/mpitestconf.h.in
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/include/mpitestcxx.h
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/include/mpithreadtest.h
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/tests/include/rectangle.h
