/*
** Process control commands.
**
** Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
**
*/

#ifdef HAVE_CONFIG_H
#include	<config.h>
#endif /* HAVE_CONFIG_H */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <getopt.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>

#include "client.h"
#include "useful.h"
#include "gvar.h"

/*
** Static global variables
*/
static int 		debug_flag = 0;
static int		gpu_flag = 0;

/*
** static function prototypes
*/
static int		DoInvoke(char *, proto_session *, char *, char **, int, list_t *, char *, char **, char *, char *, char **, char *);
static int		DoAttach(char *, proto_session *, int, list_t *, char *, char *);
static void		ReleaseInv(invoke_t *, int);
static void		KillInv(invoke_t *);

/*
** long opts definitions
*/
static const struct option session_long_opts[] = {
			{"qsub",		optional_argument, 0, 'q'},
			{"sbatch",		optional_argument, 0, 's'},
			{"end",			optional_argument, 0, 'e'},
			{0, 0, 0, 0}
			};

static const struct option launch_long_opts[] = {
			{"args",		optional_argument,	0, 'a'},
			{"aprun-args",	optional_argument,	0, 'g'},
			{"aprun-input",	optional_argument,	0, 'i'},
			{"workdir",		optional_argument,	0, 'd'},
			{"env",			optional_argument,	0, 'e'},
			{"qsub",		optional_argument,	0, 'q'},
			{"sbatch",		optional_argument,	0, 's'},
			{"gpu",			no_argument, 		&gpu_flag, 1},
			{"debug",		no_argument, 		&debug_flag, 1},
			{0, 0, 0, 0}
			};
			
static const struct option attach_long_opts[] = {
			{"gpu",			no_argument, 		&gpu_flag, 1},
			{"debug",		no_argument, 		&debug_flag, 1},
			{0, 0, 0, 0}
			};

static int 
class_info_subcmd_application_cmd(rec_set_t *rec_set, int argc, char **argv)
{
	invoke_t *		i;
	invoke_group *	ig;
	dbgprocset_t *	ps;

	if ( !SetInvoke() || (i = GetInvoke()) == (invoke_t *)NULL )
	{
		PrintOutError("There are currently no lauched/attached applications.\n");
		return 1;
	}

	PrintOutMsg("%-10s %-10s %-5s %s\n", "Handle", "Ranks", "Focus", "Command");
	
	// Get the current process set.
	ps = CurrentProcSet();

	do
	{
		SetInvokeGroup(i);

		while ( (ig = GetInvokeGroup(i)) != (invoke_group *)NULL )
		{
			PrintOutMsg("$%-9s %-10d ", SymLookup(ig->ig_sym), ig->ig_nproc);
			
			if ((strcmp(ps->s_name, "all") == 0) || (strcmp(SymLookup(ig->ig_sym), ps->s_name) == 0))
			{
				PrintOutMsg("%-5s ", "y");
			} else
			{
				PrintOutMsg("%-5s ", "n");
			}
			
			PrintOutMsg("%s\n", i->i_cmd);
		}
	}
	while ( (i = GetInvoke()) != (invoke_t *)NULL );

	return 1;
}

static int
cmd_session_cmd(rec_set_t *rec_set, int argc, char **argv)
{
	proto_session *	sess = NULL;
	char *			proto = Client.proto;
	WLM_type		manager = WLM_none;
	char *			batch_script = NULL;
	int				opt_ind = 0;
	int				opt;
	
	// reset optind
	optind = 0;
	
	//
	// Process user provided arguments
	//

	while ((opt = getopt_long(argc, argv, "q:s:e:", session_long_opts, &opt_ind)) != -1)
	{
		switch(opt)
		{
			case 0:
				// if this is a flag, do nothing
				break;
			
			case 'q':
				if (optarg == (char *)NULL)
				{
					PrintOutError("Error: --qsub provided, but missing arguments.\n");
					goto exit_with_error;
				}
				
				// Note that we want to convert any relative path name into an absolute pathname
				if ((batch_script = realpath(StripQuotes(optarg), NULL)) == (char *)NULL)
				{
					PrintOutError("Failed to resolve absolute path of provided batch script.\n");
					goto exit_with_error;
				}
				
				// set proto to proxy since we are required to use the proxy in conjunction
				// with the batch system
				proto = "proxy";
				// set the manager to qsub
				manager = WLM_qsub;
				break;
				
			case 's':
				if (optarg == (char *)NULL)
				{
					PrintOutError("Error: --sbatch provided, but missing arguments.\n");
					goto exit_with_error;
				}
				
				// Note that we want to convert any relative path name into an absolute pathname
				if ((batch_script = realpath(StripQuotes(optarg), NULL)) == (char *)NULL)
				{
					PrintOutError("Failed to resolve absolute path of provided batch script.\n");
					goto exit_with_error;
				}
				
				// set proto to proxy since we are required to use the proxy in conjunction
				// with the batch system
				proto = "proxy";
				// set the manager to sbatch
				manager = WLM_sbatch;
				break;
				
			case 'e':
				// User wants to end the session. So lets set the current session to NULL.
				// This handles cleanups for us automatically.
				if ((sess = DbgGetSession()) != NULL)
				{
					PrintOutMsg("Ending current session. Future attach/launch commands will not be associated with it.\n");
				}
				DbgSetSession(NULL);
				break;
				
			default:
				PrintOutError("Invalid session argument provided. See \"help session\" for usage.\n");
				return 1;
		}
	}
	
	// Make sure the user provided us batch arguments, otherwise return.
	if (batch_script == NULL || manager == WLM_none)
	{
		return 1;
	}
	
	// Create a new session
	if ((sess = DbgSessionCreate(proto, manager, batch_script, NULL, NULL)) == NULL)
	{
		PrintOutError("Could not create session for WLM!\n");
		goto exit_with_error;
	}
	
	// Set this session for all future launch/attach commands
	DbgSetSession(sess);
	
	PrintOutMsg("Session ready.\n");
	
	Free(batch_script);
	
	return 1;
	
exit_with_error:

	if (batch_script != NULL)
		Free(batch_script);
	
	return 0;
}

