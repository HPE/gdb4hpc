/*********************************************************************************\
 * Event defines
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

/*
** How events work:
**
** Synchronous events can have their own type. If there is nothing to return,
** the event DBGEV_OK should be used. If the event returns data it can use the
** generic event data of ev_u_string and ev_u_int, just make sure to define its
** name in the defines. For more complex return types create their own typedef
** and type.
**
** Asynchronous events should never return the generic DBGEV_OK. The type should
** always speak to something unique. For its data same rules apply as above.
**
*/

#ifndef _DBGEVENT_H
#define _DBGEVENT_H

#include <stdarg.h>

#include "AIFStream.h"
#include "dbgbp.h"
#include "dbgloc.h"
#include "list.h"
#include <optional>

#define EVENT_ERROR(e, err, ...) \
    (e) = NewEvent(DBGEV_ERROR); \
    (e)->ev_errno = (err);       \
    EventErrStr((e), __VA_ARGS__);

enum event_equiv { EV_EQUAL, EV_DIFF };
typedef enum event_equiv event_equiv;

enum dbgstat {
    DBGSTAT_INITIALISING,
    DBGSTAT_WAITING,
    DBGSTAT_RUNNING,
    DBGSTAT_STOPPED,
    DBGSTAT_STOPPED_IN_THREAD,  // non-stopped mode: some threads are running
    DBGSTAT_EXIT,
    DBGSTAT_DBGERR,
    DBGSTAT_INTERR
};
typedef enum dbgstat dbgstat;

enum dbgerr {
    DBG_OK,
    DBGERR_NOTIMP,
    DBGERR_PROTO,
    DBGERR_DEBUGGER,
    DBGERR_SERVER,
    DBGERR_CALLBACK,
    DBGERR_INPROGRESS,
    DBGERR_CBCREATE,
    DBGERR_NOSERVER,
    DBGERR_RPC,
    DBGERR_NOLINE,
    DBGERR_NOFUNC,
    DBGERR_NOFILE,
    DBGERR_NOBP,
    DBGERR_NOSYM,
    DBGERR_OPT,
    DBGERR_BADTYPE,
    DBGERR_NOMEM,
    DBGERR_CANTRUN,
    DBGERR_INVOKE,
    DBGERR_ISRUNNING,
    DBGERR_NOTRUN,
    DBGERR_FIRSTFRAME,
    DBGERR_LASTFRAME,
    DBGERR_BADBPARG,
    DBGERR_REGEX,
    DBGERR_NOSTACK,
    DBGERR_OUTOFRANGE,
    DBGERR_NOFILEDIR,
    DBGERR_NOSYMS,
    DBGERR_TEMP,
    DBGERR_PIPE,
    DBGERR_FORK,
    DBGERR_SELECT,
    DBGERR_NOTEXEC,
    DBGERR_CHDIR,
    DBGERR_SOURCE,
    DBGERR_SETVAR,
    DBGERR_ATTACH_FAILED,
    DBGERR_AT_EXIT,
    DBGERR_INTERNAL,
    DBGERR_UNKNOWN,
    // DBGERR_NULL is used to determine the size of this enum. It should only
    // be used for checks and never to set a dbgerr. This should always be the
    // final entry in the enumeration.
    DBGERR_NULL
};
typedef enum dbgerr dbgerr;

// If a new event is needed, make sure to modify the functions inside dbgevent.c
// and create the proper format conversion in format.c
enum dbgev {
    DBGEV_OK,
    DBGEV_BPHIT,
    DBGEV_SIGNAL,
    DBGEV_HALT,
    DBGEV_EXIT,
    DBGEV_STEP,
    DBGEV_BPINFO,
    DBGEV_BPSET,
    DBGEV_BPDEL,
    DBGEV_FRAME,
    DBGEV_LOC,
    DBGEV_TYPE,
    DBGEV_VARS,
    DBGEV_LIST,
    DBGEV_SOURCEPATH,
    DBGEV_DATA,
    DBGEV_STAT,
    DBGEV_VERS,
    DBGEV_SOURCES,
    DBGEV_CMD,
    DBGEV_NONE,
    DBGEV_ERROR,
    DBGEV_DATA_SLICE,
    DBGEV_THREADINFO
};
typedef enum dbgev dbgev;

