/*
** Debug server protocol definition.
**
** Copyright (c) 1996-2002 by Guardsoft Pty Ltd.
**
*/

#ifdef RPC_HDR
%#ifndef _DBGPROTO_H
%#define _DBGPROTO_H
#endif /* RPC_HDR */

%#include	"defs.h"
%#include	"aif.h"
%#include	"list.h"
%#include	"dbgevent.h"

%extern bool_t	xdr_AIF();

enum dbgcmd
{
	CMD_SETFUNCBREAK,
	CMD_SETLINEBREAK,
	CMD_DELETEBREAK,
	CMD_SHOWBREAK,
	CMD_LASTBREAK,
	CMD_LIST,
	CMD_GO,
	CMD_SETARGS,
	CMD_STEP,
	CMD_KILL,
	CMD_GETVAR,
	CMD_SETVAR,
	CMD_GETTYPE,
	CMD_MOVEFRAME,
	CMD_SHOWFRAME,
	CMD_GETLOCATION,
	CMD_DBGSTATUS,
	CMD_GETEVENT,
	CMD_INTERRUPT,
	CMD_QUIT
};

enum cmdstat
{
	CMD_OK = DBG_OK,
	ERR_DEBUGGER = DBGERR_DEBUGGER,		/* error talking to debugger */
	ERR_SERVER = DBGERR_SERVER,		/* no server for this process */
	ERR_CALLBACK = DBGERR_CALLBACK,		/* must register callback first */
	ERR_RPC = DBGERR_RPC,		/* RPC error */
	ERR_NOLINE = DBGERR_NOLINE,		/* no such line */
	ERR_NOFUNC = DBGERR_NOFUNC,		/* no such function */
	ERR_NOFILE = DBGERR_NOFILE,		/* no such source file */
	ERR_NOBP = DBGERR_NOBP,		/* no such breakpoint */
	ERR_NOSYM = DBGERR_NOSYM,		/* no such symbol */
	ERR_NOMEM = DBGERR_NOMEM,		/* can't access memory */
	ERR_CANTRUN = DBGERR_CANTRUN,	 	/* can't run program */
	ERR_INVOKE = DBGERR_INVOKE,		/* could not invoke program */
	ERR_ISRUNNING = DBGERR_ISRUNNING,		/* program is running */
	ERR_NOTRUN = DBGERR_NOTRUN,		/* program is not being run */
	ERR_FIRSTFRAME = DBGERR_FIRSTFRAME,		/* at top frame */
	ERR_LASTFRAME = DBGERR_LASTFRAME,		/* at bottom frame */
	ERR_BADBPARG = DBGERR_BADBPARG,		/* argument expected */
	ERR_REGEX = DBGERR_REGEX,		/* error in regular expression */
	ERR_NOSTACK = DBGERR_NOSTACK,		/* no stack */
	ERR_OUTOFRANGE = DBGERR_OUTOFRANGE,		/* line out of range */
	ERR_NOFILEDIR = DBGERR_NOFILEDIR,		/* no such file or directory */
	ERR_NOSYMS = DBGERR_NOSYMS,		/* no debugging symbols */
	ERR_TEMP = DBGERR_TEMP,		/* could not create temp file */
	ERR_PIPE = DBGERR_PIPE,		/* could not create pipe */
	ERR_FORK = DBGERR_FORK,		/* could not fork */
	ERR_SELECT = DBGERR_SELECT,		/* select failed */
	ERR_NOTEXEC = DBGERR_NOTEXEC,		/* not in executable format */
	ERR_CHDIR = DBGERR_CHDIR,		/* can't chdir */
	ERR_SOURCE = DBGERR_SOURCE,		/* source file is more recent than exec */
	ERR_SETVAR = DBGERR_SETVAR,		/* can't set variable */
	ERR_UNKNOWN = DBGERR_UNKNOWN		/* unknown GDB error */
};

enum status
{
	STAT_INITIALISING = DBGSTAT_INITIALISING,	/* initialising the debugger */
	STAT_WAITING = DBGSTAT_WAITING,		/* debugger started, waiting for command */
	STAT_RUNNING = DBGSTAT_RUNNING,		/* program running */
	STAT_STOPPED = DBGSTAT_STOPPED,		/* program stopped, waiting for command */
	STAT_DBGERR = DBGSTAT_DBGERR,		/* debugger error */
	STAT_INTERR = DBGSTAT_INTERR		/* internal error */
};

enum res_kind
{
	RES_OK = DBGEV_OK,
	RES_BPHIT = DBGEV_BPHIT,
	RES_SIGNAL = DBGEV_SIGNAL,
	RES_EXIT = DBGEV_EXIT,
	RES_STEP = DBGEV_STEP,
	RES_BPINFO = DBGEV_BPINFO,
	RES_BPSET = DBGEV_BPSET,
	RES_FRAME = DBGEV_FRAME,
	RES_LOC = DBGEV_LOC,
	RES_TYPE = DBGEV_TYPE,
	RES_LIST = DBGEV_LIST,
	RES_DATA = DBGEV_DATA,
	RES_STAT = DBGEV_STAT,
	RES_VERS = DBGEV_VERS,
	RES_NONE = DBGEV_NONE,
	RES_ERROR = DBGEV_ERROR
};

/*
** SPECIFY ARGUMENT TYPES
*/
struct strint
{
	string	s1<>;
	int	i1;
};

