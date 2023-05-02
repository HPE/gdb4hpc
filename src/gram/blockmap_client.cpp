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
#include "client_settings.h"
#include "defs.h"
#include "useful.h"
#include "blockmap_client.h"

static std::map<std::string, std::shared_ptr<BlockMap>> block_map_table;

/*
** Find a map.
*/
BlockMap *FindBlockMap(const std::string& blockmap) {
    if (auto loc = block_map_table.find(blockmap); loc != block_map_table.end()) {
        return loc->second.get();
    }
    return nullptr;
}

const std::vector<std::shared_ptr<BlockMap>> GetBlockMapList() {
    auto list = std::vector<std::shared_ptr<BlockMap>>{};
    std::transform(block_map_table.begin(), block_map_table.end(),
                   std::back_inserter(list), [](auto &pair) { return pair.second; });
    return list;
}

// TODO: When I wrote this (andrewg) I am still not 100% confident on how
// blockmap works. So this function should be made more versatile at some point.
bool BlockMap::verify() {
    // Check data definition
    if (m_dims.empty()) {
        SetError(
            "Must provide a dimension definition before ending a "
            "decomposition.");
        return false;
    }

    // We need to count the number of non-distributed dimensions to ensure they
    // match between each definition.
    int count = 0;
    for (const auto& d : m_dims) {
        if (d.distribution == Distribution::Star) ++count;
    }

    int star_dims = count;

    // Check to make sure the grid definition matches
    count = 0;
    for (const auto& d : m_dims) {
        if (d.numProcs == 1) ++count;
    }

    if (star_dims != count) {
        SetError(
            "'*' allocation mismatch between distribute and proc_grid "
            "definition.");
        return false;
    }

    // check order definition
    // Check to make sure the order definition matches
    count = 0;
    for (const auto& d : m_dims) {
        if (d.dim_order == int(Distribution::Star)) ++count;
    }

    if (star_dims != count) {
        SetError(
            "'*' allocation mismatch between distribute and dim_order "
            "definition.");
        return false;
    }

    // check that the dimension in each direction is a multiple of the number of ranks
    for (const auto& d : m_dims) {
        if (d.distribution != Distribution::Star && (d.size % d.numProcs != 0)) {
            SetError("dimension size %zu is not a multiple of proc_grid %zu.",
                     d.size, d.numProcs);
            return false;
        }
    }

    return true;
}


/*
** Free a map.
*/

void RegisterBlockMap(std::shared_ptr<BlockMap> bm) {
    block_map_table[bm->getName()] = bm;
}

void UnregisterBlockMap(const std::string& name) {
    if (auto loc = block_map_table.find(name); loc != block_map_table.end()) {
        block_map_table.erase(loc);
    }
}