/*
** TODO: This is now called launch, but the source refers to it as invoke
** invoke $proc['['num']',...],... program [--args "prog-args"] 
**	[--using arch] [--arch-args "arch-args"] [--arch-input "inputFile"]
**	[--on host] [--with proto] [--backend debugger] [--gpu]
*/
static int 
cmd_invoke_cmd(rec_set_t *rec_set, int argc, char **argv)
{
	proto_session *	sess = NULL;
	int				nproc = 0;
	char *			prog = NULL;
	char *			arch = Client.arch;
	char *			proto = Client.proto;
	char *			backend = Client.backend;
	char *			arch_input = NULL;
	int				set_default_workdir = 0;
	char *			workdir = NULL;
	char **			env_list = NULL;
	int				env_list_num = 0;
	int				env_list_size = 0;
	char *			env_str = NULL;
	char **			args = NULL;
	char **			arch_args = NULL;
	WLM_type		manager = WLM_none;
	char *			batch_script = NULL;
	// TODO: Allow the user to specify hostname/username
	char *			hostname = NULL;
	char *			username = NULL;
	list_t *		ig = NULL;
	expr_t *		e;
	int				opt_ind = 0;
	int				opt;
	int				i;
	
	// reset optind and flags
	optind = 0;
	debug_flag = 0;
	gpu_flag = 0;

	if ( argc < 3 )
	{
		PrintOutError("Not enough command arguments.\n");
		PrintOutError("Expected: launch $handle ./program\n");
		return 0;
	}
	
	//
	// Setup the env_list copying in any environment variables set by calling
	// the set environment command.
	//
	
	if (SizeOfList(Client.set_env_list) > 1)
	{
		// there are entires in the list, process them
		SetList(Client.set_env_list);
	
		// loop through the list
		while ((env_str = (char *)GetListElement(Client.set_env_list)) != (char *)NULL)
		{
			// realloc memory if env_list_num + 1 is GEQ to env_list_size
			// note that we need at least 1 entry that acts as a null pointer
			if ((env_list_num + 1) >= env_list_size)
			{
				// increase the list by 10 entries
				env_list_size += 10;
				// BUG 801222 - ensure the list is not null
				if (env_list == NULL)
				{
					env_list = (char **)Alloc(env_list_size * sizeof(char *));
					memset(env_list, 0, env_list_size*sizeof(char *));
				} else
				{
					Resize((void *)env_list, env_list_size * sizeof(char *));
					// set the new entries to null, note that env_list_num is where the
					// new entry will be placed, the malloced entries start after this.
					memset(&env_list[env_list_num], 0, 10 * sizeof(char *));
				}
			}
				
			// set the new entry
			env_list[env_list_num++] = strdup(env_str);
		}
	}
	
	//
	// Process user provided arguments
	//

	// Note that invoke/launch used to allow the user to provide the following args:
	//
	// --arch-args --arch-input --using --on --backend --with --path
	//
	// These are no longer supported with longopts. Their use relates back to the
	// pre-MRNet days and legacy stuff. I don't think there is value in maintaining
	// these in the Cray trunk.
	
	// This will permute the non-option stuff to the end, so parse that stuff
	// last.

	while ((opt = getopt_long(argc, argv, "a:g:i:d::e:q:", launch_long_opts, &opt_ind)) != -1)
	{
		switch(opt)
		{
			case 0:
				// if this is a flag, do nothing
				break;
			case 'a':
				if (optarg == (char *)NULL)
				{
					PrintOutError("Error: --args provided, but missing arguments.\n");
					goto exit_with_error;
				}
				args = Str2Argv(StripQuotes(optarg));
				break;
				
			case 'g':
				if (optarg == (char *)NULL)
				{
					PrintOutError("Error: --aprun-args provided, but missing arguments.\n");
					goto exit_with_error;
				}
				arch_args = Str2Argv(StripQuotes(optarg));
				break;
				
			case 'q':
				if (optarg == (char *)NULL)
				{
					PrintOutError("Error: --qsub provided, but missing arguments.\n");
					goto exit_with_error;
				}
				
				// Note that we want to convert any relative path name into an absolute pathname
				if ((batch_script = realpath(StripQuotes(optarg), NULL)) == (char *)NULL)
				{
					PrintOutError("Failed to resolve absolute path of provided batch script.\n");
					goto exit_with_error;
				}
				
				// set proto to proxy since we are required to use the proxy in conjunction
				// with the batch system
				proto = "proxy";
				// set the manager to qsub
				manager = WLM_qsub;
				break;
				
			case 's':
				if (optarg == (char *)NULL)
				{
					PrintOutError("Error: --sbatch provided, but missing arguments.\n");
					goto exit_with_error;
				}
				
				// Note that we want to convert any relative path name into an absolute pathname
				if ((batch_script = realpath(StripQuotes(optarg), NULL)) == (char *)NULL)
				{
					PrintOutError("Failed to resolve absolute path of provided batch script.\n");
					goto exit_with_error;
				}
				
				// set proto to proxy since we are required to use the proxy in conjunction
				// with the batch system
				proto = "proxy";
				// set the manager to sbatch
				manager = WLM_sbatch;
				break;
		
			case 'i':
				if (optarg == (char *)NULL)
				{
					PrintOutError("Error: --aprun-input provided, but missing arguments.\n");
					goto exit_with_error;
				}
				arch_input = strdup(StripQuotes(optarg));
				break;
		
			case 'd':
				if (optarg == (char *)NULL)
				{
					// --workdir provided without argument, so default to chdir to
					// the directory that the binary is located in.
					// We need to post process this since the name of the a.out
					// is not known yet.
					++set_default_workdir;
				} else
				{
					// Note that we want to convert any relative path name into an absolute pathname
					if ((workdir = realpath(StripQuotes(optarg), NULL)) == (char *)NULL)
					{
						PrintOutError("Error: --workdir provided, but failed to resolve its absolute path.\n");
						goto exit_with_error;
					}
				}
				break;
			
			case 'e':
				if (optarg == (char *)NULL)
				{
					PrintOutError("Error: --env provided, but missing arguments.\n");
					goto exit_with_error;
				}
				
				// realloc memory if env_list_num + 1 is GEQ to env_list_size
				// note that we need at least 1 entry that acts as a null pointer
				if ((env_list_num + 1) >= env_list_size)
				{
					// increase the list by 10 entries
					env_list_size += 10;
					// BUG 801222 - ensure the list is not null
					if (env_list == NULL)
					{
						env_list = (char **)Alloc(env_list_size * sizeof(char *));
						memset(env_list, 0, 10*sizeof(char *));
					} else
					{
						Resize((void *)env_list, env_list_size * sizeof(char *));
						// set the new entries to null, note that env_list_num is where the
						// new entry will be placed, the malloced entries start after this.
						memset(&env_list[env_list_num], 0, 10 * sizeof(char *));
					}
				}
				
				// set the new entry
				env_list[env_list_num++] = strdup(StripQuotes(optarg));
				break;
		
			default:
				PrintOutError("Invalid launch argument provided. See \"help launch\" for usage.\n");
				goto exit_with_error;
		}
	}
	
	// Ensure the user provided two non-option arguments
	if (argc-optind != 2)
	{
		PrintOutError("Invalid number of non-option arguments. See \"help launch\" for usage.\n");
		PrintOutError("Expected 2, found %d.\n", argc-optind);
		goto exit_with_error;
	}
	
	// parse the non-option arguments

	// The first argument should be the name of the process set
	if ( (e = ParseExpr(argv[optind++])) == (expr_t *)NULL )
	{
		PrintError();
		goto exit_with_error;
	}

	ig = NewList();

	if ( (nproc = CreateInvokeGroupList(e, ig, rec_set)) < 0 )
	{
		PrintError();
		FreeExpr(e);
		goto exit_with_error;
	}

	FreeExpr(e);
	
	// The second argument is the name of the a.out

	// Note that we want to convert any relative path name into an absolute pathname
	if ((prog = realpath(argv[optind++], NULL)) == (char *)NULL)
	{
		PrintOutError("Failed to resolve absolute path of provided application.\n");
		goto exit_with_error;
	}
	
	// Process any arguments from above that were dependent on the non-option
	// arguments
	
	if (set_default_workdir)
	{
		// --workdir provided without argument, so default to chdir to
		// the directory that the binary is located in.
		char *tmp;
		char *ptr;
					
		// We want to point at the final "/" in the string
		tmp = strdup(prog);
		ptr = strrchr(tmp, '/');
		
		if (ptr != NULL)
		{
			*ptr = '\0';
			workdir = strdup(tmp);
		} else
		{
			// No path provided, so we can't chdir anywhere. Silently ignore the argument
			// This should never happen since prog points at the realpath of the app
			workdir = NULL;
		}
					
		Free(tmp);
	}
	
	// Verify that the arguments are correct

	if ( arch == NULL )
	{
		PrintOutError("Must specify an MP architecure.\n");
		goto exit_with_error;
	}

	if ( proto == NULL )
	{
		PrintOutError("Must specify a protocol.\n");
		goto exit_with_error;
	}
	
	// Check to see if we need to handle a session for this invoke
	switch (manager)
	{
		struct stat sb;
	
		case WLM_none:
			// First check to see if we currently have an open session
			if ((sess = DbgGetSession()) != NULL)
			{
				// There currently is an open session, so use it in the invoke
				proto = "proxy";
				break;
			}
		
			// If on a Cray system, check to see if we are currently on an eslogin node if
			// using WLM_none.
			//
			// If so, we want to create a new session on the service node using the proxy
			// protocol and forward every command through that.
			
			// Are we on an eslogin node?
			if (stat("/proc/cray_xt/nid", &sb) == -1)
			{
				// We are on an eslogin node since the file did not exist
				char * tmp;
				
				// set proto to proxy since we are required to use the proxy in conjunction
				// with eslogin nodes
				proto = "proxy";
				
				// Get the value of the ESWRAP_LOGIN environment variable
				tmp = getenv("ESWRAP_LOGIN");
				if (tmp != NULL)
				{
					hostname = strdup(tmp);
				}
				
				// Create a new session
				if ((sess = DbgSessionCreate(proto, manager, NULL, hostname, username)) == NULL)
				{
					PrintOutError("Could not create session for eslogin!\n");
					goto exit_with_error;
				}
				
				// Set this session for all future launch/attach commands
				DbgSetSession(sess);
			}
			
			// There is no current session and we are not on an eslogin node, so we can use
			// the default protocol.
			
			break;
			
		case WLM_qsub:
		case WLM_sbatch:
		
			// These sessions are only tied to this launch command and are not set for
			// future use. Aka they are temporary in nature and get cleaned up when
			// the dbginvoke gets cleaned up.
			
			// Create a new session
			if ((sess = DbgSessionCreate(proto, manager, batch_script, NULL, NULL)) == NULL)
			{
				PrintOutError("Could not create session for WLM!\n");
				goto exit_with_error;
			}
			
			break;
	}
	
	//
	// Invoke the application
	//
	if ( !DoInvoke(proto, sess, prog, args, nproc, ig, arch, arch_args, arch_input, backend, env_list, workdir) )
	{
		// If XML mode is enabled, we want to print out a removed message. 
		if (Client.XML_mode)
		{
			invoke_group *	inv;
			char *			proc_name;
		
			SetList(ig);

			/* Print a removed message for each invoke_group */
			while ( (inv = (invoke_group *)GetListElement(ig)) != (invoke_group *)NULL )
			{
				proc_name = SymLookup(inv->ig_sym);
			
				XMLPrintRemovedMsg(proc_name);
			}
		}
		goto exit_with_error;
	}

	// normal return
	
	Free(prog);
	FreeArgv(args);
	FreeArgv(arch_args);
	if (batch_script != NULL)
		Free(batch_script);
	if (hostname != NULL)
		Free(hostname);
	if (username != NULL)
		Free(username);
	if (arch_input != NULL)
		Free(arch_input);
	if (workdir != NULL)
		Free(workdir);
	if (env_list != NULL)
	{
		i = 0;
		while(env_list[i] != NULL)
			Free(env_list[i++]);
		Free(env_list);
	}

	return 1;
	
exit_with_error:

	if (ig != NULL)
		FreeInvokeGroupList(ig);
	FreeArgv(args);
	FreeArgv(arch_args);
	if (prog != NULL)
		Free(prog);
	if (batch_script != NULL)
		Free(batch_script);
	if (hostname != NULL)
		Free(hostname);
	if (username != NULL)
		Free(username);
	if (arch_input != NULL)
		Free(arch_input);
	if (workdir != NULL)
		Free(workdir);
	if (env_list != NULL)
	{
		i = 0;
		while(env_list[i] != NULL)
			Free(env_list[i++]);
		Free(env_list);
	}
	
	return 0;
}

