/******************************************************************************\
 * cmd_passthru.c - Command line interface routines.
 *
 * © 2012-2013 Cray Inc.  All Rights Reserved.
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
#include	<unistd.h>

#include	"client.h"
#include	"useful.h"

static int 			passthru_enabled = 0;
static char *		valid_rec_set_name = NULL;

static cmdlist_t *	DirectModeCmds;
static cmdlist_t *	OldCmds;

static int 
class_maint_subcmd_direct_cmd(rec_set_t *rec_set, int argc, char **argv)
{
	rep_set_t *		rep_set;
	group_reply_t *	reply;
	char *			cmd;
	
	rep_set = DbgDirectCmd(rec_set, argc, argv);
	
	//process error
	ProcessErrorInRepSet(rec_set, rep_set);
	
	//process console messages
	while((reply = GetNextReplyInSet(rep_set)) != (group_reply_t*)NULL)
	{
		// Point at the cmd data
		cmd = EVENT_CMD(reply->rep_event);
		
		// make sure the cmd is not empty
		if (strlen(cmd) == 0)
			continue;
		
		PrintMsgWithSetInfo(rec_set->s_setname, reply->rep_group, "GDB CONSOLE OUTPUT:\n\n%s\n\n", cmd);
	}

	FreeRepSet(rep_set);
	
	return 1;
}

static int 
cmd_directmode_cmd(rec_set_t *rec_set, int argc, char **argv)
{
	rep_set_t *		rep_set;
	group_reply_t *	reply;
	char *			cmd;
	char *			tmp_cmd[2] = {"show", "version"};

	// We need to ensure that only one process set is able to actually start up the
	// passthru mode, otherwise things will not reset properly. I am using a guard
	// variable to ensure only one procset gets thru.
	//
	// Note that the way the _docmd function works is that it calls the command for
	// every invoke_group in the current focus. If two invoke_groups are in focus,
	// this command is called twice. We need to prevent that from working.
	
	if (passthru_enabled)
	{
		PrintOutError("\nWarning: Multiple process sets in focus. Only operating on first process set.\n\n");
		return 1;
	}

	if ( rec_set == (rec_set_t *)NULL )
	{
		PrintOutError("No processes in focus.\n");
		return 0;
	}

	++passthru_enabled;
	valid_rec_set_name = strdup(rec_set->s_setname);

	OldCmds = SetCommands(DirectModeCmds);

	cli_inc_prompt();
	
	PrintOutMsg("Entering gdb pass-thru mode. Type \"end\" to exit mode...\n\n");
	
	// This won't work with a "generic" debug backend, but works with gdb.
	rep_set = DbgDirectCmd(rec_set, 2, tmp_cmd);
	
	//process error
	ProcessErrorInRepSet(rec_set, rep_set);
	
	//process console messages
	while((reply = GetNextReplyInSet(rep_set)) != (group_reply_t*)NULL)
	{
		// Point at the cmd data
		cmd = EVENT_CMD(reply->rep_event);
		
		// make sure the cmd is not empty
		if (strlen(cmd) == 0)
			continue;
		
		if (GetNumberOfReply(rep_set) == 1)
		{
			PrintOutMsg("%s\n", cmd);
		} else
		{
			PrintMsgWithSetInfo(rec_set->s_setname, reply->rep_group, "%s\n", cmd);
		}
	}

	FreeRepSet(rep_set);
	
	return 1;
}

static int 
cmd_directmode_alt_end_cmd(rec_set_t *rec_set, int argc, char **argv)
{
	PrintOutMsg("\nEnding gdb pass-thru mode. If program location has changed (i.e. continue) debugger is in an unknown state.\n");

	cli_dec_prompt();
	SetCommands(OldCmds);
	
	passthru_enabled = 0;
	Free(valid_rec_set_name);
	valid_rec_set_name = NULL;
	
	return 1;
}

static int
cmd_directmode_alt_direct_cmd(rec_set_t *rec_set, int argc, char **argv)
{
	char *			cmd;
	rep_set_t *		rep_set;
	group_reply_t *	reply;
	
	if ( rec_set == (rec_set_t *)NULL )
	{
		PrintOutError("No processes in focus.\n");
		return 0;
	}
	
	if (valid_rec_set_name == NULL)
		return 1;
	
	// check to see if this rec_set is valid, if not then silently return.
	if (strcmp(valid_rec_set_name, rec_set->s_setname))
		return 1;
	
	rep_set = DbgDirectCmd(rec_set, argc, argv);
	
	//process error
	ProcessErrorInRepSet(rec_set, rep_set);
	
	//process console messages
	SetReplyHeadInSet(rep_set);
	
	while((reply = GetNextReplyInSet(rep_set)) != (group_reply_t*)NULL)
	{
		// Point at the cmd data
		cmd = EVENT_CMD(reply->rep_event);
		
		// make sure the cmd is not empty
		if (strlen(cmd) == 0)
			continue;
		
		if (GetNumberOfReply(rep_set) == 1)
		{
			PrintOutMsg("%s\n", cmd);
		} else
		{
			PrintOutMsg("\n");
			PrintMsgWithSetInfo(rec_set->s_setname, reply->rep_group, "\n%s\n", cmd);
		}
	}

	FreeRepSet(rep_set);
	
	return 1;
}

/********************************
 * Helper functions
 ********************************/

static void
directmodenotfound(char *cmd)
{
	CmdNotFound(cmd);
}

static void
directmodeambig(char *cmd)
{
	AmbiguousAltCmd(DirectModeCmds, cmd);
}

static void
directmodefailed(void)
{
	cli_dec_prompt();
	SetCommands(OldCmds);
}

void
InitCmd_passthru(void)
{
	// regular commands
	AddCommand("gdbmode", NULL, proc_required, 0, cmd_directmode_cmd,
		cmd_directmode_help, "Enter direct gdb mode (experimental).");

	// maint commands
	AddMaintCommand("passthru", proc_required, 0, class_maint_subcmd_direct_cmd,
		class_maint_subcmd_direct_help, "Pass command to gdb backend.");
		
	// gdbmode alt commands
	DirectModeCmds = NewCommandList(directmodenotfound, directmodeambig, directmodefailed);
	
	// Note that the help strings below are not viewable. The help command is passed
	// to the gdb backend while in gdbmode.
	AddAltCommand(DirectModeCmds, "_default_cmd", NULL, proc_required, 0, 
		cmd_directmode_alt_direct_cmd, class_maint_subcmd_direct_help, "Send command to gdb backend.");
	AddAltCommand(DirectModeCmds, "end", NULL, proc_ignore, 0, 
		cmd_directmode_alt_end_cmd, class_maint_subcmd_direct_help, "Exit gdb direct mode.");
	AddAltCommand(DirectModeCmds, "quit", NULL, proc_ignore, 0,
		cmd_quit_cmd, cmd_quit_help, "Exit the debugger.");
		
	SetDefaultCommand(DirectModeCmds, "_default_cmd");
}

