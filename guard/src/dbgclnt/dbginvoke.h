#ifndef _DBGCLNT_INVOKE_H
#define _DBGCLNT_INVOKE_H

#include "dbgclnt_proto.h"
#include "dbgclnt_debugfe.h"
#include "bitset.h"

// This is data used by the protocol layer for application output
// Usually this will be a g_progoutp_t *, but it depends on the proto.
typedef void *proto_outp;

// This is instance data used by the protocol layer that is allocated on a per
// dbginvoke basis. It is stored in the di since the protocol object is reused
// and is not suitable for storing instance data.
typedef void *proto_network;

// This is the protocol callback function prototype
typedef int (*proto_ev_callback_func)(struct dbginvoke *, proto_msg_set *);
// This is the client callback function prototype
// Arguments are the event_set for this event and a bitset containing the new
// ranks to report this event
typedef int (*clnt_ev_callback_func)(struct event_set *, bitset *);
// This is the proto program output callback function prototype
typedef void (*proto_outp_callback_func)(struct dbginvoke *, FILE *);

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
    bitset *d_bitset_full;  // represents all dbgsrvs
    bitset *d_bitset_launched;  // for invoke cmd: represents dbgsrvs that has
                                // been launched

    int d_proto_id;  // di specific protocol id - optionally used by certain
                     // protocols
    proto_network d_proto_net;  // di specific protocol network object
    proto_outp d_proto_outp;    // di specific protocol output object
    DebugClientProtocol * const d_proto;     // client protocol switch

    // The following two callbacks are for protocol event messages and for
    // handling the actual events in the client layer.
    proto_ev_callback_func d_proto_ev_callback;
    clnt_ev_callback_func d_clnt_ev_callback;
    // This is the callback for handling application output in the client
    proto_outp_callback_func d_proto_outp_callback;

    dbgfrontend_sw *d_frontend;  // debug frontend object

    // Extensions for client support
    // FIXME: These should be marked private once this is turned into a class
    list_t *d_eventset_list;  // pending events
    list_t *d_bpset_list;     // current breakpoints
    int d_global_bp_set_id;    // private to dbg_client
};
typedef struct dbginvoke dbginvoke_t;
#define DBG_PROTO_SW(p) (p)->d_proto

#endif // _DBGCLNT_INVOKE_H