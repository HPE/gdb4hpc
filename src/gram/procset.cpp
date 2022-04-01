/*********************************************************************************\
 * Process Set Routines.
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "client_settings.h"
#include "dbg_client.h"
#include "dbgevent.h"
#include "defs.h"
#include "gram.h"
#include "procset.h"
#include "useful.h"

// static prototypes
static dbgprocset_t *NewProcSet(const char *, dbgset_type, const char* threads);
static dbg_over_set_t *NewOverlaySet(dbginvoke_t *, std::unique_ptr<Bitset>&&, int);
static void FreeOverlaySet(dbg_over_set_t *);
static void AddOverlaySetToAllProcSet(dbg_over_set_t *);
static void _free_procset(dbgprocset_t *);
static void _print_with_set_info(const char *, const Bitset *, const char* threads, bool,
                                 const char *, va_list);

// Internal variables
static list_t *ProcSets = NULL;  // TODO: Try to get rid of this
static dbgprocset_t *CurrProcSet = NULL;
static dbgprocset_t *AllProcSet = NULL;

void InitProcSet(void) {
    expr_t *ex;
    val_t *val;

    ProcSets = NewList();

    // force all procset to be created
    if ((ex = ParseExpr("$all")) == NULL) {
        PrintError();
        return;
    }

    if (EvalExpr(&val, ex, NULL) < 0) {
        PrintError();
        FreeExpr(ex);
        return;
    }

    FreeExpr(ex);

    AllProcSet = NewProcSet("all", DBGSET_INTERNAL, nullptr);
    SetCurrentProcSet(AllProcSet);

    // Fixup the val
    RVAL_TYPE(val) = rval_proc;
    // Assign the procset directly
    RVAL_PROCSET(val) = AllProcSet;
    // Important! Set RVAL_DESTROY to false
    RVAL_DESTROY(val) = 0;
}

void FinishProcSet(void) {
    auto procSets = ProcSets;
    ProcSets = nullptr;
    DestroyList(procSets, (void (*)(void *))_free_procset);
}

// Returns a newly allocated bitset which is the passed \c slice applied to 
// the bitset in \c overlay_set. Only set bits are considered for the slice. 
// For example, given the bitset 111100001111, the slice {3..5} (zero indexed, 
// least significant bit is zero) would return 001100001000. The caller is 
// responsible for freeing the returned bitset. If the passed \c slice is null,
// a duplicate bitset will still be allocated.
//
// \param overlay_set: The overlay set containing the bitset the new bitset will 
//                     be derived from
//      \param *slice: The slice applied to \a bitset. If null, a duplicate of 
//                     \a bitset is returned.
std::unique_ptr<Bitset> sliceBitset(const dbg_over_set_t &overlay_set, const slice_t *slice) {
    const auto &bits = overlay_set.s_di_bs;
    if (bits == nullptr) return nullptr;

    auto result = bits->clone();
    if (slice == nullptr || slice->getRank() == 0) return result; // no slice requested

    result->clear();

    // get nth set bit in bits, zero indexed, beginning from start
    auto nthSetBit = [](const auto& bits, int n, unsigned int start = 0) {
        auto ret = start;

        // skip all unset bits and n set bits
        for (; ret < bits->sizeInBits(); ++ret) {
            if (bits->test(ret) && !n--) {
                break;
            }
        }

        return ret;
    };

    // check that the requested slice index is actually contained in this overlay set
    auto validIndex = [](int index, const auto& oSet) {
        return
            index >= oSet.s_ps_offset && index < oSet.s_ps_offset + static_cast<int>(oSet.s_di_bs->numSet());
    };

    for (int i = 0; i < slice->getRank(); ++i) {
        int startBit = nthSetBit(bits, slice->getLow(i) - overlay_set.s_ps_offset);

        // pick first n set bits where n = getHigh - getLow + 1 (slice ranges are inclusive)
        for (int j = slice->getLow(i); j <= slice->getHigh(i) && validIndex(j, overlay_set); ++j) {
            auto nextBit = nthSetBit(bits, 0, startBit);
            if (nextBit >= result->sizeInBits()) break;

            result->set(nextBit);
            startBit = nextBit + 1;
        }
    }

    return result;
}

void forEachOverlaySet(const dbgprocset_t& procset, std::function<void(const dbg_over_set_t* os)> fn) {
    // would make a good password given its length...
    const auto& GNOSFPS = GetNextOverlaySetForProcSet;

    // XXX: procset parameter should be treated as const, but c-style list
    //      manipulators are technically not const (changing iterator pointers etc)
    auto& rawProcset = const_cast<dbgprocset_t&>(procset);

    SetOverlaySetForProcSet(&rawProcset);
    for (auto* os = GNOSFPS(&rawProcset); os; os = GNOSFPS(&rawProcset)) {
        fn(os);
    }
}

/*
** Create a temporary process set from a slice
*/
dbgprocset_t *CreateTempProcSetSlice(dbgprocset_t *ps, slice_t *ps_slice) {
    char *ps_name;
    dbgprocset_t *new_procset;
    dbg_over_set_t *o_ptr;
    dbg_over_set_t *over_set;
    int ps_offset;

    if (ps == NULL || ps_slice == NULL) {
        PrintOutError("Error: CreateTempProcSetSlice had null args.\n");
        return NULL;
    }

    // Create the name for this temporary process set
    Asprintf(&ps_name, "%s_temp", ps->s_name);

    // create the new procset
    new_procset = NewProcSet(ps_name, DBGSET_TEMP, ps_slice->getThreads());

    DPRINT("CreateTempProcSetSlice name:%s\n", ps_name);

    // Loop over each overlay set in the procset
    SetOverlaySetForProcSet(ps);
    while ((o_ptr = GetNextOverlaySetForProcSet(ps)) != NULL) {
        auto di_mask = sliceBitset(*o_ptr, ps_slice);

        // Ensure that this mask isn't empty, otherwise we don't want to add it
        if (di_mask->isEmpty()) {
            DPRINT(
                "CreateTempProcSetSlice di $%s had an empty mask. Skipping.\n",
                o_ptr->s_di->d_handle);
            continue;
        }

        DPRINT(
            "CreateTempProcSetSlice Adding di $%s for mask[%s] to "
            "over_set_list.\n",
            o_ptr->s_di->d_handle, di_mask->toSetString().c_str());

        // We now have a properly formed di_mask for this di. Lets create a new
        // over_set object for this di and add it to the list, note that we will
        // need to recompute the offset for this new temp set.
        over_set = NewOverlaySet(o_ptr->s_di, std::move(di_mask), 0);
        AddToList(new_procset->s_over_set_list, over_set);
        // increment num sets
        new_procset->s_num_sets++;
    }

    // Second pass - need to update the offsets for each overlay set
    ps_offset = 0;
    SetOverlaySetForProcSet(new_procset);
    while ((o_ptr = GetNextOverlaySetForProcSet(new_procset)) != NULL) {
        DPRINT("CreateTempProcSetSlice offset[%d] for di $%s.\n", ps_offset,
               o_ptr->s_di->d_handle);
        o_ptr->s_ps_offset = ps_offset;
        ps_offset += o_ptr->s_di_bs->numSet();
    }

    // Update the nproc in the procset
    new_procset->s_nproc = ps_offset;

    DPRINT("CreateTempProcSetSlice ranks[%d] in temp procset.\n", ps_offset);

    return new_procset;
}

