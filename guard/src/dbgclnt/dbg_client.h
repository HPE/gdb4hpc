/*********************************************************************************\
 * Debugger client defs.
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
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



#ifndef _DBG_CLIENT_H
#define _DBG_CLIENT_H

#include <stdint.h>

/*
** These includes define a common interface that many of the functions which
** interface with the debugger client need to access. We place them here to 
** ensure that the function that is interfacing with the debugger client has the
** definitions it needs through including a single header.
*/
#include "defs.h"
#include "dbg.h"

#include "aif.h"
#include "bitset.h"
#include "dbgclnt_proto.h"
#include "dbgoutp.h"
#include "dbgclnt_debugfe.h"
#include "dbg_set_async.h"
#include "dbg_set_sync.h"

#define RPC_TIMEOUT	600
#define WDT_TIMEOUT	100
#define STARTER		"guard_starter"

/*
** debugger client struct and type definitions
*/
                                    
// This is data used by the protocol layer for application output
// Usually this will be a g_progoutp_t *, but it depends on the proto.
typedef void *	proto_outp;

// This is the protocol callback function prototype
typedef int	(*proto_ev_callback_func)(struct dbginvoke *, proto_msg_set *);
// This is the client callback function prototype
typedef int	(*clnt_ev_callback_func)(dbgevent_t *, bitset *, bitset *, void *);
// This is the proto program output callback function prototype
typedef void(*proto_outp_callback_func)(struct dbginvoke *, FILE *);

struct dbginvoke
{
	int							d_started;				/* indicates if this di has been started */
	int							d_nproc;				/* number of processes */
	int							d_pid;					/* pid of invoke process */
	int							d_apid;					/* apid of invoke process for Cray platform */
	int							d_stat;					/* exit/signal status of proc */
	list_t *					d_evfds;				/* event notification fds specific to this instance */
	bitset *					d_bitset_full;			/* represents all dbgsrvs*/
	bitset *					d_bitset_launched;		/* for invoke cmd: represents dbgsrvs that has been launched */
	
	int							d_proto_id;				/* di specific protocol id - optionally used by certain protocols */
	proto_network				d_proto_net;			/* di specific protocol network object */
	proto_outp					d_proto_outp;			/* di specific protocol output object */
	dbgclnt_proto *				d_proto;				/* client protocol switch */
	
	// The following two callbacks are for protocol event messages and for
	// handling the actual events in the client layer.
	proto_ev_callback_func		d_proto_ev_callback;
	clnt_ev_callback_func		d_clnt_ev_callback;
	// This is the callback for handling application output in the client
	proto_outp_callback_func	d_proto_outp_callback;
	
	dbgfrontend_sw *			d_frontend;				/* debug frontend object */
};
typedef struct dbginvoke	dbginvoke_t;
#define DBG_PROTO_SW(p) (p)->d_proto

// This defines the session interface. Sessions can be associated with launch/attach
// commands. It's up to the protocol implementation of this interface to handle
// the session object.
struct dbgclnt_session
{
	// determines if this session is still valid
	int					isValid;
	// number of dbginvoke_t objects that reference this session.
	int					refCount;
	// The protocol specific session implementation
	proto_session		pSession;
};
typedef struct dbgclnt_session dbgclnt_session;
#define DBG_SESS_VALID(p)			(p)->isValid
#define DBG_SESS_REFCNT(p)			(p)->refCount
#define DBG_SESS_PROTO_SESSION(p)	(p)->pSession

/*
** debugger client function prototypes
*/
extern void				ForceQuit(void);
extern void				HoldSignals(void);
extern void				ReleaseSignals(void);
extern dbginvoke_t *	NewDbgInvoke(int, dbgclnt_proto *);
extern void				FreeDbgInvoke(dbginvoke_t *);
extern SOCKET			ConnectIO(char *, int);

/*
** interface prototypes
*/
extern int				DbgInit(void);
extern void				DbgFinish(void);

extern int				DbgCheckEvents(void);
extern void				DbgDispatchEvents(void);
// DbgDispatchProtoEvent is the dbginvoke callback used to handle any protocol
// specific events received during the DbgDispatchEvents proto level call. Note
// that the msg is the unprocessed event network protocol string that will be
// converted to an actual event.
extern int				DbgDispatchProtoEvents(dbginvoke_t *di, proto_msg_set *set);
extern void				DbgDispatchProtoOutput(dbginvoke_t *di, FILE *p_o_stream);
extern void				DbgHandleEvent(dbginvoke_t *di, clnt_ev_callback_func ev_callback);
extern void				DbgHandleOutput(void (*)(dbginvoke_t *di, FILE *p_o_stream));
extern void				DbgHandleInput(int, void (*)(void));
extern void				DbgHandleSignal(int, void (*)(int));

