/*
** Debugger socket interface routines
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
#include	<sys/time.h>
#include	<sys/types.h>
#include	<sys/wait.h>
#include	<netinet/in.h>
#include	<netdb.h>
#include	<unistd.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<signal.h>
#include	<errno.h>

#include	"defs.h"
#include	"useful.h"
#include	"aif.h"
#include	"dbgevent.h"
#include	"dbg.h"
#include	"dbg_client.h"
#include	"dbgclnt_proto.h"
#include	"cb_clnt.h"
#include	"dbg_soc.h"

dbginvoke_t *	DbgSocInvoke(dbgclnt_proto *,  int (*)(struct dbgevent_t *, void *), char *, char **, int, char *, char *, char *, char *, char **, char *, char *, char **);
AIF *		DbgSocEvalExpr(dbgproc_t *, char *, int);
int		DbgSocSetVar(dbgproc_t *, char *, AIF *);
char *		DbgSocGetType(dbgproc_t *, char *);
int		DbgSocGo(dbgproc_t *);
dbgbp_t *	DbgSocSetLineBreak(dbgproc_t *, char *, int);
dbgbp_t *	DbgSocSetFuncBreak(dbgproc_t *, char *, char *);
int		DbgSocKill(dbgproc_t *);
int		DbgSocDeleteBreak(dbgproc_t *, int);
int		DbgSocList(dbgproc_t *, char *, dbglist_t **);
int		DbgSocSetArgs(dbgproc_t *, char *);
int		DbgSocStep(dbgproc_t *, int, int);
dbgframe_t *	DbgSocMoveFrame(dbgproc_t *, int, int);
dbgframe_t *	DbgSocShowFrame(dbgproc_t *);
dbgloc_t *	DbgSocGetLocation(dbgproc_t *);
dbgvars_t *	DbgSocGetVars(dbgproc_t *, char *);
dbgstat		DbgSocStatus(dbgproc_t *);
int		DbgSocInterrupt(dbgproc_t *);
int		DbgSocQuit(dbgproc_t *);
int		DbgSocCheckEvents(void **);
void		DbgSocDispatchEvents(void *);
void		DbgSocDestroyClient(void *);
int		cbSocInvProc(char *, int, int, int, dbgevent_t *, void *);
int		DbgSocSetDebuggerVar(dbgproc_t *, char *, AIF *);
AIF *           DbgSocSetSourcePath(dbgproc_t *, AIF *);
AIF *           DbgSocGetSourcePath(dbgproc_t *);
AIF *	        DbgSocGetDebuggerVar(dbgproc_t *, char *);

extern int	CallBackSocInit(cb_t *);
extern int	CallBackSocWait(cb_t *);
extern void	CallBackSocDestroy(cb_t *);

dbgclnt_proto DbgClntSocProtoSW =
{
	"socket",
	0,
	NULL,
	NULL,
	NULL, /* DbgSocInit */
	NULL, /* DbgSocFinish */

	NULL, /* DbgSocAttach */
	DbgSocInvoke,
	DbgSocEvalExpr,
	DbgSocSetVar,
	DbgSocGetType,
	DbgSocGo,
	DbgSocSetLineBreak,
	DbgSocSetFuncBreak,
	DbgSocKill,
	DbgSocDeleteBreak,
	DbgSocList,
	DbgSocSetArgs,
	DbgSocStep,
	DbgSocMoveFrame,
	DbgSocShowFrame,
	DbgSocGetLocation,
	DbgSocGetVars,
	DbgSocStatus,
	DbgSocInterrupt,
	DbgSocQuit,
	DbgSocCheckEvents,
	DbgSocDispatchEvents,
	NULL, /* DbgSocRaiseEvent */
	DbgSocDestroyClient,
	CallBackSocInit,
	CallBackSocWait,
	CallBackSocDestroy,
	DbgSocSetDebuggerVar,
	DbgSocSetSourcePath,
	DbgSocGetSourcePath,
	DbgSocGetDebuggerVar
};

