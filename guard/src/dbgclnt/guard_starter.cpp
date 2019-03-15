/*********************************************************************************\
 * Program starter.
 * 
 * usage: guard_starter [-d] -a arch -c cbhost -x cbport -r proto 
 *			[-h host] [-n procs] [-u user] [-p password] 
 *			-- prog [args]
 *
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
#include        <config.h>
#endif /* HAVE_CONFIG_H */

#ifndef WIN32
#include	<sys/param.h>
#include	<sys/socket.h>
#include	<sys/wait.h>
#include        <netinet/in.h>
#include        <netdb.h>
#include        <signal.h>
#else /* !WIN32 */
#include        <windows.h>
#include        <winsock.h>
#include        <process.h>
#endif /* !WIN32 */

#include	<stdio.h>
#include	<unistd.h>
#include	<stdlib.h>
#include	<stdarg.h>
#include	<string.h>
#include	<fcntl.h>

#include	"defs.h"
#include	"args.h"
#include	"exec.h"
#include	"alloc.h"

struct start_type
{
	char *	arch;
	int	(*proc)();
	int	mp;
	char *	nproc;
	char *	host;
	char *	user;
	char *	pass;
	char *	proto;
	char *	cbhost;
	char *	cbport;
	char *	dbgsrv;
	char *	backend;
	char *	prog;
	char *	cfg_env;
	char **	argv;
	char **	env;
	char **	arch_argv;
};

int StartLocal(struct start_type *);
#ifdef RSH
int StartRsh(struct start_type *);
#endif /* RSH */
#ifdef SSH
int StartSsh(struct start_type *);
#endif /* SSH */
#ifdef BPROC
int StartBpsh(struct start_type *);
#endif /* BPROC */
int StartClustor(struct start_type *);
#ifdef AIX_POE
int StartPOE(struct start_type *);
#endif /* AIX_POE */
#ifdef MPICH
int StartMPICH(struct start_type *);
#endif /* MPICH */
#ifdef P4
int StartP4(struct start_type *);
#endif /* P4 */
#ifdef OMPI
int StartOMPI(struct start_type *);
#endif /* OMPI */
#ifdef ALPS
int StartAlps(struct start_type *);
#endif /* ALPS */

struct start_type	StartTypes[] =
{
	{"local",	StartLocal,	0},
#ifdef RSH
	{"rsh",		StartRsh,	0},
#endif /* RSH */
#ifdef SSH
	{"ssh",		StartSsh,	0},
#endif /* SSH */
#ifdef BPROC
	{"bpsh",	StartBpsh,	0},
#endif /* BPROC */
	{"clustor",	StartClustor,	1},
#ifdef AIX_POE
	{"poe",		StartPOE,	1},
#endif /* AIX_POE */
#ifdef MPICH
	{"mpich",	StartMPICH,	1},
#ifdef MPICH_MPD
	{"mpich-mpd",	StartMPICH,	1},
	{"mpirun",		StartMPICH,	1},
#endif /* MPICH_MPD */
#ifdef BPROC
	{"mpich-gm",	StartMPICH,	1},
	{"mpich-p4",	StartMPICH,	1},
#endif /* BPROC */
#endif /* MPICH */
#ifdef P4
	{"p4",		StartP4,	1},
#endif /* P4 */
#ifdef OMPI
	{"ompi",	StartOMPI,	1},
#endif /* OMPI */
#ifdef ALPS
    {"alps",        StartAlps,      1},
#endif
//#if defined(MRNET) && defined(ALPS)
//    {"mrnet",       StartMRNet,      1},
//#endif /* ALPS */
	{NULL,		NULL,		0}
};

int			Debug = 0;

extern int		errno;
extern int		optind;
extern char *		optarg;

void
Usage(void)
{
	fprintf(stderr, "usage: guard_starter [-d] -a arch -c cbhost -x cbport -r proto [-n procs] [-h host] [-u user] [-p password] [-b bindir] [-e backend] -- prog [args] [-- arch_args]\n");
	exit(1);
}

