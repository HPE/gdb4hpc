/******************************************************************************\
 * Copyright 2021 Hewlett Packard Enterprise Development LP.
 ******************************************************************************/
#include "bitset_serialize.h"
#include "bitset.h"
#include "dense_bitset.h"
#include "sparse_bitset.h"
#include "smart_bitset.h"

#include <string.h>

#include <optional>

namespace bitset {

static std::unique_ptr<DenseBitset> deserializeDense(Bitset::Serialized bitset);
static std::unique_ptr<DenseBitset> fromStringDense(const std::string &str);

static std::unique_ptr<SparseBitset> deserializeSparse(int nbits, int strlen, Bitset::Serialized bitset);

static std::unique_ptr<SmartBitset> deserializeSmart(int nbits, int strlen, Bitset::Serialized bitset);

static int digittoint(int c);
// convert a hex string to an integer
// returns nullopt on error
static std::optional<int> atoi_hex(const char* str, int start, int end);
// returns (low, high), nullopt on error
static std::optional<std::tuple<int, int>> parse_range(char* set, int start, int end);

int digittoint(int c)
{
	if (c >= '0' && c <= '9')
		return c - '0';
		
	if (c >= 'a' && c <= 'f')
		return c - 'a' + 10;
		
	if (c >= 'A' && c <= 'F')
		return c - 'A' + 10;
	
	return 0;
}

std::optional<int> atoi_hex(const char* str, int start, int end)
{
	int res = 0, index;
	if (str == NULL || start < 0 || end < start || start >= strlen(str))
		return {}; 

	for (index = start; index <= end && isxdigit(str[index]); index++) {
		res <<= 4;
		res += digittoint(str[index]);
	}

	return res;
}

std::optional<std::tuple<int, int>> parse_range(char* set, int start, int end) {
	int index, res;

	if (set == nullptr || start < 0 || end < start) {
		return {};
	}

	for (index = start; index <= end && (set[index] != '-' && set[index] != '.'); index++);
	auto optLower = atoi_hex(set, start, index - 1);
	if(!optLower)
		return {};

	std::optional<int> optUpper = {};
	if (end > index && (set[index] == '-' || set[index] == '.')) {
		optUpper = atoi_hex(set, index + 1, end);
		if(!optUpper)
			return {};
	} else
		*optUpper = *optLower;
	
	return std::tuple<int, int>{*optLower, *optUpper};
}

std::unique_ptr<Bitset> deserialize(const SerializedPtr &ptr) {
    return deserialize(ptr.get());
}

std::unique_ptr<Bitset> deserialize(const Serialized str) {
	//get format
	int start = 0, index;
	for (index = start; str[index] != ';' && str[index] != '\0'; index++);

	auto optFormat = atoi_hex(str, start, index - 1);
	if(!optFormat)
		return nullptr;
	
	//get size
	start = index + 1;
	for (index = start; str[index] != ';' && str[index] != '\0'; index++);

	auto optSize = atoi_hex(str, start, index - 1);
	if(!optSize || *optSize < 0)
		return nullptr;

	int str_len = strlen(str);
	switch (static_cast<Format>(*optFormat))
	{
		case Format::Dense:
			return deserializeDense(&str[index+1]);
			break;
		case Format::Sparse:
			return deserializeSparse(*optSize, str_len-index-1, &str[index+1]);
			break;
		case Format::Smart:
			return deserializeSmart(*optSize, str_len-index-1, &str[index+1]);
			break;
		default:
            return nullptr;
	}
}

static std::unique_ptr<DenseBitset> deserializeDense(Bitset::Serialized str) {
	if (str == nullptr) {
		return nullptr;
	}

    int nbits;
	for (nbits = 0; *str != ':' && *str != '\0' && isxdigit(*str); str++) {
		nbits <<= 4;
		nbits += digittoint(*str);
	}

	int bytes = NUM_BYTES(nbits);

	if (*str++ != ':' || nbits == 0) {
		return nullptr;
	}

	auto result = std::make_unique<DenseBitset>(nbits);

	for (int pos = (bytes << 3) - 1; *str != '\0' && isxdigit(*str) && pos >= 0; str++) {
		int b = digittoint(*str);
		for (int n = 3; n >= 0; n--, pos--) {
			if (b & (1 << n)) {
				result->set(pos);
			}
		}
	}

	return result;
}

static std::unique_ptr<SparseBitset> deserializeSparse(int nbits, int strlen, Bitset::Serialized str) {
	auto result = std::make_unique<SparseBitset>(nbits);

	if (str == nullptr || str[0] != '{' || str[strlen-1] != '}' || strlen < 2) 
		return nullptr;

	if (strncmp(str, "{}", 2) == 0) {
		return result;
	}

	int start = 1;
	int end;
	for(int i = 1; i < strlen-1; i++) {
		if(str[i] == ',') {
			end = i - 1;

			if(end >= start) {
				auto r = parse_range(str, start, end);
				if (!r) return nullptr;
				auto [lower, upper] = *r;
				if (lower <= upper)
					result->setRange(lower, upper);
			}
			start = i + 1;
		}
	}	

	auto r = parse_range(str, start, strlen-2);
	if (!r) return nullptr;
	auto [lower, upper] = *r;
	if(lower <= upper)
		result->setRange(lower, upper);

	return result;
}

static std::unique_ptr<SmartBitset> deserializeSmart(int nbits, int strlen, Bitset::Serialized bitset) {
	// smart bitset is either a dense bitset or a sparse bitest. sparse bitsets
	// always begin with '{'
	if (bitset[0] == '{') {
		auto sparse = deserializeSparse(nbits, strlen, bitset);
		return std::make_unique<SmartBitset>(std::move(sparse));
	} else {
		// dense bitsets have their size encoded
		auto dense = deserializeDense(bitset);
		return std::make_unique<SmartBitset>(std::move(dense));
	}
}

std::unique_ptr<Bitset> fromString(const std::string &str, const Format &fmt) {
	switch (fmt) {
	case Format::Dense:
		return fromStringDense(str);
		break;
	
	default:
		return nullptr;
		break;
	}
}

std::unique_ptr<DenseBitset> fromStringDense(const std::string &cppstring) {
	const char *str = cppstring.c_str();

	int nbits;
	for (nbits = 0; *str != ':' && *str != '\0' && isxdigit(*str); str++) {
		nbits <<= 4;
		nbits += digittoint(*str);
	}

	int bytes = NUM_BYTES(nbits);

	if (*str++ != ':' || nbits == 0) {
		return NULL;
	}

	auto result = std::make_unique<DenseBitset>(nbits);

	for (int pos = (bytes << 3) - 1; *str != '\0' && isxdigit(*str) && pos >= 0; str++) {
		int b = digittoint(*str);
		for (int n = 3; n >= 0; n--, pos--) {
			if (b & (1 << n)) {
				result->set(pos);
			}
		}
	}

	return result;
}

} // bitset
