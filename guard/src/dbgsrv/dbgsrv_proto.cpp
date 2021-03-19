/*********************************************************************************\
 * Debug server protocol switch routines.
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * Copyright 2010-2013,2020 Hewlett Packard Enterprise Development LP.
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
#include "defs.h"
#include "useful.h"

#include "dbgsrv_mrn.h"

// external proto defs
extern MRNServerProtocol DbgSrvProtoMRN;

static const std::vector<DebugServerProtocol *> DbgProtoTab = {
    &DbgSrvProtoMRN
};

/*
** Set default proto.
** This defines the current protocol in use
*/
static DebugServerProtocol *DbgSrvProtoSW = &DbgSrvProtoMRN; /* Default to MRNET */

/*
** dbgsrv protocol related functions
*/

// NOTE: Only one protocol/debug backend can be active. The debug server is
//       designed to talk to only one client, whereas the client is designed to
//       talk to many different debug servers. Hence why the client handles
//       multiple protocols at once, and the server doesn't.

int DbgSetSrvProto(const char *protoName) {
    for (const auto &proto : DbgProtoTab) {
        if (strcmp(proto->name(), protoName) == 0) {
            DbgSrvProtoSW = proto;
            return 0;
        }
    }

    return 1;
}

int DbgInitProto(int procnum, int argc, char **argv, const char *key,
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

int DbgSendReply(char *response) {
    int res;

    res = DbgSrvProtoSW->sendReply(response);

    Free(response);

    return res;
}

int DbgInvokeCB(char *chost, int cprog, char *host, int prog, int pnum,
                int port, dbgevent_t *ev) {
    return DbgSrvProtoSW->invokeCallback(chost, cprog, host, prog, pnum, port, ev);
}

int DbgEventCB(dbgevent_t *ev) { return DbgSrvProtoSW->eventCallback(ev); }
