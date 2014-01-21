/*********************************************************************************\
 * network format routines.
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
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
 *********************************************************************************/



#ifdef HAVE_CONFIG_H
#include	<config.h>
#endif /* HAVE_CONFIG_H */

#include	<stdarg.h>
#include	<stdlib.h>
#include	<string.h>

#include	"defs.h"

#include	"aif.h"
#include	"dbg.h"
#include	"useful.h"

// Static prototypes
static char *	StrSocDup(char *s);
static char *	getstring(char **);
static int		getint(char **);

static char *	EventErrorToSocProto(dbgerr_t);
static char *	EventBPToSocProto(dbgbp_t *, int);
static char *	EventSigToSocProto(dbgsig_t);
static char *	EventStepToSocProto(dbgstep_t);
static char *	EventFrameToSocProto(dbgframe_t *);
static char *	EventLocToSocProto(dbgloc_t *);
static char *	EventStatusToSocProto(dbgstatus_t *);
static char *	EventVersToSocProto(dbgvers_t);
static char *	EventIntToSocProto(int);
static char *	EventListToSocProto(dbglist_t *);
static char *	EventSourcesToSocProto(dbgsources_t *);
static char *	EventVarsToSocProto(dbgvars_t *);
static char *	EventStringToSocProto(char *);

static dbgerr_t			SocProtoToErr(char *);
static dbgloc_t *		SocProtoToLoc(char *);
static dbgvars_t *		SocProtoToVars(char *);
static dbglist_t *		SocProtoToList(char *);
static dbgbp_t *		SocProtoToBP(char *);
static dbgsig_t			SocProtoToSig(char *);
static dbgframe_t *		SocProtoToFrame(char *);
static char *			SocProtoToString(char *);
static dbgstatus_t *	SocProtoToStatus(char *);
static dbgvers_t		SocProtoToVers(char *);
static int				SocProtoToInt(char *);
static dbgsources_t *	SocProtoToSources(char *);

// Used for checking null strings
static char *	__fmt_null_str = "";

#define FMT_CHECK_NULL_STR(s) \
	if (s == NULL) \
	{ \
		s = __fmt_null_str; \
	}
	
#define NULL_GUARD(s) \
	if ( s == NULL || *s == '\0') \
	{ \
		return NULL; \
	}

/************************
** Auxillary functions **
************************/

static char *
StrSocDup(char *s)
{
	if ( s == NULL )
		return strdup("");

	return strdup(s);
}

static int
getint(char **res_buf)
{
	int 	digit;
	char *	endptr = NULL;
	
	*res_buf = skipspace(*res_buf);
	
	// Get the digit
	digit = (int)strtol(*res_buf, &endptr, 10);
	
	*res_buf = endptr;
	
	return digit;
}

static char *
getstring(char **res_buf)
{
	int 		len;
	char *		endptr = NULL;
	char *		res;
	
	*res_buf = skipspace(*res_buf);

	// First get the length of the string - Note that this can be 0
	len = (int)strtol(*res_buf, &endptr, 10);
	
	// Skip past the space between the length and the start of the string
	++endptr;
	
	// Now create a new buffer and memcpy
	res = (char *)Alloc(len+1);
	// only copy if len > 0
	if (len > 0)
	{
		memcpy(res, endptr, len);
	}
	
	// set the null terminator
	res[len] = '\0';
	
	// set res_buf past string
	*res_buf = endptr + len;
	
	return res;
}



/*******************************
** To network proto functions **
*******************************/


/*
** The following are helper functions for converting events into a string for
** network transfer
*/

static char *
EventErrorToSocProto(dbgerr_t e)
{
	char *	stres = NULL;
	char *	err_str = NULL;
	
	err_str = StrSocDup(e.err_str);
	
	Asprintf(&stres, "%d %d %s", e.err_errno, (int)strlen(err_str), err_str);
	
	Free(err_str);
	
	return stres;
}

static char *
EventBPToSocProto(dbgbp_t *be, int simplified)
{
	char *	stres = NULL;
	char * 	st_stmt = NULL;
	char * 	st_file = NULL;
	char * 	st_func = NULL;
	char * 	st_addr = NULL;
	char *	st_exp = NULL;

	if ( be == (dbgbp_t *)NULL )
		return strdup("");

	st_file = StrSocDup(be->bp_loc.loc_file);
	st_func = StrSocDup(be->bp_loc.loc_func);
	st_exp = StrSocDup(be->bp_exp);

	if(!simplified)
	{
		st_stmt = StrSocDup(be->bp_stmt);
		st_addr = StrSocDup(be->bp_loc.loc_addr);
		
		Asprintf(&stres, "%d %d %d %d %d %s %d %d %d %d %s %d %s %d %s %d %d %s",
			be->bp_id,
			simplified,
			be->bp_ignore,
			be->bp_hits,
			(int)strlen(st_stmt),
			st_stmt,
			be->bp_enabled,
			be->bp_disp,
			be->bp_type,
			(int)strlen(st_file),
			st_file,
			(int)strlen(st_func),
			st_func,
			(int)strlen(st_addr),
			st_addr,
			be->bp_loc.loc_line,
			(int)strlen(st_exp),
			st_exp);
			
		Free(st_stmt);
		Free(st_addr);
	} else 
	{
		Asprintf(&stres, "%d %d %d %d %d %d %s %d %s %d %d %s",
			be->bp_id,
			simplified,
			be->bp_enabled,
			be->bp_disp,
			be->bp_type,
			(int)strlen(st_file),
			st_file,
			(int)strlen(st_func),
			st_func,
			be->bp_loc.loc_line,
			(int)strlen(st_exp),
			st_exp);
	}

	Free(st_file);
	Free(st_func);
	Free(st_exp);

	return stres;
}

static char *
EventSigToSocProto(dbgsig_t se)
{
	char *	stres = NULL;
	char *	sig;
	char *	line;
	char *	frame;
	
	sig = StrSocDup(se.sig_type);
	line = StrSocDup(se.sig_line);
	
	// get the frame
	frame = EventFrameToSocProto(se.sig_frame);

	Asprintf(&stres, "%d %s %d %s %d %s",
		(int)strlen(sig),
		sig,
		(int)strlen(line),
		line,
		(int)strlen(frame),
		frame);
		
	Free(sig);
	Free(line);
	Free(frame);
	
	return stres;
}

static char *
EventStepToSocProto(dbgstep_t se)
{
	char *	stres = NULL;
	char *	line;
	char *	frame;
	
	line = StrSocDup(se.step_line);
	
	// Get the frame
	frame = EventFrameToSocProto(se.step_frame);
	
	Asprintf(&stres, "%d %d %s %d %s",
		se.step_lno,
		(int)strlen(line),
		line,
		(int)strlen(frame),
		frame);
		
	Free(line);
	Free(frame);
	
	return stres;
}

static char *
EventFrameToSocProto(dbgframe_t *fe)
{
	char *	stres = strdup("");
	char *	tmp_str;
	char *	frame;
	char *	file;
	char *	func;
	char *	addr;

	for ( ; fe != NULL; fe=fe->frame_next)
	{
		file = StrSocDup(fe->frame_loc.loc_file);
		func = StrSocDup(fe->frame_loc.loc_func);
		addr = StrSocDup(fe->frame_loc.loc_addr);
		Asprintf(&frame, "%d %d %s %d %s %d %s %d ", 
			fe->frame_level, 
			(int)strlen(file),
			file, 
			(int)strlen(func),
			func, 
			(int)strlen(addr),
			addr, 
			fe->frame_loc.loc_line);
		Asprintf(&tmp_str, "%s%s", stres, frame);
		// Free old strings
		Free(frame);
		Free(stres);
		Free(file);
		Free(func);
		Free(addr);
		stres = tmp_str;
	}

	return stres;
}

static char *
EventLocToSocProto(dbgloc_t *le)
{
	char *	stres;
	char *	file;
	char *	func;
	char *	addr;

	if ( le == (dbgloc_t *)NULL )
		return strdup("");

	file = StrSocDup(le->loc_file);
	func = StrSocDup(le->loc_func);
	addr = StrSocDup(le->loc_addr);

	Asprintf(&stres, "%d %s %d %s %d %s %d", 
		(int)strlen(file),
		file, 
		(int)strlen(func),
		func, 
		(int)strlen(addr),
		addr, 
		le->loc_line);

	Free(file);
	Free(func);
	Free(addr);

	return stres;
}

