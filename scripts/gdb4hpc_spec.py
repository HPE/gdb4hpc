#!/usr/bin/env python
content='''%define namespace [namespace]
%define two_dig_version [package_branch_two_digit]
%define two_dig_nodot_version [package_branch_two_digit_nodot]
%define build_version [version]
%define scripts_d [script_dir]
%define arch [ARCH]
#%define man_source /cray/css/compiler/comp_rel/pubs/manpages/gdb4hpc/%{two_dig_version}/xt_gdb4hpc_%{two_dig_nodot_version}.cpio

Summary: Performance Application Programming Interface
Name: %{namespace}-%{build_version} 
Version: [revision]
Release: [release][sles_sub]
License: BSD
Group: Development/System
URL: [url]
Source: [tarball]
BuildRoot: %{_tmppath}/%{name}-root
Prefix: /opt/cray/pe
Requires: cray-cti >= 1.0.5
Vendor: Cray Inc.
Requires: set_default_2

%define _use_internal_dependency_generator 0
%define __find_requires %{scripts_d}/find-requires 
%define debug_package %{nil}

%description
LGDB4HPC is a GDB-based parallel debugger used to debug applications compiled with CCE, PGI, GNU and Intel Fortran, C and C++ compilers.

# version
%define major_version %(echo %{build_version} | awk -v n=1 'BEGIN { FS = "." } ; { print $n }')
%define minor_version %(echo %{build_version} | awk -v n=2 'BEGIN { FS = "." } ; { print $n }')

# _prefix
%define _namespace_prefix %{prefix}
%define _name_prefix %{_namespace_prefix}/%{namespace}
%define _version_prefix %{_name_prefix}/%{build_version}

# _moduledir
%define _namespace_moduledir %{prefix}/modulefiles
%define _name_moduledir %{_namespace_moduledir}/%{namespace}

%prep
%setup
%build
%define is_valid %(if [ "%{arch}" == "x86_64" ]; then echo "1"; else echo "0"; fi)


%install

mkdir -p %{buildroot}%{_version_prefix}
mkdir -p %{buildroot}/%{_namespace_moduledir}/cray-lgdb
cp -r gdb4hpc/* %{buildroot}%{_version_prefix}/
rm -rf %{buildroot}%{_version_prefix}/modulefiles

pushd %{buildroot}%{_namespace_moduledir}/cray-lgdb
ln -s ../%{namespace}/%{build_version}
popd

install -D modulefiles/%{build_version} %{buildroot}%{_name_moduledir}/%{build_version}
install -D gdb4hpc/release_info %{buildroot}%{_version_prefix}/
echo "%{build_version}-%{release}" > %{buildroot}%{_version_prefix}/.cray_rpm_release
mkdir -p %{buildroot}%{_namespace_prefix}/admin-pe/set_default_files/
install -D set_default_%{namespace}_%{build_version} %{buildroot}%{_namespace_prefix}/admin-pe/set_default_files/set_default_%{namespace}_%{build_version}
install -D set_default_%{namespace}_%{build_version} %{buildroot}%{_version_prefix}/
cp gdb4hpc/docs/ATTRIBUTIONS_gdb4hpc.txt %{buildroot}%{_version_prefix}/ATTRIBUTIONS_gdb4hpc.%{build_version}.txt



# hardlink duplicate files to save space
#%fdupes %{buildroot}

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root,-)
%dir %{_name_moduledir}
%dir %{_namespace_prefix}
%dir %{_name_prefix}
%dir %{_version_prefix}
%dir %{_version_prefix}/bin
%dir %{_version_prefix}/docs

%{_version_prefix}/bin/%{namespace}
%dir %{_version_prefix}/libexec
%{_version_prefix}/libexec/dbgsrv
%{_version_prefix}/libexec/dbgsrv_alps
#%{_version_prefix}/libexec/dlaunch 
%{_version_prefix}/libexec/gdb-guard
%{_version_prefix}/libexec/mrnet_commnode 
%{_version_prefix}/libexec/guard_proxy
%{_version_prefix}/libexec/start_proxy.sh

%if %is_valid
  %{_version_prefix}/libexec/cuda75-gdb-guard
  %{_version_prefix}/libexec/cuda80-gdb-guard
  %{_version_prefix}/libexec/cuda91-gdb-guard
%endif

%{_version_prefix}/demos
%{_version_prefix}/lib
%{_version_prefix}/man

%{_namespace_prefix}/admin-pe/set_default_files/set_default_%{namespace}_%{build_version}
%{_name_moduledir}/%{build_version}
%{_namespace_moduledir}/cray-lgdb/%{build_version}
%{_version_prefix}/.cray_rpm_release
%{_version_prefix}/set_default_%{namespace}_%{build_version}
%{_version_prefix}/release_info
%{_version_prefix}/ATTRIBUTIONS_gdb4hpc.%{build_version}.txt
%{_version_prefix}/docs/ATTRIBUTIONS_gdb4hpc.txt


%post
#
# Set as default if no default exists either because this is first install of
# gdb4hpc or CRAY_INSTALL_DEFAULT=1 and previous default was deleted
#

#Set the install path in the modulefile & set_default script(s)
sed -i "s,\[install_dir\],$RPM_INSTALL_PREFIX,g" \
$RPM_INSTALL_PREFIX/modulefiles/%{namespace}/%{build_version} \
$RPM_INSTALL_PREFIX/admin-pe/set_default_files/set_default_%{namespace}_%{build_version} \
$RPM_INSTALL_PREFIX/%{namespace}/%{build_version}/set_default_%{namespace}_%{build_version}

# prevent echo of new directory as it messes up install output
if [[ $RPM_INSTALL_PREFIX = "/opt/cray" ]] || [[ $RPM_INSTALL_PREFIX = "/opt/cray/pe" ]]
  then
    if [ ${CRAY_INSTALL_DEFAULT:-0} -eq 1 ] || [ ! -f $RPM_INSTALL_PREFIX/modulefiles/gdb4hpc/.version ]
    then
      $RPM_INSTALL_PREFIX/admin-pe/set_default_files/set_default_%{namespace}_%{build_version}
    else
      echo "%{namespace}_%{build_version} has been installed as non-default."
    fi
fi

#for XE/XK CLE5.2 create lgdb link pointing to gdb4hpc in bin dir.
ln -s $RPM_INSTALL_PREFIX/%{namespace}/%{build_version}/bin/%{namespace} $RPM_INSTALL_PREFIX/%{namespace}/%{build_version}/bin/lgdb

%preun
# Cleanup default link if it point to this version
default_link="${RPM_INSTALL_PREFIX}/%{namespace}/default"
version="%{build_version}"

# Cleanup module .version if it points to this version
if [ -f ${RPM_INSTALL_PREFIX}/modulefiles/%{namespace}/.version ]
then
  dotversion=`grep ModulesVersion ${RPM_INSTALL_PREFIX}/modulefiles/%{namespace}/.version | cut -f2 -d'"'`
  
  if [ "$dotversion" = "$version" ]
  then
    /bin/rm -f ${RPM_INSTALL_PREFIX}/modulefiles/%{namespace}/.version
    echo "Uninstalled version and .version file match = ${version}."
    echo "Removing %{namespace} .version file."
    rm -f ${default_link}
  fi
fi

if [[ -z `ls ${RPM_INSTALL_PREFIX}/%{namespace}` ]]
then
  rm -rf ${RPM_INSTALL_PREFIX}/%{namespace}
fi

if [[ -z `ls ${RPM_INSTALL_PREFIX}/modulefiles/%{namespace}`/ ]]
then
  rm -rf ${RPM_INSTALL_PREFIX}/modulefiles/%{namespace}
fi

%changelog

'''
