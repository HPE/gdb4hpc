/*
** Debugger interface routines
**
** Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
**
*/

#ifdef HAVE_CONFIG_H
#include	<config.h>
#endif /* HAVE_CONFIG_H */

#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/wait.h>

#include <arpa/inet.h>

#include "dbg_client.h"
#include "useful.h"

#if defined (RD)
#include "df.h"
#endif /* RD */

struct inpcb_t
{
	int	i_fd;
	void	(*i_rtn)();
};
typedef struct inpcb_t	inpcb_t;

struct outpcb_t
{
	void	(*o_rtn)(dbginvoke_t *, FILE *);
};
typedef struct outpcb_t outpcb_t;

/*
** Internal prototypes
*/
static void	DbgFreeSignal(void *);

/*
** Internal Variables
*/
static proto_session *	DbgCurrSess = NULL;
static list_t *			DbgInvokeList = (list_t *)NULL;
static list_t *			DbgSigList = (list_t *)NULL;
static list_t *			DbgMaskedSigList = (list_t *)NULL;
static fd_set			dispatch_fds;
static int				high_fd;
static inpcb_t			InputCB;
static outpcb_t			OutputCB;
static int				_mask_fd;
#if defined (RD)
static int				_df_ready = 0;
#endif /* RD */
static int				inferior_status_change = 0;
static int				inferior_exit_change = 0;

/*
** Hold any pending signals.
*/
void
HoldSignals(void)
{
	int *		sig;
	sigset_t	mask;

	// initialize values
	sigemptyset(&mask);

	if ( !EmptyList(DbgSigList) )
	{
		SetList(DbgSigList);

		while ( (sig = (int *) GetListElement(DbgSigList)) != (int *)NULL )
			sigaddset(&mask, *sig);

		sigprocmask(SIG_BLOCK, &mask, (sigset_t *)NULL);
	}
}

/*
** Release any pending signals.
*/
void
ReleaseSignals(void)
{
	int *		sig;
	int *		masked_sig;
	int			found = 0;
	sigset_t	mask;

	// initialize data
	sigemptyset(&mask);

	if ( !EmptyList(DbgSigList) )
	{
		SetList(DbgSigList);

		while ( (sig = (int *) GetListElement(DbgSigList)) != (int *)NULL )
		{
			// If the signal is currently masked by a call to DbgMaskSignal,
			// WE DON'T WANT TO UNMASK IT!!!!!
			if ( !EmptyList(DbgMaskedSigList) )
			{
				SetList(DbgMaskedSigList);
				
				while ( (masked_sig = (int *) GetListElement(DbgMaskedSigList)) != (int *)NULL )
				{
					if (*sig == *masked_sig)
						found = 1;
				}
			}
			if (!found)
				sigaddset(&mask, *sig);
		}

		sigprocmask(SIG_UNBLOCK, &mask, (sigset_t *)NULL);
	}
}

/*
** Create a dbginvoke_t structure.
*/
dbginvoke_t *
NewDbgInvoke(int nproc, dbgclnt_proto *proto)
{
	dbginvoke_t *		di;

	di = (dbginvoke_t *)Alloc(sizeof(dbginvoke_t));
	memset(di, 0, sizeof(dbginvoke_t));
	
	// Set non-zero members here
	di->d_nproc = nproc;
	di->d_proto = proto;

	if ( DbgInvokeList == (list_t *)NULL )
		DbgInvokeList = NewList();

	/*
	** Add to DbgInvokeList
	*/
	AddToList(DbgInvokeList, (void *)di);

	return di;
}

/*
** Free a dbginvoke_t structure.
*/
void
FreeDbgInvoke(dbginvoke_t *di)
{
	int *	ev_fd;

	HoldSignals();

	RemoveFromList(DbgInvokeList, (void *)di);
	
	// Call the protocol destructor if there is one
	if ( DBG_PROTO_SW(di)->sw_destroyclnt != NULL )
	{
		DBG_PROTO_SW(di)->sw_destroyclnt(di);
	}

	if( di->d_bitset_full != NULL )
		bitset_free(di->d_bitset_full);

	if( di->d_bitset_launched != NULL )
		bitset_free(di->d_bitset_launched);
		
	// remove any event fds from the proto if there are any for this dbginvoke.
	if (di->d_evfds != NULL)
	{
		SetList(di->d_evfds);
		while((ev_fd = (int *)GetListElement(di->d_evfds)) != NULL)
		{
			Free(ev_fd);
		}
	}
	DestroyList(di->d_evfds, NULL);

	Free(di);

	ReleaseSignals();
}

SOCKET
ConnectIO(char *host, int port)
{
	SOCKET			s;
	struct hostent *	hp;
	struct sockaddr_in	in;

	if 
	(
		(hp = gethostbyname(host)) == (struct hostent *)NULL
		||
		(s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET
	)
	{
		perror("ConnectIO:socket");
		return INVALID_SOCKET;
	}

	in.sin_family = AF_INET;
	memcpy((char *)&in.sin_addr, (char *)hp->h_addr, hp->h_length);
	in.sin_port = htons((short)port);

	if ( connect(s, (struct sockaddr *)&in, sizeof(in)) == SOCKET_ERROR )
	{
		perror("ConnectIO:connect");
		CLOSE_SOCKET(s);
		return INVALID_SOCKET;
	}

	return s;
}

/*
**********************************************************
**********************************************************
*************** DEBUGGER API ROUTINES ********************
**********************************************************
**********************************************************
*/

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
	int					res = 0;
	int	*				ev_fd;
	struct timeval		tv = {1, 0};

	// reset the fd_set
	FD_ZERO(&dispatch_fds);
	high_fd = -1;

	/*
	** Check if any events are ready.
	*/
	
#if defined (RD)
	/*
	** Begin by checking if there is an active graph that needs dispatching.
	*/
	if (ActiveDF())
	{
		++_df_ready;
		res = 1;
	}
#endif /* RD */

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
	** Add the input fd to the fd_set to select on. This enables input events.
	*/
	if ( InputCB.i_fd >= 0 )
	{
		if (high_fd < InputCB.i_fd)
		{
			high_fd = InputCB.i_fd;
		}
		//DPRINT("DbgCheckEvents: InputCB fd %d added.\n", InputCB.i_fd);
		FD_SET(InputCB.i_fd, &dispatch_fds);
	}

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
						if (res)
						{
							return res;
						}
						continue;
					
					// We expect EINVAL if there are no fd's in the fd_set. The high_fd should be negative in this case
					// and select will return with -1 and errno set to EINVAL.
					case EINVAL:
						DPRINT("DbgCheckEvents: EINVAL\n");
						return res;
						
					default:
						PrintOutError("select: %s\n", strerror(errno));
						return -1;
				}
				break;
				
			case 0:
				//DPRINT("DbgCheckEvents: select returned 0, res is %d\n", res);
				return res;
				
			default:
				// If the InputCB was set, then we don't want to dispatch the graph.
				// Commands can take awhile to return and if the user was typing something
				// we don't want to issue a new command. The user has a second to
				// type before being locked out.
				if ((InputCB.i_fd >= 0) && FD_ISSET(InputCB.i_fd, &dispatch_fds))
				{
					_df_ready = 0;
				}
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
	
	//DPRINT("*** In DbgDispatchEvents\n");
	
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
	** Process any input.
	*/
	if ( (InputCB.i_fd >= 0) && FD_ISSET(InputCB.i_fd, &dispatch_fds) && (InputCB.i_rtn != NULL) )
	{
		//DPRINT("DbgDispatchEvents: InputCB.i_fd is set\n");
		InputCB.i_rtn();
	}
	
	// We want to process the df graph last since the above can generate tokens
