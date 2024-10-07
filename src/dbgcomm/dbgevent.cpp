/*********************************************************************************\
 * Event routines
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * Copyright 2010-2013,2020-2023 Hewlett Packard Enterprise Development LP.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Hewlett Packard Enterprise Development LP.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *
 * $HeadURL$
 * $Date$
 * $Rev$
 * $Author$
 *
 *********************************************************************************/

#include "config.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "dbg.h"
#include "defs.h"
#include "useful.h"
#include <regex>

#include "unittest_macros.h"

// Static prototypes
static event_equiv CompareEvExit(int, int);
static event_equiv CompareEvSig(dbgsig_t, dbgsig_t);
static event_equiv CompareEvFrame(const dbgframe_t *, const dbgframe_t *, int);
static event_equiv CompareEvErr(dbgerr_t, dbgerr_t);

static dbgvars_t *DupVars(dbgvars_t *);
static void FreeVars(dbgvars_t *);
static dbglist_t *DupSCList(dbglist_t *);
static void FreeSCList(dbglist_t *);
static dbgsources_t *DupSources(dbgsources_t *);
static void FreeSources(dbgsources_t *);
static dbgstatus_t *DupStatus(dbgstatus_t *);
static void FreeStatus(dbgstatus_t *);

// Used for error events
constexpr auto DbgErrTab = []() -> std::array<const char*, DBGERR_NULL + 1> {
    // DBGERR_NULL is the last entry in the dbgerr enum, and has an entry
    auto table = std::array<const char*, DBGERR_NULL + 1>();
    for (auto& item : table) item = nullptr;

    table[DBG_OK]                     = "*** command succeeded";
    table[DBGERR_NOTIMP]              = "Protocol not implemented";
    table[DBGERR_PROTO]               = "No such protocol \"%s\"";
    table[DBGERR_DEBUGGER]            = "Debugger error: %s";
    table[DBGERR_SERVER]              = "No server for this process";
    table[DBGERR_CALLBACK]            = "Must register callback first";
    table[DBGERR_INPROGRESS]          = "Callback in progress";
    table[DBGERR_CBCREATE]            = "No server to register callback";
    table[DBGERR_NOSERVER]            = "Could not create callback";
    table[DBGERR_RPC]                 = "Server RPC error";
    table[DBGERR_NOLINE]              = "No line %s in file ";
    table[DBGERR_NOFUNC]              = "Function \"%s\" not defined";
    table[DBGERR_NOFILE]              = "No source file named \"%s\"";
    table[DBGERR_NOBP]                = "No breakpoint number %s";
    table[DBGERR_NOSYM]               = "No symbol \"%s\" in current context";
    table[DBGERR_OPT]                 = "Value optimized out";
    table[DBGERR_BADTYPE]             = "Unsupported symbol type";
    table[DBGERR_NOMEM]               = "*** can't access memory";
    table[DBGERR_CANTRUN]             = "*** can't run the application";
    table[DBGERR_INVOKE]              = "*** could not invoke the application";
    table[DBGERR_ISRUNNING]           = "*** The application is running";
    table[DBGERR_NOTRUN]              = "*** The application is not running";
    table[DBGERR_FIRSTFRAME]          = "Initial frame selected; you cannot go up.";
    table[DBGERR_LASTFRAME]           = "Bottom (innermost) frame selected; you cannot go down.";
    table[DBGERR_BADBPARG]            = "Argument required (breakpoint number)";
    table[DBGERR_REGEX]               = "*** error in regular expression";
    table[DBGERR_NOSTACK]             = "No stack";
    table[DBGERR_OUTOFRANGE]          = "Line number %s is out of range for ";
    table[DBGERR_NOFILEDIR]           = "%s: No such file or directory";
    table[DBGERR_NOSYMS]              = "No debugging symbols";
    table[DBGERR_TEMP]                = "Could not create temporary file";
    table[DBGERR_PIPE]                = "pipe: %s";
    table[DBGERR_FORK]                = "fork: %s";
    table[DBGERR_SELECT]              = "select: %s";
    table[DBGERR_NOTEXEC]             = "%s: not in executable format";
    table[DBGERR_CHDIR]               = "chdir: %s";
    table[DBGERR_SOURCE]              = "Source file is more recent than executable";
    table[DBGERR_SETVAR]              = "Can't set variable";
    table[DBGERR_ATTACH_FAILED]       = "Attach failed"; // FIXME: This is never used yet.
    table[DBGERR_AT_EXIT]             = "*** The application is at an exit breakpoint.";
    table[DBGERR_INTERNAL]            = "Internal error: %s";
    table[DBGERR_OPERATION_ABANDONED] = "Operation abandoned (%s), results discarded.";
    table[DBGERR_UNKNOWN]             = "Unkown GDB error: \"%s\"";
    table[DBGERR_NULL]                = "Invalid error number";

    return table;
}();

