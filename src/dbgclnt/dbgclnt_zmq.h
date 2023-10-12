/******************************************************************************\
 * Copyright 2021-2022 Hewlett Packard Enterprise Development LP. 
 ******************************************************************************/

#pragma once

#include "dbgclnt_proto.h"
#include "dbg_zmq_network.h"
#include "dbginvoke.h"

class ZMQNetClientProtocol : public DebugClientProtocol {
public:
    // ZMQnet does not support:
    //  * finish function

    ZMQNetClientProtocol();

    const char* name() const override { return "zmqnet"; };

    int attach(DbgInvoke *di, const AttachOpts& opts) override;

    int launch(DbgInvoke *di, const LaunchOpts& opts) override;

    int syncExit(DbgInvoke *) override;
    int forceExit(DbgInvoke *) override;

    bool sendCommand(DbgInvoke *di, const Bitset *target, const char *request) override;

    std::variant<ProtoMessageSet, ProtoNothing> receiveReply(DbgInvoke *di,
        const Bitset *target, std::chrono::seconds timeout) override;

    int init() override;
    
    int checkEvents(DbgInvoke *di) override;
    void dispatchEvents(DbgInvoke *di, int ev_fd) override;
private:
    // separate attach function so we can reuse its attach functionality within the launch function
    int internalAttach(DbgInvoke *di, const char *prog,
                       uint64_t appid,
                       ZMQNetNetwork::OutputPtr &&output,
                       const AttachOpts& opts,
                       int early_entry_flag, bool isNonStop,
                       bool isNonMpi);
};

// called once by registerProtos
void ZMQClientProtocol_init() {
    // will register itself in constructor
    static ZMQNetClientProtocol proto;
};