/*ARGSUSED*/
int
main(int argc, char *argv[], char *env[])
{
	int			c;
	char *			nproc = NULL;
	char *			host = NULL;
	char *			cbhost = NULL;
	char *			cbport = NULL;
	char *			proto = NULL;
	char *			prog = NULL;
	char *			arch = NULL; 
	char *			user = NULL;
	char *			pass = NULL;
	char *			cfg_env = NULL;
	char *			bindir = BINDIR;
	char			dbgsrv[MAXPATHLEN];
	char *			backend = DBG_BACKEND;
	struct start_type *	s;

	while ( (c = getopt(argc, argv, "dc:x:h:n:a:r:u:p:b:e:v:")) != EOF )
	{
		switch ( c )
		{
		case 'd':
			Debug = 1;
			break;

		case 'e':
			backend = optarg;
			break;

		case 'h':
			host = optarg;
			break;

		case 'c':
			cbhost = optarg;
			break;

		case 'x':
			cbport = optarg;
			break;

		case 'n':
			nproc = optarg;
			break;

		case 'a':
			arch = optarg;
			break;

		case 'r':
			proto = optarg;
			break;

		case 'u':
			user = optarg;
			break;

		case 'p':
			pass = optarg;
			break;

		case 'b':
			bindir = optarg;
			break;

		case 'v':
			cfg_env = optarg;
			break;

		default: 
			Usage();
		}
	}

	if ( optind > argc - 1 )
	{
		fprintf(stderr, "No program specified.\n");
		Usage();
	}

	prog = argv[optind++];

	if ( arch == NULL )
	{
		/*
		** We allow this if a host has been specified. In this case
		** we default to using SSH if it exists. Failing that we
		** use RSH if it exists.
		*/
#if defined(SSH)
		if ( host != NULL )
			arch = "ssh";
#elif defined(RSH)
		if ( host != NULL )
			arch = "rsh";
#endif /* SSH || RSH */

		/*
		** Assume local if nproc is not specified
		*/
		else if ( nproc == NULL )
			arch = "local";
		else {
			fprintf(stderr, "An arch must be specified.\n");
			Usage();
		}
	}

	if ( Debug == 0 && cbhost == NULL )
	{
		fprintf(stderr, "A callback host must be specified.\n");
		Usage();
	}

	if ( Debug == 0 && cbport == NULL )
	{
		fprintf(stderr, "A callback port must be specified.\n");
		Usage();
	}

	if ( Debug == 0 && proto == NULL )
	{
		fprintf(stderr, "A debug protocol must be specified.\n");
		Usage();
	}

#if defined(WINDOWSNT) || defined(WIN32)
	snprintf(dbgsrv, MAXPATHLEN-1, "dbgsrv");
#else /* WINDOWSNT || WIN32 */
	snprintf(dbgsrv, MAXPATHLEN-1, "%s/%s", bindir, "dbgsrv");
#endif /* WINDOWSNT || WIN32 */

	for ( s = StartTypes ; s->arch != NULL ; s++ )
		if ( strcmp(s->arch, arch) == 0 )
			break;

	if ( s->arch == NULL )
	{
		fprintf(stderr, "No such arch \"%s\"\n", arch);
		Usage();
	}

	if ( s->mp && nproc == NULL )
	{
#ifndef ALPS
//		fprintf(stderr, "Must specify number of processes\n");
//		Usage();
		fprintf(stderr, "Defaulting to single process job\n");
		nproc = "1";
#else /* !ALPS */
		/* Cray_Inc: allow single process jobs */
		fprintf(stderr, "Defaulting to single process job\n");
		nproc = "1";
#endif /* !ALPS */
	}

	s->nproc = nproc;
	s->host = host;
	s->user = user;
	s->pass = pass;
	s->proto = proto;
	s->cbhost = cbhost;
	s->cbport = cbport;
	s->dbgsrv = dbgsrv;
	s->backend = backend;
	s->prog = prog;
	s->cfg_env = cfg_env;

	/*
		If optind < argc, we have prog (and possibly arch) arguments.
		argv contains, for example: "arg1 arg2 arg3 ..." or:
		"arg1 arg2 arg3 ... -- arch_arg1 arch_arg2 arch_arg3 ..."
		If it contains arch arguments, we check for "--" and
		put the variables in s->argv and s->arch_argv respectively.
	*/

	if ( optind < argc )
	{
		int count;
		s->argv = &argv[optind];
		s->arch_argv = NULL;

		for ( count = optind ; count < argc ; count++ )
		{
			if ( strcmp(argv[count], "--") == 0 )
			{
				argv[count] = NULL;
				s->arch_argv = &argv[count+1];
				break;
			}
		}
	}
	else
	{
		s->argv = NULL;
		s->arch_argv = NULL;
	}

	s->env = env;

	exit(s->proc(s));
}

