/*********************************************************************************\
 * dbg_mrn_be.c
 *
 * Copyright 2010-2021 Hewlett Packard Enterprise Development LP.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Hewlett Packard Enterprise Development LP.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *
 *********************************************************************************/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dbg_mrn_be.h"

#include "alloc.h"
#include "dbg_mrn.h"
#include "dbgcmd.h"
#include "dbgcomm_info.h"
#include "dbgevent.h"
#include "dbgsrv_defs.h"
#include "defs.h"
#include "smart_bitset.h"
#include "bitset_serialize.h"

#include "mrnet/Stream.h"
#include "mrnet/Network.h"

// class to associate the event call-back stream for the asynch response of
// breakpoint/watchpoints
class mrncomm_info : public dbgcomm_info {
   public:
    mrncomm_info(MRN::Stream* inStream) : stream(inStream) {}
    dbgcomm_info* clone() const { return new mrncomm_info(*this); }

    MRN::Stream* getStream() const { return stream; }

   private:
    MRN::Stream* stream;
};

MRNetBackEnd::MRNetBackEnd(int argc, char* argv[])
    : m_network{MRN::Network::CreateNetworkBE(argc, argv)},
      m_bc_stream_cmd{nullptr},
      m_bc_stream_event{nullptr},
      m_stream_cmd{nullptr},
      m_stream_event{nullptr},
      m_stream_io{nullptr},
      m_stream_single{nullptr},
      m_event_fd{0},
      m_rank_bs{nullptr},
      m_rank_bs_str{Bitset::SerializedPtr(nullptr, std::free)},
      m_cmd_streams{NewList()},
      m_event_streams{NewList()},
      m_msg_list{NewList()}
{
    if (m_network == nullptr) {
        DVPRINT("ERROR: mrn_initialize_backend_node cannot create BE! (missing network)\n");
        DestroyList(m_cmd_streams, nullptr);
        DestroyList(m_event_streams, nullptr);
        DestroyList(m_msg_list, nullptr);
        throw std::runtime_error("ERROR: mrn_initialize_backend_node cannot create BE! (missing network)");
    }

    m_rank = m_network->get_LocalRank();
    DVPRINT("mrn_initialize_backend_node rank[%d]\n", m_rank);

    m_event_fd =
        m_network->get_EventNotificationFd(MRN::Event::DATA_EVENT);
}

MRNetBackEnd::~MRNetBackEnd() {
    m_network->close_EventNotificationFd(MRN::Event::DATA_EVENT);
    delete m_network;

    // XXX: Memory leak?
    DestroyList(m_cmd_streams, nullptr);
    DestroyList(m_event_streams, nullptr);
    DestroyList(m_msg_list, nullptr);
}

int MRNetBackEnd::is_shutdown() { return m_network->is_ShutDown(); }

void MRNetBackEnd::waitfor_shutdown() { m_network->waitfor_ShutDown(); }

int MRNetBackEnd::get_localrank() const { return m_rank; }

void MRNetBackEnd::set_localrank(int rank) {
    m_rank = rank;
    m_stream_single = m_network->get_Stream(rank + ATTACHBE_RANK_SKEW);;
    DVPRINT("INFO: [dbgsrv] m_stream_single is set [%p].\n", m_stream_single);
}

// Set the current command stream for the backend.
// The first call is part of the bootstrapping and is given the broadcast
// communicator that is attached to every back-end. New streams are created
// whenever the user sends a command using a new procset or focus. For
// performance reasons, we want to be sure all responses go back on the same
// stream, and any asynchronous events are triggered on the same communicator -
// by restricting the focus, we know that MRNet won't be waiting on any rank not
// involved in the command.
void MRNetBackEnd::set_cmd_stream(MRN::Stream* stream) {
    if (stream == m_stream_cmd || stream == nullptr) return;

    unsigned int streamId = stream->get_Id();

    MRN::Stream* event_stream = nullptr;
    if (m_bc_stream_cmd == nullptr) {
        // the first registered stream is broadcast stream connected
        // to every rank
        DVPRINT("INFO: [dbgsrv] m_stream_cmd is set to %u.\n",
                streamId);
        m_bc_stream_cmd = stream;
    } else if (m_stream_cmd == m_bc_stream_cmd) {
        DVPRINT(
            "INFO: [dbgsrv] m_stream_event set back to broadcast "
            "stream\n");
        event_stream = m_bc_stream_event;
    } else {
        list_t* cs = m_cmd_streams;
        list_t* es = m_event_streams;
        SetList(cs);
        SetList(es);
        for (MRN::Stream *cmd, *event;
             (cmd = static_cast<MRN::Stream *>(GetListElement(cs))) &&
             (event = static_cast<MRN::Stream *>(GetListElement(es)));) {
            if (cmd == stream) {
                event_stream = event;
                break;
            }
        }

        if (event_stream == nullptr) {
            DVPRINT("INFO: [dbgsrv] no event stream for %u, using default\n",
                    streamId);
            event_stream = m_bc_stream_event;
        } else {
            DVPRINT(
                "INFO: [dbgsrv] found event stream for command stream %u,%u\n",
                streamId, event_stream->get_Id());
        }

        m_stream_event = event_stream;
    }
    if (event_stream != nullptr) m_stream_event = event_stream;

    m_stream_cmd = stream;
}

