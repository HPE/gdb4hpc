/*
** RPC utility routines.
**
** Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
**
*/

#ifdef HAVE_CONFIG_H
#include	<config.h>
#endif /* HAVE_CONFIG_H */

#if defined(sun) && !defined(PORTMAP)
#define PORTMAP
#endif /* sun && !PORTMAP */

#include	<sys/types.h>

#include	<stdio.h>
#include	<string.h>

#include	<rpc/rpc.h>
/* FIXME! this shouldn't be required, but is used for pmap_set on Linux */
#include	<rpc/pmap_clnt.h>

#include	"defs.h"
#include	"aif.h"
#include	"useful.h"
#include	"dbgevent.h"
#include	"dbgproto_rpc.h"
#include	"dbg_rpc.h"

#define PMAP_TRIES	1000

/*
** Attempt to find a free RPC program number.
*/
u_long
FindRPCProgNum(SVCXPRT *xprt, u_long vers, u_long proto, void (*disp) ())
{
	int		tries = PMAP_TRIES;
	static u_long   prog = 0x40000000;

#ifdef notdef
	while ( pmap_set(prog, vers, proto, xprt->xp_port) == FALSE )
		prog++;
#endif /* notdef */

	/*
	** Sun's backward compatible pmap_set() is no longer
	** compatible. We must use svc_register() to find
	** a free program number.
	*/

	while ( tries-- > 0 && svc_register(xprt, prog, vers, disp, 0) == FALSE )
		prog++;

	return (tries > 0) ? prog : 0;
}

/*
** Create an RPC service.
*/
int
CreateSvc(SVCXPRT **xp, u_long *prog, void (*clnt)(struct svc_req *, SVCXPRT *), u_long vers)
{
	if ( (*xp = svctcp_create(RPC_ANYSOCK, 0, 0)) == (SVCXPRT *)NULL )
	{
		fprintf(stderr, "svctcp_create error");
		return -1;
	}

	*prog = FindRPCProgNum(*xp, vers, IPPROTO_TCP, clnt);

	if ( *prog == 0 )
	{
		fprintf(stderr, "failed to register service");
		return -1;
	}

	return 0;
}

/*
** Keep NULL's.
*/
char *
StrEventDup(char *s)
{
	if ( s == NULL )
		return NULL;

	return strdup(s);
}

/*
** Replace NULLs with empty strings for XDR routines.
*/
char *
StrRPCDup(char *s)
{
	if ( s == NULL )
		return strdup("");

	return strdup(s);
}

bpinfo *
EventBPToRes(dbgbp_t *be)
{
	bpinfo *	bp;

	if ( be == (dbgbp_t *)NULL )
		return (bpinfo *)NULL;

	bp = (bpinfo *)Alloc(sizeof(bpinfo));
	bp->id = be->bp_id;
	bp->ignore = be->bp_ignore;
	bp->hits = be->bp_hits;
	bp->type = StrRPCDup(be->bp_type);
	bp->line = StrRPCDup(be->bp_stmt);
	bp->loc.file = StrRPCDup(be->bp_loc.loc_file);
	bp->loc.func = StrRPCDup(be->bp_loc.loc_func);
	bp->loc.addr = StrRPCDup(be->bp_loc.loc_addr);
	bp->loc.line = be->bp_loc.loc_line;
	bp->next = (bpinfo *)NULL;

	return bp;
}

frameentry *
EventFrameToRes(dbgframe_t *fe)
{
	frameentry *	f = (frameentry *)NULL;
	frameentry **	fp;

	fp = &f;

	for ( ; fe != (dbgframe_t *)NULL ; fe = fe->frame_next )
	{
		*fp = (frameentry *)Alloc(sizeof(frameentry));
		(*fp)->level = fe->frame_level;
		(*fp)->loc.file = StrRPCDup(fe->frame_loc.loc_file);
		(*fp)->loc.func = StrRPCDup(fe->frame_loc.loc_func);
		(*fp)->loc.addr = StrRPCDup(fe->frame_loc.loc_addr);
		(*fp)->loc.line = fe->frame_loc.loc_line;
		(*fp)->next = (frameentry *)NULL;

		fp = &(*fp)->next;
	}

	return f;
}