/*
** Invoke helper routine. Invoke one or more processes.
** Note that the invoke process always operates synchronously.
*/
static int
DoInvoke(	char *proto,
	 		proto_session *sess,
	 		char *prog, 
	 		char **args, 
	 		int nproc, 
	 		list_t *igl,
	 		char *arch, 
	 		char **arch_args, 
	 		char *arch_input,
	 		char *backend,
	 		char **Env,
	 		char *cwd
)
{
	int					c;
	int					n;
	int					offset;
	dbginvoke_t *		di;
	invoke_t *			i;
	invoke_group *		ig;
	dbgprocset_t *		p = NULL;
	dbgframe_t *		f;
	rec_set_t *			rec_set;
	group_reply_t *		reply;
	rep_set_t *			rep_set;
	rep_set_t *			quit_rep;
	bitset *			tmp_bset;
	bitset *			bs_group;
	char *				proc_name;
	int					p2pmode=0;
	
	// Call the invoke in the dbg_client layer
	
	di = DbgInvoke(proto, sess, prog, args, nproc, arch, arch_args, arch_input, backend, Env, cwd, gpu_flag, debug_flag);

	if ( di == NULL )
	{
		// Do not print out an error message here since the lower layer already
		// printed it for us.
		return 0;
	}
	
	// This is used below for dbg_client calls
	rec_set = NewRecSet(NULL, NULL, di, di->d_proto_net, di->d_bitset_full, -1);
	
	rep_set = DbgStartup(rec_set);
	
	//process error
	if (ProcessErrorInRepSet(rec_set, rep_set))
	{
		PrintOutError("Could not startup application.\n");
		quit_rep = DbgQuit(rec_set, PROC_CREATED);
		ProcessErrorInRepSet(rec_set, quit_rep);
		FreeRecSet(rec_set);
		FreeRepSet(quit_rep);
 		FreeRepSet(rep_set);
		return 0;
	}

	i = AddInvoke(prog, igl, arch, di, di->d_proto_net, di->d_bitset_full, PROC_CREATED);

	SetList(igl);

	/*Create a procset for each invoke_group*/
	while ( (ig = (invoke_group *)GetListElement(igl)) != (invoke_group *)NULL )
	{
		bs_group = bitset_new(nproc);
		proc_name = SymLookup(ig->ig_sym);
		
		p = NewProcSet(SymLookup(ig->ig_sym));

		c = 0;
		offset = 0;

		// adjust the bitset based on the offset
		for(n=ig->ig_offset; n < (ig->ig_offset + ig->ig_nproc); n++)
		{
			bitset_set(bs_group, n);
			c++;
		}

		// Set the client event callback handler
		DbgHandleEvent(di, EventCB);

		AddOverlaySetToProcSet(p, i, ig, bs_group, ig->ig_offset, c, ig->ig_nproc, DBGSET_INVOKE);
		
		// Print the startup frames based on this bitset
		SetReplyHeadInSet(rep_set);
		while((reply = GetNextReplyInSet(rep_set)) != (group_reply_t *)NULL)
		{
			// AND the reply set with the calculated bs_group
			tmp_bset = bitset_and(reply->rep_group, bs_group);
			
			// if the calculated bitset is empty, continue
			if (bitset_isempty(tmp_bset))
			{
				bitset_free(tmp_bset);
				continue;
			}
		
			// point at the frame
			f = EVENT_FRAME(reply->rep_event);

			// print out the message based on the proc_name string and the calculated bitset
			if ( f->frame_loc.loc_line > 0 )
			{
				PrintMsgWithSetInfo(Client.cli_out_stream, proc_name, tmp_bset, "Initial breakpoint, %s at %s:%d\n",
									f->frame_loc.loc_func,
									f->frame_loc.loc_file,
									f->frame_loc.loc_line);
			} else
			{
				PrintMsgWithSetInfo(Client.cli_out_stream, proc_name, tmp_bset, "Initial breakpoint, in %s\n",
									f->frame_loc.loc_func);
			}
			
			// If XML mode is enabled, we want to print out a status message. 
			// We don't need to ask the backend for the status since this is 
			// implied by printing out the initial breakpoint.
			if (Client.XML_mode)
			{
				char *	set_str;
				char *	status_block;
				
				status_block = XMLStartStatusBlock(proc_name);
				
				// Create the full set string
				Asprintf(&set_str, "%s%s", proc_name, bitset_to_set(tmp_bset));
				XMLAddStatus(&status_block, set_str, DBGSTAT_STOPPED, &(f->frame_loc));
				Free(set_str);
				
				XMLPrintStatusBlock(status_block);
			}
			
			// free the calculated bitset
			bitset_free(tmp_bset);
		}
		
		// Free the adjusted bitset
		bitset_free(bs_group);
		
 		CreateAsyncInfoForTree(di->d_proto_net);
 		
 		// Set the created procset to the val symbol for the invoke group.
 		if (SetProcsetForInvokeGroup(ig, p))
 		{
 			quit_rep = DbgQuit(rec_set, PROC_CREATED);
 			ProcessErrorInRepSet(rec_set, quit_rep);
 			FreeRecSet(rec_set);
 			FreeRepSet(quit_rep);
 			FreeRepSet(rep_set);
 			return 0;
 		}
	}
	
	// Done with the startup reply
	FreeRepSet(rep_set);

	GetGraphVar(GRAPH_P2PMODE, (int *)&p2pmode);

	// for P2P here
	if (p2pmode)
	{
		// FIXME: andrewg: the getenv call here can fail. This needs to be
		//        fixed. When the p2p is fixed to actually ship the dbglog_netinfo
		//        file, this entire thing should be removed.
		
		// now, rename the file dbg_netinfo --> dbg_netinfo_<procset_name> file
		char filename[BUFSIZ];
		char filerename[BUFSIZ];
		char *filepath;
		filepath = getenv("CRAY_DBG_LOG_DIR");
		sprintf(filename, "%s/dbglog_netinfo_%s", filepath, p->s_name);
		sprintf(filerename, "%s/dbglog_netinfo.0.log", filepath);
		if (rename(filerename, filename) < 0)
		{
			PrintOutError("Error in renaming file.");
		}
	}
	// end
	
	// Free the created rec_set
	FreeRecSet(rec_set);

	return 1;
}

