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

BISON = /usr/bin/bison
srcdir = ../src/gdb

OFLAGS = -O0 -g

# For testing, enable the following DEBUG switches
#DFLAGS = -DDEBUG=1 -UNDEBUG -DDEBUG_UDA_RMT=1
# For maximum debug output, uncomment the following line.
#DFLAGS = -DDEBUG=1 -UNDEBUG -DDEBUG_UDA_RMT=1 -DDEBUG_LEX=1

OBJS =	uda-main.o \
	uda-rmt-server-parse.o \
	uda-rmt-utils.o \
	uda-server.o \
	uda-server-plugin.o

WFLAGS = -W -Wall -Wwrite-strings -Wstrict-prototypes \
         -Wmissing-prototypes -Wno-long-long \
	 -Wno-variadic-macros -Wold-style-definition \
	 -Wmissing-format-attribute -Wno-unused-parameter \
#	 -Werror
YYWFLAGS = -Wno-old-style-definition
CFLAGS = $(OFLAGS) $(WFLAGS) $(DFLAGS)
LDFLAGS = -ldl -Wl,-rpath=`pwd`/plugin

GEN_FILES = uda-rmt-server-parse.c


all: tags udaserver
	@cd plugin && $(MAKE) all

udaserver: $(OBJS)
	$(LINK.c) $(CFLAGS) -o $@ $^

uda-rmt-server-parse.c: uda-rmt-server-parse.y
	$(BISON) -p uda_server_cmd_yy -o uda-rmt-server-parse.c $<

uda-rmt-server-parse.h: uda-rmt-server-parse.c

#
# Disable some warnings when compiling yacc and lex generated files.
#
uda-rmt-server-parse.o: uda-rmt-server-parse.c
	$(COMPILE.c) $(CFLAGS) $(YYWFLAGS) $(OUTPUT_OPTION) $<

clean:
	@-rm -f $(OBJS) udaserver
	@-rm -f core core.* *~ *.bak gdb.log
	@cd plugin && $(MAKE) clean

scratch: clean
	@-rm -f tags $(GEN_FILES)
	@cd plugin && $(MAKE) scratch

tags: uda-*.[ch] plugin/uda-*.[ch]
	-@ctags $^
	@cd plugin && $(MAKE) tags

depend:
	-makedepend -Y *.c 1>/dev/null 2>&1
	@cd plugin && $(MAKE) depend

#
# Dependencies
# DO NOT DELETE

uda-main.o: uda-defs.h uda-types.h uda-rmt-utils.h uda-server.h
uda-rmt-utils.o: uda-defs.h uda-types.h uda-rmt-utils.h
uda-server.o: uda-defs.h uda-types.h uda-rmt-utils.h uda-server-plugin.h
uda-server.o: uda-plugin-cb.h uda-server.h
uda-server-plugin.o: uda-defs.h uda-types.h uda-server-plugin.h
uda-server-plugin.o: uda-plugin-cb.h
