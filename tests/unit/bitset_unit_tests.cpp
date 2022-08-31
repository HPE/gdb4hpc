#include <config.h>

#include "gtest/gtest.h"

#include "bitset.h"
#include "dense_bitset.h"

#ifdef MRNET
#include "sparse_bitset.h"
#endif

#ifdef ZMQNET
#include "lrunle_bitset.h"
#endif

#include <iostream>
#include <unordered_set>

template <typename Bitset>
class BitsetUnitTest : public ::testing::Test {
protected:
    const size_t SIZE = 128;

    BitsetUnitTest() 
        : m_full(SIZE) 
        , m_empty(SIZE)
        , m_sparse(SIZE)
        , m_hibit(SIZE)
        , m_lobit(SIZE)
    {
        for (size_t i = 0; i < SIZE; i++) {
            m_full.set(i);
            if (i % 4 == 0) m_sparse.set(i);
        }
        m_hibit.set(SIZE-1);
        m_lobit.set(0);
    }

    Bitset m_full;
    Bitset m_empty;
    Bitset m_sparse;
    Bitset m_hibit;
    Bitset m_lobit;
};

using testing::Types;

// These tests directly link libbitset, which is built at gdb4hpc build time. 
// Depending on the protocols selected at gdb4hpc build time, libbitset might 
// not have all the bitset types included in it, so we have to do some funny 
// stuff to make sure that we don't try to test a bitset that doesn't exist.
// DenseBitset is always defined.

#ifdef MRNET
#define MRNET_BITSETS SparseBitset,
#else   
// define as blank
#define MRNET_BITSETS
#endif

#ifdef ZMQNET
#define ZMQNET_BITSETS LRunLEBitset,
#else
// define as blank
#define ZMQNET_BITSETS
#endif

typedef Types<MRNET_BITSETS ZMQNET_BITSETS DenseBitset> BitsetImplimentations;

TYPED_TEST_CASE(BitsetUnitTest, BitsetImplimentations);

// the weird "this->m_..." syntax is required because of how google test works.

TYPED_TEST(BitsetUnitTest, clone) {
    auto clone_full = this->m_full.clone();
    auto clone_empty = this->m_empty.clone();
    auto clone_sparse = this->m_sparse.clone();

    ASSERT_TRUE(*clone_full == this->m_full);
    ASSERT_TRUE(*clone_empty == this->m_empty);
    ASSERT_TRUE(*clone_sparse == this->m_sparse);
}

TYPED_TEST(BitsetUnitTest, resizeCloneGrow) {
    const auto newSize = this->SIZE*2;

    auto clone_full = this->m_full.clone(newSize);
    auto clone_empty = this->m_empty.clone(newSize);
    auto clone_sparse = this->m_sparse.clone(newSize);

    ASSERT_TRUE(*clone_full != this->m_full);
    ASSERT_TRUE(*clone_empty != this->m_empty);
    ASSERT_TRUE(*clone_sparse != this->m_sparse);

    for (size_t i = this->SIZE; i < newSize; i++) {
        ASSERT_TRUE(clone_full->test(i) == false);
        ASSERT_TRUE(clone_empty->test(i) == false);
        ASSERT_TRUE(clone_sparse->test(i) == false);
    }

    clone_full->set(newSize-1);
    clone_empty->set(newSize-1);
    clone_sparse->set(newSize-1);

    ASSERT_EQ(clone_full->numSet(), this->SIZE + 1);
    ASSERT_TRUE(clone_full->test(newSize - 1));
    ASSERT_EQ(clone_empty->numSet(), 1ul);
    ASSERT_TRUE(clone_empty->test(newSize - 1));
    ASSERT_EQ(clone_sparse->numSet(), this->m_sparse.numSet() + 1);
    ASSERT_TRUE(clone_sparse->test(newSize - 1));
}

TYPED_TEST(BitsetUnitTest, resizeCloneShrink) {
    const auto newSize = this->SIZE/2;

    auto clone_full = this->m_full.clone(newSize);
    auto clone_empty = this->m_empty.clone(newSize);
    auto clone_sparse = this->m_sparse.clone(newSize);

    ASSERT_TRUE(*clone_full != this->m_full);
    ASSERT_TRUE(*clone_empty != this->m_empty);
    ASSERT_TRUE(*clone_sparse != this->m_sparse);

    for (size_t i = 0; i < newSize; i++) {
        ASSERT_TRUE(clone_full->test(i) == true);
        ASSERT_TRUE(clone_empty->test(i) == false);
    }

    // old bits should be 0
    for (size_t i = newSize; i < this->SIZE; i++) {
        ASSERT_TRUE(clone_full->test(i) == false);
        ASSERT_TRUE(clone_empty->test(i) == false);
        ASSERT_TRUE(clone_sparse->test(i) == false);
    }

    ASSERT_EQ(clone_full->numSet(), newSize);
    ASSERT_TRUE(clone_full->test(newSize - 1));
    ASSERT_EQ(clone_empty->numSet(), 0ul);
    ASSERT_FALSE(clone_empty->test(newSize - 1));
    ASSERT_LT(clone_sparse->numSet(), this->m_sparse.numSet());
}