void
DebugForkExec(char *prog, char **args, char **env, int fork)
{
	char **ap;

	if ( Debug )
	{
		fprintf(stderr, "exec %s ", prog);

		for ( ap = args ; *ap != NULL ; ap++ )
			fprintf(stderr, "%s ", *ap);

		fprintf(stderr, "\n");
	}

	if ( fork )
		ForkAndExec(prog, args, env);
	else
		Exec(prog, args, env);
}

#ifdef WIN32
void
RemoteCommand(void *args)
{
	int	n;
	SOCKET	s;
	char **	ap = (char **)args;
	char *	user;
	char *	host;
	char *	cmd;
	char	buf[BUFSIZ];

	host = ap[0];

	if ( ap[2] == NULL )
	{
		user = getlogin();
		cmd = ap[1];
	}
	else
	{
		user = ap[1];
		cmd = ap[2];
	}

	s = rcmd(&host, 514, user, user, cmd, NULL);

	if ( s == INVALID_SOCKET )
	{
		fprintf(stderr, "rcmd: wsa error %d\n", WSAGetLastError());
		CloseHandle((HANDLE)Thread);
		_endthreadex(-1);
	}

	while ( (n = recv(s, buf, sizeof(buf), 0)) > 0 ) ;

	closesocket(s);

	CloseHandle((HANDLE)Thread);
	_endthreadex(n < 0 ? -1 : 0);
}
#endif /* WIN32 */

char *
ExecuteCommand(char *cmd, char **args, char **env)
{
	int		pid;
	int		len;
	int		n;
	int		status;
	int		p[2];
	char *		s;
	static char	buf[BUFSIZ];

	buf[0] = '\0';

	if ( pipe(p) < 0 )
	{
		perror("pipe");
		return NULL;
	}

	switch ( pid = fork() )
	{
	case 0:
		(void)close(1);
		(void)close(2);
		dup(p[1]);
		dup(p[1]);
		(void)close(p[0]);
		(void)close(p[1]);
		DebugForkExec(cmd, args, env, 0);
		exit(1);

	case -1:
		perror("fork");
		(void)close(p[0]);
		(void)close(p[1]);
		return NULL;

	default:
		(void)close(p[1]);
		break;
	}

	len = BUFSIZ-1;
	s = buf;

	while ( len > 0 )
	{
		if ( (n = read(p[0], s, len))  <= 0 )
		{
			if ( n < 0 )
				perror("read");

			break;
		}

		s += n;
		len -= n;
	}

	(void)close(p[0]);

	if ( (len == 0 && n > 0) || n < 0 )
	{
		/* make sure we cleanup child */
		(void)kill(pid, SIGTERM);
	}

	wait(&status);

	if ( WIFEXITED(status) && WEXITSTATUS(status) == 0 )
		return buf;

	return NULL;
}

/*
*****************************************
***** STARTUP METHODS GO BELOW HERE *****
*****************************************
*/

