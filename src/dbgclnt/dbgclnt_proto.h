/*********************************************************************************\
 * Debug protocol definitions
 * TODO: This interface could use proper documentation since it's an abstraction
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * Copyright 2010-2014,2020-2021 Hewlett Packard Enterprise Development LP.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Hewlett Packard Enterprise Development LP.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *
 *********************************************************************************/

/*
** XXX: This interface is tightly coupled with dbg_client.h. It really should be
**       considered the same interface. We place all of this in a seperate file
**       because each network type will create an implementation of this
**       interface. The dbginvoke_t struct is defined in dbg_client, but is
**       passed "down" to this interface. Yet the dbg_client interface relies
**       on types defined here. This creates a circular dependency that typically
**       is frowned upon, but in this instance is alright. We want to make it
**       clear that new network types should only implement the dbgclnt_proto
**       object.
*/

#pragma once

#include "bitset.h"
#include "proto_message.h"
#include "dbginvoke.h"

#include <string>
#include <exception>
#include <memory>
#include <vector>
#include <unordered_map>
#include <chrono>
#include <variant>

// event handlers used by protocols to handle events in certain situations
//
// 
class DebugClientProtocolEventHandler {
public:
    virtual ~DebugClientProtocolEventHandler() = default;
    virtual int handleEvent(DbgInvoke *di, const ProtoMessageSet &set) = 0;
};

class DebugClientDefaultProtocolEventHandler : public DebugClientProtocolEventHandler{
public:
    int handleEvent(DbgInvoke *di, const ProtoMessageSet &set) override;
};

// main protocol definition
class DebugClientProtocol {
public:
    DebugClientProtocol();
    virtual ~DebugClientProtocol() = default;

    static const std::unordered_map<std::string, DebugClientProtocol *>& getRegistry();

    // functions required by every derived class:
    virtual const char* name() const = 0;

    virtual int attach(DbgInvoke *di,
        const char *app_ident, const char *arch, const char *backend,
        const char *gdb, const char *vgdb_file, int gpu_flag, int debug_flag) = 0;

    virtual int launch(DbgInvoke *di,
        const char *prog, const char * const *argv,
        const char *arch, const char * const *arch_args, const char *arch_input, 
        const char *backend, const char* gdb, const char * const *env,
        const char *cwd, int gpu_flag, int debug_flag, int non_mpi_flag,
        int early_entry_flag, 
        bool isNonStop) = 0;

    virtual int syncExit(DbgInvoke *di) = 0;
    virtual int forceExit(DbgInvoke *di) = 0;

    virtual std::unique_ptr<Bitset> sendCommand(DbgInvoke *di, const Bitset *target, 
        const char *request) = 0;

    virtual std::variant<ProtoMessageSet, ProtoNothing> receiveReply(DbgInvoke *di,
        const Bitset *target, std::chrono::seconds timeout, Bitset &received) = 0;

    // optional functions:
    virtual int init() { return 1; }; // success by default
    virtual void finish() {};
    
    virtual int checkEvents(DbgInvoke *di) { return 0; };
    virtual void dispatchEvents(DbgInvoke *di, int ev_fd) {};

    void resetProtocolEventHandler();
protected:
    // what client protocols are currently known
    static std::unordered_map<std::string, DebugClientProtocol *> registry;

    // callback related
    void setProtocolEventHandler(DebugClientProtocolEventHandler *handler);
    int handleProtocolEvent(DbgInvoke *di, const ProtoMessageSet &set);

    // called by derived classes' constructors to register themselves
    void registerProtocol();

    // helper to extract a reference to a network object from a di.
    template <typename Net>
    Net& getNetwork(DbgInvoke *di) {
        if (di->network() == nullptr) {
            throw std::runtime_error("Tried to extract a network from a nullptr.");
        }

        return dynamic_cast<Net&>(*di->network());
    }

    // helper function for attach to parse out the appId from appIdent and
    // register the app with CTI. returns 0 on failure.
    uint64_t parseAndRegisterAppId(const char *appIdent);
private:
    // we don't want this exposed to derived classes directly, they should only
    // be able to set it and call its handler method.
    std::unique_ptr<DebugClientProtocolEventHandler> m_protoEventHandler;
};

/*
** Function definitions
*/
DebugClientProtocol * const findClientProtoByName(const char* protoName);

extern int WaitForProtoStartupEvent(DbgInvoke *);

// called once to register available protocols
void registerProtos();
