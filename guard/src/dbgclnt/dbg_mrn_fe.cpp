/*********************************************************************************\
 * dbg_mrn_fe.c
 *
 * Copyright 2010-2020 Cray Inc.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Cray Inc.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *
 *********************************************************************************/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "Communicator_C.h"
#include "MRNet_C.h"
#include "Network_C.h"
#include "Packet_C.h"
#include "Stream_C.h"
#include "Topology_C.h"
#include "alloc.h"
#include "bitset.h"
#include "dbg_client.h"
#include "dbg_mrn.h"
#include "dbg_mrn_fe.h"
#include "defs.h"
#include "list.h"
#include "mrn_topgen.h"
#include "smart_bitset.h"

struct multicast_type {
    bitset *target;
    MRNet_Communicator *comm;
    MRNet_Stream *stream;
};
typedef struct multicast_type multicast_type;

static multicast_type *new_multicast_type(MRNet_Communicator *comm,
                                          MRNet_Stream *stream,
                                          bitset *target) {
    multicast_type *mc_type = (multicast_type *)Alloc(sizeof(multicast_type));

    mc_type->target = bitset_dup(target);
    mc_type->comm = comm;
    mc_type->stream = stream;

    return mc_type;
}

static void free_multicast_type(multicast_type *mc) {
    bitset_free(mc->target);
    mc->target = NULL;
    mc->comm = NULL;
    mc->stream = NULL;

    Free(mc);
}
inline int ptr_to_int(void *p) { return (char *)p - (char *)0; }
inline void *int_to_ptr(int i) { return (char *)0 + i; }

static void mrn_add_event_stream(mrnet_type *mrnet, MRNet_Stream *event) {
    if (event) {
        int eventFd = stream_get_DataNotificationFd(event);
        AddToList(mrnet->event_streams, event);
        AddToList(mrnet->event_nfds, int_to_ptr(eventFd));
    }
}

static mrnet_type *new_mrnet_type(MRNet_Network *network,
                                  MRNet_Communicator *comm, MRNet_Stream *cmd,
                                  MRNet_Stream *event, MRNet_Stream *io,
                                  int num_be, int filter) {
    mrnet_type *mrnet = (mrnet_type *)Alloc(sizeof(mrnet_type));

    mrnet->network = network;
    mrnet->comm_bc = comm;
    mrnet->cmd_stream = cmd;
    mrnet->event_streams = NewList();
    mrnet->event_nfds = NewList();
    mrn_add_event_stream(mrnet, event);
    mrnet->io_stream = io;
    mrnet->io_nfd = -1;

    mrnet->filter_id = filter;
    mrnet->num_be = num_be;
    if (num_be > 0) {
        mrnet->bitset_full = bitset_new(num_be);
        bitset_invert(mrnet->bitset_full);
    } else {
        mrnet->bitset_full = NULL;
    }
    mrnet->mc_streams = NewList();

    return mrnet;
}

static void free_mrnet_type(mrnet_type *mrnet) {
    mrnet->network = NULL;
    mrnet->comm_bc = NULL;
    mrnet->cmd_stream = NULL;
    DestroyList(mrnet->event_streams, NULL);
    mrnet->io_stream = NULL;
    bitset_free(mrnet->bitset_full);
    if (mrnet->mc_streams != NULL)
        DestroyList(mrnet->mc_streams, (void (*)(void *))free_multicast_type);
    DestroyList(mrnet->event_nfds, NULL);

    Free(mrnet);
}

static void release_mrnet_resource(
    mrnet_type *mrnet) /*TODO: how about communicator ? */
{
    list_t *mc_streams = NULL;
    multicast_type *mc_type = NULL;

    if (mrnet->cmd_stream != NULL) stream_delete(mrnet->cmd_stream);

    SetList(mrnet->event_streams);
    while (MRNet_Stream *s =
               (MRNet_Stream *)GetListElement(mrnet->event_streams))
        stream_delete(s);

    if (mrnet->io_stream != NULL) stream_delete(mrnet->io_stream);

    if (mrnet->mc_streams != NULL) {
        mc_streams = mrnet->mc_streams;

        SetList(mc_streams);
        while ((mc_type = (multicast_type *)GetListElement(mc_streams)) !=
               (multicast_type *)NULL) {
            if (mc_type->stream != NULL) stream_delete(mc_type->stream);
        }
    }

    if (mrnet->network != NULL) network_delete(mrnet->network);
}