TYPED_TEST(BitsetUnitTest, isFull) {
    ASSERT_TRUE(this->m_full.isFull());
    ASSERT_FALSE(this->m_empty.isFull());
    ASSERT_FALSE(this->m_sparse.isFull());
    ASSERT_FALSE(this->m_hibit.isFull());
    ASSERT_FALSE(this->m_lobit.isFull());
}

TYPED_TEST(BitsetUnitTest, isEmpty) {
    ASSERT_FALSE(this->m_full.isEmpty());
    ASSERT_TRUE(this->m_empty.isEmpty());
    ASSERT_FALSE(this->m_sparse.isEmpty());
    ASSERT_FALSE(this->m_hibit.isEmpty());
    ASSERT_FALSE(this->m_lobit.isEmpty());
}

TYPED_TEST(BitsetUnitTest, sizeInBits) {
    ASSERT_EQ(this->m_full.sizeInBits(), this->SIZE);
    ASSERT_EQ(this->m_empty.sizeInBits(), this->SIZE);
    ASSERT_EQ(this->m_sparse.sizeInBits(), this->SIZE);
    ASSERT_EQ(this->m_hibit.sizeInBits(), this->SIZE);
    ASSERT_EQ(this->m_lobit.sizeInBits(), this->SIZE);
}

TYPED_TEST(BitsetUnitTest, numSet) {
    ASSERT_EQ(this->m_full.numSet(), this->SIZE);
    ASSERT_EQ(this->m_empty.numSet(), 0lu);
    ASSERT_NE(this->m_sparse.numSet(), 0lu);
    ASSERT_NE(this->m_sparse.numSet(), this->SIZE);
    ASSERT_EQ(this->m_hibit.numSet(), 1u);
    ASSERT_EQ(this->m_lobit.numSet(), 1u);
}

TYPED_TEST(BitsetUnitTest, firstSet) {
    // this needs to be grabbed outside of a macro, for some reason
    auto none = Bitset::None;

    ASSERT_EQ(this->m_full.firstSet(), 0lu);
    ASSERT_EQ(this->m_sparse.firstSet(), 0lu);
    ASSERT_EQ(this->m_empty.firstSet(), none);
    ASSERT_EQ(this->m_hibit.firstSet(), this->SIZE - 1);
    ASSERT_EQ(this->m_lobit.firstSet(), 0lu);
}

TYPED_TEST(BitsetUnitTest, test) {
    for (size_t i = 0; i < this->SIZE; i++) {
        ASSERT_EQ(this->m_full.test(i), true);
        ASSERT_EQ(this->m_empty.test(i), false);
    }

    ASSERT_EQ(this->m_hibit.test(this->SIZE-1), true);
    ASSERT_EQ(this->m_lobit.test(this->SIZE-1), false);

    ASSERT_EQ(this->m_hibit.test(0), false);
    ASSERT_EQ(this->m_lobit.test(0), true);

    // out of bounds test should return false
    ASSERT_EQ(this->m_full.test(-1), false);
    ASSERT_EQ(this->m_full.test(this->SIZE + 1), false);
}

TYPED_TEST(BitsetUnitTest, toSetString) {
    ASSERT_EQ(this->m_full.toSetString(), "{0.." + std::to_string(this->SIZE - 1) + "}");
    ASSERT_EQ(this->m_empty.toSetString(), "{}");
    ASSERT_NE(this->m_sparse.toSetString(), "{}");
    ASSERT_NE(this->m_sparse.toSetString(), "{0.." + std::to_string(this->SIZE - 1) + "}");
    ASSERT_EQ(this->m_full.toSetString(0, this->SIZE / 2), "{0.." + std::to_string(this->SIZE/2 - 1) + "}");
    ASSERT_EQ(this->m_hibit.toSetString(), "{" + std::to_string(this->SIZE - 1) + "}");
    ASSERT_EQ(this->m_lobit.toSetString(), "{0}");
}

