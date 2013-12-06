/*
** Debug server protocol definitions.
**
** Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
**
*/

#ifndef _DBGSRV_PROTO_H
#define _DBGSRV_PROTO_H

#include	"defs.h"
#include	"dbgevent.h"

struct dbgsrv_proto
{
	int				(*sw_initproto)(int, int, char**, char*, char*, int);
	void			(*sw_endproto)(void);
	fd_set *		(*sw_getfdset)(void);
	void			(*sw_runproto)(fd_set *);
	int				(*sw_invokecb)(char *, int, char *, int, int, int, dbgevent_t *);
	int				(*sw_eventcb)(dbgevent_t *);
	int				(*sw_setupio)(int *, int *);
	int				(*sw_transferio)(int, char *, int);
	dbgevent_t *	(*sw_invokenetwork)(int);
	int				(*sw_sendreply)(char *);
};
typedef struct dbgsrv_proto	dbgsrv_proto;

extern int				DbgSetSrvProto(char *);
extern int				DbgInitProto(int, int, char**, char*, char*, int);
extern fd_set *			DbgGetFDSet(void);
extern void				DbgRunProto(fd_set *);
extern void				DbgEndProto(void);
extern int				DbgInvokeCB(char *, int, char *, int, int, int, dbgevent_t *);
extern int				DbgEventCB(dbgevent_t *);
extern int				DbgSetupIO(int *, int *);
extern int				DbgTransferIO(int, char *, int);
extern dbgevent_t *		DbgInvokeNetwork(int);
extern int				DbgSendReply(char *);

#endif /* !_DBGSRV_PROTO_H */
