/*********************************************************************************\
 * Breakpoint support routines.
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
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <string.h>
#include <stdio.h>

#include "defs.h"
#include "dbg.h"
#include "useful.h"
#include "dbgbp.h"

const char * dbgbp_type_str[] = {
	"breakpoint",
	"watchpoint",
	"acc watchpoint",
	"read watchpoint",
	"unknown",
	NULL
};

const char * dbgbp_disp_str[] = {
	"keep",
	"del",
	"dis",
	"unk",
	NULL
};

/*
** Create a new breakpoint.
*/
dbgbp_t *
NewBP(int id)
{
	dbgbp_t *	bp;

	bp = (dbgbp_t *)Alloc(sizeof(dbgbp_t));

	bp->bp_id = id;
	bp->bp_ignore = 0;
	bp->bp_deleted = 0;
	bp->bp_enabled = 1;
	bp->bp_disp = DBGBP_UNKNOWN_DISP;
	bp->bp_type = DBGBP_UNKNOWN_TYPE;
	bp->bp_loc.loc_file = NULL;
	bp->bp_loc.loc_func = NULL;
	bp->bp_loc.loc_addr = NULL;
	bp->bp_loc.loc_line = 0;
	bp->bp_exp = NULL;
	bp->bp_hits = 0;
	bp->bp_stmt = NULL;
	bp->bp_commands = (char **)NULL;
	bp->bp_next = (dbgbp_t *)NULL;

	return bp;
}

/*
** Remove any deleted breakpoints.
*/
void
CleanupBP(list_t *bplist)
{
	dbgbp_t *	bp;

	SetList(bplist);

	while ( (bp = (dbgbp_t *)GetListElement(bplist)) != (dbgbp_t *)NULL )
	{
		if ( bp->bp_deleted )
		{
			RemoveFromList(bplist, (void *)bp);
			FreeBP(bp);
		}
	}
}

/*
** Create a duplicate breakpoint (or list) for returning in an
** event (as the event is automatically freed).
*/
dbgbp_t *
DupBP(dbgbp_t *bp)
{
	int		cnt;
	char **		s;
	char **		p;
	dbgbp_t *	b;

	b = NewBP(bp->bp_id);

	b->bp_ignore = bp->bp_ignore;
	b->bp_deleted = bp->bp_deleted;
	b->bp_enabled = bp->bp_enabled;
	b->bp_disp = bp->bp_disp;
	b->bp_type = bp->bp_type;
	b->bp_hits = bp->bp_hits;
	b->bp_loc.loc_line = bp->bp_loc.loc_line;
	b->bp_loc.loc_addr = (bp->bp_loc.loc_addr != NULL) ? strdup(bp->bp_loc.loc_addr) : NULL;
	b->bp_loc.loc_func = (bp->bp_loc.loc_func != NULL) ? strdup(bp->bp_loc.loc_func) : NULL;
	b->bp_loc.loc_file = (bp->bp_loc.loc_file != NULL) ? strdup(bp->bp_loc.loc_file) : NULL;
	b->bp_exp = (bp->bp_exp != NULL) ? strdup(bp->bp_exp) : NULL;
	b->bp_stmt = (bp->bp_stmt != NULL) ? strdup(bp->bp_stmt) : NULL;

	if ( bp->bp_commands != (char **)NULL )
	{
		for ( cnt = 0, s = bp->bp_commands ; *s != NULL ; s++ )
			cnt++;

		b->bp_commands = (char **)Alloc(cnt * sizeof(char *) + 1);

		for ( s = bp->bp_commands, p = b->bp_commands ; *s != NULL ; s++ )
			*p++ = strdup(*s);

		*p = NULL;
	}

	return b;
}

/*
** Destroy a breakpoint.
*/
void
FreeBP(dbgbp_t *bp)
{
	char **		s;

	if ( bp->bp_loc.loc_file != NULL )
		Free(bp->bp_loc.loc_file);

	if ( bp->bp_loc.loc_func != NULL )
		Free(bp->bp_loc.loc_func);

	if ( bp->bp_loc.loc_addr != NULL )
		Free(bp->bp_loc.loc_addr);
		
	if ( bp->bp_exp != NULL )
		Free(bp->bp_exp);

	if ( bp->bp_stmt != NULL )
		Free(bp->bp_stmt);

	if ( bp->bp_commands != (char **)NULL )
		for ( s = bp->bp_commands ; *s != (char *)NULL ; s++ )
			Free(*s);

	Free(bp);
}

/*
** Remove a breakpoint from the list.
*/
void
RemoveBP(list_t *bplist, int bid)
{
	dbgbp_t *	bp;

	SetList(bplist);

	while ( (bp = (dbgbp_t *)GetListElement(bplist)) != (dbgbp_t *)NULL )
	{
		if ( bp->bp_id == bid )
		{
			RemoveFromList(bplist, (void *)bp);
			FreeBP(bp);
			return;
		}
	}
}

/*
** Find a breakpoint set on func.
*/
dbgbp_t *
FindBPByFunc(list_t *bplist, char *func)
{
	dbgbp_t *	bp;

	SetList(bplist);

	while ( (bp = (dbgbp_t *)GetListElement(bplist)) != (dbgbp_t *)NULL )
		if ( strcmp(func, bp->bp_loc.loc_func) == 0 )
			return bp;

	return (dbgbp_t *)NULL;
}

/*
** Find a breakpoint by its id.
*/
dbgbp_t *
FindBPByID(list_t *bplist, int bid)
{
	dbgbp_t *	bp;

	SetList(bplist);

	while ( (bp = (dbgbp_t *)GetListElement(bplist)) != (dbgbp_t *)NULL )
		if ( bid == bp->bp_id )
			return bp;

	return (dbgbp_t *)NULL;
}

/*
** Return the string representation of the breakpoint type.
*/
const char *
BPTypeToStr(dbgbp_type type)
{
	return dbgbp_type_str[type];
}

/*
** Return the string representation of the breakpoint disposition.
*/
const char *
BPDispToStr(dbgbp_disp disp)
{
	return dbgbp_disp_str[disp];
}

void
DumpBP(dbgbp_t *bp)
{
	PrintOutMsg("id=%d,", bp->bp_id);
	PrintOutMsg("ign=%s,", bp->bp_ignore ? "true" : "false");
	PrintOutMsg("del=%s,", bp->bp_deleted ? "true" : "false");
	PrintOutMsg("enb=%s,", bp->bp_enabled ? "true" : "false");
	PrintOutMsg("disp=%s,", dbgbp_disp_str[bp->bp_disp]);
	PrintOutMsg("type=%s,", dbgbp_type_str[bp->bp_type]);
	PrintOutMsg("file=%s,", bp->bp_loc.loc_file != NULL ? bp->bp_loc.loc_file : "NULL");
	PrintOutMsg("func=%s,", bp->bp_loc.loc_func != NULL ? bp->bp_loc.loc_func : "NULL");
	PrintOutMsg("addr=%s,", bp->bp_loc.loc_addr != NULL ? bp->bp_loc.loc_addr : "NULL");
	PrintOutMsg("line=%d,", bp->bp_loc.loc_line);
	PrintOutMsg("exp=%s,", bp->bp_exp != NULL ? bp->bp_exp : "NULL");
	PrintOutMsg("hits=%d,", bp->bp_hits);
	PrintOutMsg("stmt=%s,", bp->bp_stmt != NULL ? bp->bp_stmt : "NULL");
	PrintOutMsg("\n");
}
