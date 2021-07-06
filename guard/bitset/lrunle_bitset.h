/******************************************************************************\
 * Copyright 2021 Hewlett Packard Enterprise Development LP.
 ******************************************************************************/
#pragma once

#include "bitset.h"

#include <vector>

class LrleBitset : public Bitset {
public:
    LrleBitset(size_t nbits);
    LrleBitset(size_t nbits, size_t setBit);
    LrleBitset(size_t nbits, unsigned const char* buf, size_t len);
    LrleBitset(const Serialized &bitset);

    ~LrleBitset() = default;

    virtual std::unique_ptr<Bitset> clone() const override;

    virtual bool isEmpty() const override;
    virtual bool isFull() const override;

    // unset and invert are not supported, we override them to throw an exception

    virtual void set(size_t bit) override;
    virtual void unset(size_t bit) override;
    virtual bool test(size_t bit) const override;
    virtual void invert() override; 
    virtual void clear() override;

    virtual size_t numSet() const override;
    virtual size_t firstSet() const override;

    virtual size_t sizeInBits() const override;

    virtual std::string toString() const override;

    // virtual std::string toSet(int offset = 0, int len = -1) const override;

    virtual Serialized serialize() const override;

    virtual Bitset& operator= (const Bitset &other) override;
    virtual Bitset& operator&= (const Bitset &other) override;
    virtual Bitset& operator|= (const Bitset &other) override;
    // virtual Bitset& operator-= (const Bitset &other) override;

    virtual bool operator== (const Bitset &other) const override;
    virtual bool operator!= (const Bitset &other) const override;

    virtual bool hasCommonBits(const Bitset &other) const override;
private:
    void setFirstBit(size_t bit);
    std::vector<unsigned char> andBuf(unsigned char const* rhs, size_t rhs_len) const;
    std::vector<unsigned char> orBuf(unsigned char const* rhs, size_t rhs_len) const;

    size_t m_nbits;
    std::vector<unsigned char> m_compressed; 
};