/*
** Create a process set from a procset list.
*/
int CreateProcSet(expr_t *e_name, expr_t *e_set) {
    val_t *v_name = NULL;
    dbgprocset_t *new_procset;
    list_t *l;
    val_t *v;
    dbgprocset_t *procset = NULL;
    dbg_over_set_t *over_set = NULL;
    dbg_over_set_t *o_ptr = NULL;
    slice_t *ps_slice;
    int ps_offset = 0;

    if (e_name == NULL || e_set == NULL) {
        PrintOutError("Error: Create Procset had null args.\n");
        return 0;
    }

    // Eval the name and make sure it is acceptable.
    // After calling EvalExpr on the new procset, we need to call RemoveVal
    // to ensure proper cleanup if something goes wrong and this creates a new
    // dbg variable for us. We only do this if EvalExpr returns a rval_none val
    // meaning that there was not a name collision.
    if (EvalExpr(&v_name, e_name, NULL) < 0) {
        PrintError();
        return 0;
    }

    if (LVAL_TYPE(v_name) != lval_internal) {
        PrintOutError("Must specify debugger variable for process handle.\n");
        return 0;
    }

    switch (RVAL_ACTUAL_TYPE(v_name)) {
        case rval_ref:
            // This shouldn't be handled technically since we are using
            // RVAL_ACTUAL_TYPE
        case rval_none:
            // This is what we expect the val to be
            break;

        case rval_aif:
            PrintOutError("A debugger variable named %s is already defined.\n",
                          SymLookup(LVAL_SYM(v_name)));
            return 0;

        case rval_proc:
            PrintOutError("A procset named %s is already defined.\n",
                          SymLookup(LVAL_SYM(v_name)));
            return 0;

        case rval_blockmap:
            PrintOutError("A decomposition named %s is already defined.\n",
                          SymLookup(LVAL_SYM(v_name)));
            return 0;

        case rval_reply:
            PrintOutError(
                "An application variable symbol named %s already exists.\n",
                SymLookup(LVAL_SYM(v_name)));
            return 0;
    }

    // create the new procset
    new_procset = NewProcSet(SymLookup(LVAL_SYM(v_name)), DBGSET_DEFSET, nullptr);

    // Now eval the provided lists of procsets
    if (EvalExprList(&l, e_set, NULL) < 0) {
        PrintOutError("Error: Procset eval expr failed.\n");
        FreeProcSet(new_procset);
        RemoveVal(LVAL_SYM(v_name));
        DestroyVal(v_name);
        return 0;
    }

    SetList(l);
    while ((v = (val_t *)GetListElement(l)) != NULL) {
        // reset the slicing information
        ps_slice = NULL;

        if (RVAL_ACTUAL_TYPE(v) != rval_proc) {
            PrintOutError("Invalid process set argument.\n");
            DestroyList(l, NULL);
            FreeProcSet(new_procset);
            RemoveVal(LVAL_SYM(v_name));
            DestroyVal(v_name);
            return 0;
        }

        // assign procset directly
        procset = RVAL_ACTUAL_PROCSET(v);

        if (RVAL_TYPE(v) == rval_ref) {
            ps_slice = RVAL_REF_SLICE(v);
        }

        if (procset != NULL) {
            DPRINT("CreateProcSet name:%s\n", procset->s_name);

            // Loop over each overlay set in the procset
            SetOverlaySetForProcSet(procset);
            if (ps_slice) {
                SetProcSetThreads(new_procset, ps_slice->getThreads());
            }

            while ((over_set = GetNextOverlaySetForProcSet(procset)) != NULL) {
                auto di_mask = sliceBitset(*over_set, ps_slice);

                // Ensure that this mask isn't empty, otherwise we don't want to
                // add it
                if (di_mask->isEmpty()) {
                    DPRINT(
                        "CreateProcSet di $%s had an empty mask. Skipping.\n",
                        o_ptr->s_di->d_handle);
                    continue;
                }

                // Check to see if an overlay set already exists for this di in
                // the new_procset
                SetOverlaySetForProcSet(new_procset);
                while ((o_ptr = GetNextOverlaySetForProcSet(new_procset)) !=
                       NULL) {
                    if (o_ptr->s_di == over_set->s_di) {
                        // found an existing overlay set for this di
                        break;
                    }
                }

                // Update the existing overlay set if found
                if (o_ptr != NULL) {
                    *o_ptr->s_di_bs |= *di_mask;
                    DPRINT(
                        "CreateProcSet Updated existing o_ptr for di $%s to "
                        "new mask[%s].\n",
                        o_ptr->s_di->d_handle, o_ptr->s_di_bs->toSetString().c_str());
                } else {
                    // Create a new overlay set for this invoke
                    // Note that the offset gets updated on the second pass
                    DPRINT(
                        "CreateProcSet is about to add di $%s for mask[%s] to "
                        "over_set_list.\n",
                        o_ptr->s_di->d_handle, di_mask->toSetString().c_str());
                    o_ptr = NewOverlaySet(over_set->s_di, std::move(di_mask), 0);
                    AddToList(new_procset->s_over_set_list, o_ptr);
                    // increment num sets
                    new_procset->s_num_sets++;
                }
            }
        } else {
            PrintOutError("ERROR: ParseProcset cannot get procset\n");
            DestroyList(l, NULL);
            FreeProcSet(new_procset);
            RemoveVal(LVAL_SYM(v_name));
            DestroyVal(v_name);
            return 0;
        }
    }

    // Free the list from the eval
    DestroyList(l, NULL);

    // If s_num_sets is 0, then the user provided a completely invalid list.
    // lets catch that case.
    if (new_procset->s_num_sets == 0) {
        PrintOutError(
            "ERROR: No valid ranks detected in process set argument.\n");
        FreeProcSet(new_procset);
        RemoveVal(LVAL_SYM(v_name));
        DestroyVal(v_name);
        return 0;
    }

    // Second pass - need to update the offsets for each overlay set
    ps_offset = 0;
    SetOverlaySetForProcSet(new_procset);
    while ((o_ptr = GetNextOverlaySetForProcSet(new_procset)) != NULL) {
        DPRINT("CreateProcSet offset[%d] for di $%s.\n", ps_offset,
               o_ptr->s_di->d_handle);
        o_ptr->s_ps_offset = ps_offset;
        ps_offset += o_ptr->s_di_bs->numSet();
    }

    // Update the nproc in the procset
    new_procset->s_nproc = ps_offset;

    DPRINT("CreateProcSet ranks[%d] in new procset.\n", ps_offset);

    // Fixup the v_name val
    RVAL_TYPE(v_name) = rval_proc;
    // Assign the procset directly
    RVAL_PROCSET(v_name) = new_procset;
    // Important! Set RVAL_DESTROY to false
    RVAL_DESTROY(v_name) = 0;

    return 1;
}