static char *
EventVarsToSocProto(dbgvars_t *ve)
{
	char *	stres = strdup("");
	char *	tmp;
	char *	elem;
	char *	type;
	
	for ( ; ve != NULL; ve=ve->var_next)
	{
		if (ve->var_name == (char *)NULL)
		{
			continue;
		}
		
		type = StrSocDup(ve->var_type);
		
		// create the name and type
		Asprintf(&elem, "%d %s %d %s ", 
			(int)strlen(ve->var_name), 
			ve->var_name,
			(int)strlen(type),
			type);
			
		// append elem to stres
		Asprintf(&tmp, "%s%s", stres, elem);
		
		// Free the old strings
		Free(type);
		Free(elem);
		Free(stres);
		
		stres = tmp;
	}

	return stres;
}

static char *
EventStringToSocProto(char *te)
{
	char *	stres = NULL;
	char *	type;
	
	type = StrSocDup(te);
	
	Asprintf(&stres, "%d %s", (int)strlen(type), type);
	
	Free(type);
	
	return stres;
}

static char *
EventListToSocProto(dbglist_t *le)
{
	char *	stres = strdup("");
	char *	tmp;
	char *	elem;
	char *	line;

	for ( ; le != NULL; le = le->list_next )
	{
		line = StrSocDup(le->list_line);
		Asprintf(&elem, "%d %d %s ",
			le->list_lno,
			(int)strlen(line),
			line);
		
		// append elem to stres
		Asprintf(&tmp, "%s%s", stres, elem);
		
		// Free the old strings
		Free(line);
		Free(elem);
		Free(stres);
		
		stres = tmp;
	}

	return stres;
}

char *
AIFToSocProto(AIF *a)
{
	int		fl;
	char *	aif_format;
	int		dl;
	char *	aif_data;
	char *  stres;
	int		i;
	char *	src;
	char *	dst;
	char	ch;
	
	if (a == NULL)
		return strdup("");

	// Can directly print the format
	fl = strlen(AIF_FORMAT(a));
	aif_format = AIF_FORMAT(a);

	// Convert the data to string format
	dl = AIF_LEN(a);
	if (dl <= 0)
	{
		aif_data = NULL;
	} else 
	{
		aif_data = (char *) Alloc(2*dl+1);
		src = AIF_DATA(a);
		dst = aif_data;
		for (i = 0; i < dl; i++) 
		{
			ch = *src++;
			// Need to ensure that the NULL char doesn't appear, hence the '0' offset
			// FIXME: This seems messed up and a dumb way of transferring data.
			// Someone needs to investigate if this is actually necessary...
			*dst++ = ((ch >> 4) & 0xf) + '0';
			*dst++ =  (ch       & 0xf) + '0';
		}
		*dst = '\0';
	}
	
	if ( fl <= 0  &&  dl <= 0 ) 
	{
		Asprintf(&stres, "%d %d", fl, dl);
	} else if ( fl <= 0  &&  dl > 0) 
	{
		Asprintf(&stres, "%d %d %d %s", fl, dl, strlen(aif_data), aif_data);
	} else if ( fl > 0  &&  dl <= 0) 
	{
		Asprintf(&stres, "%d %s %d", fl, aif_format, dl);
	} else if ( fl > 0  &&  dl > 0) 
	{
		Asprintf(&stres, "%d %s %d %d %s", fl, aif_format, dl, strlen(aif_data), aif_data);
	}

	if ( aif_data != NULL )
		Free(aif_data);

	return stres;
}

static char *
EventStatusToSocProto(dbgstatus_t *se)
{
	char *	stres = NULL;
	char *	loc;
	
	if (se == NULL)
		return strdup("");
	
	// Get the location
	loc = EventLocToSocProto(se->stat_loc);
	
	Asprintf(&stres, "%d %d %s",
		se->stat, 
		(int)strlen(loc),
		loc);
		
	Free(loc);
	
	return stres;
}

static char *
EventVersToSocProto(dbgvers_t ve)
{
	char *	stres = NULL;
	char *	vers;
	char *	arch;
	
	vers = StrSocDup(ve.vers_version);
	arch = StrSocDup(ve.vers_arch);
	
	Asprintf(&stres, "%d %s %d %s",
		(int)strlen(vers),
		vers,
		(int)strlen(arch),
		arch);
		
	Free(vers);
	Free(arch);
	
	return stres;
}

static char *
EventIntToSocProto(int pe)
{
	char *	stres = NULL;
	
	Asprintf(&stres, "%d", pe);
	
	return stres;
}

static char *
EventSourcesToSocProto(dbgsources_t *se)
{
	char *	stres = strdup("");
	char *	tmp;
	char *	elem;
	char *	file;
	
	for ( ; se != NULL; se = se->sources_next )
	{
		file = StrSocDup(se->sources_file);
		Asprintf(&elem, "%d %s ",
			(int)strlen(file),
			file);
			
		// append elem to stres
		Asprintf(&tmp, "%s%s", stres, elem);
		
		// Free the old strings
		Free(file);
		Free(elem);
		Free(stres);
		
		stres = tmp;
	}
	
	return stres;
}

/*
** This is the main function that converts an event to network protocol
*/

char *
EventToSocProto(dbgevent_t *e)
{
	dbgev	ev;
	char *	evdata;
	char *	stres = NULL;

	if ( e == (dbgevent_t *)NULL )
	{
		// There is no data that goes along with this event
		Asprintf(&stres, "%d", DBGEV_NONE);
		
		return(stres);
	}

	ev = e->ev_event;

	switch ( ev )
	{
	case DBGEV_ERROR:
		evdata = EventErrorToSocProto(e->ev_err);
		Asprintf(&stres, "%d %d %s", ev, (int)strlen(evdata), evdata);
		Free(evdata);
		break;

	case DBGEV_NONE:
	case DBGEV_OK:
		// There is no data that goes along with this event
		Asprintf(&stres, "%d", ev);
		break;
		
	// This handles the generic string events
	case DBGEV_SOURCEPATH:
	case DBGEV_TYPE:
	case DBGEV_CMD:
		evdata = EventStringToSocProto(e->ev_string);
		Asprintf(&stres, "%d %d %s", ev, (int)strlen(evdata), evdata);
		Free(evdata);
		break;
	
	// This handles the generic int events
	case DBGEV_EXIT:
		evdata = EventIntToSocProto(e->ev_int);
		Asprintf(&stres, "%d %d %s", ev, (int)strlen(evdata), evdata);
		Free(evdata);
		break;

	case DBGEV_BPHIT:
		evdata = EventBPToSocProto(e->ev_bp, 1);
		Asprintf(&stres, "%d %d %s", ev, (int)strlen(evdata), evdata);
		Free(evdata);
		break;

	case DBGEV_BPINFO:
		evdata = EventBPToSocProto(e->ev_bp, 0);
		Asprintf(&stres, "%d %d %s", ev, (int)strlen(evdata), evdata);
		Free(evdata);
		break;

	case DBGEV_BPSET:
		evdata = EventBPToSocProto(e->ev_bp, 1);
		Asprintf(&stres, "%d %d %s", ev, (int)strlen(evdata), evdata);
		Free(evdata);
		break;

	case DBGEV_SIGNAL:
		evdata = EventSigToSocProto(e->ev_sig);
		Asprintf(&stres, "%d %d %s", ev, (int)strlen(evdata), evdata);
		Free(evdata);
		break;
		
	case DBGEV_STEP:
		evdata = EventStepToSocProto(e->ev_step);
		Asprintf(&stres, "%d %d %s", ev, (int)strlen(evdata), evdata);
		Free(evdata);
		break;

	case DBGEV_FRAME:
		evdata = EventFrameToSocProto(e->ev_frame);
		Asprintf(&stres, "%d %d %s", ev, (int)strlen(evdata), evdata);
		Free(evdata);
		break;

	case DBGEV_HALT:
	case DBGEV_LOC:
		evdata = EventLocToSocProto(e->ev_loc);
		Asprintf(&stres, "%d %d %s", ev, (int)strlen(evdata), evdata);
		Free(evdata);
		break;

	case DBGEV_VARS:
		evdata = EventVarsToSocProto(e->ev_vars);
		Asprintf(&stres, "%d %d %s", ev, (int)strlen(evdata), evdata);
		Free(evdata);
		break;

	case DBGEV_LIST:
		evdata = EventListToSocProto(e->ev_list);
		Asprintf(&stres, "%d %d %s", ev, (int)strlen(evdata), evdata);
		Free(evdata);
		break;

	case DBGEV_DATA:
		evdata = AIFToSocProto(e->ev_data);
		Asprintf(&stres, "%d %d %s", ev, (int)strlen(evdata), evdata);
		Free(evdata);
		break;

	case DBGEV_STAT:
		evdata = EventStatusToSocProto(e->ev_status);
		Asprintf(&stres, "%d %d %s", ev, (int)strlen(evdata), evdata);
		Free(evdata);
		break;

	case DBGEV_VERS:
		evdata = EventVersToSocProto(e->ev_vers);
		Asprintf(&stres, "%d %d %s", ev, (int)strlen(evdata), evdata);
		Free(evdata);
		break;
		
	case DBGEV_SOURCES:
		evdata = EventSourcesToSocProto(e->ev_sources);
		Asprintf(&stres, "%d %d %s", ev, (int)strlen(evdata), evdata);
		Free(evdata);
		break;
	}

	FreeEvent(e);

	return(stres);
}



