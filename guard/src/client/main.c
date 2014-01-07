/*
** Guard debugger
**
** Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
**
*/

#ifdef HAVE_CONFIG_H
#include	<config.h>
#endif /* HAVE_CONFIG_H */

#include	<sys/types.h>
#include	<sys/time.h>
#include	<sys/stat.h>
#include	<sys/mman.h>

#include	<ctype.h>
#include	<errno.h>
#include	<fcntl.h>
#include	<getopt.h>
#include	<stdio.h>
#include	<stdarg.h>
#include	<stdlib.h>
#include	<string.h>
#include	<signal.h>
#include	<setjmp.h>
#include	<unistd.h>
#include	<math.h>

#include	"client.h"
#include	"useful.h"
#if defined (RD)
#include	"df.h"
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
clnt_settings_t		Client;		// global settings store - initialized in main

// static globals
static jmp_buf	Top;			// Where to go when restoring upon signal
static int 		debug_flag = 0;	// Debug flag - TODO: not used yet
static int		started = 0;	// Is it alright to longjmp?

/*
 * Static Function Prototypes
 */
static void		Banner(void);
static void		SigHandler(int);
static void		WaitForEvents(int);
static void		PrintEventSet(dbg_over_set_t *over_set, bitset *setinfo, dbgevent_t *);
//static int		DoBatch(FILE *);

/*
 * long opts
 */
static const struct option long_opts[] = {
			{"interface",	required_argument,	0, 'i'},
			{"xml",			no_argument,		&(Client.XML_mode), 1},
			{"debug",		no_argument,		&debug_flag, 1},
			{0, 0, 0, 0}
			};

static void
Banner(void)
{
#ifdef ALPS
	PrintOutMsg("lgdb 2.2 - Cray Line Mode Parallel Debugger\n");
	PrintOutMsg("With Cray Comparative Debugging Technology.\n");
	PrintOutMsg("Copyright 2007-2013 Cray Inc. All Rights Reserved.\n");
#else		
	PrintOutMsg("GUARD Parallel Relative Debugger\n");
#endif /* ALPS */
	PrintOutMsg("Copyright 1996-2013 Monash University. All Rights Reserved.\n");
	PrintOutMsg("\n");
	PrintOutMsg("Type \"help\" for a list of commands.\n");
}

// All environment setup should happen here
static void
SetupEnvironment(void)
{
	char *				inst_base;
	char *				lib_dir;
	char *				libexec_dir;
	char *				cfg_dir;
	char *				cti_ld_val_path;
	
	// Adjust the environment to add our install base to LD_LIBRARY_PATH and PATH
	// of our process
	if ((inst_base = getinstallbase()) == NULL)
	{
		fprintf(stderr, "Could not locate install base. Aborting...\n");
		exit(1);
	}
	// create the path strings
	Asprintf(&lib_dir, "%s/lib", inst_base);
	Asprintf(&libexec_dir, "%s/libexec", inst_base);
	Asprintf(&cti_ld_val_path, "%s/libaudit.so", lib_dir);
	
	// Set the environment variables
	adjustLibPath(lib_dir);
	adjustExecPath(libexec_dir);
	
	// get the cfg dir settings
	if ((cfg_dir = getenv(GUARD_CFG_DIRECTORY)) == NULL)
	{
		// Not found, so default to ~/.cray_lgdb
		char *home_dir;
		if ((home_dir = getenv("HOME")) == NULL)
		{
			fprintf(stderr, "Could not read $HOME. Aborting...\n");
			exit(1);
		}
		Asprintf(&cfg_dir, "%s/.cray_lgdb", home_dir);
		setenv(GUARD_CFG_DIRECTORY, cfg_dir, 1);
		setenv(CTI_CFG_DIR_ENV_VAR, cfg_dir, 1);
		Free(cfg_dir);
	} else
	{
		setenv(CTI_CFG_DIR_ENV_VAR, cfg_dir, 1);
	}
	
	// set the ld_val path for the cti
	setenv(CTI_LIBAUDIT_ENV_VAR, cti_ld_val_path, 1);
	
	Free(lib_dir);
	Free(libexec_dir);
	Free(cti_ld_val_path);
}

