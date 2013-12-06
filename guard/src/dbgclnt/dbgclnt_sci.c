/*
** Debugger SCI(Scalable Communication Insfrastructure) interface routines
**
** Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
**
*/

#ifdef HAVE_CONFIG_H
#include	<config.h>
#endif /* HAVE_CONFIG_H */

#ifndef WIN32
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
#else /* !WIN32 */
#include	<windows.h>
#include	<process.h>
#endif /* !WIN32 */

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

#include	"bitset.h"
#include  	"dbg_set_sync.h"
#include 	"dbg_sci.h"
#include 	"dbg_sci_fe.h"
#include 	"sci_topgen.h"

#ifdef ALPS
#include  	"tool_frontend.h"
#endif /* ALPS */


static int sci_fd;

int		DbgSCIInit();
dbginvoke_t *	DbgSCIInvoke(dbgclnt_proto *,  int (*)(struct dbgevent_t *, void *), char *, char **, int, char *, char *, char *, char *, char **, char *, char *, char *, char **);
bitset *	DbgSCISendExpr(rec_set_t *, char *, int);
AIF *		DbgSCIGetExpr(rec_set_t *, bitset *, int);
AIF *		DbgSCIEvalExpr(rec_set_t *, char *, int);
int			DbgSCISetVar(rec_set_t *, char *, AIF *);
rep_set_t *	DbgSCIGetType(rec_set_t *, char *);
int			DbgSCIGo(rec_set_t *);
rep_set_t *	DbgSCISetLineBreak(rec_set_t *, char *, int);
rep_set_t *	DbgSCISetFuncBreak(rec_set_t *, char *, char *);
int			DbgSCIKill(rec_set_t *);
int			DbgSCIDeleteBreak(rec_set_t *, int);
int			DbgSCIList(rec_set_t *, char *, rep_set_t **);
int			DbgSCISetArgs(rec_set_t *, char *);
int			DbgSCIStep(rec_set_t *, int, int);
rep_set_t *	DbgSCIMoveFrame(rec_set_t *, int, int);
rep_set_t *	DbgSCIShowFrame(rec_set_t *);
rep_set_t *	DbgSCIGetLocation(rec_set_t *);
rep_set_t *	DbgSCIGetVars(rec_set_t *, char *);
dbgstat		DbgSCIStatus(rec_set_t *);
int			DbgSCISetDebuggerVar(rec_set_t *, char *, AIF *);
rep_set_t *	DbgSCISetSourcePath(rec_set_t *, AIF *);
rep_set_t *	DbgSCIGetSourcePath(rec_set_t *);
rep_set_t *	DbgSCIGetDebuggerVar(rec_set_t *, char *);
int			DbgSCIInterrupt(rec_set_t *);
int			DbgSCIQuit(dbginvoke_t *);
int			DbgSCICheckEvents(void **);
void		DbgSCIDispatchEvents(void *);
void		DbgSCIDestroyClient(void *);
int			cbSCIInvProc(rep_set_t* set, void *arg);

extern int	CallBackSCIInit(cb_t *);
extern int	CallBackSCIWait(cb_t *);
extern void	CallBackSCIDestroy(cb_t *);

dbgclnt_proto DbgClntSCIProtoSW =
{
	"sci",
	0,
	NULL,
	NULL,
	DbgSCIInit, /* DbgSCIInit */
	NULL, /* DbgSCIFinish */
	NULL, /* DbgSCIStartup */
	NULL, /* DbgSCIAttach */
	DbgSCIInvoke,
	DbgSCISendExpr,
	DbgSCIGetExpr,
	DbgSCIEvalExpr,
	DbgSCISetVar,
	DbgSCIGetType,
	DbgSCIGo,
	DbgSCISetLineBreak,
	DbgSCISetFuncBreak,
	DbgSCIKill,
	DbgSCIDeleteBreak,
	DbgSCIList,
	DbgSCISetArgs,
	DbgSCIStep,
	DbgSCIMoveFrame,
	DbgSCIShowFrame,
	DbgSCIGetLocation,
	DbgSCIGetVars,
	DbgSCIStatus,
	DbgSCIInterrupt,
	DbgSCIQuit,
	DbgSCICheckEvents,
	DbgSCIDispatchEvents,
	NULL, /* DbgSCIRaiseEvent */
	DbgSCIDestroyClient,
	CallBackSCIInit,
	CallBackSCIWait,
	CallBackSCIDestroy,
	DbgSCISetDebuggerVar,
	DbgSCISetSourcePath,
	DbgSCIGetSourcePath,
	DbgSCIGetDebuggerVar
};

#ifdef ALPS

jobHostPlacement_t*
convertALPSPlacement(appHostPlacementList_t* alpsPlacement)
{
	int index;
	int numHosts;
	nodeHostPlacement_t* placement = NULL;
	jobHostPlacement_t* jobPlacement = NULL;
	nodeHostPlacement_t* nodePlac = NULL;

	if(alpsPlacement == NULL)
	{
		fprintf(stderr, "Invalid appHostPlacementList_t[%p]", alpsPlacement);
		return NULL;
	}

	numHosts = alpsPlacement->numHosts;
	placement = alpsPlacement->hosts;

	if(numHosts <= 0 || placement == NULL)
	{
		fprintf(stderr, "Invalid appHostPlacementList_t: numHosts[%d], hosts[%p]", numHosts, placement);
		return NULL;
	}

	jobPlacement = (jobHostPlacement_t*)Alloc(sizeof(jobHostPlacement_t));
	jobPlacement->numHosts = numHosts;
	jobPlacement->hostPlacement = (hostPlacement_t*)Alloc(sizeof(hostPlacement_t) * numHosts);

	for(index = 0; index < numHosts; index++)
	{
		nodePlac = placement + index;

		if(nodePlac->hostname == NULL || nodePlac->numPes <= 0)
		{
			fprintf(stderr, "Invalid appHostPlacementList_t: host[%s], numPes[%d]", nodePlac->hostname, nodePlac->numPes);
			return NULL;
		}

		jobPlacement->hostPlacement[index].hostname = strdup(nodePlac->hostname);
		jobPlacement->hostPlacement[index].numProcs = nodePlac->numPes;
	}	

	return jobPlacement;
}

#endif

int
setupSCICfgDir(void)
{
	struct stat		st;
	char *			sci_cfg_dir;
	
	// grab the environment variable
	if ((sci_cfg_dir = getenv(GUARD_SCI_CFG_DIRECTORY)) == (char *)NULL)
	{
		fprintf(stderr, "Could not find %s env var! Please check your environment settings.\n", GUARD_SCI_CFG_DIRECTORY);
		return -1;
	}
	
	// try to stat the directory
	if (stat(sci_cfg_dir, &st) == 0)
	{
		// the directory already exists, return
		return 0;
	} 
	else
	{
		// the directory doesn't exist so we need to create it
		// use perms 755
		if (mkdir(sci_cfg_dir, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH) != 0)
		{
			fprintf(stderr, "Could not create the SCI config directory! Ensure you have write permissions in the %s path.\n", sci_cfg_dir);
			return -1;
		}
	}
	
	return 0;
}
int
DbgSCIInit()
{
	if( setupSCICfgDir() == -1)
		return 0;

	return 1;
} 

