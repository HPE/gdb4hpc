/******************************************************************************\
 * Copyright 2021 Hewlett Packard Enterprise Development LP.
 ******************************************************************************/

#include "lrunle_bitset.h"
#include "lrle_bitset.h" // the reason for the weird file name!
#include <sstream>
#include <string.h>
#include <assert.h>
#include <cmath>

#include <iostream>

LRunLEBitset::LRunLEBitset(size_t nbits)
    : m_nbits{nbits}
{}

LRunLEBitset::LRunLEBitset(size_t nbits, size_t setBit)
    : LRunLEBitset(nbits)
{
    setFirstBit(setBit);
}

LRunLEBitset::LRunLEBitset(size_t nbits, unsigned const char *buf, size_t len) 
    : m_nbits{nbits}
    , m_compressed(buf, buf + len)
{}

LRunLEBitset::LRunLEBitset(SerializedPtr bitset) {
    std::istringstream stream(bitset);

    auto start = stream.tellg();

    stream >> std::hex >> m_nbits;

    size_t memSize;
    stream >> std::hex >> memSize;

    assert(stream.get() == '/');

    auto read = stream.tellg() - start;

    m_compressed = std::vector<unsigned char>(bitset + read, bitset + read + memSize);
}

std::unique_ptr<Bitset> LRunLEBitset::clone() const {
    auto r = new LRunLEBitset(m_nbits, m_compressed.data(), m_compressed.size());
    return std::unique_ptr<LRunLEBitset>(r);
}

std::unique_ptr<Bitset> LRunLEBitset::clone(size_t newNumBits) const {
    if (newNumBits < m_nbits) {
        // efficient shrinking lrle bitsets is not possible with current api
        auto r = std::make_unique<LRunLEBitset>(newNumBits);
        for (size_t i = 0; i < m_nbits; i++) {
            if (test(i)) r->set(i);
        }
        return r;
    } else {
        return std::make_unique<LRunLEBitset>(newNumBits, m_compressed.data(), m_compressed.size());
    }
}

bool LRunLEBitset::isEmpty() const {
    return lrle_empty(m_compressed.data(), m_compressed.size());
}

bool LRunLEBitset::isFull() const {
    return lrle_count(m_compressed.data(), m_compressed.size()) == m_nbits;
}

void LRunLEBitset::set(size_t bit) {
    if (bit < 0 || bit >= m_nbits) return;

    // create new set with single bit
    auto rhs = LRunLEBitset(m_nbits, bit);
    *this |= rhs;
}

void LRunLEBitset::unset(size_t bit) {
    // we have no easy way to do this with the current api and a lack of invert()
    throw std::runtime_error("unsupported lrle bitset operation: unset()");
}

bool LRunLEBitset::test(size_t bit) const {
    if (bit < 0 || bit >= m_nbits) return false;

    return lrle_contains(m_compressed.data(), m_compressed.size(), bit);
}

void LRunLEBitset::invert() {
    // we have no easy way to do this with the current api
    throw std::runtime_error("unsupported lrle bitset operation: invert()");
}

void LRunLEBitset::clear() {
    m_compressed.clear();
}

size_t LRunLEBitset::numSet() const {
    return lrle_count(m_compressed.data(), m_compressed.size());
}

size_t LRunLEBitset::firstSet() const {
    auto bit = lrle_next_set_bit(m_compressed.data(), m_compressed.size(), nullptr);
    if (bit < 0) return Bitset::None;
    return bit;
}

size_t LRunLEBitset::sizeInBits() const {
    return m_nbits;
}

std::string LRunLEBitset::toString() const {
    auto hb_to_hex = [](unsigned char const halfbyte) {
        if (halfbyte < 10U) {
            return (char)('0' + halfbyte);
        } else {
            return (char)('a' + (halfbyte - 10U));
        }
    };
    std::string result;
    result.reserve(m_compressed.size() * 2);
    for (size_t i = 0; i < m_compressed.size(); i++) {
        result.push_back(hb_to_hex(m_compressed[i] >> 4));
        result.push_back(hb_to_hex(m_compressed[i] & 0x0f));
        result.push_back(' ');
    }
    return result;
}

Bitset::Serialized LRunLEBitset::serialize() const {
    // lrle bitsets are stored in serialized format, we just need to add metadata
    
    // metadata header
    std::ostringstream headerStream;
    headerStream << std::hex << m_nbits << " "
                 << std::hex << m_compressed.size() << "/"
    ;
    auto header = headerStream.str();

    auto size = header.size() + m_compressed.size() + 1; // +1 for \0
    auto result = (char*)malloc(sizeof(char) * size);

    // memcpy since m_compressed might validly contain a \0 byte
    // header
    memcpy(result, header.c_str(), header.size());
    // data
    memcpy(result + header.size(), m_compressed.data(), m_compressed.size());
    result[size - 1] = '\0';

    return Bitset::Serialized(result, std::free);
}

size_t LRunLEBitset::extractSerializedSize(const Serialized &data) {
    std::istringstream stream(data.get());

    auto start = stream.tellg();

    size_t nbits;
    stream >> std::hex >> nbits;

    size_t memSize;
    stream >> std::hex >> memSize;

    assert(stream.get() == '/');

    return (stream.tellg() - start) + memSize;
}