/*
** The following are helper functions for converting commands into a string for
** network transfer
*/

static char *
CmdArgsInvokeToSocProto(va_list ap)
{
	char *			res;
	cmdargs_invoke	args;
	
	args.numbe = va_arg(ap, int);
	
	Asprintf(&res, "%d", args.numbe);
	
	return res;
}

static char *
CmdArgsSetLineBreakToSocProto(va_list ap)
{
	char *					res;
	cmdargs_setlinebreak	args;
	
	args.file = va_arg(ap, char *);
	args.lno = va_arg(ap, int);
	
	FMT_CHECK_NULL_STR(args.file);
	
	Asprintf(&res, "%d %s %d", 
		(int)strlen(args.file), 
		args.file, 
		args.lno);
	
	return res;
}

static char *
CmdArgsSetFuncBreakToSocProto(va_list ap)
{
	char *					res;
	cmdargs_setfuncbreak	args;
	
	args.file = va_arg(ap, char *);
	args.func = va_arg(ap, char *);
	
	FMT_CHECK_NULL_STR(args.file);
	FMT_CHECK_NULL_STR(args.func);
	
	Asprintf(&res, "%d %s %d %s", 
		(int)strlen(args.file), 
		args.file, 
		(int)strlen(args.func),
		args.func);
	
	return res;
}

static char *
CmdArgsSetWatchToSocProto(va_list ap)
{
	char *					res;
	cmdargs_setwatch		args;
	
	args.type = va_arg(ap, int);
	args.expr = va_arg(ap, char *);
	
	FMT_CHECK_NULL_STR(args.expr);
	
	Asprintf(&res, "%d %d %s", 
		args.type,
		(int)strlen(args.expr), 
		args.expr);
	
	return res;
}

static char *
CmdArgsDeleteBreakToSocProto(va_list ap)
{
	char *					res;
	cmdargs_deletebreak		args;
	
	args.bpid = va_arg(ap, int);
	
	Asprintf(&res, "%d", args.bpid);
	
	return res;
}

static char *
CmdArgsDisableBreakToSocProto(va_list ap)
{
	char *					res;
	cmdargs_disablebreak	args;
	
	args.bpid = va_arg(ap, int);
	
	Asprintf(&res, "%d", args.bpid);
	
	return res;
}

static char *
CmdArgsEnableBreakToSocProto(va_list ap)
{
	char *					res;
	cmdargs_enablebreak		args;
	
	args.bpid = va_arg(ap, int);
	args.once = va_arg(ap, int);
	args.del = va_arg(ap, int);
	
	Asprintf(&res, "%d %d %d", 
		args.bpid, 
		args.once, 
		args.del);
	
	return res;
}

static char *
CmdArgsListToSocProto(va_list ap)
{
	char *					res;
	cmdargs_list			args;
	
	args.list_expr = va_arg(ap, char *);
	
	FMT_CHECK_NULL_STR(args.list_expr);
	
	Asprintf(&res, "%d %s", 
		(int)strlen(args.list_expr), 
		args.list_expr);
	
	return res;
}

static char *
CmdArgsSetArgsToSocProto(va_list ap)
{
	char *					res;
	cmdargs_setargs			args;
	
	args.args = va_arg(ap, char *);
	
	FMT_CHECK_NULL_STR(args.args);
	
	Asprintf(&res, "%d %s", 
		(int)strlen(args.args), 
		args.args);
	
	return res;
}

static char *
CmdArgsStepToSocProto(va_list ap)
{
	char *					res;
	cmdargs_step			args;
	
	args.count = va_arg(ap, int);
	args.into = va_arg(ap, int);
	
	Asprintf(&res, "%d %d", 
		args.count,
		args.into);
	
	return res;
}

static char *
CmdArgsMoveFrameToSocProto(va_list ap)
{
	char *					res;
	cmdargs_moveframe		args;
	
	args.count = va_arg(ap, int);
	args.up_direction = va_arg(ap, int);
	
	Asprintf(&res, "%d %d", 
		args.count, 
		args.up_direction);
	
	return res;
}

static char *
CmdArgsShowFrameToSocProto(va_list ap)
{
	char *					res;
	cmdargs_showframe		args;
	
	args.count = va_arg(ap, int);
	
	Asprintf(&res, "%d", args.count);
	
	return res;
}

static char *
CmdArgsSetDebuggerVarToSocProto(va_list ap)
{
	char *					res;
	cmdargs_setdebuggervar	args;
	char *					aif_str;
	
	args.var = va_arg(ap, char *);
	args.val = va_arg(ap, AIF *);
	
	FMT_CHECK_NULL_STR(args.var);
	
	// Covert the AIF * to string
	aif_str = AIFToSocProto(args.val);
	
	Asprintf(&res, "%d %s %d %s", 
		(int)strlen(args.var),
		args.var,
		(int)strlen(aif_str),
		aif_str);

	Free(aif_str);
	
	return res;
}

static char *
CmdArgsSetSourcePathToSocProto(va_list ap)
{
	char *					res;
	cmdargs_setsourcepath	args;
	
	args.path = va_arg(ap, char *);
	
	Asprintf(&res, "%d %s",
		(int)strlen(args.path),
		args.path);
	
	return res;
}

static char *
CmdArgsGetDebuggerVarToSocProto(va_list ap)
{
	char *					res;
	cmdargs_getdebuggervar	args;
	
	args.var = va_arg(ap, char *);
	
	FMT_CHECK_NULL_STR(args.var);
	
	Asprintf(&res, "%d %s", 
		(int)strlen(args.var), 
		args.var);
	
	return res;
}

static char *
CmdArgsSetVarToSocProto(va_list ap)
{
	char *					res;
	cmdargs_setvar			args;
	char *					aif_str;
	
	args.var = va_arg(ap, char *);
	args.val = va_arg(ap, AIF *);
	
	FMT_CHECK_NULL_STR(args.var);
	
	// Covert the AIF * to string
	aif_str = AIFToSocProto(args.val);
	
	Asprintf(&res, "%d %s %d %s", 
		(int)strlen(args.var),
		args.var,
		(int)strlen(aif_str),
		aif_str);

	Free(aif_str);
	
	return res;
}

static char *
CmdArgsEvalExprToSocProto(va_list ap)
{
	char *					res;
	cmdargs_evalexpr		args;
	
	args.expr = va_arg(ap, char *);
	
	FMT_CHECK_NULL_STR(args.expr);
	
	Asprintf(&res, "%d %s", 
		(int)strlen(args.expr), 
		args.expr);
	
	return res;
}

