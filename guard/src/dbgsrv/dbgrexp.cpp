/*********************************************************************************\
 * Regex interface routines
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * © 2010-2013,2020 Cray Inc.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Cray Inc.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *
 * $HeadURL$
 * $Date$
 * $Rev$
 * $Author$
 *
 *********************************************************************************/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dbgrexp.h"
#include "defs.h"
#include "useful.h"

list_t *RegexList = (list_t *)NULL;
dbgrexp_t *RegexError;
dbgrexp_t *RegexPrompt;

void InitRegexGDB(void) {
    (void)AddRegex(
        "ERROR: ld.so: object .* from LD_PRELOAD cannot be preloaded: "
        "ignored.$",
        0, 0, GDBREX_IGNORE);

    (void)AddRegex("^Breakpoint ([0-9]+),", REG_NOTEOL, 1, GDBREX_BPHIT);

    (void)AddRegex("^Program received signal ([^,]+), ([^.]+)\\.", 0, 2,
                   GDBREX_SIGNAL);

    (void)AddRegex("^Program terminated with signal ([^,]+), ([^.]+)\\.", 0, 2,
                   GDBREX_SIGNAL);

    (void)AddRegex("^Program exited with code (0[0-7]+)\\.", 0, 1, GDBREX_EXIT);

    (void)AddRegex("^Program exited normally\\.", 0, 0, GDBREX_EXIT2);

    (void)AddRegex("^Source directories searched: (.*)", 0, 1, GDBREX_SHOW);

    (void)AddRegex("^Num", REG_NOTEOL, 0, GDBREX_HDR);

    (void)AddRegex(
        "^([0-9]+) +([^ ]+) +([^ ]+) +([yn]) +(0x[0-9a-fA-F]+) in ([^ ]+) at "
        "([^:]+):([0-9]+)$",
        0, 8, GDBREX_BPINFO);

    (void)AddRegex(
        "^([0-9]+) +([^ ]+) +([^ ]+) +([yn]) +(0x[0-9a-fA-F]+) (.*)$", 0, 6,
        GDBREX_BPINFO2);

    (void)AddRegex("breakpoint already hit ([0-9]+) time",
                   REG_NOTBOL | REG_NOTEOL, 1, GDBREX_BPHITCOUNT);

    (void)AddRegex("^Note: breakpoint", REG_NOTEOL, 0, GDBREX_BPDUP);

    (void)AddRegex(
        "^Breakpoint ([0-9]+) at (0x[0-9a-fA-F]+): file ([^,]+), line "
        "([0-9]+)\\.",
        0, 4, GDBREX_BPSET);

    (void)AddRegex("^Single stepping until exit from function [_0-9a-zA-Z]+,",
                   0, 1, GDBREX_STEP);

    (void)AddRegex("^which has no line number information.", 0, 0,
                   GDBREX_STEP2);

    (void)AddRegex("^Breakpoint ([0-9]+) at (0x[0-9a-fA-F]+)", 0, 2,
                   GDBREX_BPSET2);

    (void)AddRegex("^Function \"([^\"]+)\" not defined\\.", 0, 1,
                   GDBREX_NOFUNC);

    (void)AddRegex("^No breakpoint( or watchpoint)? number ([0-9]+)\\.", 0, 2,
                   GDBREX_NOBP);

    (void)AddRegex("^No line ([0-9]+) in file", REG_NOTEOL, 1, GDBREX_NOLINE);

    (void)AddRegex("^Line number ([0-9]+) is out of range", REG_NOTEOL, 1,
                   GDBREX_OUTOFRANGE);

    (void)AddRegex("^No source file named (.*)\\.", 0, 1, GDBREX_NOFILE);

    (void)AddRegex("^Argument required (breakpoint number)\\.", 0, 0,
                   GDBREX_BADBPARG);

    (void)AddRegex("^(0x[0-9a-fA-F]+) in ([^)]+\\)) at ([^:]+):([0-9]+)", 0, 4,
                   GDBREX_SIGFRAME);

    (void)AddRegex(
        "^#([0-9]+) +(0x[0-9a-fA-F]+) in ([^)]+\\)) at ([^:]+):([0-9]+)", 0, 5,
        GDBREX_FRAME);

    (void)AddRegex("^#0 +([^)]+\\)) at ([^:]+):([0-9]+)", 0, 3, GDBREX_L0FRAME);

    (void)AddRegex("^Using host libthread_db library ", REG_NOTEOL, 0,
                   GDBREX_IGNORE);

    (void)AddRegex("^Attaching to process", 0, 0, GDBREX_IGNORE);

    (void)AddRegex("^ptrace: No such process", 0, 0, GDBREX_ATTACH_FAILED);

    (void)AddRegex("^\\[Thread debugging using libthread_db enabled\\]", 0, 0,
                   GDBREX_IGNORE);

    (void)AddRegex("^\\[New Thread [0-9]*[ ]?\\(LWP ([0-9]+)\\)\\]", 0, 1,
                   GDBREX_PID);

    (void)AddRegex("^\\[New process [0-9]+\\]", 0, 0, GDBREX_IGNORE);

    /* This might be needed if threads are involved, but for now simply ignore
     */
    (void)AddRegex("^\\[Switching to Thread [0-9]+ \\(LWP [0-9]+\\)\\]", 0, 0,
                   GDBREX_IGNORE);

    (void)AddRegex("^) at [^:]+:[0-9]+", 0, 0, GDBREX_IGNORE);

    (void)AddRegex("([^)]+\\)) at ([^:]+):([0-9]+)", 0, 3, GDBREX_SFRAME);

    (void)AddRegex("^#([0-9]+) +(0x[0-9a-fA-F]+) in ([^)]+\\))", 0, 3,
                   GDBREX_IFRAME);

    (void)AddRegex("(0x[0-9a-fA-F]+) in ([^)]+\\))", 0, 2, GDBREX_MINFRAME);

    (void)AddRegex("^Initial", REG_NOTEOL, 0, GDBREX_FIRSTFRAME);

    (void)AddRegex("^Bottom", REG_NOTEOL, 0, GDBREX_LASTFRAME);

    (void)AddRegex("^No stack\\.", 0, 0, GDBREX_NOSTACK);

    (void)AddRegex("^No symbol \"([^\"]*)\" in current context\\.", 0, 1,
                   GDBREX_NOSYM);

    (void)AddRegex("^Cannot access memory", REG_NOTEOL, 0, GDBREX_NOMEM);

    (void)AddRegex("^The program is not being run\\.", 0, 0, GDBREX_NOTRUN);

    (void)AddRegex("^Undefined command", REG_NOTEOL, 0, GDBREX_UNDEF);

    (void)AddRegex("^pid=([0-9]+)", 0, 1, GDBREX_PID);

    (void)AddRegex("^process ([0-9]+)", 0, 1, GDBREX_PID);

    (void)AddRegex("^Inferior has process ID ([0-9]+)\\.", 0, 1, GDBREX_PID);

    (void)AddRegex(
        "^Line ([0-9]+) of \"([^\"]+)\" starts at address (0x[0-9a-fA-F]+)",
        REG_NOTEOL, 3, GDBREX_LOC);

    (void)AddRegex(
        "^Line ([0-9]+) of \"([^\"]+)\" is at address (0x[0-9a-fA-F]+)",
        REG_NOTEOL, 3, GDBREX_LOCNOCODE);

    (void)AddRegex("^GDB ([0-9]+\\.[0-9]+) \\(([^)]+)\\)", REG_NOTEOL, 2,
                   GDBREX_VERS);

    (void)AddRegex("^GDB ([0-9]+\\.[0-9]+) \\(([^)]+)\\), Copyright",
                   REG_NOTEOL, 2, GDBREX_VERS2);

    (void)AddRegex("GNU gdb (.+)", REG_NOTEOL, 1, GDBREX_VERS3);

    (void)AddRegex("^GDB is free software", REG_NOTEOL, 0, GDBREX_IGNORE);

    (void)AddRegex("^ under certain conditions", REG_NOTEOL, 0, GDBREX_IGNORE);

    (void)AddRegex("^There is absolutely no warranty", REG_NOTEOL, 0,
                   GDBREX_IGNORE);

    (void)AddRegex("^Copyright (199[0-9])|(200[0-9]) Free Software Foundation",
                   REG_NOTEOL, 0, GDBREX_INTRO4);

    (void)AddRegex("^This GDB was configured as \"([^\"]*)\"\\.", 0, 1,
                   GDBREX_INTRO5);

    (void)AddRegex("^welcome to change it and/or distribute copies", REG_NOTEOL,
                   0, GDBREX_IGNORE);

    (void)AddRegex("^Type \"show copying\" to see the conditions", REG_NOTEOL,
                   0, GDBREX_IGNORE);

    (void)AddRegex("^\\(no debugging symbols found\\)\\.\\.\\.\\(gdb\\)$", 0, 0,
                   GDBREX_NOSYMS);

    (void)AddRegex("^type = (.*)", 0, 1, GDBREX_TYPE);

    (void)AddRegex("^global = (.*)", 0, 1, GDBREX_VARS);

    (void)AddRegex("^VAR:(.*)$", 0, 1, GDBREX_VARS);

    (void)AddRegex("^\\$[0-9]+ = ([^ ]*) .*", 0, 1, GDBREX_DATA);

    (void)AddRegex("^\\$[0-9]+ = (.*)", 0, 1, GDBREX_DATA2);

    (void)AddRegex("^(.*)exit.c: No such file or directory\\.", 0, 0,
                   GDBREX_IGNORE);

    (void)AddRegex("^(.*): No such file or directory\\.", 0, 1,
                   GDBREX_NOFILEDIR);

    (void)AddRegex("^(.*): not in executable format: (.*)", 0, 2,
                   GDBREX_NOTEXEC);

    (void)AddRegex("Reading symbols.*", 0, 0, GDBREX_SYMBOLS);

    (void)AddRegex("Current language:[ ]*(.*)", 0, 1, GDBREX_LANG);

    (void)AddRegex("Source file is more recent than executable\\.", 0, 0,
                   GDBREX_SOURCE);

    (void)AddRegex("warning: .*cloned process", REG_NOTEOL, 0, GDBREX_IGNORE);

    (void)AddRegex("(Warning: .*)", 0, 1, GDBREX_WARN);

    (void)AddRegex("\\[Switching to process", 0, 0, GDBREX_IGNORE);

    (void)AddRegex("^cmdline = ", REG_NOTEOL, 0, GDBREX_IGNORE);

    (void)AddRegex("^cwd = ", REG_NOTEOL, 0, GDBREX_IGNORE);

    (void)AddRegex("^exe = ", REG_NOTEOL, 0, GDBREX_IGNORE);

    /*
    ** Windows NT messages to be ignored.
    */

    (void)AddRegex("[0-9a-f]+:.*\\.dll", 0, 0, GDBREX_IGNORE);

    (void)AddRegex("\\[failed reading symbols from DLL\\]", 0, 0,
                   GDBREX_IGNORE);

    (void)AddRegex("[^:]*: error reading line numbers", 0, 0, GDBREX_IGNORE);

    (void)AddRegex("[^:]*: error reading line numbers", 0, 0, GDBREX_IGNORE);

    (void)AddRegex("\\(symbols previously loaded\\)", 0, 0, GDBREX_IGNORE);

    (void)AddRegex("\tin .+", 0, 0, GDBREX_IGNORE);

    /*
    ** GDBREX_LINE must be last as it is most general.
    */

    (void)AddRegex("^([0-9]+)\t(\t*.*)", 0, 2, GDBREX_LINE);

    RegexPrompt = AddRegex("^\\(gdb\\)", 0, 0, GDBREX_PROMPT);

    RegexError = AddRegex("", 0, 0, REX_NULL);
}

