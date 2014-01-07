/*
** Breakpoint commands.
**
** Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
**
*/

#ifdef HAVE_CONFIG_H
#include	<config.h>
#endif /* HAVE_CONFIG_H */

#include	<stdio.h>
#include	<stdlib.h>
#include	<stdarg.h>
#include	<signal.h>
#include	<string.h>

#include	"client.h"
#include	"useful.h"

static int 
cmd_break_cmd(rec_set_t *rec_set, int argc, char **argv)
{
	char *		s;
	char *		file;
	char *		arg;
	dbg_over_set_t* over_set = rec_set->s_over_set;
	rep_set_t *	rep_set = NULL;
	group_reply_t *	reply = NULL;
	bp_set_t*	bpset;
	dbgbp_t *	bpi;
	int			bp_id;

	if ( argc < 2 )
	{
		/* get current loc */
		PrintOutError("Not enough command arguments.\n");
		PrintOutError("Expected: break main.c:20\n");
		return 1;
	}

	if ( argc > 2 )
	{
		PrintOutError("Too many command arguments.\n");
		PrintOutError("Expected: break main.c:20\n");
		return 0;
	}

	arg = strdup(argv[1]);

	if ( (s = strchr(arg, ':')) != NULL )
	{
		file = arg;
		*s++ = '\0';
	}
	else
	{
		file = NULL;
		s = arg;
	}

	DPERFLOG(SetTimerStart("setbreak"));

	if ( IsInteger(s) )
	{
		rep_set = DbgSetLineBreak(rec_set, file, atoi(s));
	} 
	else
	{
		rep_set = DbgSetFuncBreak(rec_set, file, s);
	}

	DPERFLOG(SetTimerStopAndLog("setbreak"));

	Free(arg);

	ProcessErrorInRepSet(rec_set, rep_set);
	
	while((reply = GetNextReplyInSet(rep_set)) != (group_reply_t*)NULL)
	{
		// point at the breakpoint data
		bpi = EVENT_BP(reply->rep_event);
		
		AddBP(rep_set->tree, bpi, reply->rep_group);
		bpset = FindBPSetByBP(rep_set->tree, bpi, &bp_id);

		if(bpset == (bp_set_t *)NULL)//this is not supposed to happen
		{
			PrintMsgWithSetInfoByOffset(Client.cli_err_stream, rec_set->s_setname, reply->rep_group, over_set->s_offset, over_set->s_len, 
				"ERROR: cannot find breakpoint [%d: file %s, line %d] in BPSetList of tree[%p].\n",
				bpi->bp_id, 
				bpi->bp_loc.loc_file, 
				bpi->bp_loc.loc_line, 
				rep_set->tree);
			return 0;
		}

		if ( bpset->b_loc_line >= 0 )
			PrintMsgWithSetInfoByOffset(Client.cli_out_stream, rec_set->s_setname, reply->rep_group, over_set->s_offset, over_set->s_len, 
				"Breakpoint %d: file %s, line %d.\n",
				bp_id, 
				bpset->b_loc_file, 
				bpset->b_loc_line);
		else
			PrintMsgWithSetInfoByOffset(Client.cli_out_stream, rec_set->s_setname, reply->rep_group, over_set->s_offset, over_set->s_len, 
				"Breakpoint %d in %s\n",
				bp_id, 
				bpset->b_loc_func, 
				bpset->b_loc_line);
	}

	FreeRepSet(rep_set);

	return 1;
}

