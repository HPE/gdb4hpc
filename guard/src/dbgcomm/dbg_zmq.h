/******************************************************************************\
 * Copyright 2021 Hewlett Packard Enterprise Development LP.
 ******************************************************************************/
#pragma once

#include <mutex>
#include <condition_variable>
#include <queue>

#include "zmqnet.hpp"
#include "capnp/message.h"
#include "capnp/serialize.h"

enum class ZMQNetStream : zmqnet::Stream::Id {
    BroadcastCommandStream,
    BroadcastEventStream,
    InternalCommandStream,
    FirstArbitraryStreamId
};

inline capnp::UnalignedFlatArrayMessageReader getReaderFromData(const zmqnet::ByteArray &data) {
    assert(data.size() != 0);
    assert(data.size() % sizeof(capnp::word) == 0);
    auto words_ptr = reinterpret_cast<capnp::word const *>(data.begin());
    auto words_size = data.size() / sizeof(capnp::word);
    auto words_array = kj::arrayPtr(words_ptr, words_size);

    return capnp::UnalignedFlatArrayMessageReader(words_array);
}