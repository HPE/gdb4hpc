/*********************************************************************************\
 * Server socket dispatch routines
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



#ifdef HAVE_CONFIG_H
#include	<config.h>
#endif /* HAVE_CONFIG_H */

#include	<sys/types.h>
#include	<sys/time.h>
#include	<sys/socket.h>
#include	<stdlib.h>
#include	<string.h>

#include	<netdb.h>

#include	"alloc.h"
#include	"defs.h"
#include	"aif.h"
#include	"dbgevent.h"
#include	"dbgsrv_proto.h"
#include	"dbgfn.h"
#include	"dbg_soc.h"

static SOCPRT *	SocPtr;
static SOCPRT *	DataSocPtr = NULL;
extern int      Shutdown;
extern void     dbgsrv_soc();
extern char *	dbg_setlinebreak_soc(char *);
extern char *	dbg_setfuncbreak_soc(char *);
extern char *	dbg_deletebreak_soc(char *);
extern char *	dbg_lastbreak_soc();
extern char *	dbg_list_soc(char *);
extern char *	dbg_go_soc(char *);
extern char *	dbg_setargs_soc(char *);
extern char *	dbg_step_soc(char *);
extern char *	dbg_kill_soc();
extern char *	dbg_moveframe_soc(char *);
extern char *	dbg_showframe_soc();
extern char *   dbg_setdebuggervar_soc(char *);
extern char *   dbg_setsourcepath_soc(char *);
extern char *	dbg_getsourcepath_soc();
extern char *   dbg_getdebuggervar_soc(char *);
extern char *   dbg_setvar_soc(char *);
extern char *   dbg_evalexpr_soc(char *);
extern char *   dbg_gettype_soc(char *);
extern char *   dbg_getlocation_soc();
extern char *   dbg_getvars_soc(char *);
extern char *	dbg_status_soc();
extern char *	dbg_getevent_soc();
extern char *	dbg_quit_soc();
extern char *	dbg_interrupt_soc(char *);

int		DbgSRVSocInitProto(int, int, char **, char*, char*, int);
void		DbgSRVSocEndProto(void);
fd_set *	DbgSRVSocGetFDSet(void);
void		DbgSRVSocRunProto(fd_set *);
extern int	InvokeSocCallBack(char *, int, char *, int, int, int, dbgevent_t *);
extern int	EventSocCallBack(char *, int, dbgevent_t *);
int			DbgSRVSocTransferIO(int fd, char* buffer, int size);

dbgsrv_proto	DbgSrvSocProtoSW =
{
	DbgSRVSocInitProto,
	DbgSRVSocEndProto,
	DbgSRVSocGetFDSet,
	DbgSRVSocRunProto,
	InvokeSocCallBack,
	EventSocCallBack,
	DbgSRVSocTransferIO,
};

int
DbgSRVSocInitProto(int procnum, int argc, char **argv, char* key, char* host, int port)
{
/*        int             pnum; */

        if ( CreateSoc(&SocPtr, dbgsrv_soc) < 0 )
                return -1;

        return SocPtr->soc_port;
}

void
DbgSRVSocEndProto(void)
{
        soc_unregister(SocPtr);
        soc_destroy(SocPtr);
	/* !!! manjka: is soc_destroy missing ??? */
}

fd_set *
DbgSRVSocGetFDSet(void)
{
	static fd_set	fds;

	fds = soc_fdset;

	return &fds;
}

int
DbgSRVSocTransferIO(int fd, char* buffer, int size)
{
	int n;

	if ( fd < 0 )
		return -1;

	if ( (n = write(fd, bufffer, size)) < 0 )
	{
		perror("DoProgIO: write");
		return -1;
	}

	return 0;
}

