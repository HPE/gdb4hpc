TESTS_ROOT = @doctest_root@/doctest

AM_LIBTOOLFLAGS = --quiet

noinst_LIBRARIES	= libuseful.a
libuseful_a_CXXFLAGS    = -I$(TESTS_ROOT)
libuseful_a_SOURCES 	= alloc.cpp args.cpp debug.cpp error.cpp exec.cpp \
			  hash.cpp hashfunctions.cpp list.cpp log.cpp maths.cpp \
			  path.cpp queue.cpp str.cpp onscopeexit.cpp
