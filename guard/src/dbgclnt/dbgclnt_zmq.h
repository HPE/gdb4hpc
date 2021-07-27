/******************************************************************************\
 * Copyright 2021 Hewlett Packard Enterprise Development LP.
 ******************************************************************************/

#pragma once

#include <mutex>
#include <condition_variable>
#include <queue>

#include "dbgclnt_proto.h"
#include "dbginvoke.h"
#include "zmqnet.hpp"
#include "dbg_zmq.h"
#include "lrunle_bitset.h"

#include "dbg_zmq.capnp.h"
#include "capnp/serialize.h"

class ZMQNetClientProtocol : public DebugClientProtocol {
public:
    // ZMQnet does not support:
    //  * finish function
    //  * any session functions

    ZMQNetClientProtocol();

    const char* name() const override { return "zmqnet"; };

    int attach(struct dbginvoke *di, proto_session *session, 
        const char *app_ident, const char *arch, const char *backend, 
        int gpu_flag, int debug_flag) override;

    int launch(struct dbginvoke *di, proto_session *session, 
        const char *prog, const char * const *argv,
        const char *arch, const char * const *arch_args, const char *arch_input, 
        const char *backend, const char* gdb, char **env,
        const char *cwd, int gpu_flag, int debug_flag, int early_entry_flag, 
        bool isNonStop) override;
        
    int exit(struct dbginvoke *, int detach) override;

    std::unique_ptr<Bitset> sendCommand(struct dbginvoke *di, const Bitset *target, 
        const char *request) override;

    std::optional<ProtoMessageSet> receiveReply(struct dbginvoke *di, 
        const Bitset *target, Bitset &received) override;

    int init() override;
    
    int checkEvents(struct dbginvoke *di) override;
    void dispatchEvents(struct dbginvoke *di, int ev_fd) override;

    void destroyClient(struct dbginvoke *di) override;
private:
    struct UnpackedMessage {
        std::unique_ptr<char, decltype(&std::free)> data;
        uint64_t hash;
        std::unique_ptr<LRunLEBitset> bitset;
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
};

// called once by registerProtos
void ZMQClientProtocol_init() {
    // will register itself in constructor
    static ZMQNetClientProtocol proto;
};
