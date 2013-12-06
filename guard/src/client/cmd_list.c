/*
** Source list command.
**
** Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
**
*/

#ifdef HAVE_CONFIG_H
#include	<config.h>
#endif /* HAVE_CONFIG_H */

#include	<stdio.h>
#include	<signal.h>
#include	<stdarg.h>

#include	"client.h"
#include	"useful.h"

static int 
cmd_list_cmd(rec_set_t *rec_set, int argc, char **argv)
{
	char *			s = NULL;
	dbglist_t *		l;
	dbglist_t *		lp;
	rep_set_t * 	rep_set = NULL;
	group_reply_t *	reply;
	char *			tmp_str;

	if ( argc > 1 )
		s = argv[1];

	rep_set = DbgList(rec_set, s);

	ProcessErrorInRepSet(rec_set, rep_set);

	while((reply = GetNextReplyInSet(rep_set)) != (group_reply_t*)NULL)
	{
		// point at the list data
		lp = EVENT_LIST(reply->rep_event);
		
		for ( l = lp ; l != (dbglist_t *)NULL ; l = l->list_next )
		{
			// Replace any \\t or \\n with \t and "" respectively
			tmp_str = ReplaceStr(l->list_line, "\\t", "\t");
			// Free the old list_line
			Free(l->list_line);
			l->list_line = ReplaceStr(tmp_str, "\\n", "");
			// Free the tmp_str
			Free(tmp_str);
			PrintMsgWithSetInfo(Client.cli_out_stream, rec_set->s_setname, reply->rep_group, "%d\t%s\n", l->list_lno, l->list_line);
		}
	}

	FreeRepSet(rep_set);

	return 1;
}

static int
class_info_subcmd_sources_cmd(rec_set_t *rec_set, int argc, char **argv)
{
	rep_set_t *		rep_set = NULL;
	group_reply_t *	reply;
	dbgsources_t *	s;
	dbgsources_t *	sp;
	
	rep_set = DbgGetSources(rec_set);
	
	ProcessErrorInRepSet(rec_set, rep_set);
	
	// Only print this if there are actual replies
	if (GetNumberOfReply(rep_set))
	{
		PrintOutMsg("Source files for $%s:\n", rec_set->s_setname);
	}
	
	// process replies
	while((reply = GetNextReplyInSet(rep_set)) != (group_reply_t*)NULL)
	{
		// point at the sources data
		sp = EVENT_SOURCES(reply->rep_event);
		
		for (s=sp; s != NULL; s=s->sources_next)
		{
			PrintOutMsg("%s\n", s->sources_file);
		}
	}
	
	FreeRepSet(rep_set);
	
	return 1;
}

void
InitCmd_list(void)
{
	// Regular commands
	AddCommand("list", "l", proc_required, 0, cmd_list_cmd,
		cmd_list_help, "List source for specified function or line.");
		
	// Info commands
	AddInfoCommand("sources", proc_required, 0, class_info_subcmd_sources_cmd,
		class_info_subcmd_sources_help, "Display the source files in the application.");
}