static int 
cmd_tbreak_cmd(rec_set_t *rec_set, int argc, char **argv)
{
	char *				s;
	char *				file;
	char *				arg;
	dbg_over_set_t *	over_set = rec_set->s_over_set;
	rep_set_t *			rep_set;
	rep_set_t *			enb_rep;
	group_reply_t *		reply = NULL;
	bp_set_t *			bpset;
	dbgbp_t *			bpi;
	int					bp_id = 0;
	rec_set_t *			new_rec;
	
	if ( argc < 2 )
	{
		/* get current loc */
		PrintOutError("Not enough command arguments.\n");
		PrintOutError("Expected: tbreak main.c:20\n");
		return 1;
	}

	if ( argc > 2 )
	{
		PrintOutError("Too many command arguments.\n");
		PrintOutError("Expected: tbreak main.c:20\n");
		return 0;
	}

	arg = strdup(argv[1]);

	if ( (s = strchr(arg, ':')) != NULL )
	{
		file = arg;
		*s++ = '\0';
	}
	else
	{
		file = NULL;
		s = arg;
	}

	DPERFLOG(SetTimerStart("settbreak"));

	// Set the breakpoint
	if ( IsInteger(s) )
	{
		rep_set = DbgSetLineBreak(rec_set, file, atoi(s));
	} 
	else
	{
		rep_set = DbgSetFuncBreak(rec_set, file, s);
	}

	Free(arg);

	// process the replies
	ProcessErrorInRepSet(rec_set, rep_set);

	while((reply = GetNextReplyInSet(rep_set)) != (group_reply_t*)NULL)
	{
		// point at the breakpoint data
		bpi = EVENT_BP(reply->rep_event);

		// Now set this to a temporary breakpoint using the enable command for this reply group
		new_rec = NewRecSet(rec_set->s_setname, rec_set->s_invoke_handler, rec_set->s_dbginvoke_handler, rec_set->s_proto_handler, reply->rep_group);
		
		if ((enb_rep = DbgEnableBreak(new_rec, bpi->bp_id, 0, 1)) != NULL)
		{
			ProcessErrorInRepSet(new_rec, enb_rep);
			PrintOutError("ERROR: Could not set temporary breakpoint.\n");
			FreeRepSet(enb_rep);
			FreeRecSet(new_rec);
			continue;
		}
		
		FreeRecSet(new_rec);
		
		// Set the disposition 
		bpi->bp_disp = DBGBP_DELETE_DISP;
		
		// Add the modified breakpoint to the bpset
		AddBP(rep_set->tree, bpi, reply->rep_group);
		bpset = FindBPSetByBP(rep_set->tree, bpi, &bp_id);
		
		// Print out status
		if(bpset == (bp_set_t *)NULL)//this is not supposed to happen
		{
			PrintMsgWithSetInfoByOffset(Client.cli_err_stream, rec_set->s_setname, reply->rep_group, over_set->s_offset, over_set->s_len, 
				"ERROR: cannot find breakpoint [%d: file %s, line %d] in BPSetList of tree[%p].\n",
				bpi->bp_id, 
				bpi->bp_loc.loc_file, 
				bpi->bp_loc.loc_line, 
				rep_set->tree);
			return 0;
		}

		if ( bpset->b_loc_line >= 0 )
			PrintMsgWithSetInfoByOffset(Client.cli_out_stream, rec_set->s_setname, reply->rep_group, over_set->s_offset, over_set->s_len, 
				"Temporary breakpoint %d: file %s, line %d.\n",
				bp_id, 
				bpset->b_loc_file, 
				bpset->b_loc_line);
		else
			PrintMsgWithSetInfoByOffset(Client.cli_out_stream, rec_set->s_setname, reply->rep_group, over_set->s_offset, over_set->s_len, 
				"Temporary breakpoint %d in %s\n",
				bp_id, 
				bpset->b_loc_func, 
				bpset->b_loc_line);
	}
	
	DPERFLOG(SetTimerStopAndLog("settbreak"));

	FreeRepSet(rep_set);

	return 1;
}

