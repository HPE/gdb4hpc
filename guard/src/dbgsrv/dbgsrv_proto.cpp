/*********************************************************************************\
 * Debug server protocol switch routines.
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * Copyright 2010-2013,2020-2021 Hewlett Packard Enterprise Development LP.
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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <vector>

#include <stdlib.h>
#include <string.h>

#include "dbgsrv_proto.h"
#include "dbgsrv_defs.h"
#include "defs.h"
#include "useful.h"

/*
 * don't use this function!
 * this function is only here to make sure the linker pulls in the protocol
 * translation units so each protocol calls its constructor and registers itself.
*/
static void linkProtos() {
    #ifdef MRNET
    extern void MRNServerProtocol_link();
    MRNServerProtocol_link();
    #endif
    #ifdef ZMQNET
    extern void ZMQServerProtocol_link();
    ZMQServerProtocol_link();
    #endif

    // avoid "unused function" warning
    linkProtos();
}

std::unordered_map<std::string, DebugServerProtocol *> DebugServerProtocol::registry;

const std::unordered_map<std::string, DebugServerProtocol *>& DebugServerProtocol::getRegistry() {
    return DebugServerProtocol::registry;
}

void DebugServerProtocol::registerProtocol() {
    // NOTE:
    // fprintf is used instead of DVPRINT because registration happens before 
    //   main(), meaning debug_flag will always be 0 at this point, which
    //   stops DVPRINT from outputting anything.

    // called by derived class constructors to register themselves in the registry
    if (registry.insert({name(), this}).second == true) {
        // this protocol doesn't exist in the registry yet, add it
        fprintf(stderr, "Registered server protocol: %s\n", name());
    } else {
        // this protocol already exists
        fprintf(stderr, "Duplicate protocol: %s\n", name());
    }
}

/*
** This defines the current protocol in use
*/
static DebugServerProtocol *DbgSrvProtoSW = nullptr;

/*
** dbgsrv protocol related functions
*/

// NOTE: Only one protocol/debug backend can be active. The debug server is
//       designed to talk to only one client, whereas the client is designed to
//       talk to many different debug servers. Hence why the client handles
//       multiple protocols at once, and the server doesn't.

int DbgSetSrvProto(const char *protoName) {
    auto protoRef = DebugServerProtocol::getRegistry().find(protoName);
    if (protoRef != DebugServerProtocol::getRegistry().end()) {
        DbgSrvProtoSW = protoRef->second;
        DVPRINT("Selected protocol: %s\n", protoRef->second->name());
        return 0;
    }

    return 1;
}

int DbgInitProto(int procnum, int argc, const char **argv, const char *key,
                 const char *host, int port) {
    return DbgSrvProtoSW->initProto(procnum, argc, argv, key, host, port);
}

fd_set *DbgGetFDSet(void) { return DbgSrvProtoSW->getFdSet(); }

void DbgRunProto(fd_set *fds) { DbgSrvProtoSW->runProto(fds); }

void DbgEndProto(void) { DbgSrvProtoSW->endProto(); }

int DbgSetupIO(int *fd, int *port) {
    return DbgSrvProtoSW->setupIo(fd, port);
}

int DbgTransferIO(int fd, char *buffer, int size) {
    return DbgSrvProtoSW->transferIo(fd, buffer, size);
}

dbgevent_t *DbgInvokeNetwork(int numbe) {
    return DbgSrvProtoSW->invokeNetwork(numbe);
}

int DbgSendReply(const char *response) {
    int res;

    res = DbgSrvProtoSW->sendReply(response);

    return res;
}

int DbgInvokeCB(char *chost, int cprog, char *host, int prog, int pnum,
                int port, dbgevent_t *ev) {
    return DbgSrvProtoSW->invokeCallback(chost, cprog, host, prog, pnum, port, ev);
}

int DbgEventCB(dbgevent_t *ev) { return DbgSrvProtoSW->eventCallback(ev); }
