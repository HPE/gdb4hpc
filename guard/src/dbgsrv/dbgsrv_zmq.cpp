/******************************************************************************\
 * Copyright 2021 Hewlett Packard Enterprise Development LP.
 ******************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>

#include "zmqnet.hpp"

#include "dbgsrv_zmq.h"
#include "dbgsrv_defs.h"
#include "dbgsrv_cmds.h"
#include "dbgformat.h"

#include "dbg_zmq.capnp.h"

#include "capnp/message.h"
#include "capnp/serialize.h"

#include "common_tools_be.h"

#define LOG(...) (debug_flag ? _dvprint(__VA_ARGS__) : (void)0)

// Proto instantiation
ZMQNetServerProtocol DbgSrvProtoZMQNet;

ZMQNetServerProtocol::ZMQNetServerProtocol()
    : m_network{nullptr} 
    , m_pending_command{nullptr}
    , m_rank_bitset{nullptr}
    , m_rank_serialized{nullptr}
    , m_rank_serialized_size{0}
{ 
    registerProtocol();
};

const char *ZMQNetServerProtocol::name() {
    return "zmqnet";
}

// Callbacks
int ZMQNetServerProtocol::invokeCallback(const char *chost, int cport, 
    const char *host, int prog, int pnum, int port, dbgevent_t *e) {
    // we have nothing to do here
    LOG("ZMQNetServerProtocol::invokeCallback\n");
    return 1;
}

int ZMQNetServerProtocol::eventCallback(dbgevent_t *ev) {
    LOG("ZMQNetServerProtocol::eventCallback\n");

    auto eventResult = ResultPtr(EventToSocProto(ev), std::free);
    auto id = static_cast<zmqnet::Stream::Id>(ZMQNetStream::EventStream);

    m_network->sendResult(id, packResult(std::move(eventResult)));

    return 0;
}

int ZMQNetServerProtocol::initProto(int &procnum, int &pid, int argc, const char * const * argv,
                              const char *key, const char *host, int port) {
    LOG("ZMQNetServerProtocol::initProto\n");

    LOG("argc: %d\n", argc);
    LOG("argv: ");
    // print out the argv
    for (auto tmp = argv; *tmp != nullptr; tmp++) {
        fprintf(stderr, "%s ", *tmp);
    }
    fprintf(stderr, "\n"); 

    auto const [upstreamControlEndpoint, upstreamDataEndpoint, node_id,
		rest_argc, rest_argv] = zmqnet::AsyncBackend::getParams(argc, argv);

    // set rank for layer above
    procnum = node_id;

    {
        // figure out our pid
        auto pids = std::unique_ptr<cti_pidList_t, decltype(&cti_be_destroyPidList)>
            (cti_be_findAppPids(), cti_be_destroyPidList);
        assert(pids != nullptr);
        auto first = cti_be_getNodeFirstPE();
        assert(first >= 0);
        auto offset = node_id - first;

        LOG("zmqnet: pid offset is %d from %d. %d ranks reside on this node.\n", offset, first, pids->numPids);

        if (offset >= pids->numPids) {
            LOG("zmqnet: ERROR, offset (%d) >= numPids (%d).\n", offset, pids->numPids);
            return -1;
        }

        // set pid for layer above
        pid = pids->pids[offset].pid;

        // double check that our assumption that node_id == rank is correct
        if (pids->pids[offset].rank != node_id) {
            LOG("zmqnet: ERROR: rank != node_id. (%d != %d)\n", pids->pids[offset].rank, node_id);
            return -1;
        }
    }

    LOG("zmqnet: set rank and pid: %d and %d.\n", procnum, pid);

    m_network = std::make_unique<zmqnet::AsyncBackend>(upstreamControlEndpoint, upstreamDataEndpoint, node_id);

    { // register command stream callback
        const auto commandStreamId = static_cast<zmqnet::Stream::Id>(ZMQNetStream::CommandStream);
        const auto commandCallback = [&](zmqnet::Stream::Id const& streamId, zmqnet::ByteArray command) {
            LOG("zmqnet: command callback %d got command \"%s\" from stream %d\n", node_id, command.begin(), streamId);
            assert(streamId == commandStreamId);
            
            /*    Two locks are needed here to handle the (unlikely) edge case 
            * that two zmqnet threads try to run a command at the same time.
            *     m_command_lock, used in the main thread call to runProto, is 
            * used to make sure the main thread doesn't destroy the command 
            * before we get the future. The command is destroyed in the main 
            * thread to make it impossible to run a single command twice.
            *     zmqnet_thread_lock, specific to this callback function, is 
            * used to prevent a second zmqnet thread from overwriting the 
            * pending command before the main thread has a chance to execute it.
            */

            // only process one zmqnet thread at a time
            static std::mutex zmqnet_thread_lock;
            std::scoped_lock<std::mutex> thread_lock(zmqnet_thread_lock);

            // make sure a command isn't currently running on the main thread
            std::unique_lock<std::mutex> command_lock(m_command_lock);
            
            // bind argument to command and get future
            m_pending_command = std::make_unique<CommandDispatchFunc>
                (std::bind(dbgsrv_dispatch_cmds, command.asChars().begin(), std::placeholders::_1));
            auto future = m_pending_command->get_future();

            // ok for main thread to run (and delete) m_pending_command now
            command_lock.unlock();

            LOG("zmqnet: waiting for result\n");

            try {
                auto result = future.get();

                LOG("zmqnet: got result: \"%s\"\n", result.get());

                return result ? 
                    packResult(std::move(result))
                    :
                    // intentionally blank result; the frontend will be waiting for something
                    zmqnet::make_ByteArray("", 1) 
                ;
            } catch (const std::exception &e) {
                LOG("zmqnet: exception thrown during command execution: %s.\n", e.what());
                return zmqnet::make_ByteArray("", 1); // TODO more descriptive error result
            } catch (...) {
                LOG("zmqnet: unknown exception thrown during command execution.\n");
                return zmqnet::make_ByteArray("", 1); // TODO more descriptive error result
            }
        };

        m_network->registerCallback(commandStreamId, commandCallback);
    }

    { // register event stream callback
        const auto eventStreamId = static_cast<zmqnet::Stream::Id>(ZMQNetStream::EventStream);
        const auto eventCallback = [&](zmqnet::Stream::Id const& streamId, zmqnet::ByteArray command) {
            // Nothing should be sent on this stream from the client; it is used to
            // send async events back to the client for processing. We still need
            // to register a callback to use it this way, though.
            
            LOG("zmqnet: event callback %d got command \"%s\" from stream %d\n", node_id, command.begin(), streamId);
            LOG("zmqnet: WARNING: client shouldn't be sending data on the event stream.\n");
            assert(streamId == eventStreamId);

            return zmqnet::ByteArray{};
        };

        m_network->registerCallback(eventStreamId, eventCallback);
    }

    // remember my rank for when INVOKE comes with rest of info
    m_rank = procnum;
    LOG("Registered backend number %lu.\n", m_rank);

    m_network->notifyReady();

    LOG("Made it out of initProto!\n");

    return 0;
}