struct dbgvars_t {
    char *var_name;
    char *var_type;
    struct dbgvars_t *var_next;
};
typedef struct dbgvars_t dbgvars_t;

struct dbglist_t {
    int list_lno;
    char *list_line;
    char *list_threads;
    struct dbglist_t *list_next;
};
typedef struct dbglist_t dbglist_t;

class dbgframe_t {
private:
    int m_level;                // depth of the frame
    dbgloc_t m_loc;             // source file location
    dbgframe_t* m_next;         // frame of the calling function
    char* m_threads;            // threads (only if called during thread focus, and only top initial frame)
    dbgframe_t* m_nextThreadFrame;  // (initial frame only) - frame list for the next thread set
public:
    int getLevel() const { return m_level; }
    const dbgloc_t& getLoc() const { return m_loc; }
    const char* getThreads() const { return m_threads; }

    const dbgframe_t* getNext() const { return m_next; }
    const dbgframe_t* getNextThreadFrame() const { return m_nextThreadFrame; }

    void setNext(dbgframe_t* next);
    void setNextThreadFrame(dbgframe_t* next);

    dbgframe_t(int level, const dbgloc_t& loc, dbgframe_t* next,
               const char* threads, dbgframe_t* nextThreadFrame);
    dbgframe_t* clone() const;
    static dbgframe_t* copy(const dbgframe_t* rhs) { return rhs ? rhs->clone() : nullptr; }
    ~dbgframe_t();

    dbgframe_t(const dbgframe_t&) = delete;
    dbgframe_t& operator=(const dbgframe_t&) = delete;
};

struct dbgsig_t {
    char *sig_type;
    char *sig_line;
    dbgframe_t *sig_frame;
};
typedef struct dbgsig_t dbgsig_t;

struct dbgerr_t {
    char *err_str;
    dbgerr err_errno;
};
typedef struct dbgerr_t dbgerr_t;

struct dbgvers_t {
    char *vers_version;
    char *vers_arch;
};
typedef struct dbgvers_t dbgvers_t;

struct dbgstep_t {
    int step_lno;
    char *step_line;
    dbgframe_t *step_frame;
};
typedef struct dbgstep_t dbgstep_t;

struct dbgsources_t {
    char *sources_file;
    struct dbgsources_t *sources_next;
};
typedef struct dbgsources_t dbgsources_t;

struct dbgstatus_t {
    dbgstat stat;
    dbgloc_t *stat_loc;
};
typedef struct dbgstatus_t dbgstatus_t;

struct dbgevent_t {
    dbgev ev_event;

