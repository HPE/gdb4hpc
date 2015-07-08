# set before inclcuding definitions
TOPDIR = $(CURDIR)

include $(TOPDIR)/definitions.mk

RPM_RELEASE ?= 0

.PHONY: all

files: $(PACKAGE_NAME).spec \
	set_default_$(NAMESPACE)-$(INTRANAMESPACE_NAME)_$(PACKAGE_VERSION) \
	$(INTRANAMESPACE_NAME).module \
	release_info

$(PACKAGE_NAME).spec: $(PACKAGE_NAME).spec.template.new
	sed -e 's/\[intranamespace_name\]/$(INTRANAMESPACE_NAME)/g' \
            -e 's/\[namespace\]/$(NAMESPACE)/g' \
            -e 's/\[rpm_release\]/$(RPM_RELEASE)/g' \
            -e 's/\[tarball\]/$(TARBALL)/g' \
            -e 's/\[version\]/$(PACKAGE_VERSION)/g' \
            -e 's/\[package_branch_two_digit\]/$(PACKAGE_BRANCH_TWO_DIGIT)/g' \
            -e 's/\[package_branch_two_digit_nodot\]/$(PACKAGE_BRANCH_TWO_DIGIT_NODOT)/g' \
            -e 's/\[package_revision\]/$(PACKAGE_REVISION)/g' \
            $< > $@

set_default_$(NAMESPACE)-$(INTRANAMESPACE_NAME)_$(PACKAGE_VERSION):
	sed -e 's/\[product_name\]/$(INTRANAMESPACE_NAME)/g' \
            -e 's/\[version_string\]/$(PACKAGE_VERSION)/g' \
            -e 's/\[module_name_list\]/$(NAMESPACE)-$(INTRANAMESPACE_NAME)/g' \
            set_default_template > $@

$(INTRANAMESPACE_NAME).module: $(INTRANAMESPACE_NAME).module.template
	sed -e 's|\[VERSION_NUMBER\]|$(PACKAGE_VERSION)|g' $< > $@

release_info:
	sed -e 's/\%VERSION\%/$(PACKAGE_VERSION)/g' \
            -e 's/\%RELEASE\%/$(RPM_RELEASE)/g' \
            -e 's/\%REVISION\%/$(PACKAGE_REVISION)/g' \
            -e 's/\%RELEASEDATE\%/$(RELEASE_DATE)/g'  \
            release_info > $@