// the first call to this function sets the default event stream for the
// back-end, which is connected to all of back ends. Any additional calls are
// setting an event stream that matches the current procSet. Note that "event"
// in this case is not the same as the MRNet events, this stream is for sending
// asynchronous debugger events (e.g. breakpoint-hit)
void MRNetBackEnd::set_event_stream(MRN::Stream* stream) {
    if (!stream || stream == m_stream_event) return;

    if (m_bc_stream_event == nullptr) {
        // the first registered stream is the default (broadcast) stream
        m_bc_stream_event = stream;
        DVPRINT("INFO: [dbgsrv] m_event_stream is set %u.\n",
                stream->get_Id());

    } else if (m_stream_cmd && !InList(m_cmd_streams, (void*)m_stream_cmd)) {
        DVPRINT("INFO: [dbgsrv] creating cmd/event stream pair %d, %d\n",
                m_stream_cmd->get_Id(), stream->get_Id());

        AddToList(m_cmd_streams, m_stream_cmd);
        AddToList(m_event_streams, stream);
    }
    m_stream_event = stream;
}

void MRNetBackEnd::set_event_stream_from_id(unsigned int streamId) {
    if (m_network == nullptr) {
        DVPRINT("Network on server side is not set\n");
        return;
    }

    if (auto* event_stream = m_network->get_Stream(streamId)) {
        set_event_stream(event_stream);
    } else {
        DVPRINT("Could not get event stream from event stream id!\n");
    }
}

void MRNetBackEnd::set_io_stream(MRN::Stream* stream) {
    m_stream_io = stream;
    DVPRINT("INFO: [dbgsrv] m_io_stream is set.\n");
}

int MRNetBackEnd::get_event_fd() const { return m_event_fd; }

void MRNetBackEnd::clear_event_fd() {
    m_network->clear_EventNotificationFd(MRN::Event::DATA_EVENT);
}

unsigned int MRNetBackEnd::num_events_pending() {
    return m_network->num_EventsPending();
}

MRN::Event* MRNetBackEnd::ack_event() { return m_network->next_Event(); }

void MRNetBackEnd::clear_events() { return m_network->clear_Events(); }

void MRNetBackEnd::set_size(int size) {
    DVPRINT("mrn_be_set_size is called. size[%d]\n", size);

    m_network_size = size;
    m_rank_bs = std::make_unique<SmartBitset>(size);

    m_rank_bs->set(m_rank);
    m_rank_bs_str = m_rank_bs->serialize();
}

int MRNetBackEnd::recv(MRN::Stream** stream, int* tag, bool blocking) {
    MRN::Stream* stream_be = nullptr;
    MRN::PacketPtr packet_be;
    int tag_be = -1;

    m_network->recv(&tag_be, packet_be, &stream_be, blocking);

    char* msg = nullptr;
    if (packet_be && packet_be->unpack("%s", &msg) == -1) {
        fprintf(stderr, "MRNetBackEnd::recv packet_unpack() failure\n");
        return -1;
    }

#ifdef DEBUG
    if (msg != nullptr) fprintf(stderr, "INFO: [dbgsrv] MRNetBackEnd::recv: [%s]\n", msg);
#endif

    if (msg != nullptr) {
        AddToList(m_msg_list, msg);
        *stream = stream_be;
        *tag = tag_be;
        return 1;
    }

    return 0;
}

int MRNetBackEnd::send_reply(MRN::Stream* stream, const char* msg) {
    if (m_stream_cmd == nullptr) return -1;

    DVPRINT("INFO: [dbgsrv] mrn_be_send_reply [%.1000s], via stream[%u]\n", msg,
            stream->get_Id());

    const char* strArr[2];
    strArr[0] = msg;
    strArr[1] = m_rank_bs_str.get();

    if (stream == nullptr) stream = m_stream_cmd;

    if (stream->send(DBG_MRNet_PRO_COMMAND, "%as", strArr, 2) == -1) {
        fprintf(stderr, "ERROR: mrn_be_send_reply stream->send(%s) failed!\n",
                msg);
        return -1;
    }

    if (stream->flush() == -1) {
        fprintf(stderr, "ERROR: mrn_be_send_reply stream_flush() failed!\n");
        return -1;
    }

    return 0;
}

