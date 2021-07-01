/******************************************************************************\
 * Copyright 2021 Hewlett Packard Enterprise Development LP.
 ******************************************************************************/
#pragma once

#include <memory>
#include <string>

// some bitset implimentations are made for a specific use case such as fast
// building or efficient space representation. these bitsets don't always 
// implement every bitset operation available in the interface. trying to do an
// operation that isn't implemented will result in this error being thrown.
class UnimplementedBitsetOperationError : public std::exception {
public:
    UnimplementedBitsetOperationError(std::string funcName) : m_funcName{funcName} {};
    ~UnimplementedBitsetOperationError() noexcept {};
    virtual const char * what() const noexcept override { return m_funcName.c_str(); };
private:    
    std::string m_funcName;
};

class Bitset {
public:
    // clone the bitset.
    virtual std::unique_ptr<Bitset> clone() const = 0;

    virtual ~Bitset() = default;

    // returns true if all bits are 0.
    virtual bool isEmpty() const;
    // returns true if all bits are 1.
    virtual bool isFull() const;

    // sets a bit to 1. offset starts from LSB.
    virtual void set(int bit);
    // sets a bit to 0. offset starts from LSB.
    virtual void unset(int bit);
    // returns true if the bit at the given offset is 1. offset starts from LSB.
    // if an out-of-bounds bit is tested, the result should be false.
    virtual bool test(int bit) const;
    // inverts (bitwise NOT) all bits.
    virtual void invert();
    // sets all bits to 0.
    virtual void clear();

    // returns the number of bits set to 1.
    virtual unsigned int numSet() const;
    // returns the offset of the first set bit. offset starts from LSB.
    virtual int firstSet() const;

    // returns the size of the bitset in bits (not bytes).
    virtual size_t sizeInBits() const;

    // returns a human readable string representation of the bitset.
    virtual std::string toString() const;

    // returns a string representation in set notation, e.g. {0..2,4,5..100}.
    virtual std::string toSet(int offset = 0, int len = -1) const;

    // a serialized bitset, not necessarily in a human readable format. derived
    // classes should implement a (const Serialized&) constructor to
    // do deserialization.
    // e.g.
    //   auto serialized_bitset = bitset->serialize();
    //   auto new_bitset = Bitset(serialized_bitset);
    class Serialized {
    public:
        using DataPtr = std::unique_ptr<char, decltype(&std::free)>;
        // takes ownership of data
        explicit Serialized(char * data) : m_data(data, std::free), m_offset{0} {};

        // useful when you have layered data and want to read part of the
        // message and then send the rest off to a different function
        void setOffset(unsigned int offset) const { m_offset = offset; }
        const char * get() const { 
            if (m_data)
                return m_data.get() + m_offset;
            else
                return nullptr;
        }
        char * release() { return m_data.release(); }
    private:
        DataPtr m_data;
        mutable unsigned int m_offset;
    };

    // returns a Bitset::Serialized class that can be used to construct a new
    // bitset.
    virtual Serialized serialize() const;

    // for binary operators, it is up to the implimentation to make sure the
    // formats of the two bitsets being operated on are compatible.

    virtual Bitset& operator= (const Bitset &other);
    virtual Bitset& operator&= (const Bitset &other);
    virtual Bitset& operator|= (const Bitset &other);
    // set removal operation: 0b1110 -= 0b0110 -> 0b1000
    virtual Bitset& operator-= (const Bitset &other);

    virtual bool operator== (const Bitset &other) const;
    virtual bool operator!= (const Bitset &other) const;

    // returns true if lhs & rhs != 0.
    virtual bool hasCommonBits(const Bitset &other) const;
};

namespace bitset {
    // returns a new bitset containing the result of a bitwise AND between the 
    // arguments. the format of the returned bitset will be that of the lhs bitset.
    // it is up to the implimentation of &= to make sure that the two bitsets
    // are compatible for the operation.
    std::unique_ptr<Bitset> fromAnd(const Bitset &lhs, const Bitset &rhs);

    // returns a new bitset containing the result of a bitwise OR between the 
    // arguments. the format of the returned bitset will be that of the lhs bitset.
    // it is up to the implimentation of |= to make sure that the two bitsets
    // are compatible for the operation.
    std::unique_ptr<Bitset> fromOr(const Bitset &lhs, const Bitset &rhs);

    // generic function to copy the bits from source to dest. since we don't know
    // the underlying types of the bitsets, this is a slow, one-by-one copy. use
    // this function only if you can't copy any other way. dest and source must
    // be the same sizeInBits().
    void copyBits(Bitset &dest, const Bitset &source);
} // bitset
