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

LrleBitset::LrleBitset(size_t nbits)
    : m_nbits{nbits}
{}

LrleBitset::LrleBitset(size_t nbits, size_t setBit)
    : LrleBitset(nbits)
{
    setFirstBit(setBit);
}

LrleBitset::LrleBitset(size_t nbits, unsigned const char *buf, size_t len) 
    : m_nbits{nbits}
    , m_compressed(buf, buf + len)
{}

LrleBitset::LrleBitset(const Serialized &bitset) {
    std::istringstream stream(bitset.get());

    auto start = stream.tellg();

    stream >> std::hex >> m_nbits;

    size_t memSize;
    stream >> std::hex >> memSize;

    assert(stream.get() == '/');

    auto read = stream.tellg() - start;

    m_compressed = std::vector<unsigned char>(bitset.get() + read, bitset.get() + read + memSize);
}

std::unique_ptr<Bitset> LrleBitset::clone() const {
    auto r = new LrleBitset(m_nbits, m_compressed.data(), m_compressed.size());
    return std::unique_ptr<LrleBitset>(r);
}

bool LrleBitset::isEmpty() const {
    return lrle_empty(m_compressed.data(), m_compressed.size());
}

bool LrleBitset::isFull() const {
    return lrle_count(m_compressed.data(), m_compressed.size()) == m_nbits;
}

void LrleBitset::set(size_t bit) {
    if (bit < 0 || bit >= m_nbits) return;

    // create new set with single bit
    auto rhs = LrleBitset(m_nbits, bit);
    *this |= rhs;
}

void LrleBitset::unset(size_t bit) {
    // we have no easy way to do this with the current api and a lack of invert()
    throw std::runtime_error("unsupported lrle bitset operation: unset()");
}

bool LrleBitset::test(size_t bit) const {
    if (bit < 0 || bit >= m_nbits) return false;

    return lrle_contains(m_compressed.data(), m_compressed.size(), bit);
}

void LrleBitset::invert() {
    // we have no easy way to do this with the current api
    throw std::runtime_error("unsupported lrle bitset operation: invert()");
}

void LrleBitset::clear() {
    m_compressed.clear();
}

size_t LrleBitset::numSet() const {
    return lrle_count(m_compressed.data(), m_compressed.size());
}

size_t LrleBitset::firstSet() const {
    auto bit = lrle_next_set_bit(m_compressed.data(), m_compressed.size(), nullptr);
    if (bit < 0) return Bitset::None;
    return bit;
}

size_t LrleBitset::sizeInBits() const {
    return m_nbits;
}

std::string LrleBitset::toString() const {
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

size_t LrleBitset::getSerializedSize() const {
    // serialized format: "<num bits> <m_compressed.size()>/<raw m_compressed data>" 
    // all numbers in base 16.

    auto numHexDigits = [](size_t x) -> size_t { 
        if (x == 0) return 1; // '0' is one digit
        return (size_t)(std::log(x) / std::log(16)) + 1; 
    };

    //                <num bits>             ' '  <m_compressed.size()>              '/'
    auto headerSize = numHexDigits(m_nbits) + 1 + numHexDigits(m_compressed.size()) + 1;

    return headerSize + m_compressed.size() + 1; // + 1 for '\0'
}

Bitset::Serialized LrleBitset::serialize() const {
    // lrle bitsets are stored in serialized format, we just need to add metadata
    auto result = new char[getSerializedSize()];
    
    // metadata header
    std::ostringstream header;
    header << std::hex << m_nbits << " "
           << std::hex << m_compressed.size() << "/"
    ;

    // + 1 because snprintf will truncate with a \0 if exact length is given
    snprintf(result, header.str().size() + 1, "%s", header.str().c_str());

    // memcpy since data might validly contain '\0'
    memcpy(result + header.str().size(), m_compressed.data(), m_compressed.size());
    
    result[getSerializedSize() - 1] = '\0';
    return Bitset::Serialized(result);
}

Bitset& LrleBitset::operator=(const Bitset &_other) {
    auto &other = dynamic_cast<const LrleBitset&>(_other);

    m_nbits = other.m_nbits;
    m_compressed = other.m_compressed;

    return *this;
}

Bitset& LrleBitset::operator&=(const Bitset &_other) {
    auto &other = dynamic_cast<const LrleBitset&>(_other);

    m_nbits = std::max(m_nbits, other.m_nbits);
    m_compressed = std::move(andBuf(other.m_compressed.data(), other.m_compressed.size()));

    return *this;
}

Bitset& LrleBitset::operator|=(const Bitset &_other) {
    auto &other = dynamic_cast<const LrleBitset&>(_other);

    m_nbits = std::max(m_nbits, other.m_nbits);
    m_compressed = std::move(orBuf(other.m_compressed.data(), other.m_compressed.size()));

    return *this;
}

bool LrleBitset::operator==(const Bitset &_other) const {
    auto &other = dynamic_cast<const LrleBitset&>(_other);

    if (m_nbits != other.m_nbits) return false;

    if (m_compressed == other.m_compressed) {
        return true;
    } else {
        // it's possible that we are dealing with two empty bitsets encoded in
        // different but equal ways. e.g. a bitset with a 0 byte buffer is empty,
        // but so is a 4 byte buffer of "00 04 00 00" (a 4 byte run of zeros)

        return isEmpty() == true && other.isEmpty() == true;
    }
}

bool LrleBitset::operator!=(const Bitset &other) const {
    return !(*this == other);
}

bool LrleBitset::hasCommonBits(const Bitset &_other) const {
    auto &other = dynamic_cast<const LrleBitset&>(_other);

    return lrle_any_intersection(m_compressed.data(), m_compressed.size(),
        other.m_compressed.data(), other.m_compressed.size());
}

void LrleBitset::setFirstBit(size_t bit) {
    auto size = lrle_single_bit_size(bit);
    unsigned char buf[size];

    auto const m_cap = lrle_encode_single_bit(bit, buf, size);
    if (m_cap < 0) {
        throw std::runtime_error("lrle single bit encode failed");
    }

    m_compressed = std::vector<unsigned char>{buf, buf + m_cap};
}

std::vector<unsigned char> LrleBitset::andBuf(unsigned char const* rhs, size_t rhs_len) const {
    unsigned char buf[(m_compressed.size() + rhs_len) * CHUNK_SIZE];

    auto const len = lrle_and_default(buf, sizeof(buf),
        m_compressed.data(), m_compressed.size(), rhs, rhs_len);
    if (len < 0) {
        throw std::runtime_error("lrle and failed");
    }

    return std::vector<unsigned char>(buf, buf + (size_t)len);
}

std::vector<unsigned char> LrleBitset::orBuf(unsigned char const* rhs, size_t rhs_len) const {
    unsigned char buf[(m_compressed.size() + rhs_len) * CHUNK_SIZE];

    auto const len = lrle_or_default(buf, sizeof(buf),
        m_compressed.data(), m_compressed.size(), rhs, rhs_len);
    if (len < 0) {
        throw std::runtime_error("lrle or failed");
    }

    return std::vector<unsigned char>(buf, buf + (size_t)len);
}
