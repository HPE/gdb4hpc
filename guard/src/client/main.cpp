/*********************************************************************************\
 * Guard debugger
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * © 2010-2015 Cray Inc.  All Rights Reserved.
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
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <sys/types.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <pwd.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <setjmp.h>
#include <unistd.h>
#include <math.h>

#include "client.h"
#include "useful.h"
#if defined (RD)
#include "df.h"
#endif /* RD */

#ifdef ALPS
#include "cray_tools_fe.h"
#endif /* ALPS */

/*
 * External Global Variables
 */
extern char *	optarg;
extern int		optind;

/*
 * Global Variables
 */
clnt_settings_t	Client;				// global settings store - initialized in main

// static globals
static jmp_buf	Top;				// Where to go when restoring upon signal
static int 		debug_flag;			// Debug flag - TODO: not used yet
static int		started;			// Is it alright to longjmp?

/*
 * Static Function Prototypes
 */
static void		Banner(void);
static void		SigHandler(int);

/*
 * long opts
 */
static const struct option long_opts[] = {
			{"batch",		required_argument,	0, 'b'},
			{"help",		no_argument,		0, 'h'},
			{"interface",	required_argument,	0, 'i'},
			{"output",		required_argument,	0, 'o'},
			{"silent",		no_argument,		0, 's'},
			{"version",		no_argument,		0, 'v'},
			{"xml",			no_argument,		&(Client.XML_mode), 1},
			{"debug",		no_argument,		&debug_flag, 1},
			{0, 0, 0, 0}
			};
			
static void
usage(char *name)
{
	PrintOutMsg("Usage: %s [options]\n\n", name);
	
	PrintOutMsg("Options:\n\n");
	
	PrintOutMsg("  --batch=FILE, -b     Execute debugger commands from FILE and exit\n");
	PrintOutMsg("  --help, -h           Display this text\n");
	PrintOutMsg("  --output=FILE, -o    Redirect stdout/stderr to FILE. Must be used with --batch.\n");
	PrintOutMsg("  --silent, -s         Suppress version banner on startup\n");
	PrintOutMsg("  --version, -v        Display version\n");
	
	PrintOutMsg("\nFor more information, type \"help\" inside gdb4hpc, or consult the\n");
	PrintOutMsg("gdb4hpc manpage. (man gdb4hpc)\n");
}

static void
Banner(void)
{
#ifdef ALPS
	PrintOutMsg("gdb4hpc 3.0 - Cray Line Mode Parallel Debugger\n");
	PrintOutMsg("With Cray Comparative Debugging Technology.\n");
	PrintOutMsg("Copyright 2007-2018 Cray Inc. All Rights Reserved.\n");
#else		
	PrintOutMsg("GUARD Parallel Relative Debugger\n");
#endif /* ALPS */
	PrintOutMsg("Copyright 1996-2016 University of Queensland. All Rights Reserved.\n");
	PrintOutMsg("\n");
	PrintOutMsg("Type \"help\" for a list of commands.\n"); 
	PrintOutMsg("Type \"help <cmd>\" for detailed help about a command.\n");
}

static int
checkDirPerms(const char *dir)
{
	struct stat		st;
	
	if (dir == NULL)
		return -1;
	
	// Stat the tmp_dir
	if (stat(dir, &st))
	{
		// could not stat the directory
		return -1;
	}
	
	// check to see if the user id is the same as this directory
	if (st.st_uid == getuid())
	{
		// Check the user perms
		return ((st.st_mode & S_IRWXU) ? 0:-1);
	}
	
	// check to see if the group id is the same as the directory
	if (st.st_gid == getgid())
	{
		// Check the group perms
		return ((st.st_mode & S_IRWXG) ? 0:-1);
	}
	
	// Check the other perms
	return ((st.st_mode & S_IRWXO) ? 0:-1);
}

