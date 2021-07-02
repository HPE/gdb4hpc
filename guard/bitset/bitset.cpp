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
	DPRINT("Warning: slow default bitset operation in use: %s\n", __FUNCTION__);
	for (size_t i = 0, n = sizeInBits(); i < n; i++) {
		if (test(i)) return false;
	}
	return true;
}

bool Bitset::isFull() const {
	DPRINT("Warning: slow default bitset operation in use: %s\n", __FUNCTION__);
	for (size_t i = 0, n = sizeInBits(); i < n; i++) {
		if (!test(i)) return false;
	}
	return true;
}

void Bitset::invert() {
	DPRINT("Warning: slow default bitset operation in use: %s\n", __FUNCTION__);
	for (size_t i = 0, n = sizeInBits(); i < n; i++) {
		if (test(i)) unset(i); else set(i);
	}
}

void Bitset::clear() {
	DPRINT("Warning: slow default bitset operation in use: %s\n", __FUNCTION__);
	for (size_t i = 0, n = sizeInBits(); i < n; i++) {
		unset(i);
	}
}

size_t Bitset::numSet() const {
	DPRINT("Warning: slow default bitset operation in use: %s\n", __FUNCTION__);
	size_t count = 0;
	for (size_t i = 0, n = sizeInBits(); i < n; i++) {
		if (test(i)) count++;
	}
	return count;
}

size_t Bitset::firstSet() const {
	DPRINT("Warning: slow default bitset operation in use: %s\n", __FUNCTION__);
	for (size_t i = 0, n = sizeInBits(); i < n; i++) {
		if (test(i)) return i;
	}
	return Bitset::None;
}

std::string Bitset::toString() const {
	return toSet(0, -1);
}

std::string Bitset::toSet(size_t offset, size_t len) const {
	std::ostringstream str;

	if (len == Bitset::None) {
		// default argument
		len = sizeInBits() - offset;
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

Bitset::Serialized Bitset::serialize() const {
	throw std::runtime_error("unimplemented bitset operation: serialize");
	return Serialized(nullptr);
}

Bitset& Bitset::operator= (const Bitset &other) {
	DPRINT("Warning: slow default bitset operation in use: %s\n", __FUNCTION__);
	if (other.sizeInBits() != sizeInBits()) 
		throw std::runtime_error("unimplemented bitset operation: unequal sizeInBits");
	
	for (size_t i = 0, n = sizeInBits(); i < n; i++) {
		if (other.test(i)) set(i); else unset(i);
	}
	return *this;
}

// if unequal sizes, high bits are treated as 0
Bitset& Bitset::operator&= (const Bitset &other) {
	DPRINT("Warning: slow default bitset operation in use: %s\n", __FUNCTION__);
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
	DPRINT("Warning: slow default bitset operation in use: %s\n", __FUNCTION__);
	auto commonSize = std::min(sizeInBits(), other.sizeInBits());

	for (size_t i = 0; i < commonSize; i++) {
		if (test(i) || other.test(i)) set(i); else unset(i);
	}

	return *this;
}

// if unequal sizes, high bits are unchanged
Bitset& Bitset::operator-= (const Bitset &other) {
	DPRINT("Warning: slow default bitset operation in use: %s\n", __FUNCTION__);
	auto commonSize = std::min(sizeInBits(), other.sizeInBits());

	for (size_t i = 0; i < commonSize; i++) {
		if (test(i) && !other.test(i)) set(i); else unset(i);
	}

	return *this;
}

bool Bitset::operator== (const Bitset &other) const {
	DPRINT("Warning: slow default bitset operation in use: %s\n", __FUNCTION__);
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
	DPRINT("Warning: slow default bitset operation in use: %s\n", __FUNCTION__);
	auto commonSize = std::min(sizeInBits(), other.sizeInBits());

	for (size_t i = 0; i < commonSize; i++) {
		if (test(i) && other.test(i)) return true;
	}

	return false;
}