static MRNet_Stream *get_stream(mrnet_type *mrnet, DBG_MRNet_PROTOCOL proto,
                                bitset *target) {
    list_t *mc_streams = NULL;
    multicast_type *mc_type = NULL;
    MRNet_Stream *stream = NULL;

    if (mrnet == NULL) return NULL;

    switch (proto) {
        case DBG_MRNet_PRO_COMMAND:
            if (target == NULL ||
                bitset_eq(mrnet->bitset_full, target))  // is it a broadcast?
            {
                stream = mrnet->cmd_stream;
            } else {
                mc_streams = mrnet->mc_streams;
                // look for multicast stream
                SetList(mc_streams);
                while ((mc_type = (multicast_type *)GetListElement(
                            mc_streams)) != (multicast_type *)NULL) {
                    if (bitset_eq(mc_type->target, target)) break;
                }

                if (mc_type != NULL)  // found multicast stream
                {
                    stream = mc_type->stream;
                }
            }
            break;
        case DBG_MRNet_PRO_EVENT:
            stream = (MRNet_Stream *)(EmptyList(mrnet->event_streams)
                                          ? NULL
                                          : GetListNumElement(
                                                mrnet->event_streams, 0));
            break;
        case DBG_MRNet_PRO_IO:
            stream = mrnet->io_stream;
            break;
        default:
            PrintOutError("ERROR: get_stream invalid proto[%d]\n", proto);
    }

    return stream;
}

static MRNet_Stream *create_stream(mrnet_type *mrnet, bitset *target,
                                   MRNet_Stream **event_stream) {
    if (mrnet == NULL || target == NULL) {
        *event_stream = NULL;
        return NULL;
    }

    MRNet_Communicator *comm =
        network_create_communicator(mrnet->network, target, ATTACHBE_RANK_SKEW);

    // Create the broadcast stream
    MRNet_Stream *stream = network_create_stream_syncfilter(
        mrnet->network, comm, mrnet->filter_id);

    multicast_type *mc_type = new_multicast_type(comm, stream, target);
    AddToList(mrnet->mc_streams, mc_type);

    // Create the event stream for handling asynchronous output from the
    // debugger
    int filter_timeout = get_mrn_filter_timeout();
    *event_stream = network_create_stream_timefilter(
        mrnet->network, comm, mrnet->filter_id, filter_timeout);

    mrn_add_event_stream(mrnet, *event_stream);

    return stream;
}

/*------------------------------------------ ---------------------*/
/*----------------------APIs start from here----------------------*/
/*------------------------------------------- --------------------*/

/*
** initialize frontend and launch backend program
** Currently not called by Cray
*/
mrnet_type *mrn_fe_launch_with_filter(const char *topology,
                                      const char *filter_file,
                                      const char *filter_name,
                                      const char *backend_exe,
                                      char *backend_args[]) {
    MRNet_Network *network =
        network_create_FE(topology, backend_exe, backend_args);

    if (network == NULL) return NULL;

    if (network_has_error(network)) {
        network_print_error(network, "MRNet Network creation failed!\n");
        return NULL;
    }

    int filter_id = network_load_filter_func(network, filter_file, filter_name);
    if (filter_id == -1) {
        PrintOutError(
            "ERROR: mrn_fe_launch_with_filter network_load_filter_func() "
            "failed!\n");
        network_delete(network);
        return NULL;
    }

    MRNet_Communicator *comm_bc = network_get_broadcast_communicator(network);
    MRNet_Stream *stream_bc =
        network_create_stream_syncfilter(network, comm_bc, filter_id);
    int num_be = communicator_get_endpoints_size(comm_bc);

    MRNet_Stream *stream_io =
        network_create_stream_nofid_nowait(network, comm_bc);

    int filter_timeout = get_mrn_filter_timeout();
    MRNet_Stream *stream_event = network_create_stream_timefilter(
        network, comm_bc, filter_id, filter_timeout);

    mrnet_type *mrnet =
        new_mrnet_type(network, comm_bc, stream_bc, stream_event, stream_io,
                       num_be, filter_id);

    DPRINT(
        "mrn_fe_launch_with_filter network[%p] bc_comm[%p] bc_stream[%p] "
        "num_be[%d]!\n",
        network, comm_bc, stream_bc, num_be);

    network_set_failure_recovery(network, 0);

    return mrnet;
}

