/******************************************************************************\
 * Copyright 2021 Hewlett Packard Enterprise Development LP.
 ******************************************************************************/
#include "bitset.h"
#include "defs.h"

namespace bitset {

std::unique_ptr<Bitset> doAnd(const Bitset& l, const Bitset &r) {
	auto lhs = l.clone();
	*lhs &= r;
	return lhs;
}

std::unique_ptr<Bitset> doOr(const Bitset& l, const Bitset &r) {
	auto lhs = l.clone();
	*lhs |= r;
	return lhs;
}

void copyBits(Bitset &dest, const Bitset &source) {
	if (dest.sizeInBits() != source.sizeInBits()) {
		throw UnimplementedBitsetOperationError("copyBits - unequal sizeInBits");
	}

	dest.clear();
	for (unsigned int i = 0; i < dest.sizeInBits(); i++) {
		if (source.test(i)) dest.set(i);
	}
}

} // bitset

bool Bitset::isEmpty() const {
	throw UnimplementedBitsetOperationError("isEmpty");
	return false;
}

bool Bitset::isFull() const {
	throw UnimplementedBitsetOperationError("isFull");
	return false;
}

void Bitset::set(int bit) {
	throw UnimplementedBitsetOperationError("set");
}

void Bitset::unset(int bit) {
	throw UnimplementedBitsetOperationError("unset");
}

bool Bitset::test(int bit) const {
	throw UnimplementedBitsetOperationError("test");
	return false;
}

void Bitset::invert() {
	throw UnimplementedBitsetOperationError("invert");
}

void Bitset::clear() {
	throw UnimplementedBitsetOperationError("clear");
}

unsigned int Bitset::numSet() const {
	throw UnimplementedBitsetOperationError("numSet");
	return -1;
}

int Bitset::firstSet() const {
	throw UnimplementedBitsetOperationError("firstSet");
}

size_t Bitset::sizeInBits() const {
	throw UnimplementedBitsetOperationError("sizeInBits");
	return 0;
}

std::string Bitset::toString() const {
	throw UnimplementedBitsetOperationError("toString");
	return "";
}

std::string Bitset::toSet(int offset, int len) const {
	throw UnimplementedBitsetOperationError("toSet");
	return "";
}

Bitset::Serialized Bitset::serialize() const {
	throw UnimplementedBitsetOperationError("serialize");
	return Serialized(nullptr);
}

Bitset& Bitset::operator= (const Bitset &other) {
	throw UnimplementedBitsetOperationError("=");
	return *this;
}
Bitset& Bitset::operator&= (const Bitset &other) {
	throw UnimplementedBitsetOperationError("&=");
	return *this;
}
Bitset& Bitset::operator|= (const Bitset &other) {
	throw UnimplementedBitsetOperationError("|=");
	return *this;
}
Bitset& Bitset::operator-= (const Bitset &other) {
	throw UnimplementedBitsetOperationError("-=");
	return *this;
}

bool Bitset::operator== (const Bitset &other) const {
	throw UnimplementedBitsetOperationError("==");
	return false;
}
bool Bitset::operator!= (const Bitset &other) const {
	throw UnimplementedBitsetOperationError("!=");
	return false;
}

bool Bitset::hasCommonBits(const Bitset &other) const {
	throw UnimplementedBitsetOperationError("hasCommonBits");
	return false;
}
