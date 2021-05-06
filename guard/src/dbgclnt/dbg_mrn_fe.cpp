/*********************************************************************************\
 * dbg_mrn_fe.cpp
 *
 * Copyright 2010-2021 Hewlett Packard Enterprise Development LP.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Hewlett
 * Packard Enterprise Development LP., no part of this work or its content may
 * be used, reproduced or disclosed in any form.
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

#include "alloc.h"
#include "bitset.h"
#include "dbg_client.h"
#include "dbg_mrn.h"
#include "dbg_mrn_fe.h"
#include "defs.h"
#include "list.h"
#include "mrn_topgen.h"
#include "mrnet/Communicator.h"
#include "mrnet/Event.h"
#include "mrnet/FilterIds.h"
#include "mrnet/Network.h"
#include "mrnet/NetworkTopology.h"
#include "mrnet/Packet.h"
#include "mrnet/Stream.h"
#include "smart_bitset.h"

struct MulticastType {
    MulticastType(MRN::Communicator *comm, MRN::Stream *stream, bitset *target);
    ~MulticastType();

    bitset *target;
    MRN::Communicator *comm;
    MRN::Stream *stream;
};

struct CallbackHandlerInfo {
    int num_callbacks;
    pthread_mutex_t callback_lock;
    pthread_mutexattr_t callback_lock_attr;
};

struct FailureHandlerInfo {
    void (*failure_func)(void *);
    pthread_mutex_t failure_lock;
    pthread_mutexattr_t failure_lock_attr;
};

static CallbackHandlerInfo callback_handler;
static FailureHandlerInfo *failure_handler = nullptr;

static int get_num_callbacks() {
    int rtn;
    // grab the lock, read the value, then release
    // we had a data contention here before
    pthread_mutex_lock(&callback_handler.callback_lock);
    rtn = callback_handler.num_callbacks;
    pthread_mutex_unlock(&callback_handler.callback_lock);

    return rtn;
}

MulticastType::MulticastType(MRN::Communicator *comm, MRN::Stream *stream,
                             bitset *target)
    : comm{comm}, stream{stream} 
{
    target = bitset_dup(target);
}

MulticastType::~MulticastType() { bitset_free(target); }

static inline int ptr_to_int(void *p) { return (char *)p - (char *)0; }
static inline void *int_to_ptr(int i) { return (char *)0 + i; }

void MRNetFrontEnd::add_event_stream(MRN::Stream *event_stream) {
    if (event_stream != nullptr) {
        int eventFd = event_stream->get_DataNotificationFd();
        AddToList(m_event_streams, event_stream);
    AddToList(m_event_nfds, int_to_ptr(eventFd));
    }
}

MRNetFrontEnd::MRNetFrontEnd(MRN::Network *network, MRN::Stream *cmd, MRN::Stream *event,
             MRN::Stream *io, int num_be, int filter)
    : m_network{network},
      m_cmd_stream{cmd},
      m_io_stream{io},
      m_event_streams{NewList()},
      m_mc_streams{NewList()},
      m_event_nfds{NewList()},
      m_io_nfd{-1},
      m_filter_id{filter},
      m_num_be{num_be}
{
    add_event_stream(event);
    if (num_be > 0) {
        m_bitset_full = bitset_new(num_be);
        bitset_invert(m_bitset_full);
    } else {
        m_bitset_full = nullptr;
    }
}

MRNetFrontEnd::~MRNetFrontEnd() {
    delete m_cmd_stream;
    delete m_io_stream;

    // DestroyList is safe to call with nullptr, no need to check if
    // m_event_streams is null
    DestroyList(m_event_streams, [](void *stream) {
        delete static_cast<MRN::Stream *>(stream);
    });

    DestroyList(m_mc_streams, [](void *voidMulti) {
        auto *multi = static_cast<MulticastType *>(voidMulti);
        if (multi != nullptr)
            delete multi->stream;
        delete multi;
    });

    // for this list we are cheeky and store the fds directly as void
    // pointers and cast them to ints to use them. so there is no need to pass a
    // deletion function here. see the ptr_to_int and int_to_ptr functions.
    DestroyList(m_event_nfds, nullptr);

    bitset_free(m_bitset_full);
    
    delete m_network; // needs to be deleted last
}

MRN::Stream *MRNetFrontEnd::get_stream(DBG_MRNet_PROTOCOL proto, bitset *target) const {
    MRN::Stream *stream = nullptr;

    switch (proto) {
        case DBG_MRNet_PRO_COMMAND:
            // is it a broadcast?
            if (target == nullptr || bitset_eq(m_bitset_full, target)) {
                stream = m_cmd_stream;
            } else {
                // look for multicast stream
                SetList(m_mc_streams);
                while (auto *mc_type = static_cast<MulticastType *>(GetListElement(m_mc_streams))) {
                    if (bitset_eq(mc_type->target, target)) {
                        // found multicast stream
                        stream = mc_type->stream;
                        break;
                    }
                }
            }
            break;
        case DBG_MRNet_PRO_EVENT:
            stream = static_cast<MRN::Stream *>(
                EmptyList(m_event_streams)
                    ? nullptr
                    : GetListNumElement(m_event_streams, 0));
            break;
        case DBG_MRNet_PRO_IO:
            stream = m_io_stream;
            break;
        default:
            PrintOutError("ERROR: get_stream invalid proto[%d]\n", proto);
    }

    return stream;
}

MRN::Stream *MRNetFrontEnd::create_stream(bitset *target, MRN::Stream *&event_stream) {
    if (target == nullptr) {
        event_stream = nullptr;
        return nullptr;
    }

    std::set<MRN::Rank> endpointers;
    int size = bitset_size(target);
    for (int bit = 0; bit < size; ++bit)
        if (bitset_test(target, bit))
            endpointers.insert(static_cast<MRN::Rank>(bit + ATTACHBE_RANK_SKEW));
    auto *comm = m_network->new_Communicator(endpointers);

    // Create the broadcast stream
    auto *stream =
        m_network->new_Stream(comm, m_filter_id, MRN::SFILTER_WAITFORALL);

    auto *mc_type = new MulticastType(comm, stream, target);
    AddToList(m_mc_streams, mc_type);

    // Create the event stream for handling asynchronous output from the
    // debugger
    int filter_timeout = get_mrn_filter_timeout();
    event_stream =
        m_network->new_Stream(comm, m_filter_id, MRN::SFILTER_TIMEOUT);
    event_stream->set_FilterParameters(MRN::FILTER_UPSTREAM_SYNC, "%ud",
                                       filter_timeout);

    add_event_stream(event_stream);

    return stream;
}

/* temporary supporting only one network */
/* TODO: extension for multiple networks */
static void BE_Add_Callback(MRN::Event *evt, void *) {
    if ((evt->get_Class() == MRN::Event::TOPOLOGY_EVENT) &&
        (evt->get_Type() == MRN::TopologyEvent::TOPOL_ADD_BE)) {
        // grab the lock, write the value, then release
        pthread_mutex_lock(&callback_handler.callback_lock);
        ++(callback_handler.num_callbacks);
        pthread_mutex_unlock(&callback_handler.callback_lock);
    }
}