#if defined (RD)
	/*
	** If the DbgCheckEvents fuction flagged _df_ready, then we need to 
	** call the dataflow dispatch routine.
	*/
	if (_df_ready)
	{
		_df_ready = 0;
		if (!DispatchDF())
			EndActiveDF(); // Finish the current graph execution
	}
#endif /* RD */
}

int
DbgDispatchProtoEvents(dbginvoke_t *di, proto_msg_set *set)
{
	dbgev			last_event = DBGEV_NONE;
	proto_message *	msg;
	event_set_t *	eventset = NULL;
	dbgevent_t *	event;

	if(set == NULL)
		return 0;

	ResetProtoMsgSet(set);
	while((msg = NextProtoMsgInSet(set)) != NULL)
	{
		// convert this protocol msg string to an event
		event = SocProtoToEvent(msg->message);
		
		// switch on the event to check if the event changed the status
		switch (EVENT_EV(event))
		{
			// These events coorespond to possible status changes
			case DBGEV_BPHIT:
			case DBGEV_STEP:
			case DBGEV_SIGNAL:
			case DBGEV_EXIT:
			case DBGEV_ERROR:
				// Check the status
				inferior_status_change = 1;
				break;
				
			default:
				// do nothing
				break;
		}
		
		eventset = AddEvent(di->d_proto_net, event, msg->group);
		
		// This is the callback that was set by calling DbgHandleEvent
		(*(di->d_clnt_ev_callback))(eventset->e_ev, eventset->e_group, msg->group, di->d_proto_net);

		/* If all events have been received, remove the eventset*/
		// I looked into this and it appears that right before calling step, 
		// continue, and finish a call to RemoveEventSet is made since those
		// commands won't allow proceeding while the debugger is in a run status
		// state. I think there still might be a memory leak on error though.
		// Right now this will clear events where only some of the ranks have
		// reported in. Sometimes this can happen if only some of the ranks are
		// expected to hit a breakpoint.
		// FIXME: We should be smart about this. If a halt command is sent,
		//        then any pending events should be removed since its impossible
		//        for them to be hit. A change in status should also remove
		//        some types of events. 
		// Right now this will only check for a full event set if the last
		// event recieved doesn't match the one we just recieved. This avoids
		// having to do this for each and every event we recieve if the aggregation
		// in the tree is particularly bad. If it is bad, we expect to receive
		// multiple events of the same type in a row.
		if (EVENT_EV(eventset->e_ev) != last_event)
		{
			// Only check if last_event is not DBGEV_NONE since this is the default
			// noneness case
			if (last_event != DBGEV_NONE)
			{
				DPRINT("Checking for full event set for %s\n", DbgevToStr(last_event));
				// This will remove the eventset if we have recieved an event
				// of this type for every rank.
				CheckForFullEventSet(di->d_proto_net, di->d_bitset_full, last_event);
			}
			// set the last event to this event type
			last_event = EVENT_EV(eventset->e_ev);
		}
	}
	
	// Check the last_event if its not noneness
	if (last_event != DBGEV_NONE)
	{
		DPRINT("Checking for full event set for %s\n", DbgevToStr(last_event));
		// This will remove the eventset if we have recieved an event
		// of this type for every rank.
		CheckForFullEventSet(di->d_proto_net, di->d_bitset_full, last_event);
	}
	
#ifdef DEBUG
	// Make sure we reset the stream since the async handler might have 
	// modified it. This is so that we don't become confused when looking at the
	// debug output and wait for a prompt.
	extern void	cli_reset_stream(void);
	
	cli_reset_stream();
#endif
	
	return 1;
}

void		
DbgDispatchProtoOutput(dbginvoke_t *di, FILE *p_o_stream)
{
	if (di == NULL || p_o_stream == NULL)
		return;

	// Call the output callback
	OutputCB.o_rtn(di, p_o_stream);
}

/*
** Set client event handler
*/
void
DbgHandleEvent(dbginvoke_t *di, clnt_ev_callback_func ev_callback)
{
	if (di == NULL)
		return;

	// set the new callback
	di->d_clnt_ev_callback = ev_callback;
}

/*
** Set local output stream handler.
*/
void
DbgHandleOutput(void (*outp)(dbginvoke_t *, FILE *))
{
	OutputCB.o_rtn = outp;			// callback handler
}

/*
** Set local input stream handler.
*/
void
DbgHandleInput(int fd, void (*inp)(void))
{
	InputCB.i_fd = fd;
	InputCB.i_rtn = inp;
}

/*
** Mask input stream events temporarily
*/
void
DbgMaskInput(void)
{
	_mask_fd = InputCB.i_fd;
	InputCB.i_fd = -1;
}

/*
** Remove temporary input stream mask.
*/
void
DbgUnmaskInput(void)
{
	InputCB.i_fd = _mask_fd;
}

void
DbgMaskSignal(int sig)
{
	int *		l_sig;
	sigset_t	mask;

	sigemptyset(&mask);
	sigaddset(&mask, sig);
	sigprocmask(SIG_BLOCK, &mask, (sigset_t *)NULL);
	
	// We want to block the signal before adding it to the masked list to avoid
	// a race condition.
	l_sig = Alloc(sizeof(int));
	memcpy(l_sig, &sig, sizeof(int));
	
	AddToList(DbgMaskedSigList, (void *)l_sig);
}

void
DbgUnmaskSignal(int sig)
{
	int *		l_sig;
	sigset_t	mask;
	
	SetList(DbgMaskedSigList);
	
	// We want to remove the signal from the masked list before unblocking it to
	// avoid a race condition.
	while ( (l_sig = (int *) GetListElement(DbgMaskedSigList)) != (int *)NULL )
	{
		if (*l_sig == sig)
		{
			RemoveFromList(DbgMaskedSigList, (void *)l_sig);
			Free(l_sig);
		}
	}
	
	sigemptyset(&mask);
	sigaddset(&mask, sig);
	sigprocmask(SIG_UNBLOCK, &mask, (sigset_t *)NULL);
}

/*
** Set signal handler.
*/
void
DbgHandleSignal(int sig, void (*hand)(int))
{
	int *				l_sig;
	struct sigaction	handler;

	l_sig = Alloc(sizeof(int));
	memcpy(l_sig, &sig, sizeof(int));
	
	if ( hand == SIG_DFL || hand == SIG_IGN )
		RemoveFromList(DbgSigList, (void *) l_sig);
	else if ( !InList(DbgSigList, (void *) l_sig) )
		AddToList(DbgSigList, (void *) l_sig);
		
	// make sure to initialize the handler
	memset(&handler, 0, sizeof(handler));
	sigemptyset(&handler.sa_mask);
	
	// set the handler function
	handler.sa_handler = hand;
	
	sigaction(sig, &handler, NULL);
}

/*
** Free signal handler.
*/
static void
DbgFreeSignal(void *l_sig)
{
	if ( l_sig != (void *)NULL )
		Free(l_sig);
}

/*
** Used to check if a command was called that changed the status
*/
int
DbgCheckForStatusChange(void)
{
	return inferior_status_change;
}

/*
** Used to reset the inferior_status_change value to 0
*/
void
DbgResetStatusChange(void)
{
	inferior_status_change = 0;
}

/*
** Used to check if a command was called that caused the inferior to exit
*/
int
DbgCheckForExit(void)
{
	return inferior_exit_change;
}