static void
setupCfgDir(const char *dir)
{
	struct stat		st;
	
	if (dir == NULL)
		return;
		
	// try to stat the directory
	if (stat(dir, &st))
	{
		// the directory doesn't exist so we need to create it
		// use perms 755
		if (mkdir(dir, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH))
		{
			PrintOutError("Could not create config directory %s.\n", dir);
			PrintOutError("Try setting env var %s and try again. Aborting...\n", GUARD_CFG_DIRECTORY);
			exit(1);
		}
	} else
	{
		// chmod the existing directory to ensure there isn't any funny business
		// going on
		if (chmod(dir, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH))
		{
			PrintOutError("Could not chmod config directory %s.\n", dir);
			PrintOutError("Try setting env var %s and try again. Aborting...\n", GUARD_CFG_DIRECTORY);
			exit(1);
		}
	}
}

// All environment setup should happen here
// XXX: This function and it's dependencies should be synced with the stuff in
// the proxy_daemon.c source. If you change it here, change it there also.
static void
SetupEnvironment(void)
{
	char *				inst_base;
	char *				lib_dir;
	char *				libexec_dir;
	char *				cfg_dir;
	char *				env_lib_dir;
	char *				gdb_dir;
	
	// Adjust the environment to add our install base to LD_LIBRARY_PATH and PATH
	// of our process
	if ((inst_base = getinstallbase()) == NULL)
	{
		PrintOutError("Could not locate install base. Aborting...\n");
		exit(1);
	}
	// create the path strings
	Asprintf(&lib_dir, "%s/lib", inst_base);
	Asprintf(&libexec_dir, "%s/libexec", inst_base);
	
	// Set the environment variables
	adjustLibPath(lib_dir);
	adjustExecPath(libexec_dir);
	// Ensure GDB4HPC_LIB_DIR is set, otherwise set it to lib_dir
	if ((env_lib_dir = getenv(GUARD_LIB_DIRECTORY)) == NULL)
	{
		// Not found, we should set this to our lib_dir
		setenv(GUARD_LIB_DIRECTORY, lib_dir, 1);
	}
	// Ensure GUARD_INST_DIRECTORY is set, otherwise set it to inst_base
	if ((env_lib_dir = getenv(GUARD_INST_DIRECTORY)) == NULL)
	{
		// Not found, we should set this to our lib_dir
		setenv(GUARD_INST_DIRECTORY, inst_base, 1);
	}
	
	
	
	// get the cfg dir settings
	if ((cfg_dir = getenv(GUARD_CFG_DIRECTORY)) == NULL)
	{
		// Not found.
		// We now want to use $HOME since this will contain guard only files
		// like editline history files.
		char *home_dir;
				
		if ((home_dir = getenv("HOME")) == NULL)
		{
			PrintOutError("Could not read $HOME. Aborting...\n");
			exit(1);
		}
		
		// Check if we can write to HOME
		if (checkDirPerms(home_dir))
		{
			// We have no where to create a temporary directory...
			PrintOutError("No write permission for $HOME. Aborting...\n");
			exit(1);
		}
				
		// Create the directory name string
		Asprintf(&cfg_dir, "%s/.gdb4hpc", home_dir);
				
		// Setup/create this directory
		setupCfgDir(cfg_dir);
		setenv(GUARD_CFG_DIRECTORY, cfg_dir, 1);
		Free(cfg_dir);
	} else
	{
		// The user set GUARD_CFG_DIRECTORY, so we *ALWAYS* want to use that
		struct passwd *		pw;
		char *				tmp_dir;
		
		// Check to see if we can write to this directory
		if (checkDirPerms(cfg_dir))
		{
			PrintOutError("Bad permissions on directory specified by env var %s. Aborting...\n", GUARD_CFG_DIRECTORY);
			exit(1);
		}
		
		// Lets create a unique directory just in case the user gave us some
		// weird location that others can access
		
		// Get the pw info, this is used in the unique name part of cfg directories
		if ((pw = getpwuid(getuid())) == NULL)
		{
			PrintOutError("getpwuid() failed. Aborting...\n");
			exit(1);
		}
	
		tmp_dir = cfg_dir;
		cfg_dir = NULL;
		Asprintf(&cfg_dir, "%s/.gdb4hpc-%s", tmp_dir, pw->pw_name);
		
		// Setup/create this directory
		setupCfgDir(cfg_dir);
		setenv(GUARD_CFG_DIRECTORY, cfg_dir, 1);
		Free(cfg_dir);
	}
	
	// set the gdb_path for CTI
	Asprintf(&gdb_dir, "%s/gdb-guard", libexec_dir);
	setenv(CTI_GDB_LOC_ENV_VAR, gdb_dir, 1);
	
	Free(gdb_dir);
	Free(lib_dir);
	Free(libexec_dir);
	Free(inst_base);
}

