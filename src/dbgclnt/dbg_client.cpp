/*********************************************************************************\
 * Debugger interface routines
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * Copyright 2010-2023 Hewlett Packard Enterprise Development LP.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Hewlett Packard Enterprise Development LP.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *
 *********************************************************************************/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <errno.h>
#include <string.h>
#include <vector>
#include <mutex>
#include <filesystem>
#include <fstream>
#include <ranges>

#include "script_client.h"
#include "dbg_client.h"
#include "useful.h"
#include "dbginvoke.h"

#include "common_tools_fe.h"
#include "observable.h"

/*
** Internal types
*/
class inpcb_t {
    // inpcb_ts are used by the main loop to connect file descriptors to the current
    // stream. see #StreamInput in client_io.md.
public:
    inpcb_t(int fd, cli_stream_t* stream, void (*f)());
    int getFd() const;
    cli_stream_t* getStream() const { return i_stream; }
    void invoke() { if (i_rtn) { (*i_rtn)(); } }

    void mask(bool masked);
    // `mask` masks the input. when an input is masked, `getFd` returns -1.

private:
    int i_fd;
    cli_stream_t* i_stream;
    void (*i_rtn)();
    int masked;
    // This needs a lock because the MRNet error callback is threaded and will
    // mess with the inpcb_t object during printing error messages
    mutable std::recursive_mutex i_mutex;
};

struct InputList {
    // `InputList` is a wrapper around a list of inpcb_t.
    std::vector<std::unique_ptr<inpcb_t>> inputs;

    inpcb_t* get_active_input();
    // `get_active_input` returns an input that is active (has data waiting to
    // be read on its file descriptor). if no inputs are active, it returns
    // nullptr.
    //
    // if multiple inputs in the list are active simultaneously,
    // `get_active_input` returns the active input that appears latest in the
    // list. this means that newer inputs have higher priority in ties. the tie
    // breaker *only* happens when *multiple* inputs are active. the first input
    // in the list will still be returned over the last if the first is active
    // and the last isn't.
    //
    // this priority strategy is useful for file inputs because they are always
    // considered active. since they are always active, the newest file input
    // will always be returned from `get_active_input`, which gets us the
    // following:
    //   1. user input will automatically be blocked after a source command is
    //      run, and unblocked when the source command completes.
    //   2. source commands can themselves safely call recursive source commands.
    //      the new child source input will have a higher priority than the
    //      parent, pausing the parent until the child is done.

    void add_input(int fd, cli_stream_t* stream, void (*invoke_fn)(void));
    // `add_input` creates a new inpcb_t from the arguments and appends it the
    // the end of the list.

    void remove_input(const cli_stream_t* stream);
    // `remove_input` searches the list for the first inpcb_t that contains
    // `stream` and removes it from the list. `stream` is not deleted.

    void mask_all_inputs();
    void unmask_all_inputs();
    // `mask_all_inputs` and `unmask_all_inputs` mask/unmask each input respectively.
    // see inpcb_t::mask.
};

struct OutputCallbacks {
    // Called by protocols when output from the program being debugged is detected.
    // The job of each callback is to output the data to the cli.
    // Different protocols deliver the data in different formats.
    
    std::function<void (DbgInvoke *, FILE*)> filestreamCallback;
    std::function<void (DbgInvoke *, const std::vector<std::string>&)> stringVectorCallback; 
};

/*
** Internal prototypes
*/
static rep_set_t DbgRecvReply(group_recv_t *set, dbgev req_type, dbgev alt_req_type=DBGEV_NONE);
static void FreeGroupReceiver(group_recv_t *);
static group_recv_t *CopyGroupReceiver(group_recv_t *);
static int GetNumberOfReceivers(rec_set_t *);
static void DestroyEventSet(DbgInvoke *, dbgev);
static DbgInvoke::EventSet *AddEvent(DbgInvoke *, const Bitset *, dbgevent_t *);
static void CheckForFullEventSet(DbgInvoke *, const Bitset *, dbgev);
static void ResetForAsyncCmd(rec_set_t *);
static void RemoveGroupFromAsyncSet(DbgInvoke *, const Bitset *);
static group_reply_t* JoinReplies(const Bitset* group,
                                  const std::vector<group_reply*>& replies,
                                  bool& isError);

static rep_set_t DbgSendCmdAndRecvReply(rec_set_t *set, const std::string& request,
                                         dbgev req_type, dbgev alt_req_type=DBGEV_NONE);
static std::pair<std::string,std::unique_ptr<const char*[]>>
GetTrampolineArgs(const char* prog, const char* const* args);

// if `repset` contains any DBGEV_FRAMEs, copy them into the reply di's frame cache.
static void cacheFrames(rep_set_t& repset);

/*
** Internal Variables
*/
static std::vector<int> DbgSigList;
static std::vector<int> DbgUnblockableSigList;

// FIXME: these fd variables and the functions that use them
//        should be wrapped up in a class
static fd_set dispatch_fds;
static int high_fd;

static OutputCallbacks OutputCB;
static int async_pending;     // used to track completion of async events
static rec_set_t *async_set;  // full set of async receivers

static auto InputCBs = InputList{};

// value of $bpnum
static auto g_bpnum = -1;

// If a sync command doesn't return in this many seconds, attempt to cancel it. 0 means no
// timeout is applied.
static auto sync_command_timeout = std::chrono::seconds(0);

/*
** Hold any pending signals.
*/
void HoldSignals(void) {
    sigset_t mask;

    // initialize values
    sigemptyset(&mask);

    if (!DbgSigList.empty()) {
        for (auto&& sig : DbgSigList) {
            sigaddset(&mask, sig);
        }

        sigprocmask(SIG_BLOCK, &mask, nullptr);
    }
}

/*
** Release any pending signals.
*/
void ReleaseSignals(void) {
    sigset_t mask;

    // initialize data
    sigemptyset(&mask);

    if (!DbgSigList.empty()) {
        for (auto&& sig : DbgSigList)
            sigaddset(&mask, sig);

        sigprocmask(SIG_UNBLOCK, &mask, nullptr);
    }
}

/**************************
** Receiver set functions
**************************/

/*
 * data structures for group receivers to dbgsrv
 */

group_recv_t *NewGroupReceiver(DbgInvoke& di, const Bitset *proc_mask) {
    auto rec = new group_recv_t {
        .rec_di = di,
        .rec_group = proc_mask->clone(),
    };

    return rec;
}

group_recv_t* NewGroupReceiver(DbgInvoke& di, std::unique_ptr<Bitset>&& mask) {
    auto rec = new group_recv_t {
        .rec_di = di,
        .rec_group = std::move(mask),
    };

    return rec;
}

static void FreeGroupReceiver(group_recv_t *rec) {
    delete rec;
}

static group_recv_t *CopyGroupReceiver(group_recv_t *rec) {
    if (rec == nullptr) return nullptr;

    auto newrec = new group_recv_t {
        .rec_di = rec->rec_di,
        .rec_group = rec->rec_group->clone(),
    };

    return newrec;
}

/*
 * data structures for sets of group receivers to dbgsrv
 */

rec_set_t *NewRecSet(void) {
    return new rec_set_t;
}

rec_set_t::rec_set_t() : m_threads(nullptr) {}

rec_set_t::~rec_set_t() {
    clear();
}

std::unique_ptr<rec_set_t> rec_set_t::clone() const {
    auto r = std::make_unique<rec_set_t>();
    
    // copy receivers
    for (const auto& rec : m_recs) {
        r->m_recs.emplace_back(new group_recv_t {
            .rec_di = rec->rec_di,
            .rec_group = rec->rec_group
                ? rec->rec_group->clone()
                : nullptr,
        });
    }
    // copy thread string
    r->m_threads = m_threads
        ? strdup(m_threads)
        : nullptr
    ;

    return r;
}

rec_set_t::rec_set_t(rec_set_t&& other) {
    m_recs = std::move(other.m_recs);
    m_threads = other.m_threads;
    other.m_threads = nullptr;
}

PickledRecSet rec_set_t::pickle() const {
    auto result = PickledRecSet{};

    for (const auto* recv : m_recs) {
        result.addReceiver(*recv);
    }

    result.setThreads(m_threads);

    return result;
}

void rec_set_t::setThreads(const char* threads)
{
    char* t = ( threads && threads[0] ? strdup(threads) : nullptr);
    std::swap(t, m_threads);
    free(t);
}

std::strong_ordering operator<=>(const group_recv& left, const group_recv& right) {
    const auto LESS = std::strong_ordering::less;
    const auto GREATER = std::strong_ordering::greater;
    const auto EQUAL = std::strong_ordering::equivalent;

    // first, check dis
    if (left.rec_di.handle() != right.rec_di.handle()) {
        return left.rec_di.handle() < right.rec_di.handle() ?
            LESS : GREATER;
    }

    // fall back to checking bitsets
    const auto* left_bitset = left.rec_group.get();
    const auto* right_bitset = right.rec_group.get();
    if (left_bitset && right_bitset) {
        if (*left_bitset == *right_bitset) return EQUAL;
        return *left_bitset < *right_bitset ? LESS : GREATER;
    } else if (right_bitset) {
        // left is nullptr, and nullptr is always less than non-nullptr
        return LESS;
    } else if (left_bitset) {
        return GREATER;
    } else {
        // two nullptrs
        return EQUAL;
    }
}

bool operator==(const group_recv& left, const group_recv& right) {
    return (left <=> right) == std::strong_ordering::equivalent;
}

bool rec_set_t::operator==(const rec_set_t& other) const {
    if (m_recs.size() != other.m_recs.size()) return false;

    // m_recs is a short list of pointers, so a copy is ok
    auto left = m_recs;
    auto right = other.m_recs;

    // use a dereference projection, otherwise these would compare pointer addresses
    const auto deref = [](const auto* ptr) -> const auto& { return *ptr; };
    std::ranges::sort(left, {}, deref);
    std::ranges::sort(right, {}, deref);
    return std::ranges::equal(left, right, {}, deref, deref);
}

void FreeRecSet(rec_set_t *set) {
    delete set;
}

void rec_set_t::addReceiver(group_recv_t *rec)
{
    if (!rec) return;

    // Here we want to enforce merging since this is done *before* any command
    // is sent. We want to send all commands at the same time to each di tree.
    for (auto&& r : m_recs) {
        // If the di objects match, they are they point to the same thing.
        if (r->rec_di == rec->rec_di) {
            // The provided di matches, we need to merge
            *r->rec_group |= *rec->rec_group;
            FreeGroupReceiver(rec);
            return;
        }
    }
    m_recs.push_back(rec);

}

void AddReceiverToSet(rec_set_t *set, group_recv_t *rec) {
    if (rec == nullptr || set == nullptr) return;
    set->addReceiver(rec);
}

void rec_set_t::removeReceiver(group_recv_t* rec) {
    m_recs.erase(std::remove(m_recs.begin(), m_recs.end(), rec), m_recs.end());
    FreeGroupReceiver(rec);
}

void rec_set_t::intersectReceiver(const group_recv_t* rec) {
    if (!rec || !rec->rec_group) return;
    auto toRemove = std::set<group_recv_t*>{};

    for (auto* group : m_recs) {
        if (group->rec_di == rec->rec_di) {
            *group->rec_group &= *rec->rec_group;
            if (group->rec_group->numSet() == 0) {
                toRemove.insert(group);
            }
        }
    }

    for (auto* group : toRemove) {
        removeReceiver(group); // frees group
    }
}

void rec_set_t::clear() {
    for (auto&& recv : m_recs) {
        FreeGroupReceiver(recv);
    }
    m_recs.clear();
    free(m_threads);
    m_threads = nullptr;
}

void rec_set_t::merge(rec_set_t&& other) {
    for (auto*& group : other.m_recs) {
        addReceiver(group); // frees group
        group = nullptr;
    }
    other.m_recs.clear();
}

void rec_set_t::merge(std::unique_ptr<rec_set_t>&& other) {
    if (!other) return; // nothing to merge
    merge(std::move(*other));
}

PickledRecSet PickledRecSet::clone() const {
    auto result = PickledRecSet{};

    for (const auto& recv : m_recs) {
        result.m_recs.emplace_back(GroupRecv{
            .dih = recv.dih,
            .group = recv.group->clone(),
        });
    }

    result.m_threads = m_threads;

    return result;
}

void PickledRecSet::merge(PickledRecSet&& other) {
    for (auto&& recv : other.m_recs) {
        addReceiver(std::move(recv));
    }
}

void PickledRecSet::addReceiver(const group_recv_t& recv) {
    addReceiver(PickledRecSet::GroupRecv{
        .dih = recv.rec_di.handle(),
        .group = recv.rec_group->clone(),
    });
}

void PickledRecSet::addReceiver(const PickledRecSet::GroupRecv& recv) {
    addReceiver(PickledRecSet::GroupRecv{
        .dih = recv.dih,
        .group = recv.group->clone(),
    });
}

void PickledRecSet::addReceiver(PickledRecSet::GroupRecv&& recv) {
    for (auto& my_recv : m_recs) {
        if (my_recv.dih == recv.dih) {
            *my_recv.group |= *recv.group;
            return;
        }
    }

    m_recs.emplace_back(std::move(recv));
}

const std::vector<PickledRecSet::GroupRecv>& PickledRecSet::getReceivers() const {
    return m_recs;
}

void PickledRecSet::setThreads(const char* threads) {
    m_threads = threads ? std::optional{std::string{threads}} : std::nullopt;
}

const char* PickledRecSet::getThreads() const {
    return m_threads ? m_threads->c_str() : nullptr;
}

rec_set_t PickledRecSet::instantiate() const {
    auto result = rec_set_t();

    for (const auto& rec : m_recs) {
        if (auto* recv = rec.instantiate()) {
            result.addReceiver(recv); // result takes ownership of recv
        }
    }

    result.setThreads(m_threads ? m_threads->c_str() : nullptr);

    return result;
}

group_recv* PickledRecSet::GroupRecv::instantiate() const {
    if (auto* di = this->dih.lock()) {
        return new group_recv {
            .rec_di = *di,
            .rec_group = this->group->clone(),
        };
    }

    // expired
    return nullptr;
}

// This returns the number of dbginvoke_t in the rec_set
static int GetNumberOfReceivers(rec_set_t *set) {
    return set ? set->getReceivers().size() : 0;
}

bool RecSetHasOneProcPerReceiver(rec_set_t* set)
{
    if (!set) return false;
    for (auto&& recv : set) {
        if (recv->rec_group->numSet() != 1) {
            return false;
        }
    }
    return true;
}

/**********************************
** Reply set and related functions
**********************************/

/*
 * data structures for group replies from dbgsrv
 */

group_reply_t *NewGroupReply(DbgInvoke::Handle di, std::unique_ptr<Bitset> &&src, dbgevent_t *reply) {
    return new group_reply_t {
        .rep_di = std::move(di),
        .rep_group = std::move(src),
        .rep_event = reply
    };
}

void FreeGroupReply(group_reply_t *rep) {
    FreeEvent(rep->rep_event);
    delete rep;
}

group_reply_t *CopyGroupReply(group_reply_t *rep) {
    if (rep == nullptr) return nullptr;

    auto newrep = new group_reply_t {
        .rep_di = rep->rep_di,
        .rep_group = rep->rep_group->clone(),
        .rep_event = DupEvent(rep->rep_event)
    };

    return newrep;
}

/*
 * data structures for sets of group replies from dbgsrv
 */

rep_set_t::rep_set_t()
    : m_set(nullptr), m_dataId(0), m_returnedChunk0(false)
{}

rep_set_t::~rep_set_t() {
    for (auto&& reply : m_reps)
        FreeGroupReply(reply);
    for (auto&& reply : m_errs)
        FreeGroupReply(reply);
}