static char *
CmdArgsGetTypeToSocProto(va_list ap)
{
	char *					res;
	cmdargs_gettype			args;
	
	args.sym = va_arg(ap, char *);
	args.isType = va_arg(ap, int);
	
	FMT_CHECK_NULL_STR(args.sym);
	
	Asprintf(&res, "%d %s %d", 
		(int)strlen(args.sym), 
		args.sym,
		args.isType);
	
	return res;
}

static char *
CmdArgsGetVarsToSocProto(va_list ap)
{
	char *					res;
	cmdargs_getvars			args;
	
	args.file = va_arg(ap, char *);
	
	FMT_CHECK_NULL_STR(args.file);
	
	Asprintf(&res, "%d %s", 
		(int)strlen(args.file), 
		args.file);
	
	return res;
}

static char *
CmdArgsGetScopeToSocProto(va_list ap)
{
	char *					res;
	cmdargs_getscope		args;
	
	args.location = va_arg(ap, char *);
	
	FMT_CHECK_NULL_STR(args.location);
	
	Asprintf(&res, "%d %s", 
		(int)strlen(args.location), 
		args.location);
	
	return res;
}

static char *
CmdArgsDirectToSocProto(va_list ap)
{
	char *					res;
	cmdargs_direct			args;
	int						i;
	char *					tmp;
	
	args.cmd_argc = va_arg(ap, int);
	args.cmd_argv = va_arg(ap, char **);
	
	Asprintf(&res, "%d ", args.cmd_argc);
	
	// Need to loop over the argv array
	for (i=0; i < args.cmd_argc; ++i)
	{
		Asprintf(&tmp, "%s%d %s ",
			res,
			(int)strlen(args.cmd_argv[i]),
			args.cmd_argv[i]);
		
		// Free the old res
		Free(res);
		res = tmp;
	}
	
	return res;
}

static char *
CmdArgsStatusToSocProto(va_list ap)
{
	char *					res;
	cmdargs_status			args;
	
	args.getLocation = va_arg(ap, int);
	
	Asprintf(&res, "%d", args.getLocation);
	
	return res;
}

static char *
CmdArgsQuitToSocProto(va_list ap)
{
	char *					res;
	cmdargs_quit			args;
	
	args.detach = va_arg(ap, int);
	
	Asprintf(&res, "%d", args.detach);
	
	return res;
}

static char *
CmdArgsOpenSocketToSocProto(va_list ap)
{
	char *					res;
	cmdargs_opensocket		args;
	
	args.opset_name = va_arg(ap, char *);
	args.opset_size = va_arg(ap, int);
	
	FMT_CHECK_NULL_STR(args.opset_name);
	
	Asprintf(&res, "%d %s %d", 
		(int)strlen(args.opset_name), 
		args.opset_name, 
		args.opset_size);
	
	return res;
}

/*
** This is the main function that converts a command to network protocol
*/
char *
CommandToSocProto(dbgcmd cmd, ...)
{
	va_list	ap;
	char *	cmdargs;
	char *	stres = NULL;
	
	// setup the args
	va_start(ap, cmd);
	
	switch (cmd)
	{
	case DBGCMD_INVOKE:
		cmdargs = CmdArgsInvokeToSocProto(ap);
		Asprintf(&stres, "%d %d %s", cmd, (int)strlen(cmdargs), cmdargs);
		Free(cmdargs);
		break;
		
	case DBGCMD_SETLINEBREAK:
		cmdargs = CmdArgsSetLineBreakToSocProto(ap);
		Asprintf(&stres, "%d %d %s", cmd, (int)strlen(cmdargs), cmdargs);
		Free(cmdargs);
		break;
		
	case DBGCMD_SETFUNCBREAK:
		cmdargs = CmdArgsSetFuncBreakToSocProto(ap);
		Asprintf(&stres, "%d %d %s", cmd, (int)strlen(cmdargs), cmdargs);
		Free(cmdargs);
		break;
		
	case DBGCMD_SETWATCH:
		cmdargs = CmdArgsSetWatchToSocProto(ap);
		Asprintf(&stres, "%d %d %s", cmd, (int)strlen(cmdargs), cmdargs);
		Free(cmdargs);
		break;
		
	case DBGCMD_DELETEBREAK:
		cmdargs = CmdArgsDeleteBreakToSocProto(ap);
		Asprintf(&stres, "%d %d %s", cmd, (int)strlen(cmdargs), cmdargs);
		Free(cmdargs);
		break;
		
	case DBGCMD_DISABLEBREAK:
		cmdargs = CmdArgsDisableBreakToSocProto(ap);
		Asprintf(&stres, "%d %d %s", cmd, (int)strlen(cmdargs), cmdargs);
		Free(cmdargs);
		break;
		
	case DBGCMD_ENABLEBREAK:
		cmdargs = CmdArgsEnableBreakToSocProto(ap);
		Asprintf(&stres, "%d %d %s", cmd, (int)strlen(cmdargs), cmdargs);
		Free(cmdargs);
		break;
	
	case DBGCMD_LIST:
		cmdargs = CmdArgsListToSocProto(ap);
		Asprintf(&stres, "%d %d %s", cmd, (int)strlen(cmdargs), cmdargs);
		Free(cmdargs);
		break;
		
	case DBGCMD_SETARGS:
		cmdargs = CmdArgsSetArgsToSocProto(ap);
		Asprintf(&stres, "%d %d %s", cmd, (int)strlen(cmdargs), cmdargs);
		Free(cmdargs);
		break;
		
	case DBGCMD_STEP:
		cmdargs = CmdArgsStepToSocProto(ap);
		Asprintf(&stres, "%d %d %s", cmd, (int)strlen(cmdargs), cmdargs);
		Free(cmdargs);
		break;
		
	case DBGCMD_MOVEFRAME:
		cmdargs = CmdArgsMoveFrameToSocProto(ap);
		Asprintf(&stres, "%d %d %s", cmd, (int)strlen(cmdargs), cmdargs);
		Free(cmdargs);
		break;
		
	case DBGCMD_SHOWFRAME:
		cmdargs = CmdArgsShowFrameToSocProto(ap);
		Asprintf(&stres, "%d %d %s", cmd, (int)strlen(cmdargs), cmdargs);
		Free(cmdargs);
		break;
		
	case DBGCMD_SETDEBUGGERVAR:
		cmdargs = CmdArgsSetDebuggerVarToSocProto(ap);
		Asprintf(&stres, "%d %d %s", cmd, (int)strlen(cmdargs), cmdargs);
		Free(cmdargs);
		break;
		
	case DBGCMD_SETSOURCEPATH:
		cmdargs = CmdArgsSetSourcePathToSocProto(ap);
		Asprintf(&stres, "%d %d %s", cmd, (int)strlen(cmdargs), cmdargs);
		Free(cmdargs);
		break;
		
	case DBGCMD_GETDEBUGGERVAR:
		cmdargs = CmdArgsGetDebuggerVarToSocProto(ap);
		Asprintf(&stres, "%d %d %s", cmd, (int)strlen(cmdargs), cmdargs);
		Free(cmdargs);
		break;
		
	case DBGCMD_SETVAR:
		cmdargs = CmdArgsSetVarToSocProto(ap);
		Asprintf(&stres, "%d %d %s", cmd, (int)strlen(cmdargs), cmdargs);
		Free(cmdargs);
		break;
		
	case DBGCMD_EVALEXPR:
		cmdargs = CmdArgsEvalExprToSocProto(ap);
		Asprintf(&stres, "%d %d %s", cmd, (int)strlen(cmdargs), cmdargs);
		Free(cmdargs);
		break;
		
	case DBGCMD_GETTYPE:
		cmdargs = CmdArgsGetTypeToSocProto(ap);
		Asprintf(&stres, "%d %d %s", cmd, (int)strlen(cmdargs), cmdargs);
		Free(cmdargs);
		break;
		
	case DBGCMD_GETVARS:
		cmdargs = CmdArgsGetVarsToSocProto(ap);
		Asprintf(&stres, "%d %d %s", cmd, (int)strlen(cmdargs), cmdargs);
		Free(cmdargs);
		break;
		
	case DBGCMD_GETSCOPE:
		cmdargs = CmdArgsGetScopeToSocProto(ap);
		Asprintf(&stres, "%d %d %s", cmd, (int)strlen(cmdargs), cmdargs);
		Free(cmdargs);
		break;
		
	case DBGCMD_DIRECT:
		cmdargs = CmdArgsDirectToSocProto(ap);
		Asprintf(&stres, "%d %d %s", cmd, (int)strlen(cmdargs), cmdargs);
		Free(cmdargs);
		break;
		
	case DBGCMD_STATUS:
		cmdargs = CmdArgsStatusToSocProto(ap);
		Asprintf(&stres, "%d %d %s", cmd, (int)strlen(cmdargs), cmdargs);
		Free(cmdargs);
		break;
		
	case DBGCMD_QUIT:
		cmdargs = CmdArgsQuitToSocProto(ap);
		Asprintf(&stres, "%d %d %s", cmd, (int)strlen(cmdargs), cmdargs);
		Free(cmdargs);
		break;
		
	case DBGCMD_OPENSOCKET:
		cmdargs = CmdArgsOpenSocketToSocProto(ap);
		Asprintf(&stres, "%d %d %s", cmd, (int)strlen(cmdargs), cmdargs);
		Free(cmdargs);
		break;
		
	// Commands without arguments go here
	case DBGCMD_STARTUP:
	case DBGCMD_LASTBREAK:
	case DBGCMD_GETSOURCES:
	case DBGCMD_GO:
	case DBGCMD_FINISH:
	case DBGCMD_KILL:
	case DBGCMD_GETFRAME:
	case DBGCMD_GETSOURCEPATH:
	case DBGCMD_GETLOCATION:
	case DBGCMD_INTERRUPT:
	case DBGCMD_CLOSESOCKET:
		Asprintf(&stres, "%d", cmd);
		break;
	}
	
	va_end(ap);

	return stres;
}



