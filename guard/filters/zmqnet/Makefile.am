ZMQNET = @zmqnet_root@

AM_CPPFLAGS = -I$(ZMQNET)/include
AM_LIBTOOLFLAGS = --quiet

lib_LTLIBRARIES		     = libzmqnetagg.la
libzmqnetagg_la_SOURCES	 = zmqnetagg.cpp
libzmqnetagg_la_CXXFLAGS = $(CXXFLAGS) -fPIC
libzmqnetagg_la_LDFLAGS	 = $(LDFLAGS) -shared
