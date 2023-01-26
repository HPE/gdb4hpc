// Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
// Copyright 2010-2013,2020-2023 Hewlett Packard Enterprise Development LP.

#include "dbgblockmap.h"
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <regex>

// Create a new map
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

static void writeDists(std::ostream& os, const Distribution* vals, int dim) {
    os << ",(";
    for (int i=0; i<dim; ++i) {
        const char* s = nullptr;
        switch (vals[i]) {
        case Distribution::Star: s = "Star"; break;
        case Distribution::Block: s = "Block"; break;
        case Distribution::Cyclic: s = "Cyclic"; break;
        case Distribution::BlockCyclic: s = "BlockCyclic"; break;
        }
        os << (i ? "," : "") << s;
    }
    os << ')';
}
static Distribution* readDists(std::istream& is, int dim) {
    // I know: shockingly primitive
    auto d = new Distribution[dim];
    is.get(); is.get(); // ,(
    for (int i=0; i<dim; ++i) {
        auto type = std::string{};
        for (int c; (c=is.get()) && c != EOF && c != ',' && c != ')'; ) {
            type += c;
        }
        d[i] = Distribution::Star;
        if (type == "Block") d[i] = Distribution::Block;
        else if (type == "Cyclic") d[i] = Distribution::Cyclic;
        else if (type == "BlockCyclic") d[i] = Distribution::BlockCyclic;
    }
    return d;
}

static void writeInts(std::ostream& os, const int* vals, int dim) {
    os << ",(";
    for (int i=0; i<dim; ++i) {
        os << (i ? "," : "") << vals[i];
    }
    os << ')';
}

static int* readInts(std::istream& is, int dim) {
    auto v = new int[dim];
    char sep;
    is >> sep >> sep; // ,(
    for (int i=0; i<dim; ++i) {
        is >> v[i] >> sep;
    }
    return v;
}

std::string GetBlockMapDefinition(g_block_map_t* bm) {
    auto rank = bm->m_rank;
    std::ostringstream os;
    os << "(" << std::quoted(bm->m_name) << ',' << rank;
    writeDists(os, bm->m_distribute, rank);
    writeInts(os, bm->m_data, rank);
    writeInts(os, bm->m_procs, rank);
    writeInts(os, bm->m_blockcyclic_val, rank);
    writeInts(os, bm->m_dim_order, rank);
    os << ')';
    return os.str();
}

g_block_map_t* MakeBlockMapFromDefinition(const std::string& definition)
{
    auto is = std::istringstream{definition};
    auto name = std::string{}; int rank = 0;
    char comma;

    is >> comma >> std::quoted(name) >> comma >> rank;
    auto bm = new g_block_map_t(name,false);

    bm->m_rank = rank;
    bm->m_distribute = readDists(is,rank);
    bm->m_data = readInts(is,rank);
    bm->m_procs = readInts(is,rank);
    bm->m_blockcyclic_val = readInts(is,rank);
    bm->m_dim_order = readInts(is,rank);

    return bm;
}

std::vector<int> BlockMapGetDimPerRank(g_block_map_t* m) {
    auto dims = std::vector<int>{};
    for (int i=0; i<m->m_rank; ++i) {
        auto dim = m->m_data[i];
        if (m->m_distribute[i] != Distribution::Star &&
            m->m_procs[i] > 0) {
            // there's no reason m_procs[i] would be zero,
            // just being careful
            dim = (m->m_data[i] / m->m_procs[i]);
        }
        dims.push_back(dim);
    }
    return dims;
}

std::vector<std::pair<int,int>> BlockMapGetIndexRng(g_block_map_t* m,
                                                    const std::string& rng) {
    static std::regex rngRE(R"(^\[(\d+)\.\.(\d+)\])");
    static std::regex elemRE(R"(^\[(\d+)\])");

    auto idxRngs = std::vector<std::pair<int,int>>{};

    for (auto remIdx = rng; !remIdx.empty(); ) {
        auto match = std::smatch{};
        if (regex_search(remIdx, match, rngRE)) {
            idxRngs.push_back(std::make_pair(std::stoi(match[1]),
                                             std::stoi(match[2])));
        } else if (regex_search(remIdx, match, elemRE)) {
            auto i = std::stoi(match[1]);
            idxRngs.push_back(std::make_pair(i,i));
        } else {
            break;
        }
        remIdx = match.suffix().str();
    }

    for (int i=0; i<m->m_rank; ++i) {
        auto up = m->m_data[i]-1;
        if (size_t(i) >= idxRngs.size()) {
            // the range doesn't have all dimensions, but with a 2d array
            // in C/C++  array[0] naturally refers to the full sub-array.
            idxRngs.push_back(std::make_pair(0,up));
        } else {
            auto& [lo,hi] = idxRngs[i];
            lo = std::max(0,lo);
            hi = std::min(up,hi);
        }
    }

    return idxRngs;
}


