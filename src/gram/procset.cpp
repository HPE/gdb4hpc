/*********************************************************************************\
 * Process Set Routines.
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex>

#include "client_settings.h"
#include "dbg_client.h"
#include "dbgevent.h"
#include "defs.h"
#include "gram.h"
#include "procset.h"
#include "useful.h"

// static prototypes
static dbgprocset_t *NewProcSet(const char *, dbgset_type, const char* threads);
static dbg_over_set_t *NewOverlaySet(DbgInvoke::Handle, std::unique_ptr<Bitset>&&, int);
static void AddOverlaySetToAllProcSet(dbg_over_set_t *);
static void _print_with_set_info(const char *, const Bitset *, const char* threads, bool,
                                 const char *, va_list);

// Internal variables
static std::vector<std::shared_ptr<dbgprocset_t>> ProcSets;
static dbgprocset_t *CurrProcSet = nullptr;
static dbgprocset_t *AllProcSet = nullptr;

void InitProcSet(void) {
    AllProcSet = NewProcSet("all", DBGSET_INTERNAL, nullptr);
    SetCurrentProcSet(AllProcSet);
}

void FinishProcSet(void) {
    ProcSets.clear();
}

const std::vector<std::shared_ptr<dbgprocset_t>>& GetProcSets() {
    return ProcSets;
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

DbgInvoke::Handle getFirstDbgInvoke(const dbgprocset_t& procset) {
    return procset.s_over_sets.empty() ? nullptr : procset.s_over_sets[0]->s_di;
}

/*
** Create a temporary process set from a slice
*/
dbgprocset_t *CreateTempProcSetSlice(dbgprocset_t *ps, slice_t *ps_slice) {
    dbgprocset_t *new_procset;
    dbg_over_set_t *over_set;
    int ps_offset;

    if (ps == NULL || ps_slice == NULL) {
        PrintOutError("Error: CreateTempProcSetSlice had null args.\n");
        return NULL;
    }

    // Create the name for this temporary process set
    auto ps_name = std::string{ps->s_name} + "_temp";

    // create the new procset
    new_procset = NewProcSet(ps_name.c_str(), DBGSET_TEMP, ps_slice->getThreads());

    DPRINT("CreateTempProcSetSlice name:%s\n", ps_name.c_str());


    // Loop over each overlay set in the procset
    for (auto o_ptr : ps->s_over_sets) {
        auto di_mask = sliceBitset(*o_ptr, ps_slice);

        // Ensure that this mask isn't empty, otherwise we don't want to add it
        if (di_mask->isEmpty()) {
            DPRINT(
                "CreateTempProcSetSlice di $%s had an empty mask. Skipping.\n",
                o_ptr->s_di.c_str());
            continue;
        }

        DPRINT(
            "CreateTempProcSetSlice Adding di $%s for mask[%s] to "
            "over_set_list.\n",
            o_ptr->s_di.c_str(), di_mask->toSetString().c_str());

        // We now have a properly formed di_mask for this di. Lets create a new
        // over_set object for this di and add it to the list, note that we will
        // need to recompute the offset for this new temp set.
        over_set = NewOverlaySet(o_ptr->s_di, std::move(di_mask), 0);
        new_procset->s_over_sets.push_back(over_set);
    }

    // Second pass - need to update the offsets for each overlay set
    ps_offset = 0;
    for (auto o_ptr : new_procset->s_over_sets) {
        DPRINT("CreateTempProcSetSlice offset[%d] for di $%s.\n", ps_offset,
               o_ptr->s_di.c_str());
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
dbgprocset_t* CreateProcSet(const std::string& name, const gram::Expression& set) {

    if (!IdMap::idIsAvailable(name,true)) return nullptr;

    std::vector<std::pair<DbgInvoke::Handle,std::unique_ptr<Bitset>>> psSet;

    std::vector<const gram::Expression*> eset;

    if (auto list = dynamic_cast<const gram::List*>(&set)) {
        auto& elems = list->getElems();
        eset.insert(eset.end(), elems.begin(), elems.end());
    } else {
        eset.push_back(&set);
    }

    auto threads = std::string{};

    for (auto&& e : eset) {
        if (auto ds = dynamic_cast<const gram::DSym*>(e)) {
            bool ok = false;
            try {
                auto val = ds->eval(nullptr);
                if (auto psv = dynamic_cast<ProcsetValue*>(val.get())) {
                    psSet.push_back(psv->getProcSet());
                    threads = psv->getThreads();
                    ok = true;
                }
            } catch (EvalError& err) {}
            if (!ok) {
                PrintOutError("%s is not a valid procset.\n", e->toString().c_str());
                return nullptr;
            }
        }
    }
        
    // create the new procset
    auto new_procset = NewProcSet(name.c_str(), DBGSET_DEFSET,
                                  threads.empty() ? nullptr : threads.c_str());

    int offset = 0;
    for (auto& [ di, grp ] : psSet) {
        auto n = grp->numSet();
        new_procset->s_over_sets.push_back(NewOverlaySet(di, std::move(grp), offset));
        offset += n;
    }
    new_procset->s_nproc = offset;

    return new_procset;
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
dbgprocset_t *CreateProcSetForInvoke(const std::string& psName) {

    static std::regex psRE(R"((\$\w+)(\{(\d+)\})?)");
    auto match = std::smatch{};
    if (!std::regex_match(psName, match, psRE)) {
        PrintOutError("Invalid proc handle syntax. Expected $handle{n} where n is "
                      "the number of application ranks.\n");
        return nullptr;
    }

    auto name = match[1].str();
    
    if (!IdMap::idIsAvailable(name, true)) return nullptr;

    auto numProcs = 1;
    if (match[3].length()) numProcs = std::stoi(match[3]);

    // create the new procset
    auto rtn = NewProcSet(name.c_str(), DBGSET_INVOKE, nullptr);
    rtn->s_nproc = numProcs;
    rtn->s_init = 0;

    return rtn;
}

/*
** Finish setting up the procset after the application has finished
** launching/attaching in the dbg client layer.
*/
int FinalizeProcSetForInvoke(dbgprocset_t *ps, DbgInvoke *di) {
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
    ps->s_nproc = di->numRanks();

    over_set = NewOverlaySet(di->handle(), di->fullBitset()->clone(), 0);
    ps->s_over_sets.push_back(over_set);
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

    // for each overlay set, add the bitset to the recset
    for (auto over_set : ps->s_over_sets) {
        // create a group_recv_t for the over_set and add it to the res
        auto* diptr = over_set->s_di.lock();
        if (!diptr) {
            PrintOutMsg("Warning: expired procset $%s. Ignoring.\n", over_set->s_di.c_str());
            continue;
        }

        AddReceiverToSet(res, NewGroupReceiver(*diptr, over_set->s_di_bs.get()));
    }

    if (ps->s_threads) {
        res->setThreads(ps->s_threads);
    }

    return res;
}

dbgprocset_t::dbgprocset_t(std::string name, dbgset_type type, const char* threads)
    : s_name(std::move(name)),
      s_type(type),
      s_nproc(0),
      s_init(0),
      s_threads(threads ? strdup(threads) : nullptr)
{
    if (!s_name.empty() && s_name[0] == '$')
        s_name = s_name.substr(1);
}

dbgprocset_t::~dbgprocset_t() {
    if (s_threads) free(const_cast<char*>(s_threads));
    for (auto overlay : s_over_sets) {
        delete overlay;
    }
}

static dbgprocset_t *NewProcSet(const char *name, dbgset_type type, const char* threads) {

    auto s = std::make_shared<dbgprocset_t>(name, type, threads);
    ProcSets.push_back(s);
    return s.get();
}

static dbg_over_set_t *NewOverlaySet(DbgInvoke::Handle di, std::unique_ptr<Bitset> &&bs, int offset) {
    return new dbg_over_set_t {
        .s_di = std::move(di),
        .s_di_bs = std::move(bs),
        .s_ps_offset = offset
    };
}

static void AddOverlaySetToAllProcSet(dbg_over_set_t *over_set) {
    // This is playing a little fast and loose with memory management, since two 
    // procset will shared over_set.   I'm leaving it in for now because FreeProcSet
    // relies on finding the same one back.  
    AllProcSet->s_over_sets.push_back(over_set);
    AllProcSet->s_nproc += over_set->s_di_bs->numSet();
}

// Change this to only be used in val, if the val gets destroyed so does this.
void FreeProcSet(dbgprocset_t *s) {
    if (!s) return;

    if (s->s_type == DBGSET_INVOKE) {
        for (auto over_set : s->s_over_sets) {
            if (auto loc = std::find(AllProcSet->s_over_sets.begin(),
                                     AllProcSet->s_over_sets.end(), over_set);
                loc != AllProcSet->s_over_sets.end()) {
                AllProcSet->s_over_sets.erase(loc);
                AllProcSet->s_nproc -= over_set->s_di_bs->numSet();
            }
        }
    }

    // The procset memory is controlled by the ProcSet list
    if (auto loc = std::find_if(ProcSets.begin(), ProcSets.end(),
                                [s](auto x) { return x.get() == s; }); loc != ProcSets.end()) {
        ProcSets.erase(loc);
    } else {
        PrintOutError("Procset %s is not in ProcSet list\n", s->s_name.c_str());
    }
}

dbgprocset_t *FindProcSet(const char *name) {
    const auto look = (name[0] == '$' ? name+1 : name);

    for (auto&& ps : ProcSets) {
        if (ps->s_name == look)
            return ps.get();
    }
    return nullptr;
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
    const char* t = (threads ? strdup(threads) : nullptr);
    std::swap(procSet->s_threads, t);
    free(const_cast<char*>(t));
}

dbgprocset_t *CurrentProcSet(void) { return CurrProcSet; }

void PrintProcSet(dbgprocset_t *s) {


    if (s->s_over_sets.empty()) {
        PrintOutMsg("none");
        return;
    }

    int n = 0;

    // Here we want to print the "actual" ranks in the procset, not the "offset"
    // ranks.
    for (auto over_set : s->s_over_sets) {
        PrintOutMsg("%s", over_set->s_di.c_str());

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

std::string GetProcSetString(const char *setname, const Bitset *group, const char* threads) {
    if (!setname) return "";

    auto info = (setname[0] == '$' ? setname+1 : setname) + group->toSetString();
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
        // We should be able to assume set_info does not contain a '%'
        auto f2 = set_info + ": " + (f ? f : "");
        if (isErr) {
            vPrintOutError(f2.c_str(), ap);
        } else {
            vPrintOutMsg(f2.c_str(), ap);
        }
    } else if (isErr) {
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

