/*********************************************************************************\
 * Event routines
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * Copyright 2010-2013,2020 Cray Inc.
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

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "dbg.h"
#include "defs.h"
#include "useful.h"

// Static prototypes
static event_equiv CompareEvExit(int, int);
static event_equiv CompareEvBp(dbgbp_t *, dbgbp_t *);
static event_equiv CompareEvSig(dbgsig_t, dbgsig_t);
static event_equiv CompareEvStep(dbgstep_t, dbgstep_t);
static event_equiv CompareEvFrame(dbgframe_t *, dbgframe_t *, int);
static event_equiv CompareEvErr(dbgerr_t, dbgerr_t);

static dbgframe_t *DupFrame(dbgframe_t *);
static void FreeFrame(dbgframe_t *);
static dbgloc_t *DupLocation(dbgloc_t *);
static void FreeLocation(dbgloc_t *);
static dbgvars_t *DupVars(dbgvars_t *);
static void FreeVars(dbgvars_t *);
static dbglist_t *DupSCList(dbglist_t *);
static void FreeSCList(dbglist_t *);
static dbgsources_t *DupSources(dbgsources_t *);
static void FreeSources(dbgsources_t *);
static dbgstatus_t *DupStatus(dbgstatus_t *);
static void FreeStatus(dbgstatus_t *);

// Used for error events
const char *DbgErrTab[] = {
    "*** command succeded",                       // DBG_OK
    "Protocol not implemented",                   // DBGERR_NOTIMP
    "No such protocol \"%s\"",                    // DBGERR_PROTO
    "Debugger error: %s",                         // DBGERR_DEBUGGER
    "No server for this process",                 // DBGERR_SERVER
    "Must register callback first",               // DBGERR_CALLBACK
    "Callback in progress",                       // DBGERR_INPROGRESS
    "No server to register callback",             // DBGERR_CBCREATE
    "Could not create callback",                  // DBGERR_NOSERVER
    "Server RPC error",                           // DBGERR_RPC
    "No line %s in file ",                        // DBGERR_NOLINE
    "Function \"%s\" not defined",                // DBGERR_NOFUNC
    "No source file named \"%s\"",                // DBGERR_NOFILE
    "No breakpoint number %s",                    // DBGERR_NOBP
    "No symbol \"%s\" in current context",        // DBGERR_NOSYM
    "Value optimized out",                        // DBGERR_OPT
    "Unsupported symbol type",                    // DBGERR_BADTYPE
    "*** can't access memory",                    // DBGERR_NOMEM
    "*** can't run the application",              // DBGERR_CANTRUN
    "*** could not invoke the application",       // DBGERR_INVOKE
    "*** The application is running",             // DBGERR_ISRUNNING
    "*** The application is not running",         // DBGERR_NOTRUN
    "Initial frame selected; you cannot go up.",  // DBGERR_FIRSTFRAME
    "Bottom (innermost) frame selected; you cannot go down.",  // DBGERR_LASTFRAME
    "Argument required (breakpoint number)",       // DBGERR_BADBPARG
    "*** error in regular expression",             // DBGERR_REGEX
    "No stack",                                    // DBGERR_NOSTACK
    "Line number %s is out of range for ",         // DBGERR_OUTOFRANGE
    "%s: No such file or directory",               // DBGERR_NOFILEDIR
    "No debugging symbols",                        // DBGERR_NOSYMS
    "Could not create temporary file",             // DBGERR_TEMP
    "pipe: %s",                                    // DBGERR_PIPE
    "fork: %s",                                    // DBGERR_FORK
    "select: %s",                                  // DBGERR_SELECT
    "%s: not in executable format",                // DBGERR_NOTEXEC
    "chdir: %s",                                   // DBGERR_CHDIR
    "Source file is more recent than executable",  // DBGERR_SOURCE
    "Can't set variable",                          // DBGERR_SETVAR
    "Attach failed",  // DBGERR_ATTACH_FAILED FIXME: This is never used yet.
    "*** The application is at an exit breakpoint.",  // DBGERR_AT_EXIT
    "Internal error: %s",                             // DBGERR_INTERNAL
    "Unkown GDB error: \"%s\"",                       // DBGERR_UNKNOWN
    "Invalid error number"                            // DBGERR_NULL
};

dbgevent_t *NewEvent(dbgev stat) {
    dbgevent_t *e;

    e = (dbgevent_t *)Alloc(sizeof(dbgevent_t));
    e->ev_event = stat;
    e->save_ev_data = 0;

    return e;
}

dbgevent_t *ChangeEvent(dbgevent_t *e, dbgev stat) {
    FreeEvent(e);
    return NewEvent(stat);
}

dbgevent_t *DupEvent(dbgevent_t *event) {
    dbgevent_t *e;

    if (event == (dbgevent_t *)NULL) return (dbgevent_t *)NULL;

    e = (dbgevent_t *)Alloc(sizeof(dbgevent_t));

    switch (e->ev_event = event->ev_event) {
        // Generic strings go here
        case DBGEV_SOURCEPATH:
        case DBGEV_TYPE:
        case DBGEV_CMD:
        case DBGEV_THREADINFO:
            e->ev_string = strdup(event->ev_string);
            break;

        // Generic int go here
        case DBGEV_EXIT:
            e->ev_int = event->ev_int;

        case DBGEV_BPHIT:
        case DBGEV_BPINFO:
        case DBGEV_BPSET:
        case DBGEV_BPDEL:
            e->ev_bp = DupBP(event->ev_bp);
            break;

        case DBGEV_SIGNAL:
            e->ev_sig_type = strdup(event->ev_sig_type);
            e->ev_sig_line = strdup(event->ev_sig_line);
            e->ev_sig_frame = DupFrame(event->ev_sig_frame);
            break;

        case DBGEV_STEP:
            e->ev_step_lno = event->ev_step_lno;
            e->ev_step_line = strdup(event->ev_step_line);
            e->ev_step_frame = DupFrame(event->ev_step_frame);
            break;

        case DBGEV_FRAME:
            e->ev_frame = DupFrame(event->ev_frame);
            break;

        case DBGEV_VARS:
            e->ev_vars = DupVars(event->ev_vars);

        case DBGEV_HALT:
        case DBGEV_LOC:
            e->ev_loc = DupLocation(event->ev_loc);
            break;

        case DBGEV_DATA:
        case DBGEV_DATA_SLICE:
            e->ev_data = (event->ev_data ? new AIFStream(*event->ev_data) : nullptr);
            break;

        case DBGEV_LIST:
            e->ev_list = DupSCList(event->ev_list);
            break;

        case DBGEV_STAT:
            e->ev_status = DupStatus(event->ev_status);
            break;

        case DBGEV_ERROR:
            e->ev_errno = event->ev_errno;
            e->ev_errstr = strdup(event->ev_errstr);
            break;

        case DBGEV_VERS:
            e->ev_version = strdup(event->ev_version);
            e->ev_arch = strdup(event->ev_arch);
            break;

        case DBGEV_SOURCES:
            e->ev_sources = DupSources(event->ev_sources);
            break;

        case DBGEV_OK:
        case DBGEV_NONE:
            break;
    }

    e->save_ev_data = event->save_ev_data;

    return e;
}

static event_equiv CompareEvExit(int status1, int status2) {
    if (status1 != status2) {
        return EV_DIFF;
    }

    return EV_EQUAL;
}

// FIXME: We consider a breakpoint to be the same if its id we give
// it matches. I don't know if we want to keep it this way in the long term.
static event_equiv CompareEvBp(dbgbp_t *bp1, dbgbp_t *bp2) {
    if (bp1->bp_id != bp2->bp_id) {
        return EV_DIFF;
    }

    return EV_EQUAL;
}

static event_equiv CompareEvSig(dbgsig_t sig1, dbgsig_t sig2) {
    // Ensure the sig types match
    if (strcmp(sig1.sig_type, sig2.sig_type)) {
        return EV_DIFF;
    }

    // Ensure the frame matches, if no frame ensure the line matches
    // otherwise assume they are the same
    if (sig1.sig_frame == NULL && sig2.sig_frame == NULL) {
        // If both have NULL frames, check the line
        if (sig1.sig_line == NULL || sig2.sig_line == NULL) {
            // If only one has a NULL line, they are different
            if (sig1.sig_line != NULL || sig2.sig_line != NULL) {
                return EV_DIFF;
            }
        } else {
            // Compare the line directly
            if (strcmp(sig1.sig_line, sig2.sig_line)) {
                return EV_DIFF;
            }
        }
    } else if (sig1.sig_frame == NULL || sig2.sig_frame == NULL) {
        // If only one of the frames is NULL, they are different
        return EV_DIFF;
    } else {
        // Both have frames, so compare those
        // Note that we don't want to recurse since we don't print out all the
        // frames.
        return CompareEvFrame(sig1.sig_frame, sig2.sig_frame, 0);
    }

    // If we get here they are both equal
    return EV_EQUAL;
}

static event_equiv CompareEvStep(dbgstep_t s1, dbgstep_t s2) {
    // Ensure the frame matches, if no frame ensure the line matches
    // otherwise assume they are the same
    if (s1.step_frame == NULL && s2.step_frame == NULL) {
        // If both have NULL frames check the line
        if (s1.step_line == NULL || s2.step_line == NULL) {
            // If only one has a NULL line, they are different
            if (s1.step_line != NULL || s2.step_line != NULL) {
                return EV_DIFF;
            }
        } else {
            // Compare the line directly
            if (strcmp(s1.step_line, s2.step_line)) {
                return EV_DIFF;
            }
        }

        // Also need to check the lno
        if (s1.step_lno != s2.step_lno) {
            return EV_DIFF;
        }
    } else if (s1.step_frame == NULL || s2.step_frame == NULL) {
        // If only one of the frames is NULL, they are different
        return EV_DIFF;
    } else {
        // Both have frames, so compare those
        // Note that we don't want to recurse since we don't print out all the
        // frames.
        return CompareEvFrame(s1.step_frame, s2.step_frame, 0);
    }

    // If we get here they are both equal
    return EV_EQUAL;
}

static event_equiv CompareEvFrame(dbgframe_t *fp1, dbgframe_t *fp2,
                                  int recurse) {
    // compare the level
    if (fp1->frame_level != fp2->frame_level) {
        return EV_DIFF;
    }

    // compare the loc_file
    if (fp1->frame_loc.loc_file == NULL || fp2->frame_loc.loc_file == NULL) {
        if (fp1->frame_loc.loc_file != NULL ||
            fp2->frame_loc.loc_file != NULL) {
            // This checks if only one is null, in which case they are different
            return EV_DIFF;
        }
    } else {
        // compare the file directly
        if (strcmp(fp1->frame_loc.loc_file, fp2->frame_loc.loc_file)) {
            return EV_DIFF;
        }
    }

    // compare the loc_func
    if (fp1->frame_loc.loc_func == NULL || fp2->frame_loc.loc_func == NULL) {
        if (fp1->frame_loc.loc_func != NULL ||
            fp2->frame_loc.loc_func != NULL) {
            // This checks if only one is null, in which case they are different
            return EV_DIFF;
        }
    } else {
        // compare the func directly
        if (strcmp(fp1->frame_loc.loc_func, fp2->frame_loc.loc_func)) {
            return EV_DIFF;
        }
    }

    // compare the loc_addr
    if (fp1->frame_loc.loc_addr == NULL || fp2->frame_loc.loc_addr == NULL) {
        if (fp1->frame_loc.loc_addr != NULL ||
            fp2->frame_loc.loc_addr != NULL) {
            // This checks if only one is null, in which case they are different
            return EV_DIFF;
        }
    } else {
        // compare the addr directly
        if (strcmp(fp1->frame_loc.loc_addr, fp2->frame_loc.loc_addr)) {
            return EV_DIFF;
        }
    }

    // compare the loc_line
    if (fp1->frame_loc.loc_line != fp2->frame_loc.loc_line) {
        return EV_DIFF;
    }

    // Check to see if recurse is set, if so we might need to continue checking
    // the frames.
    if (recurse) {
        // Check to see if both have a frame_next
        if (fp1->frame_next == NULL && fp2->frame_next == NULL) {
            // there is no frame_next, so return equal
            return EV_EQUAL;
        } else if (fp1->frame_next != NULL || fp2->frame_next != NULL) {
            // One has a frame_next, and the other doesn't. They are different.
            return EV_DIFF;
        }

        // Both have a frame_next
        return CompareEvFrame(fp1->frame_next, fp2->frame_next, 1);
    }

    // If we get here everything matches, so return equal.
    return EV_EQUAL;
}

static event_equiv CompareEvErr(dbgerr_t e1, dbgerr_t e2) {
    // Make sure the errno matches
    if (e1.err_errno != e2.err_errno) {
        return EV_DIFF;
    }

    // Make sure the error string matches
    if (e1.err_str == NULL || e2.err_str == NULL) {
        // If one has an error string and the other doesn't they
        // don't match
        if (e1.err_str != NULL || e2.err_str != NULL) {
            return EV_DIFF;
        }
    } else {
        // compare the error string directly
        if (strcmp(e1.err_str, e2.err_str)) {
            return EV_DIFF;
        }
    }

    // If we get here everything matches, so return equal.
    return EV_EQUAL;
}

event_equiv CompareEvents(dbgevent_t *ev1, dbgevent_t *ev2) {
    // First ensure the two events have the same type
    if (ev1->ev_event != ev2->ev_event) {
        return EV_DIFF;
    }

    // FIXME: This does not check for every event type. Right now its only
    // handling asynch events. This might need to be expanded if it starts
    // being called elsewhere.

    // Parse down into the actual event to test for equivalence.
    switch (ev1->ev_event) {
        case DBGEV_EXIT:
            return CompareEvExit(ev1->ev_exit, ev2->ev_exit);

        case DBGEV_BPHIT:
            return CompareEvBp(ev1->ev_bp, ev2->ev_bp);

        case DBGEV_SIGNAL:
            return CompareEvSig(ev1->ev_sig, ev2->ev_sig);

        case DBGEV_STEP:
            return CompareEvStep(ev1->ev_step, ev2->ev_step);

        case DBGEV_ERROR:
            return CompareEvErr(ev1->ev_err, ev2->ev_err);

        case DBGEV_OK:
            // always match
            return EV_EQUAL;

        default:
            // assume it matches by default
            PrintOutError(
                "CompareEvents: No compare function defined. File a bug "
                "report!\n");
            return EV_EQUAL;
    }

    // If we made it here, the two events are equivalent.
    return EV_EQUAL;
}

void FreeEvent(dbgevent_t *event) {
    if (event == (dbgevent_t *)NULL) return;

    // Check to see if this event should save its data
    if (event->save_ev_data) {
        Free(event);
        return;
    }

    switch (event->ev_event) {
        // generic strings go here
        case DBGEV_SOURCEPATH:
        case DBGEV_TYPE:
        case DBGEV_CMD:
        case DBGEV_THREADINFO:
            Free(event->ev_string);
            break;

        case DBGEV_BPHIT:
        case DBGEV_BPINFO:
        case DBGEV_BPSET:
        case DBGEV_BPDEL:
            FreeBP(event->ev_bp);
            break;

        case DBGEV_SIGNAL:
            Free(event->ev_sig_type);
            Free(event->ev_sig_line);
            FreeFrame(event->ev_sig_frame);
            break;

        case DBGEV_FRAME:
            if (event->ev_frame != (dbgframe_t *)NULL)
                FreeFrame(event->ev_frame);
            break;

        case DBGEV_STEP:
            Free(event->ev_step_line);
            FreeFrame(event->ev_step_frame);
            break;

        case DBGEV_STAT:
            FreeStatus(event->ev_status);
            break;

        case DBGEV_HALT:
        case DBGEV_LOC:
            FreeLocation(event->ev_loc);
            break;

        case DBGEV_VARS:
            FreeVars(event->ev_vars);
            break;

        case DBGEV_DATA:
        case DBGEV_DATA_SLICE:
            delete event->ev_data;
            break;

        case DBGEV_LIST:
            if (event->ev_list != (dbglist_t *)NULL) FreeSCList(event->ev_list);
            break;

        case DBGEV_ERROR:
            Free(event->ev_errstr);
            break;

        case DBGEV_VERS:
            Free(event->ev_version);
            Free(event->ev_arch);
            break;

        case DBGEV_SOURCES:
            FreeSources(event->ev_sources);
            break;

        // Anything that doesn't alloc should go here
        case DBGEV_EXIT:
        case DBGEV_OK:
        case DBGEV_NONE:
            break;
    }

    Free(event);
}

void EventErrStr(dbgevent_t *e, const char *fmt, ...) {
    va_list ap;

    if (fmt == NULL) {
        e->ev_errstr = NULL;
        return;
    }

    va_start(ap, fmt);
    vasprintf(&e->ev_errstr, fmt, ap);
    va_end(ap);
}

static dbgframe_t *DupFrame(dbgframe_t *frame) {
    dbgframe_t *f;
    dbgframe_t *fp = (dbgframe_t *)NULL;
    dbgframe_t **fe = &fp;

    for (; frame != (dbgframe_t *)NULL; frame = frame->frame_next) {
        f = *fe = (dbgframe_t *)Alloc(sizeof(dbgframe_t));
        f->frame_loc.loc_file = NULL;
        f->frame_loc.loc_func = NULL;
        f->frame_loc.loc_addr = NULL;

        f->frame_level = frame->frame_level;
        if (frame->frame_loc.loc_file != NULL)
            f->frame_loc.loc_file = strdup(frame->frame_loc.loc_file);
        if (frame->frame_loc.loc_addr != NULL)
            f->frame_loc.loc_addr = strdup(frame->frame_loc.loc_addr);
        if (frame->frame_loc.loc_func != NULL)
            f->frame_loc.loc_func = strdup(frame->frame_loc.loc_func);
        f->frame_loc.loc_line = frame->frame_loc.loc_line;
        f->frame_next = (dbgframe_t *)NULL;

        fe = &f->frame_next;
    }

    return fp;
}

static void FreeFrame(dbgframe_t *f) {
    dbgframe_t *fp;

    while (f != (dbgframe_t *)NULL) {
        fp = f->frame_next;

        if (f->frame_loc.loc_addr != NULL) Free(f->frame_loc.loc_addr);
        if (f->frame_loc.loc_func != NULL) Free(f->frame_loc.loc_func);
        if (f->frame_loc.loc_file != NULL) Free(f->frame_loc.loc_file);

        Free(f);

        f = fp;
    }
}

static dbgloc_t *DupLocation(dbgloc_t *l) {
    dbgloc_t *loc;

    if (l == NULL) return NULL;

    loc = (dbgloc_t *)Alloc(sizeof(dbgloc_t));
    if (l->loc_file != NULL) loc->loc_file = strdup(l->loc_file);
    if (l->loc_addr != NULL) loc->loc_addr = strdup(l->loc_addr);
    if (l->loc_func != NULL) loc->loc_func = strdup(l->loc_func);
    loc->loc_line = l->loc_line;

    return loc;
}

static void FreeLocation(dbgloc_t *l) {
    if (l == NULL) return;

    if (l->loc_file != NULL) Free(l->loc_file);
    if (l->loc_addr != NULL) Free(l->loc_addr);
    if (l->loc_func != NULL) Free(l->loc_func);

    Free(l);
}

static dbgvars_t *DupVars(dbgvars_t *vars) {
    dbgvars_t *v;
    dbgvars_t *vp = (dbgvars_t *)NULL;
    dbgvars_t **ve = &vp;

    for (; vars != (dbgvars_t *)NULL; vars = vars->var_next) {
        v = *ve = (dbgvars_t *)Alloc(sizeof(dbgvars_t));

        v->var_name = strdup(vars->var_name);
        v->var_type = strdup(vars->var_type);
        v->var_next = (dbgvars_t *)NULL;

        ve = &v->var_next;
    }

    return vp;
}

static void FreeVars(dbgvars_t *v) {
    dbgvars_t *vp;

    while (v != (dbgvars_t *)NULL) {
        vp = v->var_next;

        if (v->var_name != NULL) Free(v->var_name);
        if (v->var_type != NULL) Free(v->var_type);

        Free(v);

        v = vp;
    }
}

static dbglist_t *DupSCList(dbglist_t *list) {
    dbglist_t *l;
    dbglist_t *lp = (dbglist_t *)NULL;
    dbglist_t **le = &lp;

    for (; list != (dbglist_t *)NULL; list = list->list_next) {
        l = *le = (dbglist_t *)Alloc(sizeof(dbglist_t));

        l->list_lno = list->list_lno;
        l->list_line = strdup(list->list_line);
        l->list_next = (dbglist_t *)NULL;

        le = &l->list_next;
    }

    return lp;
}

static void FreeSCList(dbglist_t *l) {
    dbglist_t *lp;

    while (l != (dbglist_t *)NULL) {
        lp = l->list_next;

        if (l->list_line != NULL) Free(l->list_line);

        Free(l);

        l = lp;
    }
}

static dbgsources_t *DupSources(dbgsources_t *sources) {
    dbgsources_t *s;
    dbgsources_t *sp = NULL;
    dbgsources_t **se = &sp;

    for (; sources != NULL; sources = sources->sources_next) {
        s = *se = (dbgsources_t *)Alloc(sizeof(dbgsources_t));

        s->sources_file = strdup(sources->sources_file);
        s->sources_next = NULL;

        se = &s->sources_next;
    }

    return sp;
}

static void FreeSources(dbgsources_t *s) {
    dbgsources_t *sp;

    while (s != NULL) {
        sp = s->sources_next;

        if (s->sources_file != NULL) Free(s->sources_file);

        Free(s);

        s = sp;
    }
}

static dbgstatus_t *DupStatus(dbgstatus_t *stat) {
    dbgstatus_t *s;

    if (stat == NULL) return NULL;

    s = (dbgstatus_t *)Alloc(sizeof(dbgstatus_t));

    s->stat = stat->stat;

    if (stat->stat_loc != NULL) {
        s->stat_loc = DupLocation(stat->stat_loc);
    } else {
        s->stat_loc = NULL;
    }

    return s;
}

static void FreeStatus(dbgstatus_t *stat) {
    if (stat == NULL) return;

    FreeLocation(stat->stat_loc);
    Free(stat);
}

/*
** Return last event error string.
*/
const char *DbgEventErrorStr(dbgevent_t *e) {
    static char buf[BUFSIZ];

    if (e == (dbgevent_t *)NULL || e->ev_event != DBGEV_ERROR) return "";

    if (e->ev_errno >= DBGERR_NULL)
        snprintf(buf, BUFSIZ - 1, "Unknown error %d >= DBGERR_NULL occurred.",
                 e->ev_errno);
    else
        snprintf(buf, BUFSIZ - 1, DbgErrTab[e->ev_errno], e->ev_errstr);

    return buf;
}

