/******************************************************************************\
 * proxy_daemon.c - Debugger proxy daemon
 *
 * © 2013 Cray Inc.  All Rights Reserved.
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
 ******************************************************************************/

#include <fcntl.h>
#include <getopt.h>
#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <errno.h>

#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <netinet/tcp.h>

#include "dbgproxy.h"
#include "dbg_client.h"
#include "useful.h"

#include "proxy.h"

#define RECONNECT_TRIES		10

/*
** global variables
*/
FILE *			err_stream = NULL;
list_t *		instances = (list_t *)NULL;
int				Quit = 0;

/*
** Static global variables
*/
static proxy_session *		sess = NULL;
static struct addrinfo *	host;	// addrinfo object for hostname
static fd_set				dispatch_fds;
static int					high_fd;

/*
** long opts definitions
*/
static const struct option proxy_long_opts[] = {
			{"hostname",	optional_argument,	0, 'h'},
			{"port",		optional_argument,	0, 'p'},
			{0, 0, 0, 0}
			};

struct proxy_instance
{
	int				clnt_id;	// id used by the client to identify this instance
	proxy_type *	inst;		// instance of a proxy_type associated with an attach/invoke
};
typedef struct proxy_instance proxy_instance;

static void
NewProxyInstance(int id, proxy_type *inst)
{
	// Alloc the obj
	proxy_instance *	obj = (proxy_instance *)Alloc(sizeof(proxy_instance));
	// memset everything to 0
	memset(obj, 0, sizeof(proxy_instance));
	
	obj->clnt_id = id;
	obj->inst = inst;
	
	AddToList(instances, obj);
}

static void
FreeProxyInstance(proxy_type *inst)
{
	proxy_instance *	obj;	
	
	if (inst == NULL)
		return;
		
	// find the obj associated with this instance
	SetList(instances);
	while ((obj = (proxy_instance *)GetListElement(instances)) != NULL)
	{
		if (obj->inst == inst)
		{
			RemoveFromList(instances, obj);
			
			FreeProxyType(obj->inst);
			Free(obj);
			
			return;
		}
	}
}

static proxy_type *
GetProxyTypeFromClntId(int id)
{
	proxy_instance *	obj;

	SetList(instances);
	
	while ((obj = (proxy_instance *)GetListElement(instances)) != NULL)
	{
		if (obj->clnt_id == id)
			return obj->inst;
	}
	
	return NULL;
}

static proxy_type *
GetProxyTypeFromInstId(int id)
{
	proxy_instance *	obj;
	
	SetList(instances);
	
	while ((obj = (proxy_instance *)GetListElement(instances)) != NULL)
	{
		if (PROXY_ID(obj->inst) == id)
			return obj->inst;
	}
	
	return NULL;
}

static void
CleanupOnSocketClose(void)
{
	dbginvoke_t *		di = NULL;
	proxy_instance *	obj = NULL;

	// find the dbginvoke_t associated with this id
	if (!EmptyList(DbgInvokeList))
	{
		// Reset the DbgInvokeList
		SetList(DbgInvokeList);
		
		// Loop through each dbginvoke entry and call its exit function
		while ( (di = (dbginvoke_t *)GetListElement(DbgInvokeList)) != (dbginvoke_t *)NULL )
		{
			// ensure the exit function is implemented
			if (DBG_PROTO_SW(di)->sw_exit != NULL)
			{
				// call the exit function
				// FIXME: This forces things to quit instead of detach. I think
				// the dbginvoke should probably hold onto information if we attached
				// or launched the application.
				DBG_PROTO_SW(di)->sw_exit(di, 0);
			}
				
			// Cleanup the dbginvoke, we are done with it.
			FreeDbgInvoke(di);
		}
	}
	
	// Free all the instances
	if (!EmptyList(instances))
	{
		SetList(instances);
	
		while ((obj = (proxy_instance *)GetListElement(instances)) != NULL)
		{
			FreeProxyInstance(obj->inst);
			// Need to reset the list since it will be messed up in the function call
			SetList(instances);
		}
	}

	_exit(1);
}

