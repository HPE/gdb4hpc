/******************************************************************************\
 * Copyright 2021 Hewlett Packard Enterprise Development LP.
 ******************************************************************************/
#pragma once

#include <memory>
#include <string>
#include <set>
#include <limits>
#include <vector>
#include <stdexcept>

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
    // return a new bitset of size nbits with the same set bits as the old
    // bitset. newly created bits and truncated bits are set to 0.
    virtual std::unique_ptr<Bitset> clone(size_t nbits) const = 0;
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
    static const size_t None;

    // returns the offset of the first set bit. offset starts from LSB. if no
    // bit is set, returns Bitset::None.
    virtual size_t firstSet() const;

    // returns a human readable string representation of the bitset.
    virtual std::string toString() const;

    // returns a string representation in set notation, e.g. {0..2,4,5..100}.
    virtual std::string toSetString(size_t offset = 0, size_t len = Bitset::None) const;

    // returns a set containing the individual ranks of the bitset
    virtual std::set<size_t> toSet() const;

    // a serialized bitset, not necessarily in a human readable format. derived
    // classes should implement a (SerializedPtr) constructor to
    // do deserialization.
    // e.g.
    //   auto serialized_bitset = bitset->serialize();
    //   auto new_bitset = Bitset(serialized_bitset.get());
    using SerializedPtr = const char *;
    using Serialized = std::unique_ptr<char, decltype(&std::free)>;

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
    // 0b0000 < 0b0001 {1} < 0b0010 {2} < 0b0011 {1..2} < ... < 0b1111
    virtual bool operator< (const Bitset &other) const;

    // returns true if lhs & rhs != 0.
    virtual bool hasCommonBits(const Bitset &other) const;

    virtual size_t hash() const;

    // iterates over *set bits* in the bitset, skipping all unset bits.
    // returns Bitset::None when no more bits are left
    class SetBitsIterator {
    public:
        virtual ~SetBitsIterator() = default;
        virtual size_t operator*() const = 0;
        virtual SetBitsIterator& operator++() = 0;
    };

    // XXX: Can't make a proper "std like" interface (begin(), end()) because
    // those need to return iterators by value, not reference, and therefore can't use polymorphism
    virtual std::unique_ptr<SetBitsIterator> iterator() const = 0;

protected:
    Bitset() {}
    Bitset(const Bitset&) = default;
};

struct BitsetHasher {
    size_t operator()(const Bitset *bitset) const {
        if (!bitset) throw std::runtime_error("BitsetHasher tried to hash a nullptr");
        return bitset->hash();
    }

    struct KeyEqual {
        bool operator()(const Bitset *a, const Bitset *b) const {
            if (!a || !b) throw std::runtime_error("Tried to compare a null bitset pointer");
            return *a == *b;
        }
    };
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

template <typename T>
class BitsetMRUCache {
    // Bitset Most Recently Used Cache. See unit tests in bitset_tests.h for usage examples.
    //
    // This data structure is designed to quickly keep track of data associated with ranks
    // (represented by their bitsets) over time. Entries are `push`ed
    // into the data structure and then retrieved by iterating. A newly pushed
    // entry will overwrite any previously stored entry with matching bits in the bitset.
    // A partially matching entry will only partially overwrite a previous item.
    //
    // Keeping a perfect record of data for every rank of a 100,000+ rank job would
    // get slow and big. To stay fast and small, this data structure makes some sacrifices in the
    // amount of data it stores and lookup abilities in exchange for fast `push`ing and iteration.
    //
    // Its sacrifices are similar to those of a cache in a CPU, hence the name.
    //   - In order to keep iteration fast, the cache is not big enough to store a huge
    //     amount of data. As more `push`es come in, the oldest entries will be overwritten.
    //   - When iterating, there is no way to know what bitset an item in the
    //     cache was originally `push`ed with. (But you can set up a mask before you start iterating)
    //
    // These sacrifices make the data structure well suited for "best-effort" tracking jobs
    // where permanence and completeness are not necessary. It was originally designed
    // to quickly and passively track recent rank locations as they changed to assist with tab completion.
    //
    // Alternatively, it is suited for use cases where an algorithm that needs data checks
    // the cache first with a full lookup fallback.
private:
    class Iterator;
    friend Iterator;

    class MaskedBitsetMRUCache;

public:
    // `full_bitset` should be a bitset of all 1s that represents the space of data that will
    // be tracked.
    BitsetMRUCache(const Bitset& full_bitset);

