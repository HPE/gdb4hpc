/******************************************************************************\
 * Copyright 2021 Hewlett Packard Enterprise Development LP.
 ******************************************************************************/
#pragma once

#include "bitset.h"

#include <vector>

class DenseBitset : public Bitset {
public:
    DenseBitset(unsigned int nbits);

    virtual std::unique_ptr<Bitset> clone() const override;
    std::unique_ptr<DenseBitset> typedClone() const;

    virtual bool isEmpty() const override; 
    virtual bool isFull() const override; 

    virtual void set(int bit) override; 
    virtual void unset(int bit) override; 
    virtual bool test(int bit) const override; 
    virtual void invert() override; 
    virtual void clear() override; 

    virtual int numSet() const override; 
    virtual int firstSet() const override; 

    virtual size_t sizeInBits() const override; 

    virtual std::string toString() const override; 
    
    virtual std::string toSet(int offset = 0, int len = -1) const override;

    virtual SerializedPtr serialize() const override;
 
    virtual Bitset& operator= (const Bitset &other) override;
    DenseBitset& operator= (const DenseBitset &other);

    virtual Bitset& operator&= (const Bitset &other) override;
    virtual Bitset& operator-= (const Bitset &other) override;

    virtual Bitset& operator|= (const Bitset &other) override;

    virtual bool operator== (const Bitset &other) const override;
    virtual bool operator!= (const Bitset &other) const override;

    virtual bool hasCommonBits(const Bitset &other) const override;
private:
    // NOTE: if you change the size of bits, then the ffs() routine may no 
    // longer work. see firstSet implimentation.
    typedef unsigned int Bits;

    unsigned int m_bs_nbits;      // total number of bits in this bitset
    std::vector<Bits> m_bs_bits;  // actual bits storage (unused bits are always 0)
};

// convert a bit into the index of the Bits containing it in the m_bs_bits vector
#define BIT_INDEX(bit) ((bit) >> 3) / (sizeof(Bits))
// (bit) % (number of bits in a Bits object)
#define BIT_IN_OBJ(bit) ((bit) % (sizeof(Bits) << 3))
// convert the length of the m_bs_bits vector to its size in bytes
#define SIZE_TO_BYTES(size) (sizeof(Bits) * (size))
// number of bytes required to store n bits
#define NUM_BYTES(bits) (((bits-1) >> 3) + 1)
