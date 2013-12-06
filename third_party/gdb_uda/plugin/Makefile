# Copyright 2007 by Intrepid Technology, Inc.
# Copyright 2007 by Hewlett-Packard.
# 
# Written by; Gary Funck <gary@intrepid.com>
# Reviewed by: Brian Wibecan and Tanya Klinchina, HP
# 
# All rights reserved.
# 
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
# 
# (1) Redistributions of source code must retain the above copyright
# notice, this list of conditions and the following disclaimer.
# (2) Redistributions in binary form must reproduce the above copyright
# notice, this list of conditions and the following disclaimer in the
# documentation and/or other materials provided with the distribution.
# (3) Neither the name of Intrepid Technology, or Hewlett-Packard
# nor the names of its contributors may be used to endorse or promote
# products derived from this software without specific prior written
# permission.
# 
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#
# Copyright 2013 Cray Inc. All Rights Reserved.
#

CC = gcc

OFLAGS = -O0 -g

# For testing, enable the following DEBUG switches
DFLAGS = -DDEBUG=1 -UNDEBUG -DDEBUG_UDA_PLUGIN=1
# DFLAGS = -DDEBUG=1 -UNDEBUG -DDEBUG_UDA_PLUGIN=1

# For UPC with struct pointer representation on 32 bit platform
# SPTRFLAGS = -DSTRUCT_SPTR_32BIT
# For UPC with packed pointer representation and struct on 64 bit platform
SPTRFLAGS =

WFLAGS = -W -Wall -Wwrite-strings -Wstrict-prototypes \
         -Wmissing-prototypes -Wno-long-long \
	 -Wno-variadic-macros -Wold-style-definition \
	 -Wmissing-format-attribute -Wno-unused-parameter \
	 -Werror
CFLAGS= $(OFLAGS) -fPIC $(WFLAGS) $(SPTRFLAGS) $(DFLAGS) -D__GCC_UPC__=1
IFLAGS = -I..
CPPFLAGS = $(IFLAGS)
PLUGIN_LIB = uda-plugin-cray-upc.so
LDFLAGS= -shared -o $(PLUGIN_LIB)

OBJS = uda-plugin.o uda-plugin-image.o uda-plugin-job.o \
       uda-plugin-shmem.o uda-plugin-pts.o uda-plugin-thread.o \
       uda-plugin-utils.o

all: $(PLUGIN_LIB)

$(PLUGIN_LIB):	$(OBJS)
	$(CC) $(LDFLAGS) $(OBJS)

tags: uda-*.[ch]
	-@ctags uda-*.[ch] ../uda-*.[ch]

clean:
	-@rm -f $(OBJS) $(PLUGIN_LIB)
	@-rm -f core core.* *~ *.bak gdb.log

scratch: clean
	-@rm -f tags

depend:
	-makedepend -I.. -Y *.c 1>/dev/null 2>&1

# Makefile dependencies
# DO NOT DELETE

uda-plugin.o: ../uda-types.h ../uda-defs.h uda-plugin-config.h
uda-plugin.o: uda-plugin-types.h uda-plugin-defs.h ../uda-plugin-cb.h
uda-plugin.o: uda-plugin.h uda-plugin-utils.h
uda-plugin-image.o: ../uda-types.h ../uda-defs.h uda-plugin-config.h
uda-plugin-image.o: uda-plugin-types.h uda-plugin-defs.h ../uda-plugin-cb.h
uda-plugin-image.o: uda-plugin.h uda-plugin-utils.h
uda-plugin-job.o: ../uda-types.h ../uda-defs.h uda-plugin-config.h
uda-plugin-job.o: uda-plugin-types.h uda-plugin-defs.h ../uda-plugin-cb.h
uda-plugin-job.o: uda-plugin.h uda-plugin-utils.h
uda-plugin-pts.o: ../uda-types.h ../uda-defs.h uda-plugin-config.h
uda-plugin-pts.o: uda-plugin-types.h uda-plugin-defs.h ../uda-plugin-cb.h
uda-plugin-pts.o: uda-plugin.h uda-plugin-utils.h
uda-plugin-shmem.o: ../uda-types.h ../uda-defs.h uda-plugin-config.h
uda-plugin-shmem.o: uda-plugin-types.h uda-plugin-defs.h ../uda-plugin-cb.h
uda-plugin-shmem.o: uda-plugin.h uda-plugin-utils.h
uda-plugin-thread.o: ../uda-types.h ../uda-defs.h uda-plugin-config.h
uda-plugin-thread.o: uda-plugin-types.h uda-plugin-defs.h ../uda-plugin-cb.h
uda-plugin-thread.o: uda-plugin.h uda-plugin-utils.h
uda-plugin-utils.o: ../uda-types.h ../uda-defs.h uda-plugin-config.h
uda-plugin-utils.o: uda-plugin-types.h uda-plugin-defs.h ../uda-plugin-cb.h
uda-plugin-utils.o: uda-plugin.h uda-plugin-utils.h