int 
main(int argc, char *argv[])
{
	int					c;
	double				ulp;
	int					opt_ind = 0;
	cli_stream_args_t *	cli_args = NULL;	// CLI args
	char *				in_file = NULL;
	char *				out_file = NULL;
	
	// Ensure the environment has been setup, otherwise set it up and re-exec
	// This is required because otherwise dlopen's will be fubared.
	if (getenv(GUARD_ENV_SETUP) == NULL)
	{
		char *exe_path;
	
		SetupEnvironment();
		// Done. Set the proper environment variable so that we don't have an infinite
		// loop here.
		setenv(GUARD_ENV_SETUP, "true", 1);
		exe_path = getexepath();
		execve(exe_path, argv, environ);
	}
	
	// Initialize the settings to the default values
	Client.confirm		= 1;				// Confirm dangerous commands?
	Client.quit			= 0;				// Quit the debugger?
	Client.interactive	= 1;				// Interactive mode?
	Client.silent		= 0;				// Be quiet?
	Client.unsafe		= 0;				// Be unsafe?
	Client.early_entry	= 0;				// Enter at pmi_init?
	Client.sync_wait	= 0;				// Treat asynchronous cmds as sync?
	Client.sync_blocked	= 0;				// Current blocked on async cmd?
	Client.proto		= DBG_PROTO;		// Default debugger protocol
	Client.arch			= DBG_ARCH;			// Default starter achitecture
	Client.backend		= DBG_BACKEND;		// Default debug backend
	Client.XML_mode		= 0;				// XML output mode
	Client.set_env_list	= NewList();		// Env vars from set env cmd
	// find out the ulp for this machine
	ulp = nextafter(0.0, 0.1);
	Client.cmp_lower_eps = DoubleToAIF(ulp);
	Client.cmp_upper_eps = DoubleToAIF(ulp);
	
	/*
	** Set signal handlers.
	** Note that this MUST happen before the InitCLI function is called, otherwise
	** things like editline/readline won't register the signal handlers properly.
	** In the future, if a signal handler needs to be added it should happen early
	** on before InitCLI.
	*/
	DbgHandleSignal(SIGINT, SigHandler, DBG_SIG_MASKABLE);
	DbgHandleSignal(SIGHUP, SigHandler, DBG_SIG_NOMASK);
	DbgHandleSignal(SIGTERM, SigHandler, DBG_SIG_NOMASK);
	DbgHandleSignal(SIGQUIT, SigHandler, DBG_SIG_NOMASK);
	// we want to ignore SIGPIPE
	DbgHandleSignal(SIGPIPE, SIG_IGN, DBG_SIG_NOMASK);

	while ((c = getopt_long(argc, argv, "b:hi:o:sv", long_opts, &opt_ind)) != -1)
	{
		switch (c)
		{
			case 0:
				// if this is a flag, do nothing
				break;
			case 'b':
			{
				char *stringp;
				
				if (optarg == NULL)
				{
					PrintOutError("Error: --batch provided, but missing arguments.\n");
					exit(1);
				}
				
				// Set the mode and create the cli args
				Client.cli_mode = CLI_FILE_STREAM;
				stringp = strdup(optarg);
				in_file = strdup(StripQuotes(stringp));
				Free(stringp);
				
				// Turn confirm off, we will not be able to call fgetc on stdin
				// with batch. If this is needed, rework the query function
				// to call an external cli function that is cli stream dependent.
				Client.confirm = 0;
				
				break;
			}
			case 'i':
			{
				char	*stringp, *argstr, *token;
				char	*hoststr, *portstr, *errpstr;
				
				if (optarg == NULL)
				{
					PrintOutError("Error: --interface provided, but missing arguments.\n");
					exit(1);
				}
				
				// Setup things. Note that strsep modifies the string, so it shouldn't
				// use optarg directly.
				argstr = strdup(optarg);
				stringp = argstr;
				
				// First parse the host argument
				// Make sure its not null or that it doesn't contain a null terminator
				if ((token = strsep(&stringp, ":")) == NULL || *token == '\0')
				{
					PrintOutError("Error:    Encountered problem with host argument.\n");
					PrintOutError("Expected: --interface host:IOPort:ERRPort\n");
					PrintOutError("Found:    --interface %s\n", optarg);
					exit(1);
				}
				hoststr = token;
				// Next parse the IO port argument
				// Make sure its not null or that it doesn't contain a null terminator
				if ((token = strsep(&stringp, ":")) == NULL  || *token == '\0')
				{
					PrintOutError("Error:    Encountered problem with IOPort argument.\n");
					PrintOutError("Expected: --interface host:IOPort:ERRPort\n");
					PrintOutError("Found:    --interface %s\n", optarg);
					exit(1);
				}
				portstr = token;
				// The final argument is what is left in stringp
				// Make sure this isn't pointing at the null terminator
				if (*stringp == '\0')
				{
					PrintOutError("Error:    Encountered problem with ERRPort argument.\n");
					PrintOutError("Expected: --interface host:IOPort:ERRPort\n");
					PrintOutError("Found:    --interface %s\n", optarg);
					exit(1);
				}
				errpstr = stringp;
				
				// Set the default CLI and args to socket type
				Client.cli_mode = CLI_SOCKET_STREAM;
				cli_args = NewCLIArgs(CLI_SOCKET_STREAM, hoststr, portstr, errpstr);
				// Turn confirm off, we will not be able to call fgetc on stdin
				// through a socket. If this is needed, rework the query function
				// to call an external cli function that is cli stream dependent.
				Client.confirm = 0;
				// Set silent to 1, we don't want to print out banner
				// This should be the case when output is XML
				Client.silent = 1;
				
				// Cleanup original strsep string that we strdup'ed.
				Free(argstr);
				
				break;
			}
			case 'o':
			{
				char *stringp;
				
				if (optarg == NULL)
				{
					PrintOutError("Error: --output provided, but missing arguments.\n");
					exit(1);
				}
				
				// Set the mode and create the cli args
				Client.cli_mode = CLI_FILE_STREAM;
				stringp = strdup(optarg);
				out_file = strdup(StripQuotes(stringp));
				Free(stringp);
				
				break;
			}
			case 's':
				// Set silent to 1, we don't want to print out banner
				Client.silent = 1;
				break;
			case 'v':
				PrintOutMsg("gdb4hpc-%s\n", GUARD_VERSION);
				exit(0);
			case 'h':
				usage(argv[0]);
				exit(0);
			default:
				usage(argv[0]);
				exit(1);
		}
	}
	
	// Check if we should use the default CLI, if one of the above options didn't
	// create cli_args, then we know we should use the default.
	// We now also create the CLI_FILE_STREAM CLI here so that an output file
	// can be used with the batch mode
	if (cli_args == NULL)
	{
		if (Client.cli_mode == CLI_FILE_STREAM)
		{
			// Ensure that there is an in_file, this is a requirement.
			if (in_file == NULL)
			{
				PrintOutError("Error: Cannot use --output argument without --batch argument.\n");
				exit(1);
			}
			cli_args = NewCLIArgs(CLI_FILE_STREAM, in_file, out_file, out_file);
			Free(in_file);
			if (out_file != NULL)
				Free(out_file);
		} else
		{
			// Use the default CLI
			
			// Check to see if stdin is a tty, if not set the default CLI to notty
			if (!isatty(STDIN_FILENO))
			{
				// Set the default stream to notty
				Client.cli_mode = CLI_NOTTY_STREAM;	// CLI mode
			} else
			{
				// Set the default stream to tty
				Client.cli_mode = CLI_TTY_STREAM;	// CLI mode
			}
			// Create the CLI args
			cli_args = NewCLIArgs(Client.cli_mode, stdin, stdout, stderr);
		}
	}

	if ( DbgInit() < 0 )
	{
		PrintOutError("Could not initialise debugger.\n");
		exit(1);
	}

	InitVal();
	InitProcSet();
	InitCommands();
#ifdef RD
	InitDF();
#endif /* RD */
	InitCLI(Client.cli_mode, cli_args);

	if ( !Client.silent )
	{
		Banner();
	}
	
	// Initial reset of the stream, for good measure.
	// If we don't do this, the prompt will be missing when using the editline
	// library. I think this is due to some weirdness with the Banner() call.
	cli_reset_stream();

	(void)setjmp(Top);
	started = 1;

	// Main event loop
	for ( ;; )
	{
		// Check this first in case we got here after a signal arrived, that
		// might have set Client.quit.
		if ( Client.quit )
			break;
	
		// Check to see if we should block on an async event
		if (Client.sync_wait)
		{
			// get current async pending status
			int astatus = DbgCheckForAsyncPending();
			
			// If we are currently blocking on async command and the async pending
			// is false, we can unblock.
			if (Client.sync_blocked && !astatus)
			{
				// Block signals here because there is a race condition otherwise
				HoldSignals();
				DbgUnmaskInput();
				Client.sync_blocked = 0;
				ReleaseSignals();
				
			// If we are not currently blocking on an async command and the async
			// pending is true, we need to block
			} else if (!Client.sync_blocked && astatus)
			{
				// Block signals here because there is a race condition otherwise
				HoldSignals();
				DbgMaskInput();
				Client.sync_blocked = 1;
				ReleaseSignals();
			}
		}

		switch ( DbgCheckEvents() )
		{
			case -1:
				Client.quit = 1;

			case 0:
				continue;
			
			default:
				DbgDispatchEvents();
		}
	}

#ifdef RD
	FinishDF();
#endif /* RD */
	EndAll();
	DbgFinish();
	FinishProcSet();
	FinishFunc();
	FinishVal();
	DestroyCommands();
	DestroyCLI();

	exit(0);
}

