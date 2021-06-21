/*
 * Copyright 2021 Hewlett Packard Enterprise Development LP.
 */

#include <cassert>

#include <vector>
#include <string>
#include <iostream>

#include "zmqnet.hpp"

#include "dbg_zmq.capnp.h"
#include "capnp/message.h"
#include "capnp/serialize.h"

extern "C" {

// This function is unfinished. Right now, it does not actually do any
// aggregation, only copies and flattens the incoming list of messages. gdb4hpc 
// expects identical results to be merged, which does not happen right now.

zmqnet::ByteArray run(zmqnet::Stream::Id const& streamId, std::vector<zmqnet::ByteArray> const& inputs) {
	std::vector<ZmqnetMessagePackage::ZmqnetMessage::Reader> all_messages;

	std::cerr << "agg: unpacking" << std::endl;

	for (const auto &byte_array : inputs) {
		std::cerr << "agg: processing \"";
		for (const auto &byte : byte_array) {
			if (byte >= 0x20 && byte <= 0x7E) std::cerr << byte;
			else fprintf(stderr, "0x%02x ", (int)byte);
		}
		std::cerr << "\" size:" << byte_array.size() << std::endl;

		// interpret byte array as capnp::word array
		std::cerr << "agg: reinterpreting as words" << std::endl;
		auto words_ptr = reinterpret_cast<capnp::word const *>(byte_array.begin());
		assert(byte_array.size() % sizeof(capnp::word) == 0);
		auto words_length = (byte_array.size() / sizeof(capnp::word));
		auto word_array = kj::arrayPtr(words_ptr, words_length);

		std::cerr << "agg: creating reader" << std::endl;
		auto reader = capnp::UnalignedFlatArrayMessageReader(word_array);

		std::cerr << "agg: getting root" << std::endl;
		auto message_pack = reader.getRoot<ZmqnetMessagePackage>();

		std::cerr << "agg: getting messages" << std::endl;
		auto messages = message_pack.getMessages();

		std::cerr << "agg: pushing back" << std::endl;
		for (const auto &message : messages) {
			all_messages.push_back(message);
		}
	}

	std::cerr << "agg: got " << all_messages.size() << " messages." << std::endl;
	
	// TODO: sort and aggregate messages

	capnp::MallocMessageBuilder outgoing_builder;
	auto outgoing_message_pack = outgoing_builder.initRoot<ZmqnetMessagePackage>();
	auto outgoing_message_list = outgoing_message_pack.initMessages(all_messages.size());
	
	for (size_t i = 0; i < all_messages.size(); i++) {
		outgoing_message_list.setWithCaveats(i, all_messages[i]);
	}

	auto flat = capnp::messageToFlatArray(outgoing_builder);

	std::cerr << "agg: sending" << std::endl;

	return zmqnet::make_ByteArray(flat.asBytes().begin(), flat.asBytes().size());
}

}