location *
EventLocToRes(dbgloc_t *le)
{
	location *	l;

	if ( le == (dbgloc_t *)NULL )
		return (location *)NULL;

	l = (location *)Alloc(sizeof(location));
	l->file = StrRPCDup(le->loc_file);
	l->func = StrRPCDup(le->loc_func);
	l->addr = StrRPCDup(le->loc_addr);
	l->line = le->loc_line;

	return l;
}

sclist *
EventListToRes(dbglist_t *le)
{
	sclist *	l = (sclist *)NULL;
	sclist **	lp;

	lp = &l;

	for ( ; le != (dbglist_t *)NULL ; le = le->list_next )
	{
		*lp = (sclist *)Alloc(sizeof(sclist));
		(*lp)->lno = le->list_lno;
		(*lp)->line = StrRPCDup(le->list_line);
		(*lp)->next = (sclist *)NULL;
		lp = &(*lp)->next;
	}

	return l;
}

void
EventToRes(dbg_res *res, dbgevent_t *e)
{
	res->status = RES_OK;

	if ( e == (dbgevent_t *)NULL )
		return;

	switch ( e->ev_event )
	{
	case DBGEV_ERROR:
		res->status = RES_ERROR;
		res->res_err = e->ev_errno;
		res->res_errstr = strdup(e->ev_errstr);
		break;

	case DBGEV_OK:
		break;

	case DBGEV_BPHIT:
		res->status = RES_BPHIT;
		res->res_bphit = EventBPToRes(e->ev_bp);
		break;

	case DBGEV_BPINFO:
		res->status = RES_BPINFO;
		res->res_bpinfo = EventBPToRes(e->ev_bp);
		break;

	case DBGEV_BPSET:
		res->status = RES_BPSET;
		res->res_bpset = EventBPToRes(e->ev_bp);
		break;

	case DBGEV_SIGNAL:
		res->status = RES_SIGNAL;
		res->res_sig = StrRPCDup(e->ev_sig_type);
		res->res_sigline = StrRPCDup(e->ev_sig_line);
		res->res_sigframe = EventFrameToRes(e->ev_sig_frame);
		break;

	case DBGEV_EXIT:
		res->status = RES_EXIT;
		res->res_exit = e->ev_exit;
		break;

	case DBGEV_STEP:
		res->status = RES_STEP;
		res->res_steplno = e->ev_step_lno;
		res->res_stepline = StrRPCDup(e->ev_step_line);
		res->res_stepframe = EventFrameToRes(e->ev_step_frame);
		break;

	case DBGEV_FRAME:
		res->status = RES_FRAME;
		res->res_frame = EventFrameToRes(e->ev_frame);
		break;

	case DBGEV_LOC:
		res->status = RES_LOC;
		res->res_loc = EventLocToRes(e->ev_loc);
		break;

	case DBGEV_TYPE:
		res->status = RES_TYPE;
		res->res_type = StrRPCDup(e->ev_type);
		break;

	case DBGEV_LIST:
		res->status = RES_LIST;
		res->res_list = EventListToRes(e->ev_list);
		break;

	case DBGEV_DATA:
		res->status = RES_DATA;
		res->res_data = CopyAIF(e->ev_data);
		break;

	case DBGEV_STAT:
		res->status = RES_STAT;
		res->res_status = e->ev_status;
		break;

	case DBGEV_VERS:
		res->status = RES_VERS;
		res->res_version = StrRPCDup(e->ev_version);
		res->res_arch = StrRPCDup(e->ev_arch);
		break;

	default:
		break;
	}

	FreeEvent(e);
}

