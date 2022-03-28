/*********************************************************************************\
 * Debugger interface routines
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * Copyright 2010-2022 Hewlett Packard Enterprise Development LP.
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

#include <assert.h>
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>
#include <set>
#include <algorithm>
#include <mutex>

#include "assert_client.h"
#include "dbg_client.h"
#include "useful.h"
#include "gram.h"

/*
** Internal types
*/
struct inpcb_t {
    int i_fd;
    void (*i_rtn)();
    // This needs a lock because the MRNet error callback is threaded and will
    // mess with the inpcb_t object during printing error messages
    std::recursive_mutex i_mutex;
};
typedef struct inpcb_t inpcb_t;

struct OutputCallbacks {
    // Called by protocols when output from the program being debugged is detected.
    // The job of each callback is to output the data to the cli.
    // Different protocols deliver the data in different formats.
    
    std::function<void (dbginvoke *, FILE*)> filestreamCallback;
    std::function<void (dbginvoke *, const std::vector<std::string>&)> stringVectorCallback; 
};

/*
** Internal prototypes
*/
static dbginvoke_t *NewDbgInvoke(const char *, dbgproc_type, int,
                                DebugClientProtocol * const);
static void FreeDbgInvoke(dbginvoke_t *);
static rep_set_t *DbgRecvReply(group_recv_t *set, dbgev req_type, dbgev alt_req_type=DBGEV_NONE);
static void FreeGroupReceiver(group_recv_t *);
static void ClearCmdSent(group_recv_t *);
static group_recv_t *CopyGroupReceiver(group_recv_t *);
static int GetNumberOfReceivers(rec_set_t *);
static rep_set_t *MergeRepSet(rep_set_t *, rep_set_t *);
static void AddErrToSet(rep_set_t *, group_reply_t *);
//static void SetErrHeadInSet(rep_set_t *);
//static group_reply_t *GetNextErrInSet(rep_set_t *);
static event_set_t *NewEventSet(dbginvoke_t *, const Bitset *, dbgevent_t *);
static void FreeEventSet(event_set_t *);
static void DestroyEventSet(dbginvoke_t *, dbgev);
static event_set_t *AddEvent(dbginvoke_t *, const Bitset *, dbgevent_t *);
static void CheckForFullEventSet(dbginvoke_t *, const Bitset *, dbgev);
static void ResetForAsyncCmd(rec_set_t *);
static void RemoveGroupFromAsyncSet(dbginvoke_t *, const Bitset *);
static group_reply_t* JoinReplies(const Bitset* group,
                                  const std::vector<group_reply*>& replies,
                                  bool& isError);

static rep_set_t *DbgSendCmdAndRecvReply(rec_set_t *set, const std::string& request,
                                         dbgev req_type, dbgev alt_req_type=DBGEV_NONE);
static std::pair<std::string,std::unique_ptr<const char*[]>>
GetTrampolineArgs(const char* prog, const char* const* args);

/*
** Internal Variables
*/
static proto_session *DbgCurrSess;
static list_t *DbgInvokeList;
static std::vector<int> DbgSigList;
static std::vector<int> DbgUnblockableSigList;
static fd_set dispatch_fds;
static int high_fd;
static inpcb_t InputCB;
static OutputCallbacks OutputCB;
static int _mask_cnt;
static int _mask_fd;
static int _df_ready;
static int inferior_status_change;
static int async_pending;     // used to track completion of async events
static rec_set_t *async_set;  // full set of async receivers

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

/*
** Create a dbginvoke_t structure.
*/
static dbginvoke_t *NewDbgInvoke(const char *handle, dbgproc_type type,
                                 int nproc, DebugClientProtocol * const proto) {
    dbginvoke_t *di = new dbginvoke_t(proto);

    // Set non-zero members here
    di->d_handle = (handle != nullptr) ? strdup(handle) : nullptr;
    di->d_type = type;
    di->d_nproc = nproc;
    // di->d_proto = proto; // proto is set in constructor
    di->d_eventset_list = NewList();

    if (DbgInvokeList == nullptr) DbgInvokeList = NewList();

    /*
    ** Add to DbgInvokeList
    */
    AddToList(DbgInvokeList, (void *)di);

    return di;
}

/*
** Free a dbginvoke_t structure.
*/
static void FreeDbgInvoke(dbginvoke_t *di) {
    dbgevent_t *e;

    HoldSignals();

    RemoveFromList(DbgInvokeList, (void *)di);

    DBG_PROTO_SW(di)->destroyClient(di);

    // Quit the debug frontend if there is one
    if (di->d_frontend != nullptr) {
        e = DbgDestroyFE(di->d_frontend);
        FreeEvent(e);
    }

    di->d_bitset_full.reset();
    di->d_bitset_launched.reset();

    if (di->d_cmd != nullptr) Free(di->d_cmd);

    // remove any event fds from the proto
    DestroyList(di->d_evfds, Free);

    // remove the async lists
    DestroyList(di->d_eventset_list, (void (*)(void *))FreeEventSet);

    Free(di->d_handle);
    delete di;

    ReleaseSignals();
}

/**************************
** Receiver set functions
**************************/

/*
 * data structures for group receivers to dbgsrv
 */

group_recv_t *NewGroupReceiver(dbginvoke_t *di, const Bitset *proc_mask) {
    auto rec = new group_recv_t;
    rec->rec_di = di;

    DASSERT_NOT_NULL(proc_mask);
    rec->rec_group = proc_mask->clone();
    rec->_cmd_sent = std::unique_ptr<Bitset>();

    return rec;
}

group_recv_t* NewGroupReceiver(dbginvoke *di, std::unique_ptr<Bitset>&& mask) {
    auto rec = new group_recv_t;
    rec->rec_di = di;

    DASSERT_NOT_NULL(proc_mask.get());
    rec->rec_group = std::move(mask);
    rec->_cmd_sent = std::unique_ptr<Bitset>();

    return rec;
}

static void FreeGroupReceiver(group_recv_t *rec) {
    delete rec;
}

static void ClearCmdSent(group_recv_t *rec) {
    rec->_cmd_sent.reset();
}

static group_recv_t *CopyGroupReceiver(group_recv_t *rec) {
    if (rec == nullptr) return nullptr;

    auto newrec = new group_recv_t;

    newrec->rec_di = rec->rec_di;
    DASSERT_NOT_NULL(rec->rec_group);
    newrec->rec_group = rec->rec_group->clone();
    newrec->_cmd_sent = std::unique_ptr<Bitset>();

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
    for (auto&& recv : m_recs) {
        FreeGroupReceiver(recv);
    }
    free(m_threads);
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
            ._cmd_sent = rec->_cmd_sent
                ? rec->_cmd_sent->clone()
                : nullptr
        });
    }
    // copy thread string
    r->m_threads = m_threads
        ? strdup(m_threads)
        : nullptr
    ;

    return r;
}

void rec_set_t::setThreads(const char* threads)
{
    char* t = ( threads && threads[0] ? strdup(threads) : nullptr);
    std::swap(t, m_threads);
    free(t);
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
    if (!rec || !rec->rec_group || !rec->rec_di) return;
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

group_reply_t *NewGroupReply(dbginvoke_t *di, std::unique_ptr<Bitset> &&src, dbgevent_t *reply) {
    group_reply_t *rep = new group_reply_t;

    rep->rep_di = di;
    rep->rep_event = reply;
    rep->rep_group = std::move(src);

    return rep;
}

void FreeGroupReply(group_reply_t *rep) {
    FreeEvent(rep->rep_event);
    delete rep;
}

group_reply_t *CopyGroupReply(group_reply_t *rep) {
    if (rep == nullptr) return nullptr;

    auto newrep = new group_reply_t;

    newrep->rep_di = rep->rep_di;
    DASSERT_NOT_NULL(rep->rep_group);
    newrep->rep_group = rep->rep_group->clone();
    newrep->rep_event = DupEvent(rep->rep_event);

    return newrep;
}

/*
 * data structures for sets of group replies from dbgsrv
 */

rep_set_t *NewRepSet(void) {
    return new rep_set_t();
}

rep_set_t::rep_set_t()
    : m_set(nullptr), m_dataId(0), m_isSliced(false), m_returnedSlice0(false),
      m_moreSlices(false)
{}

rep_set_t::rep_set_t(const std::vector<std::unique_ptr<rep_set_t>>& slices, const Bitset* group)
    : rep_set_t()
{
    mergeReplySliceSet(slices, group);
}

void FreeRepSet(rep_set_t *set) {
    delete set;
}

rep_set_t::~rep_set_t() {
    for (auto&& reply : m_reps)
        FreeGroupReply(reply);
    for (auto&& reply : m_errs)
        FreeGroupReply(reply);

    if (m_set) {
        FreeRecSet(m_set);
    }
}

rep_set_t *CopyRepSet(rep_set_t *set) {
    return (set ? set->clone() : nullptr);
}

rep_set_t* rep_set_t::clone() const
{
    rep_set_t *newset = new rep_set_t();
    for (auto&& reply : m_reps) {
        newset->m_reps.push_back(CopyGroupReply(reply));
    }
    for (auto&& reply : m_errs) {
        newset->m_errs.push_back(CopyGroupReply(reply));
    }
    if (m_set) {
        newset->m_set = NewRecSet();
        for (auto&& recv : m_set) {
            AddReceiverToSet(newset->m_set, CopyGroupReceiver(recv));
        }
    }

    newset->m_dataId = m_dataId;
    newset->m_moreSlices = m_moreSlices;

    return newset;
}

static rep_set_t *MergeRepSet(rep_set_t *set, rep_set_t *tmp_set) {
    if (!set || !tmp_set) return nullptr;

    // merge the replies
    return set->merge(tmp_set);
}

// merge the reply from a single group_recv_t (\a tmp) into the reply set
// and delete \a tmp
rep_set_t* rep_set_t::merge(rep_set_t* tmp) {
    std::vector<group_reply_t*> r;
    r.swap(tmp->m_reps);
    m_reps.insert(m_reps.end(), r.begin(), r.end());

    r.clear();
    r.swap(tmp->m_errs);
    m_errs.insert(m_errs.end(), r.begin(), r.end());

    m_isSliced |= tmp->m_isSliced;
    m_moreSlices |= tmp->m_moreSlices;
    m_returnedSlice0 = false;

    if (m_dataId != tmp->m_dataId) {
        fprintf(stderr, "data ids do not match\n");
    }

    if (tmp->m_set) {
        if (!m_set) {
            std::swap(m_set,tmp->m_set);
        } else {
            for (auto&& recv : tmp->m_set) {
                AddReceiverToSet(m_set, CopyGroupReceiver(recv));
            }
        }
    }
    delete tmp;

    return this;
}

int SetHasError(rep_set_t *set) {
    return (set ? set->hasError() : 0);
}
int rep_set_t::hasError() const {
    return m_errs.size();
}

void AddReplyToSet(rep_set_t *set, group_reply_t *rep) {
    if (rep && set)
        set->addReply(rep);
}
void rep_set_t::addReply(group_reply_t *rep)
{
    m_reps.push_back(rep);
}

static void AddErrToSet(rep_set_t *set, group_reply_t *err_rep) {
    if (err_rep && set) {
        set->addErr(err_rep);
    }
}

void rep_set_t::addErr(group_reply_t* rep)
{
    m_errs.push_back(rep);
}

bool rep_set_t::isEmpty() const
{
    return m_reps.empty() && m_errs.empty();
}

size_t rep_set_t::getNumReplies() const
{
    return getReplies().size();
}

group_reply_t* rep_set_t::getFirstReply() const
{
    auto msgs = getReplies();
    return msgs.empty() ? nullptr : msgs.front();
}

const std::vector<group_reply_t*>& rep_set_t::getReplies() const
{
    if (m_moreSlices) {
        // make the single reply on demand if the processing doesn't handle
        // packetized.  -- delete the slices and clears the m_slices vector.
        const_cast<rep_set_t*>(this)->mergeReplySlices();
    }

    return m_reps;
}

std::vector<group_reply_t*>& rep_set_t::getMutableReplies()
{
    getReplies();
    return m_reps;
}

const std::vector<group_reply_t*>& rep_set_t::getErrs() const
{
    if (!m_moreSlices) {
        // make the single reply on demand if the processing doesn't handle
        // packetized.  -- delete the slices and clears the m_slices vector.
        const_cast<rep_set_t*>(this)->mergeReplySlices();
    }

    return m_errs;
}

// get the next reply slice if we haven't already processed the last one
std::unique_ptr<rep_set_t> rep_set_t::getNextSlice() {
    if (!m_returnedSlice0) {
        rep_set_t* slice0 = new rep_set_t;
        m_reps.swap(slice0->m_reps);
        m_errs.swap(slice0->m_errs);
        m_returnedSlice0 = true;
        return std::unique_ptr<rep_set_t>(slice0);
    } else if (m_isSliced && m_moreSlices) {
        return recvSlice();
    } else {
        return nullptr;
    }
}

void rep_set_t::setContinuation(group_recv_t* set) {
    if (!set)
        return;

    if (!m_set) {
        m_set = NewRecSet();
    }

    if (m_set) {
        AddReceiverToSet(m_set, CopyGroupReceiver(set));
    }
    m_returnedSlice0 = false;
    m_isSliced = m_moreSlices = true;
}

void rep_set_t::setDataId(size_t id) { m_dataId = id; }

std::unique_ptr<rep_set_t> rep_set_t::recvSlice()
{
    if (!m_set) {
        fprintf(stderr, "no receptor for receiving the next slice\n");
        return nullptr;
    }

    auto req = CommandToSocProto(DBGCMD_CONTINUEEVALEXPR, m_dataId);

    if (rep_set_t* slice = DbgSendCmdAndRecvReply(m_set, req, DBGEV_DATA)) {
        m_moreSlices = slice->m_moreSlices;
        slice->m_isSliced = slice->m_returnedSlice0 = slice->m_moreSlices = false;
        return std::unique_ptr<rep_set_t>(slice);
    } else {
        m_moreSlices = false;
        return nullptr;
    }
}

// keep an empty vector to iterator over when trying to use
// a rep_set_t* == nullptr
const std::vector<group_reply_t*>& rep_set_t::getNullReply()
{
    static std::vector<group_reply_t*> nullReply;
    return nullReply;
}

int GetNumberOfReply(rep_set_t *set) {
    return (set ? int(set->getNumReplies()): 0);
}

// Used to print out an error from a rep_set
int ProcessErrorInRepSet(rep_set_t *rep_set) {
    if (rep_set && rep_set->hasError()) {
        for (auto&& err : rep_set->getErrs()) {
            // Print out the error
            PrintErrorWithSetInfo(err->rep_di->d_handle, err->rep_group.get(), "%s\n",
                                  DbgEventErrorStr(err->rep_event));
        }
        return 1;
    }

    return 0;
}

/*****************************
** Event set for async events
*****************************/

/*
** Create a new event set
*/
static event_set_t *NewEventSet(dbginvoke_t *di, const Bitset *group,
                                dbgevent_t *event) {
    auto *eventset = new event_set_t;

    DASSERT_NOT_NULL(group);

    eventset->e_di = di;
    eventset->e_group = group->clone();
    eventset->e_ev = DupEvent(event);

    return eventset;
}

/*
** Free an event set
*/
static void FreeEventSet(event_set_t *eventset) {
    if (eventset == nullptr) return;

    if (eventset->e_ev != nullptr) FreeEvent(eventset->e_ev);

    delete eventset;
}

static void DestroyEventSet(dbginvoke_t *di, dbgev ev) {
    event_set_t *eventset;

    if (di == nullptr) return;

    SetList(di->d_eventset_list);
    while ((eventset = (event_set_t *)GetListElement(di->d_eventset_list)) !=
           nullptr) {
        if ((ev < 0) || (EVENT_EV(eventset->e_ev) == ev)) {
            RemoveFromList(di->d_eventset_list, (void *)eventset);
            FreeEventSet(eventset);
        }
    }
}

/*
** Add a async event to an event set
*/
static event_set_t *AddEvent(dbginvoke_t *di, const Bitset *group, dbgevent_t *event) {
    event_set_t *eventset;

    if (di == nullptr || group == nullptr || event == nullptr) return nullptr;

    SetList(di->d_eventset_list);
    while ((eventset = (event_set_t *)GetListElement(di->d_eventset_list)) != nullptr) {
        // compare two events with the extra context a dbginvoke provides
        const auto eventsAreEqual = [](const dbginvoke& di, const dbgevent_t& left, const dbgevent_t& right) -> bool {
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

                const auto& lbp1 = di.d_breakpoints.findByDbgBreakpoint(*left.ev_bp);
                const auto& lbp2 = di.d_breakpoints.findByDbgBreakpoint(*right.ev_bp);
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
                    return left.ev_bp->bp_id == right.ev_bp->bp_id;
                }
            }

            // not a BPHIT; fall back to dbgcomm CompareEvents
            return CompareEvents(&left, &right) == EV_EQUAL;
        };

        if (eventsAreEqual(*di, *eventset->e_ev, *event)) {
            // Add this group to the event set
            *eventset->e_group |= *group;
            return eventset;
        }
    }

    // If we get here we need to make a new event set
    eventset = NewEventSet(di, group, event);
    AddToList(di->d_eventset_list, eventset);

    return eventset;
}