/*
void
DbgSRVSocRunProto(fd_set *fds)
{
	soc_getreqset(fds);
}

void
dbgsrv_soc(int fd, SOCPRT *tp)
{
	int n;
        struct sockaddr sockaddr;
        socklen_t fromlen;
        int ns;
        SOCPRT *np;
	char *result;
	char *response;
        char command[1024];
	char * tail;
/*	dbgevent_t * e; */

	if ( fd == SocPtr->soc_fd ) {

                fromlen = sizeof(sockaddr);
                ns = accept(fd, &sockaddr, &fromlen);
                if (ns < 0) {
                        fprintf(stderr,"dbgsrv accept failed\n");
                        exit(1);
                }

                np = (SOCPRT *) Alloc(sizeof(SOCPRT));
                np->soc_fd = ns;
                np->soc_port = -1;
                np->soc_ofd = fd;

                if ( soc_register(ns, np, dbgsrv_soc ) < 0 )
                {
                        fprintf(stderr, "dbgsrv soc_register error\n");
                        exit(1);
                }

		if ( DataSocPtr != NULL ) {
                        fprintf(stderr, "dbgsrv dbgsrv_soc: two connections\n");
			
		}

		DataSocPtr = np;
		
		return;
	}

        n = soc_read(fd,&result);
        if (n <= 0) {
		soc_unregister(tp);
		soc_destroy(tp);
                return;
        }

	tail = getword(result,command);

	response = NULL;

	if (strcmp(command,"SETLINEBREAK") == 0)
	{
		response = dbg_setlinebreak_soc(tail);
	}
	else if (strcmp(command,"SETFUNCBREAK") == 0) 
	{
		response = dbg_setfuncbreak_soc(tail);
	}
	else if (strcmp(command,"DELETEBREAK") == 0) 
	{
		response = dbg_deletebreak_soc(tail);
	}
	else if (strcmp(command,"LASTBREAK") == 0) 
	{
		response = dbg_lastbreak_soc();
	}
	else if (strcmp(command,"LIST") == 0) 
	{
		response = dbg_list_soc(tail);
	}
	else if (strcmp(command,"GO") == 0) 
	{
		response = dbg_go_soc(tail);
	}
	else if (strcmp(command,"SETARGS") == 0) 
	{
		response = dbg_setargs_soc(tail);
	}
	else if (strcmp(command,"STEP") == 0) 
	{
		response = dbg_step_soc(tail);
	}
	else if (strcmp(command,"KILL") == 0) 
	{
		response = dbg_kill_soc();
	}
	else if (strcmp(command,"MOVEFRAME") == 0) 
	{
		response = dbg_moveframe_soc(tail);
	}
	else if (strcmp(command,"SHOWFRAME") == 0) 
	{
		response = dbg_showframe_soc();
	}
	else if (strcmp(command,"SETDEBUGGERVAR") == 0) 
	{
		response = dbg_setdebuggervar_soc(tail);
	}
	else if (strcmp(command,"SETSOURCEPATH") == 0)
	{
	        response = dbg_setsourcepath_soc(tail);
	}
	else if (strcmp(command,"GETSOURCEPATH") == 0)
	{
		response = dbg_getsourcepath_soc();
	}
	else if (strcmp(command,"GETDEBUGGERVAR") == 0) 
	{
		response = dbg_getdebuggervar_soc(tail);
	}
	else if (strcmp(command,"SETVAR") == 0) 
	{
		response = dbg_setvar_soc(tail);
	}
	else if (strcmp(command,"EVALEXPR") == 0) 
	{
		response = dbg_evalexpr_soc(tail);
	}
	else if (strcmp(command,"GETTYPE") == 0) 
	{
		response = dbg_gettype_soc(tail);
	}
	else if (strcmp(command,"GETLOCATION") == 0) 
	{
		response = dbg_getlocation_soc();
	}
	else if (strcmp(command,"GETVARS") == 0) 
	{
		response = dbg_getvars_soc(tail);
	}
	else if (strcmp(command,"STATUS") == 0) 
	{
		response = dbg_status_soc();
	}
#ifdef notdef
	else if (strcmp(command,"GETEVENT") == 0) 
	{
		response = dbg_getevent_soc();
	}
