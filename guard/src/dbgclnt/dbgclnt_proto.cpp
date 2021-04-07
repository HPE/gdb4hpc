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

#include <vector>

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

#ifdef MRNET
#include "dbgclnt_mrn.h"
extern MRNClientProtocol DbgClntProtoMRN;
#endif

#ifdef ZMQNET
#include "dbgclnt_zmq.h"
extern ZMQNetClientProtocol DbgClntProtoZMQ;
#endif

#include "dbgclnt_proxy.h"
extern ProxyClientProtocol DbgClntProtoProxy;

static const std::vector<DebugClientProtocol *> DbgProtoTab = {
#ifdef MRNET
    &DbgClntProtoMRN,
#endif
#ifdef ZMQNET
    &DbgClntProtoZMQ,
#endif
    &DbgClntProtoProxy
};

// Used for the protocol wait for events function
static fd_set proto_dispatch_fds;
static int proto_high_fd;

// used to iterate through DbgProtoTab and call init or finish on each proto
// when gdb4hpc starts up or finishes
const std::vector<DebugClientProtocol *> getClientProtos() {
    return DbgProtoTab;
}

DebugClientProtocol * const findClientProtoByName(std::string protoName) {
    for (const auto &p : DbgProtoTab) {
        if (p != nullptr && p->name() == protoName) return p;
    }

    return nullptr;
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
     Note: This should be used for protocols that do not use an event
     notification fd. If a event notification fd can be used, then it should 
     be added to the sw_evfd list and sw_checkevents should simply return 0.
    */
    if (DBG_PROTO_SW(di)->checkEvents(di) > 0) {
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
                DBG_PROTO_SW(di)->dispatchEvents(di, *ev_fd);
            }
        }
    } else {
        // event notification fd's not in use by this protocol, so call
        // the generic dispatch function with -1 for event fd arg.
        DBG_PROTO_SW(di)->dispatchEvents(di, -1);
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

void DebugClientProtocol::outputCallback(dbginvoke *di, FILE *stream) {
    // by default, just forward to generic handler
    DbgDispatchProtoOutput(di, stream);
}

// public facing function to set the handler back to the default generic
void DebugClientProtocol::resetProtocolEventHandler() {
    setProtocolEventHandler(new DebugClientDefaultProtocolEventHandler());
}

void DebugClientProtocol::setProtocolEventHandler(DebugClientProtocolEventHandler *handler) {
    // since this is a unique pointer, this will delete any old handler.
    m_protoEventHandler.reset(handler);
}

int DebugClientProtocol::handleProtocolEvent(dbginvoke *di, proto_msg_set *set) { 
    if (m_protoEventHandler) return m_protoEventHandler->handleEvent(di, set);
    return 1;
};

int DebugClientDefaultProtocolEventHandler::handleEvent(dbginvoke *di, proto_msg_set* set) {
    // by default, just forward to generic handler
    return DbgDispatchProtoEvents(di, set);
}