#ifdef RSH
int
StartRsh(struct start_type *s)
{
#ifndef WIN32
	char **	ap;
#else
	char *	args[4];
#endif /* !WIN32 */

	if ( s->host == NULL )
	{
		fprintf(stderr, "No host specified.\n");
		exit(1);
	}

#ifndef WIN32
	if ( s->user != NULL )
		ap = NewArgv("rsh", "-n", "-l", s->user, s->host, NULL);
	else
		ap = NewArgv("rsh", "-n", s->host, NULL);

	ap = AppendArgs
	(
		ap,
		s->dbgsrv, 
		"-h", s->cbhost, 
		"-c", s->cbport, 
		"-a", "serial", 
		"-p", s->proto, 
		"-e", s->backend,
		s->prog,
		NULL
	);

	if ( s->argv != NULL )
		ap = AppendArgv(ap, &s->argv[1]);

	DebugForkExec(RSH, ap, s->env, 0);

	FreeArgv(ap);
	return 1;
#else /* !WIN32 */
	snprintf(cmd, BUFSIZ-1, "dbgsrv -h %s -c %d -a serial -p %s -e %s %s", s->cbhost, socp->soc_port, s->proto, s->backend, s->prog);

	args[0] = s->host;

	if ( s->user != NULL )
	{
		args[1] = s->user;
		args[2] = cmd;
		args[3] = NULL;
	}
	else
	{
		args[1] = cmd;
		args[2] = NULL;
	}

	Thread = _beginthread(RemoteCommand, 0, (void *)args);
	return 0;
#endif /* !WIN32 */
}
#endif /* RSH */

#ifdef SSH
int
StartSsh(struct start_type *s)
{
	char **	ap;

	if ( s->host == NULL )
	{
		fprintf(stderr, "No host specified.\n");
		exit(1);
	}

	if ( s->user != NULL )
		ap = NewArgv("ssh", "-n", "-l", s->user, s->host, NULL);
	else
		ap = NewArgv("ssh", "-n", s->host, NULL);

	ap = AppendArgs
	(
		ap,
		s->dbgsrv, 
		"-h", s->cbhost, 
		"-c", s->cbport, 
		"-a", "serial", 
		"-p", s->proto, 
		"-e", s->backend,
		s->prog,
		NULL
	);

	if ( s->argv != NULL )
		ap = AppendArgv(ap, &s->argv[1]);

	DebugForkExec(SSH, ap, s->env, 0);

	FreeArgv(ap);
	return 1;
}
#endif /* SSH */

#ifdef BPROC
int
StartBpsh(struct start_type *s)
{
	char **	ap;

	if ( s->host != NULL )
	{
		ap = NewArgv
		(
			"bpsh",
			"-n",
			s->host,
			s->dbgsrv, 
			"-h", s->cbhost, 
			"-c", s->cbport, 
			"-a", "serial", 
			"-p", s->proto, 
			"-e", s->backend,
			s->prog,
			NULL
		);
	}
	else if ( s->nproc != NULL )
	{
		/*
		** find available nodes
		** build a nodespec from available nodes s.t. #nodes == nproc
		** bpsh nodespec dbgsrv -h cbhost -c cbport -a bpsh -p proto prog args
		*/
		fprintf(stderr, "Multiprocess bpsh startup not implemented yet.\n");
		return 1;
	}
	else
	{
		fprintf(stderr, "Either host or nproc must be specified.\n");
		exit(1);
	}

	if ( s->argv != NULL )
		ap = AppendArgv(ap, &s->argv[1]);

	DebugForkExec(BPSH, ap, s->env, 0);

	FreeArgv(ap);
	return 1;
}
#endif /* BPROC */