int MRNetFrontEnd::register_TopAddBE_event_callback(MRN::Network &network) {
    callback_handler.num_callbacks = 0;

    // setup the mutex lock
    if (pthread_mutexattr_init(&callback_handler.callback_lock_attr)) {
        // fprintf(*err_stream_ptr, "ERROR: pthread_mutexattr_init failed!\n");
        return 1;
    }

    if (pthread_mutex_init(&callback_handler.callback_lock,
                           &callback_handler.callback_lock_attr)) {
        // fprintf(*err_stream_ptr, "ERROR: pthread_mutex_init failed!\n");
        return 1;
    }

    if (!network.register_EventCallback(MRN::Event::TOPOLOGY_EVENT,
                                        MRN::TopologyEvent::TOPOL_ADD_BE,
                                        BE_Add_Callback, nullptr)) {
        // fprintf( *err_stream_ptr, "ERROR: Failed to register callback for
        // back-end add topology event!\n");
        return 1;
    }

    return 0;
}

/*------------------------------------------ ---------------------*/
/*----------------------APIs start from here----------------------*/
/*------------------------------------------- --------------------*/

/*
** initialize frontend for attaching backend programs
** Main way of launching/attaching on a Cray
*/
#ifdef ALPS
MRNetFrontEnd *MRNetFrontEnd::create_network_no_be(char *topology_file,
                                   MRN::NetworkTopology *&p_topology,
                                   int ctiAppid, int ctiMid) {
    if (topology_file == nullptr) {
        PrintOutError(
            "ERROR: mrn_fe_create_network_no_be topology_file is nullptr!\n");
        return nullptr;
    }

    // create network with CTI
    std::map<std::string, std::string> mrnet_attrs;
    std::ostringstream appidStr;
    std::ostringstream midStr;

    appidStr << ctiAppid;
    midStr << ctiMid;
    mrnet_attrs.insert(std::make_pair("CRAY_CTI_APPID", appidStr.str()));
    mrnet_attrs.insert(std::make_pair("CRAY_CTI_MID", midStr.str()));

    auto *network = MRN::Network::CreateNetworkFE(topology_file, nullptr,
                                                  nullptr, &mrnet_attrs);

    if (network->has_Error()) {
        PrintOutError(
            "ERROR: mrn_fe_create_network_no_be failed to create MRNet "
            "network!\n");
        return nullptr;
    }

    // enable network event callbacks
    if (register_TopAddBE_event_callback(*network)) {
        PrintOutError(
            "ERROR: mrn_fe_create_network_no_be failed to enable event "
            "callbacks!\n");
        delete network;
        return nullptr;
    }

    auto *topology = network->get_NetworkTopology();

    if (topology == nullptr) {
        PrintOutError(
            "ERROR: mrn_fe_create_network_no_be cannot get topology for "
            "network[%p]\n",
            network);
        delete network;
        return nullptr;
    }

    p_topology = topology;

    network->set_FailureRecovery(false);

    auto *mrnet = new MRNetFrontEnd(network, nullptr, nullptr, nullptr, 0, -1);

    return mrnet;
}
#else
MRNetFrontEnd *MRNetFrontEnd::create_network_no_be(char *topology_file,
                                   MRN::NetworkTopology *&p_topology) {
    if (topology_file == nullptr) {
        PrintOutError(
            "ERROR: mrn_fe_create_network_no_be topology_file is nullptr!\n");
        return nullptr;
    }

    auto *network =
        MRN::Network::CreateNetworkFE(topology_file, nullptr, nullptr);

    if (network->has_Error()) {
        network->print_error(
            "ERROR: mrn_fe_create_network_no_be failed to "
            "create MRNet network!\n");
        return nullptr;
    }

    // enable network event callbacks
    if (register_TopAddBE_event_callback(*network)) {
        network->print_error(
            "ERROR: mrn_fe_create_network_no_be failed to "
            "enable event callbacks!\n");
        delete network;
        return nullptr;
    }

    auto *topology = network->get_NetworkTopology();

    if (topology == nullptr) {
        PrintOutError(
            "ERROR: mrn_fe_create_network_no_be cannot get topology for "
            "network[%p]\n",
            network);
        delete network;
        return nullptr;
    }

    p_topology = topology;

    network->set_FailureRecovery(false);

    auto *mrnet = new MRNetFrontEnd(network, nullptr, nullptr, nullptr, 0, -1);

    return mrnet;
}
#endif

