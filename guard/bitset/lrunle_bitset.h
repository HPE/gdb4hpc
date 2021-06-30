/******************************************************************************\
 * Copyright 2021 Hewlett Packard Enterprise Development LP.
 ******************************************************************************/
#pragma once

#include "bitset.h"

#include <vector>

class LrleBitset : public Bitset {
public:
    LrleBitset(size_t nbits);
    LrleBitset(size_t nbits, unsigned int setBit);
    LrleBitset(size_t nbits, unsigned const char* buf, size_t len);

    ~LrleBitset() = default;

    virtual std::unique_ptr<Bitset> clone() const override;

    virtual bool isEmpty() const override;
    virtual bool isFull() const override;

    virtual void set(int bit) override;
    // virtual void unset(int bit) override;
    virtual bool test(int bit) const override;
    // virtual void invert() override;
    virtual void clear() override;

    virtual unsigned int numSet() const override;
    virtual int firstSet() const override;

    virtual size_t sizeInBits() const override;

    virtual std::string toString() const override;

    // virtual std::string toSet(int offset = 0, int len = -1) const override;

    virtual SerializedPtr serialize() const override;

    virtual Bitset& operator= (const Bitset &other) override;
    virtual Bitset& operator&= (const Bitset &other) override;
    virtual Bitset& operator|= (const Bitset &other) override;
    // virtual Bitset& operator-= (const Bitset &other) override;

    virtual bool operator== (const Bitset &other) const override;
    virtual bool operator!= (const Bitset &other) const override;

    virtual bool hasCommonBits(const Bitset &other) const override;
private:
    void setFirstBit(unsigned int bit);
    std::vector<unsigned char> andBuf(unsigned char const* rhs, size_t rhs_len) const;
    std::vector<unsigned char> orBuf(unsigned char const* rhs, size_t rhs_len) const;

    size_t m_nbits;
    std::vector<unsigned char> m_compressed; 
};