/*
** attach $proc apid [--gpu] [--debug]
*/
static int 
cmd_attach_cmd(rec_set_t *rec_set, int argc, char **argv)
{
	proto_session *	sess = NULL;
	int				nproc = 0;
	int				pid;
	char *			arch = Client.arch;
	char *			proto = Client.proto;
	char *			backend = Client.backend;
	char *			hostname = NULL;
	char *			username = NULL;
	list_t *		ig;
	expr_t *		e;
	int				opt_ind = 0;
	int				opt;
	struct stat 	sb;
	// reset optind and flags
	optind = 0;
	debug_flag = 0;
	gpu_flag = 0;

	if ( argc < 3 )
	{
		PrintOutError("Not enough command arguments.\n");
		PrintOutError("Expected: attach $handle <pid>\n");
		return 0;
	}
	
	//
	// Process user provided arguments
	//
	// This will permute the non-option stuff to the end, so parse that stuff
	// last.
	
	while ((opt = getopt_long(argc, argv, "", attach_long_opts, &opt_ind)) != -1)
	{
		switch(opt)
		{
			case 0:
				// if this is a flag, do nothing
				break;
			default:
				PrintOutError("Invalid attach argument provided. See \"help attach\" for usage.\n");
				return 0;
		}
	}
	
	// Ensure the user provided two non-option arguments
	if (argc-optind != 2)
	{
		PrintOutError("Invalid number of non-option arguments. See \"help attach\" for usage.\n");
		PrintOutError("Expected 2, found %d.\n", argc-optind);
		return 0;
	}
	
	// parse the non-option arguments

	// The first argument should be the name of the process set

	ig = NewList();

	if ( (e = ParseExpr(argv[optind++])) == (expr_t *)NULL )
	{
		PrintError();
		return 0;
	}

	if ( (nproc = CreateInvokeGroupList(e, ig, rec_set)) < 0 )
	{
		PrintError();
		FreeExpr(e);
		return 0;
	}

	FreeExpr(e);

	pid = atoi(argv[optind++]);
	
	// Verify that the arguments are correct

	if ( proto == NULL )
	{
		PrintOutError("Must specify a protocol.\n");
		FreeInvokeGroupList(ig);
		return 0;
	}
	
	// If on a Cray system, check to see if we are currently on an eslogin node if
	// using WLM_none.
	//
	// If so, we want to create a new session on the service node using the proxy
	// protocol and forward every command through that.
	//
	// Are we on an eslogin node?
	if (stat("/proc/cray_xt/nid", &sb) == -1)
	{
		// We are on an eslogin node since the file did not exist
		char * tmp;
		
		// set proto to proxy since we are required to use the proxy in conjunction
		// with eslogin nodes
		proto = "proxy";
		
		// First check to see if we currently have an open session
		if ((sess = DbgGetSession()) == NULL)
		{
			// There is no open session, so we need to create one.
			
			// Get the value of the ESWRAP_LOGIN environment variable
			tmp = getenv("ESWRAP_LOGIN");
			if (tmp != NULL)
			{
				hostname = strdup(tmp);
			}
		
			// Create a new session
			if ((sess = DbgSessionCreate(proto, WLM_none, NULL, hostname, username)) == NULL)
			{
				PrintOutError("Could not create session for eslogin!\n");
				FreeInvokeGroupList(ig);
				return 0;
			}
		
			// Set this session for all future launch/attach commands
			DbgSetSession(sess);
		}
	}
	
	if ( !DoAttach(proto, sess, pid, ig, arch, backend) )
	{
		FreeInvokeGroupList(ig);
		return 0;
	}

	return 1;
}