void ZMQNetServerProtocol::endProto() {
    LOG("ZMQNetServerProtocol::endProto\n");

    if (m_network) m_network->waitForExit();
}

bool ZMQNetServerProtocol::checkEvents() const {
    // LOG("ZMQNetServerProtocol::checkEvents\n");
    std::scoped_lock<std::mutex> lock(m_command_lock);
    return m_pending_command != nullptr;
}

int ZMQNetServerProtocol::setupIo(int *fd, int *port) { 
    LOG("ZMQNetServerProtocol::setupIo\n");
    return 0; 
}

int ZMQNetServerProtocol::transferIo(int fd, char *buffer, int size) {
    LOG("ZMQNetServerProtocol::transferIo\n");
    return 0;
}

void ZMQNetServerProtocol::runProto(fd_set *fds) {
    LOG("ZMQNetServerProtocol::runProto\n");
    std::scoped_lock<std::mutex> lock(m_command_lock);

    if (m_pending_command) {
        // zmqnet callback has already bound proper arguments.
        // zmqnet callback is waiting in another thread for result, 
        // delivered via a future which has already been bound to the command
        
        (*m_pending_command)(nullptr /* TODO call with ci */);
        m_pending_command.reset();
    }
}

dbgevent_t* ZMQNetServerProtocol::invokeNetwork(int num_be) {
    LOG("ZMQNetServerProtocol::invokeNetwork\n");

    if (num_be < 1) {
        LOG("zmqnet: num_be was less than 1; it was %d\n", num_be);
        return NewEvent(DBGEV_ERROR);
    }

    // create bitset
    m_rank_bitset = std::make_unique<LRunLEBitset>(num_be, m_rank);
    m_rank_serialized = m_rank_bitset->serialize();
    m_rank_serialized_size = LRunLEBitset::extractSerializedSize(m_rank_serialized);

    LOG("zmqnet: created bitset. rank %s out of %d.\n", m_rank_bitset->toString().c_str(), num_be);

    return NewEvent(DBGEV_OK);
}

int ZMQNetServerProtocol::sendReply(const char *response) {
    LOG("ZMQNetServerProtocol::sendReply\n");
    // this is done asynchronously by the zmqnet callbacks
    return 0;
}

// private /////////////////////////////////////////////////////////////////////

zmqnet::ByteArray ZMQNetServerProtocol::packResult(ResultPtr result) {
    DVPRINT("backend: sending away \"%s\"", result.get());
    
    capnp::MallocMessageBuilder builder;
    auto message_pack = builder.initRoot<ZmqnetMessagePackage>();
    auto message = message_pack.initMessages(1)[0];

    // data
    message.setData(result.get()); // copy

    // data hash
    auto hash = std::hash<std::string>{}(result.get());
    message.setHash(hash);

    // bitset
    auto bitset = message.initBitset(m_rank_serialized_size);
    memcpy(bitset.begin(), m_rank_serialized.get(), m_rank_serialized_size); // copy

    // pack
    auto flat = capnp::messageToFlatArray(builder);

    return flat.forceReleaseAsBytes();
}