/*
** wait for attaching backend programs
*/
int MRNetFrontEnd::attach_be(int num_be, int timeout) {
    int num_callbacks = 0;
    int last_num_callbacks = 0;
    int expire = 0;
    struct timespec timeslice;

    timeslice.tv_sec = 0;
    timeslice.tv_nsec = 50000000;

    PrintOutMsg(
        "Waiting for debug servers to attach to MRNet communications "
        "network...\n");
    PrintOutMsg(
        "Timeout in %d seconds. Please wait for the attach to complete.\n",
        timeout);

    do {
        nanosleep(&timeslice, (struct timespec *)nullptr);

        num_callbacks = get_num_callbacks();
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
    pthread_mutexattr_destroy(&callback_handler.callback_lock_attr);
    pthread_mutex_destroy(&callback_handler.callback_lock);
    m_network->remove_EventCallback(BE_Add_Callback, MRN::Event::TOPOLOGY_EVENT,
                                    MRN::TopologyEvent::TOPOL_ADD_BE);

    return 0;
}

/*
 * load filters and completes the creation of MRNet for attaching backend
 * programs This is how streams are created on a Cray
 */
int MRNetFrontEnd::create_streams_and_load_filter(
                                          const char *filter_file,
                                          const char *filter_name) {
    DPRINT("INFO: network_load_filter_func filter_file: %s filter_name: %s\n",
           filter_file, filter_name);
    int filter_id =
        m_network->load_FilterFunc(filter_file, filter_name);
    if (filter_id == -1) {
        PrintOutError(
            "mrn_fe_create_streams_and_load_filter "
            "mrn_network_load_filter_and_complete_creation() failed!\n");
        delete m_network;
        return 1;
    }

    auto *comm_bc = m_network->get_BroadcastCommunicator();
    auto *stream_bc = m_network->new_Stream(comm_bc, filter_id, MRN::SFILTER_WAITFORALL);
    int num_be = comm_bc->size();

    auto *stream_io = m_network->new_Stream(comm_bc, MRN::TFILTER_NULL, MRN::SFILTER_DONTWAIT);

    int filter_timeout = get_mrn_filter_timeout();
    auto *stream_event = m_network->new_Stream(comm_bc, filter_id, MRN::SFILTER_TIMEOUT);
    stream_event->set_FilterParameters(MRN::FILTER_UPSTREAM_SYNC, "%ud", filter_timeout);

    // set the mrnet members
    m_cmd_stream = stream_bc;
    add_event_stream(stream_event);
    m_io_stream = stream_io;
    m_num_be = num_be;
    m_bitset_full = bitset_new(num_be);
    bitset_invert(m_bitset_full);
    m_filter_id = filter_id;

    return 0;
}

int MRNetFrontEnd::get_num_of_be() const {
    return m_num_be;
}

int MRNetFrontEnd::setup_streams(const char *msg) {
    if (msg == nullptr)
        return -1;

    // initialize commands stream
    {
        auto *stream = get_stream(DBG_MRNet_PRO_COMMAND, nullptr);
        if (stream == nullptr) {
            PrintOutError(
                "ERROR: mrn_fe_setup_streams cannot find command stream!\n");
            return -1;
        }

        if (stream->send(DBG_MRNet_PRO_COMMAND, "%s", msg) == -1) {
            PrintOutError("ERROR: mrn_fe_setup_streams stream_send() failed!\n");
            return -1;
        }

        if (stream->flush() == -1) {
            PrintOutError("ERROR: mrn_fe_setup_streams stream_flush() failed!\n");
            return -1;
        }
        DPRINT("mrn_fe_setup_streams has set the command stream. tag[%d]\n",
            DBG_MRNet_PRO_COMMAND);
    }

    // initialize io stream
    {
        auto *stream = get_stream(DBG_MRNet_PRO_IO, nullptr);
        if (stream == nullptr) {
            PrintOutError("ERROR: mrn_fe_setup_streams cannot find io stream!\n");
            return -1;
        }

        if (stream->send(DBG_MRNet_PRO_IO, "%s", msg) == -1) {
            PrintOutError("ERROR: mrn_fe_setup_streams stream_send() failed!\n");
            return -1;
        }

        if (stream->flush() == -1) {
            PrintOutError("ERROR: mrn_fe_setup_streams stream_flush() failed!\n");
            return -1;
        }
        DPRINT("mrn_fe_setup_streams has set io stream. tag[%d]\n",
            DBG_MRNet_PRO_IO);
    }

    // initialize event stream
    {
        auto *stream = get_stream(DBG_MRNet_PRO_EVENT, nullptr);
        if (stream == nullptr) {
            PrintOutError(
                "ERROR: mrn_fe_setup_streams cannot find event stream!\n");
            return -1;
        }

        if (stream->send(DBG_MRNet_PRO_EVENT, "%s", msg) == -1) {
            PrintOutError("ERROR: mrn_fe_setup_streams stream_send() failed!\n");
            return -1;
        }

        if (stream->flush() == -1) {
            PrintOutError("ERROR: mrn_fe_setup_streams stream_flush() failed!\n");
            return -1;
        }
        DPRINT("mrn_fe_setup_streams has set event stream. tag[%d]\n",
            DBG_MRNet_PRO_EVENT);
    }

    return 0;
}

int MRNetFrontEnd::send_command(bitset *target, const char *msg,
                        MRN::Stream **event_stream_ptr) {
    auto *stream = get_stream(DBG_MRNet_PRO_COMMAND, target);
    if (stream == nullptr) {
        MRN::Stream *event_stream = nullptr;
        stream = create_stream(target, event_stream);
        *event_stream_ptr = event_stream;

        if (stream == nullptr) {
            PrintOutError(
                "ERROR: mrn_fe_send_command_to_group cannot create stream for "
                "network target[%s]\n",
                bitset_to_set(target));
            return -1;
        }

        if (event_stream != nullptr) {
            // Send an initializer command along the command stream to register
            // it's matching event stream.
            auto linkMsg = CommandToSocProto(DBGCMD_SETCOMMPARAM, "%d %u",
                                             MRNCMD_SETEVENTSTREAM,
                                             event_stream->get_Id());
            if (stream->send(DBG_MRNet_PRO_COMMAND, "%s",
                            linkMsg.c_str()) == -1) {
                PrintOutError(
                    "Could not connect event stream to command stream");
            }
        }
    }

    if (stream->send(DBG_MRNet_PRO_COMMAND, "%s", msg) == -1) {
        PrintOutError(
            "ERROR: mrn_fe_send_command_to_group stream_send() failed!\n");
        return -1;
    }

    DPRINT("[INFO]: mrn_fe_send_command_to_group via stream for %s\n",
           bitset_to_set(target));

    if (stream->flush() == -1) {
        PrintOutError(
            "ERROR: mrn_fe_send_command_to_group stream_flush() failed!\n");
        return -1;
    }

    return 0;
}

proto_msg_set *MRNetFrontEnd::recv_reply(bitset *target) {
    MRN::Stream *cmd_stream = nullptr;
    if ((cmd_stream = get_stream(DBG_MRNet_PRO_COMMAND, target)) ==
        nullptr) {
        PrintOutError(
            "ERROR: mrn_fe_recv_reply cannot find command stream for %s!\n",
            bitset_to_set(target));
        return nullptr;
    }

    MRN::PacketPtr packetPtr;
    int tag;
    int retval = cmd_stream->recv(&tag, packetPtr, MRNET_BLOCKING);

    if (retval <= 0) {
        PrintOutError(
            "ERROR: mrn_fe_recv_reply failed to receive replies! retval[%d]\n",
            retval);
        return nullptr;
    }

    char **strArr;
    int strLen;
    if (packetPtr && packetPtr->unpack("%as", &strArr, &strLen) == -1) {
        PrintOutError("ERROR: mrn_fe_recv_reply packet_unpack() failed!\n");
        return nullptr;
    }

    int msgNum = strLen / 2;
    if (msgNum <= 0 || strLen % 2 != 0) {
        PrintOutError(
            "ERROR: mrn_fe_recv_reply receivd an invalid number of message: "
            "strLen[%d]\n",
            strLen);
        return nullptr;
    }

    // Create the return type
    auto *result = NewProtoMsgSet();

    for (int index = 0; index < msgNum; index++) {
        auto *sb = smart_bitset_deserialize(strArr[index * 2 + 1]);
        smart_bitset_convert(sb);
        auto *bs = bitset_dup(sb->sb_dense);

        smart_bitset_free(sb);

        // Create a proto_message for this packet and add it to the result
        AddProtoMsgSet(result, NewProtoMsg(bs, strArr[index * 2]));

        // Cleanup the strArr for the bitset only, the message gets passed up
        // and cleaned up later.
        free(strArr[index * 2 + 1]);
    }

    // free the final strArr allocated by packet_unpack
    free(strArr);

    // reset the message set
    ResetProtoMsgSet(result);

    return result;
}

int MRNetFrontEnd::get_io_notificationfd() {
    auto* io_stream = get_stream(DBG_MRNet_PRO_IO, nullptr);

    if (io_stream != nullptr) {
        // set the notification fd for the mrnet_type
        m_io_nfd = io_stream->get_DataNotificationFd();
    }

    return m_io_nfd;
}

int MRNetFrontEnd::get_event_notificationfd() const {
    return ptr_to_int(GetListNumElement(m_event_nfds, 0));
}

proto_msg_set *MRNetFrontEnd::dispatch_io(int notification_fd) {
    proto_msg_set *result = nullptr;

    if (m_io_nfd == notification_fd) {
        DPRINT("mrn_fe_check_io_and_event: handle IO_MESSAGE.\n");

        auto *io_stream = get_stream(DBG_MRNet_PRO_IO, nullptr);

        // clear the io_stream notification fd
        io_stream->clear_DataNotificationFd();

        // Try to recv as many packets as possible
        int retval;

        do {
            int tag;
            MRN::PacketPtr packetPtr;
            retval = io_stream->recv(&tag, packetPtr, MRNET_UNBLOCKING);

            if (retval > 0 && tag == DBG_MRNet_PRO_IO) {
                char *rank, *msg;

                if (packetPtr && packetPtr->unpack("%s %s", &rank, &msg) == -1) {
                    PrintOutError("ERROR: packet_unpack() failure\n");
                    // reset the message set
                    ResetProtoMsgSet(result);
                    return result;
                }

                // Create the return type if it hasn't been created already
                if (result == nullptr) {
                    result = NewProtoMsgSet();
                }

                // Create a mrnet_message for this packet and add it to the
                // result
                bitset *source = nullptr;
                if (rank != nullptr) {
                    source = str_to_bitset(rank, nullptr);
                }

                AddProtoMsgSet(result, NewProtoMsg(source, msg));
#ifdef DEBUG
                if (msg != nullptr)
                    DPRINT(
                        "INFO: mrn_fe_check_io_and_event() received io: %s "
                        "from [%s]!\n",
                        msg, rank);
                else
                    DPRINT(
                        "INFO: mrn_fe_check_io_and_event() received nullptr "
                        "io!\n");
#endif
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

proto_msg_set *MRNetFrontEnd::dispatch_event(int notification_fd) {
    proto_msg_set *result = nullptr;

    void *notification_fdptr = int_to_ptr(notification_fd);
    if (InList(m_event_nfds, notification_fdptr)) {
        DPRINT("mrn_fe_check_io_and_event: handle EVENT_MESSAGE.\n");

        // find the event_stream corresponding to the event_fd
        SetList(m_event_nfds);
        SetList(m_event_streams);
        MRN::Stream *event_stream = nullptr;
        while (event_stream = static_cast<MRN::Stream *>(GetListElement(m_event_streams)),
               GetListElement(m_event_nfds) != notification_fdptr) {
        }

        // clear the event_stream notification fd
        event_stream->clear_DataNotificationFd();

        int retval;

        // Try to recv as many packets as possible
        do {
            int tag;
            MRN::PacketPtr packetPtr;
            retval = event_stream->recv(&tag, packetPtr, MRNET_UNBLOCKING);

            if (retval > 0 && tag == DBG_MRNet_PRO_EVENT) {
                char **strArr;
                int strLen;
                if (packetPtr &&
                    packetPtr->unpack("%as", &strArr, &strLen) == -1) {
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

                int msgNum = strLen / 2;
                if (msgNum <= 0 || strLen % 2 != 0) {
                    PrintOutError(
                        "ERROR: mrn_fe_check_io_and_event received incorrect "
                        "number of message: strLen[%d]\n",
                        strLen);
                    // reset the message set
                    ResetProtoMsgSet(result);
                    return result;
                }

                // Create the return type if it hasn't been created already
                if (result == nullptr) {
                    result = NewProtoMsgSet();
                }

                for (int index = 0; index < msgNum; index++) {
                    auto *sb = smart_bitset_deserialize(strArr[index * 2 + 1]);
                    smart_bitset_convert(sb);
                    auto *bs = bitset_dup(sb->sb_dense);

                    smart_bitset_free(sb);
                    sb = nullptr;

                    // Create a proto_message for this packet and add it to the
                    // result
                    AddProtoMsgSet(result, NewProtoMsg(bs, strArr[index * 2]));

                    // Cleanup the strArr for the bitset only, the message gets
                    // passed up and cleaned up later.
                    free(strArr[index * 2 + 1]);
                }

                // Cleanup the strArr
                free(strArr);
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

static void failure_callback(void *cb_data) {
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

static void update_and_call_failure_callback(MRN::Event *evt, void *cb_data) {
    if (evt->get_Class() == MRN::Event::TOPOLOGY_EVENT) {
        if (evt->get_Type() == MRN::TopologyEvent::TOPOL_REMOVE_NODE) {
            // fprintf(*err_stream_ptr, "MRNet [TOPOL_REMOVE_NODE] failure event
            // recieved.\n");
        } else if (evt->get_Type() == MRN::TopologyEvent::TOPOL_CHANGE_PARENT) {
            // fprintf(*err_stream_ptr, "MRNet [TOPOL_CHANGE_PARENT] failure
            // event recieved.\n");
        } else {
            return;
        }

        // lets handle the callback.

        // ensure that the failure_handler is not null
        if (failure_handler == nullptr) return;

        // grab the lock, call the registered handler, then release
        pthread_mutex_lock(&failure_handler->failure_lock);

        if (failure_handler->failure_func != nullptr)
            (failure_handler->failure_func)(cb_data);

        pthread_mutex_unlock(&failure_handler->failure_lock);
    }
}

void MRNetFrontEnd::enable_failure_callback(dbginvoke_t *di) {
    // we only want to init the failure_handler if it hasen't been initialized
    // before.
    if (failure_handler == nullptr) {
        failure_handler = new FailureHandlerInfo;

        if (failure_handler == nullptr) {
            // fprintf(*err_stream_ptr, "ERROR: malloc failed!\n");
            return;
        }

        failure_handler->failure_func = failure_callback;

        // this needs to be made threadsafe...
        // we only want to initialize the mutex once.
        if (pthread_mutexattr_init(&failure_handler->failure_lock_attr)) {
            // fprintf(*err_stream_ptr, "ERROR: pthread_mutexattr_init
            // failed!\n");
            return;
        }

        if (pthread_mutex_init(&failure_handler->failure_lock,
                                 &failure_handler->failure_lock_attr)) {
            // fprintf(*err_stream_ptr, "ERROR: pthread_mutex_init failed!\n");
            return;
        }
    } else {
        failure_handler->failure_func = failure_callback;
    }

    m_network->register_EventCallback(
        MRN::Event::TOPOLOGY_EVENT, MRN::TopologyEvent::TOPOL_REMOVE_NODE,
        update_and_call_failure_callback, static_cast<void *>(di), true);

    m_network->register_EventCallback(
        MRN::Event::TOPOLOGY_EVENT, MRN::TopologyEvent::TOPOL_CHANGE_PARENT,
        update_and_call_failure_callback, static_cast<void *>(di), true);
}

void MRNetFrontEnd::debug_print_network_topology() {
    MRN::NetworkTopology *topology = m_network->get_NetworkTopology();

    if (topology == nullptr) {
        PrintOutError(
            "ERROR: debug_print_network_topology cannot get topology for "
            "network.\n");
        return;
    }

    std::vector<MRN::NetworkTopology::Node *> leaves;
    topology->get_Leaves(leaves);
    int num_leaves = leaves.size();

    if (num_leaves <= 0) {
        PrintOutError(
            "ERROR: debug_print_network_topology network has no leaves.\n");
        return;
    }

    PrintOutMsg("Leaf node info for network.\n");
    int index = 0;
    for (const auto &leafPtr : leaves) {
        PrintOutMsg("\tcurrent_node[%d] host[%s] port[%d] rank[%d]\n", index,
                    leafPtr->get_HostName().c_str(), leafPtr->get_Port(),
                    leafPtr->get_Rank());
        index++;
    }
}