rep_set_t rep_set_t::clone() const
{
    rep_set_t newset = rep_set_t();
    for (auto&& reply : m_reps) {
        newset.m_reps.push_back(CopyGroupReply(reply));
    }
    for (auto&& reply : m_errs) {
        newset.m_errs.push_back(CopyGroupReply(reply));
    }
    for (const auto& extra : m_unmergedChunks) {
        newset.m_unmergedChunks.push_back(extra.clone());
    }

    newset.m_repsIsSorted = m_repsIsSorted;
    newset.m_errsIsSorted = m_errsIsSorted;

    return newset;
}

// merge the reply from a single group_recv_t (\a tmp) into the reply set
// and delete \a tmp
void rep_set_t::merge(rep_set_t&& tmp) {
    std::vector<group_reply_t*> r;
    r.swap(tmp.m_reps);
    m_reps.insert(m_reps.end(), r.begin(), r.end());

    r.clear();
    r.swap(tmp.m_errs);
    m_errs.insert(m_errs.end(), r.begin(), r.end());

    m_returnedChunk0 = false;

    if (m_dataId != tmp.m_dataId) {
        fprintf(stderr, "data ids do not match\n");
    }

    if (tmp.m_set) {
        if (!m_set) {
            std::swap(m_set,tmp.m_set);
        } else {
            m_set->merge(std::move(*tmp.m_set));
        }
    }

    m_repsIsSorted = false;
    m_errsIsSorted = false;
}

int rep_set_t::hasError() const {
    return m_errs.size();
}

bool rep_set_t::hasOperationAbandonedError() const {
    return std::find_if(m_errs.begin(), m_errs.end(), [](auto&& rep) {
        // assume these are all DBGEV_ERROR events since we're in m_errs
        return rep->rep_event->ev_u.ev_u_err.err_errno == DBGERR_OPERATION_ABANDONED;
    }) != m_errs.end();
}

void rep_set_t::addReply(group_reply_t *rep)
{
    m_reps.push_back(rep);
    m_repsIsSorted = false;
}

void rep_set_t::addErr(group_reply_t* rep)
{
    m_errs.push_back(rep);
    m_errsIsSorted = false;
}

bool rep_set_t::isEmpty() const
{
    return m_reps.empty() && m_errs.empty();
}

size_t rep_set_t::getNumReplies()
{
    return getReplies().size();
}

group_reply_t* rep_set_t::getFirstReply()
{
    auto msgs = getReplies();
    return msgs.empty() ? nullptr : msgs.front();
}

const std::vector<group_reply_t*>& rep_set_t::getReplies()
{
    if (m_set) {
        fetchRemainingReplyChunks();
    }

    // a freshly cloned rep_set_t has no m_set but still has unmerged backing
    // data, which is why this check is separate from the m_set check.
    if (!m_unmergedChunks.empty()) {
        mergeReplyChunks();
    }

    sortReplies();

    return m_reps;
}

const std::vector<group_reply_t*>& rep_set_t::getErrs()
{
    if (m_set) {
        fetchRemainingReplyChunks();
    }

    // a freshly cloned rep_set_t has no m_set but still has unmerged backing
    // data, which is why this check is separate from the m_set check.
    if (!m_unmergedChunks.empty()) {
        mergeReplyChunks();
    }

    sortErrors();

    return m_errs;
}

// get the next reply chunk if we haven't already processed the last one
rep_set_t rep_set_t::getNextChunk() {
    if (!m_returnedChunk0) {
        auto chunk0 = rep_set_t();
        m_reps.swap(chunk0.m_reps);
        m_errs.swap(chunk0.m_errs);
        chunk0.m_repsIsSorted = m_repsIsSorted;
        chunk0.m_errsIsSorted = m_errsIsSorted;
        m_returnedChunk0 = true;
        m_repsIsSorted = m_errsIsSorted = true; // reps and errs now empty
        return chunk0;
    } else if (m_set) {
        return recvChunk();
    } else {
        return rep_set_t();
    }
}

void rep_set_t::setContinuation(const group_recv_t* set) {
    if (!set)
        return;

    if (!m_set) {
        m_set = std::make_unique<PickledRecSet>();
    }
    m_set->addReceiver(*set);
    m_returnedChunk0 = false;
}

void rep_set_t::setDataId(size_t id) { m_dataId = id; }

rep_set_t rep_set_t::recvChunk()
{
    if (!m_set) {
        fprintf(stderr, "no receptor for receiving the next chunk\n");
        return rep_set_t();
    }

    auto rec_set = m_set->instantiate();
    auto req = CommandToSocProto(DBGCMD_CONTINUEEVALEXPR, m_dataId);

    auto chunk = DbgSendCmdAndRecvReply(&rec_set, req, DBGEV_DATA);
    if (!chunk.isEmpty()) {
        m_set = std::move(chunk.m_set);
    } else {
        m_set.reset();
    }

    return chunk;
}

// Used to print out an error from a rep_set
int ProcessErrorInRepSet(rep_set_t& rep_set) {
    if (rep_set.hasError()) {
        for (auto&& err : rep_set.getErrs()) {
            if (err->rep_group) {
                PrintErrorWithSetInfo(err->rep_di.c_str(), err->rep_group.get(), "%s\n",
                                      DbgEventErrorStr(err->rep_event));
            } else {
                PrintOutError("%s\n", DbgEventErrorStr(err->rep_event));
            }
        }
        return 1;
    }

    return 0;
}

// Remove any event matching `ev` from `di`'s internal event list
static void DestroyEventSet(DbgInvoke *di, dbgev ev) {
    if (di == nullptr) return;

    di->eventSets().remove_if([ev](const auto& eset){
        return ev < 0 || eset.event()->ev_event == ev;
    });
}

/*
** Add a async event to an event set
*/
static DbgInvoke::EventSet *AddEvent(DbgInvoke *di, const Bitset *group, dbgevent_t *event) {
    if (di == nullptr || group == nullptr || event == nullptr) return nullptr;

    for (auto& eventset : di->eventSets()) {
        // compare two events with the extra context a dbginvoke provides
        const auto eventsAreEqual = [](const DbgInvoke& di, const dbgevent_t& left, const dbgevent_t& right) -> bool {
            if (left.ev_event == DBGEV_BPHIT && right.ev_event == DBGEV_BPHIT) {
                // special behavior for BPHIT

                // additional special behavior for exit breakpoints
                auto leftIsExit = left.ev_bp->bp_type == DBGBP_EXIT_TYPE;
                auto rightIsExit = right.ev_bp->bp_type == DBGBP_EXIT_TYPE;
                if (leftIsExit || rightIsExit) {
                    // We consider all exit breakpoints to be equal
                    return leftIsExit == rightIsExit;
                }

                // To compare breakpoint hits, we need the extra context of the di's
                // logical breakpoint mapping. We can't just forward to CompareEvents()
                // because it lives in dbgcomm and doesn't know what a dbginvoke is.

                const auto& lbp1 = di.breakpoints().findByDbgBreakpoint(*left.ev_bp);
                const auto& lbp2 = di.breakpoints().findByDbgBreakpoint(*right.ev_bp);
                if (lbp1 != nullptr && lbp2 != nullptr) {
                    return lbp1->id() == lbp2->id();
                } else if ((lbp1 == nullptr) != (lbp2 == nullptr)) {
                    // one breakpoint found, other not found. most likely a
                    // breakpoint was deleted and the cached one doesn't exist
                    // any more. in any case, these events don't go together.
                    return false;
                } else {
                    // in rare cases it's possible for both breakpoints to have
                    // been deleted out from under us.
                    // comparing backend breakpoint ids is a pretty bad
                    // fallback, but it's the best we can do here
                    return left.ev_bp->bp_fe_id == right.ev_bp->bp_fe_id;
                }
            }

            // not a BPHIT; fall back to dbgcomm CompareEvents
            return CompareEvents(&left, &right) == EV_EQUAL;
        };

        if (eventsAreEqual(*di, *eventset.event(), *event)) {
            // Add this group to the event set
            eventset.group() |= *group;
            return &eventset;
        }
    }

    // If we get here we need to make a new event set
    di->eventSets().emplace_back(*di, *group, event);

    return &di->eventSets().back();
}

// This will remove the event set if an event of ev_event has been recieved for
// every rank in the set
static void CheckForFullEventSet(DbgInvoke *di, const Bitset *fullset, dbgev ev) {
    if (di == nullptr) return;

    // A set that represents all ev that we have recieved so far
    auto aggset = fullset->clone();
    aggset->clear();

    // Create a set that contains all the ev events by looping over
    // every event
    for (const auto& eventset : di->eventSets()) {
        if (EVENT_EV(eventset.event()) == ev) {
            // Add it to the bitset
            *aggset |= eventset.group();
        }
    }

    // Check to see if the aggset is the same as the fullset
    if (*aggset == *fullset) {
        // An event of ev has been recieved for every rank in fullset, so
        // we should remove this event
        DPRINT(
            "CheckForFullEventSet: Recieved ev of %s for every rank. Removing "
            "event set.\n",
            DbgevToStr(ev));
        DestroyEventSet(di, ev);
    }
}

static void ResetForAsyncCmd(rec_set_t *set) {
    if (set == nullptr) return;

    // Remove the old async_set if there is one
    if (async_set != nullptr) FreeRecSet(async_set);

    async_set = NewRecSet();

    // Remove any old events for this group, and add each one to the async_set
    for (auto&& recv_ptr : set) {
        auto& eList = recv_ptr->rec_di.eventSets();
        for (auto it = eList.begin(); it != eList.end(); ) {
            // Remove the group from the bitset
            it->group() -= *recv_ptr->rec_group;

            // Make sure the e_group isn't empty, otherwise remove it
            if (it->group().isEmpty()) {
                it = eList.erase(it);
            } else {
                it++;
            }
        }

        // Add this group to the async_set
        AddReceiverToSet(
            async_set, NewGroupReceiver(recv_ptr->rec_di, recv_ptr->rec_group.get()));
    }
}

static void RemoveGroupFromAsyncSet(DbgInvoke *di, const Bitset *group) {
    if (di == nullptr || group == nullptr || async_set == nullptr) return;

    // Find entry for this di in the async_set
    for (auto&& recv_ptr : async_set) {
        if (recv_ptr->rec_di == *di) {
            // Remove the group from the bitset
            *recv_ptr->rec_group -= *group;

            // Check to see if the rec_group is now empty
            if (recv_ptr->rec_group->isEmpty()) {
                async_set->removeReceiver(recv_ptr);
            }

            // We are done, there is only one group per di in the async_set
            break;
        }
    }

    // Check to see if we have an empty async_set, if so the async events are
    // no longer pending
    if (async_set->getNumReceivers() == 0) {
        // The async event is no longer pending
        async_pending = 0;
        // cleanup the async_set
        FreeRecSet(async_set);
        async_set = nullptr;
    }
}

/*
**********************************************************
**********************************************************
*************** DEBUGGER API ROUTINES ********************
**********************************************************
**********************************************************
*/

/*
** Main debugger event handler.
**
** Returns: -1 if an error occurred
**		0 if there is nothing to do
**		1 if there are callbacks to process
**
** This now will check on dbginvoke_t objects rather than on generic protocol
** layer.
*/
int DbgCheckEvents(void) {
    int status;
    struct timeval tv = {0, 10000};

    // reset the fd_set
    FD_ZERO(&dispatch_fds);
    high_fd = -1;

    /*
    ** Check if any events are ready.
    */

    if (auto* script = script::getCurrentScript(); script && script->hasReadyAsserts()) {
        return 1;
    }
    // Loop through each dbginvoke entry and check for events.
    for (auto& dih : DbgInvoke::getAll()) {
        auto di = dih.lock();
        if (!di) {
            PrintOutError("Warning: no di %s.\n", dih.c_str());
            continue;
        }
        /*
         Note: This should be used for protocols that do not use an event
         notification fd. If a event notification fd can be used, then it 
         should be added to the sw_evfd list and sw_checkevents should 
         simply return 0.
        */
        if (di->proto().checkEvents(di) > 0) {
            DPRINT("DbgCheckEvents: sw_checkevents returned 1\n");
            return 1;  // there is a proto event to handle. Return immediately.
        }

        /*
        ** If there is a d_evfds fd set, we want to add it to the fd_set to
        *select on.
        ** This enables protocol events.
        */
        for (auto ev_fd : di->eventFds()) {
            if (high_fd < ev_fd) {
                high_fd = ev_fd;
            }
            FD_SET(ev_fd, &dispatch_fds);
        }
    }

    /*
    ** Add the input fd to the fd_set to select on. This enables input events.
    */
    for (auto& inputCB : InputCBs.inputs) {
        int ifd = inputCB->getFd();
        if (ifd >= 0) {
            if (high_fd < ifd) {
                high_fd = ifd;
            }
            // DPRINT("DbgCheckEvents: InputCB fd %d added.\n", InputCB.i_fd);
            FD_SET(ifd, &dispatch_fds);
        }
    }

    // from the select manpage, nfds is the highest-numbered file descriptor
    // in any of the three sets, plus 1. 
    status = select(high_fd + 1, &dispatch_fds, nullptr, nullptr, &tv);

    switch (status) {
        case -1:
            switch (errno) {
                case EINTR:
                    // just return 0 and try again next time the fn is called
                    DPRINT("DbgCheckEvents: EINTR\n");
                    return 0;

                // We expect EINVAL if there are no fd's in the fd_set. The
                // high_fd should be negative in this case and select will
                // return with -1 and errno set to EINVAL.
                case EINVAL:
                    DPRINT("DbgCheckEvents: EINVAL\n");
                    return 0;

                default:
                    PrintOutError("select: %s\n", strerror(errno));
                    return -1;
            }
            break;

        case 0:
            return 0;

        default:
            return 1;
    }

    // We should never reach here...
    DPRINT("DbgCheckEvents: At final return\n");
    return -1;
}

/*
** Dispactch pending debugger events.
*/
void DbgDispatchEvents(void) {
    // DPRINT("*** In DbgDispatchEvents\n");

    /*
    ** Now dispatch any callback routines that are active.
    */
    for (auto& dih : DbgInvoke::getAll()) {
        auto di = dih.lock();
        if (!di) {
            PrintOutError("Warning: no di %s.\n", dih.c_str());
            continue;
        }

        // if the proto is shutting down, don't dispatch further pending
        // events || if it didn't complete starting
        if (di->isShutdown() || !di->network()) continue;

        // If there is an d_evfds list, then the protol layer is using
        // event notification fd's. We should try to check to see if
        // each one is set, and if so call the protocol specific
        // dispatch function with the event fd as an argument
        if (!di->eventFds().empty()) {
            for (auto ev_fd : di->eventFds()) {
                // if the previous event caused a shutdown, don't
                // continue processing events
                if (di->isShutdown()) break;

                if (FD_ISSET(ev_fd, &dispatch_fds)) {
                    // dispatch each of the set notification fds one at
                    // a time.
                    di->proto().dispatchEvents(di, ev_fd);
                }
            }
        } else {
            // event notification fd's not in use by this protocol, so
            // call the generic dispatch function with -1 for event fd
            // arg.
            di->proto().dispatchEvents(di, -1);
        }
    }

    /*
    ** Process any input.
    */
    if (auto* input = InputCBs.get_active_input()) {
        DbgSwitchCurrentStream(input->getStream());
        input->invoke();
    }

    if (auto* script = script::getCurrentScript()) {
        if (!script->hasError()) {
            // process ready asserts
            if (!script->evaluateReadyAsserts()) {
                // error, printed below
            } else if (script->shouldStop()) {
                script->uninstall();
            } else if (!script->releaseReadyAsserts()) {
                // error, printed below
            } else if (script->allRanksBlocked()) {
                PrintOutError("Assertion script reached a point where every rank is stopped and no assertions can be evaluated.\n");
                script->uninstall();
            }
        }

        if (script->hasError()) {
            PrintOutError("Assertion script error: %s\n", script->getError().c_str());
            script->uninstall();
        }
    }
}

