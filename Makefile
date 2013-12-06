TOPDIR = $(CURDIR)

include $(TOPDIR)/definitions.mk

# targets that don't generate files of the same name
.PHONY: all dist rpm

all: dist rpm

clean:
	$(MAKE) --directory dist $@
	$(MAKE) --directory rpm $@

dist:
	$(MAKE) --directory $@

rpm: dist
	$(MAKE) --directory $@

rpm_name:
	@echo $(NAMESPACE)-$(INTRANAMESPACE_NAME)-$(PACKAGE_VERSION)-$(RPM_RELEASE)_$(PACKAGE_REVISION).x86_64.rpm

manifest:
	$(if $(shell ls rpm/RPMS/x86_64/cray-$(INTRANAMESPACE_NAME)-*), , $(error Missing RPMs))
	./generate_manifest.pl $(NAMESPACE)-$(INTRANAMESPACE_NAME) $(shell ls rpm/RPMS/x86_64/$(NAMESPACE)-$(INTRANAMESPACE_NAME)-* | cut -d "-" -f3 | head -n 1) $(shell ls rpm/RPMS/x86_64/$(NAMESPACE)-$(INTRANAMESPACE_NAME)-* | cut -d "-" -f4 | cut -d "." -f 1 | head -n 1) > rpm/RPMS/x86_64/$(shell ls rpm/RPMS/x86_64/$(NAMESPACE)-$(INTRANAMESPACE_NAME)-* | cut -d "/" -f4 | head -n1).yaml

deploy_rpm:
	cp rpm/RPMS/x86_64/$(NAMESPACE)-$(INTRANAMESPACE_NAME)*.rpm $(PKGS)/

install_rpm:
	cp rpm/RPMS/x86_64/$(NAMESPACE)-$(INTRANAMESPACE_NAME)* $(DROPOFF)/

