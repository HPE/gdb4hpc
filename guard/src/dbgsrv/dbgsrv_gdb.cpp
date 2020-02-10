/*********************************************************************************\
 * GDB interface routines
 *
 * NOTE: THIS INTERFACE IS NOW DEPRECIATED.
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

#ifdef GDB
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <math.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "dbgevent.h"
#include "dbgfn.h"
#include "dbgrexp.h"
#include "dbgsrv_p2p.h"
#include "dbgsrv_proto.h"
#include "dbgsrv_util.h"
#include "defs.h"
#include "expr.h"
#include "mparch.h"
#include "useful.h"

#define DBGPROG "dbgsrv"

#define READ_TIMEOUT 1
#define MAX_TRIES 10

#define SYNC()                                            \
    {                                                     \
        dbgevent_t *e;                                    \
                                                          \
        if ((e = Sync()) != (dbgevent_t *)NULL) return e; \
    }

#define GETPID(pid, e)                                     \
    {                                                      \
        dbgevent_t *et;                                    \
                                                           \
        if ((et = GetPid(&(pid))) != (dbgevent_t *)NULL) { \
            FreeEvent((e));                                \
            return et;                                     \
        }                                                  \
    }

static char *CurrentDataType = NULL;
static dbgstat Status;
static int Pid = 0;
static int DBGIn;
static int DBGOut;
static int MPArch;
static int Synced = 0;
static char *GDBInit = NULL;
static char *RunArgs = NULL;
static char *RedirStr = NULL;
static int CurrLine = 0;
static dbgbp_t *CurrBP = (dbgbp_t *)NULL;
static list_t *BP;
static char *ProgMain;
#ifdef sun
static char *IODevName = NULL;
#endif /* sun */

extern int Shutdown;
extern int my_rank;
extern int sock_fd;
extern appInfo_t other_app;

AIF *GDBBuildAIFVar(char *, char *);
dbgevent_t *CheckBreak(void);
dbgevent_t *ShowBreak(int);
dbgevent_t *WaitEvent(dbgevent_t *);
int SetCurrentLine(int);
void Finish(int);
dbgevent_t *InitDbg(char *, char *);
dbgevent_t *InitDbgAttach(int, char *);
#ifdef P4
dbgevent_t *InitP4Dbg(char *, int, char *, char *, char *, char **);
dbgevent_t *InitP4DbgAttach(int, char *, char *, char **);
#endif /* P4 */
dbgevent_t *GetPid(int *);
char *SetupDbg(char *);
void SendCmd(char *, ...);
#ifdef notdef
char *FindLine(char *);
#endif /* notdef */
char *ReadLine(void);
char *FreeBuffer(void);
int FillBuffer(int);
int BufferReady(void);
#ifdef notdef
char *SkipLine(char *, int);
int CharsToRead(int);
#endif /* notdef */
char *ReadData(int);
int WriteBuf(char *, int);
dbgevent_t *Sync(void);
#ifdef notdef
int ParseFmt(char *, char **, int *);
char *ParseData(int);
#endif /* notdef */
char *ConvBase(char *);
int BaseSize(char *);
int FromHex(char);
void InitRegexGDB(void);

dbgevent_t *InitGDB(int *, int *, char **);
#ifdef sun
int ProgIOHookGDB(int fd);
#endif /* sun */
dbgevent_t *DbgGDBSetLineBreak(char *, int);
dbgevent_t *DbgGDBSetFuncBreak(char *, char *);
dbgevent_t *DbgGDBDeleteBreak(int);
dbgevent_t *DbgGDBLastBreak(void);
dbgevent_t *DbgGDBList(char *);
dbgevent_t *DbgGDBGo(char *, int);
dbgevent_t *DbgGDBSetArgs(char *);
dbgevent_t *DbgGDBStep(int, int, char *, int);
dbgevent_t *DbgGDBKill(void);
dbgevent_t *DbgGDBMoveFrame(int, int);
dbgevent_t *DbgGDBShowFrame(void);
dbgevent_t *DbgGDBEvalExpr(char *);
dbgevent_t *DbgGDBSetVar(char *, AIF *);
dbgevent_t *DbgGDBGetType(char *);
dbgevent_t *DbgGDBGetLocation(void);
dbgevent_t *DbgGDBGetVars(char *);
dbgevent_t *DbgGDBStatus(void);
dbgevent_t *DbgGDBQuit(void);
dbgevent_t *DbgGDBInterrupt(char *, int);
dbgevent_t *DbgGDBInvoke(char *, int, char *, char *, char *, char **, int);
dbgevent_t *DbgGDBAttach(int, char *, char *, char **, int);
void DbgGDBFinish(void);
dbgevent_t *DbgGDBGetRank(int *);
dbgevent_t *DbgGDBSetDebuggerVar(char *, AIF *);
dbgevent_t *DbgGDBSetSourcePath(AIF *);
dbgevent_t *DbgGDBGetSourcePath(void);
dbgevent_t *DbgGDBGetDebuggerVar(char *);

dbgbackend_sw DbgBackendGDB = {InitGDB,
                               FillBuffer,
                               BufferReady,
#ifdef sun
                               ProgIOHookGDB,
#else  /* sun */
                               NULL,
#endif /* sun */
                               DbgGDBSetLineBreak,
                               DbgGDBSetFuncBreak,
                               DbgGDBDeleteBreak,
                               DbgGDBLastBreak,
                               DbgGDBList,
                               DbgGDBGo,
                               DbgGDBSetArgs,
                               DbgGDBStep,
                               DbgGDBKill,
                               DbgGDBMoveFrame,
                               DbgGDBShowFrame,
                               DbgGDBSetVar,
                               DbgGDBEvalExpr,
                               DbgGDBGetType,
                               DbgGDBGetLocation,
                               DbgGDBGetVars,
                               DbgGDBStatus,
                               DbgGDBQuit,
                               DbgGDBInterrupt,
                               DbgGDBInvoke,
                               DbgGDBAttach,
                               DbgGDBFinish,
                               DbgReadyForCmd,
                               DbgOK,
                               DbgStopped,
                               DbgGDBGetRank,
                               DbgGDBSetDebuggerVar,
                               DbgGDBSetSourcePath,
                               DbgGDBGetSourcePath,
                               DbgGDBGetDebuggerVar};