TYPED_TEST(BitsetUnitTest, Equals) {
    ASSERT_TRUE(this->m_full == this->m_full);
    ASSERT_TRUE(this->m_full != this->m_empty);
    ASSERT_TRUE(this->m_sparse != this->m_full);
    ASSERT_TRUE(this->m_sparse != this->m_empty);
    ASSERT_TRUE(this->m_sparse == this->m_sparse);

    // was once an infinite recursion bug
    this->m_empty = dynamic_cast<Bitset&>(this->m_sparse);
    ASSERT_TRUE(this->m_empty == this->m_sparse);
}

TYPED_TEST(BitsetUnitTest, LessThan) {
    ASSERT_TRUE(this->m_empty < this->m_full);
    ASSERT_TRUE(this->m_empty < this->m_sparse);

    ASSERT_FALSE(this->m_full < this->m_full);
    ASSERT_FALSE(this->m_empty < this->m_empty);
    ASSERT_FALSE(this->m_sparse < this->m_sparse);

    auto left  = this->m_empty.clone();
    auto right = this->m_empty.clone();

    ASSERT_FALSE(*left < *right);
    
    left->set(0);
    right->set(1);
    // {0} < {1}
    ASSERT_TRUE(*left < *right);

    right->set(0);
    // {0} < {0..1}
    ASSERT_TRUE(*left < *right);

    left->set(1);
    right->set(2);
    // {0..1} < {0..2}
    ASSERT_TRUE(*left < *right);

    auto left2 = this->m_empty.clone();
    left2->set(0);
    left2->set(2);
    // {0, 2} < {0..2}
    ASSERT_TRUE(*left < *right);

    left->clear();
    right->clear();
    left->set(1);
    right->set(0);
    // {1} not < {0}
    ASSERT_FALSE(*left < *right);
}

TYPED_TEST(BitsetUnitTest, And) {
    auto random_anded_full = bitset::fromAnd(this->m_sparse, this->m_full);
    auto random_anded_empty = bitset::fromAnd(this->m_sparse, this->m_empty);
    auto random_anded_random = bitset::fromAnd(this->m_sparse, this->m_sparse);
    auto random_anded_full_r = bitset::fromAnd(this->m_full, this->m_sparse);
    auto random_anded_empty_r = bitset::fromAnd(this->m_empty, this->m_sparse);

    ASSERT_TRUE(*random_anded_full == *random_anded_full_r);
    ASSERT_TRUE(*random_anded_empty == *random_anded_empty_r);

    ASSERT_TRUE(*random_anded_full == this->m_sparse);
    ASSERT_TRUE(*random_anded_empty == this->m_empty);
    ASSERT_TRUE(*random_anded_random == this->m_sparse);

    *random_anded_random &= this->m_full;
    ASSERT_TRUE(*random_anded_random == this->m_sparse);

    *random_anded_random &= this->m_empty;
    ASSERT_TRUE(*random_anded_random == this->m_empty);
    ASSERT_TRUE(random_anded_random->isEmpty());
}

TYPED_TEST(BitsetUnitTest, Or) {
    auto random_ord_full = bitset::fromOr(this->m_sparse, this->m_full);
    auto random_ord_empty = bitset::fromOr(this->m_sparse, this->m_empty);
    auto random_ord_random = bitset::fromOr(this->m_sparse, this->m_sparse);
    auto random_ord_full_r = bitset::fromOr(this->m_full, this->m_sparse);
    auto random_ord_empty_r = bitset::fromOr(this->m_empty, this->m_sparse);

    ASSERT_TRUE(*random_ord_full == this->m_full);
    ASSERT_TRUE(*random_ord_empty == this->m_sparse);
    ASSERT_TRUE(*random_ord_random == this->m_sparse);

    *random_ord_random |= this->m_empty;
    ASSERT_TRUE(*random_ord_random == this->m_sparse);
    ASSERT_TRUE(!random_ord_random->isEmpty());
    
    *random_ord_random |= this->m_full;
    ASSERT_TRUE(*random_ord_random == this->m_full);
}

TYPED_TEST(BitsetUnitTest, setRemoval) {
#ifdef ZMQNET
    if (std::is_same<TypeParam, LRunLEBitset>::value) {
        std::cerr << "LRunLE bitset does not support the unset operation, required for set removal. Skipping test and reporting success.\n";
        return;
    }
#endif

    this->m_full -= this->m_sparse;
    ASSERT_FALSE(this->m_full.hasCommonBits(this->m_sparse));
}

TYPED_TEST(BitsetUnitTest, invert) {
#ifdef ZMQNET
    if (std::is_same<TypeParam, LRunLEBitset>::value) {
        std::cerr << "LRunLE bitset does not support the invert operation. Skipping test and reporting success.\n";
        return;
    }
#endif

    auto full2 = this->m_full.clone();
    full2->invert();
    ASSERT_TRUE(*full2 == this->m_empty);
    full2->invert();
    ASSERT_TRUE(full2->isFull());
}

