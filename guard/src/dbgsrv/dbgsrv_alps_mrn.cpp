/*********************************************************************************\
 * dbgsrv_alps.c - A wrapper program to launch guard dbgsrv's onto the compute
 *		 nodes.
 *
 * © 2011-2015 Cray Inc.	All Rights Reserved.
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

#include <getopt.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "defs.h"
#include "useful.h"
#include "cray_tools_be.h"

#define DBGSRV						"dbgsrv"
#define PID_BUFSIZ				64

#define DBGSRV_OPT_PID			"-i"
#define DBGSRV_OPT_HOST			"-h"
#define DBGSRV_OPT_PORT			"-c"
#define DBGSRV_OPT_ARCH			"-a"
#define DBGSRV_OPT_PROTO			"-p"
#define DBGSRV_OPT_DBG			"-e"
#define DBGSRV_OPT_PROC			"-n"
#define DBGSRV_OPT_GPU			"--gpu"
#define DBGSRV_OPT_DEBUG			"--debug"
#define DBGSRV_OPT_EARLY_ENTRY	"--early-entry"

#define UDASRV						"udaserver"
#define UDASRV_ARGC				2

#define UDASRV_ARGV_NAME			0
#define UDASRV_ARGV_NULL			1

#define UDASRV_ENV_SERVICE		"UDA_SERVICE"
#define UDASRV_ENV_PLUGIN		"UDA_PLUGIN_LIBRARY"

#define UDA_SERVICE				"/uda_test"
#define UDA_PLUGIN				"uda-plugin-cray-upc.so"

static int debug_flag = 0;
static int gpu_flag = 0;
static int early_entry_flag = 0;

const struct option long_opts[] = {
				{"callbackHost",	required_argument,	0, 'h'},
				{"callbackPort",	required_argument,	0, 'c'},
				{"networkArch",		required_argument,	0, 'a'},
				{"networkProtocol",	required_argument,	0, 'p'},
				{"dbgBackend",		required_argument,	0, 'e'},
				{"help",			no_argument,		0, 'x'},
				{"gpu",				no_argument,		&gpu_flag, 1},
				{"debug",			no_argument,		&debug_flag, 1},
				{"early-entry",			no_argument,		&early_entry_flag, 1},
				{0, 0, 0, 0}
				};

void
DVPRINT(char *format, ...)
{
	va_list	 arg_list;
	
	if (!debug_flag)
		return;
	
	// setup the va_list
	va_start(arg_list, format);
	
	// vfprintf the line we recieved.
	vfprintf(stderr, format, arg_list);
	
	// end the va_list
	va_end(arg_list);
}

// Need to define this for any library that is shared between the client and
// server components. We need to be able to hook into the CLI stream in the
// client, so just define this to stdout/stderr in the server.
FILE *	out_stream;
FILE *	err_stream;

void
usage(char *name)
{
	fprintf(stdout, "Usage: %s [OPTIONS]...\n", name);
	fprintf(stdout, "Launch guard debug servers on compute nodes.\n");
	fprintf(stdout, "\t-h, --callbackHost     Front end hostname\n");
	fprintf(stdout, "\t-c, --callbackPort     Front end callback port\n");
	fprintf(stdout, "\t-a, --networkArch      Network architecture\n");
	fprintf(stdout, "\t-p, --networkProtocol  Network protocol\n");
	fprintf(stdout, "\t-e, --dbgBackend       Debugging program backend\n");
	fprintf(stdout, "\t    --gpu              Turn on gpu mode.\n");
	fprintf(stdout, "\t    --early-entry      Turn on early entry mode.\n");
	fprintf(stdout, "\t    --debug            Turn on debug logging.\n");
	fprintf(stdout, "\t-x, --help             Display this text and exit\n");
}

int
main(int argc, char **argv)
{
	// dbgsrv arguments
	int			opt_ind = 0;
	int			c;
	char *		callbackHost = NULL;
	char *		callbackPort = NULL;
	char *		networkArch = NULL;
	char *		networkProto = NULL;
	char *		dbgBackend = NULL;
	char *		prog = NULL;
	char		pidbuff[PID_BUFSIZ];
	// node specific info
	cti_pidList_t *	myPids;
	// dbgsrv info
	int			i;
	char **		dbgsrv_argv;
	char **		this_argv;
	char **		tmp;
	pid_t		mypid;
	// debug related variables
	char * 		nidName = NULL;
	FILE *		log = NULL;
#if 1
// TODO: UPC addition here
	// udaserver info
	//char **		udasrv_argv;
	char *		scratch;
	char *		uda_str;
	int			len;
// End UPC addition
#endif

#ifdef DEBUG	
	debug_flag = 1;	
#endif

	// Set the streams
	out_stream = stdout;
	err_stream = stderr;
	
	if (debug_flag)
	{
		// print out the argv
		DVPRINT("dbgsrv_alps argv: \n");
		DVPRINT("dbgsrv_alps ");
		tmp = argv;
		while (*tmp != NULL)
		{
			DVPRINT("%s ", *tmp++);
		}
		DVPRINT("\n");
	}
	
	DVPRINT("Parsing longopts...\n");
	
	while ((c = getopt_long(argc, argv, "h:c:a:p:e:x", long_opts, &opt_ind)) != -1)
	{
		switch (c)
		{
			case 0:
				// if this is a flag, do nothing
				break;
			case 'h':
				if (optarg == NULL || *optarg == '\0')
				{
					fprintf(stderr, "Invalid callback hostname.\n");
					return 1;
				}
				callbackHost = strdup(optarg);
				DVPRINT("callbackHost: %s\n", callbackHost);
				break;
				
			case 'c':
				if (optarg == NULL || *optarg == '\0')
				{
					fprintf(stderr, "Invalid callback port number.\n");
					return 1;
				}
				callbackPort = strdup(optarg);
				DVPRINT("callbackPort: %s\n", callbackPort);
				break;
				
			case 'a':
				if (optarg == NULL || *optarg == '\0')
				{
					fprintf(stderr, "Invalid network architecture.\n");
					return 1;
				}
				networkArch = strdup(optarg);
				DVPRINT("networkArch: %s\n", networkArch);
				break;
				
			case 'p':
				if (optarg == NULL || *optarg == '\0')
				{
					fprintf(stderr, "Invalid network protocol.\n");
					return 1;
				}
				networkProto = strdup(optarg);
				DVPRINT("networkProtocol: %s\n", networkProto);
				break;
				
			case 'e':
				if (optarg == NULL || *optarg == '\0')
				{
					fprintf(stderr, "Invalid debug backend.\n");
					return 1;
				}
				dbgBackend = strdup(optarg);
				DVPRINT("dbgBackend: %s\n", dbgBackend);
				break;
				
			case 'x':
				usage(argv[0]);
				return 1;
				
			default:
				usage(argv[0]);
				return 1;
		}
	}
	
	DVPRINT("Done parsing longopts.\n");
	
	// get the final optional program argument
	if (optind < argc)
	{
		prog = strdup(argv[optind++]);
		DVPRINT("program name: %s\n", prog);
	}
	
	if (debug_flag)
	{
		// get this nodes nidName
		if ((nidName = cti_be_getNodeHostname()) == NULL)
		{
			fprintf(stderr, "cti_be_getNodeHostname failed.\n");
			return 1;
		}
		DVPRINT("My nidName: %s\n", nidName);
	}

#define CRAY_CCE_UPC 1
#ifdef CRAY_CCE_UPC
	// This only works with non-gpu mode
	if (!gpu_flag)
	{
		// Tell gdb where to find the UPC plugin 
		scratch = getenv("CRAYTOOL_LIB_DIR");
		if (!scratch)
		{
		    fprintf(stderr, "CRAYTOOL_LIB_DIR getenv failed.\n");
		    return 1;
		}
		    
		len = strlen(scratch) + strlen(UDA_PLUGIN) + 2;
		if ((uda_str = (char*)malloc(len)) == (void *)0)
		{
		    fprintf(stderr, "UDA_PLUGIN malloc failed.\n");
		    return 1;
		}
		snprintf(uda_str, len, "%s/%s", scratch, UDA_PLUGIN);

		if (setenv("UDA_GUPC_PLUGIN_LIBRARY", uda_str, 1) < 0)
		{
			// failure
			fprintf(stderr, "UDA_GUPC_PLUGIN_LIBRARY setenv failed");
			return 1;
		}
		free(uda_str);
	}
#endif
	
#if 0
// TODO: UPC addition here

	// calloc memory for the dbgsrv argv array
	if ((udasrv_argv = calloc(UDASRV_ARGC, sizeof(char *))) == (void *)0)
	{
		fprintf(stderr, "calloc failed.\n");
		return 1;
	}
	
	// set the static members of dbgsrv_argv
	udasrv_argv[UDASRV_ARGV_NAME] = strdup(UDASRV);
	udasrv_argv[UDASRV_ARGV_NULL] = NULL;
	
	// get the value of TMPDIR, this should be set by dlaunch
	scratch = getenv("TMPDIR");
	if (!scratch)
		scratch = "/tmp";
		
	len = strlen(scratch) + strlen(UDA_SERVICE) + 1;
	
	if ((uda_str = malloc(len)) == (void *)0)
	{
		fprintf(stderr, "malloc failed.\n");
		return 1;
	}
	
	snprintf(uda_str, len, "%s%s", scratch, UDA_SERVICE);
	
	// set the actual environment variable
	if (setenv(UDASRV_ENV_SERVICE, uda_str, 1) < 0)
	{
		// failure
		fprintf(stderr, "setenv failed");
		return 1;
	}
	
	// set the actual environment variable
	if (setenv(UDASRV_ENV_PLUGIN, UDA_PLUGIN, 1) < 0)
	{
		// failure
		fprintf(stderr, "setenv failed");
		return 1;
	}
	
	free(uda_str);
	
#ifdef DEBUG
	FILE *log = NULL;
	// create a log if debug is turned on
	if ((log = create_log(0, "udaserver")) == NULL)
	{
		fprintf(stderr, "Could not create udasrv logfile.\n");
	}
#endif

	// fork and exec
	mypid = fork();
		
	// error case
	if (mypid < 0)
	{
		fprintf(stderr, "Fatal fork error.\n");
		return 1;
	}
		
	// child case
	if (mypid == 0)
	{
#ifdef DEBUG
		// if debug is on, hook into the logfile
		if (log != NULL)
			hook_stdoe(log);
#endif
		// exec dbgsrv
		execvp(UDASRV, udasrv_argv);
		
		// exec shouldn't return
		fprintf(stderr, "Return from exec.\n");
		exit(1);
	}

// End UPC addition
#endif
	
	// get the app pids for this node
	if ((myPids = cti_be_findAppPids()) == NULL)
	{
		fprintf(stderr, "findAppPids failed.\n");
		return 1;
	}
	DVPRINT("Number of app pid_t's detected on this node: %d\n", myPids->numPids);
	
	// set the static members of dbgsrv_argv
	dbgsrv_argv = NewArgv(	DBGSRV, DBGSRV_OPT_HOST, callbackHost, DBGSRV_OPT_PORT, callbackPort, 
							DBGSRV_OPT_ARCH, networkArch, DBGSRV_OPT_PROTO, networkProto,
							DBGSRV_OPT_DBG, dbgBackend, NULL);
							
	if (gpu_flag)
	{
		dbgsrv_argv = AppendArgs(dbgsrv_argv, DBGSRV_OPT_GPU, NULL);
	}
	
	if (debug_flag)
	{
		dbgsrv_argv = AppendArgs(dbgsrv_argv, DBGSRV_OPT_DEBUG, NULL);
	}
	
	if (early_entry_flag)
	{
		dbgsrv_argv = AppendArgs(dbgsrv_argv, DBGSRV_OPT_EARLY_ENTRY, NULL);
	}
	
	if (debug_flag)
	{
		DVPRINT("dbgsrv argv is: \n");
		tmp = dbgsrv_argv;
		while (*tmp != NULL)
		{
			DVPRINT("%s ", *tmp++);
		}
		DVPRINT("\n");
	}
	
	// start numPids dbgsrv instances.
	for (i=0; i < myPids->numPids; ++i)
	{
		this_argv = CopyArgv(dbgsrv_argv);
		// write this instances rank into the pidbuff and create the str
		snprintf(pidbuff, PID_BUFSIZ, "%d", myPids->pids[i].rank);
		this_argv = AppendArgs(this_argv, DBGSRV_OPT_PROC, pidbuff, NULL);
		DVPRINT("app rank: %s\n", pidbuff);
		memset(pidbuff, 0, PID_BUFSIZ);
		
		// write this instances pid into the pidbuff and create the str
		snprintf(pidbuff, PID_BUFSIZ, "%d", myPids->pids[i].pid);
		this_argv = AppendArgs(this_argv, DBGSRV_OPT_PID, pidbuff, NULL);
		DVPRINT("app pid: %s\n", pidbuff);
		memset(pidbuff, 0, PID_BUFSIZ);
		
		if (debug_flag)
		{
			DVPRINT("Starting dbgsrv with argv: \n");
			tmp = this_argv;
			while (*tmp != NULL)
			{
				DVPRINT("%s ", *tmp++);
			}
			DVPRINT("\n");
		}
		
		// append the prog, this is always last
		if (prog != NULL)
		{
			this_argv = AppendArgs(this_argv, prog, NULL);
		}
		
		if (debug_flag)
		{
			log = NULL;
			// create a log if debug is turned on
			if ((log = create_log(myPids->pids[i].rank, nidName)) == NULL)
			{
				fprintf(stderr, "Could not create dbgsrv logfile.\n");
			}
		}

#ifdef BE_VALGRIND
		FILE *valgrind_log = NULL;
		char valgrind_log_str[BUFSIZ];
		char **valgrind_argv;
			
		// create a log if valgrind is turned on
		if ((valgrind_log = create_log(i, "valgrind")) == NULL)
		{
			fprintf(stderr, "Could not create valgrind logfile.\n");
		}
		snprintf(valgrind_log_str, BUFSIZ, "--log-fd=%d", fileno(valgrind_log));
		
		// create an argv for valgrind
		valgrind_argv = NewArgv("valgrind", "--read-var-info=yes", "--track-origins=yes", "--leak-check=full", valgrind_log_str, NULL);
		valgrind_argv = AppendArgv(valgrind_argv, this_argv);
#endif
		
		// fork and exec -- the final time through we don't want to fork since we only
		// want numPids processes to exist.
		if (i < (myPids->numPids - 1))
		{
			mypid = fork();
		} else
		{
			DVPRINT("Done forking dbgsrv...\n");
			DVPRINT("%d dbgsrv's forked in total.\n", i);
			DVPRINT("%d dbgsrv's launched in total.\n", i+1);
			mypid = 1;
		}
		
		// error case
		if (mypid < 0)
		{
			fprintf(stderr, "Fatal fork error.\n");
			// cleanup this_argv array
			FreeArgv(this_argv);
			
			// cleanup pid list
			cti_be_destroyPidList(myPids);

			if (debug_flag)
			{
				// close logfile
				close_log(log);
				// cleanup nidName
				free(nidName);
			}
#ifdef BE_VALGRIND
			// close valgrind logfile
			close_log(valgrind_log);
			// cleanup argv
			FreeArgv(valgrind_argv);
#endif
			
			return 1;
		}
		
		// child case - We want the child to be responsible for starting the n
		// processes. Otherwise WLM's such as slurm will fail since the WLM sees
		// the parent exit and assumes the srun is complete.
		if (mypid == 0)
		{
			FreeArgv(this_argv);
			continue;
		}
		
		// parent case, or final time through
		
		if (debug_flag)
		{
			// if debug is on, hook into the logfile
			if (log != NULL)
			{
				hook_stdoe(log);
			} else
			{
				DVPRINT("Could not call hook_stdoe\n");
			}
			DVPRINT("Exec'ing dbgsrv...\n");
		}
#ifdef BE_VALGRIND	
		// exec valgrind
		execvp("valgrind", valgrind_argv);
#else
		// exec dbgsrv
		execvp(DBGSRV, this_argv);
#endif
			
		// exec shouldn't return
		fprintf(stderr, "Return from exec.\n");
		exit(1);
	}
	
	// shouldn't get here
	fprintf(stderr, "dbgsrv_alps_mrn: At impossible part of code!\n");
	
	// cleanup static argv
	FreeArgv(dbgsrv_argv);
	
	// cleanup strdup'ed stuff
	Free(callbackHost);
	Free(callbackPort);
	Free(networkArch);
	Free(networkProto);
	Free(dbgBackend);
	if (prog != NULL)
	{
		Free(prog);
	}
	
	// cleanup pid list
	cti_be_destroyPidList(myPids);
	
	return 1;
}

