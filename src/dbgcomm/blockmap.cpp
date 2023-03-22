// Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
// Copyright 2010-2013,2020-2023 Hewlett Packard Enterprise Development LP.

#include "blockmap.h"
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <regex>

// Create a new map
BlockMap::BlockMap(const std::string& name, bool debug)
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

BlockMap::~BlockMap() {
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

std::string BlockMap::getDefinition() const {
    auto rank = m_rank;
    std::ostringstream os;
    os << "(" << std::quoted(m_name) << ',' << rank;
    writeDists(os, m_distribute, rank);
    writeInts(os, m_data, rank);
    writeInts(os, m_procs, rank);
    writeInts(os, m_blockcyclic_val, rank);
    writeInts(os, m_dim_order, rank);
    os << ')';
    return os.str();
}

std::unique_ptr<BlockMap> BlockMap::fromDefinition(const std::string& definition)
{
    auto is = std::istringstream{definition};
    auto name = std::string{}; int rank = 0;
    char comma;

    is >> comma >> std::quoted(name) >> comma >> rank;
    auto bm = std::make_unique<BlockMap>(name,false);

    bm->m_rank = rank;
    bm->m_distribute = readDists(is,rank);
    bm->m_data = readInts(is,rank);
    bm->m_procs = readInts(is,rank);
    bm->m_blockcyclic_val = readInts(is,rank);
    bm->m_dim_order = readInts(is,rank);

    return bm;
}

std::vector<std::pair<int,int>> BlockMap::getIndexRng(const std::string& rng) const {
    static std::regex rngRE{R"(^\[(\d+)\.\.(\d+)\])"};
    static std::regex elemRE{R"(^\[(\d+)\])"};
    static std::regex ftnRngRE{R"(^[,(](\d+)\.\.(\d+)[)]?)"};
    static std::regex ftnElemRE{R"(^[,(](\d+)[)]?)"};

    auto idxRngs = std::vector<std::pair<int,int>>{};

    for (auto remIdx = rng; !remIdx.empty(); ) {
        auto match = std::smatch{};
        if (regex_search(remIdx, match, rngRE)) {
            idxRngs.emplace_back(std::stoi(match[1]), std::stoi(match[2]));
        } else if (regex_search(remIdx, match, elemRE)) {
            auto i = std::stoi(match[1]);
            idxRngs.emplace_back(i,i);
        } else if (regex_search(remIdx, match, ftnRngRE)) {
            idxRngs.emplace_back(std::stoi(match[1])-1, std::stoi(match[2])-1);
        } else if (regex_search(remIdx, match, ftnElemRE)) {
            auto i = std::stoi(match[1])-1;
            idxRngs.emplace_back(i,i);
        } else {
            break;
        }
        remIdx = match.suffix().str();
    }

    for (int i=0; i<m_rank; ++i) {
        auto up = m_data[i]-1;
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
BlockMap::Indexer1D::Indexer1D(const BlockMap* m, int d) :
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
void BlockMap::Indexer1D::map(size_t& dataIdx, size_t& procIdx, size_t& repeat,
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

void BlockMap::Indexer1D::setDataSize(Indexer1D* prev) {
    if (prev) {
        m_dataScale *= prev->getTotalDataSize();
    }
}

void BlockMap::Indexer1D::setProcSize(const Indexer1D& oproc) {
    if (m_distribute != Distribution::Star &&
        oproc.m_distribute != Distribution::Star &&
        m_dimOrder > oproc.m_dimOrder) {
        m_procScale *= oproc.m_numProcs;
    }
}

std::pair<int,int> BlockMap::Indexer1D::getGlobalRng(int rank) {
    int procIdx = rank / m_procScale;
    procIdx = rank % m_numProcs;

    int idx0 = m_blockLen * procIdx;
    return std::make_pair(idx0, idx0+m_blockLen-1);
}

void BlockMap::Indexer1D::init(std::vector<Indexer1D>& idcs, bool isFortran) {
    // we want the indexers ordered from slowest changing
    // to fastest varying
    if (isFortran) {
        std::reverse(idcs.begin(), idcs.end());
    }

    // initialize the indexers
    Indexer1D* prev = nullptr;
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
    
BlockMap::Iterator::Iterator(BlockMap* bm)
{
    for (int i=0; i<bm->getRank(); ++i) {
        m_idcs.push_back(BlockMap::Indexer1D(bm, i));
    }
}

void BlockMap::Iterator::begin(bool isFortran, int index0) {
    BlockMap::Indexer1D::init(m_idcs, isFortran);
    m_idx = 0;
    m_totElems = m_idcs[0].getTotalDataSize();
}

int BlockMap::Iterator::getDim() {
    return m_idcs.size();
}

// Determine which rank the next chunk of elements will be coming from.
// The computed values include which rank to read, how may consecutive
// elements are from there, and the nesting location for multi-
// dimension arrays
bool BlockMap::Iterator::next() {
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

void BlockMap::Iterator::reset() { m_idx = 0; }

void BlockMap::Iterator::getIdcs(std::vector<int>& idcs) const {
    idcs.resize(m_idcs.size());

    // this is meant to be called after "next" to refer back to the
    auto val = m_idx - len;

    for ( auto i=m_idcs.size(); i--; ) {
        auto len = m_idcs[i].getLen();
        idcs[i] = val % len;
        val /= len;
    }
}

bool BlockMap::isRerank(int rank) const { return m_rank && m_rank != rank; }

void BlockMap::setDimOrder(const std::vector<int>& order) {
    m_rank = order.size();
    m_dim_order = new int[m_rank];
    copy(order.begin(), order.end(), m_dim_order);
}

void BlockMap::setDims(const std::vector<int>& dims) {
    m_rank = dims.size();
    m_data = new int[m_rank];
    std::copy(dims.begin(), dims.end(), m_data);
}

void BlockMap::setDistribution(const std::vector<Distribution>& distribute,
                               const std::vector<int>& blockCyclic) {
    // Allocate the distribute and blockcyclic_val array inside the blockmap
    // definition
    auto rank = distribute.size();
    m_rank = rank;
    m_distribute = new Distribution[rank];
    m_blockcyclic_val = new int[rank];

    copy(distribute.begin(), distribute.end(), m_distribute);
    copy(blockCyclic.begin(), blockCyclic.end(), m_blockcyclic_val);
}

void BlockMap::setProcGrid(const std::vector<int>& procs) {
    m_rank = procs.size();
    m_procs = new int[m_rank];
    std::copy(procs.begin(), procs.end(), m_procs);
}

