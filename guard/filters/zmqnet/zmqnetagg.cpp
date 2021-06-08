/*
 * Copyright 2021 Hewlett Packard Enterprise Development LP.
 */

#include <cassert>

#include <vector>
#include <string>

#include "zmqnet.hpp"

// TODO change this to something the dbgclnt layer expects

extern "C" {

zmqnet::ByteArray run(zmqnet::Stream::Id const& streamId, std::vector<zmqnet::ByteArray> const& inputs)
{
	auto result = std::string{"{ "};
	for (auto&& input : inputs) {
		auto const inputStr = std::string{(char const*)input.begin()};
		assert(inputStr != "ready");
		result += inputStr + " ";
	}
	result += "}";

	return zmqnet::make_ByteArray(result.c_str(), result.length() + 1);
}

}