int DbgDispatchProtoEvents(DbgInvoke *di, const ProtoMessageSet &set) {
    dbgev last_event = DBGEV_NONE;
    DbgInvoke::EventSet *eventset = nullptr;
    dbgevent_t *event;

    for (const auto &msg : set) {
        // convert this protocol msg string to an event
        event = SocProtoToEvent(msg.getMessage());

        if (event == nullptr) {
            continue;
        }

        // switch on the event to check if the event changed the status or if
        // we need to do any special pre-processing
        switch (EVENT_EV(event)) {

            // At least one rank exited, set shutdown flag
            // Some WLMs / WLM configurations will kill the entire job when
            // one rank ends
            case DBGEV_EXIT:
                di->setShutdown();
                break;

            // These events coorespond to possible status changes
            case DBGEV_BPHIT: {
                auto* bp = EVENT_BP(event);
                if (bp && msg.getBitset() != nullptr) {
                    di->cacheFrame(msg.getBitset()->clone(), dbgloc(bp->bp_loc));
                }
                [[fallthrough]];
            }
            case DBGEV_BPDEL: {
                // Handle special pre-processing for breakpoints
                auto* bp = EVENT_BP(event);
                // Fixup any pending breakpoints
                di->breakpoints().fixupPendingBreakpoints(*bp, msg.getBitset());
                if (auto bp_set = di->breakpoints().findByDbgBreakpoint(*bp)) {
                    // if the breakpoint was a watchpoint we need to update the
                    // watchpoint locale information
                    switch (bp_set->type()) {
                        case DBGBP_WATCH_TYPE:
                        case DBGBP_AWATCH_TYPE:
                        case DBGBP_RWATCH_TYPE:
                            bp_set->addWatchLocale(msg.getBitset(), bp->bp_loc.loc_file, bp->bp_loc.loc_line, bp->bp_loc.loc_func);
                            break;

                        default:
                            // do nothing
                            break;
                    }
                }
                break;
            }
            case DBGEV_STEP: {
                const auto& step = event->ev_u.ev_u_step;
                if (step.step_frame && msg.getBitset() != nullptr) {
                    di->cacheFrame(msg.getBitset()->clone(), dbgloc(step.step_frame->getLoc()));
                }
                break;
            }
            case DBGEV_SIGNAL:
            case DBGEV_ERROR:
            default:
                // do nothing
                break;
        }

        // Handle async_set status for this message.
        RemoveGroupFromAsyncSet(di, msg.getBitset());

        eventset = AddEvent(di, msg.getBitset(), event);

        // This is the callback that was set by calling DbgHandleEvent
        di->callAsyncEventCallback(*eventset, *msg.getBitset());

        // conduct special post-processing for certain events
        switch (EVENT_EV(event)) {
            // Post process breakpoints
            case DBGEV_BPHIT: {
                auto* bp = EVENT_BP(event);
                if (auto bp_set = di->breakpoints().findByDbgBreakpoint(*bp)) {
                    // If the breakpoint was set to delete, we need to delete
                    // it. If it was set to dis, we need to disable it.

                    std::vector<std::unique_ptr<Bitset>> removeGroups;
                    for (const auto &disp_set : bp_set->dispSetList()) {
                        auto group = bitset::fromAnd(*disp_set.group, *msg.getBitset());
                        if (group->isEmpty()) {
                            continue;
                        }
                        switch (disp_set.disp) {
                            case DBGBP_DELETE_DISP:
                                // remove the breakpoint
                                // NOTE: The breakpoint has already been removed
                                // by the backend.
                                removeGroups.push_back(std::move(group));
                                break;
                            case DBGBP_DISABLE_DISP:
                                // mark the breakpoint as disabled for the ranks
                                // in the reply set.
                                bp_set->markDisabled(group.get());
                                break;
                            default:
                                // do nothing
                                break;
                        }
                    }
                    // remove any breakpoints that are now empty
                    for (const auto &bitset : removeGroups) {
                        bp_set->removeGroup(bitset.get());
                    }
                    di->breakpoints().removeEmptyBreakpoints();
                }
                break;
            }

            case DBGEV_BPDEL: {
                auto* bp = EVENT_BP(event);
                if (auto bp_set = di->breakpoints().findByDbgBreakpoint(*bp)) {
                    // Need to delete the breakpoint.
                    std::vector<std::unique_ptr<Bitset>> removeGroups;
                    for (const auto &disp_set : bp_set->dispSetList()) {
                        auto group = bitset::fromAnd(*disp_set.group, *msg.getBitset());
                        if (group->isEmpty()) {
                            continue;
                        }
                        // remove the breakpoint
                        // NOTE: The breakpoint has already been removed by the
                        // backend.
                        removeGroups.push_back(std::move(group));
                    }
                    // remove any breakpoints that are now empty
                    for (const auto &bitset : removeGroups) {
                        bp_set->removeGroup(bitset.get());
                    }
                    di->breakpoints().removeEmptyBreakpoints();
                }
                break;
            }

            default:
                // do nothing
                break;
        }

        // cleanup this event
        FreeEvent(event);

        /* If all events have been received, remove the eventset*/
        // I looked into this and it appears that right before calling step,
        // continue, and finish a call to RemoveEventSet is made since those
        // commands won't allow proceeding while the debugger is in a run status
        // state. I think there still might be a memory leak on error though.
        // Right now this will clear events where only some of the ranks have
        // reported in. Sometimes this can happen if only some of the ranks are
        // expected to hit a breakpoint.
        // FIXME: We should be smart about this. If a halt command is sent,
        //        then any pending events should be removed since its impossible
        //        for them to be hit. A change in status should also remove
        //        some types of events.
        // Right now this will only check for a full event set if the last
        // event recieved doesn't match the one we just recieved. This avoids
        // having to do this for each and every event we recieve if the
        // aggregation in the tree is particularly bad. If it is bad, we expect
        // to receive multiple events of the same type in a row.
        if (EVENT_EV(eventset->event()) != last_event) {
            // Only check if last_event is not DBGEV_NONE since this is the
            // default noneness case
            if (last_event != DBGEV_NONE) {
                DPRINT("Checking for full event set for %s\n",
                       DbgevToStr(last_event));
                // This will remove the eventset if we have recieved an event
                // of this type for every rank.
                CheckForFullEventSet(di, di->fullBitset(), last_event);
            }
            // set the last event to this event type
            last_event = EVENT_EV(eventset->event());
        }
    }

    // Check the last_event if its not noneness
    if (last_event != DBGEV_NONE) {
        DPRINT("Checking for full event set for %s\n", DbgevToStr(last_event));
        // This will remove the eventset if we have recieved an event
        // of this type for every rank.
        CheckForFullEventSet(di, di->fullBitset(), last_event);
    }

#ifdef DEBUG
    // Make sure we reset the stream since the async handler might have
    // modified it. This is so that we don't become confused when looking at the
    // debug output and wait for a prompt.
    extern void cli_reset_stream(void);

    cli_reset_stream();
#endif

    return 1;
}

void DbgDispatchProtoOutput(DbgInvoke *di, FILE *p_o_stream) {
    if (di == nullptr || p_o_stream == nullptr) return;

    // Call the output callback
    OutputCB.filestreamCallback(di, p_o_stream);
}

void DbgDispatchProtoOutput(DbgInvoke *di, const std::vector<std::string> &lines) {
    if (di == nullptr) return;

    // Call the output callback
    OutputCB.stringVectorCallback(di, lines);
}

/*
** Set client event handler
*/
void DbgHandleEvent(DbgInvoke *di, DbgInvoke::AsyncEventCallback ev_callback) {
    if (di == nullptr) return;

    // set the new callback
    di->setAsyncEventCallback(ev_callback);
}

/*
** Set local output stream handler.
*/
void DbgHandleOutput(const std::function<void (DbgInvoke *, FILE *)> &outp) {
    OutputCB.filestreamCallback = outp;  // callback handler
}

void DbgHandleOutput(const std::function<void (DbgInvoke *, const std::vector<std::string> &)> &outp) {
    OutputCB.stringVectorCallback = outp;  // callback handler
}

/*
** Set local input stream handler.
*/
void DbgHandleInput(int fd, cli_stream_t* stream, void (*inp)(void)) {
    InputCBs.add_input(fd, stream, inp);
}

void DbgUnhandleInput(cli_stream_t* stream) {
    InputCBs.remove_input(stream);
}

/*
** Mask input stream events temporarily
*/
void DbgMaskInput() {
    InputCBs.mask_all_inputs();
}

/*
** Remove temporary input stream mask.
*/
void DbgUnmaskInput() {
    InputCBs.unmask_all_inputs();
}

/*
** Unblock signal from within a signal handler
*/
void DbgUnmaskSignal(int sig) {
    sigset_t mask;

    sigemptyset(&mask);
    sigaddset(&mask, sig);
    sigprocmask(SIG_UNBLOCK, &mask, nullptr);
}

/*
** Set signal handler.
*/
void DbgHandleSignal(int sig, SigHandlerFn hand, dbgsig_type mask_type) {
    struct sigaction handler;

    if (hand == SIG_DFL || hand == SIG_IGN) {
        // ensure the signal is not in the normal or unblockable list
        auto loc = std::find(DbgSigList.begin(), DbgSigList.end(), sig);
        if (loc != DbgSigList.end()) {
            DbgSigList.erase(loc);
        }
        loc = std::find(DbgUnblockableSigList.begin(), DbgUnblockableSigList.end(), sig);
        if (loc != DbgUnblockableSigList.end()) {
            DbgUnblockableSigList.erase(loc);
        }
    } else {
        // Search the list, if the signal isn't in the list, then add it.
        auto loc = std::find(DbgSigList.begin(), DbgSigList.end(), sig);
        if (loc != DbgSigList.end()) {
            if (mask_type == DBG_SIG_NOMASK) {
                // signal is in wrong list
                DbgSigList.erase(loc);
                DbgUnblockableSigList.push_back(sig);
            }
        } else if ((loc=std::find(DbgUnblockableSigList.begin(), DbgUnblockableSigList.end(), sig)) !=
                   DbgUnblockableSigList.end()) {
            if (mask_type == DBG_SIG_MASKABLE) {
                // signal is in wrong list
                DbgUnblockableSigList.erase(loc);
                DbgSigList.push_back(sig);
            }
        } else {
            // signal isn't present, so create it and add to the proper list
            if (mask_type == DBG_SIG_NOMASK) {
                DbgUnblockableSigList.push_back(sig);
            } else {
                DbgSigList.push_back(sig);
            }
        }
    }

    // make sure to initialize the handler
    memset(&handler, 0, sizeof(handler));
    sigemptyset(&handler.sa_mask);

    // set the handler function
    handler.sa_handler = hand;

    sigaction(sig, &handler, nullptr);
}

/*
** Used to check if we are currently waiting for an async command to complete.
*/
int DbgCheckForAsyncPending(void) { 
    // return true if there's an async event waiting or there's a script running
    return async_pending || (script::getCurrentScript() != nullptr); 
}

/************************************
** dbgclnt_proto command functions **
************************************/

/*
** Initialise server
*/
int DbgInit(void) {
    for (const auto &[name, proto] : DebugClientProtocol::getRegistry()) {
        if (!proto->init()) return -1;
    }

    return 0;
}

void DbgFinalize(void) {
    for (const auto &[name, proto] : DebugClientProtocol::getRegistry()) {
        proto->finish();
    }

    DbgSigList.clear();
    DbgUnblockableSigList.clear();
    DbgInvoke::deactivateAll();
}

/*
** Attach to running program.
** Returns name of newly created di or "" on failure.
*/
DbgInvoke::Handle DbgAttach(const std::string& name, const AttachOpts& opts) {
    DebugClientProtocol * const ptab = findClientProtoByName(opts.proto.c_str());

    if (ptab == nullptr) {
        PrintOutError("No such protocol \"%s\"\n", opts.proto.c_str());
        return nullptr;
    }

    if (name.empty()) {
        PrintOutError("Empty di name\n");
    }

    // Create the dbginvoke_t object to pass down, nproc is unknown since this
    // is the attach case
    auto di = std::make_unique<DbgInvoke>(DbgInvoke::ProcessType::Attached, 0, *ptab);

    if (ptab->attach(di.get(), opts)) {
        return nullptr;
    }

    // Setting up a frontend dbg is currently disabled for the attach case since I have no
    // idea how to get the actual fullpath name of the a.out.
    // FIXME: How to set the debug frontend a.out???

    auto handle = DbgInvoke::activate(name, std::move(di));
    if (!handle.lock()) {
        return handle;
    }

    return handle;
}

/*
** Launch user program.
*/
DbgInvoke::Handle DbgLaunch(const char* name, int nproc, LaunchOpts&& args) {
    DebugClientProtocol * const ptab = findClientProtoByName(args.proto_name.c_str());
    dbgevent_t *e;

    if (ptab == nullptr) {
        PrintOutError("No such protocol \"%s\"\n", args.proto_name.c_str());
        return nullptr;
    }

    if (name == nullptr) {
        PrintOutError("Empty di name\n");
        return nullptr;
    }

    // Create the dbginvoke_t object to pass down
    auto di = std::make_unique<DbgInvoke>(DbgInvoke::ProcessType::Launched, nproc, *ptab);

    // Start out by creating the frontend gdb instance. We want to do some pre
    // checks on the a.out

    // The frontend debug presence can fail silently
    const auto& gdbPath = args.gdb_path;
    di->instantiateFrontendGdb(gdbPath.empty() ? nullptr : gdbPath.c_str());

    // Set the debug frontend a.out directly.
    if (auto fe = di->frontendGdb()) {

        // XXX: only checking first MPMD binary
        e = fe->setFile(args.job_opts[0].app_argv[0].c_str());

        // handle the error case
        if (e->ev_event == DBGEV_ERROR) {
            // We want to tear down the frontend since it has no way of reading
            // symbols since loading the binary failed.
            di->resetFrontendGdb();
        }

        FreeEvent(e);
    }

    // Check for startup function presence if unsafe is false
    if (!args.unsafe_flag && !args.job_opts[0].non_mpi_flag) {
        if (auto fe = di->frontendGdb()) {
            e = fe->checkStartupFunc();

            // handle the error case
            if (e->ev_event == DBGEV_ERROR) {
                // If this fails, we want to return with error since the user
                // application does not contain a required startup function.
                PrintOutError("%s\n", DbgEventErrorStr(e));
                PrintOutError(
                    "Application is missing required function for "
                    "startup.\n\n");
                PrintOutError(
                    "This is usually MPI_Init()/shmem_init()/etc. To bypass "
                    "this check,\n");
                PrintOutError(
                    "add the --non-mpi argument to launch, or\n");
                PrintOutError(
                    "use the command \"maint set unsafe on\". Note that your "
                    "application\n");
                PrintOutError("may hang during startup.\n");

                FreeEvent(e);

                return nullptr;
            }

            FreeEvent(e);
        }
    }

    for (auto&& job_opt : args.job_opts) {
        std::pair<std::string,std::unique_ptr<const char*[]>> trampolineArgs;
        if (job_opt.non_mpi_flag) {
            trampolineArgs = GetTrampolineArgs(job_opt.app_argv[0].c_str(), job_opt.app_argv.c_strs(1).data());
            if (trampolineArgs.first.empty()) {
                job_opt.non_mpi_flag = false;
            } else {
                ArgvStream newArgv;
                newArgv << trampolineArgs.first << trampolineArgs.second.get();
                job_opt.app_argv = newArgv;
                job_opt.run_argv = std::move(newArgv);
            }
        }
    }

    if (ptab->launch(di.get(), args)) {
        return nullptr;
    }

    di->launchOptions() = std::move(args);

    auto handle = DbgInvoke::activate(name, std::move(di));
    if (!handle.lock()) {
        return handle;
    }

    return handle;
}

