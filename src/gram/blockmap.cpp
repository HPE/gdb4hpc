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
#include "useful.h"
#include "blockmap.h"

static std::map<std::string, std::shared_ptr<g_block_map_t>> _block_map_table;
static std::vector<std::shared_ptr<g_block_map_t>> _block_map_list;

/*
** Find a map.
*/
g_block_map_t *FindBlockMap(const std::string& blockmap) {
    if (auto loc = _block_map_table.find(blockmap); loc != _block_map_table.end()) {
        return loc->second.get();
    }
    return nullptr;
}

const std::vector<std::shared_ptr<g_block_map_t>> GetBlockMapList() {
    return _block_map_list;
}

// TODO: When I wrote this (andrewg) I am still not 100% confident on how
// blockmap works. So this function should be made more versatile at some point.
bool VerifyBlockMap(g_block_map_t* m) {
    // Check data definition
    if (m->m_data == (int *)NULL) {
        SetError(
            "Must provide a dimension definition before ending a "
            "decomposition.");
        return false;
    }

    // Check distribute definition
    if (m->m_distribute == nullptr) {
        SetError(
            "Must provide a distribute definition before ending a "
            "decomposition.");
        return false;
    }

    // Check grid definition
    if (m->m_procs == (int *)NULL) {
        SetError(
            "Must provide a proc_grid definition before ending a "
            "decomposition.");
        return false;
    }

    // We need to count the number of non-distributed dimensions to ensure they
    // match between each definition.
    int count = 0;
    for (auto i = 0; i < m->m_rank; ++i) {
        if (m->m_distribute[i] == Distribution::Star) ++count;
    }

    int star_dims = count;

    // Check to make sure the grid definition matches
    for (int i = 0, count = 0; i < m->m_rank; ++i) {
        if (m->m_procs[i] == 1) ++count;
    }

    if (star_dims != count) {
        SetError(
            "'*' allocation mismatch between distribute and proc_grid "
            "definition.");
        return false;
    }

    // check order definition
    if (!m->m_dim_order) {
        // If this is a 1-D distribution we can assume the dimension ordering
        if (m->m_rank - star_dims == 1) {
            // note here that count now refers to the number of "numbers"
            // written to make sure there isn't an issue in the logic
            count = 0;
            m->m_dim_order = new int[m->m_rank];
            for (int i = 0; i < m->m_rank; ++i) {
                if (m->m_distribute[i] == Distribution::Star) {
                    m->m_dim_order[i] = int(Distribution::Star);
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
                return false;
            }
        } else {
            SetError(
                "Must provide a dim_order definition before ending a "
                "decomposition.");
            return false;
        }
    } else {
        // Check to make sure the order definition matches
        for (int i = 0, count = 0; i < m->m_rank; ++i) {
            if (m->m_dim_order[i] == int(Distribution::Star)) ++count;
        }

        if (star_dims != count) {
            SetError(
                "'*' allocation mismatch between distribute and dim_order "
                "definition.");
            return false;
        }
    }

    // check that the dimension in each direction is a multiple of the number of ranks
    for (int i=0; i<m->m_rank; ++i) {
        if (m->m_distribute[i] != Distribution::Star &&
            m->m_data[i] % m->m_procs[i] != 0) {
            SetError("dimension size %d is not a multiple of proc_grid %d.",
                     m->m_data[i], m->m_procs[i]);
            return false;
        }
    }

    return true;
}

/*
** Create a new map and add to the table
*/
g_block_map_t::g_block_map_t(const std::string& name, bool debug)
    : m_name(name),
      m_rank(0),
      m_distribute(nullptr),
      m_data(nullptr),
      m_procs(nullptr),
      m_blockcyclic_val(nullptr),
      m_dim_order(nullptr),
      m_debug(debug) {
    if (name.size() && name[0] != '$') m_name = '$' + m_name;
}

g_block_map_t::~g_block_map_t() {
    delete[] m_distribute;
    delete[] m_data;
    delete[] m_procs;
    delete[] m_blockcyclic_val;
    delete[] m_dim_order;
}

/*
** Free a map.
*/

void RegisterBlockMap(std::shared_ptr<g_block_map_t> bm) {
    _block_map_table[bm->m_name] = bm;
    _block_map_list.push_back(bm);
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
    Distribution m_distribute;  // distribute scheme
    size_t m_len;               // number of elements in this dimension
    size_t m_numProcs;          // number of process for distibuting 
    size_t m_blockCyclic;       // cycle count for the Distriubution::BlockCyclic
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
    m_numProcs(std::max(m->m_procs[d],1)),
    m_blockCyclic(m->m_blockcyclic_val[d]),
    m_dimOrder(m->m_dim_order[d]),
    m_blockLen(std::max(m_len / m_numProcs,size_t{1})),
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
    case Distribution::Block:
        dproc = idx / m_blockLen;
        repeat = m_blockLen;
        break;
    case Distribution::Cyclic:
        dproc = idx % m_numProcs;
        break;
    case Distribution::Star:
        dproc = 0;
        repeat = m_len;
        break;
    case Distribution::BlockCyclic:
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
    if (m_distribute != Distribution::Star &&
        oproc.m_distribute != Distribution::Star &&
        m_dimOrder > oproc.m_dimOrder) {
        m_procScale *= oproc.m_numProcs;
    }
}

// for now remove EvalMapFunc, when need, copy from map.c
std::string GetBlockMapMsg(g_block_map_t *m) {
    int i;
    std::stringstream sc; //stringstream for cli messages
    std::stringstream sm; //stringstream for mi messages

    sc<<"$"<<m->m_name<<":\n";
    sm<<"{decomp_name=\""<<m->m_name;

    if (m->m_rank == 0) {
        SetError("Invalid decomposition.");
        return "";
    }

    sc<<"\tdimension ";
    sm<<"\",dimension=\"";
    if (m->m_data == (int *)NULL) {
        SetError("Invalid dimension definition for decomposition.");
        return "";
    } else {
        for (i = 0; i < m->m_rank; i++) {
            sc << m->m_data[i] << (i+1==m->m_rank ? "\n" : ",");
            sm << m->m_data[i] << (i+1==m->m_rank ? "" : ",");
        }
    }

    sc<<"\tdistribute ";
    sm<<"\",distribute=\"";
    if (m->m_distribute == nullptr) {
        SetError("Invalid distribute definition for decomposition.");
        return "";
    } else {
        for (i = 0; i < m->m_rank; i++) {
            switch (m->m_distribute[i]) {
                case Distribution::Block:
                    sc<<"block"<<(i+1==m->m_rank ? "\n" : ",");
                    sm<<"block"<<(i+1==m->m_rank ? "" : ",");
                    break;
                case Distribution::Cyclic:
                    sc<<"cyclic"<<(i+1==m->m_rank ? "\n" : ",");
                    sm<<"cyclic"<<(i+1==m->m_rank ? "" : ",");
                    break;
                case Distribution::Star:
                    sc<<"*"<<(i+1==m->m_rank ? "\n" : ",");
                    sm<<"*"<<(i+1==m->m_rank ? "" : ",");
                    break;
                case Distribution::BlockCyclic:
                    sc<<m->m_blockcyclic_val[i]<<(i+1==m->m_rank ? "\n" : ",");
                    sm<<m->m_blockcyclic_val[i]<<(i+1==m->m_rank ? "" : ",");
                    break;
                default:
                    SetError(
                        "Invalid distribute definition for decomposition.");
                    return "";
            }
        }
    }

    sc<<"\tproc_grid ";
    sm<<"\",proc_grid=\"";
    if (m->m_procs == (int *)NULL) {
        SetError("Invalid process grid definition for decomposition.");
        return "";
    } else {
        for (i = 0; i < m->m_rank; i++) {
            if (m->m_procs[i] == 1){
                sc<<"*"<<(i+1==m->m_rank ? "\n" : ",");
                sm<<"*"<<(i+1==m->m_rank ? "" : ",");
            }else{
                sc<<m->m_procs[i]<<(i+1==m->m_rank ? "\n" : ",");
                sm<<m->m_procs[i]<<(i+1==m->m_rank ? "" : ",");
            }
        }
    }

    sc<<"\tdim_order ";
    sm<<"\",dim_order=\"";
    if (m->m_dim_order == (int *)NULL) {
        SetError("Invalid dimension order definition for decomposition.");
        return "";
    } else {
        for (i = 0; i < m->m_rank; i++) {
            if (m->m_dim_order[i] == int(Distribution::Star)){
                sc<<"*"<<(i+1==m->m_rank ? "\n" : ",");
                sm<<"*"<<(i+1==m->m_rank ? "" : ",");
            }else{
                sc<<m->m_dim_order[i]<<(i+1==m->m_rank ? "\n" : ",");
                sm<<m->m_dim_order[i]<<(i+1==m->m_rank ? "" : ",");
            }
        }
    }

    sc<<"\n";
    sm<<"\"}";

    //return the results in the proper format
    if (Client.MI_mode){
        return sm.str();
    }
    return sc.str();

}

void FinishBlockMap(void) {
    _block_map_table.clear();
    _block_map_list.clear();
}

bool BlockMapIsDegen(g_block_map_t* m) {
    for (int i=0; i<m->m_rank; ++i) {
        if (m->m_distribute[i] != Distribution::Star) return false;
    }
    return true;
}
size_t BlockMapGetElementsPerRank(g_block_map_t* m) {
    size_t len = 1;
    for (int i=0; i<m->m_rank; ++i) {
        if (m->m_distribute[i] == Distribution::Star) {
            len *= m->m_data[i];
        } else if (m->m_procs[i] > 0) {
            // there's no reason m_procs[i] would be zero,
            // just being careful
            len *= (m->m_data[i] / m->m_procs[i]);
        }
    }
    return len;
}

AIFStream *construct_blockmap_data(g_block_map_t *bm,
                                   rec_set_t* rec_set,
                                   rep_set_t *rep_set) {
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

    size_t numRanks = 0;

    // map each DI to the subset of involved ranks and to an offset
    // within the out ranks
    auto dis = std::map<DbgInvoke*,std::pair<Bitset*,size_t>>{};
    
    auto rcvs = rec_set->getReceivers();
    for (auto rcv : rcvs) {
        dis[&rcv->rec_di] = std::make_pair(rcv->rec_group.get(), numRanks);
        numRanks += rcv->rec_group->numSet();
    }

    // Create an array of aif values
    auto da = std::vector<AIFStream*>{numRanks};

    // process rep_set, placing the AIF data into the array
    for (auto&& reply : rep_set) {
        // A blockmap across multiple DI's seems weird, but we can track
        // an offset for the mapping, so it's not a lot of work
        auto [ rcvGrp, offset ] = dis[reply->rep_di.lock()];
        auto repGrp = reply->rep_group.get();
        auto dsIdx = offset;

        // A blockmap across a subset of the procset is a bit more likely,
        // so note how we're testing the rsvGrp first since it determines 
        // the index within the result
        for (size_t i = 0, n = rcvGrp->sizeInBits(); i < n; ++i) {
            if (rcvGrp->test(i)) {
                // Check if this rank is in this reply
                if (repGrp->test(i)) {
                    // Why is the join here?
                    da[dsIdx] = EVENT_DATA(reply->rep_event)->join();
                }
                ++dsIdx;
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
        bool next() override;
        int getDim() override;
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

int BlockMapIterator::getDim() {
    return m_idcs.size();
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
