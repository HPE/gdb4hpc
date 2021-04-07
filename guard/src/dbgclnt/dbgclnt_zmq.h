/******************************************************************************\
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 * Copyright 2010-2021 Hewlett Packard Enterprise Development LP.
 ******************************************************************************/

#ifndef _DBGCLNT_ZMQ_H
#define _DBGCLNT_ZMQ_H

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

    ZMQNetClientProtocol() : DebugClientProtocol() {};

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

#endif /* _DBGCLNT_ZMQ_H */
