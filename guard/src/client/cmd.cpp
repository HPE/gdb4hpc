/******************************************************************************\
 * Command processing routines.
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * © 2010-2014 Cray Inc.  All Rights Reserved.
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
 ******************************************************************************/

#ifdef HAVE_CONFIG_H
#include	<config.h>
#endif /* HAVE_CONFIG_H */

#include	<stdio.h>
#include	<stdarg.h>
#include	<stdlib.h>
#include	<string.h>

#include	"client.h"
#include	"useful.h"
#ifdef RD
#include	"rd_cmd.h"
#endif
#include	"dbgevent.h"
#include	"bitset.h"

/*
** Internal global variables
*/
static cmdlist_t *		CurrentCmds;		/* Current command list */
static list_t *			help_class_list;	/* Class Command help lookup */
static cmdlist_t *		Cmds;				/* Main command list */

/*
** Internal function prototypes
*/
static cmdtype_t *	_addcmd(list_t *, char *, char *, cmdptype_t, int, int (*)(), const char * const[], char *);
static cmdhelp_t *	_addhelpclasscmd(char *, cmdtype_t *(*)(char *));
static void			_alphabetize_cmd_list(list_t *);
static int			_docmd(cmdlist_t *, dbgprocset_t *, cmdtype_t *, int, char *[], int);
static cmdtype_t *	_findcmd(cmdlist_t *, char *);
static void			_ambigcmd(list_t *, char *);
static int			_apply_command(dbgprocset_t *, cmdtype_t *, int, char *[]);

/*
 * Command list functions
 */
 
cmdlist_t *
NewCommandList(void (*notfound)(char *), void (*ambig)(char *), void (*failed)(void))
{
	cmdlist_t *	c;

	c = (cmdlist_t *)Alloc(sizeof(cmdlist_t));
	c->cmd_list = NewList();
	c->cmd_notfound = notfound;
	c->cmd_ambig = ambig;
	c->cmd_failed = failed;
	c->cmd_default = NULL;

	return c;
}

void
DestroyCommandList(cmdlist_t *c)
{
	if (c == (cmdlist_t *)NULL)
		return;
	DestroyList(c->cmd_list, Free);
	Free(c);
}

/*
 * AddCommand functions
 */

static cmdtype_t *
_addcmd(list_t *l, char *name, char *alias, cmdptype_t pt, int sub, int (*cmd)(), const char * const help[], char *desc)
{
	cmdtype_t *	c;

	c = (cmdtype_t *)Alloc(sizeof(cmdtype_t));
	c->c_name = name;
	c->c_alias = alias;
	c->c_cmd = (int (*)(rec_set_t *, int, char **))cmd;
	c->c_help = (char **)help;
	c->c_desc = desc;
	c->c_ptype = pt;
	c->c_hassub = sub;

	AddToList(l, (void *)c);

	return c;
}

static cmdhelp_t *
_addhelpclasscmd(char *name, cmdtype_t *(*find)(char *))
{
	cmdhelp_t *	c_help;
	
	c_help = (cmdhelp_t *)Alloc(sizeof(cmdhelp_t));
	c_help->c_name = name;
	c_help->c_find = (cmdtype_t *(*)(char *))find;
	
	AddToList(help_class_list, (void *)c_help);
	
	return c_help;
}

static void
_alphabetize_cmd_list(list_t *l)
{
	int 		swap;
	cmdtype_t *	c1;
	cmdtype_t *	c2;
	
	// This is a simple bubble sort implementation
	do {
		swap = 0;
		
		SetList(l);
		
		// set c1 and c2 to the first and second elements in the list
		c1 = (cmdtype_t *)GetListElement(l);
		c2 = (cmdtype_t *)GetListElement(l);
		
		// need to ensure c2 is not null, if so we are done
		while (c2 != (cmdtype_t *)NULL)
		{
			if (strcmp(c1->c_name, c2->c_name) > 0)
			{
				++swap;
				SwapWithNextNeighbor(l, (void *)c1);
				// no need to update c1 since it is now in c2's spot in the list
			} else
			{
				// update c1 to point at c2 since there was no swap
				c1 = c2;
			}
			
			// update c2 to point at the next element in the list
			c2 = (cmdtype_t *)GetListElement(l);
		}
	} while (swap != 0);
}

