/*
** Debugger RPC interface routines
**
** Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
**
*/

#ifdef HAVE_CONFIG_H
#include	<config.h>
#endif /* HAVE_CONFIG_H */

#include	<sys/param.h>
#include	<sys/select.h>
#include	<sys/errno.h>
#include	<sys/socket.h>
#include	<sys/stat.h>
#include	<sys/types.h>
#include	<sys/wait.h>

#include	<rpc/rpc.h>
#include	<netinet/in.h>

#include	<errno.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<signal.h>
#include	<netdb.h>
#include	<unistd.h>

#include	"defs.h"
#include	"useful.h"
#include	"aif.h"
#include	"dbgevent.h"
#include	"dbg.h"
#include	"dbgproto_rpc.h"
#include	"cbproto_rpc.h"
#include	"dbg_client.h"
#include	"dbgclnt_proto.h"
#include	"cb_clnt.h"
#include	"dbg_rpc.h"

dbginvoke_t *	DbgRPCInvoke(dbgclnt_proto *, int (*)(struct dbgevent_t *, void *), char *, char **, int, char *, char *, char *, char *, char **, char *, char *, char **);
AIF *		DbgRPCEvalExpr(dbgproc_t *, char *, int);
int		DbgRPCSetVar(dbgproc_t *, char *, AIF *);
char *		DbgRPCGetType(dbgproc_t *, char *);
int		DbgRPCGo(dbgproc_t *);
dbgbp_t *	DbgRPCSetLineBreak(dbgproc_t *, char *, int);
dbgbp_t *	DbgRPCSetFuncBreak(dbgproc_t *, char *, char *);
int		DbgRPCKill(dbgproc_t *);
int		DbgRPCDeleteBreak(dbgproc_t *, int);
int		DbgRPCList(dbgproc_t *, char *, dbglist_t **);
int		DbgRPCSetArgs(dbgproc_t *, char *);
int		DbgRPCStep(dbgproc_t *, int, int);
dbgframe_t *	DbgRPCMoveFrame(dbgproc_t *, int, int);
dbgframe_t *	DbgRPCShowFrame(dbgproc_t *);
dbgloc_t *	DbgRPCGetLocation(dbgproc_t *);
dbgstat		DbgRPCStatus(dbgproc_t *);
int		DbgRPCInterrupt(dbgproc_t *);
int		DbgRPCQuit(dbgproc_t *);
void		DbgSetErr(int, char *);
void		DbgRPCDestroyClient(void *);
int		DbgRPCCheckEvents(void **);
void		DbgRPCDispatchEvents(void *);
void		reap(int);
int		cbRPCInvProc(char *, int, int, int, dbgevent_t *, void *);

extern int	CallBackRPCInit(cb_t *);
extern int	CallBackRPCWait(cb_t *);
extern void	CallBackRPCDestroy(cb_t *);

dbgclnt_proto DbgClntRPCProtoSW =
{
	"rpc",
	0,
	NULL,

	NULL, /* DbgRPCInit */
	NULL, /* DbgRPCFinish */

	NULL, /* DbgRPCAttach */
	DbgRPCInvoke,
	DbgRPCEvalExpr,
	DbgRPCSetVar,
	DbgRPCGetType,
	DbgRPCGo,
	DbgRPCSetLineBreak,
	DbgRPCSetFuncBreak,
	DbgRPCKill,
	DbgRPCDeleteBreak,
	DbgRPCList,
	DbgRPCSetArgs,
	DbgRPCStep,
	DbgRPCMoveFrame,
	DbgRPCShowFrame,
	DbgRPCGetLocation,
	(dbgvars_t *(*)())NULL,
	DbgRPCStatus,
	DbgRPCInterrupt,
	DbgRPCQuit,
	DbgRPCCheckEvents,
	DbgRPCDispatchEvents,
	NULL, /* DbgRPCRaiseEvent */
	DbgRPCDestroyClient,
	CallBackRPCInit,
	CallBackRPCWait,
	CallBackRPCDestroy,
	NULL, /* DbgRPCSetDebuggerVar */
	NULL  /* DbgRPCGetDebuggerVar */
};

