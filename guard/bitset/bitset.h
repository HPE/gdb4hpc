/******************************************************************************\
 * Copyright 2021 Hewlett Packard Enterprise Development LP.
 ******************************************************************************/
#pragma once

#include <memory>
#include <string>
#include <limits>
#include <variant>

class Bitset {
    // interface describing a bitset that is used to represent sets of ranks.

    // when operating on multiple ranks, gdb4hpc identifies each by its bitset.
    // when messages from the dbgsrv are aggregated, bitsets are used to indicate
    // where each message came from.

    // in order to be useful, any bitset implimentation must implement set,
    // unset, test, sizeInBits, and clone. the rest of the operations have
    // default implementations in terms of the required operations, but default
    // implementations operate on one bit at a time and are slow. derived
    // classes should implement faster versions whenever possible.
public:
    virtual ~Bitset() = default;

    // pure virtual functions:

    // clone the bitset.
    virtual std::unique_ptr<Bitset> clone() const = 0;
    // sets a bit to 1. offset starts from LSB.
    virtual void set(size_t bit) = 0;
    // sets a bit to 0. offset starts from LSB.
    virtual void unset(size_t bit) = 0;
    // returns true if the bit at the given offset is 1. offset starts from LSB.
    // if an out-of-bounds bit is tested, the result should be false.
    virtual bool test(size_t bit) const = 0;
    // returns the size of the bitset in bits (not bytes).
    virtual size_t sizeInBits() const = 0;

    // virtual functions:

    // returns true if all bits are 0.
    virtual bool isEmpty() const;
    // returns true if all bits are 1.
    virtual bool isFull() const;
    
    // inverts (bitwise NOT) all bits.
    virtual void invert();
    // sets all bits to 0.
    virtual void clear();

    // returns the number of bits set to 1.
    virtual size_t numSet() const;

    // used to indicate "no bits" in operations such as firstSet()
    static const size_t None = std::numeric_limits<size_t>::max();

    // returns the offset of the first set bit. offset starts from LSB. if no
    // bit is set, returns Bitset::None.
    virtual size_t firstSet() const;

    // returns a human readable string representation of the bitset.
    virtual std::string toString() const;

    // returns a string representation in set notation, e.g. {0..2,4,5..100}.
    virtual std::string toSet(size_t offset = 0, size_t len = Bitset::None) const;

    // a serialized bitset, not necessarily in a human readable format. derived
    // classes should implement a (SerializedPtr) constructor to
    // do deserialization.
    // e.g.
    //   auto serialized_bitset = bitset->serialize();
    //   auto new_bitset = Bitset(serialized_bitset.get());
    using SerializedPtr = const char *;
    using Serialized = std::unique_ptr<char>;

    // returns a Bitset::Serialized class that can be used to construct a new
    // bitset.
    virtual Serialized serialize() const;

    // for binary operators, it is up to the implimentation to make sure the
    // formats of the two bitsets being operated on are compatible. if they
    // aren't, they should fall back to the default implementations.

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
} // bitset