static int 
cmd_watch_cmd(rec_set_t *rec_set, int argc, char **argv)
{
	char *				arg;
	dbg_over_set_t *	over_set = rec_set->s_over_set;
	rep_set_t *			rep_set = NULL;
	group_reply_t *		reply = NULL;
	bp_set_t *			bpset;
	dbgbp_t *			bpi;
	int					bp_id;
	dbgbp_type			bp_type;

	if ( argc < 2 )
	{
		PrintOutError("Not enough command arguments.\n");
		PrintOutError("Expected: watch -a foo\n");
		return 1;
	}

	if ( argc > 3 )
	{
		PrintOutError("Too many command arguments.\n");
		PrintOutError("Expected: watch -a foo\n");
		return 0;
	}
	
	// check if we have an "-a", "-r", or "-w" argument.
	if ( argc == 3 )
	{
		if ( strcmp(argv[1], "-a") == 0 )
		{
			bp_type = DBGBP_AWATCH_TYPE;
		// FIXME: There is a gdb bug that breaks read watchpoints. It will cause
		//         a segfault, so they are disabled for now.
		//} else if ( strcmp(argv[1], "-r") == 0 )
		//{
		//	bp_type = DBGBP_RWATCH_TYPE;
		} else if ( strcmp(argv[1], "-w") == 0 )
		{
			bp_type = DBGBP_WATCH_TYPE;
		} else
		{
			PrintOutError("Unknown command argument %s.\n", argv[1]);
			return 0;
		}
		
		// FIXME: This should go through the parser...
		arg = strdup(argv[2]);
	} else
	{
		// default to write watchpoint
		bp_type = DBGBP_WATCH_TYPE;
		
		// FIXME: This should go through the parser...
		arg = strdup(argv[1]);
	}
	
	rep_set = DbgSetWatch(rec_set, bp_type, arg);

	Free(arg);

	ProcessErrorInRepSet(rec_set, rep_set);
	
	while((reply = GetNextReplyInSet(rep_set)) != (group_reply_t*)NULL)
	{
		// Point at the breakpoint data 
		bpi = EVENT_BP(reply->rep_event);

		AddBP(rep_set->tree, bpi, reply->rep_group);
		bpset = FindBPSetByBP(rep_set->tree, bpi, &bp_id);

		if(bpset == (bp_set_t *)NULL)//this is not supposed to happen
		{
			PrintMsgWithSetInfoByOffset(Client.cli_err_stream, rec_set->s_setname, reply->rep_group, over_set->s_offset, over_set->s_len, 
					"ERROR: cannot find watchpoint [%d: variable %s] in BPSetList of tree[%p].\n",
					bpi->bp_id, bpi->bp_exp, rep_set->tree);
		}

		switch (bpset->b_bp_type)
		{
			case DBGBP_WATCH_TYPE:
				PrintMsgWithSetInfoByOffset(Client.cli_out_stream, rec_set->s_setname, reply->rep_group, over_set->s_offset, over_set->s_len, 
						"Watchpoint %d: Set on variable %s\n", bp_id, bpset->b_bp_exp);
				break;
			case DBGBP_AWATCH_TYPE:
				PrintMsgWithSetInfoByOffset(Client.cli_out_stream, rec_set->s_setname, reply->rep_group, over_set->s_offset, over_set->s_len, 
						"Access Watchpoint %d: Set on variable %s\n", bp_id, bpset->b_bp_exp);
				break;
			case DBGBP_RWATCH_TYPE:
				PrintMsgWithSetInfoByOffset(Client.cli_out_stream, rec_set->s_setname, reply->rep_group, over_set->s_offset, over_set->s_len, 
						"Read Watchpoint %d: Set on variable %s\n", bp_id, bpset->b_bp_exp);
				break;
			default:
				PrintMsgWithSetInfoByOffset(Client.cli_out_stream, rec_set->s_setname, reply->rep_group, over_set->s_offset, over_set->s_len, 
						"Unknown Watchpoint %d: Set on variable %s\n", bp_id, bpset->b_bp_exp);
		}
	}

	FreeRepSet(rep_set);

	return 1;
}