/*
** Used to reset the inferior_exit_change value to 0
*/
void
DbgResetExitValue(void)
{
	inferior_exit_change = 0;
}

/************************************
** dbgclnt_proto command functions **
************************************/

/*
** Initialise server
*/
int
DbgInit(void)
{
	dbgclnt_proto *	sw;

	SetClntProto();

	while ( (sw = GetClntProto()) != (dbgclnt_proto *)NULL )
		if ( sw->sw_init != (int (*)(void))NULL && !sw->sw_init() )
			return -1;

	if ( DbgSigList == (list_t *)NULL )
		DbgSigList = NewList();
		
	if ( DbgMaskedSigList == (list_t *)NULL )
		DbgMaskedSigList = NewList();

	InputCB.i_fd = -1;

	return 0;
}

void
DbgFinish(void)
{
	dbgclnt_proto *	sw;
	dbginvoke_t *	di;

	SetClntProto();
	
	// Cleanup the current session if there is one
	if (DbgCurrSess != NULL)
	{
		sw = DbgCurrSess->sess_proto;
		if (sw->sw_cleanup_session == NULL)
		{
			PrintOutError("Protocol function sw_cleanup_session not implemented.\n");
		} else
		{
			sw->sw_cleanup_session(DbgCurrSess);
		}
		DbgCurrSess = NULL;
	}

	while ( (sw = GetClntProto()) != (dbgclnt_proto *)NULL )
	{
		if ( sw->sw_finish != NULL )
		{
			sw->sw_finish();
		}
	}

	DestroyList(DbgSigList, DbgFreeSignal);
	DestroyList(DbgMaskedSigList, DbgFreeSignal);
	// Manually free the elements in the dbginvoke list
	while ((di=RemoveFirst(DbgInvokeList)) != NULL)
	{
		FreeDbgInvoke(di);
	}
	DestroyList(DbgInvokeList, NULL);

	DbgSigList = (list_t *)NULL;
	DbgMaskedSigList = (list_t *)NULL;
	DbgInvokeList = (list_t *)NULL;
}

/*
** Create a new session
*/
proto_session *
DbgSessionCreate(	char *proto, 
					WLM_type manager, 
					char *batch_template, 
					char *hostname, 
					char *username
)
{
	dbgclnt_proto * ptab;
	
	if ((ptab = FindClntProto(proto)) == NULL)
	{
		PrintOutError("No such protocol \"%s\"\n", proto);
		return NULL;
	}
	
	if (ptab->sw_create_session == NULL)
	{
		PrintOutError("Protocol function sw_create_session not implemented.\n");
		return NULL;
	}
	
	return ptab->sw_create_session(ptab, manager, batch_template, hostname, username);
}

/*
** Set the session to associate all subsequent launches/attaches with.
** If a session is currently set as being open, clean it up if possible.
** It's up to the protocol implementation to handle the actual cleanup.
*/
void
DbgSetSession(proto_session *sess)
{
	dbgclnt_proto *	ptab;
	
	// Cleanup the current session if there is one
	if (DbgCurrSess != NULL)
	{
		ptab = DbgCurrSess->sess_proto;
		if (ptab->sw_cleanup_session == NULL)
		{
			PrintOutError("Protocol function sw_cleanup_session not implemented.\n");
		} else
		{
			ptab->sw_cleanup_session(DbgCurrSess);
		}
	}
	
	// Ask the protocol layer to keep the new session around if its not null
	if (sess != NULL)
	{
		ptab = sess->sess_proto;
		if (ptab->sw_save_session == NULL)
		{
			PrintOutError("Protocol function sw_save_session not implemented.\n");
		} else
		{
			ptab->sw_save_session(sess);
		}
	}
	
	DbgCurrSess = sess;
}

/*
** Get the current session (if there is one)
*/
proto_session *
DbgGetSession(void)
{
	return DbgCurrSess;
}

/*
** Attach to running program.
*/
dbginvoke_t *
DbgAttach(	char *proto,
			proto_session *sess,
			int pid,
			char *arch,
			char *backend,
			int gpu_flag,
			int debug_flag
)
{
	dbginvoke_t *	di;
	dbgclnt_proto *	ptab;

	if ( (ptab = FindClntProto(proto)) == (dbgclnt_proto *)NULL )
	{
		PrintOutError("No such protocol \"%s\"\n", proto);
		return NULL;
	}
	
	if (ptab->sw_attach == NULL)
	{
		PrintOutError("Protocol function sw_attach not implemented.\n");
		return NULL;
	}

	if ( (di = ptab->sw_attach(ptab, sess, pid, arch, backend, gpu_flag, debug_flag)) == (dbginvoke_t *)NULL )
	{
		return NULL;
	}
	
	// The frontend debug presence can fail silently
	// This is currently disabled for the attach case since I have no idea how
	// to get the actual fullpath name of the a.out.
	//di->d_frontend = DbgInstantiateFE(backend);
	di->d_frontend = NULL;
	
	// FIXME: How to set the debug frontend a.out???

	// This command changes the status
	inferior_status_change = 1;

	return di;
}

/*
** Invoke user program.
*/
dbginvoke_t *
DbgInvoke(	char *proto,
			proto_session *sess,
			char *prog,
			char **argv,
			int nproc,
			char *arch,
			char **arch_args,
			char *arch_input,
			char *backend,
			char **env,
			char *cwd,
			int gpu_flag,
			int debug_flag
)
{
	dbginvoke_t *	di;
	dbgclnt_proto *	ptab;

	if ( (ptab = FindClntProto(proto)) == (dbgclnt_proto *)NULL )
	{
		PrintOutError("No such protocol \"%s\"\n", proto);
		return NULL;
	}
	
	if (ptab->sw_invoke == NULL)
	{
		PrintOutError("Protocol function sw_invoke not implemented.\n");
		return NULL;
	}

	if ( (di = ptab->sw_invoke(ptab, sess, prog, argv, nproc, arch, arch_args, arch_input, backend, env, cwd, gpu_flag, debug_flag)) == (dbginvoke_t *)NULL )
	{
		return NULL;
	}
	
	// The frontend debug presence can fail silently
	di->d_frontend = DbgInstantiateFE(backend);
	
	// Set the debug frontend a.out directly.
	if (di->d_frontend != NULL)
	{
		dbgevent_t * e;
		e = DbgFESetFile(di->d_frontend, prog);
		
		// handle the error case
		if (e->ev_event == DBGEV_ERROR)
		{
			// We want to tear down the frontend since it has no way of reading
			// symbols since loading the binary failed.
			FreeEvent(e);
			e = DbgDestroyFE(di->d_frontend);
			di->d_frontend = NULL;
		}
		
		FreeEvent(e);
	}
	
	// This command changes the status
	inferior_status_change = 1;

	return di;
}

static bitset *
DbgSendCmd(rec_set_t *set, char *request)
{
	dbginvoke_t * 	di;
	
	// Set the invoke handler
	di = (dbginvoke_t*)(set->s_dbginvoke_handler);
	
	// ensure the send_cmd function is implemented
	if ( DBG_PROTO_SW(di)->sw_send_cmd == NULL )
	{
		PrintOutError("Protocol function sw_send_cmd not implemented.\n");
		return NULL;
	}
	
	return DBG_PROTO_SW(di)->sw_send_cmd(set->s_proto_handler, set->s_receiver_bs, request);
}

