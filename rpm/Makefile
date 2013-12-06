# set before inclcuding definitions
TOPDIR = $(CURDIR)/..

include $(TOPDIR)/definitions.mk

RPM_RELEASE ?= 0

.PHONY: all clean rpm

all: rpm

rpm: $(PACKAGE_NAME).spec
	mkdir -p BUILD RPMS SRPMS
	rpmbuild -ba --define "_sourcedir $(RPMBUILD_SOURCEDIR)" --define "_topdir $(RPMBUILD_TOPDIR)" $<

$(PACKAGE_NAME).spec: $(PACKAGE_NAME).spec.template
	sed -e 's/\[intranamespace_name\]/$(INTRANAMESPACE_NAME)/g' \
            -e 's/\[namespace\]/$(NAMESPACE)/g' \
            -e 's/\[rpm_release\]/$(RPM_RELEASE)/g' \
            -e 's/\[tarball\]/$(TARBALL)/g' \
            -e 's/\[version\]/$(PACKAGE_VERSION)/g' \
            -e 's/\[package_branch_two_digit\]/$(PACKAGE_BRANCH_TWO_DIGIT)/g' \
            -e 's/\[package_branch_two_digit_nodot\]/$(PACKAGE_BRANCH_TWO_DIGIT_NODOT)/g' \
            -e 's/\[package_revision\]/$(PACKAGE_REVISION)/g' \
            $< > $@

clean:  
	$(RM) --force --recursive BUILD RPMS SRPMS
	$(RM) --force $(PACKAGE_NAME).spec