/*
** Invoked if there is input from the debugger after
** a non-terminating command (eg. run, step, etc.).
** Checks to see what caused the termination.
*/
dbgevent_t *WaitEvent(dbgevent_t *e) {
    int cnt;
    char *line;
    char *type;
    char *data;
    dbgbp_t *bp = NULL;
    dbglist_t *l;
    dbglist_t **lp;
    dbgvars_t *v = (dbgvars_t *)NULL;
    dbgvars_t *v_ptr = (dbgvars_t *)NULL;
    int newvers = 0;
    dbgloc_t *lc;
    dbgrexp_t *d;
    dbgframe_t *f;
    dbgframe_t **fp;

    if (e == (dbgevent_t *)NULL) e = NewEvent(DBGEV_NONE);

    for (;;) {
        if ((line = ReadLine()) == NULL) {
            DPRINT("ReadLine is NULL\n");
            e = ChangeEvent(e, DBGEV_ERROR);
            e->ev_errno = DBGERR_DEBUGGER;
            EventErrStr(e, "reading command");
            return e;
        }

        if (*line == '\0') {
            DPRINT("line is NULL\n");
            continue;
        }

        if ((d = MatchRegexList(line)) == RegexError) {
            DPRINT("regexerror\n");
            e = ChangeEvent(e, DBGEV_ERROR);
            e->ev_errno = DBGERR_REGEX;
            EventErrStr(e, d->d_errbuf);
            return e;
        }

        if (d == (dbgrexp_t *)NULL) {
            DPRINT("d is null\n");
            e = ChangeEvent(e, DBGEV_ERROR);
            e->ev_errno = DBGERR_UNKNOWN;
            EventErrStr(e, line);
            return e;
        }

        DPRINT("*** Matched %s\n", GetRegexEvent(d->d_event));

        switch (d->d_event) {
            case GDBREX_BPHIT:
                if (Status == DBGSTAT_INITIALISING) {
                    bp = NewBP(MatchToInt(d, 1, 10));

                    e->ev_event = DBGEV_BPHIT;
                    e->ev_bp = bp;
                    break;
                }

                if ((bp = FindBPByID(BP, MatchToInt(d, 1, 10))) ==
                    (dbgbp_t *)NULL) {
                    e = ChangeEvent(e, DBGEV_ERROR);
                    e->ev_errno = DBGERR_DEBUGGER;
                    EventErrStr(e, "bad breakpoint");
                    return e;
                }

                assert(bp != NULL);
                bp->bp_hits++;
                CurrBP = bp;

                e->ev_event = DBGEV_BPHIT;
                e->ev_bp = DupBP(bp);

                Status = DBGSTAT_STOPPED;

                /*
                ** Wait for line info.
                */
                break;

            case GDBREX_SIGNAL:
                /* assert(e->ev_event == DBGEV_NONE) */
                e->ev_event = DBGEV_SIGNAL;
                e->ev_sig_type = MatchToStr(d, 1);
                e->ev_sig_line = NULL;
                e->ev_sig_frame = (dbgframe_t *)NULL;

                Status = DBGSTAT_STOPPED;

                /*
                ** Get frame and line info if available.
                */
                break;

            case GDBREX_EXIT:
                /* assert(e->ev_event == DBGEV_NONE) */
                e->ev_event = DBGEV_EXIT;
                e->ev_exit = MatchToInt(d, 1, 8);
                Status = DBGSTAT_WAITING;
                Pid = 0;
                break;

            case GDBREX_EXIT2:
                /* assert(e->ev_event == DBGEV_NONE) */
                e->ev_event = DBGEV_EXIT;
                e->ev_exit = 0;
                Status = DBGSTAT_WAITING;
                Pid = 0;
                break;

            case GDBREX_SHOW:
                e->ev_event = DBGEV_DATA;
                e->ev_data = StringToAIF(MatchToStr(d, 1));
                break;

            case GDBREX_LINE:
                switch (e->ev_event) {
                    case DBGEV_BPHIT:
                        if (e->ev_bp->bp_stmt != NULL) Free(e->ev_bp->bp_stmt);

                        e->ev_bp->bp_stmt = MatchToStr(d, 2);

                        SetCurrentLine(MatchToInt(d, 1, 10));
                        break;

                    case DBGEV_SIGNAL:
                        e->ev_sig_line = MatchToStr(d, 2);
                        SetCurrentLine(MatchToInt(d, 1, 10));
                        break;

                    case DBGEV_FRAME:
                        break;

                    case DBGEV_LIST:
                        l = (dbglist_t *)Alloc(sizeof(dbglist_t));
                        l->list_lno = MatchToInt(d, 1, 10);
                        l->list_line = MatchToStr(d, 2);
                        l->list_next = (dbglist_t *)NULL;

                        /*
                        ** Find end of list.
                        */
                        for (lp = &e->ev_list; *lp != NULL;
                             lp = &(*lp)->list_next)
                            ;
                        *lp = l;
                        break;

                    case DBGEV_NONE: /* step command */
                    case DBGEV_STEP:
                        if (e->ev_event == DBGEV_NONE) {
                            e->ev_event = DBGEV_STEP;
                            e->ev_step_frame = (dbgframe_t *)NULL;
                        }

                        e->ev_step_lno = MatchToInt(d, 1, 10);
                        e->ev_step_line = MatchToStr(d, 2);
                        Status = DBGSTAT_STOPPED;
                        break;

                    default:
                        /* do nothing */
                        break;
                }
                break;

            case GDBREX_HDR:
                /* assert(e->ev_event == DBGEV_NONE) */
                e->ev_event = DBGEV_BPINFO;

                /*
                ** Get breakpoint info
                */
                break;

            case GDBREX_BPDUP:
                /* assert(e->ev_event == DBGEV_NONE) */
                e->ev_event = DBGEV_BPINFO;

                /*
                ** Get breakpoint set line.
                */
                break;

            case GDBREX_BPSET:
            case GDBREX_BPSET2:
            case GDBREX_BPINFO:
            case GDBREX_BPINFO2:
                bp = NewBP(MatchToInt(d, 1, 10));

                if (d->d_event == GDBREX_BPINFO ||
                    d->d_event == GDBREX_BPINFO2) {
                    e->ev_event = DBGEV_BPINFO;
                    bp->bp_type = MatchToStr(d, 2);
                    bp->bp_loc.loc_addr = MatchToStr(d, 5);
                    bp->bp_loc.loc_func = MatchToStr(d, 6);

                    if (d->d_event == GDBREX_BPINFO) {
                        bp->bp_loc.loc_file = MatchToStr(d, 7);
                        bp->bp_loc.loc_line = MatchToInt(d, 8, 10);
                    } else
                        bp->bp_loc.loc_line = -1;

                    e->ev_bp = bp;
                } else {
                    e->ev_event = DBGEV_BPSET;
                    bp->bp_loc.loc_addr = MatchToStr(d, 2);

                    if (d->d_event == GDBREX_BPSET) {
                        bp->bp_loc.loc_file = MatchToStr(d, 3);
                        bp->bp_loc.loc_line = MatchToInt(d, 4, 10);
                    } else /* GDBREX_BPSET2 */
                        bp->bp_loc.loc_line = -1;

                    e->ev_bp = bp;
                }

                break;

            case GDBREX_STEP:
                e->ev_event = DBGEV_STEP;
                e->ev_step_frame = (dbgframe_t *)NULL;
                e->ev_step_lno = -1;
                e->ev_step_line = MatchToStr(d, 1);
                break;

            case GDBREX_STEP2:
                Status = DBGSTAT_STOPPED;
                break;

            case GDBREX_BPHITCOUNT:
                /* assert(e->ev_event == DBGEV_BPINFO */

                bp->bp_hits = MatchToInt(d, 1, 10);
                break;

            case GDBREX_SIGFRAME:
            case GDBREX_FRAME:
            case GDBREX_L0FRAME:
            case GDBREX_SFRAME:
            case GDBREX_IFRAME:
            case GDBREX_MINFRAME: /* received on attach */
                /*
                ** XXX
                ** Need to check for MINFRAME on attach here
                ** and ignore.
                */

                f = (dbgframe_t *)Alloc(sizeof(dbgframe_t));
                f->frame_next = (dbgframe_t *)NULL;

                switch (d->d_event) {
                    case GDBREX_SIGFRAME:
                        f->frame_level = -1;
                        f->frame_loc.loc_addr = MatchToStr(d, 1);
                        f->frame_loc.loc_func = MatchToStr(d, 2);
                        f->frame_loc.loc_file = MatchToStr(d, 3);
                        f->frame_loc.loc_line = MatchToInt(d, 4, 10);
                        break;

                    case GDBREX_FRAME:
                        f->frame_level = MatchToInt(d, 1, 10);
                        f->frame_loc.loc_addr = MatchToStr(d, 2);
                        f->frame_loc.loc_func = MatchToStr(d, 3);
                        f->frame_loc.loc_file = MatchToStr(d, 4);
                        f->frame_loc.loc_line = MatchToInt(d, 5, 10);
                        break;

                    case GDBREX_L0FRAME:
                        f->frame_level = 0;
                        f->frame_loc.loc_addr = NULL;
                        f->frame_loc.loc_func = MatchToStr(d, 1);
                        f->frame_loc.loc_file = MatchToStr(d, 2);
                        f->frame_loc.loc_line = MatchToInt(d, 3, 10);
                        break;

                    case GDBREX_SFRAME:
                        f->frame_level = -1;
                        f->frame_loc.loc_addr = NULL;
                        f->frame_loc.loc_func = MatchToStr(d, 1);
                        f->frame_loc.loc_file = MatchToStr(d, 2);
                        f->frame_loc.loc_line = MatchToInt(d, 3, 10);
                        break;

                    case GDBREX_IFRAME:
                        f->frame_level = MatchToInt(d, 1, 10);
                        f->frame_loc.loc_addr = MatchToStr(d, 2);
                        f->frame_loc.loc_func = MatchToStr(d, 3);
                        f->frame_loc.loc_line = -1;
                        f->frame_loc.loc_file = NULL;
                        break;

                    case GDBREX_MINFRAME:
                        f->frame_level = -1;
                        f->frame_loc.loc_addr = MatchToStr(d, 1);
                        f->frame_loc.loc_func = MatchToStr(d, 2);
                        f->frame_loc.loc_line = -1;
                        f->frame_loc.loc_file = NULL;
                        break;

                    default:
                        /* do nothing */;
                        break;
                }

                /*
                ** If we are generating frame as a result of
                ** a signal, a step, or the up/down commands,
                ** then expect a line next.
                */

                if (d->d_event == GDBREX_SFRAME) {
                    e->ev_event = DBGEV_STEP;
                    e->ev_step_frame = f;
                    break;
                }

                switch (e->ev_event) {
                    case DBGEV_NONE:
                        e->ev_event = DBGEV_FRAME;
                        e->ev_frame = f;
                        break;

                    case DBGEV_SIGNAL:
                        e->ev_sig_frame = f;
                        break;

                    default:
                        /*
                        ** If we are called as a result of the where
                        ** command, then we will have a list of frames.
                        */

                        for (fp = &e->ev_frame; *fp != NULL;
                             fp = &(*fp)->frame_next)
                            ;
                        *fp = f;
                        break;
                }

                break;

            case GDBREX_LOC:
            case GDBREX_LOCNOCODE:
                /* assert(e->ev_event == DBGEV_NONE) */
                lc = (dbgloc_t *)Alloc(sizeof(dbgloc_t));
                lc->loc_line = MatchToInt(d, 1, 10);
                lc->loc_file = MatchToStr(d, 2);
                lc->loc_addr = MatchToStr(d, 3);
                lc->loc_func = NULL;

                e->ev_event = DBGEV_LOC;
                e->ev_loc = lc;
                break;

            case GDBREX_VARS:
                // Set begining of linked list first time only.
                if (v == (dbgvars_t *)NULL) {
                    // Alloc mem.
                    v = (dbgvars_t *)Alloc(sizeof(dbgvars_t));
                    // set the initial pointers
                    e->ev_vars = v;
                    v_ptr = v;
                    // Create the dbgvar.
                    v->var_name = MatchToStr(d, 1);
                    v->var_next = (dbgvars_t *)NULL;
                    DPRINT("START:>>>%s\n", v->var_name);
                    break;
                    // Next time through we will hit here.
                } else {
                    // Alloc mem.
                    v = (dbgvars_t *)Alloc(sizeof(dbgvars_t));
                    // Create the dbgvar.
                    v->var_name = MatchToStr(d, 1);
                    v->var_next = (dbgvars_t *)NULL;
                    DPRINT(">>>%s\n", v->var_name);
                    // Set the dbgvar in the linked list.
                    v_ptr->var_next = v;
                    v_ptr = v;
                    break;
                }
                // Won't reach here.
                break;

            case GDBREX_TYPE:
                /* assert(e->ev_event == DBGEV_NONE) */
                e->ev_event = DBGEV_TYPE;
                e->ev_type = MatchToStr(d, 1);
                break;

            case GDBREX_DATA:
            case GDBREX_DATA2:
                /* assert(e->ev_event == DBGEV_NONE) */
                e->ev_event = DBGEV_DATA;

                if (d->d_event == GDBREX_DATA2) {
                    AIF *aif_var;

                    if (CurrentDataType == NULL) {
                        e = ChangeEvent(e, DBGEV_ERROR);
                        e->ev_errno = DBGERR_DEBUGGER;
                        EventErrStr(e, "bad data format");
                        return e;
                    }

                    type = CurrentDataType;
                    data = MatchToStr(d, 1);

                    aif_var = GDBBuildAIFVar(type, data);

                    if (aif_var == NULL) {
                        e = ChangeEvent(e, DBGEV_ERROR);
                        e->ev_errno = DBGERR_DEBUGGER;
                        EventErrStr(e, "bad data format");
                        return e;
                    }

                    e->ev_data = aif_var;
                    break;
                }

                type = MatchToStr(d, 1);

                /*
                ** Next comes data
                **
                ** We don't match data in the regex because it is too
                ** expensive for large data structures.
                */

                data = line;

                for (cnt = 0; cnt < 3; cnt++) {
                    if ((data = strchr(data, ' ')) == NULL) break;

                    data++;
                }

                if (data == NULL) {
                    e = ChangeEvent(e, DBGEV_ERROR);
                    e->ev_errno = DBGERR_DEBUGGER;
                    EventErrStr(e, "bad data format");
                    return e;
                }

                e->ev_data = AsciiToAIF(type, data);
                break;

            case GDBREX_INTRO4:
            case GDBREX_INTRO5:
                if (newvers && d->d_event == GDBREX_INTRO4) break;

                if (d->d_event == GDBREX_INTRO5) e->ev_arch = MatchToStr(d, 1);

                Synced = 0;
                return e;

            case GDBREX_VERS:
            case GDBREX_VERS2:
            case GDBREX_VERS3:
                e->ev_event = DBGEV_VERS;
                e->ev_version = MatchToStr(d, 1);

                if (d->d_event == GDBREX_VERS3) {
                    newvers = 1;
                    break;
                }

                e->ev_arch = MatchToStr(d, 2);

                if (d->d_event == GDBREX_VERS2) {
                    Synced = 0;
                    return e;
                }
                break;

            case GDBREX_SYMBOLS:
                /*
                ** Expect prompt next.
                */
                break;

            case GDBREX_WARN:
            case GDBREX_LANG:
            case GDBREX_IGNORE:
                /*
                ** Ignore for now.
                */
                break;

            case GDBREX_PROMPT:
                if (e->ev_event == DBGEV_NONE) e->ev_event = DBGEV_OK;

                Synced = 1;

                if (Status == DBGSTAT_RUNNING) Status = DBGSTAT_STOPPED;

                return e;

            case GDBREX_PID:
                // This was broken if we were already
                // running. If status is 2 or DBGSTAT_RUNNING
                // then we should just ignore this.
                if (Status == DBGSTAT_RUNNING) break;
                e->ev_event = DBGEV_PID;
                e->ev_pid = atoi(MatchToStr(d, 1));
                Synced = 0;
                return e;

                /*******************************
                ******* Errors below here ******
                ********************************/

            case GDBREX_ATTACH_FAILED:
                e->ev_event = DBGEV_ERROR;
                e->ev_errno = DBGERR_ATTACH_FAILED;
                // return e; // ???
                break;

            case GDBREX_NOFUNC:
                e->ev_event = DBGEV_ERROR;
                e->ev_errno = DBGERR_NOFUNC;
                EventErrStr(e, MatchToStr(d, 1));
                break;

            case GDBREX_NOLINE:
                e->ev_event = DBGEV_ERROR;
                e->ev_errno = DBGERR_NOLINE;
                EventErrStr(e, MatchToStr(d, 1));
                break;

            case GDBREX_OUTOFRANGE:
                e->ev_event = DBGEV_ERROR;
                e->ev_errno = DBGERR_OUTOFRANGE;
                EventErrStr(e, MatchToStr(d, 1));
                break;

            case GDBREX_NOFILE:
                e->ev_event = DBGEV_ERROR;
                e->ev_errno = DBGERR_NOFILE;
                EventErrStr(e, MatchToStr(d, 1));
                break;

            case GDBREX_NOBP:
                e->ev_event = DBGEV_ERROR;
                e->ev_errno = DBGERR_NOBP;
                EventErrStr(e, MatchToStr(d, 2));
                break;

            case GDBREX_BADBPARG:
                e->ev_event = DBGEV_ERROR;
                e->ev_errno = DBGERR_BADBPARG;
                EventErrStr(e, "");
                break;

            case GDBREX_NOSTACK:
                e->ev_event = DBGEV_ERROR;
                e->ev_errno = DBGERR_NOSTACK;
                EventErrStr(e, "");
                break;

            case GDBREX_NOSYM:
                e->ev_event = DBGEV_ERROR;
                e->ev_errno = DBGERR_NOSYM;
                EventErrStr(e, MatchToStr(d, 1));
                break;

            case GDBREX_NOTRUN:
                e->ev_event = DBGEV_ERROR;
                e->ev_errno = DBGERR_NOTRUN;
                EventErrStr(e, "");
                break;

            case GDBREX_NOMEM:
                e->ev_event = DBGEV_ERROR;
                e->ev_errno = DBGERR_NOMEM;
                EventErrStr(e, "");
                break;

            case GDBREX_FIRSTFRAME:
                e->ev_event = DBGEV_ERROR;
                e->ev_errno = DBGERR_FIRSTFRAME;
                EventErrStr(e, "");
                break;

            case GDBREX_LASTFRAME:
                e->ev_event = DBGEV_ERROR;
                e->ev_errno = DBGERR_LASTFRAME;
                EventErrStr(e, "");
                break;

            case GDBREX_NOSYMS:
                e->ev_event = DBGEV_ERROR;
                e->ev_errno = DBGERR_NOSYMS;
                EventErrStr(e, "");
                /*
                ** NOSYMS seems to consume a prompt
                */
                return e;

            case GDBREX_NOFILEDIR:
                switch (e->ev_event) {
                    case DBGEV_STEP:
                        /*
                        ** On the Cray platform, we may need to ignore
                        ** the DBGEV_STEP as well (that is caused by signals).
                        */
                        break;
                    case DBGEV_SIGNAL:
                        /*
                        ** A signal in a library.
                        */
                        e->ev_sig_line = NULL;
                        break;

                    default:
                        e->ev_event = DBGEV_ERROR;
                        e->ev_errno = DBGERR_NOFILEDIR;
                        EventErrStr(e, MatchToStr(d, 1));
                        break;
                }
                break;

            case GDBREX_NOTEXEC:
                e->ev_event = DBGEV_ERROR;
                e->ev_errno = DBGERR_NOTEXEC;
                EventErrStr(e, MatchToStr(d, 1));
                break;

            case GDBREX_SOURCE:
                e->ev_event = DBGEV_ERROR;
                e->ev_errno = DBGERR_SOURCE;
                EventErrStr(e, "");
                break;

            case GDBREX_UNDEF:
                e->ev_event = DBGEV_ERROR;
                e->ev_errno = DBGERR_NOTIMP;
                EventErrStr(e, "");
                break;

            default:
                fprintf(stderr, "WaitEvent: unknown event %d\n", d->d_event);
                return e;
        }
    }

    /* NOT REACHED */
}

