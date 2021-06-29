/******************************************************************************\
 * Copyright 2021 Hewlett Packard Enterprise Development LP.
 ******************************************************************************/
#pragma once

#include "bitset.h"

#include <memory>

namespace bitset {

enum class Format {
    Dense,
    Sparse,
    Smart,
    Lrle
};

// for convenience
using Serialized = Bitset::Serialized;
using SerializedPtr = Bitset::SerializedPtr;

// a serialized bitset must have its format encoded so this function can 
// properly construct it. the serialization format is:
//   "FORMAT;SIZE;[...]"
// FORMAT is a hexidecimal integer representation of the bitset::Format enum.
// SIZE is the size of the bitset in bits, also represented in hexadecimal.
//   hexadecimal numbers should not have a leading "0x", only be in base 16.
//   capital letters or lowercase letters in the hexadecimal numbers are both ok.
// [...] is the format-specific encoding of the rest of the bitset data.
// e.g: "0;20;..." is a Dense bitset of 32 bits.
std::unique_ptr<Bitset> deserialize(const SerializedPtr &bitset);
std::unique_ptr<Bitset> deserialize(const Serialized bitset);

// cast deserialized Bitset into T before returning. Returns a (smart) nullptr
// if the cast fails.
template <typename T>
std::unique_ptr<T> deserializeAs(const SerializedPtr &bitset);
template <typename T>
std::unique_ptr<T> deserializeAs(const Serialized bitset);

// a stringified bitset does not necessarily contain its format, therefore
// the format must already be known when calling this function.
std::unique_ptr<Bitset> fromString(const std::string &string, const Format &format);

namespace detail {

// helpers for functions that wrap the return value up in a unique_ptr
Bitset* deserializeRaw(const bitset::SerializedPtr &bitset);
Bitset* deserializeRaw(const bitset::Serialized bitset);

} // detail

} // bitset

template <typename T>
std::unique_ptr<T> bitset::deserializeAs(const SerializedPtr &ptr) {
	return bitset::deserializeAs<T>(ptr.get());
}

template <typename T>
std::unique_ptr<T> bitset::deserializeAs(const Serialized bitset) {
	auto raw = bitset::detail::deserializeRaw(bitset);
	auto ptr = dynamic_cast<T*>(raw);
	return std::unique_ptr<T>(ptr);
}
