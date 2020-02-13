/*********************************************************************************\
 * Debugger interface routines
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * Copyright 2010-2020 Cray Inc.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Cray Inc.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *
 *********************************************************************************/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

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
static void DbgFreeSignal(int *);
static void FreeGroupReceiver(group_recv_t *);
static void ClearCmdSent(group_recv_t *);
static group_recv_t *CopyGroupReceiver(group_recv_t *);
static int GetNumberOfReceivers(rec_set_t *);
static rep_set_t *MergeRepSet(rep_set_t *, rep_set_t *);
static void AddErrToSet(rep_set_t *, group_reply_t *);
static void SetErrHeadInSet(rep_set_t *);
static group_reply_t *GetNextErrInSet(rep_set_t *);
static event_set_t *NewEventSet(dbginvoke_t *, bitset *, dbgevent_t *);
static void FreeEventSet(event_set_t *);
static void DestroyEventSet(dbginvoke_t *, dbgev);
static event_set_t *AddEvent(dbginvoke_t *, bitset *, dbgevent_t *);
static void CheckForFullEventSet(dbginvoke_t *, bitset *, dbgev);
static void ResetForAsyncCmd(rec_set_t *);
static void RemoveGroupFromAsyncSet(dbginvoke_t *, bitset *);
static bp_set_t *NewBPSet(bitset *, dbgbp_t *);
static bp_disp_set_t *NewBPDispSet(bitset *, dbgbp_disp);
static bp_id_set_t *NewBPIDSet(bitset *, int);
static void FreeBPSet(bp_set_t *);
static void FreeBPIDSet(bp_id_set_t *);
static void FreeBPDispSet(bp_disp_set_t *);
static void AddBP(rep_set_t *);
static void AddBPToBPSet(bp_set_t *, bitset *, dbgbp_t *);
static void EnableBPForBPSet(bp_set_t *, bitset *);
static void DisableBPForBPSet(bp_set_t *, bitset *);
static void SetBPDispositionForBPSet(bp_set_t *, bitset *, dbgbp_disp);
static void RemoveBPSet(dbginvoke_t *, bp_set_t *, bitset *);
static int CompareBPSetWithBP(bp_set_t *, dbgbp_t *);

/*
** Internal Variables
*/
static proto_session *DbgCurrSess;
static list_t *DbgInvokeList;
static list_t *DbgSigList;
static list_t *DbgUnblockableSigList;
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
    int *sig;
    sigset_t mask;

    // initialize values
    sigemptyset(&mask);

    if (!EmptyList(DbgSigList)) {
        SetList(DbgSigList);
        while ((sig = (int *)GetListElement(DbgSigList)) != (int *)NULL)
            sigaddset(&mask, *sig);

        sigprocmask(SIG_BLOCK, &mask, (sigset_t *)NULL);
    }
}