/*
** Check that debugger is ready ok
*/
int DbgOK(void) { return (int)(Status != DBGSTAT_DBGERR); }

/*
** Check if debugger is ready to receive a command.
*/
int DbgReadyForCmd(void) {
    return (int)(Status == DBGSTAT_WAITING || Status == DBGSTAT_STOPPED);
}

/*
** Check if debugger is ready to receive a command.
*/
int DbgStopped(void) { return (int)(Status == DBGSTAT_STOPPED); }

/*
** Set breakpoint at specified line.
*/
dbgevent_t *DbgGDBSetLineBreak(char *file, int lno) {
    if (file == NULL || *file == '\0') {
        DPRINT("*** DbgSetLineBreak(%d)\n", lno);
        SendCmd("break %d", lno);
    } else {
        DPRINT("*** DbgSetLineBreak(%s, %d)\n", file, lno);
        SendCmd("break %s:%d", file, lno);
    }

    return CheckBreak();
}

/*
** Set breakpoint at start of specified function.
*/
dbgevent_t *DbgGDBSetFuncBreak(char *file, char *func) {
    DPRINT("*** DbgSetFuncBreak\n");

    if (file == NULL || *file == '\0')
        SendCmd("break %s", func);
    else
        SendCmd("break %s:%s", file, func);

    return CheckBreak();
}

/*
** Check that breakpoint command has succeded and
** extract appropriate information. Returns breakpoint
** id in bid. Adds to breakpoint list if necessary.
*/
dbgevent_t *CheckBreak(void) {
    int id;
    dbgbp_t *bp;
    dbgevent_t *e;

    e = WaitEvent((dbgevent_t *)NULL);

    if (e->ev_event != DBGEV_BPSET) return e;

    id = e->ev_bp->bp_id;

    FreeEvent(e);

    /*
    ** show gives us more information.
    */

    e = ShowBreak(id);

    if (e->ev_event != DBGEV_BPINFO) return e;

    bp = DupBP(e->ev_bp);

    FreeEvent(e);

    /*
    ** Link a copy of the breakpoint onto BP.
    */
    AddToList(BP, (void *)bp);

    /*
    ** Now create a fake event and make it
    ** look like a BPSET.
    */

    e = NewEvent(DBGEV_BPSET);
    e->ev_bp = DupBP(bp);

    return e;
}

/*
** Extract breakpoint information for breakpoint id.
*/
dbgevent_t *ShowBreak(int id) {
    SendCmd("info break %d", id);

    return WaitEvent((dbgevent_t *)NULL);
}

/*
** Set current line number. Must be called after a breakpoint
** has been hit, or a step command executed.
*/
int SetCurrentLine(int line) {
    CurrLine = line;
    return line;
}

/*
** Delete a breakpoint.
*/
dbgevent_t *DbgGDBDeleteBreak(int bid) {
    /*	char *		line; */
    dbgevent_t *e;

    DPRINT("*** DbgDeleteBreak\n");

    SendCmd("delete breakpoint %d", bid);

    e = WaitEvent((dbgevent_t *)NULL);

    if (e->ev_event == DBGEV_OK && Status != DBGSTAT_INITIALISING)
        RemoveBP(BP, bid);

    return e;
}

/*
** Return id of last breakpoint to be hit.
*/
dbgevent_t *DbgGDBLastBreak(void) {
    dbgevent_t *e;

    if (CurrBP == (dbgbp_t *)NULL) {
        EVENT_ERROR(e, DBGERR_NOBP, "");
        return e;
    }

    e = NewEvent(DBGEV_BPINFO);
    e->ev_bp = CurrBP;

    return e;
}