// Receive a reply from the debug server
// \param set the the process set we expect a response from.
// \param req_type the event type we expect to be returned.   Any other event will
//      generate an error response.   (except for DBGEV_DATA_CHUNK is accepted in lieu of
//      DBG_DATA).
// \return the reply as a rep_set_t, note errors are returned with an error response.
static rep_set_t DbgRecvReply(group_recv_t *set, dbgev req_type, dbgev alt_req_type) {
    auto proto_res =
        set->rec_di.proto().receiveReply(&set->rec_di, set->rec_group.get(), sync_command_timeout);

    // handle nothing
    if (const auto* nothing = std::get_if<ProtoNothing>(&proto_res)) {
        using Why = ProtoNothing::Why;

        switch (nothing->why) {
            case Why::CancelledByUser: // fall through
            case Why::TimedOut:
            case Why::JobEnded: {
                const char* error_string = (nothing->why == Why::CancelledByUser)
                    ? "cancelled"
                    : (nothing->why == Why::TimedOut)
                        ? "timed out"
                        : "job ended";

                // omit di and group bitset from the error since this error will
                // eventually snowball into abandoning the command in *all* dis involved,
                // and to the user it's not important which di we were in when the cancel
                // happened.
                auto* error = NewErrorEvent(DBGERR_OPERATION_ABANDONED, error_string);
                auto reply = rep_set_t();
                auto* group_rep = NewGroupReply(nullptr, nullptr, error);
                reply.addErr(group_rep);
                return reply;
            }
            default: {
                auto* error = NewErrorEvent(DBGERR_INTERNAL, "Failed to receive response");
                auto reply = rep_set_t();
                auto* group_rep = NewGroupReply(set->rec_di.handle(), set->rec_group->clone(), error);
                reply.addErr(group_rep);
                return reply;
            }
        }
    }

    auto reply = rep_set_t();
    group_reply_t *rep = nullptr;
    bool isFinal = true;

    // process the proto_res
    for (const auto &msg : std::get<ProtoMessageSet>(proto_res)) {
        // convert this protocol msg string to an event
        auto e = SocProtoToEvent(msg.getMessage());
        auto ev = EVENT_EV(e);

        switch (ev) {
            // Errors get added to the special error list
            case DBGEV_ERROR:
                rep = NewGroupReply(set->rec_di.handle(), msg.getBitset()->clone(), e);
                // Add this error to the reply set
                reply.addErr(rep);
                break;

            // Any event without data should be handled here
            case DBGEV_OK:
                FreeEvent(e);
                if (ev != req_type && ev != alt_req_type) {
                    // Create a new error event based on this reply
                    e = NewErrorEvent(DBGERR_INTERNAL,
                                      "Received event of type %s, expected type %s.\n",
                                      DbgevToStr(ev), DbgevToStr(req_type));
                    rep = NewGroupReply(set->rec_di.handle(), msg.getBitset()->clone(), e);
                    // Add the error to the reply set
                    reply.addErr(rep);
                    break;
                }
                // Do nothing.
                break;

            case DBGEV_DATA_CHUNK:
                ev = DBGEV_DATA;
                isFinal = false;
                [[fallthrough]];

            default:
                if (ev != req_type && ev != alt_req_type) {
                    // Create a new error event based on this reply
                    FreeEvent(e);
                    EVENT_ERROR(
                        e, DBGERR_INTERNAL,
                        "Received event of type %s, expected type %s.\n",
                        DbgevToStr(ev), DbgevToStr(req_type));
                    rep = NewGroupReply(set->rec_di.handle(), msg.getBitset()->clone(), e);
                    // Add the error to the reply set
                    reply.addErr(rep);
                    break;
                }
                // Create a reply out of this event
                rep = NewGroupReply(set->rec_di.handle(), msg.getBitset()->clone(), e);
                // Add this reply to the reply set
                reply.addReply(rep);
                break;
        }
    }

    if (!isFinal) {
        // the reply-set will need the receiver to request the remainder of the data
        reply.setContinuation(set);
    }

    return reply;
}

void rep_set_t::pickle() {
    // if the rep_set isn't chunked, it's already in a pickled state
    if (!m_set) return;

    fetchRemainingReplyChunks();
}

void rep_set_t::fetchRemainingReplyChunks() {
    for (auto chunk = getNextChunk(); !chunk.isEmpty(); chunk = getNextChunk()) {
        m_unmergedChunks.push_back(std::move(chunk));
    }
}

// merge together a packetized reply into a monolithic rep_set
// Note that this is a short term function, the goal it to process the
// result chunks as they arrive; storing the full set of chunks
// in memory does not help (actually hinders) the memory usage on the client
// side
void rep_set_t::mergeReplyChunks()
{
    mergeReplyChunkSet(std::move(m_unmergedChunks));
}

// merge together a packetized reply into a monolithic rep_set
// Note that this is a short term function, the goal it to process the
// result chunks as they arrive; storing the full set of chunks
// in memory does not help (actually hinders) the memory usage on the client
// side
void rep_set_t::mergeReplyChunkSet(std::vector<rep_set_t>&& chunks) {
    // Each generation of packets was reduced separately, so we'll need to build
    // up new equivalence classes to get a combined result that was returned by
    // every process.   FIXME: this bit of code is temporary since it allows the
    // memory consumption in the client to grow unbounded.   So I won't try to make
    // this code super optimal.
    if (chunks.empty()) {
        return;
    } else if (chunks.size() == 1) {
        m_reps.swap(chunks[0].m_reps);
        std::swap(m_repsIsSorted, chunks[0].m_repsIsSorted);
        m_errs.swap(chunks[0].m_errs);
        std::swap(m_errsIsSorted, chunks[0].m_errsIsSorted);
        m_dataId = 0;
        m_returnedChunk0 = false;
        chunks.clear();
        return;
    }

    // find the set of dbg_invokes.   The chunk set for the delta output from
    // compare is sparse, so we can't just look at the first chunk.
    std::set<DbgInvoke::Handle> disSet;
    std::vector<DbgInvoke::Handle> dis;
    for (auto& chunk : chunks) {
        for (const auto& reply : chunk) {
            auto di = reply->rep_di;
            if (disSet.insert(di).second)
                dis.push_back(di);
        }
    }

    for (auto&& di : dis) {
        // merge the non-error and error replies into a single vector for easier
        // processing
        size_t numTiers = chunks.size();
        std::vector<std::vector<group_reply*>> tiers(numTiers);
        for (size_t i=0; i<numTiers; ++i) {
            rep_set_t& rs = chunks[i];
            auto& gs = tiers[i];
            for (int w=0; w<2; ++w) {
                const auto& l = (!w ? rs.getReplies() : rs.getErrs());
                for (auto&& reply : l) {
                    if (reply->rep_di == di) {
                        gs.push_back(reply);
                    }
                }
            }
        }

        // compute the set of processes left to process
        std::unique_ptr<Bitset> toproc = nullptr;
        for (auto&& r : tiers[0]) {
            if (toproc) {
                *toproc |= *r->rep_group;
            } else {
                toproc = r->rep_group->clone();
            }
        }

        // the algorithm here isn't too complicated.  We're just looking to pick
        // one group_reply from each tier which share at least one process.
        // Merge that set into a single group reply, removed the shared process nodes
        // from \a toproc and keep doing so until every process has been accounted for.
        std::vector<group_reply*> newChunk;
        auto setset = toproc->clone();
        while (!toproc->isEmpty()) {
            // find a common reply set
            newChunk.clear();
            *setset = *toproc;
            for (auto&& tier : tiers) {
                for (auto&& gp : tier) {
                    if (setset->hasCommonBits(*gp->rep_group)) {
                        *setset &= *gp->rep_group;
                        newChunk.push_back(gp);
                        break;
                    }
                }
            }

            // remove this set from the process set to process
            *toproc -= *setset;

            if (newChunk.size() != numTiers) {
                DPRINT("reply mismatch in MergeReplyChunks\n");
                break;
            }

            // update the set of replies left to process
            if (newChunk.size() == numTiers) {
                for (size_t i=0; i<numTiers; ++i) {
                    auto reply = newChunk[i];
                    auto& tier = tiers[i];
                    if (!reply->rep_group->hasCommonBits(*toproc)) {
                        // no processes left unprocessed - remove it from the
                        // list because we no longer need to look at it
                        tier.erase(std::find(tier.begin(), tier.end(), reply));
                    }
                }
            }

            // we have our set of overlapping replies, join the events together
            // and add to the monolithic return set.
            bool isError;
            if (group_reply_t* newReply = JoinReplies(setset.get(), newChunk, isError)) {
                if (isError) {
                    m_errs.push_back(newReply);
                    m_errsIsSorted = false;
                } else {
                    m_reps.push_back(newReply);
                    m_repsIsSorted = false;
                }
            }
        }
    }

    chunks.clear();
}

// return true if l->rep_group < r->rep_group
static bool compareRepsByBitset(const group_reply_t* l, const group_reply_t* r) {
    // nullptr < non-null ptr
    if (!l && !r) return false;        // nullptr == nullptr
    if (!l || !r) return l == nullptr; // else r is nullptr and thus < l

    // ->!rep_group < ->rep_group
    if (!l->rep_group && !r->rep_group) return false; // nullptr == nullptr
    if (!l->rep_group || !r->rep_group) return l->rep_group == nullptr;

    return *l->rep_group < *r->rep_group;
}

void rep_set_t::sortReplies() {
    if (m_repsIsSorted) return;
    std::sort(m_reps.begin(), m_reps.end(), compareRepsByBitset);
    m_repsIsSorted = true;
}

void rep_set_t::sortErrors() {
    if (m_errsIsSorted) return;
    std::sort(m_errs.begin(), m_errs.end(), compareRepsByBitset);
    m_errsIsSorted = true;
}

// Create a single unchunked group_reply from a set of chunks
// \a group the process set for the new result
// \a replies the chunks for the result
// \a shape if given is the array shape for the returned result.   This is necessary
//     if \a replies are a sparse representation, otherwise the function can
//     compute the shape by combining the array sizes in \a replies
// \a isError will contain true on return if any chunk in \a replies was an error
static group_reply_t* JoinReplies(const Bitset* group,
                                  const std::vector<group_reply*>& replies,
                                  bool& isError)
{
    // JoinReplies should be called on DATA replies - no other type is chunked
    auto di = DbgInvoke::Handle(nullptr);
    isError = false;
    for (auto&& reply : replies) {
        di = reply->rep_di;
        if (reply->rep_event->ev_event != DBGEV_DATA &&
            reply->rep_event->ev_event != DBGEV_DATA_CHUNK) {
            DPRINT("reply event is not data\n");
            isError = true;
        }
    }

    // AIFStream does the work to paste the data objects into a single object
    AIFStream* data = nullptr;
        
    if (!isError) {
        std::vector<AIFStream*> chunks;
        for (auto&& reply : replies) {
            chunks.push_back(EVENT_DATA(reply->rep_event));
        }

        data = new AIFStream(chunks);
    }


    // and create the group reply
    dbgevent_t* event = NewEvent(isError ? DBGEV_ERROR : DBGEV_DATA);
    if (data) {
        EVENT_DATA(event) = data;
    }

    group_reply_t* groupReply = NewGroupReply(di, group->clone(), event);
    return groupReply;
}

static rep_set_t DbgSendCmdAndRecvReply(rec_set_t *set, const std::string& request,
                                         dbgev req_type, dbgev alt_req_type) {
    auto result = rep_set_t();
    if (!set || set->getNumReceivers() == 0) return result;

    HoldSignals();

    // instead of a sequence of send, recv, send, recv... we want to send, send, send...
    // recv, recv, recv. But if a send fails, its recv would block forever, so we keep
    // track of which sends were successful in between sends and recvs.
    auto send_results = std::vector<bool>{};

    // Send the command to each di in the receiver set
    for (auto&& recv_ptr : set) {
        // send the command to this di
        send_results.push_back(
            recv_ptr->rec_di.proto().sendCommand(&recv_ptr->rec_di, recv_ptr->rec_group.get(), request.c_str()));
    }

    // Now recv the reply for each di in the receiver set
    for (size_t i = 0; i < set->getNumReceivers(); i++) {
        // recv the reply for this di, only if sencCommand was successful
        if (send_results[i] == true) {
            auto reply_set = DbgRecvReply(set->getReceivers()[i], req_type, alt_req_type);

            // check for an "operation cancelled" error. we are receiving results
            // serially from each di, but to the user it looks like one action. the
            // user should only have to cancel the operation once instead of once per di.
            //
            // we check for the cancel before MergeRepSet because MergeRepSet deletes reply_set.
            auto has_cancel = reply_set.hasOperationAbandonedError();

            // merge before potentially bailing out for the cancel so that the result
            // will include the error event.
            result.merge(std::move(reply_set));

            // FIXME: if we break out of the loop, some pending responses might
            // be abandoned, which might clog up the socket
            if (has_cancel) break;
        }
    }

    ReleaseSignals();

    return result;
}

/**********************************************************
** Start of non-proto specific commands                  **
** These functions may or may not require proto specific **
** components.                                           **
**********************************************************/

/*
** DBG_STARTUP: Startup the program (continue to Original Entry Point) and show
**				the frame.
*/
rep_set_t DbgStartup(rec_set_t *set) {
    auto request = CommandToSocProto(DBGCMD_STARTUP);
    rep_set_t result = DbgSendCmdAndRecvReply(set, request, DBGEV_FRAME);
    cacheFrames(result);
    return result;
}

// We associate a index with every expression evaluation that is used
// to accessed cached values on the server side.
static size_t DbgGetNextExprIdx()
{
    static size_t idx = 0;
    return ++idx;
}

/*
** DBGOPENSOCKET: Open socket for each debug server - for P2P
*/
rep_set_t DbgOpenSocket(rec_set_t *set, char *opset_name, int opset_size) {

    auto request = CommandToSocProto(DBGCMD_OPENSOCKET, opset_name, opset_size);

    rep_set_t result = DbgSendCmdAndRecvReply(set, request, DBGEV_BPSET);

    return result;
}

/*
** DBGCLOSESOCKET: Close socket for each debug server - for P2P
*/
rep_set_t DbgCloseSocket(rec_set_t *set) {
    auto request = CommandToSocProto(DBGCMD_CLOSESOCKET);
    return DbgSendCmdAndRecvReply(set, request, DBGEV_BPSET);
}

/*
** DBG_EVALEXPR: Evaluate expression.
*/
rep_set_t DbgEvalExpr(rec_set_t *set, const char *exp) {

    size_t exprId = DbgGetNextExprIdx();

    auto request = CommandToSocProto(DBGCMD_EVALEXPR, exprId, exp, set->getThreads());

    rep_set_t result = DbgSendCmdAndRecvReply(set, request, DBGEV_DATA);
    result.setDataId(exprId);

    return result;
}

/*
** DBG_SETDEBUGGERVAR: Set variable in debugger to new value.
*/
rep_set_t DbgSetDebuggerVar(rec_set_t *set, const char *var, AIFStream *val) {

    std::ostringstream valStr;
    val->print(valStr);

    auto request = CommandToSocProto(DBGCMD_SETDEBUGGERVAR, var, valStr.str());

    rep_set_t result = DbgSendCmdAndRecvReply(set, request, DBGEV_OK);
    return result;
}

