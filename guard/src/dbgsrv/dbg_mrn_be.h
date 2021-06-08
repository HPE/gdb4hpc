/*********************************************************************************\
 * dbg_mrn_be.h
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
#pragma once

/* interface of MRNet backend for Guard server*/

#include "dbg_mrn.h"
#include "mrnet/Stream.h"
#include "smart_bitset.h"
#include "dbgsrv_cmds.h"

class dbgcomm_info;

class MRNetBackEnd {
public:
    MRNetBackEnd(int argc, char *argv[]);
    ~MRNetBackEnd();

    void set_size(int size);

    void set_localrank(int rank);
    int get_localrank() const;

    using MRNetBackEndCallback = decltype(dbgsrv_dispatch_cmds);
    void set_callback(MRNetBackEndCallback callback);
    void handle_msg(MRN::Stream *stream);

    void set_cmd_stream(MRN::Stream *stream);
    void set_event_stream(MRN::Stream *stream);
    void set_event_stream_from_id(unsigned int streamId);
    void set_io_stream(MRN::Stream *stream);
    int get_event_fd() const;
    void clear_event_fd();
    unsigned int num_events_pending();
    MRN::Event* ack_event();
    void clear_events();

    int recv(struct MRN::Stream **stream, int *tag, bool blocking);

    int send_reply(MRN::Stream *stream, const char *msg);
    int send_event(char *msg, const dbgcomm_info *commInfo);
    int send_io(char *msg);

    int is_shutdown();
    void waitfor_shutdown();

    int shutdown();

    void set_param(MRN::Stream *stream, const char* msg);
private:
    MRN::Network* m_network;

    MRN::Stream* m_bc_stream_cmd;
    MRN::Stream* m_bc_stream_event;

    MRN::Stream* m_stream_cmd;
    MRN::Stream* m_stream_event;
    MRN::Stream* m_stream_io;
    MRN::Stream* m_stream_single;  // TODO: using singlecast stream to reply
                                   // single cast cmd
    int m_cmd_fd;    // for notification of events on cmd stream
    int m_event_fd;  // for notification of events on event stream
    int m_io_fd;     // for notification of events on io stream

    int m_rank;               // rank of backend
    smart_bitset* m_rank_bs;  // smart_bitset
    char* m_rank_bs_str;

    int m_network_size;

    list_t* m_cmd_streams;    // procset streams created for working with subset
    list_t* m_event_streams;  // asynch return streams for each of cmd_streams
    list_t* m_msg_list;

    MRNetBackEndCallback *m_dbgsrv_callback;
};
