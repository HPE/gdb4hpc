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
#include <mutex>

#include "alloc.h"
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
#include "sparse_bitset.h"
#include "bitset_convert.h"

struct MulticastType {
    MulticastType(MRN::Communicator *comm, MRN::Stream *stream, const Bitset *target);

    std::unique_ptr<Bitset> target;
    MRN::Communicator *comm;
    MRN::Stream *stream;
};

class LaunchCallbackCounter {
public:
    LaunchCallbackCounter() : m_num_callbacks{0} {};

    void reset() {
        const std::scoped_lock<std::mutex> lock(m_callback_lock);
        m_num_callbacks = 0;
    }

    int getNumCallbacks() const {
        const std::scoped_lock<std::mutex> lock(m_callback_lock);
        return m_num_callbacks;
    }

    void incrementCallbacks() {
        const std::scoped_lock<std::mutex> lock(m_callback_lock);
        m_num_callbacks++;
    }
private:
    int m_num_callbacks;
    mutable std::mutex m_callback_lock;
};

class FailureHandler {
public:
    FailureHandler()
        : m_enabled{false} {};
    
    void enable() {m_enabled = true;};
    void disable() {m_enabled = false;};
    bool isEnabled() const {return m_enabled;};

    void operator()(void * data) {
        const std::scoped_lock<std::mutex> lock(m_failure_lock);
        
        if (!m_enabled)
            return;
        
        auto di = static_cast<dbginvoke_t *>(data);

        PrintOutError("\nERROR: MRNet failure callback invoked.\n");
        PrintOutError("Debugger is shutting down! Please file a bugreport.\n");
        PrintOutError(
            "Re-run with setting CRAY_DBG_LOG_DIR to a cross mounted location\n");
        PrintOutError(
            "and adding --debug to your launch/attach statement. Submit the\n");
        PrintOutError("generated logs with the bugreport.\n\n");

        di->d_isShutdown = 1;
        m_enabled = false;

        // Force ccdb to shutdown
        ForceQuit();
    }
private:
    bool m_enabled;
    std::mutex m_failure_lock;
};

// the launch callback counter is manipulated by callbacks from mrnet, which
// means it can't be attached to a MRNetFrontEnd as a member variable. it needs
// to be reset by each launch that uses it or the launch will see data from
// previous launches.
static LaunchCallbackCounter callback_handler;

// the failure handler can be shared since a failure from one di means a global
// failure.
static FailureHandler failure_handler;

MulticastType::MulticastType(MRN::Communicator *comm, MRN::Stream *stream,
                             const Bitset *target_)
    : comm{comm}, stream{stream} 
{
    DASSERT_NOT_NULL(target_);
    target = target_->clone();
}

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
        m_bitset_full = std::make_unique<DenseBitset>(num_be);
        m_bitset_full->invert();
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
        if (auto *multi = static_cast<MulticastType *>(voidMulti)) {
            delete multi->stream;
            delete multi;
        }
    });

    // for this list we are cheeky and store the fds directly as void
    // pointers and cast them to ints to use them. so there is no need to pass a
    // deletion function here. see the ptr_to_int and int_to_ptr functions.
    DestroyList(m_event_nfds, nullptr);

    delete m_network; // needs to be deleted last
}

