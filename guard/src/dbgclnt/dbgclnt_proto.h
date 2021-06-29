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

#include <string>
#include <exception>
#include <memory>
#include <vector>
#include <unordered_map>

// forward declare struct dbginvoke - Note that because the struct dbginvoke is
// first encountered inside of a list of parameters for the function pointer, we
// *must* forward declare it otherwise the compiler will mess up.
struct dbginvoke;

// Some protocols will interact with the workload manager. Known workload
// managers are defined in the following enumeration. Note that the name
// cooresponds to the job submission command used by the WLM.
enum WLM_type { WLM_none, WLM_qsub, WLM_sbatch };
typedef enum WLM_type WLM_type;

class DebugClientProtocol;

// Used to keep track of sessions associated with the proxy based dbgclnt_proto
// implementation
struct proto_session {
    proto_session(DebugClientProtocol * const sess_proto_, void *sess_obj_)
        : sess_proto{sess_proto_}
        , sess_obj{sess_obj_} {}
    
    DebugClientProtocol * const sess_proto;
    void *sess_obj; // This is data used by the protocol layer
                    // that is allocated on a per session basis.
};

// event handlers used by protocols to handle events in certain situations
//
// 
class DebugClientProtocolEventHandler {
public:
    virtual ~DebugClientProtocolEventHandler() = default;
    virtual int handleEvent(dbginvoke *di, const ProtoMessageSet &set) = 0;
};

class DebugClientDefaultProtocolEventHandler : public DebugClientProtocolEventHandler{
public:
    int handleEvent(dbginvoke *di, const ProtoMessageSet &set) override;
};

// main protocol definition
class DebugClientProtocol {
public:
    DebugClientProtocol();
    virtual ~DebugClientProtocol() = default;

    static const std::unordered_map<std::string, DebugClientProtocol *>& getRegistry();

    // functions required by every derived class:
    virtual const char* name() const = 0;

    virtual int attach(struct dbginvoke *di, proto_session *session, 
        const char *app_ident, const char *arch, const char *backend, 
        int gpu_flag, int debug_flag) = 0;

    virtual int launch(struct dbginvoke *di, proto_session *session, 
        const char *prog, const char * const *argv,
        const char *arch, const char * const *arch_args, const char *arch_input, 
        const char *backend, const char* gdb, char **env,
        const char *cwd, int gpu_flag, int debug_flag, int early_entry_flag, 
        bool isNonStop) = 0;

    virtual int exit(struct dbginvoke *di, int detach);

    virtual std::unique_ptr<Bitset> sendCommand(struct dbginvoke *di, const Bitset *target, 
        const char *request) = 0;

    virtual std::optional<ProtoMessageSet> receiveReply(struct dbginvoke *di, 
        const Bitset *target, Bitset &received) = 0;

    // optional functions:
    virtual int init() { return 1; }; // success by default
    virtual void finish() {};
    
    virtual int checkEvents(struct dbginvoke *di) { return 0; };
    virtual void dispatchEvents(struct dbginvoke *di, int ev_fd) {};

    virtual proto_session *createSession(DebugClientProtocol * const proto, 
        WLM_type manager, const char *batch_template, const char *hostname, 
        const char *username) { return nullptr; };
    
    virtual void saveSession(proto_session *session) {};
    // called when a session is no longer valid
    virtual void cleanupSession(proto_session *session) {};

    virtual void destroyClient(struct dbginvoke *di) {};

    void resetProtocolEventHandler();
protected:
    // what client protocols are currently known
    static std::unordered_map<std::string, DebugClientProtocol *> registry;

    // callback related
    virtual void outputCallback(dbginvoke *di, FILE *f);
    void setProtocolEventHandler(DebugClientProtocolEventHandler *handler);
    int handleProtocolEvent(dbginvoke *di, const ProtoMessageSet &set);

    // called by derived classes' constructors to register themselves
    void registerProtocol();
private:
    // we don't want this exposed to derived classes directly, they should only
    // be able to set it and call its handler method.
    std::unique_ptr<DebugClientProtocolEventHandler> m_protoEventHandler;
};

/*
** Function definitions
*/
DebugClientProtocol * const findClientProtoByName(const char* protoName);

extern int WaitForProtoStartupEvent(struct dbginvoke *);

// called once to register available protocols
void registerProtos();
