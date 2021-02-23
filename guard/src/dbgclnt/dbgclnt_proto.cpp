/*********************************************************************************\
 * Debug client protocol switch routines.
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

#include <errno.h>
#include <signal.h>
#include <string.h>

#include "aif.h"
#include "bitset.h"
#include "dbg_client.h"
#include "dbgclnt_proto.h"
#include "dbgevent.h"
#include "defs.h"
#include "list.h"
#include "useful.h"

#ifdef SUN_RPC
extern dbgclnt_proto DbgClntRPCProtoSW;
#endif /* SUN_RPC */
#ifdef SOCKETS
extern dbgclnt_proto DbgClntSocProtoSW;
#endif /* SOCKETS */
#ifdef MRNET
extern dbgclnt_proto DbgClntMRNProtoSW;
#endif /* MRNET */
#ifdef SCI
extern dbgclnt_proto DbgClntSCIProtoSW;
#endif /* SCI */
// FIXME
extern dbgclnt_proto DbgClntPROXYProtoSW;

static dbgclnt_proto *DbgProtoTab[] = {
#ifdef SUN_RPC
    &DbgClntRPCProtoSW,
#endif /* SUN_RPC */
#ifdef SOCKETS
    &DbgClntSocProtoSW,
#endif /* SOCKETS */
#ifdef MRNET
    &DbgClntMRNProtoSW,
#endif /* MRNET */
#ifdef SCI
    &DbgClntSCIProtoSW,
#endif /* SCI */
    &DbgClntPROXYProtoSW, (dbgclnt_proto *)NULL};

static dbgclnt_proto **_ptab;

// Used for the protocol wait for events function
static fd_set proto_dispatch_fds;
static int proto_high_fd;

void SetClntProto(void) { _ptab = DbgProtoTab; }

dbgclnt_proto *GetClntProto(void) {
    if (*_ptab == (dbgclnt_proto *)NULL) return (dbgclnt_proto *)NULL;

    return *(_ptab++);
}

dbgclnt_proto *FindClntProto(const char *proto) {
    dbgclnt_proto **p;

    for (p = DbgProtoTab; *p != (dbgclnt_proto *)NULL; p++)
        if (strcmp((*p)->sw_name, proto) == 0) return *p;

    return (dbgclnt_proto *)NULL;
}

// Used during launch/attach to wait for the initial setup to complete

static int CheckForProtoStartupEvent(dbginvoke_t *di) {
    int *ev_fd;
    int status;

    if (di == NULL) return -1;

    // reset the fd_set
    FD_ZERO(&proto_dispatch_fds);
    proto_high_fd = -1;

    /*
    ** Note: This should be used for protocols that do not use an event
    *notification
    **       fd. If a event notification fd can be used, then it should be added
    *to
    **       the sw_evfd list and sw_checkevents should simply return 0.
    */
    if (DBG_PROTO_SW(di)->sw_checkevents !=
            NULL  // if the protocol has a checkevent function defined
        && DBG_PROTO_SW(di)->sw_checkevents(di) >
               0  // if checkevent tells us that there is an event.
    ) {
        DPRINT("DbgCheckEvents: sw_checkevents returned 1\n");
        return 1;  // there is a proto event to handle. Return immediately.
    }

    /*
    ** If there is a d_evfds fd set, we want to add it to the fd_set to select
    *on.
    ** This enables protocol events.
    */
    if (di->d_evfds != NULL) {
        SetList(di->d_evfds);

        while ((ev_fd = (int *)GetListElement(di->d_evfds)) != (int *)NULL) {
            if (proto_high_fd < *ev_fd) {
                proto_high_fd = *ev_fd;
            }
            FD_SET(*ev_fd, &proto_dispatch_fds);
        }
    }

    /*
    ** Main select event polling loop.
    */
    for (;;) {
        // from the select manpage, nfds is the highest-numbered file descriptor
        // in any of the three sets, plus 1.
        status =
            select(proto_high_fd + 1, &proto_dispatch_fds, NULL, NULL, NULL);

        switch (status) {
            case -1:
                switch (errno) {
                    case EINTR:
                        DPRINT("DbgCheckEvents: EINTR\n");
                        continue;

                    // We expect EINVAL if there are no fd's in the fd_set. The
                    // high_fd should be negative in this case and select will
                    // return with -1 and errno set to EINVAL.
                    case EINVAL:
                        DPRINT("DbgCheckEvents: EINVAL\n");
                        PrintOutError("No way to checkevents.\n");
                        return -1;

                    default:
                        PrintOutError("Select failed with %s.\n",
                                      strerror(errno));
                        return -1;
                }
                break;

            case 0:
                return 0;

            default:
                return 1;
        }
    }

    // We should never reach here...
    DPRINT("DbgCheckEvents: At final return\n");
    return 1;
}