/*
** List source code.
**
** XXX a better way to do this might be to create a socket
** to the client and just ship the stuff across.
*/
dbgevent_t *DbgGDBList(char *str) {
    dbgevent_t *e;

    if (str == NULL || *str == '\0')
        SendCmd("list");
    else
        SendCmd("list %s", str);

    e = NewEvent(DBGEV_LIST);
    e->ev_list = (dbglist_t *)NULL;

    return WaitEvent(e);
}

/*
** Start/continue executing program. If we are starting,
** then we must plant a breakpoint at main() so that we
** can find out the process id of the program.
*/
dbgevent_t *DbgGDBGo(char *host, int prog) {
    /*	int		id; */
    dbgevent_t *e;
    dbgevent_t *eb = (dbgevent_t *)NULL;

    DPRINT("*** DbgGo()\n");

    /*
    ** Check if we are starting...
    */
    if (Status != DBGSTAT_STOPPED
#ifdef notdef
#ifdef AIX_POE
        && MPArch != MPARCH_POE
#endif /* AIX_POE */
#endif /* notdef */
    ) {
        /*
        ** Set a temporary breakpoint at main() if necessary.
        */
        if (FindBPByFunc(BP, ProgMain) == (dbgbp_t *)NULL) {
            eb = DbgSetFuncBreak(NULL, ProgMain);

            if (eb->ev_event != DBGEV_BPSET) return eb;
        }

        /*
        ** Run the program.
        */

        if (RunArgs == NULL) RunArgs = strdup("");

        SendCmd("run %s %s", RunArgs, RedirStr != NULL ? RedirStr : "");

        /*
        ** Wait until we hit the breakpoint at main().
        */

        e = WaitEvent((dbgevent_t *)NULL);

        if (e->ev_event != DBGEV_BPHIT) {
            FreeEvent(eb);
            return e;
        }

        /*
        ** Find out pid.
        */

        GETPID(Pid, e);

        /*
        ** Finish if there was already a break at main.
        */
        if (eb == (dbgevent_t *)NULL) {
            Status = DBGSTAT_STOPPED;

            /*
            ** Force event.
            */
            AsyncCheck(ASYNC_FORCE, e, host, prog);

            return (dbgevent_t *)NULL;
        }

        FreeEvent(e);

        /*
        ** Delete the temporary breakpoint and continue.
        */

        e = DbgGDBDeleteBreak(eb->ev_bp->bp_id);

        if (e->ev_event != DBGEV_OK) return e;

        FreeEvent(e);
        FreeEvent(eb);
    }

#ifdef notdef
#ifdef AIX_POE
    if (MPArch == MPARCH_POE) {
        /*
        ** Find out pid.
        */

        Pid = GetPid();
    }
#endif /* AIX_POE */
#endif /* notdef */

    SendCmd("continue");

    Status = DBGSTAT_RUNNING;

    AsyncCheck(WaitEvent, (dbgevent_t *)NULL, host, prog);

    return (dbgevent_t *)NULL;
}

/*
** Set run arguments.
*/
dbgevent_t *DbgGDBSetArgs(char *arg) {
    DPRINT("*** DbgSetArgs\n");

    if (arg != NULL && *arg != '\0') {
        if (RunArgs != NULL) Free(RunArgs);

        RunArgs = strdup(arg);
    }

    return NewEvent(DBGEV_OK);
}

/*
** Execute count statements. If in == 0, do not enter
** function calls.
*/
dbgevent_t *DbgGDBStep(int count, int in, char *host, int prog) {
    char *cmd = "step";

    DPRINT("*** DbgStep()\n");

    if (!in) cmd = "next";

    if (count > 1)
        SendCmd("%s %d", cmd, count);
    else
        SendCmd(cmd);

    AsyncCheck(WaitEvent, (dbgevent_t *)NULL, host, prog);

    Status = DBGSTAT_RUNNING;

    return (dbgevent_t *)NULL;
}

/*
** Kill the currently running process.
*/
dbgevent_t *DbgGDBKill(void) {
    dbgevent_t *e;

    DPRINT("*** DbgKill\n");

    SendCmd("kill");

    e = WaitEvent((dbgevent_t *)NULL);

    if (e->ev_event == DBGEV_OK) {
        Status = DBGSTAT_WAITING;
        Pid = 0;
    }

    return e;
}

/*
** Move up or down count stack frames.
*/
dbgevent_t *DbgGDBMoveFrame(int count, int up) {
    char *cmd = "up";

    DPRINT("*** DbgMoveFrame\n");

    if (!up) cmd = "down";

    if (count > 1)
        SendCmd("%s %d", cmd, count);
    else
        SendCmd(cmd);

    return WaitEvent((dbgevent_t *)NULL);
}

/*
** Show current stack frames.
*/
dbgevent_t *DbgGDBShowFrame(void) {
    dbgevent_t *e = NewEvent(DBGEV_FRAME);

    DPRINT("*** DbgShowFrame\n");

    SendCmd("where");

    e->ev_frame = (dbgframe_t *)NULL;

    return WaitEvent(e);
}

dbgevent_t *PerformExtEvalExpr(char *exp) {
    dbgevent_t *e;
    dbgevent_t *e1;
    AIF *tmp = NULL;

    char var[BUFSIZ] = "";
    char args[BUFSIZ] = "";
    int func_id = 0;

    e = NewEvent(DBGEV_DATA);
    DPRINT("*** ParseDBGExpr %s\n", exp);
    ParseDBGExpr(exp, &func_id, var, args);
    DPRINT("*** fnish ParseDBGExpr %s\n", var);

#ifdef GDB_WITH_AIF
    SendCmd("set print aif on");

    e = WaitEvent((dbgevent_t *)NULL);

    if (e->ev_event != DBGEV_OK) return e;

    FreeEvent(e);
#else  /* GDB_WITH_AIF */
    e = DbgGDBGetType(exp);

    if (e->ev_event != DBGEV_TYPE) return e;

    if (CurrentDataType != NULL) free(CurrentDataType);

    CurrentDataType = strdup(e->ev_type);

    FreeEvent(e);
#endif /* GDB_WITH_AIF */

    // Minh modify here to evaluate some expressions with more than 1 vars
    // just subtraction for now
    /*
    if (strstr(exp, "-")) {
            tok = strtok(exp, "-");
            SendCmd("print %s", tok);
            e1 = WaitEvent((dbgevent_t *)NULL);
            tmp1 = CopyAIF(e1->ev_data);
            AIFPrint(stdout, 0, tmp1);
            printf("\n");
            FreeEvent(e1);

            tok = strtok(NULL, "-");
            SendCmd("print %s", tok);
            e1 = WaitEvent((dbgevent_t *)NULL);

            AIFPrint(stdout, 0, e1->ev_data);
            printf("\n");
            // now perform subtraction
            tmp = AIFSub(tmp1, e1->ev_data);

            AIFFree(tmp1);
            AIFFree(e1->ev_data);
            e1->ev_data = tmp;
    } else {
    */
    SendCmd("print %s", var);
    e = WaitEvent((dbgevent_t *)NULL);
    //}

    // Minh change here to do the hashing of data 03/06/2009
    if (func_id > 0) {
        int hashfunc = 0, precision_lower, precision_upper;
        char hashinfo[BUFSIZ] = "";
        double lower_bound, upper_bound;

        switch (func_id) {
            case EXPR_HASH_FNV:
            case EXPR_HASH_AP:
            case EXPR_HASH_BOB:  // hash
                ParseHashArgs(args, &hashfunc, &precision_lower,
                              &precision_upper, hashinfo);
                // now using the hashinfo string to work out hash decomposition
                tmp = PerformHash(e->ev_data, hashinfo, hashfunc,
                                  precision_lower, precision_upper);
                break;
            case EXPR_SUM:  // sum
                tmp = AIFSum(e->ev_data);
                break;
            case EXPR_PRODUCT:  // product
                tmp = AIFProduct(e->ev_data);
                break;
            case EXPR_AVERAGE:  // average
                tmp = AIFAverage(e->ev_data);
                break;
            case EXPR_MIN:  // min
                tmp = AIFMin(e->ev_data);
                break;
            case EXPR_MAX:  // max
                tmp = AIFMax(e->ev_data);
                break;
            case EXPR_STDEV:  // standard deviation
                tmp = AIFStdev(e->ev_data);
                break;
            case EXPR_COUNTELEM:  // countelem
                ParseCountelemArgs(args, &lower_bound, &upper_bound);
                tmp = AIFCountElem(e->ev_data, lower_bound, upper_bound);
                break;
            /*
            case EXPR_HISTOGRAM:    // histogram
                ParseHistogramArgs(args, &numbins, &lower_bound, &upper_bound);
                tmp = AIFHistogram(e->ev_data, numbins, lower_bound,
            upper_bound); break;
            */
            case EXPR_SLICE:  // slice data
                tmp = PerformSlicing(e->ev_data, args);
                break;
            default:
                DPRINT("*** Unknow reduce operation\n");
                break;
        }
        DPRINT("PerformExtEvalExpr: all ok\n");
        AIFFree(e->ev_data);
        e->ev_data = tmp;
    }

    if (e->ev_event != DBGEV_DATA) return e;

#ifdef GDB_WITH_AIF
    SendCmd("set print aif off");

    e1 = WaitEvent((dbgevent_t *)NULL);

    if (e1->ev_event != DBGEV_OK) {
        FreeEvent(e);
        return e1;
    }

    FreeEvent(e1);
#endif /* GDB_WITH_AIF */

    return e;
}