/*********************************
** From network proto functions **
*********************************/


/*
** The following are helper functions for converting a network string back into
** an event
*/

static dbgerr_t
SocProtoToErr(char *s)
{
	dbgerr_t	res;
	char *		tmp_str;
	
	if (s == NULL || *s == '\0')
	{
		res.err_errno = DBGERR_UNKNOWN;
		res.err_str = strdup("");
		
		return res;
	}
	
	// Get the errno
	res.err_errno = getint(&s);
	
	// Get the errstr
	res.err_str = getstring(&s);
	
	// Don't want to modify the err_str if its length is 0
	if (strlen(res.err_str) == 0)
	{
		return res;
	}
	
	// Replace any \\t or \\n in the err_str with \t and \n respectively
	tmp_str = ReplaceStr(res.err_str, "\\t", "\t");
	// Free the old string
	Free(res.err_str);
	// Set the new string
	res.err_str = ReplaceStr(tmp_str, "\\n", "\n");
	// Free the tmp_str
	Free(tmp_str);
	
	return res;
}

static dbgloc_t *
SocProtoToLoc(char *s)
{
	dbgloc_t *	res;
	char *		tmp_file;

	NULL_GUARD(s);

	// allocate the res
	res = (dbgloc_t *)Alloc(sizeof(dbgloc_t));

	// set the members
	res->loc_file = getstring(&s);
	res->loc_func = getstring(&s);
	res->loc_addr = getstring(&s);
	res->loc_line = getint(&s);
	
	// We should try to resolve the realpath of file on this end. If it fails,
	// we will use the original value.
	if ((tmp_file = realpath(res->loc_file, NULL)) != NULL)
	{
		// Free the old loc_file
		Free(res->loc_file);
		// Set the new loc_file
		res->loc_file = tmp_file;
	}

	return res;
}

static dbgvars_t *
SocProtoToVars(char *s)
{
	dbgvars_t *		res = NULL;
	dbgvars_t **    vp;
	
	NULL_GUARD(s);
	
	vp = &res;

	// Loop until we find a null terminator
	for ( ; *s != '\0'; s = skipspace(s))
	{
		*vp = (dbgvars_t *)Alloc(sizeof(dbgvars_t));
		memset(*vp, 0, sizeof(dbgvars_t));
		
		(*vp)->var_name = getstring(&s);
		(*vp)->var_type = getstring(&s);
		(*vp)->var_next = (dbgvars_t *)NULL;
		
		vp = &(*vp)->var_next;
	}

	return res;
}

AIF *
SocProtoToAIF(char *s)
{
	AIF *	res;
	char *	aif_fmt;
	int		dl;
	char *	aif_data;
	char *	dptr;
	char *	aptr;
	int		i;
	
	NULL_GUARD(s);

	res = (AIF *) Alloc(sizeof(AIF));

	// get the format
	aif_fmt = getstring(&s);

	// set the format
	if (*aif_fmt != '\0')
	{
		AIF_FORMAT(res) = aif_fmt;
	} else
	{
		Free(aif_fmt);
		AIF_FORMAT(res) = (char *)NULL;
	}
	
	// get the length of the actual aif data
	dl = getint(&s);
	AIF_LEN(res) = dl;
	
	if (dl <= 0)
	{
		AIF_DATA(res) = (char *)NULL;
	} else
	{	
		// Get the aif_data string
		aif_data = getstring(&s);
		
		if (*aif_data == '\0')
		{
			// Data is empty, so reset to empty
			AIF_LEN(res) = 0;
			AIF_DATA(res) = (char *)NULL;
		} else
		{
			AIF_DATA(res) = (char *)Alloc(dl);
			aptr = AIF_DATA(res);
			dptr = aif_data;
			// FIXME: This seems messed up and a dumb way of transferring data.
			// Someone needs to investigate if this is actually necessary...
			for (i=0; i < dl; i++)
			{
				*aptr = (*dptr - '0') * 0x10 + (*(dptr+1) - '0');
				aptr++;
				dptr += 2;
			}
		}
		
		Free(aif_data);
	}
	
	return res;
}

static dbglist_t *
SocProtoToList(char *s)
{
	dbglist_t *		res = NULL;
	dbglist_t **	lp;
	
	NULL_GUARD(s);
	
	lp = &res;
	
	// Loop until we find a null terminator
	for ( ; *s != '\0'; s = skipspace(s))
	{
		*lp = (dbglist_t *)Alloc(sizeof(dbglist_t));
		memset(*lp, 0, sizeof(dbglist_t));
		
		(*lp)->list_lno = getint(&s);
		(*lp)->list_line = getstring(&s);
		(*lp)->list_next = (dbglist_t *)NULL;
		
		lp = &(*lp)->list_next;
	}

	return res;
}

static dbgbp_t *
SocProtoToBP(char *s)
{
	dbgbp_t *	res;
	int			id;
	int			simple;
	char *		tmp_file;

	NULL_GUARD(s);
	
	// Get the breakpoint id
	id = getint(&s);
	
	// Create the new breakpoint
	res = NewBP(id);
	
	// Get value of simplified
	simple = getint(&s);
	
	// simple breakpoints don't contain this information
	if(!simple)
	{
		res->bp_ignore = getint(&s);
		res->bp_hits = getint(&s);
		res->bp_stmt = getstring(&s);
	}
	
	res->bp_enabled = getint(&s);
	res->bp_disp = getint(&s);
	res->bp_type = getint(&s);
	res->bp_loc.loc_file = getstring(&s);
	res->bp_loc.loc_func = getstring(&s);
	
	// simple breakpoints don't contain this information
	if(!simple)
	{
		res->bp_loc.loc_addr = getstring(&s);
	}
	
	res->bp_loc.loc_line = getint(&s);
	res->bp_exp = getstring(&s);
	res->bp_next = (dbgbp_t *)NULL;
	
	// We should try to resolve the realpath of file on this end. If it fails,
	// we will use the original value.
	if ((tmp_file = realpath(res->bp_loc.loc_file, NULL)) != NULL)
	{
		// Free the old loc_file
		Free(res->bp_loc.loc_file);
		// Set the new loc_file
		res->bp_loc.loc_file = tmp_file;
	}

	return res;
}