void InitRegexIDB(void) {
    (void)AddRegex("^Breakpoint ([0-9]+),", REG_NOTEOL, 1, IDBREX_BPHIT);

    (void)AddRegex("^Program received signal ([^,]+), ([^.]+)\\.", 0, 2,
                   IDBREX_SIGNAL);

    (void)AddRegex("^Program terminated with signal ([^,]+), ([^.]+)\\.", 0, 2,
                   IDBREX_SIGNAL);

    (void)AddRegex("^Program exited with code (0[0-7]+)\\.", 0, 1, IDBREX_EXIT);

    (void)AddRegex("^Program exited normally\\.", 0, 0, IDBREX_EXIT2);

    (void)AddRegex("^Num", REG_NOTEOL, 0, IDBREX_HDR);

    (void)AddRegex(
        "^([0-9]+) +([^ ]+) +([^ ]+) +([yn]) +(0x[0-9a-fA-F]+) in ([^ ]+) at "
        "([^:]+):([0-9]+)$",
        0, 8, IDBREX_BPINFO);

    (void)AddRegex(
        "^([0-9]+) +([^ ]+) +([^ ]+) +([yn]) +(0x[0-9a-fA-F]+) (.*)$", 0, 6,
        IDBREX_BPINFO2);

    (void)AddRegex("breakpoint already hit ([0-9]+) time",
                   REG_NOTBOL | REG_NOTEOL, 1, IDBREX_BPHITCOUNT);

    (void)AddRegex("^Note: breakpoint", REG_NOTEOL, 0, IDBREX_BPDUP);

    (void)AddRegex(
        "^Breakpoint ([0-9]+) at (0x[0-9a-fA-F]+): file ([^,]+), line "
        "([0-9]+)\\.",
        0, 4, IDBREX_BPSET);

    (void)AddRegex("^Single stepping until exit from function [_0-9a-zA-Z]+,",
                   0, 1, IDBREX_STEP);

    (void)AddRegex("^which has no line number information.", 0, 0,
                   IDBREX_STEP2);

    (void)AddRegex("^Breakpoint ([0-9]+) at (0x[0-9a-fA-F]+)", 0, 2,
                   IDBREX_BPSET2);

    (void)AddRegex("^Function \"([^\"]+)\" not defined\\.", 0, 1,
                   IDBREX_NOFUNC);

    (void)AddRegex("^No breakpoint( or watchpoint)? number ([0-9]+)\\.", 0, 2,
                   IDBREX_NOBP);

    (void)AddRegex("^No line ([0-9]+) in file", REG_NOTEOL, 1, IDBREX_NOLINE);

    (void)AddRegex("^Line number ([0-9]+) is out of range", REG_NOTEOL, 1,
                   IDBREX_OUTOFRANGE);

    (void)AddRegex("^No source file named (.*)\\.", 0, 1, IDBREX_NOFILE);

    (void)AddRegex("^Argument required (breakpoint number)\\.", 0, 0,
                   IDBREX_BADBPARG);

    (void)AddRegex("^(0x[0-9a-fA-F]+) in ([^)]+\\)) at ([^:]+):([0-9]+)", 0, 4,
                   IDBREX_SIGFRAME);

    (void)AddRegex(
        "^#([0-9]+) +(0x[0-9a-fA-F]+) in ([^)]+\\)) at ([^:]+):([0-9]+)", 0, 5,
        IDBREX_FRAME);

    (void)AddRegex("^#0 +([^)]+\\)) at ([^:]+):([0-9]+)", 0, 3, IDBREX_L0FRAME);

    (void)AddRegex("^\\* 1 Thread ([0-9]+) \\(LWP ", REG_NOTEOL, 1,
                   IDBREX_PID); /* Must be put before IDBREX_SFRAME */

    (void)AddRegex("^\\[New process [0-9]+\\]", 0, 0, IDBREX_IGNORE);  // Donny

    (void)AddRegex("([^)]+\\)) at ([^:]+):([0-9]+)", 0, 3, IDBREX_SFRAME);

    (void)AddRegex("^#([0-9]+) +(0x[0-9a-fA-F]+) in ([^)]+\\))", 0, 3,
                   IDBREX_IFRAME);

    (void)AddRegex("(0x[0-9a-fA-F]+) in ([^)]+\\))", 0, 2, IDBREX_MINFRAME);

    (void)AddRegex("^Initial", REG_NOTEOL, 0, IDBREX_FIRSTFRAME);

    (void)AddRegex("^Bottom", REG_NOTEOL, 0, IDBREX_LASTFRAME);

    (void)AddRegex("^No stack\\.", 0, 0, IDBREX_NOSTACK);

    (void)AddRegex("^No symbol \"([^\"]*)\" in current context\\.", 0, 1,
                   IDBREX_NOSYM);

    (void)AddRegex("^Cannot access memory", REG_NOTEOL, 0, IDBREX_NOMEM);

    (void)AddRegex("^The program is not being run\\.", 0, 0, IDBREX_NOTRUN);

    (void)AddRegex("^Undefined command", REG_NOTEOL, 0, IDBREX_UNDEF);

    (void)AddRegex("^pid=([0-9]+)", 0, 1, IDBREX_PID);

    (void)AddRegex("^process ([0-9]+)", 0, 1, IDBREX_PID);

    (void)AddRegex("^Inferior has process ID ([0-9]+)\\.", 0, 1, IDBREX_PID);

    (void)AddRegex(
        "^Line ([0-9]+) of \"([^\"]+)\" starts at address (0x[0-9a-fA-F]+)",
        REG_NOTEOL, 3, IDBREX_LOC);

    (void)AddRegex(
        "^Line ([0-9]+) of \"([^\"]+)\" is at address (0x[0-9a-fA-F]+)",
        REG_NOTEOL, 3, IDBREX_LOCNOCODE);

    (void)AddRegex("^GDB ([0-9]+\\.[0-9]+) \\(([^)]+)\\)", REG_NOTEOL, 2,
                   IDBREX_VERS);

    (void)AddRegex("^GDB ([0-9]+\\.[0-9]+) \\(([^)]+)\\), Copyright",
                   REG_NOTEOL, 2, IDBREX_VERS2);

    (void)AddRegex("^\\(no debugging symbols found\\)\\.\\.\\.\\(gdb\\)$", 0, 0,
                   IDBREX_NOSYMS);

    (void)AddRegex("^type = (.*)", 0, 1, IDBREX_TYPE);

    (void)AddRegex("^global = (.*)", 0, 1, IDBREX_VARS);

    (void)AddRegex("^\\$[0-9]+ = ([^ ]*) .*", 0, 1, IDBREX_DATA);

    (void)AddRegex("^\\$[0-9]+ = (.*)", 0, 1, IDBREX_DATA2);

    (void)AddRegex("^(.*): No such file or directory\\.", 0, 1,
                   IDBREX_NOFILEDIR);

    (void)AddRegex("^(.*): not in executable format: (.*)", 0, 2,
                   IDBREX_NOTEXEC);

    (void)AddRegex("Reading symbols.*", 0, 0, IDBREX_SYMBOLS);

    (void)AddRegex("Current language:[ ]*(.*)", 0, 1, IDBREX_LANG);

    (void)AddRegex("Source file is more recent than executable\\.", 0, 0,
                   IDBREX_SOURCE);

    (void)AddRegex("warning: ", REG_NOTEOL, 0, IDBREX_WARN);

    (void)AddRegex("\\[Switching to process", 0, 0, IDBREX_IGNORE);

    (void)AddRegex("^cmdline = ", REG_NOTEOL, 0, IDBREX_IGNORE);

    (void)AddRegex("^cwd = ", REG_NOTEOL, 0, IDBREX_IGNORE);

    (void)AddRegex("^exe = ", REG_NOTEOL, 0, IDBREX_IGNORE);

    /*
    ** IDBREX_LINE must be last as it is most general.
    */

    (void)AddRegex("^[ ]*([0-9]+)[ ]*(\t*.*)", 0, 2, IDBREX_LINE);

    RegexPrompt = AddRegex("^\\(gdb\\)", 0, 0, IDBREX_PROMPT);

    RegexError = AddRegex("", 0, 0, REX_NULL);
}

