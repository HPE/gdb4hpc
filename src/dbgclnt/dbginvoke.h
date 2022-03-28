/******************************************************************************\
 * Copyright 2022 Hewlett Packard Enterprise Development LP.
 ******************************************************************************/
#ifndef _DBGCLNT_INVOKE_H
#define _DBGCLNT_INVOKE_H

#include <list>
#include <functional>

#include "dbgclnt_breakpoints.h"
#include "dbgclnt_network.h"
#include "dbgclnt_debugfe.h"
#include "dbgoutp.h"
#include "bitset.h"

class DebugClientProtocol;

// This is the client callback function prototype
// Arguments are the event_set for this event and a bitset containing the new
// ranks to report this event
using clnt_ev_callback_func = std::function<int(struct event_set *, const Bitset *)>;

// Used to determine type of dbginvoke, currently there is either launched or
// attached launched means the application was launched by the debugger,
// attached means it was an existing application attached onto.
enum dbgproc_type { DBG_ATTACHED, DBG_LAUNCHED };
typedef enum dbgproc_type dbgproc_type;

struct dbginvoke {
    // zero everything except the const proto pointer
    dbginvoke(DebugClientProtocol * const proto);

    char *d_handle;       // invoke handle used as identifier in client layer
    uint64_t d_appid;     // app identifier - this should be generic
    dbgproc_type d_type;  // launched or attached?
    int d_started;        // indicates if this di has been started
    int d_isShutdown;  // Some networks can use this during error handling. If
                       // this is true, the network is already shutdown.
    int d_nproc;       // number of processes
    char *d_cmd;  // application command argument - set to name of executable 
                  // for invoke
    int d_stat;   // exit/signal status of proc
    list_t *d_evfds;        // event notification fds specific to this instance
    std::unique_ptr<Bitset> d_bitset_full;     // represents all dbgsrvs
    std::unique_ptr<Bitset> d_bitset_launched; // for invoke cmd: represents dbgsrvs that has been launched

    int d_proto_id;  // di specific protocol id - optionally used by certain
                     // protocols
    std::unique_ptr<DebugClientNetwork> d_proto_net;  // di specific protocol network object
    g_progoutp_t* d_proto_outp; // data used by the protocol layer for application output
    DebugClientProtocol * const d_proto;     // client protocol switch

    clnt_ev_callback_func d_clnt_ev_callback;

    dbgfrontend_sw *d_frontend;  // debug frontend object

    // Extensions for client support
    // FIXME: These should be marked private once this is turned into a class
    list_t *d_eventset_list;  // pending events
    BreakpointManager d_breakpoints; // logical breakpoints for this di
    CheckpointManager d_checkpoints; // logical checkpoints for this di
};
typedef struct dbginvoke dbginvoke_t;
#define DBG_PROTO_SW(p) (p)->d_proto

// for std::map<dbginvoke*, ..., dbginvokeMapper> etc.
struct dbginvokeMapper {
    // return true if l is "less than" r
    bool operator()(const dbginvoke* l, const dbginvoke* r) const {
        if (l == nullptr && r == nullptr) return false;

        // nullptrs considered "lowest"
        if (l == nullptr && r != nullptr) return true;
        if (l != nullptr && r == nullptr) return false;

        // handles are unique and never null
        return strcmp(l->d_handle, r->d_handle) < 0;
    }
};

#endif // _DBGCLNT_INVOKE_H