/*
** Invoke user program using RPC.
*/
dbginvoke_t *
DbgRPCInvoke(
	dbgclnt_proto *proto,
	int (*cb)(struct dbgevent_t *, void *),
	char *prog,
	char **argv,
	int nproc,
	char *user,
	char *pass,
	char *host,
	char *arch,
	char **arch_args,
	char *backend,
	char *path,
	char **env
)
{
	int			sep = 0;
	char			sp[80];
	char			sn[80];
	char			starter[MAXPATHLEN];
	char **			ap;
	dbginvoke_t *		di;

	di = NewDbgInvoke(nproc, proto);

	if ( (di->d_cbp = CallBackCreate(di->d_proto, cbRPCInvProc, (void *)di)) == (cb_t *)NULL )
	{
		FreeDbgInvoke(di, DbgRPCDestroyClient);
		return (dbginvoke_t *)NULL;
	}

	di->d_cbrtn = cb;

	snprintf(sp, sizeof(sp), "%ld", di->d_cbp->cb_prog);

#ifndef WIN32
	/*
	** There is a race between returning a pid and
	** receiving a SIGCHLD if the new process exits
	** very quickly (such as if the executable can't
	** be found). We need to hold the signal until we've
	** made sure we've got the pid.
	*/
	sighold(SIGCHLD);
#endif /* !WIN32 */

	if ( arch == NULL )
		arch = "local";

	if ( path == NULL )
		path = BINDIR;

	if ( path[strlen(path)-1] == '/' )
		sep = 1;        

	snprintf(starter, MAXPATHLEN-1, sep ? "%s%s" : "%s/%s", path, STARTER);

	ap = NewArgv
	(
		STARTER,
		"-a", arch,
		"-c", di->d_cbp->cb_host,
		"-x", sp,
		"-r", proto->sw_name,
		NULL
	);

	if ( nproc > 1 )
	{
		snprintf(sn, sizeof(sn), "%d", nproc);
		ap = AppendArgs(ap, "-n", sn, NULL);
	}

	if ( host != NULL )
		ap = AppendArgs(ap, "-h", host, NULL);

	if ( user != NULL )
		ap = AppendArgs(ap, "-u", user, NULL);

	if ( pass != NULL )
		ap = AppendArgs(ap, "-p", pass, NULL);

	if ( backend != NULL )
		ap = AppendArgs(ap, "-e", backend, NULL);

	ap = AppendArgs(ap, "--", prog, NULL);

	ap = AppendArgv(ap, &argv[1]);

#ifndef WIN32
	if ( (di->d_pid = ForkAndExec(STARTER, ap, env)) < 0 )
	{
		if ( (di->d_pid = ForkAndExec(starter, ap, env)) < 0 )
		{
			FreeArgv(ap);
			FreeDbgInvoke(di, DbgRPCDestroyClient);
			sigrelse(SIGCHLD);
			return (dbginvoke_t *)NULL;
		}
	}
#else /* !WIN32 */
	Thread = _beginthread(Command, 0, (void *)ap);
#endif /* !WIN32 */

	FreeArgv(ap);

#ifndef WIN32
	sigrelse(SIGCHLD);
#endif /* !WIN32 */

	return di;
}

int
DbgRPCCheckEvents(void **data)
{
	fd_set *	fds;
	int		dtbsz = getdtablesize();
	struct timeval	tv = {0, 10000};

	if ( data == (void **) NULL )
		return -1;

	if ( *data == (void *)NULL )
	{
		fds = (fd_set *)Alloc(sizeof(fd_set));
		*data = (void *)fds;
	}
	else
		fds = (fd_set *)*data;

	*fds = svc_fdset;

	for ( ;; )
	{
		switch ( select(dtbsz, fds, NULL, NULL, &tv) )
		{
		case SOCKET_ERROR:
			if ( errno == EINTR )
				continue;

			DbgSetErr(DBGERR_SELECT, strerror(errno));
			return -1;

		case 0:
			return 0;

		default:
			break;
		}

		break;
	}

	return 1;
}

void
DbgRPCDispatchEvents(void *data)
{
	fd_set *	fds = (fd_set *)data;

	if ( fds == (fd_set *)NULL )
		return;

	svc_getreqset(fds);
}

