/******************************************************************************\
 * Copyright 2021 Hewlett Packard Enterprise Development LP.
 ******************************************************************************/
#pragma once

#include <mutex>
#include <queue>

#include "dbgclnt_network.h"

#include "zmqnet.hpp"
#include "dbg_zmq.h"
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

    class PsuedoSyncStream {
        // Interface to a zmqnet::Async stream which behaves outwardly as if it is synchronous. 
    public:
        PsuedoSyncStream(zmqnet::AsyncFrontend &parentNetwork, zmqnet::Stream::Id streamId, zmqnet::Communicator communicator);
        PsuedoSyncStream(zmqnet::AsyncFrontend &parentNetwork, ZMQNetStream streamId, zmqnet::Communicator communicator);

        void sendCommand(const std::string &command);
        zmqnet::ByteArray extractResult();

    private:
        zmqnet::AsyncFrontend &m_parentNetwork;
        std::unique_ptr<zmqnet::Stream> m_stream;
        std::mutex m_resultLock;
        zmqnet::ByteArray m_result;
        std::condition_variable m_resultCV;
        bool m_resultReady;
    };
    
    capnp::UnalignedFlatArrayMessageReader getReaderFromData(const zmqnet::ByteArray &data) const;
    UnpackedMessage unpackMessageReader(const ZmqnetMessagePackage::ZmqnetMessage::Reader &reader) const;

    std::unique_ptr<zmqnet::AsyncFrontend> m_network;
    PsuedoSyncStream m_broadcast_command_stream;

    std::unique_ptr<zmqnet::Stream> m_event_stream;
    EventQueue m_event_queue;

    // amount of time a commnode will wait for children before sending data up the tree in async streams
    const int m_stream_timeout_ms = 400; 
};