void
ForceQuit(void)
{
	Client.quit = 1;
	// Force any running graphs to finish
#ifdef RD
	FinishDF();
#endif /* RD */
}

static void
SigHandler(int sig)
{
	dbgprocset_t *		s;
	rec_set_t *			rec_set;
	rep_set_t *			rep_set;
	group_reply_t *		reply;
	dbgloc_t *			halt_loc;
	
	
	DPRINT("\n***In SigHandler\n");
	
#if defined (RD)
	/*
	** Stop the active graph if there is one.
	*/
	if (ActiveDF())
	{
		ExitDF();
	}
#endif /* RD */

	// Any signals that should cause gdb4hpc to quit should be handled here.
	if ( sig == SIGHUP || sig == SIGTERM || sig == SIGQUIT )
	{
		Client.quit = 1;

		if ( started )
			longjmp(Top, sig);
	}

	s = CurrentProcSet();
	
	// Create a rec_set for this procset
	rec_set = CreateRecSetForProcset(s);
	
	// Call the interupt command
	rep_set = DbgInterrupt(rec_set);
		
	//process error
	ProcessErrorInRepSet(rep_set);
		
	//process location
	while((reply = GetNextReplyInSet(rep_set)) != (group_reply_t *)NULL)
	{
		halt_loc = EVENT_LOC(reply->rep_event);
		PrintHaltedLocationSet(reply->rep_di->d_handle, reply->rep_group, halt_loc);
	}
		
	FreeRepSet(rep_set);
	FreeRecSet(rec_set);
	
	// unmask the signal
	DbgUnmaskSignal(sig);

	if ( started )
	{
		// Reset the stream to default
		SetDefaultCLIStream();
		longjmp(Top, sig);
	}
}