/*
** Invoke helper routine. Invoke one or more processes.
** Note that the attach process always operates synchronously.
*/
static int
DoAttach(	char *proto,
			proto_session *sess,
	 		int	pid, 
	 		list_t *igl,
	 		char *arch, 
	 		char *backend
)
{
	int					c;
	int					n;
	int					offset;
	dbginvoke_t *		di;
	invoke_t *			i;
	invoke_group *		ig;
	dbgprocset_t *		p = NULL;
	dbgframe_t *		f;
	dbgframe_t *		fp;
	rec_set_t *			rec_set;
	group_reply_t *		reply;
	rep_set_t *			rep_set;
	rep_set_t *			quit_rep;
	bitset *			tmp_bset;
	bitset *			bs_group;
	char *				proc_name;
	int					p2pmode=0;
	
	// Note that we are reusing the invoke callback.
	di = DbgAttach(proto, sess, pid, arch, backend, gpu_flag, debug_flag);

	if ( di == NULL )
	{
		PrintOutError("Could not attach to application.\n");
		return 0;
	}
	
	// This is used below for dbg_client calls
	rec_set = NewRecSet(NULL, NULL, di, di->d_proto_net, di->d_bitset_full, -1);
	
	// Ensure the igl list has only one entry
	// FIXME: This shouldn't be how this works, its a hack for now to ensure 
	// the fixup can work, the fixup grabs the first ig and sets it to di->d_nprocs
	if (SizeOfList(igl) != 1)
	{
		PrintOutError("\nToo many procset handles given to attach command.\n");
		PrintOutError("Expected only one handle such as $a.\n");
		quit_rep = DbgQuit(rec_set, PROC_ATTACHED);
		ProcessErrorInRepSet(rec_set, quit_rep);
		FreeRecSet(rec_set);
		FreeRepSet(quit_rep);
		return 0;
	}
	
	//fix the igl to be the size of di->d_nprocs 
	//TODO: Add ability to attach to subset of ranks 
	SetList(igl);
	
	if ( (ig = (invoke_group *)GetListElement(igl)) != (invoke_group *)NULL )
	{
		ig->ig_nproc = di->d_nproc;
	}
	
	// grab all the frames
	rep_set = DbgShowFrame(rec_set, 0);

	//process error
	if (ProcessErrorInRepSet(rec_set, rep_set))
	{
		// This is fatal as its an indication that things are broken (i.e. we cannot send commands)
		PrintOutError("Could not communicate with application.\n");
		quit_rep = DbgQuit(rec_set, PROC_CREATED);
		ProcessErrorInRepSet(rec_set, quit_rep);
		FreeRecSet(rec_set);
		FreeRepSet(quit_rep);
 		FreeRepSet(rep_set);
		return 0;
	}

	i = AddInvoke(NULL, igl, NULL, di, di->d_proto_net, di->d_bitset_full, PROC_ATTACHED);

	// We print the frames below, but we need to print this message only once, so we do it here
	PrintOutMsg("Current rank location:\n");

	SetList(igl);
	/*Create a procset for each invoke_group*/
	while ( (ig = (invoke_group *)GetListElement(igl)) != (invoke_group *)NULL )
	{
		bs_group = bitset_new(ig->ig_nproc); 
		proc_name = SymLookup(ig->ig_sym);
		p = NewProcSet(proc_name);

		c = 0;
		offset = 0;

		for(n=ig->ig_offset; n < (ig->ig_offset + ig->ig_nproc); n++)
		{
			bitset_set(bs_group, n);
			c++;
		}

		// Set the client event callback handler
		DbgHandleEvent(di, EventCB);

		AddOverlaySetToProcSet(p, i, ig, bs_group, ig->ig_offset, c, ig->ig_nproc, DBGSET_INVOKE);
		
		// Print the current frame based on this bitset
		// Print the startup frames based on this bitset
		SetReplyHeadInSet(rep_set);
		while((reply = GetNextReplyInSet(rep_set)) != (group_reply_t *)NULL)
		{
			// AND the reply set with the calculated bs_group
			tmp_bset = bitset_and(reply->rep_group, bs_group);
			
			// if the calculated bitset is empty, continue
			if (bitset_isempty(tmp_bset))
			{
				bitset_free(tmp_bset);
				continue;
			}
		
			// point at the frame
			fp = EVENT_FRAME(reply->rep_event);
			
			// print the frame
			for (f=fp; f != (dbgframe_t *)NULL ; f = f->frame_next)
			{
				PrintFrameSet(proc_name, tmp_bset, f);
			}
			
			// If XML mode is enabled, we want to print out a status message. 
			// We don't need to ask the backend for the status since this is 
			// implied by printing out the initial breakpoint.
			if (Client.XML_mode)
			{
				char *	set_str;
				char *	status_block;
				
				status_block = XMLStartStatusBlock(proc_name);
				
				// Create the full set string
				Asprintf(&set_str, "%s%s", proc_name, bitset_to_set(tmp_bset));
				// Set the location based on the top frame
				XMLAddStatus(&status_block, set_str, DBGSTAT_STOPPED, (fp == NULL) ? NULL : &(fp->frame_loc));
				Free(set_str);
				
				XMLPrintStatusBlock(status_block);
			}
			
			// free the calculated bitset
			bitset_free(tmp_bset);
		}
		
		// Free the adjusted bitset
		bitset_free(bs_group);
		
 		CreateAsyncInfoForTree(di->d_proto_net);
 		
 		// Set the created procset to the val symbol for the invoke group.
 		if (SetProcsetForInvokeGroup(ig, p))
 		{
 			quit_rep = DbgQuit(rec_set, PROC_ATTACHED);
 			ProcessErrorInRepSet(rec_set, quit_rep);
 			FreeRecSet(rec_set);
 			FreeRepSet(quit_rep);
 			FreeRepSet(rep_set);
 			return 0;
 		}
	}
	
	// Done with the frames reply
	FreeRepSet(rep_set);
	
	GetGraphVar(GRAPH_P2PMODE, (int *)&p2pmode);

	// for P2P here
	if (p2pmode)
	{
		// FIXME: andrewg: the getenv call here can fail. This needs to be
		//        fixed. When the p2p is fixed to actually ship the dbglog_netinfo
		//        file, this entire thing should be removed.
		
		// now, rename the file dbg_netinfo --> dbg_netinfo_<procset_name> file
		char filename[BUFSIZ];
		char filerename[BUFSIZ];
		char *filepath;
		filepath = getenv("CRAY_DBG_LOG_DIR");
		sprintf(filename, "%s/dbglog_netinfo_%s", filepath, p->s_name);
		sprintf(filerename, "%s/dbglog_netinfo.0.log", filepath);
		if (rename(filerename, filename) < 0)
		{
			PrintOutError("Error in renaming file.");
		}
	}
	// end
	
	// Free the created rec_set
	FreeRecSet(rec_set);

	return 1;
}