std::string BlockMap::oldMsg() const {
    int i;
    std::stringstream sc; //stringstream for cli messages
    std::stringstream sm; //stringstream for mi messages

    auto rank = getRank();

    sc<<m_name<<":\n";
    sm<<"{name=\""<<m_name;

    if (getRank() == 0) {
        SetError("Invalid decomposition.");
        return "";
    }

    auto csep = [rank](int i) { return (i+1 == rank ? "\n" : ","); };
    auto msep = [rank](int i) { return (i+1 == rank ? "" : ","); };

    sc<<"\tdimension ";
    sm<<"\",dimension=\"";

    for (i = 0; i < rank; i++) {
        const auto& d = getDimension(i);
        sc << d.size << csep(i);
        sm << d.size << msep(i);
    }

    sc<<"\tdistribute ";
    sm<<"\",distribute=\"";
    for (i = 0; i < rank; i++) {
        auto c = csep(i), m = msep(i);
        const auto& d = getDimension(i);
        switch (d.distribution) {
        case Distribution::Block:
            sc<<"block"<<c;
            sm<<"block"<<m;
            break;
        case Distribution::Cyclic:
            sc<<"cyclic"<<c;
            sm<<"cyclic"<<m;
            break;
        case Distribution::Star:
            sc<<"*"<<c;
            sm<<"*"<<m;
            break;
        case Distribution::BlockCyclic:
            sc<<d.blockCyclic<<c;
            sm<<d.blockCyclic<<m;
            break;
        default:
            SetError(
                "Invalid distribute definition for decomposition.");
            return "";
        }
    }

    sc<<"\tproc_grid ";
    sm<<"\",proc_grid=\"";
    for (i = 0; i < rank; i++) {
        const auto& d = getDimension(i);
        if (d.numProcs == 1){
            sc<<"*"<<csep(i);
            sm<<"*"<<msep(i);
        }else{
            sc<<d.numProcs<<csep(i);
            sm<<d.numProcs<<msep(i);
        }
    }

    sc<<"\tdim_order ";
    sm<<"\",dim_order=\"";
    for (i = 0; i < rank; i++) {
        const auto& d = getDimension(i);
        if (d.dim_order == int(Distribution::Star)){
            sc<<"*"<<csep(i);
            sm<<"*"<<msep(i);
        }else{
            sc<<d.dim_order<<csep(i);
            sm<<d.dim_order<<msep(i);
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

std::string BlockMap::msg() const {
    if (m_oldStyle) return oldMsg();

    std::ostringstream os;
    os << m_name << ": ";
    int n = 0;
    for ( auto d : m_dims ) {
        if (n++) os << ", ";
        d.printConcise(os, m_explicitOrder);
    }
    os << "\n";
    return os.str();
}

void FinishBlockMap(void) {
    block_map_table.clear();
}

bool BlockMap::isDegen() const {
    for (const auto& d : m_dims) {
        if (d.distribution != Distribution::Star) return false;
    }
    return true;
}
size_t BlockMap::getElementsPerRank() const {
    size_t len = 1;
    for (const auto& d : m_dims) {
        if (d.distribution == Distribution::Star) {
            len *= d.size;
        } else if (d.numProcs > 0) {
            // there's no reason m_procs[i] would be zero,
            // just being careful
            len *= (d.size / d.numProcs);
        }
    }
    return len;
}


AIFStream *construct_blockmap_data(BlockMap *bm,
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
void PrintWithBlockMap(AIFStream* aif, BlockMap* bm, std::ostream& out) {
    if (!bm || !bm->isDegen()) {
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
        RangeBlockMapIterator(BlockMap* bm, const std::string& subRng);
        void begin(bool isFortran, int index0) override;
        bool next() override;
        int getDim() override;
    private:
        std::vector<BlockMap::Indexer1D> m_idcs;
        std::vector<std::pair<int,int>> m_subRng;
        std::vector<int> m_lens;
        std::vector<int> m_curIdx;
        int m_resDim = 0;
    };
}


RangeBlockMapIterator::RangeBlockMapIterator(BlockMap* bm,
                                             const std::string& subRng){
    for (int i=0; i<bm->getRank(); ++i) {
        auto d = bm->getDimension(i);
        m_idcs.push_back(BlockMap::Indexer1D(d, i));
        m_lens.push_back(d.size);
    }
    m_subRng = bm->getIndexRng(subRng);
    m_resDim = 0;
    for (auto& [lo,hi] : m_subRng) {
        if (hi > lo) ++m_resDim;
    }
}

void RangeBlockMapIterator::begin(bool isFortran, int index0) {
    BlockMap::Indexer1D::init(m_idcs, isFortran);

    // index to zero
    if (index0) {
        for (auto& [lo,hi] : m_subRng) {
            lo -= index0;
            hi -= index0;
        }
    }

    // trim the range to the available size
    int i=0;
    for (auto& [lo,hi] : m_subRng) {
        lo = std::max(0,lo);
        hi = std::min(hi,m_lens[i++]-1);
    }

    // arrange slowet to fastest changing
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


std::shared_ptr<ais::DecompIterator> MakeBlockMapIterator(BlockMap *bm,
                                                          const std::string& idx) {
    if (idx.empty()) {
        return std::make_shared<BlockMap::Iterator>(bm);
    } else {
        return std::make_shared<RangeBlockMapIterator>(bm,idx);
    }
}
