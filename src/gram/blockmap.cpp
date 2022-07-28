/*********************************************************************************\
 * Block Map routines.
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

#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <iostream>

#include "ais.h"
#include "bitset.h"
#include "blockmap_util.h"
#include "client_settings.h"
#include "defs.h"
#include "gram.h"
#include "useful.h"

#define BLOCKMAP_TABLE_SIZE 7 /* 2^7 = 128 = number that can be active */

static htable_t *_block_map_table = (htable_t *)NULL;
static list_t *_block_map_list = (list_t *)NULL;
// int	EvalBlockMapFunc(int *, g_block_map_t *, func_t *, int);

/*
** Find a map.
*/
g_block_map_t *FindBlockMap(const char *blockmap) {
    hentry_t *h;

    if ((h = HashSearch(_block_map_table, blockmap)) != (hentry_t *)NULL)
        return (g_block_map_t *)h->h_data;

    return (g_block_map_t *)NULL;
}

int CheckBlockMap(hentry_t *sym) {
    g_block_map_t *m = FindBlockMap(SymLookup(sym));

    return m != (g_block_map_t *)NULL;
}

void SetBlockMap(void) {
    if (_block_map_list != (list_t *)NULL) SetList(_block_map_list);
}

g_block_map_t *GetBlockMap(void) {
    if (_block_map_list == (list_t *)NULL) return (g_block_map_t *)NULL;

    return (g_block_map_t *)GetListElement(_block_map_list);
}

// TODO: When I wrote this (andrewg) I am still not 100% confident on how
// blockmap works. So this function should be made more versatile at some point.
int VerifyBlockMap(hentry_t *sym) {
    g_block_map_t *m;
    int i, count, star_dims;

    if (sym == NULL) {
        SetError("Attempted to lookup null decomposition sym.");
        return 0;
    }

    if ((m = FindBlockMap(SymLookup(sym))) == NULL) {
        SetError("Could not find a decomposition named %s.", SymLookup(sym));
        return 0;
    }

    // Check data definition
    if (m->m_data == (int *)NULL) {
        SetError(
            "Must provide a dimension definition before ending a "
            "decomposition.");
        return 0;
    }

    // Check distribute definition
    if (m->m_distribute == (int *)NULL) {
        SetError(
            "Must provide a distribute definition before ending a "
            "decomposition.");
        return 0;
    }

    // Check grid definition
    if (m->m_procs == (int *)NULL) {
        SetError(
            "Must provide a proc_grid definition before ending a "
            "decomposition.");
        return 0;
    }

    // We need to count the number of non-distributed dimensions to ensure they
    // match between each definition.
    for (i = 0, count = 0; i < m->m_rank; ++i) {
        if (m->m_distribute[i] == CT_STAR) ++count;
    }

    star_dims = count;

    // Check to make sure the grid definition matches
    for (i = 0, count = 0; i < m->m_rank; ++i) {
        if (m->m_procs[i] == 1) ++count;
    }

    if (star_dims != count) {
        SetError(
            "'*' allocation mismatch between distribute and proc_grid "
            "definition.");
        return 0;
    }

    // check order definition
    if (m->m_dim_order == (int *)NULL) {
        // If this is a 1-D distribution we can assume the dimension ordering
        if (m->m_rank - star_dims == 1) {
            // note here that count now refers to the number of "numbers"
            // written to make sure there isn't an issue in the logic
            count = 0;
            m->m_dim_order = (int *)Alloc(sizeof(int) * m->m_rank);
            for (i = 0; i < m->m_rank; ++i) {
                if (m->m_distribute[i] == CT_STAR) {
                    m->m_dim_order[i] = CT_STAR;
                } else {
                    m->m_dim_order[i] = 1;
                    ++count;
                }
            }
            if (count != 1) {
                // This should never happen
                SetError(
                    "'*' allocation mismatch between distribute and dim_order "
                    "definitions.");
                return 0;
            }
        } else {
            SetError(
                "Must provide a dim_order definition before ending a "
                "decomposition.");
            return 0;
        }
    } else {
        // Check to make sure the order definition matches
        for (i = 0, count = 0; i < m->m_rank; ++i) {
            if (m->m_dim_order[i] == CT_STAR) ++count;
        }

        if (star_dims != count) {
            SetError(
                "'*' allocation mismatch between distribute and dim_order "
                "definition.");
            return 0;
        }
    }

    return 1;
}

