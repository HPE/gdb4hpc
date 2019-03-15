/*********************************************************************************\
 * Stack frame commands.
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * © 2010-2019 Cray Inc.  All Rights Reserved.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Cray Inc.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *
 *********************************************************************************/

#ifdef HAVE_CONFIG_H
#include	<config.h>
#endif /* HAVE_CONFIG_H */

#include	<stdio.h>
#include	<signal.h>
#include	<stdlib.h>
#include	<stdarg.h>

#include	"client.h"
#include	"useful.h"

void
PrintFrameSet(char* setname, bitset* group, dbgframe_t *f)
{
	PrintMsgWithSetInfo(setname, group, "#%-2d ", f->frame_level);

	if ( *f->frame_loc.loc_addr != '\0' )
		PrintOutMsg("%s in ", f->frame_loc.loc_addr);

	PrintOutMsg("%s", f->frame_loc.loc_func); 

	if ( *f->frame_loc.loc_file != '\0' )
		PrintOutMsg(" at %s:%d", f->frame_loc.loc_file, f->frame_loc.loc_line);

	PrintOutMsg("\n");
}

static int 
movecmd(rec_set_t *rec_set, int argc, char **argv)
{
	int		up = 0;
	int		count = 1;
	dbgframe_t *	f;

	if ( *argv[0] == 'u' )
		up++;

	if ( argc >= 2 )
		count = atoi(argv[1]);

	rep_set_t* rep_set;
	group_reply_t *reply;

	rep_set = DbgMoveFrame(rec_set, count, up);

	//process error
	ProcessErrorInRepSet(rep_set);

	//process frames
	while((reply = GetNextReplyInSet(rep_set)) != (group_reply_t*)NULL)
	{
		// Point at the frame data
		f = EVENT_FRAME(reply->rep_event);

		PrintFrameSet(reply->rep_di->d_handle, reply->rep_group, f);
	}
	
	FreeRepSet(rep_set);

	return 1;
}

static int 
cmd_backtrace_cmd(rec_set_t *rec_set, int argc, char **argv)
{
	int				count = 0;
	dbgframe_t *	f;
	dbgframe_t *	fp;
	rep_set_t *		rep_set;
	group_reply_t *	reply;

	if ( argc >= 2 )
		count = atoi(argv[1]);

	rep_set = DbgShowFrame(rec_set, count);

	//process error
	ProcessErrorInRepSet(rep_set);

	//process frames
	while((reply = GetNextReplyInSet(rep_set)) != (group_reply_t*)NULL)
	{
		// Point at the frame data
		fp = EVENT_FRAME(reply->rep_event);
		
		for ( f = fp ; f != (dbgframe_t *)NULL ; f = f->frame_next)
		{
			PrintFrameSet(reply->rep_di->d_handle, reply->rep_group, f);
		}
	}

	FreeRepSet(rep_set);

	return 1;
}

static int
cmd_frame_cmd(rec_set_t *rec_set, int argc, char **argv)
{
	rep_set_t *		rep_set;
	group_reply_t *	reply;
	dbgframe_t *	fp;
	
	rep_set = DbgGetFrame(rec_set);
	
	//process error
	ProcessErrorInRepSet(rep_set);
	
	//process frame - there should only be one
	while((reply = GetNextReplyInSet(rep_set)) != (group_reply_t*)NULL)
	{
		// Point at the frame data
		fp = EVENT_FRAME(reply->rep_event);
		
		PrintFrameSet(reply->rep_di->d_handle, reply->rep_group, fp);
	}
	
	FreeRepSet(rep_set);
	
	return 1;
}

void
InitCmd_frame(void)
{
	AddCommand("up", NULL, proc_required, 0, (int (*)())movecmd,
		cmd_up_help, "Move up one or more stack frames.");
	AddCommand("down", NULL, proc_required, 0, (int (*)())movecmd,
		cmd_down_help, "Move down one or more stack frames.");
	AddCommand("backtrace", "bt", proc_required, 0, (int (*)())cmd_backtrace_cmd,
		cmd_backtrace_help, "Print backtrace of all stack frames.");
	AddCommand("frame", NULL, proc_required, 0, (int (*)())cmd_frame_cmd,
		cmd_frame_help, "Print the currently selected stack frame.");
}