int MRNetBackEnd::send_event(char* msg, const dbgcomm_info* commInfo) {
    if (m_stream_event == nullptr) return -1;

    char* strArr[2];
    strArr[0] = msg;
    strArr[1] = m_rank_bs_str.get();

    MRN::Stream* stream = m_stream_event;

    // a breakpoint should have passed the stream back so be can send it back
    // on the same communicator.  This lets MRNet know exactly which end-points
    // can be sending this event keeping it from waiting for a response from
    // an uninvolved node.	(otherwise, just use the big pipe)
    if (const mrncomm_info* ci = dynamic_cast<const mrncomm_info*>(commInfo)) {
        if (ci->getStream()) {
            stream = ci->getStream();
            DVPRINT("got stream %u from event\n", stream->get_Id());
        }
    }

    DVPRINT("INFO: [dbgsrv] MRNetBackEnd::send_event [%s] via stream[%u]%s\n", msg,
            stream->get_Id(),
            commInfo ? " from comm_info" : " no comm_info");

    if (stream->send(DBG_MRNet_PRO_EVENT, "%as", strArr, 2) == -1) {
        fprintf(stderr, "ERROR: MRNetBackEnd::send_event stream_send(%s) failed!\n",
                msg);
        return -1;
    }

    if (stream->flush() == -1) {
        fprintf(stderr, "ERROR: MRNetBackEnd::send_event stream_flush() failed!\n");
        return -1;
    }

    return 0;
}

int MRNetBackEnd::send_io(char* msg) {
    char* strArr[2];

    if (m_stream_io == nullptr) return -1;

    DVPRINT("INFO: [dbgsrv] MRNetBackEnd::send_io [%s] via stream[%p]\n", msg,
            m_stream_io);

    strArr[0] = msg;
    strArr[1] = m_rank_bs_str.get();

    if (m_stream_io->send(DBG_MRNet_PRO_IO, "%as", strArr, 2) ==
        -1) {
        fprintf(stderr, "ERROR: MRNetBackEnd::send_io stream_send(%s) failed!\n", msg);
        return -1;
    }

    if (m_stream_io->flush() == -1) {
        fprintf(stderr, "ERROR: MRNetBackEnd::send_io stream_flush() failed!\n");
        return -1;
    }

    return 0;
}

void MRNetBackEnd::set_callback(MRNetBackEndCallback callback) {
    m_dbgsrv_callback = callback;
    return;
}

void MRNetBackEnd::handle_msg(MRN::Stream *stream) {
    char* msg = (char*)RemoveFirst(m_msg_list);

    if (msg != nullptr) {
        int cmdIdx = 0;
        if (sscanf(msg, "%d", &cmdIdx) == 1 && cmdIdx == DBGCMD_SETCOMMPARAM) {
            // strip off the command number and toss it back into the mrn
            // specific processing.  I don't think we need to fire up
            // regex or strtok to pull of one number.
            char* submsg = msg;
            for (; *submsg && isspace(*submsg); ++submsg) {
            }
            for (; *submsg && isdigit(*submsg); ++submsg) {
            }
            for (; *submsg && isspace(*submsg); ++submsg) {
            }
            set_param(stream, submsg);
        } else {
            // push all other commands to the back-end.
            mrncomm_info ci(m_stream_event);
            m_dbgsrv_callback(msg, &ci);
            free(msg);
        }
    }

    return;
}

void MRNetBackEnd::set_param(MRN::Stream *stream, const char *msg) {
    // note: the prototype always puts length of the remainder of the message
    // before the message itself. Since at this point we only support a single
    // special command, we can keep the handling pretty simple.
    int len, subCommand;
    unsigned int eventStreamId;
    if (sscanf(msg, "%d %d %u", &len, &subCommand, &eventStreamId) != 3) {
        DVPRINT("Could not parse MRNSetParam\n");
    } else if (subCommand != MRNCMD_SETEVENTSTREAM) {
        DVPRINT("Unrecognized MRNSetParam command\n");
    } else {
        unsigned int commandStreamId = stream->get_Id();
        DVPRINT("Stream pair %u %u\n", commandStreamId, eventStreamId);
        set_event_stream_from_id(eventStreamId);
    }
}