/*
** Create a new map.
*/
g_block_map_t *NewBlockMap(hentry_t *sym) {
    g_block_map_t *m;

    m = (g_block_map_t *)Alloc(sizeof(g_block_map_t));
    m->m_sym = sym;
    m->m_distribute = (int *)NULL;
    m->m_data = (int *)NULL;
    m->m_procs = (int *)NULL;
    m->m_blockcyclic_val = (int *)NULL;
    m->m_dim_order = (int *)NULL;
    m->m_rank = 0;
    m->m_builtin = (int (*)(list_t *))NULL;
    m->m_debug = 0;

    if (sym == (hentry_t *)NULL) return m;

    if (_block_map_list == (list_t *)NULL) _block_map_list = NewList();

    if (_block_map_table == (htable_t *)NULL)
        _block_map_table = HashCreate(BLOCKMAP_TABLE_SIZE);

    if (HashInsert(_block_map_table, SymLookup(sym), (void *)m) ==
        (hentry_t *)NULL) {
        SetError("Too many decompositions defined");
        return (g_block_map_t *)NULL;
        ;
    }

    AddToList(_block_map_list, (void *)m);

    return m;
}

/*
** Free a map.
*/
void DestroyBlockMap(g_block_map_t *m) {
    hentry_t *h;

    if (m == (g_block_map_t *)NULL) return;

    if ((h = HashSearch(_block_map_table, SymLookup(m->m_sym))) !=
        (hentry_t *)NULL)
        HashRemove(_block_map_table, h);

    RemoveFromList(_block_map_list, (void *)m);

    if (m->m_distribute != (int *)NULL) Free(m->m_distribute);
    if (m->m_data != (int *)NULL) Free(m->m_data);
    if (m->m_procs != (int *)NULL) Free(m->m_procs);
    if (m->m_blockcyclic_val != (int *)NULL) Free(m->m_blockcyclic_val);
    if (m->m_dim_order != (int *)NULL) Free(m->m_dim_order);

    Free(m);
}

namespace {
// handle the data distribution in a single dimension
class indexer1D {
public:
    indexer1D(const g_block_map_t* m, int dimIdx);
    indexer1D() : m_dimIdx(0), m_len(0) {}
    void map(size_t& dataIdx, size_t& procIdx, size_t& repeat, bool& atEnd) const;
    
    size_t getTotalProcSize() const { return m_totalProcSize; }
    size_t getTotalDataSize() const { return m_dataScale * m_len; }
    int getDimIdx() const { return m_dimIdx; }

    void setDataSize(indexer1D* prev);
    void setProcSize(const indexer1D& oproc);

private:
    int m_dimIdx;               // dimension represented
    int m_distribute;           // distribute scheme (from cmdtoken.h)
    size_t m_len;               // number of elements in this dimension
    size_t m_numProcs;          // number of process for distibuting 
    size_t m_blockCyclic;       // cycle count for the CT_BLOCK_CYCLIC
    size_t m_dimOrder;

    size_t m_blockLen;          // number of elements in each block
    size_t m_totalProcSize;     // total processes from this and faster varying dims
    size_t m_dataScale;         // step in the data array for each subsequent element
    size_t m_procScale;         // step in the rank for each subsequent element
};
}

// define the distribution in single dimension
// \param m is the block map definition
// \param d is the index for this dimension, starting at 0
// \param prev if given is the distribution for the next faster dimension
indexer1D::indexer1D(const g_block_map_t* m, int d) :
    m_dimIdx(d),
    m_distribute(m->m_distribute[d]),
    m_len(m->m_data[d]),
    m_numProcs(m->m_procs[d]),
    m_blockCyclic(m->m_blockcyclic_val[d]),
    m_dimOrder(m->m_dim_order[d]),
    m_blockLen(m_len / m_numProcs),
    m_totalProcSize(m_numProcs),
    m_dataScale(1),
    m_procScale(1)
{
}

