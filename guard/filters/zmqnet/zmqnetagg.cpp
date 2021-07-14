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

extern "C" {

struct Message {
	LRunLEBitset bitset;
	const char * data;
};

zmqnet::ByteArray run(zmqnet::Stream::Id const& streamId, std::vector<zmqnet::ByteArray> const& inputs) {
	// any reader created from a flat array reader will have pointers into the
	// flat array reader, thus it must be kept alive until everything derived
	// from it is done using it.
	std::vector<capnp::UnalignedFlatArrayMessageReader> readers;

	// extract capnp::readers from inputs
	for (const auto &byteArray : inputs) {
		// interpret byte array as capnp::word array
		auto wordsPtr = reinterpret_cast<capnp::word const *>(byteArray.begin());
		assert(byteArray.size() % sizeof(capnp::word) == 0);
		auto wordsLen = (byteArray.size() / sizeof(capnp::word));
		auto wordsArr = kj::arrayPtr(wordsPtr, wordsLen);

		readers.emplace_back(wordsArr);
	}

	// data hash -> Message struct table
	std::unordered_map<uint64_t, Message> table;

	// build up table
	for (auto &&reader : readers) {
		const auto &messages = reader.getRoot<ZmqnetMessagePackage>().getMessages();
		for (const auto &message : messages) {
			auto hash = message.getHash();
			const auto &serializedBitset = message.getBitset().asChars().begin();

			if (table.count(hash)) {
				// entry with same data exists
				table.at(hash).bitset |= LRunLEBitset(serializedBitset);
			} else {
				// new entry
				table.insert({hash, {LRunLEBitset(serializedBitset), message.getData().cStr()}});
			}
		}
	}

	// put all messages into single outgoing message pack
	capnp::MallocMessageBuilder outgoingBuilder;
	auto outgoingMessagePack = outgoingBuilder.initRoot<ZmqnetMessagePackage>();
	auto outgoingMessageList = outgoingMessagePack.initMessages(table.size());
	
	size_t i{};
	for (const auto &[hash, messageStruct] : table) {
		auto message = outgoingMessageList[i];

		message.setData(messageStruct.data);
		message.setHash(hash);

		auto serializedBitset = messageStruct.bitset.serialize();
		auto bitsetSize = LRunLEBitset::extractSerializedSize(serializedBitset);
		auto bitset = message.initBitset(bitsetSize);
		memcpy(bitset.begin(), serializedBitset.get(), bitsetSize);

		i++;
	}

	auto flat = capnp::messageToFlatArray(outgoingBuilder);

	return flat.forceReleaseAsBytes();
}

} // extern C