int
main(int argc, char **argv)
{
	// arguments from argv
	int					opt;
	int					opt_ind = 0;
	char *				hostname = NULL;
	char *				port = NULL;
	// other stuff
	int					rc, sopt;
	struct addrinfo		hints;	// hints object for call to getaddrinfo
	int					tries = 0;
	dbgclnt_proto *		ptab;
	
	// Setup
	
	close(0);
	close(1);
	close(2);
	open("/dev/null", O_RDONLY);
	open("/dev/null", O_WRONLY);
	open("/dev/null", O_WRONLY);
	
	memset(&hints, 0, sizeof(hints));	// ensure the hints struct is cleared
	hints.ai_family = AF_UNSPEC;		// use AF_INET6 to force IPv6
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_NUMERICSERV;
	
	//
	// Process user provided arguments
	//
	while ((opt = getopt_long(argc, argv, "h:p:", proxy_long_opts, &opt_ind)) != -1)
	{
		switch(opt)
		{
			case 0:
				// if this is a flag, do nothing
				break;
			case 'h':
				if (optarg == (char *)NULL)
				{
					PrintOutError("Error: --hostname provided, but missing argument.\n");
					return 1;
				}
				hostname = strdup(optarg);
				break;
				
			case 'p':
				if (optarg == (char *)NULL)
				{
					PrintOutError("Error: --port provided, but missing argument.\n");
					return 1;
				}
				port = strdup(optarg);
				break;
			
			default:
				PrintOutError("Invalid argument provided.\n");
				return 1;
		}
	}
	
	// ensure arguments were provided
	if (hostname == NULL)
	{
		PrintOutError("Error: Missing argument --hostname.\n");
		return 1;
	}
	
	if (port == NULL)
	{
		PrintOutError("Error: Missing argument --port.\n");
		return 1;
	}
	
	// get the addrinfo of the hostname
	if ((rc = getaddrinfo(hostname, port, &hints, &host)) != 0)
	{
		fprintf(stderr, "Getaddrinfo failed: %s\n", gai_strerror(rc));
		// Handle error
		return 1;
	}
	
	// Create a new session
	sess = NewProxySession();

	// Create the cmd socket
	if ((PROXY_SESSION_CMDFD(sess) = socket(host->ai_family, host->ai_socktype, host->ai_protocol)) < 0)
	{
		perror("socket");
		// Handle error
		return 1;
	}
	
	// Set the command socket to TCP_NODELAY
	sopt = 1;
	if (setsockopt(PROXY_SESSION_CMDFD(sess), IPPROTO_TCP, TCP_NODELAY, &sopt, sizeof(sopt)) < 0) 
	{
		// Non-fatal error
		perror("setsockopt(TCP_NODELAY)");
	}
	
	// Connect the cmd socket to host
	while (connect(PROXY_SESSION_CMDFD(sess), host->ai_addr, host->ai_addrlen)) 
	{
		perror("connect");
		sleep(1);
		if (tries++ == RECONNECT_TRIES)
		{
			// Handle error
			return 1;
		}
	}
	
	tries = 0;
	
	// Create the error socket
	if ((PROXY_SESSION_ERRORFD(sess) = socket(host->ai_family, host->ai_socktype, host->ai_protocol)) < 0)
	{
		perror("socket");
		return 1;
	}
	
	// Set the error socket to TCP_NODELAY
	sopt = 1;
	if (setsockopt(PROXY_SESSION_ERRORFD(sess), IPPROTO_TCP, TCP_NODELAY, &sopt, sizeof(sopt)) < 0) 
	{
		// Non-fatal error
		perror("setsockopt(TCP_NODELAY)");
	}
				
	// Connect the error socket to host
	while (connect(PROXY_SESSION_ERRORFD(sess), host->ai_addr, host->ai_addrlen)) 
	{
		perror("connect");
		sleep(1);
		if (tries++ == RECONNECT_TRIES)
		{
			return 1;
		}
	}
	
	// Create a file stream for the error socket.
	PROXY_SESSION_ERRORSTREAM(sess) = fdopen(PROXY_SESSION_ERRORFD(sess), "w");
	
	// Set the stream to be line buffered
	setlinebuf(PROXY_SESSION_ERRORSTREAM(sess));
	
	// set the global error stream
	err_stream = PROXY_SESSION_ERRORSTREAM(sess);
	
	// Create the message socket
	if ((PROXY_SESSION_MSGFD(sess) = socket(host->ai_family, host->ai_socktype, host->ai_protocol)) < 0)
	{
		perror("socket");
		return 1;
	}
	
	// Set the message socket to TCP_NODELAY
	sopt = 1;
	if (setsockopt(PROXY_SESSION_MSGFD(sess), IPPROTO_TCP, TCP_NODELAY, &sopt, sizeof(sopt)) < 0) 
	{
		// Non-fatal error
		perror("setsockopt(TCP_NODELAY)");
	}
	
	// Connect the message socket to host
	while (connect(PROXY_SESSION_MSGFD(sess), host->ai_addr, host->ai_addrlen)) 
	{
		perror("connect");
		sleep(1);
		if (tries++ == RECONNECT_TRIES)
		{
			return 1;
		}
	}
	
	// Create a file stream for the message socket.
	PROXY_SESSION_MSGSTREAM(sess) = fdopen(PROXY_SESSION_MSGFD(sess), "w");
	
	// Set the stream to be line buffered
	setlinebuf(PROXY_SESSION_MSGSTREAM(sess));
	
	// create a new instance list to hold onto proxy_type instances
	instances = NewList();
	
	// initialize the protocols
	// TODO: Allow this to select any protocol, other than the hardcoded
	//       mrnet we see now.
	if ( (ptab = FindClntProto("mrnet")) == (dbgclnt_proto *)NULL )
	{
		// Handle error
		return 1;
	}
	
	// call the init function
	if ( ptab->sw_init != NULL && !ptab->sw_init() )
	{
		// Handle error
		return 1;
	}
	
	for ( ;; )
	{
		switch ( DbgCheckEvents() )
		{
			case -1:
				Quit = 1;

			case 0:
				// we only want to continue if we haven't triggered the Quit condition.
				if (!Quit)
				{
					continue;
				}
				break;
			
			default:
				DbgDispatchEvents();
		}
		
		if (Quit)
			break;
	}
	
	// call the finish function
	// TODO: Allow this to select any protocol.
	if (ptab->sw_finish != NULL)
	{
		ptab->sw_finish();
	}
	
	return 0;
}

