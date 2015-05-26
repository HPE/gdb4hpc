#!/usr/bin/env python

import os
import definitions

content='''---
#----------------------------------------------------------------------------
# IMPORTANT: do not rename sections! The installer uses section names
# to detect changes made to sections, using the MD5 algorithm.
#----------------------------------------------------------------------------
section-1:
    ACCELERATOR           : NONE
    EFFECTIVE_DATE        : NONE
    EMAILS                : kdavis@cray.com
    EMAIL_BUILDER         : NO
    HOSTS                 : ALL 
    INSTALL_DEFAULT_HOSTS : NONE
    NETWORK_TYPE:
        ALL_EXCEPT:
            - SEASTAR
    OPERATION             : INSTALL
    OS_VERSION:
        ALL_EXCEPT:
            - 2.2
            - 2.1
    RPM_LIST              : NONE
    SYSTEM_TYPE           : HARDWARE

section-2:
    ACCELERATOR           : NONE
    EFFECTIVE_DATE        : NONE
    EMAILS                : kdavis@cray.com
    EMAIL_BUILDER         : NO
    HOSTS:
        ALL_EXCEPT:
            - chpbld01
            - chpbld02
    INSTALL_DEFAULT_HOSTS : NONE
    NETWORK_TYPE          : NONE
    OPERATION             : INSTALL
    OS_VERSION:
        ALL_EXCEPT:
            - SLES10
    RPM_LIST              : NONE
    SYSTEM_TYPE           : WHITEBOX

section-3:
    ACCELERATOR           : NONE
    EFFECTIVE_DATE        : NONE
    EMAILS                : kdavis@cray.com
    EMAIL_BUILDER         : NO
    HOSTS                 : ALL
    INSTALL_DEFAULT_HOSTS : NONE
    NETWORK_TYPE:
        ALL_EXCEPT:
            - SEASTAR
    OPERATION             : UPDATE
    OS_VERSION:
        ALL_EXCEPT:
            - 2.2
            - 2.1
    RPM_LIST              : NONE
    SYSTEM_TYPE           : HARDWARE
    COMMANDS: |
        /opt/cray/admin-pe/set_default_files/set_default_cray-lgdb_<version>

section-4:
    ACCELERATOR           : NONE
    EFFECTIVE_DATE        : NONE
    EMAILS                : kdavis@cray.com
    EMAIL_BUILDER         : NO
    HOSTS:
        ALL_EXCEPT:
            - chpbld01
            - chpbld02
    INSTALL_DEFAULT_HOSTS : NONE
    NETWORK_TYPE          : NONE
    OPERATION             : UPDATE
    OS_VERSION:
        ALL_EXCEPT:
            - SLES10
    RPM_LIST              : NONE
    SYSTEM_TYPE           : WHITEBOX
    COMMANDS: |
        /opt/cray/admin-pe/set_default_files/set_default_cray-lgdb_<version>

section-5:
    ACCELERATOR           : NONE
    EFFECTIVE_DATE        : 2017-09-19
    EMAILS                : kdavis@cray.com
    EMAIL_BUILDER         : NO
    HOSTS                 : ALL
    INSTALL_DEFAULT_HOSTS : NONE
    NETWORK_TYPE          : NONE
    OPERATION             : REMOVE
    OS_VERSION            : ALL
    RPM_LIST              : NONE
    SYSTEM_TYPE           : ALL
    COMMANDS: |
        rpm -e cray-lgdb-<version>-<release>_<revision>

--- |
#Remove previous versions of this rpm
for package in  $(rpm -qa | grep cray-lgdb | grep <version>)
do
  rpm -e $package
done

rpm -hiv --oldpackage cray-lgdb-<version>-<release>_<revision>.x86_64.rpm

'''
manifest_dest = definitions.rpm_name + ".yaml"

manifest_content = content.replace("<version>", definitions.version)
manifest_content = manifest_content.replace("<release>", definitions.release_number)
manifest_content = manifest_content.replace("<revision>", definitions.revision_number)

manifest_yaml = open(manifest_dest, "w+")
manifest_yaml.write(manifest_content)
manifest_yaml.close()