#endif
	else if (strcmp(command,"QUIT") == 0) 
	{
		response = dbg_quit_soc();
	}
	else if (strcmp(command,"INTERRUPT") == 0) 
	{
		response = dbg_interrupt_soc(tail);
	}
	else
	{
		fprintf(stderr,"dbgsrv: unknown command .%s.\n",command);
	}

	Free(result);

	if ( response != NULL ) 
	{
		if ( soc_sendreply(DataSocPtr,response) < 0 ) {
                        fprintf(stderr, "dbgsrv dbgsrv_soc soc_sendreply failed\n");
		}
		Free(response);
	}
}

char *
dbg_setlinebreak_soc(char *s)
{
	int		len;
	dbgevent_t *	e;
	char		par[1024];
	char *		file;
	int		line;
	char *		st_event;
	char *		response;

	s = getword(s,par);
	file = StrSocEventDup(par);

	s = getword(s,par);
	line = atoi(par);

	e = DbgSetLineBreak(file,line);

	Free(file);

	st_event = EventToSocProto(e);
	len = strlen(st_event) + 16;
	response = (char *) Alloc(len);
	snprintf(response, len-1, "%s\n",st_event);
	Free(st_event);
	
	return response;
}

char *
dbg_setfuncbreak_soc(char *s)
{
	int		len;
        dbgevent_t *    e;
        /* char *          tail; */
	char		par[1024];
        char *          file;
        char *          func;
        char *          st_event;
        char *          response;

        s = getword(s,par);
        file = StrSocEventDup(par);

        s = getword(s,par);
        func = StrSocEventDup(par);

        e = DbgSetFuncBreak(file,func);

	Free(file);
	Free(func);

        st_event = EventToSocProto(e);
	len = strlen(st_event) + 16;
	response = (char *) Alloc(len);
        snprintf(response, len-1, "%s\n",st_event);
        Free(st_event);

        return response;
}

char *
dbg_deletebreak_soc(char *s)
{
	int		len;
        dbgevent_t *    e;
        char            par[1024];
	int		bp;
        char *          st_event;
        char *          response;

        s = getword(s,par);
	bp = atoi(par);

        e = DbgDeleteBreak(bp);

        st_event = EventToSocProto(e);
	len = strlen(st_event) + 16;
	response = (char *) Alloc(len);
        snprintf(response, len-1, "%s\n",st_event);
        Free(st_event);

        return response;
}

char *
dbg_lastbreak_soc()
{
	int		len;
        dbgevent_t *    e;
        char *          st_event;
        char *          response;

        e = DbgLastBreak();

        st_event = EventToSocProto(e);
	len = strlen(st_event) + 16;
	response = (char *) Alloc(len);
        snprintf(response, len-1, "%s\n",st_event);
        Free(st_event);

        return response;
}

char *
dbg_list_soc(char *s)
{
	int		len;
        dbgevent_t *    e;
	char		par[1024];
	char *		list;
        char *          st_event;
        char *          response;

        s = getword(s,par);
	list = StrSocEventDup(par);

        e = DbgList(list);

        st_event = EventToSocProto(e);
	len = strlen(st_event) + 16;
	response = (char *) Alloc(len);
        snprintf(response, len-1, "%s\n",st_event);
        Free(st_event);

        return response;
}

char *
dbg_go_soc(char *s)
{
	int		len;
	dbgevent_t *	e;
	char		par[1024];
	char *		host;
	int		cb;
	char *		st_event;
	char *		response;

	s = getword(s,par);
        host = StrSocEventDup(par);

	s = getword(s,par);
	cb = atoi(par);

	e = DbgGo(host,cb);

	Free(host);

	st_event = EventToSocProto(e);
	len = strlen(st_event) + 16;
	response = (char *) Alloc(len);
	snprintf(response, len-1, "%s\n",st_event);
	Free(st_event);
	
	return response;
}

char *
dbg_setargs_soc(char *s)
{
	int		len;
        dbgevent_t *    e;
        char *          args;
        char *          st_event;
        char *          response;

        //s = getword(s,par);
	//args = StrSocEventDup(par);
	args = StrSocEventDup(s);

        e = DbgSetArgs(args);

	Free(args);

        st_event = EventToSocProto(e);
	len = strlen(st_event) + 16;
	response = (char *) Alloc(len);
        snprintf(response, len-1, "%s\n",st_event);
        Free(st_event);

        return response;
}