/*
** Invoke user program using sockets.
*/
dbginvoke_t *
DbgSCIInvoke(
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
	char *arch_input,
	char *backend,
	char *path,
	char **env
)
{
	char			sp[80];
	char			sn[80];
	char			starter[MAXPATHLEN];
	char			root[MAXPATHLEN];
	int				num_be;
#ifdef ALPS
	int						apid;
	int                     nid, dbg=0;
	appHostPlacementList_t *	placement;
	jobHostPlacement_t*		jobPlacement;
	int                     redirectInput = 0;
#endif /* ALPS */
	int                     fd;
//	char			filter_name[MAXPATHLEN];
	char *			filter_name;
	char **			ap;
	char **                 tmp;
	dbginvoke_t *		di;

	/*SCI specific*/
	int 			res;
	char			*cfg_dir;
	char			*scid_log;
	char			*be_path;
	char			real_be_path[MAXPATHLEN];
	char			*filter_path;
	char			real_filter_path[MAXPATHLEN];
	char			*lib_path;
	char			*agent_path;
	char			jobkey[64];
	long			jobkey_num;
	char			hfile[MAXPATHLEN];
	char			env_buf[1024];
	struct timeval current_tv;

	di = NewDbgInvoke(nproc, proto);

	if ( (di->d_cbp = CallBackCreate(di->d_proto, cbSCIInvProc, (void *)di)) == (cb_t *)NULL )
	{
		FreeDbgInvoke(di, DbgSCIDestroyClient);
		return (dbginvoke_t *)NULL;
	}

	di->d_cbrtn = cb;

	/*
	** There is a race between returning a pid and
	** receiving a SIGCHLD if the new process exits
	** very quickly (such as if the executable can't
	** be found). We need to hold the signal until we've
	** made sure we've got the pid.
	*/
	sighold(SIGCHLD);


	cfg_dir = getenv(GUARD_SCI_CFG_DIRECTORY);
	if(cfg_dir == NULL)
	{
		fprintf(stderr, "ERROR: Cannot find GUARD_SCI_CFG_DIRECTORY! Please set your SCI configurations!\n");
		return NULL;
	}
	snprintf(hfile, MAXPATHLEN-1, "%s/%s", cfg_dir, MACHINES_FILE);

	be_path = getenv(GUARD_SCI_BE_PATH);
	if(be_path == NULL)
	{
		fprintf(stderr, "ERROR: Cannot find GUARD_SCI_BE_PATH! Please set your SCI configurations!\n");
		return NULL;
	}

	//TODO: check if SCI_AGENT_PATH and SCI_LIB_PATH contain agent and lib respectively
	agent_path = getenv(SCI_AGENT_PATH);
	if(agent_path == NULL)
	{
		fprintf(stderr, "ERROR: Cannot find SCI_AGENT_PATH! Please set your SCI configurations!\n");
		return NULL;
	}

	lib_path = getenv(SCI_LIB_PATH);
	if(lib_path == NULL)
	{
		fprintf(stderr, "ERROR: Cannot find SCI_LIB_PATH! Please set your SCI configurations!\n");
		return NULL;
	}

	filter_path = getenv(GUARD_SCI_FILTER_PATH);
	if(filter_path == NULL)
	{
		fprintf(stderr, "ERROR: Cannot find GUARD_SCI_FILTER_PATH! Please set your SCI configurations!\n");
		return NULL;
	}

	gettimeofday(&current_tv, NULL);
	srand(current_tv.tv_sec);
	jobkey_num = rand();
	sprintf(jobkey, "%d", jobkey_num);
	DPRINT("INFO: DbgSCIINvoke is going to create an SCI job with key: %s\n", jobkey);

	snprintf(sp, sizeof(sp), "%s", DEFAULT_LISTENER_PORT);

#if defined (ALPS)
	// we are going to read the location of the filter from the env var
	if ((filter_name = libFind(AGGREGATION_FILTER, CCDB_LIBRARY_DIR)) == (char *)NULL)
	{
		fprintf(stderr, "ERROR: Cannot find %s in %s. Please set SCI environment variables!\n", AGGREGATION_FILTER, CCDB_LIBRARY_DIR);
		return (dbginvoke_t *)NULL;
	}
#else
	if ((filter_name = libFind(AGGREGATION_FILTER, NULL)) == (char *)NULL)
	{
		fprintf(stderr, "ERROR: Cannot find %s in LD_LIBRARY_PATH. Please set your environment variables!\n", AGGREGATION_FILTER);
		return (dbginvoke_t *)NULL;
	}
#endif

#ifdef ALPS /* start sci and dbgsrv via craytool_interface */

	if(prepare_scicfg_files())
	{
		fprintf(stderr, "ERROR: cannot remove sci cfg file!\n");
		return NULL;
	}

	// if PERFLOG is turned on, we need to create a string to the location to 
	// create the log file.
#ifdef PERFLOG
	char *envDir;
	char logfile[PATH_MAX];
	
	if ((envDir = getenv(DBG_LOG_ENV_VAR)) == (char *)NULL)
	{
		// user didn't set the environment variable, so lets
		// read the HOME variable
		if ((envDir = getenv("HOME")) == (char *)NULL)
		{
			// couldn't read HOME, so lets default to tmp
			envDir = "/tmp";
		}
	}
	
	// create the fullpath string to the log file using PATH_MAX plus a null term
	snprintf(logfile, PATH_MAX+1, "%s/ccdb_perf_frontend_sci.log", envDir);
	
	DPERFLOG(CreateTimerLog(logfile));
	DPERFLOG(SetLogInfo("Invoking %d processes\n", nproc));
	DPERFLOG(SetTimerStart("Invoke total"));
#endif /* PERFLOG */

	// create the argv for aprun note that here argv[0] should not be the program name
	// this is to address the fact that guard should be ignorant of the starter applications
	// real name.
	snprintf(sn, sizeof(sn), "%d", nproc);
#ifdef DEBUG
	ap = NewArgv("-n", sn, NULL);
#else
	// we want aprun to be silent when not in debug mode.
	ap = NewArgv("-q", "-n", sn, NULL);
#endif /* DEBUG */

	// only add the launcher args if there are more than 1.
	// we expect at least one containing our stdout fd to dup2 onto.
	tmp = &arch_args[1];
	while (*tmp != (char *)NULL)
	{
		ap = AppendArgs(ap, *tmp++, NULL);
	}
	
	ap = AppendArgs(ap, prog, NULL); // and the prog 
	ap = AppendArgv(ap, argv); // plus its args

#ifdef DEBUG
	DPRINT("aprun args: ");
	tmp = ap;
	while (*tmp != (char *)NULL)
	{
		DPRINT("%s ", *tmp++);
	}
	DPRINT("\n");
#endif /* DEBUG */

	// get the fd to redirect stdout/stderr to
	fd = atoi(arch_args[0]);
	
	// check to see if arch_input is null, if not we need to try and redirect
	// aprun stdin to it.
	if ( arch_input != (char *)NULL )
	{
	        redirectInput = 1;
	}
	
	fprintf(stdout, "launching aprun application...\n"); 
	DPERFLOG(SetTimerStart("aprun launch"));

	// launch aprun using the tool interface
	if ((di->d_pid = launchAprun_barrier(ap, 1, redirectInput, fd, fd, arch_input)) == 0)
	{
		FreeArgv(ap);
		fprintf(stderr, "ERROR: DbgSCIInvoke failed to launch aprun.\n");
		FreeDbgInvoke(di, (void(*)(void *))NULL);
		sigrelse(SIGCHLD);
		
		return (dbginvoke_t *)NULL;
	}
	
	DPERFLOG(SetTimerStopAndLog("aprun launch"));
	// done with this ap
	FreeArgv(ap);

	fprintf(stdout, "transferring binaries to computing nodes...\n"); 
	// ship over the binaries to the compute nodes
	DPERFLOG(SetTimerStart("transfer binaries"));
	
	// first send over gdb-guard
	if (sendCNodeBinary(di->d_pid, "gdb-guard"))
	{
		fprintf(stderr, "ERROR: DbgSCIInvoke failed to transfer gdb-guard.\n");
		killAprun(di->d_pid, SIGKILL);
		FreeDbgInvoke(di, (void(*)(void *))NULL);
		sigrelse(SIGCHLD);
		
		return (dbginvoke_t *)NULL;
	}

	// next send the dbgsrv
	if (sendCNodeBinary(di->d_pid, "dbgsrv"))
	{
		fprintf(stderr, "ERROR: DbgSCIInvoke failed to transfer dbgsrv.\n");
		killAprun(di->d_pid, SIGKILL);
		FreeDbgInvoke(di, (void(*)(void *))NULL);
		sigrelse(SIGCHLD);
		
		return (dbginvoke_t *)NULL;
	}
        
	// next send the dbgsrv
	if (sendCNodeBinary(di->d_pid, "dbgsrv_alps"))
	{
		fprintf(stderr, "ERROR: DbgSCIInvoke failed to transfer dbgsrv_alps.\n");
		killAprun(di->d_pid, SIGKILL);
		FreeDbgInvoke(di, (void(*)(void *))NULL);
		sigrelse(SIGCHLD);
		
		return (dbginvoke_t *)NULL;
	}
        
/*#ifdef BE_VALGRIND
        // send over valgrind
	if (sendCNodeBinary(di->d_pid, "valgrind"))
	{
		fprintf(stderr, "ERROR: DbgSCIInvoke failed to transfer valgrind.\n");
		killAprun(di->d_pid, SIGKILL);
		FreeDbgInvoke(di, (void(*)(void *))NULL);
		sigrelse(SIGCHLD);
		
		return (dbginvoke_t *)NULL;
	}
#endif*/
        
	// send over the aggregation filter
	if (sendCNodeLibrary(di->d_pid, filter_name))
	{
		fprintf(stderr, "ERROR: DbgSCIInvoke failed to transfer %s.\n", AGGREGATION_FILTER);
		killAprun(di->d_pid, SIGKILL);
		FreeDbgInvoke(di, (void(*)(void *))NULL);
		sigrelse(SIGCHLD);
		
		return (dbginvoke_t *)NULL;
	}
	
	DPERFLOG(SetTimerStopAndLog("transfer binaries"));

	DPRINT("Aprun session initialized.\n");
	fprintf(stdout, "parsing SCI topology...\n"); 
        
	DPRINT("aprun has pid [%d]\n", di->d_pid);
	
	// get the apid for sci 
	if ((apid = getApid(di->d_pid)) == 0)
	{
		fprintf(stderr, "ERROR: DbgSCIInvoke failed to retrieve apid.\n");
		killAprun(di->d_pid, SIGKILL);
		FreeDbgInvoke(di, (void(*)(void *))NULL);
		sigrelse(SIGCHLD);
		
		return (dbginvoke_t *)NULL;
	}

	// get a list of the nid hostnames for topology creation
	if ((placement = getAppHostsPlacement(di->d_pid)) == (appHostPlacementList_t*)NULL)
	{
		fprintf(stderr, "ERROR: DbgSCIInvoke failed to retrieve nid hostname list.\n");
		killAprun(di->d_pid, SIGKILL);
		FreeDbgInvoke(di, (void(*)(void *))NULL);
		sigrelse(SIGCHLD);
		
		return (dbginvoke_t *)NULL;
	}

	jobPlacement = convertALPSPlacement(placement);
	destroy_appHostPlacementList(placement);
	if(jobPlacement == NULL)
	{
		fprintf(stderr, "ERROR: DbgSCIInvoke failed to get appHostPlacement!\n"); 
		killAprun(di->d_pid, SIGKILL);
		FreeDbgInvoke(di, (void(*)(void *))NULL);
		sigrelse(SIGCHLD);
		
		return (dbginvoke_t *)NULL;
	}

	// create the topology file used to create the sci front end network
	if(create_placement_file(jobPlacement) < 0)
	{
		destroyJobPlacement(jobPlacement);
		fprintf(stderr, "ERROR: DbgSCIInvoke failed to create machine file!\n"); 
		killAprun(di->d_pid, SIGKILL);
		FreeDbgInvoke(di, (void(*)(void *))NULL);
		sigrelse(SIGCHLD);
		
		return (dbginvoke_t *)NULL;
	}

	destroyJobPlacement(jobPlacement);
	DPRINT("SCI is creating a tree that dbgsrvs can attach!\n");

	scid_log = getenv(SCI_DAEMON_LOG_DIRECTORY);

	ap = NULL;
#ifdef DEBUG
	ap = NewArgv("-l", scid_log, "-s", "6",  NULL);
#endif
	
	DPERFLOG(SetTimerStart("Launch SCI daemon"));

	fprintf(stdout, "launching SCI daemon on computing nodes..\n"); 
	if (sendCNodeExec(di->d_pid, "scidv1", ap, (char **)NULL, dbg))
	{
		fprintf(stderr, "ERROR: DbgSCIInvoke failed to launch scidv1 onto compute nodes!\n");
		FreeArgv(ap);
		killAprun(di->d_pid, SIGKILL);
		FreeDbgInvoke(di, (void(*)(void *))NULL);
		sigrelse(SIGCHLD);
		
		return (dbginvoke_t *)NULL;
	}
	
	DPERFLOG(SetTimerStopAndLog("Launch SCI daemon"));
	// done with the argv array
	if(ap != NULL)
		FreeArgv(ap);

	DPERFLOG(SetTimerStart("SCI_Initiliazation"));
	sprintf(env_buf, "%s=%s", GUARD_SCI_BE_JOBKEY, jobkey);
	ap = NewArgv(env_buf, NULL);
	
	sprintf(env_buf, "%s=%s", GUARD_SCI_BE_HOSTNAME, di->d_cbp->cb_host);
	ap = AppendArgs(ap, env_buf, NULL);

	sprintf(env_buf, "%s=%s", GUARD_SCI_BE_HOSTPORT, sp);
	ap = AppendArgs(ap, env_buf, NULL);

	sprintf(env_buf, "%s=%s", GUARD_SCI_BE_ARCH, arch);
	ap = AppendArgs(ap, env_buf, NULL);

	sprintf(env_buf, "%s=%s", GUARD_SCI_BE_PROTO, proto->sw_name);
	ap = AppendArgs(ap, env_buf, NULL);

	sprintf(env_buf, "%s=%s", GUARD_SCI_BE_DBGBE, backend);
	ap = AppendArgs(ap, env_buf, NULL);

	sprintf(env_buf, "%s=%s", GUARD_SCI_BE_PROGRAM, prog);
	ap = AppendArgs(ap, env_buf, NULL);

	sprintf(real_be_path, "/var/spool/alps/%d/toolhelper%d", apid, apid);
	sprintf(env_buf, "%s=%s", "LD_LIBRARY_PATH", real_be_path);
	ap = AppendArgs(ap, env_buf, NULL);
//#ifdef DEBUG
	char* dbgsrv_log = getenv(GUARD_DBGSRV_LOG_DIR);
	sprintf(env_buf, "%s=%s", GUARD_SCI_BE_LOG_DIR, dbgsrv_log);
	ap = AppendArgs(ap, env_buf, NULL);
//#endif

	fprintf(stdout, "Creating SCI communication network...\n"); 
	sprintf(real_be_path, "/var/spool/alps/%d/toolhelper%d/%s", apid, apid, be_path);
	sci_fe_initialize(jobkey, agent_path, lib_path, hfile, real_be_path, ap);

//	sprintf(real_filter_path, "%s/%s", filter_path, AGGREGATION_FILTER);
	sprintf(real_filter_path, "/var/spool/alps/%d/toolhelper%d/%s", apid, apid, AGGREGATION_FILTER);
//	res = sci_fe_create();
	res = sci_fe_create_with_filter(real_filter_path, DBG_SCI_FILTER_CMD, real_filter_path, DBG_SCI_FILTER_EVENT);
	DPERFLOG(SetTimerStopAndLog("SCI_Initiliazation"));
	FreeArgv(ap);

	if(res < 0)
	{
		DPRINT("ERROR: sci_fe_create failed!\n");
		return NULL;
	}

	DPERFLOG(SetTimerStopAndLog("Invoke total"));
	fprintf(stdout, "SCI communication network has been created...\n"); 

#else /* using MPICH to launch dbgsrv */
	snprintf(starter, MAXPATHLEN-1, "%s/%s", BINDIR, STARTER);
	
	gethostname(root, MAXPATHLEN);

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
/*
	if ( host != NULL )
	{
		ap = AppendArgs(ap, "-h", host, NULL);
		DPRINT("INFO: -=<>=- DbgSCIInvoke host: %s\n", host);
	}
*/
	if ( root != NULL )
	{
		ap = AppendArgs(ap, "-h", root, NULL);
		DPRINT("INFO: -=<>=- DbgSCIInvoke root: %s\n", root);
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

	if ( jobkey != NULL )
		ap = AppendArgs(ap, "-k", jobkey, NULL);

	ap = AppendArgs(ap, "--", prog, NULL);

	ap = AppendArgv(ap, argv);

	/* if we have arch arguments, append it to "ap": "... -- arch_args ..." */

	if ( arch_args != NULL )
	{
		ap = AppendArgs(ap, "--", NULL);
		ap = AppendArgv(ap, arch_args);
	}

	DPRINT("INFO: DbgSCIInvoke is going to launch dbgsrvs...\n");

	sci_fe_initialize(jobkey, agent_path, lib_path, hfile, be_path, NULL);

	if ( (di->d_pid = ForkAndExec(STARTER, ap, env)) < 0 )
	{
		if ( (di->d_pid = ForkAndExec(starter, ap, env)) < 0 )
		{
			FreeArgv(ap);
			FreeDbgInvoke(di, DbgSCIDestroyClient); 
			sigrelse(SIGCHLD);
			return (dbginvoke_t *)NULL;
		}
	}

	FreeArgv(ap);

	DPERFLOG(SetTimerStart("SCI_Initiliazation"));
	res = sci_fe_create();
	DPERFLOG(SetTimerStopAndLog("SCI_Initiliazation"));
	if(res < 0)
	{
		DPRINT("ERROR: sci_fe_create failed!\n");
		return NULL;
	}

#endif /* ALPS */

	num_be = sci_fe_get_be_num();
	res = sci_get_polling_fd(&sci_fd);
	if(res < 0)
	{
		DPRINT("ERROR: sci_get_pooling_fd failed!\n");
		sci_fe_shutdown();
		return NULL;
	}

	fprintf(stdout, "SCI communication network has been created with %d backends...\n", num_be); 
	DPRINT("INFO: SCI communication network has been created succesfully!\n");

	char *request;
	request = (char *) Alloc (1024);
	snprintf(request, 1024 - 1, "INVOKE %d\n", num_be);
	sci_fe_send_command(NULL, request);

	fprintf(stdout, "sending INVOKE to %d backends...\n", num_be); 

    bitset* idset = bitset_new(num_be);
    di->d_bitset_full = (void *)idset;
    bitset_invert(idset);

    idset = bitset_new(num_be);
    di->d_bitset_launched = (void *)idset;

	di->d_tree = (void*)jobkey_num; /*TODO: support multiple SCI fe*/
	di->d_cbp->cb_xprt = di->d_tree;

	DPRINT("DbgSCIInvoke completed di[%p], bitset_full[%p], bitset_launched[%p]!\n", di, di->d_bitset_full, di->d_bitset_launched);


	DPRINT("DbgSCIInvoke STARTER pid %d\n", di->d_pid);

	sigrelse(SIGCHLD);

	return di;
}

int
DbgSCICheckEvents(void **data)
{
	/* TODO: using fd
	fd_set*	fds;
	int		dtbsz = getdtablesize();

	FD_ZERO(&fds);
	FD_SET(sci_fd, &fds);	

	return &fds;*/
	int res = sci_fe_check_io_and_event();

	return res;
}

void
DbgSCIDispatchEvents(void *data)
{
/*	int res = sci_fe_recv();
	if( res <0 ) {
		fprintf(stderr, "DbgSCIDispatchEvents() sci_fe_recv() failed!\n");
		exit(1);
   	}
	DPRINT("DbgSCIDispatchEvents() sci_fe_recv() has got msg!\n");

	if(res > 0)
		sci_fe_handle_event();
*/
//	sci_fe_handle_io();
	sci_fe_handle_event();
}

/*
** Compatible callback routine for sci invoke.
*/
int
cbSCIInvProc(rep_set_t *set, void *arg)
{
	static int	started = 0;
	int		(*cbrtn)();
	dbgproc_t *	dp;
	dbginvoke_t *	di = (dbginvoke_t *)arg;
	group_reply_t* reply = NULL;
	dbgevent_t* e = NULL;

	if(set == NULL)
		return 0;

	if(started == 0)
	{
		started ++;
		fprintf(stderr, "cbMRNInvProc is invokded for the first time!\n");
	}
	DPRINT("cbMRNInvProc is invokded!\n");

    bitset* full_set = (bitset*)di->d_bitset_full;
    bitset* launched_set = (bitset*)di->d_bitset_launched;

	SetReplyHeadInSet(set);
	while((reply = GetNextReplyInSet(set)) != (group_reply_t*)NULL)
	{
		bitset* source = reply->group;
		char* source_str = bitset_to_str(source);
		char* msg = reply->msg;
		e = SocProtoToEvent(msg);

   		DPRINT("cbMRNInvProc received a response from %s with event [%s]!\n", source_str, msg);
    	free(source_str);

    	bitset_oreq(launched_set, source);

		if(e->ev_event != DBGEV_OK)
			break;
	}
	FreeRepSet(set);

    int full = bitset_eq(launched_set, full_set); 

    DPRINT("cbMRNInvProc received responses [%s]!\n", bitset_to_str(launched_set));

	if(full || e->ev_event != DBGEV_OK) 
	{
		cbrtn = di->d_cbrtn;

		CallBackDestroy(di->d_proto, di->d_cbp);
		di->d_cbp = (cb_t *)NULL;
		di->d_cbdone = 1;

		if(full && e->ev_event == DBGEV_OK) 
		{
			int index;
			for(index = 0; index < di->d_nproc; ++index)
			{
				dp = di->d_procs[index];
				dp->d_started = 1;
			}

			sci_fe_set_invoke_completed();
			DPERFLOG(SetTimerStopAndLog("stream_init"));

			fprintf(stderr, "dbgsrv callback complete.\n");
#ifdef ALPS
			fprintf(stdout, "Releasing application from the startup barrier...\n");
	        // tell aprun to release the application from the startup barrier
			if (releaseAprun_barrier(di->d_pid))
			{
				fprintf(stderr, "ERROR: Could not release application from the startup barrier.\n");
				killAprun(di->d_pid, SIGKILL);
		
				return 0;
			}
#endif

			fprintf(stderr, "Invoke complete.\n");
		}

		return (*cbrtn)(e, (void *)di);
	}
	return 0;
}

/*
**********************************************************
**********************************************************
************ WRAPPERS FOR SCI ROUTINES *****************
**********************************************************
**********************************************************
*/

#ifdef DEBUG
void
LogCmdReceiver(char* cmd, int ind, bitset* target)
{
	char* str = NULL;
	if(ind >= 0)
	{
	    fprintf(stderr, "%s is sending to be[%d]\n", cmd, ind);
	}
	else if(target != NULL)
	{
		str = bitset_to_str(target);
		fprintf(stderr, "%s is sending to set[%s]\n", cmd, str);
		free(str);
	}
}
#endif

bitset*
send_cmd(void* net, int be_rank, bitset* target, char* request, int crr_status)
{
	bitset *		received = NULL;

	HoldSignals();
	sci_fe_send_command(target, request);

	received = bitset_new(bitset_size(target));
	
	DPRINT("INFO: Sending cmd to set[%s]\n", bitset_to_set(target));

	ReleaseSignals();

	return received;
}

rep_set_t*
recv_reply(void* net, int be_rank, bitset* target, bitset* received, int crr_status)
{
	int				res;
	int				full = 0;
	int				status;
	char *			s = NULL;
	char			par[1024];
	char *			result = NULL;
	bitset *		source = NULL;	
	rep_set_t**	recSets;
	rep_set_t*	recSet;
	group_reply_t* reply = NULL;
	int				number, index;
	rep_set_t *		set = NewRepSet(net);
	group_reply_t *	group_rep = NULL;
	group_err_t *	group_err = NULL;

	HoldSignals();

	while(!full)
	{
		number = 0;
		res = sci_fe_recv_reply(target, &recSets, &number);
		if(res != 1)
			continue;

		for(index = 0; index < number; index++)
		{
		recSet = recSets[index];

		SetReplyHeadInSet(recSet);	
		while((reply = GetNextReplyInSet(recSet)) != (group_reply_t*)NULL)
		{
		source = reply->group;
		result = reply->msg;
    	bitset_oreq(received, source);
    	full = bitset_eq(target, received); 		

    	DPRINT("INFO: recv_reply() received %s from %s\n", result, bitset_to_set(source));

		s = result;
		s = getword(s,par);
		status = atoi(par);

		if(status != crr_status )
		{
			if (status == DBGEV_ERROR) {
				s = getword(s,par);
				s = skipspace(s);
				group_err = NewGroupErr(source, atoi(par), strdup(s));
				AddErrToSet(set, group_err);
			}
		} else if(s != NULL){
			s = skipspace(s);
			group_rep = NewGroupReply(source, s);
			AddReplyToSet(set, group_rep);
		}
		}
		}
    	DPRINT("INFO: recv_reply() already received reply from %s\n", bitset_to_str(received));
	}

	ReleaseSignals();

	return set;
}

rep_set_t*
send_cmd_and_recv_reply(void* net, bitset* target, char* request, int crr_status)
{
	int 	full = 0;
	int		res = 0;
	int		status;
	char	*s = NULL;
	char	par[1024];
	char	*result = NULL;
	bitset	*source = NULL;	
	bitset* received = NULL;
	rep_set_t**	recSets;
	rep_set_t*	recSet;
	group_reply_t* reply = NULL;
	int 	number, index;

	sci_group_t group;
	rep_set_t* set = NewRepSet(net);

	group_reply_t* group_rep = NULL;
	group_err_t* group_err = NULL;

	HoldSignals();
    sci_fe_send_command(target, request);

	received = bitset_new(bitset_size(target));
	
    DPRINT("INFO: send_cmd_and_recv_reply() before receiving reply.\n");

	while(!full)
	{
//		res = sci_fe_recv_reply(target, &result, &source, &number);
		number = 0;
		res = sci_fe_recv_reply(target, &recSets, &number);
		if(res != 1)
			continue;


		for(index = 0; index < number; index++)
		{
		recSet = recSets[index];

		SetReplyHeadInSet(recSet);	
		while((reply = GetNextReplyInSet(recSet)) != (group_reply_t*)NULL)
		{
		source = reply->group;
		result = reply->msg;
    	bitset_oreq(received, source);
    	full = bitset_eq(target, received); 		

    	DPRINT("INFO: send_cmd_and_recv_reply() received %s from %s\n", result, bitset_to_set(source));

		s = result;
		s = getword(s,par);
		status = atoi(par);

		if(status != crr_status )
		{
			if (status == DBGEV_ERROR) {
				s = getword(s,par);
				s = skipspace(s);
				group_err = NewGroupErr(source, atoi(par), strdup(s));
				AddErrToSet(set, group_err);
			}
		} else if(s != NULL){
			s = skipspace(s);
			group_rep = NewGroupReply(source, s);
			AddReplyToSet(set, group_rep);
		}
		}
		}
    	DPRINT("INFO: send_cmd_and_recv_reply() already received reply from %s\n", bitset_to_str(received));
	}

	ReleaseSignals();

	return set;
}

/*
** DBG_SENDEXPR: Send expression to BE.
*/
bitset *
DbgSCISendExpr(rec_set_t *rec_set, char *exp, int assid)
{
	int		rlen;
	char *		request = NULL;
	int 		num = 0;
	void  *		network = rec_set->s_proto_handler;
	bitset *	target = rec_set->s_receiver_bs;
	int			ind = rec_set->s_ind_proc;

#ifdef DEBUG
	LogCmdReceiver("DbgSCISendExpr", ind, target);
#endif

	num = bitset_size(target);

	rlen = 1024 + strlen(exp);
	request = (char *) Alloc(rlen);

	snprintf(request, rlen-1, "EVALEXPR %s", exp);

	bitset* set = send_cmd(network, ind, target, request, DBGEV_DATA);
	Free(request);
   
	return set;
}

/*
** DBG_ETEXPR: Get results of evaluate expression.
*/
AIF *
DbgSCIGetExpr(rec_set_t *rec_set, bitset *received, int assid)
{
	void *		network = rec_set->s_proto_handler;
	bitset *	target = rec_set->s_receiver_bs;
	int			ind = rec_set->s_ind_proc;

#ifdef DEBUG
	LogCmdReceiver("DbgSCIGetExpr", ind, target);
#endif

	rep_set_t* set = recv_reply(network, ind, target, received, DBGEV_DATA);
   
	DbgSetRepSet(set);
	return ConvertRepSetToAIF(set);
}

/*
** DBG_EVALEXPR: Evaluate expression.
*/
AIF *
DbgSCIEvalExpr(rec_set_t *rec_set, char *exp, int assid)
{
	int		rlen;
	int 		status;
	char *		request = NULL;
	char **		result = NULL;
	AIF **		aif_res = NULL;
	int 			num = 0;
	int				index = 0;
	char *		s = NULL;
	char		par[1024];
	int 		full = 0;

/*	if ( !dp->d_started )
	{
		DbgSetErr(DBGERR_SERVER, NULL);
		return (AIF *)NULL;
	}*/

	void * 		network = rec_set->s_proto_handler;
	bitset *	target = rec_set->s_receiver_bs;
	int			ind = rec_set->s_ind_proc;
#ifdef DEBUG
	LogCmdReceiver("DbgSCIEvalExpr", ind, target);
#endif

	num = bitset_size(target);

	rlen = 1024 + strlen(exp);
	request = (char *) Alloc(rlen);

	snprintf(request, rlen-1, "EVALEXPR %s\n", exp);

	rep_set_t* set = send_cmd_and_recv_reply(network, target, request, DBGEV_DATA);
    Free(request);

	DbgSetRepSet(set);
	return ConvertRepSetToAIF(set);
}

/*
** DBG_SETVAR: Set variable in program to new value.
*/
int
DbgSCISetVar(rec_set_t *rec_set, char *var, AIF *val)
{
	int             rlen;
    int             status;
	char *		st_aif;
    char *          request;
    char *          result;
	char *		s;
	char		par[1024];
	int 	num;
	int		ok = 0;
	int		full = 0;

	void * 		network = rec_set->s_proto_handler;
	bitset *	target = rec_set->s_receiver_bs;
	int			ind = rec_set->s_ind_proc;

/*	if ( !dp->d_started )
	{
		DbgSetErr(DBGERR_SERVER, NULL);
		return -1;
	}*/

#ifdef DEBUG
	LogCmdReceiver("DbgSCISetVar", ind, target);
#endif

	num = bitset_size(target);

	st_aif = AIFToSocProto(val);

	rlen = 1024 + strlen(var) + strlen(st_aif);
	request = (char *) Alloc(rlen);

	snprintf(request, rlen-1, "SETVAR %s %s\n", var, st_aif);
	Free(st_aif);

	rep_set_t* set = send_cmd_and_recv_reply(network, target, request, DBGEV_OK);

    Free(request); 

	if(SetHasError(set))
	{
		DbgSetErrSet(set);
		ok = -1;
	} else
		FreeRepSet(set);

	return ok;
}

/*
** DBG_GETTYPE: Get type of variable.
*/
rep_set_t *
DbgSCIGetType(rec_set_t *rec_set, char *var)
{
	int             rlen;
	int             status;
	char *          request;
	char **          result;
	char **     types;
	char *		s = NULL;
	char		par[1024];
	int		num, index;
	int		full = 0;
	void * 		network = rec_set->s_proto_handler;
	bitset *	target = rec_set->s_receiver_bs;
	int			ind = rec_set->s_ind_proc;

/*	if ( !dp->d_started )
	{
		DbgSetErr(DBGERR_SERVER, NULL);
		return NULL;
	}*/

#ifdef DEBUG
	LogCmdReceiver("DbgSCIGetType", ind, target);
#endif

	num = bitset_size(target);

	rlen = 1024 + strlen(var);
	request = (char *) Alloc(rlen);

	snprintf(request, rlen-1, "GETTYPE %s\n", var);

	rep_set_t* set = send_cmd_and_recv_reply(network, target, request, DBGEV_TYPE);
    Free(request);

	return set;
}

/*
** DBG_GO: Continue/start execution.
*/
int
DbgSCIGo(rec_set_t *rec_set)
{
	int             rlen;
	int             status;
	char *          request;
	char **          result;
	SOCPRT *        socp;
	char *		s;
	char		par[1024];
	int 		full = 0;
	int 		ok = 0;
	void * 		network = rec_set->s_proto_handler;
	bitset *	target = rec_set->s_receiver_bs;
	int			ind = rec_set->s_ind_proc;

/*	if ( !dp->d_started ) 
	{
		DbgSetErr(DBGERR_SERVER, NULL);
		return -1;
	}

	if ( dp->d_cbp == (cb_t *)NULL )
	{
		DbgSetErr(DBGERR_CALLBACK, NULL);
		return -1;
	}*/

#ifdef DEBUG
	LogCmdReceiver("DbgSCIGo", ind, target);
#endif

	char sciHost[] = "sci"; 	//compatible with the format
	int sciPort = 0;			//compatible with the format
	rlen = 1024 + strlen(sciHost);
	request = (char *) Alloc(rlen);

    snprintf(request, rlen-1, "GO %s %d\n", sciHost, sciPort);

	rep_set_t* set = send_cmd_and_recv_reply(network, target, request, DBGEV_OK);
	Free(request);

	if(SetHasError(set))
	{
		DbgSetErrSet(set);
		ok = -1;
	} else 
		FreeRepSet(set);

	return ok; 
}

/*
** DBG_SETLINEBREAK: Set a breakpoint in program at 
**                   specified line.
*/
rep_set_t *
DbgSCISetLineBreak(rec_set_t *rec_set, char *file, int line)
{
        int             rlen;
        int             status;
        char *          request;
        char **          result;
		dbgbp_t **	bpt = NULL;
		int 			num = 0;
		int				index = 0;
	char *		s;
	char		par[1024];
	int			full = 0;
	void * 		network = rec_set->s_proto_handler;
	bitset *	target = rec_set->s_receiver_bs;
	int			ind = rec_set->s_ind_proc;

/*	if ( !dp->d_started )
	{
		DbgSetErr(DBGERR_SERVER, NULL);
		return (dbgbp_t *)NULL;
	}*/

#ifdef DEBUG
	LogCmdReceiver("DbgSCISetLineBreak", ind, target);
#endif
	num = bitset_size(target);

	if ( file == NULL )
		file = "<null>";

	rlen = 1024 + strlen(file);
	request = (char *) Alloc(rlen);

	snprintf(request, rlen-1, "SETLINEBREAK %s %d\n", file, line);

	rep_set_t* set = send_cmd_and_recv_reply(network, target, request, DBGEV_BPSET);
    Free(request);

	return set;
}

/*
** DBG_SETFUNCBREAK: Set a breakpoint in program at 
**                   specified function.
*/
rep_set_t *
DbgSCISetFuncBreak(rec_set_t *rec_set, char *file, char *func)
{

        int             rlen;
        int             status;
        char *          request;
        char **          result;
        dbgbp_t **       bpt = NULL;
		int 			num = 0;
		int				index = 0;
	char		par[1024];
	char *		s;
	int			full = 0;
	void * 		network = rec_set->s_proto_handler;
	bitset *	target = rec_set->s_receiver_bs;
	int			ind = rec_set->s_ind_proc;

/*	if ( !dp->d_started )
	{
		DbgSetErr(DBGERR_SERVER, NULL);
		return (dbgbp_t *)NULL;
	}*/

#ifdef DEBUG
	LogCmdReceiver("DbgSCISetFuncBreak", ind, target);
#endif

	if ( file == NULL )
                file = "<null>";

	rlen = 1024 + strlen(file) + strlen(func);
	request = (char *) Alloc(rlen);

	snprintf(request, rlen-1, "SETFUNCBREAK %s %s\n", file, func);

	rep_set_t* set = send_cmd_and_recv_reply(network, target, request, DBGEV_BPSET);
    Free(request);

	return set;
}

/*
** DBG_KILL: Terminate execution of program.
*/
int
DbgSCIKill(rec_set_t *rec_set)
{
        int             status;
        char **          result;
	char *		s;
	char		par[1024];
	int			full = 0;
	int			ok = 0;
	void * 		network = rec_set->s_proto_handler;
	bitset *	target = rec_set->s_receiver_bs;
	int			ind = rec_set->s_ind_proc;

/*	if ( !dp->d_started ) 
	{
		DbgSetErr(DBGERR_SERVER, NULL);
		return -1;
	}*/

#ifdef DEBUG
	LogCmdReceiver("DbgSCIKill", ind, target);
#endif

    char cmd[] = "KILL\n";

	rep_set_t* set = send_cmd_and_recv_reply(network, target, cmd, DBGEV_OK);

	if(SetHasError(set))
	{
		DbgSetErrSet(set);
		ok = -1;
	} else
		FreeRepSet(set);

	return ok;
}

/*
** DBG_DELETEBREAK: Delete a breakpoint.
**
** Note: the GDB "delete breakpoint" command (delete all
** breakpoints) must be implemented using DBG_SHOWBREAK
** and repeated DBG_DELETEBREAK.
*/
int
DbgSCIDeleteBreak(rec_set_t *rec_set, int bp)
{
	int             rlen;
	int             status;
	char *          request;
	char *          result;
	char *		s;
	char		par[1024];
	int			ok = 0;
	void * 		network = rec_set->s_proto_handler;
	bitset *	target = rec_set->s_receiver_bs;
	int			ind = rec_set->s_ind_proc;

/*	if ( !dp->d_started )
	{
		DbgSetErr(DBGERR_SERVER, NULL);
		return -1;
	}*/

#ifdef DEBUG
	LogCmdReceiver("DbgSCIDeleteBreak", ind, target);
#endif

	rlen = 1024;
	request = (char *) Alloc(rlen);
	snprintf(request, rlen-1, "DELETEBREAK %d\n", bp);

	rep_set_t* set = send_cmd_and_recv_reply(network, target, request, DBGEV_OK);
	if(SetHasError(set))
	{
		DbgSetErrSet(set);
		ok = -1;
	} else
        FreeRepSet(set);

	Free(request);

	return 0;
}

#ifdef notdef
/*
** DBG_SHOWBREAK: List all breakpoints.
*/
dbgbp_t *
DbgSCIShowBreak(rec_set_t *rec_set)
{
    fprintf(stderr, "ERROR: DbgSCIGetShowBreak should not be called!\n");
	return NULL;
}
#endif /* notdef */

/*
** DBG_LIST: List specified function or line.
*/

int
DbgSCIList(rec_set_t *rec_set, char *str, rep_set_t **result)
{
	int             rlen;
	int             status;
	char *          request;
	char            par[1024];
	char *          s;
	void * 		network = rec_set->s_proto_handler;
	bitset *	target = rec_set->s_receiver_bs;
	int			ind = rec_set->s_ind_proc;

/*	if ( !dp->d_started )
	{
		DbgSetErr(DBGERR_SERVER, NULL);
		return -1;
	}*/

#ifdef DEBUG
	LogCmdReceiver("DbgSCIList", ind, target);
#endif

	if ( str == NULL )
		str = "<null>";

	rlen = 1024 + strlen(str);
	request = (char *) Alloc(rlen);

	snprintf(request, rlen-1, "LIST %s\n", str);

	rep_set_t* set = send_cmd_and_recv_reply(network, target, request, DBGEV_LIST);

	*result = set;

	Free(request);

	return 0;
}

/*
** DBG_SETARGS: Set arguments to pass to program when run.
*/
int
DbgSCISetArgs(rec_set_t *rec_set, char *args)
{
    int             rlen;
    int             status;
    char *          request;
    char **          result;
	char *		s;
	char		par[1024];
	int			ok = 0;
	int			full = 0;

	void *		network = rec_set->s_proto_handler;
	bitset* 	target = rec_set->s_receiver_bs;
	int			ind = rec_set->s_ind_proc;
        
/*	if ( !dp->d_started )
	{
		DbgSetErr(DBGERR_SERVER, NULL);
		return -1;
	}*/
#ifdef DEBUG
	LogCmdReceiver("DbgSCISetArgs", ind, target);
#endif

	if ( args == NULL )
		args = "<null>";

    rlen = 1024 + strlen(args);
    request = (char *) Alloc(rlen);

    snprintf(request, rlen-1, "SETARGS %s\n", args);

	rep_set_t* set = send_cmd_and_recv_reply(network, target, request, DBGEV_OK);

	Free(request);

	if(SetHasError(set))
	{
		DbgSetErrSet(set);
		ok = -1;
	} else
        FreeRepSet(set);

	return ok;
}

/*
** DBG_STEP: Execute to next statement. Repeat count times.
**           If in == 0, pass over function calls (ala next).
*/
int
DbgSCIStep(rec_set_t *rec_set, int count, int in)
{
        int             rlen;
        int             status;
        char *          request;
        char **          result;
        SOCPRT *        socp;
	char *		s;
	char		par[1024];
	int			full = 0;
	int			ok = 0;

	void *		network = rec_set->s_proto_handler;
	bitset* 	target = rec_set->s_receiver_bs;
	int			ind = rec_set->s_ind_proc;
        
/*	if ( !dp->d_started )
	{
		DbgSetErr(DBGERR_SERVER, NULL);
		return -1;
	}

	if ( dp->d_cbp == (cb_t *)NULL )
	{
		DbgSetErr(DBGERR_CALLBACK, NULL);
		return -1;
	}*/
#ifdef DEBUG
	LogCmdReceiver("DbgSCIStep", ind, target);
#endif

	char sciHost[] = "sci";		//compatiable with the format of cmd
	int sciPort = 0;
	rlen = 1024 + strlen(sciHost);
	request = (char *) Alloc(rlen);
	snprintf(request, rlen-1, "STEP %d %d %s %d\n", count, in, sciHost, sciPort);

	rep_set_t* set = send_cmd_and_recv_reply(network, target, request, DBGEV_OK);

	if(SetHasError(set))
	{
		DbgSetErrSet(set);
		ok = -1;
	} else
		 FreeRepSet(set);

	Free(request);

	return ok;
}

/*
** DBG_MOVEFRAME: Move up or down stack frames.
*/
rep_set_t *
DbgSCIMoveFrame(rec_set_t *rec_set, int count, int up)
{
    int             rlen;
    int             status;
    char *          request;
    char **          result;
	dbgframe_t *	st_frame;
	char *          s;
	char            par[1024];
	int			full = 0;

	void *		network = rec_set->s_proto_handler;
	bitset* 	target = rec_set->s_receiver_bs;
	int			ind = rec_set->s_ind_proc;
        
/*	if ( !dp->d_started )
	{
		DbgSetErr(DBGERR_SERVER, NULL);
		return (dbgframe_t *)NULL;
	}*/
#ifdef DEBUG
	LogCmdReceiver("DbgSCIMoveFrame", ind, target);
#endif

    rlen = 1024;
    request = (char *) Alloc(rlen);

    snprintf(request, rlen-1, "MOVEFRAME %d %d\n", count, up);

	rep_set_t* set = send_cmd_and_recv_reply(network, target, request, DBGEV_FRAME);

    Free(request);

	return set;
}

/*
** DBG_SHOWFRAME: Show current stack.
*/
rep_set_t *
DbgSCIShowFrame(rec_set_t *rec_set)
{

	dbgframe_t *	dpf;
	int				status;
	char **			result;
	char *          s;
	char            par[1024];
	void *			network = rec_set->s_proto_handler;
	bitset* 		target = rec_set->s_receiver_bs;
	int				ind = rec_set->s_ind_proc;


/*	if ( !dp->d_started )
	{
		DbgSetErr(DBGERR_SERVER, NULL);
		return (dbgframe_t *)NULL;
	}*/
#ifdef DEBUG
	LogCmdReceiver("DbgSCIShowFrame", ind, target);
#endif

	char cmd[] = "SHOWFRAME\n";

	rep_set_t* set = send_cmd_and_recv_reply(network, target, cmd, DBGEV_FRAME);
	return set;
}

/*
** DBG_GETLOCATION: Return current location in program.
*/
rep_set_t *
DbgSCIGetLocation(rec_set_t *rec_set)
{
    fprintf(stderr, "DbgSCIGetLocation is not supported yet!\n");

	return NULL;
}

/*
** DBG_GETVARS: Return current variables in program.
*/
rep_set_t *
DbgSCIGetVars(rec_set_t *rec_set, char *file)
{
    int         status=1;
	int			rlen;
	char *			request;
    char *                  result;
	dbgvars_t **		st_vars;
	char *			s;
	char			par[1024];
	int 			num = 0;
	int				index = 0;
	int			full = 0;
	void *		network = rec_set->s_proto_handler;
	bitset* 		target = rec_set->s_receiver_bs;
	int				ind = rec_set->s_ind_proc;
        
/*	if ( !dp->d_started )
	{
		DbgSetErr(DBGERR_SERVER, NULL);
		return (dbgvars_t *)NULL;
	}*/
#ifdef DEBUG
	LogCmdReceiver("DbgSCIGetVars", ind, target);
#endif

	num = bitset_size(target);

	if ( file == NULL )
		file = "<null>";

    rlen = 1024 + strlen(file);
    request = (char *) Alloc(rlen);

	snprintf(request, rlen-1, "GETVARS %s\n", file);

	rep_set_t* set = send_cmd_and_recv_reply(network, target, request, DBGEV_VARS);
    Free(request);
	return set;
}

/*
** DBG_DBGSTATUS: Return status of debugger.
*/
dbgstat
DbgSCIStatus(rec_set_t *rec_set)
{
	int			status;
	char **		result;
	int			res_event;
	int			res_status;
	char		par[1024];

	void *		network = rec_set->s_proto_handler;
	bitset* 		target = rec_set->s_receiver_bs;
	int				ind = rec_set->s_ind_proc;
        
/*	if ( !dp->d_started )
	{
		DbgSetErr(DBGERR_SERVER, NULL);
		return DBGSTAT_INTERR;
	}*/
#ifdef DEBUG
	LogCmdReceiver("DbgSCIStatus", ind, target);
#endif

    char cmd[] = "STATUS\n";

	rep_set_t *set = send_cmd_and_recv_reply(network, target, cmd, DBGEV_STAT);
	group_reply_t* 	reply;

	if(SetHasError(set))
	{
		DbgSetErrSet(set);
		res_status = DBGSTAT_INTERR;
	} else 
	{
		SetReplyHeadInSet(set);
		while((reply = GetNextReplyInSet(set)) != (group_reply_t*)NULL)
		{
			char * msg = reply->msg;
			msg = skipspace(msg);
			res_status = atoi(msg);
		}
	}
	FreeRepSet(set);

	return res_status;
}

/*
** DBG_SETDEBUGGERVAR: Set variable in debugger to new value.
*/
int
DbgSCISetDebuggerVar(rec_set_t *rec_set, char *var, AIF *val)
{
	int		rlen;
	char *	        st_aif;
	char *	        request;

	void *		network = rec_set->s_proto_handler;
	bitset* 		target = rec_set->s_receiver_bs;
	int				ind = rec_set->s_ind_proc;
	int				ok = 0;

/*	if ( !dp->d_started )
	{
		DbgSetErr(DBGERR_SERVER, NULL);
		return -1;
	}*/

	st_aif = AIFToSocProto(val);

	rlen = 1024 + strlen(var) + strlen(st_aif);
	request = (char *) Alloc(rlen);

	snprintf(request, rlen-1, "SETDEBUGGERVAR %s %s\n", var, st_aif);
	Free(st_aif);

	rep_set_t *set = send_cmd_and_recv_reply(network, target, request, DBGEV_OK);

	Free(request);

	if(SetHasError(set))
	{
		DbgSetErrSet(set);
		ok = -1;
	} else 
		FreeRepSet(set);

	return ok;
}

/*
** DBG_SETSOURCEPATH: Set source search path in debugger to new value.
*/
rep_set_t *
DbgSCISetSourcePath(rec_set_t *rec_set, AIF *val)
{
	int		rlen;
	char *	        st_aif;
	char *	        request;

	void *			network = rec_set->s_proto_handler;
	bitset* 		target = rec_set->s_receiver_bs;
	int				ind = rec_set->s_ind_proc;

/*	if ( !dp->d_started )
	{
		DbgSetErr(DBGERR_SERVER, NULL);
		return (AIF *)NULL;
	}*/

	st_aif = AIFToSocProto(val);

	rlen = 1024 + strlen(st_aif);
	request = (char *) Alloc(rlen);

	snprintf(request, rlen-1, "SETSOURCEPATH %s\n", st_aif);
	Free(st_aif);

	rep_set_t *set = send_cmd_and_recv_reply(network, target, request, DBGEV_DATA);
	Free(request);
	return set;
}

/*
** DBG_GETSOURCEPATH: Get source search path from debugger.
*/
rep_set_t *
DbgSCIGetSourcePath(rec_set_t *rec_set)
{
	int		rlen;
	char *	        request;

	void *			network = rec_set->s_proto_handler;
	bitset* 		target = rec_set->s_receiver_bs;
	int				ind = rec_set->s_ind_proc;
/*	if ( !dp->d_started )
	{
		DbgSetErr(DBGERR_SERVER, NULL);
		return (AIF *)NULL;
	}*/

	rlen = 1024;
	request = (char *) Alloc(rlen);

	snprintf(request, rlen-1, "GETSOURCEPATH");

	rep_set_t *set = send_cmd_and_recv_reply(network, target, request, DBGEV_DATA);
	Free(request);
	return set;
}

/*
** DBG_GETDEBUGGERVAR: Get variable in debugger.
*/
rep_set_t *
DbgSCIGetDebuggerVar(rec_set_t *rec_set, char *var)
{
	int		rlen;
	char *	request;

	void *			network = rec_set->s_proto_handler;
	bitset* 		target = rec_set->s_receiver_bs;
	int				ind = rec_set->s_ind_proc;

/*
	if ( !dp->d_started )
	{
		DbgSetErr(DBGERR_SERVER, NULL);
		return (AIF *)NULL;
	}*/

	rlen = 1024 + strlen(var);
	request = (char *) Alloc(rlen);

	snprintf(request, rlen-1, "GETDEBUGGERVAR %s\n", var);

	rep_set_t *set = send_cmd_and_recv_reply(network, target, request, DBGEV_DATA);
	Free(request);
	return set;
}

/*
** DBG_INTERRUPT: Interrupt current debugger operation.
*/
int
DbgSCIInterrupt(rec_set_t *rec_set)
{
	int             rlen;
	int             status;
	char *          request;
	char **          result;
	SOCPRT *	socp;
	char *		s;
	char		par[1024];
	int			full = 0;
	int			ok = 0;
	void *			network = rec_set->s_proto_handler;
	bitset* 		target = rec_set->s_receiver_bs;
	int				ind = rec_set->s_ind_proc;
        
#ifdef DEBUG
	LogCmdReceiver("DbgSCIInterrupt", ind, target);
#endif

/*	if ( !dp->d_started )
	{
		DbgSetErr(DBGERR_SERVER, NULL);
		return -1;
	}*/

	/*
	** Check if process has callback (i.e. is running)
	*/

/*	if ( dp->d_cbp == (cb_t *)NULL )
		return 0;*/

	char sciHost[] = "sci";
	int	sciPort = 0;
    rlen = 1024 + strlen(sciHost);
    request = (char *) Alloc(rlen);
    snprintf(request, rlen-1, "INTERRUPT %s %d\n", sciHost, sciPort);

	rep_set_t* set = send_cmd_and_recv_reply(network, target, request, DBGEV_OK);
	Free(request);

	if(SetHasError(set))
	{
		DbgSetErrSet(set);
		ok = -1;
	} else 
		FreeRepSet(set);

	return ok;
}

/*
** Called by DbgQuit() and DbgQuitAll()
*/
int
DbgSCIQuit(dbginvoke_t *di)
{
	int	status;
	char **	result;
	char *	s;
	char	par[1024];
	int 	full = 0;

	void *			network = di->d_tree;
   	bitset* 		target = di->d_bitset_full;
	int				res = 0;

	fprintf(stderr, "INFO: shutting down all dbg servers via SCI.\n");
#ifdef DEBUG
	LogCmdReceiver("DbgSCIQuit", -1, target);
#endif
	char cmd[] = "QUIT\n";
/*	rep_set_t* set = send_cmd_and_recv_reply(network, target, cmd, DBGEV_OK);

	if(SetHasError(set))
	{
		DbgSetErrSet(set);
		res = -1;
	} else 
		FreeRepSet(set); */

	send_cmd(network, -1, target, cmd, DBGEV_OK);

//	sci_fe_shutdown();
#ifdef ALPS
	// send a kill signal using apkill to the apid of the aprun pid
	killAprun(di->d_pid, SIGKILL);
#endif

	return res;
}

void
DbgSCIDestroyClient(void *clnt)
{
	//socclnt_destroy((SOCCLIENT *)clnt);
}
