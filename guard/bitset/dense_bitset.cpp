/******************************************************************************\
 * Copyright 2021 Hewlett Packard Enterprise Development LP.
 ******************************************************************************/
#include "dense_bitset.h"

#include <string.h>
#include <sstream>
#include <algorithm>

DenseBitset::DenseBitset(size_t nbits)
	: m_bs_nbits{nbits}
	, m_bs_bits{}
{
	if (nbits != 0) {
		m_bs_bits = std::vector<Bits>(BIT_INDEX(nbits-1) + 1, 0);
	}
}

DenseBitset::DenseBitset(const Serialized &serialized) {
	if (serialized.get() == nullptr) {
		throw std::runtime_error("nullptr passed to DenseBitset constructor.");
	}

	auto digitToInt = [](int c) {
		if (c >= '0' && c <= '9')
			return c - '0';
		if (c >= 'a' && c <= 'f')
			return c - 'a' + 10;
		if (c >= 'A' && c <= 'F')
			return c - 'A' + 10;
		
		return 0;
	};

	std::istringstream converter(serialized.get());

	int nbits;
	converter >> std::hex >> nbits;

	std::string data;
	converter >> data;

	m_bs_nbits = nbits;
	m_bs_bits = std::vector<Bits>(BIT_INDEX(nbits - 1) + 1, 0);

	int bytes = NUM_BYTES(nbits);
	const char *dataPtr = data.c_str();
	for (int pos = (bytes * 8) - 1; *dataPtr != '\0' && isxdigit(*dataPtr) && pos >= 0; dataPtr++) {
		int b = digitToInt(*dataPtr);
		for (int n = 3; n >= 0; n--, pos--) {
			if (b & (1 << n)) {
				set(pos);
			}
		}
	}
}

std::unique_ptr<Bitset> DenseBitset::clone() const {
	return typedClone();
}

std::unique_ptr<DenseBitset> DenseBitset::typedClone() const {
	auto r = new DenseBitset(m_bs_nbits);
	*r = *this;
	return std::unique_ptr<DenseBitset>(r);
}

bool DenseBitset::isEmpty() const {
	return !std::any_of(m_bs_bits.begin(), m_bs_bits.end(), 
		[](Bits bits) {
			return bits != 0;
		});
}

bool DenseBitset::isFull() const {
	auto invNotZero = [](Bits bits){return ~bits != 0;};

	// use bit operations for everything until the last chunk
	if (std::find_if(m_bs_bits.begin(), m_bs_bits.end() - 1, invNotZero) != m_bs_bits.end() - 1) {
		return false;
	}

	// last chunk might not be fully used, so test individually
	auto starting_bit = (m_bs_bits.size() - 1) * (sizeof(Bits) * 8);
	for(unsigned int i = starting_bit; i < m_bs_nbits; i++)
		if(!test(i))
			return false;

	return true;
}

void DenseBitset::set(size_t bit) {
    if (bit < 0 || (unsigned int)bit >= m_bs_nbits)
		return;

	m_bs_bits[BIT_INDEX(bit)] |= (1 << BIT_IN_OBJ(bit));
}

void DenseBitset::unset(size_t bit) {
    if (bit < 0 || (unsigned int)bit >= m_bs_nbits)
		return;

	m_bs_bits[BIT_INDEX(bit)] &= ~(1 << BIT_IN_OBJ(bit));
}

bool DenseBitset::test(size_t bit) const {
    Bits mask = (1 << BIT_IN_OBJ(bit));

	if (bit < 0 || (unsigned int)bit >= m_bs_nbits)
		return 0;

	return (m_bs_bits[BIT_INDEX(bit)] & mask) == mask;
}

void DenseBitset::invert() {
	for (size_t i = 0; i < m_bs_bits.size(); i++)
		m_bs_bits[i] = ~m_bs_bits[i];

	/*
	 * Mask out unused high bits
	 */
	if (BIT_IN_OBJ(m_bs_nbits) != 0)
		m_bs_bits[m_bs_bits.size()-1] &= (1 << (BIT_IN_OBJ(m_bs_nbits-1) + 1)) - 1;
}

void DenseBitset::clear() {
	std::fill(m_bs_bits.begin(), m_bs_bits.end(), 0);
}

size_t DenseBitset::numSet() const {
    // Need to loop over each bs_bits
	size_t count = 0;
    
    for (size_t i = 0; i < m_bs_bits.size(); i++) {
		// This clears the LSB set in curBits - we loop until curBits is 0
		for (auto curBits = m_bs_bits[i]; curBits; ++count) curBits &= curBits - 1;
	}
	
	return count;
}

/*
 * NOTE: ffs() assumes an integer argument. If sizeof(bits) is anything
 * else this will need to be fixed.
 */
