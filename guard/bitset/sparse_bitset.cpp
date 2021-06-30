/*********************************************************************************\
 * <sparse_bitset.c>
 *
 * Copyright 2012-2013,2020 Hewlett Packard Enterprise Development LP.
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * $HeadURL$
 * $Date$
 * $Rev$
 * $Author$
 *
 *********************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>

#include <sstream>

#include "sparse_bitset.h"
#include "bitset_serialize.h"

SparseBitset::SparseBitset(unsigned int num_bits) 
	: m_sb_num_bits{num_bits}
{}

std::unique_ptr<Bitset> SparseBitset::clone() const {
	return typedClone();
}

std::unique_ptr<SparseBitset> SparseBitset::typedClone() const {
	auto clone = new SparseBitset(m_sb_num_bits);
	*clone = *this;
	return std::unique_ptr<SparseBitset>(clone);
}

bool SparseBitset::isEmpty() const {
	return m_sb_ranges.size() == 0;
}

bool SparseBitset::isFull() const {
	return m_sb_ranges.size() == 1 && m_sb_ranges.back().sr_end == m_sb_num_bits - 1;
}

void SparseBitset::set(int sn) {
	if (sn < 0) return;
	unsigned int n = (unsigned int)sn;

	// find range that bit n is nearest (without exceeding the range's sr_end)
	size_t rangeIdx = findRangeIdx(n);
	
	if (rangeIdx < m_sb_ranges.size() && m_sb_ranges[rangeIdx].contains(n)) {
		// the bit "n" has been set
		return;
	}

	// if n is greater than any current range, rangeIdx will equal m_sb_ranges.size()

	if(rangeIdx > 0 && m_sb_ranges[rangeIdx-1].sr_end == n-1) {
		// n == previous range's end + 1, extend previous range upwards
		m_sb_ranges[rangeIdx-1].sr_end = n;
		mergeIfNecessary(rangeIdx);
	} else if(rangeIdx < m_sb_ranges.size() && m_sb_ranges[rangeIdx].sr_start == n+1) {
		// n is one below the beginning of rangeIdx,
		// extend range downards
		m_sb_ranges[rangeIdx].sr_start = n;
		mergeIfNecessary(rangeIdx);
	} else {
		// n does not border any ranges, insert a new Range before rangeidx
		m_sb_ranges.insert(
			m_sb_ranges.begin() + rangeIdx, 
			Range{.sr_start = n, .sr_end = n}
		);
	}
}

void SparseBitset::setRange(unsigned int start, unsigned int end) {
	if (start >= m_sb_num_bits || end < start) return;

	size_t num_ranges = m_sb_ranges.size();

	size_t start_pos = findRangeIdx(start);
	size_t end_pos = findRangeIdx(end);

	if (start_pos == end_pos) {
		if (start_pos < num_ranges && m_sb_ranges[start_pos].contains(start) && m_sb_ranges[start_pos].contains(end)) {
			// the bits are already set, there is nothing to do
			return;
		} else if (start_pos == num_ranges || end < m_sb_ranges[start_pos].sr_start - 1) {
			// case start_pos == num_ranges -> append new range
			// case end < ...sr_start - 1   -> new range is disjoint on the end, insert it as a new range
			m_sb_ranges.insert(
				m_sb_ranges.begin() + start_pos, 
				Range{.sr_start = (unsigned int)start, .sr_end = (unsigned int)end}
			);
			mergeIfNecessary(start_pos);
		} else if (start < m_sb_ranges[start_pos].sr_start) {
			// we've determined that the range is not disjoint on the end,
			// so we extend the existing range down. we know that end is
			// already in this range, so we don't worry about it.
			m_sb_ranges[start_pos].sr_start = start;
			mergeIfNecessary(start_pos);
		}
	} else {
		// here, the new range spans multiple existing ranges, so they will be 
		// combined into one
		int real_start = start, real_end = end;
		int num_shrink = (end_pos - start_pos) - 1;

		if(start_pos != num_ranges && start >= m_sb_ranges[start_pos].sr_start)
			// move real_start to beginning of existing range 
			real_start = m_sb_ranges[start_pos].sr_start; 

		if(end_pos != num_ranges && m_sb_ranges[end_pos].sr_start - 1 <= end && end <= m_sb_ranges[end_pos].sr_end) {
			// move real_end to end of existing range
			real_end = m_sb_ranges[end_pos].sr_end;
			// since end is not disjoint, we will erase an additional range
			num_shrink++;
		}

		// extend range
		m_sb_ranges[start_pos].sr_start = real_start;
		m_sb_ranges[start_pos].sr_end = real_end;
		
		// shrink
		// if num_shrink is 0, this will do nothing
		if (num_shrink >= 0) {
			m_sb_ranges.erase(
				m_sb_ranges.begin() + start_pos + 1, 
				m_sb_ranges.begin() + start_pos + 1 + num_shrink
			);
		}
	}
}

void SparseBitset::clear() {
	m_sb_ranges.clear();
}

unsigned int SparseBitset::numSet() const {
	unsigned int set = 0;
	for (const auto &range : m_sb_ranges) {
		set += range.sr_end - range.sr_start + 1;
	}
	return set;
}

int SparseBitset::firstSet() const {
	if (m_sb_ranges.size() == 0) return -1;
	return m_sb_ranges.front().sr_start;
}

size_t SparseBitset::sizeInBits() const {
	return m_sb_num_bits;
}

size_t SparseBitset::sizeInRanges() const {
	return m_sb_ranges.size();
}

Bitset::SerializedPtr SparseBitset::serialize() const {
	int sep = 0;
	std::stringstream ss;

	ss << static_cast<int>(bitset::Format::Sparse) << ";"
	   << std::hex << sizeInBits() << ";";

	ss << "{";

	for (const auto &range : m_sb_ranges) {
		if (sep == 0)
			sep = 1;
		else
			ss << ",";
		
		if (range.sr_start != range.sr_end) {
			ss << std::hex << range.sr_start << "-" << range.sr_end;
		} else {
			ss << std::hex << range.sr_start;
		}
	}

	ss << "}";

	return SerializedPtr(strdup(ss.str().c_str()), std::free);
}

Bitset& SparseBitset::operator=(const Bitset &_other) {
	auto other = dynamic_cast<const SparseBitset &>(_other);
	*this = other;
	return *this;
}

SparseBitset& SparseBitset::operator=(const SparseBitset &other) {
	m_sb_num_bits = other.m_sb_num_bits;
	m_sb_ranges = other.m_sb_ranges;
	return *this;
}

Bitset& SparseBitset::operator|=(const Bitset &_other) {
	auto &other = dynamic_cast<const SparseBitset&>(_other);
	
	m_sb_num_bits = std::max(m_sb_num_bits, other.m_sb_num_bits);

	for (const auto &range : other.m_sb_ranges) {
		setRange(range.sr_start, range.sr_end);
	}

	return *this;
}

bool SparseBitset::operator==(const Bitset &_other) const {
	auto &other = dynamic_cast<const SparseBitset&>(_other);

	if (m_sb_num_bits != other.m_sb_num_bits) return false;
	if (m_sb_ranges.size() != other.m_sb_ranges.size()) return false;

	for (size_t i = 0; i < m_sb_ranges.size(); i++) {
		auto lhs = m_sb_ranges[i];
		auto rhs = other.m_sb_ranges[i];

		if (lhs.sr_start != rhs.sr_start) return false;
		if (lhs.sr_end != rhs.sr_end) return false;
	}

	return true;
}

bool SparseBitset::operator!=(const Bitset &other) const {
	return !(*this == other);
}

size_t SparseBitset::findRangeIdx(unsigned int bit) {
	if (bit >= m_sb_num_bits) return m_sb_ranges.size();

	for(size_t i = 0; i < m_sb_ranges.size(); i++) {
		if (bit <= m_sb_ranges[i].sr_end) {
			return i;
		}
	}

	return m_sb_ranges.size();
}

void SparseBitset::mergeIfNecessary(unsigned int idx) {
	// to merge, idx must point into the list past the first element, 
	// and the previous range's end must equal idx range's start - 1
	if (idx == 0 || idx >= m_sb_ranges.size() || m_sb_ranges[idx-1].sr_end != m_sb_ranges[idx].sr_start - 1) {
		return;
	}

	// extend previous range to overlap idx range
	m_sb_ranges[idx-1].sr_end = m_sb_ranges[idx].sr_end;

	// collapse list
	m_sb_ranges.erase(m_sb_ranges.begin() + idx);
}
