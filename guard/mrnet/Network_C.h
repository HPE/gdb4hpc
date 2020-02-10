/*********************************************************************************\
 * <Network_C.h>
 *
 * © 2010-2020 Cray Inc.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Cray Inc.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *
 *********************************************************************************/

#ifndef _GUARD_MRNET_NETWORK_H
#define _GUARD_MRNET_NETWORK_H

#include "MRNet_C.h"
#include "bitset.h"

#ifdef __cplusplus
extern "C" {
#endif

// create mrnet network front end
MRNet_Network* network_create_FE(const char* itopology,
                                 const char* ibackend_exe,
                                 char** ibackend_argv);

MRNet_Network* network_create_FE_with_cti(const char* itopology, int ctiAppid,
                                          int ctiMid);

// create mrnet network back end
MRNet_Network* network_create_BE(int argc, char** argv);

// MRNet_Network* network_attach(char *iphostname, Port ipport, Rank iprank,
// char *imyhostname, Rank imyrank );

void network_delete(MRNet_Network* network);

// void network_shutdown(MRNet_Network* network);

MRNet_Topology* network_get_topology(MRNet_Network* network);

void network_set_terminate_backends_on_shutdown(MRNet_Network* network,
                                                int terminate);

MRNet_Communicator* network_get_broadcast_communicator(MRNet_Network* network);

MRNet_Communicator* network_create_communicator(MRNet_Network* network,
                                                bitset* target, int skew);

int network_load_filter_func(MRNet_Network* network, const char* so_file,
                             const char* func);

// MRNet_Stream* network_create_stream(MRNet_Network* network,
// MRNet_Communicator* communicator, int us_filter_id);

// long network_create_pointer();

MRNet_Stream* network_create_stream_syncfilter(MRNet_Network* network,
                                               MRNet_Communicator* communicator,
                                               int us_filter_id);

MRNet_Stream* network_create_stream_timefilter(MRNet_Network* network,
                                               MRNet_Communicator* communicator,
                                               int us_filter_id, int timeout);

MRNet_Stream* network_create_stream_nofid_waitall(
    MRNet_Network* network, MRNet_Communicator* communicator);

MRNet_Stream* network_create_stream_nofid_nowait(
    MRNet_Network* network, MRNet_Communicator* communicator);

MRNet_Stream* network_create_stream_sum(MRNet_Network* network,
                                        MRNet_Communicator* communicator);
// MRNet_Stream* network_create_stream_with_string(MRNet_Network* network,
// MRNet_Communicator* communicator, char* us_filters, char* sync_filters, char*
// ds_filters );

MRNet_Stream* network_get_stream(MRNet_Network* network, unsigned int iid);

int network_send(MRNet_Network* network, int ibe, int itag,
                 const char* iformat_str, ...);
int network_flush(MRNet_Network* network);

int network_recv(MRNet_Network* network, int* otag, MRNet_Packet** packet,
                 MRNet_Stream** ostream, int blocking);

int network_has_error(MRNet_Network* network);

void network_print_error(MRNet_Network* network, const char* error);

char* network_get_local_host(MRNet_Network* network);

int network_get_local_port(MRNet_Network* network);

int network_get_local_rank(MRNet_Network* network);

int network_register_TopAddBE_event_callback(MRNet_Network* network);

void network_remove_TopAddBE_event_callback(MRNet_Network* network);

int network_get_num_callbacks();

int network_register_Failure_event_callback(MRNet_Network* network,
                                            void (*callback)(), void* data);

int network_is_shutdown(MRNet_Network* network);

void network_waitfor_shutdown(MRNet_Network* network);

int network_set_failure_recovery(MRNet_Network* network, int on);

int network_get_EventNotificationFd(MRNet_Network* network, MRNEventType etyp);

void network_clear_EventNotificationFd(MRNet_Network* network,
                                       MRNEventType etyp);

void network_close_EventNotificationFd(MRNet_Network* network,
                                       MRNEventType etyp);

unsigned int network_get_num_events_pending(MRNet_Network* network);

void network_get_next_event(MRNet_Network* network);

void network_clear_events(MRNet_Network* network);

#ifdef __cplusplus
}
#endif

#endif /* _GUARD_MRNET_NETWORK_H */
