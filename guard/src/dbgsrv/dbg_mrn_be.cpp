/*********************************************************************************\
 * dbg_mrn_be.c
 *
 * © 2010-2019 Cray Inc.  All Rights Reserved.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Cray Inc.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *
 *********************************************************************************/



#ifdef HAVE_CONFIG_H
#include    <config.h>
#endif /* HAVE_CONFIG_H */

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<ctype.h>

#include 	"defs.h"
#include	"alloc.h"
#include	"dbgevent.h"
#include 	"smart_bitset.h"

#include 	"MRNet_C.h"
#include 	"Network_C.h"
#include 	"Communicator_C.h"
#include 	"Stream_C.h"
#include 	"Packet_C.h"
#include	"dbg_mrn.h"
#include	"dbgsrv_defs.h"
#include	"dbgcmd.h"

class dbgcomm_info;

/*
* data structures for MRNet backend 
*/

struct mrnet_be_type
{
	MRNet_Network * network;

	MRNet_Stream *  bc_stream_cmd;
	MRNet_Stream *  bc_stream_event;

	MRNet_Stream *  stream_cmd;
	MRNet_Stream *  stream_event;
	MRNet_Stream *  stream_io;
	MRNet_Stream *  stream_single;  //TODO: using singlecast stream to reply simgle cast cmd
	int             cmd_fd;         // for notification of events on cmd stream
	int             event_fd;       // for notification of events on event stream
	int             io_fd;          // for notification of events on io stream       

	int 	        rank;			// rankd of backend 
	smart_bitset *	rank_bs;		// smart_bitset
	char *          rank_bs_str;

	int		network_size;

	list_t * 	cmd_streams;   // procset streams created for working with subset
	list_t *	event_streams; // asynch return streams for each of cmd_streams
};
typedef struct mrnet_be_type mrnet_be_type;

static mrnet_be_type*	mrnet_be = NULL;
static list_t*  	msg_list = NULL;

void (*dbgsrv_callback)(char* msg,const dbgcomm_info*) = NULL;

mrnet_be_type*
new_mrnet_be_type()
{
	mrnet_be_type* mrnet = (mrnet_be_type*)Alloc(sizeof(mrnet_be_type));

	mrnet->network = NULL;

	mrnet->bc_stream_cmd = NULL;
	mrnet->bc_stream_event = NULL;

	mrnet->stream_cmd = NULL;
	mrnet->stream_event = NULL;
	mrnet->stream_io = NULL;
	mrnet->stream_single = NULL;
	mrnet->event_fd = 0;

	mrnet->rank_bs = NULL;
	mrnet->rank_bs_str = NULL;

	mrnet->cmd_streams = NewList();
	mrnet->event_streams = NewList();
	
	return mrnet;
}

void
free_mrnet_be_type(mrnet_be_type* mrnet)
{
	if(mrnet == NULL)
		return;

	if(mrnet->rank_bs != NULL)
		smart_bitset_free(mrnet->rank_bs);
	
	Free(mrnet->rank_bs_str);

	Free(mrnet);
	DestroyList(mrnet->cmd_streams, NULL);
	DestroyList(mrnet->event_streams, NULL);
}

/*
** initialize backend 
*/
int 
mrn_be_create_network(int argc, char* argv[])
{
	mrnet_be = new_mrnet_be_type();
	msg_list = NewList();
	
	mrnet_be->network = network_create_BE(argc, argv);
	if(mrnet_be->network == NULL)
	{
		DVPRINT("ERROR: mrn_initialize_backend_node cannot create BE!\n");
		return -1;
	}

	mrnet_be->rank = network_get_local_rank(mrnet_be->network);
	DVPRINT("mrn_initialize_backend_node rank[%d]\n", mrnet_be->rank);
	
	mrnet_be->event_fd = network_get_EventNotificationFd(mrnet_be->network, MRNet_EVENT_DATA);

	return 0;
}

int
mrn_be_is_shutdown()
{
	return network_is_shutdown(mrnet_be->network);
}

void
mrn_be_waitfor_shutdown()
{
	network_waitfor_shutdown(mrnet_be->network);
}

int
mrn_be_get_localrank()
{
	return mrnet_be->rank;
}

void
mrn_be_set_localrank(int rank)
{
	mrnet_be->rank = rank;
	MRNet_Stream* single = network_get_stream(mrnet_be->network, rank + ATTACHBE_RANK_SKEW);
	mrnet_be->stream_single = single;
	DVPRINT("INFO: [dbgsrv] mrnet_be->stream_single is set [%p].\n", single);
}

