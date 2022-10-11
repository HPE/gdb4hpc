/******************************************************************************\
 * Copyright 2022 Hewlett Packard Enterprise Development LP.
 ******************************************************************************/

#include "bitset.h"

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "doctest.h"

#include <unordered_set>

TEST_CASE_TEMPLATE_DEFINE("libbitset", BitsetT, libbitset) {
    const size_t SIZE = 128;
    BitsetT full(SIZE);
    BitsetT empty(SIZE);
    BitsetT sparse(SIZE);
    BitsetT hibit(SIZE);
    BitsetT lobit(SIZE);

    for (size_t i = 0; i < SIZE; i++) {
        full.set(i);
        if (i % 4 == 0) sparse.set(i);
    }
    hibit.set(SIZE-1);
    lobit.set(0);

    SUBCASE("clone") {
        auto clone_full = full.clone();
        auto clone_empty = empty.clone();
        auto clone_sparse = sparse.clone();

        CHECK(*clone_full == full);
        CHECK(*clone_empty == empty);
        CHECK(*clone_sparse == sparse);
    }

    SUBCASE("resizeCloneGrow") {
        const auto newSize = SIZE*2;

        auto clone_full = full.clone(newSize);
        auto clone_empty = empty.clone(newSize);
        auto clone_sparse = sparse.clone(newSize);

        CHECK(*clone_full != full);
        CHECK(*clone_empty != empty);
        CHECK(*clone_sparse != sparse);

        for (size_t i = SIZE; i < newSize; i++) {
            CHECK(clone_full->test(i) == false);
            CHECK(clone_empty->test(i) == false);
            CHECK(clone_sparse->test(i) == false);
        }

        clone_full->set(newSize-1);
        clone_empty->set(newSize-1);
        clone_sparse->set(newSize-1);

        CHECK(clone_full->numSet() == SIZE + 1);
        CHECK(clone_full->test(newSize - 1));
        CHECK(clone_empty->numSet() == 1ul);
        CHECK(clone_empty->test(newSize - 1));
        CHECK(clone_sparse->numSet() == sparse.numSet() + 1);
        CHECK(clone_sparse->test(newSize - 1));
    }

    SUBCASE("resizeCloneShrink") {
        const auto newSize = SIZE/2;

        auto clone_full = full.clone(newSize);
        auto clone_empty = empty.clone(newSize);
        auto clone_sparse = sparse.clone(newSize);

        CHECK(*clone_full != full);
        CHECK(*clone_empty != empty);
        CHECK(*clone_sparse != sparse);

        for (size_t i = 0; i < newSize; i++) {
            CHECK(clone_full->test(i) == true);
            CHECK(clone_empty->test(i) == false);
        }

        // old bits should be 0
        for (size_t i = newSize; i < SIZE; i++) {
            CHECK(clone_full->test(i) == false);
            CHECK(clone_empty->test(i) == false);
            CHECK(clone_sparse->test(i) == false);
        }

        CHECK(clone_full->numSet() == newSize);
        CHECK(clone_full->test(newSize - 1));
        CHECK(clone_empty->numSet() == 0ul);
        CHECK(!clone_empty->test(newSize - 1));
        CHECK(clone_sparse->numSet() < sparse.numSet());
    }

    SUBCASE("isFull") {
        CHECK(full.isFull());
        CHECK(!empty.isFull());
        CHECK(!sparse.isFull());
        CHECK(!hibit.isFull());
        CHECK(!lobit.isFull());
    }

    SUBCASE("isEmpty") {
        CHECK(!full.isEmpty());
        CHECK(empty.isEmpty());
        CHECK(!sparse.isEmpty());
        CHECK(!hibit.isEmpty());
        CHECK(!lobit.isEmpty());
    }

    SUBCASE("sizeInBits") {
        CHECK(full.sizeInBits() == SIZE);
        CHECK(empty.sizeInBits() == SIZE);
        CHECK(sparse.sizeInBits() == SIZE);
        CHECK(hibit.sizeInBits() == SIZE);
        CHECK(lobit.sizeInBits() == SIZE);
    }

    SUBCASE("numSet") {
        CHECK(full.numSet() == SIZE);
        CHECK(empty.numSet() == 0lu);
        CHECK(sparse.numSet() != 0lu);
        CHECK(sparse.numSet() != SIZE);
        CHECK(hibit.numSet() == 1u);
        CHECK(lobit.numSet() == 1u);
    }

    SUBCASE("firstSet") {
        // this needs to be grabbed outside of a macro, for some reason
        auto none = Bitset::None;

        CHECK(full.firstSet() == 0lu);
        CHECK(sparse.firstSet() == 0lu);
        CHECK(empty.firstSet() == none);
        CHECK(hibit.firstSet() == SIZE - 1);
        CHECK(lobit.firstSet() == 0lu);
    }

    SUBCASE("test") {
        for (size_t i = 0; i < SIZE; i++) {
            CHECK(full.test(i) == true);
            CHECK(empty.test(i) == false);
        }

        CHECK(hibit.test(SIZE-1) == true);
        CHECK(lobit.test(SIZE-1) == false);

        CHECK(hibit.test(0) == false);
        CHECK(lobit.test(0) == true);

        // out of bounds test should return false
        CHECK(full.test(-1) == false);
        CHECK(full.test(SIZE + 1) == false);
    }

    SUBCASE("toSetString") {
        CHECK(full.toSetString() == "{0.." + std::to_string(SIZE - 1) + "}");
        CHECK(empty.toSetString() == "{}");
        CHECK(sparse.toSetString() != "{}");
        CHECK(sparse.toSetString() != "{0.." + std::to_string(SIZE - 1) + "}");
        CHECK(full.toSetString(0, SIZE / 2) == "{0.." + std::to_string(SIZE/2 - 1) + "}");
        CHECK(hibit.toSetString() == "{" + std::to_string(SIZE - 1) + "}");
        CHECK(lobit.toSetString() == "{0}");
    }

    SUBCASE("Equals") {
        CHECK(full == full);
        CHECK(full != empty);
        CHECK(sparse != full);
        CHECK(sparse != empty);
        CHECK(sparse == sparse);

        // was once an infinite recursion bug
        empty = dynamic_cast<Bitset&>(sparse);
        CHECK(empty == sparse);
    }

    SUBCASE("LessThan") {
        CHECK(empty < full);
        CHECK(empty < sparse);

        CHECK(!(full < full));
        CHECK(!(empty < empty));
        CHECK(!(sparse < sparse));

        auto left  = empty.clone();
        auto right = empty.clone();

        CHECK(!(*left < *right));
        
        left->set(0);
        right->set(1);
        // {0} < {1}
        CHECK(*left < *right);

        right->set(0);
        // {0} < {0..1}
        CHECK(*left < *right);

        left->set(1);
        right->set(2);
        // {0..1} < {0..2}
        CHECK(*left < *right);

        auto left2 = empty.clone();
        left2->set(0);
        left2->set(2);
        // {0, 2} < {0..2}
        CHECK(*left < *right);

        left->clear();
        right->clear();
        left->set(1);
        right->set(0);
        // {1} not < {0}
        CHECK(!(*left < *right));
    }

    SUBCASE("And") {
        auto random_anded_full = bitset::fromAnd(sparse, full);
        auto random_anded_empty = bitset::fromAnd(sparse, empty);
        auto random_anded_random = bitset::fromAnd(sparse, sparse);
        auto random_anded_full_r = bitset::fromAnd(full, sparse);
        auto random_anded_empty_r = bitset::fromAnd(empty, sparse);

        CHECK(*random_anded_full == *random_anded_full_r);
        CHECK(*random_anded_empty == *random_anded_empty_r);

        CHECK(*random_anded_full == sparse);
        CHECK(*random_anded_empty == empty);
        CHECK(*random_anded_random == sparse);

        *random_anded_random &= full;
        CHECK(*random_anded_random == sparse);

        *random_anded_random &= empty;
        CHECK(*random_anded_random == empty);
        CHECK(random_anded_random->isEmpty());
    }

    SUBCASE("Or") {
        auto random_ord_full = bitset::fromOr(sparse, full);
        auto random_ord_empty = bitset::fromOr(sparse, empty);
        auto random_ord_random = bitset::fromOr(sparse, sparse);
        auto random_ord_full_r = bitset::fromOr(full, sparse);
        auto random_ord_empty_r = bitset::fromOr(empty, sparse);

        CHECK(*random_ord_full == full);
        CHECK(*random_ord_empty == sparse);
        CHECK(*random_ord_random == sparse);

        *random_ord_random |= empty;
        CHECK(*random_ord_random == sparse);
        CHECK(!random_ord_random->isEmpty());
        
        *random_ord_random |= full;
        CHECK(*random_ord_random == full);
    }

    SUBCASE("setRemoval") {
        // lrle bitsets don't support this and should throw
        try {
            full -= sparse;
            CHECK(!full.hasCommonBits(sparse));
        } catch (const std::runtime_error& e) {
            CHECK(e.what() == doctest::Contains("unsupported lrle bitset operation"));
        }
    }

    SUBCASE("invert") {
        // lrle bitsets don't support this and should throw
        try {
            auto full2 = full.clone();
            full2->invert();
            CHECK(*full2 == empty);
            full2->invert();
            CHECK(full2->isFull());
        } catch (const std::runtime_error& e) {
            CHECK(e.what() == doctest::Contains("unsupported lrle bitset operation"));
        }
    }

    SUBCASE("hasCommonBits") {
        CHECK(full.hasCommonBits(sparse));
        CHECK(!empty.hasCommonBits(sparse));
    }

    SUBCASE("setUnset") {
        auto random_clone = sparse.clone();
        CHECK(*random_clone == sparse);

        auto first = random_clone->firstSet();
        CHECK(first != Bitset::None);

        // lrle bitsets don't support this and should throw
        try {
            random_clone->unset(first);
            CHECK(*random_clone != sparse);
            random_clone->set(first);
            CHECK(*random_clone == sparse);
        } catch (const std::runtime_error& e) {
            CHECK(e.what() == doctest::Contains("unsupported lrle bitset operation"));
        }
    }

    SUBCASE("clear") {
        auto full2 = full.clone();
        auto empty2 = empty.clone();
        auto random2 = sparse.clone();
        full2->clear();
        empty2->clear();
        random2->clear();

        CHECK(full2->isEmpty());
        CHECK(full2->numSet() == 0);
        CHECK(empty2->isEmpty());
        CHECK(empty2->numSet() == 0);
        CHECK(random2->isEmpty());
        CHECK(random2->numSet() == 0);
    }

    SUBCASE("serialize") {
        auto full_s = full.serialize();
        auto empty_s = empty.serialize();
        auto sparse_s = sparse.serialize();

        // Call deserialization constructor
        auto full_ds = BitsetT(full_s.get());
        auto empty_ds = BitsetT(empty_s.get());
        auto sparse_ds = BitsetT(sparse_s.get());

        CHECK(full_ds == full);
        CHECK(empty_ds != full);
        CHECK(full_ds != empty);
        CHECK(sparse_ds == sparse);
    }

    SUBCASE("hash") {
        auto map = std::unordered_set<Bitset*, BitsetHasher>{};

        map.insert(&full);
        map.insert(&sparse);
        map.insert(&empty);

        CHECK(map.size() == 3);
    }

    SUBCASE("iteratorFull") {
        size_t count = 0;
        auto iterator_storage = full.iterator();

        // iterators should return Bitset::None when they're done
        for (auto& it = *iterator_storage; *it != Bitset::None && count < 999999; ++it) {
            count++;
        }

        CHECK(full.sizeInBits() == count);
    }

    SUBCASE("iteratorEmpty") {
        size_t count = 0;
        auto iterator_storage = empty.iterator();

        // iterators should return Bitset::None when they're done
        for (auto& it = *iterator_storage; *it != Bitset::None && count < 999999; ++it) {
            count++;
        }

        CHECK(count == 0u);
    }

    SUBCASE("iteratorSparse") {
        size_t count = 0;
        auto iterator_storage = sparse.iterator();

        // iterators should return Bitset::None when they're done
        for (auto& it = *iterator_storage; *it != Bitset::None && count < 999999; ++it) {
            count++;
        }

        CHECK(count > 0u);
        CHECK(count < sparse.sizeInBits());
    }

    SUBCASE("iteratorHi") {
        size_t count = 0;
        auto iterator_storage = hibit.iterator();

        // iterators should return Bitset::None when they're done
        for (auto& it = *iterator_storage; *it != Bitset::None && count < 999999; ++it) {
            CHECK(*it == hibit.sizeInBits() - 1);
            count++;
        }

        CHECK(count == 1u);
    }

    SUBCASE("iteratorLo") {
        size_t count = 0;
        auto iterator_storage = lobit.iterator();

        // iterators should return Bitset::None when they're done
        for (auto& it = *iterator_storage; *it != Bitset::None && count < 999999; ++it) {
            CHECK(*it == 0u);
            count++;
        }

        CHECK(count == 1u);
    }
}

