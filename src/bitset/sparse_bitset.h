/*********************************************************************************\
 * <sparse_bitset.h>
 *
 * Copyright 2012-2013,2021 Hewlett Packard Enterprise Development LP.
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 *********************************************************************************/
#pragma once

#include "bitset.h"

#include <vector>

/*
 * Sparse bitset consists of a number of ranges. Each ranges is denoted by using
 * its start and end.
 * 
 * Sparse bitsets are only half implimented - they are used for quickly building
 * a bitset and then sending it over the network. Once over the network, they
 * are typically converted into fully dense bitsets before being passed to the
 * client layer.
 */

class SparseBitset : public Bitset {
private:
    // data representing runs of bits that have been set. ranges are inclusive.
    // e.g. {sr_start = 2, sr_end = 5} = 0b00111100
    struct Range {
        size_t sr_start;
        size_t sr_end;

        bool contains(size_t bit) const {
            return bit >= sr_start && bit <= sr_end;
        }
    };
public:
    // iterates over all bits that are set. returns integer offsets representing
    // the position of the set bits.
    class SetBitsIterator : public Bitset::SetBitsIterator {
    public:
        SetBitsIterator(const SparseBitset &bits)
            : m_bits{bits}
        {
            if (m_bits.sizeInRanges() == 0) {
                m_current_bit = Bitset::None;
            } else {
                m_range_it = m_bits.m_sb_ranges.begin();
                m_current_bit = m_range_it->sr_start;
            }
        }

        size_t operator*() const override {
            return m_current_bit;
        }

        SetBitsIterator& operator++() override {
            if (m_current_bit >= m_bits.sizeInBits()) {
                return *this;
            }

            m_current_bit++;

            if (m_current_bit > m_range_it->sr_end) {
                m_range_it++;
                if (m_range_it == m_bits.m_sb_ranges.end()) {
                    m_current_bit = Bitset::None;
                } else {
                    m_current_bit = m_range_it->sr_start;
                }
            }

            return *this;
        }

        bool operator!=(const SetBitsIterator &other) {
            return m_current_bit != other.m_current_bit;
        };
    private:
        const SparseBitset& m_bits;
        size_t m_current_bit;

        std::vector<Range>::const_iterator m_range_it;
    };

    SetBitsIterator getSetBitsIterator() const { return SetBitsIterator(*this); }

    SparseBitset(size_t nbits);
    SparseBitset(SparseBitset&& other) = default;
    explicit SparseBitset(const SparseBitset& other) = default;
    // e.g. ::fromBits({1, 2, 3, 4, 12, 13, 14, 15})
    static SparseBitset fromBits(std::initializer_list<size_t> bits);
    SparseBitset(SerializedPtr serialized);

    virtual std::unique_ptr<Bitset> clone() const override;
    virtual std::unique_ptr<Bitset> clone(size_t newSize) const override;
    std::unique_ptr<SparseBitset> typedClone() const;
    std::unique_ptr<SparseBitset> typedClone(size_t newSize) const;

    virtual bool isEmpty() const override; 
    virtual bool isFull() const override; 

    virtual void set(size_t bit) override; 
    void setRange(unsigned int start, unsigned int end);
    virtual void unset(size_t bit) override;
    virtual bool test(size_t bit) const override;
    // virtual void invert() override;
    virtual void clear() override; 

    virtual size_t numSet() const override; 
    virtual size_t firstSet() const override;

    virtual size_t sizeInBits() const override;
    size_t sizeInRanges() const;

    // virtual std::string toString() const override;
    
    // virtual std::string toSetString(int offset = 0, int len = -1) const override;

    // returns the bitset in the form of a comma separated list of disjoint ranges, e.g. "1-3,5,9-1000"
    std::string toRangeString() const;

    virtual Serialized serialize() const override;

    virtual Bitset& operator= (const Bitset &other) override;
    SparseBitset& operator= (const SparseBitset &other);

    // virtual Bitset& operator&= (const Bitset &other) override;
    // virtual Bitset& operator-= (const Bitset &other) override;

    virtual Bitset& operator|= (const Bitset &other) override;

    virtual bool operator== (const Bitset &other) const override;
    // virtual bool operator!= (const Bitset &other) const override;

    // virtual bool hasCommonBits(const Bitset &other) const override;

    virtual std::unique_ptr<Bitset::SetBitsIterator> iterator() const override;

    friend SetBitsIterator;
private:
    // find index of first range with an end >= bit. returns m_sb_ranges.size()
    // if none are found.
    size_t findRangeIdx(size_t bit) const;

    // check if range at idx connects with range at idx - 1 and merge if so.
    // e.g. given two ranges: {2-5}, {6-8}, calling mergeIfNecessary(1) would
    // turn them into {2-8}.
    void mergeIfNecessary(size_t idx);

    size_t m_sb_num_bits; // total number of bits
    std::vector<Range> m_sb_ranges; // ranges in an ascending order
};
