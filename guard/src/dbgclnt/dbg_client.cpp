/*********************************************************************************\
 * Debugger interface routines
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * Copyright 2010-2020 Hewlett Packard Enterprise Development LP.
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
#include <pthread.h>
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

#include "dbg_client.h"
#include "useful.h"

#if defined(RD)
#include "df.h"
#endif /* RD */

/*
** Internal types
*/
struct inpcb_t {
    int i_fd;
    void (*i_rtn)();
    // This needs a lock because the MRNet error callback is threaded and will
    // mess with the inpcb_t object during printing error messages
    pthread_mutex_t i_lock;
    pthread_mutexattr_t i_lock_attr;
};
typedef struct inpcb_t inpcb_t;

struct outpcb_t {
    void (*o_rtn)(dbginvoke_t *, FILE *);
};
typedef struct outpcb_t outpcb_t;

/*
** Internal prototypes
*/
static dbginvoke_t *NewDbgInvoke(const char *, dbgproc_type, int,
                                 dbgclnt_proto *);
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
static event_set_t *NewEventSet(dbginvoke_t *, bitset *, dbgevent_t *);
static void FreeEventSet(event_set_t *);
static void DestroyEventSet(dbginvoke_t *, dbgev);
static event_set_t *AddEvent(dbginvoke_t *, bitset *, dbgevent_t *);
static void CheckForFullEventSet(dbginvoke_t *, bitset *, dbgev);
static void ResetForAsyncCmd(rec_set_t *);
static void RemoveGroupFromAsyncSet(dbginvoke_t *, bitset *);
static bp_disp_set_t *NewBPDispSet(bitset *, dbgbp_disp);
static bp_id_set_t *NewBPIDSet(bitset *, int);
static void FreeBPSet(bp_set_t *);
static void FreeBPIDSet(bp_id_set_t *);
static void FreeBPDispSet(bp_disp_set_t *);
static void AddBP(dbginvoke_t *di, dbgbp_t *bp, bitset *group);
static void AddBPToBPSet(bp_set_t *, bitset *, dbgbp_t *);
static void EnableBPForBPSet(bp_set_t *, bitset *);
static void DisableBPForBPSet(bp_set_t *, bitset *);
static void SetBPDispositionForBPSet(bp_set_t *, bitset *, dbgbp_disp);
static void RemoveBPSet(dbginvoke_t *, bp_set_t *, bitset *);
static int CompareBPSetWithBP(bp_set_t *, dbgbp_t *);
static void FixupPendingBPSetForBP(dbginvoke_t *di, dbgbp_t *bp, bitset *group);
static group_reply_t* JoinReplies(bitset* group,
                                  const std::vector<group_reply*>& replies,
                                  arrayDimension_t* shape,
                                  bool& isError);
static rep_set_t *DbgSendCmdAndRecvReply(rec_set_t *set, const std::string& request,
                                         dbgev req_type, dbgev alt_req_type=DBGEV_NONE);
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
static outpcb_t OutputCB;
static int _mask_cnt;
static int _mask_fd;
#if defined(RD)
static int _df_ready;
#endif /* RD */
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
                                 int nproc, dbgclnt_proto *proto) {
    dbginvoke_t *di;

    di = (dbginvoke_t *)Alloc(sizeof(dbginvoke_t));
    memset(di, 0, sizeof(dbginvoke_t));

    // Set non-zero members here
    di->d_handle = (handle != nullptr) ? strdup(handle) : nullptr;
    di->d_type = type;
    di->d_nproc = nproc;
    di->d_proto = proto;
    di->d_eventset_list = NewList();
    di->d_bpset_list = NewList();

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

    // Call the protocol destructor if there is one
    if (DBG_PROTO_SW(di)->sw_destroyclnt != nullptr) {
        DBG_PROTO_SW(di)->sw_destroyclnt(di);
    }

    // Quit the debug frontend if there is one
    if (di->d_frontend != nullptr) {
        e = DbgDestroyFE(di->d_frontend);
        FreeEvent(e);
    }

    if (di->d_bitset_full != nullptr) bitset_free(di->d_bitset_full);

    if (di->d_bitset_launched != nullptr) bitset_free(di->d_bitset_launched);

    if (di->d_cmd != nullptr) Free(di->d_cmd);

    // remove any event fds from the proto
    DestroyList(di->d_evfds, Free);

    // remove the async lists
    DestroyList(di->d_eventset_list, (void (*)(void *))FreeEventSet);
    DestroyList(di->d_bpset_list, (void (*)(void *))FreeBPSet);

    Free(di->d_handle);
    Free(di);

    ReleaseSignals();
}

/**************************
** Receiver set functions
**************************/

/*
 * data structures for group receivers to dbgsrv
 */

group_recv_t *NewGroupReceiver(dbginvoke_t *di, bitset *proc_mask) {
    group_recv_t *rec;

    rec = (group_recv_t *)Alloc(sizeof(group_recv_t));
    rec->rec_di = di;
    rec->rec_group = bitset_dup(proc_mask);
    rec->_cmd_sent = nullptr;

    return rec;
}

static void FreeGroupReceiver(group_recv_t *rec) {
    bitset_free(rec->rec_group);
    if (rec->_cmd_sent != nullptr) {
        bitset_free(rec->_cmd_sent);
    }
    Free(rec);
}

static void ClearCmdSent(group_recv_t *rec) {
    if (rec->_cmd_sent != nullptr) {
        bitset_free(rec->_cmd_sent);
        rec->_cmd_sent = nullptr;
    }
}