/*
** Evaluate the expression exp.
*/
dbgevent_t *DbgGDBEvalExpr(char *exp) {
    char *type;
    AIF *a;
    dbgevent_t *e;

    // Check whether it has hashing/slicing
    if (strpbrk(exp, "`")) {
        // TODO: we don't want to parse this if the user escaped the input
        DPRINT("DbgGDBEvalExpr: Found reduction/slicing info.\n");
        e = PerformExtEvalExpr(exp);
        if (e == NULL) {
            e = NewEvent(DBGEV_ERROR);
            e->ev_errno = DBGERR_DEBUGGER;
            EventErrStr(e, "bad data format");
            return e;
        } else {
            return e;
        }
    }

#ifdef GDB_WITH_AIF
    SendCmd("set print aif on");

    e = WaitEvent((dbgevent_t *)NULL);

    if (e->ev_event != DBGEV_OK) return e;

    FreeEvent(e);
#else  /* GDB_WITH_AIF */
    e = DbgGDBGetType(exp);

    if (e->ev_event != DBGEV_TYPE) return e;

    if (CurrentDataType != NULL) free(CurrentDataType);

    CurrentDataType = strdup(e->ev_type);

    FreeEvent(e);
#endif /* GDB_WITH_AIF */

    SendCmd("print %s", exp);
    e = WaitEvent((dbgevent_t *)NULL);

    return e;
}

/*
** Set contents of GDB debug variable var.
*/
dbgevent_t *DbgGDBSetDebuggerVar(char *var, AIF *val) {
    int i;

    DPRINT("*** DbgSetDebuggerVar\n");

    AIFToInt(val, &i);

    SendCmd("set %s %d", var, i);

    return WaitEvent((dbgevent_t *)NULL);
}

/*
** Set contents of GDB source search path.
*/
dbgevent_t *DbgGDBSetSourcePath(AIF *val) {
    char *data;

    DPRINT("*** DbgSetSourcePath\n");

    AIFToStr(&data, 0, val);

    SendCmd("directory %s", data);

    Free(data);

    return WaitEvent((dbgevent_t *)NULL);
}

/*
** Get contents of GDB source search path.
*/
dbgevent_t *DbgGDBGetSourcePath() {
    DPRINT("*** DbgGetSourcePath\n");

    SendCmd("show directories");

    return WaitEvent((dbgevent_t *)NULL);
}

/*
** Get contents of GDB debug variable var.
*/
dbgevent_t *DbgGDBGetDebuggerVar(char *var) {
    /*If we want to return a response (event), we need to modify:
    - regexev in dbgrexp.h, eg:
            GDBREX_NEW_EXPR
    - InitRegex() in dbgrexp.c, eg:
            (void)AddRegex("^Number of source lines gdb will list by default is
    ([0-9]+).", 0, 1, GDBREX_NEW_EXPR);
    - WaitEvent() in dbgsrv_gdb.c, eg:
            case GDBREX_DONNY:
                    e->ev_event = DBGEV_DATA;
                    e->ev_data = IntToAIF(atoi(MatchToStr(d, 1)));
                    break;
    */

    DPRINT("*** DbgGetDebuggerVar\n");

    SendCmd("show %s", var);

    return WaitEvent((dbgevent_t *)NULL);
}

/*
** Set contents of variable var.
*/
dbgevent_t *DbgGDBSetVar(char *var, AIF *a) {
    int len;
    dbgevent_t *e;

    e = DbgGDBGetType(var);

    if (e->ev_event != DBGEV_TYPE) return e;

    len = FDSTypeSize(e->ev_type);

    if (len != AIF_LEN(a)) {
        EVENT_ERROR(e, DBGERR_SETVAR, "");
        return e;
    }

    SendCmd("dbg write %s", var);

    if (WriteBuf(AIF_DATA(a), len) < 0) {
        SYNC();

        EVENT_ERROR(e, DBGERR_DEBUGGER, "writing data");
        return e;
    }

    return WaitEvent((dbgevent_t *)NULL);
}

/*
** Find type of variable.
*/
dbgevent_t *DbgGDBGetType(char *var) {
#ifdef GDB_WITH_AIF
    dbgevent_t *e;
#endif /* GDB_WITH_AIF */
    dbgevent_t *e1;

    DPRINT("*** DbgGetType(%s)\n", var);

#ifdef GDB_WITH_AIF
    SendCmd("set print aif on");

    e = WaitEvent((dbgevent_t *)NULL);

    if (e->ev_event != DBGEV_OK) return e;

    FreeEvent(e);
#endif /* GDB_WITH_AIF */

    SendCmd("ptype %s", var);

    e1 = WaitEvent((dbgevent_t *)NULL);

#ifdef GDB_WITH_AIF
    SendCmd("set print aif off");

    e = WaitEvent((dbgevent_t *)NULL);

    if (e->ev_event != DBGEV_OK) {
        FreeEvent(e1);
        return e;
    }

    FreeEvent(e);
#endif /* GDB_WITH_AIF */

    return e1;
}

/*
** Find current dbgloc_t.
*/
dbgevent_t *DbgGDBGetLocation(void) {
    DPRINT("*** DbgGetLocation\n");

    SendCmd("info line");

    return WaitEvent((dbgevent_t *)NULL);
}

/*
** List variables.
*/
dbgevent_t *DbgGDBGetVars(char *file) {
    dbgevent_t *e;

    DPRINT("*** DbgGetVars\n");

    if (file == NULL || *file == '\0')
        SendCmd("dbg globals");
    else
        SendCmd("dbg globals %s", file);

    e = NewEvent(DBGEV_VARS);
    e->ev_vars = (dbgvars_t *)NULL;

    return WaitEvent(e);
}

/*
** Get debugger status.
*/
dbgevent_t *DbgGDBStatus(void) {
    dbgevent_t *e = NewEvent(DBGEV_STAT);

    DPRINT("*** DbgStatus\n");

    e->ev_status = Status;

    return e;
}

/*
** Quit debugger.
*/
dbgevent_t *DbgGDBQuit(void) {
    DPRINT("*** DbgQuit\n");

    /*
    ** For ALPS the app was launched before the debugger and then
    ** the debugger attached to it. Exiting the debugger will
    ** not cause the app process to exit. We need to kill it. Period.
    */
#ifdef ALPS
    DPRINT("DbgGDBQuit: SIGKILL to pid=%d\n", Pid);
    (void)kill(Pid, SIGKILL);
#else
    /*
    ** Interrupt the process if it is running.
    */
    if (Status == DBGSTAT_RUNNING) (void)kill(Pid, SIGINT);
#endif

    return (dbgevent_t *)NULL;
}

/*
** Interrupt the debugger.
*/
dbgevent_t *DbgGDBInterrupt(char *host, int prog) {
    DPRINT("*** DbgInterrupt\n");

    if (Status == DBGSTAT_RUNNING) {
        (void)kill(Pid, SIGINT);

        AsyncCheck(WaitEvent, (dbgevent_t *)NULL, host, prog);
    }

    return (dbgevent_t *)NULL;
}

/*
** Start debugger.
*/
dbgevent_t *StartDebugger(char **env, char *slave) {
    int p[2];
#ifndef sun
    int p1[2];
#endif /* !sun */
    char **ap;
    dbgevent_t *e;

    Status = DBGSTAT_INITIALISING;

    if (pipe(p) < 0
#ifndef sun
        || pipe(p1) < 0
#endif /* !sun */
    ) {
        EVENT_ERROR(e, DBGERR_PIPE, strerror(errno));
        return e;
    }
    DPRINT("StartDebugger(): pipe fds: p=%d,%d p1=%d,%d\n", p[0], p[1], p1[0],
           p1[1]);

    signal(SIGCHLD, Reap);
    signal(SIGTERM, Finish);
    signal(SIGHUP, Finish);
    signal(SIGINT, Finish);
    signal(SIGPIPE, Finish);

    if ((GDBInit = SetupDbg(slave)) == NULL) {
        EVENT_ERROR(e, DBGERR_TEMP, "");
        return e;
    }

    (void)setsid();

    /* 20071017 - jsc - adding print statement for LD_LIBRARY_PATH env variable
     */
    DPRINT("StartDebugger: LD_LIBRARY_PATH=%s\n", getenv("LD_LIBRARY_PATH"));

    switch (fork()) {
        case 0:
            (void)close(0);
            (void)close(1);
            (void)close(2);
#ifndef sun
            dup(p1[0]);
            dup(p[1]);
            dup(p[1]);
            (void)close(p[0]);
            (void)close(p[1]);
            (void)close(p1[0]);
            (void)close(p1[1]);
#else  /* !sun */
            dup(p[1]);
            dup(p[1]);
            dup(p[1]);
            (void)close(p[0]);
            (void)close(p[1]);
#endif /* !sun */

            ap = NewArgv("gdb-guard", "-n", "-x", GDBInit, NULL);

            /* 20071017 - jsc - commented out for call to gdb-guard */
            /* Exec(GDB, ap, env); */
            Exec("gdb-guard", ap, env);

            exit(1);

        case -1:
            DbgGDBFinish();
            EVENT_ERROR(e, DBGERR_FORK, strerror(errno));
            return e;

        default:
            break;
    }

#ifndef sun
    DBGIn = p[0];
    DBGOut = p1[1];
    (void)close(p[1]);
    (void)close(p1[0]);
#else  /* !sun */
    DBGIn = DBGOut = p[0];
    (void)close(p[1]);
#endif /* !sun */

    InitRegexGDB();

    BP = NewList();

    return (dbgevent_t *)NULL;
}

extern int AllocPty(int *, char **, char **);
extern int AllocFifo(int *, char **, char **);
extern char **environ;

dbgevent_t *InitGDB(int *dfd, int *iofd, char **env) {
    char *master;
    char *slave;
    dbgevent_t *e;

    /*
    ** Try allocating a pty for backend i/o first. If this fails then
    ** we use a fifo. Note that using a fifo restricts us to output only.
    */
    if (AllocPty(iofd, &master, &slave) < 0 &&
        AllocFifo(iofd, &master, &slave) < 0)
        *iofd = -1;

    /* TODO Check if env is really needed */
    /* 20071017 - jsc - replacing NULL in StartDebugger(NULL, slave) with
     * environ */
    /* if ( (e = StartDebugger(environ, slave)) != (dbgevent_t *)NULL ) */
    if ((e = StartDebugger(env, slave)) != (dbgevent_t *)NULL) return e;

    *dfd = DBGIn;

#ifdef sun
    IODevName = master;
#endif /* sun */

    return (dbgevent_t *)NULL;
    ;
}