static rep_set_t *
DbgRecvReply(rec_set_t* set, bitset* received, dbgev req_type)
{
	dbginvoke_t * 		di;
	proto_msg_set *		proto_res;
	proto_message *		msg;
	rep_set_t *			reply;
	group_reply_t *		rep;
	dbgevent_t *		e;
	dbgev				ev;
	
	// Set the invoke handler
	di = (dbginvoke_t*)(set->s_dbginvoke_handler);
	
	// ensure the recv_reply function is implemented
	if ( DBG_PROTO_SW(di)->sw_recv_reply == NULL )
	{
		PrintOutError("Protocol function sw_recv_cmd not implemented.\n");
		return (rep_set_t *)NULL;
	}
	
	proto_res = DBG_PROTO_SW(di)->sw_recv_reply(set->s_proto_handler, set->s_receiver_bs, received);
	
	reply = NewRepSet(di->d_proto_net);

	if (proto_res == NULL)
	{
		// Create a new error event based on this reply
		EVENT_ERROR(e, DBGERR_INTERNAL, "Protocol returned null proto_msg_set.\n");
		rep = NewGroupReply(NULL, e);
		// Add the error to the reply set
		AddErrToSet(reply, rep);
		
		return reply;
	}
	
	// process the proto_res
	while ((msg = NextProtoMsgInSet(proto_res)) != NULL)
	{
		// convert this protocol msg string to an event
		e = SocProtoToEvent(msg->message);
		ev = EVENT_EV(e);
		
		switch (ev)
		{
			// Errors get added to the special error list
			case DBGEV_ERROR:
				rep = NewGroupReply(bitset_dup(msg->group), e);
				// Add this error to the reply set
				AddErrToSet(reply, rep);
				break;
			
			// Any event without data should be handled here
			case DBGEV_OK:
				if (ev != req_type)
				{
					// Create a new error event based on this reply
					FreeEvent(e);
					EVENT_ERROR(e, DBGERR_INTERNAL, "Received event of type %s, expected type %s.\n", 
													DbgevToStr(ev), DbgevToStr(req_type));
					rep = NewGroupReply(bitset_dup(msg->group), e);
					// Add the error to the reply set
					AddErrToSet(reply, rep);
					break;
				}
				// Do nothing.
				break;
			
			default:
				if (ev != req_type)
				{
					// Create a new error event based on this reply
					FreeEvent(e);
					EVENT_ERROR(e, DBGERR_INTERNAL, "Received event of type %s, expected type %s.\n", 
													DbgevToStr(ev), DbgevToStr(req_type));
					rep = NewGroupReply(bitset_dup(msg->group), e);
					// Add the error to the reply set
					AddErrToSet(reply, rep);
					break;
				}
				// Create a reply out of this event
				rep = NewGroupReply(bitset_dup(msg->group), e);
				// Add this reply to the reply set
				AddReplyToSet(reply, rep);
				break;
		}
	}
	
	// Free the proto set, we are done with it.
	FreeProtoMsgSet(proto_res);
	
	// Check to see if there are no replies or errors in the reply set, if so we
	// want to return NULL. This is because some of the event types have no data.
	// These functions will want to receive null, otherwise they assume that there
	// was an error
	if (!SizeOfList(reply->err_list) && !SizeOfList(reply->rep_list))
	{
		FreeRepSet(reply);
		return NULL;
	}
	
	return reply;
}

static rep_set_t *
DbgSendCmdAndRecvReply(rec_set_t* set, char* request, dbgev req_type)
{
	bitset *		received;
	rep_set_t *		reply_set;
	
	HoldSignals();
	
	// send the command
	received = DbgSendCmd(set, request);
	
	if (received == NULL)
	{
		return NULL;
	}
	
	// recv the reply
	reply_set = DbgRecvReply(set, received, req_type);
	
	// free the send_cmd bitset
	bitset_free(received);
	
	ReleaseSignals();
	
	return reply_set;
}

/**********************************************************
** Start of non-proto specific commands                  **
** These functions may or may not require proto specific **
** components.                                           **
**********************************************************/

/*
** DBG_STARTUP: Startup the program (continue to Original Entry Point) and show
**				the frame.
*/
rep_set_t *
DbgStartup(rec_set_t *set)
{
	char *	request;
	
	request = CommandToSocProto(DBGCMD_STARTUP);

	rep_set_t* result = DbgSendCmdAndRecvReply(set, request, DBGEV_FRAME);
	
	Free(request);
	
	// This command changes the status
	inferior_status_change = 1;
	
	ResetSet(result);
	return result;
}

/*
** DBG_SENDEXPR: Send expression to BE.
*/
bitset *
DbgSendExpr(rec_set_t *set, char *exp, int assid)
{
	char *	request;
	
	request = CommandToSocProto(DBGCMD_EVALEXPR, exp);

	bitset* result = DbgSendCmd(set, request);
	
	Free(request);
	
	return result;
}

/*
** DBG_GETEXPR: Get results of evaluate expression.
*/
rep_set_t *
DbgGetExpr(rec_set_t *set, bitset *received, int assid)
{
	rep_set_t* result = DbgRecvReply(set, received, DBGEV_DATA);
	
	ResetSet(result);
	return result;
}

/*
** DBGOPENSOCKET: Open socket for each debug server - for P2P 
*/
rep_set_t *
DbgOpenSocket(rec_set_t *set, char *opset_name, int opset_size)
{
	char *	request;

	request = CommandToSocProto(DBGCMD_OPENSOCKET, opset_name, opset_size);

	rep_set_t* result = DbgSendCmdAndRecvReply(set, request, DBGEV_BPSET);
	
	Free(request);
	
	ResetSet(result);
	return result;
}

/*
** DBGCLOSESOCKET: Close socket for each debug server - for P2P 
*/
rep_set_t *
DbgCloseSocket(rec_set_t *set)
{
	char *	request;
	
	request = CommandToSocProto(DBGCMD_CLOSESOCKET);

	rep_set_t* result = DbgSendCmdAndRecvReply(set, request, DBGEV_BPSET);
	
	Free(request);
	
	ResetSet(result);
	return result;
}

/*
** DBG_EVALEXPR: Evaluate expression.
*/
rep_set_t *
DbgEvalExpr(rec_set_t *set, char *exp, int assid)
{
	char *	request;
	
	request = CommandToSocProto(DBGCMD_EVALEXPR, exp);

	rep_set_t* result = DbgSendCmdAndRecvReply(set, request, DBGEV_DATA);
	
	Free(request);
	
	ResetSet(result);
	return result;
}

/*
** DBG_SETDEBUGGERVAR: Set variable in debugger to new value.
*/
rep_set_t *
DbgSetDebuggerVar(rec_set_t *set, char *var, AIF *val)
{
	char *	request;
	
	request = CommandToSocProto(DBGCMD_SETDEBUGGERVAR, var, val);

	rep_set_t *result = DbgSendCmdAndRecvReply(set, request, DBGEV_OK);
	
	Free(request);
	
	ResetSet(result);
	return result;
}

/*
** DBG_GETDEBUGGERVAR: Get variable in debugger.
*/
rep_set_t *
DbgGetDebuggerVar(rec_set_t *set, char *var)
{
	char *	request;

	request = CommandToSocProto(DBGCMD_GETDEBUGGERVAR, var);

	rep_set_t *result = DbgSendCmdAndRecvReply(set, request, DBGEV_DATA);
	
	Free(request);
	
	ResetSet(result);
	return result;
}

