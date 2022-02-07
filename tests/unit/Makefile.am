# Unit tests for gdb4hpc
# Copyright 2021 Hewlett Packard Enterprise Development LP.

# This assumes you have built and installed gdb4hpc and its required libraries.

UNIT   = $(shell pwd)
BITSET = ../../src/bitset
PREFIX = $(prefix)
GTEST  = $(UNIT)/googletest/googletest

bin_PROGRAMS = unit_tests
BUILT_SOURCES = libgtest.so

unit_tests_SOURCES = unit_tests.cpp bitset_unit_tests.cpp
unit_tests_CPPFLAGS = -O2 -std=c++17 -I$(GTEST) -I$(GTEST)/include -I$(BITSET)
unit_tests_LDFLAGS = -Wl,-rpath,$(UNIT) -Wl,-rpath,$(PREFIX)/lib
unit_tests_LDADD = -L$(UNIT) -lgtest -L$(PREFIX)/lib -lbitset

libgtest.so: googletest/googletest/src/gtest-all.cc
	$(CXX) -pthread -I$(GTEST)/include -I$(GTEST) -fPIC -shared $^ -o $@

clean-local:
	rm -f libgtest.so