/*
** Callback routine for RPC invoke.
*/
int
cbRPCInvProc(char *host, int prog, int pnum, int port, dbgevent_t *e, void *arg)
{
	int		(*cbrtn)();
	dbginvoke_t *	di = (dbginvoke_t *)arg;
	dbgproc_t *	dp;
	struct timeval	tv = {RPC_TIMEOUT, 0};

	if ( pnum >= 0 )
	{
		dp = di->d_procs[pnum];
		dp->d_tid = pnum;
	}
	else
	{
		dp = di->d_procs[di->d_tid];
		dp->d_tid = di->d_tid++;
	}

	if ( !dp->d_started )
	{
		dp->d_host = strdup(host);
		dp->d_prog = prog;
		dp->d_ioport = port;

		if ( e->ev_event == DBGEV_ERROR )
		{
			/*
			** Make callback to inform client of error
			*/
			dp->d_started = 0;
			di->d_failed = dp->d_tid;

			cbrtn = di->d_cbrtn;

			CallBackDestroy(di->d_proto, di->d_cbp);

			di->d_cbp = (cb_t *)NULL;
			di->d_cbdone = 1;

			return (*cbrtn)(e, (void *)di);
		}

		dp->d_started = 1;
	}
	else
		fprintf(stderr, "Received duplicate process number: %d\n", dp->d_tid);
	
	if ( (dp->d_client = (void *)clnt_create(dp->d_host, dp->d_prog, DBGVERSION, "tcp")) == (CLIENT *)NULL )
	{
		dbgevent_t *ee;

		clnt_pcreateerror(dp->d_host);

		EVENT_ERROR(ee, DBGERR_INVOKE, "");
		DbgSetErr(DBGERR_INVOKE, NULL);

		di->d_failed = dp->d_tid;

		cbrtn = di->d_cbrtn;

		CallBackDestroy(di->d_proto, di->d_cbp);

		di->d_cbp = (cb_t *)NULL;
		di->d_cbdone = 1;

		return (*cbrtn)(ee, (void *)di);
	}

	clnt_control((CLIENT *)dp->d_client, CLSET_TIMEOUT, (char *)&tv);

	HoldSignals();
	AddToList(DbgProcList, (void *)dp);
	ReleaseSignals();

	DPRINT("connected to task %d on %s\n", dp->d_tid, dp->d_host);

	if 
	( 
		dp->d_ioport >= 0
		&&
		(dp->d_iofd = ConnectIO(dp->d_host, dp->d_ioport)) == INVALID_SOCKET 
	)
		printf("i/o connection failed (%d)\n", dp->d_ioport);

	++(di->d_nstart);

	if ( di->d_nstart == di->d_nproc )
	{
		cbrtn = di->d_cbrtn;

		CallBackDestroy(di->d_proto, di->d_cbp);
		di->d_cbp = (cb_t *)NULL;
		di->d_cbdone = 1;
		return (*cbrtn)(e, (void *)di);
	}	

	return dp->d_tid;
}

/*
**********************************************************
**********************************************************
************ WRAPPERS FOR RPC ROUTINES *******************
**********************************************************
**********************************************************
*/

/*
** DBG_EVALEXPR: Evaluate expression.
*/
AIF *
DbgRPCEvalExpr(dbgproc_t *dp, char *exp, int assid)
{
	dbg_res *	res;
	AIF *		a;

	if ( !dp->d_started )
	{
		DbgSetErr(DBGERR_SERVER, NULL);
		return (AIF *)NULL;
	}

	HoldSignals();

	if ( (res = dbg_evalexpr_2(&exp, (CLIENT *)dp->d_client)) == (dbg_res *)NULL )
	{
		clnt_perror((CLIENT *)dp->d_client, dp->d_host);
		DbgSetErr(DBGERR_RPC, NULL);
		DestroyDbgProc(dp, DbgRPCDestroyClient);
		ReleaseSignals();
		return (AIF *)NULL;
	}

	ReleaseSignals();

	if ( res->status == RES_ERROR )
	{
		DbgSetErr(res->res_err, res->res_errstr);
		clnt_freeres((CLIENT *)dp->d_client, (xdrproc_t)xdr_dbg_res, (char *)res);
		return (AIF *)NULL;
	}

	a = CopyAIF(res->res_data);

	clnt_freeres((CLIENT *)dp->d_client, (xdrproc_t)xdr_dbg_res, (char *)res);

	return a;
}