void
AddCommand(char *name, char *alias, cmdptype_t pt, int sub, int (*cmd)(), const char *const help[], char *desc)
{
	_addcmd(Cmds->cmd_list, name, alias, pt, sub, cmd, help, desc);
}

void
AddAltCommand(cmdlist_t *cmds, char *name, char *alias, cmdptype_t pt, int sub, int (*cmd)(), const char * const help[], char *desc)
{
	_addcmd(cmds->cmd_list, name, alias, pt, sub, cmd, help, desc);
}

void
AddClassCommand(char *name, char *alias, cmdptype_t pt, int sub, int (*cmd)(), cmdtype_t *(*find)(char *), const char * const help[], char *desc)
{
	// register the class command with the regular command list
	_addcmd(Cmds->cmd_list, name, alias, pt, sub, cmd, help, desc);
	// register the class command name in the help list
	_addhelpclasscmd(name, find);
}

void
AddClassSubCommand(list_t *l, char *name, cmdptype_t pt, int sub, int (*cmd)(), const char * const help[], char *desc)
{
	_addcmd(l, name, NULL, pt, sub, cmd, help, desc);
}

/*
 * DoCommand functions
 */

static int
_docmd(cmdlist_t *cmd, dbgprocset_t *s, cmdtype_t *c, int argc, char *argv[], int isSubCmd)
{
	int	res=0;

	if ( !isSubCmd && c->c_hassub )
		res = (*c->c_cmd)(NULL, argc, argv);
	else
	{
		switch ( c->c_ptype )
		{
		case proc_required:
			if ( s == (dbgprocset_t *)NULL	|| s->s_nproc <= 0 )
			{
				PrintOutError("No process, use launch/attach first.\n");
				res = 0;
			}
			else
				res = _apply_command(s, c, argc, argv);
			break;

		case proc_optional:
			if ( s == (dbgprocset_t *)NULL || s->s_nproc <= 0 )
				res = (*c->c_cmd)(NULL, argc, argv);
			else
				res = _apply_command(s, c, argc, argv);

			break;

		case proc_ignore:
			res = (*c->c_cmd)(NULL, argc, argv);
			break;
		}
	}

	if ( !res && cmd->cmd_failed != (void (*)(void))NULL )
		cmd->cmd_failed();
		
	return res;
}

int
DoClassSubCommand(cmdlist_t *cmd_l, dbgprocset_t *s, cmdtype_t *c, int argc, char *argv[], int isSubCmd)
{
	return _docmd(cmd_l, s, c, argc, argv, isSubCmd);
}

/*
** Apply a command to each process in a process set.
** Print and compare handle blockmap function
*/
static int
_apply_command(dbgprocset_t *s, cmdtype_t *c, int argc, char *argv[])
{
	rec_set_t *			rec_set;
	int					res = 1;
	// used for checking status
	rep_set_t *			rep_set;
	group_recv_t *		recv_ptr;
	group_reply_t *		reply;
	char *				bitset_str;
	char *				set_str;
	dbgstatus_t *		stat;
	char *				status_block;
	
	// Create a new rec_set for this procset
	rec_set = CreateRecSetForProcset(s);

	// Call the command
	if ( !(*c->c_cmd)(rec_set, argc, argv) )
		res = 0;
			
	// If XML mode is enabled, we need to ask the dbgclient layer if the
	// command we just executed changed the status. If it did, we want to
	// add the status information to the xml output. We also check to see
	// if the command caused the inferior to exit. Inferior exit can only be
	// caused by a command.
	if (Client.XML_mode)
	{
		if (DbgCheckForStatusChange())
		{
			// Status has changed. We need to generate updated status
			// information
			
			// Query the new status from the backends, note that for this we
			// want to grab the location information
			rep_set = DbgStatus(rec_set, 1);
				
			ProcessErrorInRepSet(rep_set);
			
			// We need to loop over each di in the rec_set and process it one at
			// a time. This is because we need to create a status block for each
			// di.
			SetReceiverHeadInSet(rec_set);
			while ((recv_ptr = GetNextReceiverInSet(rec_set)) != NULL)
			{
				// Start a new status block for this di
				status_block = XMLStartStatusBlock(recv_ptr->rec_di->d_handle);
			
				// Process each status message and add it to the status block
				SetReplyHeadInSet(rep_set);
				while((reply = GetNextReplyInSet(rep_set)) != NULL)
				{
					// Ensure the di match
					if (recv_ptr->rec_di != reply->rep_di)
					{
						continue;
					}
				
					// First create the bitset string
					bitset_str = bitset_to_set(reply->rep_group);
				
					// Next create the full set string
					Asprintf(&set_str, "%s%s", reply->rep_di->d_handle, bitset_str);
					
					// Done with the bitset_str
					Free(bitset_str);
				
					// point at the status data
					stat = EVENT_STATUS(reply->rep_event);

					// The XML status that is printed is based on the status value
					XMLAddStatus(&status_block, set_str, stat->stat, stat->stat_loc);
				
					// Done with the set_str
					Free(set_str);
				}
			
				// Print the entire status block, this will free the string
				XMLPrintStatusBlock(status_block);
			}
			
			// Free the reply set
			FreeRepSet(rep_set);
				
			// Reset the status change since we handled it
			DbgResetStatusChange();
		}
	}
	
	// Done with the rec_set
	FreeRecSet(rec_set);

	return res;
}

