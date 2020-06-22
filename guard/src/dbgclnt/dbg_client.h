/*********************************************************************************\
 * Debugger client defs.
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * Copyright 2010-2014,2020 Cray Inc.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Cray Inc.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *
 *********************************************************************************/

#ifndef _DBG_CLIENT_H
#define _DBG_CLIENT_H

#include <stdint.h>

/*
** These includes define a common interface that many of the functions which
** interface with the debugger client need to access. We place them here to
** ensure that the function that is interfacing with the debugger client has the
** definitions it needs through including a single header.
*/
#include "aif.h"
#include "bitset.h"
#include "dbg.h"
#include "dbgclnt_debugfe.h"
#include "dbgclnt_proto.h"
#include "dbgevent.h"
#include "dbgoutp.h"
#include "defs.h"
#include <vector>
/*
** debugger client struct and type definitions
*/

// Forward declare event set
struct event_set;

// This is data used by the protocol layer for application output
// Usually this will be a g_progoutp_t *, but it depends on the proto.
typedef void *proto_outp;

// This is instance data used by the protocol layer that is allocated on a per
// dbginvoke basis. It is stored in the di since the protocol object is reused
// and is not suitable for storing instance data.
typedef void *proto_network;

// This is the protocol callback function prototype
typedef int (*proto_ev_callback_func)(struct dbginvoke *, proto_msg_set *);
// This is the client callback function prototype
// Arguments are the event_set for this event and a bitset containing the new
// ranks to report this event
typedef int (*clnt_ev_callback_func)(struct event_set *, bitset *);
// This is the proto program output callback function prototype
typedef void (*proto_outp_callback_func)(struct dbginvoke *, FILE *);

// Used to determine type of dbginvoke, currently there is either launched or
// attached launched means the application was launched by the debugger,
// attached means it was an existing application attached onto.
enum dbgproc_type { DBG_ATTACHED, DBG_LAUNCHED };
typedef enum dbgproc_type dbgproc_type;

// Used with the DbgQuit command, can override the default behavior.
enum dbgquit_type { DBG_KILL, DBG_DETACH, DBG_DEFAULT };
typedef enum dbgquit_type dbgquit_type;

struct dbginvoke {
    char *d_handle;       // invoke handle used as identifier in client layer
    uint64_t d_appid;     // app identifier - this should be generic
    dbgproc_type d_type;  // launched or attached?
    int d_started;        // indicates if this di has been started
    int d_isShutdown;  // Some networks can use this during error handling. If
                       // this is true, the network is already shutdown.
    int d_nproc;       // number of processes
    char *d_cmd;  // application command argument - set to name of executable
                  // for invoke
    int d_stat;   // exit/signal status of proc
    list_t *d_evfds;        // event notification fds specific to this instance
    bitset *d_bitset_full;  // represents all dbgsrvs
    bitset *d_bitset_launched;  // for invoke cmd: represents dbgsrvs that has
                                // been launched

    int d_proto_id;  // di specific protocol id - optionally used by certain
                     // protocols
    proto_network d_proto_net;  // di specific protocol network object
    proto_outp d_proto_outp;    // di specific protocol output object
    dbgclnt_proto *d_proto;     // client protocol switch

    // The following two callbacks are for protocol event messages and for
    // handling the actual events in the client layer.
    proto_ev_callback_func d_proto_ev_callback;
    clnt_ev_callback_func d_clnt_ev_callback;
    // This is the callback for handling application output in the client
    proto_outp_callback_func d_proto_outp_callback;

    dbgfrontend_sw *d_frontend;  // debug frontend object

    // Extensions for client support
    list_t *d_eventset_list;  // pending events
    list_t *d_bpset_list;     // current breakpoints
    int d_global_bpset_id;    // PRIVATE TO DBG_CLIENT!!! Do not use!
};
typedef struct dbginvoke dbginvoke_t;
#define DBG_PROTO_SW(p) (p)->d_proto

/*
** A group receiver is the actual dbginvoke_t/process mask for each di palced
** inside the rec_set_t.
*/
struct group_recv {
    dbginvoke_t *rec_di;
    bitset *rec_group;
    bitset *_cmd_sent;  // Internal to dbg_clnt layer - DO NOT USE!!!
};
typedef struct group_recv group_recv_t;

