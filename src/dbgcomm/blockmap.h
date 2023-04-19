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

class BlockMap {
public:    
    BlockMap(const std::string& name);
    BlockMap(const std::string& name, const std::string& definition);
    ~BlockMap();

    const std::string& getName() const { return m_name; }

    int getRank() const { return m_dims.size(); }
    //const int* getDimOrder() const;
    //const int* getDims();
    //const Distribution *getDistribution();
    const int* getProcGrid();

    // check that the block map is valid
    bool verify();

    // print out the blockmap in user readable form
    std::string msg() const;

    // a blockmap is degenerate if it's fully distributed
    bool isDegen() const;

    // get the number of elements for each rank
    size_t getElementsPerRank() const;

    // translate a ranged array reference to min,max ranges
    std::vector<std::pair<int,int>> getIndexRng(const std::string& rng) const;

    // translate a blockmap to and from a string value
    std::string getDefinition() const;
    static std::unique_ptr<BlockMap> fromDefinition(const std::string& definition);

    // modifiers
    void setDims(const std::vector<int>& dims);
    void setDistribution(const std::vector<Distribution>& dist,
                         const std::vector<int>& blockCyclic);
    void setDimOrder(const std::vector<int>& order);
    void setProcGrid(const std::vector<int>& procs);

    bool isRerank(int dim) const;

    class Indexer1D;
    class Iterator;

    struct Dimension {
        size_t size;
        Distribution distribution;
        size_t numProcs;
        size_t blockCyclic;
        int dim_order;

        std::ostream& printConcise(std::ostream& s) const;
    };

    const Dimension& getDimension(int i) const { return m_dims[i]; }
        
private:
    void parseDefinition(const std::string& definition);
    std::string m_name;

    std::vector<Dimension> m_dims;
};

// parse a range defined as [min..max][val] into ranges

// handle the data distribution in a single dimension
class BlockMap::Indexer1D {
public:
    Indexer1D(const BlockMap::Dimension& m, int dimIdx);
    Indexer1D() : m_dimIdx(0), m_len(0) {}
    void map(size_t& dataIdx, size_t& procIdx, size_t& repeat, bool& atEnd) const;
    
    size_t getTotalProcSize() const { return m_totalProcSize; }
    size_t getTotalDataSize() const { return m_dataScale * m_len; }
    int getDimIdx() const { return m_dimIdx; }
    size_t getLen() const { return m_len; }

    void setDataSize(Indexer1D* prev);
    void setProcSize(const Indexer1D& oproc);

    std::pair<int,int> getGlobalRng(int rank);

    static void init(std::vector<Indexer1D>& idcs, bool isFortran);

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

class BlockMap::Iterator : public ais::DecompIterator {
public:
    Iterator(BlockMap* bm);
    void begin(bool isFortran, int index0) override;
    bool next() override;
    int getDim() override;

    void getIdcs(std::vector<int>& idcs) const;
    void reset();
protected:
    size_t m_idx;
    size_t m_totElems;
    std::vector<Indexer1D> m_idcs;
};