#ifdef sun
/*
** It appears (in Solaris 2.5) that if the slave
** closes its pty before the master has time to read
** the current data, then we get EIO's until the
** slave opens the pty again. The only way to stop this
** seems to be to close and reopen the master side.
*/
int ProgIOHookGDB(int fd) {
    if (IODevName == NULL) return -1;

    (void)close(fd);

    if ((fd = open(IODevName, O_RDWR, 0600)) < 0) {
        perror(IODevName);
        return -1;
    }

    return fd;
}
#endif /* sun */

dbgevent_t *DbgGDBInvoke(char *host, int cb, char *proto, char *prog,
                         char *args, char **env, int arch) {
    dbgevent_t *e;

    DPRINT("*** DbgInvoke\n");

    MPArch = arch;

    switch (arch) {
#ifdef P4
        case MPARCH_P4MASTER:
            e = InitP4Dbg(host, cb, proto, prog, args, env);
            break;
#endif /* P4 */

        default:
            e = InitDbg(prog, args);
    }

    Status = DBGSTAT_STOPPED;

    return e;
}

/*
** Attach debugger to running process.
*/
dbgevent_t *DbgGDBAttach(int pid, char *prog, char *args, char **env,
                         int arch) {
    dbgevent_t *e;

    DPRINT("*** DbgAttach\n");

    MPArch = arch;

    switch (arch) {
#ifdef P4
        case MPARCH_P4SLAVE:
            e = InitP4DbgAttach(pid, prog, args, env);
            break;
#endif /* P4 */

        default:
            e = InitDbgAttach(pid, prog);
            break;
    }

    Status = DBGSTAT_STOPPED;

    return e;
}

/*
** Signal handler.
*/
void Finish(int sig) {
    Shutdown++;

    if (GDBInit != NULL) {
        if (unlink(GDBInit) < 0) perror(GDBInit);

        free(GDBInit);
    }

    if (RedirStr != NULL) free(RedirStr);
}

#ifdef ALPS
/*
** The attached process is waiting at the startup barrier and I can
** read the rank it was assigned. However, rank reordering will not
** have happened yet.
**
** This tapes into the PMI interface common to all Cray programming models.
*/
dbgevent_t *DbgGDBGetRank(int *rank) {
    dbgevent_t *e;

    /* set debugger status */
    Status = DBGSTAT_INITIALISING;

    /* Turn on AIF printing and print the mpi rank.
     * If the debugger did not do a data event,
     * print error and return event
     */
    SendCmd("set print aif on");
    FreeEvent(WaitEvent((dbgevent_t *)NULL));

    SendCmd("print _pmi_rank");
    e = WaitEvent((dbgevent_t *)NULL);
    if (e->ev_event != DBGEV_DATA) {
        fprintf(stderr, "DbgGDBGetRank: _pmi_rank did not return data! (%d)\n",
                e->ev_event);
        return (e);
    }
    AIFToInt(e->ev_data, rank);
    FreeEvent(e);

    DPRINT("DbgGDBGetRank: _pmi_rank is %d\n", *rank);

    /* turn off AIF printing */
    SendCmd("set print aif off");
    FreeEvent(WaitEvent((dbgevent_t *)NULL));

    /* set debugger status */
    Status = DBGSTAT_STOPPED;

    return NewEvent(DBGEV_OK);
}
#else
dbgevent_t *DbgGDBGetRank(int *rank) {
    // Place holder for generic or other platform
}
#endif

/*
** Called if Shutdown > 0 to finalise debugger.
*/
void DbgGDBFinish(void) {
    SendCmd("quit");

#ifdef sun
    if (IODevName != NULL) (void)unlink(IODevName);
#endif /* sun */
}

void SendCmd(char *fmt, ...) {
    int l;
    int n;
    char *s;
    char *p;
    char buf[BUFSIZ];
    va_list ap;

    Synced = 0;

    s = (char *)Alloc(strlen(fmt) + 2);
    strcpy(s, fmt);

    if (fmt[strlen(fmt) - 1] != '\n') strcat(s, "\n");

    va_start(ap, fmt);

    vsnprintf(buf, BUFSIZ - 1, s, ap);

    va_end(ap);

    l = strlen(buf);
    p = buf;

    DPRINT(">>>%s", buf);

    while (l > 0) {
        if ((n = write(DBGOut, p, l)) < 0) {
            perror("SendCmd");
            Free(s);
            return;
        }

        l -= n;
        p += n;
    }

    Free(s);
}

static char *_dbg_oldline = NULL;
static char *_dbg_line = NULL;
static int _dbg_linelen = 0;
static int _dbg_ready = 0;

char *findchar(char *str, int len, char c) {
    if (str == NULL) return NULL;

    while (--len >= 0) {
        if (*str == c) return str;

        str++;
    }

    return NULL;
}

/*
** Fill a buffer with all chars up to next newline.
*/
int FillBuffer(int fd) {
    /*	int		i; */
    int n;
    int len;
    char *p;
    char buf[BUFSIZ + 1];
    static int _dbg_buflen = 0;
    static char _dbg_buf[BUFSIZ + 1];

    DPRINT("*** FillBuffer %d\n", _dbg_ready);

    /*
    ** If we already have a line, just return.
    */
    if (_dbg_ready) return 1;

    /*
    ** First check if we've already got some data.
    */
    if (_dbg_buflen > 0) {
        /*
        ** See if it contains a newline.
        */
        if ((p = findchar(_dbg_buf, _dbg_buflen, '\n')) != NULL) {
            len = p - _dbg_buf + 1;
            _dbg_ready = 1;
        } else
            len = _dbg_buflen;

        /*
        ** Add it to the current line, or start a new one.
        */

        if (_dbg_line == NULL) {
            p = _dbg_line = (char *)Alloc(len + 1);
            _dbg_linelen = len;
        } else {
            _dbg_line =
                (char *)Resize((void *)_dbg_line, _dbg_linelen + len + 1);
            p = _dbg_line + _dbg_linelen;
            _dbg_linelen += len;
        }

        memcpy(p, _dbg_buf, len);
        p[len] = '\0';

        /*
        ** Fix up saved buffer
        */
        if ((_dbg_buflen -= len) > 0)
            memcpy(_dbg_buf, _dbg_buf + len, _dbg_buflen);

        /*
        ** Return if we have a complete line.
        */
        if (_dbg_ready) return 1;
    }

    /*
    ** Next, read some data. We can read a whole BUFSIZ here, 'cause
    ** if we got this far _dbg_buf musn't have contained a newline,
    ** and so was appended to the current line (or was empty).
    */
    if ((n = read(fd, buf, BUFSIZ)) <= 0) {
        if (n < 0 && errno != EINTR)
            perror("FillBuffer");
        else if (n == 0)
            fprintf(stderr, "FillBuffer: EOF encountered\n");

        return 0;
    }

    /*
    ** Check if we got a newline.
    */
    if ((p = findchar(buf, n, '\n')) != NULL) {
        len = p - buf + 1;
        _dbg_ready = 1;
    } else
        len = n;

    /*
    ** Add it to the current line, or start a new one.
    */

    if (_dbg_line == NULL) {
        p = _dbg_line = (char *)Alloc(len + 1);
        _dbg_linelen = len;
    } else {
        _dbg_line = (char *)Resize((void *)_dbg_line, _dbg_linelen + len + 1);
        p = _dbg_line + _dbg_linelen;
        _dbg_linelen += len;
    }

    memcpy(p, buf, len);
    p[len] = '\0';

    DPRINT("FillBuffer: line is \"%s\"\n", _dbg_line);

    /*
    ** Save any leftovers.
    */
    if ((n -= len) > 0) {
        memcpy(_dbg_buf, buf + len, n);
        _dbg_buflen = n;
    }

    return 1;
}

/*
** Get ready to read a new line and free the old one.
*/
char *FreeBuffer(void) {
    if (_dbg_oldline != NULL) Free(_dbg_oldline);

    _dbg_oldline = _dbg_line;

    _dbg_line = NULL;
    _dbg_linelen = 0;
    _dbg_ready = 0;

    return _dbg_oldline;
}

/*
** Test if we have a line ready.
*/
int BufferReady(void) {
    DPRINT("*** BufferReady %d\n", _dbg_ready);

    return _dbg_ready;
}

/*
 * Read a complete line. Automatically frees last line.
 */
char *ReadLine(void) {
    char *line;

    DPRINT("*** ReadLine %d\n", _dbg_ready);

    /*
    ** Read data until we have a complete line.
    */
    while (!BufferReady()) {
        if (!FillBuffer(DBGIn)) {
            Status = DBGSTAT_DBGERR;
            return NULL;
        }
    }

    /*
    ** Remove newline.
    */
    _dbg_line[_dbg_linelen - 1] = '\0';

    /*
    ** Free up resources
    */
    line = FreeBuffer();

    DPRINT("ReadLine: \"%s\"\n", line);

    return line;
}

#ifdef notdef
/*
** Find out how many chars are available to read
** from the pipe.
*/
int CharsToRead(int fd) {
    struct stat st;

    if (fstat(fd, &st) < 0) {
        perror("fstat");
        return -1;
    }

    return st.st_size;
}
#endif /* notdef */

/*
** Read size bytes.
**
** NOTE: We do this by reading newline terminated
** chunks until we have enough data. This implies that the
** data must also be newline terminated.
**
** ReadData does not automatically free last data read.
*/
char *ReadData(int size) {
    int len = 0;
    char *data = NULL;

    DPRINT("ReadData: reading %d bytes\n", size);

    if (size == 0) return NULL;

    while (len < size) {
        /*
        ** Read data until we have a complete line.
        */
        while (!BufferReady()) {
            if (!FillBuffer(DBGIn)) {
                if (data != NULL) Free(data);

                Status = DBGSTAT_DBGERR;
                return NULL;
            }
        }

        if (data == NULL)
            data = (char *)Alloc(_dbg_linelen + 1);
        else
            data = (char *)Resize((void *)data, len + _dbg_linelen + 1);

        memcpy(data + len, _dbg_line, _dbg_linelen);
        len += _dbg_linelen;

        (void)FreeBuffer();
    }

    DPRINT("ReadData: read \"%s\"\n", data);

    return data;
}