/*
** initialize frontend and launch backend program without filter
** Currently not called by Cray
*/
mrnet_type *mrn_fe_launch(char *topology, char *backend_exe,
                          char *backend_args[]) {
    MRNet_Network *network =
        network_create_FE(topology, backend_exe, backend_args);

    if (network == NULL) return NULL;

    if (network_has_error(network)) {
        network_print_error(network, "Network creation failed!\n");
        return NULL;
    }

    MRNet_Communicator *comm_bc = network_get_broadcast_communicator(network);
    MRNet_Stream *stream_bc =
        network_create_stream_nofid_nowait(network, comm_bc);
    int num_be = communicator_get_endpoints_size(comm_bc);

    MRNet_Stream *stream_io =
        network_create_stream_nofid_nowait(network, comm_bc);
    MRNet_Stream *stream_event =
        network_create_stream_nofid_nowait(network, comm_bc);

    mrnet_type *mrnet = new_mrnet_type(network, comm_bc, stream_bc,
                                       stream_event, stream_io, num_be, -1);

    DPRINT(
        "mrn_initialize_fe network[%p] comm_bc[%p] bc_stream[%p] num_be[%d] "
        "mc_streams[%p]!\n",
        network, comm_bc, stream_bc, num_be, mrnet->mc_streams);

    network_set_failure_recovery(network, 0);

    return mrnet;
}

/*
** initialize frontend for attaching backend programs
** Main way of launching/attaching on a Cray
*/
#ifdef ALPS
mrnet_type *mrn_fe_create_network_no_be(char *topology_file,
                                        MRNet_Topology **p_topology,
                                        int ctiAppid, int ctiMid) {
    MRNet_Network *network = NULL;
    int rtn;

    if (topology_file == NULL) {
        PrintOutError(
            "ERROR: mrn_fe_create_network_no_be topology_file is NULL!\n");
        return NULL;
    }

    network = network_create_FE_with_cti(topology_file, ctiAppid, ctiMid);

    if (network_has_error(network)) {
        PrintOutError(
            "ERROR: mrn_fe_create_network_no_be failed to create MRNet "
            "network!\n");
        return NULL;
    }

    // enable network event callbacks
    rtn = network_register_TopAddBE_event_callback(network);

    if (rtn) {
        PrintOutError(
            "ERROR: mrn_fe_create_network_no_be failed to enable event "
            "callbacks!\n");
        network_delete(network);
        return NULL;
    }

    MRNet_Topology *topology = network_get_topology(network);

    if (topology == NULL) {
        PrintOutError(
            "ERROR: mrn_fe_create_network_no_be cannot get topology for "
            "network[%p]\n",
            network);
        network_delete(network);
        return NULL;
    }

    *p_topology = topology;

    network_set_failure_recovery(network, 0);

    mrnet_type *mrnet = new_mrnet_type(network, NULL, NULL, NULL, NULL, 0, -1);

    return mrnet;
}
#else
mrnet_type *mrn_fe_create_network_no_be(char *topology_file,
                                        MRNet_Topology **p_topology) {
    MRNet_Network *network = NULL;
    int rtn;

    if (topology_file == NULL) {
        PrintOutError(
            "ERROR: mrn_fe_create_network_no_be topology_file is NULL!\n");
        return NULL;
    }

    network = network_create_FE(topology_file, NULL, NULL);

    if (network_has_error(network)) {
        network_print_error(network,
                            "ERROR: mrn_fe_create_network_no_be failed to "
                            "create MRNet network!\n");
        return NULL;
    }

    // enable network event callbacks
    rtn = network_register_TopAddBE_event_callback(network);

    if (rtn) {
        network_print_error(network,
                            "ERROR: mrn_fe_create_network_no_be failed to "
                            "enable event callbacks!\n");
        network_delete(network);
        return NULL;
    }

    MRNet_Topology *topology = network_get_topology(network);

    if (topology == NULL) {
        PrintOutError(
            "ERROR: mrn_fe_create_network_no_be cannot get topology for "
            "network[%p]\n",
            network);
        network_delete(network);
        return NULL;
    }

    *p_topology = topology;

    network_set_failure_recovery(network, 0);

    mrnet_type *mrnet = new_mrnet_type(network, NULL, NULL, NULL, NULL, 0, -1);

    return mrnet;
}
#endif