// define the distribution in single dimension
// \param m is the block map definition
// \param d is the index for this dimension, starting at 0
// \param prev if given is the distribution for the next faster dimension
blockmap::indexer1D::indexer1D(const g_block_map_t* m, int d) :
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

// map an index in the global array into the offsets into the
// array of process ranks and the data array from that rank
// \param idx is the index of the global array in this dimension,
//      it always has origin 0
// \param procIdx i/o is updated with the offset into the array of ranks
//      it is unchanged if this dimension is not decomposed
// \param repeat will contain on exit the number of elements in to row.
//      note that the result will only be used for the fastest varying
//      dimension because \a repeat elements will be contiguously stored
//      in the data result for that rank so they can be copied in one shot
void blockmap::indexer1D::map(size_t& dataIdx, size_t& procIdx, size_t& repeat,
                    bool& atEnd) const {
    repeat = 1;
    size_t dproc = 0;

    auto idx = dataIdx / m_dataScale;
    
    switch (m_distribute) {
    case Distribution::Block:
        dproc = idx / m_blockLen;
        repeat = m_blockLen - idx % m_blockLen;
        break;
    case Distribution::Cyclic:
        dproc = idx % m_numProcs;
        break;
    case Distribution::Star:
        dproc = 0;
        repeat = m_len - idx % m_len;
        break;
    case Distribution::BlockCyclic:
        dproc = (idx / m_blockCyclic) % m_numProcs;
        repeat = m_blockCyclic - idx % m_blockCyclic;
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

void blockmap::indexer1D::setDataSize(indexer1D* prev) {
    if (prev) {
        m_dataScale *= prev->getTotalDataSize();
    }
}

void blockmap::indexer1D::setProcSize(const indexer1D& oproc) {
    if (m_distribute != Distribution::Star &&
        oproc.m_distribute != Distribution::Star &&
        m_dimOrder > oproc.m_dimOrder) {
        m_procScale *= oproc.m_numProcs;
    }
}

std::pair<int,int> blockmap::indexer1D::getGlobalRng(int rank) {
    int procIdx = rank / m_procScale;
    procIdx = rank % m_numProcs;

    int idx0 = m_blockLen * procIdx;
    return std::make_pair(idx0, idx0+m_blockLen-1);
}

void blockmap::indexer1D::init(std::vector<indexer1D>& idcs, bool isFortran) {
    // we want the indexers ordered from slowest changing
    // to fastest varying
    if (isFortran) {
        std::reverse(idcs.begin(), idcs.end());
    }

    // initialize the indexers
    indexer1D* prev = nullptr;
    for (auto it=idcs.rbegin(); it!=idcs.rend(); ++it) {
        it->setDataSize(prev);
        prev = &*it;
    }

    for (auto&& a : idcs) {
        for (auto&& b : idcs) {
            a.setProcSize(b);
        }
    }
}
    
blockmap::BlockMapIterator::BlockMapIterator(g_block_map_t* bm)
{
    for (int i=0; i<bm->m_rank; ++i) {
        m_idcs.push_back(blockmap::indexer1D(bm, i));
    }
}

void blockmap::BlockMapIterator::begin(bool isFortran) {
    blockmap::indexer1D::init(m_idcs, isFortran);
    m_idx = 0;
    m_totElems = m_idcs[0].getTotalDataSize();
}

int blockmap::BlockMapIterator::getDim() {
    return m_idcs.size();
}

// Determine which rank the next chunk of elements will be coming from.
// The computed values include which rank to read, how may consecutive
// elements are from there, and the nesting location for multi-
// dimension arrays
bool blockmap::BlockMapIterator::next() {
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

void blockmap::BlockMapIterator::reset() { m_idx = 0; }

void blockmap::BlockMapIterator::getIdcs(std::vector<int>& idcs) const {
    idcs.resize(m_idcs.size());

    // this is meant to be called after "next" to refer back to the
    auto val = m_idx - len;

    for ( auto i=m_idcs.size(); i--; ) {
        auto len = m_idcs[i].getLen();
        idcs[i] = val % len;
        val /= len;
    }
}