/*
** DBG_GETDEBUGGERVAR: Get variable in debugger.
*/
rep_set_t DbgGetDebuggerVar(rec_set_t *set, const char *var) {
    auto request = CommandToSocProto(DBGCMD_GETDEBUGGERVAR, var);

    rep_set_t result = DbgSendCmdAndRecvReply(set, request, DBGEV_DATA);

    return result;
}

/*
** DBG_SETSOURCEPATH: Set source search path in debugger to new value.
*/
rep_set_t DbgSetSourcePath(rec_set_t *set, DbgDirOp op, const char *path) {
    rec_set_t *to_send = NewRecSet();
    auto result = rep_set_t();
    if (!set) return result;

    // We want to do the actual reading of the source code on the frontend if
    // possible. This means this command should be directed towards the
    // frontend if possible.
    // Loop over each di in the rec set
    for (auto&& recv_ptr : set) {
        // point at the debug frontend
        if (auto dbg_fe = recv_ptr->rec_di.frontendGdb()) {
            auto e = dbg_fe->setSourcePath(op, path);

            if (e->ev_event != DBGEV_SOURCEPATH && e->ev_event != DBGEV_ERROR) {
                // Free the event
                FreeEvent(e);
                // Create a new error event
                EVENT_ERROR(
                    e, DBGERR_DEBUGGER,
                    "DbgFEList returned unknown event other than error.");
            }

            // Add the event to the result based on its type
            auto group_rep = NewGroupReply(recv_ptr->rec_di.handle(),
                                           recv_ptr->rec_group->clone(), e);

            if (e->ev_event == DBGEV_ERROR) {
                result.addErr(group_rep);
            } else {
                result.addReply(group_rep);
            }

        } else {
            // Add this recv_ptr to the to_send list
            AddReceiverToSet(to_send, CopyGroupReceiver(recv_ptr));
        }
    }

    // check to see if there are any commands left to send
    if (GetNumberOfReceivers(to_send)) {
        auto request = CommandToSocProto(DBGCMD_SETSOURCEPATH, int(op), path);

        auto tmp_res = DbgSendCmdAndRecvReply(to_send, request, DBGEV_SOURCEPATH);

        // merge the results
        result.merge(std::move(tmp_res));
    }

    FreeRecSet(to_send);
    return result;
}

/*
** DBG_GETSOURCEPATH: Get source search path from debugger.
*/
rep_set_t DbgGetSourcePath(rec_set_t *set) {
    auto result = rep_set_t();
    if (!set) return result;

    auto to_send = NewRecSet();

    // We want to do the actual reading of the source code on the frontend if
    // possible. This means this command should be directed towards the
    // frontend if possible.
    // Loop over each di in the rec set
    for (auto&& recv_ptr : set) {
        // point at the debug frontend
        if (auto* dbg_fe = recv_ptr->rec_di.frontendGdb()) {
            auto e = dbg_fe->getSourcePath();

            if (e->ev_event != DBGEV_SOURCEPATH && e->ev_event != DBGEV_ERROR) {
                // Free the event
                FreeEvent(e);
                // Create a new error event
                EVENT_ERROR(
                    e, DBGERR_DEBUGGER,
                    "DbgFEList returned unknown event other than error.");
            }

            // Add the event to the result based on its type
            auto group_rep = NewGroupReply(recv_ptr->rec_di.handle(),
                                           recv_ptr->rec_group->clone(), e);

            if (e->ev_event == DBGEV_ERROR) {
                result.addErr(group_rep);
            } else {
                result.addReply(group_rep);
            }
        } else {
            // Add this recv_ptr to the to_send list
            AddReceiverToSet(to_send, CopyGroupReceiver(recv_ptr));
        }
    }

    // check to see if there are any commands left to send
    if (GetNumberOfReceivers(to_send)) {
        auto request = CommandToSocProto(DBGCMD_GETSOURCEPATH);

        auto tmp_res = DbgSendCmdAndRecvReply(to_send, request, DBGEV_SOURCEPATH);

        // merge the results
        result.merge(std::move(tmp_res));
    }

    FreeRecSet(to_send);
    return result;
}

rep_set_t DbgSetPrintElements(rec_set_t *set, const char *elements) {
    auto request = CommandToSocProto(DBGCMD_SETPRINTELEMENTS, elements);
    rep_set_t result = DbgSendCmdAndRecvReply(set, request, DBGEV_OK);
    return result;
}

rep_set_t DbgResetBeforeDirectCmd(rec_set_t *set) {
    auto request = CommandToSocProto(DBGCMD_RESETBEFOREDIRECTCMD);
    rep_set_t result = DbgSendCmdAndRecvReply(set, request, DBGEV_OK);
    return result;
}

rep_set_t DbgRestoreAfterDirectCmd(rec_set_t *set) {
    auto request = CommandToSocProto(DBGCMD_RESTOREAFTERDIRECTCMD);
    rep_set_t result = DbgSendCmdAndRecvReply(set, request, DBGEV_OK);
    return result;
}

/*
** DBG_SETVAR: Set variable in program to new value.
*/
rep_set_t DbgSetVar(rec_set_t *set, const char *var, const char* val) {
    auto request = CommandToSocProto(DBGCMD_SETVAR, var, val, set->getThreads());

    rep_set_t result = DbgSendCmdAndRecvReply(set, request, DBGEV_OK);

    return result;
}

/*
** DBG_GETTYPE: Get type of variable.
** If isType is true, then get type of type.
*/
rep_set_t DbgGetType(rec_set_t *set, const char *var, int isType) {
    auto request = CommandToSocProto(DBGCMD_GETTYPE, var, isType, set->getThreads());
    rep_set_t result = DbgSendCmdAndRecvReply(set, request, DBGEV_TYPE, DBGEV_THREADINFO);
    return result;
}

/*
** DBG_GO: Continue/start execution.
*/
rep_set_t DbgGo(rec_set_t *set, bool all) {
    // Remove any old events for this group
    ResetForAsyncCmd(set);

    auto request = CommandToSocProto(DBGCMD_GO, int(all), set->getThreads());

    auto result = DbgSendCmdAndRecvReply(set, request, DBGEV_OK);

    // Clear the list location state since this modified the frame only if there
    // was no error
    if (!result.hasError()) {
        for (auto&& recv_ptr : set) {
            recv_ptr->rec_di.clearFrontendGdb();
            recv_ptr->rec_di.setAtInitialBreakpoint(false);
        }
        if (set->getNumReceivers() > 0) {
            // update global status flags
            async_pending = 1;
            observers::target_resumed.notify();
        }
    } else {
        // selectively clear the list
        for (auto&& recv_ptr : set) {
            auto has_err = false;

            // Ensure this di is not in the error list
            for (auto&& err : result.getErrs()) {
                if (err->rep_di == recv_ptr->rec_di.handle()) {
                    // this di has an error
                    has_err = true;
                    break;
                }
            }

            // only proceed if there wasn't an error with this di
            if (!has_err) {
                recv_ptr->rec_di.clearFrontendGdb();

                // update global status flags
                async_pending = 1;
                observers::target_resumed.notify();
                recv_ptr->rec_di.setAtInitialBreakpoint(false);
            }
        }
    }

    return result;
}

/*
** DBG_FINISHFUNC: Continue execution until the end of the current function
*frame.
*/
rep_set_t DbgFinishFunc(rec_set_t *set) {
    if (!set) return rep_set_t();

    // Remove any old events for this group
    ResetForAsyncCmd(set);

    auto request = CommandToSocProto(DBGCMD_FINISH, set->getThreads());

    auto result = DbgSendCmdAndRecvReply(set, request, DBGEV_OK);


    // Clear the list location state since this modified the frame only if there
    // was no error
    if (!result.hasError()) {
        for (auto&& recv_ptr : set) {
            recv_ptr->rec_di.clearFrontendGdb();
        }
    } else {
        // selectively clear the list
        for (auto&& recv_ptr : set) {
            // reset the has_err
            auto has_err = 0;

            // Ensure this di is not in the error list
            for (auto&& err : result.getErrs()) {
                if (err->rep_di == recv_ptr->rec_di.handle()) {
                    // this di has an error
                    ++has_err;
                    break;
                }
            }

            // only proceed if there wasn't an error with this di
            if (!has_err) {
                recv_ptr->rec_di.clearFrontendGdb();
                recv_ptr->rec_di.setAtInitialBreakpoint(false);
            }
        }
    }

    // An async event is now pending
    async_pending = 1;
    observers::target_resumed.notify();

    return result;
}

static int nextBPID(rec_set_t* set) {
    auto id = int{1};
    for (auto rec : set) id = std::max(id, rec->rec_di.breakpoints().getNextID());
    return id;
}

/*
** DBG_SETLINEBREAK: Set a breakpoint in program at
**			specified line.
*/
rep_set_t DbgSetLineBreak(rec_set_t *set, int allowPending, const char *file, int line,
                           const char* threadSet) {

    auto id = nextBPID(set);

    auto request = CommandToSocProto(DBGCMD_SETLINEBREAK, id, allowPending, file, line, threadSet);

    auto result = DbgSendCmdAndRecvReply(set, request, DBGEV_BPSET);

    // process breakpoint information
    for (auto&& reply : result) {
        auto* di = reply->rep_di.lock();
        di->breakpoints().recordBreakpoint(*EVENT_BP(reply->rep_event), reply->rep_group.get());
    }

    g_bpnum = id;

    return result;
}

/*
** DBG_SETFUNCBREAK: Set a breakpoint in program at
**			specified function.
*/
rep_set_t DbgSetFuncBreak(rec_set_t *set, int allowPending, const char *file, 
        const char *func, const char* threadSet) {

    auto id = nextBPID(set);

    auto request = CommandToSocProto(DBGCMD_SETFUNCBREAK, id, allowPending, file, func, threadSet);

    auto result = DbgSendCmdAndRecvReply(set, request, DBGEV_BPSET);

    // process breakpoint information
    for (auto&& reply : result) {
        auto* di = reply->rep_di.lock();
        di->breakpoints().recordBreakpoint(*EVENT_BP(reply->rep_event), reply->rep_group.get());
    }

    g_bpnum = id;

    return result;
}

/*
** DBG_SETWATCH: Set a watchpoint in program for
**			specified expression.
*/
rep_set_t DbgSetWatch(rec_set_t *set, int type, char *expr) {

    auto id = nextBPID(set);

    auto request = CommandToSocProto(DBGCMD_SETWATCH, id, type, expr);

    rep_set_t result = DbgSendCmdAndRecvReply(set, request, DBGEV_BPSET);

    // process breakpoint information
    for (auto&& reply : result) {
        auto* di = reply->rep_di.lock();
        di->breakpoints().recordBreakpoint(*EVENT_BP(reply->rep_event), reply->rep_group.get());
    }

    g_bpnum = id;

    return result;
}

rep_set_t DbgSetCatch(rec_set_t *recSet, const std::string &event, const std::string &regex) {
    auto id = nextBPID(recSet);
    auto request = CommandToSocProto(DBGCMD_SETCATCH, id, event, regex);
    auto result = DbgSendCmdAndRecvReply(recSet, request, DBGEV_BPSET);

    // process catchpoint information
    for (const auto &reply : result) {
        auto* di = reply->rep_di.lock();
        di->breakpoints().recordBreakpoint(*EVENT_BP(reply->rep_event), reply->rep_group.get());
    }

    g_bpnum = id;

    return result;
}

rep_set_t DbgInfoBreakpoints(rec_set_t* set) {
    auto request = CommandToSocProto(DBGCMD_INFOBREAK);
    rep_set_t result = DbgSendCmdAndRecvReply(set, request, DBGEV_STRING);
    return result;
}

rep_set_t DbgSetCheckpoint(rec_set_t* recSet) {
    static auto nextId = 0u;

    auto request = CommandToSocProto(DBGCMD_SETCHECKPOINT);
    auto result = DbgSendCmdAndRecvReply(recSet, request, DBGEV_CHECKPOINT);

    if (!result.hasError()) {
        // bucket each reply by its di
        std::unordered_map<DbgInvoke*, std::vector<std::pair<const Bitset *, const dbgcheckpoint_t*>>> diMap;
        for (const auto reply : result) {
            if (!reply->rep_di.lock()) continue;
            diMap[reply->rep_di.lock()].push_back({reply->rep_group.get(), EVENT_CHECKPOINT(reply->rep_event)});
        }

        auto mostMade = 0u;

        // add checkpoint to each di
        for (auto& [di, checkpointV] : diMap) {
            auto produced = di->checkpoints().recordCheckpoint(nextId, checkpointV);

            // recordCheckpoint can make either one or two checkpoints each call,
            // see comments in recordCheckpoint
            mostMade = std::max(mostMade, produced);
            if (produced == 0) {
                PrintOutError("Failed to record checkpoint\n");
                PrintOutError("(%s)\n", di->checkpoints().getError().c_str());
            }
        }

        nextId += mostMade;
    }

    return result;
}

rep_set_t DbgRestoreCheckpoint(rec_set_t* recSet, unsigned int id) {
    if (!recSet) return rep_set_t();

    // add string error to rep_set_t result
    const auto addStrError = [](rep_set_t &result, const DbgInvoke& di, const Bitset &group, const std::string message) {
        result.addErr(NewGroupReply(di.handle(), group.clone(), NewErrorEvent(DBGERR_DEBUGGER, message.c_str())));
    };

    // preliminary error checking
    auto foundCheckp = std::any_of(begin(recSet), end(recSet),
        [&](const auto *rec) {
        return rec->rec_di.checkpoints().find(id) != nullptr;
    });

    using namespace std::string_literals;

    auto result = rep_set_t();

    if (!foundCheckp) {
        // just use the first di to generate the error
        auto first = recSet->getReceivers().front();
        addStrError(result, first->rec_di, *first->rec_group, 
            "Checkpoint "s + std::to_string(id) + " doesn't exist."
        );
        return result;
    }

    for (const auto rec : recSet) {
        auto checkp = rec->rec_di.checkpoints().find(id);

        // it's valid for the checkpoint to only exist on some dis and not others
        if (!checkp) continue;
        if (!checkp->currentSet().isEmpty()) continue;

        // send restart commands to individual ranks
        for (const auto &idSet : checkp->idSetList()) {
            auto group = bitset::fromAnd(*idSet.group, *rec->rec_group);
            if (group->isEmpty()) {
                continue;
            }

            auto new_rec_set = NewRecSet();
            AddReceiverToSet(new_rec_set, NewGroupReceiver(rec->rec_di, group.get()));

            // Get frames to update client side location data on success
            auto frames = DbgGetFrame(new_rec_set, false);
            if (frames.isEmpty()) {
                addStrError(result, rec->rec_di, *group, "Error fetching frame");
                continue;
            }
            for (const auto* frameReply : frames) {
                const auto &frameGroup = frameReply->rep_group;
                const auto &frameLoc = EVENT_FRAME(frameReply->rep_event)->getLoc();

                auto request = CommandToSocProto(DBGCMD_RESTORECHECKPOINT, idSet.id);

                // returns empty set on success
                if (auto tmp_result = DbgSendCmdAndRecvReply(new_rec_set, request, DBGEV_OK); tmp_result.isEmpty()) {
                    // update client side location data of any checkpoints that
                    // are current
                    for (auto &checkp : rec->rec_di.checkpoints()) {
                        if (checkp.currentSet().hasCommonBits(*frameGroup)) {
                            checkp.locations().replace(
                                bitset::fromAnd(*frameGroup, checkp.currentSet()).get(), 
                                frameLoc.loc_file, frameLoc.loc_line, frameLoc.loc_func
                            );
                        }
                        checkp.unmarkCurrent(*frameGroup);
                    }
                    rec->rec_di.checkpoints().find(id)->markCurrent(*frameGroup);
                } else {
                    // deletes tmp_result
                    result.merge(std::move(tmp_result));
                }
            }

            FreeRecSet(new_rec_set);
        }
    }

    return result;
}