Bitset& LRunLEBitset::operator=(const Bitset &other) {
    auto LRLEother = dynamic_cast<const LRunLEBitset*>(&other);
    if (!LRLEother) return Bitset::operator=(other);

    m_nbits = LRLEother->m_nbits;
    m_compressed = LRLEother->m_compressed;

    return *this;
}

Bitset& LRunLEBitset::operator&=(const Bitset &other) {
    auto LRLEother = dynamic_cast<const LRunLEBitset*>(&other);
    if (!LRLEother) return Bitset::operator&=(other);

    m_nbits = std::max(m_nbits, LRLEother->m_nbits);
    m_compressed = std::move(andBuf(LRLEother->m_compressed.data(), LRLEother->m_compressed.size()));

    return *this;
}

Bitset& LRunLEBitset::operator|=(const Bitset &other) {
    auto LRLEother = dynamic_cast<const LRunLEBitset*>(&other);
    if (!LRLEother) return Bitset::operator|=(other);

    m_nbits = std::max(m_nbits, LRLEother->m_nbits);
    m_compressed = std::move(orBuf(LRLEother->m_compressed.data(), LRLEother->m_compressed.size()));

    return *this;
}

bool LRunLEBitset::operator==(const Bitset &other) const {
    auto LRLEother = dynamic_cast<const LRunLEBitset*>(&other);
    if (!LRLEother) return Bitset::operator==(other);

    if (m_nbits != LRLEother->m_nbits) return false;

    if (m_compressed == LRLEother->m_compressed) {
        return true;
    } else {
        // it's possible that we are dealing with two empty bitsets encoded in
        // different but equal ways. e.g. a bitset with a 0 byte buffer is empty,
        // but so is a 4 byte buffer of "00 04 00 00" (a 4 byte run of zeros)

        return isEmpty() == true && LRLEother->isEmpty() == true;
    }
}

bool LRunLEBitset::operator!=(const Bitset &other) const {
    return !(*this == other);
}

bool LRunLEBitset::hasCommonBits(const Bitset &other) const {
    auto LRLEother = dynamic_cast<const LRunLEBitset*>(&other);
    if (!LRLEother) return Bitset::hasCommonBits(other);

    return lrle_any_intersection(m_compressed.data(), m_compressed.size(),
        LRLEother->m_compressed.data(), LRLEother->m_compressed.size());
}

void LRunLEBitset::setFirstBit(size_t bit) {
    auto size = lrle_single_bit_size(bit);
    unsigned char buf[size];

    auto const m_cap = lrle_encode_single_bit(bit, buf, size);
    if (m_cap < 0) {
        throw std::runtime_error("lrle single bit encode failed");
    }

    m_compressed = std::vector<unsigned char>{buf, buf + m_cap};
}

std::vector<unsigned char> LRunLEBitset::andBuf(unsigned char const* rhs, size_t rhs_len) const {
    auto buf = std::vector<unsigned char>((m_compressed.size() + rhs_len) * CHUNK_SIZE);

    auto const len = lrle_and_default(buf.data(), buf.size(),
        m_compressed.data(), m_compressed.size(), rhs, rhs_len);
    if (len < 0) {
        throw std::runtime_error("lrle and failed");
    }

    buf.resize(len);
    return buf;
}

std::vector<unsigned char> LRunLEBitset::orBuf(unsigned char const* rhs, size_t rhs_len) const {
    auto buf = std::vector<unsigned char>((m_compressed.size() + rhs_len) * CHUNK_SIZE);

    auto const len = lrle_or_default(buf.data(), buf.size(),
        m_compressed.data(), m_compressed.size(), rhs, rhs_len);
    if (len < 0) {
        throw std::runtime_error("lrle or failed");
    }

    buf.resize(len);
    return buf;
}

using SetBitsIterator = LRunLEBitset::SetBitsIterator;

SetBitsIterator::SetBitsIterator(const LRunLEBitset &bits) 
    : m_bits{bits}
    , m_context(new lrle_context)
    , m_current_bit{0}
{ 
    reset_lrle_context(m_context.get());
    ++*this;
}

SetBitsIterator::SetBitsIterator(const LRunLEBitset &bits, SetBitsIterator::End end)
    : m_bits{bits}
    , m_context(nullptr)
    , m_current_bit{m_bits.m_nbits}
{
    // used for LRunLEBitset::end()
}

// we need to declare the default destructor here, in the .cpp file, since the
// header file doesn't include enough other headers to know how to destroy a 
// unique_ptr to an lrle_context and generate the default destructor. 
//
// we do this to let files include lrunle_bitset.h without needing to know where lrle_bitset.h is.
SetBitsIterator::~SetBitsIterator() = default;

SetBitsIterator& SetBitsIterator::operator++() {
    if (m_current_bit >= m_bits.sizeInBits()) return *this;

    auto next_bit = lrle_next_set_bit(m_bits.m_compressed.data(), m_bits.m_compressed.size(), m_context.get());

    if (next_bit >= 0) {
        m_current_bit = next_bit;
    } else {
        m_current_bit = m_bits.sizeInBits();
    }

    return *this;
}