/******************************************************************************\
 * Copyright 2021 Hewlett Packard Enterprise Development LP.
 ******************************************************************************/

#include "dbgclnt_zmq.h"
#include "defs.h"
#include "bitset.h"

// Instantiation of the protocol - accessed via DbgProtoTab in dbgclnt_proto.cpp
ZMQNetClientProtocol DbgClntProtoZMQ;

int ZMQNetClientProtocol::init() {
    DPRINT("ZMQNetClientProtocol::init\n");
    return 0;
}

bitset* ZMQNetClientProtocol::sendCommand(dbginvoke_t *di, bitset *target,
                             const char *request) {
    DPRINT("ZMQNetClientProtocol::sendCommand\n");
    return nullptr;
}

struct proto_msg_set* ZMQNetClientProtocol::receiveReply(dbginvoke_t *di, 
                                            bitset *target, bitset *received) {
    DPRINT("ZMQNetClientProtocol::receiveReply\n");
    return nullptr;
}

void ZMQNetClientProtocol::dispatchEvents(dbginvoke_t *di, int ev_fd) {
    DPRINT("ZMQNetClientProtocol::dispatchEvents\n");
}

int ZMQNetClientProtocol::attach(dbginvoke_t *di, proto_session *sess,
                        const char *app_ident, const char *arch,
                        const char *backend, int gpu_flag, int debug_flag) {
    DPRINT("ZMQNetClientProtocol::attach\n");
    return 0;
}

int ZMQNetClientProtocol::launch(dbginvoke_t *di, proto_session *sess, const char *prog,
                        char **argv, const char *arch, char **arch_args,
                        const char *arch_input, const char *backend, const char* gdb, char **env,
                        const char *cwd, int gpu_flag, int debug_flag,
                        int early_entry_flag, bool isNonStop) {
    DPRINT("ZMQNetClientProtocol::launch\n");
    return 0;
}

void ZMQNetClientProtocol::destroyClient(dbginvoke *di) {
    DPRINT("ZMQNetClientProtocol::destroyClient\n");
}

int ZMQNetClientProtocol::exit(dbginvoke_t *di, int detach) {
    DPRINT("ZMQNetClientProtocol::exit\n");
    return 0;
}

int ZMQNetAttachHandler::handleEvent(dbginvoke_t *di, proto_msg_set *set) {
    DPRINT("ZMQNetAttachHandler::handleEvent\n");
    return 0;
}

int ZMQNetLaunchHandler::handleEvent(dbginvoke_t *di, proto_msg_set *set) {
    DPRINT("ZMQNetAttachHandler::handleEvent\n");
    return 0;
}