/*
** Release any pending signals.
*/
void ReleaseSignals(void) {
    int *sig;
    sigset_t mask;

    // initialize data
    sigemptyset(&mask);

    if (!EmptyList(DbgSigList)) {
        SetList(DbgSigList);
        while ((sig = (int *)GetListElement(DbgSigList)) != (int *)NULL)
            sigaddset(&mask, *sig);

        sigprocmask(SIG_UNBLOCK, &mask, (sigset_t *)NULL);
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
    di->d_handle = (handle != NULL) ? strdup(handle) : NULL;
    di->d_type = type;
    di->d_nproc = nproc;
    di->d_proto = proto;
    di->d_eventset_list = NewList();
    di->d_bpset_list = NewList();

    if (DbgInvokeList == NULL) DbgInvokeList = NewList();

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
    if (DBG_PROTO_SW(di)->sw_destroyclnt != NULL) {
        DBG_PROTO_SW(di)->sw_destroyclnt(di);
    }

    // Quit the debug frontend if there is one
    if (di->d_frontend != NULL) {
        e = DbgDestroyFE(di->d_frontend);
        FreeEvent(e);
    }

    if (di->d_bitset_full != NULL) bitset_free(di->d_bitset_full);

    if (di->d_bitset_launched != NULL) bitset_free(di->d_bitset_launched);

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
    rec->_cmd_sent = NULL;

    return rec;
}

static void FreeGroupReceiver(group_recv_t *rec) {
    bitset_free(rec->rec_group);
    if (rec->_cmd_sent != NULL) {
        bitset_free(rec->rec_group);
    }
    Free(rec);
}

static void ClearCmdSent(group_recv_t *rec) {
    if (rec->_cmd_sent != NULL) {
        bitset_free(rec->_cmd_sent);
        rec->_cmd_sent = NULL;
    }
}

static group_recv_t *CopyGroupReceiver(group_recv_t *rec) {
    group_recv_t *newrec;

    if (rec == NULL) return NULL;

    newrec = (group_recv_t *)Alloc(sizeof(group_recv_t));

    newrec->rec_di = rec->rec_di;
    newrec->rec_group = bitset_dup(rec->rec_group);
    newrec->_cmd_sent = NULL;

    return newrec;
}

/*
 * data structures for sets of group reveivers to dbgsrv
 */

rec_set_t *NewRecSet(void) {
    rec_set_t *set;

    set = (rec_set_t *)Alloc(sizeof(rec_set_t));
    set->rec_list = NewList();

    return set;
}

void FreeRecSet(rec_set_t *set) {
    if (set == NULL) return;

    DestroyList(set->rec_list, (void (*)(void *))FreeGroupReceiver);

    Free(set);
}

void AddReceiverToSet(rec_set_t *set, group_recv_t *rec) {
    group_recv_t *r_ptr;

    if (rec == NULL || set == NULL) return;

    // Here we want to enforce merging since this is done *before* any command
    // is sent. We want to send all commands at the same time to each di tree.
    SetList(set->rec_list);
    while ((r_ptr = (group_recv_t *)GetListElement(set->rec_list)) != NULL) {
        // If the di objects match, they are they point to the same thing.
        if (r_ptr->rec_di == rec->rec_di) {
            // The provided di matches, we need to merge
            bitset_oreq(r_ptr->rec_group, rec->rec_group);
            FreeGroupReceiver(rec);
            return;
        }
    }

    // This is a unique di, add it to the list.
    AddToList(set->rec_list, (void *)rec);
}

void SetReceiverHeadInSet(rec_set_t *set) {
    if (set != NULL) SetList(set->rec_list);
}

group_recv_t *GetNextReceiverInSet(rec_set_t *set) {
    if (set != NULL)
        return (group_recv_t *)GetListElement(set->rec_list);
    else
        return NULL;
}

// This returns the number of dbginvoke_t in the rec_set
static int GetNumberOfReceivers(rec_set_t *set) {
    if (set == NULL) return 0;

    return SizeOfList(set->rec_list);
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

    if (rep == NULL) return NULL;

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
    rep_set_t *set = (rep_set_t *)Alloc(sizeof(rep_set_t));

    set->rep_list = NewList();
    set->err_list = NewList();

    return set;
}

void FreeRepSet(rep_set_t *set) {
    if (set == NULL) return;

    DestroyList(set->rep_list, (void (*)(void *))FreeGroupReply);

    DestroyList(set->err_list, (void (*)(void *))FreeGroupReply);

    Free(set);
}

rep_set_t *CopyRepSet(rep_set_t *set) {
    rep_set_t *newset;
    group_reply_t *elem;

    if (set == NULL) return NULL;

    newset = NewRepSet();

    // copy the replies
    SetList(set->rep_list);
    while ((elem = (group_reply_t *)GetListElement(set->rep_list)) != NULL) {
        AddReplyToSet(newset, CopyGroupReply(elem));
    }

    // copy the errors
    SetList(set->err_list);
    while ((elem = (group_reply_t *)GetListElement(set->err_list)) != NULL) {
        AddErrToSet(newset, CopyGroupReply(elem));
    }

    return newset;
}

static rep_set_t *MergeRepSet(rep_set_t *set, rep_set_t *tmp_set) {
    group_reply_t *elem;

    if (set == NULL || tmp_set == NULL) return NULL;

    // merge the replies
    while ((elem = (group_reply_t *)RemoveFirst(tmp_set->rep_list)) != NULL) {
        AddReplyToSet(set, elem);
    }

    // Merge the errors
    while ((elem = (group_reply_t *)RemoveFirst(tmp_set->err_list)) != NULL) {
        AddErrToSet(set, elem);
    }

    FreeRepSet(tmp_set);

    return set;
}

int SetHasError(rep_set_t *set) {
    if (set == (rep_set_t *)NULL) return 0;

    if (set->err_list == (list_t *)NULL) return 0;

    return SizeOfList(set->err_list);
}

void AddReplyToSet(rep_set_t *set, group_reply_t *rep) {
    if (rep == NULL || set == NULL) return;

    AddToList(set->rep_list, (void *)rep);
}

static void AddErrToSet(rep_set_t *set, group_reply_t *err_rep) {
    if (err_rep == NULL || set == NULL) return;

    AddToList(set->err_list, (void *)err_rep);
}

void ResetSet(rep_set_t *set) {
    if (set == NULL) return;

    SetList(set->rep_list);
    SetList(set->err_list);
}

static void SetErrHeadInSet(rep_set_t *set) {
    if (set != NULL) SetList(set->err_list);
}

static group_reply_t *GetNextErrInSet(rep_set_t *set) {
    if (set != NULL)
        return (group_reply_t *)GetListElement(set->err_list);
    else
        return NULL;
}

void SetReplyHeadInSet(rep_set_t *set) {
    if (set != NULL) SetList(set->rep_list);
}

group_reply_t *GetNextReplyInSet(rep_set_t *set) {
    if (set != NULL)
        return (group_reply_t *)GetListElement(set->rep_list);
    else
        return NULL;
}

int GetNumberOfReply(rep_set_t *set) {
    if (set == NULL) return 0;

    return SizeOfList(set->rep_list);
}

// Used to print out an error from a rep_set
int ProcessErrorInRepSet(rep_set_t *rep_set) {
    group_reply_t *err;

    if (rep_set == NULL) return 0;

    if (SetHasError(rep_set)) {
        SetErrHeadInSet(rep_set);
        while ((err = GetNextErrInSet(rep_set)) != NULL) {
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
    if (eventset == NULL) return;

    if (eventset->e_group != NULL) bitset_free(eventset->e_group);

    if (eventset->e_ev != NULL) FreeEvent(eventset->e_ev);

    Free(eventset);
}

static void DestroyEventSet(dbginvoke_t *di, dbgev ev) {
    event_set_t *eventset;

    if (di == NULL) return;

    SetList(di->d_eventset_list);
    while ((eventset = (event_set_t *)GetListElement(di->d_eventset_list)) !=
           NULL) {
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

    if (di == NULL || group == NULL || event == NULL) return NULL;

    SetList(di->d_eventset_list);
    while ((eventset = (event_set_t *)GetListElement(di->d_eventset_list)) !=
           NULL) {
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
    if (di == NULL) return;

    // dup the fullset and then clear it
    aggset = bitset_dup(fullset);
    bitset_clear(aggset);

    // Create a set that contains all the ev events by looping over
    // every event
    SetList(di->d_eventset_list);
    while ((eventset = (event_set_t *)GetListElement(di->d_eventset_list)) !=
           NULL) {
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
    group_recv_t *recv_ptr;
    event_set_t *eventset;

    if (set == NULL) return;

    // Remove the old async_set if there is one
    if (async_set != NULL) FreeRecSet(async_set);

    async_set = NewRecSet();

    // Remove any old events for this group, and add each one to the async_set
    SetReceiverHeadInSet(set);
    while ((recv_ptr = GetNextReceiverInSet(set)) != NULL) {
        // Clear any pending events for the group we are sending an async
        // command to
        SetList(recv_ptr->rec_di->d_eventset_list);
        while ((eventset = (event_set_t *)GetListElement(
                    recv_ptr->rec_di->d_eventset_list)) != NULL) {
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
    group_recv_t *recv_ptr;

    if (di == NULL || group == NULL || async_set == NULL) return;

    // Find entry for this di in the async_set
    SetReceiverHeadInSet(async_set);
    while ((recv_ptr = GetNextReceiverInSet(async_set)) != NULL) {
        if (recv_ptr->rec_di == di) {
            // Remove the group from the bitset
            bitset_andeqnot(recv_ptr->rec_group, group);

            // Check to see if the rec_group is now empty
            if (bitset_isempty(recv_ptr->rec_group)) {
                RemoveFromList(async_set->rec_list, (void *)recv_ptr);
                FreeGroupReceiver(recv_ptr);
            }

            // We are done, there is only one group per di in the async_set
            break;
        }
    }

    // Check to see if we have an empty async_set, if so the async events are
    // no longer pending
    if (SizeOfList(async_set->rec_list) == 0) {
        // The async event is no longer pending
        async_pending = 0;
        // cleanup the async_set
        FreeRecSet(async_set);
        async_set = NULL;
    }
}

/*************************************************
** Internal dbgclient breakpoint handling routines
**************************************************/

/*
** Create a new breakpoint set
*/
static bp_set_t *NewBPSet(bitset *group, dbgbp_t *bp) {
    bp_id_set_t *id_set;
    bp_disp_set_t *disp_set;

    bp_set_t *bp_set = (bp_set_t *)Alloc(sizeof(bp_set_t));
    memset(bp_set, 0, sizeof(bp_set_t));

    bp_set->b_group = bitset_dup(group);

    // Add the group to the enabled set if the bp is enabled
    if (bp->bp_enabled) {
        bp_set->b_bp_enabled = bitset_dup(group);
    }

    // Add the group and disposition to the disp list
    bp_set->b_bp_disp = NewList();
    disp_set = NewBPDispSet(group, bp->bp_disp);
    AddToList(bp_set->b_bp_disp, (void *)disp_set);

    // Set the type, this cannot vary by rank
    bp_set->b_bp_type = bp->bp_type;

    // Set the location
    if (bp->bp_loc.loc_addr != NULL)
        bp_set->b_loc_addr = strdup(bp->bp_loc.loc_addr);

    bp_set->b_loc_line = bp->bp_loc.loc_line;

    if (bp_set->b_loc_line >= 0) {
        bp_set->b_loc_file = strdup(bp->bp_loc.loc_file);
        bp_set->b_loc_func = strdup(bp->bp_loc.loc_func);
    } else {
        bp_set->b_loc_file = NULL;
        bp_set->b_loc_func = NULL;
    }

    // Set the expression for watchpoints
    bp_set->b_bp_exp = strdup(bp->bp_exp);

    // Set the id unique to the rank
    bp_set->b_bp_id_list = NewList();
    id_set = NewBPIDSet(group, bp->bp_id);
    AddToList(bp_set->b_bp_id_list, id_set);

    return bp_set;
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
    if (bp_set == NULL) return;

    if (bp_set->b_group != NULL) bitset_free(bp_set->b_group);

    if (bp_set->b_bp_id_list != NULL)
        DestroyList(bp_set->b_bp_id_list, (void (*)(void *))FreeBPIDSet);

    if (bp_set->b_bp_enabled != NULL) bitset_free(bp_set->b_bp_enabled);

    if (bp_set->b_bp_disp != NULL)
        DestroyList(bp_set->b_bp_disp, (void (*)(void *))FreeBPDispSet);

    if (bp_set->b_loc_addr != NULL) Free(bp_set->b_loc_addr);

    if (bp_set->b_loc_file != NULL) Free(bp_set->b_loc_file);

    if (bp_set->b_loc_func != NULL) Free(bp_set->b_loc_func);

    if (bp_set->b_bp_exp != NULL) Free(bp_set->b_bp_exp);

    Free(bp_set);
}

/*
** Free a breakpoint id (gdb mapping) set
*/
static void FreeBPIDSet(bp_id_set_t *id_set) {
    if (id_set == NULL) return;

    bitset_free(id_set->b_group);
    id_set->b_group = NULL;
    id_set->b_bp_id = -1;

    Free(id_set);
}

/*
** Free a breakpoint disposition set
*/
static void FreeBPDispSet(bp_disp_set_t *disp_set) {
    if (disp_set == NULL) return;

    bitset_free(disp_set->b_group);

    Free(disp_set);
}

/*
** Process a breakpoint reply set - update internal breakpoint information
** for the dbginvoke_t
*/
static void AddBP(rep_set_t *res) {
    group_reply_t *reply;
    dbgbp_t *bp;
    bp_set_t *bpset;
    dbginvoke_t *di;

    ResetSet(res);

    while ((reply = GetNextReplyInSet(res)) != NULL) {
        bp = EVENT_BP(reply->rep_event);

        di = (dbginvoke_t *)(reply->rep_di);

        SetList(di->d_bpset_list);
        while ((bpset = (bp_set_t *)GetListElement(di->d_bpset_list)) != NULL) {
            if (!CompareBPSetWithBP(bpset, bp)) {
                break;
            }
        }

        if (bpset != NULL) /* This is a known breakpoint */
        {
            // update the bpset with this group and info from the bp for disp
            // and id
            AddBPToBPSet(bpset, reply->rep_group, bp);
        } else {
            // create a new breakpoint for this group based on the bp
            bpset = NewBPSet(reply->rep_group, bp);
            AddToList(di->d_bpset_list, bpset);
        }
    }
}

static void AddBPToBPSet(bp_set_t *bp_set, bitset *group, dbgbp_t *bp) {
    bp_id_set_t *id_set = NULL;
    bitset_oreq(bp_set->b_group, group);

    SetList(bp_set->b_bp_id_list);
    while ((id_set = (bp_id_set_t *)GetListElement(bp_set->b_bp_id_list)) !=
           NULL) {
        if (id_set->b_bp_id == bp->bp_id) break;
    }

    if (id_set == NULL) {
        id_set = NewBPIDSet(group, bp->bp_id);
        AddToList(bp_set->b_bp_id_list, id_set);
    } else {
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
    if (bp_set == NULL || group == NULL) return;

    bitset_oreq(bp_set->b_bp_enabled, group);
}

static void DisableBPForBPSet(bp_set_t *bp_set, bitset *group) {
    if (bp_set == NULL || group == NULL) return;

    bitset_andeqnot(bp_set->b_bp_enabled, group);
}

static void SetBPDispositionForBPSet(bp_set_t *bp_set, bitset *group,
                                     dbgbp_disp bp_disp) {
    list_t *bpdisp_list;
    bp_disp_set_t *disp_set;
    int found = 0;

    if (bp_set == NULL || group == NULL) return;

    bpdisp_list = bp_set->b_bp_disp;

    SetList(bpdisp_list);
    while ((disp_set = (bp_disp_set_t *)GetListElement(bpdisp_list)) != NULL) {
        if (disp_set->b_bp_disp == bp_disp) {
            // This matches the requested disposition
            ++found;
            bitset_oreq(disp_set->b_group, group);
        } else {
            // This doesn't match the requested disposition, so remove the group
            // We do this in case there is any overlap from a previous set
            // disposition command on a set.
            bitset_andeqnot(disp_set->b_group, group);

            // Make sure the disp_set isn't empty, otherwise remove it
            if (bitset_isempty(disp_set->b_group)) {
                RemoveFromList(bpdisp_list, (void *)disp_set);
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
    bp_id_set_t *id_set;
    bp_disp_set_t *disp_set;

    if (di == NULL || bp_set == NULL) return;

    // Remove the group from the global bp list
    bitset_andeqnot(bp_set->b_group, group);

    // Remove the group from the bp_id list
    SetList(bp_set->b_bp_id_list);
    while ((id_set = (bp_id_set_t *)GetListElement(bp_set->b_bp_id_list)) !=
           NULL) {
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
    SetList(bp_set->b_bp_disp);
    while ((disp_set = (bp_disp_set_t *)GetListElement(bp_set->b_bp_disp)) !=
           NULL) {
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
            // check if both have the same file name and line number
            if (bp->bp_loc.loc_file != NULL && bp->bp_loc.loc_line >= 0) {
                // First check if the file name matches.
                if (strcmp(set->b_loc_file, bp->bp_loc.loc_file)) return 1;
                // The file name matches, so we can now safely check the line
                // number.
                if (set->b_loc_line != bp->bp_loc.loc_line) return 1;
            }

            // check if both have the same address
            if (bp->bp_loc.loc_addr != NULL) {
                if (strcmp(set->b_loc_addr, bp->bp_loc.loc_addr)) return 1;
            }

            break;

        case DBGBP_WATCH_TYPE:
        case DBGBP_AWATCH_TYPE:
        case DBGBP_RWATCH_TYPE:
            // check if both have the same bp expression
            if (bp->bp_exp != NULL) {
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

bp_set_t *FindBPSetByID(dbginvoke_t *di, int id) {
    bp_set_t *bpset;
    int bpset_id = 0;

    if (id < 1 || di == NULL) return NULL;

    SetList(di->d_bpset_list);
    while ((bpset = (bp_set_t *)GetListElement(di->d_bpset_list)) != NULL) {
        bpset_id++;

        if (bpset_id == id) {
            break;
        }
    }

    return bpset;
}

bp_set_t *FindBPSetForBP(dbginvoke_t *di, dbgbp_t *bp) {
    bp_set_t *bpset;
    int bpset_id = 0;

    if (di == NULL) return NULL;

    SetList(di->d_bpset_list);
    while ((bpset = (bp_set_t *)GetListElement(di->d_bpset_list)) != NULL) {
        bpset_id++;

        if (!CompareBPSetWithBP(bpset, bp)) {
            break;
        }
    }

    if (bpset == NULL) return NULL;

    // Always update the b_bp_id
    bpset->b_bp_id = bpset_id;

    return bpset;
}

void ResetBPSetList(dbginvoke_t *di) {
    if (di == NULL) return;

    SetList(di->d_bpset_list);
    di->d_global_bpset_id = 0;
}

bp_set_t *GetNextBPSet(dbginvoke_t *di) {
    bp_set_t *bp_set;

    if (di == NULL) return NULL;

    // Get the next element in the list
    bp_set = (bp_set_t *)GetListElement(di->d_bpset_list);

    // update the b_bp_id if there was another element
    if (bp_set != NULL) {
        bp_set->b_bp_id = ++(di->d_global_bpset_id);
    }

    return bp_set;
}

int EmptyBPSetList(dbginvoke_t *di) {
    if (di == NULL) return 1;

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
        while ((di = (dbginvoke_t *)GetListElement(DbgInvokeList)) !=
               (dbginvoke_t *)NULL) {
            /*
            ** Note: This should be used for protocols that do not use an event
            *notification
            **       fd. If a event notification fd can be used, then it should
            *be added to
            **       the sw_evfd list and sw_checkevents should simply return 0.
            */
            if (DBG_PROTO_SW(di)->sw_checkevents !=
                    NULL  // if the protocol has a checkevent function defined
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
            if (di->d_evfds != NULL) {
                SetList(di->d_evfds);

                while ((ev_fd = (int *)GetListElement(di->d_evfds)) !=
                       (int *)NULL) {
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
        status = select(high_fd + 1, &dispatch_fds, NULL, NULL, &tv);

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
        while ((di = (dbginvoke_t *)GetListElement(DbgInvokeList)) !=
               (dbginvoke_t *)NULL) {
            // if the proto is shutting down, don't dispatch further pending
            // events
            if (di->d_isShutdown) continue;

            // Ensure there is a protocol specific dispatch function
            if (DBG_PROTO_SW(di)->sw_dispatchevents != NULL) {
                // If there is an d_evfds list, then the protol layer is using
                // event notification fd's. We should try to check to see if
                // each one is set, and if so call the protocol specific
                // dispatch function with the event fd as an argument
                if (di->d_evfds != NULL) {
                    SetList(di->d_evfds);

                    while ((ev_fd = (int *)GetListElement(di->d_evfds)) !=
                           (int *)NULL) {
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
        (InputCB.i_rtn != NULL)) {
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
    event_set_t *eventset = NULL;
    dbgevent_t *event;
    bp_set_t *bp_set;
    dbgbp_t *bp;
    bitset *group;
    bp_disp_set_t *disp_set;

    if (set == NULL) return 0;

    ResetProtoMsgSet(set);
    while ((msg = NextProtoMsgInSet(set)) != NULL) {
        // convert this protocol msg string to an event
        event = SocProtoToEvent(msg->message);

        if (event == NULL) {
            continue;
        }

        // switch on the event to check if the event changed the status or if
        // we need to do any special pre-processing
        switch (EVENT_EV(event)) {
            // These events coorespond to possible status changes
            case DBGEV_BPHIT:
                // Handle special pre-processing for breakpoints
                bp = EVENT_BP(event);
                if ((bp_set = FindBPSetForBP(di, bp)) != NULL) {
                    // if the breakpoint was a watchpoint we need to update the
                    // location information
                    // FIXME: This could vary by rank and the current
                    // implementation is broken.
                    switch (bp_set->b_bp_type) {
                        case DBGBP_WATCH_TYPE:
                        case DBGBP_AWATCH_TYPE:
                        case DBGBP_RWATCH_TYPE:
                            if (bp->bp_loc.loc_file != NULL) {
                                // Free the old value and set it to the new
                                // value
                                if (bp_set->b_loc_file != NULL)
                                    Free(bp_set->b_loc_file);
                                bp_set->b_loc_file =
                                    strdup(bp->bp_loc.loc_file);
                            }
                            if (bp->bp_loc.loc_func != NULL) {
                                // Free the old value and set it to the new
                                // value
                                if (bp_set->b_loc_func != NULL)
                                    Free(bp_set->b_loc_func);
                                bp_set->b_loc_func =
                                    strdup(bp->bp_loc.loc_func);
                            }
                            bp_set->b_loc_line = bp->bp_loc.loc_line;
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
                if ((bp_set = FindBPSetForBP(di, bp)) != NULL) {
                    // If the breakpoint was set to delete, we need to delete
                    // it. If it was set to dis, we need to disable it.
                    SetList(bp_set->b_bp_disp);
                    while ((disp_set = (bp_disp_set_t *)GetListElement(
                                bp_set->b_bp_disp)) != NULL) {
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
                if ((bp_set = FindBPSetForBP(di, bp)) != NULL) {
                    // Need to delete the breakpoint.
                    SetList(bp_set->b_bp_disp);
                    while ((disp_set = (bp_disp_set_t *)GetListElement(
                                bp_set->b_bp_disp)) != NULL) {
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
    if (di == NULL || p_o_stream == NULL) return;

    // Call the output callback
    OutputCB.o_rtn(di, p_o_stream);
}

/*
** Set client event handler
*/
void DbgHandleEvent(dbginvoke_t *di, clnt_ev_callback_func ev_callback) {
    if (di == NULL) return;

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
    sigprocmask(SIG_UNBLOCK, &mask, (sigset_t *)NULL);
}

/*
** Set signal handler.
*/
void DbgHandleSignal(int sig, void (*hand)(int), dbgsig_type mask_type) {
    int *l_sig;
    struct sigaction handler;

    if (hand == SIG_DFL || hand == SIG_IGN) {
        // ensure the signal is not in the normal or unblockable list
        SetList(DbgSigList);
        while ((l_sig = (int *)GetListElement(DbgSigList)) != NULL) {
            if (*l_sig == sig) {
                RemoveFromList(DbgSigList, (void *)l_sig);
                Free(l_sig);
                break;
            }
        }
        SetList(DbgUnblockableSigList);
        while ((l_sig = (int *)GetListElement(DbgUnblockableSigList)) != NULL) {
            if (*l_sig == sig) {
                RemoveFromList(DbgUnblockableSigList, (void *)l_sig);
                Free(l_sig);
                break;
            }
        }
    } else {
        dbgsig_type cur_type;
        // Search the list, if the signal isn't in the list, then add it.
        SetList(DbgSigList);
        while ((l_sig = (int *)GetListElement(DbgSigList)) != NULL) {
            if (*l_sig == sig) {
                cur_type = DBG_SIG_MASKABLE;
                break;
            }
        }
        SetList(DbgUnblockableSigList);
        while ((l_sig = (int *)GetListElement(DbgUnblockableSigList)) != NULL) {
            if (*l_sig == sig) {
                cur_type = DBG_SIG_NOMASK;
                break;
            }
        }

        // if the signal is already present, make sure it is in the proper list
        if (l_sig != NULL) {
            if (mask_type == DBG_SIG_NOMASK && cur_type == DBG_SIG_MASKABLE) {
                // signal is in wrong list
                RemoveFromList(DbgSigList, (void *)l_sig);
                AddToList(DbgUnblockableSigList, (void *)l_sig);
            } else if (mask_type == DBG_SIG_MASKABLE &&
                       cur_type == DBG_SIG_NOMASK) {
                // signal is in wrong list
                RemoveFromList(DbgUnblockableSigList, (void *)l_sig);
                AddToList(DbgSigList, (void *)l_sig);
            }
        } else {
            // signal isn't present, so create it and add to the proper list
            l_sig = (int *)Alloc(sizeof(int));
            memcpy(l_sig, &sig, sizeof(int));
            if (mask_type == DBG_SIG_NOMASK) {
                AddToList(DbgUnblockableSigList, (void *)l_sig);
            } else {
                AddToList(DbgSigList, (void *)l_sig);
            }
        }
    }

    // make sure to initialize the handler
    memset(&handler, 0, sizeof(handler));
    sigemptyset(&handler.sa_mask);

    // set the handler function
    handler.sa_handler = hand;

    sigaction(sig, &handler, NULL);
}

/*
** Free signal handler.
*/
static void DbgFreeSignal(int *l_sig) {
    if (l_sig != NULL) Free(l_sig);
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

    while ((sw = GetClntProto()) != (dbgclnt_proto *)NULL)
        if (sw->sw_init != NULL && !sw->sw_init()) return -1;

    if (DbgSigList == NULL) DbgSigList = NewList();

    if (DbgUnblockableSigList == NULL) DbgUnblockableSigList = NewList();

    // Initialize the mutex lock for the InputCB
    InputCB.i_fd = -1;
    if (pthread_mutexattr_init(&InputCB.i_lock_attr)) return -1;
    if (pthread_mutexattr_settype(&InputCB.i_lock_attr,
                                  PTHREAD_MUTEX_RECURSIVE))
        return -1;
    if (pthread_mutex_init(&InputCB.i_lock, &InputCB.i_lock_attr)) return -1;

    return 0;
}

void DbgFinish(void) {
    dbgclnt_proto *sw;
    dbginvoke_t *di;

    SetClntProto();

    // Cleanup the current session if there is one
    if (DbgCurrSess != NULL) {
        sw = DbgCurrSess->sess_proto;
        if (sw->sw_cleanup_session == NULL) {
            PrintOutError(
                "Protocol function sw_cleanup_session not implemented.\n");
        } else {
            sw->sw_cleanup_session(DbgCurrSess);
        }
        DbgCurrSess = NULL;
    }

    while ((sw = GetClntProto()) != (dbgclnt_proto *)NULL) {
        if (sw->sw_finish != NULL) {
            sw->sw_finish();
        }
    }

    DestroyList(DbgSigList, (void (*)(void *))DbgFreeSignal);
    DbgSigList = NULL;

    DestroyList(DbgUnblockableSigList, (void (*)(void *))DbgFreeSignal);
    DbgUnblockableSigList = NULL;

    // Manually free the elements in the dbginvoke list
    while ((di = (dbginvoke_t *)RemoveFirst(DbgInvokeList)) != NULL) {
        FreeDbgInvoke(di);
    }
    DestroyList(DbgInvokeList, NULL);
    DbgInvokeList = NULL;

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

    if ((ptab = FindClntProto(proto)) == NULL) {
        PrintOutError("No such protocol \"%s\"\n", proto);
        return NULL;
    }

    if (ptab->sw_create_session == NULL) {
        PrintOutError("Protocol function sw_create_session not implemented.\n");
        return NULL;
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
    if (DbgCurrSess != NULL) {
        ptab = DbgCurrSess->sess_proto;
        if (ptab->sw_cleanup_session == NULL) {
            PrintOutError(
                "Protocol function sw_cleanup_session not implemented.\n");
        } else {
            ptab->sw_cleanup_session(DbgCurrSess);
        }
    }

    // Ask the protocol layer to keep the new session around if its not null
    if (sess != NULL) {
        ptab = sess->sess_proto;
        if (ptab->sw_save_session == NULL) {
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

    if ((ptab = FindClntProto(proto)) == (dbgclnt_proto *)NULL) {
        PrintOutError("No such protocol \"%s\"\n", proto);
        return NULL;
    }

    if (ptab->sw_attach == NULL) {
        PrintOutError("Protocol function sw_attach not implemented.\n");
        return NULL;
    }

    // Create the dbginvoke_t object to pass down, nproc is unknown since this
    // is the attach case
    di = NewDbgInvoke(handle, DBG_ATTACHED, 0, ptab);

    if (ptab->sw_attach(di, sess, app_ident, arch, backend, gpu_flag,
                        debug_flag)) {
        FreeDbgInvoke(di);
        return NULL;
    }

    // The frontend debug presence can fail silently
    // This is currently disabled for the attach case since I have no idea how
    // to get the actual fullpath name of the a.out.
    // di->d_frontend = DbgInstantiateFE(backend);
    di->d_frontend = NULL;

    // FIXME: How to set the debug frontend a.out???

    // This command changes the status
    inferior_status_change = 1;

    return di;
}

/*
** Invoke user program.
*/
dbginvoke_t *DbgInvoke(const char *proto, proto_session *sess, const char *prog,
                       char **argv, const char *handle, int nproc,
                       const char *arch, char **arch_args,
                       const char *arch_input, const char *backend, char **env,
                       const char *cwd, int gpu_flag, int debug_flag,
                       int unsafe_flag, int early_entry_flag) {
    dbginvoke_t *di;
    dbgclnt_proto *ptab;
    char **ap;
    char **ptr;
    int ap_argc = 0;
    dbgevent_t *e;

    if ((ptab = FindClntProto(proto)) == (dbgclnt_proto *)NULL) {
        PrintOutError("No such protocol \"%s\"\n", proto);
        return NULL;
    }

    if (ptab->sw_invoke == NULL) {
        PrintOutError("Protocol function sw_invoke not implemented.\n");
        return NULL;
    }

    // Create the dbginvoke_t object to pass down
    di = NewDbgInvoke(handle, DBG_LAUNCHED, nproc, ptab);

    // Start out by creating the frontend gdb instance. We want to do some pre
    // checks on the a.out

    // The frontend debug presence can fail silently
    di->d_frontend = DbgInstantiateFE(backend);

    // Set the debug frontend a.out directly.
    if (di->d_frontend != NULL) {
        e = DbgFESetFile(di->d_frontend, prog);

        // handle the error case
        if (e->ev_event == DBGEV_ERROR) {
            // We want to tear down the frontend since it has no way of reading
            // symbols since loading the binary failed.
            FreeEvent(e);
            e = DbgDestroyFE(di->d_frontend);
            di->d_frontend = NULL;
        }

        FreeEvent(e);
    }

    // Check for startup function presence if unsafe is false
    if (!unsafe_flag) {
        if (di->d_frontend != NULL) {
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

                return NULL;
            }

            FreeEvent(e);
        }
    }

    // Create the cmd str and set it
    ap = NewArgv(prog, NULL);
    ap = AppendArgv(ap, argv);
    // count the number of args
    for (ptr = ap; *ptr != NULL; ++ptr) {
        ++ap_argc;
    }
    di->d_cmd = ArgsToBuf(ap, ap_argc);
    FreeArgv(ap);

    if (ptab->sw_invoke(di, sess, prog, argv, arch, arch_args, arch_input,
                        backend, env, cwd, gpu_flag, debug_flag,
                        early_entry_flag)) {
        FreeDbgInvoke(di);
        return NULL;
    }

    // This command changes the status
    inferior_status_change = 1;

    return di;
}

static int DbgSendCmd(group_recv_t *set, char *request) {
    dbginvoke_t *di = set->rec_di;

    // ensure the send_cmd function is implemented
    if (DBG_PROTO_SW(di)->sw_send_cmd == NULL) {
        PrintOutError("Protocol function sw_send_cmd not implemented.\n");
        return 1;
    }

    set->_cmd_sent = DBG_PROTO_SW(di)->sw_send_cmd(di, set->rec_group, request);

    return (set->_cmd_sent != NULL ? 0 : 1);
}

static rep_set_t *DbgRecvReply(group_recv_t *set, dbgev req_type) {
    dbginvoke_t *di = set->rec_di;
    proto_msg_set *proto_res;
    proto_message *msg;
    rep_set_t *reply;
    group_reply_t *rep;
    dbgevent_t *e;
    dbgev ev;

    // ensure the recv_reply function is implemented
    if (DBG_PROTO_SW(di)->sw_recv_reply == NULL) {
        PrintOutError("Protocol function sw_recv_cmd not implemented.\n");
        ClearCmdSent(set);
        return NULL;
    }

    proto_res =
        DBG_PROTO_SW(di)->sw_recv_reply(di, set->rec_group, set->_cmd_sent);

    // Done with the received bitset
    ClearCmdSent(set);

    reply = NewRepSet();

    if (proto_res == NULL) {
        // Create a new error event based on this reply
        EVENT_ERROR(e, DBGERR_INTERNAL,
                    "Protocol returned null proto_msg_set.\n");
        rep = NewGroupReply(di, bitset_dup(set->rec_group), e);
        // Add the error to the reply set
        AddErrToSet(reply, rep);

        return reply;
    }

    // process the proto_res
    while ((msg = NextProtoMsgInSet(proto_res)) != NULL) {
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

            default:
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
    // want to return NULL. This is because some of the event types have no
    // data. These functions will want to receive null, otherwise they assume
    // that there was an error
    if (!SizeOfList(reply->err_list) && !SizeOfList(reply->rep_list)) {
        FreeRepSet(reply);
        return NULL;
    }

    return reply;
}

static rep_set_t *DbgSendCmdAndRecvReply(rec_set_t *set, char *request,
                                         dbgev req_type) {
    group_recv_t *recv_ptr;
    rep_set_t *result = NewRepSet();
    rep_set_t *reply_set;

    HoldSignals();

    // Send the command to each di in the receiver set
    SetReceiverHeadInSet(set);
    while ((recv_ptr = GetNextReceiverInSet(set)) != NULL) {
        // send the command to this di
        DbgSendCmd(recv_ptr, request);
    }

    // Now recv the reply for each di in the receiver set
    SetReceiverHeadInSet(set);
    while ((recv_ptr = GetNextReceiverInSet(set)) != NULL) {
        // recv the reply for this di, only if there is a _cmd_sent set.
        // Otherwise there was an error
        if (recv_ptr->_cmd_sent != NULL) {
            reply_set = DbgRecvReply(recv_ptr, req_type);
            if (reply_set != NULL) {
                result = MergeRepSet(result, reply_set);
            }
        }
    }

    ReleaseSignals();

    // If there is nothing in the result set, then return NULL
    if (!SizeOfList(result->err_list) && !SizeOfList(result->rep_list)) {
        FreeRepSet(result);
        return NULL;
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
    char *request;

    request = CommandToSocProto(DBGCMD_STARTUP);

    rep_set_t *result = DbgSendCmdAndRecvReply(set, request, DBGEV_FRAME);

    Free(request);

    // This command changes the status
    inferior_status_change = 1;

    ResetSet(result);
    return result;
}

/*
** DBG_SENDEXPR: Send expression to BE.
*/
void DbgSendExpr(rec_set_t *set, char *exp) {
    char *request;
    group_recv_t *recv_ptr;

    request = CommandToSocProto(DBGCMD_EVALEXPR, exp);

    SetReceiverHeadInSet(set);
    while ((recv_ptr = GetNextReceiverInSet(set)) != NULL) {
        // send the command to this di
        DbgSendCmd(recv_ptr, request);
    }

    Free(request);

    return;
}

/*
** DBG_GETEXPR: Get results of evaluate expression.
*/
rep_set_t *DbgGetExpr(rec_set_t *set) {
    group_recv_t *recv_ptr;
    rep_set_t *result = NewRepSet();
    rep_set_t *tmp_result;

    SetReceiverHeadInSet(set);
    while ((recv_ptr = GetNextReceiverInSet(set)) != NULL) {
        tmp_result = DbgRecvReply(recv_ptr, DBGEV_DATA);

        result = MergeRepSet(result, tmp_result);
    }

    ResetSet(result);
    return result;
}

/*
** DBGOPENSOCKET: Open socket for each debug server - for P2P
*/
rep_set_t *DbgOpenSocket(rec_set_t *set, char *opset_name, int opset_size) {
    char *request;

    request = CommandToSocProto(DBGCMD_OPENSOCKET, opset_name, opset_size);

    rep_set_t *result = DbgSendCmdAndRecvReply(set, request, DBGEV_BPSET);

    Free(request);

    ResetSet(result);
    return result;
}

/*
** DBGCLOSESOCKET: Close socket for each debug server - for P2P
*/
rep_set_t *DbgCloseSocket(rec_set_t *set) {
    char *request;

    request = CommandToSocProto(DBGCMD_CLOSESOCKET);

    rep_set_t *result = DbgSendCmdAndRecvReply(set, request, DBGEV_BPSET);

    Free(request);

    ResetSet(result);
    return result;
}

/*
** DBG_EVALEXPR: Evaluate expression.
*/
rep_set_t *DbgEvalExpr(rec_set_t *set, char *exp) {
    char *request;

    request = CommandToSocProto(DBGCMD_EVALEXPR, exp);

    rep_set_t *result = DbgSendCmdAndRecvReply(set, request, DBGEV_DATA);

    Free(request);

    ResetSet(result);
    return result;
}

/*
** DBG_SETDEBUGGERVAR: Set variable in debugger to new value.
*/
rep_set_t *DbgSetDebuggerVar(rec_set_t *set, char *var, AIF *val) {
    char *request;

    request = CommandToSocProto(DBGCMD_SETDEBUGGERVAR, var, val);

    rep_set_t *result = DbgSendCmdAndRecvReply(set, request, DBGEV_OK);

    Free(request);

    ResetSet(result);
    return result;
}

/*
** DBG_GETDEBUGGERVAR: Get variable in debugger.
*/
rep_set_t *DbgGetDebuggerVar(rec_set_t *set, char *var) {
    char *request;

    request = CommandToSocProto(DBGCMD_GETDEBUGGERVAR, var);

    rep_set_t *result = DbgSendCmdAndRecvReply(set, request, DBGEV_DATA);

    Free(request);

    ResetSet(result);
    return result;
}

/*
** DBG_SETSOURCEPATH: Set source search path in debugger to new value.
*/
rep_set_t *DbgSetSourcePath(rec_set_t *set, char *path) {
    rec_set_t *to_send = NewRecSet();
    group_recv_t *recv_ptr;
    dbgfrontend_sw *dbg_fe;
    char *request = NULL;
    rep_set_t *result = NULL;
    rep_set_t *tmp_res = NULL;
    dbgevent_t *e;
    group_reply_t *group_rep;

    // We want to do the actual reading of the source code on the frontend if
    // possible. This means this command should be directed towards the
    // frontend if possible.
    // Loop over each di in the rec set
    SetReceiverHeadInSet(set);
    while ((recv_ptr = GetNextReceiverInSet(set)) != NULL) {
        // point at the debug frontend
        if ((dbg_fe = recv_ptr->rec_di->d_frontend) != NULL) {
            if (result == NULL) {
                result = NewRepSet();
            }

            e = DbgFESetSourcePath(dbg_fe, path);

            if (e->ev_event != DBGEV_SOURCEPATH && e->ev_event != DBGEV_ERROR) {
                // Free the event
                FreeEvent(e);
                // Create a new error event
                EVENT_ERROR(
                    e, DBGERR_DEBUGGER,
                    "DbgFEList returned unknown event other than error.");
            }

            // Add the event to the result based on its type
            group_rep = NewGroupReply(recv_ptr->rec_di,
                                      bitset_dup(recv_ptr->rec_group), e);

            if (e->ev_event == DBGEV_ERROR) {
                AddErrToSet(result, group_rep);
            } else {
                AddReplyToSet(result, group_rep);
            }

            ResetSet(result);
        } else {
            // Add this recv_ptr to the to_send list
            AddReceiverToSet(to_send, CopyGroupReceiver(recv_ptr));
        }
    }

    // check to see if there are any commands left to send
    if (GetNumberOfReceivers(to_send)) {
        request = CommandToSocProto(DBGCMD_SETSOURCEPATH, path);

        tmp_res = DbgSendCmdAndRecvReply(to_send, request, DBGEV_SOURCEPATH);

        Free(request);

        // merge the results
        result = MergeRepSet(result, tmp_res);
    }

    FreeRecSet(to_send);
    ResetSet(result);
    return result;
}

/*
** DBG_GETSOURCEPATH: Get source search path from debugger.
*/
rep_set_t *DbgGetSourcePath(rec_set_t *set) {
    rec_set_t *to_send = NewRecSet();
    group_recv_t *recv_ptr;
    dbgfrontend_sw *dbg_fe;
    char *request = NULL;
    rep_set_t *result = NewRepSet();
    ;
    rep_set_t *tmp_res = NULL;
    dbgevent_t *e;
    group_reply_t *group_rep;

    // We want to do the actual reading of the source code on the frontend if
    // possible. This means this command should be directed towards the
    // frontend if possible.
    // Loop over each di in the rec set
    SetReceiverHeadInSet(set);
    while ((recv_ptr = GetNextReceiverInSet(set)) != NULL) {
        // point at the debug frontend
        if ((dbg_fe = recv_ptr->rec_di->d_frontend) != NULL) {
            e = DbgFEGetSourcePath(dbg_fe);

            if (e->ev_event != DBGEV_SOURCEPATH && e->ev_event != DBGEV_ERROR) {
                // Free the event
                FreeEvent(e);
                // Create a new error event
                EVENT_ERROR(
                    e, DBGERR_DEBUGGER,
                    "DbgFEList returned unknown event other than error.");
            }

            // Add the event to the result based on its type
            group_rep = NewGroupReply(recv_ptr->rec_di,
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
        request = CommandToSocProto(DBGCMD_GETSOURCEPATH);

        tmp_res = DbgSendCmdAndRecvReply(to_send, request, DBGEV_SOURCEPATH);

        Free(request);

        // merge the results
        result = MergeRepSet(result, tmp_res);
    }

    FreeRecSet(to_send);
    ResetSet(result);
    return result;
}

/*
** DBG_SETVAR: Set variable in program to new value.
*/
rep_set_t *DbgSetVar(rec_set_t *set, char *var, AIF *val) {
    char *request;

    request = CommandToSocProto(DBGCMD_SETVAR, var, val);

    rep_set_t *result = DbgSendCmdAndRecvReply(set, request, DBGEV_OK);

    Free(request);

    ResetSet(result);
    return result;
}

/*
** DBG_GETTYPE: Get type of variable.
** If isType is true, then get type of type.
*/
rep_set_t *DbgGetType(rec_set_t *set, char *var, int isType) {
    char *request;

    request = CommandToSocProto(DBGCMD_GETTYPE, var, isType);

    rep_set_t *result = DbgSendCmdAndRecvReply(set, request, DBGEV_TYPE);

    Free(request);

    ResetSet(result);
    return result;
}

/*
** DBG_GO: Continue/start execution.
*/
rep_set_t *DbgGo(rec_set_t *set) {
    group_recv_t *recv_ptr;
    char *request;
    rep_set_t *result;
    dbgfrontend_sw *dbg_fe;
    int has_err;
    group_reply_t *err;

    // Remove any old events for this group
    ResetForAsyncCmd(set);

    request = CommandToSocProto(DBGCMD_GO);

    result = DbgSendCmdAndRecvReply(set, request, DBGEV_OK);

    Free(request);

    // Clear the list location state since this modified the frame only if there
    // was no error
    if (!SetHasError(result)) {
        SetReceiverHeadInSet(set);
        while ((recv_ptr = GetNextReceiverInSet(set)) != NULL) {
            dbg_fe = recv_ptr->rec_di->d_frontend;
            if (dbg_fe != NULL && dbg_fe->dbg_list_isPending) {
                ClearListState(dbg_fe);
            }
        }
    } else {
        // selectively clear the list
        SetReceiverHeadInSet(set);
        while ((recv_ptr = GetNextReceiverInSet(set)) != NULL) {
            // reset the has_err
            has_err = 0;

            // Ensure this di is not in the error list
            SetErrHeadInSet(result);
            while ((err = GetNextErrInSet(result)) != NULL) {
                if (err->rep_di == recv_ptr->rec_di) {
                    // this di has an error
                    ++has_err;
                    break;
                }
            }

            // only proceed if there wasn't an error with this di
            if (!has_err) {
                dbg_fe = recv_ptr->rec_di->d_frontend;
                if (dbg_fe != NULL && dbg_fe->dbg_list_isPending) {
                    ClearListState(dbg_fe);
                }
            }
        }
    }

    // This command changes the status
    inferior_status_change = 1;

    // An async event is now pending
    async_pending = 1;

    ResetSet(result);
    return result;
}

/*
** DBG_FINISHFUNC: Continue execution until the end of the current function
*frame.
*/
rep_set_t *DbgFinishFunc(rec_set_t *set) {
    group_recv_t *recv_ptr;
    char *request;
    rep_set_t *result;
    dbgfrontend_sw *dbg_fe;
    int has_err;
    group_reply_t *err;

    // Remove any old events for this group
    ResetForAsyncCmd(set);

    request = CommandToSocProto(DBGCMD_FINISH);

    result = DbgSendCmdAndRecvReply(set, request, DBGEV_OK);

    Free(request);

    // Clear the list location state since this modified the frame only if there
    // was no error
    if (!SetHasError(result)) {
        SetReceiverHeadInSet(set);
        while ((recv_ptr = GetNextReceiverInSet(set)) != NULL) {
            dbg_fe = recv_ptr->rec_di->d_frontend;
            if (dbg_fe != NULL && dbg_fe->dbg_list_isPending) {
                ClearListState(dbg_fe);
            }
        }
    } else {
        // selectively clear the list
        SetReceiverHeadInSet(set);
        while ((recv_ptr = GetNextReceiverInSet(set)) != NULL) {
            // reset the has_err
            has_err = 0;

            // Ensure this di is not in the error list
            SetErrHeadInSet(result);
            while ((err = GetNextErrInSet(result)) != NULL) {
                if (err->rep_di == recv_ptr->rec_di) {
                    // this di has an error
                    ++has_err;
                    break;
                }
            }

            // only proceed if there wasn't an error with this di
            if (!has_err) {
                dbg_fe = recv_ptr->rec_di->d_frontend;
                if (dbg_fe != NULL && dbg_fe->dbg_list_isPending) {
                    ClearListState(dbg_fe);
                }
            }
        }
    }

    // This command changes the status
    inferior_status_change = 1;

    // An async event is now pending
    async_pending = 1;

    ResetSet(result);
    return result;
}

/*
** DBG_SETLINEBREAK: Set a breakpoint in program at
**			specified line.
*/
rep_set_t *DbgSetLineBreak(rec_set_t *set, char *file, int line) {
    char *request;
    rep_set_t *result;

    request = CommandToSocProto(DBGCMD_SETLINEBREAK, file, line);

    result = DbgSendCmdAndRecvReply(set, request, DBGEV_BPSET);

    Free(request);

    // process breakpoint information
    AddBP(result);

    ResetSet(result);
    return result;
}

/*
** DBG_SETFUNCBREAK: Set a breakpoint in program at
**			specified function.
*/
rep_set_t *DbgSetFuncBreak(rec_set_t *set, char *file, char *func) {
    char *request;
    rep_set_t *result;

    request = CommandToSocProto(DBGCMD_SETFUNCBREAK, file, func);

    result = DbgSendCmdAndRecvReply(set, request, DBGEV_BPSET);

    Free(request);

    // process breakpoint information
    AddBP(result);

    ResetSet(result);
    return result;
}

/*
** DBG_SETWATCH: Set a watchpoint in program for
**			specified expression.
*/
rep_set_t *DbgSetWatch(rec_set_t *set, int type, char *expr) {
    char *request;

    request = CommandToSocProto(DBGCMD_SETWATCH, type, expr);

    rep_set_t *result = DbgSendCmdAndRecvReply(set, request, DBGEV_BPSET);

    Free(request);

    // process breakpoint information
    AddBP(result);

    ResetSet(result);
    return result;
}

/*
** DBG_KILL: Terminate execution of program.
** TODO: Probably remove this, the DbgQuit should be sufficient.
*/
rep_set_t *DbgKill(rec_set_t *set) {
    group_recv_t *recv_ptr;
    char *request;
    rep_set_t *result;
    dbginvoke_t *di;
    int res;
    dbgevent_t *e;
    group_reply_t *err_rep;

    request = CommandToSocProto(DBGCMD_KILL);

    result = DbgSendCmdAndRecvReply(set, request, DBGEV_OK);

    Free(request);

    // Loop over each di and call the protocol specific hook
    SetReceiverHeadInSet(set);
    while ((recv_ptr = GetNextReceiverInSet(set)) != NULL) {
        // point at the di
        di = recv_ptr->rec_di;

        if (DBG_PROTO_SW(di)->sw_exit == NULL) {
            if (result == NULL) {
                result = NewRepSet();
            }

            // Create a new error event
            EVENT_ERROR(e, DBGERR_NOTIMP, NULL);
            err_rep = NewGroupReply(di, bitset_dup(recv_ptr->rec_group), e);
            // Add the error to the reply set
            AddErrToSet(result, err_rep);

        } else {
            // We always want to kill the application, so hardcode detatch to 0
            res = DBG_PROTO_SW(di)->sw_exit(di, 0);
        }

        FreeDbgInvoke(di);

        recv_ptr->rec_di = NULL;
    }

    ResetSet(result);
    return result;
}

/*
** DBG_DELETEBREAK: Delete a breakpoint.
*/
rep_set_t *DbgDeleteBreak(rec_set_t *set, int bp_set_num) {
    group_recv_t *recv_ptr;
    dbginvoke_t *di;
    bp_set_t *bp_set;
    rep_set_t *tmp_result;
    bitset *group;
    bp_id_set_t *id_set;
    char *request;
    rec_set_t *new_rec_set;
    rep_set_t *result = NULL;

    // if bp_set_num is 0, we want this to apply to every breakpoint associated
    // with each dbginvoke_t
    SetReceiverHeadInSet(set);
    while ((recv_ptr = GetNextReceiverInSet(set)) != NULL) {
        // point at the di
        di = recv_ptr->rec_di;

        if (bp_set_num == 0) {
            ResetBPSetList(di);
            while ((bp_set = GetNextBPSet(di)) != NULL) {
                // We send the command to the backend based on the backend gdb
                // id. Loop over this list
                SetList(bp_set->b_bp_id_list);
                while ((id_set = (bp_id_set_t *)GetListElement(
                            bp_set->b_bp_id_list)) != NULL) {
                    // Get the new group, based on the intersection between the
                    // bp_set and the rec_set.
                    group = bitset_and(id_set->b_group, recv_ptr->rec_group);

                    // Ensure that there are ranks in the bitset, otherwise
                    // continue
                    if (bitset_isempty(group)) {
                        bitset_free(group);
                        continue;
                    }

                    // Create the request for this group
                    request =
                        CommandToSocProto(DBGCMD_DELETEBREAK, id_set->b_bp_id);

                    // Create the new rec set for this group
                    new_rec_set = NewRecSet();
                    AddReceiverToSet(new_rec_set, NewGroupReceiver(di, group));

                    tmp_result =
                        DbgSendCmdAndRecvReply(new_rec_set, request, DBGEV_OK);

                    // We expect this to return NULL on success, handle the case
                    // were things worked
                    if (tmp_result == NULL) {
                        // remove the breakpoint
                        RemoveBPSet(di, bp_set, group);
                    }

                    // Add this to the overall result
                    if (result == NULL) {
                        result = tmp_result;
                    } else {
                        if (tmp_result != NULL) {
                            result = MergeRepSet(result, tmp_result);
                        }
                    }

                    Free(request);
                    bitset_free(group);
                    FreeRecSet(new_rec_set);
                }
            }
        } else {
            // find the bp_set for the bp_set_num
            if ((bp_set = FindBPSetByID(di, bp_set_num)) == NULL) {
                // Error case: the provided bp_set_num does not exist
                dbgevent_t *e;
                group_reply_t *group_rep;

                // Create a new error event
                EVENT_ERROR(e, DBGERR_DEBUGGER, "No breakpoint number %d.\n",
                            bp_set_num);
                // Add the event to the result based on its type
                group_rep =
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
                        bp_set->b_bp_id_list)) != NULL) {
                // Get the new group, based on the intersection between the
                // bp_set and the set.
                group = bitset_and(id_set->b_group, recv_ptr->rec_group);

                // Ensure that there are ranks in the bitset, otherwise continue
                if (bitset_isempty(group)) {
                    bitset_free(group);
                    continue;
                }

                // Create the request for this group
                request =
                    CommandToSocProto(DBGCMD_DELETEBREAK, id_set->b_bp_id);

                // Create the new rec set for this group
                new_rec_set = NewRecSet();
                AddReceiverToSet(new_rec_set, NewGroupReceiver(di, group));

                tmp_result =
                    DbgSendCmdAndRecvReply(new_rec_set, request, DBGEV_OK);

                // We expect this to return NULL on success, handle the case
                // were things worked
                if (tmp_result == NULL) {
                    // remove the breakpoint
                    RemoveBPSet(di, bp_set, group);
                }

                // Add this to the overall result
                if (result == NULL) {
                    result = tmp_result;
                } else {
                    if (tmp_result != NULL) {
                        result = MergeRepSet(result, tmp_result);
                    }
                }

                Free(request);
                bitset_free(group);
                FreeRecSet(new_rec_set);
            }
        }
    }

    ResetSet(result);
    return result;
}

/*
** DBG_DISABLEBREAK: Disable a breakpoint.
*/
rep_set_t *DbgDisableBreak(rec_set_t *set, int bp_set_num) {
    group_recv_t *recv_ptr;
    dbginvoke_t *di;
    bp_set_t *bp_set;
    rep_set_t *tmp_result;
    bitset *group;
    bp_id_set_t *id_set;
    char *request;
    rec_set_t *new_rec_set;
    rep_set_t *result = NULL;

    // if bp_set_num is 0, we want this to apply to every breakpoint associated
    // with each dbginvoke_t
    SetReceiverHeadInSet(set);
    while ((recv_ptr = GetNextReceiverInSet(set)) != NULL) {
        // point at the di
        di = recv_ptr->rec_di;

        if (bp_set_num == 0) {
            ResetBPSetList(di);
            while ((bp_set = GetNextBPSet(di)) != NULL) {
                // We send the command to the backend based on the backend gdb
                // id. Loop over this list
                SetList(bp_set->b_bp_id_list);
                while ((id_set = (bp_id_set_t *)GetListElement(
                            bp_set->b_bp_id_list)) != NULL) {
                    // Get the new group, based on the intersection between the
                    // bp_set and the rec_set.
                    group = bitset_and(id_set->b_group, recv_ptr->rec_group);

                    // Ensure that there are ranks in the bitset, otherwise
                    // continue
                    if (bitset_isempty(group)) {
                        bitset_free(group);
                        continue;
                    }

                    // Create the request for this group
                    request =
                        CommandToSocProto(DBGCMD_DISABLEBREAK, id_set->b_bp_id);

                    // Create the new rec set for this group
                    new_rec_set = NewRecSet();
                    AddReceiverToSet(new_rec_set, NewGroupReceiver(di, group));

                    tmp_result =
                        DbgSendCmdAndRecvReply(new_rec_set, request, DBGEV_OK);

                    // We expect this to return NULL on success, handle the case
                    // were things worked
                    if (tmp_result == NULL) {
                        // mark the breakpoint as enabled for this group
                        DisableBPForBPSet(bp_set, group);
                    }

                    // Add this to the overall result
                    if (result == NULL) {
                        result = tmp_result;
                    } else {
                        if (tmp_result != NULL) {
                            result = MergeRepSet(result, tmp_result);
                        }
                    }

                    Free(request);
                    bitset_free(group);
                    FreeRecSet(new_rec_set);
                }
            }
        } else {
            // find the bp_set for the bp_set_num
            if ((bp_set = FindBPSetByID(di, bp_set_num)) == NULL) {
                // Error case: the provided bp_set_num does not exist
                dbgevent_t *e;
                group_reply_t *group_rep;

                // Create a new error event
                EVENT_ERROR(e, DBGERR_DEBUGGER, "No breakpoint number %d.\n",
                            bp_set_num);
                // Add the event to the result based on its type
                group_rep =
                    NewGroupReply(di, bitset_dup(recv_ptr->rec_group), e);
                // Create a new result and return it
                if (result == NULL) {
                    result = NewRepSet();
                }
                AddErrToSet(result, group_rep);
                continue;
            }

            // Need to loop over the b_bp_id_list to operate on the correct
            // breakpoint number in the backend
            SetList(bp_set->b_bp_id_list);
            while ((id_set = (bp_id_set_t *)GetListElement(
                        bp_set->b_bp_id_list)) != NULL) {
                // Get the new group, based on the intersection between the
                // bp_set and the rec_set.
                group = bitset_and(id_set->b_group, recv_ptr->rec_group);

                // Ensure that there are ranks in the bitset, otherwise continue
                if (bitset_isempty(group)) {
                    bitset_free(group);
                    continue;
                }

                // Create the request for this group
                request =
                    CommandToSocProto(DBGCMD_DISABLEBREAK, id_set->b_bp_id);

                // Create the new rec set for this group
                new_rec_set = NewRecSet();
                AddReceiverToSet(new_rec_set, NewGroupReceiver(di, group));

                tmp_result =
                    DbgSendCmdAndRecvReply(new_rec_set, request, DBGEV_OK);

                // We expect this to return NULL on success, handle the case
                // were things worked
                if (tmp_result == NULL) {
                    // mark the breakpoint as disabled for this group
                    DisableBPForBPSet(bp_set, group);
                }

                // Add this to the overall result
                if (result == NULL) {
                    result = tmp_result;
                } else {
                    if (tmp_result != NULL) {
                        result = MergeRepSet(result, tmp_result);
                    }
                }

                Free(request);
                bitset_free(group);
                FreeRecSet(new_rec_set);
            }
        }
    }

    ResetSet(result);
    return result;
}

/*
** DBG_ENABLEBREAK: Enable a breakpoint.
*/
rep_set_t *DbgEnableBreak(rec_set_t *set, int bp_set_num, int once, int del) {
    group_recv_t *recv_ptr;
    dbginvoke_t *di;
    bp_set_t *bp_set;
    rep_set_t *tmp_result;
    bitset *group;
    bp_id_set_t *id_set;
    char *request;
    rec_set_t *new_rec_set;
    rep_set_t *result = NULL;

    // if bp_set_num is 0, we want this to apply to every breakpoint associated
    // with each dbginvoke_t
    SetReceiverHeadInSet(set);
    while ((recv_ptr = GetNextReceiverInSet(set)) != NULL) {
        // point at the di
        di = recv_ptr->rec_di;

        if (bp_set_num == 0) {
            ResetBPSetList(di);
            while ((bp_set = GetNextBPSet(di)) != NULL) {
                // We send the command to the backend based on the backend gdb
                // id. Loop over this list
                SetList(bp_set->b_bp_id_list);
                while ((id_set = (bp_id_set_t *)GetListElement(
                            bp_set->b_bp_id_list)) != NULL) {
                    // Get the new group, based on the intersection between the
                    // bp_set and the rec_set.
                    group = bitset_and(id_set->b_group, recv_ptr->rec_group);

                    // Ensure that there are ranks in the bitset, otherwise
                    // continue
                    if (bitset_isempty(group)) {
                        bitset_free(group);
                        continue;
                    }

                    // Create the request for this group
                    request = CommandToSocProto(DBGCMD_ENABLEBREAK,
                                                id_set->b_bp_id, once, del);

                    // Create the new rec set for this group
                    new_rec_set = NewRecSet();
                    AddReceiverToSet(new_rec_set, NewGroupReceiver(di, group));

                    tmp_result =
                        DbgSendCmdAndRecvReply(new_rec_set, request, DBGEV_OK);

                    // We expect this to return NULL on success, handle the case
                    // were things worked
                    if (tmp_result == NULL) {
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
                    if (result == NULL) {
                        result = tmp_result;
                    } else {
                        if (tmp_result != NULL) {
                            result = MergeRepSet(result, tmp_result);
                        }
                    }

                    Free(request);
                    bitset_free(group);
                    FreeRecSet(new_rec_set);
                }
            }
        } else {
            // find the bp_set for the bp_set_num
            if ((bp_set = FindBPSetByID(di, bp_set_num)) == NULL) {
                // Error case: the provided bp_set_num does not exist
                dbgevent_t *e;
                group_reply_t *group_rep;

                // Create a new error event
                EVENT_ERROR(e, DBGERR_DEBUGGER, "No breakpoint number %d.\n",
                            bp_set_num);
                // Add the event to the result based on its type
                group_rep =
                    NewGroupReply(di, bitset_dup(recv_ptr->rec_group), e);
                // Create a new result and return it
                if (result == NULL) {
                    result = NewRepSet();
                }
                AddErrToSet(result, group_rep);
                continue;
            }

            // Need to loop over the b_bp_id_list to operate on the correct
            // breakpoint number in the backend
            SetList(bp_set->b_bp_id_list);
            while ((id_set = (bp_id_set_t *)GetListElement(
                        bp_set->b_bp_id_list)) != NULL) {
                // Get the new group, based on the intersection between the
                // bp_set and the rec_set.
                group = bitset_and(id_set->b_group, recv_ptr->rec_group);

                // Ensure that there are ranks in the bitset, otherwise continue
                if (bitset_isempty(group)) {
                    bitset_free(group);
                    continue;
                }

                // Create the request for this group
                request = CommandToSocProto(DBGCMD_ENABLEBREAK, id_set->b_bp_id,
                                            once, del);

                // Create the new rec set for this group
                new_rec_set = NewRecSet();
                AddReceiverToSet(new_rec_set, NewGroupReceiver(di, group));

                tmp_result =
                    DbgSendCmdAndRecvReply(new_rec_set, request, DBGEV_OK);

                // We expect this to return NULL on success, handle the case
                // were things worked
                if (tmp_result == NULL) {
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
                if (result == NULL) {
                    result = tmp_result;
                } else {
                    if (tmp_result != NULL) {
                        result = MergeRepSet(result, tmp_result);
                    }
                }

                Free(request);
                bitset_free(group);
                FreeRecSet(new_rec_set);
            }
        }
    }

    ResetSet(result);
    return result;
}

/*
** DBG_LIST: List specified function or line.
*/
rep_set_t *DbgList(rec_set_t *set, char *str) {
    rec_set_t *to_send = NewRecSet();
    group_recv_t *recv_ptr;
    dbgfrontend_sw *dbg_fe;
    char *request;
    rec_set_t *new_rec_set;
    rep_set_t *result = NewRepSet();
    rep_set_t *tmp_result;
    group_reply_t *group_rep;
    dbgframe_t *fp;
    dbgevent_t *e;
    char *location = NULL;
    int isReverse, i, cont;
    set_location *set_loc;

    // We want to do the actual reading of the source code on the frontend if
    // possible. This is because there is a performance hit associated with
    // opening cross mounted files on the compute nodes. Its also the case that
    // sometimes we are unable to read the source files since they are located
    // in a non-cross mounted location.

    // Note that we can only do this if there is a debug frontend associated
    // with the dbginvoke

    // Loop over each di in the set
    SetReceiverHeadInSet(set);
    while ((recv_ptr = GetNextReceiverInSet(set)) != NULL) {
        // point at the dbg fe
        dbg_fe = recv_ptr->rec_di->d_frontend;

        if (dbg_fe != NULL) {
            // set the forward/reverse direction
            isReverse = (str == NULL) ? 0 : (*str == '-');

            // First is the logic for the case where a previous list is not
            // pending. If str is null, then we need to ask each of the ranks
            // for their current frame.
            if (((str == NULL) ? 1 : isReverse) &&
                !(dbg_fe->dbg_list_isPending)) {
                // create and send the command, we only want the innermost
                // frame.
                request = CommandToSocProto(DBGCMD_SHOWFRAME, 1);

                // Create the new rec set for this group
                new_rec_set = NewRecSet();
                AddReceiverToSet(new_rec_set, CopyGroupReceiver(recv_ptr));

                tmp_result =
                    DbgSendCmdAndRecvReply(new_rec_set, request, DBGEV_FRAME);
                Free(request);
                FreeRecSet(new_rec_set);

                // Make sure there isn't an error in the result, otherwise add
                // this to the to_send list
                if (SetHasError(tmp_result)) {
                    FreeRepSet(tmp_result);
                    tmp_result = NULL;
                    AddReceiverToSet(to_send, CopyGroupReceiver(recv_ptr));
                    continue;
                }

                // process the frame
                SetReplyHeadInSet(tmp_result);
                while ((group_rep = GetNextReplyInSet(tmp_result)) != NULL) {
                    // Point at the event frame
                    fp = EVENT_FRAME(group_rep->rep_event);

                    // Now ask the frontend for the source listing around this
                    // location. This is set depending on the value of isReverse
                    if (isReverse) {
                        Asprintf(&location, ",%s:%d", fp->frame_loc.loc_file,
                                 fp->frame_loc.loc_line);
                    } else {
                        Asprintf(&location, "%s:%d,", fp->frame_loc.loc_file,
                                 fp->frame_loc.loc_line);
                    }

                    // Save this new list location state for later calls to list
                    if (AddListStateLocation(dbg_fe, group_rep->rep_group,
                                             location) < 0) {
                        PrintOutError(
                            "Failed to save list location. Further calls to "
                            "list will not save state.\n");
                    }

                    // Send the list command to the frontend
                    e = DbgFEList(dbg_fe, location);

                    // Free the location, we are done with it
                    Free(location);
                    location = NULL;

                    // Make sure the event is a DBGEV_LIST, otherwise add this
                    // to the to_send list
                    if (e->ev_event != DBGEV_LIST) {
                        FreeRepSet(tmp_result);
                        tmp_result = NULL;
                        FreeEvent(e);
                        e = NULL;
                        ClearListState(dbg_fe);
                        AddReceiverToSet(to_send, CopyGroupReceiver(recv_ptr));
                        continue;
                    }

                    // Replace the event in the group_rep with this one
                    FreeEvent(group_rep->rep_event);
                    group_rep->rep_event = e;
                }

                // This has converted the SHOWFRAME result into a LIST result.
                result = MergeRepSet(result, tmp_result);

            } else if (((str == NULL) ? 1 : isReverse) &&
                       dbg_fe->dbg_list_isPending) {
                // We will create the result by hand based on the saved list
                // location state data

                // loop over every set_location
                SetListStateLocation(dbg_fe);
                while ((set_loc = GetListStateLocation(dbg_fe)) != NULL) {
                    // Call the list command using the saved location string to
                    // set things up
                    e = DbgFEList(dbg_fe, set_loc->loc_str);

                    // handle the error case
                    if (e->ev_event == DBGEV_ERROR) {
                        group_rep = NewGroupReply(recv_ptr->rec_di,
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
                            e = DbgFEList(dbg_fe, "-");

                            // handle the error case
                            if (e->ev_event == DBGEV_ERROR) {
                                group_rep =
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
                            e = DbgFEList(dbg_fe, "");

                            // handle the error case
                            if (e->ev_event == DBGEV_ERROR) {
                                group_rep =
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
                        e = DbgFEList(dbg_fe, "-");
                        set_loc->count--;
                    } else {
                        e = DbgFEList(dbg_fe, "");
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
                    group_rep = NewGroupReply(recv_ptr->rec_di,
                                              bitset_dup(set_loc->set), e);

                    if (e->ev_event == DBGEV_ERROR) {
                        AddErrToSet(result, group_rep);
                    } else {
                        AddReplyToSet(result, group_rep);
                    }
                }

                // This created the result from the saved list location
                // directly.

            } else if (str != NULL && !isReverse) {
                // Note that we are going to create the result by hand here
                // based on the bitset in the group_recv_t. If the user calls
                // the list cmd with a string, then all ranks are going to have
                // the same answer.

                // Make sure we clear the state if there was any saved
                ClearListState(dbg_fe);

                // Save this new list location state for later calls to list
                if (AddListStateLocation(dbg_fe, recv_ptr->rec_group, str) <
                    0) {
                    PrintOutError(
                        "Failed to save list location. Further calls to list "
                        "will not save state.\n");
                }

                // we can pass the str directly to the frontend list command
                // since its not null and was provided by the user.
                e = DbgFEList(dbg_fe, str);

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
                group_rep = NewGroupReply(recv_ptr->rec_di,
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

        request = CommandToSocProto(DBGCMD_LIST, str);

        tmp_result = DbgSendCmdAndRecvReply(to_send, request, DBGEV_LIST);

        Free(request);

        // merge the results
        result = MergeRepSet(result, tmp_result);
    }

    FreeRecSet(to_send);
    ResetSet(result);
    return result;
}

/*
** DBG_GET_SOURCES: Ask for source files of inferior.
*/
rep_set_t *DbgGetSources(rec_set_t *set) {
    rec_set_t *to_send = NewRecSet();
    group_recv_t *recv_ptr;
    dbgfrontend_sw *dbg_fe;
    char *request;
    dbgevent_t *e;
    rep_set_t *result = NewRepSet();
    rep_set_t *tmp_result;
    group_reply_t *group_rep;

    // We want to do the actual reading of the source files on the frontend if
    // possible. This is because there is a performance hit associated with
    // opening cross mounted files on the compute nodes. Its also the case that
    // sometimes we are unable to read the source files since they are located
    // in a non-cross mounted location.

    // Note that we can only do this if there is a debug frontend associated
    // with the dbginvoke

    // Loop over each di in the set
    SetReceiverHeadInSet(set);
    while ((recv_ptr = GetNextReceiverInSet(set)) != NULL) {
        // point at the dbg fe
        dbg_fe = recv_ptr->rec_di->d_frontend;

        if (dbg_fe != NULL) {
            e = DbgFEGetSources(dbg_fe);

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
            group_rep = NewGroupReply(recv_ptr->rec_di,
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

        request = CommandToSocProto(DBGCMD_GETSOURCES);

        tmp_result = DbgSendCmdAndRecvReply(to_send, request, DBGEV_SOURCES);

        Free(request);

        // merge the results
        result = MergeRepSet(result, tmp_result);
    }

    FreeRecSet(to_send);
    ResetSet(result);
    return result;
}

/*
** DBG_SETARGS: Set arguments to pass to program when run.
*/
rep_set_t *DbgSetArgs(rec_set_t *set, char *args) {
    char *request;

    request = CommandToSocProto(DBGCMD_SETARGS, args);

    rep_set_t *result = DbgSendCmdAndRecvReply(set, request, DBGEV_OK);

    Free(request);

    ResetSet(result);
    return result;
}

/*
** DBG_STEP: Execute to next statement. Repeat count times.
**		If in == 0, pass over function calls (ala next).
*/
rep_set_t *DbgStep(rec_set_t *set, int count, int in) {
    char *request;
    group_recv_t *recv_ptr;
    dbgfrontend_sw *dbg_fe;
    int has_err;
    group_reply_t *err;

    // Remove any old events for this group
    ResetForAsyncCmd(set);

    request = CommandToSocProto(DBGCMD_STEP, count, in);

    rep_set_t *result = DbgSendCmdAndRecvReply(set, request, DBGEV_OK);

    Free(request);

    // Clear the list location state since this modified the frame only if there
    // was no error
    if (!SetHasError(result)) {
        SetReceiverHeadInSet(set);
        while ((recv_ptr = GetNextReceiverInSet(set)) != NULL) {
            dbg_fe = recv_ptr->rec_di->d_frontend;
            if (dbg_fe != NULL && dbg_fe->dbg_list_isPending) {
                ClearListState(dbg_fe);
            }
        }
    } else {
        // selectively clear the list
        SetReceiverHeadInSet(set);
        while ((recv_ptr = GetNextReceiverInSet(set)) != NULL) {
            // reset the has_err
            has_err = 0;

            // Ensure this di is not in the error list
            SetErrHeadInSet(result);
            while ((err = GetNextErrInSet(result)) != NULL) {
                if (err->rep_di == recv_ptr->rec_di) {
                    // this di has an error
                    ++has_err;
                    break;
                }
            }

            // only proceed if there wasn't an error with this di
            if (!has_err) {
                dbg_fe = recv_ptr->rec_di->d_frontend;
                if (dbg_fe != NULL && dbg_fe->dbg_list_isPending) {
                    ClearListState(dbg_fe);
                }
            }
        }
    }

    // This command changes the status
    inferior_status_change = 1;

    // An async event is now pending
    async_pending = 1;

    ResetSet(result);
    return result;
}

/*
** DBG_MOVEFRAME: Move up or down stack frames.
*/
rep_set_t *DbgMoveFrame(rec_set_t *set, int count, int up) {
    char *request;
    rep_set_t *result;
    group_recv_t *recv_ptr;
    dbgfrontend_sw *dbg_fe;
    dbgframe_t *fp;
    group_reply_t *group_rep;
    char *location = NULL;
    int has_err;
    group_reply_t *err;

    request = CommandToSocProto(DBGCMD_MOVEFRAME, count, up);

    result = DbgSendCmdAndRecvReply(set, request, DBGEV_FRAME);

    Free(request);

    // Clear the list location state and set it to the new frame since this
    // modified the frame location. Only do it if it didn't have an error.
    if (!SetHasError(result)) {
        SetReceiverHeadInSet(set);
        while ((recv_ptr = GetNextReceiverInSet(set)) != NULL) {
            // point at the dbg fe
            dbg_fe = recv_ptr->rec_di->d_frontend;

            if (dbg_fe != NULL) {
                // Clear any existing state
                ClearListState(dbg_fe);

                // process the frame
                SetReplyHeadInSet(result);
                while ((group_rep = GetNextReplyInSet(result)) != NULL) {
                    // Point at the frame
                    fp = EVENT_FRAME(group_rep->rep_event);

                    // Create the location string based on the new frame
                    // location Note that this should one short of the new
                    // location since we didn't generate this by listing source
                    // a first time. Otherwise we will start getting 10 lines
                    // after this location and it will be messed up.
                    Asprintf(&location, ",%s:%d", fp->frame_loc.loc_file,
                             fp->frame_loc.loc_line - 1);

                    // Save this new list location state for later calls to list
                    if (AddListStateLocation(dbg_fe, group_rep->rep_group,
                                             location) < 0) {
                        PrintOutError(
                            "Failed to save list location. Further calls to "
                            "list will not save state.\n");
                    }

                    // Free the location, we are done with it
                    Free(location);
                    location = NULL;
                }
            }
        }
    } else {
        // selectively clear the list
        SetReceiverHeadInSet(set);
        while ((recv_ptr = GetNextReceiverInSet(set)) != NULL) {
            // reset the has_err
            has_err = 0;

            // Ensure this di is not in the error list
            SetErrHeadInSet(result);
            while ((err = GetNextErrInSet(result)) != NULL) {
                if (err->rep_di == recv_ptr->rec_di) {
                    // this di has an error
                    ++has_err;
                    break;
                }
            }

            // only proceed if there wasn't an error with this di
            if (!has_err) {
                // point at the dbg fe
                dbg_fe = recv_ptr->rec_di->d_frontend;

                if (dbg_fe != NULL) {
                    // Clear any existing state
                    ClearListState(dbg_fe);

                    // process the frame
                    SetReplyHeadInSet(result);
                    while ((group_rep = GetNextReplyInSet(result)) != NULL) {
                        // Point at the frame
                        fp = EVENT_FRAME(group_rep->rep_event);

                        // Create the location string based on the new frame
                        // location Note that this should one short of the new
                        // location since we didn't generate this by listing
                        // source a first time. Otherwise we will start getting
                        // 10 lines after this location and it will be messed
                        // up.
                        Asprintf(&location, ",%s:%d", fp->frame_loc.loc_file,
                                 fp->frame_loc.loc_line - 1);

                        // Save this new list location state for later calls to
                        // list
                        if (AddListStateLocation(dbg_fe, group_rep->rep_group,
                                                 location) < 0) {
                            PrintOutError(
                                "Failed to save list location. Further calls "
                                "to list will not save state.\n");
                        }

                        // Free the location, we are done with it
                        Free(location);
                        location = NULL;
                    }
                }
            }
        }
    }

    ResetSet(result);
    return result;
}

/*
** DBG_SHOWFRAME: Show current stack.
*/
rep_set_t *DbgShowFrame(rec_set_t *set, int count) {
    char *request;

    request = CommandToSocProto(DBGCMD_SHOWFRAME, count);

    rep_set_t *result = DbgSendCmdAndRecvReply(set, request, DBGEV_FRAME);

    Free(request);

    ResetSet(result);
    return result;
}

/*
** DBG_GETFRAME: Get selected stack frame.
*/
rep_set_t *DbgGetFrame(rec_set_t *set) {
    char *request;

    request = CommandToSocProto(DBGCMD_GETFRAME);

    rep_set_t *result = DbgSendCmdAndRecvReply(set, request, DBGEV_FRAME);

    Free(request);

    ResetSet(result);
    return result;
}

/*
** DBG_GETVARS: Return defined variables.
*/
rep_set_t *DbgGetVars(rec_set_t *set, char *file) {
    char *request;

    request = CommandToSocProto(DBGCMD_GETVARS, file);

    rep_set_t *result = DbgSendCmdAndRecvReply(set, request, DBGEV_VARS);

    Free(request);

    ResetSet(result);
    return result;
}

/*
** DBG_GETSCOPE: Return variables local to a scope.
*/
rep_set_t *DbgGetScope(rec_set_t *set, char *location) {
    rec_set_t *to_send = NewRecSet();
    group_recv_t *recv_ptr;
    dbgfrontend_sw *dbg_fe;
    char *request;
    rep_set_t *result = NewRepSet();
    rep_set_t *tmp_result;
    dbgevent_t *e;
    group_reply_t *group_rep;

    // We want to do the actual reading of the source files on the frontend if
    // possible. This is because there is a performance hit associated with
    // opening cross mounted files on the compute nodes. Its also the case that
    // sometimes we are unable to read the source files since they are located
    // in a non-cross mounted location.

    // Note that we can only do this if there is a debug frontend associated
    // with the dbginvoke

    // Loop over each di in the set
    SetReceiverHeadInSet(set);
    while ((recv_ptr = GetNextReceiverInSet(set)) != NULL) {
        // point at the dbg fe
        dbg_fe = recv_ptr->rec_di->d_frontend;

        if (dbg_fe != NULL) {
            e = DbgFEGetScope(dbg_fe, location);

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
            group_rep = NewGroupReply(recv_ptr->rec_di,
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

        request = CommandToSocProto(DBGCMD_GETSCOPE, location);

        tmp_result = DbgSendCmdAndRecvReply(to_send, request, DBGEV_VARS);

        Free(request);

        // merge the results
        result = MergeRepSet(result, tmp_result);
    }

    FreeRecSet(to_send);
    ResetSet(result);
    return result;
}

/*
** DBG_DIRECTCMD: Send direct command to debug backend
*/
rep_set_t *DbgDirectCmd(rec_set_t *set, int argc, char **argv) {
    char *request;

    request = CommandToSocProto(DBGCMD_DIRECT, argc, argv);

    rep_set_t *result = DbgSendCmdAndRecvReply(set, request, DBGEV_CMD);

    Free(request);

    ResetSet(result);
    return result;
}

/*
** DBG_DBGSTATUS: Return status of debugger.
*/
rep_set_t *DbgStatus(rec_set_t *set, int getLocation) {
    char *request;

    request = CommandToSocProto(DBGCMD_STATUS, getLocation);

    rep_set_t *result = DbgSendCmdAndRecvReply(set, request, DBGEV_STAT);

    Free(request);

    ResetSet(result);
    return result;
}

/*
** DBG_INTERRUPT: Interrupt current debugger operation.
*/
rep_set_t *DbgInterrupt(rec_set_t *set) {
    char *request;
    rep_set_t *result;
    group_recv_t *recv_ptr;
    dbgfrontend_sw *dbg_fe;
    int has_err;
    group_reply_t *err;

    // Remove any old events for this group
    ResetForAsyncCmd(set);

    request = CommandToSocProto(DBGCMD_INTERRUPT);

    result = DbgSendCmdAndRecvReply(set, request, DBGEV_HALT);

    Free(request);

    // Clear the list location state since this modified the frame only if there
    // was no error
    if (!SetHasError(result)) {
        SetReceiverHeadInSet(set);
        while ((recv_ptr = GetNextReceiverInSet(set)) != NULL) {
            dbg_fe = recv_ptr->rec_di->d_frontend;
            if (dbg_fe != NULL && dbg_fe->dbg_list_isPending) {
                ClearListState(dbg_fe);
            }
        }
    } else {
        // selectively clear the list
        SetReceiverHeadInSet(set);
        while ((recv_ptr = GetNextReceiverInSet(set)) != NULL) {
            // reset the has_err
            has_err = 0;

            // Ensure this di is not in the error list
            SetErrHeadInSet(result);
            while ((err = GetNextErrInSet(result)) != NULL) {
                if (err->rep_di == recv_ptr->rec_di) {
                    // this di has an error
                    ++has_err;
                    break;
                }
            }

            // only proceed if there wasn't an error with this di
            if (!has_err) {
                dbg_fe = recv_ptr->rec_di->d_frontend;
                if (dbg_fe != NULL && dbg_fe->dbg_list_isPending) {
                    ClearListState(dbg_fe);
                }
            }
        }
    }

    // This command changes the status
    inferior_status_change = 1;

    // An async event is now pending
    async_pending = 1;

    ResetSet(result);
    return result;
}

/*
** DBG_QUIT: Tell server to quit immediately.
** Use the value of det to override the default behavior
*/
rep_set_t *DbgQuit(rec_set_t *set, dbgquit_type det) {
    int detach = 0;
    char *request;
    rep_set_t *result = NewRepSet();
    rep_set_t *tmp_result;
    group_recv_t *recv_ptr;
    dbginvoke_t *di;
    group_reply_t *err_rep;
    int res;
    dbgevent_t *e;
    rec_set_t *to_send;

    // set the detach value
    switch (det) {
        case DBG_KILL:
            detach = 0;
            goto uniform_exit;

        case DBG_DETACH:
            detach = 1;

        uniform_exit:
            // The caller hardcoded the quit type, so it is the same for every
            // di. There is no need to selectively quit based on the di type.
            request = CommandToSocProto(DBGCMD_QUIT, detach);

            tmp_result = DbgSendCmdAndRecvReply(set, request, DBGEV_OK);

            Free(request);

            // merge the results
            result = MergeRepSet(result, tmp_result);

            // Call protocol specific communication network exit function and
            // destroy the di for each di in the rec_set
            SetReceiverHeadInSet(set);
            while ((recv_ptr = GetNextReceiverInSet(set)) != NULL) {
                // point at the di
                di = recv_ptr->rec_di;

                if (DBG_PROTO_SW(di)->sw_exit == NULL) {
                    // Create a new error event
                    EVENT_ERROR(e, DBGERR_NOTIMP, NULL);
                    err_rep =
                        NewGroupReply(di, bitset_dup(recv_ptr->rec_group), e);
                    // Add the error to the reply set
                    AddErrToSet(result, err_rep);
                } else {
                    // FIXME: How to handle error case?
                    res = DBG_PROTO_SW(di)->sw_exit(di, detach);
                }

                // cleanup the di object
                FreeDbgInvoke(di);
            }

            break;

        case DBG_DEFAULT:
            // Here the caller used the default quit type which is based on the
            // attach value in each di. So we need to loop over each di in the
            // rec_set individually since these can be different.
            SetReceiverHeadInSet(set);
            while ((recv_ptr = GetNextReceiverInSet(set)) != NULL) {
                // point at the di
                di = recv_ptr->rec_di;

                // create the rec_set
                to_send = NewRecSet();
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

                request = CommandToSocProto(DBGCMD_QUIT, detach);

                tmp_result = DbgSendCmdAndRecvReply(to_send, request, DBGEV_OK);

                Free(request);
                FreeRecSet(to_send);

                // merge the results
                result = MergeRepSet(result, tmp_result);

                // Call protocol specific communication network exit function
                if (DBG_PROTO_SW(di)->sw_exit == NULL) {
                    // Create a new error event
                    EVENT_ERROR(e, DBGERR_NOTIMP, NULL);
                    err_rep =
                        NewGroupReply(di, bitset_dup(recv_ptr->rec_group), e);
                    // Add the error to the reply set
                    AddErrToSet(result, err_rep);
                } else {
                    // FIXME: How to handle error case?
                    res = DBG_PROTO_SW(di)->sw_exit(di, detach);
                }

                // cleanup the di object
                FreeDbgInvoke(di);
            }

            break;
    }

    ResetSet(result);
    return result;
}
