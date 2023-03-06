UNITTEST = @gdb4hpc_srcdir@/unittest

AM_LIBTOOLFLAGS = --quiet

noinst_LIBRARIES	= libuseful.a
libuseful_a_CXXFLAGS    = -I$(UNITTEST)
libuseful_a_SOURCES 	= alloc.cpp args.cpp debug.cpp error.cpp exec.cpp \
			  hashfunctions.cpp list.cpp log.cpp maths.cpp \
			  path.cpp queue.cpp str.cpp onscopeexit.cpp parse.cpp signal_util.cpp