void PrintDbgError(dbgerr dbg_errno) {
    switch (dbg_errno) {
        case DBG_OK:
            PrintOutMsg("DBG_OK");
            break;
        case DBGERR_NOTIMP:
            PrintOutMsg("DBGERR_NOTIMP");
            break;
        case DBGERR_PROTO:
            PrintOutMsg("DBGERR_PROTO");
            break;
        case DBGERR_DEBUGGER:
            PrintOutMsg("DBGERR_DEBUGGER");
            break;
        case DBGERR_SERVER:
            PrintOutMsg("DBGERR_SERVER");
            break;
        case DBGERR_CALLBACK:
            PrintOutMsg("DBGERR_CALLBACK");
            break;
        case DBGERR_INPROGRESS:
            PrintOutMsg("DBGERR_INPROGRESS");
            break;
        case DBGERR_CBCREATE:
            PrintOutMsg("DBGERR_CBCREATE");
            break;
        case DBGERR_NOSERVER:
            PrintOutMsg("DBGERR_NOSERVER");
            break;
        case DBGERR_RPC:
            PrintOutMsg("DBGERR_RPC");
            break;
        case DBGERR_NOLINE:
            PrintOutMsg("DBGERR_NOLINE");
            break;
        case DBGERR_NOFUNC:
            PrintOutMsg("DBGERR_NOFUNC");
            break;
        case DBGERR_NOFILE:
            PrintOutMsg("DBGERR_NOFILE");
            break;
        case DBGERR_NOBP:
            PrintOutMsg("DBGERR_NOBP");
            break;
        case DBGERR_NOSYM:
            PrintOutMsg("DBGERR_NOSYM");
            break;
        case DBGERR_NOMEM:
            PrintOutMsg("DBGERR_NOMEM");
            break;
        case DBGERR_CANTRUN:
            PrintOutMsg("DBGERR_CANTRUN");
            break;
        case DBGERR_INVOKE:
            PrintOutMsg("DBGERR_INVOKE");
            break;
        case DBGERR_ISRUNNING:
            PrintOutMsg("DBGERR_ISRUNNING");
            break;
        case DBGERR_NOTRUN:
            PrintOutMsg("DBGERR_NOTRUN");
            break;
        case DBGERR_FIRSTFRAME:
            PrintOutMsg("DBGERR_FIRSTFRAME");
            break;
        case DBGERR_LASTFRAME:
            PrintOutMsg("DBGERR_LASTFRAME");
            break;
        case DBGERR_BADBPARG:
            PrintOutMsg("DBGERR_BADBPARG");
            break;
        case DBGERR_REGEX:
            PrintOutMsg("DBGERR_REGEX");
            break;
        case DBGERR_NOSTACK:
            PrintOutMsg("DBGERR_NOSTACK");
            break;
        case DBGERR_OUTOFRANGE:
            PrintOutMsg("DBGERR_OUTOFRANGE");
            break;
        case DBGERR_NOFILEDIR:
            PrintOutMsg("DBGERR_NOFILE_DIR");
            break;
        case DBGERR_NOSYMS:
            PrintOutMsg("DBGERR_NOSYMS");
            break;
        case DBGERR_TEMP:
            PrintOutMsg("DBGERR_TEMP");
            break;
        case DBGERR_PIPE:
            PrintOutMsg("DBGERR_PIPE");
            break;
        case DBGERR_FORK:
            PrintOutMsg("DBGERR_FORK");
            break;
        case DBGERR_SELECT:
            PrintOutMsg("DBGERR_SELECT");
            break;
        case DBGERR_NOTEXEC:
            PrintOutMsg("DBGERR_NOTEXEC");
            break;
        case DBGERR_CHDIR:
            PrintOutMsg("DBGERR_CHDIR");
            break;
        case DBGERR_SOURCE:
            PrintOutMsg("DBGERR_SOURCE");
            break;
        case DBGERR_SETVAR:
            PrintOutMsg("DBGERR_SETVAR");
            break;
        case DBGERR_ATTACH_FAILED:
            PrintOutMsg("DBGERR_ATTACH_FAILED");
            break;
        case DBGERR_AT_EXIT:
            PrintOutMsg("DBGERR_AT_EXIT");
            break;
        case DBGERR_INTERNAL:
            PrintOutMsg("DBGERR_INTERNAL");
            break;
        case DBGERR_UNKNOWN:
            PrintOutMsg("DBGERR_UNKNOWN");
            break;
        default:
            PrintOutMsg("UNKNOWN ERROR");
    }
}