void InitRegexGDBMI(void) {
    (void)AddRegex(
        "\\*stopped,reason=\"breakpoint-hit\",bkptno=\"([0-9]+)\",thread-id=\"["
        "0-9]+\",frame=\\{addr=\"(0x[0-9a-fA-F]+)\",func=\"[^,]+\",",
        0, 1, GDBMIREX_BPHIT);

    (void)AddRegex(
        "\\*stopped,reason=\"breakpoint-hit\",disp=\"[^\"]+\",bkptno=\"([0-9]+)"
        "\",frame=",
        0, 1, GDBMIREX_BPHIT);

    (void)AddRegex("^Program received signal ([^,]+), ([^.]+)\\.", 0, 2,
                   GDBMIREX_SIGNAL);

    (void)AddRegex("^Program terminated with signal ([^,]+), ([^.]+)\\.", 0, 2,
                   GDBMIREX_SIGNAL);

    (void)AddRegex("^Program exited with code (0[0-7]+)\\.", 0, 1,
                   GDBMIREX_EXIT);

    (void)AddRegex("^Program exited normally\\.", 0, 0, GDBMIREX_EXIT2);

    (void)AddRegex(
        "\\^done,BreakpointTable=\\{nr_rows=\"[^\"]+\",nr_cols=\"[^\"]+\",hdr="
        "\\[\\{width=\"[^\"]+\",alignment=\"[^\"]+\",col_name=\"[^\"]+\","
        "colhdr=\"[^\"]+\"\\},\\{width=\"[^\"]+\",alignment=\"[^\"]+\",col_"
        "name=\"[^\"]+\",colhdr=\"[^\"]+\"\\},\\{width=\"[^\"]+\",alignment=\"["
        "^\"]+\",col_name=\"[^\"]+\",colhdr=\"[^\"]+\"\\},\\{width=\"[^\"]+\","
        "alignment=\"[^\"]+\",col_name=\"[^\"]+\",colhdr=\"[^\"]+\"\\},\\{"
        "width=\"[^\"]+\",alignment=\"[^\"]+\",col_name=\"[^\"]+\",colhdr=\"[^"
        "\"]+\"\\},\\{width=\"[^\"]+\",alignment=\"[^\"]+\",col_name=\"[^\"]+"
        "\",colhdr=\"[^\"]+\"\\}\\],body=\\[bkpt=\\{number=\"([0-9]+)\",type="
        "\"([^\"]+)\",disp=\"([^\"]+)\",enabled=\"([yn])\",addr=\"(0x[0-9a-fA-"
        "F]+)\",func=\"([^\"]+)\",file=\"([^\"]+)\",fullname=\"[^\"]+\",line="
        "\"([0-9]+)\",times=\"[0-9]+\"\\}\\]\\}",
        0, 8, GDBMIREX_BPINFO);

    (void)AddRegex(
        "^([0-9]+) +([^ ]+) +([^ ]+) +([yn]) +(0x[0-9a-fA-F]+) (.*)$", 0, 6,
        GDBMIREX_BPINFO2);

    (void)AddRegex("breakpoint already hit ([0-9]+) time",
                   REG_NOTBOL | REG_NOTEOL, 1, GDBMIREX_BPHITCOUNT);

    (void)AddRegex("^Note: breakpoint", REG_NOTEOL, 0, GDBMIREX_BPDUP);

    (void)AddRegex(
        "\\^done,bkpt=\\{number=\"([0-9]+)\",type=\"breakpoint\",disp=\"keep\","
        "enabled=\"y\",addr=\"(0x[0-9a-fA-F]+)\",func=\"[^,]+\",file=\"([^,]+)"
        "\",fullname=\"[^,]+\",line=\"([0-9]+)\",times=\"[0-9]+\"\\}",
        0, 4, GDBMIREX_BPSET);

    (void)AddRegex(
        "\\*stopped,reason=\"end-stepping-range\",frame=\\{addr=\"[^\"]+\","
        "func=\"[^\"]+\",args=\"[[^\"]]+],file=\"[^\"]+\",fullname=\"[^\"]+\","
        "line=\"([^\"]+)\"},thread-id=\"[^\"]+\",stopped-threads=\"[^\"]+\","
        "core=\"[^\"]+\"",
        0, 1, GDBMIREX_STEP);

    (void)AddRegex("^Single stepping until exit from function [_0-9a-zA-Z]+,",
                   0, 1, GDBMIREX_STEP);

    (void)AddRegex("^which has no line number information.", 0, 0,
                   GDBMIREX_STEP2);

    (void)AddRegex("&\"Function \\\\\"([^\"]+)\\\\\" not defined\\.", 0, 1,
                   GDBMIREX_NOFUNC);

    (void)AddRegex("^No breakpoint( or watchpoint)? number ([0-9]+)\\.", 0, 2,
                   GDBMIREX_NOBP);

    (void)AddRegex("^No line ([0-9]+) in file", REG_NOTEOL, 1, GDBMIREX_NOLINE);

    (void)AddRegex("~\"Line number ([0-9]+) is out of range", REG_NOTEOL, 1,
                   GDBMIREX_OUTOFRANGE);

    (void)AddRegex("^No source file named (.*)\\.", 0, 1, GDBMIREX_NOFILE);

    (void)AddRegex("^Argument required (breakpoint number)\\.", 0, 0,
                   GDBMIREX_BADBPARG);

    (void)AddRegex("^(0x[0-9a-fA-F]+) in ([^)]+\\)) at ([^:]+):([0-9]+)", 0, 4,
                   GDBMIREX_SIGFRAME);

    (void)AddRegex(
        "^#([0-9]+) +(0x[0-9a-fA-F]+) in ([^)]+\\)) at ([^:]+):([0-9]+)", 0, 5,
        GDBMIREX_FRAME);

    (void)AddRegex(
        "\\^done,stack=\\[frame=\\{level=\"0\",addr=\"0x[0-9a-fA-F]+\",func=\"("
        "[^,]+)\",file=\"([^,]+)\",fullname=\"[^,]+\",line=\"([0-9]+)\"\\}\\]",
        0, 3, GDBMIREX_L0FRAME);

    (void)AddRegex("~\"Using host libthread_db library ", 0, 0,
                   GDBMIREX_IGNORE);

    (void)AddRegex("^Attaching to process", 0, 0, GDBMIREX_IGNORE);

    (void)AddRegex("^ptrace: No such process", 0, 0, GDBMIREX_ATTACH_FAILED);

    (void)AddRegex("^\\[Thread debugging using libthread_db enabled\\]", 0, 0,
                   GDBMIREX_IGNORE);

    (void)AddRegex("^\\[New Thread [0-9]+ \\(LWP ([0-9]+)\\)\\]", 0, 1,
                   GDBMIREX_PID);

    (void)AddRegex("^\\[New process [0-9]+\\]", 0, 0, GDBMIREX_IGNORE);

    /* This might be needed if threads are involved, but for now simply ignore
     */
    (void)AddRegex("^\\[Switching to Thread [0-9]+ \\(LWP [0-9]+\\)\\]", 0, 0,
                   GDBMIREX_IGNORE);

    (void)AddRegex("([^)]+\\)) at ([^:]+):([0-9]+)", 0, 3, GDBMIREX_SFRAME);

    (void)AddRegex("^#([0-9]+) +(0x[0-9a-fA-F]+) in ([^)]+\\))", 0, 3,
                   GDBMIREX_IFRAME);

    (void)AddRegex("(0x[0-9a-fA-F]+) in ([^)]+\\))", 0, 2, GDBMIREX_MINFRAME);

    (void)AddRegex("^Initial", REG_NOTEOL, 0, GDBMIREX_FIRSTFRAME);

    (void)AddRegex("^Bottom", REG_NOTEOL, 0, GDBMIREX_LASTFRAME);

    (void)AddRegex("^No stack\\.", 0, 0, GDBMIREX_NOSTACK);

    (void)AddRegex("^No symbol \"([^\"]*)\" in current context\\.", 0, 1,
                   GDBMIREX_NOSYM);

    (void)AddRegex("^Cannot access memory", REG_NOTEOL, 0, GDBMIREX_NOMEM);

    (void)AddRegex("^The program is not being run\\.", 0, 0, GDBMIREX_NOTRUN);

    (void)AddRegex("^Undefined command", REG_NOTEOL, 0, GDBMIREX_UNDEF);

    (void)AddRegex("&\"dbg pid", 0, 0, GDBMIREX_IGNORE);

    (void)AddRegex("~\"pid=([0-9]+)", 0, 1, GDBMIREX_PID);

    (void)AddRegex("^process ([0-9]+)", 0, 1, GDBMIREX_PID);

    (void)AddRegex("^Inferior has process ID ([0-9]+)\\.", 0, 1, GDBMIREX_PID);

    (void)AddRegex(
        "~\"Line ([0-9]+) of \\\\\"([^\"]+)\\\\\" starts at address "
        "(0x[0-9a-fA-F]+)",
        REG_NOTEOL, 3, GDBMIREX_LOC);

    (void)AddRegex(
        "~\"Line ([0-9]+) of \\\\\"([^\"]+)\\\\\" is at address "
        "(0x[0-9a-fA-F]+)",
        REG_NOTEOL, 3, GDBMIREX_LOCNOCODE);

    (void)AddRegex("~\"GNU gdb (.+)\\\\n", 0, 1, GDBMIREX_VERS);

    (void)AddRegex("~\"GDB is free software", 0, 0, GDBMIREX_IGNORE);

    // rm "^" to ignore possible leading escape sequence
    (void)AddRegex("~\"GNU gdb (.+)\\\\n", 0, 1, GDBMIREX_VERS);

    (void)AddRegex("~\"(.+) under certain conditions", 0, 0, GDBMIREX_IGNORE);

    (void)AddRegex("~\"GDB is free software", 0, 0, GDBMIREX_IGNORE);

    (void)AddRegex("~\"There is absolutely no warranty", REG_NOTEOL, 0,
                   GDBMIREX_IGNORE);

    (void)AddRegex("~\"(.+) under certain conditions", 0, 0, GDBMIREX_IGNORE);

    (void)AddRegex(
        "~\"Copyright \\(C\\) 199[0-9]|200[0-9] Free Software Foundation", 0, 0,
        GDBMIREX_IGNORE);

    (void)AddRegex("~\"This GDB was configured as \\\\\"([^\"]*)\\\\\"\\.", 0,
                   1, GDBMIREX_INTRO);

    (void)AddRegex("^welcome to change it and/or distribute copies", REG_NOTEOL,
                   0, GDBMIREX_IGNORE);

    (void)AddRegex("~\"Type \\\\\"show copying\\\\\" to see the conditions", 0,
                   0, GDBMIREX_IGNORE);

    (void)AddRegex("^\\(no debugging symbols found\\)\\.\\.\\.\\(gdb\\)$", 0, 0,
                   GDBMIREX_NOSYMS);

    (void)AddRegex("^type = (.*)", 0, 1, GDBMIREX_TYPE);

    (void)AddRegex("^global = (.*)", 0, 1, GDBMIREX_VARS);

    (void)AddRegex("^\\$[0-9]+ = ([^ ]*) .*", 0, 1, GDBMIREX_DATA);

    (void)AddRegex("^\\$[0-9]+ = (.*)", 0, 1, GDBMIREX_DATA2);

    (void)AddRegex("^(.*): No such file or directory\\.", 0, 1,
                   GDBMIREX_NOFILEDIR);

    (void)AddRegex("^(.*): not in executable format: (.*)", 0, 2,
                   GDBMIREX_NOTEXEC);

    (void)AddRegex("Reading symbols.*", 0, 0, GDBMIREX_SYMBOLS);

    (void)AddRegex("Current language:[ ]*(.*)", 0, 1, GDBMIREX_LANG);

    (void)AddRegex("Source file is more recent than executable\\.", 0, 0,
                   GDBMIREX_SOURCE);

    (void)AddRegex("warning: .*cloned process", REG_NOTEOL, 0, GDBMIREX_IGNORE);

    (void)AddRegex("warning: ", REG_NOTEOL, 0, GDBMIREX_WARN);

    (void)AddRegex("\\[Switching to process", 0, 0, GDBMIREX_IGNORE);

    (void)AddRegex("^cmdline = ", REG_NOTEOL, 0, GDBMIREX_IGNORE);

    (void)AddRegex("^cwd = ", REG_NOTEOL, 0, GDBMIREX_IGNORE);

    (void)AddRegex("^exe = ", REG_NOTEOL, 0, GDBMIREX_IGNORE);

    (void)AddRegex("\\^done", 0, 0, GDBMIREX_DONE);

    (void)AddRegex("\\^running", 0, 0, GDBMIREX_RUNNING);

    (void)AddRegex("\\^exit", 0, 0, GDBMIREX_IGNORE);

    (void)AddRegex("&\"list", 0, 0, GDBMIREX_IGNORE);

    (void)AddRegex("&\"info line", 0, 0, GDBMIREX_IGNORE);

    /*
    ** GDBMIREX_LINE must be last as it is most general.
    */

    (void)AddRegex("~\"([0-9]+)\\\\t(.*)", 0, 2, GDBMIREX_LINE);

    RegexPrompt = AddRegex("^\\(gdb\\)", 0, 0, GDBMIREX_PROMPT);

    RegexError = AddRegex("", 0, 0, REX_NULL);
}