int 
main(int argc, char *argv[])
{
	int					c;
	int					opt_ind = 0;
	//FILE *			fp = stdin;
	cli_stream_args_t *	cli_args;	// CLI args
	double				ulp;
	
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
	Client.proto		= DBG_PROTO;		// Default debugger protocol
	Client.arch			= DBG_ARCH;			// Default starter achitecture
	Client.backend		= DBG_BACKEND;		// Default debug backend
	Client.XML_mode		= 0;				// XML output mode
	Client.set_env_list	= NewList();		// Env vars from set env cmd
	// find out the ulp for this machine
	ulp = nextafter(0.0, 0.1);
	Client.cmp_lower_eps = DoubleToAIF(ulp);
	Client.cmp_upper_eps = DoubleToAIF(ulp);
	
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
	
	/*
	** Set signal handlers.
	** Note that this MUST happen before the InitCLI function is called, otherwise
	** things like editline/readline won't register the signal handlers properly.
	** In the future, if a signal handler needs to be added it should happen early
	** on before InitCLI.
	*/
	DbgHandleSignal(SIGINT, SigHandler);
	DbgHandleSignal(SIGHUP, SigHandler);
	DbgHandleSignal(SIGTERM, SigHandler);
	DbgHandleSignal(SIGQUIT, SigHandler);

	while ((c = getopt_long(argc, argv, "i:", long_opts, &opt_ind)) != -1)
	{
		switch (c)
		{
			case 0:
				// if this is a flag, do nothing
				break;
			case 'i':
			{
				char	*stringp, *argstr, *token;
				char	*hoststr, *portstr, *errpstr;
				
				// destroy the existing default args
				DestroyCLIArgs(Client.cli_mode, cli_args);
				
				if (optarg == (char *)NULL)
				{
					fprintf(stderr, "Error: --interface provided, but missing arguments.\n");
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
					fprintf(stderr, "Error:    Encountered problem with host argument.\n");
					fprintf(stderr, "Expected: --interface host:IOPort:ERRPort\n");
					fprintf(stderr, "Found:    --interface %s\n", optarg);
					exit(1);
				}
				hoststr = token;
				// Next parse the IO port argument
				// Make sure its not null or that it doesn't contain a null terminator
				if ((token = strsep(&stringp, ":")) == NULL  || *token == '\0')
				{
					fprintf(stderr, "Error:    Encountered problem with IOPort argument.\n");
					fprintf(stderr, "Expected: --interface host:IOPort:ERRPort\n");
					fprintf(stderr, "Found:    --interface %s\n", optarg);
					exit(1);
				}
				portstr = token;
				// The final argument is what is left in stringp
				// Make sure this isn't pointing at the null terminator
				if (*stringp == '\0')
				{
					fprintf(stderr, "Error:    Encountered problem with ERRPort argument.\n");
					fprintf(stderr, "Expected: --interface host:IOPort:ERRPort\n");
					fprintf(stderr, "Found:    --interface %s\n", optarg);
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
				// This should only be the case when output is XML
				Client.silent = 1;
				
				// Cleanup original strsep string that we strdup'ed.
				Free(argstr);
				
				break;
			}
			default:
				//usage(argv[0]);
				exit(1);
		}
	}

	if ( DbgInit() < 0 )
	{
		fprintf(stderr, "Could not initialise debugger.\n");
		exit(1);
	}

	InitProcSet();
	InitCommands();
#ifdef RD
	InitDF();
#endif /* RD */
	InitVal();
	InitCLI(Client.cli_mode, cli_args);

	// TODO: This is used for batch mode. It should be re-added to the arguments
	// if needed.
	/*
	if ( !Client.interactive )
	{
		if ( optind < argc && (fp = fopen(argv[optind], "r")) == NULL )
		{
			PrintOutError("Could not open \"%s\"\n", argv[optind]);
			exit(1);
		}

		exit(DoBatch(fp));
	}
	*/

	if ( !Client.silent )
	{
		Banner();
	}
	
	// Initial reset of the stream, for good measure.
	// If we don't do this, the prompt will be missing when using the editline
	// library.
	cli_reset_stream();

	(void)setjmp(Top);
	started = 1;

	for ( ;; )
	{
		if ( Client.quit )
			break;
		DbgHandleInput(Client.cli_in_fd, cli_handle_input);
		WaitForEvents(0);
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
	KillAll();
	DestroyCLI();
	
	_exit(1);
}

static void
SigHandler(int sig)
{
	dbgprocset_t *	s;
	
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

	if ( sig == SIGHUP || sig == SIGTERM || sig == SIGQUIT )
	{
		Client.quit = 1;

		if ( started )
			longjmp(Top, sig);
	}

	s = CurrentProcSet();

	SetOverlaySetForProcSet(s);
	
	dbg_over_set_t *	over_set = NULL;
	
	while((over_set = GetNextOverlaySetForProcSet(s)) != (dbg_over_set_t*)NULL)
	{
		rec_set_t *			rec_set = NewRecSetByOverSet(over_set, over_set->s_set_bs);
		rep_set_t *			rep_set = DbgInterrupt(rec_set);
		group_reply_t *		reply;
		dbgloc_t *			halt_loc;
		
		//process error
		ProcessErrorInRepSet(rec_set, rep_set);
		
		//process location
		while((reply = GetNextReplyInSet(rep_set)) != (group_reply_t *)NULL)
		{
			halt_loc = EVENT_LOC(reply->rep_event);
			PrintHaltedLocationSet(rec_set->s_setname, reply->rep_group, halt_loc);
		}
		
		FreeRepSet(rep_set);
		FreeRecSet(rec_set);
	}
	
	// unmask the signal
	DbgUnmaskSignal(sig);

	if ( started )
	{
		// Reset the stream to default
		SetDefaultCLIStream();
		longjmp(Top, sig);
	}
}

static void
WaitForEvents(int noblock)
{
	for ( ;; )
	{
		switch ( DbgCheckEvents() )
		{
			case -1:
				Client.quit = 1;

			case 0:
				// we only want to continue if we haven't triggered the Quit condition.
				if ( !noblock && !Client.quit)
					continue;
				break;
			
			default:
				DbgDispatchEvents();
		}

		break;
	}
}

static void
PrintEventSet(dbg_over_set_t *over_set, bitset *setinfo, dbgevent_t *e)
{
	bp_set_t*	bpset;
	int			bp_id;
	char*		setname = over_set->s_setname;
	invoke_t *	invoke = over_set->s_invoke;

	DPRINT("PrintEventSet over_set[%p], offset[%d], nproc[%d], setinfo:%s\n", over_set, over_set->s_offset, over_set->s_nproc, bitset_to_set(setinfo));

	switch ( e->ev_event )
	{
	case DBGEV_EXIT:
		if ( e->ev_exit == 0 )
		{
			PrintMsgWithSetInfoByOffset(Client.cli_out_stream, setname, setinfo, over_set->s_offset, over_set->s_len, "Program exited normally.\n");
		} else
		{
			PrintMsgWithSetInfoByOffset(Client.cli_out_stream, setname, setinfo, over_set->s_offset, over_set->s_len, "Program exited with code 0%o.\n", e->ev_exit);
		}
		break;

	case DBGEV_BPHIT:
		// ensure the recv bp has a cooresponding bp_set. If not, this is an unknown breakpoint.
		// If its unknown either its a breakpoint encountered from ending the graph (graph breakpoints are hidden)
		// or an exit breakpoint that was set by the dbg servers.
		if ( (bpset = FindBPSetByBP( invoke->i_tree, e->ev_bp, &bp_id)) == (bp_set_t *)NULL )
		{
			if(e->ev_bp->bp_type == DBGBP_EXIT_TYPE)
			{
				PrintMsgWithSetInfoByOffset(Client.cli_out_stream, setname, setinfo, over_set->s_offset, over_set->s_len, "The application has reached an exit breakpoint. Please quit CCDB!\n");
			}
			else
			{
				if (e->ev_bp->bp_loc.loc_line > 0)
				{
					PrintMsgWithSetInfoByOffset(Client.cli_out_stream, setname, setinfo, over_set->s_offset, over_set->s_len, "In %s at %s:%d\n",
						e->ev_bp->bp_loc.loc_func,
						e->ev_bp->bp_loc.loc_file,
						e->ev_bp->bp_loc.loc_line);
				} else
				{
					PrintMsgWithSetInfoByOffset(Client.cli_out_stream, setname, setinfo, over_set->s_offset, over_set->s_len, "In %s\n",
						e->ev_bp->bp_loc.loc_func);
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
				PrintMsgWithSetInfoByOffset(Client.cli_out_stream, setname, setinfo, over_set->s_offset, over_set->s_len, "Watchpoint %d, variable expression: %s\n",
					bp_id,
					bpset->b_bp_exp);
				// We should also print out the updated locale information for the watchpoint.
				// This was updated in EventCB before calling this function.
				if ( bpset->b_loc_line > 0 )
				{
					PrintMsgWithSetInfoByOffset(Client.cli_out_stream, setname, setinfo, over_set->s_offset, over_set->s_len, "In %s at %s:%d\n",
						bpset->b_loc_func,
						bpset->b_loc_file,
						bpset->b_loc_line);
				} else
				{
					PrintMsgWithSetInfoByOffset(Client.cli_out_stream, setname, setinfo, over_set->s_offset, over_set->s_len, "In %s\n",
						bpset->b_loc_func);
				}
				break;
				
			case DBGBP_BREAK_TYPE:
				if ( bpset->b_loc_line > 0 )
				{
					PrintMsgWithSetInfoByOffset(Client.cli_out_stream, setname, setinfo, over_set->s_offset, over_set->s_len, "Breakpoint %d, %s at %s:%d\n",
						bp_id,
						bpset->b_loc_func,
						bpset->b_loc_file,
						bpset->b_loc_line);
				} else
				{
					PrintMsgWithSetInfoByOffset(Client.cli_out_stream, setname, setinfo, over_set->s_offset, over_set->s_len, "Breakpoint %d, in %s\n",
						bp_id,
						bpset->b_loc_func);
				}
				break;
				
			default:
				if ( bpset->b_loc_line > 0 )
				{
					PrintMsgWithSetInfoByOffset(Client.cli_out_stream, setname, setinfo, over_set->s_offset, over_set->s_len, "Unknown break type %d, %s at %s:%d\n",
						bp_id,
						bpset->b_loc_func,
						bpset->b_loc_file,
						bpset->b_loc_line);
				} else
				{
					PrintMsgWithSetInfoByOffset(Client.cli_out_stream, setname, setinfo, over_set->s_offset, over_set->s_len, "Unknown break type %d, in %s\n",
						bp_id,
						bpset->b_loc_func);
				}
				break;
		}
		
		break;

	case DBGEV_SIGNAL:
		PrintMsgWithSetInfoByOffset(Client.cli_out_stream, setname, setinfo, over_set->s_offset, over_set->s_len, "Program received signal %s.\n", e->ev_sig_type);

		if ( e->ev_sig_frame != (dbgframe_t *)NULL )
		{
			if (e->ev_sig_frame->frame_loc.loc_file != NULL)
			{
				PrintMsgWithSetInfoByOffset(Client.cli_out_stream, setname, setinfo, over_set->s_offset, over_set->s_len, "In %s at %s:%d\n",
					e->ev_sig_frame->frame_loc.loc_func, 
					e->ev_sig_frame->frame_loc.loc_file, 
					e->ev_sig_frame->frame_loc.loc_line
					);
			} else
			{
				PrintMsgWithSetInfoByOffset(Client.cli_out_stream, setname, setinfo, over_set->s_offset, over_set->s_len, "In %s\n",
					e->ev_sig_frame->frame_loc.loc_func
					);
			}
		} else if (e->ev_sig_line != NULL)
		{
			PrintMsgWithSetInfoByOffset(Client.cli_out_stream, setname, setinfo, over_set->s_offset, over_set->s_len, "%s\n",
				e->ev_sig_line
				);
		}
		
		break;

	case DBGEV_STEP:
		if ( e->ev_step_frame != (dbgframe_t *) NULL )
		{
			if(e->ev_step_frame->frame_loc.loc_line > 0)
			{
				PrintMsgWithSetInfoByOffset(Client.cli_out_stream, setname, setinfo, over_set->s_offset, over_set->s_len, "%s at %s:%d\n",
					e->ev_step_frame->frame_loc.loc_func, 
					e->ev_step_frame->frame_loc.loc_file, 
					e->ev_step_frame->frame_loc.loc_line
					);
			} else
			{
				PrintMsgWithSetInfoByOffset(Client.cli_out_stream, setname, setinfo, over_set->s_offset, over_set->s_len, "in %s\n",
					e->ev_step_frame->frame_loc.loc_func
					);
			}
			break;
		}

		if ( e->ev_step_lno < 0 )
		{
			PrintMsgWithSetInfoByOffset(Client.cli_out_stream, setname, setinfo, over_set->s_offset, over_set->s_len, "Single stepping until exit from function %s,\n", e->ev_step_line);
			PrintMsgWithSetInfoByOffset(Client.cli_out_stream, setname, setinfo, over_set->s_offset, over_set->s_len, "which has no line number information.\n");
		}
		else if(e->ev_step_lno == 0)
		{
			PrintMsgWithSetInfoByOffset(Client.cli_out_stream, setname, setinfo, over_set->s_offset, over_set->s_len, "Program has reached the end. Please quit CCDB!\n");
		} else
			PrintMsgWithSetInfoByOffset(Client.cli_out_stream, setname, setinfo, over_set->s_offset, over_set->s_len, "%d\t%s\n", 
				e->ev_step_lno, 
				e->ev_step_line);
		break;

	case DBGEV_ERROR:
		PrintMsgWithSetInfoByOffset(Client.cli_err_stream, setname, setinfo, over_set->s_offset, over_set->s_len, "%s\n", DbgEventErrorStr(e));
		break;
		
	case DBGEV_OK:
		break;

	default:
		PrintMsgWithSetInfoByOffset(Client.cli_err_stream, setname, setinfo, over_set->s_offset, over_set->s_len, "Can't print event %d\n", e->ev_event);
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
			// information
			
			// Create a new rec_set from the overset, using the full set.
			rec_set = NewRecSetByOverSet(over_set, over_set->s_set_bs);
			
			// Query the new status from the backends, note that for this we
			// want to grab the location information
			rep_set = DbgStatus(rec_set, 1);
			
			ProcessErrorInRepSet(rec_set, rep_set);
			
			// Start a new status block
			status_block = XMLStartStatusBlock(rec_set->s_setname);
			
			// Process each status message and add it to the status block
			while((reply = GetNextReplyInSet(rep_set)) != (group_reply_t*)NULL)
			{
				// First create the bitset string
				if (over_set->s_offset >= 0 && over_set->s_len > 0)
				{
					bitset_str = bitset_to_set_by_offset(reply->rep_group, over_set->s_offset, over_set->s_len);
				} else
				{
					bitset_str = bitset_to_set(reply->rep_group);
				}
					
				// Next create the full set string
				Asprintf(&set_str, "%s%s", rec_set->s_setname, bitset_str);
					
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
}

// This is called each time an asynchronous reply is recieved. The entire event
// set which contains the aggregated event is passed in as e/ev_set and the
// individual reply that was just received is passed in as reply_set.
// FIXME: This should be taking in an event_set_t not e/ev_set.
int	
EventCB(dbgevent_t *e, bitset* ev_set, bitset* reply_set, void *arg)
{
	void *				tree = arg;
	dbg_over_set_t *	over_set = NULL;
	bp_set_t *			bpset;
	int					bp_id;
	invoke_t *			invoke;
	bp_disp_set_t *		disp_set;
	bitset *			group;
	int					bpset_invalid = 0;
	
	// need to do "special" things if we are currently reading from stdin
	// for readline support.
	cli_save_stream();

	list_t* overSetList = (list_t*)GetOverlaySetListByTree(tree);

	DPRINT("INFO[EventCB]: received event type:[%s], ev_set:%s, reply_set:%s\n", EventToStr(e), bitset_to_set(ev_set), bitset_to_set(reply_set));	

	if(overSetList == NULL)
	{
		PrintOutError("ERROR: no procset info is found for tree[%p]\n", tree);
	} else 
	{
		SetList(overSetList);
		while((over_set = (dbg_over_set_t*)GetListElement(overSetList)) != (dbg_over_set_t*)NULL)
		{
			if(bitset_compare(over_set->s_set_bs, reply_set) != 0)
			{
				// conduct pre processing for special events
				switch ( e->ev_event )
				{
					case DBGEV_BPHIT:
						invoke = over_set->s_invoke;
						if ( (bpset = FindBPSetByBP( invoke->i_tree, e->ev_bp, &bp_id)) == (bp_set_t *)NULL )
						{
							// do nothing
							break;
						}
						
						// if the breakpoint was a watchpoint we need to update the location information
						switch (bpset->b_bp_type)
						{
							case DBGBP_WATCH_TYPE:
							case DBGBP_AWATCH_TYPE:
							case DBGBP_RWATCH_TYPE:
								if (e->ev_bp->bp_loc.loc_file != NULL)
								{
									// Free the old value and set it to the new value
									if(bpset->b_loc_file != NULL)
										Free(bpset->b_loc_file);
									bpset->b_loc_file = strdup(e->ev_bp->bp_loc.loc_file);
								}
								if (e->ev_bp->bp_loc.loc_func != NULL)
								{
									// Free the old value and set it to the new value
									if(bpset->b_loc_func != NULL)
										Free(bpset->b_loc_func);
									bpset->b_loc_func = strdup(e->ev_bp->bp_loc.loc_func);
								}
								bpset->b_loc_line = e->ev_bp->bp_loc.loc_line;
								
								break;
							default:
								// do nothing
								break;
						}
						break;
					
					default:
						// do nothing
						break;
				}
				
				// print the event
				PrintEventSet(over_set, ev_set, e);
				
				// conduct post processing for special events
				switch ( e->ev_event )
				{
					case DBGEV_BPHIT:
						invoke = over_set->s_invoke;
						if ( (bpset = FindBPSetByBP( invoke->i_tree, e->ev_bp, &bp_id)) == (bp_set_t *)NULL )
						{
							// do nothing
							break;
						}
						
						// If the breakpoint was set to delete, we need to delete it. If it was set to dis, we need to disable it.
						SetList(bpset->b_bp_disp);
						while((disp_set = (bp_disp_set_t *)GetListElement(bpset->b_bp_disp)) != NULL)
						{
							group = bitset_and(disp_set->b_group, reply_set);
							switch (disp_set->b_bp_disp)
							{
								case DBGBP_DELETE_DISP:
									// remove the breakpoint
									// NOTE: The breakpoint has already been removed by the backend.
									bpset_invalid = RemoveBPSet(invoke->i_tree, bpset, e->ev_bp->bp_id, group);
									break;
								case DBGBP_DISABLE_DISP:
									// mark the breakpoint as disabled for the ranks in the reply set.
									DisableBPForBPSet(bpset, group);
									break;
								default:
									// do nothing
									break;
							}
							bitset_free(group);
							if (bpset_invalid)
							{
								bpset_invalid = 0;
								break;
							}
						}
						break;
						
					default:
						// do nothing
						break;
				}
			}
		}
	}
	
	cli_restore_stream();

	return 1;
}

/*
** Process a batch file.
** FIXME: This doesn't work right now...
*/
/*
static int
DoBatch(FILE *fp)
{
	//CurrentStream = NewCLIStream(CLI_FILE_STREAM, fp, stdout, stderr, 0);

	//DbgHandleInput(CurrentStream->cs_fdin, cli_handle_input);

	while ( !Client.quit )
		WaitForEvents(0);

	//if ( fp != stdin )
		//fclose(fp);

	EndAll();
	DbgFinish();
	//DestroyCLIStream(CurrentStream);
	FinishProcSet();
#ifdef RD
	FinishDF();
#endif
	FinishFunc();
	FinishVal();
	DestroyCommands();

	return 0;
}
*/
