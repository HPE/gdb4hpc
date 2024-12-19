/*********************************************************************************\
 * Event defines
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
#include "sparse_bitset.h"
#include <optional>
#include <string>

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
    DBGERR_OPERATION_ABANDONED,
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
    DBGEV_CHECKPOINT,
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
    DBGEV_DATA_CHUNK,
    DBGEV_THREADINFO,
    DBGEV_PACKEDTHREADINFO,
    DBGEV_STRING,
    DBGEV_SYMBOLS,
    DBGEV_PYEXCEPTION,
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

    struct Arg {
        std::string name;
        // Maybe one day `value` can be an AIFStream
        std::string value;

        bool operator==(const Arg&) const = default;
    };

    std::vector<Arg> m_args;
public:
    int getLevel() const { return m_level; }
    const dbgloc_t& getLoc() const { return m_loc; }
    void clearAddress();
    const char* getThreads() const { return m_threads; }

    const dbgframe_t* getNext() const { return m_next; }
    const dbgframe_t* getNextThreadFrame() const { return m_nextThreadFrame; }

    // access the frame's list of function arguments
    const std::vector<Arg>& getArgs() const;
    void addArg(Arg&& arg);

    // replace the value field of all hexdecimal integer arguments (determined by regex)
    // with "<ptr>" for easier network aggregation
    void anonymizePointerArgs();

    // replace the value field of all decimal integer arguments (determined by regex)
    // with "<int>" for easier network aggregation
    void anonymizeIntArgs();

    void setNext(dbgframe_t* next);
    void setNextThreadFrame(dbgframe_t* next);

    // get a human readable string describing the location of the frame, e.g.
    // "[0xaaaa] in fn [(x=2,y=4)] at filename:XX", or other forms depending on what
    // information is available in the frame.
    //
    // set `with_args` to false to omit function arguments. (see CPE-6427 for why you might want to do that)
    // set `with_addr` to true to include the frame's address.
    std::string getLocationString(bool with_args, bool with_addr) const;

    dbgframe_t(int level, const dbgloc_t& loc, dbgframe_t* next,
               const char* threads, dbgframe_t* nextThreadFrame);
    dbgframe_t(dbgframe_t &&other);
    dbgframe_t* clone() const;
    static dbgframe_t* copy(const dbgframe_t* rhs) { return rhs ? rhs->clone() : nullptr; }
    ~dbgframe_t();

    explicit dbgframe_t(const dbgframe_t&);
    dbgframe_t& operator=(const dbgframe_t&) = delete;
};

// a location with an ID attached
struct dbgcheckpoint_t {
    unsigned long id;
    dbgloc_t location;
};

struct dbgstep_t {
    dbgframe_t *step_frame;
    int step_threadId;
    bool step_frame_changed; // true when a step event stepped into/out of a function
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

struct dbgsymbols_t {
    struct Symbol {
        std::string name;
    };

    std::vector<Symbol> debug;
    std::vector<Symbol> nondebug;
};

struct dbgpackedthreadinfo_t;

struct dbgthreadinfo_t {
    // a collection of threads and an indicator of the current thread.
    // i.e. the results of "info thread" command.
    //
    // all fields here might be omitted by gdbmi for arbitrary reasons. this even
    // applies to `current_thread` - e.g. when the current thread in a cuda program
    // is on a GPU (since "info threads" only returns CPU threads in a cuda program)
    // or when specific threads are queried e.g. "info thread 1-3".

    // has the same semantics as a copy ctor. (if we added an actual copy ctor, this
    // type would cease to be an aggregate type and we would lose the ability to
    // initialize it via aggregate initialization).
    dbgthreadinfo_t clone() const;

    struct Thread {
        // each field maps to the value returned by gdbmi.
        // see the "GDB/MI Thread Informtation" of gdbmi docs.
        // same arbitary omission warnings as dbgthreadinfo_t apply here.

        // has the same semantics as a copy ctor. (if we added an actual copy ctor, this
        // type would cease to be an aggregate type and we would lose the ability to
        // initialize it via aggregate initialization).
        Thread clone() const;

        int id;
        std::string target_id; // might be empty
        std::string details; // might be empty
        std::string name; // might be empty

        enum class State {
            Unspecified,
            Running,
            Stopped,
        };
        State state; // might be Unspecified

        int core; // might be < 0
        std::unique_ptr<dbgframe_t> frame; // might be nullptr

        // return a name appropriate for labeling this thread in a user interface.
        // since threads can have multiple names (.name, .details, .target_id, or none),
        // this function picks the "best" name and returns it. if none are present, it
        // will generate one.
        std::string getDisplayName() const;

        // remove some rank-specific information from the thread to make it
        // easier to aggregate with other ranks' threads in the network tree.
        void anonymize();

        // returns true if all fields match and the lowest level frames match.
        // i.e. this function doesn't consider the full backtrace of the .frame members.
        bool operator==(const Thread& other) const;
    };

    int current_thread; // might be <= 0
    std::vector<Thread> threads; // might be empty

    // pack threads into dbgpackedthreadinfo.
    // threads are merged based on thread names and frames.
    dbgpackedthreadinfo_t pack() const;
};

struct dbgpackedthreadinfo_t {
    // like dbgthreadinfo_t, but in a packed form with a smaller set of information.
    //
    // same arbitrary omission warnings as dbgthreadinfo_t apply here.

    // has the same semantics as a copy ctor. (if we added an actual copy ctor, this
    // type would cease to be an aggregate type and we would lose the ability to
    // initialize it via aggregate initialization).
    dbgpackedthreadinfo_t clone() const;

    struct PackedThread {
        // a PackedThread necessarily has less information than a Thread since
        // information was lost with packing.

        // has the same semantics as a copy ctor. (if we added an actual copy ctor, this
        // type would cease to be an aggregate type and we would lose the ability to
        // initialize it via aggregate initialization).
        PackedThread clone() const;

        // all ids with matching names and frames from the full thread set
        SparseBitset ids;
        std::string name; // might be empty
        std::unique_ptr<dbgframe_t> frame; // might be nullptr

        // returns true if all fields match and the lowest level frames match.
        // i.e. this function doesn't consider the full backtrace of the .frame members.
        bool operator==(const PackedThread& other) const;
    };

    int current_thread; // might be <= 0
    std::vector<PackedThread> threads;
};

struct dbgpyexception_t {
    // result of type(e).__name__
    std::string type_name;
    // result of str(e)
    std::string err_str;
    dbgframe_t* backtrace;
    // if true, python is about to exit
    bool fatal;
};

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
        AIFStream *ev_u_data;
        dbglist_t *ev_u_list;
        dbgframe_t *ev_u_frame;
        dbgcheckpoint_t *ev_u_checkpoint;
        dbgstep_t ev_u_step;
        dbgloc_t *ev_u_loc;
        dbgvars_t *ev_u_vars;
        dbgstatus_t *ev_u_status;
        dbgerr_t ev_u_err;
        dbgvers_t ev_u_vers;
        dbgsources_t *ev_u_sources;
        dbgsymbols_t *ev_u_symbols;
        dbgpackedthreadinfo_t *ev_u_packedthreadinfo;
        dbgthreadinfo_t *ev_u_threadinfo;
        dbgpyexception_t *ev_u_pyexception;
    } ev_u;
// These are generic and should not be used outside the format/event functions.
#define ev_string ev_u.ev_u_string
#define ev_int ev_u.ev_u_int
// These should be used everywhere
#define ev_bp ev_u.ev_u_bp
#define ev_sig ev_u.ev_u_sig
#define ev_sig_type ev_u.ev_u_sig.sig_type
#define ev_sig_line ev_u.ev_u_sig.sig_line
#define ev_sig_frame ev_u.ev_u_sig.sig_frame
#define ev_exit ev_u.ev_u_int
#define ev_frame ev_u.ev_u_frame
#define ev_checkpoint ev_u.ev_u_checkpoint
#define ev_step ev_u.ev_u_step
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
#define EVENT_CHECKPOINT(e) (e)->ev_u.ev_u_checkpoint
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
extern dbgevent_t *DupEvent(const dbgevent_t *);
extern void FreeEvent(dbgevent_t *);
extern void EventErrStr(dbgevent_t *, const char *, ...);
extern event_equiv CompareEvents(const dbgevent_t *, const dbgevent_t *);

// Used for error events
extern const char *DbgEventErrorStr(dbgevent_t *);

// Used for debug
extern dbgevent_t* NewErrorEvent(dbgerr err, const char* fmt, ...) __attribute__((format (printf,2,3)));
extern dbgevent_t* NewErrorEvent(dbgerr err);
extern const char *EventToStr(const dbgevent_t *);
extern const char *DbgevToStr(dbgev);
extern void PrintDbgError(dbgerr);

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