static group_recv_t *CopyGroupReceiver(group_recv_t *rec) {
    group_recv_t *newrec;

    if (rec == nullptr) return nullptr;

    newrec = (group_recv_t *)Alloc(sizeof(group_recv_t));

    newrec->rec_di = rec->rec_di;
    newrec->rec_group = bitset_dup(rec->rec_group);
    newrec->_cmd_sent = nullptr;

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
            bitset_oreq(r->rec_group, rec->rec_group);
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


// This returns the number of dbginvoke_t in the rec_set
static int GetNumberOfReceivers(rec_set_t *set) {
    return set ? set->getReceivers().size() : 0;
}

bool RecSetHasOneProcPerReceiver(rec_set_t* set)
{
    if (!set) return false;
    for (auto&& recv : set) {
        if (bitset_numset(recv->rec_group) != 1) {
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

group_reply_t *NewGroupReply(dbginvoke_t *di, bitset *src, dbgevent_t *reply) {
    group_reply_t *rep = (group_reply_t *)Alloc(sizeof(group_reply_t));

    rep->rep_di = di;
    rep->rep_event = reply;
    rep->rep_group = src;

    return rep;
}

void FreeGroupReply(group_reply_t *rep) {
    FreeEvent(rep->rep_event);
    bitset_free(rep->rep_group);
    Free(rep);
}

group_reply_t *CopyGroupReply(group_reply_t *rep) {
    group_reply_t *newrep;

    if (rep == nullptr) return nullptr;

    newrep = (group_reply_t *)Alloc(sizeof(group_reply_t));

    newrep->rep_di = rep->rep_di;
    newrep->rep_group = bitset_dup(rep->rep_group);
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

rep_set_t::rep_set_t(const std::vector<std::unique_ptr<rep_set_t>>& slices, arrayDimension_t* shape, bitset* group)
    : rep_set_t()
{
    mergeReplySliceSet(slices, shape, group);
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
            PrintErrorWithSetInfo(err->rep_di->d_handle, err->rep_group, "%s\n",
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
static event_set_t *NewEventSet(dbginvoke_t *di, bitset *group,
                                dbgevent_t *event) {
    event_set_t *eventset = (event_set_t *)Alloc(sizeof(event_set_t));

    eventset->e_di = di;
    eventset->e_group = bitset_dup(group);  // duplicate group
    eventset->e_ev = DupEvent(event);

    return eventset;
}

/*
** Free an event set
*/
static void FreeEventSet(event_set_t *eventset) {
    if (eventset == nullptr) return;

    if (eventset->e_group != nullptr) bitset_free(eventset->e_group);

    if (eventset->e_ev != nullptr) FreeEvent(eventset->e_ev);

    Free(eventset);
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
static event_set_t *AddEvent(dbginvoke_t *di, bitset *group,
                             dbgevent_t *event) {
    event_set_t *eventset;

    if (di == nullptr || group == nullptr || event == nullptr) return nullptr;

    SetList(di->d_eventset_list);
    while ((eventset = (event_set_t *)GetListElement(di->d_eventset_list)) !=
           nullptr) {
        // Call the CompareEvents function and handle it if they are equal
        if (CompareEvents(eventset->e_ev, event) == EV_EQUAL) {
            // Add this group to the event set
            bitset_oreq(eventset->e_group, group);
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
static void CheckForFullEventSet(dbginvoke_t *di, bitset *fullset, dbgev ev) {
    // A set that represents all ev that we have recieved so far
    bitset *aggset;
    // individual event set
    event_set_t *eventset;

    // Return if di is null
    if (di == nullptr) return;

    // dup the fullset and then clear it
    aggset = bitset_dup(fullset);
    bitset_clear(aggset);

    // Create a set that contains all the ev events by looping over
    // every event
    SetList(di->d_eventset_list);
    while ((eventset = (event_set_t *)GetListElement(di->d_eventset_list)) !=
           nullptr) {
        if (EVENT_EV(eventset->e_ev) == ev) {
            // Add it to the bitset
            bitset_oreq(aggset, eventset->e_group);
        }
    }

    // Check to see if the aggset is the same as the fullset
    if (bitset_eq(aggset, fullset) == 1) {
        // An event of ev has been recieved for every rank in fullset, so
        // we should remove this event
        DPRINT(
            "CheckForFullEventSet: Recieved ev of %s for every rank. Removing "
            "event set.\n",
            DbgevToStr(ev));
        DestroyEventSet(di, ev);
    }

    // Destroy the aggset we dup'ed
    bitset_free(aggset);
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
            bitset_andeqnot(eventset->e_group, recv_ptr->rec_group);

            // Make sure the e_group isn't empty, otherwise remove it
            if (bitset_isempty(eventset->e_group)) {
                RemoveFromList(recv_ptr->rec_di->d_eventset_list,
                               (void *)eventset);
                FreeEventSet(eventset);
            }
        }

        // Add this group to the async_set
        AddReceiverToSet(
            async_set, NewGroupReceiver(recv_ptr->rec_di, recv_ptr->rec_group));
    }
}

static void RemoveGroupFromAsyncSet(dbginvoke_t *di, bitset *group) {
    if (di == nullptr || group == nullptr || async_set == nullptr) return;

    // Find entry for this di in the async_set
    for (auto&& recv_ptr : async_set) { 
        if (recv_ptr->rec_di == di) {
            // Remove the group from the bitset
            bitset_andeqnot(recv_ptr->rec_group, group);

            // Check to see if the rec_group is now empty
            if (bitset_isempty(recv_ptr->rec_group)) {
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

/*************************************************
** Internal dbgclient breakpoint handling routines
**************************************************/

/*
** Create a new breakpoint set
*/
bp_set_t::bp_set_t(int bp_id, bitset *group, dbgbp_t *bp)
    : b_bp_set_id(bp_id),
      b_group(bitset_dup(group)),
      b_bp_id_list(NewList()),
      b_bp_enabled(bp->bp_enabled ? bitset_dup(group) : nullptr),
      b_bp_disp(NewList()),
      b_bp_type(bp->bp_type),
      b_bp_pending(bp->bp_pending && bp->bp_stmt != nullptr),
      b_loc_stmt(b_bp_pending ? strdup(bp->bp_stmt) : nullptr),
      b_loc_addr(bp->bp_loc.loc_addr ? strdup(bp->bp_loc.loc_addr) : nullptr),
      b_loc_file(nullptr),
      b_loc_func(nullptr),
      b_loc_line(bp->bp_loc.loc_line),
      b_bp_exp(strdup(bp->bp_exp)),
      b_thread_id(bp->bp_threadId ? strdup(bp->bp_threadId) : nullptr)
{
    // Add the group and disposition to the disp list
    bp_disp_set_t *disp_set = NewBPDispSet(group, bp->bp_disp);
    AddToList(b_bp_disp, (void *)disp_set);

    // Set the id unique to the rank
    bp_id_set_t *id_set = NewBPIDSet(group, bp->bp_id);
    AddToList(b_bp_id_list, id_set);

    if (b_loc_line >= 0) {
        b_loc_file = strdup(bp->bp_loc.loc_file);
        b_loc_func = strdup(bp->bp_loc.loc_func);
    }
}

/*
** Create a new breakpoint disposition set, this goes inside a breakpoint set.
** It tracks the disposition of the breakpoint for each rank
*/
static bp_disp_set_t *NewBPDispSet(bitset *group, dbgbp_disp bp_disp) {
    bp_disp_set_t *disp_set = (bp_disp_set_t *)Alloc(sizeof(bp_disp_set_t));

    disp_set->b_group = bitset_dup(group);
    disp_set->b_bp_disp = bp_disp;

    return disp_set;
}

/*
** Create a new breakpoint id set, this goes inside a breakpoint set.
** It tracks the backend gdb breakpoint id mapping for each rank.
*/
static bp_id_set_t *NewBPIDSet(bitset *group, int bp_id) {
    bp_id_set_t *id_set = (bp_id_set_t *)Alloc(sizeof(bp_id_set_t));

    id_set->b_group = bitset_dup(group);
    id_set->b_bp_id = bp_id;

    return id_set;
}

/*
** Free a breakpoint set.
*/
static void FreeBPSet(bp_set_t *bp_set) {
    delete bp_set;
}

bp_set_t::~bp_set_t() {
    if (b_group != nullptr) bitset_free(b_group);

    if (b_bp_id_list != nullptr)
        DestroyList(b_bp_id_list, (void (*)(void *))FreeBPIDSet);

    if (b_bp_enabled != nullptr) bitset_free(b_bp_enabled);

    if (b_bp_disp != nullptr)
        DestroyList(b_bp_disp, (void (*)(void *))FreeBPDispSet);

    free(b_loc_stmt);
    free(b_loc_addr);
    free(b_loc_file);
    free(b_loc_func);
    free(b_bp_exp);
    free(b_thread_id);
}

/*
** Free a breakpoint id (gdb mapping) set
*/
static void FreeBPIDSet(bp_id_set_t *id_set) {
    if (id_set == nullptr) return;

    bitset_free(id_set->b_group);
    id_set->b_group = nullptr;
    id_set->b_bp_id = -1;

    Free(id_set);
}

/*
** Free a breakpoint disposition set
*/
static void FreeBPDispSet(bp_disp_set_t *disp_set) {
    if (disp_set == nullptr) return;

    bitset_free(disp_set->b_group);

    Free(disp_set);
}

/*
** Process a breakpoint reply set - update internal breakpoint information
** for the dbginvoke_t
*/

static void AddBP(dbginvoke_t *di, dbgbp_t *bp, bitset *group) {
    assert(di != nullptr);
    assert(bp != nullptr);
    assert(group != nullptr);

    bp_set_t *bp_set;
    SetList(di->d_bpset_list);
    while ((bp_set = (bp_set_t *)GetListElement(di->d_bpset_list)) != nullptr) {
        if (!CompareBPSetWithBP(bp_set, bp)) {
            break;
        }
    }

    if (bp_set) /* This is a known breakpoint */
    {
        // update the bp_set with this group and info from the bp for disp and id
        AddBPToBPSet(bp_set, group, bp);
    } else {
        // create a new breakpoint for this group based on the bp
        bp_set = new bp_set_t(++(di->d_global_bp_set_id), group, bp);
        AddToList(di->d_bpset_list, bp_set);
    }
}

static void AddBPToBPSet(bp_set_t *bp_set, bitset *group, dbgbp_t *bp) {
    assert(bp_set != nullptr);
    assert(group != nullptr);
    assert(bp != nullptr);

    // Add this group into the bp_set group
    bitset_oreq(bp_set->b_group, group);

    // Add internal gdb id to the bp_id_list
    bp_id_set_t *id_set = nullptr;
    SetList(bp_set->b_bp_id_list);
    while ((id_set = (bp_id_set_t *)GetListElement(bp_set->b_bp_id_list)) != nullptr) {
        if (id_set->b_bp_id == bp->bp_id) break;
    }

    if (id_set == nullptr) {
        // no existing internal gdb id found, add it.
        id_set = NewBPIDSet(group, bp->bp_id);
        AddToList(bp_set->b_bp_id_list, id_set);
    } else {
        // existing internal gdb id found, add group to existing id_set
        bitset_oreq(id_set->b_group, group);
    }

    // Add this group to the enabled bitset if enabled
    if (bp->bp_enabled) {
        EnableBPForBPSet(bp_set, group);
    }

    // Set the disposition for the group
    SetBPDispositionForBPSet(bp_set, group, bp->bp_disp);
}

static void EnableBPForBPSet(bp_set_t *bp_set, bitset *group) {
    assert (bp_set != nullptr);
    assert (group != nullptr);
    bitset_oreq(bp_set->b_bp_enabled, group);
}

static void DisableBPForBPSet(bp_set_t *bp_set, bitset *group) {
    assert (bp_set != nullptr);
    assert (group != nullptr);
    bitset_andeqnot(bp_set->b_bp_enabled, group);
}

static void SetBPDispositionForBPSet(bp_set_t *bp_set, bitset *group,
                                     dbgbp_disp bp_disp) {
    assert (bp_set != nullptr);
    assert (group != nullptr);

    bool found = false;
    bp_disp_set_t *disp_set;
    SetList(bp_set->b_bp_disp);
    while ((disp_set = (bp_disp_set_t *)GetListElement(bp_set->b_bp_disp)) != nullptr) {
        if (disp_set->b_bp_disp == bp_disp) {
            // This matches the requested disposition
            found = true;
            bitset_oreq(disp_set->b_group, group);
        } else {
            // This doesn't match the requested disposition, so remove the group
            // We do this in case there is any overlap from a previous set
            // disposition command on a set.
            bitset_andeqnot(disp_set->b_group, group);

            // Make sure the disp_set isn't empty, otherwise remove it
            if (bitset_isempty(disp_set->b_group)) {
                RemoveFromList(bp_set->b_bp_disp, (void *)disp_set);
                FreeBPDispSet(disp_set);
            }
        }
    }

    // If we didn't find the disp_set, we need to create a new entry and add it
    // to the list
    if (!found) {
        disp_set = NewBPDispSet(group, bp_disp);
        AddToList(bp_set->b_bp_disp, (void *)disp_set);
    }
}

static void RemoveBPSet(dbginvoke_t *di, bp_set_t *bp_set, bitset *group) {
    assert (bp_set != nullptr);
    assert (group != nullptr);

    // Remove the group from the global bp list
    bitset_andeqnot(bp_set->b_group, group);

    // Remove the group from the bp_id list
    bp_id_set_t *id_set;
    SetList(bp_set->b_bp_id_list);
    while ((id_set = (bp_id_set_t *)GetListElement(bp_set->b_bp_id_list)) != nullptr) {
        bitset_andeqnot(id_set->b_group, group);
        // Make sure the id_set isn't empty, otherwise remove it
        if (bitset_isempty(id_set->b_group)) {
            RemoveFromList(bp_set->b_bp_id_list, (void *)id_set);
            FreeBPIDSet(id_set);
        }
    }

    // Remove the group from the enabled list
    bitset_andeqnot(bp_set->b_bp_enabled, group);

    // Remove the group from the disp list
    bp_disp_set_t *disp_set;
    SetList(bp_set->b_bp_disp);
    while ((disp_set = (bp_disp_set_t *)GetListElement(bp_set->b_bp_disp)) !=
           nullptr) {
        // Try to remove the group from this set
        bitset_andeqnot(disp_set->b_group, group);
        // Make sure the disp_set isn't empty, otherwise remove it
        if (bitset_isempty(disp_set->b_group)) {
            RemoveFromList(bp_set->b_bp_disp, (void *)disp_set);
            FreeBPDispSet(disp_set);
        }
    }

    // Remove the breakpoint set if its now empty
    if (bitset_isempty(bp_set->b_group)) {
        RemoveFromList(di->d_bpset_list, (void *)bp_set);
        FreeBPSet(bp_set);
    }

    return;
}

static int CompareBPSetWithBP(bp_set_t *set, dbgbp_t *bp) {
    // This returns 1 if the breakpoints don't match, otherwise it returns 0 if
    // they do. Do not check the state of the enabled since this can be
    // different for every rank Do not check the state of the disposition since
    // this can be different for every rank. check if both are the same type
    if (set->b_bp_type != bp->bp_type) return 1;

    // Here we only want to check locale info if it is a breakpoint, otherwise
    // if its a watchpoint we want to check the expression.
    switch (bp->bp_type) {
        case DBGBP_BREAK_TYPE:
            // check if both are pending and have the same stmt
            if (bp->bp_pending && bp->bp_stmt != nullptr && set->b_bp_pending && set->b_loc_stmt != nullptr) {
                if (strcmp(set->b_loc_stmt, bp->bp_stmt)) return 1;
            }
            // check if both have the same file name and line number
            if (bp->bp_loc.loc_file != nullptr && set->b_loc_file != nullptr && bp->bp_loc.loc_line >= 0) {
                // First check if the file name matches.
                if (strcmp(set->b_loc_file, bp->bp_loc.loc_file)) return 1;
                // The file name matches, so we can now safely check the line
                // number.
                if (set->b_loc_line != bp->bp_loc.loc_line) return 1;
            }

            // check if both have the same address
            if (bp->bp_loc.loc_addr != nullptr && set->b_loc_addr != nullptr) {
                if (strcmp(set->b_loc_addr, bp->bp_loc.loc_addr)) return 1;
            }

            // check the thread
            {
                const char* t1 = set->b_thread_id, *t2 = bp->bp_threadId;
                if ( (!t1 ^ !t2) || (t1 && t2 && strcmp(t1,t2))) return 1;
            }
            break;

        case DBGBP_WATCH_TYPE:
        case DBGBP_AWATCH_TYPE:
        case DBGBP_RWATCH_TYPE:
            // check if both have the same bp expression
            if (bp->bp_exp != nullptr && set->b_bp_exp != nullptr) {
                if (strcmp(set->b_bp_exp, bp->bp_exp)) return 1;
            }

            break;

        case DBGBP_EXIT_TYPE:
        case DBGBP_UNKNOWN_TYPE:
            // FIXME: What to do here???
            return 1;
    }

    // If we reach this point, everything matches
    return 0;
}

static void FixupPendingBPSetForBP(dbginvoke_t *di, dbgbp_t *bp, bitset *group) {
    assert(di != nullptr);
    assert (bp != nullptr);
    assert (group != nullptr);

    bitset *search_group = nullptr;
    do {
        bitset *remove_group = nullptr;
        bp_set_t *bp_set;
        SetList(di->d_bpset_list);
        while ((bp_set = (bp_set_t *)GetListElement(di->d_bpset_list)) != nullptr) {
            // check to see if this bp_set is pending
            if (bp_set->b_bp_pending) {
                // see if this bp_id is in the id list
                bp_id_set_t *id_set;
                SetList(bp_set->b_bp_id_list);
                while ((id_set = (bp_id_set_t *)GetListElement(bp_set->b_bp_id_list)) != nullptr) {
                    if (id_set->b_bp_id == bp->bp_id) {
                        // Found a matching bp_id! Check if anything in the group matches
                        if (search_group == nullptr) {
                            search_group = bitset_dup(group);
                        }
                        if (bitset_compare(search_group, id_set->b_group)) {
                            // Remove the matching bits, we found a match
                            remove_group = bitset_and(search_group, id_set->b_group);
                            bitset_andeqnot(search_group, id_set->b_group);
                            break;
                        }
                    }
                }
                // See if we found a matching bp_id for group
                if (id_set != nullptr) break;
            }
        }
        // See if we found a matching bp_set, if so we need to clean it up
        if (bp_set != nullptr) {
            assert(remove_group != nullptr);
            RemoveBPSet(di, bp_set, remove_group);
            AddBP(di, bp, remove_group);
            bitset_free(remove_group);
            remove_group = nullptr;
        }
        else {
            // no other pending breakpoints found, we are done
            if (search_group != nullptr) bitset_free(search_group);
            return;
        }
    } while(!bitset_isempty(search_group));
    bitset_free(search_group);
}

bp_set_t *FindBPSetByID(dbginvoke_t *di, int id) {
    assert(di != nullptr);
    if (id < 1) return nullptr;

    bp_set_t *bp_set;
    SetList(di->d_bpset_list);
    while ((bp_set = (bp_set_t *)GetListElement(di->d_bpset_list)) != nullptr) {
        if (bp_set->b_bp_set_id == id) {
            break;
        }
    }
    return bp_set;
}

bp_set_t *FindBPSetForBP(dbginvoke_t *di, dbgbp_t *bp) {
    assert(di != nullptr);
    assert(bp != nullptr);

    bp_set_t *bp_set;
    SetList(di->d_bpset_list);
    while ((bp_set = (bp_set_t *)GetListElement(di->d_bpset_list)) != nullptr) {
        if (!CompareBPSetWithBP(bp_set, bp)) {
            break;
        }
    }
    return bp_set;
}

void ResetBPSetList(dbginvoke_t *di) {
    assert(di != nullptr);
    SetList(di->d_bpset_list);
}

bp_set_t *GetNextBPSet(dbginvoke_t *di) {
    assert(di != nullptr);
    return (bp_set_t *)GetListElement(di->d_bpset_list);
}

int EmptyBPSetList(dbginvoke_t *di) {
    assert(di != nullptr);
    return EmptyList(di->d_bpset_list);
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
    struct timeval tv = {1, 0};
    int doUnlock = 0;

    // reset the fd_set
    FD_ZERO(&dispatch_fds);
    high_fd = -1;

    /*
    ** Check if any events are ready.
    */

#if defined(RD)
    /*
    ** Begin by checking if there is an active graph that needs dispatching.
    */
    if (ActiveDF()) {
        ++_df_ready;
        res = 1;
    }
#endif /* RD */

    // Check the protocols for each dbginvoke if there are any.
    if (!EmptyList(DbgInvokeList)) {
        // Reset the DbgInvokeList
        SetList(DbgInvokeList);

        // Loop through each dbginvoke entry and check for events.
        while ((di = (dbginvoke_t *)GetListElement(DbgInvokeList)) != nullptr) {
            /*
            ** Note: This should be used for protocols that do not use an event
            *notification
            **       fd. If a event notification fd can be used, then it should
            *be added to
            **       the sw_evfd list and sw_checkevents should simply return 0.
            */
            if (DBG_PROTO_SW(di)->sw_checkevents !=
                    nullptr  // if the protocol has a checkevent function defined
                && DBG_PROTO_SW(di)->sw_checkevents(di) >
                       0  // if checkevent tells us that there is an event.
            ) {
                DPRINT("DbgCheckEvents: sw_checkevents returned 1\n");
                return 1;  // there is a proto event to handle. Return
                           // immediately.
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
    pthread_mutex_lock(&InputCB.i_lock);
    if (InputCB.i_fd >= 0) {
        if (high_fd < InputCB.i_fd) {
            high_fd = InputCB.i_fd;
        }
        // DPRINT("DbgCheckEvents: InputCB fd %d added.\n", InputCB.i_fd);
        FD_SET(InputCB.i_fd, &dispatch_fds);
        doUnlock = 1;
    } else {
        pthread_mutex_unlock(&InputCB.i_lock);
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
                            if (doUnlock) pthread_mutex_unlock(&InputCB.i_lock);
                            return res;
                        }
                        continue;

                    // We expect EINVAL if there are no fd's in the fd_set. The
                    // high_fd should be negative in this case and select will
                    // return with -1 and errno set to EINVAL.
                    case EINVAL:
                        DPRINT("DbgCheckEvents: EINVAL\n");
                        if (doUnlock) pthread_mutex_unlock(&InputCB.i_lock);
                        return res;

                    default:
                        PrintOutError("select: %s\n", strerror(errno));
                        if (doUnlock) pthread_mutex_unlock(&InputCB.i_lock);
                        return -1;
                }
                break;

            case 0:
                // DPRINT("DbgCheckEvents: select returned 0, res is %d\n",
                // res);
                if (doUnlock) pthread_mutex_unlock(&InputCB.i_lock);
                return res;

            default:
                if (doUnlock) {
                    // If the InputCB was set, then we don't want to dispatch
                    // the graph. Commands can take awhile to return and if the
                    // user was typing something we don't want to issue a new
                    // command. The user has a second to type before being
                    // locked out.
                    if (FD_ISSET(InputCB.i_fd, &dispatch_fds)) {
                        _df_ready = 0;
                    }
                    pthread_mutex_unlock(&InputCB.i_lock);
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

            // Ensure there is a protocol specific dispatch function
            if (DBG_PROTO_SW(di)->sw_dispatchevents != nullptr) {
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
                            DBG_PROTO_SW(di)->sw_dispatchevents(di, *ev_fd);
                        }
                    }
                } else {
                    // event notification fd's not in use by this protocol, so
                    // call the generic dispatch function with -1 for event fd
                    // arg.
                    DBG_PROTO_SW(di)->sw_dispatchevents(di, -1);
                }
            }
        }
    }

    /*
    ** Process any input.
    */
    pthread_mutex_lock(&InputCB.i_lock);
    if ((InputCB.i_fd >= 0) && FD_ISSET(InputCB.i_fd, &dispatch_fds) &&
        (InputCB.i_rtn != nullptr)) {
        // DPRINT("DbgDispatchEvents: InputCB.i_fd is set\n");
        InputCB.i_rtn();
    }
    pthread_mutex_unlock(&InputCB.i_lock);

    // We want to process the df graph last since the above can generate tokens
#if defined(RD)
    /*
    ** If the DbgCheckEvents fuction flagged _df_ready, then we need to
    ** call the dataflow dispatch routine.
    */
    if (_df_ready) {
        _df_ready = 0;
        if (!DispatchDF()) EndActiveDF();  // Finish the current graph execution
    }
#endif /* RD */
}

int DbgDispatchProtoEvents(dbginvoke_t *di, proto_msg_set *set) {
    dbgev last_event = DBGEV_NONE;
    proto_message *msg;
    event_set_t *eventset = nullptr;
    dbgevent_t *event;
    bp_set_t *bp_set;
    dbgbp_t *bp;
    bitset *group;
    bp_disp_set_t *disp_set;

    if (set == nullptr) return 0;

    ResetProtoMsgSet(set);
    while ((msg = NextProtoMsgInSet(set)) != nullptr) {
        // convert this protocol msg string to an event
        event = SocProtoToEvent(msg->message);

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
                FixupPendingBPSetForBP(di, bp, msg->group);
                if ((bp_set = FindBPSetForBP(di, bp)) != nullptr) {
                    // if the breakpoint was a watchpoint we need to update the
                    // watchpoint locale information
                    switch (bp_set->b_bp_type) {
                        case DBGBP_WATCH_TYPE:
                        case DBGBP_AWATCH_TYPE:
                        case DBGBP_RWATCH_TYPE:
                            bp_set->b_bp_watch_loc.add(msg->group, bp->bp_loc.loc_file, bp->bp_loc.loc_line, bp->bp_loc.loc_func);
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
        RemoveGroupFromAsyncSet(di, msg->group);

        eventset = AddEvent(di, msg->group, event);

        // This is the callback that was set by calling DbgHandleEvent
        (*(di->d_clnt_ev_callback))(eventset, msg->group);

        // conduct special post-processing for certain events
        switch (EVENT_EV(event)) {
            // Post process breakpoints
            case DBGEV_BPHIT:
                bp = EVENT_BP(event);
                if ((bp_set = FindBPSetForBP(di, bp)) != nullptr) {
                    // If the breakpoint was set to delete, we need to delete
                    // it. If it was set to dis, we need to disable it.
                    SetList(bp_set->b_bp_disp);
                    while ((disp_set = (bp_disp_set_t *)GetListElement(
                                bp_set->b_bp_disp)) != nullptr) {
                        group = bitset_and(disp_set->b_group, msg->group);
                        if (bitset_isempty(group)) {
                            bitset_free(group);
                            continue;
                        }
                        switch (disp_set->b_bp_disp) {
                            case DBGBP_DELETE_DISP:
                                // remove the breakpoint
                                // NOTE: The breakpoint has already been removed
                                // by the backend.
                                RemoveBPSet(di, bp_set, group);
                                break;
                            case DBGBP_DISABLE_DISP:
                                // mark the breakpoint as disabled for the ranks
                                // in the reply set.
                                DisableBPForBPSet(bp_set, group);
                                break;
                            default:
                                // do nothing
                                break;
                        }
                        bitset_free(group);
                    }
                }
                break;

            case DBGEV_BPDEL:
                bp = EVENT_BP(event);
                if ((bp_set = FindBPSetForBP(di, bp)) != nullptr) {
                    // Need to delete the breakpoint.
                    SetList(bp_set->b_bp_disp);
                    while ((disp_set = (bp_disp_set_t *)GetListElement(
                                bp_set->b_bp_disp)) != nullptr) {
                        group = bitset_and(disp_set->b_group, msg->group);
                        if (bitset_isempty(group)) {
                            bitset_free(group);
                            continue;
                        }
                        // remove the breakpoint
                        // NOTE: The breakpoint has already been removed by the
                        // backend.
                        RemoveBPSet(di, bp_set, group);
                        bitset_free(group);
                    }
                }
                break;

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
                CheckForFullEventSet(di, di->d_bitset_full, last_event);
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
        CheckForFullEventSet(di, di->d_bitset_full, last_event);
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
    OutputCB.o_rtn(di, p_o_stream);
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
void DbgHandleOutput(void (*outp)(dbginvoke_t *, FILE *)) {
    OutputCB.o_rtn = outp;  // callback handler
}

/*
** Set local input stream handler.
*/
void DbgHandleInput(int fd, void (*inp)(void)) {
    pthread_mutex_lock(&InputCB.i_lock);
    InputCB.i_fd = fd;
    InputCB.i_rtn = inp;
    pthread_mutex_unlock(&InputCB.i_lock);
}

/*
** Mask input stream events temporarily
*/
void DbgMaskInput(void) {
    pthread_mutex_lock(&InputCB.i_lock);
    if (_mask_cnt == 0) {
        _mask_fd = InputCB.i_fd;
        InputCB.i_fd = -1;
    }
    ++_mask_cnt;
    pthread_mutex_unlock(&InputCB.i_lock);
}

/*
** Remove temporary input stream mask.
*/
void DbgUnmaskInput(void) {
    pthread_mutex_lock(&InputCB.i_lock);
    if (_mask_cnt > 0) {
        --_mask_cnt;
    }
    if (_mask_cnt == 0) {
        InputCB.i_fd = _mask_fd;
    }
    pthread_mutex_unlock(&InputCB.i_lock);
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
    dbgclnt_proto *sw;

    SetClntProto();

    while ((sw = GetClntProto()) != nullptr)
        if (sw->sw_init != nullptr && !sw->sw_init()) return -1;

    // Initialize the mutex lock for the InputCB
    InputCB.i_fd = -1;
    if (pthread_mutexattr_init(&InputCB.i_lock_attr)) return -1;
    if (pthread_mutexattr_settype(&InputCB.i_lock_attr,
                                  PTHREAD_MUTEX_RECURSIVE))
        return -1;
    if (pthread_mutex_init(&InputCB.i_lock, &InputCB.i_lock_attr)) return -1;

    return 0;
}

void DbgFinalize(void) {
    dbgclnt_proto *sw;
    dbginvoke_t *di;

    SetClntProto();

    // Cleanup the current session if there is one
    if (DbgCurrSess != nullptr) {
        sw = DbgCurrSess->sess_proto;
        if (sw->sw_cleanup_session == nullptr) {
            PrintOutError(
                "Protocol function sw_cleanup_session not implemented.\n");
        } else {
            sw->sw_cleanup_session(DbgCurrSess);
        }
        DbgCurrSess = nullptr;
    }

    while ((sw = GetClntProto()) != nullptr) {
        if (sw->sw_finish != nullptr) {
            sw->sw_finish();
        }
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
    pthread_mutexattr_destroy(&InputCB.i_lock_attr);
    pthread_mutex_destroy(&InputCB.i_lock);
}

/*
** Create a new session
*/
proto_session *DbgSessionCreate(const char *proto, WLM_type manager,
                                char *batch_template, char *hostname,
                                char *username) {
    dbgclnt_proto *ptab;

    if ((ptab = FindClntProto(proto)) == nullptr) {
        PrintOutError("No such protocol \"%s\"\n", proto);
        return nullptr;
    }

    if (ptab->sw_create_session == nullptr) {
        PrintOutError("Protocol function sw_create_session not implemented.\n");
        return nullptr;
    }

    return ptab->sw_create_session(ptab, manager, batch_template, hostname,
                                   username);
}

/*
** Set the session to associate all subsequent launches/attaches with.
** If a session is currently set as being open, clean it up if possible.
** It's up to the protocol implementation to handle the actual cleanup.
*/
void DbgSetSession(proto_session *sess) {
    dbgclnt_proto *ptab;

    // Cleanup the current session if there is one
    if (DbgCurrSess != nullptr) {
        ptab = DbgCurrSess->sess_proto;
        if (ptab->sw_cleanup_session == nullptr) {
            PrintOutError(
                "Protocol function sw_cleanup_session not implemented.\n");
        } else {
            ptab->sw_cleanup_session(DbgCurrSess);
        }
    }

    // Ask the protocol layer to keep the new session around if its not null
    if (sess != nullptr) {
        ptab = sess->sess_proto;
        if (ptab->sw_save_session == nullptr) {
            PrintOutError(
                "Protocol function sw_save_session not implemented.\n");
        } else {
            ptab->sw_save_session(sess);
        }
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
    dbgclnt_proto *ptab;

    if ((ptab = FindClntProto(proto)) == nullptr) {
        PrintOutError("No such protocol \"%s\"\n", proto);
        return nullptr;
    }

    if (ptab->sw_attach == nullptr) {
        PrintOutError("Protocol function sw_attach not implemented.\n");
        return nullptr;
    }

    // Create the dbginvoke_t object to pass down, nproc is unknown since this
    // is the attach case
    di = NewDbgInvoke(handle, DBG_ATTACHED, 0, ptab);

    if (ptab->sw_attach(di, sess, app_ident, arch, backend, gpu_flag,
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
                       char **argv, const char *handle, int nproc,
                       const char *arch, char **arch_args,
                       const char *arch_input, const char *backend, const char* gdb,
                       char **env,
                       const char *cwd, int gpu_flag, int debug_flag,
                       int unsafe_flag, int early_entry_flag, bool isNonStop) {
    dbginvoke_t *di;
    dbgclnt_proto *ptab;
    char **ap;
    char **ptr;
    int ap_argc = 0;
    dbgevent_t *e;

    if ((ptab = FindClntProto(proto)) == nullptr) {
        PrintOutError("No such protocol \"%s\"\n", proto);
        return nullptr;
    }

    if (ptab->sw_launch == nullptr) {
        PrintOutError("Protocol function sw_launch not implemented.\n");
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
    if (!unsafe_flag) {
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

    // Create the cmd str and set it
    ap = NewArgv(prog, nullptr);
    ap = AppendArgv(ap, argv);
    // count the number of args
    for (ptr = ap; *ptr != nullptr; ++ptr) {
        ++ap_argc;
    }
    di->d_cmd = ArgsToBuf(ap, ap_argc);
    FreeArgv(ap);

    if (ptab->sw_launch(di, sess, prog, argv, arch, arch_args, arch_input,
                        backend, gdb, env, cwd, gpu_flag, debug_flag,
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

    // ensure the send_cmd function is implemented
    if (DBG_PROTO_SW(di)->sw_send_cmd == nullptr) {
        PrintOutError("Protocol function sw_send_cmd not implemented.\n");
        return 1;
    }

    set->_cmd_sent = DBG_PROTO_SW(di)->sw_send_cmd(di, set->rec_group, request.c_str());

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
    proto_msg_set *proto_res;
    proto_message *msg;
    rep_set_t *reply;
    group_reply_t *rep;
    dbgevent_t *e;
    dbgev ev;

    bool isFinal = true;

    // ensure the recv_reply function is implemented
    if (DBG_PROTO_SW(di)->sw_recv_reply == nullptr) {
        PrintOutError("Protocol function sw_recv_cmd not implemented.\n");
        return nullptr;
    }

    proto_res =
        DBG_PROTO_SW(di)->sw_recv_reply(di, set->rec_group, set->_cmd_sent);

    reply = NewRepSet();

    if (proto_res == nullptr) {
        // Create a new error event based on this reply
        EVENT_ERROR(e, DBGERR_INTERNAL,
                    "Protocol returned null proto_msg_set.\n");
        rep = NewGroupReply(di, bitset_dup(set->rec_group), e);
        // Add the error to the reply set
        AddErrToSet(reply, rep);
        ClearCmdSent(set);
        return reply;
    }

    // process the proto_res
    while ((msg = NextProtoMsgInSet(proto_res)) != nullptr) {
        // convert this protocol msg string to an event
        e = SocProtoToEvent(msg->message);
        ev = EVENT_EV(e);

        switch (ev) {
            // Errors get added to the special error list
            case DBGEV_ERROR:
                rep = NewGroupReply(di, bitset_dup(msg->group), e);
                // Add this error to the reply set
                AddErrToSet(reply, rep);
                break;

            // Any event without data should be handled here
            case DBGEV_OK:
                if (ev != req_type) {
                    // Create a new error event based on this reply
                    FreeEvent(e);
                    EVENT_ERROR(
                        e, DBGERR_INTERNAL,
                        "Received event of type %s, expected type %s.\n",
                        DbgevToStr(ev), DbgevToStr(req_type));
                    rep = NewGroupReply(di, bitset_dup(msg->group), e);
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
                    rep = NewGroupReply(di, bitset_dup(msg->group), e);
                    // Add the error to the reply set
                    AddErrToSet(reply, rep);
                    break;
                }
                // Create a reply out of this event
                rep = NewGroupReply(di, bitset_dup(msg->group), e);
                // Add this reply to the reply set
                AddReplyToSet(reply, rep);
                break;
        }
    }

    // Free the proto set, we are done with it.
    FreeProtoMsgSet(proto_res);


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

    mergeReplySliceSet(slices, nullptr, nullptr);

    m_isSliced = m_returnedSlice0 = m_moreSlices =  false;
}

// merge together a packetized reply into a monolithic rep_set
// Note that this is a short term function, the goal it to process the
// result slices as they arrive; storing the full set of slices
// in memory does not help (actually hinders) the memory usage on the client
// side
void rep_set_t::mergeReplySliceSet(const std::vector<std::unique_ptr<rep_set_t>>& slices,
                                   arrayDimension_t* shape, bitset* group)
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
        bitset* toproc = nullptr;
        if (group) {
            toproc = bitset_dup(group);
        } else {
            for (auto&& r : tiers[0]) {
                if (toproc)
                    bitset_oreq(toproc, r->rep_group);
                else
                    toproc = bitset_dup(r->rep_group);
            }
        }

        // the algorithm here isn't too complicated.  We're just looking to pick
        // one group_reply from each tier which share at least one process.
        // Merge that set into a single group reply, removed the shared process nodes
        // from \a toproc and keep doing so until every process has been accounted for.
        std::vector<group_reply*> newSlice;
        bitset* setset = bitset_dup(toproc);
        while (!bitset_isempty(toproc)) {
            // find a common reply set
            newSlice.clear();
            bitset_copy(setset, toproc);
            for (auto&& tier : tiers) {
                for (auto&& gp : tier) {
                    if (bitset_compare(setset, gp->rep_group)) {
                        bitset_andeq(setset, gp->rep_group);
                        newSlice.push_back(gp);
                        break;
                    }
                }
            }

            // remove this set from the process set to process
            bitset_andeqnot(toproc, setset);

            if (!group && newSlice.size() != numTiers) {
                DPRINT("reply mismatch in MergeReplySlices\n");
                break;
            }

            // update the set of replies left to process
            if (newSlice.size() == numTiers) {
                for (size_t i=0; i<numTiers; ++i) {
                    auto reply = newSlice[i];
                    auto& tier = tiers[i];
                    if (!bitset_compare(reply->rep_group, toproc)) {
                        // no processes left unprocessed - remove it from the
                        // list because we no longer need to look at it
                        tier.erase(std::find(tier.begin(), tier.end(), reply));
                    }
                }
            }

            // we have our set of overlapping replies, join the events together
            // and add to the monolithic return set.
            bool isError;
            if (group_reply_t* newReply = JoinReplies(setset, newSlice, shape, isError)) {
                if (isError) {
                    m_errs.push_back(newReply);
                } else {
                    m_reps.push_back(newReply);
                }
            }
        }

        // cleanup and return
        bitset_free(toproc);
        bitset_free(setset);
    }
}


namespace {
// Manage a bunch of *in memory* AIF slices as an AIF stream.
// This class is temporary (since it still potentially tries to
// hold a data structure of nearly unlimited size in memory at once).
// We define it here just in order to access the joinAIF functionality
// in AIFStream.  (which is also a temporary expedient).
class LocalSlices : public AIFStream::Slices {
public:
    LocalSlices(arrayDimension_t* shape) : m_shape(shape ? dupArrayDimension(shape) : nullptr) {}
    ~LocalSlices() { if (m_shape) freeArrayDimension(m_shape); }
    void addSlice(AIF* aif);

    virtual arrayDimension_t* getFullShape() const override;
    virtual AIF* getNextSlice(bool& isFinal);
    virtual void freeSlice(AIF* aif) override;
private:
    std::vector<AIF*> m_aifs;
    std::vector<AIF*>::iterator m_current;
    mutable arrayDimension_t* m_shape;
};

void LocalSlices::addSlice(AIF* aif) {
    m_aifs.push_back(aif);
    m_current = m_aifs.begin();
}

// compute the full shape of the AIF array by combining the information from the slices
// if necessary
arrayDimension_t* LocalSlices::getFullShape() const
{
    if (m_shape)
        return m_shape;

    int* fullMins = nullptr;
    int* fullMaxs = nullptr;
    int fullDim = 0;
    bool isFortran = false;

    for (auto&& aif : m_aifs) {
        isFortran = (AIFType(aif) == AIF_FTN_ARRAY);
        int dim = AIFArrayRank(aif);
        if (dim <= 0) {
            DPRINT("could not get array dimension\n");
            return nullptr;
        }

        // get the array bounds, (note not using AIFArrayBounds because it
        // errors out on a fortran array)
        int* mins, *maxs;
        if (AIFType(aif) != AIF_ARRAY && AIFType(aif) != AIF_FTN_ARRAY) {
            fprintf(stderr, "aif is not an array type\n");
            return nullptr;
        }
        FDSArrayBounds(AIF_FORMAT(aif), dim, &mins, &maxs, nullptr);

        if (!fullMins) {
            fullMins = mins;
            fullMaxs = maxs;
            fullDim = dim;
        } else if (dim == fullDim) {
            for (int i=0; i<dim; ++i) {
                fullMins[i] = std::min(fullMins[i], mins[i]);
                fullMaxs[i] = std::max(fullMaxs[i], maxs[i]);
            }
            free(mins);
            free(maxs);
        } else {
            DPRINT("dimensions to not match\n");
            return nullptr;
        }
    }

    // Got the bounds, create the array dimension object
    arrayDimension_t *shape = newArrayDimension();
    if (!shape) {
        DPRINT("newArrayDimension failed");
        return nullptr;
    }

    shape->isFortran = isFortran;

    for (int i=0; i<fullDim; ++i) {
        // arrayDimension orders dims from fastest to slowest
        int d = (isFortran ? i : fullDim-1-i);
        addArrayDimensionDef(shape, fullMaxs[d]-fullMins[d]+1,
                             fullMins[d], fullMaxs[d]);
    }

    free(fullMins);
    free(fullMaxs);

    m_shape = shape;

    return shape;
}

AIF* LocalSlices::getNextSlice(bool& isFinal)
{
    if (m_current == m_aifs.end()) {
        return nullptr;
    } else {
        isFinal = (m_current+1 == m_aifs.end());
        return *(m_current++);
    }
}
// LocalSlices does not control the aif objects which may be reused
void LocalSlices::freeSlice(AIF* aif) {}
}

// Create a single unsliced group_reply from a set of slices
// \a group the process set for the new result
// \a replies the slices for the result
// \a shape if given is the array shape for the returned result.   This is necessary
//     if \a replies are a sparse representation, otherwise the function can
//     compute the shape by combining the array sizes in \a replies
// \a isError will contain true on return if any slice in \a replies was an error
static group_reply_t* JoinReplies(bitset* group,
                                  const std::vector<group_reply*>& replies,
                                  arrayDimension_t* shape,
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
        auto assemblerSlices = std::make_shared<LocalSlices>(shape);
        for (auto&& reply : replies) {
            assemblerSlices->addSlice(EVENT_DATA(reply->rep_event)->joinAIF());
        }

        AIFStream assembler(assemblerSlices);
        AIF* joinedAIF = assembler.joinAIF(AIFStream::STEAL);
        data = new AIFStream(joinedAIF);

        if (!data)
            isError = true;
    }

    // and create the group reply
    dbgevent_t* event = NewEvent(isError ? DBGEV_ERROR : DBGEV_DATA);
    if (data) {
        EVENT_DATA(event) = data;
    }

    group_reply_t* groupReply = NewGroupReply(di, bitset_dup(group), event);
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
rep_set_t *DbgEvalExpr(rec_set_t *set, char *exp) {

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
rep_set_t *DbgSetDebuggerVar(rec_set_t *set, char *var, AIF *val) {

    auto request = CommandToSocProto(DBGCMD_SETDEBUGGERVAR, var, val);

    rep_set_t *result = DbgSendCmdAndRecvReply(set, request, DBGEV_OK);
    return result;
}

/*
** DBG_GETDEBUGGERVAR: Get variable in debugger.
*/
rep_set_t *DbgGetDebuggerVar(rec_set_t *set, char *var) {
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
                                           bitset_dup(recv_ptr->rec_group), e);

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
                                           bitset_dup(recv_ptr->rec_group), e);

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

/*
** DBG_SETVAR: Set variable in program to new value.
*/
rep_set_t *DbgSetVar(rec_set_t *set, const char *var, AIF *val) {
    auto request = CommandToSocProto(DBGCMD_SETVAR, var, val, set->getThreads());

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
rep_set_t *DbgSetLineBreak(rec_set_t *set, int allowPending, char *file, int line,
                           const char* threadSet) {

    auto request = CommandToSocProto(DBGCMD_SETLINEBREAK, allowPending, file, line, threadSet);

    auto result = DbgSendCmdAndRecvReply(set, request, DBGEV_BPSET);

    // process breakpoint information
    for (auto&& reply : result) {
        AddBP((dbginvoke_t *)reply->rep_di, EVENT_BP(reply->rep_event), reply->rep_group);
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
        AddBP((dbginvoke_t *)reply->rep_di, EVENT_BP(reply->rep_event), reply->rep_group);
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
        AddBP((dbginvoke_t *)reply->rep_di, EVENT_BP(reply->rep_event), reply->rep_group);
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

        if (DBG_PROTO_SW(di)->sw_exit == nullptr) {
            if (result == nullptr) {
                result = NewRepSet();
            }

            // Create a new error event
            auto e = NewErrorEvent(DBGERR_NOTIMP, nullptr);
            auto err_rep = NewGroupReply(di, bitset_dup(recv_ptr->rec_group), e);
            // Add the error to the reply set
            AddErrToSet(result, err_rep);

        } else {
            // We always want to kill the application, so hardcode detatch to 0
            DBG_PROTO_SW(di)->sw_exit(di, 0);
        }

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

    bp_id_set_t *id_set;
    rep_set_t *result = nullptr;

    // if bp_set_num is 0, we want this to apply to every breakpoint associated
    // with each dbginvoke_t
    for (auto&& recv_ptr : set) {
        // point at the di
        auto di = recv_ptr->rec_di;

        if (bp_set_num == 0) {
            ResetBPSetList(di);
            for (bp_set_t* bp_set; (bp_set=GetNextBPSet(di)); ) {
                // We send the command to the backend based on the backend gdb
                // id. Loop over this list
                SetList(bp_set->b_bp_id_list);
                while ((id_set = (bp_id_set_t *)GetListElement(
                            bp_set->b_bp_id_list)) != nullptr) {
                    // Get the new group, based on the intersection between the
                    // bp_set and the rec_set.
                    auto group = bitset_and(id_set->b_group, recv_ptr->rec_group);

                    // Ensure that there are ranks in the bitset, otherwise
                    // continue
                    if (bitset_isempty(group)) {
                        bitset_free(group);
                        continue;
                    }

                    // Create the request for this group
                    auto request =
                        CommandToSocProto(DBGCMD_DELETEBREAK, id_set->b_bp_id);

                    // Create the new rec set for this group
                    auto new_rec_set = NewRecSet();
                    AddReceiverToSet(new_rec_set, NewGroupReceiver(di, group));

                    auto tmp_result =
                        DbgSendCmdAndRecvReply(new_rec_set, request, DBGEV_OK);

                    // We expect this to return nullptr on success, handle the case
                    // were things worked
                    if (tmp_result == nullptr) {
                        // remove the breakpoint
                        RemoveBPSet(di, bp_set, group);
                    }

                    // Add this to the overall result
                    if (result == nullptr) {
                        result = tmp_result;
                    } else {
                        if (tmp_result != nullptr) {
                            result = MergeRepSet(result, tmp_result);
                        }
                    }

                    bitset_free(group);
                    FreeRecSet(new_rec_set);
                }
            }
        } else {
            // find the bp_set for the bp_set_num
            auto bp_set = FindBPSetByID(di, bp_set_num);
            if (bp_set == nullptr) {
                // Error case: the provided bp_set_num does not exist
                dbgevent_t *e;

                // Create a new error event
                EVENT_ERROR(e, DBGERR_DEBUGGER, "No breakpoint number %d.\n",
                            bp_set_num);
                // Add the event to the result based on its type
                auto group_rep =
                    NewGroupReply(di, bitset_dup(recv_ptr->rec_group), e);
                // Create a new result and return it
                result = NewRepSet();
                AddErrToSet(result, group_rep);
                continue;
            }

            // Need to loop over the b_bp_id_list to operate on the correct
            // breakpoint number in the backend
            SetList(bp_set->b_bp_id_list);
            while ((id_set = (bp_id_set_t *)GetListElement(
                        bp_set->b_bp_id_list)) != nullptr) {
                // Get the new group, based on the intersection between the
                // bp_set and the set.
                auto group = bitset_and(id_set->b_group, recv_ptr->rec_group);

                // Ensure that there are ranks in the bitset, otherwise continue
                if (bitset_isempty(group)) {
                    bitset_free(group);
                    continue;
                }

                // Create the request for this group
                auto request =
                    CommandToSocProto(DBGCMD_DELETEBREAK, id_set->b_bp_id);

                // Create the new rec set for this group
                auto new_rec_set = NewRecSet();
                AddReceiverToSet(new_rec_set, NewGroupReceiver(di, group));

                auto tmp_result =
                    DbgSendCmdAndRecvReply(new_rec_set, request, DBGEV_OK);

                // We expect this to return nullptr on success, handle the case
                // were things worked
                if (tmp_result == nullptr) {
                    // remove the breakpoint
                    RemoveBPSet(di, bp_set, group);
                }

                // Add this to the overall result
                if (result == nullptr) {
                    result = tmp_result;
                } else {
                    if (tmp_result != nullptr) {
                        result = MergeRepSet(result, tmp_result);
                    }
                }

                bitset_free(group);
                FreeRecSet(new_rec_set);
            }
        }
    }

    return result;
}

/*
** DBG_DISABLEBREAK: Disable a breakpoint.
*/
rep_set_t *DbgDisableBreak(rec_set_t *set, int bp_set_num) {
    if (!set) return nullptr;

    bp_set_t *bp_set;
    bp_id_set_t *id_set;
    rep_set_t *result = nullptr;

    // if bp_set_num is 0, we want this to apply to every breakpoint associated
    // with each dbginvoke_t
    for (auto&& recv_ptr : set) {
        // point at the di
        auto di = recv_ptr->rec_di;

        if (bp_set_num == 0) {
            ResetBPSetList(di);
            while ((bp_set = GetNextBPSet(di)) != nullptr) {
                // We send the command to the backend based on the backend gdb
                // id. Loop over this list
                SetList(bp_set->b_bp_id_list);
                while ((id_set = (bp_id_set_t *)GetListElement(
                            bp_set->b_bp_id_list)) != nullptr) {
                    // Get the new group, based on the intersection between the
                    // bp_set and the rec_set.
                    auto group = bitset_and(id_set->b_group, recv_ptr->rec_group);

                    // Ensure that there are ranks in the bitset, otherwise
                    // continue
                    if (bitset_isempty(group)) {
                        bitset_free(group);
                        continue;
                    }

                    // Create the request for this group
                    auto request =
                        CommandToSocProto(DBGCMD_DISABLEBREAK, id_set->b_bp_id);

                    // Create the new rec set for this group
                    auto new_rec_set = NewRecSet();
                    AddReceiverToSet(new_rec_set, NewGroupReceiver(di, group));

                    auto tmp_result =
                        DbgSendCmdAndRecvReply(new_rec_set, request, DBGEV_OK);

                    // We expect this to return nullptr on success, handle the case
                    // were things worked
                    if (tmp_result == nullptr) {
                        // mark the breakpoint as enabled for this group
                        DisableBPForBPSet(bp_set, group);
                    }

                    // Add this to the overall result
                    if (result == nullptr) {
                        result = tmp_result;
                    } else {
                        if (tmp_result != nullptr) {
                            result = MergeRepSet(result, tmp_result);
                        }
                    }

                    bitset_free(group);
                    FreeRecSet(new_rec_set);
                }
            }
        } else {
            // find the bp_set for the bp_set_num
            if ((bp_set = FindBPSetByID(di, bp_set_num)) == nullptr) {
                // Error case: the provided bp_set_num does not exist
                dbgevent_t *e;

                // Create a new error event
                EVENT_ERROR(e, DBGERR_DEBUGGER, "No breakpoint number %d.\n",
                            bp_set_num);
                // Add the event to the result based on its type
                auto group_rep =
                    NewGroupReply(di, bitset_dup(recv_ptr->rec_group), e);
                // Create a new result and return it
                if (result == nullptr) {
                    result = NewRepSet();
                }
                AddErrToSet(result, group_rep);
                continue;
            }

            // Need to loop over the b_bp_id_list to operate on the correct
            // breakpoint number in the backend
            SetList(bp_set->b_bp_id_list);
            while ((id_set = (bp_id_set_t *)GetListElement(
                        bp_set->b_bp_id_list)) != nullptr) {
                // Get the new group, based on the intersection between the
                // bp_set and the rec_set.
                auto group = bitset_and(id_set->b_group, recv_ptr->rec_group);

                // Ensure that there are ranks in the bitset, otherwise continue
                if (bitset_isempty(group)) {
                    bitset_free(group);
                    continue;
                }

                // Create the request for this group
                auto request =
                    CommandToSocProto(DBGCMD_DISABLEBREAK, id_set->b_bp_id);

                // Create the new rec set for this group
                auto new_rec_set = NewRecSet();
                AddReceiverToSet(new_rec_set, NewGroupReceiver(di, group));

                auto tmp_result =
                    DbgSendCmdAndRecvReply(new_rec_set, request, DBGEV_OK);

                // We expect this to return nullptr on success, handle the case
                // were things worked
                if (tmp_result == nullptr) {
                    // mark the breakpoint as disabled for this group
                    DisableBPForBPSet(bp_set, group);
                }

                // Add this to the overall result
                if (result == nullptr) {
                    result = tmp_result;
                } else if (tmp_result != nullptr) {
                    result = MergeRepSet(result, tmp_result);
                }

                bitset_free(group);
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

    bp_set_t *bp_set;
    bp_id_set_t *id_set;
    rep_set_t *result = nullptr;

    // if bp_set_num is 0, we want this to apply to every breakpoint associated
    // with each dbginvoke_t
    for (auto&& recv_ptr : set) {
        // point at the di
        auto di = recv_ptr->rec_di;

        if (bp_set_num == 0) {
            ResetBPSetList(di);
            while ((bp_set = GetNextBPSet(di)) != nullptr) {
                // We send the command to the backend based on the backend gdb
                // id. Loop over this list
                SetList(bp_set->b_bp_id_list);
                while ((id_set = (bp_id_set_t *)GetListElement(
                            bp_set->b_bp_id_list)) != nullptr) {
                    // Get the new group, based on the intersection between the
                    // bp_set and the rec_set.
                    auto group = bitset_and(id_set->b_group, recv_ptr->rec_group);

                    // Ensure that there are ranks in the bitset, otherwise
                    // continue
                    if (bitset_isempty(group)) {
                        bitset_free(group);
                        continue;
                    }

                    // Create the request for this group
                    auto request = CommandToSocProto(DBGCMD_ENABLEBREAK,
                                                id_set->b_bp_id, once, del);

                    // Create the new rec set for this group
                    auto new_rec_set = NewRecSet();
                    AddReceiverToSet(new_rec_set, NewGroupReceiver(di, group));

                    auto tmp_result =
                        DbgSendCmdAndRecvReply(new_rec_set, request, DBGEV_OK);

                    // We expect this to return nullptr on success, handle the case
                    // were things worked
                    if (tmp_result == nullptr) {
                        // mark the breakpoint as enabled for this group
                        EnableBPForBPSet(bp_set, group);
                        // Set the disposition for this group
                        if (once) {
                            SetBPDispositionForBPSet(bp_set, group,
                                                     DBGBP_DISABLE_DISP);
                        } else if (del) {
                            SetBPDispositionForBPSet(bp_set, group,
                                                     DBGBP_DELETE_DISP);
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

                    bitset_free(group);
                    FreeRecSet(new_rec_set);
                }
            }
        } else {
            // find the bp_set for the bp_set_num
            if ((bp_set = FindBPSetByID(di, bp_set_num)) == nullptr) {
                // Error case: the provided bp_set_num does not exist
                dbgevent_t *e;

                // Create a new error event
                EVENT_ERROR(e, DBGERR_DEBUGGER, "No breakpoint number %d.\n",
                            bp_set_num);
                // Add the event to the result based on its type
                auto group_rep =
                    NewGroupReply(di, bitset_dup(recv_ptr->rec_group), e);
                // Create a new result and return it
                if (result == nullptr) {
                    result = NewRepSet();
                }
                AddErrToSet(result, group_rep);
                continue;
            }

            // Need to loop over the b_bp_id_list to operate on the correct
            // breakpoint number in the backend
            SetList(bp_set->b_bp_id_list);
            while ((id_set = (bp_id_set_t *)GetListElement(
                        bp_set->b_bp_id_list)) != nullptr) {
                // Get the new group, based on the intersection between the
                // bp_set and the rec_set.
                auto group = bitset_and(id_set->b_group, recv_ptr->rec_group);

                // Ensure that there are ranks in the bitset, otherwise continue
                if (bitset_isempty(group)) {
                    bitset_free(group);
                    continue;
                }

                // Create the request for this group
                auto request = CommandToSocProto(DBGCMD_ENABLEBREAK, id_set->b_bp_id,
                                            once, del);

                // Create the new rec set for this group
                auto new_rec_set = NewRecSet();
                AddReceiverToSet(new_rec_set, NewGroupReceiver(di, group));

                auto tmp_result =
                    DbgSendCmdAndRecvReply(new_rec_set, request, DBGEV_OK);

                // We expect this to return nullptr on success, handle the case
                // were things worked
                if (tmp_result == nullptr) {
                    // mark the breakpoint as enabled for this group
                    EnableBPForBPSet(bp_set, group);
                    // Set the disposition for this group
                    if (once) {
                        SetBPDispositionForBPSet(bp_set, group,
                                                 DBGBP_DISABLE_DISP);
                    } else if (del) {
                        SetBPDispositionForBPSet(bp_set, group,
                                                 DBGBP_DELETE_DISP);
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

                bitset_free(group);
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
                    
                        auto loc = fp->getLoc();

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
                        if (AddListStateLocation(dbg_fe, group_rep->rep_group, fp->getThreads(),
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
                                                          bitset_dup(group_rep->rep_group), e);
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
                                                       bitset_dup(set_loc->set), e);
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
                                                  bitset_dup(set_loc->set), e);
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
                                                  bitset_dup(set_loc->set), e);
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
                                                   bitset_dup(set_loc->set), e);

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
                // based on the bitset in the group_recv_t. If the user calls
                // the list cmd with a string, then all ranks are going to have
                // the same answer.

                // Make sure we clear the state if there was any saved
                ClearListState(dbg_fe);

                // Save this new list location state for later calls to list
                if (AddListStateLocation(dbg_fe, recv_ptr->rec_group, nullptr, str) <
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
                                          bitset_dup(recv_ptr->rec_group), e);

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
                                           bitset_dup(recv_ptr->rec_group), e);

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
                    auto loc = fp->getLoc();
                        
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
                    if (AddListStateLocation(dbg_fe, group_rep->rep_group, fp->getThreads(),
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
                                      bitset_dup(recv_ptr->rec_group), e);

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

                if (DBG_PROTO_SW(di)->sw_exit == nullptr) {
                    // Create a new error event
                    auto e = NewErrorEvent(DBGERR_NOTIMP, nullptr);
                    auto err_rep =
                        NewGroupReply(di, bitset_dup(recv_ptr->rec_group), e);
                    // Add the error to the reply set
                    AddErrToSet(result, err_rep);
                } else {
                    // FIXME: How to handle error case?
                    DBG_PROTO_SW(di)->sw_exit(di, detach);
                }

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
                if (DBG_PROTO_SW(di)->sw_exit == nullptr) {
                    // Create a new error event
                    auto e = NewErrorEvent(DBGERR_NOTIMP, nullptr);
                    auto err_rep =
                        NewGroupReply(di, bitset_dup(recv_ptr->rec_group), e);
                    // Add the error to the reply set
                    AddErrToSet(result, err_rep);
                } else {
                    // FIXME: How to handle error case?
                    DBG_PROTO_SW(di)->sw_exit(di, detach);
                }

                // cleanup the di object
                FreeDbgInvoke(di);
            }

            break;
    }

    return result;
}
