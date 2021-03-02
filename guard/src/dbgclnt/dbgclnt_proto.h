/*********************************************************************************\
 * Debug protocol definitions
 * TODO: This interface could use proper documentation since it's an abstraction
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * Copyright 2010-2014,2020 Hewlett Packard Enterprise Development LP.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Hewlett Packard Enterprise Development LP.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *
 * $HeadURL$
 * $Date$
 * $Rev$
 * $Author$
 *
 *********************************************************************************/

/*
** XXX: This interface is tightly coupled with dbg_client.h. It really should be
**       considered the same interface. We place all of this in a seperate file
**       because each network type will create an implementation of this
**       interface. The dbginvoke_t struct is defined in dbg_client, but is
**       passed "down" to this interface. Yet the dbg_client interface relies
**       on types defined here. This creates a circular dependency that
*typically
**       is frowned upon, but in this instance is alright. We want to make it
**       clear that new network types should only implement the dbgclnt_proto
**       object.
*/

#ifndef _DBGCLNT_PROTO_H
#define _DBGCLNT_PROTO_H

#include "bitset.h"
#include "proto_message.h"

#include <string>
#include <exception>
#include <vector>

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

// thrown when a child class of DebugClientProtocol attempts to call a function
// that the child class does not implement.
struct FunctionNotImplementedException : public std::exception {
    const char* fName;

    FunctionNotImplementedException(const char* functionName) 
        : std::exception()
        , fName{functionName}
    {}

    const char* functionName() const { return fName; }
};

// main protocol definition
//
// child classes should provide a name() and various functions. once a function
// has been implimented, the child class should override the matching has...() function.
class DebugClientProtocol {
public:
    // name of the protocol
    virtual const std::string name() const = 0;

    virtual int init() { throw FunctionNotImplementedException("init"); };
    virtual bool hasInit() const { return false; };

    virtual void finish() { throw FunctionNotImplementedException("finish"); };
    virtual bool hasFinish() const { return false; };

    virtual bitset* sendCommand(struct dbginvoke *, bitset *, const char *) { throw FunctionNotImplementedException("sendCommand"); };
    virtual bool hasSendCommand() const { return false; };

    virtual struct proto_msg_set* receiveReply(struct dbginvoke *, bitset *,bitset *) { throw FunctionNotImplementedException("receiveReply"); };
    virtual bool hasReceiveReply() const { return false; };

    virtual int checkEvents(struct dbginvoke *) { throw FunctionNotImplementedException("checkEvents"); }; // check for ready event
    virtual bool hasCheckEvents() const { return false; };

    virtual void dispatchEvents(struct dbginvoke *, int) { throw FunctionNotImplementedException("dispatchEvents"); };
    virtual bool hasDispatchEvents() const { return false; };

    // Client protocol routines
    virtual proto_session *createSession(DebugClientProtocol * const, WLM_type, 
        const char *, const char *,const char *) { throw FunctionNotImplementedException("createSession"); };
    virtual bool hasCreateSession() const { return false; };

    virtual void saveSession(proto_session *) { throw FunctionNotImplementedException("saveSession"); };
    virtual bool hasSaveSession() const { return false; };

    virtual void cleanupSession(proto_session *) { throw FunctionNotImplementedException("cleanupSession"); }; // called when a session is no longer valid
    virtual bool hasCleanupSession() const { return false; };

    virtual int attach(struct dbginvoke *, proto_session *, const char *,
        const char *, const char *, int, int) { throw FunctionNotImplementedException("attach"); };
    virtual bool hasAttach() const { return false; };

    virtual int launch(struct dbginvoke *, proto_session *, const char *, char **,
        const char *, char **, const char *, const char *, const char*, char **,
        const char *, int, int, int, bool) { throw FunctionNotImplementedException("launch"); };
    virtual bool hasLaunch() const { return false; };

    virtual int exit(struct dbginvoke *, int detach) { throw FunctionNotImplementedException("exit"); };
    virtual bool hasExit() const { return false; };

    virtual void destroyClient(struct dbginvoke *) { throw FunctionNotImplementedException("destroyClient"); };
    virtual bool hasDestroyClient() const { return false; };
};

/*
** Function definitions
*/
const std::vector<DebugClientProtocol *> getClientProtos();
DebugClientProtocol * const findClientProtoByName(std::string protoName);

extern int WaitForProtoStartupEvent(struct dbginvoke *);

#endif /* !_DBGCLNT_PROTO_H */