/*
** DBG_SETSOURCEPATH: Set source search path in debugger to new value.
*/
rep_set_t *
DbgSetSourcePath(rec_set_t *set, char *path)
{
	dbgfrontend_sw *	dbg_fe = ((dbginvoke_t *)(set->s_dbginvoke_handler))->d_frontend;
	char *				request = NULL;
	rep_set_t *			result;
	dbgevent_t *		e;
	group_reply_t *		group_rep;
	
	// We want to do the actual reading of the source code on the frontend if
	// possible. This means this command should be directed towards the
	// frontend if possible.
	if (dbg_fe != NULL)
	{
		result = NewRepSet(set->s_proto_handler);
		
		e = DbgFESetSourcePath(dbg_fe, path);
		
		if (e->ev_event != DBGEV_SOURCEPATH && e->ev_event != DBGEV_ERROR)
		{
			// Free the event
			FreeEvent(e);
			// Create a new error event
			EVENT_ERROR(e, DBGERR_DEBUGGER, "DbgFEList returned unknown event other than error.");
		}
		
		// Add the event to the result based on its type
		group_rep = NewGroupReply(bitset_dup(set->s_receiver_bs), e);
		
		if (e->ev_event == DBGEV_ERROR)
		{
			AddErrToSet(result, group_rep);
		} else
		{
			AddReplyToSet(result, group_rep);
		}
		
		ResetSet(result);
		return result;
	}
	
	request = CommandToSocProto(DBGCMD_SETSOURCEPATH, path);

	result = DbgSendCmdAndRecvReply(set, request, DBGEV_SOURCEPATH);
	
	Free(request);
	
	ResetSet(result);
	return result;
}

/*
** DBG_GETSOURCEPATH: Get source search path from debugger.
*/
rep_set_t *
DbgGetSourcePath(rec_set_t *set)
{
	dbgfrontend_sw *	dbg_fe = ((dbginvoke_t *)(set->s_dbginvoke_handler))->d_frontend;
	char *				request = NULL;
	rep_set_t *			result;
	dbgevent_t *		e;
	group_reply_t *		group_rep;
	
	// We want to do the actual reading of the source code on the frontend if
	// possible. This means this command should be directed towards the
	// frontend if possible.
	if (dbg_fe != NULL)
	{
		result = NewRepSet(set->s_proto_handler);
		
		e = DbgFEGetSourcePath(dbg_fe);
		
		if (e->ev_event != DBGEV_SOURCEPATH && e->ev_event != DBGEV_ERROR)
		{
			// Free the event
			FreeEvent(e);
			// Create a new error event
			EVENT_ERROR(e, DBGERR_DEBUGGER, "DbgFEList returned unknown event other than error.");
		}
		
		// Add the event to the result based on its type
		group_rep = NewGroupReply(bitset_dup(set->s_receiver_bs), e);
		
		if (e->ev_event == DBGEV_ERROR)
		{
			AddErrToSet(result, group_rep);
		} else
		{
			AddReplyToSet(result, group_rep);
		}
		
		ResetSet(result);
		return result;
	}
	
	request = CommandToSocProto(DBGCMD_GETSOURCEPATH);

	result = DbgSendCmdAndRecvReply(set, request, DBGEV_SOURCEPATH);
	
	Free(request);
	
	ResetSet(result);
	return result;
}

/*
** DBG_SETVAR: Set variable in program to new value.
*/
rep_set_t *
DbgSetVar(rec_set_t *set, char *var, AIF *val)
{
	char *	request;
	
	request = CommandToSocProto(DBGCMD_SETVAR, var, val);

	rep_set_t* result = DbgSendCmdAndRecvReply(set, request, DBGEV_OK);
	
	Free(request);
	
	ResetSet(result);
	return result;
}

/*
** DBG_GETTYPE: Get type of variable.
** If isType is true, then get type of type.
*/
rep_set_t *
DbgGetType(rec_set_t *set, char *var, int isType)
{
	char *	request;

	request = CommandToSocProto(DBGCMD_GETTYPE, var, isType);

	rep_set_t* result = DbgSendCmdAndRecvReply(set, request, DBGEV_TYPE);
	
	Free(request);
	
	ResetSet(result);
	return result;
}

/*
** DBG_GO: Continue/start execution.
*/
rep_set_t *
DbgGo(rec_set_t *set)
{
	char *	request;
	
	request = CommandToSocProto(DBGCMD_GO);

	rep_set_t* result = DbgSendCmdAndRecvReply(set, request, DBGEV_OK);
	
	Free(request);

	// Clear the list location state since this modified the frame only if there
	// was no error
	if(!SetHasError(result))
	{
		dbgfrontend_sw *dbg_fe = ((dbginvoke_t *)(set->s_dbginvoke_handler))->d_frontend;
		if (dbg_fe != NULL && dbg_fe->dbg_list_isPending)
		{
			ClearListState(dbg_fe);
		}
	}
	
	// This command changes the status
	inferior_status_change = 1;
	
	ResetSet(result);
	return result;
}

/*
** DBG_FINISHFUNC: Continue execution until the end of the current function frame.
*/
rep_set_t *
DbgFinishFunc(rec_set_t *set)
{
	char *	request;
	
	request = CommandToSocProto(DBGCMD_FINISH);

	rep_set_t* result = DbgSendCmdAndRecvReply(set, request, DBGEV_OK);
	
	Free(request);

	// Clear the list location state since this modified the frame only if there
	// was no error
	if (!SetHasError(result))
	{
		dbgfrontend_sw *dbg_fe = ((dbginvoke_t *)(set->s_dbginvoke_handler))->d_frontend;
		if (dbg_fe != NULL && dbg_fe->dbg_list_isPending)
		{
			ClearListState(dbg_fe);
		}
	}
	
	// This command changes the status
	inferior_status_change = 1;

	ResetSet(result);
	return result; 
}

/*
** DBG_SETLINEBREAK: Set a breakpoint in program at 
**			specified line.
*/
rep_set_t *
DbgSetLineBreak(rec_set_t *set, char *file, int line)
{
	char *	request;

	request = CommandToSocProto(DBGCMD_SETLINEBREAK, file, line);

	rep_set_t* result = DbgSendCmdAndRecvReply(set, request, DBGEV_BPSET);
	
	Free(request);
	
	ResetSet(result);
	return result;
}

/*
** DBG_SETFUNCBREAK: Set a breakpoint in program at 
**			specified function.
*/
rep_set_t *
DbgSetFuncBreak(rec_set_t *set, char *file, char *func)
{
	char *	request;
	
	request = CommandToSocProto(DBGCMD_SETFUNCBREAK, file, func);

	rep_set_t* result = DbgSendCmdAndRecvReply(set, request, DBGEV_BPSET);
	
	Free(request);
	
	ResetSet(result);
	return result;
}

/*
** DBG_SETWATCH: Set a watchpoint in program for
**			specified expression.
*/
rep_set_t *
DbgSetWatch(rec_set_t *set, int type, char *expr)
{
	char *	request;
	
	request = CommandToSocProto(DBGCMD_SETWATCH, type, expr);

	rep_set_t* result = DbgSendCmdAndRecvReply(set, request, DBGEV_BPSET);
	
	Free(request);
	
	ResetSet(result);
	return result;
}