static int 
cmd_halt_cmd(rec_set_t *rec_set, int argc, char **argv)
{
	rep_set_t *		rep_set;
	group_reply_t *	reply;
	dbgloc_t *		halt_loc;
	
	rep_set = DbgInterrupt(rec_set);
	
	//process error
	ProcessErrorInRepSet(rec_set, rep_set);
	
	//process location
	while((reply = GetNextReplyInSet(rep_set)) != (group_reply_t *)NULL)
	{
		// point at location data
		halt_loc = EVENT_LOC(reply->rep_event);
		PrintHaltedLocationSet(rec_set->s_setname, reply->rep_group, halt_loc);
	}

	FreeRepSet(rep_set);

	return 1;
}

// external exposure
void
PrintHaltedLocationSet(char *setname, bitset *group, dbgloc_t *loc)
{
	if (loc->loc_line > 0)
	{
		PrintMsgWithSetInfo(Client.cli_out_stream, setname, group, "Application halted in %s at %s:%d\n", 
			loc->loc_func,
			loc->loc_file,
			loc->loc_line
			);
	} else
	{
		PrintMsgWithSetInfo(Client.cli_out_stream, setname, group, "Application halted in %s\n", 
			loc->loc_func
			);
	}
}

static int 
cmd_kill_cmd(rec_set_t *rec_set, int argc, char **argv)
{
	char *			s;
	expr_t *		e;
	val_t *			val;
	invoke_group *	ig;
	
	if ( argc < 2 )
	{
		PrintOutError("Not enough command arguments.\n");
		PrintOutError("Expected: kill $proc\n");
		return 0;
	}
	
	s = argv[1];
	
	if ( (e = ParseExpr(s)) == (expr_t *)NULL )
	{
		PrintError();
		return 0;
	}
	
	if ( EvalExpr(&val, e, rec_set) < 0 )
	{
		PrintError();
		FreeExpr(e);
		return 0;
	}
	
	FreeExpr(e);

	if ( RVAL_ACTUAL_TYPE(val) != rval_proc )
	{
		PrintOutError("Invalid process set argument.\n");
		DestroyVal(val);
		return 0;
	}

	if ( (ig = FindInvokeGroup(LVAL_SYM(val))) != (invoke_group *)NULL )
	{
		// We always want to kill, not detach, the application when the user
		// issues the kill command.
		ReleaseInv(ig->ig_invoke, PROC_CREATED);
	}

	return 1;
}