MRN::Stream *MRNetFrontEnd::get_stream(DBG_MRNet_PROTOCOL proto, const Bitset *target) const {
    MRN::Stream *stream = nullptr;

    switch (proto) {
        case DBG_MRNet_PRO_COMMAND:
            // is it a broadcast?
            DASSERT_NOT_NULL(m_bitset_full);
            if (target == nullptr || *m_bitset_full == *target) {
                stream = m_cmd_stream;
            } else {
                // look for multicast stream
                SetList(m_mc_streams);
                while (auto *mc_type = static_cast<MulticastType *>(GetListElement(m_mc_streams))) {
                    if (*mc_type->target == *target) {
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

MRN::Stream *MRNetFrontEnd::create_stream(const Bitset *target, MRN::Stream *&event_stream) {
    if (target == nullptr) {
        event_stream = nullptr;
        return nullptr;
    }

    std::set<MRN::Rank> endpointers;
    int size = target->sizeInBits();
    for (int bit = 0; bit < size; ++bit)
        if (target->test(bit))
            endpointers.insert(static_cast<MRN::Rank>(bit + ATTACHBE_RANK_SKEW));
    auto *comm = m_network->new_Communicator(endpointers);

    // Create the broadcast stream
    auto stream =
        m_network->new_Stream(comm, m_filter_id, MRN::SFILTER_WAITFORALL);

    auto mc_type = new MulticastType(comm, stream, target);
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
        callback_handler.incrementCallbacks();
    }
}

int MRNetFrontEnd::register_TopAddBE_event_callback(MRN::Network &network) {
    callback_handler.reset();

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
    mrnet_attrs.insert(std::make_pair("CRAY_CTI_APPID", std::to_string(ctiAppid)));
    mrnet_attrs.insert(std::make_pair("CRAY_CTI_MID", std::to_string(ctiMid)));

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

    auto mrnet = new MRNetFrontEnd(network, nullptr, nullptr, nullptr, 0, -1);

    return mrnet;
}

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

        num_callbacks = callback_handler.getNumCallbacks();
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
        m_network = nullptr;
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
    m_bitset_full = std::make_unique<DenseBitset>(num_be);
    m_bitset_full->invert();
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

int MRNetFrontEnd::send_command(const Bitset *target, const char *msg,
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
                target ? target->toSet().c_str() : "{}");
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
           target ? target->toSet().c_str() : "{}");

    if (stream->flush() == -1) {
        PrintOutError(
            "ERROR: mrn_fe_send_command_to_group stream_flush() failed!\n");
        return -1;
    }

    return 0;
}

std::optional<ProtoMessageSet> MRNetFrontEnd::recv_reply(const Bitset *target) {
    MRN::Stream *cmd_stream = get_stream(DBG_MRNet_PRO_COMMAND, target);
    if (!cmd_stream) {
        PrintOutError(
            "ERROR: mrn_fe_recv_reply cannot find command stream for %s!\n",
            target ? target->toSet().c_str() : "{}");
        return {};
    }

    MRN::PacketPtr packetPtr;
    int tag;
    int retval = cmd_stream->recv(&tag, packetPtr, MRNET_BLOCKING);

    if (retval <= 0) {
        PrintOutError(
            "ERROR: mrn_fe_recv_reply failed to receive replies! retval[%d]\n",
            retval);
        return {};
    }

    char **strArr = nullptr;
    int strLen = -1;
    if (packetPtr && packetPtr->unpack("%as", &strArr, &strLen) == -1) {
        PrintOutError("ERROR: mrn_fe_recv_reply packet_unpack() failed!\n");
        return {};
    }

    int msgNum = strLen / 2;
    if (msgNum <= 0 || strLen % 2 != 0) {
        PrintOutError(
            "ERROR: mrn_fe_recv_reply receivd an invalid number of message: "
            "strLen[%d]\n",
            strLen);
        return {};
    }

    // Create the return type
    auto result = ProtoMessageSet();

    for (int index = 0; index < msgNum; index++) {
        auto sb = std::make_unique<SparseBitset>(strArr[index * 2 + 1]);
        auto bs = bitset::createDenseFrom(*sb);

        free(strArr[index * 2 + 1]);

        // Create a proto_message for this packet and add it to the result
        // ProtoMessage takes ownership of its arguments
        result.emplace_back(bs.release(), strArr[index * 2]);
    }

    // free the final strArr allocated by packet_unpack
    free(strArr);

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

std::optional<ProtoMessageSet> MRNetFrontEnd::dispatch_io(int notification_fd) {
    std::optional<ProtoMessageSet> result{};

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
                    return result;
                }

                // Create the return type if it hasn't been created already
                if (!result) {
                    result = ProtoMessageSet();
                }
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
                // Create a mrnet_message for this packet and add it to the result
                std::unique_ptr<Bitset> source = nullptr;
                if (rank != nullptr) {
                    source = std::make_unique<DenseBitset>(rank);
                    free(rank);
                }

                // ProtoMessageSet takes ownership of source and msg
                result->emplace_back(source.release(), msg);
            } else if (retval < 0) {
                PrintOutError(
                    "ERROR: mrn_fe_check_io_and_event() received an invalid "
                    "tag[%d] for io!\n",
                    tag);
                return result;
            }
        } while (retval > 0);
    }

    return result;
}

std::optional<ProtoMessageSet> MRNetFrontEnd::dispatch_event(int notification_fd) {
    std::optional<ProtoMessageSet> result{};

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
                char **strArr = nullptr;
                int strLen = -1;
                if (packetPtr &&
                    packetPtr->unpack("%as", &strArr, &strLen) == -1) {
                    PrintOutError(
                        "ERROR: mrn_fe_check_io_and_event packet_unpack() "
                        "failed!\n");
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
                    return result;
                }

                // Create the return type if it hasn't been created already
                if (!result) {
                    result = ProtoMessageSet();
                }

                for (int index = 0; index < msgNum; index++) {
                    auto sb = std::make_unique<SparseBitset>(strArr[index * 2 + 1]);
                    auto bs = bitset::createDenseFrom(*sb);

                    free(strArr[index * 2 + 1]);

                    // Create a proto_message for this packet and add it to the result
                    // ProtoMessageSet takes ownership of the arguments
                    result->emplace_back(bs.release(), strArr[index * 2]);
                }

                // Cleanup the strArr
                free(strArr);
            } else if (retval < 0) {
                PrintOutError(
                    "ERROR: mrn_fe_check_io_and_event() invalid tag[%d] for "
                    "event!\n",
                    tag);
                return result;
            }
        } while (retval > 0);
    }


    return result;
}

static void call_failure_callback(MRN::Event *evt, void *cb_data) {
    if (evt->get_Class() == MRN::Event::TOPOLOGY_EVENT) {
        if (evt->get_Type() == MRN::TopologyEvent::TOPOL_REMOVE_NODE) {
            // fprintf(*err_stream_ptr, "MRNet [TOPOL_REMOVE_NODE] failure event
            // received.\n");
        } else if (evt->get_Type() == MRN::TopologyEvent::TOPOL_CHANGE_PARENT) {
            // fprintf(*err_stream_ptr, "MRNet [TOPOL_CHANGE_PARENT] failure
            // event received.\n");
        } else {
            return;
        }

        failure_handler(cb_data);
    }
}

void MRNetFrontEnd::enable_failure_callback(dbginvoke_t *di) {
    failure_handler.enable();

    m_network->register_EventCallback(
        MRN::Event::TOPOLOGY_EVENT, MRN::TopologyEvent::TOPOL_REMOVE_NODE,
        call_failure_callback, static_cast<void *>(di), true);

    m_network->register_EventCallback(
        MRN::Event::TOPOLOGY_EVENT, MRN::TopologyEvent::TOPOL_CHANGE_PARENT,
        call_failure_callback, static_cast<void *>(di), true);
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
