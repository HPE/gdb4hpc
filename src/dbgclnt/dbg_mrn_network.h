/*********************************************************************************\
 * interface of MRNet frontend for Guard client
 *
 * Copyright 2010-2013,2021 Hewlett Packard Enterprise Development LP.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Hewlett
 * Packard Enterprise Development LP., no part of this work or its content may be
 * used, reproduced or disclosed in any form.
 *********************************************************************************/

#pragma once

#include "dbgclnt_network.h"
#include "dbg_mrn.h"
#include "mrn_topgen.h"
#include "dense_bitset.h"

#include "mrnet/Stream.h"
#include "mrnet/NetworkTopology.h"

class MRNetNetwork : public DebugClientNetwork {
public:
    MRNetNetwork() = delete; // use create_network methods below
    ~MRNetNetwork();

    static MRNetNetwork *create_network_no_be(char *topology_file,
                                       MRN::NetworkTopology *&ptopology, int ctiAppid,
                                       int ctiMid);

    int attach_be(int num_be, int timeout);
    int create_streams_and_load_filter(const char *filter_file,
                                       const char *filter_name);

    /* return the number of backends in MRNet */
    int get_num_of_be() const;

    /* setup the command, event and I/O streams for debugging sessions */
    int setup_streams(const char *msg);

    /* send commands to backends via the command stream*/
    int send_command(const Bitset *target, const char *msg,
                     MRN::Stream **event_stream_ptr);

    /* collect replies from backends via the command stream*/
    std::optional<ProtoMessageSet> recv_reply(const Bitset *target);

    /* enable MRNet frontend to handle failures. */
    void enable_failure_callback(DbgInvoke *);

    /* Obtain event notification fds for the io and event streams */
    int get_io_notificationfd();
    int get_event_notificationfd() const;

    /* dispatch routines for streams */
    std::optional<ProtoMessageSet> dispatch_io(int notification_fd);
    std::optional<ProtoMessageSet> dispatch_event(int notification_fd);

    void debug_print_network_topology();

private:
    MRNetNetwork(MRN::Network *network, MRN::Stream *cmd,
          MRN::Stream *event, MRN::Stream *io, int num_be, int filter);
    
    void add_event_stream(MRN::Stream *event_stream);
    MRN::Stream *get_stream(DBG_MRNet_PROTOCOL proto, const Bitset *target) const;
    MRN::Stream *create_stream(const Bitset *target, MRN::Stream *&event_stream);
    static int register_TopAddBE_event_callback(MRN::Network &network);

    MRN::Network *m_network;
    MRN::Stream *m_cmd_stream;
    MRN::Stream *m_io_stream;
    list_t *m_event_streams;
    list_t *m_mc_streams;  // list of multicast streams
    list_t *m_event_nfds;
    int m_io_nfd;
    int m_filter_id;
    int m_num_be;  // number of dbgsrvs
    std::unique_ptr<DenseBitset> m_bitset_full;
};