static int 
cmd_delete_cmd(rec_set_t *rec_set, int argc, char **argv)
{
	list_t *			id_set_list = NULL;
	bp_id_set_t *		id_set = NULL;
	bp_set_t *			bpset;
	rec_set_t *			real_rec;
	bitset *			group;
	int					bpset_id;
	invoke_t *			invoke = (invoke_t*)(rec_set->s_invoke_handler);
	dbg_over_set_t * 	over_set = rec_set->s_over_set;
	rep_set_t *			rep_set;

	if ( EmptyBPSetList(invoke->i_tree) )
	{
		PrintMsgWithSetInfoByOffset(Client.cli_err_stream, rec_set->s_setname, rec_set->s_receiver_bs, over_set->s_offset, over_set->s_len, "No breakpoints or watchpoints.\n");
		return 1;
	}

	if ( argc < 2 )
	{
		/*
		** delete all
		*/
		if (!cli_query("Delete all breakpoints in focus?"))
			return 1;

		SetBPSetHead(invoke->i_tree);
		while((bpset = GetNextBPSet(invoke->i_tree, &bpset_id)) != (bp_set_t*)NULL)
		{
			id_set_list = bpset->b_bp_id_list;
			SetList(id_set_list);
			while((id_set = (bp_id_set_t*)GetListElement(id_set_list)) != (bp_id_set_t*)NULL)
			{
				group = bitset_and(id_set->b_group, rec_set->s_receiver_bs);
				if (!bitset_isempty(group))
				{
					// create the rec_set
					real_rec = NewRecSet(rec_set->s_setname, rec_set->s_invoke_handler, rec_set->s_dbginvoke_handler, rec_set->s_proto_handler, group);
					
					// send the command
					if ((rep_set = DbgDeleteBreak(real_rec, id_set->b_bp_id)) != NULL)
					{
						ProcessErrorInRepSet(real_rec, rep_set);
						PrintOutError("ERROR: Could not delete breakpoint.\n");
						FreeRepSet(rep_set);
						FreeRecSet(real_rec);
						bitset_free(group);
						continue;
					}
					
					FreeRecSet(real_rec);
					
					RemoveBPSet(invoke->i_tree, bpset, id_set->b_bp_id, group);
				}
				bitset_free(group);
			}
		}

		return 1;
	}

	if ( !IsInteger(argv[1]) )
	{
		PrintMsgWithSetInfoByOffset(Client.cli_err_stream, rec_set->s_setname, rec_set->s_receiver_bs, over_set->s_offset, over_set->s_len, "Argument required (breakpoint number).\n");
		return 0;
	}

	if ( (bpset = FindBPSetByID(invoke->i_tree, atoi(argv[1]))) == (bp_set_t *)NULL )
	{
		PrintMsgWithSetInfoByOffset(Client.cli_err_stream, rec_set->s_setname, rec_set->s_receiver_bs, over_set->s_offset, over_set->s_len, "No breakpoint number %d.\n", atoi(argv[1]));
		return 0;
	}

	id_set_list = bpset->b_bp_id_list;
	SetList(id_set_list);
	while((id_set = (bp_id_set_t*)GetListElement(id_set_list)) != (bp_id_set_t*)NULL)
	{
		group = bitset_and(id_set->b_group, rec_set->s_receiver_bs);
		if(!bitset_isempty(group))
		{
			// create the rec_set
			real_rec = NewRecSet(rec_set->s_setname, rec_set->s_invoke_handler, rec_set->s_dbginvoke_handler, rec_set->s_proto_handler, group);
			
			// send the command
			if ((rep_set = DbgDeleteBreak(real_rec, id_set->b_bp_id)) != NULL)
			{
				ProcessErrorInRepSet(real_rec, rep_set);
				PrintOutError("ERROR: Could not delete breakpoint.\n");
				FreeRepSet(rep_set);
				FreeRecSet(real_rec);
				bitset_free(group);
				continue;
			}
			
			FreeRecSet(real_rec);

			RemoveBPSet(invoke->i_tree, bpset, id_set->b_bp_id, group);
		}
		bitset_free(group);
	}
	
	return 1;
}