    // create a new entry in the cache. if there was already an entry associated
    // with `bitset`, the newly pushed entry will "replace" it. The entry is
    // only logically replaced - the old entry is not necessarily
    // deallocated/destroyed immediately. Old entries are only destroyed when
    // the cache's internal list becomes full list slots are reused (or the
    // cache itself is destroyed).
    //
    // the `push`ed `bitset` should never be nullptr.
    void push(std::unique_ptr<Bitset>&& bitset, T&& item);

    // `for (const auto& item : cache) {}`.
    // read-only iterate over the items in the cache, covering each bit in
    // the constructor's `full_bitset` at most once. iteration order is most
    // recently `push`ed entries first.
    //
    // note that the iterator holds a `const T&` and no associated bitset. there
    // is no way to know what bitset the T is/was associated with during
    // iteration (but see `masked`).
    //
    // iterators follow the usual iterator rules - they are only valid as long
    // as the original cache is valid, and if the original cache is modified,
    // iterators are invalidated.
    Iterator begin() const;
    Iterator end() const;

    // `for (const auto& item : cache.masked(...)) {}`
    //
    // like normal iteration, but a bitmask can be given to filter results.
    //
    // this is the intended way to do something like `cache.find(...)`. a
    // standalone `find` method would always have to return a vector anyway
    // because of use cases like
    //   push(1111, t1)
    //   push(0011, t2)
    //   find(1111) -> [t2, t1]
    //
    // during iteration, items returned will cover `mask` at most once.
    //
    // normal iteration (via `begin`) is equivalent to
    // `for (const auto& item : cache.masked(full_bitset))`, where `full_bitset`
    // is the set that was passed to the contructor. this means that you can
    // construct a cache that's always masked to a certain bitset by default
    // ...if you want.
    MaskedBitsetMRUCache masked(const Bitset& mask) const;

    // the maximum amount of entries the cache will ever store.
    static constexpr auto max_capacity = 1024ul;

private:
    class Iterator {
    public:
        const T& operator*() const;
        Iterator& operator++();
        bool operator==(const Iterator& other) const;
        bool operator!=(const Iterator& other) const;
    private:
        friend BitsetMRUCache;
        friend MaskedBitsetMRUCache;
        Iterator(const BitsetMRUCache& cache, std::unique_ptr<Bitset>&& mask);
        static Iterator make_end(const BitsetMRUCache& cache);

        void move_to_match();
        void become_end();

        const BitsetMRUCache& m_cache;
        std::unique_ptr<Bitset> m_mask;
        size_t m_offset = 0;
    };

    class MaskedBitsetMRUCache {
        friend BitsetMRUCache;
        MaskedBitsetMRUCache(const BitsetMRUCache& cache, std::unique_ptr<Bitset>&& mask);
        const BitsetMRUCache& m_cache;
        std::unique_ptr<Bitset> m_mask;

    public:
        Iterator begin() const;
        Iterator end() const;
    };

    struct Entry {
        std::unique_ptr<Bitset> bitset;
        // unique_ptr instead of raw T so T doesn't need to implement operator= and friends
        // and so that the default constructed Entry<T> has a nil value that makes sense
        std::unique_ptr<T> item;
    };

    // return the index of the most recently added item. undefined if empty.
    size_t top_index() const;

    // this is a std::vector so that an appropriately sized list can be allocated at ctor
    // time - it should never change length after that.
    std::vector<Entry> m_list;
    size_t m_next_avail_slot;
    size_t m_num_slots_used;

    std::unique_ptr<Bitset> m_fullset;
};

template <typename T>
BitsetMRUCache<T>::BitsetMRUCache(const Bitset& full_bitset)
    // enough to store data for every individual rank for small jobs, or some arbitrary upper limit.
    // TODO: benchmark the effects of the upper limit and try to increase the upper limit while staying fast.
    : m_list(std::min(BitsetMRUCache<T>::max_capacity, full_bitset.sizeInBits()))