/*
** Create an invoke procset given an internal variable with rval_none. This
*internal
** variable can contain slicing information. This is the first step for
*launching
** or attaching. Use this to determine the handle name and number of ranks.
**
** This should be extended to support multi-dimensional procsets with the slice.
** XXX: This only works for a single handle (i.e. $a), not a list of handles.
*/
dbgprocset_t *CreateProcSetForInvoke(expr_t *e, dbgprocset_t* &existingProcSet) {
    val_t *val;
    dbgprocset_t *rtn;
    slice_t *proc_slice;

    existingProcSet = nullptr;

    if (EvalExpr(&val, e, NULL) < 0) {
        PrintError();
        return NULL;
    }

    if (LVAL_TYPE(val) != lval_internal) {
        PrintOutError("Must specify debugger variable for process handle.\n");
        return NULL;
    }

    switch (RVAL_ACTUAL_TYPE(val)) {
        case rval_proc:
            existingProcSet = RVAL_ACTUAL_PROCSET(val);
            return NULL;

        case rval_blockmap:
            PrintOutError(
                "A decomposition has already been defined using this name.\n");
            return NULL;

        case rval_aif:
            PrintOutError(
                "A debugger variable has already been defined using this "
                "name.\n");
            return NULL;

        case rval_reply:
            PrintOutError(
                "A reply has already been defined using this name.\n");
            return NULL;

        case rval_ref:  // This shouldn't happen since we called actual type
        case rval_none:
            // we expect the type to be rval_none
            break;
    }

    // create the new procset
    rtn = NewProcSet(SymLookup(LVAL_SYM(val)), DBGSET_INVOKE, nullptr);

    // Calculate the number of ranks in the provided list
    if (RVAL_TYPE(val) == rval_ref) {
        // Note that we only expect the user to be providing us with a slice
        // without range, that is $a{n} where n is the number of ranks. This
        // syntax is unique to launch only.
        proc_slice = RVAL_REF_SLICE(val);

        if (proc_slice->getRank() > 1 ||
            (proc_slice->getLow(0) != proc_slice->getHigh(0))) {
            PrintOutError(
                "Invalid proc handle syntax. Expected $handle{n} where n is "
                "the number of application ranks.\n");
            FreeProcSet(rtn);
            RemoveVal(LVAL_SYM(val));
            DestroyVal(val);
            return NULL;
        }

        rtn->s_nproc = proc_slice->getHigh(0);
        SetProcSetThreads(rtn, proc_slice->getThreads());
        

    } else {
        // Always default to one rank
        rtn->s_nproc = 1;
    }

    // We are going to turn this debugger variable into a procset rval. Destroy
    // the old value, including discarding any slicing info.
    DestroyRval(RVAL_TYPE(val), RVAL_VAL(val));
    // Now turn the rval into an rval_proc
    RVAL_TYPE(val) = rval_proc;
    // Set the procset in the val
    RVAL_PROCSET(val) = rtn;
    // Important! Set RVAL_DESTROY to false since it will be true here since
    // the original rval was a ref type
    RVAL_DESTROY(val) = 0;

    rtn->s_init = 0;

    return rtn;
}