static int 
cmd_disable_cmd(rec_set_t *rec_set, int argc, char **argv)
{
	list_t *			id_set_list = NULL;
	bp_id_set_t *		id_set = NULL;
	bp_set_t *			bpset;
	rec_set_t *			real_rec;
	bitset *			group;
	int					bpset_id;
	invoke_t *			invoke = (invoke_t*)(rec_set->s_invoke_handler);
	dbg_over_set_t *	over_set = rec_set->s_over_set;
	rep_set_t *			rep_set;

	if ( EmptyBPSetList(invoke->i_tree) )
	{
		PrintMsgWithSetInfoByOffset(Client.cli_err_stream, rec_set->s_setname, rec_set->s_receiver_bs, over_set->s_offset, over_set->s_len, "No breakpoints or watchpoints.\n");
		return 1;
	}

	if ( argc < 2 )
	{
		/*
		** disable all
		*/

		SetBPSetHead(invoke->i_tree);
		while((bpset = GetNextBPSet(invoke->i_tree, &bpset_id)) != (bp_set_t*)NULL)
		{
			id_set_list = bpset->b_bp_id_list;
			SetList(id_set_list);
			while((id_set = (bp_id_set_t*)GetListElement(id_set_list)) != (bp_id_set_t*)NULL)
			{
				group = bitset_and(id_set->b_group, rec_set->s_receiver_bs);
				if (!bitset_isempty(group))
				{
					real_rec = NewRecSet(rec_set->s_setname, rec_set->s_invoke_handler, rec_set->s_dbginvoke_handler, rec_set->s_proto_handler, group);
					
					if ((rep_set = DbgDisableBreak(real_rec, id_set->b_bp_id)) != NULL)
					{
						ProcessErrorInRepSet(real_rec, rep_set);
						PrintOutError("ERROR: Could not disable breakpoint.\n");
						FreeRepSet(rep_set);
						FreeRecSet(real_rec);
						bitset_free(group);
						continue;
					}
					
					FreeRecSet(real_rec);
					// mark the breakpoint as disabled for this group
					DisableBPForBPSet(bpset, id_set->b_group);
				}
				bitset_free(group);
			}
		}
		
		return 1;
	}

	if ( !IsInteger(argv[1]) )
	{
		PrintMsgWithSetInfoByOffset(Client.cli_err_stream, rec_set->s_setname, rec_set->s_receiver_bs, over_set->s_offset, over_set->s_len, "Numeric argument required (breakpoint number).\n");
		return 0;
	}

	if ( (bpset = FindBPSetByID(invoke->i_tree, atoi(argv[1]))) == (bp_set_t *)NULL )
	{
		PrintMsgWithSetInfoByOffset(Client.cli_err_stream, rec_set->s_setname, rec_set->s_receiver_bs, over_set->s_offset, over_set->s_len, "No breakpoint number %d.\n", atoi(argv[1]));
		return 0;
	}

	id_set_list = bpset->b_bp_id_list;
	SetList(id_set_list);
	while((id_set = (bp_id_set_t*)GetListElement(id_set_list)) != (bp_id_set_t*)NULL)
	{
		group = bitset_and(id_set->b_group, rec_set->s_receiver_bs);
		if(!bitset_isempty(group))
		{
			real_rec = NewRecSet(rec_set->s_setname, rec_set->s_invoke_handler, rec_set->s_dbginvoke_handler, rec_set->s_proto_handler, group);
			
			if ((rep_set = DbgDisableBreak(real_rec, id_set->b_bp_id)) != NULL)
			{
				ProcessErrorInRepSet(real_rec, rep_set);
				PrintOutError("ERROR: Could not disable breakpoint.\n");
				FreeRepSet(rep_set);
				FreeRecSet(real_rec);
				bitset_free(group);
				continue;
			}
			
			FreeRecSet(real_rec);
			// mark the breakpoint as disabled for this group
			DisableBPForBPSet(bpset, group);
		}
		bitset_free(group);
	}
	
	return 1;
}