/*
** wait for attaching backend programs
*/
int mrn_fe_attach_be(mrnet_type *mrnet, int num_be, int timeout) {
    int num_callbacks = 0;
    int last_num_callbacks = 0;
    int expire = 0;
    struct timespec timeslice;

    if (mrnet == NULL) return -1;

    timeslice.tv_sec = 0;
    timeslice.tv_nsec = 50000000;

    PrintOutMsg(
        "Waiting for debug servers to attach to MRNet communications "
        "network...\n");
    PrintOutMsg(
        "Timeout in %d seconds. Please wait for the attach to complete.\n",
        timeout);

    do {
        nanosleep(&timeslice, (struct timespec *)NULL);

        num_callbacks = network_get_num_callbacks();
        if (num_callbacks == last_num_callbacks) {
            expire++;
        } else {
            expire = 0;
            last_num_callbacks = num_callbacks;
            DPRINT("INFO: mrn_fe_attach_be number of attached dbgsrvs: [%d]\n",
                   num_callbacks);
        }

        if (expire >= (timeout * 20)) {
            PrintOutError("ERROR: mrn_fe_attach_be timeout!\n");
            return 1;
        }

        if (expire % 20 == 0) {
            PrintOutMsg(
                "Number of dbgsrvs connected: [%d];  Timeout Counter: [%d]\n",
                num_callbacks, expire / 20);
        }

    } while (num_callbacks != num_be);

    DPRINT("INFO: mrn_network_wait_for_attaching all dbgsrv have attached!\n");

    // remove network event callbacks
    network_remove_TopAddBE_event_callback(mrnet->network);

    return 0;
}

/*
 * load filters and completes the creation of MRNet for attaching backend
 * programs This is how streams are created on a Cray
 */
int mrn_fe_create_streams_and_load_filter(mrnet_type *mrnet,
                                          const char *filter_file,
                                          const char *filter_name) {
    if (mrnet == NULL) return 1;

    DPRINT("INFO: network_load_filter_func filter_file: %s filter_name: %s\n",
           filter_file, filter_name);
    int filter_id =
        network_load_filter_func(mrnet->network, filter_file, filter_name);
    if (filter_id == -1) {
        PrintOutError(
            "mrn_fe_create_streams_and_load_filter "
            "mrn_network_load_filter_and_complete_creation() failed!\n");
        network_delete(mrnet->network);
        return 1;
    }

    MRNet_Communicator *comm_bc =
        network_get_broadcast_communicator(mrnet->network);
    MRNet_Stream *stream_bc =
        network_create_stream_syncfilter(mrnet->network, comm_bc, filter_id);
    int num_be = communicator_get_endpoints_size(comm_bc);

    MRNet_Stream *stream_io =
        network_create_stream_nofid_nowait(mrnet->network, comm_bc);

    int filter_timeout = get_mrn_filter_timeout();
    MRNet_Stream *stream_event = network_create_stream_timefilter(
        mrnet->network, comm_bc, filter_id, filter_timeout);

    // set the mrnet members
    mrnet->comm_bc = comm_bc;
    mrnet->cmd_stream = stream_bc;
    mrn_add_event_stream(mrnet, stream_event);
    mrnet->io_stream = stream_io;
    mrnet->num_be = num_be;
    mrnet->bitset_full = bitset_new(num_be);
    bitset_invert(mrnet->bitset_full);
    mrnet->filter_id = filter_id;

    return 0;
}