/*
** Finish setting up the procset after the application has finished
** launching/attaching in the dbg client layer.
*/
int FinalizeProcSetForInvoke(dbgprocset_t *ps, dbginvoke_t *di) {
    dbg_over_set_t *over_set = NULL;

    if (ps == NULL) {
        PrintOutError("Invalid procset_t in SetupInvoke()\n");
        return 1;
    }

    if (ps->s_init) {
        PrintOutError("Procset already initialized.\n");
        return 1;
    }

    // reconcile the nprocs with the di, it is our master
    ps->s_nproc = di->d_nproc;

    over_set = NewOverlaySet(di, di->d_bitset_full->clone(), 0);
    AddToList(ps->s_over_set_list, over_set);
    // increment num sets
    ps->s_num_sets++;

    AddOverlaySetToAllProcSet(over_set);

    ps->s_init = 1;

    return 0;
}

/*
** Call this function to create a rec_set_t for a procset that is suitable for
** passing down to the dbg_client layer.
*/
rec_set_t *CreateRecSetForProcset(const dbgprocset_t *ps) {

    if (ps == NULL) return NULL;

    // create an empty rec_set
    rec_set_t* res = NewRecSet();

    {   // for each overlay set, add the bitset to the recset

        // passed procset should be treated as const, but to iterate over its
        // overlay set list, we need to reset/increment the list iterators
        auto listHandle = const_cast<dbgprocset_t*>(ps);
        
        SetOverlaySetForProcSet(listHandle);
        while (const dbg_over_set_t* over_set = GetNextOverlaySetForProcSet(listHandle)) {
            // create a group_recv_t for the over_set and add it to the res
            AddReceiverToSet(res, NewGroupReceiver(over_set->s_di, over_set->s_di_bs.get()));
        }
    }

    if (ps->s_threads) {
        res->setThreads(ps->s_threads);
    }

    return res;
}