static int 
cmd_release_cmd(rec_set_t *rec_set, int argc, char **argv)
{
	char *			s;
	expr_t *		e;
	val_t *			val;
	invoke_group *	ig;

	if ( argc < 2 )
	{
		PrintOutError("Not enough command arguments.\n");
		PrintOutError("Expected: release $proc\n");
		return 0;
	}

	s = argv[1];

	if ( (e = ParseExpr(s)) == (expr_t *)NULL )
	{
		PrintError();
		return 0;
	}

	if ( EvalExpr(&val, e, rec_set) < 0 )
	{
		PrintError();
		FreeExpr(e);
		return 0;
	}

	FreeExpr(e);

	if ( RVAL_ACTUAL_TYPE(val) != rval_proc )
	{
		PrintOutError("Invalid process set argument.\n");
		DestroyVal(val);
		return 0;
	}

	if ( (ig = FindInvokeGroup(LVAL_SYM(val))) != (invoke_group *)NULL )
	{
		// We always want to detach, not kill, the application when the user
		// issues the release command.
		ReleaseInv(ig->ig_invoke, PROC_ATTACHED);
	}

	return 1;
}

static void
ReleaseInv(invoke_t *i, int detach)
{
	dbgprocset_t *	ps;
	invoke_group *	ig;
	dbginvoke_t *	di;
	rec_set_t *		rec_set;
	rep_set_t *		rep_set;

	/*
	** Tell server to quit.
	*/
	DPRINT("Quitting invoke[%p]\n",i);
	
	SetInvokeGroup(i);
	
	while ( (ig = GetInvokeGroup(i)) != (invoke_group *)NULL )
	{
		if (detach)
			PrintOutMsg("Shutting down debugger and resuming application for \'%s\'.\n", SymLookup(ig->ig_sym));
		else
			PrintOutMsg("Shutting down debugger and killing application for \'%s\'.\n", SymLookup(ig->ig_sym));
	}

	// Grab the dbginvoke_t from the i_iexec - note that this should be fixed its a hack
	// somewhere that I haven't fully understood yet (andrewg)
	di = (dbginvoke_t *)i->i_iexec;
	
	// Create the rec_set based on the di
	rec_set = NewRecSetByDbgInvoke(di, di->d_proto_net, di->d_bitset_full, -1);

	rep_set = DbgQuit(rec_set, detach);

	ProcessErrorInRepSet(rec_set, rep_set);

	FreeRecSet(rec_set);
	FreeRepSet(rep_set);

	SetInvokeGroup(i);

	while ( (ig = GetInvokeGroup(i)) != (invoke_group *)NULL )
	{
		/*
		** Remove procset entry.
		*/
		if ( (ps = FindProcSet(SymLookup(ig->ig_sym))) != (dbgprocset_t *)NULL )
		{
			if ( ps == CurrentProcSet() )
				SetCurrentProcSet((dbgprocset_t *)NULL);

			FreeProcSet(ps);
		}
	}

	/*
	** Cleanup invoke information.
	*/
	DestroyAsyncInfoForTree(i->i_tree);
	FreeInvoke(i);
}

