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
    ~UnimplementedBitsetOperationError() noexcept {};
    UnimplementedBitsetOperationError(std::string funcName) : m_funcName{funcName} {};
    virtual const char * what() const noexcept override { return m_funcName.c_str(); };
private:    
    std::string m_funcName;
};

class Bitset {
public:
    // clone the bitset.
    virtual std::unique_ptr<Bitset> clone() const;
    // useful if you intend to cast immediately after a clone
    virtual Bitset* rawClone() const;

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

    using Serialized = char *;
    using SerializedPtr = std::unique_ptr<char, decltype(&std::free)>;

    // serialize the bitset, not necessicarily in a human readable way. the bitset's
    // format must be encoded in the serialization so that bitset::deserialize
    // can reconstruct it in the correct format. see bitset::deserialize 
    // (bitset_serialize.h) for info on the serialization format.
    virtual SerializedPtr serialize() const;

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

// helper functions
namespace bitset {
    // returns a new bitset containing the result of a bitwise AND between the 
    // arguments. the format of the returned bitset will be that of the lhs bitset.
    // it is up to the implimentation of &= to make sure that the two bitsets
    // are compatible for the operation.
    std::unique_ptr<Bitset> doAnd(const Bitset &lhs, const Bitset &rhs);

    // returns a new bitset containing the result of a bitwise OR between the 
    // arguments. the format of the returned bitset will be that of the lhs bitset.
    // it is up to the implimentation of |= to make sure that the two bitsets
    // are compatible for the operation.
    std::unique_ptr<Bitset> doOr(const Bitset &lhs, const Bitset &rhs);

    // generic function to copy the bits from source to dest. since we don't know
    // the underlying types of the bitsets, this is a slow, one-by-one copy. use
    // this function only if you can't copy any other way. dest and source must
    // be the same sizeInBits().
    void copyBits(Bitset &dest, const Bitset &source);
} // bitset
