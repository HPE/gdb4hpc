/*********************************************************************************\
 * dbg_mrn_be.c
 *
 * © 2010-2013 Cray Inc.  All Rights Reserved.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Cray Inc.,
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
#include    <config.h>
#endif /* HAVE_CONFIG_H */

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>

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

/*
* data structures for MRNet backend 
*/

struct mrnet_be_type
{
	MRNet_Network * network;

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
};
typedef struct mrnet_be_type mrnet_be_type;

static mrnet_be_type*	mrnet_be = NULL;
static list_t*  	msg_list = NULL;

void (*dbgsrv_callback)(char* msg) = NULL;

mrnet_be_type*
new_mrnet_be_type()
{
	mrnet_be_type* mrnet = (mrnet_be_type*)Alloc(sizeof(mrnet_be_type));

	mrnet->network = NULL;

	mrnet->stream_cmd = NULL;
	mrnet->stream_event = NULL;
	mrnet->stream_io = NULL;
	mrnet->stream_single = NULL;
	mrnet->event_fd = 0;

	mrnet->rank_bs = NULL;
	mrnet->rank_bs_str = NULL;
	
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


void
mrn_be_set_cmd_stream(MRNet_Stream *stream)
{
	mrnet_be->stream_cmd = stream;	
	DVPRINT("INFO: [dbgsrv] mrnet_be->stream_cmd is set.\n");
}

void
mrn_be_set_event_stream(MRNet_Stream *stream)
{
	mrnet_be->stream_event = stream;	
	DVPRINT("INFO: [dbgsrv] mrnet_be->event_stream is set.\n");
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

	DVPRINT("INFO: [dbgsrv] mrn_be_send_reply [%s], via stream[%p]\n", msg, stream);

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

	DVPRINT("INFO: [dbgsrv] mrn_be_send_event [%s] via stream[%p]\n", msg, mrnet_be->stream_event);

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
mrn_be_set_callback(void (*callback)(char*))
{
	dbgsrv_callback = callback;
	return;
}

void
mrn_be_handle_msg()
{
	char* msg = (char*)RemoveFirst(msg_list);	
	
	if(msg != NULL)
		dbgsrv_callback(msg);

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