/*
** DBG_SETVAR: Set variable in program to new value.
*/
int
DbgRPCSetVar(dbgproc_t *dp, char *var, AIF *val)
{
	dbg_res *	res;
	dbgsetvar_args	args;

	if ( !dp->d_started )
	{
		DbgSetErr(DBGERR_SERVER, NULL);
		return -1;
	}

	args.var = var;
	args.data = val;

	HoldSignals();

	if ( (res = dbg_setvar_2(&args, (CLIENT *)dp->d_client)) == (dbg_res *)NULL )
	{
		clnt_perror((CLIENT *)dp->d_client, dp->d_host);
		DbgSetErr(DBGERR_RPC, NULL);
		DestroyDbgProc(dp, DbgRPCDestroyClient);
		ReleaseSignals();
		return -1;
	}

	ReleaseSignals();

	if ( res->status == RES_ERROR )
	{
		DbgSetErr(res->res_err, res->res_errstr);
		clnt_freeres((CLIENT *)dp->d_client, (xdrproc_t)xdr_dbg_res, (char *)res);
		return -1;
	}

	clnt_freeres((CLIENT *)dp->d_client, (xdrproc_t)xdr_dbg_res, (char *)res);

	return 0;
}

/*
** DBG_GETTYPE: Get type of variable.
*/
char *
DbgRPCGetType(dbgproc_t *dp, char *var)
{
	dbg_res *	res;
	char *		val;

	if ( !dp->d_started )
	{
		DbgSetErr(DBGERR_SERVER, NULL);
		return NULL;
	}

	HoldSignals();

	if ( (res = dbg_gettype_2(&var, (CLIENT *)dp->d_client)) == (dbg_res *)NULL )
	{
		clnt_perror((CLIENT *)dp->d_client, dp->d_host);
		DbgSetErr(DBGERR_RPC, NULL);
		DestroyDbgProc(dp, DbgRPCDestroyClient);
		ReleaseSignals();
		return NULL;
	}

	ReleaseSignals();

	if ( res->status == RES_ERROR )
	{
		DbgSetErr(res->res_err, res->res_errstr);
		clnt_freeres((CLIENT *)dp->d_client, (xdrproc_t)xdr_dbg_res, (char *)res);
		return NULL;
	}

	val = strdup(res->res_type);

	clnt_freeres((CLIENT *)dp->d_client, (xdrproc_t)xdr_dbg_res, (char *)res);

	return val;
}

/*
** DBG_GO: Continue/start execution.
*/
int
DbgRPCGo(dbgproc_t *dp)
{
	dbg_res *	res;
	dbgcb_args	args;

	if ( !dp->d_started )
	{
		DbgSetErr(DBGERR_SERVER, NULL);
		return -1;
	}

	if ( dp->d_cbp == (cb_t *)NULL )
	{
		DbgSetErr(DBGERR_CALLBACK, NULL);
		return -1;
	}

	args.cb_host = dp->d_cbp->cb_host;
	args.cb_prog = dp->d_cbp->cb_prog;

	HoldSignals();

	if ( (res = dbg_go_2(&args, (CLIENT *)dp->d_client)) == (dbg_res *)NULL )
	{
		clnt_perror((CLIENT *)dp->d_client, dp->d_host);
		DbgSetErr(DBGERR_RPC, NULL);
		DestroyDbgProc(dp, DbgRPCDestroyClient);
		ReleaseSignals();
		return -1;
	}

	ReleaseSignals();

	if ( res->status == RES_ERROR )
	{
		DbgSetErr(res->res_err, res->res_errstr);
		clnt_freeres((CLIENT *)dp->d_client, (xdrproc_t)xdr_dbg_res, (char *)res);
		return -1;
	}

	clnt_freeres((CLIENT *)dp->d_client, (xdrproc_t)xdr_dbg_res, (char *)res);

	return 0;
}

/*
** DBG_SETLINEBREAK: Set a breakpoint in program at 
**                   specified line.
*/
dbgbp_t *
DbgRPCSetLineBreak(dbgproc_t *dp, char *file, int line)
{
	dbg_res *	res;
	dbgbp_t *	bp;
	strint		args;

	if ( !dp->d_started )
	{
		DbgSetErr(DBGERR_SERVER, NULL);
		return (dbgbp_t *)NULL;
	}

	if ( file == NULL )
		file = "";

	args.s1 = file;
	args.i1 = line;

	HoldSignals();

	if ( (res = dbg_setlinebreak_2(&args, (CLIENT *)dp->d_client)) == (dbg_res *)NULL )
	{
		clnt_perror((CLIENT *)dp->d_client, dp->d_host);
		DbgSetErr(DBGERR_RPC, NULL);
		DestroyDbgProc(dp, DbgRPCDestroyClient);
		ReleaseSignals();
		return (dbgbp_t *)NULL;
	}

	ReleaseSignals();

	if ( res->status == RES_ERROR )
	{
		DbgSetErr(res->res_err, res->res_errstr);
		clnt_freeres((CLIENT *)dp->d_client, (xdrproc_t)xdr_dbg_res, (char *)res);
		return (dbgbp_t *)NULL;
	}

	bp = ResToBP(res->res_bpset);

	clnt_freeres((CLIENT *)dp->d_client, (xdrproc_t)xdr_dbg_res, (char *)res);

	return bp;
}