/*
** Main debugger event handler. 
**
** Returns: -1 if an error occurred
**		0 if there is nothing to do
**		1 if there are callbacks to process
**
** This now will check on dbginvoke_t objects rather than on generic protocol
** layer.
*/
int
DbgCheckEvents(void)
{
	dbginvoke_t *		di;
	int					status;
	int	*				ev_fd;
	struct timeval		tv = {1, 0};

	// reset the fd_set
	FD_ZERO(&dispatch_fds);
	high_fd = -1;

	/*
	** Check if any events are ready.
	*/
	
	// Check the protocols for each dbginvoke if there are any.
	if (!EmptyList(DbgInvokeList))
	{
		// Reset the DbgInvokeList
		SetList(DbgInvokeList);

		// Loop through each dbginvoke entry and check for events.
		while ( (di = (dbginvoke_t *)GetListElement(DbgInvokeList)) != (dbginvoke_t *)NULL )
		{
			/*
			** Note: This should be used for protocols that do not use an event notification
			**       fd. If a event notification fd can be used, then it should be added to
			**       the sw_evfd list and sw_checkevents should simply return 0.
			*/
			if ( 	DBG_PROTO_SW(di)->sw_checkevents != NULL	// if the protocol has a checkevent function defined
					&&
					DBG_PROTO_SW(di)->sw_checkevents(di) > 0	// if checkevent tells us that there is an event.
				)
			{
				DPRINT("DbgCheckEvents: sw_checkevents returned 1\n");
				return 1;	// there is a proto event to handle. Return immediately.
			}
			
			/*
			** If there is a d_evfds fd set, we want to add it to the fd_set to select on.
			** This enables protocol events.
			*/
			if ( di->d_evfds != NULL )
			{
				SetList(di->d_evfds);
			
				while ( (ev_fd = (int *) GetListElement(di->d_evfds)) != (int *)NULL )
				{
					if (high_fd < *ev_fd)
					{
						high_fd = *ev_fd;
					}
					FD_SET(*ev_fd, &dispatch_fds);
				}
			}
		}
	}
	
	/*
	** Add the cmd stream fd of the main proxy object to the fd_set to select on. This enables commands.
	*/
	if (high_fd < PROXY_SESSION_CMDFD(sess))
	{
		high_fd = PROXY_SESSION_CMDFD(sess);
	}
	
	FD_SET(PROXY_SESSION_CMDFD(sess), &dispatch_fds);

	/*
	** Main select event polling loop.
	*/
	for ( ;; )
	{
		// from the select manpage, nfds is the highest-numbered file descriptor in any of the three sets, plus 1. 
		status = select(high_fd+1, &dispatch_fds, NULL, NULL, &tv);

		switch ( status )
		{
			case -1:
				switch (errno)
				{
					case EINTR:
						DPRINT("DbgCheckEvents: EINTR\n");
						continue;
					
					default:
						//DbgSetClntErr(DBGERR_SELECT, strerror(errno));
						return -1;
				}
				break;
				
			case 0:
				return 0;
				
			default:
				return 1;
		}
	}
	
	// We should never reach here...
	DPRINT("DbgCheckEvents: At final return\n");
	return 1;
}