rep_set_t DbgDeleteCheckpoint(rec_set_t* recSet, unsigned int id) {
    if (!recSet) return rep_set_t();

    // add string error to rep_set_t result
    const auto addStrError = [](rep_set_t &result, const DbgInvoke& di, const Bitset &group, const std::string message) {
        dbgevent_t *e;
        EVENT_ERROR(e, DBGERR_DEBUGGER, message.c_str());
        auto group_rep = NewGroupReply(di.handle(), group.clone(), e);
        result.addErr(group_rep);
    };

    using namespace std::string_literals;

    rep_set_t result = rep_set_t();

    // preliminary error checking
    auto foundCheckp = std::any_of(begin(recSet), end(recSet),
        [&](const auto* rec) {
        return rec->rec_di.checkpoints().find(id) != nullptr;
    });

    if (!foundCheckp) {
        // just use the first di
        auto first = recSet->getReceivers().front();
        addStrError(result, first->rec_di, *first->rec_group, 
            "Checkpoint "s + std::to_string(id) + " doesn't exist."
        );
        return result;
    }

    // more preliminary error checking
    for (const auto &rec : recSet) {
        const auto* checkpoint = rec->rec_di.checkpoints().find(id);

        // don't delete a checkpoint that is current somewhere
        if (!checkpoint->currentSet().isEmpty()) {
            addStrError(result, rec->rec_di, *rec->rec_group, 
                "Checkpoint "s + std::to_string(id) + " is the current checkpoint on $"
                + rec->rec_di.handle().c_str() + checkpoint->currentSet().toSetString() + "."
            );
            continue;
        }

        // deleting the original checkpoint thread (ID 0)
        // results in gdb killing the program, which we don't want
        auto IDZero = std::find_if(checkpoint->idSetList().begin(), checkpoint->idSetList().end(),
        [](const auto &idset) {
            return idset.id == 0;
        });
        if (IDZero != checkpoint->idSetList().end()) {
            addStrError(result, rec->rec_di, *rec->rec_group,
                "Checkpoint "s + std::to_string(id) + " is the original thread of execution on $"
                + rec->rec_di.handle().c_str() + IDZero->group->toSetString() + " and can't be directly deleted. "
                + "When all other checkpoints on $" + rec->rec_di.handle().c_str() + " are deleted, checkpoint " + std::to_string(id)
                + " will be implicitly deleted."
            );
        }
    }

    // return here if errors are detected
    if (result.hasError()) {
        return result;
    }

    for (const auto &rec : recSet) {
        auto checkp = rec->rec_di.checkpoints().find(id);

        // it's valid for the checkpoint to only exist on some dis and not others
        if (!checkp) continue;

        std::vector<std::unique_ptr<Bitset>> removeGroups;

        // send delete checkpoint commands to individual ranks
        for (const auto &idSet : checkp->idSetList()) {
            auto group = bitset::fromAnd(*idSet.group, *rec->rec_group);
            if (group->isEmpty()) {
                continue;
            }

            auto request = CommandToSocProto(DBGCMD_DELETECHECKPOINT, idSet.id);

            auto new_rec_set = NewRecSet();
            AddReceiverToSet(new_rec_set, NewGroupReceiver(rec->rec_di, group.get()));

            if (auto tmp_result = DbgSendCmdAndRecvReply(new_rec_set, request, DBGEV_OK); tmp_result.isEmpty()) {
                // will remove the group from the checkpoint after loop
                removeGroups.push_back(std::move(group));
            } else {
                // deletes tmp_result
                result.merge(std::move(tmp_result));
            }

            FreeRecSet(new_rec_set);
        }

        // groups that were removed with checkpoint 
        for (const auto &bitset : removeGroups) {
            checkp->removeGroup(*bitset.get());
        }

        // remove any empty checkpoints left by above
        rec->rec_di.checkpoints().removeEmpty();
        // if the deletion left a single checkpoint, gdb will implicitly delete it,
        // so reflect that in our client side data as well
        rec->rec_di.checkpoints().removeSingular();
    }

    return result;
}

// Set the current thread for subsequent commands
rep_set_t DbgSetThread(rec_set_t* set, const char* threadId)
{
    auto request = CommandToSocProto(DBGCMD_THREAD, threadId);

    rep_set_t result = DbgSendCmdAndRecvReply(set, request, DBGEV_OK);
    return result;
}

rep_set_t DbgSetThreadName(rec_set_t* set, const char* name)
{
    auto request = CommandToSocProto(DBGCMD_SETTHREADNAME, name);
    rep_set_t result = DbgSendCmdAndRecvReply(set, request, DBGEV_OK);
    return result;
}

rep_set_t DbgFindThread(rec_set_t* set, const char* pattern)
{
    auto request = CommandToSocProto(DBGCMD_FINDTHREAD,pattern);
    rep_set_t result = DbgSendCmdAndRecvReply(set, request, DBGEV_THREADINFO);
    return result;
}

rep_set_t DbgThreadApply(rec_set_t* set, const char* op)
{
    auto request = CommandToSocProto(DBGCMD_THREADAPPLY,op);
    rep_set_t result = DbgSendCmdAndRecvReply(set, request, DBGEV_THREADINFO);
    return result;
}

// Set the current thread for subsequent commands
rep_set_t DbgGetThreadInfo(rec_set_t* set, const char* threadList)
{
    auto request = CommandToSocProto(DBGCMD_GETTHREADINFO, threadList);

    rep_set_t result = DbgSendCmdAndRecvReply(set, request, DBGEV_THREADINFO);

    return result;
}

rep_set_t DbgSetSchedulerLocking(rec_set_t* set, const char* mode)
{
    auto request = CommandToSocProto(DBGCMD_SETSCHEDULERLOCKING,mode);
    rep_set_t result = DbgSendCmdAndRecvReply(set, request, DBGEV_OK);
    return result;
}

rep_set_t DbgGetSchedulerLocking(rec_set_t* set)
{
    auto request = CommandToSocProto(DBGCMD_GETSCHEDULERLOCKING);
    rep_set_t result = DbgSendCmdAndRecvReply(set, request, DBGEV_THREADINFO);
    return result;
}

rep_set_t DbgSetNonStop(rec_set_t* set, const char* mode)
{
    auto request = CommandToSocProto(DBGCMD_SETNONSTOP,mode);
    rep_set_t result = DbgSendCmdAndRecvReply(set, request, DBGEV_OK);
    return result;
}

rep_set_t DbgGetNonStop(rec_set_t* set)
{
    auto request = CommandToSocProto(DBGCMD_GETNONSTOP);
    rep_set_t result = DbgSendCmdAndRecvReply(set, request, DBGEV_THREADINFO);
    return result;
}

rep_set_t DbgOmpd(rec_set_t* set, const char* op)
{
    auto request = CommandToSocProto(DBGCMD_OMPD,op);
    rep_set_t result = DbgSendCmdAndRecvReply(set, request, DBGEV_THREADINFO, DBGEV_OK);
    return result;
}

rep_set_t DbgCuda(rec_set_t* set, const char* op)
{
    auto request = CommandToSocProto(DBGCMD_CUDA,op);
    rep_set_t result = DbgSendCmdAndRecvReply(set, request, DBGEV_THREADINFO, DBGEV_OK);
    return result;
}

rep_set_t DbgSleep(rec_set_t* rec_set, int seconds) {
    auto request = CommandToSocProto(DBGCMD_SLEEP, seconds);
    auto result = DbgSendCmdAndRecvReply(rec_set, request, DBGEV_OK);
    return result;
}

rep_set_t DbgShell(rec_set_t* recSet, int argc, const char * const * argv){
    auto request = CommandToSocProto(DBGCMD_SHELL, std::make_pair(argc, argv));
    auto result = DbgSendCmdAndRecvReply(recSet, request, DBGEV_STRING);
    return result;
}

rep_set_t DbgSignal(rec_set_t* recSet, const char* signame) {
    auto request = CommandToSocProto(DBGCMD_SIGNAL, signame);
    auto result = DbgSendCmdAndRecvReply(recSet, request, DBGEV_STRING);
    return result;
}

rep_set_t DbgPythonBacktrace(rec_set_t* rec) {
    auto req = CommandToSocProto(DBGCMD_PYBT);
    auto result = DbgSendCmdAndRecvReply(rec, req, DBGEV_FRAME);
    return result;
}

rep_set_t DbgPythonList(rec_set_t* rec, std::string_view list_arg) {
    auto req = CommandToSocProto(DBGCMD_PYLIST, list_arg);
    auto result = DbgSendCmdAndRecvReply(rec, req, DBGEV_STRING);
    return result;
}

rep_set_t DbgPythonMoveFrame(rec_set_t* rec, DbgFrameDir dir) {
    if (dir == DbgFrameDir::ABSOLUTE) {
        // this error case could be better, but with the currently
        // available py- commands, it will never happen
        return rep_set_t();
    }

    auto req = CommandToSocProto(DBGCMD_PYMOVEFRAME, int(dir));
    auto result = DbgSendCmdAndRecvReply(rec, req, DBGEV_FRAME);
    return result;
}

rep_set_t DbgPythonPrint(rec_set_t* rec, std::string_view name) {
    auto req = CommandToSocProto(DBGCMD_PYPRINT, name);
    auto result = DbgSendCmdAndRecvReply(rec, req, DBGEV_STRING);
    return result;
}

rep_set_t DbgPythonLocals(rec_set_t* rec) {
    auto req = CommandToSocProto(DBGCMD_PYLOCALS);
    auto result = DbgSendCmdAndRecvReply(rec, req, DBGEV_STRING);
    return result;
}

/*
** DBG_DELETEBREAK: Delete a breakpoint.
*/
rep_set_t DbgDeleteBreak(rec_set_t *set, int bp_set_num) {
    if (!set) return rep_set_t();

    auto result = rep_set_t();

    // if bp_set_num is 0, we want this to apply to every breakpoint associated
    // with each dbginvoke_t
    for (auto&& recv_ptr : set) {
        if (bp_set_num == 0) {
            for (auto &bp_set : recv_ptr->rec_di.breakpoints()) {
                // We send the command to the backend based on the backend gdb
                // id. Loop over this list
                std::vector<std::unique_ptr<Bitset>> removeGroups;
                for (const auto &id_set : bp_set.idSetList()) {
                    // Get the new group, based on the intersection between the
                    // bp_set and the rec_set.
                    auto group = bitset::fromAnd(*id_set.group, *recv_ptr->rec_group);

                    // Ensure that there are ranks in the bitset, otherwise
                    // continue
                    if (group->isEmpty()) {
                        continue;
                    }

                    // Create the request for this group
                    auto request =
                        CommandToSocProto(DBGCMD_DELETEBREAK, id_set.id);

                    // Create the new rec set for this group
                    auto new_rec_set = NewRecSet();
                    AddReceiverToSet(new_rec_set, NewGroupReceiver(recv_ptr->rec_di, group.get()));

                    auto tmp_result =
                        DbgSendCmdAndRecvReply(new_rec_set, request, DBGEV_OK);

                    // We expect this to return nullptr on success, handle the case
                    // were things worked
                    if (tmp_result.isEmpty()) {
                        // remove the group from the breakpoint
                        removeGroups.push_back(std::move(group));
                    }

                    // Add this to the overall result
                    result.merge(std::move(tmp_result));

                    FreeRecSet(new_rec_set);
                }
                for (const auto &bitset : removeGroups) {
                    bp_set.removeGroup(bitset.get());
                }
            }
            // remove any breakpoints that are now empty
            recv_ptr->rec_di.breakpoints().removeEmptyBreakpoints();
        } else {
            // find the bp_set for the bp_set_num
            auto bp_set = recv_ptr->rec_di.breakpoints().findByLogicalID(bp_set_num);
            if (bp_set == nullptr) {
                // Error case: the provided bp_set_num does not exist
                dbgevent_t *e;

                // Create a new error event
                EVENT_ERROR(e, DBGERR_DEBUGGER, "No breakpoint number %d.\n",
                            bp_set_num);
                // Add the event to the result based on its type
                auto group_rep =
                    NewGroupReply(recv_ptr->rec_di.handle(), recv_ptr->rec_group->clone(), e);
                // Create a new result and return it
                result.addErr(group_rep);
                continue;
            }

            // Need to loop over the idSetList() to operate on the correct
            // breakpoint number in the backend
            std::vector<std::unique_ptr<Bitset>> removeGroups;
            for (const auto &id_set : bp_set->idSetList()) {
                // Get the new group, based on the intersection between the
                // bp_set and the set.
                auto group = bitset::fromAnd(*id_set.group, *recv_ptr->rec_group);

                // Ensure that there are ranks in the bitset, otherwise continue
                if (group->isEmpty()) {
                    continue;
                }

                // Create the request for this group
                auto request =
                    CommandToSocProto(DBGCMD_DELETEBREAK, id_set.id);

                // Create the new rec set for this group
                auto new_rec_set = NewRecSet();
                AddReceiverToSet(new_rec_set, NewGroupReceiver(recv_ptr->rec_di, group.get()));

                auto tmp_result =
                    DbgSendCmdAndRecvReply(new_rec_set, request, DBGEV_OK);

                // We expect this to return nullptr on success, handle the case
                // were things worked
                if (tmp_result.isEmpty()) {
                    // remove the group from the breakpoint
                    removeGroups.push_back(std::move(group));
                }

                // Add this to the overall result
                result.merge(std::move(tmp_result));

                FreeRecSet(new_rec_set);
            }
            // remove any breakpoints that are now empty
            for (const auto &bitset : removeGroups) {
                bp_set->removeGroup(bitset.get());
            }
            recv_ptr->rec_di.breakpoints().removeEmptyBreakpoints();
        }
    }

    return result;
}

// Apply a modifying operation to a breakpoint, or possible all breakpoints
// 
static rep_set_t DbgModifyBreak(
    rec_set_t *set,
    int bp_set_num,
    const std::function<std::string(int)>& makeRequest,  // it is the (backend)  bp-id
    const std::function<void(MappedBreakpoint&,const Bitset*)>& updateBPs) {

    if (!set) return rep_set_t();

    auto result = rep_set_t();

    // if bp_set_num is 0, we want this to apply to every breakpoint associated
    // with each dbginvoke_t
    for (auto&& recv_ptr : set) {
        if (bp_set_num == 0) {
            for (auto &bp_set : recv_ptr->rec_di.breakpoints()) {
                // We send the command to the backend based on the backend gdb
                // id. Loop over this list
                for (const auto &id_set : bp_set.idSetList()) {
                    // Get the new group, based on the intersection between the
                    // bp_set and the rec_set.
                    auto group = bitset::fromAnd(*id_set.group, *recv_ptr->rec_group);

                    // Ensure that there are ranks in the bitset, otherwise
                    // continue
                    if (group->isEmpty()) {
                        continue;
                    }

                    // Create the request for this group
                    auto request = makeRequest(id_set.id);

                    // Create the new rec set for this group
                    auto new_rec_set = NewRecSet();
                    AddReceiverToSet(new_rec_set, NewGroupReceiver(recv_ptr->rec_di, group.get()));

                    auto tmp_result =
                        DbgSendCmdAndRecvReply(new_rec_set, request, DBGEV_OK);

                    // We expect this to return nullptr on success, handle the case
                    // were things worked
                    if (tmp_result.isEmpty()) {
                        updateBPs(bp_set, group.get());
                    }

                    // Add this to the overall result
                    result.merge(std::move(tmp_result));

                    FreeRecSet(new_rec_set);
                }
            }
        } else {
            auto bp_set = recv_ptr->rec_di.breakpoints().findByLogicalID(bp_set_num);

            // find the bp_set for the bp_set_num
            if (!bp_set) {
                // Error case: the provided bp_set_num does not exist
                dbgevent_t *e;

                // Create a new error event
                EVENT_ERROR(e, DBGERR_DEBUGGER, "No breakpoint number %d.\n",
                            bp_set_num);
                // Add the event to the result based on its type
                auto group_rep =
                    NewGroupReply(recv_ptr->rec_di.handle(), recv_ptr->rec_group->clone(), e);
                // Create a new result and return it
                result.addErr(group_rep);
                continue;
            }

            // Need to loop over the idSetList() to operate on the correct
            // breakpoint number in the backend
            for (const auto &id_set : bp_set->idSetList()) {
                // Get the new group, based on the intersection between the
                // bp_set and the rec_set.
                auto group = bitset::fromAnd(*id_set.group, *recv_ptr->rec_group);

                // Ensure that there are ranks in the bitset, otherwise continue
                if (group->isEmpty()) {
                    continue;
                }

                // Create the request for this group
                auto request = makeRequest(id_set.id);

                // Create the new rec set for this group
                auto new_rec_set = NewRecSet();
                AddReceiverToSet(new_rec_set, NewGroupReceiver(recv_ptr->rec_di, group.get()));

                auto tmp_result =
                    DbgSendCmdAndRecvReply(new_rec_set, request, DBGEV_OK);

                // We expect this to return nullptr on success, handle the case
                // were things worked
                if (tmp_result.isEmpty()) {
                    // mark the breakpoint as changed for this group
                    updateBPs(*bp_set, group.get());
                }

                // Add this to the overall result
                result.merge(std::move(tmp_result));

                FreeRecSet(new_rec_set);
            }
        }
    }

    return result;
}
    
