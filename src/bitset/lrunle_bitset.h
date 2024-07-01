/******************************************************************************\
 * Copyright 2021 Hewlett Packard Enterprise Development LP.
 ******************************************************************************/
#pragma once

#include "gdb4_bitset.h"

#include <vector>

class lrle_context;

class LRunLEBitset : public Bitset {
public:
    class SetBitsIterator : public Bitset::SetBitsIterator {
    public:
        SetBitsIterator(const LRunLEBitset &bits);
        ~SetBitsIterator();

        size_t operator*() const override;

        SetBitsIterator& operator++() override;

        bool operator!=(const SetBitsIterator &other);

        friend LRunLEBitset;
    private:
        struct End {};
        SetBitsIterator(const LRunLEBitset &bits, SetBitsIterator::End);

        const LRunLEBitset& m_bits;
        std::unique_ptr<lrle_context> m_context;
        size_t m_current_bit;
    };

    SetBitsIterator begin() const { return SetBitsIterator(*this); }
    SetBitsIterator end() const {
        return SetBitsIterator(*this, SetBitsIterator::End());
    }

    LRunLEBitset(size_t nbits);
    LRunLEBitset(size_t nbits, size_t setBit);
    LRunLEBitset(size_t nbits, unsigned const char* buf, size_t len);
    LRunLEBitset(SerializedPtr bitset);

    ~LRunLEBitset() = default;

    virtual std::unique_ptr<Bitset> clone() const override;
    virtual std::unique_ptr<Bitset> clone(size_t newNumBits) const override;

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

    // virtual std::string toSetString(int offset = 0, int len = -1) const override;

    virtual Serialized serialize() const override;
    // given a serialized LRunLEBitset, return how many bytes the serialized data is
    static size_t extractSerializedSize(const Serialized &data);

    virtual Bitset& operator= (const Bitset &other) override;
    virtual Bitset& operator&= (const Bitset &other) override;
    virtual Bitset& operator|= (const Bitset &other) override;
    // virtual Bitset& operator-= (const Bitset &other) override;

    virtual bool operator== (const Bitset &other) const override;
    virtual bool operator!= (const Bitset &other) const override;

    virtual bool hasCommonBits(const Bitset &other) const override;

    virtual std::unique_ptr<Bitset::SetBitsIterator> iterator() const override;

    friend SetBitsIterator;
private:
    void setFirstBit(size_t bit);
    std::vector<unsigned char> andBuf(unsigned char const* rhs, size_t rhs_len) const;
    std::vector<unsigned char> orBuf(unsigned char const* rhs, size_t rhs_len) const;

    size_t m_nbits;
    std::vector<unsigned char> m_compressed; 
};