    , m_next_avail_slot{m_list.size() - 1}
    , m_num_slots_used{0}
    , m_fullset{full_bitset.clone()}
{}

template <typename T>
void BitsetMRUCache<T>::push(std::unique_ptr<Bitset>&& bitset, T&& item) {
    // trivial optimization - if the `bitset` matches the top bitset exactly, just reuse the slot.
    // TODO: experiment with doing this check on the top n items - potentially all of them?
    if (m_num_slots_used > 0 && *m_list[top_index()].bitset == *bitset) {
        m_list[top_index()] =
            { .bitset = std::move(bitset), .item = std::make_unique<T>(std::move(item)) };

        return;
    }

    m_list[m_next_avail_slot] =
        { .bitset = std::move(bitset), .item = std::make_unique<T>(std::move(item)) };

    // we fill the array backwards so that iterators emit most recent first when they go forwards
    if (m_next_avail_slot > 0) {
        m_next_avail_slot--;
    } else {
        m_next_avail_slot = m_list.size() - 1;
    }

    if (m_num_slots_used < m_list.size()) m_num_slots_used++;
}

template <typename T>
BitsetMRUCache<T>::Iterator BitsetMRUCache<T>::begin() const {
    // nullptr case will return end()
    return BitsetMRUCache<T>::Iterator(*this, m_fullset ? m_fullset->clone() : nullptr);
}

template <typename T>
BitsetMRUCache<T>::Iterator BitsetMRUCache<T>::end() const {
    return Iterator::make_end(*this);
}

template <typename T>
BitsetMRUCache<T>::MaskedBitsetMRUCache BitsetMRUCache<T>::masked(const Bitset& mask) const {
    return MaskedBitsetMRUCache(*this, mask.clone());
}

template <typename T>
size_t BitsetMRUCache<T>::top_index() const {
    return (m_next_avail_slot + 1) % m_list.size();
}

template <typename T>
BitsetMRUCache<T>::Iterator::Iterator(const BitsetMRUCache<T>& parent, std::unique_ptr<Bitset>&& mask)
    : m_cache{parent}
    , m_mask{std::move(mask)}
    , m_offset{0}
{
    if (m_cache.m_num_slots_used > 0 && m_mask != nullptr) {
        move_to_match();
    } else {
        become_end();
    }
}

template <typename T>
BitsetMRUCache<T>::Iterator BitsetMRUCache<T>::Iterator::make_end(const BitsetMRUCache<T>& parent) {
    // ctor will interpret nullptr mask as meaning end()
    return Iterator(parent, nullptr);
}

template <typename T>
void BitsetMRUCache<T>::Iterator::become_end() {
    m_offset = std::numeric_limits<size_t>::max();
}

template <typename T>
void BitsetMRUCache<T>::Iterator::move_to_match() {
    const auto* entry = &m_cache.m_list[(m_cache.top_index() + m_offset) % m_cache.m_list.size()];

    while (m_offset < m_cache.m_num_slots_used
        && !m_mask->isEmpty()
        && !m_mask->hasCommonBits(*entry->bitset))
    {
        m_offset++;
        entry = &m_cache.m_list[(m_cache.top_index() + m_offset) % m_cache.m_list.size()];
    }

    if (m_offset < m_cache.m_num_slots_used && !m_mask->isEmpty()) {
        *m_mask -= *entry->bitset;
    } else {
        // no valid item found
        become_end();
    }
}

template <typename T>
const T& BitsetMRUCache<T>::Iterator::operator*() const {
    return *m_cache.m_list[(m_cache.top_index() + m_offset) % m_cache.m_list.size()].item;
}

template <typename T>
BitsetMRUCache<T>::Iterator& BitsetMRUCache<T>::Iterator::operator++() {
    m_offset++;
    move_to_match();
    return *this;
}

template <typename T>
bool BitsetMRUCache<T>::Iterator::operator==(const BitsetMRUCache<T>::Iterator& other) const {
    return &m_cache == &other.m_cache && m_offset == other.m_offset;
}

template <typename T>
bool BitsetMRUCache<T>::Iterator::operator!=(const BitsetMRUCache<T>::Iterator& other) const {
    return !(*this == other);
}

template <typename T>
BitsetMRUCache<T>::MaskedBitsetMRUCache::MaskedBitsetMRUCache(const BitsetMRUCache& parent, std::unique_ptr<Bitset>&& mask)
    : m_cache{parent}
    , m_mask{std::move(mask)}
{}

template <typename T>
BitsetMRUCache<T>::Iterator BitsetMRUCache<T>::MaskedBitsetMRUCache::begin() const {
    // nullptr case will return end()
    return BitsetMRUCache<T>::Iterator(m_cache, m_mask ? m_mask->clone() : nullptr);
}

template <typename T>
BitsetMRUCache<T>::Iterator BitsetMRUCache<T>::MaskedBitsetMRUCache::end() const {
    return Iterator::make_end(m_cache);
}