/*
 * completes the creation of MRNet for attaching backend programs without
 * filters Currently not called by Cray
 */
int mrn_fe_create_streams(mrnet_type *mrnet) {
    if (mrnet == NULL) return 1;

    MRNet_Communicator *comm_bc =
        network_get_broadcast_communicator(mrnet->network);
    MRNet_Stream *stream_bc =
        network_create_stream_nofid_nowait(mrnet->network, comm_bc);
    int num_be = communicator_get_endpoints_size(comm_bc);

    MRNet_Stream *stream_io =
        network_create_stream_nofid_nowait(mrnet->network, comm_bc);
    MRNet_Stream *stream_event =
        network_create_stream_nofid_nowait(mrnet->network, comm_bc);

    // set the mrnet members
    mrnet->comm_bc = comm_bc;
    mrnet->cmd_stream = stream_bc;
    mrn_add_event_stream(mrnet, stream_event);
    mrnet->io_stream = stream_io;
    mrnet->num_be = num_be;
    mrnet->bitset_full = bitset_new(num_be);
    bitset_invert(mrnet->bitset_full);

    return 0;
}

int mrn_fe_get_num_of_be(mrnet_type *mrnet) {
    if (mrnet == NULL) {
        return 0;
    }

    return mrnet->num_be;
}

int mrn_fe_setup_streams(mrnet_type *mrnet, char *msg) {
    MRNet_Stream *stream = NULL;

    if (mrnet == NULL || msg == NULL) return -1;

    // initialize commands stream
    stream = get_stream(mrnet, DBG_MRNet_PRO_COMMAND, NULL);
    if (stream == NULL) {
        PrintOutError(
            "ERROR: mrn_fe_setup_streams cannot find command stream!\n");
        return -1;
    }

    if (stream_send(stream, DBG_MRNet_PRO_COMMAND, "%s", msg) == -1) {
        PrintOutError("ERROR: mrn_fe_setup_streams stream_send() failed!\n");
        return -1;
    }

    if (stream_flush(stream) == -1) {
        PrintOutError("ERROR: mrn_fe_setup_streams stream_flush() failed!\n");
        return -1;
    }
    DPRINT("mrn_fe_setup_streams has set the command stream. tag[%d]\n",
           DBG_MRNet_PRO_COMMAND);

    // initialize io stream
    stream = get_stream(mrnet, DBG_MRNet_PRO_IO, NULL);
    if (stream == NULL) {
        PrintOutError("ERROR: mrn_fe_setup_streams cannot find io stream!\n");
        return -1;
    }

    if (stream_send(stream, DBG_MRNet_PRO_IO, "%s", msg) == -1) {
        PrintOutError("ERROR: mrn_fe_setup_streams stream_send() failed!\n");
        return -1;
    }

    if (stream_flush(stream) == -1) {
        PrintOutError("ERROR: mrn_fe_setup_streams stream_flush() failed!\n");
        return -1;
    }
    DPRINT("mrn_fe_setup_streams has set io stream. tag[%d]\n",
           DBG_MRNet_PRO_IO);

    // initialize event stream
    stream = get_stream(mrnet, DBG_MRNet_PRO_EVENT, NULL);
    if (stream == NULL) {
        PrintOutError(
            "ERROR: mrn_fe_setup_streams cannot find event stream!\n");
        return -1;
    }

    if (stream_send(stream, DBG_MRNet_PRO_EVENT, "%s", msg) == -1) {
        PrintOutError("ERROR: mrn_fe_setup_streams stream_send() failed!\n");
        return -1;
    }

    if (stream_flush(stream) == -1) {
        PrintOutError("ERROR: mrn_fe_setup_streams stream_flush() failed!\n");
        return -1;
    }
    DPRINT("mrn_fe_setup_streams has set event stream. tag[%d]\n",
           DBG_MRNet_PRO_EVENT);

    return 0;
}

