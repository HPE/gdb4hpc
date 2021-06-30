/******************************************************************************\
 * Copyright 2021 Hewlett Packard Enterprise Development LP.
 ******************************************************************************/

#include "lrunle_bitset.h"
#include "lrle_bitset.h" // the reason for the silly file name!
#include "bitset_serialize.h"
#include <sstream>
#include <string.h>

LrleBitset::LrleBitset(size_t nbits)
    : m_nbits{nbits}
{}

LrleBitset::LrleBitset(size_t nbits, unsigned int setBit)
    : LrleBitset(nbits)
{
    setFirstBit(setBit);
}

LrleBitset::LrleBitset(size_t nbits, unsigned const char *buf, size_t len) 
    : m_nbits{nbits}
    , m_compressed(buf, buf + len)
{}

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

void LrleBitset::set(int bit) {
    if (bit < 0 || (unsigned int)bit >= m_nbits) return;

    // create new set with single bit
    auto rhs = LrleBitset(m_nbits, bit);
    *this |= rhs;
}

bool LrleBitset::test(int bit) const {
    if (bit < 0 || (unsigned int)bit >= m_nbits) return false;

    return lrle_contains(m_compressed.data(), m_compressed.size(), bit);
}

void LrleBitset::clear() {
    m_compressed.clear();
}

unsigned int LrleBitset::numSet() const {
    return lrle_count(m_compressed.data(), m_compressed.size());
}

int LrleBitset::firstSet() const {
    return lrle_next_set_bit(m_compressed.data(), m_compressed.size(), nullptr);
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

Bitset::SerializedPtr LrleBitset::serialize() const {
    // lrle bitsets are stored in serialized format, we just need to add metadata
    std::stringstream formatStr;

    formatStr << std::hex << static_cast<unsigned int>(bitset::Format::Lrle) << ";" // format
              << std::hex << m_nbits << ";" // size
              << std::hex << m_compressed.size() << ";" // data size
    ;
    auto resultLen = formatStr.str().size() + m_compressed.size() + 1; // +1 for \0
    auto result = (char *)malloc(sizeof(char) * resultLen);

    // format string
    snprintf(result, formatStr.str().size(), "%s", formatStr.str().c_str());

    memcpy(result + formatStr.str().size(), m_compressed.data(), m_compressed.size());
    result[resultLen - 1] = '\0';

    return bitset::SerializedPtr(result, std::free);
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

    return m_nbits == other.m_nbits && m_compressed == other.m_compressed;
}

bool LrleBitset::operator!=(const Bitset &other) const {
    return !(*this == other);
}

bool LrleBitset::hasCommonBits(const Bitset &_other) const {
    auto &other = dynamic_cast<const LrleBitset&>(_other);

    return lrle_any_intersection(m_compressed.data(), m_compressed.size(),
        other.m_compressed.data(), other.m_compressed.size());
}

void LrleBitset::setFirstBit(unsigned int bit) {
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
