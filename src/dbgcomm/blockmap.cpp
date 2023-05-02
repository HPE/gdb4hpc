// Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
// Copyright 2010-2013,2020-2023 Hewlett Packard Enterprise Development LP.

#include "blockmap.h"
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <regex>

// Create a new map
BlockMap::BlockMap(const std::string& name)
    : m_name(name), m_oldStyle(true)
{
    if (name.size() && name[0] != '$') m_name = '$' + m_name;
}

BlockMap::BlockMap(const std::string& name, const std::string& definition)
    : m_name(name), m_oldStyle(false)
{
    if (name.size() && name[0] != '$') m_name = '$' + m_name;
    parseDefinition(definition);
}

BlockMap::~BlockMap() {}

std::string BlockMap::getDefinition() const {
    std::ostringstream os;
    os << std::quoted(m_name) << " ";
    int n = 0;
    for ( auto d : m_dims ) {
        if (n++) os << ", ";
        d.printConcise(os, m_explicitOrder);
    }
    return os.str();
}

std::unique_ptr<BlockMap> BlockMap::fromDefinition(const std::string& definition)
{
    auto is = std::istringstream{definition};
    auto name = std::string{}, dims = std::string{};
    is >> std::quoted(name);
    std::getline(is, dims);

    auto bm = std::make_unique<BlockMap>(name);
    bm->parseDefinition(dims);  // may throw exception
    return bm;
}

void BlockMap::parseDefinition(const std::string& definition) {

    static auto num = std::string{R"(\s*(\d+)\s*)"};
    static auto num0 = "^" + num;
    static auto optComma = R"(\s*,?\s*)";
    static auto order = R"((order\(\s*(\d+)\s*\))?)";
    static auto starRE = std::regex{num0 + optComma};
    static auto blockRE = std::regex{num0 + "/" + num + order + optComma};
    static auto cyclicRE = std::regex{num0 + "%" + num + order + optComma};
    static auto blockCyclicRE = std::regex{num0 + "/" + num + "%" + num + order + optComma};

    int dim_order = 1;
    auto val = [](const auto& m) { return size_t(m.length() ? std::stoi(m.str()) : 0); };
    auto oval = [this, &dim_order](const auto& m) {
                    if (m.length()) {
                        this->m_explicitOrder = true;
                        return std::stoi(m.str());
                    } else {
                        return dim_order++;
                    } };

    auto err = std::ostringstream{};
    std::smatch match;

    auto toMatch = definition;
    std::transform(toMatch.begin(), toMatch.end(), toMatch.begin(),
                   [](char c) { return std::tolower(c); });
    
    for ( ; !toMatch.empty(); toMatch = match.suffix().str()) {
        if (std::regex_search(toMatch, match, blockCyclicRE)) {
            auto len = val(match[1]), procs = val(match[2]), bc = val(match[3]);
            if ( !procs || !bc || (len % (procs * bc))) {
                err << "array length " << len << " must be a multiple of num_procs "
                    <<  procs << " * cycle " << bc;
                break;
            } else {
                m_dims.push_back(Dimension{len, Distribution::BlockCyclic, procs, bc, oval(match[5])});
            }
        } else if (std::regex_search(toMatch, match, blockRE)) {
            auto len = val(match[1]);
            auto procs = val(match[2]);
            if (!procs || (len % procs)) {
                err << "length " << len << " must be a multiple of num_procs " << procs;
                break;
            } else {
                m_dims.push_back(Dimension{len, Distribution::Block, procs, 0, oval(match[4])});
            }
        } else if (std::regex_search(toMatch, match, cyclicRE)) {
            auto len = val(match[1]);
            auto procs = val(match[2]);
            if (!procs || (len % procs)) {
                err << "length " << len << " must be a multiple of num_procs " << procs;
                break;
            } else {
                m_dims.push_back(Dimension{len, Distribution::Cyclic, procs, 0, oval(match[4])});
            }
        } else if (std::regex_search(toMatch, match, starRE)) {
            auto len = val(match[1]);
            if (!len) {
                err << "array length must be positive";
                break;
            } else {
                m_dims.push_back(Dimension{len, Distribution::Star, 1, 0, int(Distribution::Star)});
            }
        } else {
            err << "unrecognized decomposition expression";
            break;
        }
    }

    if (err.tellp()) throw std::invalid_argument(err.str());
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
            idxRngs.emplace_back(std::stoi(match[1]), std::stoi(match[2]));
        } else if (regex_search(remIdx, match, ftnElemRE)) {
            auto i = std::stoi(match[1]);
            idxRngs.emplace_back(i,i);
        } else {
            break;
        }
        remIdx = match.suffix().str();
    }
    
    // the range doesn't have all dimensions, but with a 2d array
    // in C/C++  array[0] naturally refers to the full sub-array.
    for (int i = idxRngs.size(); i<getRank(); ++i) {
        idxRngs.push_back(std::make_pair(0,m_dims[i].size-1));
    }

    return idxRngs;
}


// define the distribution in single dimension
// \param m is the block map definition
// \param d is the index for this dimension, starting at 0
// \param prev if given is the distribution for the next faster dimension
BlockMap::Indexer1D::Indexer1D(const BlockMap::Dimension& m, int d) :
    m_dimIdx(d),
    m_distribute(m.distribution),
    m_len(m.size),
    m_numProcs(std::max(m.numProcs,size_t(1))),
    m_blockCyclic(m.blockCyclic),
    m_dimOrder(m.dim_order),
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
        m_idcs.push_back(BlockMap::Indexer1D(bm->m_dims[i], i));
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

bool BlockMap::isRerank(int rank) const { return getRank() && getRank() != rank; }

void BlockMap::setDimOrder(const std::vector<int>& order) {
    auto n = order.size();
    m_dims.resize(n);
    for (size_t i=0; i<n; ++i)
        m_dims[i].dim_order = order[i];
    m_explicitOrder = true;
}

void BlockMap::setDims(const std::vector<int>& dims) {
    auto n = dims.size();
    m_dims.resize(n);
    for (size_t i=0; i<n; ++i)
        m_dims[i].size = dims[i];
}

void BlockMap::setDistribution(const std::vector<Distribution>& distribute,
                               const std::vector<int>& blockCyclic) {
    // Allocate the distribute and blockcyclic_val array inside the blockmap
    // definition
    auto n = distribute.size();
    auto nd = 0;
    m_dims.resize(n);
    for (size_t i=0; i<n; ++i) {
        auto& d = m_dims[i];
        d.distribution = distribute[i];
        // set default ordering
        d.dim_order = (distribute[i] == Distribution::Star ? int(Distribution::Star) : ++nd);
        d.blockCyclic = blockCyclic[i];
    }
}

void BlockMap::setProcGrid(const std::vector<int>& procs) {
    auto n = procs.size();
    m_dims.resize(n);
    for (size_t i=0; i<n; ++i) {
        m_dims[i].numProcs = procs[i];
    }
}

std::ostream&
BlockMap::Dimension::printConcise(std::ostream& os, bool explicitOrder) const {
    os << size;
    switch (distribution) {
    case Distribution::Star: break;
    case Distribution::Block: os << "/" << numProcs; break;
    case Distribution::Cyclic: os << "%" << numProcs; break;
    case Distribution::BlockCyclic: os << "/" << numProcs << "%" << blockCyclic; break;
    };

    if (explicitOrder && dim_order != int(Distribution::Star)) {
        os << " order(" << dim_order << ")";
    }
    return os;        

}