int mrn_fe_send_command(mrnet_type *mrnet, bitset *target, const char *msg,
                        MRNet_Stream **event_stream_ptr) {
    if (mrnet == NULL) return -1;

    MRNet_Stream *stream = NULL;

    stream = get_stream(mrnet, DBG_MRNet_PRO_COMMAND, target);
    if (stream == NULL) {
        MRNet_Stream *event_stream = NULL;
        stream = create_stream(mrnet, target, &event_stream);
        *event_stream_ptr = event_stream;

        if (stream == NULL) {
            PrintOutError(
                "ERROR: mrn_fe_send_command_to_group cannot create stream for "
                "network target[%s]\n",
                bitset_to_set(target));
            return -1;
        }

        if (event_stream != NULL) {
            // Send an initializer command along the command stream to register
            // it's matching event stream.
            char *linkMsg = CommandToSocProto(DBGCMD_SETCOMMPARAM, "%d %u",
                                              MRNCMD_SETEVENTSTREAM,
                                              stream_get_Id(event_stream));
            if (stream_send(stream, DBG_MRNet_PRO_COMMAND, "%s", linkMsg) ==
                -1) {
                PrintOutError(
                    "Could not connect event stream to command stream");
            }
            Free(linkMsg);
        }
    }

    if (stream_send(stream, DBG_MRNet_PRO_COMMAND, "%s", msg) == -1) {
        PrintOutError(
            "ERROR: mrn_fe_send_command_to_group stream_send() failed!\n");
        return -1;
    }

    DPRINT("[INFO]: mrn_fe_send_command_to_group via stream for %s\n",
           bitset_to_set(target));

    if (stream_flush(stream) == -1) {
        PrintOutError(
            "ERROR: mrn_fe_send_command_to_group stream_flush() failed!\n");
        return -1;
    }

    return 0;
}

proto_msg_set *mrn_fe_recv_reply(mrnet_type *mrnet, bitset *target) {
    MRNet_Stream *cmd_stream = NULL;
    int tag, retval;
    MRNet_Packet *packet = NULL;
    char **strArr;
    int strLen, msgNum, index;
    smart_bitset *sb;
    bitset *bs;
    proto_msg_set *result;

    if ((cmd_stream = get_stream(mrnet, DBG_MRNet_PRO_COMMAND, target)) ==
        NULL) {
        PrintOutError(
            "ERROR: mrn_fe_recv_reply cannot find command stream for %s!\n",
            bitset_to_set(target));
        return NULL;
    }

    retval = stream_recv(cmd_stream, &tag, &packet, MRNET_BLOCKING);

    if (retval <= 0) {
        PrintOutError(
            "ERROR: mrn_fe_recv_reply failed to receive replies! retval[%d]\n",
            retval);
        return NULL;
    }

    if (packet != NULL &&
        packet_unpack(packet, "%as", &strArr, &strLen) == -1) {
        PrintOutError("ERROR: mrn_fe_recv_reply packet_unpack() failed!\n");
        return NULL;
    }

    msgNum = strLen / 2;
    if (msgNum <= 0 || strLen % 2 != 0) {
        PrintOutError(
            "ERROR: mrn_fe_recv_reply receivd an invalid number of message: "
            "strLen[%d]\n",
            strLen);
        return NULL;
    }

    // Create the return type
    result = NewProtoMsgSet();

    for (index = 0; index < msgNum; index++) {
        sb = smart_bitset_deserialize(strArr[index * 2 + 1]);
        smart_bitset_convert(sb);
        bs = bitset_dup(sb->sb_dense);

        smart_bitset_free(sb);

        // Create a proto_message for this packet and add it to the result
        AddProtoMsgSet(result, NewProtoMsg(bs, strArr[index * 2]));

        // Cleanup the strArr for the bitset only, the message gets passed up
        // and cleaned up later.
        free(strArr[index * 2 + 1]);
    }

    // free the final strArr allocated by packet_unpack
    free(strArr);

    // delete the packet
    packet_delete(packet);

    // reset the message set
    ResetProtoMsgSet(result);

    return result;
}