TEST_CASE("DbgErrTab is fully filled out") {
    for (size_t i = 0; i < DbgErrTab.size(); i++) {
        CHECK_MESSAGE(DbgErrTab[i] != nullptr, "Index ", i, " of DbgErrTab is nullptr");
    }
}

TEST_CASE("dbgframe_t::clone") {
    // thread a
    auto loc0a = dbgloc(strdup("dbgformat.cpp"), strdup("FreeEvent"), strdup("0xaaaa"), 180);
    auto loc1a = dbgloc(strdup("dbgformat.cpp"), strdup("FreeBp"), strdup("0xbbbb") ,200);
    auto loc2a = dbgloc(strdup("dbgformat.cpp"), strdup("EventToSocProto"), strdup("0xcccc"), 60);
    // thread b
    auto loc0b = dbgloc(strdup("dbgevent.cpp"), strdup("FreeEvent"), strdup("0xdddd"), 20);
    auto loc1b = dbgloc(strdup("dbgevent.cpp"), strdup("FreeBp"), strdup("0xeeee"), 210);
    auto loc2b = dbgloc(strdup("dbgevent.cpp"), strdup("EventToSocProto"), strdup("0xffff"), 90);

    auto frame2b = new dbgframe_t(2, loc2b, nullptr, "thread b", nullptr);
    auto frame1b = new dbgframe_t(1, loc1b, frame2b, "thread b", nullptr);
    auto frame0b = new dbgframe_t(0, loc0b, frame1b, "thread b", nullptr);

    auto frame2a = new dbgframe_t(2, loc2a, nullptr, "thread a", nullptr);
    auto frame1a = new dbgframe_t(1, loc1a, frame2a, "thread a", nullptr);
    auto frame0a = new dbgframe_t(0, loc0a, frame1a, "thread a", frame0b);

    auto sameFrame = [](const dbgframe_t& l, const dbgframe_t& r) {
        auto str = [](const char* s) { return s ? std::string(s) : ""; };
        auto* lthf = &l;
        auto* rthf = &r;
        for (; lthf && rthf; lthf=lthf->getNextThreadFrame(), rthf=rthf->getNextThreadFrame()) {
            auto* lf = lthf;
            auto* rf = rthf;
            for (; lf && rf; lf = lf->getNext(), rf = rf->getNext()) {
                const auto &lloc = lf->getLoc();
                const auto lfile = str(lloc.loc_file);
                const auto lfunc = str(lloc.loc_func);
                const auto laddr = str(lloc.loc_addr);
                const auto lthreads = str(lf->getThreads());

                const auto &rloc = rf->getLoc();
                const auto rfile = str(rloc.loc_file);
                const auto rfunc = str(rloc.loc_func);
                const auto raddr = str(rloc.loc_addr);
                const auto rthreads = str(rf->getThreads());

                CHECK(lfile == rfile);
                CHECK(lfunc == rfunc);
                CHECK(laddr == raddr);
                CHECK(lthreads == rthreads);
            }
            CHECK_MESSAGE(lf == nullptr, "Check that backtraces have same number of frames");
            CHECK_MESSAGE(rf == nullptr, "Check that backtraces have same number of frames");
        }
        CHECK_MESSAGE(lthf == nullptr, "Check that frames have same number of threads");
        CHECK_MESSAGE(rthf == nullptr, "Check that frames have same number of threads");
    };

    auto* clone = frame0a->clone();
    sameFrame(*frame0a, *clone);
    delete clone;
    delete frame0a;
}

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