// map an index int the global array into the offsets into the
// array of process ranks and the data array from that rank
// \param idx is the index of the global array in this dimension,
//      it always has origin 0
// \param procIdx i/o is updated with the offset into the array of ranks
//      it is unchanged if this dimension is not decomposed
// \param repeat will contain on exit the number of elements in to row.
//      note that the result will only be used for the fastest varying
//      dimension because \a repeat elements will be contiguously stored
//      in the data result for that rank so they can be copied in one shot
void indexer1D::map(size_t& dataIdx, size_t& procIdx, size_t& repeat,
                    bool& atEnd) const {
    repeat = 1;
    size_t dproc = 0;

    auto idx = dataIdx / m_dataScale;
    
    switch (m_distribute) {
    case CT_BLOCK:
        dproc = idx / m_blockLen;
        repeat = m_blockLen;
        break;
    case CT_CYCLIC:
        dproc = idx % m_numProcs;
        break;
    case CT_STAR:
        dproc = 0;
        repeat = m_len;
        break;
    case CT_BLOCK_CYCLIC:
        dproc = (idx / m_blockCyclic) % m_numProcs;
        repeat = m_blockCyclic;
        break;
    default:  // checked at construction time
        break;
    }

    if (m_dataScale > 1) {
        repeat = 1;
    }

    procIdx += dproc * m_procScale;

    atEnd = (idx+repeat == m_len);
    dataIdx = dataIdx % m_dataScale;

}

void indexer1D::setDataSize(indexer1D* prev) {
    if (prev) {
        m_dataScale *= prev->getTotalDataSize();
    }
}

void indexer1D::setProcSize(const indexer1D& oproc) {
    if (m_distribute != CT_STAR &&
        oproc.m_distribute != CT_STAR &&
        m_dimOrder > oproc.m_dimOrder) {
        m_procScale *= oproc.m_numProcs;
    }
}

// for now remove EvalMapFunc, when need, copy from map.c

void PrintBlockMap(cli_out_t &fp, g_block_map_t *m) {
    int i;

    fp("$%s:\n", SymLookup(m->m_sym));

    if (m->m_rank == 0) {
        SetError("Invalid decomposition.");
        return;
    }

    fp("\tdimension ");

    if (m->m_data == (int *)NULL) {
        SetError("Invalid dimension definition for decomposition.");
        return;
    } else {
        // want to stop before last index
        for (i = 0; i < (m->m_rank - 1); i++) {
            fp("%d,", m->m_data[i]);
        }
        // now print the last index
        fp("%d\n", m->m_data[m->m_rank - 1]);
    }

    fp("\tdistribute ");

    if (m->m_distribute == (int *)NULL) {
        SetError("Invalid distribute definition for decomposition.");
        return;
    } else {
        // want to stop before last index
        for (i = 0; i < (m->m_rank - 1); i++) {
            switch (m->m_distribute[i]) {
                case CT_BLOCK:
                    fp("%s,", "block");
                    break;
                case CT_CYCLIC:
                    fp("%s,", "cyclic");
                    break;
                case CT_STAR:
                    fp("%s,", "*");
                    break;
                case CT_BLOCK_CYCLIC:
                    fp("%d,", m->m_blockcyclic_val[i]);
                    break;
                default:
                    SetError(
                        "Invalid distribute definition for decomposition.");
                    return;
            }
        }
        // need to print the last index
        switch (m->m_distribute[m->m_rank - 1]) {
            case CT_BLOCK:
                fp("%s\n", "block");
                break;
            case CT_CYCLIC:
                fp("%s\n", "cyclic");
                break;
            case CT_STAR:
                fp("%s\n", "*");
                break;
            case CT_BLOCK_CYCLIC:
                fp("%d\n", m->m_blockcyclic_val[m->m_rank - 1]);
                break;
            default:
                SetError("Invalid distribute definition for decomposition.");
                return;
        }
    }

    fp("\tproc_grid ");

    if (m->m_procs == (int *)NULL) {
        SetError("Invalid process grid definition for decomposition.");
        return;
    } else {
        // want to stop before last index
        for (i = 0; i < (m->m_rank - 1); i++) {
            if (m->m_procs[i] == 1)
                fp("%s,", "*");
            else
                fp("%d,", m->m_procs[i]);
        }
        // now print the last index
        if (m->m_procs[i] == 1)
            fp("%s\n", "*");
        else
            fp("%d\n", m->m_procs[m->m_rank - 1]);
    }

    fp("\tdim_order ");

    if (m->m_dim_order == (int *)NULL) {
        SetError("Invalid dimension order definition for decomposition.");
        return;
    } else {
        // want to stop before last index
        for (i = 0; i < (m->m_rank - 1); i++) {
            if (m->m_dim_order[i] == CT_STAR)
                fp("%s,", "*");
            else
                fp("%d,", m->m_dim_order[i]);
        }
        // now print the last index
        if (m->m_dim_order[i] == CT_STAR)
            fp("%s\n", "*");
        else
            fp("%d\n", m->m_dim_order[m->m_rank - 1]);
    }

    fp("\n");
}

