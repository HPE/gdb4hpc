ifndef TOPDIR
  $(error Define TOPDIR before including definitions.mk)
endif

NAMESPACE = cray
INTRANAMESPACE_NAME = lgdb
PACKAGE_NAME = $(NAMESPACE)-$(INTRANAMESPACE_NAME)
URL = $(shell svn info $(TOPDIR) | awk  '/^URL/ { print $$2 }')
PACKAGE_REVISION = $(shell svn info $(TOPDIR) | awk  '/^Last Changed Rev/ { print $$4 }')

PACKAGE_BRANCH = 2.0.2.0
PACKAGE_BRANCH_TWO_DIGIT = $(shell echo $(PACKAGE_BRANCH) | cut -d '.' -f1-2)
PACKAGE_BRANCH_TWO_DIGIT_NODOT = $(shell echo $(PACKAGE_BRANCH_TWO_DIGIT) | sed 's/\.//g')
PACKAGE_BRANCH_THREE_DIGIT = $(shell echo $(PACKAGE_BRANCH) | cut -d '.' -f1-3)

DROPOFF=/cray/css/pe/dropoff
PKGS=/cray/css/pe/pkgs/cray-lgdb/$(PACKAGE_BRANCH_TWO_DIGIT)

ifeq ($(FINAL), 1)
  PACKAGE_VERSION = $(PACKAGE_BRANCH_THREE_DIGIT)
else
  PACKAGE_VERSION = $(PACKAGE_BRANCH)
endif

RPM_RELEASE=$(shell $(TOPDIR)/./fetch_release.py $(PACKAGE-NAME)-$(PACKAGE_VERSION) $(DROPOFF) $(PACKAGE_REVISION) 2>&1)

EXPORT_DIR = $(PACKAGE_NAME)-$(PACKAGE_VERSION)
TARBALL = $(EXPORT_DIR).tar.gz

RPMBUILD_SOURCEDIR = $(TOPDIR)/dist
RPMBUILD_TOPDIR = $(TOPDIR)/rpm