/*
** This is used to describe which ranks to send commands to in the dbgclnt
*interface.
** If multiple di's are present in the list, the command will be sent to each di
** according to the group mask. The rep_set will contain responses and errors
*for
** each di involved in the command.
*/
struct rec_set {
    list_t *rec_list;  // recievers of command
};
typedef struct rec_set rec_set_t;

/*
** A group reply is the actual element placed inside the rep_set_t lists.
** All replies belong to a base dbginvoke_t struct, the mask of processes that
** this reply belongs to, and an event. All replies must be an event.
*/
struct group_reply {
    dbginvoke_t *rep_di;
    bitset *rep_group;
    dbgevent_t *rep_event;
};
typedef struct group_reply group_reply_t;

/*
** This is used to hold replies from commands in the dbgclnt interface.
** The client layer can call the ProcessErrorInRepSet() function to
** print out any errors.
** A ReplySet(rep_set_t) keeps results replied by a number of BEs.
** These results are aggregated into several unique groups.
** Each group may be an error message (maintained by m_errs)
** or a normal message (maintained by group_reply_t).

For very large arrays, rep_set_t supports the ability to hold just slice of the array
result at one time.   Code using the rep_set can iterate over the slices by using the
\a getNextSlice function.   Using this approach a client side operation could potentially
process a terabyte sized array while never needing to allocate more than a fixed number of
of megabytes.

Most debugger replies do not have the potential to generate gigantic replies, so these
operations can and should just use getReplies or the iterator forms.    These original
forms will also accumulate the result slices into a single reply, though operations that
potentially access large data should avoid that usage.
*/
class rep_set_t {
public:
    // contruct an empty reply set
    rep_set_t();

    ~rep_set_t();
    rep_set_t(const rep_set_t&) = delete;

    // create from slices
    rep_set_t(const std::vector<std::unique_ptr<rep_set_t>>& slices, arrayDimension_t* shape, bitset* group);
    rep_set_t& operator=(const rep_set_t&) = delete;

    // create a copy of the contents of the rep_set
    rep_set_t* clone() const;

    // merge the replies in \a tmp into this object *and delete \a tmp*
    rep_set_t* merge(rep_set_t* tmp);

    // return the number of group_sets in this reply
    int hasError() const;

    // add a successful reply
    void addReply(group_reply_t* reply);

    // add a error reply
    void addErr(group_reply_t* reply);

    // return true if the rep_set has neigher replies nor error replies
    bool isEmpty() const;

    // get the first non-error reply if there is one
    group_reply_t* getFirstReply() const;

    // get the number of replies in the set ( does not include errors)
    size_t getNumReplies() const;

    // access the set of replies - generally prefer the iterator for (auto&& reply : repSet)
    const std::vector<group_reply_t*>& getReplies() const;

    // acccess the set of error replies
    const std::vector<group_reply_t*>& getErrs() const;

    // get the next reply slice - return nullptr when none remain.
    std::unique_ptr<rep_set_t> getNextSlice();

    // install information to get the next slice
    void setContinuation(group_recv_t* set);

    // each expression result keeps and sequential id for requesting additional slices.
    void setDataId(size_t id);

    std::vector<group_reply_t*>& getMutableReplies();
    static const std::vector<group_reply_t*>& getNullReply();

private:
    void mergeReplySlices();
    void mergeReplySliceSet(const std::vector<std::unique_ptr<rep_set_t>>& slices,
                            arrayDimension_t* shape, bitset* group);
    std::unique_ptr<rep_set_t> recvSlice();

    std::vector<group_reply_t*> m_reps;  // event replies of cmd
    std::vector<group_reply_t*> m_errs;  // error events
    rec_set_t* m_set;                 // for continuation of large arrays
    size_t m_dataId;                     // for continuation of large arrays
    bool m_isSliced;                     // the data can be accessed in sequential pieces
    bool m_returnedSlice0;               // the first result has been returned by getNextSlice
    bool m_moreSlices;                   // more slices remain to be retrieved
};

// Iteration support for processing the non-error replies of a rep_set_t, which
// the overwhelmingly most common operation.   Most commonly used as:
// for (auto&& reply : repSet)   and it treats repSet==nullptr as an empty set.
inline std::vector<group_reply_t*>::const_iterator begin(const rep_set_t* set) {
    return (set ? set->getReplies() : rep_set_t::getNullReply()).begin();
}
inline std::vector<group_reply_t*>::const_iterator end(const rep_set_t* set) {
    return (set ? set->getReplies() : rep_set_t::getNullReply()).end();
}