/*
** Invoke user program using sockets.
*/
dbginvoke_t *
DbgSocInvoke(
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
	char			sp[80];
	char			sn[80];
	char			starter[MAXPATHLEN];
	char **			ap;
	SOCPRT *		socp;
	dbginvoke_t *		di;

	di = NewDbgInvoke(nproc, proto);

	if ( (di->d_cbp = CallBackCreate(di->d_proto, cbSocInvProc, (void *)di)) == (cb_t *)NULL )
	{
		FreeDbgInvoke(di, DbgSocDestroyClient);
		return (dbginvoke_t *)NULL;
	}

	di->d_cbrtn = cb;

	socp = (SOCPRT *) di->d_cbp->cb_xprt;
	snprintf(sp, sizeof(sp), "%d", socp->soc_port);

	/*
	** There is a race between returning a pid and
	** receiving a SIGCHLD if the new process exits
	** very quickly (such as if the executable can't
	** be found). We need to hold the signal until we've
	** made sure we've got the pid.
	*/
	sighold(SIGCHLD);

	snprintf(starter, MAXPATHLEN-1, "%s/%s", BINDIR, STARTER);
	ap = NewArgv(STARTER, NULL);

	if ( arch )
		ap = AppendArgs(ap, "-a", arch, NULL);

	ap = AppendArgs(ap,
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

	if ( path != NULL )
		ap = AppendArgs(ap, "-b", path, NULL);

	if ( backend != NULL )
		ap = AppendArgs(ap, "-e", backend, NULL);

	ap = AppendArgs(ap, "--", prog, NULL);

	ap = AppendArgv(ap, argv);

	/* if we have arch arguments, append it to "ap": "... -- arch_args ..." */

	if ( arch_args != NULL )
	{
		ap = AppendArgs(ap, "--", NULL);
		ap = AppendArgv(ap, arch_args);
	}
	
	if ( (di->d_pid = ForkAndExec(STARTER, ap, env)) < 0 )
	{
		if ( (di->d_pid = ForkAndExec(starter, ap, env)) < 0 )
		{
			FreeArgv(ap);
			FreeDbgInvoke(di, DbgSocDestroyClient);
			sigrelse(SIGCHLD);
			return (dbginvoke_t *)NULL;
		}
	}

	FreeArgv(ap);
	sigrelse(SIGCHLD);

	return di;
}

int
DbgSocCheckEvents(void **data)
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

	*fds = soc_fdset;

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
DbgSocDispatchEvents(void *data)
{
	fd_set *	fds = (fd_set *)data;
	
	if ( fds == (fd_set *)NULL )
		return;

	soc_getreqset(fds);
}

/*
** Callback routine for socket invoke.
*/
int
cbSocInvProc(char *host, int prog, int pnum, int port, dbgevent_t *e, void *arg)
{
	int		(*cbrtn)();
	dbgproc_t *	dp;
	dbginvoke_t *	di = (dbginvoke_t *)arg;
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
		fprintf(stderr, "Received duplicate task id: %d\n", dp->d_tid);
	
	if ( (dp->d_client = (void *) socclnt_create(dp->d_host, dp->d_prog)) == (void *)NULL )
	{
		dbgevent_t *ee;

		fprintf(stderr, "Unable to connect to host %s port %d\n", dp->d_host, dp->d_prog);

		EVENT_ERROR(ee, DBGERR_INVOKE, "");
		DbgSetErr(DBGERR_INVOKE, NULL);

		di->d_failed = dp->d_tid;

		cbrtn = di->d_cbrtn;

		CallBackDestroy(di->d_proto, di->d_cbp);

		di->d_cbp = (cb_t *)NULL;
		di->d_cbdone = 1;

		return (*cbrtn)(ee, (void *)di);
	}

	socclnt_timeout(&tv);

	HoldSignals();
	AddToList(DbgProcList, (void *)dp);
	ReleaseSignals();

	DPRINT("connected to task %d on %s\n", pnum, dp->d_host);

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
************ WRAPPERS FOR SOCKET ROUTINES ****************
**********************************************************
**********************************************************
*/

/*
** DBG_EVALEXPR: Evaluate expression.
*/
AIF *
DbgSocEvalExpr(dbgproc_t *dp, char *exp, int assid)
{
	int		rlen;
	int 		status;
	char *		request;
	char *		result;
	AIF *		aif_res;
	char *		s;
	char		par[1024];

	if ( !dp->d_started )
	{
		DbgSetErr(DBGERR_SERVER, NULL);
		return (AIF *)NULL;
	}

	rlen = 1024 + strlen(exp);
	request = (char *) Alloc(rlen);

	snprintf(request, rlen-1, "EVALEXPR %s\n", exp);

	HoldSignals();

        if ( socclnt_call((SOCCLIENT *) dp->d_client, request, &result, 
                                &status, NULL) < 0 )
        {
                fprintf(stderr,"DbgSocEvalExpr failed\n");
		Free(request);
		DbgSetErr(DBGERR_RPC, NULL);
                DestroyDbgProc(dp, DbgSocDestroyClient);
                ReleaseSignals();
		return (AIF *)NULL;
        }

	ReleaseSignals();

	Free(request);

        if ( status < 0 ) {
                Free(result);
		return (AIF *)NULL;
        }

	s = result;

	s = getword(s,par);
	status = atoi(par);

	if (status != DBGEV_DATA) {
		if (status == DBGEV_ERROR) {
			s = getword(s,par);
			s = skipspace(s);
			DbgSetErr(atoi(par), s);
		}
		Free(result);
		return (AIF *)NULL;
	}

	aif_res = SocProtoToAIF(s);

	Free(result);

	return aif_res;
}

/*
** DBG_SETDEBUGGERVAR: Set variable in debugger to new value.
*/
int
DbgSocSetDebuggerVar(dbgproc_t *dp, char *var, AIF *val)
{
	int		rlen;
	int		status;
	char *	        st_aif;
	char *	        request;
	char *	        result;
	char *	        s;
	char	        par[1024];

	if ( !dp->d_started )
	{
		DbgSetErr(DBGERR_SERVER, NULL);
		return -1;
	}

	st_aif = AIFToSocProto(val);

	rlen = 1024 + strlen(var) + strlen(st_aif);
	request = (char *) Alloc(rlen);

	snprintf(request, rlen-1, "SETDEBUGGERVAR %s %s\n", var, st_aif);
	Free(st_aif);

	HoldSignals();

	if ( socclnt_call((SOCCLIENT *) dp->d_client, request, &result, 
                                &status, NULL) < 0 )
	{
		fprintf(stderr,"DbgSocSetDebuggerVar failed\n");
		Free(request);
		DbgSetErr(DBGERR_RPC, NULL);
		DestroyDbgProc(dp, DbgSocDestroyClient);
		ReleaseSignals();
		return -1;
	}

	ReleaseSignals();

	Free(request);

	if ( status < 0 ) {
		Free(result);
		return -1;
	}

	s = result;

	s = getword(s,par);
	status = atoi(par);

	if (status != DBGEV_OK) {
		if (status == DBGEV_ERROR) {
			s = getword(s,par);
			s = skipspace(s);
			DbgSetErr(atoi(par), s);
		}
		Free(result);
		return -1;
	}

	Free(result);

	return 0;
}

/*
** DBG_SETSOURCEPATH: Set source search path in debugger to new value.
*/
AIF *
DbgSocSetSourcePath(dbgproc_t *dp, AIF *val)
{
	int		rlen;
	int		status;
	char *	        st_aif;
	char *	        request;
	char *	        result;
	AIF  *          aif_res;
	char *	        s;
	char	        par[1024];

	if ( !dp->d_started )
	{
		DbgSetErr(DBGERR_SERVER, NULL);
		return (AIF *)NULL;
	}

	st_aif = AIFToSocProto(val);

	rlen = 1024 + strlen(st_aif);
	request = (char *) Alloc(rlen);

	snprintf(request, rlen-1, "SETSOURCEPATH %s\n", st_aif);
	Free(st_aif);

	HoldSignals();

	if ( socclnt_call((SOCCLIENT *) dp->d_client, request, &result, 
                                &status, NULL) < 0 )
	{
		fprintf(stderr,"DbgSocSetSourcePath failed\n");
		Free(request);
		DbgSetErr(DBGERR_RPC, NULL);
		DestroyDbgProc(dp, DbgSocDestroyClient);
		ReleaseSignals();
		return (AIF *)NULL;
	}

	ReleaseSignals();

	Free(request);

	if ( status < 0 ) {
		Free(result);
		return (AIF *)NULL;
	}

	s = result;

	s = getword(s,par);
	status = atoi(par);

	if (status != DBGEV_DATA) {
		if (status == DBGEV_ERROR) {
			s = getword(s,par);
			s = skipspace(s);
			DbgSetErr(atoi(par), s);
		}
		Free(result);
		return (AIF *)NULL;
	}

        aif_res = SocProtoToAIF(s);

	Free(result);

	return aif_res;
}

/*
** DBG_GETSOURCEPATH: Get source search path from debugger.
*/
AIF *
DbgSocGetSourcePath(dbgproc_t *dp)
{
	int		rlen;
	int		status;
	char *	        request;
	char *	        result;
	AIF  *          aif_res;
	char *	        s;
	char	        par[1024];

	if ( !dp->d_started )
	{
		DbgSetErr(DBGERR_SERVER, NULL);
		return (AIF *)NULL;
	}

	rlen = 1024;
	request = (char *) Alloc(rlen);

	snprintf(request, rlen-1, "GETSOURCEPATH");

	HoldSignals();

	if ( socclnt_call((SOCCLIENT *) dp->d_client, request, &result, 
                                &status, NULL) < 0 )
	{
		fprintf(stderr,"DbgSocGetSourcePath failed\n");
		Free(request);
		DbgSetErr(DBGERR_RPC, NULL);
		DestroyDbgProc(dp, DbgSocDestroyClient);
		ReleaseSignals();
		return (AIF *)NULL;
	}

	ReleaseSignals();

	Free(request);

	if ( status < 0 ) {
		Free(result);
		return (AIF *)NULL;
	}

	s = result;

	s = getword(s,par);
	status = atoi(par);

	if (status != DBGEV_DATA) {
		if (status == DBGEV_ERROR) {
			s = getword(s,par);
			s = skipspace(s);
			DbgSetErr(atoi(par), s);
		}
		Free(result);
		return (AIF *)NULL;
	}

        aif_res = SocProtoToAIF(s);

	Free(result);

	return aif_res;
}

/*
** DBG_GETDEBUGGERVAR: Get variable in debugger.
*/
AIF *
DbgSocGetDebuggerVar(dbgproc_t *dp, char *var)
{
	int		rlen;
	int		status;
	char *	request;
	char *	result;
	AIF *	aif_res;
	char *	s;
	char	par[1024];

	if ( !dp->d_started )
	{
		DbgSetErr(DBGERR_SERVER, NULL);
		return (AIF *)NULL;
	}

	rlen = 1024 + strlen(var);
	request = (char *) Alloc(rlen);

	snprintf(request, rlen-1, "GETDEBUGGERVAR %s\n", var);

	HoldSignals();

	if ( socclnt_call((SOCCLIENT *) dp->d_client, request, &result, 
                                &status, NULL) < 0 )
	{
		fprintf(stderr,"DbgSocGetDebuggerVar failed\n");
		Free(request);
		DbgSetErr(DBGERR_RPC, NULL);
		DestroyDbgProc(dp, DbgSocDestroyClient);
		ReleaseSignals();
		return (AIF *)NULL;
	}

	ReleaseSignals();

	Free(request);

	if ( status < 0 ) {
		Free(result);
		return (AIF *)NULL;
	}

	s = result;

	s = getword(s,par);
	status = atoi(par);

	if (status != DBGEV_DATA) {
		if (status == DBGEV_ERROR) {
			s = getword(s,par);
			s = skipspace(s);
			DbgSetErr(atoi(par), s);
		}
		Free(result);
		return (AIF *)NULL;
	}

	aif_res = SocProtoToAIF(s);

	Free(result);

	return aif_res;
}

/*
** DBG_SETVAR: Set variable in program to new value.
*/
int
DbgSocSetVar(dbgproc_t *dp, char *var, AIF *val)
{
	int             rlen;
        int             status;
	char *		st_aif;
        char *          request;
        char *          result;
	char *		s;
	char		par[1024];

	if ( !dp->d_started )
	{
		DbgSetErr(DBGERR_SERVER, NULL);
		return -1;
	}

	st_aif = AIFToSocProto(val);

	rlen = 1024 + strlen(var) + strlen(st_aif);
	request = (char *) Alloc(rlen);

        snprintf(request, rlen-1, "SETVAR %s %s\n", var, st_aif);
	Free(st_aif);

	HoldSignals();

        if ( socclnt_call((SOCCLIENT *) dp->d_client, request, &result, 
                                &status, NULL) < 0 )
	{
                fprintf(stderr,"DbgSocSetVar failed\n");
		Free(request);
		DbgSetErr(DBGERR_RPC, NULL);
		DestroyDbgProc(dp, DbgSocDestroyClient);
		ReleaseSignals();
		return -1;
	}

	ReleaseSignals();

	Free(request);

        if ( status < 0 ) {
                Free(result);
                return -1;
        }

        s = result;

        s = getword(s,par);
        status = atoi(par);

        if (status != DBGEV_OK) {
                if (status == DBGEV_ERROR) {
                        s = getword(s,par);
			s = skipspace(s);
			DbgSetErr(atoi(par), s);
                }
                Free(result);
                return -1;
        }

        Free(result);
	return 0;
}

/*
** DBG_GETTYPE: Get type of variable.
*/
char *
DbgSocGetType(dbgproc_t *dp, char *var)
{
	int             rlen;
        int             status;
        char *          request;
        char *          result;
	char *		s;
	char		par[1024];

	if ( !dp->d_started )
	{
		DbgSetErr(DBGERR_SERVER, NULL);
		return NULL;
	}

        rlen = 1024 + strlen(var);
        request = (char *) Alloc(rlen);

        snprintf(request, rlen-1, "GETTYPE %s\n", var);

	HoldSignals();

        if ( socclnt_call((SOCCLIENT *) dp->d_client, request, &result, 
                                &status, NULL) < 0 )
	{
                fprintf(stderr,"DbgSocGetType failed\n");
		Free(request);
		DbgSetErr(DBGERR_RPC, NULL);
		DestroyDbgProc(dp, DbgSocDestroyClient);
		ReleaseSignals();
		return NULL;
	}

	ReleaseSignals();

        Free(request);

        if ( status < 0 ) {
                Free(result);
                return NULL;
        }

        s = result;

        s = getword(s,par);
        status = atoi(par);

        if (status != DBGEV_TYPE) {
                if (status == DBGEV_ERROR) {
                        s = getword(s,par);
			s = skipspace(s);
			DbgSetErr(atoi(par), s);
                }

                Free(result);
                return NULL;
        }

	s = skipspace(s);
	s = strdup(s);

	Free(result);

	return s;
}

/*
** DBG_GO: Continue/start execution.
*/
int
DbgSocGo(dbgproc_t *dp)
{
        int             rlen;
        int             status;
        char *          request;
        char *          result;
	SOCPRT *        socp;
	char *		s;
	char		par[1024];

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

        rlen = 1024 + strlen(dp->d_cbp->cb_host);
        request = (char *) Alloc(rlen);

        socp = (SOCPRT *) dp->d_cbp->cb_xprt;

        snprintf(request, rlen-1, "GO %s %d\n", dp->d_cbp->cb_host, socp->soc_port);

	HoldSignals();

        if ( socclnt_call((SOCCLIENT *) dp->d_client, request, &result, 
                                &status, NULL) < 0 )
	{
                fprintf(stderr,"DbgSocGo failed\n");
		Free(request);
		DbgSetErr(DBGERR_RPC, NULL);
		DestroyDbgProc(dp, DbgSocDestroyClient);
		ReleaseSignals();
		return -1;
	}

	ReleaseSignals();

        Free(request);

        if ( status < 0 ) {
                Free(result);
                return -1;
        }

        s = result;

        s = getword(s,par);
        status = atoi(par);

        if (status != DBGEV_OK) {
                if (status == DBGEV_ERROR) {
                        s = getword(s,par);
			s = skipspace(s);
			DbgSetErr(atoi(par), s);
                }
                Free(result);
                return -1;
        }

        Free(result);
	return 0;
}

/*
** DBG_SETLINEBREAK: Set a breakpoint in program at 
**                   specified line.
*/
dbgbp_t *
DbgSocSetLineBreak(dbgproc_t *dp, char *file, int line)
{
        int             rlen;
        int             status;
        char *          request;
        char *          result;
	dbgbp_t *	bpt;
	char *		s;
	char		par[1024];

	if ( !dp->d_started )
	{
		DbgSetErr(DBGERR_SERVER, NULL);
		return (dbgbp_t *)NULL;
	}

	if ( file == NULL )
		file = "<null>";

        rlen = 1024 + strlen(file);
        request = (char *) Alloc(rlen);

        snprintf(request, rlen-1, "SETLINEBREAK %s %d\n", file, line);

	HoldSignals();

        if ( socclnt_call((SOCCLIENT *) dp->d_client, request, &result, 
                                &status, NULL) < 0 )
	{
                fprintf(stderr,"DbgSocSetLineBreak failed\n");
		Free(request);
		DbgSetErr(DBGERR_RPC, NULL);
		DestroyDbgProc(dp, DbgSocDestroyClient);
		ReleaseSignals();
		return (dbgbp_t *)NULL;
	}

	ReleaseSignals();

        Free(request);

        if ( status < 0 ) {
                Free(result);
		return (dbgbp_t *)NULL;
	}

        s = result;

        s = getword(s,par);
        status = atoi(par);

        if (status != DBGEV_BPSET) {
                if (status == DBGEV_ERROR) {
                        s = getword(s,par);
			s = skipspace(s);
			DbgSetErr(atoi(par), s);
                }
                Free(result);
                return (dbgbp_t *) NULL;
        }

        bpt = SocProtoToBP(s);
        Free(result);

        return bpt;
}

/*
** DBG_SETFUNCBREAK: Set a breakpoint in program at 
**                   specified function.
*/
dbgbp_t *
DbgSocSetFuncBreak(dbgproc_t *dp, char *file, char *func)
{

        int             rlen;
        int             status;
        char *          request;
        char *          result;
        dbgbp_t *       bpt;
	char		par[1024];
	char *		s;

	if ( !dp->d_started )
	{
		DbgSetErr(DBGERR_SERVER, NULL);
		return (dbgbp_t *)NULL;
	}

	if ( file == NULL )
                file = "<null>";

        rlen = 1024 + strlen(file) + strlen(func);
        request = (char *) Alloc(rlen);

        snprintf(request, rlen-1, "SETFUNCBREAK %s %s\n", file, func);

	HoldSignals();

        if ( socclnt_call((SOCCLIENT *) dp->d_client, request, &result, 
                                &status, NULL) < 0 )
	{
                fprintf(stderr,"DbgSocSetFuncBreak failed\n");
		Free(request);
		DbgSetErr(DBGERR_RPC, NULL);
		DestroyDbgProc(dp, DbgSocDestroyClient);
		ReleaseSignals();
		return (dbgbp_t *)NULL;
	}

	ReleaseSignals();

        Free(request);

        if ( status < 0 ) {
                Free(result);
		return (dbgbp_t *)NULL;
	}

        s = result;

        s = getword(s,par);
        status = atoi(par);

        if (status != DBGEV_BPSET) {
                if (status == DBGEV_ERROR) {
                        s = getword(s,par);
			s = skipspace(s);
			DbgSetErr(atoi(par), s);
                }
                Free(result);
                return (dbgbp_t *) NULL;
        }

        bpt = SocProtoToBP(s);
        Free(result);

        return bpt;
}

/*
** DBG_KILL: Terminate execution of program.
*/
int
DbgSocKill(dbgproc_t *dp)
{
        int             status;
        char *          result;
	char *		s;
	char		par[1024];

	if ( !dp->d_started )
	{
		DbgSetErr(DBGERR_SERVER, NULL);
		return -1;
	}

	HoldSignals();

        if ( socclnt_call((SOCCLIENT *) dp->d_client, "KILL\n", &result, 
                                &status, NULL) < 0 )
        {
                fprintf(stderr,"DbgSocKill failed\n");
		DbgSetErr(DBGERR_RPC, NULL);
		DestroyDbgProc(dp, DbgSocDestroyClient);
		ReleaseSignals();
		return -1;
	}

	ReleaseSignals();

        if ( status < 0 ) {
                Free(result);
                return -1;
        }

        s = result;

        s = getword(s,par);
        status = atoi(par);

        if (status != DBGEV_OK) {
                if (status == DBGEV_ERROR) {
                        s = getword(s,par);
			s = skipspace(s);
			DbgSetErr(atoi(par), s);
                }
                Free(result);
                return -1;
        }

        Free(result);
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
DbgSocDeleteBreak(dbgproc_t *dp, int bp)
{
        int             rlen;
        int             status;
        char *          request;
        char *          result;
	char *		s;
	char		par[1024];

	if ( !dp->d_started )
	{
		DbgSetErr(DBGERR_SERVER, NULL);
		return -1;
	}

        rlen = 1024;
        request = (char *) Alloc(rlen);

        snprintf(request, rlen-1, "DELETEBREAK %d\n", bp);

	HoldSignals();

        if ( socclnt_call((SOCCLIENT *) dp->d_client, request, &result, 
                                &status, NULL) < 0 )
	{
                fprintf(stderr,"DbgSocDeleteBreak failed\n");
		Free(request);
		DbgSetErr(DBGERR_RPC, NULL);
		DestroyDbgProc(dp, DbgSocDestroyClient);
		ReleaseSignals();
		return -1;
	}

	ReleaseSignals();

	Free(request);

        if ( status < 0 ) {
                Free(result);
		return -1;
	}

        s = result;

        s = getword(s,par);
        status = atoi(par);

        if (status != DBGEV_OK) {
                if (status == DBGEV_ERROR) {
                        s = getword(s,par);
			s = skipspace(s);
			DbgSetErr(atoi(par), s);
                }
                Free(result);
                return -1;
        }

        Free(result);
	return 0;
}

#ifdef notdef
/*
** DBG_SHOWBREAK: List all breakpoints.
*/
dbgbp_t *
DbgSocShowBreak(dbgproc_t *dp)
{
        int             status;
        char *          result;
        dbgbp_t *       bpt;
	char *		s;

	if ( !dp->d_started )
	{
		DbgSetErr(DBGERR_SERVER, NULL);
		return (dbgbp_t *)NULL;
	}

	HoldSignals();

        if ( socclnt_call((SOCCLIENT *) dp->d_client, "SHOWBREAK\n", &result, 
                                &status, NULL) < 0 )
	{
                fprintf(stderr,"DbgSocShowBreak failed\n");
		DbgSetErr(DBGERR_RPC, NULL);
		DestroyDbgProc(dp, DbgSocDestroyClient);
		ReleaseSignals();
		return (dbgbp_t *)NULL;
	}

	ReleaseSignals();

        if ( status < 0 ) {
                Free(result);
                return (dbgbp_t *)NULL;
        }

        s = result;

        s = getword(s,par);
        status = atoi(par);

        if (status != DBGEV_OK) {
                if (status == DBGEV_ERROR) {
                        s = getword(s,par);
			s = skipspace(s);
			DbgSetErr(atoi(par), s);
		}
                Free(result);
                return -1;
	}

	bpt = SocProtoToBP(result);
	Free(result);

	return bpt;
}
#endif /* notdef */

/*
** DBG_LIST: List specified function or line.
*/

int
DbgSocList(dbgproc_t *dp, char *str, dbglist_t **st_list)
{
        int             rlen;
        int             status;
        char *          request;
        char *          result;
	char            par[1024];
        char *          s;

	if ( !dp->d_started )
	{
		DbgSetErr(DBGERR_SERVER, NULL);
		return -1;
	}

	if ( str == NULL )
		str = "<null>";

        rlen = 1024 + strlen(str);
        request = (char *) Alloc(rlen);

        snprintf(request, rlen-1, "LIST %s\n", str);

	HoldSignals();

        if ( socclnt_call((SOCCLIENT *) dp->d_client, request, &result, 
                                &status, NULL) < 0 )
	{
                fprintf(stderr,"DbgSocList failed\n");
		Free(request);
		DbgSetErr(DBGERR_RPC, NULL);
		DestroyDbgProc(dp, DbgSocDestroyClient);
		ReleaseSignals();
		return -1;
	}

	ReleaseSignals();

        Free(request);

        if ( status < 0 ) {
                Free(result);
		return -1;
	}
        s = result;

        s = getword(s,par);
        status = atoi(par);

        if (status != DBGEV_LIST) {
                if (status == DBGEV_ERROR) {
                        s = getword(s,par);
			s = skipspace(s);
			DbgSetErr(atoi(par), s);
                }
                Free(result);
                return -1;
        }

	*st_list = SocProtoToList(s);

	Free(result);

	return 0;
}

/*
** DBG_SETARGS: Set arguments to pass to program when run.
*/
int
DbgSocSetArgs(dbgproc_t *dp, char *args)
{
        int             rlen;
        int             status;
        char *          request;
        char *          result;
	char *		s;
	char		par[1024];

	if ( !dp->d_started )
	{
		DbgSetErr(DBGERR_SERVER, NULL);
		return -1;
	}

	if ( args == NULL )
		args = "<null>";

        rlen = 1024 + strlen(args);
        request = (char *) Alloc(rlen);

        snprintf(request, rlen-1, "SETARGS %s\n", args);

	HoldSignals();

        if ( socclnt_call((SOCCLIENT *) dp->d_client, request, &result, 
                                &status, NULL) < 0 )
	{
                fprintf(stderr,"DbgSocSetArgs failed\n");
		Free(request);
		DbgSetErr(DBGERR_RPC, NULL);
		DestroyDbgProc(dp, DbgSocDestroyClient);
		ReleaseSignals();
		return -1;
	}

	ReleaseSignals();

        Free(request);

        if ( status < 0 ) {
                Free(result);
		return -1;
	}

        s = result;

        s = getword(s,par);
        status = atoi(par);

        if (status != DBGEV_OK) {
                if (status == DBGEV_ERROR) {
                        s = getword(s,par);
			s = skipspace(s);
			DbgSetErr(atoi(par), s);
                }
                Free(result);
                return -1;
        }

        Free(result);
	return 0;
}

/*
** DBG_STEP: Execute to next statement. Repeat count times.
**           If in == 0, pass over function calls (ala next).
*/
int
DbgSocStep(dbgproc_t *dp, int count, int in)
{
        int             rlen;
        int             status;
        char *          request;
        char *          result;
        SOCPRT *        socp;
	char *		s;
	char		par[1024];

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

        rlen = 1024 + strlen(dp->d_cbp->cb_host);
        request = (char *) Alloc(rlen);

        socp = (SOCPRT *) dp->d_cbp->cb_xprt;

        snprintf(request, rlen-1, "STEP %d %d %s %d\n",
		count, in, dp->d_cbp->cb_host, socp->soc_port);

        if ( socclnt_call((SOCCLIENT *) dp->d_client, request, &result, 
                                &status, NULL) < 0 )
	{
                fprintf(stderr,"DbgSocStep failed\n");
		Free(request);
		DbgSetErr(DBGERR_RPC, NULL);
		DestroyDbgProc(dp, DbgSocDestroyClient);
		ReleaseSignals();
		return -1;
	}

        Free(request);
	ReleaseSignals();

        if ( status < 0 ) {
                Free(result);
		return -1;
	}

        s = result;

        s = getword(s,par);
        status = atoi(par);

        if (status != DBGEV_OK) {
                if (status == DBGEV_ERROR) {
                        s = getword(s,par);
			s = skipspace(s);
			DbgSetErr(atoi(par), s);
                }
                Free(result);
                return -1;
        }

        Free(result);
	return 0;
}

/*
** DBG_MOVEFRAME: Move up or down stack frames.
*/
dbgframe_t *
DbgSocMoveFrame(dbgproc_t *dp, int count, int up)
{
        int             rlen;
        int             status;
        char *          request;
        char *          result;
	dbgframe_t *	st_frame;
	char *          s;
	char            par[1024];

	if ( !dp->d_started )
	{
		DbgSetErr(DBGERR_SERVER, NULL);
		return (dbgframe_t *)NULL;
	}

        rlen = 1024;
        request = (char *) Alloc(rlen);

        snprintf(request, rlen-1, "MOVEFRAME %d %d\n", count, up);

	HoldSignals();

        if ( socclnt_call((SOCCLIENT *) dp->d_client, request, &result, 
                                &status, NULL) < 0 )
	{
                fprintf(stderr,"DbgSocMoveFrame failed\n");
		Free(request);
		DbgSetErr(DBGERR_RPC, NULL);
		DestroyDbgProc(dp, DbgSocDestroyClient);
		ReleaseSignals();
		return (dbgframe_t *)NULL;
	}

	ReleaseSignals();

        Free(request);

        if ( status < 0 ) {
                Free(result);
		return (dbgframe_t *)NULL;
	}

        s = result;

        s = getword(s,par);
        status = atoi(par);

        if (status != DBGEV_FRAME) {
                if (status == DBGEV_ERROR) {
                        s = getword(s,par);
			s = skipspace(s);
			DbgSetErr(atoi(par), s);
                }
                Free(result);
                return (dbgframe_t *)NULL;
        }

	st_frame = SocProtoToFrame(s);
        Free(result);

	return st_frame;
}

/*
** DBG_SHOWFRAME: Show current stack.
*/
dbgframe_t *
DbgSocShowFrame(dbgproc_t *dp)
{
	dbgframe_t *		dpf;
	int			status;
	char *			result;
        char *          s;
        char            par[1024];

	if ( !dp->d_started )
	{
		DbgSetErr(DBGERR_SERVER, NULL);
		return (dbgframe_t *)NULL;
	}

	HoldSignals();

        if ( socclnt_call((SOCCLIENT *) dp->d_client, "SHOWFRAME\n", &result, 
				&status, NULL) < 0 )
        {
		fprintf(stderr,"DbgSocShowFrame failed\n");
		DbgSetErr(DBGERR_RPC, NULL);
		DestroyDbgProc(dp, DbgSocDestroyClient);
		ReleaseSignals();
		return (dbgframe_t *)NULL;
        }

	ReleaseSignals();

	if ( status < 0 ) {
                Free(result);
		return (dbgframe_t *)NULL;
	}

        s = result;

        s = getword(s,par);
        status = atoi(par);

        if (status != DBGEV_FRAME) {
                if (status == DBGEV_ERROR) {
                        s = getword(s,par);
			s = skipspace(s);
			DbgSetErr(atoi(par), s);
                }
                Free(result);
                return (dbgframe_t *)NULL;
        }

	dpf = SocProtoToFrame(s);

	Free(result);

	return dpf;
}

/*
** DBG_GETLOCATION: Return current location in program.
*/
dbgloc_t *
DbgSocGetLocation(dbgproc_t *dp)
{
        int                     status;
        char *                  result;
	dbgloc_t *		st_loc;
	char *			s;
	char			par[1024];

	if ( !dp->d_started )
	{
		DbgSetErr(DBGERR_SERVER, NULL);
		return (dbgloc_t *)NULL;
	}

	HoldSignals();

        if ( socclnt_call((SOCCLIENT *) dp->d_client, "GETLOCATION\n", &result, 
                                &status, NULL) < 0 )
	{
                fprintf(stderr,"DbgSocGetLocation failed\n");
		DbgSetErr(DBGERR_RPC, NULL);
		DestroyDbgProc(dp, DbgSocDestroyClient);
		ReleaseSignals();
		return (dbgloc_t *)NULL;
	}

	ReleaseSignals();

        if ( status < 0 ) {
                Free(result);
		return (dbgloc_t *)NULL;
	}

        s = result;

        s = getword(s,par);
        status = atoi(par);

        if (status != DBGEV_LOC) {
                if (status == DBGEV_ERROR) {
                        s = getword(s,par);
			s = skipspace(s);
			DbgSetErr(atoi(par), s);
                }
                Free(result);
                return (dbgloc_t *) NULL;
        }

	st_loc = SocProtoToLoc(s);
	Free(result);

	return st_loc;
}

/*
** DBG_GETVARS: Return current variables in program.
*/
dbgvars_t *
DbgSocGetVars(dbgproc_t *dp, char *file)
{
        int                     status=1;
	int			rlen;
	char *			request;
        char *                  result;
	dbgvars_t *		st_vars;
	char *			s;
	char			par[1024];

	if ( !dp->d_started )
	{
		DbgSetErr(DBGERR_SERVER, NULL);
		return (dbgvars_t *)NULL;
	}

	if ( file == NULL )
		file = "<null>";

        rlen = 1024 + strlen(file);
        request = (char *) Alloc(rlen);

	snprintf(request, rlen-1, "GETVARS %s\n", file);

	HoldSignals();

        if ( socclnt_call((SOCCLIENT *) dp->d_client, request, &result, 
                                &status, NULL) < 0 )
	{
                fprintf(stderr,"DbgSocGetVars failed %d\n", status);
		DbgSetErr(DBGERR_RPC, NULL);
		Free(request);
		DestroyDbgProc(dp, DbgSocDestroyClient);
		ReleaseSignals();
		return (dbgvars_t *)NULL;
	}

	ReleaseSignals();

	Free(request);

        if ( status < 0 ) {
                Free(result);
		return (dbgvars_t *)NULL;
	}

        s = result;

        s = getword(s,par);
        status = atoi(par);

        if (status != DBGEV_VARS) {
                if (status == DBGEV_ERROR) {
                        s = getword(s,par);
			s = skipspace(s);
			DbgSetErr(atoi(par), s);
                }
                Free(result);
                return (dbgvars_t *) NULL;
        }

	st_vars = SocProtoToVars(s);
	Free(result);

	return st_vars;
}

/*
** DBG_DBGSTATUS: Return status of debugger.
*/
dbgstat
DbgSocStatus(dbgproc_t *dp)
{
        int             status;
        char *          result;
	int		res_event;
	int		res_status;

	if ( !dp->d_started )
	{
		DbgSetErr(DBGERR_SERVER, NULL);
		return DBGSTAT_INTERR;
	}

	HoldSignals();

        if ( socclnt_call((SOCCLIENT *) dp->d_client, "STATUS\n", &result,
                                &status, NULL) < 0 )        {
                fprintf(stderr,"DbgSocStatus failed\n");
		DbgSetErr(DBGERR_RPC, NULL);
		DestroyDbgProc(dp, DbgSocDestroyClient);
		ReleaseSignals();
		return DBGSTAT_INTERR;
	}

        if ( status < 0 ) {
                Free(result);
		ReleaseSignals();
                return DBGSTAT_INTERR;
        }

	sscanf(result,"%d %d",&res_event,&res_status);
        Free(result);
	
	if ( res_status == DBGEV_ERROR ) {
		DbgSetErr(res_status, NULL);
		ReleaseSignals();
		return DBGSTAT_INTERR;
	}

	ReleaseSignals();

	return res_status;
}

/*
** DBG_INTERRUPT: Interrupt current debugger operation.
*/
int
DbgSocInterrupt(dbgproc_t *dp)
{
        int             rlen;
        int             status;
        char *          request;
        char *          result;
	SOCPRT *	socp;
	char *		s;
	char		par[1024];

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

        rlen = 1024 + strlen(dp->d_cbp->cb_host);
        request = (char *) Alloc(rlen);

	socp = (SOCPRT *) dp->d_cbp->cb_xprt;

        snprintf(request, rlen-1, "INTERRUPT %s %d\n",
		dp->d_cbp->cb_host, socp->soc_port);

	HoldSignals();

        if ( socclnt_call((SOCCLIENT *) dp->d_client, request, &result, 
                                &status, NULL) < 0 )
	{
                fprintf(stderr,"DbgSocInterrupt failed\n");
		Free(request);
		DbgSetErr(DBGERR_RPC, NULL);
		DestroyDbgProc(dp, DbgSocDestroyClient);
		ReleaseSignals();
		return -1;
	}

	ReleaseSignals();

	Free(request);

        if ( status < 0 ) {
                Free(result);
		return -1;
	}

        s = result;

        s = getword(s,par);
        status = atoi(par);

        if (status != DBGEV_OK) {
                if (status == DBGEV_ERROR) {
                        s = getword(s,par);
			s = skipspace(s);
			DbgSetErr(atoi(par), s);
                }
                Free(result);
                return -1;
        }

        Free(result);   // Memory leak
	return 0;
}

/*
** Called by DbgQuit() and DbgQuitAll()
*/
int
DbgSocQuit(dbgproc_t *dp)
{
	int	res = 0;
	int	status;
	char *	result;
	char *	s;
	char	par[1024];

	if ( dp->d_started )
	{
		printf("Shutting down server on %s.\n", dp->d_host);

		HoldSignals();

		if ( socclnt_call((SOCCLIENT *) dp->d_client, "QUIT\n", &result, &status, NULL) < 0 )
		{
			fprintf(stderr,"DbgSocQuit failed\n");
			DbgSetErr(DBGERR_RPC, NULL);
			DestroyDbgProc(dp, DbgSocDestroyClient);
			ReleaseSignals();
			return -1;
		}

		s = result;

		s = getword(s,par);
		status = atoi(par);

		if
		(	
			status != DBGEV_OK
			&&
			status == DBGEV_ERROR
		)
		{
			s = getword(s,par);
			s = skipspace(s);

			DbgSetErr(atoi(par), s);
			res = -1;
		}

		Free(result);

		ReleaseSignals();
	}

	DestroyDbgProc(dp, DbgSocDestroyClient);

	return res;
}

void
DbgSocDestroyClient(void *clnt)
{
	socclnt_destroy((SOCCLIENT *)clnt);
}
