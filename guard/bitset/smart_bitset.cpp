/*********************************************************************************\
 * <smart_bitset.c>
 *
 * Copyright 2012-2013,2021 Hewlett Packard Enterprise Development LP.
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 *********************************************************************************/
#include "smart_bitset.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>

#include <sstream>

#include "dense_bitset.h"
#include "sparse_bitset.h"

SmartBitset::SmartBitset()
	: m_dense{nullptr}
	, m_sparse{nullptr}
{}

SmartBitset::SmartBitset(std::unique_ptr<DenseBitset> &&denseBitset)
	: m_dense{std::move(denseBitset)}
	, m_sparse{nullptr}
{}

SmartBitset::SmartBitset(std::unique_ptr<SparseBitset> &&sparseBitset)
	: m_dense{nullptr}
	, m_sparse{std::move(sparseBitset)}
{}

SmartBitset::SmartBitset(int num_bits) 
	: m_dense{nullptr}
	, m_sparse{std::make_unique<SparseBitset>(num_bits)}
{}

SmartBitset::SmartBitset(const Serialized &serialized) {
	if (serialized.get() == nullptr) {
		throw std::runtime_error("null ptr passed to SmartBitset constructor.");
	}

	std::istringstream stream(serialized.get());

	auto start = stream.tellg();

	int typeSwitch;
	stream >> typeSwitch;

	auto offset = stream.tellg() - start;
	serialized.setOffset(offset);

	if (typeSwitch == 0) {
		m_dense = std::make_unique<DenseBitset>(std::move(serialized));
	} else {
		m_sparse = std::make_unique<SparseBitset>(std::move(serialized));
	}
}

std::unique_ptr<Bitset> SmartBitset::clone() const {
	return typedClone();
}

std::unique_ptr<SmartBitset> SmartBitset::typedClone() const {
	auto r = new SmartBitset();
	if (m_dense) r->m_dense = m_dense->typedClone();
	if (m_sparse) r->m_sparse = m_sparse->typedClone();
	return std::unique_ptr<SmartBitset>(r);
}

bool SmartBitset::isSparse() const {
	return m_sparse.get() != nullptr;
}

bool SmartBitset::isDense() const {
	return m_dense.get() != nullptr;
}

std::unique_ptr<DenseBitset> SmartBitset::releaseAsDense() {
	convertToDense();
	return std::move(m_dense);
}

bool SmartBitset::convertToDense() {
	// don't convert if we don't have a sparse bitset or if we're already dense
	if (!m_sparse || m_dense) return false;

	m_dense = std::make_unique<DenseBitset>(m_sparse->sizeInBits());

	if (m_sparse->sizeInRanges() == 0) {
		// empty bitset, we're done
		return true;
	}

	for (auto it = m_sparse->getSetBitsIterator(); *it < m_sparse->sizeInBits(); ++it) {
		m_dense->set(*it);
	}

	return true;
}

bool SmartBitset::isEmpty() const {
	if (m_dense) return m_dense->isEmpty();
	else if (m_sparse) return m_sparse->isEmpty();
	return true;
}

bool SmartBitset::isFull() const {
	if (m_dense) return m_dense->isFull();
	else if (m_sparse) return m_sparse->isFull();
	return false;
}

void SmartBitset::set(int bit) {
	if (m_dense) m_dense->set(bit);
	if (m_sparse) m_sparse->set(bit);
}

void SmartBitset::clear() {
	if (m_dense) m_dense->clear();
	if (m_sparse) m_sparse->clear();
}

unsigned int SmartBitset::numSet() const {
	if (m_dense) return m_dense->numSet();
	else if (m_sparse) return m_sparse->numSet();

	return 0;
}

int SmartBitset::firstSet() const {
	if (m_dense) return m_dense->firstSet();
	else if (m_sparse) return m_sparse->firstSet();

	return -1;
}

size_t SmartBitset::sizeInBits() const {
	if (m_dense) return m_dense->sizeInBits();
	else if (m_sparse) return m_sparse->sizeInBits();

	return 0;
}

Bitset::Serialized SmartBitset::serialize() const {
	if(!m_dense && !m_sparse)
		// in the case of an empty bitset, we serialize to an empty sparse bitset.
		return Serialized(strdup("1 0"));

	std::string result = m_dense ? "0 " : "1 ";

	if (m_dense) {
		result += m_dense->serialize().get();
	} else if (m_sparse) {
		result += m_sparse->serialize().get();
	}

	return Serialized(strdup(result.c_str()));
}

Bitset& SmartBitset::operator=(const Bitset &_other) {
	auto &other = dynamic_cast<const SmartBitset&>(_other);
	*this = other;
	return *this;
}

SmartBitset& SmartBitset::operator=(const SmartBitset &other) {
	if (m_dense && other.m_dense) {
		*m_dense = *other.m_dense;
	} else if (m_sparse && other.m_sparse) {
		*m_sparse = *other.m_sparse;
	}

	// if we get here, other is empty
	m_dense = nullptr;
	m_sparse = nullptr;

	return *this;
}

Bitset& SmartBitset::operator|=(const Bitset &_other) {
	auto &other = dynamic_cast<const SmartBitset&>(_other);

	if (m_dense && other.m_dense) {
		*m_dense |= *other.m_dense;
	} else if (m_sparse && other.m_sparse) {
		*m_sparse |= *other.m_sparse;
	} else if (m_dense && other.m_sparse) {
		// XXX: legacy code converted other to dense here, which breaks the 
		// const-ness of the function. we'll have to settle for a clone.
		auto clone = other.typedClone();
		clone->convertToDense();
		*m_dense |= *clone->m_dense;
	} else if (m_sparse && other.m_dense) {
		convertToDense();
		*m_dense |= *other.m_dense;
	}

	return *this;
}

bool SmartBitset::operator==(const Bitset &_other) const {
	auto &other = dynamic_cast<const SmartBitset&>(_other);

	if (m_dense && other.m_dense) {
		return *m_dense == *other.m_dense;
	} else if (m_sparse && other.m_sparse) {
		return *m_sparse == *other.m_sparse;
	}

	// return true if both are empty
	return !m_dense && !other.m_dense && !m_sparse && !other.m_sparse;
}

bool SmartBitset::operator!=(const Bitset &other) const {
	return !(*this == other);
}