// These are used to describe asynchronous events to the client interface
struct event_set {
    dbginvoke_t *e_di;
    bitset *e_group;
    dbgevent_t *e_ev;
};
typedef struct event_set event_set_t;

// Used to represent a breakpoint location
typedef struct {
    bitset *    group;      // All ranks that hit this location
    std::string loc_file;   // locale file
    int         loc_line;   // locale line
    std::string loc_func;   // locale function
} bp_loc_t;

class bp_locale_set
{
public:
    using bp_loc_set_t = std::vector<bp_loc_t>;
private:
    bp_loc_set_t bp_loc;
public:
    using const_iterator = bp_loc_set_t::const_iterator;

    void add(bitset *group, char *loc_file, int loc_line, char *loc_func)
    {
        // search the list
        if (loc_file != nullptr) {
            const std::string file{loc_file};
            for (auto&& i : bp_loc) {
                if (i.loc_file == loc_file && i.loc_line == loc_line) {
                    // found it!
                    bitset_oreq(i.group, group);
                    return;
                }
            }
        }
        else {
            const std::string func{loc_func};
            for (auto&& i : bp_loc) {
                if (i.loc_func == loc_func) {
                    // found it!
                    bitset_oreq(i.group, group);
                    return;
                }
            }
        }
        // Not found! Add it.
        bp_loc.push_back({bitset_dup(group), loc_file, loc_line, loc_func});
    }

    void clear()
    {
        for (auto&& i : bp_loc) {
            // free the bitset
            bitset_free(i.group);
        }
        bp_loc.clear();
    }

    const_iterator begin() const { return bp_loc.begin(); }
    const_iterator end() const { return bp_loc.end(); }
    const_iterator cbegin() { return bp_loc.cbegin(); }
    const_iterator cend() { return bp_loc.cend(); }

    bp_locale_set();
    ~bp_locale_set()
    {
        clear();
    }

    // Delete copy/move constructors and assignment
    bp_locale_set(bp_locale_set const&) = delete;
    bp_locale_set& operator=(bp_locale_set const&) = delete;
};


// This is used to represent breakpoints and is accessible by the client layer.
// It gets stored in the dbginvoke and is specific to that pariticular dbginvoke
// object.
typedef struct {
    // Current bp num - This gets reset every time a bp_set is returned to the
    // client. These numbers are issued per dbginvoke_t object. The dbg_client
    // layer needs to remember to update these when returning the object to the
    // client layer.
    int b_bp_id;  // The current breakpoint number

    // group information
    bitset *b_group;  // All ranks that contain this breakpoint

    // breakpoint unique gdb id's
    list_t *b_bp_id_list;  // a list of bp_id_set_t - private to dbgclnt layer

    // breakpoint information
    bitset *b_bp_enabled;  // set representing enabled breakpoints, this can
                           // vary by rank.
    list_t *
        b_bp_disp;  // a list of bp_disp_set_t representing breakpoint
                    // disposition (i.e. keep, dis, del), this can vary by rank.
    dbgbp_type b_bp_type;  // breakpoint type (i.e. breakpoint, watchpoint)
    bool b_bp_pending; // pending breakpoint

    // breakpoint locale
    char *b_loc_stmt;  // locale statement (pending)
    char *b_loc_addr;  // locale address
    char *b_loc_file;  // locale file
    char *b_loc_func;  // locale function
    int b_loc_line;    // locale line

    // Watchpoint information
    char *b_bp_exp;               // watchpoint expression
    bp_locale_set b_bp_watch_loc; // watchpoint location

    // Thread specific breakpoints
    int b_thread_id;  // thread-id (if > 0)
} bp_set_t;

// Only should be used in the dbg_client layer. This is used to map the client
// breakpoint number onto gdb backend breakpoint numbers.
struct bp_id_set {
    bitset *b_group;
    int b_bp_id;  // unique bkpt id assigned by each gdb
};
typedef struct bp_id_set bp_id_set_t;

// This is used when checking the disposition of the breakpoint (i.e. enabled,
// disabled, etc)
struct bp_disp_set {
    bitset *b_group;  // bitset representing the ranks with this disposition
    dbgbp_disp b_bp_disp;  // disposition value
};
typedef struct bp_disp_set bp_disp_set_t;