/*
** DBG_KILL: Terminate execution of program.
*/
rep_set_t *
DbgKill(rec_set_t *set)
{
	char *			request;
	rep_set_t *		result; 
	dbginvoke_t *	di = (dbginvoke_t*)(set->s_dbginvoke_handler);
	int				res;
	dbgevent_t *	e;
	group_reply_t *	err_rep;
	
	request = CommandToSocProto(DBGCMD_KILL);

	result = DbgSendCmdAndRecvReply(set, request, DBGEV_OK);
	
	Free(request);

	// Call the protocol specific hook
	if ( DBG_PROTO_SW(di)->sw_exit == NULL )
	{
		if (result == NULL)
		{
			result = NewRepSet(di->d_proto_net);
		}
		
		// Create a new error event
		EVENT_ERROR(e, DBGERR_NOTIMP, NULL);
		err_rep = NewGroupReply(bitset_dup(set->s_receiver_bs), e);
		// Add the error to the reply set
		AddErrToSet(result, err_rep);
	} else
	{
		// We always want to kill the application, so hardcode detatch to 0
		res = DBG_PROTO_SW(di)->sw_exit(di, 0);
	}
	
	// Quit the debug frontend if there is one
	if (di->d_frontend != NULL)
	{
		e = DbgDestroyFE(di->d_frontend);
		// FIXME: How to error check here?
		FreeEvent(e);
		di->d_frontend = NULL;
	}
	
	FreeDbgInvoke(di);
	
	// This command causes the inferior to exit
	inferior_exit_change = 1;
	
	ResetSet(result);
	return result;
}

/*
** DBG_DELETEBREAK: Delete a breakpoint.
*/
rep_set_t *
DbgDeleteBreak(rec_set_t *set, int bp)
{
	char *	request;
	
	request = CommandToSocProto(DBGCMD_DELETEBREAK, bp);

	rep_set_t* result = DbgSendCmdAndRecvReply(set, request, DBGEV_OK);
	
	Free(request);
	
	ResetSet(result);
	return result;
}

/*
** DBG_DISABLEBREAK: Disable a breakpoint.
*/
rep_set_t *
DbgDisableBreak(rec_set_t *set, int bp)
{
	char *	request;

	request = CommandToSocProto(DBGCMD_DISABLEBREAK, bp);

	rep_set_t* result = DbgSendCmdAndRecvReply(set, request, DBGEV_OK);
	
	Free(request);
	
	ResetSet(result);
	return result;
}

/*
** DBG_ENABLEBREAK: Enable a breakpoint.
*/
rep_set_t *
DbgEnableBreak(rec_set_t *set, int bp, int once, int del)
{
	char *	request;

	request = CommandToSocProto(DBGCMD_ENABLEBREAK, bp, once, del);

	rep_set_t* result = DbgSendCmdAndRecvReply(set, request, DBGEV_OK);
	
	Free(request);
	
	ResetSet(result);
	return result;
}

/*
** DBG_LIST: List specified function or line.
*/
rep_set_t *
DbgList(rec_set_t *set, char *str)
{
	dbgfrontend_sw *dbg_fe = ((dbginvoke_t *)(set->s_dbginvoke_handler))->d_frontend;
	char *			request;
	rep_set_t * 	result;
	group_reply_t *	group_rep;
	dbgframe_t *	fp;
	dbgevent_t * 	e;
	char *			location = NULL;
	int				isReverse, i, cont;
	set_location *	set_loc;

	// We want to do the actual reading of the source code on the frontend if
	// possible. This is because there is a performance hit associated with
	// opening cross mounted files on the compute nodes. Its also the case that
	// sometimes we are unable to read the source files since they are located
	// in a non-cross mounted location.
	
	// Note that we can only do this if there is a debug frontend associated
	// with the dbginvoke
	if (dbg_fe != NULL)
	{
		// set the forward/reverse direction
		isReverse = (str == NULL) ? 0 : (*str == '-');
	
		// First is the logic for the case where a previous list is not pending.
		// If str is null, then we need to ask each of the ranks for their
		// current frame.
		if (((str == NULL) ? 1 : isReverse) && !(dbg_fe->dbg_list_isPending))
		{
			// create and send the command, we only want the innermost frame.
			request = CommandToSocProto(DBGCMD_SHOWFRAME, 1);

			result = DbgSendCmdAndRecvReply(set, request, DBGEV_FRAME);
			Free(request);
			
			// Make sure there isn't an error in the result, otherwise fallback.
			if (SetHasError(result))
			{
				FreeRepSet(result);
				result = NULL;
				goto dbglist_call_backend;
			}
			
			// process the frame
			SetReplyHeadInSet(result);
			while ((group_rep = GetNextReplyInSet(result)) != (group_reply_t*)NULL)
			{
				// Point at the event frame
				fp = EVENT_FRAME(group_rep->rep_event);
				
				// Now ask the frontend for the source listing around this
				// location. This is set depending on the value of isReverse
				if (isReverse)
				{
					asprintf(&location, ",%s:%d", fp->frame_loc.loc_file, fp->frame_loc.loc_line);
				} else
				{
					asprintf(&location, "%s:%d,", fp->frame_loc.loc_file, fp->frame_loc.loc_line);
				}
				
				// Save this new list location state for later calls to list
				if (AddListStateLocation(dbg_fe, group_rep->rep_group, location) < 0)
				{
					PrintOutError("Failed to save list location. Further calls to list will not save state.\n");
				}
				
				// Send the list command to the frontend
				e = DbgFEList(dbg_fe, location);
				
				// Free the location, we are done with it
				Free(location);
				location = NULL;
				
				// Make sure the event is a DBGEV_LIST, otherwise fallback.
				if (e->ev_event != DBGEV_LIST)
				{
					FreeRepSet(result);
					result = NULL;
					FreeEvent(e);
					e = NULL;
					ClearListState(dbg_fe);
					goto dbglist_call_backend;
				}
				
				// Replace the event in the group_rep with this one
				FreeEvent(group_rep->rep_event);
				group_rep->rep_event = e;
			}
			
			// return the new reply set, we are done.
			// This has converted the SHOWFRAME result into a LIST result.
			ResetSet(result);
			return result;
			
		} else if (((str == NULL) ? 1 : isReverse) && dbg_fe->dbg_list_isPending)
		{
			// We will create the result by hand based on the saved list
			// location state data
			
			// Create a new reply set
			result = NewRepSet(set->s_proto_handler);
			
			// loop over every set_location
			SetListStateLocation(dbg_fe);
			while ((set_loc = GetListStateLocation(dbg_fe)) != NULL)
			{
				// Call the list command using the saved location string to set
				// things up
				e = DbgFEList(dbg_fe, set_loc->loc_str);
				
				// handle the error case
				if (e->ev_event == DBGEV_ERROR)
				{
					group_rep = NewGroupReply(bitset_dup(set_loc->set), e);
					AddErrToSet(result, group_rep);
					continue;
				}
				
				// Free the event
				FreeEvent(e);
				
				// Now we need to call list count times with direction depending
				// on its sign.
				if (set_loc->count < 0)
				{
					cont = 0;
				
					// We need to go in the reverse direction
					for (i=set_loc->count; i < 0; ++i)
					{
						e = DbgFEList(dbg_fe, "-");
						
						// handle the error case
						if (e->ev_event == DBGEV_ERROR)
						{
							group_rep = NewGroupReply(bitset_dup(set_loc->set), e);
							AddErrToSet(result, group_rep);
							++cont;
							break;
						}
						// Free the event
						FreeEvent(e);
					}
					if (cont)
					{
						continue;
					}
				} else if (set_loc->count > 0)
				{
					cont = 0;
					
					// We need to go in the forward direction
					for (i=0; i < set_loc->count; ++i)
					{
						e = DbgFEList(dbg_fe, "");
						
						// handle the error case
						if (e->ev_event == DBGEV_ERROR)
						{
							group_rep = NewGroupReply(bitset_dup(set_loc->set), e);
							AddErrToSet(result, group_rep);
							++cont;
							break;
						}
						// Free the event
						FreeEvent(e);
					}
					if (cont)
					{
						continue;
					}
				}
				
				// Now we have everything setup for the actual list command we
				// are interested in. The command depends on the isReverse value.
				// We also increment or decrement the saved count here
				if (isReverse)
				{
					e = DbgFEList(dbg_fe, "-");
					set_loc->count--;
				} else
				{
					e = DbgFEList(dbg_fe, "");
					set_loc->count++;
				}
				
				// Make sure we got a DBGEV_LIST back, otherwise create an error
				if (e->ev_event != DBGEV_LIST && e->ev_event != DBGEV_ERROR)
				{
					// Free the event
					FreeEvent(e);
					// Create a new error event
					EVENT_ERROR(e, DBGERR_DEBUGGER, "DbgFEList returned unknown event other than error.");
				}
				
				// Add the event to the result based on its type
				group_rep = NewGroupReply(bitset_dup(set_loc->set), e);
				
				if (e->ev_event == DBGEV_ERROR)
				{
					AddErrToSet(result, group_rep);
				} else
				{
					AddReplyToSet(result, group_rep);
				}
			}
			
			// return the new reply set, we are done.
			// This created the result from the saved list location directly.
			ResetSet(result);
			return result;
			
		} else if (str != NULL && !isReverse)
		{
			// Note that we are going to create the result by hand here based
			// on the bitset in the rec_set_t. If the user calls the list cmd
			// with a string, then all ranks are going to have the same answer.
			
			// Make sure we clear the state if there was any saved
			ClearListState(dbg_fe);
		
			// Create a new reply set
			result = NewRepSet(set->s_proto_handler);
			
			// Save this new list location state for later calls to list
			if (AddListStateLocation(dbg_fe, set->s_receiver_bs, str) < 0)
			{
				PrintOutError("Failed to save list location. Further calls to list will not save state.\n");
			}
		
			// we can pass the str directly to the frontend list command since
			// its not null and was provided by the user.
			e = DbgFEList(dbg_fe, str);
			
			// Make sure the event is a DBGEV_LIST or DBGEV_ERROR, otherwise create an error
			if (e->ev_event != DBGEV_LIST && e->ev_event != DBGEV_ERROR)
			{
				// Free the event
				FreeEvent(e);
				// Create a new error event
				EVENT_ERROR(e, DBGERR_DEBUGGER, "DbgFEList returned unknown event other than error.");
			}
			
			// Add the event to the result based on its type
			group_rep = NewGroupReply(bitset_dup(set->s_receiver_bs), e);
			
			if (e->ev_event == DBGEV_ERROR)
			{
				AddErrToSet(result, group_rep);
				// Make sure to clear the saved list state since there was an
				// error with the provided string
				ClearListState(dbg_fe);
			} else
			{
				AddReplyToSet(result, group_rep);
			}
			
			// return the new reply set, we are done.
			// This created the result from the event directly.
			ResetSet(result);
			return result;
		}
	}

// This is as a last resort. If the above fails we can still try doing things
// on the backend
dbglist_call_backend:

	DPRINT("DbgList: Falling back to backend evaluation.\n");
	
	request = CommandToSocProto(DBGCMD_LIST, str);

	result = DbgSendCmdAndRecvReply(set, request, DBGEV_LIST);
	
	Free(request);

	ResetSet(result);
	return result;
}