void PrintDbgEvent(dbgevent_t *event) {
    switch (event->ev_event) {
        case DBGEV_OK:
            PrintOutMsg("DBGEV_OK");
            break;
        case DBGEV_BPHIT:
            PrintOutMsg("DBGEV_BPHIT");
            break;
        case DBGEV_SIGNAL:
            PrintOutMsg("DBGEV_SIGNAL");
            break;
        case DBGEV_HALT:
            PrintOutMsg("DBGEV_HALT");
            break;
        case DBGEV_EXIT:
            PrintOutMsg("DBGEV_EXIT");
            break;
        case DBGEV_STEP:
            PrintOutMsg("DBGEV_STEP");
            break;
        case DBGEV_BPINFO:
            PrintOutMsg("DBGEV_BPINFO");
            break;
        case DBGEV_BPSET:
            PrintOutMsg("DBGEV_BPSET");
            break;
        case DBGEV_BPDEL:
            PrintOutMsg("DBGEV_BPDEL");
            break;
        case DBGEV_FRAME:
            PrintOutMsg("DBGEV_FRAME");
            break;
        case DBGEV_LOC:
            PrintOutMsg("DBGEV_LOC");
            break;
        case DBGEV_VARS:
            PrintOutMsg("DBGEV_VARS");
            break;
        case DBGEV_TYPE:
            PrintOutMsg("DBGEV_TYPE");
            break;
        case DBGEV_THREADINFO:
            PrintOutMsg("DBGEV_THREADINFO");
            break;
        case DBGEV_LIST:
            PrintOutMsg("DBGEV_LIST");
            break;
        case DBGEV_SOURCEPATH:
            PrintOutMsg("DBGEV_SOURCEPATH");
            break;
        case DBGEV_DATA:
            PrintOutMsg("DBGEV_DATA");
            break;
        case DBGEV_DATA_SLICE:
            PrintOutMsg("DBGEV_SLICE");
            break;
        case DBGEV_STAT:
            PrintOutMsg("DBGEV_STAT");
            break;
        case DBGEV_VERS:
            PrintOutMsg("DBGEV_VERS");
            break;
        case DBGEV_SOURCES:
            PrintOutMsg("DBGEV_SOURCES");
            break;
        case DBGEV_CMD:
            PrintOutMsg("DBGEV_CMD");
            break;
        case DBGEV_NONE:
            PrintOutMsg("DBGEV_NONE");
            break;
        case DBGEV_ERROR:
            PrintOutMsg("DBGEV_ERROR");
            break;
    }
}