// Set the current command stream for the backend.
// The first call is part of the bootstrapping and is given the broadcast communicator 
// that is attached to every back-end.
// New streams are created whenever the user sends a command using a new procset or focus.
// For performance reasons, we want to be sure all responses go back on the same stream, 
// and any asynchronous events are triggered on the same communicator - by restricting the 
// focus, we know that MRNet won't be waiting on any rank not involved in the command.
void
mrn_be_set_cmd_stream(MRNet_Stream *stream)
{
	if (stream == mrnet_be->stream_cmd)
		return;

	unsigned int streamId = stream_get_Id(stream);
	
	MRNet_Stream* event_stream = NULL;
	if (!mrnet_be->bc_stream_cmd) {
		// the first registered stream is broadcast stream connected
		// to every rank
		DVPRINT("INFO: [dbgsrv] mrnet_be->stream_cmd is set to %u.\n", streamId);
		mrnet_be->bc_stream_cmd = stream;
	} else if (mrnet_be->stream_cmd == mrnet_be->bc_stream_cmd) {
		DVPRINT("INFO: [dbgsrv] mrnet_be->stream_event set back to broadcast stream\n");
		event_stream = mrnet_be->bc_stream_event;
	} else {
		list_t* cs = mrnet_be->cmd_streams;
		list_t* es = mrnet_be->event_streams;
		SetList(cs);
		SetList(es);
		for (MRNet_Stream* cmd, *event; (cmd=(MRNet_Stream*)GetListElement(cs)) &&
						(event=(MRNet_Stream*)GetListElement(es)); ) {
			if (cmd == stream) {
				event_stream = event;
				break;
			}
		}

		if (event_stream == NULL) {
			DVPRINT("INFO: [dbgsrv] no event stream for %u, using default\n", streamId);
			event_stream = mrnet_be->bc_stream_event;
		} else {
			DVPRINT("INFO: [dbgsrv] found event stream for command stream %u,%u\n",
				streamId, stream_get_Id(event_stream));
		}
			
		mrnet_be->stream_event = event_stream;
	}
	if (event_stream != NULL)
		mrnet_be->stream_event = event_stream;

	mrnet_be->stream_cmd = stream;	
	}

// the first call to this function sets the default event stream for the back-end, 
// which is connected to all of back ends.
// Any additional calls are setting an event stream that matches the current procSet.
// Note that "event" in this case is not the same as the MRNet events, this stream
// is for sending asynchronous debugger events (e.g. breakpoint-hit) 
void
mrn_be_set_event_stream(MRNet_Stream *stream)
{
	if (!stream || stream == mrnet_be->stream_event)
		return;
	
	// current command stream
	MRNet_Stream* cmd = mrnet_be->stream_cmd;

	if (!mrnet_be->bc_stream_event) {
		// the first registered stream is the default (broadcast) stream
		mrnet_be->bc_stream_event = stream;
		DVPRINT("INFO: [dbgsrv] mrnet_be->event_stream is set %u.\n", stream_get_Id(stream));

	} else if (cmd && !InList(mrnet_be->cmd_streams, (void*)cmd)) {
		DVPRINT("INFO: [dbgsrv] creating cmd/event stream pair %d, %d\n",
			stream_get_Id(cmd), stream_get_Id(stream));

		AddToList(mrnet_be->cmd_streams, cmd);
		AddToList(mrnet_be->event_streams, stream);
	}
	mrnet_be->stream_event = stream;	
}

void
mrn_be_set_event_stream_from_id(unsigned int streamId)
{
	MRNet_Network* network = mrnet_be->network;
	if (!network) {
		DVPRINT("Network on server side is not set\n");
		return;
	}

	if (MRNet_Stream* event_stream = network_get_stream(network, streamId)) {
		mrn_be_set_event_stream(event_stream);
	} else {
		DVPRINT("Could not get event stream from event stream id!\n");
	}
}

void 
mrn_be_set_io_stream(MRNet_Stream *stream)
{
	mrnet_be->stream_io = stream;
	DVPRINT("INFO: [dbgsrv] mrnet_be->io_stream is set.\n");
}

int
mrn_be_get_event_fd()
{
	return mrnet_be->event_fd;
}

void
mrn_be_clear_event_fd()
{
	network_clear_EventNotificationFd(mrnet_be->network, MRNet_EVENT_DATA);
}

unsigned int
mrn_be_num_events_pending()
{
	return network_get_num_events_pending(mrnet_be->network);
}

void
mrn_be_ack_event()
{
	return network_get_next_event(mrnet_be->network);
}

void
mrn_be_clear_events()
{
	return network_clear_events(mrnet_be->network);
}

void
mrn_be_set_size(int size)
{
	DVPRINT("mrn_be_set_size is called. size[%d]\n", size);

	mrnet_be->network_size = size;
	mrnet_be->rank_bs = smart_bitset_new(size);

	smart_bitset_set(mrnet_be->rank_bs, mrnet_be->rank);	
	mrnet_be->rank_bs_str = smart_bitset_serialize(mrnet_be->rank_bs);
}

