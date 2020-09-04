/*********************************************************************************\
 * <Network.cpp>
 *
 * Copyright 2011-2013,2020 Hewlett Packard Enterprise Development LP.
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

#include "mrnet/Network.h"

#include <pthread.h>
#include <string.h>

#include <iostream>
#include <map>
#include <sstream>
#include <string>

#include "MRNet_type_wrapper.h"
#include "bitset.h"
#include "mrnet/Stream.h"
#include "mrnet/Types.h"

using namespace MRN;

typedef struct {
    int num_callbacks;
    pthread_mutex_t callback_lock;
    pthread_mutexattr_t callback_lock_attr;
} callback_handler_info_t;

typedef struct {
    void (*failure_func)(void*);
    pthread_mutex_t failure_lock;
    pthread_mutexattr_t failure_lock_attr;
} failure_handler_info_t;

static PacketPtr* _network_pack = NULL;
static int _network_counter = 0;
static callback_handler_info_t callback;
static failure_handler_info_t* failure_handler = NULL;

void allocate_packet() {
    if (_network_pack == NULL) _network_pack = new PacketPtr;

    _network_counter++;
}

void free_packet() {
    _network_counter--;

    if (_network_counter == 0 && _network_pack != NULL) {
        delete _network_pack;
        _network_pack = NULL;
    }

    if (_network_counter < 0) {
        // fprintf(*err_stream_ptr, "ERROR: _network_counter[%d]! MRNet network
        // has been deleted more than supposed to be.\n", _network_counter);
        exit(1);
    }
}

extern "C" MRNet_Network* network_create_FE(const char* itopology,
                                            const char* ibackend_exe,
                                            const char** ibackend_argv) {
    Network* network =
        Network::CreateNetworkFE(itopology, ibackend_exe, ibackend_argv);

    allocate_packet();

    return (MRNet_Network*)network;
}

/*
 * MRNet now uses the CTI interface to launch the CP processes.
 */
extern "C" MRNet_Network* network_create_FE_with_cti(const char* itopology,
                                                     int ctiAppid, int ctiMid) {
    std::map<std::string, std::string> mrnet_attrs;
    std::ostringstream appidStr;
    std::ostringstream midStr;

    appidStr << ctiAppid;
    midStr << ctiMid;
    mrnet_attrs.insert(std::make_pair("CRAY_CTI_APPID", appidStr.str()));
    mrnet_attrs.insert(std::make_pair("CRAY_CTI_MID", midStr.str()));

    Network* network =
        Network::CreateNetworkFE(itopology, NULL, NULL, &mrnet_attrs);

    allocate_packet();

    return (MRNet_Network*)network;
}

extern "C" MRNet_Network* network_create_BE(int argc, char** argv) {
    Network* network = Network::CreateNetworkBE(argc, argv);

    allocate_packet();

    return (MRNet_Network*)network;
}

extern "C" void network_delete(MRNet_Network* network) {
    if (network != NULL) {
        Network* pNetwork = (Network*)network;
        delete pNetwork;
    }

    free_packet();
}

/*
extern "C" void network_shutdown(MRNet_Network* network)
{
        Network* pNetwork = (Network*)network;
        pNetwork->shutdown_Network();
}

extern "C" void network_set_terminate_backends_on_shutdown(MRNet_Network*
network, int terminate)
{
        Network* pNetwork = (Network*)network;
        pNetwork->set_TerminateBackEndsOnShutdown(terminate);
}
*/

extern "C" MRNet_Topology* network_get_topology(MRNet_Network* network) {
    Network* pNetwork = (Network*)network;

    MRNet_Topology* pTopology =
        (MRNet_Topology*)pNetwork->get_NetworkTopology();
    return pTopology;
}

extern "C" MRNet_Communicator* network_get_broadcast_communicator(
    MRNet_Network* network) {
    Network* pNetwork = (Network*)network;
    Communicator* communicator = pNetwork->get_BroadcastCommunicator();
    return (MRNet_Communicator*)communicator;
}

extern "C" MRNet_Communicator* network_create_communicator(
    MRNet_Network* network, bitset* target, int skew) {
    std::set<Rank> endpointers;
    Network* pNetwork = (Network*)network;

    int size = bitset_size(target);
    for (int bit = 0; bit < size; ++bit)
        if (bitset_test(target, bit)) endpointers.insert((Rank)(bit + skew));

    Communicator* communicator = pNetwork->new_Communicator(endpointers);
    return (MRNet_Communicator*)communicator;
}

