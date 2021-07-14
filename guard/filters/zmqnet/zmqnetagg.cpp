/*
 * Copyright 2021 Hewlett Packard Enterprise Development LP.
 */

#include <cassert>

#include <vector>
#include <string>
#include <iostream>
#include <unordered_map>

#include "zmqnet.hpp"

#include "lrunle_bitset.h"

#include "dbg_zmq.capnp.h"
#include "capnp/message.h"
#include "capnp/serialize.h"

extern "C" {

// This function is unfinished. Right now, it does not actually do any
// aggregation, only copies and flattens the incoming list of messages. gdb4hpc 
// expects identical results to be merged, which does not happen right now.

struct Message {
	LRunLEBitset bitset;
	const char * data;
};

zmqnet::ByteArray run(zmqnet::Stream::Id const& streamId, std::vector<zmqnet::ByteArray> const& inputs) {
	// any reader created from a flat array reader will have pointers into the
	// flat array reader, thus it must be kept alive until everything derived
	// from it is done using it.
	std::vector<capnp::UnalignedFlatArrayMessageReader> readers;

	std::cerr << "agg: unpacking readers" << std::endl;

	// extract capnp::readers from inputs
	for (const auto &byte_array : inputs) {
		std::cerr << "agg: processing \"";
		for (const auto &byte : byte_array) {
			if (byte >= 0x20 && byte <= 0x7E)
				std::cerr << byte;
			else
				fprintf(stderr, "0x%02x ", (int)byte);
		}
		std::cerr << "\" size:" << byte_array.size() << std::endl;

		// interpret byte array as capnp::word array
		std::cerr << "agg: reinterpreting as words" << std::endl;
		auto words_ptr = reinterpret_cast<capnp::word const *>(byte_array.begin());
		assert(byte_array.size() % sizeof(capnp::word) == 0);
		auto words_length = (byte_array.size() / sizeof(capnp::word));
		auto word_array = kj::arrayPtr(words_ptr, words_length);

		std::cerr << "agg: creating reader/root" << std::endl;
		readers.emplace_back(word_array);
	}

	std::cerr << "agg: got " << readers.size() << " readers." << std::endl;
	
	std::unordered_map<uint64_t, Message> table;
	for (auto &&reader : readers) {
		const auto &messages = reader.getRoot<ZmqnetMessagePackage>().getMessages();
		for (const auto &message : messages) {
			auto hash = message.getHash();
			const auto &serializedBitset = message.getBitset().asChars().begin();
			
			{
				std::cerr << "agg: merging message: " << std::endl;
				std::cerr << "\tdata: " << message.getData().cStr() << std::endl;
				std::cerr << "\thash: " << message.getHash() << std::endl;
				auto bitsetPtr = message.getBitset().asChars().begin();
				auto bitset = LRunLEBitset(bitsetPtr);
				std::cerr << "\tbset: " << bitset.toSet().c_str() << std::endl;
			}

			if (table.count(hash)) {
				// entry with same data exists
				std::cerr << "agg: entry already exists" << std::endl;
				std::cerr << "\t" << table.at(hash).bitset.toSet() << " <- " << LRunLEBitset(serializedBitset).toSet() << std::endl;
				table.at(hash).bitset |= LRunLEBitset(serializedBitset);
			} else {
				// new entry
				std::cerr << "agg: new entry" << std::endl;
				table.insert({hash, {LRunLEBitset(serializedBitset), message.getData().cStr()}});
			}
		}
	}

	std::cerr << "agg: got " << table.size() << " messages after merging." << std::endl;

	// put all messages into single outgoing message pack
	capnp::MallocMessageBuilder outgoing_builder;
	auto outgoing_message_pack = outgoing_builder.initRoot<ZmqnetMessagePackage>();
	auto outgoing_message_list = outgoing_message_pack.initMessages(table.size());
	
	size_t i{};
	for (const auto &[hash, messageStruct] : table) {
		auto message = outgoing_message_list[i];

		message.setData(messageStruct.data);
		message.setHash(hash);

		auto serializedBitset = messageStruct.bitset.serialize();
		auto bitsetSize = LRunLEBitset::extractSerializedSize(serializedBitset);
		auto bitset = message.initBitset(bitsetSize);
		memcpy(bitset.begin(), serializedBitset.get(), bitsetSize);

		i++;
	}

	std::cerr << "agg: flattening" << std::endl;
	auto flat = capnp::messageToFlatArray(outgoing_builder);

	std::cerr << "agg: sending" << std::endl;
	return zmqnet::make_ByteArray(flat.asBytes().begin(), flat.asBytes().size());
}

} // extern C