static dbgprocset_t *NewProcSet(const char *name, dbgset_type type, const char* threads) {
    dbgprocset_t *s;

    s = (dbgprocset_t *)Alloc(sizeof(dbgprocset_t));
    s->s_name = strdup(name);
    s->s_type = type;
    s->s_nproc = 0;
    s->s_init = 0;
    s->s_num_sets = 0;
    s->s_over_set_list = NewList();
    s->s_threads = (threads ? strdup(threads) : nullptr);

    AddToList(ProcSets, (void *)s);

    return s;
}

static dbg_over_set_t *NewOverlaySet(dbginvoke_t *di, std::unique_ptr<Bitset> &&bs, int offset) {
    auto *set = new dbg_over_set_t;

    set->s_di = di;
    set->s_di_bs = std::move(bs);
    set->s_ps_offset = offset;

    return set;
}

static void FreeOverlaySet(dbg_over_set_t *set) {
    set->s_di = NULL;
    delete set;
}

static void AddOverlaySetToAllProcSet(dbg_over_set_t *over_set) {
    AddToList(AllProcSet->s_over_set_list, over_set);
    AllProcSet->s_nproc += over_set->s_di_bs->numSet();
}

static void _free_procset(dbgprocset_t *s) {
    // FIXME: Destroy the val!!!
    DestroyList(s->s_over_set_list, (void (*)(void *))FreeOverlaySet);
    s->s_over_set_list = NULL;

    Free(s->s_name);
    s->s_name = NULL;
    Free(s);
}