char *
dbg_step_soc(char *s)
{
	int		len;
	dbgevent_t *	e;
	char		par[1024];
	int		count;
	int		in;
	char *		host;
	int		cb;
	char *		st_event;
	char *		response;

	s = getword(s,par);
	count = atoi(par);

	s = getword(s,par);
	in = atoi(par);

	s = getword(s,par);
	host = StrSocEventDup(par);

	s = getword(s,par);
	cb = atoi(par);

	e = DbgStep(count,in,host,cb);

	Free(host);

	st_event = EventToSocProto(e);
	len = strlen(st_event) + 16;
	response = (char *) Alloc(len);
	snprintf(response, len-1, "%s\n",st_event);
	Free(st_event);
	
	return response;
}

char *
dbg_kill_soc()
{
	int		len;
        dbgevent_t *    e;
        char *          st_event;
        char *          response;

        e = DbgKill();

        st_event = EventToSocProto(e);
	len = strlen(st_event) + 16;
	response = (char *) Alloc(len);
        snprintf(response, len-1, "%s\n",st_event);
        Free(st_event);

        return response;
}

char *
dbg_moveframe_soc(char *s)
{
	int		len;
	dbgevent_t *	e;
	char		par[1024];
	int		count;
	int		up;
	char *		st_event;
	char *		response;

	s = getword(s,par);
	count = atoi(par);

	s = getword(s,par);
	up = atoi(par);

	e = DbgMoveFrame(count,up);

	st_event = EventToSocProto(e);
	len = strlen(st_event) + 16;
	response = (char *) Alloc(len);
	snprintf(response, len-1, "%s\n",st_event);
	Free(st_event);
	
	return response;
}

char *
dbg_showframe_soc()
{
	int		len;
	dbgevent_t *	e;
	char *		st_frame;
	char *		response;

	e = DbgShowFrame();
	
	st_frame = EventToSocProto(e);
	len = strlen(st_frame) + 16;
	response = (char *) Alloc(len);
	snprintf(response, len-1, "%s\n", st_frame);
	Free(st_frame);

	return response;
}

char *
dbg_setdebuggervar_soc(char *s)
{
	int		len;
	dbgevent_t *    e;
	char		var[1024];
	AIF *           aif;
	char *          st_event;
	char *          response;

	s = getword(s,var);
	s = skipspace(s);
	aif = SocProtoToAIF(s);

	e = DbgSetDebuggerVar(&var[0],aif);
	Free(aif);

	st_event = EventToSocProto(e);
	len = strlen(st_event) + 16;
	response = (char *) Alloc(len);
	snprintf(response, len-1, "%s\n",st_event);
	Free(st_event);

	return response;
}

char *
dbg_setsourcepath_soc(char *s)
{
	int		len;
	dbgevent_t *    e;
	AIF *           aif;
	char *          st_event;
	char *          response;

	aif = SocProtoToAIF(s);

	e = DbgSetSourcePath(aif);
	Free(aif);
	
	e = DbgGetSourcePath();

	st_event = EventToSocProto(e);
	len = strlen(st_event) + 16;
	response = (char *) Alloc(len);
	snprintf(response, len-1, "%s\n",st_event);
	Free(st_event);

	return response;
}

char *
dbg_getsourcepath_soc()
{
        int len;
  	dbgevent_t *    e;
	char *          st_event;
	char *          response;

	e = DbgGetSourcePath();

	st_event = EventToSocProto(e);
	len = strlen(st_event) + 16;
	response = (char *) Alloc(len);
	snprintf(response, len-1, "%s\n",st_event);
	Free(st_event);

	return response;
}

char *
dbg_getdebuggervar_soc(char *s)
{
	int		len;
	dbgevent_t *    e;
	char *          st_event;
	char *          response;

	s = skipspace(s);

	e = DbgGetDebuggerVar(s);

	st_event = EventToSocProto(e);
	len = strlen(st_event) + 16;
	response = (char *) Alloc(len);
	snprintf(response, len-1, "%s\n",st_event);
	Free(st_event);

	return response;
}

