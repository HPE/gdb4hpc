/******************************************************************************\
 * Copyright 2022 Hewlett Packard Enterprise Development LP.
 ******************************************************************************/

#include "bitset.h"
#include "dense_bitset.h"

#include <unordered_set>

#include "unittest_macros.h"

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

static std::unique_ptr<Bitset> get_small_bitset(unsigned short bits) {
    auto result = std::make_unique<DenseBitset>(4);
    for (size_t b = 0; bits != 0; b++, bits >>= 1) {
        if ((bits & 1) == 1) result->set(b);
    }
    return result;
}

TEST_CASE("BitsetMRUCache") {
    SUBCASE("Empty history iteration") {
        auto h = BitsetMRUCache<size_t>(*std::make_unique<DenseBitset>(4, DenseBitset::InitialState::Full));
        auto mask = get_small_bitset(0b0000);
        bool did_stuff_inside = false;
        for ([[maybe_unused]] const auto& item : h) { did_stuff_inside = true; };
        for ([[maybe_unused]] const auto& item : h.masked(*mask)) { did_stuff_inside = true; };
        CHECK(did_stuff_inside == false);
    }

    SUBCASE("basics") {
        auto h = BitsetMRUCache<size_t>(*std::make_unique<DenseBitset>(4, DenseBitset::InitialState::Full));
        // push items, then iterate over them with most recent item first
        auto entries = std::vector<std::pair<std::unique_ptr<Bitset>, size_t>>{};
        entries.push_back({get_small_bitset(0b0001), 0});
        entries.push_back({get_small_bitset(0b0010), 1});
        entries.push_back({get_small_bitset(0b0100), 2});

        h.push(entries[0].first->clone(), size_t{entries[0].second});
        h.push(entries[1].first->clone(), size_t{entries[1].second});
        h.push(entries[2].first->clone(), size_t{entries[2].second});

        auto i = (int)entries.size() - 1;
        for (const auto& item : h) {
            CHECK(entries[i--].second == item);
        }
        CHECK(i == -1);
    }

    SUBCASE("exactly full list") {
        auto h = BitsetMRUCache<size_t>(*std::make_unique<DenseBitset>(4, DenseBitset::InitialState::Full));
        // push items to completely fill the internal list without wrapping
        auto entries = std::vector<std::pair<std::unique_ptr<Bitset>, size_t>>{};
        entries.push_back({get_small_bitset(0b0001), 0});
        entries.push_back({get_small_bitset(0b0010), 1});
        entries.push_back({get_small_bitset(0b0100), 2});
        entries.push_back({get_small_bitset(0b1000), 3});

        h.push(entries[0].first->clone(), size_t{entries[0].second});
        h.push(entries[1].first->clone(), size_t{entries[1].second});
        h.push(entries[2].first->clone(), size_t{entries[2].second});
        h.push(entries[3].first->clone(), size_t{entries[3].second});

        auto i = (int)entries.size() - 1;
        for (const auto& item : h) {
            CHECK(entries[i--].second == item);
        }
        CHECK(i == -1);
    }

    SUBCASE("new items can overwrite old items even if their bitsets don't overlap") {
        auto h = BitsetMRUCache<size_t>(*std::make_unique<DenseBitset>(4, DenseBitset::InitialState::Full));

        auto entries = std::vector<std::pair<std::unique_ptr<Bitset>, size_t>>{};
        entries.push_back({get_small_bitset(0b0001), 0});
        entries.push_back({get_small_bitset(0b0010), 1});
        entries.push_back({get_small_bitset(0b0100), 2});
        entries.push_back({get_small_bitset(0b1000), 3});

        // first entry
        h.push(entries[0].first->clone(), size_t{entries[0].second});

        for (int i = 0; i < 10000; i++) {
            // note that these entries' bitsets don't overlap the first entry...
            h.push(entries[1].first->clone(), size_t{entries[1].second});
            h.push(entries[2].first->clone(), size_t{entries[2].second});
            h.push(entries[3].first->clone(), size_t{entries[3].second});
        }

        // ...but the first entry has been lost due to the number of new entries pushed.
        // the cache makes no attempt to keep old data around, even if it doesn't overlap
        // new data.
        auto i = (int)entries.size() - 1;
        for (const auto& item : h) {
            CHECK(entries[i--].second == item);
        }
        CHECK(i == 0); // only 3 items present
    }

    SUBCASE("Only the most recent item of a given bitset is shown") {
        auto h = BitsetMRUCache<size_t>(*std::make_unique<DenseBitset>(4, DenseBitset::InitialState::Full));
        // push two elements with the same bitset - the new one overwrites the old one
        // when iterating
        auto entries = std::vector<std::pair<std::unique_ptr<Bitset>, size_t>>{};
        entries.push_back({get_small_bitset(0b0001), 0});
        entries.push_back({get_small_bitset(0b0001), 1});

        h.push(entries[0].first->clone(), size_t{entries[0].second});
        h.push(entries[1].first->clone(), size_t{entries[1].second});

        auto i = (int)entries.size() - 1;
        for (const auto& item : h) {
            CHECK(entries[i--].second == item);
        }
        CHECK(i == 0); // only one item present
    }

    SUBCASE("Items with only one bit will still appear") {
        auto h = BitsetMRUCache<size_t>(*std::make_unique<DenseBitset>(4, DenseBitset::InitialState::Full));
        // push two elements with the same bitset - the new one only partially overwrites the old one
        // when iterating and both will appear
        auto entries = std::vector<std::pair<std::unique_ptr<Bitset>, size_t>>{};
        entries.push_back({get_small_bitset(0b0111), -1});
        entries.push_back({get_small_bitset(0b0011), 0});
        entries.push_back({get_small_bitset(0b0110), 1});

        h.push(entries[0].first->clone(), size_t{entries[0].second});
        h.push(entries[1].first->clone(), size_t{entries[1].second});
        h.push(entries[2].first->clone(), size_t{entries[2].second});

        auto i = (int)entries.size() - 1;
        for (const auto& item : h) {
            CHECK(entries[i--].second == item);
        }
        CHECK(i == 0); // only two items present
    }

    SUBCASE("Large bitsets will be split by newer, smaller bitsets") {
        auto h = BitsetMRUCache<size_t>(*std::make_unique<DenseBitset>(4, DenseBitset::InitialState::Full));
        auto entries = std::vector<std::pair<std::unique_ptr<Bitset>, size_t>>{};
        entries.push_back({get_small_bitset(0b1111), 0});
        entries.push_back({get_small_bitset(0b0001), 1});
        entries.push_back({get_small_bitset(0b0010), 2});
        entries.push_back({get_small_bitset(0b0100), 3});

        h.push(entries[0].first->clone(), size_t{entries[0].second});
        h.push(entries[1].first->clone(), size_t{entries[1].second});
        h.push(entries[2].first->clone(), size_t{entries[2].second});
        h.push(entries[3].first->clone(), size_t{entries[3].second});

        auto i = (int)entries.size() - 1;
        for (const auto& item : h) {
            CHECK(entries[i--].second == item);
        }
        CHECK(i == -1);
    }

    SUBCASE("Masked iteration") {
        auto h = BitsetMRUCache<size_t>(*std::make_unique<DenseBitset>(4, DenseBitset::InitialState::Full));
        auto entries = std::vector<std::pair<std::unique_ptr<Bitset>, size_t>>{};
        entries.push_back({get_small_bitset(0b1000), 0});
        entries.push_back({get_small_bitset(0b0100), 1});
        entries.push_back({get_small_bitset(0b0010), 2});
        entries.push_back({get_small_bitset(0b0001), 3});

        h.push(entries[0].first->clone(), size_t{entries[0].second});
        h.push(entries[1].first->clone(), size_t{entries[1].second});
        h.push(entries[2].first->clone(), size_t{entries[2].second});
        h.push(entries[3].first->clone(), size_t{entries[3].second});

        auto mask = get_small_bitset(0b0110);

        auto i = 2; // starting from middle of list since most recent item is masked away
        for (const auto& item : h.masked(*mask)) {
            CHECK(entries[i--].second == item);
        }
        CHECK(i == 0); // only two items present
    }

    SUBCASE("Masked iteration that doesn't match anything") {
        auto h = BitsetMRUCache<size_t>(*std::make_unique<DenseBitset>(4, DenseBitset::InitialState::Full));
        auto entries = std::vector<std::pair<std::unique_ptr<Bitset>, size_t>>{};
        entries.push_back({get_small_bitset(0b1000), 0});
        entries.push_back({get_small_bitset(0b0100), 1});
        entries.push_back({get_small_bitset(0b0010), 2});

        h.push(entries[0].first->clone(), size_t{entries[0].second});
        h.push(entries[1].first->clone(), size_t{entries[1].second});
        h.push(entries[2].first->clone(), size_t{entries[2].second});

        SUBCASE("Empty mask") {
            auto mask = get_small_bitset(0b0000);
            bool did_inner_stuff = false;
            for ([[maybe_unused]] const auto& item : h.masked(*mask)) {
                did_inner_stuff = true;
            }
            CHECK(did_inner_stuff == false);
        }

        SUBCASE("Non empty mismatching mask") {
            auto mask = get_small_bitset(0b0001);
            bool did_inner_stuff = false;
            for ([[maybe_unused]] const auto& item : h.masked(*mask)) {
                did_inner_stuff = true;
            }
            CHECK(did_inner_stuff == false);
        }
    }

    SUBCASE("Pushing the same bitset multiple times replaces the top item") {
        // as an optimization, pushing the same bitset multiple times to the cache
        // will simply overwrite the entry at the tip instead of pushing new items.
        //
        // this effect isn't actually observable from the API that the cache gives,
        // so this test only exists to test the internal code path and ensure the
        // observable effects are still correct.

        auto h = BitsetMRUCache<size_t>(*std::make_unique<DenseBitset>(4, DenseBitset::InitialState::Full));
        auto entries = std::vector<std::pair<std::unique_ptr<Bitset>, size_t>>{};
        entries.push_back({get_small_bitset(0b1000), 0});
        entries.push_back({get_small_bitset(0b1000), 1});
        entries.push_back({get_small_bitset(0b1000), 2});

        h.push(entries[0].first->clone(), size_t{entries[0].second});
        h.push(entries[1].first->clone(), size_t{entries[1].second});
        h.push(entries[2].first->clone(), size_t{entries[2].second});

        auto i = 0;
        for (const auto& item : h) {
            i++;
            CHECK(item == 2); // only expect to CHECK once
        }
        CHECK(i == 1); // only one item present
    }

    SUBCASE("Huge bitsets and cache") {
        // not really a correctness test - just making sure a big cache containing
        // big bitsets is indeed reasonably fast.
        //
        // "reasonably" currently only defined by a human running the unit tests
        // manually and gut feeling it. TODO: more strict tests for performance

        constexpr auto bit_width = 100000ul;
        constexpr auto num_pushes = BitsetMRUCache<size_t>::max_capacity;
        static_assert(num_pushes < bit_width);

        auto h = BitsetMRUCache<size_t>(*std::make_unique<DenseBitset>(bit_width, DenseBitset::InitialState::Full));

        for (size_t i = 0; i < num_pushes; i++) {
            auto bitset = std::make_unique<DenseBitset>(bit_width);
            bitset->set(i);
            h.push(std::move(bitset), size_t{i});
        }

        size_t count = 0;
        for ([[maybe_unused]] auto& item : h) {
            count++;
        }
        CHECK(count == num_pushes);

        count = 0;
        auto mask = std::make_unique<DenseBitset>(bit_width);
        for (size_t i = 0; i < num_pushes/2; i++) {
            mask->set(i);
        }
        for ([[maybe_unused]] auto& item: h.masked(*mask)) {
            count++;
        }
        CHECK(count == num_pushes/2);
    }
}