void FinishBlockMap(void) {
    g_block_map_t *m;

    SetList(_block_map_list);

    while ((m = (g_block_map_t *)GetListElement(_block_map_list)) !=
           (g_block_map_t *)NULL)
        DestroyBlockMap(m);

    DestroyList(_block_map_list, (void (*)(void *))NULL);
    HashDestroy(_block_map_table, (void (*)(char *, void *))NULL);
}

bool BlockMapIsDegen(g_block_map_t* m) {
    for (int i=0; i<m->m_rank; ++i) {
        if (m->m_distribute[i] != CT_STAR) return false;
    }
    return true;
}
size_t BlockMapGetElementsPerRank(g_block_map_t* m) {
    size_t len = 1;
    for (int i=0; i<m->m_rank; ++i) {
        if (m->m_distribute[i] == CT_STAR) {
            len *= m->m_data[i];
        } else if (m->m_procs[i] > 0) {
            // there's no reason m_procs[i] would be zero,
            // just being careful
            len *= (m->m_data[i] / m->m_procs[i]);
        }
    }
    return len;
}

/* This function turn the given blockmap function into a description string for
 * p2p comp purposes 21/06/2011 - Minh Dinh
 */
void BlockMapToString(g_block_map_t *m, var_t *v, int numprocs, char *str) {
    int i;
    char buf[100];

    sprintf(buf, "$%s,%d,%d,", v->v_pname, numprocs, m->m_rank);
    strcat(str, buf);
    for (i = 0; i < m->m_rank; i++) {
        sprintf(buf, "%d,", m->m_distribute[i]);
        strcat(str, buf);
    }

    for (i = 0; i < m->m_rank; i++) {
        sprintf(buf, "%d,", m->m_blockcyclic_val[i]);
        strcat(str, buf);
    }

    for (i = 0; i < m->m_rank; i++) {
        sprintf(buf, "%d,", m->m_data[i]);
        strcat(str, buf);
    }

    for (i = 0; i < m->m_rank; i++) {
        sprintf(buf, "%d,", m->m_procs[i]);
        strcat(str, buf);
    }

    for (i = 0; i < m->m_rank; i++) {
        sprintf(buf, "%d", m->m_dim_order[i]);
        strcat(str, buf);
        if (i < m->m_rank - 1)
            strcat(str, ",");
        else
            strcat(str, "");
    }

    return;
}