const char *EventToStr(dbgevent_t *event) {
    switch (event->ev_event) {
        case DBGEV_OK:
            return "DBGEV_OK";
        case DBGEV_BPHIT:
            return "DBGEV_BPHIT";
        case DBGEV_SIGNAL:
            return "DBGEV_SIGNAL";
        case DBGEV_HALT:
            return "DBGEV_HALT";
        case DBGEV_EXIT:
            return "DBGEV_EXIT";
        case DBGEV_STEP:
            return "DBGEV_STEP";
        case DBGEV_BPINFO:
            return "DBGEV_BPINFO";
        case DBGEV_BPSET:
            return "DBGEV_BPSET";
        case DBGEV_BPDEL:
            return "DBGEV_BPDEL";
        case DBGEV_FRAME:
            return "DBGEV_FRAME";
        case DBGEV_LOC:
            return "DBGEV_LOC";
        case DBGEV_VARS:
            return "DBGEV_VARS";
        case DBGEV_TYPE:
            return "DBGEV_TYPE";
        case DBGEV_THREADINFO:
            return "DBGEV_THREADINFO";
        case DBGEV_LIST:
            return "DBGEV_LIST";
        case DBGEV_SOURCEPATH:
            return "DBGEV_SOURCEPATH";
        case DBGEV_DATA:
            return "DBGEV_DATA";
        case DBGEV_DATA_SLICE:
            return "DBGEV_DATA_SLICE";
        case DBGEV_STAT:
            return "DBGEV_STAT";
        case DBGEV_VERS:
            return "DBGEV_VERS";
        case DBGEV_SOURCES:
            return "DBGEV_SOURCES";
        case DBGEV_CMD:
            return "DBGEV_CMD";
        case DBGEV_NONE:
            return "DBGEV_NONE";
        case DBGEV_ERROR:
            return "DBGEV_ERROR";
    }

    return "UNKNOWN_EVENT";
}