static dbgsig_t
SocProtoToSig(char *s)
{
	dbgsig_t	res;
	char *		fstr;

	if (s == NULL || *s == '\0')
	{
		res.sig_type = strdup("");
		res.sig_line = strdup("");
		res.sig_frame = NULL;
		
		return res;
	}
	
	// get the type
	res.sig_type = getstring(&s);
	
	// get the line
	res.sig_line = getstring(&s); 
	
	// get the frame string and convert it to a frame pointer
	fstr = getstring(&s);
	res.sig_frame = SocProtoToFrame(fstr);
	Free(fstr);
	
	return res;
}

static dbgframe_t *
SocProtoToFrame(char *s)
{
	dbgframe_t *    res = NULL;
	dbgframe_t **   fp;
	char *			tmp_file;

	NULL_GUARD(s);

	fp = &res;
	
	// Loop until we find a null terminator
	for ( ; *s != '\0'; s = skipspace(s))
	{
		*fp = (dbgframe_t *)Alloc(sizeof(dbgframe_t));
		memset(*fp, 0, sizeof(dbgframe_t));
		
		(*fp)->frame_level = getint(&s);
		(*fp)->frame_loc.loc_file = getstring(&s);
		(*fp)->frame_loc.loc_func = getstring(&s);
		(*fp)->frame_loc.loc_addr = getstring(&s);
		(*fp)->frame_loc.loc_line = getint(&s);
		(*fp)->frame_next = (dbgframe_t *)NULL;
		
		// We should try to resolve the realpath of file on this end. If it fails,
		// we will use the original value.
		if ((tmp_file = realpath((*fp)->frame_loc.loc_file, NULL)) != NULL)
		{
			// Free the old loc_file
			Free((*fp)->frame_loc.loc_file);
			// Set the new loc_file
			(*fp)->frame_loc.loc_file = tmp_file;
		}
		
		fp = &(*fp)->frame_next;
	}

	return res;
}

dbgstep_t
SocProtoToStep(char *s)
{
	// FIXME: No way to check for error.......
	dbgstep_t	res;
	char *		fstr;

	if (s == NULL || *s == '\0')
	{
		res.step_lno = 0;
		res.step_line = strdup("");
		res.step_frame = NULL;
		
		return res;
	}
	
	// get the lno
	res.step_lno = getint(&s);
	
	// get the line
	res.step_line = getstring(&s);
	
	// get the frame string
	fstr = getstring(&s);
	res.step_frame = SocProtoToFrame(fstr);
	Free(fstr);
	
	return res;
}

static char *
SocProtoToString(char *s)
{
	char *	res;
	
	if ( s == NULL || *s == '\0') 
	{
		return strdup("");
	}
	
	res = getstring(&s);
	
	return res;
}

static dbgstatus_t *
SocProtoToStatus(char *s)
{
	dbgstatus_t *	res;
	char *			lstr;
	
	NULL_GUARD(s);
	
	// allocate the res
	res = (dbgstatus_t *)Alloc(sizeof(dbgstatus_t));
	
	res->stat = getint(&s);
	
	// get the location string and convert it to a loc pointer
	lstr = getstring(&s);
	res->stat_loc = SocProtoToLoc(lstr);
	Free(lstr);
	
	return res;
}

static dbgvers_t
SocProtoToVers(char *s)
{
	dbgvers_t	res;
	
	if ( s == NULL || *s == '\0')
	{
		res.vers_version = strdup("");
		res.vers_arch = strdup("");
		return res;
	}
	
	// Get the version string
	res.vers_version = getstring(&s);
	
	// Get the arch string
	res.vers_arch = getstring(&s);
	
	return res;
}

static int
SocProtoToInt(char *s)
{
	int	res;
	
	if ( s == NULL || *s == '\0') 
	{
		return 0;
	}
	
	res = getint(&s);
	
	return res;
}

static dbgsources_t *
SocProtoToSources(char *s)
{
	dbgsources_t *	res = NULL;
	dbgsources_t **	sp;
	
	NULL_GUARD(s);
	
	sp = &res;
	
	// Loop until we find a null terminator
	for ( ; *s != '\0'; s = skipspace(s))
	{
		*sp = (dbgsources_t *)Alloc(sizeof(dbgsources_t));
		memset(*sp, 0, sizeof(dbgsources_t));
		
		(*sp)->sources_file = getstring(&s);
		(*sp)->sources_next = (dbgsources_t *)NULL;
		
		sp = &(*sp)->sources_next;
	}

	return res;
}

/*
** This is the main function that converts a network protocol string back into
** an event.
*/

dbgevent_t *
SocProtoToEvent(char *s)
{
	dbgevent_t *	e;
	dbgev			ev;
	char *			evdata;
	// The following are for non-pointer types
	// FIXME: At some point these should be changed to be pointers to make
	// a sane interface.
	dbgerr_t		st_err;
	dbgsig_t		st_sig;
	dbgstep_t		st_step;
	dbgvers_t		st_vers;
	
	if (s == NULL) 
	{
		return (dbgevent_t *)NULL;
	}

	s = skipspace(s);

	if ( *s == '\0' )
	{
		return (dbgevent_t *)NULL;
	}
	
	// We expect the first entry to be an int which is the dbgev
	ev = getint(&s);
	
	// Create the event
	e = NewEvent(ev);

	switch ( ev )
	{
	case DBGEV_NONE:
		FreeEvent(e);
		return (dbgevent_t *)NULL;
	
	case DBGEV_OK:
		// OK has no data
		e = NewEvent(ev);
		break;

	case DBGEV_ERROR:
		// Get the string and convert to dbgerr_t
		evdata = getstring(&s);
		st_err = SocProtoToErr(evdata);
		Free(evdata);
		// Set the event members
		e->ev_errno = st_err.err_errno;
		e->ev_errstr = st_err.err_str;
		break;
		
	// This handles the generic string events
	case DBGEV_SOURCEPATH:
	case DBGEV_TYPE:
	case DBGEV_CMD:
		// Get the string and convert to char * and set directly
		evdata = getstring(&s);
		e->ev_string = SocProtoToString(evdata);
		Free(evdata);
		break;
		
	// This handles the generic int events
	case DBGEV_EXIT:
		// Get the string and convert to int and set directly
		evdata = getstring(&s);
		e->ev_int = SocProtoToInt(evdata);
		Free(evdata);
		break;

	case DBGEV_BPINFO:
	case DBGEV_BPSET:
	case DBGEV_BPHIT:
		// Get the string and convert to dbgbp_t * and set directly
		evdata = getstring(&s);
		e->ev_bp = SocProtoToBP(evdata);
		Free(evdata);
		break;

	case DBGEV_SIGNAL:
		// Get the string and convert to dbgsig_t
		evdata = getstring(&s);
		st_sig = SocProtoToSig(evdata);
		Free(evdata);
		// Set the sig members
		e->ev_sig_type = st_sig.sig_type;
		e->ev_sig_line = st_sig.sig_line;
		e->ev_sig_frame = st_sig.sig_frame;
		break;
		
	case DBGEV_HALT:
	case DBGEV_LOC:
		// get the string and convert to dbgloc_t * and set directly
		evdata = getstring(&s);
		e->ev_loc = SocProtoToLoc(evdata);
		Free(evdata);
		break;

	case DBGEV_STEP:
		// Get the string and convert to dbgstep_t
		evdata = getstring(&s);
		st_step = SocProtoToStep(evdata);
		Free(evdata);
		// Set step members
		e->ev_step_lno = st_step.step_lno;
		e->ev_step_line = st_step.step_line;
		e->ev_step_frame = st_step.step_frame;
		break;
		
	case DBGEV_STAT:
		// Get the string and convert to dbgstatus_t * and set directly
		evdata = getstring(&s);
		e->ev_status = SocProtoToStatus(evdata);
		Free(evdata);
		break;
		
	case DBGEV_FRAME:
		// Get the string and convert to dbgframe_t * and set directly
		evdata = getstring(&s);
		e->ev_frame = SocProtoToFrame(evdata);
		Free(evdata);
		break;
		
	case DBGEV_VARS:
		// Get the string and convert to dbgvars_t * and set directly
		evdata = getstring(&s);
		e->ev_vars = SocProtoToVars(evdata);
		Free(evdata);
		break;

	case DBGEV_LIST:
		// Get the string and convert to dbglist_t * and set directly
		evdata = getstring(&s);
		e->ev_list = SocProtoToList(evdata);
		Free(evdata);
		break;
		
	case DBGEV_DATA:
		// Get the string and convert to AIF * and set directly
		evdata = getstring(&s);
		e->ev_data = SocProtoToAIF(evdata);
		Free(evdata);
		break;
		
	case DBGEV_VERS:
		// Get the string and conver to dbgvers_t
		evdata = getstring(&s);
		st_vers = SocProtoToVers(evdata);
		Free(evdata);
		// Set vers
		e->ev_version = st_vers.vers_version;
		e->ev_arch = st_vers.vers_arch;
		break;
		
	case DBGEV_SOURCES:
		// Get the string and convert to dbgsources_t * and set directly
		evdata = getstring(&s);
		e->ev_sources = SocProtoToSources(evdata);
		Free(evdata);
		break;
	}

	return e;
}