static int 
cmd_enable_cmd(rec_set_t *rec_set, int argc, char **argv)
{
	int					argp = 1;
	int					once = 0;
	int					delete = 0;
	int					bp_id = 0;
	list_t *			id_set_list = NULL;
	bp_id_set_t *		id_set = NULL;
	bp_set_t *			bpset;
	rec_set_t *			real_rec;
	bitset *			group;
	int					bpset_id;
	invoke_t *			invoke = (invoke_t*)(rec_set->s_invoke_handler);
	dbg_over_set_t *	over_set = rec_set->s_over_set;
	rep_set_t *			rep_set;
	
	// FIXME: Currently GDB has no way to turn a breakpoint that was marked as
	//         delete or disable back to keep. We could solve this by internaly
	//         deleting the original breakpoint and creating a new breakpoint
	//         but this would involve modifying the bp_id_set_t objects in the bpset.
	//         I don't want to do that right now, and besides doing so would make
	//         us "better" than gdb.

	if ( EmptyBPSetList(invoke->i_tree) )
	{
		PrintMsgWithSetInfoByOffset(Client.cli_err_stream, rec_set->s_setname, rec_set->s_receiver_bs, over_set->s_offset, over_set->s_len, "No breakpoints or watchpoints.\n");
		return 1;
	}
	
	while ( argp < argc )
	{
		if ( strcmp(argv[argp], "-once") == 0 )
		{
			++argp;
			++once;
		} else if ( strcmp(argv[argp], "-delete") == 0 )
		{
			++argp;
			++delete;
		} else if ( IsInteger(argv[argp]) )
		{
			bp_id = atoi(argv[argp]);
			++argp;
		} else
		{
			PrintOutError("Unknown command argument %s.\n", argv[argp]);
			return 0;
		}
	}
	
	// you cannot set once and delete at the same time
	if ( once && delete )
	{
		PrintOutError("The -once and -delete arguments cannot be used at the same time.\n");
		return 0;
	}
	
	if ( bp_id == 0 )
	{
		/*
		** enable all
		*/
		SetBPSetHead(invoke->i_tree);
		while((bpset = GetNextBPSet(invoke->i_tree, &bpset_id)) != (bp_set_t*)NULL)
		{
			id_set_list = bpset->b_bp_id_list;
			SetList(id_set_list);
			while((id_set = (bp_id_set_t*)GetListElement(id_set_list)) != (bp_id_set_t*)NULL)
			{
				group = bitset_and(id_set->b_group, rec_set->s_receiver_bs);
				if (!bitset_isempty(group))
				{
				
					real_rec = NewRecSet(rec_set->s_setname, rec_set->s_invoke_handler, rec_set->s_dbginvoke_handler, rec_set->s_proto_handler, group);
					
					if ((rep_set = DbgEnableBreak(real_rec, id_set->b_bp_id, once, delete)) != NULL)
					{
						ProcessErrorInRepSet(real_rec, rep_set);
						PrintOutError("ERROR: Could not enable breakpoint.\n");
						FreeRepSet(rep_set);
						FreeRecSet(real_rec);
						bitset_free(group);
						continue;
					}
					
					FreeRecSet(real_rec);
					
					// mark the breakpoint as enabled for this group
					EnableBPForBPSet(bpset, group);
					
					// Set the disposition for this group
					if (once)
					{
						SetBPDispositionForBPSet(DBGBP_DISABLE_DISP, bpset, group);
					} else if (delete)
					{
						SetBPDispositionForBPSet(DBGBP_DELETE_DISP, bpset, group);
					}
				}
				bitset_free(group);
			}
		}
		
		return 1;
	}

	if ( (bpset = FindBPSetByID(invoke->i_tree, bp_id)) == (bp_set_t *)NULL )
	{
		PrintMsgWithSetInfoByOffset(Client.cli_err_stream, rec_set->s_setname, rec_set->s_receiver_bs, over_set->s_offset, over_set->s_len, "No breakpoint number %d.\n", bp_id);
		return 0;
	}

	id_set_list = bpset->b_bp_id_list;
	SetList(id_set_list);
	while((id_set = (bp_id_set_t*)GetListElement(id_set_list)) != (bp_id_set_t*)NULL)
	{
		group = bitset_and(id_set->b_group, rec_set->s_receiver_bs);
		if(!bitset_isempty(group))
		{
			real_rec = NewRecSet(rec_set->s_setname, rec_set->s_invoke_handler, rec_set->s_dbginvoke_handler, rec_set->s_proto_handler, group);
			
			if ((rep_set = DbgEnableBreak(real_rec, id_set->b_bp_id, once, delete)) != NULL)
			{
				ProcessErrorInRepSet(real_rec, rep_set);
				PrintOutError("ERROR: Could not enable breakpoint.\n");
				FreeRepSet(rep_set);
				FreeRecSet(real_rec);
				bitset_free(group);
				continue;
			}
			
			FreeRecSet(real_rec);
			
			// mark the breakpoint as enabled for this group
			EnableBPForBPSet(bpset, group);
			
			// Set the disposition for this group
			if (once)
			{
				SetBPDispositionForBPSet(DBGBP_DISABLE_DISP, bpset, group);
			} else if (delete)
			{
				SetBPDispositionForBPSet(DBGBP_DELETE_DISP, bpset, group);
			}
		}
		bitset_free(group);
	}
	
	return 1;
}

