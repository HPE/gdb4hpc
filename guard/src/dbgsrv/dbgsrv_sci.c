/*
** Server socket dispatch routines
**
** Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
**
*/

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

#include	"dbg_sci.h"
#include	"dbg_sci_be.h"
//#include	"bitset.h"

static int	tag;
static int	my_sci_rank; //task_id for dbgsrv
static int	sci_fd;

/*
Temporarily we use mpd to launch SCI backends.
Not sure why "PMI_RANK" does not work.
We use "MPD_JRANK" instead.
*/

static char * MPIRankVars[] = {
	"OMPI_MCA_orte_ess_vpid",       /* Open MPI 1.3 */
	"OMPI_MCA_ns_nds_vpid",         /* Open MPI 1.2 */
	"PMI_RANK",                     /* MPICH2. PMI_RANK does not work for MPICH2. */
	"MPD_JRANK",                     /* MPICH2. MPD_JRANK works on MPICH2. */
	"MP_CHILD",                     /* IBM PE */
	"SLURM_PROCID",                 /* SLURM */
	NULL
};


static SOCPRT *	SocPtr;
static SOCPRT *	DataSocPtr = NULL;
extern int      Shutdown;
extern void     dbgsrv_sci();
extern char * 	dbg_invoke_sci();
extern char *	dbg_setlinebreak_sci(char *);
extern char *	dbg_setfuncbreak_sci(char *);
extern char *	dbg_deletebreak_sci(char *);
extern char *	dbg_lastbreak_sci();
extern char *	dbg_list_sci(char *);
extern char *	dbg_go_sci(char *);
extern char *	dbg_setargs_sci(char *);
extern char *	dbg_step_sci(char *);
extern char *	dbg_kill_sci();
extern char *	dbg_moveframe_sci(char *);
extern char *	dbg_showframe_sci();
extern char *   dbg_setdebuggervar_sci(char *);
extern char *   dbg_setsourcepath_sci(char *);
extern char *	dbg_getsourcepath_sci();
extern char *   dbg_getdebuggervar_sci(char *);
extern char *   dbg_setvar_sci(char *);
extern char *   dbg_evalexpr_sci(char *);
extern char *   dbg_gettype_sci(char *);
extern char *   dbg_getlocation_sci();
extern char *   dbg_getvars_sci(char *);
extern char *	dbg_status_sci();
extern char *	dbg_getevent_sci();
extern char *	dbg_quit_sci();
extern char *	dbg_interrupt_sci(char *);

int		DbgSRVSCIInitProto(int, int, char **, char*, char*, int);
void		DbgSRVSCIEndProto(void);
fd_set *	DbgSRVSCIGetFDSet(void);
void		DbgSRVSCIRunProto(fd_set *);
extern int	InvokeSCICallBack(char *, int, char *, int, int, int, dbgevent_t *);
extern int	EventSCICallBack(char *, int, dbgevent_t *);

dbgsrv_proto	DbgSrvSCIProtoSW =
{
	DbgSRVSCIInitProto,
	DbgSRVSCIEndProto,
	DbgSRVSCIGetFDSet,
	DbgSRVSCIRunProto,
	InvokeSCICallBack,
	EventSCICallBack,
};

int
DbgSRVSCIInitProto(int procnum, int argc, char **argv, char* key, char* host, int cb)
{
	int res;
	char **var;
	char *envval;
	char single_rank[16];
    sci_info_t info;
	
	char* job_key;
	char* host_name;
	char host_port[32];
	char* client_id;
	
	DPRINT("enterring DbgSRVSCIInitProto\n");

#ifndef ALPS 
    for (var = MPIRankVars; *var != NULL; var++) {
        envval = getenv(*var);
        if (envval != NULL) {
            break;
        }
    }

	if(envval == NULL) /* we assume only single process is debugged */
	{
		/*
		fprintf(stderr, "ERROR: DbgSRVSCIInitProto cannot get MPIRank\n");
		return -1;
		*/
		sprintf(single_rank, "0");
		envval = single_rank;
		DPRINT("DbgSRVSCIInitProto cannot get MPIRank. Debugging single process is assumed!\n");
	}
#else
	int	pmi_rank = procnum;
	my_sci_rank = pmi_rank; //my_mrn_rank is not rank in mrnet, but is used by client to identifiy dbgsrv 
	sprintf(single_rank, "%d", my_sci_rank);
	envval = single_rank;
	DPRINT("my_sci_rank: %d\n", my_sci_rank);
#endif
	job_key = key;
	host_name = host;
	sprintf(host_port, "%d", cb);
	client_id = envval;

#ifdef DEBUG
	fprintf(stderr, "SCI Backend argvs: \n");
	fprintf(stderr, "\t job_key=%s\n", job_key);
	fprintf(stderr, "\t host_name=%s\n", host_name);
	fprintf(stderr, "\t host_port=%s\n", host_port);
	fprintf(stderr, "\t client_id=%s\n", client_id);
	fprintf(stderr, "\t LD_LIBRARY_PATH=%s\n", getenv("LD_LIBRARY_PATH"));
#endif

	sci_be_initialize(job_key, host_name, host_port, client_id);
	DPRINT("sci_be_initialize is done\n");

	res = sci_be_create();
	DPRINT("sci_be_create is done! res[%d]\n", res);

	res = sci_get_polling_fd(&sci_fd);
	DPRINT("sci_get_polling_fd is done! res[%d], sci_fd[%d]\n", res, sci_fd);

	sci_be_set_callback(dbgsrv_sci);

	DPRINT("leaving DbgSRVSCIInitProto\n");

	return res;
}