/*
** The following are helper functions for converting a network string back into
** a command
*/

static cmdargs_invoke *
SocProtoToInvokeArgs(char *s)
{
	cmdargs_invoke *	res;
	
	NULL_GUARD(s);
	
	res = (cmdargs_invoke *)Alloc(sizeof(cmdargs_invoke));
	
	res->numbe = getint(&s);
	
	return res;
}

static cmdargs_setlinebreak *
SocProtoToSetLineBreakArgs(char *s)
{
	cmdargs_setlinebreak *	res;
	
	NULL_GUARD(s);
	
	res = (cmdargs_setlinebreak *)Alloc(sizeof(cmdargs_setlinebreak));
	
	res->file = getstring(&s);
	res->lno = getint(&s);
	
	return res;
}

static cmdargs_setfuncbreak *
SocProtoToSetFuncBreakArgs(char *s)
{
	cmdargs_setfuncbreak *	res;
	
	NULL_GUARD(s);
	
	res = (cmdargs_setfuncbreak *)Alloc(sizeof(cmdargs_setfuncbreak));
	
	res->file = getstring(&s);
	res->func = getstring(&s);
	
	return res;
}

static cmdargs_setwatch *
SocProtoToSetWatchArgs(char *s)
{
	cmdargs_setwatch *	res;
	
	NULL_GUARD(s);
	
	res = (cmdargs_setwatch *)Alloc(sizeof(cmdargs_setwatch));
	
	res->type = getint(&s);
	res->expr = getstring(&s);
	
	return res;
}

static cmdargs_deletebreak *
SocProtoToDeleteBreakArgs(char *s)
{
	cmdargs_deletebreak *	res;
	
	NULL_GUARD(s);
	
	res = (cmdargs_deletebreak *)Alloc(sizeof(cmdargs_deletebreak));
	
	res->bpid = getint(&s);
	
	return res;
}

static cmdargs_disablebreak *
SocProtoToDisableBreakArgs(char *s)
{
	cmdargs_disablebreak *	res;
	
	NULL_GUARD(s);
	
	res = (cmdargs_disablebreak *)Alloc(sizeof(cmdargs_disablebreak));
	
	res->bpid = getint(&s);
	
	return res;
}

static cmdargs_enablebreak *
SocProtoToEnableBreakArgs(char *s)
{
	cmdargs_enablebreak *	res;
	
	NULL_GUARD(s);
	
	res = (cmdargs_enablebreak *)Alloc(sizeof(cmdargs_enablebreak));
	
	res->bpid = getint(&s);
	res->once = getint(&s);
	res->del = getint(&s);
	
	return res;
}

static cmdargs_list *
SocProtoToListArgs(char *s)
{
	cmdargs_list *	res;
	
	NULL_GUARD(s);
	
	res = (cmdargs_list *)Alloc(sizeof(cmdargs_list));
	
	res->list_expr = getstring(&s);
	
	return res;
}

static cmdargs_setargs *
SocProtoToSetArgsArgs(char *s)
{
	cmdargs_setargs *	res;
	
	NULL_GUARD(s);
	
	res = (cmdargs_setargs *)Alloc(sizeof(cmdargs_setargs));
	
	res->args = getstring(&s);
	
	return res;
}

static cmdargs_step *
SocProtoToStepArgs(char *s)
{
	cmdargs_step *	res;
	
	NULL_GUARD(s);
	
	res = (cmdargs_step *)Alloc(sizeof(cmdargs_step));
	
	res->count = getint(&s);
	res->into = getint(&s);
	
	return res;
}

static cmdargs_moveframe *
SocProtoToMoveFrameArgs(char *s)
{
	cmdargs_moveframe *	res;
	
	NULL_GUARD(s);
	
	res = (cmdargs_moveframe *)Alloc(sizeof(cmdargs_moveframe));
	
	res->count = getint(&s);
	res->up_direction = getint(&s);
	
	return res;
}

static cmdargs_showframe *
SocProtoToShowFrameArgs(char *s)
{
	cmdargs_showframe *	res;
	
	NULL_GUARD(s);
	
	res = (cmdargs_showframe *)Alloc(sizeof(cmdargs_showframe));
	
	res->count = getint(&s);
	
	return res;
}

static cmdargs_setdebuggervar *
SocProtoToSetDebuggerVarArgs(char *s)
{
	cmdargs_setdebuggervar *	res;
	char *	aif_str;
	
	NULL_GUARD(s);
	
	res = (cmdargs_setdebuggervar *)Alloc(sizeof(cmdargs_setdebuggervar));
	
	res->var = getstring(&s);
	aif_str = getstring(&s);
	res->val = SocProtoToAIF(aif_str);
	Free(aif_str);
	
	return res;
}

static cmdargs_setsourcepath *
SocProtoToSetSourcePathArgs(char *s)
{
	cmdargs_setsourcepath *	res;
	
	NULL_GUARD(s);
	
	res = (cmdargs_setsourcepath *)Alloc(sizeof(cmdargs_setsourcepath));
	
	res->path = getstring(&s);
	
	return res;
}

static cmdargs_getdebuggervar *
SocProtoToGetDebuggerVarArgs(char *s)
{
	cmdargs_getdebuggervar *	res;
	
	NULL_GUARD(s);
	
	res = (cmdargs_getdebuggervar *)Alloc(sizeof(cmdargs_getdebuggervar));
	
	res->var = getstring(&s);
	
	return res;
}

static cmdargs_setvar *
SocProtoToSetVarArgs(char *s)
{
	cmdargs_setvar *	res;
	char *	aif_str;
	
	NULL_GUARD(s);
	
	res = (cmdargs_setvar *)Alloc(sizeof(cmdargs_setvar));
	
	res->var = getstring(&s);
	aif_str = getstring(&s);
	res->val = SocProtoToAIF(aif_str);
	Free(aif_str);
	
	return res;
}

static cmdargs_evalexpr *
SocProtoToEvalExprArgs(char *s)
{
	cmdargs_evalexpr *	res;
	
	NULL_GUARD(s);
	
	res = (cmdargs_evalexpr *)Alloc(sizeof(cmdargs_evalexpr));
	
	res->expr = getstring(&s);
	
	return res;
}

static cmdargs_gettype *
SocProtoToGetTypeArgs(char *s)
{
	cmdargs_gettype *	res;
	
	NULL_GUARD(s);
	
	res = (cmdargs_gettype *)Alloc(sizeof(cmdargs_gettype));
	
	res->sym = getstring(&s);
	res->isType = getint(&s);
	
	return res;
}

static cmdargs_getvars *
SocProtoToGetVarsArgs(char *s)
{
	cmdargs_getvars *	res;
	
	NULL_GUARD(s);
	
	res = (cmdargs_getvars *)Alloc(sizeof(cmdargs_getvars));
	
	res->file = getstring(&s);
	
	return res;
}

static cmdargs_getscope *
SocProtoToGetScopeArgs(char *s)
{
	cmdargs_getscope *	res;
	
	NULL_GUARD(s);
	
	res = (cmdargs_getscope *)Alloc(sizeof(cmdargs_getscope));
	
	res->location = getstring(&s);
	
	return res;
}

