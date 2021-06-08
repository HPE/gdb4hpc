/******************************************************************************\
 * Copyright 2021 Hewlett Packard Enterprise Development LP.
 ******************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#include "zmqnet.hpp"

#include "dbgsrv_zmq.h"
#include "dbgsrv_defs.h"
#include "dbgsrv_cmds.h"

#define LOG(...) (debug_flag ? _dvprint(__VA_ARGS__) : (void)0)

// Proto definition
ZMQNetServerProtocol DbgSrvProtoZMQNet;

ZMQNetServerProtocol::ZMQNetServerProtocol()
    : m_network{nullptr} 
    , m_pending_command{nullptr}
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
    return 0;
}

int ZMQNetServerProtocol::initProto(int procnum, int argc, const char **argv,
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

    m_network = std::make_unique<zmqnet::AsyncBackend>(upstreamControlEndpoint, upstreamDataEndpoint, node_id);

    { // register command stream callback
        const auto commandStreamId = static_cast<zmqnet::Stream::Id>(ZMQNetStream::CommandStream);
        const auto command_callback = [&](zmqnet::Stream::Id const& streamId, zmqnet::ByteArray command) {
            LOG("zmqnet: callback %d got command \"%s\" from stream %d\n", node_id, command.begin(), streamId);
            assert(streamId == commandStreamId);
            
            // only process one zmqnet thread at a time
            static std::mutex zmqnet_thread_lock;
            std::scoped_lock<std::mutex> thread_lock(zmqnet_thread_lock);

            // make sure a command isn't currently running on the main thread
            std::unique_lock<std::mutex> command_lock(m_command_lock);
            
            // bind argument to command and get future
            m_pending_command = std::make_unique<CommandDispatchFunc>
                (std::bind(dbgsrv_dispatch_cmds, command.asChars().begin(), std::placeholders::_1));
            auto future = m_pending_command->get_future();
            
            // tell main thread there is an event waiting
            incEventFlag();

            // ok to run (and delete) m_pending_command now
            command_lock.unlock();

            LOG("zmqnet: waiting for result\n");

            auto result = future.get();

            LOG("zmqnet: got result: \"%s\"\n", result.get());

            return result ? 
                zmqnet::make_ByteArray(result.get(), std::strlen(result.get()))
                :
                zmqnet::make_ByteArray("", 1)
            ;
        };

        m_network->registerCallback(commandStreamId, command_callback);
    }

    // Set up event fd
    if (pipe2(m_event_pipe, O_NONBLOCK)) {
        LOG("Creation of event pipe failed.\n");
        return -1;
    }

    m_network->notifyReady();

    LOG("Made it out of initProto!\n");

    return 0;
}

void ZMQNetServerProtocol::endProto() {
    LOG("ZMQNetServerProtocol::endProto\n");

    close(m_event_pipe[0]);
    close(m_event_pipe[1]);

    if (m_network) m_network->waitForExit();
}

fd_set *ZMQNetServerProtocol::getFdSet() {
    LOG("ZMQNetServerProtocol::getFdSet\n");
    // this is reconstructed every call because the main loop layer adds to it
    // every iteration

    FD_ZERO(&m_fds);
    FD_SET(m_event_pipe[0], &m_fds);

    return &m_fds;
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
    } else {
        LOG("warning: called runProto with null pending command.\n");
    }

    decEventFlag();
}

dbgevent_t *ZMQNetServerProtocol::invokeNetwork(int numbe) {
    LOG("ZMQNetServerProtocol::invokeNetwork\n");
    return nullptr;
}

int ZMQNetServerProtocol::sendReply(const char *response) {
    LOG("ZMQNetServerProtocol::sendReply\n");
    // this is done asynchronously by the zmqnet callbacks
    return 0;
}

void ZMQNetServerProtocol::incEventFlag() {
    LOG("ZMQNetServerProtocol::incEventFlag\n");
    // put an x in the pipe to have the main thread call runProto.
    const char buf[] = {'x'};
    auto sent = write(m_event_pipe[1], &buf, sizeof(char));

    if (sent < 0) {
        LOG("error in write: %d: %s\n", errno, strerror(errno));
    } else if (sent == 0) {
        LOG("error: write didn't write anything\n");
    }
}

void ZMQNetServerProtocol::decEventFlag() {
    LOG("ZMQNetServerProtocol::decEventFlag\n");
    // consume a byte in the pipe
    char buf[1];
    auto nread = read(m_event_pipe[0], buf, 1);

    if (nread < 0) {
        LOG("error in read: %d: %s\n", errno, strerror(errno));
    } else if (nread == 0) {
        LOG("error: read didn't read anything\n");
    }
}
