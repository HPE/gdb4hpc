/*********************************************************************************\
 * interface of MRNet frontend for Guard client
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



#ifndef _GUARD_MRNET_FRONTEND_H
#define _GUARD_MRNET_FRONTEND_H

#include "dbg_mrn.h"
#include "mrn_topgen.h"

// Internal use only!!! This should be private.
struct mrnet_type
{
	MRNet_Network			*network;
	MRNet_Communicator		*comm_bc;
	MRNet_Stream			*cmd_stream;
	MRNet_Stream			*event_stream;
	MRNet_Stream			*io_stream;
	list_t					*mc_streams;	// list of multicast streams
	int						io_nfd;
	int						event_nfd;
	int						filter_id;
	int						num_be;			// number of dbgsrvs
	bitset					*bitset_full;
};
typedef struct mrnet_type mrnet_type;

/* launch MRNet frontend and backend */
extern mrnet_type *		mrn_fe_launch_with_filter(char *topology, char *filter_file, char* filter_name, char *backend_exe, char *backend_args[]);
extern mrnet_type * 	mrn_fe_launch(char *topology, char *backend_exe, char *backend_args[]);

/* launch MRNet fontend and attach backend. Currently, it is only available for ALPS */
#ifdef ALPS
extern mrnet_type * 	mrn_fe_create_network_no_be(char *topology_file, MRNet_Topology **ptopology, int apid);
#else
extern mrnet_type * 	mrn_fe_create_network_no_be(char *topology_file, MRNet_Topology **ptopology);
#endif
extern int 				mrn_fe_attach_be(mrnet_type *, int num_be, int timeout);
extern int 				mrn_fe_create_streams_and_load_filter(mrnet_type *, char *filter_file, char* filter_name);
extern int				mrn_fe_create_streams(mrnet_type *);

/*--------------------------------------------------------------------------*/
/*------ -The following APIs are valid only after MRNet is launched --------*/
/*--------------------------------------------------------------------------*/

/* return the number of backends in MRNet */
extern int				mrn_fe_get_num_of_be(mrnet_type *);

/* setup the command, event and I/O streams for debugging sessions */
extern int				mrn_fe_setup_streams(mrnet_type *, char *msg);

/* send commands to backends via the command stream*/
extern int				mrn_fe_send_command(mrnet_type *, bitset *target, char *msg);

/* collect replies from backends via the command stream*/
extern proto_msg_set * 	mrn_fe_recv_reply(mrnet_type *, bitset *target);

/* enable MRNet frontend to handle failures. */
extern void				mrn_fe_enable_failure_callback(mrnet_type *, dbginvoke_t *);

/* Obtain event notification fds for the io and event streams */
extern int				mrn_fe_get_io_notificationfd(mrnet_type *);
extern int				mrn_fe_get_event_notificationfd(mrnet_type *);

/* dispatch routines for streams */
extern proto_msg_set *	mrn_fe_dispatch_io(mrnet_type *, int notification_fd);
extern proto_msg_set *	mrn_fe_dispatch_event(mrnet_type *, int notification_fd);

/* shutdown one MRNet network */
extern void	mrn_fe_delete_network(mrnet_type *);

extern void	mrn_fe_debug_print_network_topology(mrnet_type *);
#endif /* _GUARD_MRNET_FRONTEND_H */