dbgrexp_t *AddRegex(char *pat, int flags, int nm, regexev_t event) {
    dbgrexp_t *d;

    if (RegexList == (list_t *)NULL) RegexList = NewList();

    if (nm > 0) nm++;

    d = (dbgrexp_t *)Alloc(sizeof(dbgrexp_t));
    d->d_pattern = pat;
    d->d_flags = flags;
    d->d_nmatch = nm;
    d->d_event = event;
    d->d_regex = (regex_t *)Alloc(sizeof(regex_t));

    if (d->d_event != REX_NULL) {
        if ((d->d_res = regcomp(d->d_regex, pat, REG_EXTENDED)) != 0) {
            RegexGetError(d);
            fprintf(stderr, "Compiling pattern: \"");
            fprintf(stderr, "%s\"\n", d->d_pattern);
            fprintf(stderr, "produced error: \"");
            fprintf(stderr, "%s\"\n", d->d_errbuf);
            FreeRegex(d);
            Free(d->d_regex);
            Free(d);
            return (dbgrexp_t *)NULL;
        }

        if (nm > 0)
            d->d_match = (regmatch_t *)Alloc(sizeof(regmatch_t) * nm);
        else
            d->d_match = (regmatch_t *)NULL;
    }

    AddToList(RegexList, (void *)d);

    return d;
}