dbgevent_t *DupEvent(const dbgevent_t *event) {
    dbgevent_t *e;

    if (event == (dbgevent_t *)NULL) return (dbgevent_t *)NULL;

    e = (dbgevent_t *)Alloc(sizeof(dbgevent_t));

    switch (e->ev_event = event->ev_event) {
        // Generic strings go here
        case DBGEV_SOURCEPATH:
        case DBGEV_TYPE:
        case DBGEV_CMD:
        case DBGEV_THREADINFO:
        case DBGEV_STRING:
            e->ev_string = strdup(event->ev_string);
            break;

        // Generic int go here
        case DBGEV_EXIT:
            e->ev_int = event->ev_int;
            break;

        case DBGEV_BPHIT:
        case DBGEV_BPINFO:
        case DBGEV_BPSET:
        case DBGEV_BPDEL:
            e->ev_bp = event->ev_bp->clone();
            break;

        case DBGEV_SIGNAL:
            e->ev_sig_type = strdup(event->ev_sig_type);
            e->ev_sig_line = strdup(event->ev_sig_line);
            e->ev_sig_frame = dbgframe_t::copy(event->ev_sig_frame);
            break;

        case DBGEV_STEP:
            e->ev_step.step_frame = dbgframe_t::copy(event->ev_step.step_frame);
            e->ev_step.step_frame_changed = event->ev_step.step_frame_changed;
            e->ev_step.step_threadId = event->ev_step.step_threadId;
            break;

        case DBGEV_FRAME:
            e->ev_frame = dbgframe_t::copy(event->ev_frame);
            break;

        case DBGEV_CHECKPOINT: {
            e->ev_checkpoint = 
                event->ev_checkpoint ? new dbgcheckpoint_t{*event->ev_checkpoint} : nullptr;
            break;
        }

        case DBGEV_VARS:
            e->ev_vars = DupVars(event->ev_vars);
            break;

        case DBGEV_HALT:
        case DBGEV_LOC:
            e->ev_loc = DupLocation(event->ev_loc);
            break;

        case DBGEV_DATA:
        case DBGEV_DATA_CHUNK:
            e->ev_data = (event->ev_data ? event->ev_data->clone() : nullptr);
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

        case DBGEV_SYMBOLS:
            e->ev_u.ev_u_symbols = event->ev_u.ev_u_symbols;
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

static event_equiv CompareEvFrame(const dbgframe_t *fp1, const dbgframe_t *fp2,
                                  int recurse) {
    // compare the level
    if (fp1->getLevel() != fp2->getLevel()) {
        return EV_DIFF;
    }

    // compare the loc_file
    const auto& loc1 = fp1->getLoc(), &loc2 = fp2->getLoc();
    
    if (loc1.loc_file == NULL || loc2.loc_file == NULL) {
        if (loc1.loc_file != NULL ||
            loc2.loc_file != NULL) {
            // This checks if only one is null, in which case they are different
            return EV_DIFF;
        }
    } else {
        // compare the file directly
        if (strcmp(loc1.loc_file, loc2.loc_file)) {
            return EV_DIFF;
        }
    }

    // compare the loc_func
    if (loc1.loc_func == NULL || loc2.loc_func == NULL) {
        if (loc1.loc_func != NULL ||
            loc2.loc_func != NULL) {
            // This checks if only one is null, in which case they are different
            return EV_DIFF;
        }
    } else {
        // compare the func directly
        if (strcmp(loc1.loc_func, loc2.loc_func)) {
            return EV_DIFF;
        }
    }

    // compare the loc_addr
    if (loc1.loc_addr == NULL || loc2.loc_addr == NULL) {
        if (loc1.loc_addr != NULL ||
            loc2.loc_addr != NULL) {
            // This checks if only one is null, in which case they are different
            return EV_DIFF;
        }
    } else {
        // compare the addr directly
        if (strcmp(loc1.loc_addr, loc2.loc_addr)) {
            return EV_DIFF;
        }
    }

    // compare the loc_line
    if (loc1.loc_line != loc2.loc_line) {
        return EV_DIFF;
    }

    // Check to see if recurse is set, if so we might need to continue checking
    // the frames.
    if (recurse) {
        // Check to see if both have a frame_next
        if (fp1->getNext() == NULL && fp2->getNext() == NULL) {
            // there is no frame_next, so return equal
            return EV_EQUAL;
        } else if (fp1->getNext() != NULL || fp2->getNext() != NULL) {
            // One has a frame_next, and the other doesn't. They are different.
            return EV_DIFF;
        }

        // Both have a frame_next
        return CompareEvFrame(fp1->getNext(), fp2->getNext(), 1);
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

event_equiv CompareEvents(const dbgevent_t *ev1, const dbgevent_t *ev2) {
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

        case DBGEV_SIGNAL:
            return CompareEvSig(ev1->ev_sig, ev2->ev_sig);

        case DBGEV_STEP:
            return CompareEvFrame(ev1->ev_step.step_frame, ev2->ev_step.step_frame, 0);

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
        case DBGEV_STRING:
            Free(event->ev_string);
            break;

        case DBGEV_BPHIT:
        case DBGEV_BPINFO:
        case DBGEV_BPSET:
        case DBGEV_BPDEL:
            delete event->ev_bp;
            break;

        case DBGEV_SIGNAL:
            Free(event->ev_sig_type);
            Free(event->ev_sig_line);
            delete event->ev_sig_frame;
            break;

        case DBGEV_CHECKPOINT:
            delete event->ev_checkpoint;
            break;

        case DBGEV_FRAME:
            delete event->ev_frame;
            break;

        case DBGEV_STEP:
            delete event->ev_step.step_frame;
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
        case DBGEV_DATA_CHUNK:
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

        case DBGEV_SYMBOLS:
            delete event->ev_u.ev_u_symbols;
            break;

        // Anything that doesn't alloc should go here
        case DBGEV_EXIT:
        case DBGEV_OK:
        case DBGEV_NONE:
            break;
    }

    Free(event);
}

void EventErrStr(dbgevent_t *e, const char *fmt, va_list ap) {
    if (fmt == NULL) {
        e->ev_errstr = NULL;
        return;
    }
    vasprintf(&e->ev_errstr, fmt, ap);
}

void EventErrStr(dbgevent_t *e, const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    EventErrStr(e, fmt, ap);
    va_end(ap);
}

dbgframe_t::dbgframe_t(const dbgframe_t& other)
    : dbgframe_t(other.m_level, other.m_loc,
                 other.m_next ? other.m_next->clone() : nullptr,
                 other.m_threads,
                 other.m_nextThreadFrame ? other.m_nextThreadFrame->clone() : nullptr)
{}

dbgframe_t *dbgframe_t::clone() const {
    return new dbgframe_t(*this);
}

dbgframe_t::dbgframe_t(int level, const dbgloc_t& loc, dbgframe_t* next,
                       const char* threads, dbgframe_t* nextThreadFrame)
    : m_level(level), m_next(next), m_threads(StrdupOpt(threads)),
      m_nextThreadFrame(nextThreadFrame)
{
    // dealing with loc directly - it's still POD
    m_loc.loc_file = StrdupOpt(loc.loc_file);
    m_loc.loc_addr = StrdupOpt(loc.loc_addr);
    m_loc.loc_func = StrdupOpt(loc.loc_func);
    m_loc.loc_line = loc.loc_line;
}

dbgframe_t::dbgframe_t(dbgframe_t &&other) {
    m_level = other.m_level;
    m_loc = std::move(other.m_loc);

    m_next = nullptr;
    std::swap(other.m_next, m_next);

    m_threads = nullptr;
    std::swap(other.m_threads, m_threads);

    m_nextThreadFrame = nullptr;
    std::swap(other.m_nextThreadFrame, m_nextThreadFrame);
}

dbgframe_t::~dbgframe_t() {
    delete m_next;
    delete m_nextThreadFrame;
    free(m_threads);
}

void dbgframe_t::clearAddress() {
    if (m_loc.loc_addr) {
        ::free(m_loc.loc_addr);
        m_loc.loc_addr = nullptr;
    }
}

void dbgframe_t::setNext(dbgframe_t* next) {
    if (next != m_next) {
        std::swap(next, m_next);
        delete next;
    }
}

void dbgframe_t::setNextThreadFrame(dbgframe_t* next) {
    if (next != m_nextThreadFrame) {
        std::swap(next, m_nextThreadFrame);
        delete next;
    }
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
        l->list_threads = StrdupOpt(list->list_threads);
        l->list_next = (dbglist_t *)NULL;

        le = &l->list_next;
    }

    return lp;
}

static void FreeSCList(dbglist_t *l) {
    dbglist_t *lp;

    while (l != (dbglist_t *)NULL) {
        lp = l->list_next;

        free(l->list_line);
        free(l->list_threads);
        free(l);

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
        case DBGERR_OPERATION_ABANDONED:
            PrintOutMsg("DBGERR_OPERATION_ABANDONED");
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
        case DBGEV_CHECKPOINT:
            PrintOutMsg("DBGEV_CHECKPOINT");
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
        case DBGEV_STRING:
            PrintOutMsg("DBGEV_STRING");
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
        case DBGEV_DATA_CHUNK:
            PrintOutMsg("DBGEV_CHUNK");
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
        case DBGEV_SYMBOLS:
            PrintOutMsg("DBGEV_SYMBOLS");
            break;
        case DBGEV_NONE:
            PrintOutMsg("DBGEV_NONE");
            break;
        case DBGEV_ERROR:
            PrintOutMsg("DBGEV_ERROR");
            break;
    }
}

const char *EventToStr(const dbgevent_t *event) {
    return DbgevToStr(event->ev_event);
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
        case DBGEV_CHECKPOINT:
            return "DBGEV_CHECKPOINT";
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
        case DBGEV_STRING:
            return "DBGEV_STRING";
        case DBGEV_LIST:
            return "DBGEV_LIST";
        case DBGEV_SOURCEPATH:
            return "DBGEV_SOURCEPATH";
        case DBGEV_DATA:
            return "DBGEV_DATA";
        case DBGEV_DATA_CHUNK:
            return "DBGEV_DATA_CHUNK";
        case DBGEV_STAT:
            return "DBGEV_STAT";
        case DBGEV_VERS:
            return "DBGEV_VERS";
        case DBGEV_SOURCES:
            return "DBGEV_SOURCES";
        case DBGEV_CMD:
            return "DBGEV_CMD";
        case DBGEV_SYMBOLS:
            return "DBGEV_SYMBOLS";
        case DBGEV_NONE:
            return "DBGEV_NONE";
        case DBGEV_ERROR:
            return "DBGEV_ERROR";
    }

    return "UNKNOWN_EVENT";
}

dbgevent_t* NewErrorEvent(dbgerr err, const char* fmt, ...) {
    auto e = NewEvent(DBGEV_ERROR);
    e->ev_errno = err;

    va_list ap;
    va_start(ap, fmt);
    EventErrStr(e, fmt, ap);
    va_end(ap);

    return e;
}

// get next thread,value pair from the string if any exist
std::optional<std::pair<std::string,std::string>> ThreadMsgParser::nextResult() {
    static auto threadHeadRE=std::regex{R"((^|\n)Thread\s+([0-9 ,-]+)(\s*\(AMDGPU Thread.*)?:\s*\n)"};

    std::optional<std::pair<std::string,std::string>> res;
    if (m_str.empty()) return res;

    auto match = std::smatch{};
    while (std::regex_search(m_str, match, threadHeadRE)) {
        if (!m_thread.empty()) {
            // we have a thread result, and the *next* thread id
            res = std::make_pair(m_thread, match.prefix().str());
            m_thread = match[2].str();
            m_str = match.suffix().str();
            return res;
        } else {
            // first thread, save the threads and keep looking
            m_thread = match[2].str();
            m_str = match.suffix().str();
        }
    }

    if (m_thread.empty()) return res;

    // the final result
    res = std::make_pair(m_thread, m_str);
    m_str.clear();
    return res;
}