/*
** DBG_DISABLEBREAK: Disable a breakpoint.
*/
rep_set_t DbgDisableBreak(rec_set_t *set, int bp_set_num) {
    auto makeRequest = [](int id) { return CommandToSocProto(DBGCMD_DISABLEBREAK, id); };
    auto disableBP = [](MappedBreakpoint& bp_set, const Bitset* group) { bp_set.markDisabled(group); };

    return DbgModifyBreak(set, bp_set_num, makeRequest, disableBP);
}

/*
** DBG_ENABLEBREAK: Enable a breakpoint.
*/
rep_set_t DbgEnableBreak(rec_set_t *set, int bp_set_num, bool once, bool del) {
    auto makeRequest = [once, del](int id) {
                           return CommandToSocProto(DBGCMD_ENABLEBREAK, id, once, del); };
    auto enableBP = [once, del](MappedBreakpoint& bp_set, const Bitset* group) {
                        bp_set.markEnabled(group);
                        // Set the disposition for this group
                        if (once) {
                            bp_set.markDisposition(group, DBGBP_DISABLE_DISP);
                        } else if (del) {
                            bp_set.markDisposition(group, DBGBP_DELETE_DISP);
                        }
                    };

    return DbgModifyBreak(set, bp_set_num, makeRequest, enableBP);
}

/*
** DBG_IGNOREBREAK: Set an ignore count on a breakpoint
*/
rep_set_t DbgIgnoreBreak(rec_set_t *set, int bp_set_num, int n) {
    auto makeRequest = [n](int id) {
                           return CommandToSocProto(DBGCMD_IGNOREBREAK, id, n); };
    auto ignoreBP = [n](MappedBreakpoint&, const Bitset*) {
                        // not marking it locally yet
                    };
    return DbgModifyBreak(set, bp_set_num, makeRequest, ignoreBP);
}

rep_set_t DbgConditionBreak(rec_set_t *set, int bp_set_num, const std::string& cond) {
    auto makeRequest = [cond](int id) {
                           return CommandToSocProto(DBGCMD_CONDITIONBREAK, id, cond); };
    auto conditionBP = [cond](MappedBreakpoint& bp_set, const Bitset*) {
                           bp_set.setCondition(cond);
                    };
    return DbgModifyBreak(set, bp_set_num, makeRequest, conditionBP);
}

    

/*
** DBG_LIST: List specified function or line.
*/
rep_set_t DbgList(rec_set_t *set, const char *str) {
    if (!set) return rep_set_t();

    auto result = rep_set_t();
    rec_set_t *to_send = NewRecSet();

    // We want to do the actual reading of the source code on the frontend if
    // possible. This is because there is a performance hit associated with
    // opening cross mounted files on the compute nodes. Its also the case that
    // sometimes we are unable to read the source files since they are located
    // in a non-cross mounted location.

    // Note that we can only do this if there is a debug frontend associated
    // with the dbginvoke

    // Loop over each di in the set
    for (auto&& recv_ptr : set) {
        if (auto dbg_fe = recv_ptr->rec_di.frontendGdb()) {
            // set the forward/reverse direction
            auto isReverse = (str == nullptr) ? false : (*str == '-');

            // First is the logic for the case where a previous list is not
            // pending. If str is null, then we need to ask each of the ranks
            // for their current frame.
            if ((!str || isReverse) && !dbg_fe->listIsPending()) {
                // Get the innermost frame.
                // Create the new rec set for this group - it holds state so it's not
                // directly re-usable

                auto new_rec_set = NewRecSet();
                AddReceiverToSet(new_rec_set, CopyGroupReceiver(recv_ptr));

                auto tmp_result = DbgShowFrame(new_rec_set, 1);
                FreeRecSet(new_rec_set);

                // Make sure there isn't an error in the result, otherwise add
                // this to the to_send list to get the list from the back end.
                if (tmp_result.hasError()) {
                    AddReceiverToSet(to_send, CopyGroupReceiver(recv_ptr));
                    continue;
                }

                // process the frame -- note that we may be adding replies to tmp_result
                // inside the loop.
                for (auto* group_rep : tmp_result) {
                    // Point at the event frame
                    const dbgframe_t* fp0 = EVENT_FRAME(group_rep->rep_event);
                    for (auto fp = fp0; fp; fp = fp->getNextThreadFrame()) {

                        const auto &loc = fp->getLoc();

                        // Now ask the frontend for the source listing around this
                        // location. This is set depending on the value of isReverse
                        std::ostringstream os;
                        if (isReverse) {
                            os << "," << loc.loc_file << ":" << loc.loc_line;
                        } else {
                            os << loc.loc_file << ":" << loc.loc_line << ",";
                        }
                        auto location = os.str();

                        // Save this new list location state for later calls to list
                        dbg_fe->addListStateLocation(group_rep->rep_group.get(), fp->getThreads(),
                                                     location.c_str());

                        // Send the list command to the frontend
                        auto e = dbg_fe->list(fp0->getThreads(), location.c_str());

                        // Make sure the event is a DBGEV_LIST, otherwise add this
                        // to the to_send list
                        if (e->ev_event != DBGEV_LIST) {
                            FreeEvent(e);
                            dbg_fe->clearListState();
                            AddReceiverToSet(to_send, CopyGroupReceiver(recv_ptr));
                            break; // for (fp)
                        }

                        // Replace the event in the group_rep with this one
                        FreeEvent(group_rep->rep_event);
                        if (fp == fp0) {
                            group_rep->rep_event = e;
                        } else {
                            auto newReply = NewGroupReply(group_rep->rep_di,
                                                          group_rep->rep_group->clone(), e);
                            tmp_result.addReply(newReply);
                        }
                    }
                }

                // This has converted the SHOWFRAME result into a LIST result.
                result.merge(std::move(tmp_result));

            } else if ((!str || isReverse) && dbg_fe->listIsPending()) {
                // We will create the result by hand based on the saved list
                // location state data

                // loop over every set_location
                for (auto& set_loc : dbg_fe->getListStateLocations()) {
                    // Call the list command using the saved location string to
                    // set things up
                    auto e = dbg_fe->list(set_loc);

                    // handle the error case
                    if (e->ev_event == DBGEV_ERROR) {
                        auto group_rep = NewGroupReply(recv_ptr->rec_di.handle(),
                                                       set_loc.set->clone(), e);
                        result.addErr(group_rep);
                        continue;
                    }

                    // Free the event
                    FreeEvent(e);

                    // Now we need to call list count times with direction
                    // depending on its sign.
                    auto dir = ((set_loc.count < 0) ? "-" : "");
                    auto cont = false;
                    for (auto i = 0, n = abs(set_loc.count); i < n; ++i) {
                        e = dbg_fe->list(set_loc.threads.get(), dir);
                        
                        // handle the error case
                        if (e->ev_event == DBGEV_ERROR) {
                            auto group_rep = NewGroupReply(recv_ptr->rec_di.handle(),
                                                           set_loc.set->clone(), e);
                            result.addErr(group_rep);
                            cont = true;
                            break;
                        }
                        // Free the event
                        FreeEvent(e);
                    }
                    if (cont) {
                        continue;
                    }

                    // Now we have everything setup for the actual list command
                    // we are interested in. The command depends on the
                    // isReverse value. We also increment or decrement the saved
                    // count here
                    if (isReverse) {
                        e = dbg_fe->list(set_loc.threads.get(), "-");
                        set_loc.count--;
                    } else {
                        e = dbg_fe->list(set_loc.threads.get(), "");
                        set_loc.count++;
                    }

                    // Make sure we got a DBGEV_LIST back, otherwise create an
                    // error
                    if (e->ev_event != DBGEV_LIST &&
                        e->ev_event != DBGEV_ERROR) {
                        // Free the event
                        FreeEvent(e);
                        // Create a new error event
                        EVENT_ERROR(e, DBGERR_DEBUGGER,
                                    "DbgFE::list returned unknown event other "
                                    "than error.");
                    }

                    // Add the event to the result based on its type
                    auto group_rep = NewGroupReply(recv_ptr->rec_di.handle(),
                                                   set_loc.set->clone(), e);

                    if (e->ev_event == DBGEV_ERROR) {
                        result.addErr(group_rep);
                    } else {
                        result.addReply(group_rep);
                    }
                }

                // This created the result from the saved list location
                // directly.

            } else if (str != nullptr && !isReverse) {
                // Note that we are going to create the result by hand here
                // based on the Bitset in the group_recv_t. If the user calls
                // the list cmd with a string, then all ranks are going to have
                // the same answer.

                // Make sure we clear the state if there was any saved
                dbg_fe->clearListState();

                // Save this new list location state for later calls to list
                dbg_fe->addListStateLocation(recv_ptr->rec_group.get(), nullptr, str);

                // we can pass the str directly to the frontend list command
                // since its not null and was provided by the user.
                auto e = dbg_fe->list(nullptr, str);

                // Make sure the event is a DBGEV_LIST or DBGEV_ERROR, otherwise
                // create an error
                if (e->ev_event != DBGEV_LIST && e->ev_event != DBGEV_ERROR) {
                    // Free the event
                    FreeEvent(e);
                    // Create a new error event
                    EVENT_ERROR(
                        e, DBGERR_DEBUGGER,
                        "DbgFE::list returned unknown event other than error.");
                }

                // Add the event to the result based on its type
                auto group_rep = NewGroupReply(recv_ptr->rec_di.handle(),
                                          recv_ptr->rec_group->clone(), e);

                if (e->ev_event == DBGEV_ERROR) {
                    result.addErr(group_rep);
                    // Make sure to clear the saved list state since there was
                    // an error with the provided string
                    dbg_fe->clearListState();
                } else {
                    result.addReply(group_rep);
                }

                // This created the result from the event directly.
            }
        } else {
            // add this recv_ptr to the to_send list
            AddReceiverToSet(to_send, CopyGroupReceiver(recv_ptr));
        }
    }

    // check to see if there are any commands left to send
    if (GetNumberOfReceivers(to_send)) {
        DPRINT("DbgList: Falling back to backend evaluation.\n");

        auto request = CommandToSocProto(DBGCMD_LIST, str);

        auto tmp_result = DbgSendCmdAndRecvReply(to_send, request, DBGEV_LIST);

        // merge the results
        result.merge(std::move(tmp_result));
    }

    FreeRecSet(to_send);
    return result;
}

/*
** DBG_GET_SOURCES: Ask for source files of inferior.
*/
rep_set_t DbgGetSources(rec_set_t *set) {
    if (!set) return rep_set_t();

    auto result = rep_set_t();
    auto to_send = NewRecSet();

    // We want to do the actual reading of the source files on the frontend if
    // possible. This is because there is a performance hit associated with
    // opening cross mounted files on the compute nodes. Its also the case that
    // sometimes we are unable to read the source files since they are located
    // in a non-cross mounted location.

    // Note that we can only do this if there is a debug frontend associated
    // with the dbginvoke

    // Loop over each di in the set
    for (auto&& recv_ptr : set) {
        dbgevent_t* e = nullptr;
        if (auto dbg_fe = recv_ptr->rec_di.frontendGdb()) {
            e = dbg_fe->getSources();
        }

        if (e && e->ev_event == DBGEV_SOURCES && EVENT_SOURCES(e) != nullptr) {
            // Add the event to the result based on its type
            auto group_rep = NewGroupReply(recv_ptr->rec_di.handle(),
                                           recv_ptr->rec_group->clone(), e);
            result.addReply(group_rep);
        } else {
            // Currently (2023) gdb-guard at version 10.0 does not recognize
            // the dwarf put out by the rocm compilers and will fail to find
            // any sources, so we have that case fall through here to give
            // rocgdb a chance on the back-end.
            FreeEvent(e);
            
            // add this recv_ptr to the to_send list to get the result from the
            // backend
            AddReceiverToSet(to_send, CopyGroupReceiver(recv_ptr));
        }
    }

    // check to see if there are any commands left to send
    if (GetNumberOfReceivers(to_send)) {
        DPRINT("DbgGetSources: Falling back to backend evaluation.\n");

        auto request = CommandToSocProto(DBGCMD_GETSOURCES);

        auto tmp_result = DbgSendCmdAndRecvReply(to_send, request, DBGEV_SOURCES);

        // merge the results
        result.merge(std::move(tmp_result));
    }

    FreeRecSet(to_send);
    return result;
}

/*
** DBG_SETARGS: Set arguments to pass to program when run.
*/
rep_set_t DbgSetArgs(rec_set_t *set, char *args) {

    auto request = CommandToSocProto(DBGCMD_SETARGS, args);

    rep_set_t result = DbgSendCmdAndRecvReply(set, request, DBGEV_OK);

    return result;
}

/*
** DBG_STEP: Execute to next statement. Repeat count times.
**		If in == 0, pass over function calls (ala next).
*/
rep_set_t DbgStep(rec_set_t *set, int count, int in) {
    // Remove any old events for this group
    ResetForAsyncCmd(set);

    auto request = CommandToSocProto(DBGCMD_STEP, count, in, set->getThreads());

    auto result = DbgSendCmdAndRecvReply(set, request, DBGEV_OK);

    // Clear the list location state since this modified the frame only if there
    // was no error
    if (!result.hasError()) {
        for (auto&& recv_ptr : set) {
            recv_ptr->rec_di.clearFrontendGdb();
            recv_ptr->rec_di.setAtInitialBreakpoint(false);
        }
    } else {
        // selectively clear the list
        for (auto&& recv_ptr : set) {
            auto has_err = false;

            // Ensure this di is not in the error list
            for (auto&& err : result.getErrs()) {
                if (err->rep_di == recv_ptr->rec_di.handle()) {
                    // this di has an error
                    has_err = true;
                    break;
                }
            }

            // only proceed if there wasn't an error with this di
            if (!has_err) {
                recv_ptr->rec_di.clearFrontendGdb();
                recv_ptr->rec_di.setAtInitialBreakpoint(false);
            }
        }
    }

    // An async event is now pending
    async_pending = 1;
    observers::target_resumed.notify();

    return result;
}

