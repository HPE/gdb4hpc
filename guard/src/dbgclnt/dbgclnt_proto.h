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

// forward declare struct dbginvoke - Note that because the struct dbginvoke is
// first encountered inside of a list of parameters for the function pointer, we
// *must* forward declare it otherwise the compiler will mess up.
struct dbginvoke;

// Used to keep track of sessions associated with the proxy based dbgclnt_proto
// implementation
struct proto_session {
    struct dbgclnt_proto *sess_proto;
    void *sess_obj;  // This is data used by the protocol layer
                     // that is allocated on a per session basis.
};
typedef struct proto_session proto_session;

// Some protocols will interact with the workload manager. Known workload
// managers are defined in the following enumeration. Note that the name
// cooresponds to the job submission command used by the WLM.
enum WLM_type { WLM_none, WLM_qsub, WLM_sbatch };
typedef enum WLM_type WLM_type;

// main protocol definition
struct dbgclnt_proto {
    const char *sw_name; /* Name of the protocol */

    int (*sw_init)(void);    /* protocol init function */
    void (*sw_finish)(void); /* protocol finish function */

    bitset *(*sw_send_cmd)(struct dbginvoke *, bitset *,
                           const char *); /* protocol send command function */
    struct proto_msg_set *(*sw_recv_reply)(
        struct dbginvoke *, bitset *,
        bitset *); /* protocol receive reply function */
    int (*sw_checkevents)(
        struct dbginvoke *); /* protocol check for ready event function */
    void (*sw_dispatchevents)(struct dbginvoke *,
                              int); /* protocol event dispatch function */

    /*
    ** Client protocol routines.
    */
    proto_session *(*sw_create_session)(
        struct dbgclnt_proto *, WLM_type, const char *, const char *,
        const char *); /* protocol create session function */
    void (*sw_save_session)(proto_session *);
    void (*sw_cleanup_session)(
        proto_session *); /* protocol cleanup session function - called when a
                             session is no longer valid */
    int (*sw_attach)(struct dbginvoke *, proto_session *, const char *,
                     const char *, const char *, int,
                     int); /* protocol attach function */
    int (*sw_launch)(struct dbginvoke *, proto_session *, const char *, char **,
                     const char *, char **, const char *, const char *, char **,
                     const char *, int, int,
                     int, bool); /* protocol launch function */
    int (*sw_exit)(struct dbginvoke *, int detach); /* protocol exit function */
    void (*sw_destroyclnt)(
        struct dbginvoke *); /* protocol destroy dbginvoke specific instance */
};
typedef struct dbgclnt_proto dbgclnt_proto;

/*
** Function definitions
*/
extern void SetClntProto(void);
extern dbgclnt_proto *GetClntProto(void);
extern dbgclnt_proto *FindClntProto(const char *);

extern int WaitForProtoStartupEvent(struct dbginvoke *);

#endif /* !_DBGCLNT_PROTO_H */
