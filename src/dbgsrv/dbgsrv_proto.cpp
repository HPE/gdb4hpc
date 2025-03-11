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

#include "config.h"

#include <vector>

#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <unistd.h>

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
    extern void ZMQServerProtocol_link();
    ZMQServerProtocol_link();

    // avoid "unused function" warning
    if (false)
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

int DbgInitProto(int &procnum, int &pid, int argc, const char * const * argv, const char *key,
                 const char *host, int port) {
    return DbgSrvProtoSW->initProto(procnum, pid, argc, argv, key, host, port);
}

fd_set *DbgGetFdSet(int dbgFd, int ioClientFd, int ioBackendFd) { 
    auto readfds = DbgSrvProtoSW->getFdSet(); 

    if (!readfds) {
        // if the protocol doesn't use fdsets, it will return nullptr. in that
        // case we'll make our own.
        static fd_set fds{};
        readfds = &fds;
    }

    // add the debugger output file descriptor to the set
    if (dbgFd >= 0) FD_SET(dbgFd, readfds);

    /*
    ** If IOBackendFD >= 0 then we are reading the target output and
    ** the debugger has been told to use read it. We should read from the
    ** master side.
    */
    if (ioBackendFd >= 0) {
        FD_SET(ioBackendFd, readfds);

        /*
        ** IOClientFD is just used to set up the socket
        ** to the client. Once this has been done,
        ** it is set to -1 and not used again.
        */
        if (ioClientFd >= 0) FD_SET(ioClientFd, readfds);
    }

    return readfds;
}

int DbgCheckEvents(fd_set *fds) {
    int dtbsz = min(FD_SETSIZE, getdtablesize());

    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 10000;

    switch (select(dtbsz, fds, NULL, NULL, &tv)) {
        case -1:
            // error case, layer above will handle it for us.
            return -1;
        case 0:
            // if the proto supports a checkEvents function, check it here
            if (DbgSrvProtoSW->checkEvents()) return 1;
            return 0;
        default:
            // event found, layer above will handle it for us.
            return 1;
    }

    // should never get here
    return 0;
}

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