TYPED_TEST(BitsetUnitTest, hasCommonBits) {
    ASSERT_TRUE(this->m_full.hasCommonBits(this->m_sparse));
    ASSERT_TRUE(!this->m_empty.hasCommonBits(this->m_sparse));
}

TYPED_TEST(BitsetUnitTest, setUnset) {
#ifdef ZMQNET
    if (std::is_same<TypeParam, LRunLEBitset>::value) {
        std::cerr << "LRunLE bitset does not support the unset operation. Skipping test and reporting success.\n";
        return;
    }
#endif

    auto random_clone = this->m_sparse.clone();
    ASSERT_TRUE(*random_clone == this->m_sparse);
    
    auto first = random_clone->firstSet();
    ASSERT_TRUE(first != Bitset::None);
    random_clone->unset(first);
    ASSERT_TRUE(*random_clone != this->m_sparse);
    random_clone->set(first);
    ASSERT_TRUE(*random_clone == this->m_sparse);
}

TYPED_TEST(BitsetUnitTest, clear) {
    auto full2 = this->m_full.clone();
    auto empty2 = this->m_empty.clone();
    auto random2 = this->m_sparse.clone();
    full2->clear();
    empty2->clear();
    random2->clear();

    ASSERT_TRUE(full2->isEmpty());
    ASSERT_TRUE(full2->numSet() == 0);
    ASSERT_TRUE(empty2->isEmpty());
    ASSERT_TRUE(empty2->numSet() == 0);
    ASSERT_TRUE(random2->isEmpty());
    ASSERT_TRUE(random2->numSet() == 0);
}

TYPED_TEST(BitsetUnitTest, serialize) {
    auto full_s = this->m_full.serialize();
    auto empty_s = this->m_empty.serialize();
    auto sparse_s = this->m_sparse.serialize();

    // Call deserialization constructor
    auto full_ds = TypeParam(full_s.get());
    auto empty_ds = TypeParam(empty_s.get());
    auto sparse_ds = TypeParam(sparse_s.get());

    ASSERT_TRUE(full_ds == this->m_full);
    ASSERT_TRUE(empty_ds != this->m_full);
    ASSERT_TRUE(full_ds != this->m_empty);
    ASSERT_TRUE(sparse_ds == this->m_sparse);
}

TYPED_TEST(BitsetUnitTest, hash) {
    auto map = std::unordered_set<Bitset*, BitsetHasher>{};

    map.insert(&this->m_full);
    map.insert(&this->m_sparse);
    map.insert(&this->m_empty);

    ASSERT_TRUE(map.size() == 3);
}

TYPED_TEST(BitsetUnitTest, iteratorFull) {
    size_t count = 0;
    auto iterator_storage = this->m_full.iterator();

    // iterators should return Bitset::None when they're done
    for (auto& it = *iterator_storage; *it != Bitset::None && count < 999999; ++it) {
        count++;
    }

    ASSERT_EQ(this->m_full.sizeInBits(), count);
}

TYPED_TEST(BitsetUnitTest, iteratorEmpty) {
    size_t count = 0;
    auto iterator_storage = this->m_empty.iterator();

    // iterators should return Bitset::None when they're done
    for (auto& it = *iterator_storage; *it != Bitset::None && count < 999999; ++it) {
        count++;
    }

    ASSERT_EQ(count, 0u);
}

TYPED_TEST(BitsetUnitTest, iteratorSparse) {
    size_t count = 0;
    auto iterator_storage = this->m_sparse.iterator();

    // iterators should return Bitset::None when they're done
    for (auto& it = *iterator_storage; *it != Bitset::None && count < 999999; ++it) {
        count++;
    }

    ASSERT_GT(count, 0u);
    ASSERT_LT(count, this->m_sparse.sizeInBits());
}

TYPED_TEST(BitsetUnitTest, iteratorHi) {
    size_t count = 0;
    auto iterator_storage = this->m_hibit.iterator();

    // iterators should return Bitset::None when they're done
    for (auto& it = *iterator_storage; *it != Bitset::None && count < 999999; ++it) {
        ASSERT_EQ(*it, this->m_hibit.sizeInBits() - 1);
        count++;
    }

    ASSERT_EQ(count, 1u);
}

TYPED_TEST(BitsetUnitTest, iteratorLo) {
    size_t count = 0;
    auto iterator_storage = this->m_lobit.iterator();

    // iterators should return Bitset::None when they're done
    for (auto& it = *iterator_storage; *it != Bitset::None && count < 999999; ++it) {
        ASSERT_EQ(*it, 0u);
        count++;
    }

    ASSERT_EQ(count, 1u);
}