// Used when calling DbgHandleSignal to determine if the signal should be
// maskable
enum dbgsig_type { DBG_SIG_MASKABLE, DBG_SIG_NOMASK };
typedef enum dbgsig_type dbgsig_type;

/*
** Receiver routines
*/
extern group_recv_t *NewGroupReceiver(dbginvoke_t *di, bitset *proc_mask);
extern rec_set_t *NewRecSet(void);
extern void AddReceiverToSet(rec_set_t *set, group_recv_t *rec);
extern void FreeRecSet(rec_set_t *set);
extern void SetReceiverHeadInSet(rec_set_t *set);
extern group_recv_t *GetNextReceiverInSet(rec_set_t *set);
extern bool RecSetHasOneProcPerReceiver(rec_set_t* set);

/*
** Reply routines
*/
extern group_reply_t *NewGroupReply(dbginvoke_t *di, bitset *src,
                                    dbgevent_t *reply);  // Make static
extern group_reply_t *CopyGroupReply(group_reply_t *reply);
extern void FreeGroupReply(group_reply_t *reply);
extern rep_set_t *NewRepSet(void);  // Make static
extern void FreeRepSet(rep_set_t *set);
extern rep_set_t *CopyRepSet(rep_set_t *set);
extern void AddReplyToSet(rep_set_t *set, group_reply_t *rep);  // Make static
//extern void ResetSet(rep_set_t *set);                           // Make static
extern int SetHasError(rep_set_t *set);  // Maybe make static
//extern void SetReplyHeadInSet(rep_set_t *set);
extern int GetNumberOfReply(rep_set_t *set);
//extern group_reply_t *GetNextReplyInSet(rep_set_t *set);
extern int ProcessErrorInRepSet(rep_set_t *);

/*
** Async event routines
*/
extern bp_set_t *FindBPSetByID(dbginvoke_t *, int);
extern bp_set_t *FindBPSetForBP(dbginvoke_t *di, dbgbp_t *bp);
extern void ResetBPSetList(dbginvoke_t *di);
extern bp_set_t *GetNextBPSet(dbginvoke_t *di);
extern int EmptyBPSetList(dbginvoke_t *di);

/*
** debugger client function prototypes
*/
extern void ForceQuit(void);
extern void HoldSignals(void);
extern void ReleaseSignals(void);

/*
** interface prototypes
*/
extern int DbgInit(void);
extern void DbgFinalize(void);

extern int DbgCheckEvents(void);
extern void DbgDispatchEvents(void);
// DbgDispatchProtoEvent is the dbginvoke callback used to handle any protocol
// specific events received during the DbgDispatchEvents proto level call. Note
// that the msg is the unprocessed event network protocol string that will be
// converted to an actual event.
extern int DbgDispatchProtoEvents(dbginvoke_t *di, proto_msg_set *set);
extern void DbgDispatchProtoOutput(dbginvoke_t *di, FILE *p_o_stream);
extern void DbgHandleEvent(dbginvoke_t *di, clnt_ev_callback_func ev_callback);
extern void DbgHandleOutput(void (*)(dbginvoke_t *di, FILE *p_o_stream));
extern void DbgHandleInput(int, void (*)(void));
extern void DbgHandleSignal(int, void (*)(int), dbgsig_type);
extern void DbgMaskInput(void);
extern void DbgUnmaskInput(void);
extern void DbgUnmaskSignal(int);

// Used by higher layers to check if the inferior status has changed
extern int DbgCheckForStatusChange(void);
extern void DbgResetStatusChange(void);

// Used by higher layers to check if we are currently waiting on an asynchronous
// event
extern int DbgCheckForAsyncPending(void);

extern proto_session *DbgSessionCreate(const char *, WLM_type, char *, char *,
                                       char *);
extern void DbgSetSession(proto_session *);
extern proto_session *DbgGetSession(void);

extern dbginvoke_t *DbgAttach(const char *, proto_session *, const char *,
                              const char *, const char *, const char *, int,
                              int);
extern dbginvoke_t *DbgInvoke(const char *, proto_session *, const char *,
                              char **, const char *, int, const char *, char **,
                              const char *, const char *, char **, const char *,
                              int, int, int, int, bool);