/*
** DBG_SETFUNCBREAK: Set a breakpoint in program at 
**                   specified function.
*/
dbgbp_t *
DbgRPCSetFuncBreak(dbgproc_t *dp, char *file, char *func)
{
	dbg_res *	res;
	dbgbp_t *	bp;
	str2		args;

	if ( !dp->d_started )
	{
		DbgSetErr(DBGERR_SERVER, NULL);
		return (dbgbp_t *)NULL;
	}

	if ( file == NULL )
		file = "";

	args.s1 = file;
	args.s2 = func;

	HoldSignals();

	if ( (res = dbg_setfuncbreak_2(&args, (CLIENT *)dp->d_client)) == (dbg_res *)NULL )
	{
		clnt_perror((CLIENT *)dp->d_client, dp->d_host);
		DbgSetErr(DBGERR_RPC, NULL);
		DestroyDbgProc(dp, DbgRPCDestroyClient);
		ReleaseSignals();
		return (dbgbp_t *)NULL;
	}

	ReleaseSignals();

	if ( res->status == RES_ERROR )
	{
		DbgSetErr(res->res_err, res->res_errstr);
		clnt_freeres((CLIENT *)dp->d_client, (xdrproc_t)xdr_dbg_res, (char *)res);
		return (dbgbp_t *)NULL;
	}

	bp = ResToBP(res->res_bpset);

	clnt_freeres((CLIENT *)dp->d_client, (xdrproc_t)xdr_dbg_res, (char *)res);

	return bp;
}

/*
** DBG_KILL: Terminate execution of program.
*/
int
DbgRPCKill(dbgproc_t *dp)
{
	dbg_res *	res;

	if ( !dp->d_started )
	{
		DbgSetErr(DBGERR_SERVER, NULL);
		return -1;
	}

	HoldSignals();

	if ( (res = dbg_kill_2(NULL, (CLIENT *)dp->d_client)) == (dbg_res *)NULL )
	{
		clnt_perror((CLIENT *)dp->d_client, dp->d_host);
		DbgSetErr(DBGERR_RPC, NULL);
		DestroyDbgProc(dp, DbgRPCDestroyClient);
		ReleaseSignals();
		return -1;
	}

	ReleaseSignals();

	if ( res->status == RES_ERROR )
	{
		DbgSetErr(res->res_err, res->res_errstr);
		clnt_freeres((CLIENT *)dp->d_client, (xdrproc_t)xdr_dbg_res, (char *)res);
		return -1;
	}

	clnt_freeres((CLIENT *)dp->d_client, (xdrproc_t)xdr_dbg_res, (char *)res);

	return 0;
}

/*
** DBG_DELETEBREAK: Delete a breakpoint.
**
** Note: the GDB "delete breakpoint" command (delete all
** breakpoints) must be implemented using DBG_SHOWBREAK
** and repeated DBG_DELETEBREAK.
*/
int
DbgRPCDeleteBreak(dbgproc_t *dp, int bp)
{
	dbg_res *	res;

	if ( !dp->d_started )
	{
		DbgSetErr(DBGERR_SERVER, NULL);
		return -1;
	}

	HoldSignals();

	if ( (res = dbg_deletebreak_2(&bp, (CLIENT *)dp->d_client)) == (dbg_res *)NULL )
	{
		clnt_perror((CLIENT *)dp->d_client, dp->d_host);
		DbgSetErr(DBGERR_RPC, NULL);
		DestroyDbgProc(dp, DbgRPCDestroyClient);
		ReleaseSignals();
		return -1;
	}

	ReleaseSignals();

	if ( res->status == RES_ERROR )
	{
		DbgSetErr(res->res_err, res->res_errstr);
		clnt_freeres((CLIENT *)dp->d_client, (xdrproc_t)xdr_dbg_res, (char *)res);
		return -1;
	}

	clnt_freeres((CLIENT *)dp->d_client, (xdrproc_t)xdr_dbg_res, (char *)res);

	return 0;
}

