/*********************************************************************************\
 * Debugger client defs.
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * Copyright 2010-2014,2020-2023 Hewlett Packard Enterprise Development LP.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Hewlett Packard Enterprise Development LP.,
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
#include "bitset.h"
#include "dbg.h"
#include "dbgclnt_breakpoints.h"
#include "dbgclnt_debugfe.h"
#include "dbgclnt_proto.h"
#include "dbgevent.h"
#include "dbgoutp.h"
#include "defs.h"
#include "dbginvoke.h"
#include <vector>
#include <chrono>
#include <functional>
#include <string_view>

// Forward declare event set
struct event_set;

// Used with the DbgQuit command, can override the default behavior.
enum class DbgQuitType { Default, Kill, Detach };

/*
** A group receiver is the actual dbginvoke_t/process mask for each di placed
** inside the rec_set_t.
*/
struct group_recv {
    DbgInvoke& rec_di;
    std::unique_ptr<Bitset> rec_group;
};
typedef struct group_recv group_recv_t;

std::strong_ordering operator<=>(const group_recv&, const group_recv&);
bool operator==(const group_recv&, const group_recv&);

class PickledRecSet;

/*
** This is used to describe which ranks to send commands to in the dbgclnt
*interface.
** If multiple di's are present in the list, the command will be sent to each di
** according to the group mask. The rep_set will contain responses and errors
*for
** each di involved in the command.
*/
class rec_set_t {
private:
    std::vector<group_recv_t*> m_recs;  // receivers of command
    char* m_threads;

public:
    rec_set_t();
    ~rec_set_t();
    rec_set_t(const rec_set_t&) = delete;
    rec_set_t& operator=(const rec_set_t&) = delete;
    std::unique_ptr<rec_set_t> clone() const;
    rec_set_t(rec_set_t&&);

    // `pickle` returns a copy of the rec_set_t that is safe to store long term.
    PickledRecSet pickle() const;

    void addReceiver(group_recv_t* rec);
    // remove receiver with the same memory address as rec from internal list.
    // frees the passed receiver.
    void removeReceiver(group_recv_t* rec);  
    // for each internal receiver that matches rec's di, intersect its group with
    // the passed rec's group.
    void intersectReceiver(const group_recv_t* rec);

    // deletes all internal data
    void clear();

    void merge(rec_set_t&& other);
    void merge(std::unique_ptr<rec_set_t>&& other);

    void setThreads(const char* threads);
    const char* getThreads() const { return m_threads; }
    
    size_t getNumReceivers() const { return m_recs.size(); }
    const std::vector<group_recv_t*>& getReceivers() const { return m_recs; }

    bool operator==(const rec_set_t& other) const;
};

inline std::vector<group_recv_t*>::const_iterator begin(const rec_set_t* set) {
    return set->getReceivers().begin();
}
inline std::vector<group_recv_t*>::const_iterator end(const rec_set_t* set) {
    return set->getReceivers().end();
}

class PickledRecSet {
    // rec_set_t stores group_recvs and group_recvs store DbgInvoke&. the
    // reference makes it unsafe to store group_recvs and thus rec_set_ts
    // between commands since the user could release the DbgInvoke and
    // invalidate the reference. PickledRecSet is a version of rec_set_t that
    // stores handles instead of references and is safe for storage.
public:
    PickledRecSet clone() const;
    void merge(PickledRecSet&& other);

    struct GroupRecv {
        // a group_recv that is safe to store long terem

        DbgInvoke::Handle dih;
        std::unique_ptr<Bitset> group;

        // `instantiate` creates a group_recv from the pickled version. if the stored
        // handle has become invalid since being stored, `instantiate` returns nullptr.
        group_recv* instantiate() const;
    };

    void addReceiver(const group_recv& recv);
    const std::vector<GroupRecv>& getReceivers() const;

    void setThreads(const char* threads);
    const char* getThreads() const;

    // `instantiate` creates a usable rec_set from the pickled version. if any
    // stored DbgInvoke handles are invalid, their group_recvs are omitted from
    // the resulting rec_set_t.
    rec_set_t instantiate() const;

private:
    void addReceiver(const GroupRecv& recv);
    void addReceiver(GroupRecv&& recv);

    std::vector<GroupRecv> m_recs;

    // emulating legacy char* behavior of rec_set_t where nullptr is a valid state
    std::optional<std::string> m_threads;
};