dbgloc_t *
ResToLoc(location *le)
{
	dbgloc_t *	l;

	if ( le == (location *)NULL )
		return (dbgloc_t *)NULL;

	l = (dbgloc_t *)Alloc(sizeof(dbgloc_t));
	l->loc_file = StrEventDup(le->file);
	l->loc_func = StrEventDup(le->func);
	l->loc_addr = StrEventDup(le->addr);
	l->loc_line = le->line;

	return l;
}

dbglist_t *
ResToList(sclist *le)
{
	dbglist_t *	l = (dbglist_t *)NULL;
	dbglist_t **	lp;

	lp = &l;

	for ( ; le != (sclist *)NULL ; le = le->next )
	{
		*lp = (dbglist_t *)Alloc(sizeof(dbglist_t));
		(*lp)->list_lno = le->lno;
		(*lp)->list_line = StrEventDup(le->line);
		(*lp)->list_next = (dbglist_t *)NULL;
		lp = &(*lp)->list_next;
	}

	return l;
}

dbgbp_t *
ResToBP(bpinfo *bp)
{
	dbgbp_t *	be;

	if ( bp == (bpinfo *)NULL )
		return (dbgbp_t *)NULL;

	be = NewBP(bp->id);
	be->bp_ignore = bp->ignore;
	be->bp_hits = bp->hits;
	be->bp_type = StrEventDup(bp->type);
	be->bp_stmt = StrEventDup(bp->line);
	be->bp_loc.loc_file = StrEventDup(bp->loc.file);
	be->bp_loc.loc_func = StrEventDup(bp->loc.func);
	be->bp_loc.loc_addr = StrEventDup(bp->loc.addr);
	be->bp_loc.loc_line = bp->loc.line;
	be->bp_next = (dbgbp_t *)NULL;

	return be;
}

dbgframe_t *
ResToFrame(frameentry *f)
{
	dbgframe_t *	fe = (dbgframe_t *)NULL;
	dbgframe_t **	fp;

	fp = &fe;

	for ( ; f != (frameentry *)NULL ; f = f->next )
	{
		*fp = (dbgframe_t *)Alloc(sizeof(dbgframe_t));
		(*fp)->frame_level = f->level;
		(*fp)->frame_loc.loc_file = StrEventDup(f->loc.file);
		(*fp)->frame_loc.loc_func = StrEventDup(f->loc.func);
		(*fp)->frame_loc.loc_addr = StrEventDup(f->loc.addr);
		(*fp)->frame_loc.loc_line = f->loc.line;
		(*fp)->frame_next = (dbgframe_t *)NULL;

		fp = &(*fp)->frame_next;
	}

	return fe;
}

/*
** Only cater for asyncronous events.
*/
dbgevent_t *
ResToEvent(dbg_res *res)
{
	dbgevent_t *	e;

	if ( res == (dbg_res *)NULL )
		return (dbgevent_t *)NULL;

	switch ( res->status )
	{
	case RES_OK:
		e = NewEvent(DBGEV_OK);
		break;

	case RES_ERROR:
		e = NewEvent(DBGEV_ERROR);
		e->ev_errno = res->res_err;
		e->ev_errstr = StrEventDup(res->res_errstr);
		break;

	case RES_BPHIT:
		e = NewEvent(DBGEV_BPHIT);
		e->ev_bp = ResToBP(res->res_bphit);
		break;

	case RES_SIGNAL:
		e = NewEvent(DBGEV_SIGNAL);
		e->ev_sig_type = StrEventDup(res->res_sig);
		e->ev_sig_line = StrEventDup(res->res_sigline);
		e->ev_sig_frame = ResToFrame(res->res_sigframe);
		break;

	case RES_EXIT:
		e = NewEvent(DBGEV_EXIT);
		e->ev_exit = res->res_exit;
		break;

	case RES_STEP:
		e = NewEvent(DBGEV_STEP);
		e->ev_step_lno = res->res_steplno;
		e->ev_step_line = StrEventDup(res->res_stepline);
		e->ev_step_frame = ResToFrame(res->res_stepframe);
		break;

	default:
		/* FreeEvent(e); */
		return (dbgevent_t *)NULL;
	}

	return e;
}