#ifdef notdef
/*
** DBG_SHOWBREAK: List all breakpoints.
*/
dbgbp_t *
DbgRPCShowBreak(dbgproc_t *dp)
{
	dbg_res *	res;

	if ( !dp->d_started )
	{
		DbgSetErr(DBGERR_SERVER, NULL);
		return (dbgbp_t *)NULL;
	}

	HoldSignals();

	if ( (res = dbg_showbreak_2(NULL, (CLIENT *)dp->d_client)) == (dbg_res *)NULL )
	{
		clnt_perror((CLIENT *)dp->d_client, dp->d_host);
		DbgSetErr(DBGERR_RPC, NULL);
		DestroyDbgProc(dp, DbgRPCDestroyClient);
		ReleaseSignals();
		return (dbgbp_t *)NULL;
	}

	ReleaseSignals();

	if ( res->status == RES_ERROR )
		return (dbgbp_t *)NULL;

	return (dbgbp_t *)res->res_bpinfo;
}
#endif /* notdef */

/*
** DBG_LIST: List specified function or line.
*/

int
DbgRPCList(dbgproc_t *dp, char *str, dbglist_t **list)
{
	char *		s = "";
	dbg_res *	res;

	if ( !dp->d_started )
	{
		DbgSetErr(DBGERR_SERVER, NULL);
		return -1;
	}

	if ( str != NULL )
		s = str;

	HoldSignals();

	if ( (res = dbg_list_2(&s, (CLIENT *)dp->d_client)) == (dbg_res *)NULL )
	{
		clnt_perror((CLIENT *)dp->d_client, dp->d_host);
		DbgSetErr(DBGERR_RPC, NULL);
		DestroyDbgProc(dp, DbgRPCDestroyClient);
		ReleaseSignals();
		return -1;
	}

	ReleaseSignals();

	if ( res->status == RES_ERROR )
	{
		DbgSetErr(res->res_err, res->res_errstr);
		clnt_freeres((CLIENT *)dp->d_client, (xdrproc_t)xdr_dbg_res, (char *)res);
		return -1;
	}

	*list = ResToList(res->res_list);

	clnt_freeres((CLIENT *)dp->d_client, (xdrproc_t)xdr_dbg_res, (char *)res);

	return 0;
}

/*
** DBG_SETARGS: Set arguments to pass to program when run.
*/
int
DbgRPCSetArgs(dbgproc_t *dp, char *args)
{
	char *		s = "";
	dbg_res *	res;

	if ( !dp->d_started )
	{
		DbgSetErr(DBGERR_SERVER, NULL);
		return -1;
	}

	if ( args != NULL )
		s = args;

	HoldSignals();

	if ( (res = dbg_setargs_2(&s, (CLIENT *)dp->d_client)) == (dbg_res *)NULL )
	{
		clnt_perror((CLIENT *)dp->d_client, dp->d_host);
		DbgSetErr(DBGERR_RPC, NULL);
		DestroyDbgProc(dp, DbgRPCDestroyClient);
		ReleaseSignals();
		return -1;
	}

	ReleaseSignals();

	if ( res->status == RES_ERROR )
	{
		DbgSetErr(res->res_err, res->res_errstr);
		clnt_freeres((CLIENT *)dp->d_client, (xdrproc_t)xdr_dbg_res, (char *)res);
		return -1;
	}

	clnt_freeres((CLIENT *)dp->d_client, (xdrproc_t)xdr_dbg_res, (char *)res);

	return 0;
}