/*
** A group reply is the actual element placed inside the rep_set_t lists.
** All replies belong to a base dbginvoke_t struct, the mask of processes that
** this reply belongs to, and an event. All replies must be an event.
*/
struct group_reply {
    // Replies generated by expressions (compares, math, etc) will have no `rep_di`.
    // `rep_di` will be an empty string.
    DbgInvoke::Handle rep_di;
    std::unique_ptr<Bitset> rep_group;
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

Most debugger replies do not have the potential to generate gigantic replies, so these
operations can and should just use getReplies or the iterator forms.    These original
forms will also accumulate the result chunks into a single reply, though operations that
potentially access large data should avoid that usage.
*/
class rep_set_t {
public:
    // contruct an empty reply set
    rep_set_t();

    ~rep_set_t();
    rep_set_t(const rep_set_t&) = delete;
    rep_set_t(rep_set_t&&) = default;

    // create from chunks
    rep_set_t& operator=(const rep_set_t&) = delete;
    rep_set_t& operator=(rep_set_t&&) = default;

    // `pickle` tries to convert the rep_set into a form that is storable long
    // term and can be freely `clone`d multiple times.
    //
    // XXX: use with caution. not all states of rep_set_t are possible to
    // pickle. `pickle` has no way to report that case. the safest strategy is
    // to call `pickle` immediately after getting the rep_set from e.g.
    // DbgEvalExpr.
    //
    // an example of an unpicklable state is after the rep_set's backing data
    // has been converted to single use streams, which are not copyable by
    // design.
    void pickle();

    // create a copy of the contents of the rep_set.
    rep_set_t clone() const;

    // merge the replies in `expiring` into this object
    void merge(rep_set_t&& expiring);

    // return the number of errors in this reply
    int hasError() const;

    // return true if the rep set contains at least one DBGERR_OPERATION_ABANDONED
    bool hasOperationAbandonedError() const;

    // add a successful reply
    void addReply(group_reply_t* reply);

    // add a error reply
    void addErr(group_reply_t* reply);

    // return true if the rep_set has neigher replies nor error replies
    bool isEmpty() const;

    // get the first non-error reply if there is one
    //
    // XXX: PE-51027: this method is not const because it calls getReplies.
    group_reply_t* getFirstReply();

    // get the number of replies in the set ( does not include errors)
    //
    // XXX: PE-51027: this method is not const because it calls getReplies.
    size_t getNumReplies();

    // access the set of replies - generally prefer the iterator for (auto&& reply : repSet)
    //
    // XXX: PE-51027: this method is not const because it might fetch more data
    // and convert the backing data into single use streams, making the reply
    // uncloneable.
    const std::vector<group_reply_t*>& getReplies();

    // acccess the set of error replies
    //
    // XXX: PE-51027: this method is not const because it might fetch more data
    // and convert the backing data into single use streams, making the reply
    // uncloneable.
    const std::vector<group_reply_t*>& getErrs();

    // install information to get the next chunk. copies `set`.
    void setContinuation(const group_recv_t* set);

    // each expression result keeps and sequential id for requesting additional chunks.
    void setDataId(size_t id);
private:
    void mergeReplyChunks();
    void mergeReplyChunkSet(std::vector<rep_set_t>&& chunks);

    void fetchRemainingReplyChunks();
    // `getNextChunk` gets the next reply chunk - it returns an empty set when none remain.
    rep_set_t getNextChunk();
    rep_set_t recvChunk();

    // these lists should always be sorted by their groups (bitset::operator<) 
    // when accessed externally
    std::vector<group_reply_t*> m_reps;  // event replies of cmd
    std::vector<group_reply_t*> m_errs;  // error events

    std::unique_ptr<PickledRecSet> m_set; // for continuation of large arrays
    size_t m_dataId;                      // for continuation of large arrays
    bool m_returnedChunk0;                // the first result has been returned by getNextChunk
    std::vector<rep_set_t> m_unmergedChunks;

