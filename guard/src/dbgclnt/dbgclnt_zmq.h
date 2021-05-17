/******************************************************************************\
 * Copyright 2021 Hewlett Packard Enterprise Development LP.
 ******************************************************************************/

#pragma once

#include "dbgclnt_proto.h"
#include "dbginvoke.h"
#include "zmqnet.hpp"

namespace {
class ZMQNetAttachHandler : public DebugClientProtocolEventHandler {
public:
    int handleEvent(dbginvoke *di, proto_msg_set *set) override;
};

class ZMQNetLaunchHandler : public DebugClientProtocolEventHandler {
public:
    int handleEvent(dbginvoke *di, proto_msg_set *set) override;
};
} // private namespace

class ZMQNetClientProtocol : public DebugClientProtocol {
public:
    // ZMQnet does not support:
    //  * finish function
    //  * checkEvents
    //  * any session functions

    ZMQNetClientProtocol() : DebugClientProtocol() { registerProtocol(); };

    const char* name() const override { return "zmqnet"; };

    int attach(struct dbginvoke *di, proto_session *session, 
        const char *app_ident, const char *arch, const char *backend, 
        int gpu_flag, int debug_flag) override;

    int launch(struct dbginvoke *di, proto_session *session, 
        const char *prog, char **argv,
        const char *arch, char **arch_args, const char *arch_input, 
        const char *backend, const char* gdb, char **env,
        const char *cwd, int gpu_flag, int debug_flag, int early_entry_flag, 
        bool isNonStop) override;

    int exit(struct dbginvoke *, int detach) override;

    bitset* sendCommand(struct dbginvoke *di, bitset *target, 
        const char *request) override;

    struct proto_msg_set* receiveReply(struct dbginvoke *di, 
        bitset *target, bitset *received) override;

    int init() override;
    
    void dispatchEvents(struct dbginvoke *di, int ev_fd) override;

    void destroyClient(struct dbginvoke *di) override;
};

// called once by registerProtos
void ZMQClientProtocol_init() {
    // will register itself in constructor
    static ZMQNetClientProtocol proto;
};
