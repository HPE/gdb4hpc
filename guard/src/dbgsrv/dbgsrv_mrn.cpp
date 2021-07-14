/*********************************************************************************\
 * Server MRNet dispatch routines
 *
 * Copyright (c) 1996-2012 by Monash Uni.
 *
 * Copyright 2010-2013,2020-2021 Hewlett Packard Enterprise Development LP.
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

#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include "dbgsrv_mrn.h"

#include "aif.h"
#include "bitset.h"
#include "dbg_mrn_be.h"
#include "dbgcomm_info.h"
#include "dbgevent.h"
#include "dbgfn.h"
#include "dbgformat.h"
#include "dbgsrv_cmds.h"
#include "dbgsrv_defs.h"
#include "dbgsrv_proto.h"
#include "defs.h"
#include "mrn_cfg.h"
#include "mrn_topo.h"
#include "sparse_bitset.h"
#include "useful.h"

#ifdef PERFLOG
extern struct timeval tv_srv;
extern struct timeval tv_para;
extern struct timeval tv_arch;
extern struct timeval tv_init;
extern struct timeval tv_io;
extern struct timeval tv_end;
struct timeval tv_att;
#endif

// Proto definition
MRNServerProtocol DbgSrvProtoMRN;

const char *MRNServerProtocol::name() {
    return "mrnet";
}

// Callbacks
int MRNServerProtocol::invokeCallback(const char *chost, int cport, 
    const char *host, int prog, int pnum, int port, dbgevent_t *e) {
    // we have nothing to do here
    return 1;
}

int MRNServerProtocol::eventCallback(dbgevent_t *ev) {
    // find the communication info - now because EventToSocProto deletes the
    // event
    const dbgcomm_info *comm_info = nullptr;
    if (ev->ev_event == DBGEV_BPHIT && ev->ev_bp->bp_comminfo) {
        comm_info = ev->ev_bp->bp_comminfo->clone();
    }

    char *st_dres = EventToSocProto(ev);

    DVPRINT("EventMRNCallBack is invoked: %s!\n", st_dres);

    int res = m_network->send_event(st_dres, comm_info);
    Free(st_dres);
    delete comm_info;

    return res;
}

int MRNServerProtocol::initProto(int &procnum, int &pid, int argc, const char * const * argv,
                              const char *key, const char *host, int port) {
    int res;

    int pmi_rank = procnum;
    int be_argc = 6;
    char *hostname;
    char par_addr[MAXADDRLEN + 1];
    char par_port_str[64];
    char par_rank_str[64];
    char my_rank_str[64];
    char *be_argv[6];
    unsigned par_port, par_rank;
    unsigned my_mrnrank;

    // in mrnet's case, procnum and pid are passed as arguments to the layer
    // above and don't need to be set here.
    assert(procnum >= 0);
    assert(pid >= 0);

    if ((hostname = cti_be_getNodeHostname()) == nullptr) {
        fprintf(stderr, "DbgSRVMRNInitProto cannot get hostname\n");
        ++Shutdown;

        return -1;
    }
    DVPRINT("INFO: hostname[%s] is attaching to MRNet network\n", hostname);

    // get parent information
    res = get_myparent_info(pmi_rank, hostname, par_addr, &par_port, &par_rank);
    if (res < 0) {
        fprintf(stderr, "DbgSRVMRNInitProto cannot find parent to attach!\n");
        ++Shutdown;
        free(hostname);

        return -1;
    }

    // calculate local rank
    my_mrnrank = (unsigned)ProcRankToMRNRank(pmi_rank);

    sprintf(par_port_str, "%d", par_port);
    sprintf(par_rank_str, "%d", par_rank);
    sprintf(my_rank_str, "%d", my_mrnrank);

    // be_argv[0] is not actually used by mrnet (but we need argc to be correct),
    // so it's ok to cast away the const
    be_argv[0] = const_cast<char *>(argv[0]);
    be_argv[1] = par_addr;
    be_argv[2] = par_port_str;
    be_argv[3] = par_rank_str;
    be_argv[4] = hostname;
    be_argv[5] = my_rank_str;

    DVPRINT(
        "DbgSRVMRNInitProto is attaching to: par_addr[%s] par_port[%d] "
        "par_rank[%d] my_rank[%d]\n",
        par_addr, par_port, par_rank, my_mrnrank);

    if (debug_flag) {
        fprintf(stderr, "BE argc: %d\n", be_argc);
        int i;
        for (i = 0; i < be_argc; i++) {
            fprintf(stderr, "BE argv[%d]=%s\n", i, be_argv[i]);
        }
    }

#ifdef PERFLOG
    gettimeofday(&tv_att, nullptr);
#endif

    DVPRINT("Creating be network");
    try {
        m_network = new MRNetBackEnd(be_argc, be_argv);
        res = 0;
    } catch (...) {
        free(hostname);
        return -1;
    }
    DVPRINT("Done Creating network");
    m_my_mrn_rank = pmi_rank;  // my_mrn_rank is not rank in mrnet, but is used by
                             // client to identifiy dbgsrv
    m_network->set_localrank(m_my_mrn_rank);
    DVPRINT("Done setting local rank");

    m_network->set_callback(dbgsrv_dispatch_cmds);

    // get the mrnet event fd to select on
    m_mrn_event_fd = m_network->get_event_fd();

    free(hostname);

    return res;
}

void MRNServerProtocol::endProto() {
    DVPRINT("Waiting for MRNet network shutdown.\n");
    m_network->waitfor_shutdown();
    delete m_network;
}

fd_set *MRNServerProtocol::getFdSet() {
    static fd_set fds;

    FD_ZERO(&fds);
    FD_SET(m_mrn_event_fd, &fds);

    return &fds;
}

int MRNServerProtocol::setupIo(int *fd, int *port) { return 0; }

int MRNServerProtocol::transferIo(int fd, char *buffer, int size) {
    buffer[size] = '\0';

    DVPRINT("DbgSRVMRNTransferIO: %s\n", buffer);

    m_network->send_io(buffer);

    return 0;
}

/*
 * This is the real part checking and delivering any packets from MRNet frontend
 */
