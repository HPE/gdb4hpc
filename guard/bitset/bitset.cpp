/******************************************************************************\
 * Copyright 2021 Hewlett Packard Enterprise Development LP.
 ******************************************************************************/
#include "bitset.h"
#include "defs.h"

#include <sstream>

namespace bitset {

std::unique_ptr<Bitset> fromAnd(const Bitset& l, const Bitset &r) {
	auto lhs = l.clone();
	*lhs &= r;
	return lhs;
}

std::unique_ptr<Bitset> fromOr(const Bitset& l, const Bitset &r) {
	auto lhs = l.clone();
	*lhs |= r;
	return lhs;
}

} // bitset

bool Bitset::isEmpty() const {
	for (size_t i = 0, n = sizeInBits(); i < n; i++) {
		if (test(i)) return false;
	}
	return true;
}

bool Bitset::isFull() const {
	for (size_t i = 0, n = sizeInBits(); i < n; i++) {
		if (!test(i)) return false;
	}
	return true;
}

void Bitset::invert() {
	for (size_t i = 0, n = sizeInBits(); i < n; i++) {
		if (test(i)) unset(i); else set(i);
	}
}

void Bitset::clear() {
	for (size_t i = 0, n = sizeInBits(); i < n; i++) {
		unset(i);
	}
}

size_t Bitset::numSet() const {
	size_t count = 0;
	for (size_t i = 0, n = sizeInBits(); i < n; i++) {
		if (test(i)) count++;
	}
	return count;
}

size_t Bitset::firstSet() const {
	for (size_t i = 0, n = sizeInBits(); i < n; i++) {
		if (test(i)) return i;
	}
	return Bitset::None;
}

std::string Bitset::toString() const {
	return toSetString(0, Bitset::None);
}

std::string Bitset::toSetString(size_t offset, size_t len) const {
	std::ostringstream str;

	if (len == Bitset::None) {
		// default argument
		if (sizeInBits() >= offset) {
			len = sizeInBits() - offset;
		} else {
			len = 0;
		}
	}

	std::string sep = "";
	size_t firstInRun = Bitset::None;

	auto printRange = [&](size_t start, size_t end) {
		if (start == end) {
			// single bit range
			str << sep << start;
			sep = ",";
		} else {
			// multi bit range
			str << sep << start << ".." << end;
			sep = ",";
		}
	};

	str << "{";

	bool inRun = false;

	for (size_t i = 0, n = len; i < n; i++) {
		if (test(i + offset) && !inRun) {
			inRun = true;
			firstInRun = i;
		} else if (!test(i + offset) && inRun) {
			inRun = false;
			printRange(firstInRun, i - 1);
			firstInRun = Bitset::None;
		}
	}

	// catch any runs at the end
	if (inRun) {
		printRange(firstInRun, len-1);
	}

	str << "}";

	return str.str();
}

std::set<size_t> Bitset::toSet() const {
	std::set<size_t> set;

	for (size_t i = 0; i < sizeInBits(); i++) {
		if (test(i)) set.insert(i);
	}

	return set;
}

Bitset::Serialized Bitset::serialize() const {
	throw std::runtime_error("unimplemented bitset operation: serialize");
	return Serialized(nullptr, std::free);
}

Bitset& Bitset::operator= (const Bitset &other) {
	if (other.sizeInBits() != sizeInBits()) 
		throw std::runtime_error("unimplemented bitset operation: unequal sizeInBits");
	
	for (size_t i = 0, n = sizeInBits(); i < n; i++) {
		if (other.test(i)) set(i); else unset(i);
	}
	return *this;
}

// if unequal sizes, high bits are treated as 0
Bitset& Bitset::operator&= (const Bitset &other) {
	auto commonSize = std::min(sizeInBits(), other.sizeInBits());

	for (size_t i = 0; i < commonSize; i++) {
		if (test(i) && other.test(i)) set(i); else unset(i);
	}

	for (auto i = commonSize; i < sizeInBits(); i++) {
		unset(i);
	}

	return *this;
}

Bitset& Bitset::operator|= (const Bitset &other) {
	auto commonSize = std::min(sizeInBits(), other.sizeInBits());

	for (size_t i = 0; i < commonSize; i++) {
		if (test(i) || other.test(i)) set(i); else unset(i);
	}

	return *this;
}

// if unequal sizes, high bits are unchanged
Bitset& Bitset::operator-= (const Bitset &other) {
	auto commonSize = std::min(sizeInBits(), other.sizeInBits());

	for (size_t i = 0; i < commonSize; i++) {
		if (test(i) && other.test(i)) unset(i);
	}

	return *this;
}

bool Bitset::operator== (const Bitset &other) const {
	if (sizeInBits() != other.sizeInBits()) return false;

	for (size_t i = 0, n = sizeInBits(); i < n; i++) {
		if (test(i) != other.test(i)) return false;
	}
	return true;
}

bool Bitset::operator!= (const Bitset &other) const {
	return !(*this == other);
}

bool Bitset::hasCommonBits(const Bitset &other) const {
	auto commonSize = std::min(sizeInBits(), other.sizeInBits());

	for (size_t i = 0; i < commonSize; i++) {
		if (test(i) && other.test(i)) return true;
	}

	return false;
}

size_t Bitset::hash() const {
	// take the first 64 bits and convert them to a size_t
	size_t hash{0};
	for (size_t i = 0; i < sizeof(size_t) * 8; i++) {
		// test() is safe out of bounds
		auto bit = test(i) ? 1 : 0;
		hash |= (bit << i); 
	}
	// hash the size_t for better uniformity across the hash space
	return std::hash<size_t>{}(hash);
}