// This is called each time an asynchronous reply is recieved. The entire event
// set which contains the aggregated event is passed in as ev_set and the
// individual reply that was just received is passed in as reply_set.
int	
EventCB(event_set_t *ev_set, bitset *reply_set)
{
	bp_set_t *		bpset;
	
	// need to do "special" things if we are currently reading from stdin
	// for readline support.
	cli_save_stream();
	
	DPRINT(	"INFO[EventCB]: received event type:[%s], ev_bitset:%s, reply_set:%s from:%s\n", 
			EventToStr(ev_set->e_ev), bitset_to_set(ev_set->e_group), 
			bitset_to_set(reply_set), ev_set->e_di->d_handle);

	switch ( ev_set->e_ev->ev_event )
	{
	case DBGEV_EXIT:
		if ( ev_set->e_ev->ev_exit == 0 )
		{
			PrintMsgWithSetInfo(ev_set->e_di->d_handle, ev_set->e_group, "Program exited normally.\n");
		} else
		{
			PrintMsgWithSetInfo(ev_set->e_di->d_handle, ev_set->e_group, "Program exited with code 0%o.\n", ev_set->e_ev->ev_exit);
		}
		break;

	case DBGEV_BPHIT:
		// ensure the recv bp has a cooresponding bp_set. If not, this is an unknown breakpoint.
		// If its unknown either its a breakpoint encountered from ending the graph (graph breakpoints are hidden)
		// or an exit breakpoint that was set by the dbg servers.
		
		if ((bpset = FindBPSetForBP(ev_set->e_di, ev_set->e_ev->ev_bp)) == NULL)
		{
			if(ev_set->e_ev->ev_bp->bp_type == DBGBP_EXIT_TYPE)
			{
				PrintMsgWithSetInfo(ev_set->e_di->d_handle, ev_set->e_group, "The application has reached an exit breakpoint.\n");
			}
			else
			{
				if (ev_set->e_ev->ev_bp->bp_loc.loc_line > 0)
				{
					PrintMsgWithSetInfo(ev_set->e_di->d_handle, ev_set->e_group, "In %s at %s:%d\n",
						ev_set->e_ev->ev_bp->bp_loc.loc_func,
						ev_set->e_ev->ev_bp->bp_loc.loc_file,
						ev_set->e_ev->ev_bp->bp_loc.loc_line);
				} else
				{
					PrintMsgWithSetInfo(ev_set->e_di->d_handle, ev_set->e_group, "In %s\n",
						ev_set->e_ev->ev_bp->bp_loc.loc_func);
				}
			}
			break;
		}
		
		// What we print out is based on if we are at a watchpoint or breakpoint
		// TODO: What should this default to? Right now I am having it default to 
		// the breakpoint message.
		switch (bpset->b_bp_type)
		{
			case DBGBP_WATCH_TYPE:
			case DBGBP_AWATCH_TYPE:
			case DBGBP_RWATCH_TYPE:
				PrintMsgWithSetInfo(ev_set->e_di->d_handle, ev_set->e_group, "Watchpoint %d, variable expression: %s\n",
					bpset->b_bp_id,
					bpset->b_bp_exp);
				// We should also print out the updated locale information for the watchpoint.
				// This was updated in EventCB before calling this function.
				if ( bpset->b_loc_line > 0 )
				{
					PrintMsgWithSetInfo(ev_set->e_di->d_handle, ev_set->e_group, "In %s at %s:%d\n",
						bpset->b_loc_func,
						bpset->b_loc_file,
						bpset->b_loc_line);
				} else
				{
					PrintMsgWithSetInfo(ev_set->e_di->d_handle, ev_set->e_group, "In %s\n",
						bpset->b_loc_func);
				}
				break;
				
			case DBGBP_BREAK_TYPE:
				if ( bpset->b_loc_line > 0 )
				{
					PrintMsgWithSetInfo(ev_set->e_di->d_handle, ev_set->e_group, "Breakpoint %d, %s at %s:%d\n",
						bpset->b_bp_id,
						bpset->b_loc_func,
						bpset->b_loc_file,
						bpset->b_loc_line);
				} else
				{
					PrintMsgWithSetInfo(ev_set->e_di->d_handle, ev_set->e_group, "Breakpoint %d, in %s\n",
						bpset->b_bp_id,
						bpset->b_loc_func);
				}
				break;
				
			default:
				if ( bpset->b_loc_line > 0 )
				{
					PrintMsgWithSetInfo(ev_set->e_di->d_handle, ev_set->e_group, "Unknown break type %d, %s at %s:%d\n",
						bpset->b_bp_id,
						bpset->b_loc_func,
						bpset->b_loc_file,
						bpset->b_loc_line);
				} else
				{
					PrintMsgWithSetInfo(ev_set->e_di->d_handle, ev_set->e_group, "Unknown break type %d, in %s\n",
						bpset->b_bp_id,
						bpset->b_loc_func);
				}
				break;
		}
		
		break;

	case DBGEV_BPDEL:
		// ensure the recv bp has a cooresponding bp_set. If not, this is an unknown breakpoint.
		// If its unknown either its a breakpoint encountered from ending the graph (graph breakpoints are hidden)
		// or an exit breakpoint that was set by the dbg servers.
		if ((bpset = FindBPSetForBP(ev_set->e_di, ev_set->e_ev->ev_bp)) == NULL)
		{
			if(ev_set->e_ev->ev_bp->bp_type == DBGBP_EXIT_TYPE)
			{
				// How can the exit breakpoint be automatically deleted?
			}
			else
			{
			}
			break;
		}
		
		// What we print out is based on if we are at a watchpoint or breakpoint
		switch (bpset->b_bp_type)
		{
			case DBGBP_WATCH_TYPE:
			case DBGBP_AWATCH_TYPE:
			case DBGBP_RWATCH_TYPE:
				PrintMsgWithSetInfo(ev_set->e_di->d_handle, ev_set->e_group, "Watchpoint %d deleted because the has left the block in which its expression is valid.\n",
					bpset->b_bp_id);
				// We should also print out the updated locale information for the watchpoint.
				if ( bpset->b_loc_line > 0 )
				{
					PrintMsgWithSetInfo(ev_set->e_di->d_handle, ev_set->e_group, "In %s at %s:%d\n",
						bpset->b_loc_func,
						bpset->b_loc_file,
						bpset->b_loc_line);
				} else
				{
					PrintMsgWithSetInfo(ev_set->e_di->d_handle, ev_set->e_group, "In %s\n",
						bpset->b_loc_func);
				}
				break;
				
			case DBGBP_BREAK_TYPE:
				if ( bpset->b_loc_line > 0 )
				{
					PrintMsgWithSetInfo(ev_set->e_di->d_handle, ev_set->e_group, "Breakpoint %d deleted, %s at %s:%d\n",
						bpset->b_bp_id,
						bpset->b_loc_func,
						bpset->b_loc_file,
						bpset->b_loc_line);
				} else
				{
					PrintMsgWithSetInfo(ev_set->e_di->d_handle, ev_set->e_group, "Breakpoint %d deleted, in %s\n",
						bpset->b_bp_id,
						bpset->b_loc_func);
				}
				break;
				
			default:
				if ( bpset->b_loc_line > 0 )
				{
					PrintMsgWithSetInfo(ev_set->e_di->d_handle, ev_set->e_group, "Unknown break type %d deleted, %s at %s:%d\n",
						bpset->b_bp_id,
						bpset->b_loc_func,
						bpset->b_loc_file,
						bpset->b_loc_line);
				} else
				{
					PrintMsgWithSetInfo(ev_set->e_di->d_handle, ev_set->e_group, "Unknown break type %d deleted, in %s\n",
						bpset->b_bp_id,
						bpset->b_loc_func);
				}
				break;
		}

		break;

	case DBGEV_SIGNAL:
		PrintMsgWithSetInfo(ev_set->e_di->d_handle, ev_set->e_group, "Program received signal %s.\n", ev_set->e_ev->ev_sig_type);

		if ( ev_set->e_ev->ev_sig_frame != (dbgframe_t *)NULL )
		{
			if (ev_set->e_ev->ev_sig_frame->frame_loc.loc_file != NULL)
			{
				PrintMsgWithSetInfo(ev_set->e_di->d_handle, ev_set->e_group, "In %s at %s:%d\n",
					ev_set->e_ev->ev_sig_frame->frame_loc.loc_func, 
					ev_set->e_ev->ev_sig_frame->frame_loc.loc_file, 
					ev_set->e_ev->ev_sig_frame->frame_loc.loc_line
					);
			} else
			{
				PrintMsgWithSetInfo(ev_set->e_di->d_handle, ev_set->e_group, "In %s\n",
					ev_set->e_ev->ev_sig_frame->frame_loc.loc_func
					);
			}
		} else if (ev_set->e_ev->ev_sig_line != NULL)
		{
			PrintMsgWithSetInfo(ev_set->e_di->d_handle, ev_set->e_group, "%s\n",
				ev_set->e_ev->ev_sig_line
				);
		}
		
		break;

	case DBGEV_STEP:
		if ( ev_set->e_ev->ev_step_frame != (dbgframe_t *) NULL )
		{
			if(ev_set->e_ev->ev_step_frame->frame_loc.loc_line > 0)
			{
				PrintMsgWithSetInfo(ev_set->e_di->d_handle, ev_set->e_group, "%s at %s:%d\n",
					ev_set->e_ev->ev_step_frame->frame_loc.loc_func, 
					ev_set->e_ev->ev_step_frame->frame_loc.loc_file, 
					ev_set->e_ev->ev_step_frame->frame_loc.loc_line
					);
			} else
			{
				PrintMsgWithSetInfo(ev_set->e_di->d_handle, ev_set->e_group, "in %s\n",
					ev_set->e_ev->ev_step_frame->frame_loc.loc_func
					);
			}
			break;
		}

		if ( ev_set->e_ev->ev_step_lno < 0 )
		{
			PrintMsgWithSetInfo(ev_set->e_di->d_handle, ev_set->e_group, "Single stepping until exit from function %s,\n", ev_set->e_ev->ev_step_line);
			PrintMsgWithSetInfo(ev_set->e_di->d_handle, ev_set->e_group, "which has no line number information.\n");
		}
		else if(ev_set->e_ev->ev_step_lno == 0)
		{
			PrintMsgWithSetInfo(ev_set->e_di->d_handle, ev_set->e_group, "Program has reached the end.\n");
		} else
			PrintMsgWithSetInfo(ev_set->e_di->d_handle, ev_set->e_group, "%d\t%s\n", 
				ev_set->e_ev->ev_step_lno, 
				ev_set->e_ev->ev_step_line);
		break;

	case DBGEV_ERROR:
		PrintErrorWithSetInfo(ev_set->e_di->d_handle, ev_set->e_group, "%s\n", DbgEventErrorStr(ev_set->e_ev));
		break;
		
	case DBGEV_OK:
		break;

	default:
		PrintErrorWithSetInfo(ev_set->e_di->d_handle, ev_set->e_group, "Can't print event %d\n", ev_set->e_ev->ev_event);
		break;
	}
	
	// If XML mode is enabled, we need to ask the dbgclient layer if the event
	// we just received changed the status. If it did, we want to add the status 
	// information to the xml output.
	if (Client.XML_mode)
	{
		// used for checking status
		rec_set_t *			rec_set;
		rep_set_t *			rep_set;
		group_reply_t *		reply;
		char *				bitset_str;
		char *				set_str;
		dbgstatus_t *		s;
		char *				status_block;
	
		if (DbgCheckForStatusChange())
		{
			// Status has changed. We need to generate updated status
			// information - this only impacts a single di
			
			// create the rec_set - FOR THE ENTIRE DI!!!
			// Bugfix for BUG 814782
			rec_set = NewRecSet();
			AddReceiverToSet(rec_set, NewGroupReceiver(ev_set->e_di, ev_set->e_di->d_bitset_full));
			
			// Query the new status from the backends, note that for this we
			// want to grab the location information
			rep_set = DbgStatus(rec_set, 1);
			
			ProcessErrorInRepSet(rep_set);
			
			// Start a new status block
			status_block = XMLStartStatusBlock(ev_set->e_di->d_handle);
			
			// Process each status message and add it to the status block
			while((reply = GetNextReplyInSet(rep_set)) != NULL)
			{
				// First create the bitset string
				bitset_str = bitset_to_set(reply->rep_group);
				
				// Create the set string
				Asprintf(&set_str, "%s%s", reply->rep_di->d_handle, bitset_str);
				
				// Done with the bitset_str
				Free(bitset_str);
				
				// point at the status data
				s = EVENT_STATUS(reply->rep_event);
				
				// The XML status that is printed is based on the status value
				XMLAddStatus(&status_block, set_str, s->stat, s->stat_loc);
					
				// Done with the set_str
				Free(set_str);
			}
			
			// Print the entire status block, this will free the string
			XMLPrintStatusBlock(status_block);
				
			// Free the reply set
			FreeRepSet(rep_set);
			
			// Done with the rec_set
			FreeRecSet(rec_set);
				
			// Reset the status change since we handled it
			DbgResetStatusChange();
		}
	}
	
	cli_restore_stream();
	
	return 1;
}