dbgrexp_t *MatchRegexList(char *s) {
    int res;
    dbgrexp_t *d;

    SetList(RegexList);

    while ((d = (dbgrexp_t *)GetListElement(RegexList)) != (dbgrexp_t *)NULL) {
        if (d->d_event == REX_NULL) continue;

        if ((res = MatchRegex(d, s)) < 0) {
            fprintf(stderr, "%s\n", d->d_errbuf);
            return RegexError;
        }

        if (res) return d;
    }

    return (dbgrexp_t *)NULL;
}

#define MATCH_LEN 1024

int MatchRegex(dbgrexp_t *d, char *s) {
    int c = 0;

    if (strlen(s) > MATCH_LEN) {
        c = s[MATCH_LEN];
        s[MATCH_LEN] = '\0';
    }

    if ((d->d_res = regexec(d->d_regex, s, d->d_nmatch, d->d_match,
                            d->d_flags)) == 0) {
        d->d_string = s;
        if (c) s[MATCH_LEN] = c;
        return 1;
    }

    if (c) s[MATCH_LEN] = c;

    if (d->d_res == REG_NOMATCH) return 0;

    RegexGetError(d);

    return -1;
}

void RegexGetError(dbgrexp_t *d) {
    regerror(d->d_res, d->d_regex, d->d_errbuf, sizeof(d->d_errbuf));
}