extern "C" int network_load_filter_func(MRNet_Network* network,
                                        const char* so_file, const char* func) {
    Network* pNetwork = (Network*)network;
    return pNetwork->load_FilterFunc(so_file, func);
}

extern "C" MRNet_Stream* network_create_stream_syncfilter(
    MRNet_Network* network, MRNet_Communicator* communicator,
    int us_filter_id) {
    Network* pNetwork = (Network*)network;
    Communicator* pCommunicator = (Communicator*)communicator;
    Stream* pStream =
        pNetwork->new_Stream(pCommunicator, us_filter_id, SFILTER_WAITFORALL);

    return (MRNet_Stream*)pStream;
}

extern "C" MRNet_Stream* network_create_stream_timefilter(
    MRNet_Network* network, MRNet_Communicator* communicator, int us_filter_id,
    int timeout) {
    Network* pNetwork = (Network*)network;
    Communicator* pCommunicator = (Communicator*)communicator;
    Stream* pStream =
        pNetwork->new_Stream(pCommunicator, us_filter_id, SFILTER_TIMEOUT);
    pStream->set_FilterParameters(FILTER_UPSTREAM_SYNC, "%ud", timeout);

    return (MRNet_Stream*)pStream;
}

extern "C" MRNet_Stream* network_create_stream_nofid_waitall(
    MRNet_Network* network, MRNet_Communicator* communicator) {
    Network* pNetwork = (Network*)network;
    Communicator* pCommunicator = (Communicator*)communicator;
    Stream* pStream =
        pNetwork->new_Stream(pCommunicator, TFILTER_MAX, SFILTER_WAITFORALL);

    return (MRNet_Stream*)pStream;
}

extern "C" MRNet_Stream* network_create_stream_nofid_nowait(
    MRNet_Network* network, MRNet_Communicator* communicator) {
    Network* pNetwork = (Network*)network;
    Communicator* pCommunicator = (Communicator*)communicator;
    Stream* pStream =
        pNetwork->new_Stream(pCommunicator, TFILTER_NULL, SFILTER_DONTWAIT);

    return (MRNet_Stream*)pStream;
}

extern "C" MRNet_Stream* network_create_stream_sum(
    MRNet_Network* network, MRNet_Communicator* communicator) {
    Network* pNetwork = (Network*)network;
    Communicator* pCommunicator = (Communicator*)communicator;
    Stream* pStream = pNetwork->new_Stream(pCommunicator, TFILTER_SUM);

    return (MRNet_Stream*)pStream;
}
/*
extern "C" MRNet_Stream* network_create_stream_with_string(MRNet_Network*
network, MRNet_Communicator* communicator, char* us_filters, char* sync_filters,
char* ds_filters )
{
        Network* pNetwork = (Network*)network;
        Communicator* pCommunicator = (Communicator*)communicator;
        Stream* pStream = pNetwork->new_Stream(pCommunicator, us_filters,
sync_filters, ds_filters); return (MRNet_Stream*)pStream;
}
*/

extern "C" MRNet_Stream* network_get_stream(MRNet_Network* network,
                                            unsigned int iid) {
    Network* pNetwork = (Network*)network;
    Stream* pStream = pNetwork->get_Stream(iid);
    return (MRNet_Stream*)pStream;
}

extern "C" int network_send(MRNet_Network* network, int ibe, int itag,
                            const char* ifmt, ...) {
    Network* pNetwork = (Network*)network;
    va_list arg_list;
    int res;

    va_start(arg_list, ifmt);
    res = pNetwork->send((Rank)ibe, ifmt, arg_list, itag);
    va_end(arg_list);

    return res;
}

extern "C" int network_flush(MRNet_Network* network) {
    Network* pNetwork = (Network*)network;
    return pNetwork->flush();
}

extern "C" int network_recv(MRNet_Network* network, int* otag,
                            MRNet_Packet** packet, MRNet_Stream** ostream,
                            int blocking) {
    Network* pNetwork = (Network*)network;
    Stream** pStream = (Stream**)ostream;

    if (_network_pack == NULL) {
        // fprintf(*err_stream_ptr, "Invalid network operation! (Please call
        // network_create_FE/BE() first.)\n");
        exit(1);
    }

    *_network_pack = Packet::NullPacket;
    *packet = NULL;

    int res = 0;
    if (blocking)
        res = pNetwork->recv(otag, *_network_pack, pStream);
    else
        res = pNetwork->recv(otag, *_network_pack, pStream, false);

    if (*_network_pack != Packet::NullPacket)
        *packet = (MRNet_Packet*)_network_pack;

    return res;
}