extern rep_set_t *DbgStartup(rec_set_t *);
// This is the only thing that should be returning something other than
// rep_set_t * I don't know how to fix it since its for sending a synchronous
// command asynchronously
// TODO: This should probably be removed.
extern void DbgSendExpr(rec_set_t *, char *);
// If the above is removed, get rid of this too.
extern rep_set_t *DbgGetExpr(rec_set_t *);
extern rep_set_t *DbgEvalExpr(rec_set_t *, char *);
// DbgSetDebuggerVar and DbgGetDebuggerVar is never called, this might be useful
// at some point though for staging data in gdb variables on the backends
extern rep_set_t *DbgSetDebuggerVar(rec_set_t *, char *, AIF *);
extern rep_set_t *DbgGetDebuggerVar(rec_set_t *, char *);
extern rep_set_t *DbgSetSourcePath(rec_set_t *, char *);
extern rep_set_t *DbgGetSourcePath(rec_set_t *);
// Returns NULL on success, otherwise reply will contain error
extern rep_set_t *DbgSetVar(rec_set_t *, char *, AIF *);
extern rep_set_t *DbgGetType(rec_set_t *, char *, int);
// Returns NULL on success, otherwise reply will contain error
extern rep_set_t *DbgGo(rec_set_t *, bool all);
// Returns NULL on success, otherwise reply will contain error
extern rep_set_t *DbgFinishFunc(rec_set_t *);
extern rep_set_t *DbgSetLineBreak(rec_set_t *, int, char *, int, int);
extern rep_set_t *DbgSetFuncBreak(rec_set_t *, int, char *, char *, int);
extern rep_set_t *DbgSetWatch(rec_set_t *, int, char *);
extern rep_set_t *DbgSetThread(rec_set_t *, const char *);
extern rep_set_t *DbgGetThreadInfo(rec_set_t*, const char *);
extern rep_set_t *DbgSetThreadName(rec_set_t *, const char *);
extern rep_set_t *DbgFindThread(rec_set_t *, const char*);
extern rep_set_t *DbgThreadApply(rec_set_t *,const char* op);
extern rep_set_t *DbgSetSchedulerLocking(rec_set_t *,const char* mode);
extern rep_set_t *DbgGetSchedulerLocking(rec_set_t *);
extern rep_set_t *DbgSetNonStop(rec_set_t *,const char* mode);
extern rep_set_t *DbgGetNonStop(rec_set_t *);


// Returns NULL on success, otherwise reply will contain error
extern rep_set_t *DbgKill(rec_set_t *);
// Returns NULL on success, otherwise reply will contain error
// Pass in bp_set num, 0 means everything
extern rep_set_t *DbgDeleteBreak(rec_set_t *, int);
// Returns NULL on success, otherwise reply will contain error
// Pass in bp_set num, 0 means everything
extern rep_set_t *DbgDisableBreak(rec_set_t *, int);
// Returns NULL on success, otherwise reply will contain error
// Pass in bp_set num, 0 means everything
extern rep_set_t *DbgEnableBreak(rec_set_t *, int, bool, bool);
extern rep_set_t *DbgList(rec_set_t *, char *);
extern rep_set_t *DbgGetSources(rec_set_t *);
// Not called on a Cray, might be useful on a cluster though
extern rep_set_t *DbgSetArgs(rec_set_t *, char *);
// Returns NULL on success, otherwise reply will contain error
extern rep_set_t *DbgStep(rec_set_t *, int, int);
extern rep_set_t *DbgMoveFrame(rec_set_t *, int, int);
extern rep_set_t *DbgShowFrame(rec_set_t *, int);
extern rep_set_t *DbgGetFrame(rec_set_t *);
extern rep_set_t *DbgGetVars(rec_set_t *, char *);
extern rep_set_t *DbgGetScope(rec_set_t *, char *);
extern rep_set_t *DbgDirectCmd(rec_set_t *, int, char **);
extern rep_set_t *DbgStatus(rec_set_t *, int);
extern rep_set_t *DbgInterrupt(rec_set_t *, bool all);
extern rep_set_t *DbgQuit(rec_set_t *, dbgquit_type);
extern rep_set_t *DbgOpenSocket(rec_set_t *, char *, int);
extern rep_set_t *DbgCloseSocket(rec_set_t *);

#endif /* !_DBG_CLIENT_H */