int mrn_fe_get_io_notificationfd(mrnet_type *mrnet) {
    MRNet_Stream *io_stream = NULL;

    if (mrnet == NULL) return -1;

    io_stream = get_stream(mrnet, DBG_MRNet_PRO_IO, NULL);

    // set the notification fd for the mrnet_type
    mrnet->io_nfd = stream_get_DataNotificationFd(io_stream);

    return mrnet->io_nfd;
}

int mrn_fe_get_event_notificationfd(mrnet_type *mrnet) {
    return ptr_to_int(GetListNumElement(mrnet->event_nfds, 0));
}

proto_msg_set *mrn_fe_dispatch_io(mrnet_type *mrnet, int notification_fd) {
    MRNet_Stream *io_stream = NULL;
    MRNet_Packet *packet = NULL;
    int tag, retval;
    char *rank = NULL;
    char *msg = NULL;
    bitset *source;
    proto_msg_set *result = NULL;

    if (mrnet == NULL) return NULL;

    if (mrnet->io_nfd == notification_fd) {
        DPRINT("mrn_fe_check_io_and_event: handle IO_MESSAGE.\n");

        io_stream = get_stream(mrnet, DBG_MRNet_PRO_IO, NULL);

        // clear the io_stream notification fd
        stream_clear_DataNotificationFd(io_stream);

        // Try to recv as many packets as possible
        do {
            retval = stream_recv(io_stream, &tag, &packet, MRNET_UNBLOCKING);

            if (retval > 0 && tag == DBG_MRNet_PRO_IO) {
                if (packet != NULL &&
                    packet_unpack(packet, "%s %s", &rank, &msg) == -1) {
                    PrintOutError("ERROR: packet_unpack() failure\n");
                    // reset the message set
                    ResetProtoMsgSet(result);
                    return result;
                }

                // Create the return type if it hasn't been created already
                if (result == NULL) {
                    result = NewProtoMsgSet();
                }

                // Create a mrnet_message for this packet and add it to the
                // result
                if (rank != NULL) {
                    source = str_to_bitset(rank, NULL);
                } else {
                    source = NULL;
                }

                AddProtoMsgSet(result, NewProtoMsg(source, msg));

#ifdef DEBUG
                if (msg != NULL)
                    DPRINT(
                        "INFO: mrn_fe_check_io_and_event() received io: %s "
                        "from [%s]!\n",
                        msg, rank);
                else
                    DPRINT(
                        "INFO: mrn_fe_check_io_and_event() received NULL "
                        "io!\n");
#endif
                // delete the packet
                packet_delete(packet);

            } else if (retval < 0) {
                PrintOutError(
                    "ERROR: mrn_fe_check_io_and_event() received an invalid "
                    "tag[%d] for io!\n",
                    tag);
                // reset the message set
                ResetProtoMsgSet(result);
                return result;
            }
        } while (retval > 0);
    }

    // reset the message set
    ResetProtoMsgSet(result);

    return result;
}

