/*********************************************************************************\
 * Process Set Routines.
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
static dbg_over_set_t *NewOverlaySet(dbginvoke_t *, bitset *, int);
static void FreeOverlaySet(dbg_over_set_t *);
static void AddOverlaySetToAllProcSet(dbg_over_set_t *);
static void _free_procset(dbgprocset_t *);
static void _print_with_set_info(const char *, const bitset *, int, int, int, int,
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
    DestroyList(ProcSets, (void (*)(void *))_free_procset);
}

/*
** Create a temporary process set from a slice
*/
dbgprocset_t *CreateTempProcSetSlice(dbgprocset_t *ps, slice_t *ps_slice) {
    char *ps_name;
    dbgprocset_t *new_procset;
    dbg_over_set_t *o_ptr;
    dbg_over_set_t *over_set;
    bitset *di_mask = NULL;
    int num_set;
    int i, index, bit, i_cnt;
    int ps_offset;

    if (ps == NULL || ps_slice == NULL) {
        PrintOutError("Error: CreateTempProcSetSlice had null args.\n");
        return NULL;
    }

    // Create the name for this temporary process set
    Asprintf(&ps_name, "%s_temp", ps->s_name);

    // create the new procset
    new_procset = NewProcSet(ps_name, DBGSET_TEMP, ps_slice->s_threads);

    DPRINT("CreateTempProcSetSlice name:%s\n", ps_name);

    // Loop over each overlay set in the procset
    SetOverlaySetForProcSet(ps);
    while ((o_ptr = GetNextOverlaySetForProcSet(ps)) != NULL) {
        // grab the procset for this invoke, dup it, and clear it
        di_mask = bitset_dup(o_ptr->s_di_bs);
        bitset_clear(di_mask);

        // Get the number of bits set to 1, this is the number of "ranks" found
        // in this di for the procset.
        num_set = bitset_numset(o_ptr->s_di_bs);

        // Now check the slice information to see if this di has any specified
        // ranks
        for (i = 0; i < ps_slice->s_rank; ++i) {
            // initialize the bit counter for this set, we need to ignore bits
            // in the bitset that are not set since these are not part of the
            // procset This could be more efficient, but I guess the slice
            // information is not guaranteed to be sorted.
            bit = 0;
            // This corresponds to the offset plus the number of on bits in the
            // bitset
            i_cnt = o_ptr->s_ps_offset;

            DPRINT("CreateTempProcSetSlice looping over slice:[%d..%d]\n",
                   ps_slice->s_low[i], ps_slice->s_high[i]);

            // Loop over the range of this rank in the slice
            for (index = ps_slice->s_low[i]; index <= ps_slice->s_high[i];
                 ++index) {
                // If the index is less than the offset of this o_ptr, we know
                // that the requested rank cannot be found here.
                if (index < o_ptr->s_ps_offset) break;

                // If the index is greater than the offset plus the number of
                // ranks set in the bitset, we know that the requested rank
                // cannot be found here.
                if (index > (o_ptr->s_ps_offset + num_set)) break;

                // advance i_cnt to index
                do {
                    // Ignore unset bits
                    while (!bitset_test(o_ptr->s_di_bs, bit)) ++bit;

                    DPRINT(
                        "CreateTempProcSetSlice bit[%d] i_cnt[%d] index[%d]\n",
                        bit, i_cnt, index);

                    // increment bit if i_cnt does not equal index, we want to
                    // skip over this bit
                    if (i_cnt != index) {
                        ++bit;
                    }

                    // only advance i_cnt, bit currently points to the bit in
                    // the set that corresponds to index if i_cnt == index
                } while (i_cnt == index ? 0 : ++i_cnt);

                // set this bit in the di_mask to 1, this corresponds to this
                // index
                bitset_set(di_mask, bit);
            }
        }

        // Ensure that this mask isn't empty, otherwise we don't want to add it
        if (bitset_isempty(di_mask)) {
            bitset_free(di_mask);
            DPRINT(
                "CreateTempProcSetSlice di $%s had an empty mask. Skipping.\n",
                o_ptr->s_di->d_handle);
            continue;
        }

        DPRINT(
            "CreateTempProcSetSlice Adding di $%s for mask[%s] to "
            "over_set_list.\n",
            o_ptr->s_di->d_handle, bitset_to_set(di_mask));

        // We now have a properly formed di_mask for this di. Lets create a new
        // over_set object for this di and add it to the list, note that we will
        // need to recompute the offset for this new temp set.
        over_set = NewOverlaySet(o_ptr->s_di, di_mask, 0);
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
        ps_offset += bitset_numset(o_ptr->s_di_bs);
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
    bitset *di_mask = NULL;
    dbg_over_set_t *over_set = NULL;
    dbg_over_set_t *o_ptr = NULL;
    int num_set;
    int i, index, bit, i_cnt;
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

        case rval_graph:
            PrintOutError("An assertion script named %s is already defined.\n",
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
                SetProcSetThreads(new_procset, ps_slice->s_threads);
            }

            while ((over_set = GetNextOverlaySetForProcSet(procset)) != NULL) {
                // grab the procset for this invoke, dup it
                di_mask = bitset_dup(over_set->s_di_bs);

                // Check to see if slicing information was provided
                if (ps_slice != NULL) {
                    DPRINT("CreateProcSet slicing information provided.\n");

                    // We need to compute the slice, start by clearing the
                    // di_mask
                    bitset_clear(di_mask);

                    // Get the number of bits set to 1, this is the number of
                    // "ranks" found in this di for the procset.
                    num_set = bitset_numset(over_set->s_di_bs);

                    // Now check the slice information to see if this di has any
                    // specified ranks
                    for (i = 0; i < ps_slice->s_rank; ++i) {
                        // initialize the bit counter for this set, we need to
                        // ignore bits in the bitset that are not set since
                        // these are not part of the procset This could be more
                        // efficient, but I guess the slice information is not
                        // guaranteed to be sorted.
                        bit = 0;
                        // This corresponds to the offset plus the number of on
                        // bits in the bitset
                        i_cnt = over_set->s_ps_offset;

                        DPRINT("CreateProcSet looping over slice:[%d..%d]\n",
                               ps_slice->s_low[i], ps_slice->s_high[i]);

                        // Loop over the range of this rank in the slice
                        for (index = ps_slice->s_low[i];
                             index <= ps_slice->s_high[i]; ++index) {
                            // If the index is less than the offset of this
                            // over_set, we know that the requested rank cannot
                            // be found here.
                            if (index < over_set->s_ps_offset) break;

                            // If the index is greater than the offset plus the
                            // number of ranks set in the bitset, we know that
                            // the requested rank cannot be found here.
                            if (index > (over_set->s_ps_offset + num_set))
                                break;

                            // advance i_cnt to index
                            do {
                                // Ignore unset bits
                                while (!bitset_test(over_set->s_di_bs, bit))
                                    ++bit;

                                DPRINT(
                                    "CreateProcSet bit[%d] i_cnt[%d] "
                                    "index[%d]\n",
                                    bit, i_cnt, index);

                                // increment bit if i_cnt does not equal index,
                                // we want to skip over this bit
                                if (i_cnt != index) {
                                    ++bit;
                                }

                                // only advance i_cnt, bit currently points to
                                // the bit in the set that corresponds to index
                                // if i_cnt == index
                            } while (i_cnt == index ? 0 : ++i_cnt);

                            // set this bit in the di_mask to 1, this
                            // corresponds to this index
                            bitset_set(di_mask, bit);
                            DPRINT("CreateProcSet found index at bit[%d]\n",
                                   bit);
                        }
                    }
                }

                // Ensure that this mask isn't empty, otherwise we don't want to
                // add it
                if (bitset_isempty(di_mask)) {
                    bitset_free(di_mask);
                    DPRINT(
                        "CreateProcSet di $%s had an empty mask. Skipping.\n",
                        o_ptr->s_di->d_handle);
                    continue;
                }

                // If slicing information wasn't provided, the user specified
                // the whole set. In that case we want to use the entire bitset
                // "as is".

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
                    bitset_oreq(o_ptr->s_di_bs, di_mask);
                    bitset_free(di_mask);
                    DPRINT(
                        "CreateProcSet Updated existing o_ptr for di $%s to "
                        "new mask[%s].\n",
                        o_ptr->s_di->d_handle, bitset_to_set(o_ptr->s_di_bs));
                } else {
                    // Create a new overlay set for this invoke
                    // Note that the offset gets updated on the second pass
                    o_ptr = NewOverlaySet(over_set->s_di, di_mask, 0);
                    AddToList(new_procset->s_over_set_list, o_ptr);
                    // increment num sets
                    new_procset->s_num_sets++;
                    DPRINT(
                        "CreateProcSet Added di $%s for mask[%s] to "
                        "over_set_list.\n",
                        o_ptr->s_di->d_handle, bitset_to_set(di_mask));
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
        ps_offset += bitset_numset(o_ptr->s_di_bs);
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
dbgprocset_t *CreateProcSetForInvoke(expr_t *e) {
    val_t *val;
    dbgprocset_t *rtn;
    slice_t *proc_slice;

    if (EvalExpr(&val, e, NULL) < 0) {
        PrintError();
        return NULL;
    }

    if (LVAL_TYPE(val) != lval_internal) {
        PrintOutError("Must specify debugger variable for process handle.\n");
        return NULL;
    }

    switch (RVAL_ACTUAL_TYPE(val)) {
        case rval_graph:
            PrintOutError(
                "An assertion script has already been defined using this "
                "name.\n");
            return NULL;

        case rval_proc:
            PrintOutError(
                "A process set has already been defined using this name.\n");
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

        if (proc_slice->s_rank > 1 ||
            (proc_slice->s_low[0] != proc_slice->s_high[0])) {
            PrintOutError(
                "Invalid proc handle syntax. Expected $handle{n} where n is "
                "the number of application ranks.\n");
            FreeProcSet(rtn);
            RemoveVal(LVAL_SYM(val));
            DestroyVal(val);
            return NULL;
        }

        rtn->s_nproc = proc_slice->s_high[0];
        SetProcSetThreads(rtn, proc_slice->s_threads);
        

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

    over_set = NewOverlaySet(di, bitset_dup(di->d_bitset_full), 0);
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
rec_set_t *CreateRecSetForProcset(dbgprocset_t *ps) {
    rec_set_t *res;
    dbg_over_set_t *over_set;

    if (ps == NULL) return NULL;

    // create an empty rec_set
    res = NewRecSet();

    // loop over each over_set in the procset
    SetOverlaySetForProcSet(ps);
    while ((over_set = GetNextOverlaySetForProcSet(ps)) != NULL) {
        // create a group_recv_t for the over_set and add it to the res
        AddReceiverToSet(res,
                         NewGroupReceiver(over_set->s_di, over_set->s_di_bs));
    }

    return res;
}

static dbgprocset_t *NewProcSet(const char *name, dbgset_type type, const char* threads) {
    dbgprocset_t *s;

    s = (dbgprocset_t *)Alloc(sizeof(dbgprocset_t));
    s->s_name = strdup(name);
    s->s_type = type;
    s->s_nproc = 0;
    s->s_num_sets = 0;
    s->s_over_set_list = NewList();
    s->s_threads = (threads ? strdup(threads) : nullptr);

    AddToList(ProcSets, (void *)s);

    return s;
}

static dbg_over_set_t *NewOverlaySet(dbginvoke_t *di, bitset *bs, int offset) {
    dbg_over_set_t *set = (dbg_over_set_t *)Alloc(sizeof(dbg_over_set_t));
    memset(set, 0, sizeof(dbg_over_set_t));

    set->s_di = di;
    set->s_di_bs = bs;
    set->s_ps_offset = offset;

    return set;
}

static void FreeOverlaySet(dbg_over_set_t *set) {
    set->s_di = NULL;
    bitset_free(set->s_di_bs);
    Free(set);
}

static void AddOverlaySetToAllProcSet(dbg_over_set_t *over_set) {
    AddToList(AllProcSet->s_over_set_list, over_set);
    AllProcSet->s_nproc += bitset_numset(over_set->s_di_bs);
}

static void _free_procset(dbgprocset_t *s) {
    // FIXME: Destroy the val!!!
    DestroyList(s->s_over_set_list, (void (*)(void *))FreeOverlaySet);

    Free(s->s_name);
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
                AllProcSet->s_nproc -= bitset_numset(over_set->s_di_bs);
            }
            break;
        case DBGSET_DEFSET:
        case DBGSET_TEMP:
        case DBGSET_INTERNAL:
            // do nothing
            break;
    }

    RemoveFromList(ProcSets, (void *)s);

    _free_procset(s);
}

// TODO: Remove
dbgprocset_t *FindProcSet(char *name) {
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
    char *setinfo;

    if (EmptyList(s->s_over_set_list)) {
        PrintOutMsg("none");
        return;
    }

    SetList(s->s_over_set_list);

    // Here we want to print the "actual" ranks in the procset, not the "offset"
    // ranks.
    while ((over_set = (dbg_over_set_t *)GetListElement(s->s_over_set_list)) !=
           NULL) {
        setinfo = bitset_to_set(over_set->s_di_bs);

        PrintOutMsg("%s", over_set->s_di->d_handle);

        if (setinfo != NULL) {
            PrintOutMsg("%s", setinfo);
            Free(setinfo);
            setinfo = NULL;
        }

        n += bitset_numset(over_set->s_di_bs);
        if (n < s->s_nproc) PrintOutMsg(",");
    }
    if (s->s_threads) {
        PrintOutMsg(":%s", s->s_threads);
    }
}

void SetOverlaySetForProcSet(dbgprocset_t *s) { SetList(s->s_over_set_list); }

dbg_over_set_t *GetNextOverlaySetForProcSet(dbgprocset_t *s) {
    dbg_over_set_t *set = (dbg_over_set_t *)GetListElement(s->s_over_set_list);
    return set;
}

static void _print_with_set_info(const char *setname, const bitset *group,
                                 int isOffset, int offset, int len, int isErr,
                                 const char *f, va_list ap) {
    char *set_info = NULL;

    if (isOffset) {
        if (offset >= 0 && len > 0) {
            set_info = bitset_to_set_by_offset(group, offset, len);
        }
    }

    if (set_info == NULL) {
        set_info = bitset_to_set(group);
    }

    if (setname != NULL && set_info != NULL) {
        if (isErr) {
            PrintOutError("%s%s: ", setname, set_info);
        } else {
            PrintOutMsg("%s%s: ", setname, set_info);
        }
    }

    if (isErr) {
        vPrintOutError(f, ap);
    } else {
        vPrintOutMsg(f, ap);
    }

    Free(set_info);
}

void PrintMsgWithSetInfo(const char *setname, const bitset *group, const char *f,
                         ...) {
    va_list ap;

    // setup the va_list
    va_start(ap, f);

    _print_with_set_info(setname, group, 0, 0, 0, 0, f, ap);

    // end the va_list
    va_end(ap);
}

void PrintErrorWithSetInfo(const char *setname, const bitset *group, const char *f,
                           ...) {
    va_list ap;

    // setup the va_list
    va_start(ap, f);

    _print_with_set_info(setname, group, 0, 0, 0, 1, f, ap);

    // end the va_list
    va_end(ap);
}