size_t DenseBitset::firstSet() const {
	static_assert(sizeof(Bits) == sizeof(int));
    
	size_t i;
    for (i = 0; i < m_bs_bits.size(); i++)
		if (m_bs_bits[i] != 0)
			return (SIZE_TO_BYTES(i) << 3) + ffs(m_bs_bits[i]) - 1;

	return Bitset::None;
}

size_t DenseBitset::sizeInBits() const {
    return m_bs_nbits;
}

std::string DenseBitset::toString() const {
    std::stringstream output;

	// Start with actual number of bits (silently truncate to 32 bits)
    output << std::hex << (m_bs_nbits & 0xffffffff);

	output << ' ';

    int bytes = NUM_BYTES(m_bs_nbits);
    int bit = 0;
	for (int pbit = (bytes * 8) - 1; pbit > 0; ) {
        int val = 0;
		for (val = 0, bit = 3; bit >= 0; bit--, pbit--) {
			if ((unsigned int)pbit < m_bs_nbits && test(pbit)) {
				val |= (1 << bit);
			}
		}
		output << std::hex << (val & 0x0f);
	}

	if (m_bs_nbits == 0) {
		output << '0';
	}

	return output.str();
}

Bitset::Serialized DenseBitset::serialize() const {
	std::string serialized = toString();
	auto len = serialized.size() + 1; // +1 for \0
	auto result = new char[len];
	snprintf(result, len, "%s", serialized.c_str());

	return Bitset::Serialized(result);
}

Bitset& DenseBitset::operator=(const Bitset &other) {
	auto denseOther = dynamic_cast<const DenseBitset*>(&other);
	if (!denseOther) return Bitset::operator=(other);

	*this = other;
	return *this;
}

DenseBitset& DenseBitset::operator=(const DenseBitset &other) {
	m_bs_nbits = other.m_bs_nbits;
	m_bs_bits = other.m_bs_bits;

	return *this;
}

/*
 * If bitsets are different sizes, high bits are assumed
 * to be 0.
 */
Bitset& DenseBitset::operator&=(const Bitset &other) {
	auto denseOther = dynamic_cast<const DenseBitset*>(&other);
	if (!denseOther) return Bitset::operator&=(other);

	size_t i;

	for (i = 0; i < std::min(m_bs_bits.size(), denseOther->m_bs_bits.size()); i++)
		m_bs_bits[i] &= denseOther->m_bs_bits[i];

	/*
	 * Mask high bits (if any)
	 */
	for ( ; i < m_bs_bits.size(); i++)
		m_bs_bits[i] = 0;

	return *this;
}

Bitset& DenseBitset::operator-=(const Bitset &other) {
	auto denseOther = dynamic_cast<const DenseBitset*>(&other);
	if (!denseOther) return Bitset::operator-=(other);

	size_t i;

	for (i = 0; i < std::min(m_bs_bits.size(), denseOther->m_bs_bits.size()); i++)
		m_bs_bits[i] &= ~denseOther->m_bs_bits[i];

	/*
	 * Mask high bits (if any)
	 */
	for ( ; i < m_bs_bits.size(); i++)
		m_bs_bits[i] = 0;

	return *this;
}

Bitset& DenseBitset::operator|=(const Bitset &other) {
	auto denseOther = dynamic_cast<const DenseBitset*>(&other);
	if (!denseOther) return Bitset::operator|=(other);

	for (size_t i = 0; i < std::min(m_bs_bits.size(), denseOther->m_bs_bits.size()); i++)
		m_bs_bits[i] |= denseOther->m_bs_bits[i];

	/*
	 * Mask out unused high bits
	 */
	if (BIT_IN_OBJ(m_bs_nbits) != 0)
		m_bs_bits[m_bs_bits.size()-1] &= (1 << (BIT_IN_OBJ(m_bs_nbits-1) + 1)) - 1;

	return *this;
}

/*
 * Bitsets must be the same size
 */
bool DenseBitset::operator==(const Bitset &other) const {
	auto denseOther = dynamic_cast<const DenseBitset*>(&other);
	if (!denseOther) return Bitset::operator==(other);
	
	if (m_bs_nbits != denseOther->m_bs_nbits)
		return false;

	for (size_t i = 0; i < m_bs_bits.size(); i++)
		if (m_bs_bits[i] != denseOther->m_bs_bits[i])
			return false;

	return true;
}

bool DenseBitset::hasCommonBits(const Bitset &other) const {
    auto denseOther = dynamic_cast<const DenseBitset*>(&other);
	if (!denseOther) return Bitset::hasCommonBits(other);

	auto end = std::min(m_bs_bits.size(), denseOther->m_bs_bits.size());

	for (size_t i = 0; i < end; i++)
		if ((m_bs_bits[i] & denseOther->m_bs_bits[i]) != 0)
			return true;

	return false;
}