proto_msg_set *mrn_fe_dispatch_event(mrnet_type *mrnet, int notification_fd) {
    MRNet_Stream *event_stream = NULL;
    MRNet_Packet *packet = NULL;
    int tag, retval;
    char **strArr;
    int strLen, msgNum, index;
    smart_bitset *sb = NULL;
    bitset *bs = NULL;
    proto_msg_set *result = NULL;

    if (mrnet == NULL) return NULL;

    void *notification_fdptr = int_to_ptr(notification_fd);
    if (InList(mrnet->event_nfds, notification_fdptr)) {
        DPRINT("mrn_fe_check_io_and_event: handle EVENT_MESSAGE.\n");

        // find the event_stream corresponding to the event_fd
        SetList(mrnet->event_nfds);
        SetList(mrnet->event_streams);
        while ((event_stream =
                    (MRNet_Stream *)GetListElement(mrnet->event_streams)),
               (GetListElement(mrnet->event_nfds) != notification_fdptr)) {
        }

        // clear the event_stream notification fd
        stream_clear_DataNotificationFd(event_stream);

        // Try to recv as many packets as possible
        do {
            retval = stream_recv(event_stream, &tag, &packet, MRNET_UNBLOCKING);

            if (retval > 0 && tag == DBG_MRNet_PRO_EVENT) {
                if (packet != NULL &&
                    packet_unpack(packet, "%as", &strArr, &strLen) == -1) {
                    PrintOutError(
                        "ERROR: mrn_fe_check_io_and_event packet_unpack() "
                        "failed!\n");
                    // reset the message set
                    ResetProtoMsgSet(result);
                    return result;
                }

                DPRINT(
                    "mrn_fe_check_io_and_event() received [%d] event "
                    "messages.\n",
                    strLen);

                if (((msgNum = strLen / 2) <= 0) || (strLen % 2 != 0)) {
                    PrintOutError(
                        "ERROR: mrn_fe_check_io_and_event received incorrect "
                        "number of message: strLen[%d]\n",
                        strLen);
                    // reset the message set
                    ResetProtoMsgSet(result);
                    return result;
                }

                // Create the return type if it hasn't been created already
                if (result == NULL) {
                    result = NewProtoMsgSet();
                }

                for (index = 0; index < msgNum; index++) {
                    sb = smart_bitset_deserialize(strArr[index * 2 + 1]);
                    smart_bitset_convert(sb);
                    bs = bitset_dup(sb->sb_dense);

                    smart_bitset_free(sb);
                    sb = NULL;

                    // Create a proto_message for this packet and add it to the
                    // result
                    AddProtoMsgSet(result, NewProtoMsg(bs, strArr[index * 2]));

                    // Cleanup the strArr for the bitset only, the message gets
                    // passed up and cleaned up later.
                    free(strArr[index * 2 + 1]);
                }

                // Cleanup the strArr
                free(strArr);

                // delete the packet
                packet_delete(packet);

            } else if (retval < 0) {
                PrintOutError(
                    "ERROR: mrn_fe_check_io_and_event() invalid tag[%d] for "
                    "event!\n",
                    tag);
                // reset the message set
                ResetProtoMsgSet(result);
                return result;
            }
        } while (retval > 0);
    }

    // reset the message set
    ResetProtoMsgSet(result);

    return result;
}

void mrn_fe_delete_network(mrnet_type *mrnet) {
    if (mrnet == NULL) return;

    release_mrnet_resource(mrnet);
    free_mrnet_type(mrnet);
}

static void mrnet_fe_failure_callback(void *cb_data) {
    dbginvoke_t *di = (dbginvoke_t *)cb_data;

    PrintOutError("\nERROR: MRNet failure callback invoked.\n");
    PrintOutError("Debugger is shutting down! Please file a bugreport.\n");
    PrintOutError(
        "Re-run with setting CRAY_DBG_LOG_DIR to a cross mounted location\n");
    PrintOutError(
        "and adding --debug to your launch/attach statement. Submit the\n");
    PrintOutError("generated logs with the bugreport.\n\n");

    di->d_isShutdown = 1;

    // Force ccdb to shutdown
    ForceQuit();
}

void mrn_fe_enable_failure_callback(mrnet_type *mrnet, dbginvoke_t *di) {
    if (mrnet == NULL) return;

    network_register_Failure_event_callback(
        mrnet->network, (void (*)())mrnet_fe_failure_callback, (void *)di);
}

static void display_leaves_for_network(MRNet_Network *network) {
    MRNet_Topology *topology;
    MRNet_Node_t *nodes;

    topology = network_get_topology(network);
    if (topology == NULL) {
        PrintOutError(
            "ERROR: display_leaves_for_network cannot get topology for "
            "network.\n");
        return;
    }

    int num_leaves = topology_get_leaves(topology, &nodes);

    if (num_leaves <= 0) {
        PrintOutError(
            "ERROR: display_leaves_for_network network has no leaves.\n");
        return;
    }

    PrintOutMsg("Leaf node info for network.\n");
    int index;
    for (index = 0; index < num_leaves; ++index) {
        PrintOutMsg("\tcurrent_node[%d] host[%s] port[%d] rank[%d]\n", index,
                    nodes[index].hostname, nodes[index].port,
                    nodes[index].rank);
    }
}

void mrn_fe_debug_print_network_topology(mrnet_type *mrnet) {
    if (mrnet == NULL) return;

    display_leaves_for_network(mrnet->network);
}