/*
** DBG_STEP: Execute to next statement. Repeat count times.
**           If in == 0, pass over function calls (ala next).
*/
int
DbgRPCStep(dbgproc_t *dp, int count, int in)
{
	dbg_res *	res;
	dbgstep_args	args;

	if ( !dp->d_started )
	{
		DbgSetErr(DBGERR_SERVER, NULL);
		return -1;
	}

	if ( dp->d_cbp == (cb_t *)NULL )
	{
		DbgSetErr(DBGERR_CALLBACK, NULL);
		return -1;
	}

	args.step_host = dp->d_cbp->cb_host;
	args.step_prog = dp->d_cbp->cb_prog;
	args.step_count = count;
	args.step_in = in;

	HoldSignals();

	if ( (res = dbg_step_2(&args, (CLIENT *)dp->d_client)) == (dbg_res *)NULL )
	{
		clnt_perror((CLIENT *)dp->d_client, dp->d_host);
		DbgSetErr(DBGERR_RPC, NULL);
		DestroyDbgProc(dp, DbgRPCDestroyClient);
		ReleaseSignals();
		return -1;
	}

	ReleaseSignals();

	if ( res->status == RES_ERROR )
	{
		DbgSetErr(res->res_err, res->res_errstr);
		clnt_freeres((CLIENT *)dp->d_client, (xdrproc_t)xdr_dbg_res, (char *)res);
		return -1;
	}

	clnt_freeres((CLIENT *)dp->d_client, (xdrproc_t)xdr_dbg_res, (char *)res);

	return 0;
}

/*
** DBG_MOVEFRAME: Move up or down stack frames.
*/
dbgframe_t *
DbgRPCMoveFrame(dbgproc_t *dp, int count, int up)
{
	dbg_res *	res;
	dbgframe_t *	fr;
	int2		frame;

	if ( !dp->d_started )
	{
		DbgSetErr(DBGERR_SERVER, NULL);
		return (dbgframe_t *)NULL;
	}

	frame.i1 = count;
	frame.i2 = up;

	HoldSignals();

	if ( (res = dbg_moveframe_2(&frame, (CLIENT *)dp->d_client)) == (dbg_res *)NULL )
	{
		clnt_perror((CLIENT *)dp->d_client, dp->d_host);
		DbgSetErr(DBGERR_RPC, NULL);
		DestroyDbgProc(dp, DbgRPCDestroyClient);
		ReleaseSignals();
		return (dbgframe_t *)NULL;
	}

	ReleaseSignals();

	if ( res->status == RES_ERROR )
	{
		DbgSetErr(res->res_err, res->res_errstr);
		clnt_freeres((CLIENT *)dp->d_client, (xdrproc_t)xdr_dbg_res, (char *)res);
		return (dbgframe_t *)NULL;
	}

	fr = ResToFrame(res->res_frame);

	clnt_freeres((CLIENT *)dp->d_client, (xdrproc_t)xdr_dbg_res, (char *)res);

	return fr;
}

/*
** DBG_SHOWFRAME: Show current stack.
*/
dbgframe_t *
DbgRPCShowFrame(dbgproc_t *dp)
{
	dbg_res *	res;
	dbgframe_t *	fr;

	if ( !dp->d_started )
	{
		DbgSetErr(DBGERR_SERVER, NULL);
		return (dbgframe_t *)NULL;
	}

	HoldSignals();

	if ( (res = dbg_showframe_2(NULL, (CLIENT *)dp->d_client)) == (dbg_res *)NULL )
	{
		clnt_perror((CLIENT *)dp->d_client, dp->d_host);
		DbgSetErr(DBGERR_RPC, NULL);
		DestroyDbgProc(dp, DbgRPCDestroyClient);
		ReleaseSignals();
		return (dbgframe_t *)NULL;
	}

	ReleaseSignals();

	if ( res->status == RES_ERROR )
	{
		DbgSetErr(res->res_err, res->res_errstr);
		clnt_freeres((CLIENT *)dp->d_client, (xdrproc_t)xdr_dbg_res, (char *)res);
		return (dbgframe_t *)NULL;
	}

	fr = ResToFrame(res->res_frame);

	clnt_freeres((CLIENT *)dp->d_client, (xdrproc_t)xdr_dbg_res, (char *)res);

	return fr;
}

/*
** DBG_GETLOCATION: Return current location in program.
*/
dbgloc_t *
DbgRPCGetLocation(dbgproc_t *dp)
{
	dbg_res *	res;
	dbgloc_t *	loc;

	if ( !dp->d_started )
	{
		DbgSetErr(DBGERR_SERVER, NULL);
		return (dbgloc_t *)NULL;
	}

	HoldSignals();

	if ( (res = dbg_getlocation_2(NULL, (CLIENT *)dp->d_client)) == (dbg_res *)NULL )
	{
		clnt_perror((CLIENT *)dp->d_client, dp->d_host);
		DbgSetErr(DBGERR_RPC, NULL);
		DestroyDbgProc(dp, DbgRPCDestroyClient);
		ReleaseSignals();
		return (dbgloc_t *)NULL;
	}

	ReleaseSignals();

	if ( res->status == RES_ERROR )
	{
		DbgSetErr(res->res_err, res->res_errstr);
		clnt_freeres((CLIENT *)dp->d_client, (xdrproc_t)xdr_dbg_res, (char *)res);
		return (dbgloc_t *)NULL;
	}

	loc = ResToLoc(res->res_loc);

	clnt_freeres((CLIENT *)dp->d_client, (xdrproc_t)xdr_dbg_res, (char *)res);

	return loc;
}