// This will remove the event set if an event of ev_event has been recieved for
// every rank in the set
static void CheckForFullEventSet(dbginvoke_t *di, const Bitset *fullset, dbgev ev) {
    // individual event set
    event_set_t *eventset;

    // Return if di is null
    if (di == nullptr) return;

    // A set that represents all ev that we have recieved so far
    DASSERT_NOT_NULL(fullset);
    auto aggset = fullset->clone();
    aggset->clear();

    // Create a set that contains all the ev events by looping over
    // every event
    SetList(di->d_eventset_list);
    while ((eventset = (event_set_t *)GetListElement(di->d_eventset_list)) !=
           nullptr) {
        if (EVENT_EV(eventset->e_ev) == ev) {
            // Add it to the bitset
            *aggset |= *eventset->e_group;
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
    event_set_t *eventset;

    if (set == nullptr) return;

    // Remove the old async_set if there is one
    if (async_set != nullptr) FreeRecSet(async_set);

    async_set = NewRecSet();

    // Remove any old events for this group, and add each one to the async_set
    for (auto&& recv_ptr : set) {
        // Clear any pending events for the group we are sending an async
        // command to
        SetList(recv_ptr->rec_di->d_eventset_list);
        while ((eventset = (event_set_t *)GetListElement(
                    recv_ptr->rec_di->d_eventset_list)) != nullptr) {
            // Remove the group from the bitset
            DASSERT_NOT_NULL(eventset->e_group);
            DASSERT_NOT_NULL(recv_ptr->rec_group);
            *eventset->e_group -= *recv_ptr->rec_group;

            // Make sure the e_group isn't empty, otherwise remove it
            if (eventset->e_group->isEmpty()) {
                RemoveFromList(recv_ptr->rec_di->d_eventset_list,
                               (void *)eventset);
                FreeEventSet(eventset);
            }
        }

        // Add this group to the async_set
        AddReceiverToSet(
            async_set, NewGroupReceiver(recv_ptr->rec_di, recv_ptr->rec_group.get()));
    }
}

static void RemoveGroupFromAsyncSet(dbginvoke_t *di, const Bitset *group) {
    if (di == nullptr || group == nullptr || async_set == nullptr) return;

    // Find entry for this di in the async_set
    for (auto&& recv_ptr : async_set) {
        if (recv_ptr->rec_di == di) {
            // Remove the group from the bitset
            DASSERT_NOT_NULL(recv_ptr->rec_group);
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
    dbginvoke_t *di;
    int status;
    int res = 0;
    int *ev_fd;
    struct timeval tv = {0, 10000};

    // reset the fd_set
    FD_ZERO(&dispatch_fds);
    high_fd = -1;

    /*
    ** Check if any events are ready.
    */

    if (auto* script = script::getCurrentScript(); script && script->hasReadyAsserts()) {
        res = 1;
    }
 
    // Check the protocols for each dbginvoke if there are any.
    if (!EmptyList(DbgInvokeList)) {
        // Reset the DbgInvokeList
        SetList(DbgInvokeList);

        // Loop through each dbginvoke entry and check for events.
        while ((di = (dbginvoke_t *)GetListElement(DbgInvokeList)) != nullptr) {
            /*
             Note: This should be used for protocols that do not use an event
             notification fd. If a event notification fd can be used, then it 
             should be added to the sw_evfd list and sw_checkevents should 
             simply return 0.
            */
            if (DBG_PROTO_SW(di)->checkEvents(di) > 0) {
                DPRINT("DbgCheckEvents: sw_checkevents returned 1\n");
                return 1;  // there is a proto event to handle. Return immediately.
            }

            /*
            ** If there is a d_evfds fd set, we want to add it to the fd_set to
            *select on.
            ** This enables protocol events.
            */
            if (di->d_evfds != nullptr) {
                SetList(di->d_evfds);

                while ((ev_fd = (int *)GetListElement(di->d_evfds)) != nullptr) {
                    if (high_fd < *ev_fd) {
                        high_fd = *ev_fd;
                    }
                    FD_SET(*ev_fd, &dispatch_fds);
                }
            }
        }
    }

    /*
    ** Add the input fd to the fd_set to select on. This enables input events.
    */
    // grab the lock

    int ifd = -1;
    {
        std::scoped_lock<std::recursive_mutex> ilock(InputCB.i_mutex);
        ifd = InputCB.i_fd;
    }

    if (ifd >= 0) {
        if (high_fd < ifd) {
            high_fd = ifd;
        }
        // DPRINT("DbgCheckEvents: InputCB fd %d added.\n", InputCB.i_fd);
        FD_SET(ifd, &dispatch_fds);
    }

    /*
    ** Main select event polling loop.
    */
    for (;;) {
        // from the select manpage, nfds is the highest-numbered file descriptor
        // in any of the three sets, plus 1. 
        status = select(high_fd + 1, &dispatch_fds, nullptr, nullptr, &tv);

        switch (status) {
            case -1:
                switch (errno) {
                    case EINTR:
                        DPRINT("DbgCheckEvents: EINTR\n");
                        if (res) {
                            return res;
                        }
                        continue;

                    // We expect EINVAL if there are no fd's in the fd_set. The
                    // high_fd should be negative in this case and select will
                    // return with -1 and errno set to EINVAL.
                    case EINVAL:
                        DPRINT("DbgCheckEvents: EINVAL\n");
                        return res;

                    default:
                        PrintOutError("select: %s\n", strerror(errno));
                        return -1;
                }
                break;

            case 0:
                // DPRINT("DbgCheckEvents: select returned 0, res is %d\n",
                // res);
                return res;

            default:
                if (ifd >= 0) {
                    // If the InputCB was set, then we don't want to dispatch
                    // the graph. Commands can take awhile to return and if the
                    // user was typing something we don't want to issue a new
                    // command. The user has a second to type before being
                    // locked out.
                    if (FD_ISSET(ifd, &dispatch_fds)) {
                        _df_ready = 0;
                    }
                }
                return 1;
        }
    }

    // We should never reach here...
    DPRINT("DbgCheckEvents: At final return\n");
    return 1;
}

/*
** Dispactch pending debugger events.
*/
void DbgDispatchEvents(void) {
    dbginvoke_t *di;
    int *ev_fd;

    // DPRINT("*** In DbgDispatchEvents\n");

    /*
    ** Now dispatch any callback routines that are active.
    */
    if (!EmptyList(DbgInvokeList)) {
        // Reset the DbgInvokeList
        SetList(DbgInvokeList);

        // Loop through each dbginvoke entry and check for events.
        while ((di = (dbginvoke_t *)GetListElement(DbgInvokeList)) != nullptr) {
            // if the proto is shutting down, don't dispatch further pending
            // events
            if (di->d_isShutdown) continue;

            // If there is an d_evfds list, then the protol layer is using
            // event notification fd's. We should try to check to see if
            // each one is set, and if so call the protocol specific
            // dispatch function with the event fd as an argument
            if (di->d_evfds != nullptr) {
                SetList(di->d_evfds);

                while ((ev_fd = (int *)GetListElement(di->d_evfds)) != nullptr) {
                    // if the previous event caused a shutdown, don't
                    // continue processing events
                    if (di->d_isShutdown) break;

                    if (FD_ISSET(*ev_fd, &dispatch_fds)) {
                        // dispatch each of the set notification fds one at
                        // a time.
                        DBG_PROTO_SW(di)->dispatchEvents(di, *ev_fd);
                    }
                }
            } else {
                // event notification fd's not in use by this protocol, so
                // call the generic dispatch function with -1 for event fd
                // arg.
                DBG_PROTO_SW(di)->dispatchEvents(di, -1);
            }
        }
    }

    /*
    ** Process any input.
    */
    {
        std::scoped_lock<std::recursive_mutex> ilock(InputCB.i_mutex);
        if ((InputCB.i_fd >= 0) && FD_ISSET(InputCB.i_fd, &dispatch_fds) &&
            (InputCB.i_rtn != nullptr)) {
            // DPRINT("DbgDispatchEvents: InputCB.i_fd is set\n");
            InputCB.i_rtn();
        }
    }

    if (auto* script = script::getCurrentScript()) {
        if (!script->hasError()) {
            // process ready asserts
            if (!script->evaluateReadyAsserts()) {
                // error, printed below
            } else if (!script->releaseReadyAsserts()) {
                // error, printed below
            } else if (script->finished()) {
                PrintOutMsg("Assertion script $%s finished.\n", script->name().c_str());
                PrintOutMsg("Use \"info script $%s results\" to view results.\n", script->name().c_str());
                script->uninstall();
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

int DbgDispatchProtoEvents(dbginvoke_t *di, const ProtoMessageSet &set) {
    dbgev last_event = DBGEV_NONE;
    event_set_t *eventset = nullptr;
    dbgevent_t *event;
    dbgbp_t *bp;

    for (const auto &msg : set) {
        // convert this protocol msg string to an event
        event = SocProtoToEvent(msg.getMessage());

        if (event == nullptr) {
            continue;
        }

        // switch on the event to check if the event changed the status or if
        // we need to do any special pre-processing
        switch (EVENT_EV(event)) {
            // These events coorespond to possible status changes
            case DBGEV_BPHIT:
            case DBGEV_BPDEL:
                // Handle special pre-processing for breakpoints
                bp = EVENT_BP(event);
                // Fixup any pending breakpoints
                di->d_breakpoints.fixupPendingBreakpoints(*bp, msg.getBitset());
                if (auto bp_set = di->d_breakpoints.findByDbgBreakpoint(*bp)) {
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
                // fall through
            case DBGEV_STEP:
            case DBGEV_SIGNAL:
            case DBGEV_EXIT:
            case DBGEV_ERROR:
                // Check the status
                inferior_status_change = 1;
                break;

            default:
                // do nothing
                break;
        }

        // Handle async_set status for this message.
        RemoveGroupFromAsyncSet(di, msg.getBitset());

        eventset = AddEvent(di, msg.getBitset(), event);

        // This is the callback that was set by calling DbgHandleEvent
        di->d_clnt_ev_callback(eventset, msg.getBitset());

        // conduct special post-processing for certain events
        switch (EVENT_EV(event)) {
            // Post process breakpoints
            case DBGEV_BPHIT: {

                bp = EVENT_BP(event);
                if (auto bp_set = di->d_breakpoints.findByDbgBreakpoint(*bp)) {
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
                    di->d_breakpoints.removeEmptyBreakpoints();
                }
                break;
            }

            case DBGEV_BPDEL: {
                bp = EVENT_BP(event);
                if (auto bp_set = di->d_breakpoints.findByDbgBreakpoint(*bp)) {
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
                    di->d_breakpoints.removeEmptyBreakpoints();
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
        if (EVENT_EV(eventset->e_ev) != last_event) {
            // Only check if last_event is not DBGEV_NONE since this is the
            // default noneness case
            if (last_event != DBGEV_NONE) {
                DPRINT("Checking for full event set for %s\n",
                       DbgevToStr(last_event));
                // This will remove the eventset if we have recieved an event
                // of this type for every rank.
                CheckForFullEventSet(di, di->d_bitset_full.get(), last_event);
            }
            // set the last event to this event type
            last_event = EVENT_EV(eventset->e_ev);
        }
    }

    // Check the last_event if its not noneness
    if (last_event != DBGEV_NONE) {
        DPRINT("Checking for full event set for %s\n", DbgevToStr(last_event));
        // This will remove the eventset if we have recieved an event
        // of this type for every rank.
        CheckForFullEventSet(di, di->d_bitset_full.get(), last_event);
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

void DbgDispatchProtoOutput(dbginvoke_t *di, FILE *p_o_stream) {
    if (di == nullptr || p_o_stream == nullptr) return;

    // Call the output callback
    OutputCB.filestreamCallback(di, p_o_stream);
}

void DbgDispatchProtoOutput(dbginvoke_t *di, const std::vector<std::string> &lines) {
    if (di == nullptr) return;

    // Call the output callback
    OutputCB.stringVectorCallback(di, lines);
}

/*
** Set client event handler
*/
void DbgHandleEvent(dbginvoke_t *di, clnt_ev_callback_func ev_callback) {
    if (di == nullptr) return;

    // set the new callback
    di->d_clnt_ev_callback = ev_callback;
}

/*
** Set local output stream handler.
*/
void DbgHandleOutput(const std::function<void (dbginvoke *, FILE *)> &outp) {
    OutputCB.filestreamCallback = outp;  // callback handler
}

void DbgHandleOutput(const std::function<void (dbginvoke *, const std::vector<std::string> &)> &outp) {
    OutputCB.stringVectorCallback = outp;  // callback handler
}

/*
** Set local input stream handler.
*/
void DbgHandleInput(int fd, void (*inp)(void)) {
    std::scoped_lock<std::recursive_mutex> ilock(InputCB.i_mutex);
    InputCB.i_fd = fd;
    InputCB.i_rtn = inp;

    // reset input mask
    _mask_fd = -1;
    _mask_cnt = 0;
}

/*
** Mask input stream events temporarily
*/
void DbgMaskInput(void) {
    std::scoped_lock<std::recursive_mutex> ilock(InputCB.i_mutex);
    if (_mask_cnt == 0) {
        _mask_fd = InputCB.i_fd;
        InputCB.i_fd = -1;
    }
    ++_mask_cnt;
}

/*
** Remove temporary input stream mask.
*/
void DbgUnmaskInput(void) {
    std::scoped_lock<std::recursive_mutex> ilock(InputCB.i_mutex);
    if (_mask_cnt > 0) {
        --_mask_cnt;
    }
    if (_mask_cnt == 0) {
        InputCB.i_fd = _mask_fd;
    }
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
void DbgHandleSignal(int sig, void (*hand)(int), dbgsig_type mask_type) {
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
** Used to check if a command was called that changed the status
*/
int DbgCheckForStatusChange(void) { return inferior_status_change; }

/*
** Used to reset the inferior_status_change value to 0
*/
void DbgResetStatusChange(void) { inferior_status_change = 0; }

/*
** Used to check if we are currently waiting for an async command to complete.
*/
int DbgCheckForAsyncPending(void) { return async_pending; }

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

    // Initialize the mutex lock for the InputCB
    InputCB.i_fd = -1;
    return 0;
}

void DbgFinalize(void) {
    dbginvoke_t *di;

    // Cleanup the current session if there is one
    if (DbgCurrSess != nullptr) {
        DebugClientProtocol * const sw = DbgCurrSess->sess_proto;
        sw->cleanupSession(DbgCurrSess);
        DbgCurrSess = nullptr;
    }

    for (const auto &[name, proto] : DebugClientProtocol::getRegistry()) {
        proto->finish();
    }

    DbgSigList.clear();
    DbgUnblockableSigList.clear();

    // Manually free the elements in the dbginvoke list
    while ((di = (dbginvoke_t *)RemoveFirst(DbgInvokeList)) != nullptr) {
        FreeDbgInvoke(di);
    }
    DestroyList(DbgInvokeList, nullptr);
    DbgInvokeList = nullptr;

    InputCB.i_fd = -1;
}

/*
** Create a new session
*/
proto_session *DbgSessionCreate(const char *proto, WLM_type manager,
                                char *batch_template, char *hostname,
                                char *username) {
    DebugClientProtocol * const ptab = findClientProtoByName(proto);

    if (ptab == nullptr) {
        PrintOutError("No such protocol \"%s\"\n", proto);
        return nullptr;
    }

    return ptab->createSession(ptab, manager, batch_template, hostname, username);
}

/*
** Set the session to associate all subsequent launches/attaches with.
** If a session is currently set as being open, clean it up if possible.
** It's up to the protocol implementation to handle the actual cleanup.
*/
void DbgSetSession(proto_session *sess) {
    // Cleanup the current session if there is one
    if (DbgCurrSess != nullptr) {
        DebugClientProtocol * const ptab = DbgCurrSess->sess_proto;
        ptab->cleanupSession(DbgCurrSess);
    }

    // Ask the protocol layer to keep the new session around if its not null
    if (sess != nullptr) {
        DebugClientProtocol * const ptab = sess->sess_proto;
        ptab->saveSession(sess);
    }

    DbgCurrSess = sess;
}

/*
** Get the current session (if there is one)
*/
proto_session *DbgGetSession(void) { return DbgCurrSess; }

/*
** Attach to running program.
*/
dbginvoke_t *DbgAttach(const char *proto, proto_session *sess,
                       const char *handle, const char *app_ident,
                       const char *arch, const char *backend, int gpu_flag,
                       int debug_flag) {
    dbginvoke_t *di;
    DebugClientProtocol * const ptab = findClientProtoByName(proto);

    if (ptab == nullptr) {
        PrintOutError("No such protocol \"%s\"\n", proto);
        return nullptr;
    }

    // Create the dbginvoke_t object to pass down, nproc is unknown since this
    // is the attach case
    di = NewDbgInvoke(handle, DBG_ATTACHED, 0, ptab);

    if (ptab->attach(di, sess, app_ident, arch, backend, gpu_flag,
                        debug_flag)) {
        FreeDbgInvoke(di);
        return nullptr;
    }

    // The frontend debug presence can fail silently
    // This is currently disabled for the attach case since I have no idea how
    // to get the actual fullpath name of the a.out.
    // di->d_frontend = DbgInstantiateFE(backend);
    di->d_frontend = nullptr;

    // FIXME: How to set the debug frontend a.out???

    // This command changes the status
    inferior_status_change = 1;

    return di;
}

/*
** Launch user program.
*/
dbginvoke_t *DbgLaunch(const char *proto, proto_session *sess, const char *prog,
                       const char * const *argv, const char *handle, int nproc,
                       const char *arch, const char * const *arch_args,
                       const char *arch_input, const char *backend, const char* gdb,
                       char **env,
                       const char *cwd, int gpu_flag, int debug_flag, int non_mpi_flag,
                       int unsafe_flag, int early_entry_flag, bool isNonStop) {
    dbginvoke_t *di;
    DebugClientProtocol * const ptab = findClientProtoByName(proto);
    dbgevent_t *e;

    if (ptab == nullptr) {
        PrintOutError("No such protocol \"%s\"\n", proto);
        return nullptr;
    }

    // Create the dbginvoke_t object to pass down
    di = NewDbgInvoke(handle, DBG_LAUNCHED, nproc, ptab);

    // Start out by creating the frontend gdb instance. We want to do some pre
    // checks on the a.out

    // The frontend debug presence can fail silently
    di->d_frontend = DbgInstantiateFE(backend);

    // Set the debug frontend a.out directly.
    if (di->d_frontend != nullptr) {
        e = DbgFESetFile(di->d_frontend, prog);

        // handle the error case
        if (e->ev_event == DBGEV_ERROR) {
            // We want to tear down the frontend since it has no way of reading
            // symbols since loading the binary failed.
            FreeEvent(e);
            e = DbgDestroyFE(di->d_frontend);
            di->d_frontend = nullptr;
        }

        FreeEvent(e);
    }

    // Check for startup function presence if unsafe is false
    if (!unsafe_flag && !non_mpi_flag) {
        if (di->d_frontend != nullptr) {
            e = DbgFECheckStartupFunc(di->d_frontend);

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
                FreeDbgInvoke(di);

                return nullptr;
            }

            FreeEvent(e);
        }
    }

    std::pair<std::string,std::unique_ptr<const char*[]>> trampolineArgs;
    if (non_mpi_flag) {
        trampolineArgs = GetTrampolineArgs(prog, argv);
        if (trampolineArgs.first.empty()) {
            non_mpi_flag = false;
        } else {
            prog = trampolineArgs.first.c_str();
            argv = trampolineArgs.second.get();
        }
    }

    // Create the cmd str and set it
    di->d_cmd = ArgvStream(prog, argv).createBuffer();

    if (ptab->launch(di, sess, prog, argv, arch, arch_args, arch_input,
                     backend, gdb, env, cwd, gpu_flag, debug_flag,
                     non_mpi_flag,
                     early_entry_flag, isNonStop)) {
        FreeDbgInvoke(di);
        return nullptr;
    }

    // This command changes the status
    inferior_status_change = 1;

    return di;
}

static int DbgSendCmd(group_recv_t *set, const std::string& request) {
    dbginvoke_t *di = set->rec_di;

    set->_cmd_sent = DBG_PROTO_SW(di)->sendCommand(di, set->rec_group.get(), request.c_str());

    return (set->_cmd_sent != nullptr ? 0 : 1);
}

// Receive a reply from the debug server
// \param set the the process set we expect a response from.
// \param req_type the event type we expect to be returned.   Any other event will
//      generate an error response.   (except for DBGEV_DATA_SLICE is accepted in lieu of
//      DBG_DATA).
// \return the reply as a rep_set_t*, note errors are returned with an error response.
static rep_set_t *DbgRecvReply(group_recv_t *set, dbgev req_type, dbgev alt_req_type) {
    dbginvoke_t *di = set->rec_di;
    rep_set_t *reply;
    group_reply_t *rep;
    dbgevent_t *e;
    dbgev ev;

    bool isFinal = true;

    auto proto_res =
        DBG_PROTO_SW(di)->receiveReply(di, set->rec_group.get(), *set->_cmd_sent);

    reply = NewRepSet();

    if (!proto_res) {
        // Create a new error event based on this reply
        EVENT_ERROR(e, DBGERR_INTERNAL,
                    "Protocol returned null proto_msg_set.\n");
        rep = NewGroupReply(di, set->rec_group->clone(), e);
        // Add the error to the reply set
        AddErrToSet(reply, rep);
        ClearCmdSent(set);
        return reply;
    }

    // process the proto_res
    for (const auto &msg : *proto_res) {
        // convert this protocol msg string to an event
        e = SocProtoToEvent(msg.getMessage());
        ev = EVENT_EV(e);

        switch (ev) {
            // Errors get added to the special error list
            case DBGEV_ERROR:
                rep = NewGroupReply(di, msg.getBitset()->clone(), e);
                // Add this error to the reply set
                AddErrToSet(reply, rep);
                break;

            // Any event without data should be handled here
            case DBGEV_OK:
                FreeEvent(e);
                if (ev != req_type && ev != alt_req_type) {
                    // Create a new error event based on this reply
                    e = NewErrorEvent(DBGERR_INTERNAL,
                                      "Received event of type %s, expected type %s.\n",
                                      DbgevToStr(ev), DbgevToStr(req_type));
                    rep = NewGroupReply(di, msg.getBitset()->clone(), e);
                    // Add the error to the reply set
                    AddErrToSet(reply, rep);
                    break;
                }
                // Do nothing.
                break;

            case DBGEV_DATA_SLICE:
                ev = DBGEV_DATA;
                isFinal = false;
                // intentionally falling through

            default:
                if (ev != req_type && ev != alt_req_type) {
                    // Create a new error event based on this reply
                    FreeEvent(e);
                    EVENT_ERROR(
                        e, DBGERR_INTERNAL,
                        "Received event of type %s, expected type %s.\n",
                        DbgevToStr(ev), DbgevToStr(req_type));
                    rep = NewGroupReply(di, msg.getBitset()->clone(), e);
                    // Add the error to the reply set
                    AddErrToSet(reply, rep);
                    break;
                }
                // Create a reply out of this event
                rep = NewGroupReply(di, msg.getBitset()->clone(), e);
                // Add this reply to the reply set
                AddReplyToSet(reply, rep);
                break;
        }
    }

    // Check to see if there are no replies or errors in the reply set, if so we
    // want to return nullptr. This is because some of the event types have no
    // data. These functions will want to receive null, otherwise they assume
    // that there was an error
    if (reply->isEmpty()) {
        FreeRepSet(reply);
        reply = nullptr;
    } else if (!isFinal) {
        // the reply-set will need the receiver to request the remainder of the data
        reply->setContinuation(set);
    }
    ClearCmdSent(set);

    return reply;
}

// merge together a packetized reply into a monolithic rep_set
// Note that this is a short term function, the goal it to process the
// result slices as they arrive; storing the full set of slices
// in memory does not help (actually hinders) the memory usage on the client
// side
void rep_set_t::mergeReplySlices()
{
    if (!m_isSliced)
        return;

    // Each generation of packets was reduced separately, so we'll need to build
    // up new equivalence classes to get a combined result that was returned by
    // every process.

    std::vector<std::unique_ptr<rep_set_t>> slices;
    while (auto slice = getNextSlice()) {
        slices.push_back(move(slice));
    }

    mergeReplySliceSet(slices, nullptr);

    m_isSliced = m_returnedSlice0 = m_moreSlices =  false;
}

// merge together a packetized reply into a monolithic rep_set
// Note that this is a short term function, the goal it to process the
// result slices as they arrive; storing the full set of slices
// in memory does not help (actually hinders) the memory usage on the client
// side
void rep_set_t::mergeReplySliceSet(const std::vector<std::unique_ptr<rep_set_t>>& slices,
                                   const Bitset* group)
{
    // Each generation of packets was reduced separately, so we'll need to build
    // up new equivalence classes to get a combined result that was returned by
    // every process.   FIXME: this bit of code is temporary since it allows the
    // memory consumption in the client to grow unbounded.   So I won't try to make
    // this code super optimal.
    if (slices.empty()) {
        return;
    } else if (slices.size() == 1) {
        m_reps.swap(slices[0]->m_reps);
        m_errs.swap(slices[0]->m_errs);
        m_dataId = 0;
        m_isSliced = m_returnedSlice0 = m_moreSlices = false;
        return;
    }

    // find the set of dbg_invokes.   The slice set for the delta output from
    // compare is sparse, so we can't just look at the first slice.
    std::set<dbginvoke_t*> disSet;
    std::vector<dbginvoke_t*> dis;
    for (auto&& slice : slices) {
        for (auto&& reply : slice.get()) {
            auto di = reply->rep_di;
            if (disSet.insert(di).second)
                dis.push_back(di);
        }
    }

    for (auto&& di : dis) {
        // merge the non-error and error replies into a single vector for easier
        // processing
        size_t numTiers = slices.size();
        std::vector<std::vector<group_reply*>> tiers(numTiers);
        for (size_t i=0; i<numTiers; ++i) {
            rep_set_t& rs = *slices[i];
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
        if (group) {
            toproc = group->clone();
        } else {
            for (auto&& r : tiers[0]) {
                if (toproc)
                    *toproc |= *r->rep_group;
                else
                    toproc = r->rep_group->clone();
            }
        }

        // the algorithm here isn't too complicated.  We're just looking to pick
        // one group_reply from each tier which share at least one process.
        // Merge that set into a single group reply, removed the shared process nodes
        // from \a toproc and keep doing so until every process has been accounted for.
        std::vector<group_reply*> newSlice;
        auto setset = toproc->clone();
        while (!toproc->isEmpty()) {
            // find a common reply set
            newSlice.clear();
            *setset = *toproc;
            for (auto&& tier : tiers) {
                for (auto&& gp : tier) {
                    if (setset->hasCommonBits(*gp->rep_group)) {
                        *setset &= *gp->rep_group;
                        newSlice.push_back(gp);
                        break;
                    }
                }
            }

            // remove this set from the process set to process
            *toproc -= *setset;

            if (!group && newSlice.size() != numTiers) {
                DPRINT("reply mismatch in MergeReplySlices\n");
                break;
            }

            // update the set of replies left to process
            if (newSlice.size() == numTiers) {
                for (size_t i=0; i<numTiers; ++i) {
                    auto reply = newSlice[i];
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
            if (group_reply_t* newReply = JoinReplies(setset.get(), newSlice, isError)) {
                if (isError) {
                    m_errs.push_back(newReply);
                } else {
                    m_reps.push_back(newReply);
                }
            }
        }
    }
}

// Create a single unsliced group_reply from a set of slices
// \a group the process set for the new result
// \a replies the slices for the result
// \a shape if given is the array shape for the returned result.   This is necessary
//     if \a replies are a sparse representation, otherwise the function can
//     compute the shape by combining the array sizes in \a replies
// \a isError will contain true on return if any slice in \a replies was an error
static group_reply_t* JoinReplies(const Bitset* group,
                                  const std::vector<group_reply*>& replies,
                                  bool& isError)
{
    // JoinReplies should be called on DATA replies - no other type is sliced
    dbginvoke_t* di = nullptr;
    isError = false;
    for (auto&& reply : replies) {
        di = reply->rep_di;
        if (reply->rep_event->ev_event != DBGEV_DATA &&
            reply->rep_event->ev_event != DBGEV_DATA_SLICE) {
            DPRINT("reply event is not data\n");
            isError = true;
        }
    }

    // AIFStream does the work to paste the data objects into a single object
    AIFStream* data = nullptr;
        
    if (!isError) {
        std::vector<std::string> slices;
        for (auto&& reply : replies) {
            bool isFinal;
            slices.push_back(EVENT_DATA(reply->rep_event)->getNextSlice(isFinal));
        }

        data = new AIFStream(std::move(slices));
    }


    // and create the group reply
    dbgevent_t* event = NewEvent(isError ? DBGEV_ERROR : DBGEV_DATA);
    if (data) {
        EVENT_DATA(event) = data;
    }

    group_reply_t* groupReply = NewGroupReply(di, group->clone(), event);
    return groupReply;
}

static rep_set_t *DbgSendCmdAndRecvReply(rec_set_t *set, const std::string& request,
                                         dbgev req_type, dbgev alt_req_type) {
    if (!set) return nullptr;
    auto result = NewRepSet();

    HoldSignals();

    // Send the command to each di in the receiver set
    for (auto&& recv_ptr : set) {
        // send the command to this di
        DbgSendCmd(recv_ptr, request);
    }

    // Now recv the reply for each di in the receiver set
    for (auto&& recv_ptr : set) {
        // recv the reply for this di, only if there is a _cmd_sent set.
        // Otherwise there was an error
        if (recv_ptr->_cmd_sent != nullptr) {
            auto reply_set = DbgRecvReply(recv_ptr, req_type, alt_req_type);
            if (reply_set != nullptr) {
                result = MergeRepSet(result, reply_set);
            }
        }
    }

    ReleaseSignals();

    // If there is nothing in the result set, then return nullptr
    if (result->isEmpty()) {
        FreeRepSet(result);
        return nullptr;
    }

    // Return the result set
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
rep_set_t *DbgStartup(rec_set_t *set) {
    auto request = CommandToSocProto(DBGCMD_STARTUP);

    rep_set_t *result = DbgSendCmdAndRecvReply(set, request, DBGEV_FRAME);

    // This command changes the status
    inferior_status_change = 1;

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
** DBG_SENDEXPR: Send expression to BE.
*/
void DbgSendExpr(rec_set_t *set, char *exp) {
    if (!set) return;

    size_t id = DbgGetNextExprIdx();

    auto request = CommandToSocProto(DBGCMD_EVALEXPR, exp, id);

    for (auto&& recv_ptr : set) {
        // send the command to this di
        DbgSendCmd(recv_ptr, request);
    }

    return;
}

/*
** DBG_GETEXPR: Get results of evaluate expression.
*/
rep_set_t *DbgGetExpr(rec_set_t *set) {
    rep_set_t *result = NewRepSet();

    if (!set) return result;

    for (auto&& recv_ptr : set) {
        auto tmp_result = DbgRecvReply(recv_ptr, DBGEV_DATA);
        result = MergeRepSet(result, tmp_result);
    }

    return result;
}

/*
** DBGOPENSOCKET: Open socket for each debug server - for P2P
*/
rep_set_t *DbgOpenSocket(rec_set_t *set, char *opset_name, int opset_size) {

    auto request = CommandToSocProto(DBGCMD_OPENSOCKET, opset_name, opset_size);

    rep_set_t *result = DbgSendCmdAndRecvReply(set, request, DBGEV_BPSET);

    return result;
}

/*
** DBGCLOSESOCKET: Close socket for each debug server - for P2P
*/
rep_set_t *DbgCloseSocket(rec_set_t *set) {
    auto request = CommandToSocProto(DBGCMD_CLOSESOCKET);
    return DbgSendCmdAndRecvReply(set, request, DBGEV_BPSET);
}

/*
** DBG_EVALEXPR: Evaluate expression.
*/
rep_set_t *DbgEvalExpr(rec_set_t *set, const char *exp) {

    size_t exprId = DbgGetNextExprIdx();

    auto request = CommandToSocProto(DBGCMD_EVALEXPR, exprId, exp, set->getThreads());

    rep_set_t *result = DbgSendCmdAndRecvReply(set, request, DBGEV_DATA);
    if (result) {
        result->setDataId(exprId);
    }

    return result;
}

/*
** DBG_SETDEBUGGERVAR: Set variable in debugger to new value.
*/
rep_set_t *DbgSetDebuggerVar(rec_set_t *set, const char *var, AIFStream *val) {

    std::ostringstream valStr;
    val->print(valStr);

    auto request = CommandToSocProto(DBGCMD_SETDEBUGGERVAR, var, valStr.str());

    rep_set_t *result = DbgSendCmdAndRecvReply(set, request, DBGEV_OK);
    return result;
}

/*
** DBG_GETDEBUGGERVAR: Get variable in debugger.
*/
rep_set_t *DbgGetDebuggerVar(rec_set_t *set, const char *var) {
    auto request = CommandToSocProto(DBGCMD_GETDEBUGGERVAR, var);

    rep_set_t *result = DbgSendCmdAndRecvReply(set, request, DBGEV_DATA);

    return result;
}

/*
** DBG_SETSOURCEPATH: Set source search path in debugger to new value.
*/
rep_set_t *DbgSetSourcePath(rec_set_t *set, char *path) {
    if (!set) return nullptr;

    rec_set_t *to_send = NewRecSet();
    rep_set_t *result = nullptr;

    // We want to do the actual reading of the source code on the frontend if
    // possible. This means this command should be directed towards the
    // frontend if possible.
    // Loop over each di in the rec set
    for (auto&& recv_ptr : set) {
        // point at the debug frontend
        if (auto dbg_fe = recv_ptr->rec_di->d_frontend) {
            if (result == nullptr) {
                result = NewRepSet();
            }

            auto e = DbgFESetSourcePath(dbg_fe, path);

            if (e->ev_event != DBGEV_SOURCEPATH && e->ev_event != DBGEV_ERROR) {
                // Free the event
                FreeEvent(e);
                // Create a new error event
                EVENT_ERROR(
                    e, DBGERR_DEBUGGER,
                    "DbgFEList returned unknown event other than error.");
            }

            // Add the event to the result based on its type
            auto group_rep = NewGroupReply(recv_ptr->rec_di,
                                           recv_ptr->rec_group->clone(), e);

            if (e->ev_event == DBGEV_ERROR) {
                AddErrToSet(result, group_rep);
            } else {
                AddReplyToSet(result, group_rep);
            }

        } else {
            // Add this recv_ptr to the to_send list
            AddReceiverToSet(to_send, CopyGroupReceiver(recv_ptr));
        }
    }

    // check to see if there are any commands left to send
    if (GetNumberOfReceivers(to_send)) {
        auto request = CommandToSocProto(DBGCMD_SETSOURCEPATH, path);

        auto tmp_res = DbgSendCmdAndRecvReply(to_send, request, DBGEV_SOURCEPATH);

        // merge the results
        result = MergeRepSet(result, tmp_res);
    }

    FreeRecSet(to_send);
    return result;
}

/*
** DBG_GETSOURCEPATH: Get source search path from debugger.
*/
rep_set_t *DbgGetSourcePath(rec_set_t *set) {
    rep_set_t *result = NewRepSet();
    if (!set) return result;

    auto to_send = NewRecSet();

    // We want to do the actual reading of the source code on the frontend if
    // possible. This means this command should be directed towards the
    // frontend if possible.
    // Loop over each di in the rec set
    for (auto&& recv_ptr : set) {
        // point at the debug frontend
        if (auto dbg_fe = recv_ptr->rec_di->d_frontend) {
            auto e = DbgFEGetSourcePath(dbg_fe);

            if (e->ev_event != DBGEV_SOURCEPATH && e->ev_event != DBGEV_ERROR) {
                // Free the event
                FreeEvent(e);
                // Create a new error event
                EVENT_ERROR(
                    e, DBGERR_DEBUGGER,
                    "DbgFEList returned unknown event other than error.");
            }

            // Add the event to the result based on its type
            auto group_rep = NewGroupReply(recv_ptr->rec_di,
                                           recv_ptr->rec_group->clone(), e);

            if (e->ev_event == DBGEV_ERROR) {
                AddErrToSet(result, group_rep);
            } else {
                AddReplyToSet(result, group_rep);
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
        result = MergeRepSet(result, tmp_res);
    }

    FreeRecSet(to_send);
    return result;
}

rep_set_t *DbgSetPrintElements(rec_set_t *set, const char *elements) {
    auto request = CommandToSocProto(DBGCMD_SETPRINTELEMENTS, elements);
    rep_set_t *result = DbgSendCmdAndRecvReply(set, request, DBGEV_OK);
    return result;
}

rep_set_t *DbgResetBeforeDirectCmd(rec_set_t *set) {
    auto request = CommandToSocProto(DBGCMD_RESETBEFOREDIRECTCMD);
    rep_set_t *result = DbgSendCmdAndRecvReply(set, request, DBGEV_OK);
    return result;
}

rep_set_t *DbgRestoreAfterDirectCmd(rec_set_t *set) {
    auto request = CommandToSocProto(DBGCMD_RESTOREAFTERDIRECTCMD);
    rep_set_t *result = DbgSendCmdAndRecvReply(set, request, DBGEV_OK);
    return result;
}

/*
** DBG_SETVAR: Set variable in program to new value.
*/
rep_set_t *DbgSetVar(rec_set_t *set, const char *var, AIFStream *val) {
    std::ostringstream valStr;
    val->print(valStr);

    auto request = CommandToSocProto(DBGCMD_SETVAR, var, valStr.str(), set->getThreads());

    rep_set_t *result = DbgSendCmdAndRecvReply(set, request, DBGEV_OK);

    return result;
}

/*
** DBG_GETTYPE: Get type of variable.
** If isType is true, then get type of type.
*/
rep_set_t *DbgGetType(rec_set_t *set, const char *var, int isType) {
    auto request = CommandToSocProto(DBGCMD_GETTYPE, var, isType, set->getThreads());
    rep_set_t *result = DbgSendCmdAndRecvReply(set, request, DBGEV_TYPE, DBGEV_THREADINFO);
    return result;
}

/*
** DBG_GO: Continue/start execution.
*/
rep_set_t *DbgGo(rec_set_t *set, bool all) {
    dbgfrontend_sw *dbg_fe;
    int has_err;

    // Remove any old events for this group
    ResetForAsyncCmd(set);

    auto request = CommandToSocProto(DBGCMD_GO, int(all), set->getThreads());

    auto result = DbgSendCmdAndRecvReply(set, request, DBGEV_OK);

    // Clear the list location state since this modified the frame only if there
    // was no error
    if (!SetHasError(result)) {
        for (auto&& recv_ptr : set) {
            dbg_fe = recv_ptr->rec_di->d_frontend;
            if (dbg_fe != nullptr && dbg_fe->dbg_list_isPending) {
                ClearListState(dbg_fe);
            }
        }
    } else {
        // selectively clear the list
        for (auto&& recv_ptr : set) {
            // reset the has_err
            has_err = 0;

            // Ensure this di is not in the error list
            for (auto&& err : result->getErrs()) {
                if (err->rep_di == recv_ptr->rec_di) {
                    // this di has an error
                    ++has_err;
                    break;
                }
            }

            // only proceed if there wasn't an error with this di
            if (!has_err) {
                dbg_fe = recv_ptr->rec_di->d_frontend;
                if (dbg_fe != nullptr && dbg_fe->dbg_list_isPending) {
                    ClearListState(dbg_fe);
                }
            }
        }
    }

    // This command changes the status
    inferior_status_change = 1;

    // An async event is now pending
    async_pending = 1;

    return result;
}

/*
** DBG_FINISHFUNC: Continue execution until the end of the current function
*frame.
*/
rep_set_t *DbgFinishFunc(rec_set_t *set) {
    if (!set) return nullptr;

    // Remove any old events for this group
    ResetForAsyncCmd(set);

    auto request = CommandToSocProto(DBGCMD_FINISH, set->getThreads());

    auto result = DbgSendCmdAndRecvReply(set, request, DBGEV_OK);


    // Clear the list location state since this modified the frame only if there
    // was no error
    if (!SetHasError(result)) {
        for (auto&& recv_ptr : set) {
            auto dbg_fe = recv_ptr->rec_di->d_frontend;
            if (dbg_fe != nullptr && dbg_fe->dbg_list_isPending) {
                ClearListState(dbg_fe);
            }
        }
    } else {
        // selectively clear the list
        for (auto&& recv_ptr : set) {
            // reset the has_err
            auto has_err = 0;

            // Ensure this di is not in the error list
            for (auto&& err : result->getErrs()) {
                if (err->rep_di == recv_ptr->rec_di) {
                    // this di has an error
                    ++has_err;
                    break;
                }
            }

            // only proceed if there wasn't an error with this di
            if (!has_err) {
                auto dbg_fe = recv_ptr->rec_di->d_frontend;
                if (dbg_fe != nullptr && dbg_fe->dbg_list_isPending) {
                    ClearListState(dbg_fe);
                }
            }
        }
    }

    // This command changes the status
    inferior_status_change = 1;

    // An async event is now pending
    async_pending = 1;

    return result;
}

/*
** DBG_SETLINEBREAK: Set a breakpoint in program at
**			specified line.
*/
rep_set_t *DbgSetLineBreak(rec_set_t *set, int allowPending, const char *file, int line,
                           const char* threadSet) {

    auto request = CommandToSocProto(DBGCMD_SETLINEBREAK, allowPending, file, line, threadSet);

    auto result = DbgSendCmdAndRecvReply(set, request, DBGEV_BPSET);

    // process breakpoint information
    for (auto&& reply : result) {
        reply->rep_di->d_breakpoints.recordBreakpoint(*EVENT_BP(reply->rep_event), reply->rep_group.get());
    }

    return result;
}

/*
** DBG_SETFUNCBREAK: Set a breakpoint in program at
**			specified function.
*/
rep_set_t *DbgSetFuncBreak(rec_set_t *set, int allowPending, char *file, char *func,
                           const char* threadSet) {

    auto request = CommandToSocProto(DBGCMD_SETFUNCBREAK, allowPending, file, func, threadSet);

    auto result = DbgSendCmdAndRecvReply(set, request, DBGEV_BPSET);

    // process breakpoint information
    for (auto&& reply : result) {
        reply->rep_di->d_breakpoints.recordBreakpoint(*EVENT_BP(reply->rep_event), reply->rep_group.get());
    }

    return result;
}

/*
** DBG_SETWATCH: Set a watchpoint in program for
**			specified expression.
*/
rep_set_t *DbgSetWatch(rec_set_t *set, int type, char *expr) {

    auto request = CommandToSocProto(DBGCMD_SETWATCH, type, expr);

    rep_set_t *result = DbgSendCmdAndRecvReply(set, request, DBGEV_BPSET);

    // process breakpoint information
    for (auto&& reply : result) {
        reply->rep_di->d_breakpoints.recordBreakpoint(*EVENT_BP(reply->rep_event), reply->rep_group.get());
    }

    return result;
}

rep_set_t *DbgSetCatch(rec_set_t *recSet, const std::string &event, const std::string &regex) {
    auto request = CommandToSocProto(DBGCMD_SETCATCH, event, regex);
    auto result = DbgSendCmdAndRecvReply(recSet, request, DBGEV_BPSET);

    // process catchpoint information
    for (const auto &reply : result) {
        reply->rep_di->d_breakpoints.recordBreakpoint(*EVENT_BP(reply->rep_event), reply->rep_group.get());
    }

    return result;
}

rep_set_t *DbgSetCheckpoint(rec_set_t* recSet) {
    static auto nextId = 0u;

    auto request = CommandToSocProto(DBGCMD_SETCHECKPOINT);
    auto result = DbgSendCmdAndRecvReply(recSet, request, DBGEV_CHECKPOINT);

    // bucket each reply by its di
    std::unordered_map<dbginvoke*, std::vector<std::pair<const Bitset *, const dbgcheckpoint_t*>>> diMap;
    for (const auto reply : result) {
        if (!reply->rep_di) {
            return nullptr;
        }
        diMap[reply->rep_di].push_back({reply->rep_group.get(), EVENT_CHECKPOINT(reply->rep_event)});
    }

    auto mostMade = 0u;

    // add checkpoint to each di
    for (auto [di, checkpointV] : diMap) {
        auto produced = di->d_checkpoints.recordCheckpoint(nextId, checkpointV);
        // recordCheckpoint can make either one or two checkpoints each call,
        // see comments in recordCheckpoint
        mostMade = std::max(mostMade, produced);
        if (produced == 0) {
            PrintOutError("Failed to record checkpoint\n");
            PrintOutError("(%s)\n", di->d_checkpoints.getError().c_str());
        }
    }

    nextId += mostMade;

    return result;
}

rep_set_t *DbgRestoreCheckpoint(rec_set_t* recSet, unsigned int id) {
    if (!recSet) return nullptr;

    // add string error to rep_set_t result
    const auto addStrError = [](rep_set_t *&result, dbginvoke &di, const Bitset &group, const std::string message) {
        if (!result) {
            result = NewRepSet();
        }

        AddErrToSet(result, NewGroupReply(&di, group.clone(), NewErrorEvent(DBGERR_DEBUGGER, message.c_str())));
    };

    // add rep_set_t error to rep_set_t result
    // deletes toMerge
    const auto addRepSetError = [](rep_set_t *&result, rep_set_t *&toMerge) {
        if (!result) {
            result = NewRepSet();
        }

        MergeRepSet(result, toMerge);
    };

    using namespace std::string_literals;

    rep_set_t *result = nullptr;

    // preliminary error checking
    auto foundCheckp = std::any_of(begin(recSet), end(recSet),
        [&](const auto &rec) { 
        return rec->rec_di->d_checkpoints.find(id) != nullptr;
    });

    if (!foundCheckp) {
        // just use the first di to generate the error
        auto first = recSet->getReceivers().front();
        addStrError(result, *first->rec_di, *first->rec_group, 
            "Checkpoint "s + std::to_string(id) + " doesn't exist."
        );
    }

    // return here if errors are detected
    if (result) {
        return result;
    }

    for (const auto rec : recSet) {
        auto di = rec->rec_di;
        auto checkp = di->d_checkpoints.find(id);
        
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
            AddReceiverToSet(new_rec_set, NewGroupReceiver(di, group.get()));

            // Get frames to update client side location data on success
            auto frames = DbgGetFrame(new_rec_set);
            if (!frames) {
                addStrError(result, *di, *group, "Error fetching frame");
                continue;
            }
            for (const auto* frameReply : frames) {
                const auto &frameGroup = frameReply->rep_group;
                const auto &frameLoc = EVENT_FRAME(frameReply->rep_event)->getLoc();

                auto request = CommandToSocProto(DBGCMD_RESTORECHECKPOINT, idSet.id);

                // returns nullptr on success
                if (auto tmp_result = DbgSendCmdAndRecvReply(new_rec_set, request, DBGEV_OK); !tmp_result) {
                    // update client side location data of any checkpoints that
                    // are current
                    for (auto &checkp : di->d_checkpoints) {
                        if (checkp.currentSet().hasCommonBits(*frameGroup)) {
                            checkp.locations().replace(
                                bitset::fromAnd(*frameGroup, checkp.currentSet()).get(), 
                                frameLoc.loc_file, frameLoc.loc_line, frameLoc.loc_func
                            );
                        }
                        checkp.unmarkCurrent(*frameGroup);
                    }
                    di->d_checkpoints.find(id)->markCurrent(*frameGroup);
                } else {
                    // deletes tmp_result
                    addRepSetError(result, tmp_result);
                }
            }

            FreeRecSet(new_rec_set);
        }
    }

    return result;
}

rep_set_t *DbgDeleteCheckpoint(rec_set_t* recSet, unsigned int id) {
    if (!recSet) return nullptr;

    // add string error to rep_set_t result
    const auto addStrError = [](rep_set_t *&result, dbginvoke &di, const Bitset &group, const std::string message) {
        if (!result) {
            result = NewRepSet();
        }

        dbgevent_t *e;
        EVENT_ERROR(e, DBGERR_DEBUGGER, message.c_str());
        auto group_rep = NewGroupReply(&di, group.clone(), e);
        AddErrToSet(result, group_rep);
    };

    // add rep_set_t error to rep_set_t result
    // deletes toMerge
    const auto addRepSetError = [](rep_set_t *&result, rep_set_t *&toMerge) {
        if (!result) {
            result = NewRepSet();
        }

        MergeRepSet(result, toMerge);
    };

    using namespace std::string_literals;

    rep_set_t *result = nullptr;
    
    // preliminary error checking
    auto foundCheckp = std::any_of(begin(recSet), end(recSet),
        [&](const auto &rec) { 
        return rec->rec_di->d_checkpoints.find(id) != nullptr;
    });

    if (!foundCheckp) {
        // just use the first di
        auto first = recSet->getReceivers().front();
        addStrError(result, *first->rec_di, *first->rec_group, 
            "Checkpoint "s + std::to_string(id) + " doesn't exist."
        );
    }

    // return here if errors are detected
    if (result) {
        return result;
    }

    // more preliminary error checking
    for (const auto &rec : recSet) {
        auto* di = rec->rec_di;
        const auto* checkpoint = di->d_checkpoints.find(id);

        // don't delete a checkpoint that is current somewhere
        if (!checkpoint->currentSet().isEmpty()) {
            addStrError(result, *di, *rec->rec_group, 
                "Checkpoint "s + std::to_string(id) + " is the current checkpoint on $"
                + di->d_handle + checkpoint->currentSet().toSetString() + "."
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
            addStrError(result, *di, *rec->rec_group,
                "Checkpoint "s + std::to_string(id) + " is the original thread of execution on $"
                + di->d_handle + IDZero->group->toSetString() + " and can't be directly deleted. "
                + "When all other checkpoints on $" + di->d_handle + " are deleted, checkpoint " + std::to_string(id)
                + " will be implicitly deleted."
            );
        }
    }

    // return here if errors are detected
    if (result) {
        return result;
    }

    for (const auto &rec : recSet) {
        auto di = rec->rec_di;
        auto checkp = di->d_checkpoints.find(id);
        
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
            AddReceiverToSet(new_rec_set, NewGroupReceiver(di, group.get()));

            if (auto tmp_result = DbgSendCmdAndRecvReply(new_rec_set, request, DBGEV_OK); !tmp_result) {
                // will remove the group from the checkpoint after loop
                removeGroups.push_back(std::move(group));
            } else {
                // deletes tmp_result
                addRepSetError(result, tmp_result);
            }

            FreeRecSet(new_rec_set);
        }

        // groups that were removed with checkpoint 
        for (const auto &bitset : removeGroups) {
            checkp->removeGroup(*bitset.get());
        }

        // remove any empty checkpoints left by above
        di->d_checkpoints.removeEmpty();
        // if the deletion left a single checkpoint, gdb will implicitly delete it,
        // so reflect that in our client side data as well
        di->d_checkpoints.removeSingular();
    }

    return result;
}

// Set the current thread for subsequent commands
rep_set_t *DbgSetThread(rec_set_t* set, const char* threadId)
{
    auto request = CommandToSocProto(DBGCMD_THREAD, threadId);

    rep_set_t* result  = DbgSendCmdAndRecvReply(set, request, DBGEV_OK);
    return result;
}

rep_set_t *DbgSetThreadName(rec_set_t* set, const char* name)
{
    auto request = CommandToSocProto(DBGCMD_SETTHREADNAME, name);
    rep_set_t* result = DbgSendCmdAndRecvReply(set, request, DBGEV_OK);
    return result;
}

rep_set_t *DbgFindThread(rec_set_t* set, const char* pattern)
{
    auto request = CommandToSocProto(DBGCMD_FINDTHREAD,pattern);
    rep_set_t* result = DbgSendCmdAndRecvReply(set, request, DBGEV_THREADINFO);
    return result;
}

rep_set_t *DbgThreadApply(rec_set_t* set, const char* op)
{
    auto request = CommandToSocProto(DBGCMD_THREADAPPLY,op);
    rep_set_t* result = DbgSendCmdAndRecvReply(set, request, DBGEV_THREADINFO);
    return result;
}

// Set the current thread for subsequent commands
rep_set_t *DbgGetThreadInfo(rec_set_t* set, const char* threadList)
{
    auto request = CommandToSocProto(DBGCMD_GETTHREADINFO, threadList);

    rep_set_t* result  = DbgSendCmdAndRecvReply(set, request, DBGEV_THREADINFO);

    return result;
}

rep_set_t *DbgSetSchedulerLocking(rec_set_t* set, const char* mode)
{
    auto request = CommandToSocProto(DBGCMD_SETSCHEDULERLOCKING,mode);
    rep_set_t* result = DbgSendCmdAndRecvReply(set, request, DBGEV_OK);
    return result;
}

rep_set_t *DbgGetSchedulerLocking(rec_set_t* set)
{
    auto request = CommandToSocProto(DBGCMD_GETSCHEDULERLOCKING);
    rep_set_t* result = DbgSendCmdAndRecvReply(set, request, DBGEV_THREADINFO);
    return result;
}

rep_set_t *DbgSetNonStop(rec_set_t* set, const char* mode)
{
    auto request = CommandToSocProto(DBGCMD_SETNONSTOP,mode);
    rep_set_t* result = DbgSendCmdAndRecvReply(set, request, DBGEV_OK);
    return result;
}

rep_set_t *DbgGetNonStop(rec_set_t* set)
{
    auto request = CommandToSocProto(DBGCMD_GETNONSTOP);
    rep_set_t* result = DbgSendCmdAndRecvReply(set, request, DBGEV_THREADINFO);
    return result;
}

rep_set_t *DbgOmpd(rec_set_t* set, const char* op)
{
    auto request = CommandToSocProto(DBGCMD_OMPD,op);
    rep_set_t* result = DbgSendCmdAndRecvReply(set, request, DBGEV_THREADINFO, DBGEV_OK);
    return result;
}

rep_set_t *DbgCuda(rec_set_t* set, const char* op)
{
    auto request = CommandToSocProto(DBGCMD_CUDA,op);
    rep_set_t* result = DbgSendCmdAndRecvReply(set, request, DBGEV_THREADINFO, DBGEV_OK);
    return result;
}


/*
** DBG_KILL: Terminate execution of program.
** TODO: Probably remove this, the DbgQuit should be sufficient.
*/
rep_set_t *DbgKill(rec_set_t *set) {

    if (!set) return nullptr;

    auto request = CommandToSocProto(DBGCMD_KILL);

    auto result = DbgSendCmdAndRecvReply(set, request, DBGEV_OK);

    // Loop over each di and call the protocol specific hook
    for (auto&& recv_ptr : set) {
        // point at the di
        auto di = recv_ptr->rec_di;

        // We always want to kill the application, so hardcode detatch to 0
        DBG_PROTO_SW(di)->exit(di, 0);

        FreeDbgInvoke(di);

        recv_ptr->rec_di = nullptr;
    }

    return result;
}

/*
** DBG_DELETEBREAK: Delete a breakpoint.
*/
rep_set_t *DbgDeleteBreak(rec_set_t *set, int bp_set_num) {
    if (!set) return nullptr;

    rep_set_t *result = nullptr;

    // if bp_set_num is 0, we want this to apply to every breakpoint associated
    // with each dbginvoke_t
    for (auto&& recv_ptr : set) {
        // point at the di
        auto di = recv_ptr->rec_di;

        if (bp_set_num == 0) {
            for (auto &bp_set : di->d_breakpoints) {
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
                    AddReceiverToSet(new_rec_set, NewGroupReceiver(di, group.get()));

                    auto tmp_result =
                        DbgSendCmdAndRecvReply(new_rec_set, request, DBGEV_OK);

                    // We expect this to return nullptr on success, handle the case
                    // were things worked
                    if (tmp_result == nullptr) {
                        // remove the group from the breakpoint
                        removeGroups.push_back(std::move(group));
                    }

                    // Add this to the overall result
                    if (result == nullptr) {
                        result = tmp_result;
                    } else {
                        if (tmp_result != nullptr) {
                            result = MergeRepSet(result, tmp_result);
                        }
                    }

                    FreeRecSet(new_rec_set);
                }
                for (const auto &bitset : removeGroups) {
                    bp_set.removeGroup(bitset.get());
                }
            }
            // remove any breakpoints that are now empty
            di->d_breakpoints.removeEmptyBreakpoints();
        } else {
            // find the bp_set for the bp_set_num
            auto bp_set = di->d_breakpoints.findByLogicalID(bp_set_num);
            if (bp_set == nullptr) {
                // Error case: the provided bp_set_num does not exist
                dbgevent_t *e;

                // Create a new error event
                EVENT_ERROR(e, DBGERR_DEBUGGER, "No breakpoint number %d.\n",
                            bp_set_num);
                // Add the event to the result based on its type
                auto group_rep =
                    NewGroupReply(di, recv_ptr->rec_group->clone(), e);
                // Create a new result and return it
                result = NewRepSet();
                AddErrToSet(result, group_rep);
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
                AddReceiverToSet(new_rec_set, NewGroupReceiver(di, group.get()));

                auto tmp_result =
                    DbgSendCmdAndRecvReply(new_rec_set, request, DBGEV_OK);

                // We expect this to return nullptr on success, handle the case
                // were things worked
                if (tmp_result == nullptr) {
                    // remove the group from the breakpoint
                    removeGroups.push_back(std::move(group));
                }

                // Add this to the overall result
                if (result == nullptr) {
                    result = tmp_result;
                } else {
                    if (tmp_result != nullptr) {
                        result = MergeRepSet(result, tmp_result);
                    }
                }

                FreeRecSet(new_rec_set);
            }
            // remove any breakpoints that are now empty
            for (const auto &bitset : removeGroups) {
                bp_set->removeGroup(bitset.get());
            }
            di->d_breakpoints.removeEmptyBreakpoints();
        }
    }

    return result;
}

/*
** DBG_DISABLEBREAK: Disable a breakpoint.
*/
rep_set_t *DbgDisableBreak(rec_set_t *set, int bp_set_num) {
    if (!set) return nullptr;

    rep_set_t *result = nullptr;

    // if bp_set_num is 0, we want this to apply to every breakpoint associated
    // with each dbginvoke_t
    for (auto&& recv_ptr : set) {
        // point at the di
        auto di = recv_ptr->rec_di;

        if (bp_set_num == 0) {
            for (auto &bp_set : di->d_breakpoints) {
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
                    auto request =
                        CommandToSocProto(DBGCMD_DISABLEBREAK, id_set.id);

                    // Create the new rec set for this group
                    auto new_rec_set = NewRecSet();
                    AddReceiverToSet(new_rec_set, NewGroupReceiver(di, group.get()));

                    auto tmp_result =
                        DbgSendCmdAndRecvReply(new_rec_set, request, DBGEV_OK);

                    // We expect this to return nullptr on success, handle the case
                    // were things worked
                    if (tmp_result == nullptr) {
                        // mark the breakpoint as enabled for this group
                        bp_set.markDisabled(group.get());
                    }

                    // Add this to the overall result
                    if (result == nullptr) {
                        result = tmp_result;
                    } else {
                        if (tmp_result != nullptr) {
                            result = MergeRepSet(result, tmp_result);
                        }
                    }

                    FreeRecSet(new_rec_set);
                }
            }
        } else {
            MappedBreakpoint *bp_set;

            // find the bp_set for the bp_set_num
            if ((bp_set = di->d_breakpoints.findByLogicalID(bp_set_num)) == nullptr) {
                // Error case: the provided bp_set_num does not exist
                dbgevent_t *e;

                // Create a new error event
                EVENT_ERROR(e, DBGERR_DEBUGGER, "No breakpoint number %d.\n",
                            bp_set_num);
                // Add the event to the result based on its type
                auto group_rep =
                    NewGroupReply(di, recv_ptr->rec_group->clone(), e);
                // Create a new result and return it
                if (result == nullptr) {
                    result = NewRepSet();
                }
                AddErrToSet(result, group_rep);
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
                auto request =
                    CommandToSocProto(DBGCMD_DISABLEBREAK, id_set.id);

                // Create the new rec set for this group
                auto new_rec_set = NewRecSet();
                AddReceiverToSet(new_rec_set, NewGroupReceiver(di, group.get()));

                auto tmp_result =
                    DbgSendCmdAndRecvReply(new_rec_set, request, DBGEV_OK);

                // We expect this to return nullptr on success, handle the case
                // were things worked
                if (tmp_result == nullptr) {
                    // mark the breakpoint as disabled for this group
                    bp_set->markDisabled(group.get());
                }

                // Add this to the overall result
                if (result == nullptr) {
                    result = tmp_result;
                } else if (tmp_result != nullptr) {
                    result = MergeRepSet(result, tmp_result);
                }

                FreeRecSet(new_rec_set);
            }
        }
    }

    return result;
}

/*
** DBG_ENABLEBREAK: Enable a breakpoint.
*/
rep_set_t *DbgEnableBreak(rec_set_t *set, int bp_set_num, bool once, bool del) {
    if (!set) return nullptr;

    rep_set_t *result = nullptr;

    // if bp_set_num is 0, we want this to apply to every breakpoint associated
    // with each dbginvoke_t
    for (auto&& recv_ptr : set) {
        // point at the di
        auto di = recv_ptr->rec_di;

        if (bp_set_num == 0) {
            for (auto &bp_set : di->d_breakpoints) {
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
                    auto request = CommandToSocProto(DBGCMD_ENABLEBREAK,
                                                id_set.id, once, del);

                    // Create the new rec set for this group
                    auto new_rec_set = NewRecSet();
                    AddReceiverToSet(new_rec_set, NewGroupReceiver(di, group.get()));

                    auto tmp_result =
                        DbgSendCmdAndRecvReply(new_rec_set, request, DBGEV_OK);

                    // We expect this to return nullptr on success, handle the case
                    // were things worked
                    if (tmp_result == nullptr) {
                        // mark the breakpoint as enabled for this group
                        bp_set.markEnabled(group.get());
                        // Set the disposition for this group
                        if (once) {
                            bp_set.markDisposition(group.get(), DBGBP_DISABLE_DISP);
                        } else if (del) {
                            bp_set.markDisposition(group.get(), DBGBP_DELETE_DISP);
                        }
                    }

                    // Add this to the overall result
                    if (result == nullptr) {
                        result = tmp_result;
                    } else {
                        if (tmp_result != nullptr) {
                            result = MergeRepSet(result, tmp_result);
                        }
                    }

                    FreeRecSet(new_rec_set);
                }
            }
        } else {
            MappedBreakpoint *bp_set;

            // find the bp_set for the bp_set_num
            if ((bp_set = di->d_breakpoints.findByLogicalID(bp_set_num)) == nullptr) {
                // Error case: the provided bp_set_num does not exist
                dbgevent_t *e;

                // Create a new error event
                EVENT_ERROR(e, DBGERR_DEBUGGER, "No breakpoint number %d.\n",
                            bp_set_num);
                // Add the event to the result based on its type
                auto group_rep =
                    NewGroupReply(di, recv_ptr->rec_group->clone(), e);
                // Create a new result and return it
                if (result == nullptr) {
                    result = NewRepSet();
                }
                AddErrToSet(result, group_rep);
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
                auto request = CommandToSocProto(DBGCMD_ENABLEBREAK, id_set.id,
                                            once, del);

                // Create the new rec set for this group
                auto new_rec_set = NewRecSet();
                AddReceiverToSet(new_rec_set, NewGroupReceiver(di, group.get()));

                auto tmp_result =
                    DbgSendCmdAndRecvReply(new_rec_set, request, DBGEV_OK);

                // We expect this to return nullptr on success, handle the case
                // were things worked
                if (tmp_result == nullptr) {
                    // mark the breakpoint as enabled for this group
                    bp_set->markEnabled(group.get());
                    // Set the disposition for this group
                    if (once) {
                        bp_set->markDisposition(group.get(), DBGBP_DISABLE_DISP);
                    } else if (del) {
                        bp_set->markDisposition(group.get(), DBGBP_DELETE_DISP);
                    }
                }

                // Add this to the overall result
                if (result == nullptr) {
                    result = tmp_result;
                } else {
                    if (tmp_result != nullptr) {
                        result = MergeRepSet(result, tmp_result);
                    }
                }

                FreeRecSet(new_rec_set);
            }
        }
    }

    return result;
}

/*
** DBG_LIST: List specified function or line.
*/
rep_set_t *DbgList(rec_set_t *set, char *str) {
    auto result = NewRepSet();
    if (!set) return result;

    rec_set_t *to_send = NewRecSet();

    int i, cont;

    // We want to do the actual reading of the source code on the frontend if
    // possible. This is because there is a performance hit associated with
    // opening cross mounted files on the compute nodes. Its also the case that
    // sometimes we are unable to read the source files since they are located
    // in a non-cross mounted location.

    // Note that we can only do this if there is a debug frontend associated
    // with the dbginvoke

    // Loop over each di in the set
    for (auto&& recv_ptr : set) {
        // point at the dbg fe
        if (auto dbg_fe = recv_ptr->rec_di->d_frontend) {
            // set the forward/reverse direction
            auto isReverse = (str == nullptr) ? false : (*str == '-');

            // First is the logic for the case where a previous list is not
            // pending. If str is null, then we need to ask each of the ranks
            // for their current frame.
            if ((!str || isReverse) && !dbg_fe->dbg_list_isPending) {
                // Get the innermost frame.
                // Create the new rec set for this group - it holds state so it's not
                // directly re-usable

                auto new_rec_set = NewRecSet();
                AddReceiverToSet(new_rec_set, CopyGroupReceiver(recv_ptr));

                auto tmp_result = DbgShowFrame(new_rec_set, 1);
                FreeRecSet(new_rec_set);

                // Make sure there isn't an error in the result, otherwise add
                // this to the to_send list to get the list from the back end.
                if (SetHasError(tmp_result)) {
                    FreeRepSet(tmp_result);
                    tmp_result = nullptr;
                    AddReceiverToSet(to_send, CopyGroupReceiver(recv_ptr));
                    continue;
                }

                // process the frame -- note that we may be adding replies to tmp_result
                // inside the loop.
                for (size_t repIdx=0, numReps= tmp_result->getNumReplies();
                     tmp_result && repIdx<numReps; ++repIdx) {

                    auto group_rep = tmp_result->getReplies()[repIdx];

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
                        if (AddListStateLocation(dbg_fe, group_rep->rep_group.get(), fp->getThreads(),
                                                 location.c_str()) < 0) {
                            PrintOutError("Failed to save list location. Further calls to "
                                          "list will not save state.\n");
                        }

                        // Send the list command to the frontend
                        auto e = DbgFEList(dbg_fe, fp0->getThreads(), location.c_str());

                        // Make sure the event is a DBGEV_LIST, otherwise add this
                        // to the to_send list
                        if (e->ev_event != DBGEV_LIST) {
                            FreeRepSet(tmp_result);
                            tmp_result = nullptr;
                            FreeEvent(e);
                            ClearListState(dbg_fe);
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
                            tmp_result->addReply(newReply);
                        }
                    }
                }

                // This has converted the SHOWFRAME result into a LIST result.
                result = MergeRepSet(result, tmp_result);  // it checks for tmp_result==nulptr

            } else if ((!str || isReverse) && dbg_fe->dbg_list_isPending) {
                // We will create the result by hand based on the saved list
                // location state data

                // loop over every set_location
                SetListStateLocation(dbg_fe);
                for (set_location *set_loc; (set_loc = GetListStateLocation(dbg_fe));) {
                    // Call the list command using the saved location string to
                    // set things up
                    auto e = DbgFEList(dbg_fe, set_loc->threads, set_loc->loc_str);

                    // handle the error case
                    if (e->ev_event == DBGEV_ERROR) {
                        auto group_rep = NewGroupReply(recv_ptr->rec_di,
                                                       set_loc->set->clone(), e);
                        AddErrToSet(result, group_rep);
                        continue;
                    }

                    // Free the event
                    FreeEvent(e);

                    // Now we need to call list count times with direction
                    // depending on its sign.
                    if (set_loc->count < 0) {
                        cont = 0;

                        // We need to go in the reverse direction
                        for (i = set_loc->count; i < 0; ++i) {
                            e = DbgFEList(dbg_fe, set_loc->threads, "-");

                            // handle the error case
                            if (e->ev_event == DBGEV_ERROR) {
                                auto group_rep =
                                    NewGroupReply(recv_ptr->rec_di,
                                                  set_loc->set->clone(), e);
                                AddErrToSet(result, group_rep);
                                ++cont;
                                break;
                            }
                            // Free the event
                            FreeEvent(e);
                        }
                        if (cont) {
                            continue;
                        }
                    } else if (set_loc->count > 0) {
                        cont = 0;

                        // We need to go in the forward direction
                        for (i = 0; i < set_loc->count; ++i) {
                            e = DbgFEList(dbg_fe, set_loc->threads, "");

                            // handle the error case
                            if (e->ev_event == DBGEV_ERROR) {
                                auto group_rep =
                                    NewGroupReply(recv_ptr->rec_di,
                                                  set_loc->set->clone(), e);
                                AddErrToSet(result, group_rep);
                                ++cont;
                                break;
                            }
                            // Free the event
                            FreeEvent(e);
                        }
                        if (cont) {
                            continue;
                        }
                    }

                    // Now we have everything setup for the actual list command
                    // we are interested in. The command depends on the
                    // isReverse value. We also increment or decrement the saved
                    // count here
                    if (isReverse) {
                        e = DbgFEList(dbg_fe, set_loc->threads, "-");
                        set_loc->count--;
                    } else {
                        e = DbgFEList(dbg_fe, set_loc->threads, "");
                        set_loc->count++;
                    }

                    // Make sure we got a DBGEV_LIST back, otherwise create an
                    // error
                    if (e->ev_event != DBGEV_LIST &&
                        e->ev_event != DBGEV_ERROR) {
                        // Free the event
                        FreeEvent(e);
                        // Create a new error event
                        EVENT_ERROR(e, DBGERR_DEBUGGER,
                                    "DbgFEList returned unknown event other "
                                    "than error.");
                    }

                    // Add the event to the result based on its type
                    auto group_rep = NewGroupReply(recv_ptr->rec_di,
                                                   set_loc->set->clone(), e);

                    if (e->ev_event == DBGEV_ERROR) {
                        AddErrToSet(result, group_rep);
                    } else {
                        AddReplyToSet(result, group_rep);
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
                ClearListState(dbg_fe);

                // Save this new list location state for later calls to list
                if (AddListStateLocation(dbg_fe, recv_ptr->rec_group.get(), nullptr, str) <
                    0) {
                    PrintOutError(
                        "Failed to save list location. Further calls to list "
                        "will not save state.\n");
                }

                // we can pass the str directly to the frontend list command
                // since its not null and was provided by the user.
                auto e = DbgFEList(dbg_fe, nullptr, str);

                // Make sure the event is a DBGEV_LIST or DBGEV_ERROR, otherwise
                // create an error
                if (e->ev_event != DBGEV_LIST && e->ev_event != DBGEV_ERROR) {
                    // Free the event
                    FreeEvent(e);
                    // Create a new error event
                    EVENT_ERROR(
                        e, DBGERR_DEBUGGER,
                        "DbgFEList returned unknown event other than error.");
                }

                // Add the event to the result based on its type
                auto group_rep = NewGroupReply(recv_ptr->rec_di,
                                          recv_ptr->rec_group->clone(), e);

                if (e->ev_event == DBGEV_ERROR) {
                    AddErrToSet(result, group_rep);
                    // Make sure to clear the saved list state since there was
                    // an error with the provided string
                    ClearListState(dbg_fe);
                } else {
                    AddReplyToSet(result, group_rep);
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
        result = MergeRepSet(result, tmp_result);
    }

    FreeRecSet(to_send);
    return result;
}

/*
** DBG_GET_SOURCES: Ask for source files of inferior.
*/
rep_set_t *DbgGetSources(rec_set_t *set) {
    auto result = NewRepSet();
    if (!set) return result;

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
        // point at the dbg fe
        auto dbg_fe = recv_ptr->rec_di->d_frontend;

        if (dbg_fe != nullptr) {
            auto e = DbgFEGetSources(dbg_fe);

            // Make sure the event is a DBGEV_SOURCES or DBGEV_ERROR, otherwise
            // create an error
            if (e->ev_event != DBGEV_SOURCES && e->ev_event != DBGEV_ERROR) {
                // Free the event
                FreeEvent(e);
                // Create a new error event
                EVENT_ERROR(
                    e, DBGERR_DEBUGGER,
                    "DbgFEGetSources returned unknown event other than error.");
            }

            // Add the event to the result based on its type
            auto group_rep = NewGroupReply(recv_ptr->rec_di,
                                           recv_ptr->rec_group->clone(), e);

            if (e->ev_event == DBGEV_ERROR) {
                AddErrToSet(result, group_rep);
            } else {
                AddReplyToSet(result, group_rep);
            }

        } else {
            // add this recv_ptr to the to_send list
            AddReceiverToSet(to_send, CopyGroupReceiver(recv_ptr));
        }
    }

    // check to see if there are any commands left to send
    if (GetNumberOfReceivers(to_send)) {
        DPRINT("DbgGetSources: Falling back to backend evaluation.\n");

        auto request = CommandToSocProto(DBGCMD_GETSOURCES);

        auto tmp_result = DbgSendCmdAndRecvReply(to_send, request, DBGEV_SOURCES);

        // merge the results
        result = MergeRepSet(result, tmp_result);
    }

    FreeRecSet(to_send);
    return result;
}

/*
** DBG_SETARGS: Set arguments to pass to program when run.
*/
rep_set_t *DbgSetArgs(rec_set_t *set, char *args) {

    auto request = CommandToSocProto(DBGCMD_SETARGS, args);

    rep_set_t *result = DbgSendCmdAndRecvReply(set, request, DBGEV_OK);

    return result;
}

/*
** DBG_STEP: Execute to next statement. Repeat count times.
**		If in == 0, pass over function calls (ala next).
*/
rep_set_t *DbgStep(rec_set_t *set, int count, int in) {
    // Remove any old events for this group
    ResetForAsyncCmd(set);

    auto request = CommandToSocProto(DBGCMD_STEP, count, in, set->getThreads());

    rep_set_t *result = DbgSendCmdAndRecvReply(set, request, DBGEV_OK);

    // Clear the list location state since this modified the frame only if there
    // was no error
    if (!SetHasError(result)) {
        for (auto&& recv_ptr : set) {
            auto dbg_fe = recv_ptr->rec_di->d_frontend;
            if (dbg_fe != nullptr && dbg_fe->dbg_list_isPending) {
                ClearListState(dbg_fe);
            }
        }
    } else {
        // selectively clear the list
        for (auto&& recv_ptr : set) {
            auto has_err = false;

            // Ensure this di is not in the error list
            for (auto&& err : result->getErrs()) {
                if (err->rep_di == recv_ptr->rec_di) {
                    // this di has an error
                    has_err = true;
                    break;
                }
            }

            // only proceed if there wasn't an error with this di
            if (!has_err) {
                auto dbg_fe = recv_ptr->rec_di->d_frontend;
                if (dbg_fe != nullptr && dbg_fe->dbg_list_isPending) {
                    ClearListState(dbg_fe);
                }
            }
        }
    }

    // This command changes the status
    inferior_status_change = 1;

    // An async event is now pending
    async_pending = 1;

    return result;
}

/*
** DBG_MOVEFRAME: Move up or down stack frames.
*/
rep_set_t *DbgMoveFrame(rec_set_t *set, int count, int up) {
    if (!set) return nullptr;

    auto request = CommandToSocProto(DBGCMD_MOVEFRAME, count, up, set->getThreads());

    auto result = DbgSendCmdAndRecvReply(set, request, DBGEV_FRAME);

    // Clear the list location state and set it to the new frame since this
    // modified the frame location. Only do it if it didn't have an error.
    // process DIs that did move the frame
    for (auto recv_ptr : set) {
        // reset the has_err
        auto hasErr = false;

        // Ensure this di is not in the error list
        for (auto&& err : result->getErrs()) {
            if (err->rep_di == recv_ptr->rec_di) {
                // this di has an error
                hasErr = true;
                break;
            }
        }

        if (hasErr) {
            continue;
        }

        // point at the dbg fe
        if (auto dbg_fe = recv_ptr->rec_di->d_frontend) {
            // Clear any existing state
            ClearListState(dbg_fe);

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
                    if (AddListStateLocation(dbg_fe, group_rep->rep_group.get(), fp->getThreads(),
                                             locStr.str().c_str()) < 0) {
                        PrintOutError(
                            "Failed to save list location. Further calls "
                            "to list will not save state.\n");
                    }
                }
            }
        }
    }

    return result;
}

/*
** DBG_SHOWFRAME: Show current stack.
*/
rep_set_t *DbgShowFrame(rec_set_t *set, int count) {

    auto request = CommandToSocProto(DBGCMD_SHOWFRAME, count, set->getThreads());

    rep_set_t *result = DbgSendCmdAndRecvReply(set, request, DBGEV_FRAME);

    return result;
}

/*
** DBG_GETFRAME: Get selected stack frame.
*/
rep_set_t *DbgGetFrame(rec_set_t *set) {
    auto request = CommandToSocProto(DBGCMD_GETFRAME, set->getThreads());

    rep_set_t *result = DbgSendCmdAndRecvReply(set, request, DBGEV_FRAME);

    return result;
}

/*
** DBG_GETVARS: Return defined variables.
*/
rep_set_t *DbgGetVars(rec_set_t *set) {
    auto request = CommandToSocProto(DBGCMD_GETVARS, set->getThreads());

    rep_set_t *result = DbgSendCmdAndRecvReply(set, request, DBGEV_VARS, DBGEV_THREADINFO);


    return result;
}

/*
** DBG_GETSCOPE: Return variables local to a scope.
*/
rep_set_t *DbgGetScope(rec_set_t *set, char *location) {
    rep_set_t *result = NewRepSet();

    if (!set) return result;

    rec_set_t *to_send = NewRecSet();

    // We want to do the actual reading of the source files on the frontend if
    // possible. This is because there is a performance hit associated with
    // opening cross mounted files on the compute nodes. Its also the case that
    // sometimes we are unable to read the source files since they are located
    // in a non-cross mounted location.

    // Note that we can only do this if there is a debug frontend associated
    // with the dbginvoke

    // Loop over each di in the set
    for (auto&& recv_ptr : set) {
        // point at the dbg fe
        auto dbg_fe = recv_ptr->rec_di->d_frontend;

        if (dbg_fe != nullptr) {
            auto e = DbgFEGetScope(dbg_fe, location);

            // Make sure the event is a DBGEV_VARS or DBGEV_ERROR, otherwise
            // create an error
            if (e->ev_event != DBGEV_VARS && e->ev_event != DBGEV_ERROR) {
                // Free the event
                FreeEvent(e);
                // Create a new error event
                EVENT_ERROR(
                    e, DBGERR_DEBUGGER,
                    "DbgFEGetScope returned unknown event other than error.");
            }

            // Add the event msg to the result based on its type
            auto group_rep = NewGroupReply(recv_ptr->rec_di,
                                      recv_ptr->rec_group->clone(), e);

            if (e->ev_event == DBGEV_ERROR) {
                AddErrToSet(result, group_rep);
            } else {
                AddReplyToSet(result, group_rep);
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
        result = MergeRepSet(result, tmp_result);
    }

    FreeRecSet(to_send);
    return result;
}

/*
** DBG_DIRECTCMD: Send direct command to debug backend
*/
rep_set_t *DbgDirectCmd(rec_set_t *set, int argc, char **argv) {

    auto request = CommandToSocProto(DBGCMD_DIRECT, std::make_pair(argc, argv));

    rep_set_t *result = DbgSendCmdAndRecvReply(set, request, DBGEV_CMD);

    return result;
}

/*
** DBG_DBGSTATUS: Return status of debugger.
*/
rep_set_t *DbgStatus(rec_set_t *set, int getLocation) {
    auto request = CommandToSocProto(DBGCMD_STATUS, getLocation);
    rep_set_t *result = DbgSendCmdAndRecvReply(set, request, DBGEV_STAT);

    return result;
}

/*
** DBG_INTERRUPT: Interrupt current debugger operation.
*/
rep_set_t *DbgInterrupt(rec_set_t *set, bool all) {
    // Remove any old events for this group
    ResetForAsyncCmd(set);

    auto request = CommandToSocProto(DBGCMD_INTERRUPT, all);

    auto result = DbgSendCmdAndRecvReply(set, request, DBGEV_HALT);

    // Clear the list location state since this modified the frame only if there
    // was no error
    if (!SetHasError(result)) {
        for (auto&& recv_ptr : set) {
            auto dbg_fe = recv_ptr->rec_di->d_frontend;
            if (dbg_fe != nullptr && dbg_fe->dbg_list_isPending) {
                ClearListState(dbg_fe);
            }
        }
    } else {
        // selectively clear the list
        for (auto&& recv_ptr : set) {
            // reset the has_err
            auto has_err = false;

            // Ensure this di is not in the error list
            for (auto&& err : result->getErrs()) {
                if (err->rep_di == recv_ptr->rec_di) {
                    // this di has an error
                    has_err = true;
                    break;
                }
            }

            // only proceed if there wasn't an error with this di
            if (!has_err) {
                auto dbg_fe = recv_ptr->rec_di->d_frontend;
                if (dbg_fe != nullptr && dbg_fe->dbg_list_isPending) {
                    ClearListState(dbg_fe);
                }
            }
        }
    }

    // This command changes the status
    inferior_status_change = 1;

    // An async event is now pending
    async_pending = 1;

    return result;
}

/*
** DBG_QUIT: Tell server to quit immediately.
** Use the value of det to override the default behavior
*/
rep_set_t *DbgQuit(rec_set_t *set, dbgquit_type det) {
    int detach = 0;
    rep_set_t *result = NewRepSet();

    // set the detach value
    switch (det) {
        case DBG_KILL:
            detach = 0;
            goto uniform_exit;

        case DBG_DETACH:
            detach = 1;

        uniform_exit:
            {
                // The caller hardcoded the quit type, so it is the same for every
                // di. There is no need to selectively quit based on the di type.
                auto request = CommandToSocProto(DBGCMD_QUIT, detach);

                auto tmp_result = DbgSendCmdAndRecvReply(set, request, DBGEV_OK);

                // merge the results
                result = MergeRepSet(result, tmp_result);
            }

            // Call protocol specific communication network exit function and
            // destroy the di for each di in the rec_set
            for (auto&& recv_ptr : set) {
                // point at the di
                auto di = recv_ptr->rec_di;

                DBG_PROTO_SW(di)->exit(di, detach);

                // cleanup the di object
                FreeDbgInvoke(di);
            }

            break;

        case DBG_DEFAULT:
            // Here the caller used the default quit type which is based on the
            // attach value in each di. So we need to loop over each di in the
            // rec_set individually since these can be different.
            for (auto&& recv_ptr : set) {
                // point at the di
                auto di = recv_ptr->rec_di;

                // create the rec_set
                auto to_send = NewRecSet();
                AddReceiverToSet(to_send, CopyGroupReceiver(recv_ptr));

                // use default based on attach value
                switch (di->d_type) {
                    case DBG_ATTACHED:
                        detach = 1;
                        break;
                    case DBG_LAUNCHED:
                        detach = 0;
                        break;
                }

                auto request = CommandToSocProto(DBGCMD_QUIT, detach);

                auto tmp_result = DbgSendCmdAndRecvReply(to_send, request, DBGEV_OK);

                FreeRecSet(to_send);

                // merge the results
                result = MergeRepSet(result, tmp_result);

                // Call protocol specific communication network exit function
                DBG_PROTO_SW(di)->exit(di, detach);

                // cleanup the di object
                FreeDbgInvoke(di);
            }

            break;
    }

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