extern "C" int network_has_error(MRNet_Network* network) {
    Network* pNetwork = (Network*)network;
    if (pNetwork->has_Error() == true) return 1;

    return 0;
}

extern "C" void network_print_error(MRNet_Network* network, const char* error) {
    Network* pNetwork = (Network*)network;
    pNetwork->perror(error);
}

extern "C" char* network_get_local_host(MRNet_Network* network) {
    Network* pNetwork = (Network*)network;
    std::string host = pNetwork->get_LocalHostName();
    return strdup(host.c_str());
}

extern "C" int network_get_local_port(MRNet_Network* network) {
    Network* pNetwork = (Network*)network;
    Port port = pNetwork->get_LocalPort();
    return (int)port;
}

extern "C" int network_get_local_rank(MRNet_Network* network) {
    Network* pNetwork = (Network*)network;
    Rank rank = pNetwork->get_LocalRank();
    return (int)rank;
}

EventType transfer_to_mrn_event_class(MRNEventType etyp) {
    switch (etyp) {
        case MRNet_EVENT_DATA:
            return Event::DATA_EVENT;
        case MRNet_EVENT_ERROR:
            return Event::ERROR_EVENT;
        case MRNet_EVENT_TOPOLOGY:
            return Event::TOPOLOGY_EVENT;
        case MRNet_EVENT_UNKNOWN:
        default:
            return Event::EVENT_CLASS_ALL;
    }

    return (EventType)NULL;
}

extern "C" int network_get_EventNotificationFd(MRNet_Network* network,
                                               MRNEventType etyp) {
    Network* pNetwork = (Network*)network;
    EventClass event = transfer_to_mrn_event_class(etyp);

    return pNetwork->get_EventNotificationFd(event);
}

extern "C" void network_clear_EventNotificationFd(MRNet_Network* network,
                                                  MRNEventType etyp) {
    Network* pNetwork = (Network*)network;
    EventClass event = transfer_to_mrn_event_class(etyp);

    return pNetwork->clear_EventNotificationFd(event);
}

extern "C" void network_close_EventNotificationFd(MRNet_Network* network,
                                                  MRNEventType etyp) {
    Network* pNetwork = (Network*)network;
    EventClass event = transfer_to_mrn_event_class(etyp);

    return pNetwork->close_EventNotificationFd(event);
}

extern "C" unsigned int network_get_num_events_pending(MRNet_Network* network) {
    Network* pNetwork = (Network*)network;

    return pNetwork->num_EventsPending();
}

extern "C" void network_get_next_event(MRNet_Network* network) {
    Network* pNetwork = (Network*)network;

    return (void)pNetwork->next_Event();
}

extern "C" void network_clear_events(MRNet_Network* network) {
    Network* pNetwork = (Network*)network;

    return pNetwork->clear_Events();
}

/* temporary supporting only one network */
/* TODO: extension for multiple networks */
void BE_Add_Callback(Event* evt, void*) {
    if ((evt->get_Class() == Event::TOPOLOGY_EVENT) &&
        (evt->get_Type() == TopologyEvent::TOPOL_ADD_BE)) {
        // grab the lock, write the value, then release
        pthread_mutex_lock(&callback.callback_lock);
        ++(callback.num_callbacks);
        pthread_mutex_unlock(&callback.callback_lock);
    }
}

void Failure_Callback(Event* evt, void* cb_data) {
    if (evt->get_Class() == Event::TOPOLOGY_EVENT) {
        if (evt->get_Type() == TopologyEvent::TOPOL_REMOVE_NODE) {
            // fprintf(*err_stream_ptr, "MRNet [TOPOL_REMOVE_NODE] failure event
            // recieved.\n");
        } else if (evt->get_Type() == TopologyEvent::TOPOL_CHANGE_PARENT) {
            // fprintf(*err_stream_ptr, "MRNet [TOPOL_CHANGE_PARENT] failure
            // event recieved.\n");
        } else {
            return;
        }

        // lets handle the callback.

        // ensure that the failure_handler is not null
        if (failure_handler == NULL) return;

        // grab the lock, call the registered handler, then release
        pthread_mutex_lock(&failure_handler->failure_lock);

        if (failure_handler->failure_func != NULL)
            (failure_handler->failure_func)(cb_data);

        pthread_mutex_unlock(&failure_handler->failure_lock);
    }
}