/*
** DBG_GET_SOURCES: Ask for source files of inferior.
*/
rep_set_t *
DbgGetSources(rec_set_t *set)
{
	dbgfrontend_sw *dbg_fe = ((dbginvoke_t *)(set->s_dbginvoke_handler))->d_frontend;
	char *			request;
	dbgevent_t *	e;
	rep_set_t * 	result;
	group_reply_t *	group_rep;

	// We want to do the actual reading of the source files on the frontend if
	// possible. This is because there is a performance hit associated with
	// opening cross mounted files on the compute nodes. Its also the case that
	// sometimes we are unable to read the source files since they are located
	// in a non-cross mounted location.
	
	// Note that we can only do this if there is a debug frontend associated
	// with the dbginvoke
	if (dbg_fe != NULL)
	{
		// Create a new reply set
		result = NewRepSet(set->s_proto_handler);
		
		e = DbgFEGetSources(dbg_fe);
		
		// Make sure the event is a DBGEV_SOURCES or DBGEV_ERROR, otherwise create an error
		if (e->ev_event != DBGEV_SOURCES && e->ev_event != DBGEV_ERROR)
		{
			// Free the event
			FreeEvent(e);
			// Create a new error event
			EVENT_ERROR(e, DBGERR_DEBUGGER, "DbgFEGetSources returned unknown event other than error.");
		}
		
		// Add the event to the result based on its type
		group_rep = NewGroupReply(bitset_dup(set->s_receiver_bs), e);
		
		if (e->ev_event == DBGEV_ERROR)
		{
			AddErrToSet(result, group_rep);
		} else
		{
			AddReplyToSet(result, group_rep);
		}
		
		ResetSet(result);
		return result;
	}
	
	DPRINT("DbgGetSources: Falling back to backend evaluation.\n");
	
	request = CommandToSocProto(DBGCMD_GETSOURCES);
	
	result = DbgSendCmdAndRecvReply(set, request, DBGEV_SOURCES);
	
	Free(request);
	
	ResetSet(result);
	return result;
}

/*
** DBG_SETARGS: Set arguments to pass to program when run.
*/
rep_set_t *
DbgSetArgs(rec_set_t *set, char *args)
{
	char *	request;
	
	request = CommandToSocProto(DBGCMD_SETARGS, args);

	rep_set_t* result = DbgSendCmdAndRecvReply(set, request, DBGEV_OK);
	
	Free(request);
	
	ResetSet(result);
	return result;

}

/*
** DBG_STEP: Execute to next statement. Repeat count times.
**		If in == 0, pass over function calls (ala next).
*/
rep_set_t *
DbgStep(rec_set_t *set, int count, int in)
{
	char *	request;

	request = CommandToSocProto(DBGCMD_STEP, count, in);

	rep_set_t* result = DbgSendCmdAndRecvReply(set, request, DBGEV_OK);
	
	Free(request);
	
	// Clear the list location state since this modified the frame only if there
	// was no error
	if (!SetHasError(result))
	{
		dbgfrontend_sw *dbg_fe = ((dbginvoke_t *)(set->s_dbginvoke_handler))->d_frontend;
		if (dbg_fe != NULL && dbg_fe->dbg_list_isPending)
		{
			ClearListState(dbg_fe);
		}
	}
	
	// This command changes the status
	inferior_status_change = 1;

	ResetSet(result);
	return result;
}