void
DbgSRVSCIEndProto(void)
{ 
	sleep(10);
	int rc = SCI_Terminate();
}

fd_set *
DbgSRVSCIGetFDSet(void)
{
	static fd_set	fds;

	FD_ZERO(&fds);
    FD_SET(sci_fd, &fds);

	return &fds;
}

/*
* This is the real part checking and delivering any packets from SCI frontend
*/
void
DbgSRVSCIRunProto(fd_set *fds)
{
	int res = sci_be_recv();
	if( res <0 ) {
		fprintf(stderr, "DbgSRVSCIRunProto() sci_be_recv() failed!\n");
	//	exit(1);
		return;
   	}
	DPRINT("DbgSRVSCIRunProto() sci_be_recv() has got msg!\n");

	res = sci_be_handle_msg();
	if(res != 0)
	{
		fprintf(stderr, "DbgSRVSCIRunProto() sci_be_handle_msg() failed!\n");
		exit(-1);
	}
}

void
dbgsrv_sci(char *msg, sci_group_t group, int seq)
{
	int n;
    struct sockaddr sockaddr;
    int fromlen;
    int ns;
    SOCPRT *np;
	char *result;
	char *response;
    char command[1024];
	char * tail;

	struct timeval  start, end;
	static long ela_secs, ela_usecs;
	static double ela_time;

	result = msg;
	tail = getword(result,command);

	response = NULL;

	DPRINT("[INFO] dbgsrv_sci() is processing cmd[%s]!\n", msg);

	if (strcmp(command,"INVOKE") == 0)
	{
		response = dbg_invoke_sci(tail);
	}	
	else if (strcmp(command,"SETLINEBREAK") == 0)
	{
		response = dbg_setlinebreak_sci(tail);
	}
	else if (strcmp(command,"SETFUNCBREAK") == 0) 
	{
		response = dbg_setfuncbreak_sci(tail);
	}
	else if (strcmp(command,"DELETEBREAK") == 0) 
	{
		response = dbg_deletebreak_sci(tail);
	}
	else if (strcmp(command,"LASTBREAK") == 0) 
	{
		response = dbg_lastbreak_sci();
	}
	else if (strcmp(command,"LIST") == 0) 
	{
		response = dbg_list_sci(tail);
	}
	else if (strcmp(command,"GO") == 0) 
	{
		response = dbg_go_sci(tail);
	}
	else if (strcmp(command,"SETARGS") == 0) 
	{
		response = dbg_setargs_sci(tail);
	}
	else if (strcmp(command,"STEP") == 0) 
	{
		response = dbg_step_sci(tail);
	}
	else if (strcmp(command,"KILL") == 0) 
	{
		response = dbg_kill_sci();
	}
	else if (strcmp(command,"MOVEFRAME") == 0) 
	{
		response = dbg_moveframe_sci(tail);
	}
	else if (strcmp(command,"SHOWFRAME") == 0) 
	{
		response = dbg_showframe_sci();
	}
	else if (strcmp(command,"SETDEBUGGERVAR") == 0) 
	{
		response = dbg_setdebuggervar_sci(tail);
	}
	else if (strcmp(command,"SETSOURCEPATH") == 0)
	{
	        response = dbg_setsourcepath_sci(tail);
	}
	else if (strcmp(command,"GETSOURCEPATH") == 0)
	{
		response = dbg_getsourcepath_sci();
	}
	else if (strcmp(command,"GETDEBUGGERVAR") == 0) 
	{
		response = dbg_getdebuggervar_sci(tail);
	}
	else if (strcmp(command,"SETVAR") == 0) 
	{
		response = dbg_setvar_sci(tail);
	}
	else if (strcmp(command,"EVALEXPR") == 0) 
	{
		response = dbg_evalexpr_sci(tail);
	}
	else if (strcmp(command,"GETTYPE") == 0) 
	{
		response = dbg_gettype_sci(tail);
	}
	else if (strcmp(command,"GETLOCATION") == 0) 
	{
		response = dbg_getlocation_sci();
	}
	else if (strcmp(command,"GETVARS") == 0) 
	{
		response = dbg_getvars_sci(tail);
	}
	else if (strcmp(command,"STATUS") == 0) 
	{
		response = dbg_status_sci();
	}
#ifdef notdef
	else if (strcmp(command,"GETEVENT") == 0) 
	{
		response = dbg_getevent_sci();
	}
#endif
	else if (strcmp(command,"QUIT") == 0) 
	{
		response = dbg_quit_sci();
	}
	else if (strcmp(command,"INTERRUPT") == 0) 
	{
		response = dbg_interrupt_sci(tail);
	}
	else
	{
		fprintf(stderr,"dbgsrv: unknown command .%s.\n",command);
	}

	Free(result);

	if ( response != NULL ) 
	{
		DPRINT("dbgsrv_sci[%d] is sending reply: cmd[%s], reply[%s].\n", my_sci_rank, command, response);
		sci_be_send_reply(group, 0, response);
		Free(response);
	}
}