void FreeRegex(dbgrexp_t *d) { regfree(d->d_regex); }

char *MatchToStr(dbgrexp_t *d, int i) {
    int len;
    char *s;
    regmatch_t *m;

    m = &d->d_match[i];

    len = m->rm_eo - m->rm_so;

    s = (char *)Alloc(len + 1);

    bcopy(d->d_string + m->rm_so, s, len);

    s[len] = '\0';

    return s;
}

int MatchToInt(dbgrexp_t *d, int i, int base) {
    int res;
    char *s;

    s = MatchToStr(d, i);
    res = (int)strtol(s, NULL, base);
    Free(s);

    return res;
}

#ifdef DEBUG
char *GetRegexEvent(regexev_t event) {
    switch (event) {
        /* GDB Regex */
        case GDBREX_PROMPT:
            return "GDBREX_PROMPT";
        case GDBREX_BPHIT:
            return "GDBREX_BPHIT";
        case GDBREX_SIGNAL:
            return "GDBREX_SIGNAL";
        case GDBREX_EXIT:
            return "GDBREX_EXIT";
        case GDBREX_EXIT2:
            return "GDBREX_EXIT2";
        case GDBREX_SHOW:
            return "GDBREX_SHOW";
        case GDBREX_LINE:
            return "GDBREX_LINE";
        case GDBREX_HDR:
            return "GDBREX_HDR";
        case GDBREX_BPINFO:
            return "GDBREX_BPINFO";
        case GDBREX_BPINFO2:
            return "GDBREX_BPINFO2";
        case GDBREX_BPHITCOUNT:
            return "GDBREX_BPHITCOUNT";
        case GDBREX_BPDUP:
            return "GDBREX_BPDUP";
        case GDBREX_BPSET:
            return "GDBREX_BPSET";
        case GDBREX_BPSET2:
            return "GDBREX_BPSET2";
        case GDBREX_STEP:
            return "GDBREX_STEP";
        case GDBREX_STEP2:
            return "GDBREX_STEP2";
        case GDBREX_NOFUNC:
            return "GDBREX_NOFUNC";
        case GDBREX_NOLINE:
            return "GDBREX_NOLINE";
        case GDBREX_OUTOFRANGE:
            return "GDBREX_OUTOFRANGE";
        case GDBREX_NOFILE:
            return "GDBREX_NOFILE";
        case GDBREX_NOFILEDIR:
            return "GDBREX_NOFILEDIR";
        case GDBREX_NOBP:
            return "GDBREX_NOBP";
        case GDBREX_BADBPARG:
            return "GDBREX_BADBPARG";
        case GDBREX_SIGFRAME:
            return "GDBREX_SIGFRAME";
        case GDBREX_FRAME:
            return "GDBREX_FRAME";
        case GDBREX_L0FRAME:
            return "GDBREX_L0FRAME";
        case GDBREX_SFRAME:
            return "GDBREX_SFRAME";
        case GDBREX_IFRAME:
            return "GDBREX_IFRAME";
        case GDBREX_FIRSTFRAME:
            return "GDBREX_FIRSTFRAME";
        case GDBREX_LASTFRAME:
            return "GDBREX_LASTFRAME";
        case GDBREX_MINFRAME:
            return "GDBREX_MINFRAME";
        case GDBREX_NOSTACK:
            return "GDBREX_NOSTACK";
        case GDBREX_NOSYM:
            return "GDBREX_NOSYM";
        case GDBREX_NOMEM:
            return "GDBREX_NOMEM";
        case GDBREX_NOTRUN:
            return "GDBREX_NOTRUN";
        case GDBREX_NOSYMS:
            return "GDBREX_NOSYMS";
        case GDBREX_NOTEXEC:
            return "GDBREX_NOTEXEC";
        case GDBREX_LOC:
            return "GDBREX_LOC";
        case GDBREX_LOCNOCODE:
            return "GDBREX_LOCNOCODE";
        case GDBREX_VARS:
            return "GDBREX_VARS";
        case GDBREX_TYPE:
            return "GDBREX_TYPE";
        case GDBREX_DATA:
            return "GDBREX_DATA";
        case GDBREX_DATA2:
            return "GDBREX_DATA2";
        case GDBREX_INTRO1:
            return "GDBREX_INTRO1";
        case GDBREX_INTRO2:
            return "GDBREX_INTRO2";
        case GDBREX_INTRO3:
            return "GDBREX_INTRO3";
        case GDBREX_INTRO4:
            return "GDBREX_INTRO4";
        case GDBREX_INTRO5:
            return "GDBREX_INTRO5";
        case GDBREX_INTRO6:
            return "GDBREX_INTRO6";
        case GDBREX_INTRO7:
            return "GDBREX_INTRO7";
        case GDBREX_INTRO8:
            return "GDBREX_INTRO8";
        case GDBREX_VERS:
            return "GDBREX_VERS";
        case GDBREX_VERS2:
            return "GDBREX_VERS2";
        case GDBREX_VERS3:
            return "GDBREX_VERS3";
        case GDBREX_SYMBOLS:
            return "GDBREX_SYMBOLS";
        case GDBREX_LANG:
            return "GDBREX_LANG";
        case GDBREX_SOURCE:
            return "GDBREX_SOURCE";
        case GDBREX_UNDEF:
            return "GDBREX_UNDEF";
        case GDBREX_PID:
            return "GDBREX_PID";
        case GDBREX_WARN:
            return "GDBREX_WARN";
        case GDBREX_IGNORE:
            return "GDBREX_IGNORE";
        case GDBREX_ATTACH:
            return "GDBREX_ATTACH";
        case GDBREX_ATTACH_FAILED:
            return "GDBREX_ATTACH_FAILED";

        /* IDB Regex */
        case IDBREX_PROMPT:
            return "IDBREX_PROMPT";
        case IDBREX_BPHIT:
            return "IDBREX_BPHIT";
        case IDBREX_SIGNAL:
            return "IDBREX_SIGNAL";
        case IDBREX_EXIT:
            return "IDBREX_EXIT";
        case IDBREX_EXIT2:
            return "IDBREX_EXIT2";
        case IDBREX_LINE:
            return "IDBREX_LINE";
        case IDBREX_HDR:
            return "IDBREX_HDR";
        case IDBREX_BPINFO:
            return "IDBREX_BPINFO";
        case IDBREX_BPINFO2:
            return "IDBREX_BPINFO2";
        case IDBREX_BPHITCOUNT:
            return "IDBREX_BPHITCOUNT";
        case IDBREX_BPDUP:
            return "IDBREX_BPDUP";
        case IDBREX_BPSET:
            return "IDBREX_BPSET";
        case IDBREX_BPSET2:
            return "IDBREX_BPSET2";
        case IDBREX_STEP:
            return "IDBREX_STEP";
        case IDBREX_STEP2:
            return "IDBREX_STEP2";
        case IDBREX_NOFUNC:
            return "IDBREX_NOFUNC";
        case IDBREX_NOLINE:
            return "IDBREX_NOLINE";
        case IDBREX_OUTOFRANGE:
            return "IDBREX_OUTOFRANGE";
        case IDBREX_NOFILE:
            return "IDBREX_NOFILE";
        case IDBREX_NOFILEDIR:
            return "IDBREX_NOFILEDIR";
        case IDBREX_NOBP:
            return "IDBREX_NOBP";
        case IDBREX_BADBPARG:
            return "IDBREX_BADBPARG";
        case IDBREX_SIGFRAME:
            return "IDBREX_SIGFRAME";
        case IDBREX_FRAME:
            return "IDBREX_FRAME";
        case IDBREX_L0FRAME:
            return "IDBREX_L0FRAME";
        case IDBREX_SFRAME:
            return "IDBREX_SFRAME";
        case IDBREX_IFRAME:
            return "IDBREX_IFRAME";
        case IDBREX_FIRSTFRAME:
            return "IDBREX_FIRSTFRAME";
        case IDBREX_LASTFRAME:
            return "IDBREX_LASTFRAME";
        case IDBREX_MINFRAME:
            return "IDBREX_MINFRAME";
        case IDBREX_NOSTACK:
            return "IDBREX_NOSTACK";
        case IDBREX_NOSYM:
            return "IDBREX_NOSYM";
        case IDBREX_NOMEM:
            return "IDBREX_NOMEM";
        case IDBREX_NOTRUN:
            return "IDBREX_NOTRUN";
        case IDBREX_NOSYMS:
            return "IDBREX_NOSYMS";
        case IDBREX_NOTEXEC:
            return "IDBREX_NOTEXEC";
        case IDBREX_LOC:
            return "IDBREX_LOC";
        case IDBREX_LOCNOCODE:
            return "IDBREX_LOCNOCODE";
        case IDBREX_VARS:
            return "IDBREX_VARS";
        case IDBREX_TYPE:
            return "IDBREX_TYPE";
        case IDBREX_DATA:
            return "IDBREX_DATA";
        case IDBREX_DATA2:
            return "IDBREX_DATA2";
            //	case IDBREX_INTRO1: return "IDBREX_INTRO1";
            //	case IDBREX_INTRO2: return "IDBREX_INTRO2";
            //	case IDBREX_INTRO3: return "IDBREX_INTRO3";
        case IDBREX_INTRO4:
            return "IDBREX_INTRO4";
        case IDBREX_INTRO5:
            return "IDBREX_INTRO5";
            //	case IDBREX_INTRO6: return "IDBREX_INTRO6";
            //	case IDBREX_INTRO7: return "IDBREX_INTRO7";
            //	case IDBREX_INTRO8: return "IDBREX_INTRO8";
        case IDBREX_VERS:
            return "IDBREX_VERS";
        case IDBREX_VERS2:
            return "IDBREX_VERS2";
        case IDBREX_VERS3:
            return "IDBREX_VERS3";
        case IDBREX_SYMBOLS:
            return "IDBREX_SYMBOLS";
        case IDBREX_LANG:
            return "IDBREX_LANG";
        case IDBREX_SOURCE:
            return "IDBREX_SOURCE";
        case IDBREX_UNDEF:
            return "IDBREX_UNDEF";
        case IDBREX_PID:
            return "IDBREX_PID";
        case IDBREX_WARN:
            return "IDBREX_WARN";
        case IDBREX_IGNORE:
            return "IDBREX_IGNORE";
            //	case IDBREX_ATTACH: return "IDBREX_ATTACH";
            //	case IDBREX_ATTACH_FAILED: return "IDBREX_ATTACH_FAILED";

        /* GDBMI Regex */
        case GDBMIREX_PROMPT:
            return "GDBMIREX_PROMPT";
        case GDBMIREX_BPHIT:
            return "GDBMIREX_BPHIT";
        case GDBMIREX_SIGNAL:
            return "GDBMIREX_SIGNAL";
        case GDBMIREX_EXIT:
            return "GDBMIREX_EXIT";
        case GDBMIREX_EXIT2:
            return "GDBMIREX_EXIT2";
        case GDBMIREX_LINE:
            return "GDBMIREX_LINE";
        case GDBMIREX_BPINFO:
            return "GDBMIREX_BPINFO";
        case GDBMIREX_BPINFO2:
            return "GDBMIREX_BPINFO2";
        case GDBMIREX_BPHITCOUNT:
            return "GDBMIREX_BPHITCOUNT";
        case GDBMIREX_BPDUP:
            return "GDBMIREX_BPDUP";
        case GDBMIREX_BPSET:
            return "GDBMIREX_BPSET";
        case GDBMIREX_STEP:
            return "GDBMIREX_STEP";
        case GDBMIREX_STEP2:
            return "GDBMIREX_STEP2";
        case GDBMIREX_NOFUNC:
            return "GDBMIREX_NOFUNC";
        case GDBMIREX_NOLINE:
            return "GDBMIREX_NOLINE";
        case GDBMIREX_OUTOFRANGE:
            return "GDBMIREX_OUTOFRANGE";
        case GDBMIREX_NOFILE:
            return "GDBMIREX_NOFILE";
        case GDBMIREX_NOFILEDIR:
            return "GDBMIREX_NOFILEDIR";
        case GDBMIREX_NOBP:
            return "GDBMIREX_NOBP";
        case GDBMIREX_BADBPARG:
            return "GDBMIREX_BADBPARG";
        case GDBMIREX_SIGFRAME:
            return "GDBMIREX_SIGFRAME";
        case GDBMIREX_FRAME:
            return "GDBMIREX_FRAME";
        case GDBMIREX_L0FRAME:
            return "GDBMIREX_L0FRAME";
        case GDBMIREX_SFRAME:
            return "GDBMIREX_SFRAME";
        case GDBMIREX_IFRAME:
            return "GDBMIREX_IFRAME";
        case GDBMIREX_FIRSTFRAME:
            return "GDBMIREX_FIRSTFRAME";
        case GDBMIREX_LASTFRAME:
            return "GDBMIREX_LASTFRAME";
        case GDBMIREX_MINFRAME:
            return "GDBMIREX_MINFRAME";
        case GDBMIREX_NOSTACK:
            return "GDBMIREX_NOSTACK";
        case GDBMIREX_NOSYM:
            return "GDBMIREX_NOSYM";
        case GDBMIREX_NOMEM:
            return "GDBMIREX_NOMEM";
        case GDBMIREX_NOTRUN:
            return "GDBMIREX_NOTRUN";
        case GDBMIREX_NOSYMS:
            return "GDBMIREX_NOSYMS";
        case GDBMIREX_NOTEXEC:
            return "GDBMIREX_NOTEXEC";
        case GDBMIREX_LOC:
            return "GDBMIREX_LOC";
        case GDBMIREX_LOCNOCODE:
            return "GDBMIREX_LOCNOCODE";
        case GDBMIREX_VARS:
            return "GDBMIREX_VARS";
        case GDBMIREX_TYPE:
            return "GDBMIREX_TYPE";
        case GDBMIREX_DATA:
            return "GDBMIREX_DATA";
        case GDBMIREX_DATA2:
            return "GDBMIREX_DATA2";
        case GDBMIREX_INTRO:
            return "GDBMIREX_INTRO";
        case GDBMIREX_VERS:
            return "GDBMIREX_VERS";
        case GDBMIREX_SYMBOLS:
            return "GDBMIREX_SYMBOLS";
        case GDBMIREX_LANG:
            return "GDBMIREX_LANG";
        case GDBMIREX_SOURCE:
            return "GDBMIREX_SOURCE";
        case GDBMIREX_UNDEF:
            return "GDBMIREX_UNDEF";
        case GDBMIREX_PID:
            return "GDBMIREX_PID";
        case GDBMIREX_WARN:
            return "GDBMIREX_WARN";
        case GDBMIREX_IGNORE:
            return "GDBMIREX_IGNORE";
        case GDBMIREX_ATTACH:
            return "GDBMIREX_ATTACH";
        case GDBMIREX_ATTACH_FAILED:
            return "GDBMIREX_ATTACH_FAILED";
        case GDBMIREX_DONE:
            return "GDBMIREX_DONE";
        case GDBMIREX_RUNNING:
            return "GDBMIREX_RUNNING";

        case REX_NULL:
            return "REX_NULL";
        default:
            return "UNKNOWN REGEX";
    }
}
#endif /* DEBUG */