/*
** Dispactch pending debugger events.
*/
void
DbgDispatchEvents(void)
{
	dbginvoke_t *	di;
	int *			ev_fd;
	
	/*
	** Now dispatch any callback routines that are active.
	*/
	if (!EmptyList(DbgInvokeList))
	{
		// Reset the DbgInvokeList
		SetList(DbgInvokeList);

		// Loop through each dbginvoke entry and check for events.
		while ( (di = (dbginvoke_t *)GetListElement(DbgInvokeList)) != (dbginvoke_t *)NULL )
		{
			// Ensure there is a protocol specific dispatch function
			if (DBG_PROTO_SW(di)->sw_dispatchevents != NULL)
			{
				// If there is an d_evfds list, then the protol layer is using event notification
				// fd's. We should try to check to see if each one is set, and if so call the
				// protocol specific dispatch function with the event fd as an argument
				if (di->d_evfds != NULL)
				{
					SetList(di->d_evfds);
				
					while ((ev_fd = (int *) GetListElement(di->d_evfds)) != (int *)NULL )
					{
						if ( FD_ISSET(*ev_fd, &dispatch_fds) )
						{
							// dispatch each of the set notification fds one at a time.
							DBG_PROTO_SW(di)->sw_dispatchevents(di, *ev_fd);
						}
					}
				} else
				{
					// event notification fd's not in use by this protocol, so call
					// the generic dispatch function with -1 for event fd arg.
					DBG_PROTO_SW(di)->sw_dispatchevents(di, -1);
				}
			}
		}
	}
	
	/*
	** Process any commands
	*/
	if (FD_ISSET(PROXY_SESSION_CMDFD(sess), &dispatch_fds))
	{
		proxy_msg_header	msg_head;
		char *				msg;
		proto_msg_set *		pmsg_set;
		proto_message *		pmsg;
		bitset *			set;
		recv_type *			recv_set;
		attach_args *		att_args;
		invoke_args *		inv_args;
		dbgclnt_proto *		ptab;
		proxy_type *		inst;
		int					sopt, tries=0;
		int					detach;
		int					n = 0;
		
		// ensure there is data to read on the cmd session socket. If there isn't, that means
		// the socket was closed. In which case we should force things to quit since we lost
		// our communication mechanism with the client.
		if (ioctl(PROXY_SESSION_CMDFD(sess), FIONREAD, &n) < 0)
		{
			PrintOutError("ioctl failed.\n");
			CleanupOnSocketClose();
			return;
		}
		if (n < 1)
		{
			CleanupOnSocketClose();
		}
	
		// First recv the header
		if (recv(PROXY_SESSION_CMDFD(sess), &msg_head, sizeof(proxy_msg_header), MSG_WAITALL) < 0)
		{
			PrintOutError("Failed to recv on event socket.\n");
			return;
		}
		
		// Next recv the message
		
		// Create a string buffer to receive the message
		msg = (char *)Alloc(msg_head.msg_len);
		
		// recv the message
		if (recv(PROXY_SESSION_CMDFD(sess), msg, msg_head.msg_len, MSG_WAITALL) < 0)
		{
			PrintOutError("Failed to recv on event socket.\n");
			Free(msg);
			return;
		}
		
		// switch on the msg_type
		switch (msg_head.msg_type)
		{
			case MSG_SEND_CMD:
				// convert msg to a proto message
				if ((pmsg = SocProtoToProtoMsg(msg)) == NULL)
				{
					PrintOutError("Could not convert cmd stream msg to proto msg.\n");
					Free(msg);
					return;
				}
		
				// cleanup
				Free(msg);
				
				// find the dbginvoke_t associated with this id
				inst = GetProxyTypeFromClntId(msg_head.msg_id);
				di = NULL;
				if (!EmptyList(DbgInvokeList))
				{
					// Reset the DbgInvokeList
					SetList(DbgInvokeList);

					// Loop through each dbginvoke entry and check for events.
					while ( (di = (dbginvoke_t *)GetListElement(DbgInvokeList)) != (dbginvoke_t *)NULL )
					{
						if (di->d_proto_id == PROXY_ID(inst))
							break;
					}
				}
				
				// TODO: Handle error
				
				// ensure the send_cmd function is implemented
				if ( DBG_PROTO_SW(di)->sw_send_cmd == NULL )
				{
					// TODO: Handle error
					PrintOutError("send cmd not implemented.\n");
					return;
				}
				
				// Send the command
				set = DBG_PROTO_SW(di)->sw_send_cmd(di->d_proto_net, pmsg->group, pmsg->message);
				
				// cleanup
				FreeProtoMsg(pmsg);
				
				// send a response back containing the set
				
				pmsg = NewProtoMsg(set, strdup(""));
				msg = ProtoMsgToSocProto(pmsg);
				
				msg_head.msg_type = MSG_OK;
				msg_head.msg_len = (int)strlen(msg) * sizeof(char);
				
				// Send the header
				if (send(PROXY_SESSION_CMDFD(sess), &msg_head, sizeof(proxy_msg_header), 0) != sizeof(proxy_msg_header))
				{
					PrintOutError("Could not send header on cmd stream.\n");
					return;
				}
				
				// Send the msg
				if (send(PROXY_SESSION_CMDFD(sess), msg, msg_head.msg_len, 0) != msg_head.msg_len)
				{
					PrintOutError("Could not send msg on cmd stream.\n");
					return;
				}
				
				// cleanup
				FreeProtoMsg(pmsg);
				Free(msg);
				
				break;
			
			case MSG_RECV_REPLY:
				// convert msg to a recv type
				if ((recv_set = SocProtoToRecv(msg)) == NULL)
				{
					PrintOutError("Could not convert cmd stream msg to recv type.\n");
					Free(msg);
					return;
				}
				
				// cleanup
				Free(msg);
				
				// find the dbginvoke_t associated with this id
				inst = GetProxyTypeFromClntId(msg_head.msg_id);
				di = NULL;
				if (!EmptyList(DbgInvokeList))
				{
					// Reset the DbgInvokeList
					SetList(DbgInvokeList);

					// Loop through each dbginvoke entry and check for events.
					while ( (di = (dbginvoke_t *)GetListElement(DbgInvokeList)) != (dbginvoke_t *)NULL )
					{
						if (di->d_proto_id == PROXY_ID(inst))
							break;
					}
				}
				
				// TODO: Handle error
				
				// ensure the recv_reply function is implemented
				if ( DBG_PROTO_SW(di)->sw_recv_reply == NULL )
				{
					// TODO: handle error
					PrintOutError("recv reply not implemented.\n");
					return;
				}
			
				pmsg_set = DBG_PROTO_SW(di)->sw_recv_reply(di->d_proto_net, recv_set->target, recv_set->received);
				
				// send the result back
				
				ResetProtoMsgSet(pmsg_set);
				while ((pmsg = NextProtoMsgInSet(pmsg_set)) != NULL)
				{
					// Send the header
					msg_head.msg_type = MSG_OK;
					msg = ProtoMsgToSocProto(pmsg);
					msg_head.msg_len = (int)strlen(msg) * sizeof(char);
					
					if (send(PROXY_SESSION_CMDFD(sess), &msg_head, sizeof(proxy_msg_header), 0) != sizeof(proxy_msg_header))
					{
						PrintOutError("Could not send header on cmd stream.\n");
						return;
					}
					
					// Send the msg
					if (send(PROXY_SESSION_CMDFD(sess), msg, msg_head.msg_len, 0) != msg_head.msg_len)
					{
						PrintOutError("Could not send msg on cmd stream.\n");
						return;
					}
					
					// cleanup
					Free(msg);
				}
				
				// cleanup
				FreeProtoMsgSet(pmsg_set);
				break;
				
			case MSG_ATTACH:
				// convert msg to an attach_args type
				if ((att_args = SocProtoToAttachArgs(msg)) == NULL)
				{
					PrintOutError("Could not convert cmd stream msg to recv type.\n");
					Free(msg);
					return;
				}

				// Create a new proxy object associated with this invoke
				inst = NewProxyType(sess);

				// Create the event socket
				if ((PROXY_EVENTFD(inst) = socket(host->ai_family, host->ai_socktype, host->ai_protocol)) < 0)
				{
					perror("socket");
					return;
				}
				
				// Set the event socket to TCP_NODELAY
				sopt = 1;
				if (setsockopt(PROXY_EVENTFD(inst), IPPROTO_TCP, TCP_NODELAY, &sopt, sizeof(sopt)) < 0) 
				{
					// Non-fatal error
					perror("setsockopt(TCP_NODELAY)");
				}
				
				// Create the output socket
				if ((PROXY_OUTPUTFD(inst) = socket(host->ai_family, host->ai_socktype, host->ai_protocol)) < 0)
				{
					perror("socket");
					return;
				}
	
				// Set the output socket to TCP_NODELAY since this will output fstream data
				sopt = 1;
				if (setsockopt(PROXY_OUTPUTFD(inst), IPPROTO_TCP, TCP_NODELAY, &sopt, sizeof(sopt)) < 0) 
				{
					// Non-fatal error
					perror("setsockopt(TCP_NODELAY)");
				}
	
				/* Connect the sockets */
	
				// Connect the event socket to host
				while (connect(PROXY_EVENTFD(inst), host->ai_addr, host->ai_addrlen)) 
				{
					perror("connect");
					sleep(1);
					if (tries++ == RECONNECT_TRIES)
					{
						return;
					}
				}
				
				tries = 0;
	
				// Connect the output socket to host
				while (connect(PROXY_OUTPUTFD(inst), host->ai_addr, host->ai_addrlen)) 
				{
					perror("connect");
					sleep(1);
					if (tries++ == RECONNECT_TRIES)
					{
						return;
					}
				}
	
				// Create a file stream for the output socket.
				PROXY_OUTPUTSTREAM(inst) = fdopen(PROXY_OUTPUTFD(inst), "w");
	
				// Set the stream to be line buffered
				setlinebuf(PROXY_OUTPUTSTREAM(inst));
	
				// Set the protocol based on the proto arg
				// TODO: Allow this to select any protocol, other than the hardcoded
				//       mrnet we see now.
				if ( (ptab = FindClntProto("mrnet")) == (dbgclnt_proto *)NULL )
				{
					// Handle error
					return;
				}
	
				// Call the protocol specific attach function
				// TODO: This could be modified to allow for multiple proxy invocations,
				//       to support multiple hops, but that is not required right now...
				di = ptab->sw_attach(	ptab, NULL, att_args->pid, att_args->arch, att_args->backend,
										att_args->gpu_flag, att_args->debug_flag
									);
									
				// cleanup attach args
				FreeAttachArgs(att_args);
									
				if (di == NULL)
				{
					// Handle error
					return;
				}
				
				// set the proto id in the di
				di->d_proto_id = PROXY_ID(inst);
				
				// Create a proxy_instance object to associate the client id with our id
				NewProxyInstance(msg_head.msg_id, inst);
				
				// Send the DBG_OK event for the entire set
				set = bitset_new(di->d_nproc);
				bitset_invert(set);
				pmsg_set = NewProtoMsgSet();
				AddProtoMsgSet(pmsg_set, NewProtoMsg(set, EventToSocProto(NewEvent(DBG_OK))));
				DbgDispatchProtoEvents(di, pmsg_set);
				
				break;
			
			case MSG_INVOKE:
				// convert msg to an invoke_args type
				if ((inv_args = SocProtoToInvokeArgs(msg)) == NULL)
				{
					PrintOutError("Could not convert cmd stream msg to recv type.\n");
					Free(msg);
					return;
				}

				// Create a new proxy object associated with this invoke
				inst = NewProxyType(sess);

				// Create the event socket
				if ((PROXY_EVENTFD(inst) = socket(host->ai_family, host->ai_socktype, host->ai_protocol)) < 0)
				{
					perror("socket");
					return;
				}
				
				// Set the event socket to TCP_NODELAY
				sopt = 1;
				if (setsockopt(PROXY_EVENTFD(inst), IPPROTO_TCP, TCP_NODELAY, &sopt, sizeof(sopt)) < 0) 
				{
					// Non-fatal error
					perror("setsockopt(TCP_NODELAY)");
				}
	
				// Create the output socket
				if ((PROXY_OUTPUTFD(inst) = socket(host->ai_family, host->ai_socktype, host->ai_protocol)) < 0)
				{
					perror("socket");
					return;
				}
	
				// Set the output socket to TCP_NODELAY since this will output fstream data
				sopt = 1;
				if (setsockopt(PROXY_OUTPUTFD(inst), IPPROTO_TCP, TCP_NODELAY, &sopt, sizeof(sopt)) < 0) 
				{
					// Non-fatal error
					perror("setsockopt(TCP_NODELAY)");
				}
	
				/* Connect the sockets */
	
				// Connect the event socket to host
				while (connect(PROXY_EVENTFD(inst), host->ai_addr, host->ai_addrlen)) 
				{
					perror("connect");
					sleep(1);
					if (tries++ == RECONNECT_TRIES)
					{
						return;
					}
				}
				
				tries = 0;
	
				// Connect the output socket to host
				while (connect(PROXY_OUTPUTFD(inst), host->ai_addr, host->ai_addrlen)) 
				{
					perror("connect");
					sleep(1);
					if (tries++ == RECONNECT_TRIES)
					{
						return;
					}
				}
	
				// Create a file stream for the output socket.
				PROXY_OUTPUTSTREAM(inst) = fdopen(PROXY_OUTPUTFD(inst), "w");
	
				// Set the stream to be line buffered
				setlinebuf(PROXY_OUTPUTSTREAM(inst));
	
				// Set the protocol based on the proto arg
				// TODO: Allow this to select any protocol, other than the hardcoded
				//       mrnet we see now.
				if ( (ptab = FindClntProto("mrnet")) == (dbgclnt_proto *)NULL )
				{
					// Handle error
					return;
				}
	
				// Call the protocol specific invoke function
				// TODO: This could be modified to allow for multiple proxy invocations,
				//       to support multiple hops, but that is not required right now...
				di = ptab->sw_invoke(	ptab, NULL, inv_args->prog, inv_args->argv, inv_args->nproc, inv_args->arch, 
										inv_args->arch_args, inv_args->arch_input, inv_args->backend, inv_args->env, 
										inv_args->cwd, inv_args->gpu_flag, inv_args->debug_flag
									);
									
				if (di == NULL)
				{
					// Handle error
					return;
				}
				
				// set the proto id in the di
				di->d_proto_id = PROXY_ID(inst);
				
				// Create a proxy_instance object to associate the client id with our id
				NewProxyInstance(msg_head.msg_id, inst);
				
				// cleanup invoke args
				FreeInvokeArgs(inv_args);
				
				// Send the DBG_OK event for the entire set
				set = bitset_new(di->d_nproc);
				bitset_invert(set);
				pmsg_set = NewProtoMsgSet();
				AddProtoMsgSet(pmsg_set, NewProtoMsg(set, EventToSocProto(NewEvent(DBG_OK))));
				DbgDispatchProtoEvents(di, pmsg_set);
				
				break;
			
			case MSG_EXIT:
				// convert msg to exit args
				detach = SocProtoToExitArgs(msg);
				
				// cleanup
				Free(msg);
				
				// find the dbginvoke_t associated with this id
				inst = GetProxyTypeFromClntId(msg_head.msg_id);
				di = NULL;
				if (!EmptyList(DbgInvokeList))
				{
					// Reset the DbgInvokeList
					SetList(DbgInvokeList);

					// Loop through each dbginvoke entry and check for events.
					while ( (di = (dbginvoke_t *)GetListElement(DbgInvokeList)) != (dbginvoke_t *)NULL )
					{
						if (di->d_proto_id == PROXY_ID(inst))
							break;
					}
				}
				
				// TODO: Handle error
				
				// ensure the exit function is implemented
				if ( di != NULL && DBG_PROTO_SW(di)->sw_exit != NULL )
				{
					// call the exit function
					if (DBG_PROTO_SW(di)->sw_exit(di, detach))
					{
						PrintOutError("Exit failed.\n");
						msg_head.msg_type = MSG_ERROR;
					} else
					{
						msg_head.msg_type = MSG_OK;
					}
				} else
				{
					if (di == NULL)
					{
						msg_head.msg_type = MSG_OK;
					} else
					{
						PrintOutError("Exit not implemented.\n");
						msg_head.msg_type = MSG_ERROR;
					}
				}
				
				// Send just the header back in response
				msg_head.msg_len = 0;
				
				// Send the header
				if (send(PROXY_SESSION_CMDFD(sess), &msg_head, sizeof(proxy_msg_header), 0) != sizeof(proxy_msg_header))
				{
					PrintOutError("Could not send header on cmd stream.\n");
					return;
				}
				
				// Cleanup the proxy instance, we are done with it.
				if (di != NULL)
					FreeDbgInvoke(di);
				FreeProxyInstance(inst);
				
				break;
				
			case MSG_OK:
			case MSG_ERROR:
				// These should not be called!
				break;
		}
	}
}