char *
dbg_invoke_sci(char *s)
{
	DPRINT("[dbgsrv] dbg_invoke_sci receives INVOKE!\n");

	char par[1024];
	s = getword(s,par);
	int size = atoi(par);
	
	sci_be_set_size(size);
	dbgevent_t* e = NewEvent(DBGEV_OK);
	char* response = EventToSocProto(e);

	DPRINT("[dbgsrv] dbg_invoke_sci created response[%p]:[%s]!\n", response, response);
	sci_be_send_event(response);

	return NULL;
}

char *
dbg_setlinebreak_sci(char *s)
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
dbg_setfuncbreak_sci(char *s)
{
	int		len;
	dbgevent_t *    e;
	char		par[1024];
	char *	  file;
	char *	  func;
	char *	  st_event;
	char *	  response;

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
dbg_deletebreak_sci(char *s)
{
	int		len;
	dbgevent_t *    e;
	char		par[1024];
	int			bp;
	char *	  st_event;
	char *	  response;

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
dbg_lastbreak_sci()
{
	int		len;
	dbgevent_t *e;
	char *		st_event;
	char *		response;

	e = DbgLastBreak();

	st_event = EventToSocProto(e);
	len = strlen(st_event) + 16;
	response = (char *) Alloc(len);
	snprintf(response, len-1, "%s\n",st_event);
	Free(st_event);

	return response;
}

char *
dbg_list_sci(char *s)
{
	int		len;
	dbgevent_t *    e;
	char		par[1024];
	char *		list;
	char *	  st_event;
	char *	  response;

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
dbg_go_sci(char *s)
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
dbg_setargs_sci(char *s)
{
	int		len;
	dbgevent_t *    e;
	char		par[1024];
	char *	  args;
	char *	  st_event;
	char *	  response;

	s = getword(s,par);
	args = StrSocEventDup(par);

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
dbg_step_sci(char *s)
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
dbg_kill_sci()
{
	int		len;
	dbgevent_t *    e;
	char *	  st_event;
	char *	  response;

	e = DbgKill();

	st_event = EventToSocProto(e);
	len = strlen(st_event) + 16;
	response = (char *) Alloc(len);
	snprintf(response, len-1, "%s\n",st_event);
	Free(st_event);

	return response;
}

char *
dbg_moveframe_sci(char *s)
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
dbg_showframe_sci()
{
	int		len;
	dbgevent_t *	e;
	char *		st_frame;
	char *		response;

	e = DbgShowFrame();

	st_frame = EventToSocProto(e);
	len = strlen(st_frame) + 16;
	response = (char *) Alloc(len);
	snprintf(response, len-1, "%s\n",st_frame);
	Free(st_frame);

	return response;
}

char *
dbg_setdebuggervar_sci(char *s)
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
dbg_setsourcepath_sci(char *s)
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
dbg_getsourcepath_sci()
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
dbg_getdebuggervar_sci(char *s)
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
dbg_setvar_sci(char *s)
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
dbg_evalexpr_sci(char *s)
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
dbg_gettype_sci(char *s)
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
dbg_getlocation_sci()
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
dbg_getvars_sci(char *s)
{
	int		len;
	dbgevent_t *    e;
	char		par[1024];
	char *	  file;
	char *	  st_event;
	char *	  response;

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
dbg_status_sci()
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
dbg_getevent_sci()
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
dbg_quit_sci()
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
dbg_interrupt_sci(char *s)
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