    void sortReplies();
    void sortErrors();
    bool m_repsIsSorted;
    bool m_errsIsSorted;
};

// Iteration support for processing the non-error replies of a rep_set_t, which
// the overwhelmingly most common operation.   Most commonly used as:
// for (auto&& reply : repSet)
inline std::vector<group_reply_t*>::const_iterator begin(rep_set_t& set) {
    // XXX: PE-51027: getReplies is not const so `set` can't be const
    return set.getReplies().begin();
}
inline std::vector<group_reply_t*>::const_iterator end(rep_set_t& set) {
    // XXX: PE-51027: getReplies is not const so `set` can't be const
    return set.getReplies().end();
}

// Used when calling DbgHandleSignal to determine if the signal should be
// maskable
enum dbgsig_type { DBG_SIG_MASKABLE, DBG_SIG_NOMASK };
typedef enum dbgsig_type dbgsig_type;

/*
** Receiver routines
*/
extern group_recv_t *NewGroupReceiver(DbgInvoke& di, const Bitset *proc_mask);
extern group_recv_t *NewGroupReceiver(DbgInvoke& di, std::unique_ptr<Bitset>&& proc_mask);
extern rec_set_t *NewRecSet(void);

extern void AddReceiverToSet(rec_set_t *set, group_recv_t *rec);
extern void FreeRecSet(rec_set_t *set);
extern bool RecSetHasOneProcPerReceiver(rec_set_t* set);

/*
** Reply routines
*/
extern group_reply_t *NewGroupReply(DbgInvoke::Handle di, std::unique_ptr<Bitset> &&src,
                                    dbgevent_t *reply);  // Make static
extern group_reply_t *CopyGroupReply(group_reply_t *reply);
extern void FreeGroupReply(group_reply_t *reply);
extern rep_set_t *CopyRepSet(rep_set_t *set);
extern void AddReplyToSet(rep_set_t *set, group_reply_t *rep);  // Make static
//extern void ResetSet(rep_set_t *set);                           // Make static
extern int SetHasError(rep_set_t *set);  // Maybe make static
//extern void SetReplyHeadInSet(rep_set_t *set);
extern int GetNumberOfReply(rep_set_t *set);
//extern group_reply_t *GetNextReplyInSet(rep_set_t *set);
extern int ProcessErrorInRepSet(rep_set_t& rep_set);

/*
** debugger client function prototypes
*/
extern void ForceQuit(void);
extern void HoldSignals(void);
extern void ReleaseSignals(void);
// wrapper around DbgSetPrintElements that is aware of the cached print elements
// value in client settings, only calling DbgSetPrintElements if the value passed
// is different from the value cached.
// function defined in cmd_print.cpp. 
extern void ClientSetPrintElements(int elements, rec_set_t* rec_set, bool force = false);

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
extern int DbgDispatchProtoEvents(DbgInvoke *di, const ProtoMessageSet &set);
extern void DbgDispatchProtoOutput(DbgInvoke *di, FILE *p_o_stream);
extern void DbgDispatchProtoOutput(DbgInvoke *di, const std::vector<std::string> &lines);
extern void DbgHandleEvent(DbgInvoke *di, DbgInvoke::AsyncEventCallback ev_callback);
extern void DbgHandleOutput(const std::function<void (DbgInvoke *, FILE *)> &outp);
extern void DbgHandleOutput(const std::function<void (DbgInvoke *, const std::vector<std::string> &)> &outp);
struct cli_stream_t;
extern void DbgHandleInput(int, cli_stream_t*, void (*)(void));
extern void DbgUnhandleInput(cli_stream_t*);
extern void DbgSwitchCurrentStream(cli_stream_t*);
using SigHandlerFn = void(*)(int);
extern void DbgHandleSignal(int signal, SigHandlerFn fn, dbgsig_type type);
extern void DbgMaskInput(void);
extern void DbgUnmaskInput(void);
extern void DbgUnmaskSignal(int);

// Used by higher layers to check if the inferior status has changed
extern int DbgCheckForStatusChange(void);
extern void DbgResetStatusChange(void);

extern void DbgSetSyncCommandTimeout(std::chrono::seconds seconds);
extern std::chrono::seconds DbgGetSyncCommandTimeout();

// Used by higher layers to check if we are currently waiting on an asynchronous
// event
extern int DbgCheckForAsyncPending(void);

extern DbgInvoke::Handle DbgAttach(const std::string& name, const AttachOpts& opts);
extern DbgInvoke::Handle DbgLaunch(const char* handle, int nproc, LaunchOpts&& args);

rep_set_t DbgStartup(rec_set_t *);
rep_set_t DbgEvalExpr(rec_set_t *, const char *);
// DbgSetDebuggerVar and DbgGetDebuggerVar is never called, this might be useful
// at some point though for staging data in gdb variables on the backends
rep_set_t DbgSetDebuggerVar(rec_set_t *, const char *, AIFStream *);
rep_set_t DbgGetDebuggerVar(rec_set_t *, const char *);
rep_set_t DbgSetSourcePath(rec_set_t *, DbgDirOp op, const char *);
rep_set_t DbgGetSourcePath(rec_set_t *);
rep_set_t DbgSetPrintElements(rec_set_t *, const char *);
rep_set_t DbgResetBeforeDirectCmd(rec_set_t *);
rep_set_t DbgRestoreAfterDirectCmd(rec_set_t *);
// Returns empty set on success, otherwise reply will contain error
rep_set_t DbgSetVar(rec_set_t *, const char * var, const char* val);
rep_set_t DbgGetType(rec_set_t *, const char *, int);
// Returns empty set on success, otherwise reply will contain error
rep_set_t DbgGo(rec_set_t *, bool all);
// Returns empty set on success, otherwise reply will contain error
rep_set_t DbgFinishFunc(rec_set_t *);
rep_set_t DbgSetLineBreak(rec_set_t *, int, const char *, int, const char*);
rep_set_t DbgSetFuncBreak(rec_set_t *, int, const char *, const char *, const char*);
rep_set_t DbgSetWatch(rec_set_t *, int, char *);
rep_set_t DbgSetCatch(rec_set_t *recSet, const std::string &event, const std::string &regex);

int DbgGetBPNum();
// `DbgGetBPNum` returns the id of the last break/catch/watchpoint set.
// certain commands can use it with the convenience variable "$bpnum"
//   dbg all> break ...
//   dbg all> ignore $bpnum 10
//
// note that if the underlying breakpoint is deleted, this number will
// still refer to it (this matches gdb behavior).

rep_set_t DbgInfoBreakpoints(rec_set_t *recSet);
rep_set_t DbgSetCheckpoint(rec_set_t* recSet);
// return empty set on success, otherwise reply will contain error
rep_set_t DbgRestoreCheckpoint(rec_set_t* recSet, unsigned int id);
// returns empty set on success, otherwise reply will contain error
rep_set_t DbgDeleteCheckpoint(rec_set_t* recSet, unsigned int id);
rep_set_t DbgSetThread(rec_set_t *, const char *);
rep_set_t DbgGetThreadInfo(rec_set_t*, const char *);
rep_set_t DbgSetThreadName(rec_set_t *, const char *);
rep_set_t DbgFindThread(rec_set_t *, const char*);
rep_set_t DbgThreadApply(rec_set_t *,const char* op);
rep_set_t DbgSetSchedulerLocking(rec_set_t *,const char* mode);
rep_set_t DbgGetSchedulerLocking(rec_set_t *);
rep_set_t DbgSetNonStop(rec_set_t *,const char* mode);
rep_set_t DbgGetNonStop(rec_set_t *);
rep_set_t DbgOmpd(rec_set_t *,const char* op);
rep_set_t DbgCuda(rec_set_t *,const char* op);

// Returns empty set on success, otherwise reply will contain error
// Pass in bp_set num, 0 means everything
rep_set_t DbgDeleteBreak(rec_set_t *, int);
// Returns empty set on success, otherwise reply will contain error
// Pass in bp_set num, 0 means everything
rep_set_t DbgDisableBreak(rec_set_t *, int);
// Returns empty set on success, otherwise reply will contain error
// Pass in bp_set num, 0 means everything
rep_set_t DbgEnableBreak(rec_set_t *, int, bool, bool);
rep_set_t DbgIgnoreBreak(rec_set_t *, int, int);
rep_set_t DbgConditionBreak(rec_set_t *, int, const std::string& cond);
rep_set_t DbgList(rec_set_t *, const char *);
rep_set_t DbgGetSources(rec_set_t *);
// Not called on a Cray, might be useful on a cluster though
rep_set_t DbgSetArgs(rec_set_t *, char *);
// DbgStep returns empty set on success, otherwise reply will contain error
rep_set_t DbgStep(rec_set_t *, int, int);
rep_set_t DbgMoveFrame(rec_set_t *, int count, DbgFrameDir dir);
rep_set_t DbgShowFrame(rec_set_t *, int);
rep_set_t DbgGetFrame(rec_set_t *, bool);
rep_set_t DbgGetVars(rec_set_t *);
rep_set_t DbgGetArgs(rec_set_t *);
rep_set_t DbgGetScope(rec_set_t *, const char *);
rep_set_t DbgDirectCmd(rec_set_t *, int, const char * const *);
rep_set_t DbgStatus(rec_set_t *, int);
rep_set_t DbgInterrupt(rec_set_t *, bool all);
rep_set_t DbgQuit(DbgInvoke&, DbgQuitType type);
rep_set_t DbgOpenSocket(rec_set_t *, char *, int);
rep_set_t DbgCloseSocket(rec_set_t *);
rep_set_t DbgSleep(rec_set_t* rec_set, int seconds);
rep_set_t DbgShell(rec_set_t *, int argc, const char * const * argv);
rep_set_t DbgSignal(rec_set_t *, const char* signame);

extern int PrintMultiThreadMsg(const char* diName, const Bitset* group, const std::string& msg);
extern bool DbgSetLogDir(const std::string& logdir);
#endif /* !_DBG_CLIENT_H */