int
DbgDispatchProtoEvents(dbginvoke_t *di, proto_msg_set *set)
{
	proto_message *		pmsg;
	char *				msg;
	proxy_msg_header	msg_head;
	proxy_type *		inst;
	
	if (set == NULL)
		return 0;
		
	// get the proxy_type from the instance id
	inst = GetProxyTypeFromInstId(di->d_proto_id);
	
	ResetProtoMsgSet(set);
	while((pmsg = NextProtoMsgInSet(set)) != NULL)
	{
		// Convert this to a string
		msg = ProtoMsgToSocProto(pmsg);
	
		// Send the header
		msg_head.msg_len = (int)strlen(msg) * sizeof(char);
		msg_head.msg_type = MSG_OK;
		if (send(PROXY_EVENTFD(inst), &msg_head, sizeof(proxy_msg_header), 0) != sizeof(proxy_msg_header))
		{
			PrintOutError("Could not send header on event stream.\n");
			return 0;
		}
		
		// Send the msg
		if (send(PROXY_EVENTFD(inst), msg, msg_head.msg_len, 0) != msg_head.msg_len)
		{
			PrintOutError("Could not send msg on event stream.\n");
			return 0;
		}
		
		// Done with msg
		Free(msg);
	}
	
	return 1;
}

void		
DbgDispatchProtoOutput(dbginvoke_t *di, FILE *p_o_stream)
{
	proxy_type *	inst;
	char *			line;
	
	if (di == NULL || p_o_stream == NULL)
		return;
	
	// get the proxy_type from the instance id
	inst = GetProxyTypeFromInstId(di->d_proto_id);
	
	// Output a program line recieved on the program output pipe
	line = (char *)Alloc(BUFSIZ);
	
	while (fgets(line, BUFSIZ, p_o_stream) > 0)
	{
		fprintf(PROXY_OUTPUTSTREAM(inst), "%s", line);
	}
				
	Free(line);
}

/*
** fprintf a line to stderr.
*/
void
PrintOutError(char *format, ...)
{
	va_list arg_list;
	
	va_start(arg_list, format);
	
	// vfprintf the line we recieved
	vfprintf(PROXY_SESSION_ERRORSTREAM(sess), format, arg_list);
	
	// end the va_list
	va_end(arg_list);
}

/*
** fprintf a line to stdout.
*/
void
PrintOutMsg(char *format, ...)
{
	va_list arg_list;
	
	va_start(arg_list, format);
	
	// vfprintf the line we recieved
	vfprintf(PROXY_SESSION_MSGSTREAM(sess), format, arg_list);
	
	// end the va_list
	va_end(arg_list);
}

void
ForceQuit(void)
{
	PrintOutError("Force quit.\n");
	_exit(1);
}