static cmdargs_direct *
SocProtoToDirectArgs(char *s)
{
	cmdargs_direct *	res;
	int 				i;
	char **				argv;
	
	NULL_GUARD(s);
	
	res = (cmdargs_direct *)Alloc(sizeof(cmdargs_direct));
	
	// get the argc
	res->cmd_argc = getint(&s);
	
	// return if argc is 0
	if (res->cmd_argc <= 0)
	{
		return res;
	}
	
	// create the argv
	argv = (char **)Alloc(res->cmd_argc * sizeof(char *));
	
	for (i=0; i < res->cmd_argc; ++i)
	{
		argv[i] = getstring(&s);
	}
	
	// set the argv
	res->cmd_argv = argv;
	
	return res;
}

static cmdargs_status *
SocProtoToStatusArgs(char *s)
{
	cmdargs_status *	res;
	
	NULL_GUARD(s);
	
	res = (cmdargs_status *)Alloc(sizeof(cmdargs_status));
	
	res->getLocation = getint(&s);
	
	return res;
}

static cmdargs_quit *
SocProtoToQuitArgs(char *s)
{
	cmdargs_quit *	res;
	
	NULL_GUARD(s);
	
	res = (cmdargs_quit *)Alloc(sizeof(cmdargs_quit));
	
	res->detach = getint(&s);
	
	return res;
}

static cmdargs_opensocket *
SocProtoToOpenSocketArgs(char *s)
{
	cmdargs_opensocket *	res;
	
	NULL_GUARD(s);
	
	res = (cmdargs_opensocket *)Alloc(sizeof(cmdargs_opensocket));
	
	res->opset_name = getstring(&s);
	res->opset_size = getint(&s);
	
	return res;
}

/*
** This is the main function that converts a command from network protocol
*/
dbgcommand_t *
SocProtoToCommand(char *s)
{
	dbgcommand_t *	c;
	dbgcmd			cmd;
	char *			cmdargs;
	
	if (s == NULL) 
	{
		return (dbgcommand_t *)NULL;
	}

	s = skipspace(s);

	if ( *s == '\0' )
	{
		return (dbgcommand_t *)NULL;
	}
	
	// We expect the first entry to be an int which is the dbgcmd
	cmd = getint(&s);
	
	// Create the command
	c = NewCommand(cmd);

	switch ( cmd )
	{
	case DBGCMD_INVOKE:
		cmdargs = getstring(&s);
		c->cmd_arg_invoke = SocProtoToInvokeArgs(cmdargs);
		Free(cmdargs);
		break;
		
	case DBGCMD_SETLINEBREAK:
		cmdargs = getstring(&s);
		c->cmd_arg_setlinebreak = SocProtoToSetLineBreakArgs(cmdargs);
		Free(cmdargs);
		break;
		
	case DBGCMD_SETFUNCBREAK:
		cmdargs = getstring(&s);
		c->cmd_arg_setfuncbreak = SocProtoToSetFuncBreakArgs(cmdargs);
		Free(cmdargs);
		break;
		
	case DBGCMD_SETWATCH:
		cmdargs = getstring(&s);
		c->cmd_arg_setwatch = SocProtoToSetWatchArgs(cmdargs);
		Free(cmdargs);
		break;
		
	case DBGCMD_DELETEBREAK:
		cmdargs = getstring(&s);
		c->cmd_arg_deletebreak = SocProtoToDeleteBreakArgs(cmdargs);
		Free(cmdargs);
		break;
		
	case DBGCMD_DISABLEBREAK:
		cmdargs = getstring(&s);
		c->cmd_arg_disablebreak = SocProtoToDisableBreakArgs(cmdargs);
		Free(cmdargs);
		break;
		
	case DBGCMD_ENABLEBREAK:
		cmdargs = getstring(&s);
		c->cmd_arg_enablebreak = SocProtoToEnableBreakArgs(cmdargs);
		Free(cmdargs);
		break;
	
	case DBGCMD_LIST:
		cmdargs = getstring(&s);
		c->cmd_arg_list = SocProtoToListArgs(cmdargs);
		Free(cmdargs);
		break;
		
	case DBGCMD_SETARGS:
		cmdargs = getstring(&s);
		c->cmd_arg_setargs = SocProtoToSetArgsArgs(cmdargs);
		Free(cmdargs);
		break;
		
	case DBGCMD_STEP:
		cmdargs = getstring(&s);
		c->cmd_arg_step = SocProtoToStepArgs(cmdargs);
		Free(cmdargs);
		break;
		
	case DBGCMD_MOVEFRAME:
		cmdargs = getstring(&s);
		c->cmd_arg_moveframe = SocProtoToMoveFrameArgs(cmdargs);
		Free(cmdargs);
		break;
		
	case DBGCMD_SHOWFRAME:
		cmdargs = getstring(&s);
		c->cmd_arg_showframe = SocProtoToShowFrameArgs(cmdargs);
		Free(cmdargs);
		break;
		
	case DBGCMD_SETDEBUGGERVAR:
		cmdargs = getstring(&s);
		c->cmd_arg_setdebuggervar = SocProtoToSetDebuggerVarArgs(cmdargs);
		Free(cmdargs);
		break;
		
	case DBGCMD_SETSOURCEPATH:
		cmdargs = getstring(&s);
		c->cmd_arg_setsourcepath = SocProtoToSetSourcePathArgs(cmdargs);
		Free(cmdargs);
		break;
		
	case DBGCMD_GETDEBUGGERVAR:
		cmdargs = getstring(&s);
		c->cmd_arg_getdebuggervar = SocProtoToGetDebuggerVarArgs(cmdargs);
		Free(cmdargs);
		break;
		
	case DBGCMD_SETVAR:
		cmdargs = getstring(&s);
		c->cmd_arg_setvar = SocProtoToSetVarArgs(cmdargs);
		Free(cmdargs);
		break;
		
	case DBGCMD_EVALEXPR:
		cmdargs = getstring(&s);
		c->cmd_arg_evalexpr = SocProtoToEvalExprArgs(cmdargs);
		Free(cmdargs);
		break;
		
	case DBGCMD_GETTYPE:
		cmdargs = getstring(&s);
		c->cmd_arg_gettype = SocProtoToGetTypeArgs(cmdargs);
		Free(cmdargs);
		break;
		
	case DBGCMD_GETVARS:
		cmdargs = getstring(&s);
		c->cmd_arg_getvars = SocProtoToGetVarsArgs(cmdargs);
		Free(cmdargs);
		break;
		
	case DBGCMD_GETSCOPE:
		cmdargs = getstring(&s);
		c->cmd_arg_getscope = SocProtoToGetScopeArgs(cmdargs);
		Free(cmdargs);
		break;
		
	case DBGCMD_DIRECT:
		cmdargs = getstring(&s);
		c->cmd_arg_direct = SocProtoToDirectArgs(cmdargs);
		Free(cmdargs);
		break;
		
	case DBGCMD_STATUS:
		cmdargs = getstring(&s);
		c->cmd_arg_status = SocProtoToStatusArgs(cmdargs);
		Free(cmdargs);
		break;
		
	case DBGCMD_QUIT:
		cmdargs = getstring(&s);
		c->cmd_arg_quit = SocProtoToQuitArgs(cmdargs);
		Free(cmdargs);
		break;
		
	case DBGCMD_OPENSOCKET:
		cmdargs = getstring(&s);
		c->cmd_arg_opensocket = SocProtoToOpenSocketArgs(cmdargs);
		Free(cmdargs);
		break;
		
	// Commands without arguments go here
	case DBGCMD_STARTUP:
	case DBGCMD_LASTBREAK:
	case DBGCMD_GETSOURCES:
	case DBGCMD_GO:
	case DBGCMD_FINISH:
	case DBGCMD_KILL:
	case DBGCMD_GETFRAME:
	case DBGCMD_GETSOURCEPATH:
	case DBGCMD_GETLOCATION:
	case DBGCMD_INTERRUPT:
	case DBGCMD_CLOSESOCKET:
		break;
	}
	
	return c;	
}