/*
 * FindCommand functions
 */

static cmdtype_t *
_findcmd(cmdlist_t *cmds, char *cmd)
{
	int		cnt = 0;
	cmdtype_t *	c;
	cmdtype_t *	cp = NULL;

	SetList(cmds->cmd_list);

	while ( (c = (cmdtype_t *)GetListElement(cmds->cmd_list)) != (cmdtype_t *)NULL )
	{
		if
		(
			c->c_alias != NULL
			&&
			strcmp(cmd, c->c_alias) == 0
		)
			return c;

		if
		(
			strncmp(cmd, c->c_name, strlen(cmd)) == 0
			&&
			c->c_cmd != NULL
		)
		{
			cnt++;
			cp = c;
		}
	}

	if ( cnt > 1 )
	{
		cmds->cmd_ambig(cmd);
		return (cmdtype_t *)NULL;
	}
	
	if ( cnt == 0 )
	{
		// Check to see if there is a default command set, if so return that
		if (cmds->cmd_default != NULL)
			return cmds->cmd_default;
			
		cmds->cmd_notfound(cmd);
		return (cmdtype_t *)NULL;
	}

	return cp;
}

static cmdtype_t *
FindHelpClassCommand(char **cmd_argv)
{
	cmdhelp_t *	c;
	char *cmd = cmd_argv[0];
	
	SetList(help_class_list);
	
	while ( (c = (cmdhelp_t *)GetListElement(help_class_list)) != (cmdhelp_t *)NULL )
	{
		if (c->c_name != NULL && strcmp(cmd, c->c_name) == 0)
			break;
	}
	
	if (c != (cmdhelp_t *)NULL)
		return (*c->c_find)(cmd_argv[1]);
	else
		return _findcmd(CurrentCmds, cmd);
}

cmdtype_t *
FindCommand(char *cmd)
{
	return _findcmd(CurrentCmds, cmd);
}

cmdtype_t *
FindSubCommand(cmdlist_t *cmd_l, char *cmd)
{
	return _findcmd(cmd_l, cmd);
}

/*
 * functions which define default command not found behavior
 */

void
CmdNotFound(char *cmd)
{
	PrintOutError("Undefined command: \"%s\".  Try \"help\".\n", cmd);
}

/*
 * functions which define default ambiguous command behavior
 */

static void
_ambigcmd(list_t *cmds, char *cmd)
{
	int		cnt = 0;
	cmdtype_t *	c;
	list_t *	l = NewList();

	SetList(cmds);

	while ( (c = (cmdtype_t *)GetListElement(cmds)) != (cmdtype_t *)NULL )
	{
		if 
		(
			strncmp(cmd, c->c_name, strlen(cmd)) == 0
			&&
			c->c_cmd != NULL
		)
		{
			AddToList(l, (void *)c);
			cnt++;
		}
	}

	SetList(l);

	while ( (c = (cmdtype_t *)GetListElement(l)) != (cmdtype_t *)NULL )
	{
		PrintOutError("%s", c->c_name);

		if ( --cnt > 0 )
			PrintOutError(", ");
	}

	PrintOutError("\n");
			
	DestroyList(l, (void (*)(void*))NULL);
}

