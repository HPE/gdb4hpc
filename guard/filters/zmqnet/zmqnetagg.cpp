/*
 * Copyright 2021 Hewlett Packard Enterprise Development LP.
 */

#include <cassert>
#include <vector>
#include <string>
#include <unordered_map>

#include "zmqnet.hpp"
#include "lrunle_bitset.h"

#include "dbg_zmq.capnp.h"
#include "capnp/message.h"
#include "capnp/serialize.h"

#if DEBUG
#define LOG(...) fprintf(stderr, __VA_ARGS__)
#else
#define LOG(...) /**/
#endif

extern "C" {

struct HashKey {
	const char * data;
	size_t dataLen;
	uint64_t hash;

	struct Hash {
		size_t operator() (const HashKey &key) const {
			return key.hash;
		}
	};

	struct Compare {
		bool operator() (const HashKey &a, const HashKey &b) const {
			if (a.hash != b.hash) return false;
			if (a.dataLen != b.dataLen) return false;
			return strncmp(a.data, b.data, a.dataLen) == 0;
		}
	};
};

zmqnet::ByteArray run(std::vector<zmqnet::ByteArray> const& inputs) {
	// any reader created from a flat array reader will have pointers into the
	// flat array reader, thus it must be kept alive until everything derived
	// from it is done using it.
	std::vector<capnp::UnalignedFlatArrayMessageReader> readers;

	LOG("agg: got %lu inputs.\n", inputs.size());

	// extract capnp::readers from inputs
	for (const auto &byteArray : inputs) {
		LOG("agg: processing chunk of data of size %lu.\n", byteArray.size());
		
		// interpret byte array as capnp::word array
		auto wordsPtr = reinterpret_cast<capnp::word const *>(byteArray.begin());
		assert(byteArray.size() % sizeof(capnp::word) == 0);
		auto wordsLen = (byteArray.size() / sizeof(capnp::word));
		auto wordsArr = kj::arrayPtr(wordsPtr, wordsLen);

		readers.emplace_back(wordsArr);
	}

	// data hash -> Message struct table
	std::unordered_map<HashKey, LRunLEBitset, HashKey::Hash, HashKey::Compare> table;

	// build up table
	for (auto &&reader : readers) {
		const auto &messages = reader.getRoot<ZmqnetMessagePackage>().getMessages();
		for (const auto &message : messages) {
			HashKey key = {message.getData().cStr(), message.getData().size(), message.getHash()};
			const auto &serializedBitset = message.getBitset().asChars().begin();

			auto loc = table.find(key);
			if (loc != table.end()) {
				// entry with same data exists
				loc->second |= LRunLEBitset(serializedBitset);
			} else {
				// new entry
				table.emplace(key, LRunLEBitset(serializedBitset));
			}
		}
	}

	// put all messages into single outgoing message pack
	capnp::MallocMessageBuilder outgoingBuilder;
	auto outgoingMessagePack = outgoingBuilder.initRoot<ZmqnetMessagePackage>();
	auto outgoingMessageList = outgoingMessagePack.initMessages(table.size());
	
	size_t i{};
	for (const auto &[hashKey, bitset] : table) {
		auto message = outgoingMessageList[i];

		message.setData(hashKey.data);
		message.setHash(hashKey.hash);

		auto serializedBitset = bitset.serialize();
		auto bitsetSize = LRunLEBitset::extractSerializedSize(serializedBitset);
		auto outgoingBitset = message.initBitset(bitsetSize);
		memcpy(outgoingBitset.begin(), serializedBitset.get(), bitsetSize);

		i++;
	}

	auto flat = capnp::messageToFlatArray(outgoingBuilder);

	return flat.forceReleaseAsBytes();
}

} // extern C