/*
** DBG_DBGSTATUS: Return status of debugger.
*/
dbgstat
DbgRPCStatus(dbgproc_t *dp)
{
	dbg_res *	res;
	dbgstat		stat;

	if ( !dp->d_started )
	{
		DbgSetErr(DBGERR_SERVER, NULL);
		return DBGSTAT_INTERR;
	}

	HoldSignals();

	if ( (res = dbg_dbgstatus_2(NULL, (CLIENT *)dp->d_client)) == (dbg_res *)NULL )
	{
		clnt_perror((CLIENT *)dp->d_client, dp->d_host);
		DbgSetErr(DBGERR_RPC, NULL);
		DestroyDbgProc(dp, DbgRPCDestroyClient);
		ReleaseSignals();
		return DBGSTAT_INTERR;
	}

	if ( res->status == RES_ERROR )
	{
		DbgSetErr(res->res_err, res->res_errstr);
		clnt_freeres((CLIENT *)dp->d_client, (xdrproc_t)xdr_dbg_res, (char *)res);
		ReleaseSignals();
		return DBGSTAT_INTERR;
	}

	stat = (dbgstat)res->res_status;

	clnt_freeres((CLIENT *)dp->d_client, (xdrproc_t)xdr_dbg_res, (char *)res);

	ReleaseSignals();

	return stat;
}

/*
** DBG_INTERRUPT: Interrupt current debugger operation.
*/
int
DbgRPCInterrupt(dbgproc_t *dp)
{
	dbg_res *	res;
	dbgcb_args	args;

	if ( !dp->d_started )
	{
		DbgSetErr(DBGERR_SERVER, NULL);
		return -1;
	}

	/*
	** Check if process has callback (i.e. is running)
	*/
	if ( dp->d_cbp == (cb_t *)NULL )
		return 0;

	args.cb_host = dp->d_cbp->cb_host;
	args.cb_prog = dp->d_cbp->cb_prog;

	HoldSignals();

	if ( (res = dbg_interrupt_2(&args, (CLIENT *)dp->d_client)) == (dbg_res *)NULL )
	{
		clnt_perror((CLIENT *)dp->d_client, dp->d_host);
		DbgSetErr(DBGERR_RPC, NULL);
		DestroyDbgProc(dp, DbgRPCDestroyClient);
		ReleaseSignals();
		return -1;
	}

	ReleaseSignals();

	if ( res->status == RES_ERROR )
	{
		DbgSetErr(res->res_err, res->res_errstr);
		clnt_freeres((CLIENT *)dp->d_client, (xdrproc_t)xdr_dbg_res, (char *)res);
		return -1;
	}

	clnt_freeres((CLIENT *)dp->d_client, (xdrproc_t)xdr_dbg_res, (char *)res);

	return 0;
}

/*
** Called by DbgQuit() and DbgQuitAll()
*/
int
DbgRPCQuit(dbgproc_t *dp)
{
	int		ret = 0;
	dbg_res *	res;

	if ( !dp->d_started )
		return 0;

	printf("Shutting down server on %s.\n", dp->d_host);

	HoldSignals();

	if ( (res = dbg_quit_2(NULL, (CLIENT *)dp->d_client)) == (dbg_res *)NULL )
	{
		clnt_perror((CLIENT *)dp->d_client, dp->d_host);
		DbgSetErr(DBGERR_RPC, NULL);
		ret = -1;
	}
	else if ( res->status == RES_ERROR )
	{
		DbgSetErr(res->res_err, res->res_errstr);
		ret = -1;
	}


	clnt_freeres((CLIENT *)dp->d_client, (xdrproc_t)xdr_dbg_res, (char *)res);

	DestroyDbgProc(dp, DbgRPCDestroyClient);

	ReleaseSignals();

	return ret;
}

/*
** Destroy client connection.
*/
void
DbgRPCDestroyClient(void *clnt)
{
	clnt_destroy((CLIENT *)clnt);
}