/*
** Write data to gdb.
*/
int WriteBuf(char *data, int len) {
    int l;
    int n;

    DPRINT("WriteBuf: writing %d bytes\n", len);

    do {
        l = (len > PIPE_BUF) ? PIPE_BUF : len;

        if ((n = write(DBGOut, data, l)) <= 0) {
            fprintf(stderr, "WriteBuf: short data\n");
            return -1;
        }

        data += n;
        len -= n;
    } while (len > 0);

    /*
    ** Send newline to terminate command.
    */

    (void)write(DBGOut, "\n", 1);

    return 0;
}

/*
** Sync Debugger
*/
dbgevent_t *Sync(void) {
    int res;
    char *line;
    dbgevent_t *e;

    DPRINT("*** Sync()\n");

    if (Synced) return (dbgevent_t *)NULL;

    while ((line = ReadLine()) != NULL)
        if ((res = MatchRegex(RegexPrompt, line)) != 0) break;

    if (line == NULL) {
        EVENT_ERROR(e, DBGERR_DEBUGGER, "reading command");
        return e;
    }

    if (res < 0) {
        EVENT_ERROR(e, DBGERR_REGEX, RegexPrompt->d_errbuf);
        return e;
    }

    Synced = 1;

    return (dbgevent_t *)NULL;
}

/*
** Set up .gdbinit file. For POE we need to break at main
** otherwise the MPI initialisation does not happen and
** the code will hang.
*/
char *SetupDbg(char *slave) {
    FILE *fp;
    char *file;

#if !defined(ALPS)
    if ((file = tempnam("/tmp", "gdb")) == NULL)
#else
    // dbgsrv_alps chdir'd to the alps tool dir
    if ((file = tempnam(".", "gdb")) == NULL)
#endif
        // if ( (file = mkstemp("gdb")) == NULL )
        return NULL;

    if ((fp = fopen(file, "w")) == (FILE *)NULL) {
        free(file);
        return NULL;
    }

    /*
     * The gdb manual specifically says
     * that height and width need to be set
     * to 0, NOT -1.
     */
    fprintf(fp, "set prompt (gdb)\\n\n");
    fprintf(fp, "set confirm no\n");
    fprintf(fp, "set height 0\n");
    fprintf(fp, "set width 0\n");
    /*fprintf(fp, "set print aif on\n");*/
#if !defined(ALPS)
    fprintf(fp, "handle all nostop noprint pass\n");
#endif

    if (slave != NULL && *slave != '\0') {
        if (*slave == '>')
            RedirStr = strdup(slave);
        else
            fprintf(fp, "tty %s\n", slave);
    }

    fclose(fp);

    return file;
}

#ifdef notdef
char *SkipLine(char *line, int l) {
    int n;
    char *s;

    for (n = 0; n < l; n++) {
        if ((s = strchr(line, '\n')) == NULL) return NULL;

        line = ++s;
    }

    return line;
}
#endif /* notdef */

/*
** Check for correct debugger version.
*/
dbgevent_t *CheckVers(void) {
    dbgevent_t *e;

    e = WaitEvent((dbgevent_t *)NULL);

    if (e->ev_event != DBGEV_VERS) {
        /* 20071017 - jsc - added error print */
        fprintf(stderr, "CheckVers: GDB error: %d\n", e->ev_event);

        if (e->ev_event != DBGEV_ERROR) {
            EVENT_ERROR(e, DBGERR_DEBUGGER,
                        "Bad response from debugger, expected version.");
        } else {
            fprintf(stderr, "\terrno: %d (%s)", e->ev_errno,
                    e->ev_errstr); /* 20071017 - jsc - added errorstr print */
        }

        return e;
    }

    fprintf(stderr, "Found debugger version %s\n", e->ev_version);

    fprintf(stderr, "Debugger architecture is %s\n\n", e->ev_arch);

    DPRINT("CheckVers: Debugger vers=%s arch=%s\n", e->ev_version, e->ev_arch);

    FreeEvent(e);

    SYNC();

    return (dbgevent_t *)NULL;
}

/*
** Initialise debugger, make sure we are using
** the correct version, etc.
*/
dbgevent_t *InitDbg(char *prog, char *args) {
    dbgevent_t *e;
    int pidAlreadyObtained = 0;

    if ((e = CheckVers()) != (dbgevent_t *)NULL) {
        fprintf(stderr, "InitDbg: could not check version of GDB\n");
        return e;
    }

    SendCmd("file %s", prog);

    e = WaitEvent((dbgevent_t *)NULL);

    if (e->ev_event == DBGEV_ERROR)
        fprintf(stderr, "InitDbg: error opening %s errno: %d (%s)\n", prog,
                e->ev_errno, e->ev_errstr);

    if (e->ev_event != DBGEV_OK) {
        fprintf(stderr, "InitDbg: event error %d\n", e->ev_event);
        return e;
    }

    FreeEvent(e);

    if (args != NULL) {
        e = DbgGDBSetArgs(args);

        if (e->ev_event != DBGEV_OK) return e;

        FreeEvent(e);
    }

    /*
    ** Check for fortran.
    */
    SendCmd("break MAIN__");

    e = WaitEvent((dbgevent_t *)NULL);

    if (e->ev_event == DBGEV_BPSET) {
        /*
        ** We are Fortran
        */
        ProgMain = "MAIN__";
    } else {
        if (e->ev_event == DBGEV_ERROR) {
            if (e->ev_errno != DBGERR_NOFUNC) return e;

            FreeEvent(e);

            SendCmd("break main");

            e = WaitEvent((dbgevent_t *)NULL);

            if (e->ev_event != DBGEV_BPSET) {
                if (e->ev_event == DBGEV_ERROR) return e;

                FreeEvent(e);
                EVENT_ERROR(e, DBGERR_DEBUGGER, "Could not set break at main.");
                return e;
            }

            /*
            ** We are C
            */
            ProgMain = "main";

            /* 20071017 - jsc - added print */
            DPRINT("InitDbg(): breapoint set at main\n");
        } else {
            FreeEvent(e);
            EVENT_ERROR(e, DBGERR_DEBUGGER, "Could not set break at main.");
            return e;
        }
    }

    FreeEvent(e);

    if (RunArgs == NULL) RunArgs = strdup("");

    SendCmd("run %s %s", RunArgs, RedirStr != NULL ? RedirStr : "");

    e = WaitEvent((dbgevent_t *)NULL);

    if (e->ev_event == DBGEV_PID) {
        /* Newer versions of gdb, if libthread_db is active, will print the pid
         * of the LWP created */
        Pid = e->ev_pid;
        DPRINT("InitDbg(): obtained pid %d\n", Pid);
        pidAlreadyObtained = 1;
        FreeEvent(e);
        e = WaitEvent((dbgevent_t *)NULL);
    }

    if (e->ev_event != DBGEV_BPHIT) {
        /*
        ** Report real error rather than failure message.
        */

        /*
        FreeEvent(e);
        EVENT_ERROR(e, DBGERR_DEBUGGER, "Program failed to break at main.");
        */
        return e;
    }

    FreeEvent(DbgGDBDeleteBreak(e->ev_bp->bp_id));

    /*
    ** Find out pid.
    */

    /* 20071017 - jsc - added check for pidAlreadyObtained, which would have
     * happend if libthread_db was active above */
    if (!pidAlreadyObtained) GETPID(Pid, e);

    FreeEvent(e);

    SYNC();

    DPRINT("InitDbg: ProgMain=%s\n", ProgMain);

    return NewEvent(DBGEV_OK);
}

/*
** Attach debugger. Added specifically for Cray XT / ALPS, but it might
** work for other platforms as well.
*/
dbgevent_t *InitDbgAttach(int pid, char *prog) {
    dbgevent_t *e;
    if ((e = CheckVers()) != (dbgevent_t *)NULL) return e;
    FreeEvent(e);

    SendCmd("attach %d", pid);
    e = WaitEvent((dbgevent_t *)NULL);

    // Check for success/failure
    if (e->ev_event != DBGEV_PID) {
        fprintf(stderr, "InitDbgAttach(): Attach failed: ev=%d\n", e->ev_event);
        FreeEvent(e);
        EVENT_ERROR(e, DBGERR_DEBUGGER, "Attach failed.");
        return e;
    }
    Pid = e->ev_pid;
    DPRINT("InitAttachDbg(): obtained pid %d\n", Pid);
    if (pid != Pid) {
        fprintf(stderr, "InitDbgAttach(): Pids don't match: %d!=%d:\n", pid,
                Pid);
    }
    FreeEvent(e);

    /*
    ** Putting in the SYNC seems to be needed to get communications
    ** with gdb backon track. I don't know why. (rwm)
    */
    SYNC();

#if 0  // This seems to cause a memory fault on "b main; cont"
    /*
    ** This was in the gdb init file, but seemed to break attaching.
    ** It has been removed, but sending it puts it back, not that the
    ** attach is complete (since someone originally thought it was needed).
    */
    fprintf(stderr, "InitDbgAttach SendCmd handle...\n"); // ???
    SendCmd("handle all nostop noprint pass");
    
    fprintf(stderr, "InitDbgAttach handle WaitEvent...\n");
	e = WaitEvent((dbgevent_t *)NULL);
    FreeEvent(e);
#endif

    /*
    ** Don't even think about trying to set a breakpoint in main
    ** and continuing at this juncture. The target is still waiting
    ** at the startup barrier.
    */

    return NewEvent(DBGEV_OK);
}

