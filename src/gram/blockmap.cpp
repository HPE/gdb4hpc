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
** Free a map.
*/

void RegisterBlockMap(std::shared_ptr<g_block_map_t> bm) {
    _block_map_table[bm->m_name] = bm;
    _block_map_list.push_back(bm);
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
                                   const std::string& idx,
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

    auto blockIter = MakeBlockMapIterator(bm, idx);
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
    auto blockIter = MakeBlockMapIterator(bm, "");
    auto aifv = std::vector<AIFStream*>{aif};
    if (auto da = new AIFStream{blockIter, aifv}) {
        da->print(out);
        delete da;
    } else {
        PrintOutError("Cannot construct decomposition\n");
    }
}

namespace {
    class RangeBlockMapIterator : public ais::DecompIterator {
    public:
        RangeBlockMapIterator(g_block_map_t* bm, const std::string& subRng);
        void begin(bool isFortran) override;
        bool next() override;
        int getDim() override;
    private:
        std::vector<blockmap::indexer1D> m_idcs;
        std::vector<std::pair<int,int>> m_subRng;
        std::vector<int> m_curIdx;
        int m_resDim = 0;
    };
}


RangeBlockMapIterator::RangeBlockMapIterator(g_block_map_t* bm,
                                             const std::string& subRng)
{
    for (int i=0; i<bm->m_rank; ++i) {
        m_idcs.push_back(blockmap::indexer1D(bm, i));
    }
    m_subRng = BlockMapGetIndexRng(bm, subRng);
    m_resDim = 0;
    for (auto& [lo,hi] : m_subRng) {
        if (hi > lo) ++m_resDim;
    }

}

void RangeBlockMapIterator::begin(bool isFortran) {
    blockmap::indexer1D::init(m_idcs, isFortran);

    // slowest to fastest
    if (isFortran) {
        std::reverse(m_subRng.begin(), m_subRng.end());
    }

    for (auto& [lo,hi] : m_subRng) {
        m_curIdx.push_back(lo);
    }
}

bool RangeBlockMapIterator::next() {
    size_t dataIdx = 0;
    for (size_t i=0; i<m_curIdx.size(); ++i) {
        dataIdx = dataIdx * m_idcs[i].getLen() + m_curIdx[i];
    }
    
    if (m_curIdx[0] > m_subRng[0].second)
        return false;

    endLevel = 0;
    startLevel = m_resDim;

    size_t procIdx = 0;
    size_t repeat = 1;
    size_t level = 0;

    for (int i=0, rank=int(m_idcs.size()); i<rank; ++i) {
        const auto& idx = m_idcs[i];
        auto atEnd{false};
        idx.map(dataIdx, procIdx, repeat, atEnd);

        if (auto [lo,hi] = m_subRng[i]; hi > lo) {
            ++level;
            if (m_curIdx[i] + int(repeat) > hi) {
                repeat = hi - m_curIdx[i] + 1;
            } else {
                endLevel = level;
            }
        } else {
            repeat = 1;
        }
    }

    // set the output values
    len = int(repeat);
    streamIdx = static_cast<decltype(streamIdx)>(procIdx);

    // update the index
    m_curIdx.back() += repeat;
    for (size_t i = m_curIdx.size(); --i > 0; ) { // intentionally avoiding i==0
        auto [lo, hi] = m_subRng[i];
        if (m_curIdx[i] > hi) {
            m_curIdx[i] = lo;
            ++m_curIdx[i-1];
        } else {
            break;
        }
    }

    return true;
}

int RangeBlockMapIterator::getDim() { return m_resDim; }


std::shared_ptr<ais::DecompIterator> MakeBlockMapIterator(g_block_map_t *bm,
                                                          const std::string& idx) {
    if (idx.empty()) {
        return std::make_shared<blockmap::BlockMapIterator>(bm);
    } else {
        return std::make_shared<RangeBlockMapIterator>(bm,idx);
    }
}