struct str2
{
	string	s1<>;
	string	s2<>;
};

struct str3
{
	string	s1<>;
	string	s2<>;
	string	s3<>;
};

struct int2
{
	int	i1;
	int	i2;
};

struct int2bool
{
	int	i1;
	int	i2;
	bool	b1;
};

/*
** SPECIFY RESULT TYPES
*/
struct location
{
	string	file<>;
	string	func<>;
	string 	addr<>;
	int	line;
};

struct bpinfo
{
	int		id;
	bool		ignore;
	string		type<>;
	location	loc;
	int		hits;
	string		line<>;
	bpinfo *	next;
};

struct sclist
{
	int		lno;
	string		line<>;
	sclist *	next;
};

struct frameentry
{
	int		level;
	location	loc;
	frameentry *	next;
};

struct sigevinfo
{
	string		sigevinfo_sig<>;
	string		sigevinfo_line<>;
	frameentry *	sigevinfo_frame;
};

struct eventerr
{
	string		eventerr_errstr<>;
	cmdstat		eventerr_err;
};

struct dbgvers
{
	string		dbgvers_version<>;
	string		dbgvers_arch<>;
};

struct stepinfo
{
	int		step_lno;
	string		step_line<>;
	frameentry *	step_frame;
};

union dbg_res
switch ( res_kind status )
{
case RES_BPHIT:
	bpinfo *	res_u_bphit;

case RES_BPINFO:
	bpinfo *	res_u_bpinfo;

case RES_BPSET:
	bpinfo *	res_u_bpset;

case RES_SIGNAL:
	sigevinfo	res_u_sig;

case RES_EXIT:
	int		res_u_exit;

case RES_STEP:
	stepinfo	res_u_step;

case RES_DATA:
	AIF *		res_u_data;

case RES_TYPE:
	string		res_u_type<>;

case RES_LIST:
	sclist *	res_u_list;

case RES_FRAME:
	frameentry *	res_u_frame;

case RES_LOC:
	location *	res_u_loc;

case RES_STAT:
	status		res_u_status;

case RES_ERROR:
	eventerr	res_u_err;

case RES_VERS:
	dbgvers		res_u_vers;

default:
	void;
};
%#define res_bphit		dbg_res_u.res_u_bphit
%#define res_bpset		dbg_res_u.res_u_bpset
%#define res_bpinfo		dbg_res_u.res_u_bpinfo
%#define res_steplno		dbg_res_u.res_u_step.step_lno
%#define res_stepline		dbg_res_u.res_u_step.step_line
%#define res_stepframe		dbg_res_u.res_u_step.step_frame
%#define res_exit		dbg_res_u.res_u_exit
%#define res_sig		dbg_res_u.res_u_sig.sigevinfo_sig
%#define res_sigline		dbg_res_u.res_u_sig.sigevinfo_line
%#define res_sigframe		dbg_res_u.res_u_sig.sigevinfo_frame
%#define res_frame		dbg_res_u.res_u_frame
%#define res_data		dbg_res_u.res_u_data
%#define res_type		dbg_res_u.res_u_type
%#define res_list		dbg_res_u.res_u_list
%#define res_loc		dbg_res_u.res_u_loc
%#define res_status		dbg_res_u.res_u_status
%#define res_err		dbg_res_u.res_u_err.eventerr_err
%#define res_errstr		dbg_res_u.res_u_err.eventerr_errstr
%#define res_version		dbg_res_u.res_u_vers.dbgvers_version
%#define res_arch		dbg_res_u.res_u_vers.dbgvers_arch

struct dbgcb_args
{
	string	cb_host<>;
	int	cb_prog;
};

struct dbgstep_args
{
	int		step_count;
	int		step_in;
	dbgcb_args	step_cb;
};
%#define step_host	step_cb.cb_host
%#define step_prog	step_cb.cb_prog

struct dbgsetvar_args
{
	string	var<>;
	AIF *	data;
};

program DBGSRV
{
	version DBGVERSION
	{
		dbg_res dbg_setfuncbreak(str2) = 1;
		dbg_res dbg_setlinebreak(strint) = 2;
		dbg_res dbg_deletebreak(int) = 3;
		dbg_res dbg_lastbreak(void) = 5;
		dbg_res dbg_list(string) = 6;
		dbg_res dbg_go(dbgcb_args) = 7;
		dbg_res dbg_setargs(string) = 9;
		dbg_res dbg_step(dbgstep_args) = 10;
		dbg_res dbg_kill(void) = 11;
		dbg_res dbg_moveframe(int2) = 12;
		dbg_res dbg_showframe(void) = 13;
		dbg_res dbg_evalexpr(string) = 14;
		dbg_res dbg_setvar(dbgsetvar_args) = 15;
		dbg_res dbg_gettype(string) = 16;
		dbg_res dbg_getlocation(void) = 17;
		dbg_res dbg_dbgstatus(void) = 20;
#ifdef notdef
		dbg_res dbg_getevent(void) = 21;
#endif /*  notdef */
		dbg_res dbg_quit(void) = 22;
		dbg_res dbg_interrupt(dbgcb_args) = 23;
	} = 2;
} = 8888888;

#ifdef RPC_HDR
%#endif /*  _DBGPROTO_H */
#endif /* RPC_HDR */
