/*********************************************************************************\
 * Debug server protocol switch routines.
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * Copyright 2010-2013,2020 Cray Inc.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Cray Inc.,
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

#include <stdlib.h>
#include <string.h>

#include "dbgsrv_proto.h"
#include "defs.h"
#include "useful.h"

/*
** Setup protocol defs
*/
struct proto_tab {
    const char *proto_name;
    dbgsrv_proto *proto_def;
};
typedef struct proto_tab proto_tab;

// external proto defs
#ifdef SUN_RPC
extern dbgsrv_proto DbgSrvRPCProtoSW;
#endif /* SUN_RPC */
#ifdef SOCKETS
extern dbgsrv_proto DbgSrvSocProtoSW;
#endif /* SOCKETS */
#ifdef MRNET
extern dbgsrv_proto DbgSrvMRNProtoSW;
#endif /* MRNET */
#ifdef SCI
extern dbgsrv_proto DbgSrvSCIProtoSW;
#endif /* SCI */

// table of proto defs
static proto_tab DbgProtoTab[] = {
#ifdef SUN_RPC
    {"rpc", &DbgSrvRPCProtoSW},
#endif /* SUN_RPC */
#ifdef SOCKETS
    {"socket", &DbgSrvSocProtoSW},
#endif /* SOCKETS */
#ifdef MRNET
    {"mrnet", &DbgSrvMRNProtoSW},
#endif /* MRNET */
#ifdef SCI
    {"sci", &DbgSrvSCIProtoSW},
#endif /* SCI */
    {NULL, (dbgsrv_proto *)NULL}};

/*
** Set default proto.
** This defines the current protocol in use
*/
#if defined(MRNET)
static dbgsrv_proto *DbgSrvProtoSW = &DbgSrvMRNProtoSW; /* Default to MRNET */
#elif defined(SCI)
static dbgsrv_proto *DbgSrvProtoSW = &DbgSrvSCIProtoSW; /* Default to SCI */
#elif defined(SUN_RPC)
static dbgsrv_proto *DbgSrvProtoSW = &DbgSrvRPCProtoSW; /* Default to RPC */
#elif defined(SOCKETS)
static dbgsrv_proto *DbgSrvProtoSW = &DbgSrvSocProtoSW; /* Default to SOC */
#endif

/*
** dbgsrv protocol related functions
*/

// NOTE: Only one protocol/debug backend can be active. The debug server is
//       designed to talk to only one client, whereas the client is designed to
//       talk to many different debug servers. Hence why the client handles
//       multiple protocols at once, and the server doesn't.

int DbgSetSrvProto(const char *proto) {
    proto_tab *p;

    for (p = DbgProtoTab; p->proto_name != NULL; p++) {
        if (strcmp(p->proto_name, proto) == 0) {
            // Set the current protocol
            DbgSrvProtoSW = p->proto_def;
            return 0;
        }
    }
    return 1;
}

int DbgInitProto(int procnum, int argc, char **argv, const char *key,
                 const char *host, int port) {
    return DbgSrvProtoSW->sw_initproto(procnum, argc, argv, key, host, port);
}

fd_set *DbgGetFDSet(void) { return DbgSrvProtoSW->sw_getfdset(); }

void DbgRunProto(fd_set *fds) { DbgSrvProtoSW->sw_runproto(fds); }

void DbgEndProto(void) { DbgSrvProtoSW->sw_endproto(); }

int DbgSetupIO(int *fd, int *port) {
    return DbgSrvProtoSW->sw_setupio(fd, port);
}

int DbgTransferIO(int fd, char *buffer, int size) {
    return DbgSrvProtoSW->sw_transferio(fd, buffer, size);
}

dbgevent_t *DbgInvokeNetwork(int numbe) {
    return DbgSrvProtoSW->sw_invokenetwork(numbe);
}

int DbgSendReply(char *response) {
    int res;

    res = DbgSrvProtoSW->sw_sendreply(response);

    Free(response);

    return res;
}

int DbgInvokeCB(char *chost, int cprog, char *host, int prog, int pnum,
                int port, dbgevent_t *ev) {
    return DbgSrvProtoSW->sw_invokecb(chost, cprog, host, prog, pnum, port, ev);
}

int DbgEventCB(dbgevent_t *ev) { return DbgSrvProtoSW->sw_eventcb(ev); }
