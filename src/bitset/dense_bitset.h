/******************************************************************************\
 * Copyright 2021 Hewlett Packard Enterprise Development LP.
 ******************************************************************************/
#pragma once

#include "bitset.h"

#include <vector>

// DenseBitset is the simplest representation of a bitset possible. It holds and
// manipulates an array of bits in memory.

class DenseBitset : public Bitset {
private:
    // NOTE: if you change the size of bits, then the ffs() routine may no 
    // longer work. see firstSet implementation.
    typedef unsigned int Bits;

public:
    class SetBitsIterator : public Bitset::SetBitsIterator {
    public:
        SetBitsIterator(const DenseBitset& bits);

        size_t operator*() const override;
        SetBitsIterator& operator++() override;
        bool operator!=(const SetBitsIterator& other) const;
    private:
        void extractNextChunk();
        const DenseBitset& m_bits;

        size_t m_offset;
        std::vector<size_t> m_next_bits;
        size_t m_next_bits_i;
    };

    explicit DenseBitset(size_t nbits);
    explicit DenseBitset(SerializedPtr serialized);

    virtual std::unique_ptr<Bitset> clone() const override;
    virtual std::unique_ptr<Bitset> clone(size_t newSize) const override;
    std::unique_ptr<DenseBitset> typedClone() const;
    std::unique_ptr<DenseBitset> typedClone(size_t newSize) const;

    virtual bool isEmpty() const override; 
    virtual bool isFull() const override; 

    virtual void set(size_t bit) override; 
    virtual void unset(size_t bit) override; 
    virtual bool test(size_t bit) const override; 
    virtual void invert() override; 
    virtual void clear() override; 

    virtual size_t numSet() const override; 
    virtual size_t firstSet() const override; 

    virtual size_t sizeInBits() const override; 

    virtual std::string toString() const override; 
    
    // virtual std::string toSetString(size_t offset = 0, size_t len = Bitset::None) const override;

    virtual Serialized serialize() const override;
 
    virtual Bitset& operator= (const Bitset &other) override;
    DenseBitset& operator= (const DenseBitset &other);

    virtual Bitset& operator&= (const Bitset &other) override;
    virtual Bitset& operator-= (const Bitset &other) override;

    virtual Bitset& operator|= (const Bitset &other) override;

    virtual bool operator== (const Bitset &other) const override;
    // virtual bool operator!= (const Bitset &other) const override;

    virtual bool hasCommonBits(const Bitset &other) const override;

    virtual std::unique_ptr<Bitset::SetBitsIterator> iterator() const override;
private:
    size_t m_bs_nbits;      // total number of bits in this bitset
    std::vector<Bits> m_bs_bits;  // actual bits storage (unused bits are always 0)

    // convert a bit into the index of the Bits containing it in the m_bs_bits vector
    constexpr size_t BIT_INDEX(size_t bit) const { return ((bit) >> 3) / (sizeof(DenseBitset::Bits)); }
    // (bit) % (number of bits in a Bits object)
    constexpr size_t BIT_IN_OBJ(size_t bit) const { return ((bit) % (sizeof(DenseBitset::Bits) << 3)); }
    // convert the length of the m_bs_bits vector to its size in bytes
    constexpr size_t SIZE_TO_BYTES(size_t size) const { return (sizeof(DenseBitset::Bits) * (size)); }
    // number of bytes required to store n bits
    constexpr size_t NUM_BYTES(size_t bits) const { return (((bits-1) >> 3) + 1); }

    friend SetBitsIterator;
};
