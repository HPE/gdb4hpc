
# set before include so svn info commands are run on correct directory
TOPDIR = $(CURDIR)/..

include $(TOPDIR)/definitions.mk

.PHONY: all clean

FILES = set_default_$(NAMESPACE)-$(INTRANAMESPACE_NAME)_$(PACKAGE_VERSION) \
        $(INTRANAMESPACE_NAME).module \
	release_info

all: tarball

export: $(FILES)
	$(RM) --force --recursive $(EXPORT_DIR)
	svn export $(TOPDIR) $(EXPORT_DIR)
	cp $(FILES) $(EXPORT_DIR)

tarball: export
	tar --bzip2 --create --file $(TARBALL) --verbose $(EXPORT_DIR)	


# This section generates all files package files needed by the rpm:

set_default_$(NAMESPACE)-$(INTRANAMESPACE_NAME)_$(PACKAGE_VERSION):
	sed -e 's/\[product_name\]/$(INTRANAMESPACE_NAME)/g' \
            -e 's/\[version_string\]/$(PACKAGE_VERSION)/g' \
            -e 's|\[install_dir\]|/opt/cray|g' \
            -e 's|\[module_dir\]|/opt/cray/modulefiles/|g' \
            -e 's/\[module_name_list\]/$(NAMESPACE)-$(INTRANAMESPACE_NAME)/g' \
            set_default_template > $@

$(INTRANAMESPACE_NAME).module: $(INTRANAMESPACE_NAME).module.template
	sed -e 's|\[VERSION_NUMBER\]|$(PACKAGE_VERSION)|g' $< > $@

release_info:
	sed -e 's/\%VERSION\%/$(PACKAGE_VERSION)/g' \
	    -e 's/\%RELEASE\%/$(RPM_RELEASE)/g' \
            ../release_info > $@

clean:
	$(RM) --force --recursive $(EXPORT_DIR)
	$(RM) --force $(TARBALL)
	$(RM) --force $(FILES)