static int 
class_info_subcmd_breakpoints_cmd(rec_set_t *rec_set, int argc, char **argv)
{
	int					bid = -1;
	bitset *			group, *disp_group, *enabled_group, *disabled_group;
	bp_set_t *			bpset;
	int					bpset_id;
	invoke_t *			invoke = (invoke_t*)(rec_set->s_invoke_handler);
	dbg_over_set_t *	over_set = rec_set->s_over_set;
	bp_disp_set_t *		disp_set;

	if ( argc > 1 )
		bid = atoi(argv[1]);

	if ( EmptyBPSetList(invoke->i_tree) )
	{
		PrintMsgWithSetInfo(Client.cli_out_stream, rec_set->s_setname, rec_set->s_receiver_bs, "No breakpoints or watchpoints.\n");
		return 1;
	}

	if (
			bid >= 0 
			&& 
			(bpset = FindBPSetByID(invoke->i_tree, bid)) == (bp_set_t*)NULL
		)
	{
		PrintMsgWithSetInfo(Client.cli_err_stream, rec_set->s_setname, rec_set->s_receiver_bs, "No breakpoint or watchpoint number %d.\n", bid);
		return 0;
	}

	PrintOutMsg("Procset\t\t%-3s %-14s %-4s %-3s %s\n", "Num", "Type", "Disp", "Enb", "What");

	SetBPSetHead(invoke->i_tree);
	while((bpset = GetNextBPSet(invoke->i_tree, &bpset_id)) != (bp_set_t*)NULL)
	{
		if ( bid >= 0 && bpset_id != bid )
			continue;
		
		group = bitset_and(bpset->b_group, rec_set->s_receiver_bs);

		if(bitset_isempty(group))
		{
			bitset_free(group);
			continue;
		}
		
		// The info we print is dependent on the type of breakpoint and the enabled/disabled setting and disposition
		switch (bpset->b_bp_type)
		{
			case DBGBP_WATCH_TYPE:
			case DBGBP_AWATCH_TYPE:
			case DBGBP_RWATCH_TYPE:
				// loop over the disposition list
				SetList(bpset->b_bp_disp);
				while((disp_set = (bp_disp_set_t *)GetListElement(bpset->b_bp_disp)) != NULL)
				{
					disp_group = bitset_and(disp_set->b_group, group);
					enabled_group = bitset_and(bpset->b_bp_enabled, disp_group);
					
					if (!bitset_isempty(enabled_group))
					{
						// First print out watchpoints that are enabled
						PrintMsgWithSetInfoByOffset(Client.cli_out_stream, rec_set->s_setname, enabled_group, over_set->s_offset, over_set->s_len,
							"\t%-3d %-14s %-4s %-3s %s\n",
							bpset_id, BPTypeToStr(bpset->b_bp_type), BPDispToStr(disp_set->b_bp_disp), "y",
							bpset->b_bp_exp);
					}
					
					// Compute the disabled set
					bitset_invert(enabled_group);
					disabled_group = bitset_and(enabled_group, disp_group);
					
					if (!bitset_isempty(disabled_group))
					{
						// Next print out watchpoints that are disabled
						PrintMsgWithSetInfoByOffset(Client.cli_out_stream, rec_set->s_setname, disabled_group, over_set->s_offset, over_set->s_len,
							"\t%-3d %-14s %-4s %-3s %s\n",
							bpset_id, BPTypeToStr(bpset->b_bp_type), BPDispToStr(disp_set->b_bp_disp), "n",
							bpset->b_bp_exp);
					}
					
					// Free the bitsets
					bitset_free(disp_group);
					bitset_free(enabled_group);
					bitset_free(disabled_group);
				}
				break;
				
			case DBGBP_BREAK_TYPE:
			default:
				// loop over the disposition list
				SetList(bpset->b_bp_disp);
				while((disp_set = (bp_disp_set_t *)GetListElement(bpset->b_bp_disp)) != NULL)
				{
					disp_group = bitset_and(disp_set->b_group, group);
					enabled_group = bitset_and(bpset->b_bp_enabled, disp_group);
					
					if ( bpset->b_loc_line > 0 )
					{
						if (!bitset_isempty(enabled_group))
						{
							// First print out breakpoints that are enabled
							PrintMsgWithSetInfoByOffset(Client.cli_out_stream, rec_set->s_setname, enabled_group, over_set->s_offset, over_set->s_len,
								"\t%-3d %-14s %-4s %-3s in %s at %s:%d\n",
								bpset_id, BPTypeToStr(bpset->b_bp_type), BPDispToStr(disp_set->b_bp_disp), "y",
								bpset->b_loc_func, 
								bpset->b_loc_file, bpset->b_loc_line);
						}
						
						// Compute the disabled set
						bitset_invert(enabled_group);
						disabled_group = bitset_and(enabled_group, disp_group);
							
						if (!bitset_isempty(disabled_group))
						{
							// Next print out breakpoints that are disabled
							PrintMsgWithSetInfoByOffset(Client.cli_out_stream, rec_set->s_setname, disabled_group, over_set->s_offset, over_set->s_len,
								"\t%-3d %-14s %-4s %-3s in %s at %s:%d\n",
								bpset_id, BPTypeToStr(bpset->b_bp_type), BPDispToStr(disp_set->b_bp_disp), "n",
								bpset->b_loc_func, 
								bpset->b_loc_file, bpset->b_loc_line);
						}
					} else
					{
						if (!bitset_isempty(enabled_group))
						{
							// First print out breakpoints that are enabled
							PrintMsgWithSetInfoByOffset(Client.cli_out_stream, rec_set->s_setname, enabled_group, over_set->s_offset, over_set->s_len,
								"\t%-3d %-14s %-4s %-3s in %s\n",
								bpset_id, BPTypeToStr(bpset->b_bp_type), BPDispToStr(disp_set->b_bp_disp), "y",
								bpset->b_loc_func);
						}
						
						// Compute the disabled set
						bitset_invert(enabled_group);
						disabled_group = bitset_and(enabled_group, disp_group);
						
						if (!bitset_isempty(disabled_group))
						{
							// Next print out breakpoints that are disabled
							PrintMsgWithSetInfoByOffset(Client.cli_out_stream, rec_set->s_setname, disabled_group, over_set->s_offset, over_set->s_len,
								"\t%-3d %-14s %-4s %-3s in %s\n",
								bpset_id, BPTypeToStr(bpset->b_bp_type), BPDispToStr(disp_set->b_bp_disp), "n",
								bpset->b_loc_func);
						}
					}
					
					// Free the bitsets
					bitset_free(disp_group);
					bitset_free(enabled_group);
					bitset_free(disabled_group);
				}
				break;
		}

		/* TODO: display hits for each process ? 
		if ( bp->bp_hits > 0 )
			ProcPrint(Client.cli_out_stream, p,
				"\tbreakpoint already hit %d time%s\n",
				bp->bp_hits, bp->bp_hits > 1 ? "s" : "");
		*/
		bitset_free(group);

		if ( bid >= 0 && bpset_id == bid )
			break;
	}

	return 1;
}

void
InitCmd_break(void)
{
	AddCommand("break", "b", proc_required, 0, cmd_break_cmd,
		cmd_break_help, "Set breakpoint at specified line or function.");
	AddCommand("delete", "d", proc_required, 0, cmd_delete_cmd,
		cmd_delete_help, "Delete a breakpoint.");
	AddCommand("disable", NULL, proc_required, 0, cmd_disable_cmd,
		cmd_disable_help, "Disable a breakpoint.");
	AddCommand("enable", NULL, proc_required, 0, cmd_enable_cmd,
		cmd_enable_help, "Enable a breakpoint.");
	AddCommand("tbreak", NULL, proc_required, 0, cmd_tbreak_cmd,
		cmd_tbreak_help, "Set a temporary breakpoint at specified line or function.");
	AddCommand("watch", NULL, proc_required, 0, cmd_watch_cmd,
		cmd_watch_help, "Set watchpoint on specified expression.");
		
	AddInfoCommand("breakpoints", proc_required, 0, class_info_subcmd_breakpoints_cmd,
		class_info_subcmd_breakpoints_help, "Show status of breakpoints.");
}
