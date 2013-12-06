AM_CFLAGS = -g -O0 -h noc99

# build convienence libraries
noinst_LIBRARIES = libshallow.a libshallow2.a
libshallow_a_SOURCES = calc.c copy.c diag.c init.c \
		       time.c tstep.c dump.c decs.h
libshallow2_a_SOURCES = calc2.c copy2.c diag2.c init2.c \
			time2.c tstep2.c dump.c decs.h

# build binaries
bin_PROGRAMS = cshallow pshallow pshallow2

cshallow_SOURCES = cshallow.c
cshallow_LDFLAGS = -lm

pshallow_SOURCES = main.c
pshallow_LDFLAGS = -lm
pshallow_LDADD = libshallow.a

pshallow2_SOURCES = main2.c
pshallow2_LDFLAGS = -lm
pshallow2_LDADD = libshallow2.a