extern void				DbgMaskInput(void);
extern void				DbgMaskSignal(int);
extern void				DbgUnmaskInput(void);
extern void				DbgUnmaskSignal(int);

// Used by higher layers to check if the inferior status has changed
extern int				DbgCheckForStatusChange(void);
extern void				DbgResetStatusChange(void);

// Used by higher layers to check if the inferior has exited
extern int				DbgCheckForExit(void);
extern void				DbgResetExitValue(void);

extern proto_session *	DbgSessionCreate(char *, WLM_type, char *, char *, char *);
extern void				DbgSetSession(proto_session *);
extern proto_session *	DbgGetSession(void);

extern dbginvoke_t *	DbgAttach(char *, proto_session *, int, char *, char *, int, int);
extern dbginvoke_t *	DbgInvoke(	char *, proto_session *, char *, char **, int, char *, char **, 
									char *, char *, char **, char *, int, int);

extern rep_set_t *		DbgStartup(rec_set_t *);
// This is the only thing that should be returning something other than rep_set_t *
// I don't know how to fix it since its for sending a synchronous command asynchronously
// TODO: This should probably be removed.
extern bitset *			DbgSendExpr(rec_set_t *, char *, int);
extern rep_set_t *		DbgGetExpr(rec_set_t *, bitset *, int);
extern rep_set_t *		DbgEvalExpr(rec_set_t *, char *, int);
// DbgSetDebuggerVar and DbgGetDebuggerVar is never called, this might be useful
// at some point though for staging data in gdb variables on the backends
extern rep_set_t *		DbgSetDebuggerVar(rec_set_t *, char *, AIF *);
extern rep_set_t *		DbgGetDebuggerVar(rec_set_t *, char *);
extern rep_set_t *		DbgSetSourcePath(rec_set_t *, char *);
extern rep_set_t *		DbgGetSourcePath(rec_set_t *);
// Returns NULL on success, otherwise reply will contain error
extern rep_set_t *		DbgSetVar(rec_set_t *, char *, AIF *);
extern rep_set_t *		DbgGetType(rec_set_t *, char *, int);
// Returns NULL on success, otherwise reply will contain error
extern rep_set_t *		DbgGo(rec_set_t *);
// Returns NULL on success, otherwise reply will contain error
extern rep_set_t *		DbgFinishFunc(rec_set_t *);
extern rep_set_t *		DbgSetLineBreak(rec_set_t *, char *, int);
extern rep_set_t *		DbgSetFuncBreak(rec_set_t *, char *, char *);
extern rep_set_t *		DbgSetWatch(rec_set_t *, int, char *);
// Returns NULL on success, otherwise reply will contain error
extern rep_set_t *		DbgKill(rec_set_t *);
// Returns NULL on success, otherwise reply will contain error
extern rep_set_t *		DbgDeleteBreak(rec_set_t *, int);
// Returns NULL on success, otherwise reply will contain error
extern rep_set_t *		DbgDisableBreak(rec_set_t *, int);
// Returns NULL on success, otherwise reply will contain error
extern rep_set_t *		DbgEnableBreak(rec_set_t *, int, int, int);
extern rep_set_t *		DbgList(rec_set_t *, char *);
extern rep_set_t *		DbgGetSources(rec_set_t *);
// Not called on a Cray, might be useful on a cluster though
extern rep_set_t *		DbgSetArgs(rec_set_t *, char *);
// Returns NULL on success, otherwise reply will contain error
extern rep_set_t *		DbgStep(rec_set_t *, int, int);
extern rep_set_t *		DbgMoveFrame(rec_set_t *, int, int);
extern rep_set_t *		DbgShowFrame(rec_set_t *, int);
extern rep_set_t *		DbgGetFrame(rec_set_t *);
extern rep_set_t *		DbgGetVars(rec_set_t *, char *);
extern rep_set_t *		DbgGetScope(rec_set_t *, char *);
extern rep_set_t *		DbgDirectCmd(rec_set_t *, int, char **);
extern rep_set_t *		DbgStatus(rec_set_t *, int);
extern rep_set_t *		DbgInterrupt(rec_set_t *);
extern rep_set_t *		DbgQuit(rec_set_t *, int);
extern rep_set_t *		DbgOpenSocket(rec_set_t *, char *, int);
extern rep_set_t *		DbgCloseSocket(rec_set_t *);

#endif /* !_DBG_CLIENT_H */