// Change this to only be used in val, if the val gets destroyed so does this.
void FreeProcSet(dbgprocset_t *s) {
    dbg_over_set_t *over_set;

    if (s == NULL) return;

    switch (s->s_type) {
        case DBGSET_INVOKE:
            SetOverlaySetForProcSet(s);
            while ((over_set = GetNextOverlaySetForProcSet(s)) != NULL) {
                RemoveFromList(AllProcSet->s_over_set_list, over_set);
                AllProcSet->s_nproc -= over_set->s_di_bs->numSet();
            }
            break;
        case DBGSET_DEFSET:
        case DBGSET_TEMP:
        case DBGSET_INTERNAL:
            // do nothing
            break;
    }

    RemoveFromList(ProcSets, (void *)s);
    if (auto sym = FindSym(s->s_name)) {
        RemoveSym(sym);
    }
    if (auto val = FindVal(s->s_name)) {
        // Fixme - this can leak the value
        RemoveVal(val);
    }
        
    _free_procset(s);
}

dbgprocset_t *FindProcSet(const char *name) {
    dbgprocset_t *s;

    if (ProcSets == (list_t *)NULL) return (dbgprocset_t *)NULL;

    SetList(ProcSets);

    while ((s = (dbgprocset_t *)GetListElement(ProcSets)) !=
           (dbgprocset_t *)NULL)
        if (strcmp(s->s_name, name) == 0) return s;

    return (dbgprocset_t *)NULL;
}

bool CheckProcSet(hentry_t* sym) {
    dbgprocset_t* p = FindProcSet(SymLookup(sym));
    return (p != NULL);
}

void SetCurrentProcSet(dbgprocset_t *s) {
    // If the CurrProcSet is a temporary procset, we want to destroy it
    if (CurrProcSet != NULL) {
        switch (CurrProcSet->s_type) {
            case DBGSET_TEMP:
                FreeProcSet(CurrProcSet);
                break;

            case DBGSET_INVOKE:
            case DBGSET_DEFSET:
            case DBGSET_INTERNAL:
                // do nothing
                break;
        }
    }

    CurrProcSet = s;
}

void SetProcSetThreads(dbgprocset_t* procSet, const char* threads) {
    char* t = (threads ? strdup(threads) : nullptr);
    std::swap(procSet->s_threads, t);
    free(t);
}