int
StartClustor(struct start_type *s)
{
	SOCKET			sd;
	long			haddr;
	char			cmd[BUFSIZ];
	struct hostent *	hp;
	struct sockaddr_in	sin;

	if
	(
		s->user == NULL
		||
		s->pass == NULL
		||
		s->host == NULL
	)
	{
		fprintf(stderr, "Must specify host, username and password\n");
		exit(1);
	}

	if ( (hp = gethostbyname(s->host)) == (struct hostent *)NULL )
	{
		fprintf(stderr, "Unable to find host IP address\n");
		exit(1);
	}

	haddr = ((hp->h_addr[0] & 0xff) << 24) |
		((hp->h_addr[1] & 0xff) << 16) |
		((hp->h_addr[2] & 0xff) <<  8) |
		((hp->h_addr[3] & 0xff) <<  0);

	if ( (sd = socket(PF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET )
	{
		perror("Unable to open socket");
		exit(1);
	}

	memset(&sin, 0, sizeof(struct sockaddr_in));
	sin.sin_family = PF_INET;
	sin.sin_port = htons((u_short) 17000);
	sin.sin_addr.s_addr = htonl(haddr);

	if ( connect(sd, (struct sockaddr *)&sin, sizeof(struct sockaddr_in)) == SOCKET_ERROR )
	{
		perror("Unable to connect to remote host");
		CLOSE_SOCKET(sd);
		exit(1);
	}

	snprintf(cmd, BUFSIZ-1, "logon %s %s exec %%TEMP%% dbgsrv -h %s -c %s -a serial -p %s -e %s %s",
		s->user, 
		s->pass,
		s->cbhost,
		s->cbport,
		s->proto,
		s->backend,
		s->prog);

	if ( send(sd, cmd, strlen(cmd)+1, 0) == SOCKET_ERROR )
	{
		perror("Unable to send a request to remote host");
		CLOSE_SOCKET(sd);
		exit(1);
	}

	/* Strange! It seems that some delay is required, */
	/* otherwise the other side gets EOF before data!!! */
#ifndef WIN32
	sleep(1);
#else /* !WIN32 */
	Sleep(1000);
#endif /* !WIN32 */

	CLOSE_SOCKET(sd);

	return 0;
}

int
StartLocal(struct start_type *s)
{
	char **	ap;

#ifndef WIN32
	ap = NewArgv
	(
		"dbgsrv",
		"-h", s->cbhost,
		"-c", s->cbport,
		"-a", "serial",
		"-p", s->proto,
		"-e", s->backend,
		s->prog,
		NULL
	);

	if ( s->argv != NULL ) {
		// ORIGINAL: ap = AppendArgv(ap, &s->argv[1]);
		// The original is incorrect but why do we have "duplicated line"
		// for the other Start* functions? (donny)
		ap = AppendArgv(ap, &s->argv[0]);
	}

	DebugForkExec(s->dbgsrv, ap, s->env, 0);

	FreeArgv(ap);
	return 1;
#else /* !WIN32 */
	fprintf(stderr, "local debug server not supported.\n");
	return 1;
#endif /* !WIN32 */
}

#ifdef AIX_POE
int
StartPOE(struct start_type *s)
{
	char **	ap;
	char	np[10];

	sprintf(np, "%d", atoi(s->nproc)-1);

	ap = NewArgv
	(
		"poe",
		"dbgsrv",
		"-h", s->cbhost,
		"-c", s->cbport,
		"-a", s->arch,
		"-p", s->proto,
		"-e", s->backend,
		"--",
		s->prog,
		NULL
	);

	if ( s->argv != NULL )
		ap = AppendArgv(ap, &s->argv[1]);

	DebugForkExec(POE, ap, s->env, 0);

	FreeArgv(ap);
	return 1;
}
#endif /* AIX_POE */

#ifdef MPICH
char **
ParseProcgroup(int nproc, char *str)
{
	int	i;
	char *	s;
	char **	h;

	/*
	** first line should be 'Procgroup file:'
	*/

	if ( strncmp(str, "Procgroup file:", 15) != 0 )
		return NULL;

	if ( (s = strchr(str, '\n')) == NULL )
		return NULL;

	str = ++s;

	if ( (h = (char **)Alloc(sizeof(char *) * nproc + 1)) == NULL )
		return NULL;

	memset(h, 0, sizeof(char *) * nproc + 1);

	/*
	** there should be nproc host lines
	*/
	for ( i = 0 ; i < nproc ; i++ )
	{
		/*
		** just interested in host name
		*/

		if ( (s = strchr(str, ' ')) == NULL )
			goto err_out;

		*s++ = '\0';

		h[i] = strdup(str);

		/*
		** skip rest of line
		*/

		if ( (s = strchr(s, '\n')) == NULL )
			goto err_out;

		str = ++s;
	}

	return h;

err_out:
	i = 0;
	while ( h[i] != NULL )
		Free(h[i++]);

	Free(h);
	return NULL;
}

int
StartMPICH(struct start_type *s)
{
	int	i;
	int	fd;
	int	nproc;
	int	len;
	char *	ri;
	char *	res;
	char **	hosts;
	char **	ap;
	char	rinfo[BUFSIZ];
	char	hostname[MAXHOSTNAMELEN];
	char	pid[BUFSIZ];

#ifndef WIN32
	if ( strcmp(s->arch, "mpich") == 0 )
	{
		if(s->cfg_env == NULL)
		ap = NewArgv
		(
			"mpirun",
			"-np", s->nproc,
			s->dbgsrv,
			"-h", s->cbhost,
			"-c", s->cbport,
			"-a", s->arch,
			"-p", s->proto,
			"-e", s->backend,
			"--",
			s->prog,
			NULL
		);
		else
		ap = NewArgv
		(
			"mpirun",
			"-np", s->nproc,
			s->dbgsrv,
			"-h", s->cbhost,
			"-c", s->cbport,
			"-a", s->arch,
			"-p", s->proto,
			"-e", s->backend,
			"-v", s->cfg_env,
			"--",
			s->prog,
			NULL
		);

		if ( s->argv != NULL )
			ap = AppendArgv(ap, &s->argv[1]);

		DebugForkExec(MPIRUN, ap, s->env, 0);

		FreeArgv(ap);
		return 1;
		/*
		** Get name of this host. This is wrong - what happens if remote
		** hosts don't know us by this name?
		*/
		gethostname(hostname, MAXHOSTNAMELEN-1);

		/*
		** MPIRUN -t should return a list of hosts that will be used to
		** run the MPI job.
		*/
		ap = NewArgv
		(
			MPIRUN,
			"-np", s->nproc,
			"-t",
			s->dbgsrv,
			NULL
		);

		if ( (res = ExecuteCommand(MPIRUN, ap, s->env)) == NULL )
		{
			fprintf(stderr, "%s: failed to execute\n", MPIRUN);
			return 1;
		}

		FreeArgv(ap);

		nproc = atoi(s->nproc);

		if ( (hosts = ParseProcgroup(nproc, res)) == NULL )
		{
			fprintf(stderr, "Bad output from %s\n", MPIRUN);
			return 1;
		}

		/*
		** rinfo="$HOST 0 1 $DBGSRV" !execer code is wrong!
		*/

		ri = rinfo;
		len = BUFSIZ-1;

		for ( i = 1 ; i < nproc ; i++ )
		{
			if ( Debug == 0 )
				snprintf(ri, len, "%s %d 1 %s ", hosts[i], i, s->dbgsrv);
			else
				snprintf(ri, len, "%s %d 1 %s ", hosts[i], i, s->prog);

			ri += strlen(ri);
			len -= strlen(ri);
		}

		snprintf(pid, BUFSIZ-1, "%d", getpid());

		if ( s->host != NULL )
		{
			if ( s->user != NULL )
				ap = NewArgv("rsh", "-n", "-l", s->user, s->host, NULL);
			else
				ap = NewArgv("rsh", "-n", s->host, NULL);
		}
		else
			ap = NewArgv(NULL);

		if ( Debug == 0 )
			ap = AppendArgs
			(
				ap,
				"dbgsrv",
				"-h", s->cbhost,
				"-c", s->cbport,
				"-a", s->arch,
				"-p", s->proto,
				"-e", s->backend,
				"--",
				NULL
			);

		ap = AppendArgs
		(
			ap,
			s->prog,
			NULL
		);

		if ( s->argv != NULL )
			ap = AppendArgv(ap, s->argv);

		ap = AppendArgs
		(
			ap,
			"-execer_id", "DBG",
			"-master_host", hostname,
			"-my_hostname", hostname,
			"-my_nodenum", "0",
			"-my_numprocs", "1",
			"-total_numnodes", s->nproc,
			"-job_id", pid,
			"-remote_info", rinfo,
			NULL
		);

		if ( s->host != NULL )
			DebugForkExec(RSH, ap, s->env, 1);
		else
		{
			if ( Debug == 0 )
				DebugForkExec(s->dbgsrv, ap, s->env, 1);
			else
				DebugForkExec(s->prog, ap, s->env, 1);
		}

		FreeArgv(ap);

		for ( i = 1 ; i < nproc; i++ )
		{
			snprintf(rinfo, BUFSIZ-1, "%d", i);

			ap = NewArgv("rsh", "-n", hosts[i], NULL);

			if ( Debug == 0 )
				ap = AppendArgs
				(
					ap,
					s->dbgsrv,
					"-h", s->cbhost,
					"-c", s->cbport,
					"-a", s->arch,
					"-p", s->proto,
					"-e", s->backend,
					"--",
					NULL
				);
				

			ap = AppendArgs
			(
				ap,
				s->prog,
				"-execer_id", "DBG",
				"-master_host", hostname,
				"-my_hostname", hosts[i],
				"-my_nodenum", rinfo,
				"-my_numprocs", "1",
				"-total_numnodes", s->nproc,
				"-job_id", pid,
				NULL
			);

			DebugForkExec(RSH, ap, s->env, 1);

			FreeArgv(ap);
		}

		return 0;
	}
#ifdef MPICH_MPD
	else if ( strcmp(s->arch, "mpich-mpd") == 0 )
	{
		/*
		** Check that mpd's are running.
		*/
		ap = NewArgv
		(
			MPDTRACE,
			NULL
		);

		if ( (res = ExecuteCommand(MPDTRACE, ap, s->env)) == NULL )
		{
			fprintf(stderr, "Please start mpd's first\n");
			return 1;
		}

		FreeArgv(ap);

		if(s->cfg_env == NULL)
		ap = NewArgv
		(
			"mpirun",
			"-np", s->nproc,
			"-s",
			s->dbgsrv,
			"-h", s->cbhost,
			"-c", s->cbport,
			"-a", s->arch,
			"-p", s->proto,
			"-e", s->backend,
			"--",
			s->prog,
			NULL
		);
		else
		ap = NewArgv
		(
			"mpirun",
			"-np", s->nproc,
			"-s",
			s->dbgsrv,
			"-h", s->cbhost,
			"-c", s->cbport,
			"-a", s->arch,
			"-p", s->proto,
			"-e", s->backend,
			"-v", s->cfg_env,
			"--",
			s->prog,
			NULL
		);

		if ( s->argv != NULL )
			ap = AppendArgv(ap, &s->argv[1]);

		DebugForkExec(MPIRUN, ap, s->env, 0);

		FreeArgv(ap);
		return 1;
	}
#endif /* MPICH_MPD */

#ifdef BPROC
	/*
	** The bproc mpirun steals stdin, so give it something
	** that doesn't matter.
	*/
	fd = open("/dev/null", O_RDONLY);
	if ( fd >= 0 )
	{
		close(0);
		dup(fd);
	}

	if ( strcmp(s->arch, "mpich-gm") == 0 )
	{
		ap = NewArgv
		(
			"mpirun",
			"--np", s->nproc,
			"--gm",
			s->dbgsrv,
			"-h", s->cbhost,
			"-c", s->cbport,
			"-a", s->arch,
			"-p", s->proto,
			"-e", s->backend,
			"--",
			s->prog,
			NULL
		);

		if ( s->argv != NULL )
			ap = AppendArgv(ap, &s->argv[1]);

		DebugForkExec(MPIRUN, ap, s->env, 0);

		FreeArgv(ap);
		return 1;
	}

	if ( strcmp(s->arch, "mpich-p4") == 0 )
	{
		ap = NewArgv
		(
			"mpirun",
			"--np", s->nproc,
			"--p4",
			s->dbgsrv,
			"-h", s->cbhost,
			"-c", s->cbport,
			"-a", s->arch,
			"-p", s->proto,
			"-e", s->backend,
			"--",
			s->prog,
			NULL
		);

		if ( s->argv != NULL )
			ap = AppendArgv(ap, &s->argv[1]);

		DebugForkExec(MPIRUN, ap, s->env, 0);

		FreeArgv(ap);
		return 1;
	}
#endif /* BPROC */

	fprintf(stderr, "Unknown architecture \"%s\"\n", s->arch);
	return 1;
#else /* !WIN32 */
	if ( s->host == NULL )
	{
		fprintf(stderr, "local parallel startup not supported.\n");
		return 1;
	}

	snprintf(cmd, BUFSIZ-1, 
		"mpirun -np %s -dbg=guard %s --guard_cb_host %s --guard_cb_port %s --guard_arch %s --guard_proto %s",
		s->nproc,
		s->prog,
		s->cbhost,
		s->cbport,
		"mpich",
		s->proto);

	args[0] = s->host;

	if ( s->user != NULL )
	{
		args[1] = s->user;
		args[2] = cmd;
		args[3] = NULL;
	}
	else
	{
		args[1] = cmd;
		args[2] = NULL;
	}

	Thread = _beginthread(RemoteCommand, 0, (void *)args);
	return 0;
#endif /* !WIN32 */
}
#endif /* MPICH */

#ifdef P4
int
StartP4(struct start_type *s)
{
	char **	ap;
	char	np[10];

	/*
	** Master is automatically started, so we don't need to count it.
	*/
	sprintf(np, "%d", atoi(s->nproc)-1);

	ap = NewArgv
	(
		"dbgsrv",
		"-h", s->cbhost,
		"-c", s->cbport,
		"-a", s->arch,
		"-p", s->proto,
		"-e", s->backend,
		"--",
		s->prog,
		np,
		NULL
	);

	if ( s->argv != NULL )
		ap = AppendArgv(ap, &s->argv[1]);

	DebugForkExec(s->dbgsrv, ap, s->env, 0);

	FreeArgv(ap);
	return 1;
}
#endif /* P4 */

#ifdef OMPI
int
StartOMPI(struct start_type *s)
{
	char **	ap;

	ap = NewArgv
	(
		"mpirun",
		"-np", s->nproc,
		s->dbgsrv,
		"-h", s->cbhost,
		"-c", s->cbport,
		"-a", s->arch,
		"-p", s->proto,
		"-e", s->backend,
		"--",
		s->prog,
		NULL
	);

	if ( s->argv != NULL )
		ap = AppendArgv(ap, &s->argv[1]);

	DebugForkExec(MPIRUN, ap, s->env, 0);

	FreeArgv(ap);
	return 1;
}
#endif /* OMPI */
#ifdef ALPS
int
StartAlps(struct start_type *s)
{
        char ** ap;
        char    np[16];

        /* Given that only /bin/sh exists on CNL, gdb will fail to start 
	 * if SHELL is set to anything else, therefore unset it
	 */
        unsetenv("SHELL");

        sprintf(np, "%d", atoi(s->nproc));

	/*
	   Build call line for guard_starter_alps. It will
	   need to deliver everything that dbgsvr_alps needs
	   (which includes what dbgsvr needs) as well as
	   everything that aprun needs:

	      guard-starter_alps <np> <all of dbgsvr> s->prog s->argv 
	        aprun -n <np>, s->arch_argv, s->prog (i.e a.out), s->argv
                dbgsvr_alps <apid> <all of dbgsvr>
	          dbgsvr "-h", s->cbhost, "-c", s->cbport, "-a", s->arch, 
	                                     "-p", s->proto, "-e", s->backend
	  */
        ap = NewArgv
        (
		"guard_starter_alps",
                np,
                s->cbhost,
                s->cbport,
                s->arch,
                s->proto,
                s->backend,
                s->prog,
                NULL
        );

        // Append the launcher args and count
        int numArgs = 0;
        char **p;
        char sNumArgs[10];
        if ( s->arch_argv ) {
            for (p = s->arch_argv; *p != NULL; ++p) // count them
                ++numArgs;
        }
        sprintf(sNumArgs, "%d", numArgs);
        ap = AppendArgs(ap, sNumArgs, NULL); // append the count

        if ( s->arch_argv != NULL ) {
            DPRINT("guard_starter: Yes, there is an arch_argv\n");
            ap = AppendArgv(ap, &s->arch_argv[0]); // append the arch_args
        }

        // Append the target prog args
        if ( s->argv != NULL ) {
                DPRINT("guard_starter: Yes, there is an argv\n");
                ap = AppendArgv(ap, &s->argv[0]);
        }

#ifdef DEBUG
        fprintf(stderr, "\n%s: guard_starter_alps command: ", "guard_starter");
        p = ap;
        while (*p) {
                fprintf(stderr, "%s ", *p);
                ++p;
        }
        fprintf(stderr, "\n");
#endif

	// exec into guard_starter_alps (must be in PATH)
        DebugForkExec("guard_starter_alps", ap, s->env, 0);

        return 1; // the exec won't actually be returning...
}
#endif /* ALPS */
