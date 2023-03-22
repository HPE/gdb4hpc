/******************************************************************************\
 * Copyright 2021 Hewlett Packard Enterprise Development LP.
 ******************************************************************************/
#pragma once

#include <mutex>
#include <atomic>

#include "dbgclnt_network.h"

#include "zmqnet.hpp"
#include "dbg_zmq.h"
#include "dbgoutp.h"
#include "proto_message.h"
#include "dense_bitset.h"

#include "dbg_zmq.capnp.h"
#include "capnp/serialize.h"

namespace detail {

template <typename T>
class AsyncQueue {
    // basic thread safe queue
public:
    bool hasItem() const {
        std::scoped_lock<std::mutex> lock(m_queue_lock);
        return m_queue.size() > 0;
    }
    
    std::optional<T> getNext() {
        std::scoped_lock<std::mutex> lock(m_queue_lock);

        if (m_queue.size() > 0) {
            auto result = std::move(m_queue.front()); 
            m_queue.erase(m_queue.begin());
            return result; 
        } else {
            return {};
        }
    }

    std::vector<T> getAll() {
        std::scoped_lock<std::mutex> lock(m_queue_lock);

        std::vector<T> result;
        std::swap(result, m_queue);

        return result;
    }

    void enqueue(T &&item) {
        std::scoped_lock<std::mutex> lock(m_queue_lock);
        m_queue.push_back(std::move(item));
    }

private:
    std::vector<T> m_queue;
    mutable std::mutex m_queue_lock;
};

} // detail

class ZMQNetNetwork : public DebugClientNetwork {
public:
    using OutputPtr = std::unique_ptr<g_progoutp_t, decltype(&DbgFreeGenericProgOutp)>;
    ZMQNetNetwork(std::unique_ptr<zmqnet::AsyncFrontend> &&frontEnd, OutputPtr &&output, std::unique_ptr<Bitset> &&allRanks);

    std::unique_ptr<Bitset> sendCommand(const Bitset *target, const char *request);
    std::variant<ProtoMessageSet, ProtoNothing> receiveReply(const Bitset *target, std::chrono::seconds timeout, Bitset &received);

    int checkEvents();
    ProtoMessageSet releaseEvents();
    std::vector<std::string> releaseOutputLines();
    int syncExit();
    int forceExit();
private:
    using EventQueue = detail::AsyncQueue<ProtoMessage>;

    class StreamTable {
        // Interface to a hash table mapping bitsets to active streams. 
        //
        // We want the memory management of this table to be automatic,
        // but the hash table keys need to be (const) raw pointers, since that's 
        // what comes in from the layer above. To meet these requirements, we
        // store the actual bitsets as unique ptrs in a vector for automatic 
        // management, and use pointers to those as keys to the hash table.
    private:
        // types

        class CommandStream {
            // Interface to a zmqnet::Async stream which behaves outwardly as if it is synchronous. 
        public:
            CommandStream(zmqnet::AsyncFrontend &parentNetwork, zmqnet::Stream::Id streamId, zmqnet::Communicator communicator);
            CommandStream(zmqnet::AsyncFrontend &parentNetwork, ZMQNetStream streamId, zmqnet::Communicator communicator);

            void sendCommand(const char *command, size_t len);

            // Wait for a response to arrive. The wait can be cancelled with a SIGINT. If
            // the operation is cancelled, the function returns ProtoNothing::CancelledByUser.
            // If `timeout` it greater than 0, the wait will time out after `timeout`
            // seconds if no result was received by then. On timeout, returns ProtoNothing::TimedOut.
            std::variant<ProtoMessageSet, ProtoNothing> extractResult(std::chrono::seconds timeout);

            // Like extractResult, but the operation is not cancellable and will not time
            // out. Will wait forever for a result.
            ProtoMessageSet extractResultWait();

        private:
            std::variant<ProtoMessageSet, ProtoNothing> internalExtractResult(bool cancellable, std::chrono::seconds timeout);

            zmqnet::AsyncFrontend &m_parentNetwork;
            std::unique_ptr<zmqnet::Stream> m_stream;

            struct SharedResult {
                // memory shared between the main thread and the zmqnet stream thread

                // shared data should not be read/written without locking this mutex
                std::mutex mutex;
                std::condition_variable cv;

                // shared data
                std::atomic<size_t> results_received = 0;
                zmqnet::ByteArray data;
            };

            size_t m_commands_sent = 0;
            SharedResult m_shared_result;
        };

        class EventStream {
            // Creates new stream with given id and communicator and pushes any
            // responses into the passed EventQueue
        public:
            EventStream(zmqnet::AsyncFrontend &parentNetwork, zmqnet::Stream::Id streamId, zmqnet::Communicator communicator, EventQueue &parentQueue);
            EventStream(zmqnet::AsyncFrontend &parentNetwork, ZMQNetStream streamId, zmqnet::Communicator communicator, EventQueue &parentQueue);
        private:
            zmqnet::AsyncFrontend &m_parentNetwork;
            EventQueue &m_parentQueue;
            std::unique_ptr<zmqnet::Stream> m_stream;

            // amount of time a commnode will wait for children before sending
            // data up the tree
            const int m_stream_timeout_ms = 400; 
        };

    public:
        // interface

        StreamTable(zmqnet::AsyncFrontend &parentNetwork, std::unique_ptr<Bitset> &&allRanks);
        CommandStream& getCommandStream(const Bitset *target);
        bool hasAsyncEvent();
        void injectAsyncEvent(ProtoMessage&& expiring);
        ProtoMessageSet releaseAsyncEvents();

    private:
        zmqnet::AsyncFrontend &m_parentNetwork;
        std::vector<std::unique_ptr<Bitset>> m_bitset_pool;

        CommandStream m_internalCommandStream;
        std::unordered_map<const Bitset*, std::unique_ptr<CommandStream>, BitsetHasher, BitsetHasher::KeyEqual> 
            m_command_streams;

        std::unordered_map<const Bitset*, std::unique_ptr<EventStream>, BitsetHasher, BitsetHasher::KeyEqual>
            m_event_streams;

        EventQueue m_event_queue;
    };

    class OutputWatcher {
        // watches program output on a separate thread and deposits output text
        // into a thread safe queue
    public:
        OutputWatcher(OutputPtr &&output);
        // an OutputWatcher is not copyable/movable since once its watcher thread
        // starts, it's sending data to a specific memory address (m_output_queue)
        OutputWatcher(const OutputWatcher &other) = delete;
        OutputWatcher(OutputWatcher &&other) = delete;
        ~OutputWatcher();

        bool hasOutput() const;
        std::vector<std::string> releaseOutputLines();
    private:
        OutputPtr m_output;
        std::thread m_watcher_thread;
        detail::AsyncQueue<std::string> m_output_queue;
        std::atomic<bool> m_keep_going;
    };

    std::unique_ptr<zmqnet::AsyncFrontend> m_network;
    OutputWatcher m_output_watcher;
    StreamTable m_stream_table;
};