dbgprocset_t *CurrentProcSet(void) { return CurrProcSet; }

void SetProcSet(void) { SetList(ProcSets); }

dbgprocset_t *GetProcSet(void) {
    return (dbgprocset_t *)GetListElement(ProcSets);
}

void PrintProcSet(dbgprocset_t *s) {
    int n = 0;
    dbg_over_set_t *over_set;

    if (EmptyList(s->s_over_set_list)) {
        PrintOutMsg("none");
        return;
    }

    SetList(s->s_over_set_list);

    // Here we want to print the "actual" ranks in the procset, not the "offset"
    // ranks.
    while ((over_set = (dbg_over_set_t *)GetListElement(s->s_over_set_list)) !=
           NULL) {
        PrintOutMsg("%s", over_set->s_di->d_handle);

        std::string si = over_set->s_di_bs->toSetString();
        if (s->s_threads) {
            // insert the thread info
            std::string setInfo{si};
            auto loc = setInfo.rfind('}');
            if (loc != std::string::npos) {
                setInfo = setInfo.substr(0,loc) + ":" + s->s_threads + "}";
            }
            PrintOutMsg("%s", setInfo.c_str());
        } else {
            PrintOutMsg("%s", si.c_str());
        }

        n += over_set->s_di_bs->numSet();
        if (n < s->s_nproc) PrintOutMsg(",");
    }
}

void SetOverlaySetForProcSet(dbgprocset_t *s) { SetList(s->s_over_set_list); }

dbg_over_set_t *GetNextOverlaySetForProcSet(dbgprocset_t *s) {
    dbg_over_set_t *set = (dbg_over_set_t *)GetListElement(s->s_over_set_list);
    return set;
}

std::string GetProcSetString(const char *setname, const Bitset *group, const char* threads) {
    if (!setname) return "";

    auto info = setname + group->toSetString();
    if (threads && threads[0]) {
        // insert the thread info before the end bracket
        auto siLen = info.size();
        if (siLen > 0 && info[siLen-1] == '}') {
            info[info.size() - 1] = ':';
            info += threads;
            info += '}';
        }
    }

    return info;
}    

static void _print_with_set_info(const char *setname, const Bitset *group,
                                 const char* threads, bool isErr,
                                 const char *f, va_list ap) {
    if (auto set_info = GetProcSetString(setname, group , threads); !set_info.empty()) {
        if (isErr) {
            PrintOutError("%s: ", set_info.c_str());
        } else {
            PrintOutMsg("%s: ", set_info.c_str());
        }
    }

    if (isErr) {
        vPrintOutError(f, ap);
    } else {
        vPrintOutMsg(f, ap);
    }
}

void PrintMsgWithSetInfo(const char *setname, const Bitset *group, const char *f,
                         ...) {
    va_list ap;

    // setup the va_list
    va_start(ap, f);

    _print_with_set_info(setname, group, nullptr, false, f, ap);

    // end the va_list
    va_end(ap);
}

void PrintMsgWithSetAndThreadInfo(const char *setname, const Bitset* group,
                                  const char* threads, const char* f, ...)
{
    va_list ap;

    // setup the va_list
    va_start(ap, f);

    _print_with_set_info(setname, group, threads, false, f, ap);

    // end the va_list
    va_end(ap);
}

void PrintErrorWithSetInfo(const char *setname, const Bitset *group, const char *f,
                           ...) {
    va_list ap;

    // setup the va_list
    va_start(ap, f);

    _print_with_set_info(setname, group, nullptr, true, f, ap);

    // end the va_list
    va_end(ap);
}

void PrintErrorWithSetAndThreadInfo(const char *setname, const Bitset *group, const char* threads,
                                    const char *f, ...) {
    va_list ap;

    // setup the va_list
    va_start(ap, f);

    _print_with_set_info(setname, group, threads, true, f, ap);

    // end the va_list
    va_end(ap);
}

