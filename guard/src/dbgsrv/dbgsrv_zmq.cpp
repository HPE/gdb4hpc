/******************************************************************************\
 * Copyright 2021 Hewlett Packard Enterprise Development LP.
 ******************************************************************************/

#include <stdio.h>
#include <unistd.h>

#include "zmqnet.hpp"

#include "dbgsrv_zmq.h"
#include "dbgsrv_defs.h"
#include "dbgsrv_cmds.h"

// Proto definition
ZMQNetServerProtocol DbgSrvProtoZMQNet;

ZMQNetServerProtocol::ZMQNetServerProtocol()
     : DebugServerProtocol() 
     , m_network{nullptr} 
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
    DVPRINT("ZMQNetServerProtocol::invokeCallback\n");
    return 1;
}

int ZMQNetServerProtocol::eventCallback(dbgevent_t *ev) {
    DVPRINT("ZMQNetServerProtocol::eventCallback\n");
    return 0;
}

int ZMQNetServerProtocol::initProto(int procnum, int argc, const char **argv,
                              const char *key, const char *host, int port) {
    DVPRINT("ZMQNetServerProtocol::initProto\n");

    { 
        DVPRINT("argc: %d\n", argc);
        DVPRINT("argv: ");
        // print out the argv
        auto tmp = argv;
        while (*tmp != NULL) {
            fprintf(stderr, "%s ", *tmp++);
        }
        fprintf(stderr, "\n"); 
    }

    auto const [upstreamControlEndpoint, upstreamDataEndpoint, node_id,
		rest_argc, rest_argv] = zmqnet::AsyncBackend::getParams(argc, argv);

    m_network = std::make_unique<zmqnet::AsyncBackend>(upstreamControlEndpoint, upstreamDataEndpoint, node_id);

    { // register command stream callback
        const auto streamId = static_cast<zmqnet::Stream::Id>(ZMQNetStream::CommandStream);
        const auto command_callback = [&](zmqnet::Stream::Id const& streamId, zmqnet::ByteArray command) {
            DVPRINT("zmqnet: send callback %d got command \"%s\" from stream %d\n", node_id, command.begin(), streamId);
            assert(streamId == streamId);
            
            m_command_queue.push(command.asChars().begin());
            DVPRINT("zmqnet: done pushing command to queue. waiting for result\n");
            auto result = m_result_queue.waitForNext();

            return zmqnet::make_ByteArray(result.c_str(), result.length() + 1);
        };

        m_network->registerCallback(streamId, command_callback);
    }

    // Set up event fd
    if (pipe(m_event_pipe)) {
        DVPRINT("Creation of event pipe failed.\n");
        return -1;
    }

    { // put an x in the pipe to have the main thread call runProto.
      // this never gets cleared.
        const char buf[] = {'x'};
        auto sent = write(m_event_pipe[1], &buf, sizeof(char));
        if (sent < 0) {
            DVPRINT("error in write: %d: %s\n", errno, strerror(errno));
            return -1;
        }
    }

    m_network->notifyReady();

    DVPRINT("Made it out of initProto!\n");

    return 0;
}

void ZMQNetServerProtocol::endProto() {
    DVPRINT("ZMQNetServerProtocol::endProto\n");

    close(m_event_pipe[0]);
    close(m_event_pipe[1]);

    if (m_network) m_network->waitForExit();
}

fd_set *ZMQNetServerProtocol::getFdSet() {
    DVPRINT("ZMQNetServerProtocol::getFdSet\n");
    static fd_set fds;

    // this is reconstructed every call because the main loop layer adds to it
    // every iteration

    FD_ZERO(&fds);
    FD_SET(m_event_pipe[0], &fds);

    return &fds;
}

int ZMQNetServerProtocol::setupIo(int *fd, int *port) { 
    DVPRINT("ZMQNetServerProtocol::setupIo\n");
    return 0; 
}

int ZMQNetServerProtocol::transferIo(int fd, char *buffer, int size) {
    DVPRINT("ZMQNetServerProtocol::transferIo\n");
    return 0;
}

void ZMQNetServerProtocol::runProto(fd_set *fds) {
    DVPRINT("ZMQNetServerProtocol::runProto\n");

    while (auto command = m_command_queue.getNextIfAvailable()) {
        DVPRINT("got %s from command queue\n", command->c_str());
        // TODO pass ci
        DVPRINT("dispatching \"%s\"\n", command->c_str());
        auto result = dbgsrv_dispatch_cmds(command->c_str(), nullptr);

        DVPRINT("pushing result \"%s\"...\n", result.c_str());
        m_result_queue.push(result);
        DVPRINT("done!\n");
    }

    DVPRINT("dispatched all commands.\n");
}

dbgevent_t *ZMQNetServerProtocol::invokeNetwork(int numbe) {
    DVPRINT("ZMQNetServerProtocol::invokeNetwork\n");
    return nullptr;
}

int ZMQNetServerProtocol::sendReply(const char *response) {
    DVPRINT("ZMQNetServerProtocol::sendReply\n");
    // this is done by the zmqnet callbacks
    return 0;
}