static void
AmbiguousCmd(char *cmd)
{
	PrintOutError("Ambiguous command \"%s\": ", cmd);
	_ambigcmd(CurrentCmds->cmd_list, cmd);
}

void
AmbiguousAltCmd(cmdlist_t *cmds, char *cmd)
{
	PrintOutError("Ambiguous command \"%s\": ", cmd);
	_ambigcmd(cmds->cmd_list, cmd);
}

void
AmbiguousClassSubCmd(list_t *l, char *cmd)
{
	_ambigcmd(l, cmd);
}

/*
 * functions for getting and setting commands
 */

cmdlist_t *
SetCommands(cmdlist_t *cmds)
{
	cmdlist_t *	old = CurrentCmds;

	CurrentCmds = cmds;
	return old;
}

void
SetDefaultCommand(cmdlist_t *cmds, char *cmd)
{
	if (cmds == NULL)
		return;
		
	cmds->cmd_default = FindSubCommand(cmds, cmd);
}

/*
 * Help functions defined below
 */

int
cmd_help_cmd(rec_set_t *rec_set, int argc, char **argv)
{
	cmdtype_t *	c;

	if ( argc < 2 )
	{
		SetList(CurrentCmds->cmd_list);

		while ( (c = (cmdtype_t *)GetListElement(CurrentCmds->cmd_list)) != (cmdtype_t *)NULL )
			if ( c->c_cmd != NULL )
				PrintOutMsg("%-15s %s\n", c->c_name, c->c_desc);

		return 1;
	}

	if ( argc > 2 )
		c = FindHelpClassCommand(&argv[1]);
	else
		c = _findcmd(CurrentCmds, argv[1]);

	if ( c == (cmdtype_t *)NULL )
		return 0;
	
	if ( c->c_help == (char **)NULL )
		return 0;
	
	PrintHelpEntry(c->c_help);

	return 1;
}

void
PrintHelpEntry(char **helpStrArray)
{
	char **cur_line;

	if (helpStrArray == NULL)
		return;

	// print initial newline
	PrintOutMsg("\n");
	
	// iterate over the array of strings
	cur_line = helpStrArray;
	while (*cur_line)
	{
		PrintOutMsg("%s\n", *cur_line++);
	}
	
	// print final newline
	PrintOutMsg("\n");
	
	return;
}

/*
 * command initialization
 */

void
InitCommands(int tty)
{
	// init the internal command lists
	Cmds = NewCommandList(CmdNotFound, AmbiguousCmd, (void (*)(void))NULL);
	help_class_list = NewList();
	
	// init the class commands
	InitCmd_class();
	
	// add the help command
	AddCommand("help", "h", proc_ignore, 0, (int (*)())cmd_help_cmd,
		cmd_help_help, "Display help information about commands.");
	
	// Begin init of every other command
	InitCmd_assign();
	InitCmd_break();
	InitCmd_comp();
	InitCmd_env();
	InitCmd_frame();
	InitCmd_func();
	InitCmd_go();
	InitCmd_list();
	InitCmd_passthru();
	InitCmd_print();
	InitCmd_proc();
	InitCmd_procset();
	InitCmd_source();
	InitCmd_step();
	InitCmd_whatis();
#ifdef RD
	InitCmd_rd();
#endif /* RD */

	// alphabetize the command list
	_alphabetize_cmd_list(Cmds->cmd_list);
	
	// alphabetize the class command lists
	_alphabetize_cmd_list(InfoCmds->cmd_list);
	_alphabetize_cmd_list(MaintCmds->cmd_list);
	_alphabetize_cmd_list(MaintSetCmds->cmd_list);
	_alphabetize_cmd_list(MaintShowCmds->cmd_list);
	_alphabetize_cmd_list(SetCmds->cmd_list);
	_alphabetize_cmd_list(ShowCmds->cmd_list);
	_alphabetize_cmd_list(UnsetCmds->cmd_list);
	_alphabetize_cmd_list(UsageCmds->cmd_list);

	InitCmdToken();

	(void)SetCommands(Cmds);
	
	InitBuiltinFuncs();
};

