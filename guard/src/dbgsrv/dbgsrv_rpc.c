/*
** Server RPC dispatch routines
**
** Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
**
*/

#ifdef HAVE_CONFIG_H
#include	<config.h>
#endif /* HAVE_CONFIG_H */

#if defined(sun) && !defined(PORTMAP)
#define PORTMAP
#endif /* sun && !PORTMAP */

#include	<sys/types.h>

#include	<string.h>

#include	<rpc/rpc.h>

#include	"defs.h"
#include	"aif.h"
#include	"dbgevent.h"
#include	"dbgsrv_proto.h"
#include	"dbgproto_rpc.h"
#include	"cbproto_rpc.h"
#include	"dbgfn.h"
#include	"dbg_rpc.h"

#ifdef sun
int             _rpcpmstart; /* required by Solaris */
#endif /* sun */

static u_long	ProgNum;
extern int	Shutdown;
extern void	dbgsrv_2();

void		EventToRes(dbg_res *, dbgevent_t *);
int		DbgSRVRPCInitProto(int, char **);
void		DbgSRVRPCEndProto(void);
fd_set *	DbgSRVRPCGetFDSet(void);
void		DbgSRVRPCRunProto(fd_set *);
extern int	InvokeRPCCallBack(char *, int, char *, int, int, int, dbgevent_t *);
extern int	EventRPCCallBack(char *, int, dbgevent_t *);

dbgsrv_proto	DbgSrvRPCProtoSW =
{
	DbgSRVRPCInitProto,
	DbgSRVRPCEndProto,
	DbgSRVRPCGetFDSet,
	DbgSRVRPCRunProto,
	InvokeRPCCallBack,
	EventRPCCallBack,
};

int
DbgSRVRPCInitProto(int argc, char **argv)
{
/*	int		pnum; */
	SVCXPRT *	tp;

	if ( CreateSvc(&tp, &ProgNum, dbgsrv_2, DBGVERSION) < 0 )
		return -1;

	return ProgNum;
}

void
DbgSRVRPCEndProto(void)
{
	svc_unregister(ProgNum, DBGVERSION);
}

fd_set *
DbgSRVRPCGetFDSet(void)
{
	static fd_set	fds;

	fds = svc_fdset;

	return &fds;
}

void
DbgSRVRPCRunProto(fd_set *fds)
{
	svc_getreqset(fds);
}

dbg_res *
#if defined(__linux) || defined(__APPLE__)
dbg_setlinebreak_2_svc(strint *args, struct svc_req *sv)
#else /* __linux || __APPLE__ */
dbg_setlinebreak_2(strint *args, struct svc_req *sv)
#endif /* __linux || __APPLE__ */
{
	dbgevent_t *	e;
	static dbg_res	res;

	xdr_free((xdrproc_t)xdr_dbg_res, (char *)&res);

	e = DbgSetLineBreak(args->s1, args->i1);

	EventToRes(&res, e);

	return &res;
}

dbg_res *
#if defined(__linux) || defined(__APPLE__)
dbg_setfuncbreak_2_svc(str2 *args, struct svc_req *sv)
#else /* __linux || __APPLE__ */
dbg_setfuncbreak_2(str2 *args, struct svc_req *sv)
#endif /* __linux || __APPLE__ */
{
	dbgevent_t *	e;
	static dbg_res	res;

	xdr_free((xdrproc_t)xdr_dbg_res, (char *)&res);

	e = DbgSetFuncBreak(args->s1, args->s2);

	EventToRes(&res, e);

	return &res;
}

dbg_res *
#if defined(__linux) || defined(__APPLE__)
dbg_deletebreak_2_svc(int *bp, struct svc_req *sv)
#else /* __linux || __APPLE__ */
dbg_deletebreak_2(int *bp, struct svc_req *sv)
#endif /* __linux || __APPLE__ */
{
	dbgevent_t *	e;
	static dbg_res	res;

	xdr_free((xdrproc_t)xdr_dbg_res, (char *)&res);

	e = DbgDeleteBreak(*bp);

	EventToRes(&res, e);

	return &res;
}