const char *DbgevToStr(dbgev ev) {
    switch (ev) {
        case DBGEV_OK:
            return "DBGEV_OK";
        case DBGEV_BPHIT:
            return "DBGEV_BPHIT";
        case DBGEV_SIGNAL:
            return "DBGEV_SIGNAL";
        case DBGEV_HALT:
            return "DBGEV_HALT";
        case DBGEV_EXIT:
            return "DBGEV_EXIT";
        case DBGEV_STEP:
            return "DBGEV_STEP";
        case DBGEV_BPINFO:
            return "DBGEV_BPINFO";
        case DBGEV_BPSET:
            return "DBGEV_BPSET";
        case DBGEV_BPDEL:
            return "DBGEV_BPDEL";
        case DBGEV_FRAME:
            return "DBGEV_FRAME";
        case DBGEV_LOC:
            return "DBGEV_LOC";
        case DBGEV_VARS:
            return "DBGEV_VARS";
        case DBGEV_TYPE:
            return "DBGEV_TYPE";
        case DBGEV_THREADINFO:
            return "DBGEV_THREADINFO";
        case DBGEV_LIST:
            return "DBGEV_LIST";
        case DBGEV_SOURCEPATH:
            return "DBGEV_SOURCEPATH";
        case DBGEV_DATA:
            return "DBGEV_DATA";
        case DBGEV_DATA_SLICE:
            return "DBGEV_DATA_SLICE";
        case DBGEV_STAT:
            return "DBGEV_STAT";
        case DBGEV_VERS:
            return "DBGEV_VERS";
        case DBGEV_SOURCES:
            return "DBGEV_SOURCES";
        case DBGEV_CMD:
            return "DBGEV_CMD";
        case DBGEV_NONE:
            return "DBGEV_NONE";
        case DBGEV_ERROR:
            return "DBGEV_ERROR";
    }

    return "UNKNOWN_EVENT";
}