    // Used to control if the event data should be free'ed or not when the event
    // is free'ed, sometimes we want the data to stick around and free it
    // elsewhere.
    int save_ev_data;
    union {
        // Generic string event data
        char *ev_u_string;
        // Generic int event data
        int ev_u_int;
        dbgbp_t *ev_u_bp;
        dbgsig_t ev_u_sig;
        dbgstep_t ev_u_step;
        AIFStream *ev_u_data;
        dbglist_t *ev_u_list;
        dbgframe_t *ev_u_frame;
        dbgloc_t *ev_u_loc;
        dbgvars_t *ev_u_vars;
        dbgstatus_t *ev_u_status;
        dbgerr_t ev_u_err;
        dbgvers_t ev_u_vers;
        dbgsources_t *ev_u_sources;
    } ev_u;
// These are generic and should not be used outside the format/event functions.
#define ev_string ev_u.ev_u_string
#define ev_int ev_u.ev_u_int
// These should be used everywhere
#define ev_bp ev_u.ev_u_bp
#define ev_step ev_u.ev_u_step
#define ev_step_lno ev_u.ev_u_step.step_lno
#define ev_step_line ev_u.ev_u_step.step_line
#define ev_step_frame ev_u.ev_u_step.step_frame
#define ev_sig ev_u.ev_u_sig
#define ev_sig_type ev_u.ev_u_sig.sig_type
#define ev_sig_line ev_u.ev_u_sig.sig_line
#define ev_sig_frame ev_u.ev_u_sig.sig_frame
#define ev_exit ev_u.ev_u_int
#define ev_frame ev_u.ev_u_frame
#define ev_data ev_u.ev_u_data
#define ev_list ev_u.ev_u_list
#define ev_source_path ev_u.ev_u_string
#define ev_loc ev_u.ev_u_loc
#define ev_type ev_u.ev_u_string
#define ev_vars ev_u.ev_u_vars
#define ev_status ev_u.ev_u_status
#define ev_err ev_u.ev_u_err
#define ev_errno ev_u.ev_u_err.err_errno
#define ev_errstr ev_u.ev_u_err.err_str
#define ev_vers ev_u.ev_u_vers
#define ev_version ev_u.ev_u_vers.vers_version
#define ev_arch ev_u.ev_u_vers.vers_arch
#define ev_sources ev_u.ev_u_sources
#define ev_cmd ev_u.ev_u_string

#define EVENT_EV(e) (e)->ev_event
#define EVENT_BP(e) (e)->ev_u.ev_u_bp
#define EVENT_FRAME(e) (e)->ev_u.ev_u_frame
#define EVENT_DATA(e) (e)->ev_u.ev_u_data
#define EVENT_LIST(e) (e)->ev_u.ev_u_list
#define EVENT_SOURCE_PATH(e) (e)->ev_u.ev_u_string
#define EVENT_LOC(e) (e)->ev_u.ev_u_loc
#define EVENT_TYPE(e) (e)->ev_u.ev_u_string
#define EVENT_VARS(e) (e)->ev_u.ev_u_vars
#define EVENT_STATUS(e) (e)->ev_u.ev_u_status
#define EVENT_ERR(e) (e)->ev_u.ev_u_err
#define EVENT_SOURCES(e) (e)->ev_u.ev_u_sources
#define EVENT_CMD(e) (e)->ev_u.ev_u_string
};
typedef struct dbgevent_t dbgevent_t;

extern dbgevent_t *NewEvent(dbgev);
extern dbgevent_t *ChangeEvent(dbgevent_t *, dbgev);
extern dbgevent_t *DupEvent(dbgevent_t *);
extern void FreeEvent(dbgevent_t *);
extern void EventErrStr(dbgevent_t *, const char *, ...);
extern event_equiv CompareEvents(dbgevent_t *, dbgevent_t *);

// Used for error events
extern const char *DbgEventErrorStr(dbgevent_t *);

// Used for debug
extern dbgevent_t* NewErrorEvent(dbgerr err, const char* fmt, ...);
extern void PrintDbgEvent(dbgevent_t *);
extern const char *EventToStr(dbgevent_t *);
extern const char *DbgevToStr(dbgev);
extern void PrintDbgError(dbgerr);

constexpr const char* MULTI_THREAD_HDR = "Thread Results\n";

// Parser class for processing a THREAD_INFO result into its components.
// Usage as:
// for (ThreadMsgParser parser(std::move(msg); auto msg = parser.nextResult(); ) {
//      auto [thread, val] = msg;
class ThreadMsgParser {
private:
    std::string m_str;
    std::string m_thread;
public:
    ThreadMsgParser(const std::string& msg) : m_str(msg) {}
    ThreadMsgParser(std::string&& msg) : m_str(std::move(msg)) {}

    std::optional<std::pair<std::string,std::string>> nextResult();

    ThreadMsgParser(const ThreadMsgParser&) = delete;
    ThreadMsgParser& operator=(const ThreadMsgParser&) = delete;
};

#endif /* !_DBGEVENT_H */