static void DispatchProtoStartupEvent(dbginvoke_t *di) {
    int *ev_fd;

    if (di == NULL) return;

    // Ensure there is a protocol specific dispatch function
    if (DBG_PROTO_SW(di)->sw_dispatchevents != NULL) {
        // If there is an d_evfds list, then the protol layer is using event
        // notification fd's. We should try to check to see if each one is set,
        // and if so call the protocol specific dispatch function with the event
        // fd as an argument
        if (di->d_evfds != NULL) {
            SetList(di->d_evfds);

            while ((ev_fd = (int *)GetListElement(di->d_evfds)) !=
                   (int *)NULL) {
                if (FD_ISSET(*ev_fd, &proto_dispatch_fds)) {
                    // dispatch each of the set notification fds one at a time.
                    DBG_PROTO_SW(di)->sw_dispatchevents(di, *ev_fd);
                }
            }
        } else {
            // event notification fd's not in use by this protocol, so call
            // the generic dispatch function with -1 for event fd arg.
            DBG_PROTO_SW(di)->sw_dispatchevents(di, -1);
        }
    }
}

int WaitForProtoStartupEvent(dbginvoke_t *di) {
    for (;;) {
        switch (CheckForProtoStartupEvent(di)) {
            case -1:
                return -1;

            case 0:
                return 0;

            default:
                DispatchProtoStartupEvent(di);
                return 1;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

const std::string DebugClientProtocol::name = "Interface";

int DebugClientProtocol::init() {
    throw FunctionNotImplementedException("init");
}

void DebugClientProtocol::finish() {
    throw FunctionNotImplementedException("finish");
}

bitset* DebugClientProtocol::sendCommand(struct dbginvoke *, bitset *, const char *) {
    throw FunctionNotImplementedException("sendCommand");
}

struct proto_msg_set* DebugClientProtocol::receiveReply(struct dbginvoke *, bitset *,bitset *) {
    throw FunctionNotImplementedException("receiveReply");
}

int DebugClientProtocol::checkEvents(struct dbginvoke *) {
    throw FunctionNotImplementedException("checkEvents");
}

void DebugClientProtocol::dispatchEvents(struct dbginvoke *, int) {
    throw FunctionNotImplementedException("dispatchEvents");
}

proto_session *DebugClientProtocol::createSession(struct dbgclnt_proto *, WLM_type, const char *, const char *,const char *) {
    throw FunctionNotImplementedException("createSession");
}

void DebugClientProtocol::saveSession(proto_session *) {
    throw FunctionNotImplementedException("saveSession");
}

void DebugClientProtocol::cleanupSession(proto_session *) {
    throw FunctionNotImplementedException("cleanupSession");
}

int DebugClientProtocol::attach(struct dbginvoke *, proto_session *, const char *,
    const char *, const char *, int, int) {
    throw FunctionNotImplementedException("attach");
}
    
int DebugClientProtocol::launch(struct dbginvoke *, proto_session *, const char *, 
    char **, const char *, char **, const char *, const char *, const char*, 
    char **, const char *, int, int, int, bool) {
    throw FunctionNotImplementedException("launch");
}

int DebugClientProtocol::exit(struct dbginvoke *, int detach) {
    throw FunctionNotImplementedException("exit");
}

void DebugClientProtocol::destroyClient(struct dbginvoke *) {
    throw FunctionNotImplementedException("destroyClient");
}
