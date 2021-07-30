/******************************************************************************\
 * Copyright 2021 Hewlett Packard Enterprise Development LP.
 ******************************************************************************/
#pragma once

#include <mutex>
#include <queue>

#include "dbgclnt_network.h"

#include "zmqnet.hpp"
#include "proto_message.h"
#include "dense_bitset.h"

#include "dbg_zmq.capnp.h"
#include "capnp/serialize.h"

class ZMQNetNetwork : public DebugClientNetwork {
public:
    ZMQNetNetwork(std::unique_ptr<zmqnet::AsyncFrontend> &&frontEnd);

    std::unique_ptr<Bitset> sendCommand(const Bitset *target, const char *request);
    std::optional<ProtoMessageSet> receiveReply(const Bitset *target, Bitset &received);

    int checkEvents();
    ProtoMessageSet releaseEvents();
    int exit();
private:
    struct UnpackedMessage {
        std::unique_ptr<char, decltype(&std::free)> data;
        uint64_t hash;
        std::unique_ptr<DenseBitset> bitset;
    };

    class EventQueue {
        // Holds/distributes async responses from the event stream
    public:
        bool hasEvent();
        std::optional<zmqnet::ByteArray> getNext();
        void enqueue(zmqnet::ByteArray &&event);
    private:
        std::queue<zmqnet::ByteArray> m_queue;
        std::mutex m_queue_lock;
    };
    
    capnp::UnalignedFlatArrayMessageReader getReaderFromData(const zmqnet::ByteArray &data) const;
    UnpackedMessage unpackMessageReader(const ZmqnetMessagePackage::ZmqnetMessage::Reader &reader) const;

    std::unique_ptr<zmqnet::AsyncFrontend> m_network;

    std::unique_ptr<zmqnet::Stream> m_command_stream;
    std::mutex m_command_result_lock;
    zmqnet::ByteArray m_command_result;
    std::condition_variable m_command_result_cv;
    bool m_command_result_ready;

    std::unique_ptr<zmqnet::Stream> m_event_stream;
    EventQueue m_event_queue;

    // amount of time a commnode will wait for children before sending data up the tree in async streams
    const int m_stream_timeout_ms = 400; 
};