char *
dbg_setvar_soc(char *s)
{
	int		len;
	dbgevent_t *	e;
	char		var[1024];
	AIF *		aif;
	char *		st_event;
	char *		response;

	s = getword(s,var);
        s = skipspace(s);
	aif = SocProtoToAIF(s);

	e = DbgSetVar(var,aif);
	Free(aif);

        st_event = EventToSocProto(e);
	len = strlen(st_event) + 16;
	response = (char *) Alloc(len);
        snprintf(response, len-1, "%s\n",st_event);
        Free(st_event);

	return response;
}

char *
dbg_evalexpr_soc(char *s)
{
	int		len;
	dbgevent_t *	e;
	char *		st_event;
	char *		response;

        s = skipspace(s);

	e = DbgEvalExpr(s);

        st_event = EventToSocProto(e);
	len = strlen(st_event) + 16;
	response = (char *) Alloc(len);
        snprintf(response, len-1, "%s\n",st_event);
        Free(st_event);

	return response;
}

char *
dbg_gettype_soc(char *s)
{
	int		len;
	dbgevent_t *	e;
	char *		st_event;
	char *		response;

        s = skipspace(s);

	e = DbgGetType(s);

        st_event = EventToSocProto(e);
	len = strlen(st_event) + 16;
	response = (char *) Alloc(len);
        snprintf(response, len-1, "%s\n",st_event);
        Free(st_event);

	return response;
}

char *
dbg_getlocation_soc()
{
	int		len;
	dbgevent_t *	e;
	char *		st_event;
	char *		response;

	e = DbgGetLocation();

        st_event = EventToSocProto(e);
	len = strlen(st_event) + 16;
	response = (char *) Alloc(len);
        snprintf(response, len-1, "%s\n",st_event);
        Free(st_event);

	return response;
}

char *
dbg_getvars_soc(char *s)
{
	int		len;
        dbgevent_t *    e;
	char		par[1024];
        char *          file;
        char *          st_event;
        char *          response;

        s = getword(s,par);
	file = StrSocEventDup(par);

        e = DbgGetVars(file);
        
        Free(file);

        st_event = EventToSocProto(e);
	len = strlen(st_event) + 16;
	response = (char *) Alloc(len);
        snprintf(response, len-1, "%s\n",st_event);
        Free(st_event);

        return response;
}

char *
dbg_status_soc()
{
	int		len;
	dbgevent_t *    e;
	char *		st_status;
	char *		response;
	
	e = DbgStatus();
	st_status = EventToSocProto(e);
	len = strlen(st_status) + 16;
	response = (char *) Alloc(len);
	snprintf(response, len-1, "%s\n",st_status);
	Free(st_status);

	return response;
}

#ifdef notdef
char *
dbg_getevent_soc()
{
	int		len;
	dbgevent_t *	e;
	char *		st_event;
	char *		response;

	e = DbgGetEvent();

        st_event = EventToSocProto(e);
	len = strlen(st_event) + 16;
	response = (char *) Alloc(len);
        snprintf(response, len-1, "%s\n",st_event);
        Free(st_event);

	return response;
}
#endif

char *
dbg_quit_soc()
{
	int		len;
	char *		st_event;
	char *		response;
	
	st_event = EventToSocProto(DbgQuit());

	len = strlen(st_event) + 16;
	response = (char *) Alloc(len);
	snprintf(response, len-1, "%s\n",st_event);

	Free(st_event);

	Shutdown++;

	return response;
}

char *
dbg_interrupt_soc(char *s)
{
	int		len;
	dbgevent_t *	e;
	char		par[1024];
	char *		host;
	int		cb;
	char *		st_event;
	char *		response;

	s = getword(s,par);

	host = StrSocEventDup(par);

	s = getword(s,par);
	cb = atoi(par);

	e = DbgInterrupt(host,cb);

	Free(host);

	st_event = EventToSocProto(e);
	len = strlen(st_event) + 16;
	response = (char *) Alloc(len);
	snprintf(response, len-1, "%s\n",st_event);
	Free(st_event);
	
	return response;
}