#ifdef P4
/*
** Initialise debugger using P4.
*/
dbgevent_t *InitP4Dbg(char *host, int cb, char *proto, char *prog, char *args,
                      char **env) {
    int i;
    int nprocs;
    char *line;
    char *pid;
    char **ap;
    char **argp;
    dbgevent_t *e;
    char pnum[BUFSIZ];
    char cbstr[BUFSIZ];
    char dbgsrv[MAXPATHLEN];

    if ((e = CheckVers()) != (dbgevent_t *)NULL) return e;

    SendCmd("file %s", prog);

    e = WaitEvent((dbgevent_t *)NULL);

    if (e->ev_event != DBGEV_OK) return e;

    FreeEvent(e);

    SendCmd("break p4_debug_master_wait");

    e = WaitEvent((dbgevent_t *)NULL);

    if (e->ev_event != DBGEV_BPSET) {
        FreeEvent(e);
        EVENT_ERROR(e, DBGERR_DEBUGGER, "Program not using P4 debug mode.");
        return e;
    }

    FreeEvent(e);

    if (RunArgs == NULL) RunArgs = strdup("");

    SendCmd("run %s %s", RunArgs, RedirStr != NULL ? RedirStr : "");

    e = WaitEvent((dbgevent_t *)NULL);

    if (e->ev_event != DBGEV_BPHIT) {
        FreeEvent(e);
        EVENT_ERROR(e, DBGERR_DEBUGGER,
                    "Program failed to break at p4_debug_master_wait.");
        return e;
    }

    FreeEvent(DbgGDBDeleteBreak(e->ev_bp->bp_id));

    SendCmd("printf \"%%d\\n\", n_pids");

    if ((line = ReadLine()) == NULL) {
        EVENT_ERROR(e, DBGERR_DEBUGGER, "getting number of P4 processes");
        return e;
    }

    SYNC();

    nprocs = atoi(line);

    snprintf(dbgsrv, MAXPATHLEN - 1, "%s/%s", BINDIR, DBGPROG);
    snprintf(cbstr, BUFSIZ - 1, "%d", cb);

    for (i = 0; i < nprocs; i++) {
        /* int v; */
        SendCmd("printf \"%%d\\n\", pid_list[%d]", i);

        if ((line = ReadLine()) == NULL) {
            EVENT_ERROR(e, DBGERR_DEBUGGER, "getting P4 pid");
            return e;
        }

        SYNC();

        pid = strdup(line);

        snprintf(pnum, BUFSIZ - 1, "%d", i + 1);

        ap = NewArgv("dbgsrv", "-h", host, "-c", cbstr, "-a", "p4slave", "-i",
                     line, "-n", pnum, "-p", proto, prog, NULL);

        argp = Str2Argv(args);

        ap = AppendArgv(ap, argp);

        ForkAndExec(dbgsrv, ap, env);

        FreeArgv(ap);
        FreeArgv(argp);
        Free(pid);
    }

    /*
    ** Race condition. May not set breakpoints in slaves before
    ** we continue master. Sleep a bit here to make this less
    ** likely.
    */
    sleep(3);

    SendCmd("break p4_debug_master");

    e = WaitEvent((dbgevent_t *)NULL);

    if (e->ev_event != DBGEV_BPSET) {
        FreeEvent(e);
        EVENT_ERROR(e, DBGERR_DEBUGGER, "Program not using P4 debug mode.");
        return e;
    }

    FreeEvent(e);

    SendCmd("cont");

    e = WaitEvent((dbgevent_t *)NULL);

    if (e->ev_event != DBGEV_BPHIT) {
        FreeEvent(e);
        EVENT_ERROR(e, DBGERR_DEBUGGER,
                    "Program failed to break at p4_debug_master.");
        return e;
    }

    FreeEvent(DbgGDBDeleteBreak(e->ev_bp->bp_id));

    /*
    ** Find out pid.
    */

    GETPID(Pid, e);

    FreeEvent(e);

    return NewEvent(DBGEV_OK);
}

/*
** Attach debugger using P4.
*/
dbgevent_t *InitP4DbgAttach(int pid, char *prog, char *args, char **env) {
    /*	char **		ap; */
    /*	char **		argp; */
    dbgevent_t *e;
    /*	char		dbgsrv[MAXPATHLEN]; */

    if ((e = CheckVers()) != (dbgevent_t *)NULL) return e;

    SendCmd("file %s", prog);

    e = WaitEvent((dbgevent_t *)NULL);

    if (e->ev_event != DBGEV_OK) return e;

    FreeEvent(e);

    SendCmd("attach %d", pid);

    e = WaitEvent((dbgevent_t *)NULL);

    if (e->ev_event != DBGEV_OK) return e;

    FreeEvent(e);

    SendCmd("break p4_debug_slave");

    e = WaitEvent((dbgevent_t *)NULL);

    if (e->ev_event != DBGEV_BPSET) {
        FreeEvent(e);
        EVENT_ERROR(e, DBGERR_DEBUGGER, "Program not using P4 debug mode.");
        return e;
    }

    FreeEvent(e);

    SendCmd("cont");

    e = WaitEvent((dbgevent_t *)NULL);

    if (e->ev_event != DBGEV_BPHIT) {
        FreeEvent(e);
        EVENT_ERROR(e, DBGERR_DEBUGGER,
                    "Program failed to break at p4_debug_slave.");
        return e;
    }

    FreeEvent(DbgGDBDeleteBreak(e->ev_bp->bp_id));

    /*
    ** Find out pid.
    */

    GETPID(Pid, e);

    FreeEvent(e);

    return NewEvent(DBGEV_OK);
}
#endif /* P4 */

dbgevent_t *GetPid(int *pid) {
    dbgevent_t *e;

#ifdef GDB_WITH_AIF
    SendCmd("dbg pid");

    e = WaitEvent((dbgevent_t *)NULL);

    if (e->ev_event != DBGEV_PID) {
        EVENT_ERROR(e, DBGERR_DEBUGGER, "pid command not available");
        return e;
    }
#else /* GDB_WITH_AIF */
#if defined(__linux)
    /* We are using unpatched GDB */
    SendCmd("info proc"); /* Works with Linux */

    e = WaitEvent((dbgevent_t *)NULL);

    if (e->ev_event != DBGEV_PID) { /* Can't do anything else */
        EVENT_ERROR(e, DBGERR_DEBUGGER, "can't determine pid");
        return e;
    }
#elif defined(__APPLE__)
    SendCmd("info pid");

    e = WaitEvent((dbgevent_t *)NULL);

    if (e->ev_event != DBGEV_PID) { /* Can't do anything else */
        EVENT_ERROR(e, DBGERR_DEBUGGER, "can't determine pid");
        return e;
    }
#else
    EVENT_ERROR(e, DBGERR_DEBUGGER, "can't determine pid");
    return e;
#endif
#endif /* GDB_WITH_AIF */

    *pid = e->ev_pid;

    FreeEvent(e);

#ifdef notdef
    SendCmd("info program");

    e = WaitEvent((dbgevent_t *)NULL);

    if (e->ev_event != DBGEV_PROGRAM) {
        FreeEvent(e);
        return -1;
    }

    pid = e->event_pid;

    FreeEvent(e);
#endif /* notdef */

    return (dbgevent_t *)NULL;
}

#ifdef notdef
int ParseFmt(char *str, char **fmt, int *len) {
    int size;
    int fs;
    char *f;
    char *f1;
    char *s;
    char *base;
    static char _fmt[BUFSIZ];

    if (strncmp(str, "type = ", 7) != 0) return 0;

    base = str + 7;

    if ((s = strchr(base, ' ')) != NULL) *s++ = '\0';

    f = _fmt;
    *f = '\0';
    size = 1;

    switch (*s) {
        case '[':
            while (*s == '[') {
                s++;

                for (f1 = f; *s != '\0' && *s != ']';) *f1++ = *s++;

                *f1 = '\0';

                fs = atoi(f);
                size *= fs; /*XXX fortran! */

                if (*s++ != ']') return 0;

                snprintf(f, BUFSIZ - strlen(f), "[r0..%dis4]", fs - 1);
                f += strlen(f);
            }

            if (*s != '\0') return 0;

            break;

        case '\0':
            break;

        case '{':
        default:
            return 0;
    }

    strcat(_fmt, ConvBase(base));
    *len = size * BaseSize(base);
    *fmt = _fmt;

    return 1;
}

char *ParseData(int len) {
    int i;
    int j;
    char *d;
    char *s;
    char *t;
    char *data;
    char *line;

    if (len <= 0) return NULL;

    i = (len - 1) / 8;

    d = data = (char *)Alloc(len);

    for (; i >= 0; i--) {
        line = ReadLine();

        if ((s = strchr(line, ':')) == NULL) return NULL;

        s++;

        if (*s != '\t' || *++s == '\0') return NULL;

        while ((t = strtok(s, "\t")) != NULL) {
            s = NULL;

            if (strlen(t) < 4) return NULL;

            /* skip 0x */
            *d = FromHex(*(t + 2)) << 4;
            *d++ |= FromHex(*(t + 3));
        }
    }

    return data;
}
#endif /* notdef */

char *ConvBase(char *base) {
    char *fmt;
    static char _r[4];

    switch (*base) {
        case 'i':
            fmt = "is%d";
            break;

        case 'u':
            fmt = "iu%d";
            break;

        case 'f':
        case 'd':
            fmt = "f%d";
            break;

        default:
            return NULL;
    }

    snprintf(_r, sizeof(_r) - 1, fmt, BaseSize(base));

    return _r;
}

int BaseSize(char *base) {
    switch (*base) {
        case 'i':
            return sizeof(int);

        case 'u':
            return sizeof(unsigned);

        case 'f':
            return sizeof(float);

        case 'd':
            return sizeof(double);

        default:
            return 0;
    }
}

int FromHex(char c) {
    if (c >= '0' && c <= '9') return c - '0';

    if (c >= 'a' && c <= 'f') return c - 'a' + 10;

    if (c >= 'A' && c <= 'F') return c - 'A' + 10;

    return 0;
}

AIF *GDBBuildAIFVar(char *type, char *data) {
    if (strcmp(type, "int") == 0) {
        return IntToAIF(atoi(data));
    } else
        return NULL;
}
#endif /* GDB */