dbg_res *
#if defined(__linux) || defined(__APPLE__)
dbg_lastbreak_2_svc(void *args, struct svc_req *sv)
#else /* __linux || __APPLE__ */
dbg_lastbreak_2(void *args, struct svc_req *sv)
#endif /* __linux || __APPLE__ */
{
	dbgevent_t *	e;
	static dbg_res	res;

	xdr_free((xdrproc_t)xdr_dbg_res, (char *)&res);

	e = DbgLastBreak();

	EventToRes(&res, e);

	return &res;
}

dbg_res *
#if defined(__linux) || defined(__APPLE__)
dbg_list_2_svc(char **str, struct svc_req *sv)
#else /* __linux || __APPLE__ */
dbg_list_2(char **str, struct svc_req *sv)
#endif /* __linux || __APPLE__ */
{
	dbgevent_t *	e;
	static dbg_res	res;

	xdr_free((xdrproc_t)xdr_dbg_res, (char *)&res);

	e = DbgList(*str);

	EventToRes(&res, e);

	return &res;
}

dbg_res *
#if defined(__linux) || defined(__APPLE__)
dbg_go_2_svc(dbgcb_args *args, struct svc_req *sv)
#else /* __linux || __APPLE__ */
dbg_go_2(dbgcb_args *args, struct svc_req *sv)
#endif /* __linux || __APPLE__ */
{
	dbgevent_t *	e;
	static dbg_res	res;

	xdr_free((xdrproc_t)xdr_dbg_res, (char *)&res);

	e = DbgGo(args->cb_host, args->cb_prog);

	EventToRes(&res, e);

	return &res;
}

dbg_res *
#if defined(__linux) || defined(__APPLE__)
dbg_setargs_2_svc(char **args, struct svc_req *sv)
#else /* __linux || __APPLE__ */
dbg_setargs_2(char **args, struct svc_req *sv)
#endif /* __linux || __APPLE__ */
{
	dbgevent_t *	e;
	static dbg_res	res;

	xdr_free((xdrproc_t)xdr_dbg_res, (char *)&res);

	e = DbgSetArgs(*args);

	EventToRes(&res, e);

	return &res;
}

dbg_res *
#if defined(__linux) || defined(__APPLE__)
dbg_step_2_svc(dbgstep_args *args, struct svc_req *sv)
#else /* __linux || __APPLE__ */
dbg_step_2(dbgstep_args *args, struct svc_req *sv)
#endif /* __linux || __APPLE__ */
{
	dbgevent_t *	e;
	static dbg_res	res;

	xdr_free((xdrproc_t)xdr_dbg_res, (char *)&res);

	e = DbgStep(args->step_count, args->step_in, args->step_host, args->step_prog);

	EventToRes(&res, e);

	return &res;
}

dbg_res *
#if defined(__linux) || defined(__APPLE__)
dbg_kill_2_svc(void *args, struct svc_req *sv)
#else /* __linux || __APPLE__ */
dbg_kill_2(void *args, struct svc_req *sv)
#endif /* __linux || __APPLE__ */
{
	dbgevent_t *	e;
	static dbg_res	res;

	xdr_free((xdrproc_t)xdr_dbg_res, (char *)&res);

	e = DbgKill();

	EventToRes(&res, e);

	return &res;
}

dbg_res *
#if defined(__linux) || defined(__APPLE__)
dbg_moveframe_2_svc(int2 *args, struct svc_req *sv)
#else /* __linux || __APPLE__ */
dbg_moveframe_2(int2 *args, struct svc_req *sv)
#endif /* __linux || __APPLE__ */
{
	dbgevent_t *	e;
	static dbg_res	res;

	xdr_free((xdrproc_t)xdr_dbg_res, (char *)&res);

	e = DbgMoveFrame(args->i1, args->i2);

	EventToRes(&res, e);

	return &res;
}

dbg_res *
#if defined(__linux) || defined(__APPLE__)
dbg_showframe_2_svc(void *args, struct svc_req *sv)
#else /* __linux || __APPLE__ */
dbg_showframe_2(void *args, struct svc_req *sv)
#endif /* __linux || __APPLE__ */
{
	dbgevent_t *	e;
	static dbg_res	res;

	xdr_free((xdrproc_t)xdr_dbg_res, (char *)&res);

	e = DbgShowFrame();

	EventToRes(&res, e);

	return &res;
}