/*
** Extract and combine data using blockmap function.
*/
AIFStream *construct_blockmap_data(g_block_map_t *bm, dbgprocset_t *procset,
                                   int array_size, rep_set_t *rep_set) {
    if (!rep_set) return nullptr;

    // Make sure that there were no errors and we received rep_set, otherwise
    // print error.
    if (SetHasError(rep_set)) {
        PrintOutError("Cannot build blockmap due to error in reply!\n");
        return nullptr;
    }
    if (GetNumberOfReply(rep_set) == 0) {
        PrintOutError("Cannot build blockmap due to empty reply!\n");
        return nullptr;
    }
    // FIXME: Currently this only works with sets that have only 1 over_set
    if (procset->s_num_sets != 1) {
        PrintOutError(
            "Cannot build blockmap due to multiple handles in procset!\n");
        return nullptr;
    }

    // FIXME: This only works with procsets having a single over_set, i.e.
    // DBGSET_INVOKE
    SetOverlaySetForProcSet(procset);
    auto over_set = GetNextOverlaySetForProcSet(procset);

    // Create an array of aif values
    auto da = std::vector<AIFStream*>{size_t(array_size)};

    // process rep_set, placing the AIF data into the array
    for (auto&& reply : rep_set) {
        // Now loop over every rank to see if its in this reply
        // FIXME: There must be a more efficient way to do this...
        for (int i = 0; i < array_size; ++i) {
            // Check if this rank is in this reply
            if (reply->rep_group->test(i + over_set->s_ps_offset)) {
                da[i] = EVENT_DATA(reply->rep_event)->join();
            }
        }
    }

    auto blockIter = MakeBlockMapIterator(bm);
    auto a = new AIFStream{blockIter, da};
    if (!a) {
        PrintOutError("Cannot construct decomposition data: %s\n", GetError());
    }

    return a;
}

// Print out an AIF (presumably stream) value reformated by a block map
void PrintWithBlockMap(AIFStream* aif, g_block_map_t* bm, std::ostream& out) {
    if (!bm || !BlockMapIsDegen(bm)) {
        PrintOutError("Expected a fully distributed decomposition\n");
        return;
    }
    auto blockIter = MakeBlockMapIterator(bm);
    auto aifv = std::vector<AIFStream*>{aif};
    if (auto da = new AIFStream{blockIter, aifv}) {
        da->print(out);
        delete da;
    } else {
        PrintOutError("Cannot construct decomposition\n");
    }
}

namespace {
    class BlockMapIterator : public ais::DecompIterator {
    public:
        BlockMapIterator(g_block_map_t* bm);
        void begin(bool isFortran) override;
        bool next() override;;
    private:
        size_t m_idx;
        size_t m_totElems;
        std::vector<indexer1D> m_idcs;
    };
}

BlockMapIterator::BlockMapIterator(g_block_map_t* bm)
{
    for (int i=0; i<bm->m_rank; ++i) {
        m_idcs.push_back(indexer1D(bm, i));
    }
}

void BlockMapIterator::begin(bool isFortran) {
    // we want the indexers ordered from slowest changing
    // to fastest varying
    if (isFortran) {
        std::reverse(m_idcs.begin(), m_idcs.end());
    }

    // initialize the indexers
    indexer1D* prev = nullptr;
    for (auto it=m_idcs.rbegin(); it!=m_idcs.rend(); ++it) {
        it->setDataSize(prev);
        prev = &*it;
    }

    for (auto&& a : m_idcs) {
        for (auto&& b : m_idcs) {
            a.setProcSize(b);
        }
    }

    m_idx = 0;
    m_totElems = m_idcs[0].getTotalDataSize();
}

// Determine which rank the next chunk of elements will be coming from.
// The computed values include which rank to read, how may consecutive
// elements are from there, and the nesting location for multi-
// dimension arrays
bool BlockMapIterator::next() {
    if (m_idx >= m_totElems)
        return false;

    endLevel = 0;
    startLevel = int(m_idcs.size());

    // the mapping is independent in each direction so the
    // Indexers can do be applied one at a time.
    size_t procIdx = 0;
    size_t dataIdx = m_idx;
    size_t repeat = 1;
    for (int i=0, rank=int(m_idcs.size()); i<rank; ++i) {
        const auto& idx = m_idcs[i];
        auto atEnd{false};
        idx.map(dataIdx, procIdx, repeat, atEnd);

        if (!atEnd) {
            endLevel = i+1;
        }
    }
    
    len = int(repeat);
    streamIdx = static_cast<decltype(streamIdx)>(procIdx);

    m_idx += repeat;

    return true;
}


std::shared_ptr<ais::DecompIterator> MakeBlockMapIterator(g_block_map_t *bm) {
    return std::make_shared<BlockMapIterator>(bm);
}