// external exposure
void
EndAll(void)
{
	invoke_t *	i;
	
	SetInvoke();
	while ((i = GetInvoke()) != (invoke_t *)NULL)
		ReleaseInv(i, i->i_attached);
	
	DestroyAllAsyncInfo();
}

/*
** IMPORTANT: This function is now only used to "force kill" an application
** in an ungraceful way for ALPS systems. It should not be used except for 
** error situations.
*/
static void
KillInv(invoke_t *i)
{
	dbgprocset_t *		ps;
	invoke_group *		ig;
	dbg_over_set_t *	over_set = NULL;
	rec_set_t *			rec_set;
	rep_set_t *			rep_set;
	
	/*
	** Force the server to exit.
	*/
	DPRINT("Killing invoke[%p]\n",i);
		
	SetInvokeGroup(i);
	
	while ((ig = GetInvokeGroup(i)) != (invoke_group *)NULL)
	{
		/*
		** Remove procset entry.
		*/
		if ( (ps = FindProcSet(SymLookup(ig->ig_sym))) != (dbgprocset_t *)NULL )
		{
			// first we need to get the overlay set and then make a rec set from it so
			// that we can kill it. This process is similar to whats found in nodes.c.
			// I am totally confused about the naming scheme...
			if ((over_set = GetOverlaySetForProcSet(ps)) != (dbg_over_set_t *)NULL)
			{
				rec_set = NewRecSetByOverSet(over_set, SetIndividualProcess(over_set->s_set_bs, 0, ps->s_indiv_proc), ps->s_indiv_proc);
				
				// now kill it
				if ((rep_set = DbgKill(rec_set)) != NULL)
				{
					ProcessErrorInRepSet(rec_set, rep_set);
					FreeRepSet(rep_set);
					PrintOutError("Unable to kill launch group! Manually ensure all applications have been killed!\n");
				}
				
				FreeRecSet(rec_set);
			}
		
			if ( ps == CurrentProcSet() )
				SetCurrentProcSet((dbgprocset_t *)NULL);

			FreeProcSet(ps);
		}
		
		/*
		** Process variable removed by FreeInvoke().
		*/
	}

	/*
	** Cleanup invoke information.
	*/
	DestroyAsyncInfoForTree(i->i_tree);
	FreeInvoke(i);
}

/*
** IMPORTANT: This function is now only used to "force kill" an application
** in an ungraceful way for ALPS systems. It should not be used except for 
** error situations.
** external exposure
*/
void
KillAll(void)
{
	invoke_t *	i;
	
	SetInvoke();
	while ((i = GetInvoke()) != (invoke_t *)NULL)
		KillInv(i);
	
	DestroyAllAsyncInfo();
}

void
InitCmd_proc(void)
{
	// info commands
	AddInfoCommand("application", proc_ignore, 0, class_info_subcmd_application_cmd,
		class_info_subcmd_application_help, "Show attached/launched applications under debugger control.");
		
	// regular commands
	AddCommand("session", NULL, proc_ignore, 0, cmd_session_cmd,
		cmd_session_help, "Create a new WLM session.");
	AddCommand("launch", NULL, proc_ignore, 0, cmd_invoke_cmd,
		cmd_invoke_help, "Launch an application.");
	AddCommand("attach", NULL, proc_ignore, 0, cmd_attach_cmd,
		cmd_attach_help, "Attach to an application.");
	AddCommand("halt", NULL, proc_required, 0, cmd_halt_cmd,
		cmd_halt_help, "Halt execution of attached/launched application.");
	AddCommand("release", NULL, proc_ignore, 0, cmd_release_cmd,
		cmd_release_help, "Release attached/launched application from debugger control.");
	AddCommand("kill", NULL, proc_ignore, 0, cmd_kill_cmd,
		cmd_kill_help, "Kill attached/launched application.");
}

