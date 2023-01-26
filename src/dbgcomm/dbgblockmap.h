/*********************************************************************************
 This is the part of the blockmap data that is shared between the front and back
 ends.  

 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * Copyright 2010-2013,2020-2023 Hewlett Packard Enterprise Development LP.
 */
#pragma once

#include <string>
#include <vector>
#include "ais.h"

enum class Distribution { Star=1000, Block, Cyclic, BlockCyclic };

struct g_block_map_t {
    g_block_map_t(const std::string& name, bool debug);
    ~g_block_map_t();
    
    std::string m_name;
    int  m_rank;              // store the rank of the blockmap function
    Distribution *m_distribute;  // store the mode of the block, cyclic
    int *m_data;             // store the upperbound of the dimensions
    int *m_procs;            // store the process grid
    int *m_blockcyclic_val;  // store block-cyclic value
    int *m_dim_order;        // store dimension order of mapping
    bool m_debug;
};

g_block_map_t* MakeBlockMapFromDefinition(const std::string& definition);
extern std::string GetBlockMapDefinition(g_block_map_t* bm);

std::vector<int> BlockMapGetDimPerRank(g_block_map_t* m);

// parse a range defined as [min..max][val] into ranges
std::vector<std::pair<int,int>> BlockMapGetIndexRng(g_block_map_t* m,
                                                    const std::string& rng);

namespace blockmap {
// handle the data distribution in a single dimension
class indexer1D {
public:
    indexer1D(const g_block_map_t* m, int dimIdx);
    indexer1D() : m_dimIdx(0), m_len(0) {}
    void map(size_t& dataIdx, size_t& procIdx, size_t& repeat, bool& atEnd) const;
    
    size_t getTotalProcSize() const { return m_totalProcSize; }
    size_t getTotalDataSize() const { return m_dataScale * m_len; }
    int getDimIdx() const { return m_dimIdx; }
    size_t getLen() const { return m_len; }

    void setDataSize(indexer1D* prev);
    void setProcSize(const indexer1D& oproc);

    std::pair<int,int> getGlobalRng(int rank);

    static void init(std::vector<indexer1D>& idcs, bool isFortran);

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

class BlockMapIterator : public ais::DecompIterator {
public:
    BlockMapIterator(g_block_map_t* bm);
    void begin(bool isFortran) override;
    bool next() override;
    int getDim() override;

    void getIdcs(std::vector<int>& idcs) const;
    void reset();
protected:
    size_t m_idx;
    size_t m_totElems;
    std::vector<blockmap::indexer1D> m_idcs;
};

}