void MRNServerProtocol::runProto(fd_set *fds) {
    int res;

    DVPRINT("INFO: entering DbgSRVMRNRunProto.\n");

    if (m_network == nullptr) {
        DVPRINT("m_network is nullptr!\n");
        DbgQuit(false);
        return;
    }

    if (m_network->is_shutdown()) {
        DVPRINT("DbgSRVMRNRunProto found mrnet is in a shutdown state!\n");
        DVPRINT("Quiting...\n");
        DbgQuit(false);

        return;
    }

    // make sure event fd is ready to read on
    if (!FD_ISSET(m_mrn_event_fd, fds)) {
        return;
    }

    FD_CLR(m_mrn_event_fd, fds);

    // clear the mrnet event fd
    m_network->clear_event_fd();

    // service all pending events, otherwise the event fd might not trigger
    while (m_network->num_events_pending()) {
        DVPRINT("INFO: Servicing MRNet event.\n");

        // Acknowledge the event
        m_network->ack_event();

        // receive the event
        res = m_network->recv(&m_stream, &m_tag, MRNET_UNBLOCKING);

        if (res < 0) {
            fprintf(stderr, "DbgSRVMRNRunProto() m_network->recv() failed!\n");
            exit(1);
        }
        if (res > 0) {
            DVPRINT("Recieved message on stream %u[%d]\n",
                    m_stream->get_Id(), m_tag);
            if (m_tag == DBG_MRNet_PRO_COMMAND) m_network->set_cmd_stream(m_stream);

            m_network->handle_msg(m_stream);
        }

        DVPRINT("INFO: Finished servicing MRNet Event.\n");
    }

    DVPRINT("INFO: exiting DbgSRVMRNRunProto.\n");
}

// FIXME: This should be returning an event for each stream setup so that the
// frontend knows that setup completed on each of the streams
// TODO: This is calling the send event instead of send reply function, so its
//        incompatible with the caller...
dbgevent_t *MRNServerProtocol::invokeNetwork(int numbe) {
    if (m_network == nullptr) {
        DVPRINT("Tried to call MRNServerProtocol::invokeNetwork with a null m_network!\n");
        return nullptr;
    }

    if (m_tag == DBG_MRNet_PRO_COMMAND) {
        m_network->set_cmd_stream(m_stream);
        DVPRINT("[dbgsrv] Received SetupNetwork on command stream.\n");
        return nullptr;
    }

    if (m_tag == DBG_MRNet_PRO_IO) {
        m_network->set_io_stream(m_stream);
        DVPRINT("[dbgsrv] Received SetupNetwork on io stream!\n");
        return nullptr;
    }

    if (m_tag == DBG_MRNet_PRO_EVENT) {
        dbgevent_t *e;
        char *resp;

        m_network->set_event_stream(m_stream);
        DVPRINT("[dbgsrv] Received SetupNetwork on event stream!\n");

        m_network->set_size(numbe);

        e = NewEvent(DBGEV_OK);
        resp = EventToSocProto(e);

        m_network->send_event(resp, nullptr /*defaults to the stream we just set*/);
        free(resp);

        return nullptr;
    }

    return nullptr;
}

int MRNServerProtocol::sendReply(const char *response) {
    return m_network->send_reply(m_stream, response);
}