dbg_res *
#if defined(__linux) || defined(__APPLE__)
dbg_setvar_2_svc(dbgsetvar_args *args, struct svc_req *sv)
#else /* __linux || __APPLE__ */
dbg_setvar_2(dbgsetvar_args *args, struct svc_req *sv)
#endif /* __linux || __APPLE__ */
{
	dbgevent_t *	e;
	static dbg_res	res;

	xdr_free((xdrproc_t)xdr_dbg_res, (char *)&res);

	e = DbgSetVar(args->var, args->data);

	EventToRes(&res, e);

	return &res;
}

dbg_res *
#if defined(__linux) || defined(__APPLE__)
dbg_evalexpr_2_svc(char **expr, struct svc_req *sv)
#else /* __linux || __APPLE__ */
dbg_evalexpr_2(char **expr, struct svc_req *sv)
#endif /* __linux || __APPLE__ */
{
	dbgevent_t *	e;
	static dbg_res	res;

	xdr_free((xdrproc_t)xdr_dbg_res, (char *)&res);

	e = DbgEvalExpr(*expr);

	EventToRes(&res, e);

	return &res;
}

dbg_res *
#if defined(__linux) || defined(__APPLE__)
dbg_gettype_2_svc(char **var, struct svc_req *sv)
#else /* __linux || __APPLE__ */
dbg_gettype_2(char **var, struct svc_req *sv)
#endif /* __linux || __APPLE__ */
{
/*	char *		type; */
	dbgevent_t *	e;
	static dbg_res	res;

	xdr_free((xdrproc_t)xdr_dbg_res, (char *)&res);

	e = DbgGetType(*var);

	EventToRes(&res, e);

	return &res;
}

dbg_res *
#if defined(__linux) || defined(__APPLE__)
dbg_getlocation_2_svc(void *args, struct svc_req *sv)
#else /* __linux || __APPLE__ */
dbg_getlocation_2(void *args, struct svc_req *sv)
#endif /* __linux || __APPLE__ */
{
	dbgevent_t *	e;
	static dbg_res	res;

	xdr_free((xdrproc_t)xdr_dbg_res, (char *)&res);

	e = DbgGetLocation();

	EventToRes(&res, e);

	return &res;
}

dbg_res *
#if defined(__linux) || defined(__APPLE__)
dbg_dbgstatus_2_svc(void *args, struct svc_req *sv)
#else /* __linux || __APPLE__ */
dbg_dbgstatus_2(void *args, struct svc_req *sv)
#endif /* __linux || __APPLE__ */
{
	dbgevent_t *	e;
	static dbg_res	res;

	xdr_free((xdrproc_t)xdr_dbg_res, (char *)&res);

	e = DbgStatus();

	EventToRes(&res, e);

	return &res;
}

#ifdef notdef
dbg_res *
dbg_getevent_2(void)
{
	dbgevent_t *	e;
	static dbg_res	res;

	xdr_free((xdrproc_t)xdr_dbg_res, (char *)&res);

	e = DbgGetEvent();

	EventToRes(&res, e);

	return &res;
}
#endif /* notdef */

dbg_res *
#if defined(__linux) || defined(__APPLE__)
dbg_quit_2_svc(void *args, struct svc_req *sv)
#else /* __linux || __APPLE__ */
dbg_quit_2(void *args, struct svc_req *sv)
#endif /* __linux || __APPLE__ */
{
	static dbg_res	res;

	xdr_free((xdrproc_t)xdr_dbg_res, (char *)&res);

	EventToRes(&res, DbgQuit());

	return &res;
}

dbg_res *
#if defined(__linux) || defined(__APPLE__)
dbg_interrupt_2_svc(dbgcb_args *args, struct svc_req *sv)
#else /* __linux || __APPLE__ */
dbg_interrupt_2(dbgcb_args *args, struct svc_req *sv)
#endif /* __linux || __APPLE__ */
{
	dbgevent_t *	e;
	static dbg_res	res;

	xdr_free((xdrproc_t)xdr_dbg_res, (char *)&res);

	e = DbgInterrupt(args->cb_host, args->cb_prog);

	EventToRes(&res, e);

	return &res;
}