extern "C" int network_get_num_callbacks() {
    int rtn;
    // grab the lock, read the value, then release
    // we had a data contention here before
    pthread_mutex_lock(&callback.callback_lock);
    rtn = callback.num_callbacks;
    pthread_mutex_unlock(&callback.callback_lock);

    return rtn;
}

extern "C" int network_register_TopAddBE_event_callback(
    MRNet_Network* network) {
    Network* pNetwork = (Network*)network;
    int rtn;

    callback.num_callbacks = 0;

    // setup the mutex lock
    rtn = pthread_mutexattr_init(&callback.callback_lock_attr);
    if (rtn) {
        // fprintf(*err_stream_ptr, "ERROR: pthread_mutexattr_init failed!\n");
        return 1;
    }

    rtn = pthread_mutex_init(&callback.callback_lock,
                             &callback.callback_lock_attr);
    if (rtn) {
        // fprintf(*err_stream_ptr, "ERROR: pthread_mutex_init failed!\n");
        return 1;
    }

    bool cbrett = pNetwork->register_EventCallback(Event::TOPOLOGY_EVENT,
                                                   TopologyEvent::TOPOL_ADD_BE,
                                                   BE_Add_Callback, NULL);

    if (cbrett == false) {
        // fprintf( *err_stream_ptr, "ERROR: Failed to register callback for
        // back-end add topology event!\n");
        return 1;
    }

    return 0;
}

extern "C" void network_remove_TopAddBE_event_callback(MRNet_Network* network) {
    Network* pNetwork = (Network*)network;

    // destroy the mutex lock
    pthread_mutexattr_destroy(&callback.callback_lock_attr);
    pthread_mutex_destroy(&callback.callback_lock);
    pNetwork->remove_EventCallback(BE_Add_Callback, Event::TOPOLOGY_EVENT,
                                   TopologyEvent::TOPOL_ADD_BE);
}

extern "C" int network_register_Failure_event_callback(MRNet_Network* network,
                                                       void (*callback)(void*),
                                                       void* data) {
    Network* pNetwork = (Network*)network;
    int rtn;

    // we only want to init the failure_handler if it hasen't been initialized
    // before.
    if (failure_handler == NULL) {
        failure_handler =
            (failure_handler_info_t*)malloc(sizeof(failure_handler_info_t));

        if (failure_handler == NULL) {
            // fprintf(*err_stream_ptr, "ERROR: malloc failed!\n");
            return 1;
        }

        failure_handler->failure_func = callback;

        // this needs to be made threadsafe...
        // we only want to initialize the mutex once.
        rtn = pthread_mutexattr_init(&failure_handler->failure_lock_attr);
        if (rtn) {
            // fprintf(*err_stream_ptr, "ERROR: pthread_mutexattr_init
            // failed!\n");
            return 1;
        }

        rtn = pthread_mutex_init(&failure_handler->failure_lock,
                                 &failure_handler->failure_lock_attr);
        if (rtn) {
            // fprintf(*err_stream_ptr, "ERROR: pthread_mutex_init failed!\n");
            return 1;
        }
    } else {
        failure_handler->failure_func = callback;
    }

    int cbrett = pNetwork->register_EventCallback(
        Event::TOPOLOGY_EVENT, TopologyEvent::TOPOL_REMOVE_NODE,
        Failure_Callback, data, true);

    int cbrett2 = pNetwork->register_EventCallback(
        Event::TOPOLOGY_EVENT, TopologyEvent::TOPOL_CHANGE_PARENT,
        Failure_Callback, data, true);

    return cbrett && cbrett2;
}

extern "C" int network_is_shutdown(MRNet_Network* network) {
    Network* pNetwork = (Network*)network;
    int res = pNetwork->is_ShutDown() ? 1 : 0;

    return res;
}

extern "C" void network_waitfor_shutdown(MRNet_Network* network) {
    Network* pNetwork = (Network*)network;

    pNetwork->waitfor_ShutDown();
}

extern "C" int network_set_failure_recovery(MRNet_Network* network, int on) {
    Network* pNetwork = (Network*)network;
    if (on)
        pNetwork->set_FailureRecovery(true);
    else
        pNetwork->set_FailureRecovery(false);

    return 0;
}