int 
mrn_be_recv(MRNet_Stream **stream, int *tag, int blocking)
{
	MRNet_Stream *stream_be = NULL;
	MRNet_Packet *packet_be = NULL;
	int tag_be = -1;

	network_recv(mrnet_be->network, &tag_be, &packet_be, &stream_be, blocking);

    char* msg = NULL;
    if( packet_be != NULL && packet_unpack(packet_be, "%s", &msg) == -1 ){
        fprintf( stderr, "mrn_be_recv packet_unpack() failure\n");
        return -1;
    }

#ifdef DEBUG
	if(msg != NULL)
		fprintf(stderr, "INFO: [dbgsrv] mrn_be_recv: [%s]\n", msg);
#endif

	if(msg != NULL){
		AddToList(msg_list, msg);
		*stream = stream_be;
		*tag = tag_be;
		return 1;
	}

	return 0;
}

int 
mrn_be_send_reply(MRNet_Stream *stream, char* msg)
{
	if(mrnet_be->stream_cmd == NULL)
		return -1;

	DVPRINT("INFO: [dbgsrv] mrn_be_send_reply [%s], via stream[%u]\n", msg, stream_get_Id(stream));

	char* strArr[2];
	strArr[0] = msg;
	strArr[1] = mrnet_be->rank_bs_str;

	if(stream == NULL)
		stream = mrnet_be->stream_cmd;

	if( stream_send(stream, DBG_MRNet_PRO_COMMAND, "%as", strArr, 2) == -1 )
	{
		fprintf(stderr, "ERROR: mrn_be_send_reply stream_send(%s) failed!\n", msg);
		return -1;
	}

	if( stream_flush(stream) == -1 ){
		fprintf(stderr, "ERROR: mrn_be_send_reply stream_flush() failed!\n");
		return -1;
    }

	return 0;
}

int 
mrn_be_send_event(char* msg)
{
	if(mrnet_be->stream_event == NULL)
		return -1;

	DVPRINT("INFO: [dbgsrv] mrn_be_send_event [%s] via stream[%u]\n", msg, 
				stream_get_Id(mrnet_be->stream_event));

	char* strArr[2];
	strArr[0] = msg;
	strArr[1] = mrnet_be->rank_bs_str;

	if( stream_send(mrnet_be->stream_event, DBG_MRNet_PRO_EVENT, "%as", strArr, 2) == -1 )
	{
		fprintf(stderr, "ERROR: mrn_be_send_event stream_send(%s) failed!\n", msg);
		return -1;
	}

	if( stream_flush(mrnet_be->stream_event) == -1 ){
		fprintf(stderr, "ERROR: mrn_be_send_event stream_flush() failed!\n");
		return -1;
    }

	return 0;
}

int 
mrn_be_send_io(char* msg)
{
	char *strArr[2];

	if(mrnet_be->stream_io == NULL)
		return -1;

	DVPRINT("INFO: [dbgsrv] mrn_be_send_io [%s] via stream[%p]\n", msg, mrnet_be->stream_io);

	strArr[0] = msg;
	strArr[1] = mrnet_be->rank_bs_str;

	if( stream_send(mrnet_be->stream_io, DBG_MRNet_PRO_IO, "%as", strArr, 2) == -1 )
	{
		fprintf(stderr, "ERROR: mrn_be_send_io stream_send(%s) failed!\n", msg);
		return -1;
	}

	if( stream_flush(mrnet_be->stream_io) == -1 )
	{
		fprintf(stderr, "ERROR: mrn_be_send_io stream_flush() failed!\n");
		return -1;
    }

	return 0;
}

void
mrn_be_set_callback(void (*callback)(char*,const dbgcomm_info*))
{
	dbgsrv_callback = callback;
	return;
}

void
mrn_be_handle_msg()
{
	char* msg = (char*)RemoveFirst(msg_list);	
	
	if(msg != NULL) {
		int cmdIdx = 0;
		if (sscanf(msg, "%d", &cmdIdx) == 1 && cmdIdx == DBGCMD_SETCOMMPARAM) {
			// strip off the command number and toss it back into the mrn
			// specific processing.  I don't think we need to fire up 
			// regex or strtok to pull of one number.
			char* submsg = msg;
			for ( ; *submsg && isspace(*submsg); ++submsg) {}
			for ( ; *submsg && isdigit(*submsg); ++submsg) {}
			for ( ; *submsg && isspace(*submsg); ++submsg) {}
			DbgSRVMRNSetParam(submsg);
		} else {
			// push all other commands to the back-end.
			dbgsrv_callback(msg, NULL);
		}
	}

	return;
}

int 
mrn_be_shutdown()
{
	network_close_EventNotificationFd(mrnet_be->network, MRNet_EVENT_DATA);
	network_delete(mrnet_be->network);

	free_mrnet_be_type(mrnet_be);
	DestroyList(msg_list, NULL);

	return 0;
}