/*
** DBG_MOVEFRAME: Move up or down stack frames.
*/
rep_set_t *
DbgMoveFrame(rec_set_t *set, int count, int up)
{
	char *		request;
	rep_set_t *	result;

	request = CommandToSocProto(DBGCMD_MOVEFRAME, count, up);

	result = DbgSendCmdAndRecvReply(set, request, DBGEV_FRAME);
	
	Free(request);
	
	// Clear the list location state and set it to the new frame since this 
	// modified the frame location. Only do it if it didn't have an error.
	if (!SetHasError(result))
	{
		dbgfrontend_sw *dbg_fe = ((dbginvoke_t *)(set->s_dbginvoke_handler))->d_frontend;
		if (dbg_fe != NULL)
		{
			dbgframe_t *	fp;
			group_reply_t *	group_rep;
			char *			location = NULL;
	
			// Clear any existing state
			ClearListState(dbg_fe);
		
			// process the frame
			SetReplyHeadInSet(result);
			while ((group_rep = GetNextReplyInSet(result)) != (group_reply_t*)NULL)
			{
				// Point at the frame
				fp = EVENT_FRAME(group_rep->rep_event);
				
				// Create the location string based on the new frame location
				// Note that this should one short of the new location since we didn't generate
				// this by listing source a first time. Otherwise we will start getting 10
				// lines after this location and it will be messed up.
				asprintf(&location, ",%s:%d", fp->frame_loc.loc_file, fp->frame_loc.loc_line-1);
			
				// Save this new list location state for later calls to list
				if (AddListStateLocation(dbg_fe, group_rep->rep_group, location) < 0)
				{
					PrintOutError("Failed to save list location. Further calls to list will not save state.\n");
				}
				
				// Free the location, we are done with it
				Free(location);
				location = NULL;
			}
		}
	}
	
	ResetSet(result);
	return result;
}

/*
** DBG_SHOWFRAME: Show current stack.
*/
rep_set_t *
DbgShowFrame(rec_set_t *set, int count)
{
	char *	request;
	
	request = CommandToSocProto(DBGCMD_SHOWFRAME, count);

	rep_set_t* result = DbgSendCmdAndRecvReply(set, request, DBGEV_FRAME);
	
	Free(request);
	
	ResetSet(result);
	return result;
}

/*
** DBG_GETFRAME: Get selected stack frame.
*/
rep_set_t *
DbgGetFrame(rec_set_t *set)
{
	char *	request;
	
	request = CommandToSocProto(DBGCMD_GETFRAME);

	rep_set_t* result = DbgSendCmdAndRecvReply(set, request, DBGEV_FRAME);
	
	Free(request);
	
	ResetSet(result);
	return result;
}

/*
** DBG_GETVARS: Return defined variables.
*/
rep_set_t *
DbgGetVars(rec_set_t *set, char *file)
{
	char *	request;
	
	request = CommandToSocProto(DBGCMD_GETVARS, file);

	rep_set_t* result = DbgSendCmdAndRecvReply(set, request, DBGEV_VARS);
	
	Free(request);
	
	ResetSet(result);
	return result;
}

/*
** DBG_GETSCOPE: Return variables local to a scope.
*/
rep_set_t *
DbgGetScope(rec_set_t *set, char *location)
{
	dbgfrontend_sw *	dbg_fe = ((dbginvoke_t *)(set->s_dbginvoke_handler))->d_frontend;
	char *				request;
	rep_set_t * 		result;
	dbgevent_t *		e;
	group_reply_t *		group_rep;
	
	// We want to do the actual reading of the source files on the frontend if
	// possible. This is because there is a performance hit associated with
	// opening cross mounted files on the compute nodes. Its also the case that
	// sometimes we are unable to read the source files since they are located
	// in a non-cross mounted location.
	
	// Note that we can only do this if there is a debug frontend associated
	// with the dbginvoke
	if (dbg_fe != NULL)
	{
		// Create a new reply set
		result = NewRepSet(set->s_proto_handler);
		
		e = DbgFEGetScope(dbg_fe, location);
		
		// Make sure the event is a DBGEV_VARS or DBGEV_ERROR, otherwise create an error
		if (e->ev_event != DBGEV_VARS && e->ev_event != DBGEV_ERROR)
		{
			// Free the event
			FreeEvent(e);
			// Create a new error event
			EVENT_ERROR(e, DBGERR_DEBUGGER, "DbgFEGetScope returned unknown event other than error.");
		}
		
		// Add the event msg to the result based on its type
		group_rep = NewGroupReply(bitset_dup(set->s_receiver_bs), e);
		
		if (e->ev_event == DBGEV_ERROR)
		{
			AddErrToSet(result, group_rep);
		} else
		{
			AddReplyToSet(result, group_rep);
		}
		
		ResetSet(result);
		return result;
	}
	
	DPRINT("DbgGetScope: Falling back to backend evaluation.\n");
	
	request = CommandToSocProto(DBGCMD_GETSCOPE, location);

	result = DbgSendCmdAndRecvReply(set, request, DBGEV_VARS);
	
	Free(request);
	
	ResetSet(result);
	return result;
}

/*
** DBG_DIRECTCMD: Send direct command to debug backend
*/
rep_set_t *
DbgDirectCmd(rec_set_t *set, int argc, char **argv)
{
	char *	request;
	
	request = CommandToSocProto(DBGCMD_DIRECT, argc, argv);

	rep_set_t *result = DbgSendCmdAndRecvReply(set, request, DBGEV_CMD);
	
	Free(request);

	ResetSet(result);
	return result;
}

/*
** DBG_DBGSTATUS: Return status of debugger.
*/
rep_set_t *
DbgStatus(rec_set_t *set, int getLocation)
{
	char *	request;

	request = CommandToSocProto(DBGCMD_STATUS, getLocation); 

	rep_set_t *result = DbgSendCmdAndRecvReply(set, request, DBGEV_STAT);
	
	Free(request);
	
	ResetSet(result);
	return result;
}

/*
** DBG_INTERRUPT: Interrupt current debugger operation.
*/
rep_set_t *
DbgInterrupt(rec_set_t *set)
{
	char *	request;
	
	request = CommandToSocProto(DBGCMD_INTERRUPT);

	rep_set_t* result = DbgSendCmdAndRecvReply(set, request, DBGEV_HALT);
	
	// This command changes the status
	inferior_status_change = 1;
	
	Free(request);
	
	ResetSet(result);
	return result;
}

/*
** DBG_QUIT: Tell server to quit immediately.
** If detach is true, then we detach from application instead of killing it.
*/
rep_set_t *
DbgQuit(rec_set_t *set, int detach)
{
	char *			request;
	rep_set_t *		result;
	dbginvoke_t *	di = (dbginvoke_t *)set->s_dbginvoke_handler;
	group_reply_t *	err_rep;
	int				res;
	dbgevent_t *	e;
	
	request = CommandToSocProto(DBGCMD_QUIT, detach);

	result = DbgSendCmdAndRecvReply(set, request, DBGEV_OK);
	
	Free(request);
	
	// Call protocol specific communication network exit function
	if ( DBG_PROTO_SW(di)->sw_exit == NULL )
	{
		if (result == NULL)
		{
			result = NewRepSet(set->s_proto_handler);
		}
		
		// Create a new error event
		EVENT_ERROR(e, DBGERR_NOTIMP, NULL);
		err_rep = NewGroupReply(bitset_dup(set->s_receiver_bs), e);
		// Add the error to the reply set
		AddErrToSet(result, err_rep);
	} else
	{
		// FIXME: How to handle error case?
		res = DBG_PROTO_SW(di)->sw_exit(di, detach);
	}
	
	// Quit the debug frontend if there is one
	if (di->d_frontend != NULL)
	{
		e = DbgDestroyFE(di->d_frontend);
		// FIXME: How to error check here?
		FreeEvent(e);
		di->d_frontend = NULL;
	}
	
	FreeDbgInvoke(di);
	
	// This command causes the inferior to exit
	inferior_exit_change = 1;
	
	ResetSet(result);
	return result;
}