void
DestroyCommands(void)
{
	// destroy the internal help class list
	if (help_class_list != (list_t *)NULL)
	{
		DestroyList(help_class_list, Free);
	}

#ifdef RD
	FinishCmd_rd();
#endif /* RD */
	DestroyCommandList(Cmds);
	DestroyCmd_class();
	FinishCmdToken();
}

/*
** Execute command - used to parse a char buf into a command and execute it
*/

void
ExecuteCommand(char *line)
{
	int				argc = 0;
	int				seq = 0;
	int				num_read = 0;
	char *			cmd = NULL;
	char *			haystack = NULL;
	char *			needle = NULL;
	char **			ap = NULL;
	char **			argv = NULL;
	cmdtype_t *		c = NULL;
	dbgprocset_t *	s = CurrentProcSet();
	
	DPRINT("In ExecuteCommand\n");
	
	if (line == NULL)
		goto ExecuteCommand_exit;
	
	cmd = strdup(line);
	
	// If xml mode is enabled, we need to pull apart the xml tag we received
	if (Client.XML_mode)
	{
		if ((sscanf(line, XML_REQUEST_START, &seq, &num_read)) < 1)
		{
			PrintOutError("XML request parse error.\n");
			cli_set_error();
			goto ExecuteCommand_exit;
		}
		
		Free(cmd);
		cmd = strdup(line + num_read);
		
		haystack = cmd;
		// If we don't find the end needle the first time, its an error
		if ((haystack = strstr(haystack, XML_REQUEST_END)) == NULL)
		{
			PrintOutError("XML request parse error.\n");
			cli_set_error();
			Free(cmd);
			goto ExecuteCommand_exit;
		}
		needle = haystack;
		
		// Make sure that the user didn't type the needle in their command.
		// Its unlikely that they would do this, but we should still guard
		// against it.
		while (strlen(haystack) > strlen(XML_REQUEST_END))
		{
			haystack += strlen(XML_REQUEST_END);
			
			while ((haystack = strstr(haystack, XML_REQUEST_END)) != NULL)
			{
				needle = haystack;
			}
		}
		
		// turn the start of the needle into a newline followed by a null term
		*needle = '\n';
		*(needle+1) = '\0';
		DPRINT("ExecuteCommand: seq is %d\n", seq);
	}
	
	DPRINT("ExecuteCommand: cmd is \"%s\"\n", cmd);

	if ( (argv = Str2Argv(cmd)) == (char **)NULL )
	{
		Free(cmd);
		PrintOutError("Could not build command list!\n");
		cli_set_error();
		goto ExecuteCommand_exit;
	}

	for ( ap = argv ; *ap != NULL ; ap++ )
		argc++;

	// Inform the cli layer that we are starting to execute a command
	if (!cli_start_response(seq))
	{
		// Sequence number mismatch, so silently fail
		Free(cmd);
		FreeArgv(argv);
		goto ExecuteCommand_exit;
	}

	// These are not considered to be errors
	if (argc > 0 && (*argv[0] == '\0' || *argv[0] == '#'))
	{
		Free(cmd);
		FreeArgv(argv);
		// Inform the cli layer that we are done executing a command
		cli_end_response();
		goto ExecuteCommand_exit;
	}

	if (argc < 1 || ((c = _findcmd(CurrentCmds, argv[0])) == (cmdtype_t *)NULL))
	{
		// command not found
		Free(cmd);
		FreeArgv(argv);
		// Inform the cli layer that we are done executing a command
		cli_end_response();
		// set an error for the cli stream
		cli_set_error();
		goto ExecuteCommand_exit;
	}

	// do the command
	if ( ! _docmd(CurrentCmds, s, c, argc, argv, 0) )
	{
		Free(cmd);
		FreeArgv(argv);
		// Inform the cli layer that we are done executing a command
		cli_end_response();
		// set an error for the cli stream
		cli_set_error();
		goto ExecuteCommand_exit;
	}
	
	// Inform the cli layer that we are done executing a command
	cli_end_response();
	
	// If Quit is true after executing the command, remove the input handler.
	if (Client.quit)
	{
		DbgHandleInput(-1, NULL);
	}

	Free(cmd);
	FreeArgv(argv);

	DPRINT("ExecuteCommand: Command ok!\n");

ExecuteCommand_exit:
	DPRINT("Exit ExecuteCommand\n");
	return;
}