/*
** DBG_MOVEFRAME: Move up or down stack frames.
*/
rep_set_t DbgMoveFrame(rec_set_t *set, int count, DbgFrameDir dir) {
    if (!set) return rep_set_t();

    auto request = CommandToSocProto(DBGCMD_MOVEFRAME, count, int(dir), set->getThreads());

    auto result = DbgSendCmdAndRecvReply(set, request, DBGEV_FRAME);

    // Clear the list location state and set it to the new frame since this
    // modified the frame location. Only do it if it didn't have an error.
    // process DIs that did move the frame
    for (auto recv_ptr : set) {
        // reset the has_err
        auto hasErr = false;

        // Ensure this di is not in the error list
        for (auto&& err : result.getErrs()) {
            if (err->rep_di == recv_ptr->rec_di.handle()) {
                // this di has an error
                hasErr = true;
                break;
            }
        }

        if (hasErr) {
            continue;
        }

        if (auto dbg_fe = recv_ptr->rec_di.frontendGdb()) {
            // Clear any existing state
            dbg_fe->clearListState();

            // process the frame(s)
            for (auto&& group_rep : result) {
                // Point at the frame
                auto fp0 = const_cast<const dbgframe_t*>(EVENT_FRAME(group_rep->rep_event));
                for (auto fp = fp0; fp; fp = fp->getNextThreadFrame()) {
                    const auto &loc = fp->getLoc();

                    // Create the location string based on the new frame
                    // location Note that this should one short of the new
                    // location since we didn't generate this by listing
                    // source a first time. Otherwise we will start getting
                    // 10 lines after this location and it will be messed
                    // up.
                    std::ostringstream locStr;
                    locStr << "," << loc.loc_file << ":" << loc.loc_line - 1;

                    // Save this new list location state for later calls to
                    // list
                    dbg_fe->addListStateLocation(group_rep->rep_group.get(), fp->getThreads(),
                                                 locStr.str().c_str());
                }
            }
        }
    }

    return result;
}

/*
** DBG_SHOWFRAME: Show current stack.
*/
rep_set_t DbgShowFrame(rec_set_t *set, int count) {
    auto request = CommandToSocProto(DBGCMD_SHOWFRAME, count, set->getThreads());
    rep_set_t result = DbgSendCmdAndRecvReply(set, request, DBGEV_FRAME);
    return result;
}

/*
** DBG_GETFRAME: Get selected stack frame.
*/
rep_set_t DbgGetFrame(rec_set_t *set, bool verbose) {
    auto request = CommandToSocProto(DBGCMD_GETFRAME, int(verbose), set->getThreads());
    rep_set_t result = DbgSendCmdAndRecvReply(set, request, DBGEV_FRAME);
    cacheFrames(result);
    return result;
}

/*
** DBG_GETVARS: Return defined variables.
*/
rep_set_t DbgGetVars(rec_set_t *set) {
    auto request = CommandToSocProto(DBGCMD_GETVARS, set->getThreads());
    rep_set_t result = DbgSendCmdAndRecvReply(set, request, DBGEV_VARS, DBGEV_THREADINFO);
    return result;
}

// Return arguments of current frame
rep_set_t DbgGetArgs(rec_set_t *set) {
    auto request = CommandToSocProto(DBGCMD_GETARGS, set->getThreads());
    rep_set_t result = DbgSendCmdAndRecvReply(set, request, DBGEV_STRING, DBGEV_THREADINFO);
    return result;
}


/*
** DBG_GETSCOPE: Return variables local to a scope.
*/
rep_set_t DbgGetScope(rec_set_t *set, const char *location) {
    if (!set) return rep_set_t();

    rec_set_t *to_send = NewRecSet();
    auto result = rep_set_t();

    // We want to do the actual reading of the source files on the frontend if
    // possible. This is because there is a performance hit associated with
    // opening cross mounted files on the compute nodes. Its also the case that
    // sometimes we are unable to read the source files since they are located
    // in a non-cross mounted location.

    // Note that we can only do this if there is a debug frontend associated
    // with the dbginvoke

    // Loop over each di in the set
    for (auto&& recv_ptr : set) {
        if (auto dbg_fe = recv_ptr->rec_di.frontendGdb()) {
            auto e = dbg_fe->getScope(location);

            // Make sure the event is a DBGEV_VARS or DBGEV_ERROR, otherwise
            // create an error
            if (e->ev_event != DBGEV_VARS && e->ev_event != DBGEV_ERROR) {
                // Free the event
                FreeEvent(e);
                // Create a new error event
                EVENT_ERROR(
                    e, DBGERR_DEBUGGER,
                    "DbgFE::getScope returned unknown event other than error.");
            }

            // Add the event msg to the result based on its type
            auto group_rep = NewGroupReply(recv_ptr->rec_di.handle(),
                                      recv_ptr->rec_group->clone(), e);

            if (e->ev_event == DBGEV_ERROR) {
                result.addErr(group_rep);
            } else {
                result.addReply(group_rep);
            }
        } else {
            // add this recv_ptr to the to_send list
            AddReceiverToSet(to_send, CopyGroupReceiver(recv_ptr));
        }
    }

    // check to see if there are any commands left to send
    if (GetNumberOfReceivers(to_send)) {
        DPRINT("DbgGetScope: Falling back to backend evaluation.\n");

        auto request = CommandToSocProto(DBGCMD_GETSCOPE, location);

        auto tmp_result = DbgSendCmdAndRecvReply(to_send, request, DBGEV_VARS);

        // merge the results
        result.merge(std::move(tmp_result));
    }

    FreeRecSet(to_send);
    return result;
}

/*
** DBG_DIRECTCMD: Send direct command to debug backend
*/
rep_set_t DbgDirectCmd(rec_set_t *set, int argc, const char * const * argv) {

    auto request = CommandToSocProto(DBGCMD_DIRECT, std::make_pair(argc, argv));

    rep_set_t result = DbgSendCmdAndRecvReply(set, request, DBGEV_CMD);

    return result;
}

/*
** DBG_DBGSTATUS: Return status of debugger.
*/
rep_set_t DbgStatus(rec_set_t *set, int getLocation) {
    auto request = CommandToSocProto(DBGCMD_STATUS, getLocation);
    rep_set_t result = DbgSendCmdAndRecvReply(set, request, DBGEV_STAT);

    return result;
}

/*
** DBG_INTERRUPT: Interrupt current debugger operation.
*/
rep_set_t DbgInterrupt(rec_set_t *set, bool all) {
    auto request = CommandToSocProto(DBGCMD_INTERRUPT, all);

    auto result = DbgSendCmdAndRecvReply(set, request, DBGEV_HALT);

    // Clear the list location state since this modified the frame only if there
    // was no error
    if (!result.hasError()) {
        for (auto&& recv_ptr : set) {
            recv_ptr->rec_di.clearFrontendGdb();
        }
    } else {
        // selectively clear the list
        for (auto&& recv_ptr : set) {
            // reset the has_err
            auto has_err = false;

            // Ensure this di is not in the error list
            for (auto&& err : result.getErrs()) {
                if (err->rep_di == recv_ptr->rec_di.handle()) {
                    // this di has an error
                    has_err = true;
                    break;
                }
            }

            // only proceed if there wasn't an error with this di
            if (!has_err) {
                recv_ptr->rec_di.clearFrontendGdb();
            }
        }
    }

    return result;
}

/*
** DBG_QUIT: Tell server to quit immediately.
** Use the value of det to override the default behavior
*/
rep_set_t DbgQuit(DbgInvoke& di, DbgQuitType quit_type) {
    auto result = rep_set_t();

    // determine if we should detach or kill. this is based on the value of the
    // `quit_type` parameter. if it's Default, the choice of whether to detach or
    // kill is based on whether the job was launched in gdb4hpc or if gdb4hpc
    // attached to an external job. the caller can override this default with Kill
    // or Detach.
    auto detach = [](DbgQuitType passed_quit_type, DbgInvoke::ProcessType launch_type) {
        // for legacy reasons the value is an int.
        switch (passed_quit_type) {
        case (DbgQuitType::Kill): return 0;
        case (DbgQuitType::Detach): return 1;
        case (DbgQuitType::Default):
            switch (launch_type) {
            case DbgInvoke::ProcessType::Attached: return 1;
            case DbgInvoke::ProcessType::Launched: return 0;
            }
        }

        // should never get here
        PrintOutMsg("Warning: invalid quit/launch values %d/%d.\n",
            static_cast<int>(passed_quit_type), static_cast<int>(launch_type));
        return 0;
    }(quit_type, di.processType());

    auto job_running = true;
    auto all_quits_received = false;

    // Check via CTI with the WLM to see if the application is still running
    if (!cti_appIsValid(di.appId())) {

        PrintOutMsg("Job has already exited. Attempting to clean up...\n");
        job_running = false;

    } else {
        // send quit command. this should disconnect gdb on the backend and leave the
        // job in a stopped state. the communication network is still alive.
        auto quit_result = [](DbgInvoke& recipient, int detach) {
            auto to_send = NewRecSet();
            to_send->addReceiver(NewGroupReceiver(recipient, recipient.fullBitset()));

            auto request = CommandToSocProto(DBGCMD_QUIT, detach);
            auto tmp_result = DbgSendCmdAndRecvReply(to_send, request, DBGEV_OK);

            FreeRecSet(to_send);

            return tmp_result;
        }(di, detach);

        // nullptr means sucess and the blocking teardown will probably succeed.
        all_quits_received = quit_result.isEmpty();
        if (!all_quits_received) {

            // something went wrong with DBGCMD_QUIT, so sync exit probably won't work.
            PrintOutMsg("Warning: There was an error shutting down the debugger. Attempting to clean up...\n");
        }

        // merge the results. this deletes tmp_result
        result.merge(std::move(quit_result));
    }

    // Clean up the job.
    // In some circumstances like attach and --non-mpi launch, no exit breakpoint was set,
    // and it's possible that it ran until exit by now. We still try to send a signal, but
    // the job might be gone already. This isn't a critical error and the information
    // would be useless to the user, so only print an error if we originally launched the
    // app (not if we attached).
    auto appCleanup = [](DbgInvoke& di, bool job_running, int detach) {

        auto app_id = di.appId();
        auto handleStr = di.handle();
        auto process_type = di.processType();

        return [app_id, handleStr, process_type, job_running, detach]() {
            if (job_running) {
                if (detach) {
                    auto Launched = DbgInvoke::ProcessType::Launched;
                    if (cti_killApp(app_id, SIGCONT) && process_type == Launched) {
                        PrintOutMsg("Warning: an error occurred during release of $%s.\n",
                                    handleStr.c_str());
                        PrintOutMsg("The job might be stuck waiting for a SIGCONT.\n");
                    }

                    // Now that the app is running again, release it from cti so it won't be
                    // killed when cti is killed
                    if (cti_releaseApp(app_id)) {
                        PrintOutMsg("Warning: an error occurred during release of $%s.\n",
                                    handleStr.c_str());
                        PrintOutMsg("The job might be killed before completing.\n");
                    }

                } else {
                    auto Launched = DbgInvoke::ProcessType::Launched;
                    if (cti_killApp(app_id, SIGKILL) && process_type == Launched) {
                        PrintOutMsg("Warning: an error occurred during cleanup of $%s.\n", handleStr.c_str());
                        PrintOutMsg("Your job might still be running.\n");
                    }
                }
            }

            cti_deregisterApp(app_id);
        };
    }(di, job_running, detach);

    if (job_running && all_quits_received) {
        di.proto().syncExit(&di, appCleanup);
    } else {
        di.proto().forceExit(&di, appCleanup);
    }

    // cleanup the di object
    di.setAppId(0);
    di.destroyProtoOutput();
    DbgInvoke::deactivate(di.handle());

    return result;
}

static std::pair<std::string,std::unique_ptr<const char*[]>>
GetTrampolineArgs(const char* prog, const char* const* args)
{
    std::pair<std::string,std::unique_ptr<const char*[]>> retVal;

    auto baseDir = getinstallbase();
    if (!baseDir) {
        return retVal;
    }

    retVal.first = std::string{baseDir} + "/libexec/dbgsrv_trampoline";
    free(baseDir);

    auto n = size_t{0};
    for (auto it = args; *it; ++it) ++n;

    auto& targs = retVal.second = std::unique_ptr<const char*[]>{ new const char*[n+2] };

    targs[0] = prog;
    std::copy(args, args+n+1, &targs[1]);  // +1 for terminator

    return retVal;
}

inpcb_t::inpcb_t(int fd, cli_stream_t* stream, void (*f)()) :
    i_fd(fd), i_stream(stream), i_rtn(f), masked(0) {}

int inpcb_t::getFd() const {
    std::scoped_lock ilock(i_mutex);
    return masked ? -1 : i_fd;
}

void inpcb_t::mask(bool mask) {
    std::scoped_lock ilock(i_mutex);
    if (mask) ++masked; else --masked;
}

void DbgSetSyncCommandTimeout(std::chrono::seconds seconds) {
    sync_command_timeout = seconds;
}

std::chrono::seconds DbgGetSyncCommandTimeout() {
    return sync_command_timeout;
}

bool DbgSetLogDir(const std::string& logdir) {
    // while filesystem lets you check the permissions of a directory, the
    // only real way to check that a user can write to it is to
    // try to create a file.
    
    auto path = std::filesystem::path{logdir};

    // expand '~' does not seem to be recognized 
    if (path.begin() != path.end() && *path.begin() == "~" && getenv("HOME")) {
        auto newpath = std::filesystem::path{getenv("HOME")};
        for (auto it = path.begin(); ++it != path.end(); ) newpath /= *it;
        path = newpath;
    }

    auto tmpName = std::string{"gdb4hpc-"} + std::to_string(getpid()) + ".log";
    path /= tmpName;

    if (auto f = std::ifstream{path}; !f) {
        if (auto f = std::ofstream(path); !f) return false;
        std::filesystem::remove(path);
    }
    
    setenv(DBG_LOG_ENV_VAR, logdir.c_str(), 1);

    // Turning on CTI debugging is obnoxious because the pulse keeps printing to the terminal
    //setenv("CTI_DEBUG", "1", 1);
    //setenv("CTI_LOG_DIR", logdir.c_str(), 1);
    return true;
}

inpcb_t* InputList::get_active_input() {
    // traverse the list in reverse to give priority to newer inputs
    for (const auto& input : std::ranges::reverse_view(this->inputs)) {
        int ifd = input->getFd();
        if (ifd >= 0 && FD_ISSET(ifd, &dispatch_fds)) {
            return input.get();
        }
    }

    return nullptr;
}

void InputList::add_input(int fd, cli_stream_t* stream, void (*invoke_fn)(void)) {
    if (fd != -1){
        auto input = std::make_unique<inpcb_t>(fd, stream, invoke_fn);
        this->inputs.emplace_back(std::move(input));
    }
}

void InputList::remove_input(const cli_stream_t* stream) {
    auto loc = std::find_if(this->inputs.begin(), this->inputs.end(),
                            [stream] (auto& x) { return x->getStream() == stream; });
    if (loc != this->inputs.end()) {
        this->inputs.erase(loc);
    }
}

void InputList::mask_all_inputs() {
    for (auto& input : this->inputs) input->mask(true);
}

void InputList::unmask_all_inputs() {
    for (auto& input : this->inputs) input->mask(false);
}

int DbgGetBPNum() {
    return g_bpnum;
}

void cacheFrames(rep_set_t& repset) {
    for (const auto& rep : repset) {
        if (!rep || !rep->rep_group || !rep->rep_event || rep->rep_event->ev_event != DBGEV_FRAME || !rep->rep_event->ev_u.ev_u_frame) continue;

        if (auto* di = rep->rep_di.lock()) {
            di->cacheFrame(rep->rep_group->clone(), dbgloc(rep->rep_event->ev_u.ev_u_frame->getLoc()));
        }
    }
}
