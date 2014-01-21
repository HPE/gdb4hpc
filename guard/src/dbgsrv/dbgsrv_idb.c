/*********************************************************************************\
 * IDB interface routines
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
#include	<config.h>
#endif /* HAVE_CONFIG_H */

#ifdef IDB
#include	<sys/types.h>
#include	<sys/stat.h>
#include	<sys/param.h>
#include	<sys/wait.h>

#include	<assert.h>
#include	<stdio.h>
#include	<signal.h>
#include	<stdarg.h>
#include	<string.h>
#include	<fcntl.h>
#include	<errno.h>
#include	<stdlib.h>
#include	<unistd.h>

#include 	"defs.h"
#include 	"useful.h"
#include	"aif.h"
#include 	"dbgevent.h"
#include 	"dbgfn.h"
#include 	"dbgsrv_proto.h"
#include 	"mparch.h"
#include	"dbgrexp.h"

#define DBGPROG	"dbgsrv"

#ifdef WINDOWSNT
#define PIPE_BUF	1024
#endif /* WINDOWSNT */

#define READ_TIMEOUT	1
#define MAX_TRIES	10

#define IDBSYNC() \
	{ \
		dbgevent_t *e; \
		\
		if ( (e = IDBSync()) != (dbgevent_t *)NULL ) \
			return e; \
	}

#define IDBGETPID(pid, e) \
	{ \
		dbgevent_t *et; \
		\
		if ( (et = IDBGetPid(&(pid))) != (dbgevent_t *)NULL ) \
		{ \
			FreeEvent((e)); \
			return et; \
		} \
	}

static dbgstat		IDBStatus;
static int		IDBPid = 0;
static int		IDBDBGIn;
static int		IDBDBGOut;
static int		MPArch;
static int		IDBSynced = 0;
static char *		IDBInit = NULL;
static char *		IDBRunArgs = NULL;
static char *		IDBRedirStr = NULL;
static char *		IDBCurrentDataType = NULL;
static int		IDBCurrLine = 0;
static dbgbp_t *	IDBCurrBP = (dbgbp_t *)NULL;
static list_t *		IDBBP;
static char *		IDBProgMain;
#ifdef sun
static char *		IODevName = NULL;
#endif /* sun */

extern int	Shutdown;

AIF *		BuildAIFVar(char *, char *);
dbgevent_t *	IDBCheckBreak(void);
dbgevent_t *	IDBShowBreak(int);
dbgevent_t *	IDBWaitEvent(dbgevent_t *);
int 		IDBSetCurrentLine(int);
void		IDBFinish(int);
dbgevent_t *	IDBInitDbg(char *);
#ifdef P4
dbgevent_t *	IDBInitP4Dbg(char *, int, char *, char *, char *, char **);
dbgevent_t *	IDBInitP4DbgAttach(int, char *, char *, char **);
#endif /* P4 */
dbgevent_t *	IDBGetPid(int *);
char *		IDBSetupDbg(int, char *);
void		IDBSendCmd(char *, ...);
char *		IDBReadLine(void);
char *		IDBFreeBuffer(void);
int		IDBFillBuffer(int);
int		IDBBufferReady(void);
char *		IDBReadData(int);
int		IDBWriteBuf(char *, int);
dbgevent_t *	IDBSync(void);
void		InitRegexIDB(void);

int		IDBDbgReadyForCmd(void);
int		IDBDbgOK(void);
int		IDBDbgStopped(void);

char *		ConvBase(char *);
int		BaseSize(char *);
int		FromHex(char);
char * 		findchar(char *, int, char);

dbgevent_t *	InitIDB(int *dfd, int *iofd, char **env);
#ifdef sun
int				IDBProgIOHook(int fd);
#endif /* sun */
dbgevent_t *	DbgIDBSetLineBreak(char *, int);
dbgevent_t *	DbgIDBSetFuncBreak(char *, char *);
dbgevent_t *	DbgIDBDeleteBreak(int);
dbgevent_t *	DbgIDBLastBreak(void);
dbgevent_t *	DbgIDBList(char *);
dbgevent_t *	DbgIDBGo(char *, int);
dbgevent_t *	DbgIDBSetArgs(char *);
dbgevent_t *	DbgIDBStep(int, int, char *, int);
dbgevent_t *	DbgIDBKill(void);
dbgevent_t *	DbgIDBMoveFrame(int, int);
dbgevent_t *	DbgIDBShowFrame(void);
dbgevent_t *	DbgIDBEvalExpr(char *);
dbgevent_t *	DbgIDBSetVar(char *, AIF *);
dbgevent_t *	DbgIDBGetType(char *);
dbgevent_t *	DbgIDBGetLocation(void);
dbgevent_t *	DbgIDBGetVars(char *);
dbgevent_t *	DbgIDBStatus(void);
dbgevent_t *	DbgIDBQuit(void);
dbgevent_t *	DbgIDBInterrupt(char *, int);
dbgevent_t *	DbgIDBInvoke(char *, int, char *, char *, char *, char **, int);
dbgevent_t *	DbgIDBAttach(int, char *, char *, char **, int);
void		DbgIDBIDBFinish(void);

dbgbackend_sw	DbgBackendIDB =
{
	InitIDB,
	IDBFillBuffer,
	IDBBufferReady,
#ifdef sun
	IDBProgIOHook,
#else /* sun */
	NULL,
#endif /* sun */
	DbgIDBSetLineBreak,
	DbgIDBSetFuncBreak,
	DbgIDBDeleteBreak,
	DbgIDBLastBreak,
	DbgIDBList,
	DbgIDBGo,
	DbgIDBSetArgs,
	DbgIDBStep,
	DbgIDBKill,
	DbgIDBMoveFrame,
	DbgIDBShowFrame,
	DbgIDBSetVar,
	DbgIDBEvalExpr,
	DbgIDBGetType,
	DbgIDBGetLocation,
	DbgIDBGetVars,
	DbgIDBStatus,
	DbgIDBQuit,
	DbgIDBInterrupt,
	DbgIDBInvoke,
	DbgIDBAttach,
	DbgIDBIDBFinish,
	IDBDbgReadyForCmd,
	IDBDbgOK,
	IDBDbgStopped,
	NULL,
	NULL,
	NULL
};

/*
** Invoked if there is input from the debugger after
** a non-terminating command (eg. run, step, etc.).
** Checks to see what caused the termination.
*/
dbgevent_t *
IDBWaitEvent(dbgevent_t *e)
{
	int		cnt;
	char *		line;
	char *		type;
	char *		data;
	dbgbp_t *	bp=NULL;
	dbglist_t *	l;
	dbglist_t **	lp;
	dbgvars_t *	v;
	dbgvars_t **	vp;
	int		newvers = 0;
	dbgloc_t *	lc;
	dbgrexp_t *	d;
	dbgframe_t *	f;
	dbgframe_t **	fp;

	if ( e == (dbgevent_t *)NULL )
		e = NewEvent(DBGEV_NONE);

	for ( ;; )
	{
		if ( (line = IDBReadLine()) == NULL )
		{
			e = ChangeEvent(e, DBGEV_ERROR);
			e->ev_errno = DBGERR_DEBUGGER;
			EventErrStr(e, "reading command");
			return e;
		}

		if ( *line == '\0' )
			continue;

		if ( (d = MatchRegexList(line)) == RegexError )
		{
			e = ChangeEvent(e, DBGEV_ERROR);
			e->ev_errno = DBGERR_REGEX;
			EventErrStr(e, d->d_errbuf);
			return e;
		}

		if ( d == (dbgrexp_t *)NULL )
		{
			e = ChangeEvent(e, DBGEV_ERROR);
			e->ev_errno = DBGERR_UNKNOWN;
			EventErrStr(e, line);
			return e;
		}

		DPRINT("*** Matched %s\n", GetRegexEvent(d->d_event));

		switch ( d->d_event )
		{
		case IDBREX_BPHIT:
			if ( IDBStatus == DBGSTAT_INITIALISING )
			{
				bp = NewBP(MatchToInt(d, 1, 10));

				e->ev_event = DBGEV_BPHIT;
				e->ev_bp = bp;
				break;
			}

			if ( (bp = FindBPByID(IDBBP, MatchToInt(d, 1, 10))) == (dbgbp_t *)NULL )
			{
				e = ChangeEvent(e, DBGEV_ERROR);
				e->ev_errno = DBGERR_DEBUGGER;
				EventErrStr(e, "bad breakpoint");
				return e;
			}

			assert(bp!=NULL);
			bp->bp_hits++;
			IDBCurrBP = bp;

			e->ev_event = DBGEV_BPHIT;
			e->ev_bp = DupBP(bp);

			IDBStatus = DBGSTAT_STOPPED;

			/*
			** Wait for line info.
			*/
			break;

		case IDBREX_SIGNAL:
			/* assert(e->ev_event == DBGEV_NONE) */
			e->ev_event = DBGEV_SIGNAL;
			e->ev_sig_type = MatchToStr(d, 1);
			e->ev_sig_line = NULL;
			e->ev_sig_frame = (dbgframe_t *)NULL;

			IDBStatus = DBGSTAT_STOPPED;

			/*
			** Get frame and line info if available.
			*/
			break;

		case IDBREX_EXIT:
			/* assert(e->ev_event == DBGEV_NONE) */
			e->ev_event = DBGEV_EXIT;
			e->ev_exit = MatchToInt(d, 1, 8);
			IDBStatus = DBGSTAT_WAITING;
			IDBPid = 0;
			break;

		case IDBREX_EXIT2:
			/* assert(e->ev_event == DBGEV_NONE) */
			e->ev_event = DBGEV_EXIT;
			e->ev_exit = 0;
			IDBStatus = DBGSTAT_WAITING;
			IDBPid = 0;
			break;

		case IDBREX_LINE:
			switch ( e->ev_event )
			{
			case DBGEV_BPHIT:
				if ( e->ev_bp->bp_stmt != NULL )
					Free(e->ev_bp->bp_stmt);

				e->ev_bp->bp_stmt = MatchToStr(d, 2);

				IDBSetCurrentLine(MatchToInt(d, 1, 10));
				break;

			case DBGEV_SIGNAL:
				e->ev_sig_line = MatchToStr(d, 2);
				IDBSetCurrentLine(MatchToInt(d, 1, 10));
				break;

			case DBGEV_FRAME:
				break;

			case DBGEV_LIST:
				l = (dbglist_t *)Alloc(sizeof(dbglist_t));
				l->list_lno = MatchToInt(d, 1, 10);
				l->list_line = MatchToStr(d, 2);
				l->list_next = (dbglist_t *)NULL;

				/*
				** Find end of list.
				*/
				for ( lp = &e->ev_list ; *lp != NULL ; lp = &(*lp)->list_next);
				*lp = l;
				break;

			case DBGEV_NONE: /* step command */
			case DBGEV_STEP:
				if ( e->ev_event == DBGEV_NONE )
				{
					e->ev_event = DBGEV_STEP;
					e->ev_step_frame = (dbgframe_t *)NULL;
				}

				e->ev_step_lno = MatchToInt(d, 1, 10);
				e->ev_step_line = MatchToStr(d, 2);
				IDBStatus = DBGSTAT_STOPPED;
				break;

			default:
				/* do nothing */
				break;
			}
			break;

		case IDBREX_HDR:
			/* assert(e->ev_event == DBGEV_NONE) */
			e->ev_event = DBGEV_BPINFO;

			/*
			** Get breakpoint info
			*/
			break;

		case IDBREX_BPDUP:
			/* assert(e->ev_event == DBGEV_NONE) */
			e->ev_event = DBGEV_BPINFO;

			/*
			** Get breakpoint set line.
			*/
			break;

		case IDBREX_BPSET:
		case IDBREX_BPSET2:
		case IDBREX_BPINFO:
		case IDBREX_BPINFO2:
			bp = NewBP(MatchToInt(d, 1, 10));

			if ( d->d_event == IDBREX_BPINFO || d->d_event == IDBREX_BPINFO2 )
			{
				e->ev_event = DBGEV_BPINFO;
				bp->bp_type = MatchToStr(d, 2);
				bp->bp_loc.loc_addr = MatchToStr(d, 5);
				bp->bp_loc.loc_func = MatchToStr(d, 6);

				if ( d->d_event == IDBREX_BPINFO )
				{
					bp->bp_loc.loc_file = MatchToStr(d, 7);
					bp->bp_loc.loc_line = MatchToInt(d, 8, 10);
				}
				else
					bp->bp_loc.loc_line = -1;

				e->ev_bp = bp;
			}
			else
			{
				e->ev_event = DBGEV_BPSET;
				bp->bp_loc.loc_addr = MatchToStr(d, 2);

				if ( d->d_event == IDBREX_BPSET )
				{
					bp->bp_loc.loc_file = MatchToStr(d, 3);
					bp->bp_loc.loc_line = MatchToInt(d, 4, 10);
				}
				else /* IDBREX_BPSET2 */
					bp->bp_loc.loc_line = -1;

				e->ev_bp = bp;
			}

			break;

		case IDBREX_STEP:
			e->ev_event = DBGEV_STEP;
			e->ev_step_frame = (dbgframe_t *)NULL;
			e->ev_step_lno = -1;
			e->ev_step_line = MatchToStr(d, 1);
			break;

		case IDBREX_STEP2:
			IDBStatus = DBGSTAT_STOPPED;
			break;
		
		case IDBREX_BPHITCOUNT:
			/* assert(e->ev_event == DBGEV_BPINFO */

			bp->bp_hits = MatchToInt(d, 1, 10);
			break;

		case IDBREX_SIGFRAME:
		case IDBREX_FRAME:
		case IDBREX_L0FRAME:
		case IDBREX_SFRAME:
		case IDBREX_IFRAME:
		case IDBREX_MINFRAME: /* received on attach */
			/*
			** XXX
			** Need to check for MINFRAME on attach here
			** and ignore.
			*/

			f = (dbgframe_t *)Alloc(sizeof(dbgframe_t));
			f->frame_next = (dbgframe_t *)NULL;

			switch ( d->d_event )
			{
			case IDBREX_SIGFRAME:
				f->frame_level = -1;
				f->frame_loc.loc_addr = MatchToStr(d, 1);
				f->frame_loc.loc_func = MatchToStr(d, 2);
				f->frame_loc.loc_file = MatchToStr(d, 3);
				f->frame_loc.loc_line = MatchToInt(d, 4, 10);
				break;

			case IDBREX_FRAME:
				f->frame_level = MatchToInt(d, 1, 10);
				f->frame_loc.loc_addr = MatchToStr(d, 2);
				f->frame_loc.loc_func = MatchToStr(d, 3);
				f->frame_loc.loc_file = MatchToStr(d, 4);
				f->frame_loc.loc_line = MatchToInt(d, 5, 10);
				break;

			case IDBREX_L0FRAME:
				f->frame_level = 0;
				f->frame_loc.loc_addr = NULL;
				f->frame_loc.loc_func = MatchToStr(d, 1);
				f->frame_loc.loc_file = MatchToStr(d, 2);
				f->frame_loc.loc_line = MatchToInt(d, 3, 10);
				break;

			case IDBREX_SFRAME:
				f->frame_level = -1;
				f->frame_loc.loc_addr = NULL;
				f->frame_loc.loc_func = MatchToStr(d, 1);
				f->frame_loc.loc_file = MatchToStr(d, 2);
				f->frame_loc.loc_line = MatchToInt(d, 3, 10);
				break;

			case IDBREX_IFRAME:
				f->frame_level = MatchToInt(d, 1, 10);
				f->frame_loc.loc_addr = MatchToStr(d, 2);
				f->frame_loc.loc_func = MatchToStr(d, 3);
				f->frame_loc.loc_line = -1;
				f->frame_loc.loc_file = NULL;
				break;

			case IDBREX_MINFRAME:
				f->frame_level = -1;
				f->frame_loc.loc_addr = MatchToStr(d, 1);
				f->frame_loc.loc_func = MatchToStr(d, 2);
				f->frame_loc.loc_line = -1;
				f->frame_loc.loc_file = NULL;
				break;

			default:
				/* do nothing */;
				break;
			}

			/*
			** If we are generating frame as a result of
			** a signal, a step, or the up/down commands, 
			** then expect a line next.
			*/

			if ( d->d_event == IDBREX_SFRAME )
			{
				e->ev_event = DBGEV_STEP;
				e->ev_step_frame = f;
				break;
			}

			switch ( e->ev_event )
			{
			case DBGEV_NONE:
				e->ev_event = DBGEV_FRAME;
				e->ev_frame = f;
				break;

			case DBGEV_SIGNAL:
				e->ev_sig_frame = f;
				break;

			default:
				/*
				** If we are called as a result of the where
				** command, then we will have a list of frames.
				*/

				for ( fp = &e->ev_frame ; *fp != NULL ; fp = &(*fp)->frame_next);
				*fp = f;
				break;
			}

			break;

		case IDBREX_LOC:
		case IDBREX_LOCNOCODE:
			/* assert(e->ev_event == DBGEV_NONE) */
			lc = (dbgloc_t *)Alloc(sizeof(dbgloc_t));
			lc->loc_line = MatchToInt(d, 1, 10);
			lc->loc_file = MatchToStr(d, 2);
			lc->loc_addr = MatchToStr(d, 3);
			lc->loc_func = NULL;

			e->ev_event = DBGEV_LOC;
			e->ev_loc = lc;
			break;

		case IDBREX_VARS:
			v = (dbgvars_t *)Alloc(sizeof(dbgvars_t));
			v->var_name = MatchToStr(d, 1);
			v->var_next = (dbgvars_t *)NULL;

			/*
			** Find end of list.
			*/
			for ( vp = &e->ev_vars ; *vp != NULL ; vp = &(*vp)->var_next);
			*vp = v;
			break;

		case IDBREX_TYPE:
			/* assert(e->ev_event == DBGEV_NONE) */
			e->ev_event = DBGEV_TYPE;
			e->ev_type = MatchToStr(d, 1);
			break;

		case IDBREX_DATA:
		case IDBREX_DATA2:
			/* assert(e->ev_event == DBGEV_NONE) */
			e->ev_event = DBGEV_DATA;

			if ( d->d_event == IDBREX_DATA2 )
			{
				AIF * aif_var;

				if ( IDBCurrentDataType == NULL )
				{
					e = ChangeEvent(e, DBGEV_ERROR);
					e->ev_errno = DBGERR_DEBUGGER;
					EventErrStr(e, "bad data format");
					return e;
				}

				type = IDBCurrentDataType;
				data = MatchToStr(d, 1);

				aif_var = BuildAIFVar(type, data);

				if ( aif_var == NULL )
				{
					e = ChangeEvent(e, DBGEV_ERROR);
					e->ev_errno = DBGERR_DEBUGGER;
					EventErrStr(e, "bad data format");
					return e;
				}

				e->ev_data = aif_var;
				break;
			}

			type = MatchToStr(d, 1);

			/*
			** Next comes data
			**
			** We don't match data in the regex because it is too 
			** expensive for large data structures.
			*/

			data = line;

			for ( cnt = 0 ; cnt < 3 ; cnt++ )
			{
				if ( (data = strchr(data, ' ')) == NULL )
					break;
				
				data++;
			}

			if ( data == NULL )
			{
				e = ChangeEvent(e, DBGEV_ERROR);
				e->ev_errno = DBGERR_DEBUGGER;
				EventErrStr(e, "bad data format");
				return e;
			}

			e->ev_data = AsciiToAIF(type, data);
			break;

		case IDBREX_INTRO4:
		case IDBREX_INTRO5:
			if ( newvers && d->d_event == IDBREX_INTRO4 )
				break;

			if ( d->d_event == IDBREX_INTRO5 )
				e->ev_arch = MatchToStr(d, 1);

			IDBSynced = 0;
			return e;

		case IDBREX_VERS:
		case IDBREX_VERS2:
		case IDBREX_VERS3:
			e->ev_event = DBGEV_VERS;
			e->ev_version = MatchToStr(d, 1);

			if ( d->d_event == IDBREX_VERS3 )
			{
				newvers = 1;
				break;
			}

			e->ev_arch = MatchToStr(d, 2);

			if ( d->d_event == IDBREX_VERS2 )
			{
				IDBSynced = 0;
				return e;
			}
			break;

		case IDBREX_SYMBOLS:
			/*
			** Expect prompt next.
			*/
			break;

		case IDBREX_LANG:
		case IDBREX_WARN:
		case IDBREX_IGNORE:
			/*
			** Ignore for now.
			*/
			break;

		case IDBREX_PROMPT:
			if ( e->ev_event == DBGEV_NONE )
				e->ev_event = DBGEV_OK;

			IDBSynced = 1;

			if ( IDBStatus == DBGSTAT_RUNNING )
				IDBStatus = DBGSTAT_STOPPED;

			return e;

		case IDBREX_PID:
			e->ev_event = DBGEV_PID;
			e->ev_pid = atoi(MatchToStr(d, 1));
			IDBSynced = 0;
			return e;

		/*******************************
		******* Errors below here ******
		********************************/

		case IDBREX_NOFUNC:
			e->ev_event = DBGEV_ERROR;
			e->ev_errno = DBGERR_NOFUNC;
			EventErrStr(e, MatchToStr(d, 1));
			break;

		case IDBREX_NOLINE:
			e->ev_event = DBGEV_ERROR;
			e->ev_errno = DBGERR_NOLINE;
			EventErrStr(e, MatchToStr(d, 1));
			break;

		case IDBREX_OUTOFRANGE:
			e->ev_event = DBGEV_ERROR;
			e->ev_errno = DBGERR_OUTOFRANGE;
			EventErrStr(e, MatchToStr(d, 1));
			break;

		case IDBREX_NOFILE:
			e->ev_event = DBGEV_ERROR;
			e->ev_errno = DBGERR_NOFILE;
			EventErrStr(e, MatchToStr(d, 1));
			break;

		case IDBREX_NOBP:
			e->ev_event = DBGEV_ERROR;
			e->ev_errno = DBGERR_NOBP;
			EventErrStr(e, MatchToStr(d, 2));
			break;

		case IDBREX_BADBPARG:
			e->ev_event = DBGEV_ERROR;
			e->ev_errno = DBGERR_BADBPARG;
			EventErrStr(e, "");
			break;

		case IDBREX_NOSTACK:
			e->ev_event = DBGEV_ERROR;
			e->ev_errno = DBGERR_NOSTACK;
			EventErrStr(e, "");
			break;

		case IDBREX_NOSYM:
			e->ev_event = DBGEV_ERROR;
			e->ev_errno = DBGERR_NOSYM;
			EventErrStr(e, MatchToStr(d, 1));
			break;

		case IDBREX_NOTRUN:
			e->ev_event = DBGEV_ERROR;
			e->ev_errno = DBGERR_NOTRUN;
			EventErrStr(e, "");
			break;

		case IDBREX_NOMEM:
			e->ev_event = DBGEV_ERROR;
			e->ev_errno = DBGERR_NOMEM;
			EventErrStr(e, "");
			break;

		case IDBREX_FIRSTFRAME:
			e->ev_event = DBGEV_ERROR;
			e->ev_errno = DBGERR_FIRSTFRAME;
			EventErrStr(e, "");
			break;

		case IDBREX_LASTFRAME:
			e->ev_event = DBGEV_ERROR;
			e->ev_errno = DBGERR_LASTFRAME;
			EventErrStr(e, "");
			break;

		case IDBREX_NOSYMS:
			e->ev_event = DBGEV_ERROR;
			e->ev_errno = DBGERR_NOSYMS;
			EventErrStr(e, "");
			/*
			** NOSYMS seems to consume a prompt
			*/
			return e;

		case IDBREX_NOFILEDIR:
			switch ( e->ev_event )
			{
			case DBGEV_SIGNAL:
				/*
				** A signal in a library.
				*/
				e->ev_sig_line = NULL;
				break;

			default:
				e->ev_event = DBGEV_ERROR;
				e->ev_errno = DBGERR_NOFILEDIR;
				EventErrStr(e, MatchToStr(d, 1));
				break;
			}
			break;

		case IDBREX_NOTEXEC:
			e->ev_event = DBGEV_ERROR;
			e->ev_errno = DBGERR_NOTEXEC;
			EventErrStr(e, MatchToStr(d, 1));
			break;

		case IDBREX_SOURCE:
			e->ev_event = DBGEV_ERROR;
			e->ev_errno = DBGERR_SOURCE;
			EventErrStr(e, "");
			break;

		case IDBREX_UNDEF:
			e->ev_event = DBGEV_ERROR;
			e->ev_errno = DBGERR_NOTIMP;
			EventErrStr(e, "");
			break;

		default:
			fprintf(stderr, "IDBWaitEvent: unknown event %d\n", d->d_event);
			return e;
		}
	}

	/* NOT REACHED */
}

/*
** Check that debugger is ready ok
*/
int
IDBDbgOK(void)
{
	return (int)(IDBStatus != DBGSTAT_DBGERR);
}

/*
** Check if debugger is ready to receive a command.
*/
int
IDBDbgReadyForCmd(void)
{
	return (int)(IDBStatus == DBGSTAT_WAITING || IDBStatus == DBGSTAT_STOPPED);
}

/*
** Check if debugger is ready to receive a command.
*/
int
IDBDbgStopped(void)
{
	return (int)(IDBStatus == DBGSTAT_STOPPED);
}

/*
** Set breakpoint at specified line.
*/
dbgevent_t *
DbgIDBSetLineBreak(char *file, int lno)
{
	if ( file == NULL || *file == '\0' )
	{
		DPRINT("*** DbgSetLineBreak(%d)\n", lno);
		IDBSendCmd("break %d", lno);
	}
	else
	{
		DPRINT("*** DbgSetLineBreak(%s, %d)\n", file, lno);
		IDBSendCmd("break %s:%d", file, lno);
	}

	return IDBCheckBreak();
}

/*
** Set breakpoint at start of specified function.
*/
dbgevent_t *
DbgIDBSetFuncBreak(char *file, char *func)
{
	DPRINT("*** DbgSetFuncBreak\n");

	if ( file == NULL || *file == '\0' )
		IDBSendCmd("break %s", func);
	else
		IDBSendCmd("break %s:%s", file, func);

	return IDBCheckBreak();
}

/*
** Check that breakpoint command has succeded and
** extract appropriate information. Returns breakpoint
** id in bid. Adds to breakpoint list if necessary.
*/
dbgevent_t *
IDBCheckBreak(void)
{
	int		id;
	dbgbp_t *	bp;
	dbgevent_t *	e;

	e = IDBWaitEvent((dbgevent_t *)NULL);

	if ( e->ev_event != DBGEV_BPSET )
		return e;

	id = e->ev_bp->bp_id;

	FreeEvent(e);

	/*
	** show gives us more information.
	*/

	e = IDBShowBreak(id);

	if ( e->ev_event != DBGEV_BPINFO )
		return e;

	bp = DupBP(e->ev_bp);

	FreeEvent(e);

	/*
	** Link a copy of the breakpoint onto BP.
	*/
	AddToList(IDBBP, (void *)bp);

	/*
	** Now create a fake event and make it
	** look like a BPSET.
	*/

	e = NewEvent(DBGEV_BPSET);
	e->ev_bp = DupBP(bp);

	return e;
}

/*
** Extract breakpoint information for breakpoint id.
*/
dbgevent_t *
IDBShowBreak(int id)
{
	IDBSendCmd("info break %d", id);

	return IDBWaitEvent((dbgevent_t *)NULL);
}

/*
** Set current line number. Must be called after a breakpoint
** has been hit, or a step command executed.
*/
int
IDBSetCurrentLine(int line)
{
	IDBCurrLine = line;
	return line;
}

/*
** Delete a breakpoint.
*/
dbgevent_t *
DbgIDBDeleteBreak(int bid)
{
/*	char *		line; */
	dbgevent_t *	e;

	DPRINT("*** DbgDeleteBreak\n");

	IDBSendCmd("delete breakpoint %d", bid);

	e = IDBWaitEvent((dbgevent_t *)NULL);

	if 
	(
		e->ev_event == DBGEV_OK 
		&&
		IDBStatus != DBGSTAT_INITIALISING
	)
		RemoveBP(IDBBP, bid);

	return e;
}

/*
** Return id of last breakpoint to be hit.
*/
dbgevent_t *
DbgIDBLastBreak(void)
{
	dbgevent_t *	e;

	if ( IDBCurrBP == (dbgbp_t *)NULL )
	{
		EVENT_ERROR(e, DBGERR_NOBP, "");
		return e;
	}

	e = NewEvent(DBGEV_BPINFO);
	e->ev_bp = IDBCurrBP;

	return e;
}

/*
** List source code.
**
** XXX a better way to do this might be to create a socket
** to the client and just ship the stuff across.
*/
dbgevent_t *
DbgIDBList(char *str)
{
	dbgevent_t *	e;

	if ( str == NULL || *str == '\0' )
		IDBSendCmd("list");
	else
		IDBSendCmd("list %s", str);

	e = NewEvent(DBGEV_LIST);
	e->ev_list = (dbglist_t *)NULL;

	return IDBWaitEvent(e);
}

/*
** Start/continue executing program. If we are starting,
** then we must plant a breakpoint at main() so that we 
** can find out the process id of the program.
*/
dbgevent_t *
DbgIDBGo(char *host, int prog)
{
/*	int		id; */
	dbgevent_t *	e;
	dbgevent_t *	eb = (dbgevent_t *)NULL;

	DPRINT("*** DbgGo()\n");

	/*
	** Check if we are starting...
	*/
	if
	(
		IDBStatus != DBGSTAT_STOPPED
#ifdef notdef
#ifdef AIX_POE
		&&
		MPArch != MPARCH_POE
#endif /* AIX_POE */
#endif /* notdef */
	)
	{
		/*
		** Set a temporary breakpoint at main() if necessary.
		*/
		if ( FindBPByFunc(IDBBP, IDBProgMain) == (dbgbp_t *)NULL )
		{
			eb = DbgIDBSetFuncBreak(NULL, IDBProgMain);

			if ( eb->ev_event != DBGEV_BPSET )
				return eb;
		}

		/*
		** Run the program.
		*/

		if ( IDBRunArgs == NULL )
			IDBRunArgs = strdup("");

		IDBSendCmd("run %s %s", IDBRunArgs, IDBRedirStr);

		/*
		** Wait until we hit the breakpoint at main().
		*/

		e = IDBWaitEvent((dbgevent_t *)NULL);

		if ( e->ev_event != DBGEV_BPHIT )
		{
			FreeEvent(eb);
			return e;
		}

		/*
		** Find out pid.
		*/

		IDBGETPID(IDBPid, e);

		/*
		** Finish if there was already a break at main.
		*/
		if ( eb == (dbgevent_t *)NULL )
		{
			IDBStatus = DBGSTAT_STOPPED;

			/*
			** Force event.
			*/
			AsyncCheck(ASYNC_FORCE, e, host, prog);

			return (dbgevent_t *)NULL;
		}

		FreeEvent(e);

		/*
		** Delete the temporary breakpoint and continue.
		*/

		e = DbgIDBDeleteBreak(eb->ev_bp->bp_id);

		if ( e->ev_event != DBGEV_OK )
			return e;

		FreeEvent(e);
		FreeEvent(eb);
	}

#ifdef notdef
#ifdef AIX_POE
	if ( MPArch == MPARCH_POE )
	{
		/*
		** Find out pid.
		*/

		IDBPid = IDBGetPid();
	}
#endif /* AIX_POE */
#endif /* notdef */
	
	IDBSendCmd("continue");

	IDBStatus = DBGSTAT_RUNNING;

	AsyncCheck(IDBWaitEvent, (dbgevent_t *)NULL, host, prog);

	return (dbgevent_t *)NULL;
}

/*
** Set run arguments.
*/
dbgevent_t *
DbgIDBSetArgs(char *arg)
{
	DPRINT("*** DbgSetArgs\n");

	if ( arg != NULL && *arg != '\0' )
	{
		if ( IDBRunArgs != NULL )
			Free(IDBRunArgs);

		IDBRunArgs = strdup(arg);
	}

	return NewEvent(DBGEV_OK);
}

/*
** Execute count statements. If in == 0, do not enter
** function calls.
*/
dbgevent_t *
DbgIDBStep(int count, int in, char *host, int prog)
{
	char *		cmd = "step";

	DPRINT("*** DbgIDBStep()\n");

	if ( !in )
		cmd = "next";

	if ( count > 1 )
		IDBSendCmd("%s %d", cmd, count);
	else
		IDBSendCmd(cmd);

	AsyncCheck(IDBWaitEvent, (dbgevent_t *)NULL, host, prog);

	IDBStatus = DBGSTAT_RUNNING;

	return (dbgevent_t *)NULL;
}

/*
** Kill the currently running process.
*/
dbgevent_t *
DbgIDBKill(void)
{
	dbgevent_t *	e;

	DPRINT("*** DbgKill\n");

	IDBSendCmd("kill");

	e = IDBWaitEvent((dbgevent_t *)NULL);

	if ( e->ev_event == DBGEV_OK )
	{
		IDBStatus = DBGSTAT_WAITING;
		IDBPid = 0;
	}

	return e;
}

/*
** Move up or down count stack frames.
*/
dbgevent_t *
DbgIDBMoveFrame(int count, int up)
{
	char *		cmd = "up";

	DPRINT("*** DbgMoveFrame\n");

	if ( !up )
		cmd = "down";

	if ( count > 1 )
		IDBSendCmd("%s %d", cmd, count);
	else
		IDBSendCmd(cmd);

	return IDBWaitEvent((dbgevent_t *)NULL);
}

/*
** Show current stack frames.
*/
dbgevent_t *
DbgIDBShowFrame(void)
{
	dbgevent_t *	e = NewEvent(DBGEV_FRAME);

	DPRINT("*** DbgShowFrame\n");

	IDBSendCmd("where");

	e->ev_frame = (dbgframe_t *)NULL;

	return IDBWaitEvent(e);
}

/*
** Evaluate the expression exp.
*/
dbgevent_t *
DbgIDBEvalExpr(char *exp)
{
	dbgevent_t *	e;
	dbgevent_t *	e1;

	DPRINT("*** DbgEvalExpr(%s)\n", exp);

	e = DbgIDBGetType(exp);

	if ( e->ev_event != DBGEV_TYPE )
		return e;

	if ( IDBCurrentDataType != NULL )
		_aif_free(IDBCurrentDataType);

	IDBCurrentDataType = strdup(e->ev_type);

	FreeEvent(e);

	IDBSendCmd("print %s", exp);

	e1 = IDBWaitEvent((dbgevent_t *)NULL);

/*	
  	if ( e1->ev_event != DBGEV_DATA )
		return e1;
*/

	return e1;
}

/*
** Set contents of variable var.
*/
dbgevent_t *
DbgIDBSetVar(char *var, AIF *a)
{
	int		len;
	dbgevent_t *	e;

	e = DbgIDBGetType(var);

	if ( e->ev_event != DBGEV_TYPE )
		return e;

	len = FDSTypeSize(e->ev_type);

	if ( len != AIF_LEN(a) )
	{
		EVENT_ERROR(e, DBGERR_SETVAR, "");
		return e;
	}

	IDBSendCmd("dbg write %s", var);

	if ( IDBWriteBuf(AIF_DATA(a), len) < 0 )
	{
		IDBSYNC();

		EVENT_ERROR(e, DBGERR_DEBUGGER, "writing data");
		return e;
	}

	return IDBWaitEvent((dbgevent_t *)NULL);
}

/*
** Find type of variable.
*/
dbgevent_t *
DbgIDBGetType(char *var)
{
	dbgevent_t *	e;

	DPRINT("*** DbgGetType(%s)\n", var);

	IDBSendCmd("ptype %s", var);

	e = IDBWaitEvent((dbgevent_t *)NULL);

	return e;
}

/*
** Find current dbgloc_t.
*/
dbgevent_t *
DbgIDBGetLocation(void)
{
	DPRINT("*** DbgGetLocation\n");

	IDBSendCmd("info line");

	return IDBWaitEvent((dbgevent_t *)NULL);
}

/*
** List variables.
*/

dbgevent_t *
DbgIDBGetVars(char *file)
{
	dbgevent_t *	e;

	if ( file == NULL || *file == '\0' )
		IDBSendCmd("dbg globals");
	else
		IDBSendCmd("dbg globals %s", file);

	e = NewEvent(DBGEV_VARS);
	e->ev_vars = (dbgvars_t *)NULL;

	return IDBWaitEvent(e);
}

/*
** Get debugger status.
*/
dbgevent_t *
DbgIDBStatus(void)
{
	dbgevent_t *	e = NewEvent(DBGEV_STAT);

	DPRINT("*** DbgIDBStatus\n");

	e->ev_status = IDBStatus;

	return e;
}

/*
** Quit debugger.
*/
dbgevent_t *
DbgIDBQuit(void)
{
	DPRINT("*** DbgQuit\n");

	/*
	** Interrupt the process if it is running.
	*/
	if ( IDBStatus == DBGSTAT_RUNNING )
		(void)kill(IDBPid, SIGINT);

	return (dbgevent_t *)NULL;
}

/*
** Interrupt the debugger.
*/
dbgevent_t *
DbgIDBInterrupt(char *host, int prog)
{
	DPRINT("*** DbgInterrupt\n");

	if ( IDBStatus == DBGSTAT_RUNNING )
	{
		(void)kill(IDBPid, SIGINT);

		AsyncCheck(IDBWaitEvent, (dbgevent_t *)NULL, host, prog);
	}

	return (dbgevent_t *)NULL;
}

/*
** Start debugger.
*/
dbgevent_t *
IDBStartDebugger(char *prog, char *args, char **env, int arch, char *slave)
{
	int		p[2];
#ifndef sun
	int		p1[2];
#endif /* !sun */
	char **		ap;
	dbgevent_t *	e;

	IDBStatus = DBGSTAT_INITIALISING;

	if
	(
		pipe(p) < 0 
#ifndef sun
		||
		pipe(p1) < 0 
#endif /* !sun */
	)
	{
		EVENT_ERROR(e, DBGERR_PIPE, strerror(errno));
		return e;
	}

	signal(SIGCHLD, Reap);
	signal(SIGTERM, IDBFinish);
	signal(SIGHUP, IDBFinish);
	signal(SIGINT, IDBFinish);
	signal(SIGPIPE, IDBFinish);

	if ( (IDBInit = IDBSetupDbg(arch, slave)) == NULL )
	{
		EVENT_ERROR(e, DBGERR_TEMP, "");
		return e;
	}

	(void)setsid();

	switch ( fork() )
	{
	case 0:
		(void)close(0);
		(void)close(1);
		(void)close(2);
#ifndef sun
		dup(p1[0]);
		dup(p[1]);
		dup(p[1]);
		(void)close(p[0]);
		(void)close(p[1]);
		(void)close(p1[0]);
		(void)close(p1[1]);
#else /* !sun */
		dup(p[1]);
		dup(p[1]);
		dup(p[1]);
		(void)close(p[0]);
		(void)close(p[1]);
#endif /* !sun */

		ap = NewArgv
		(
			"idb",
			"-quiet",
			"-interactive",
			"-gdb",
			"-c",
			IDBInit,
			NULL
		);

		Exec(IDB, ap, env);

		exit(1);

	case -1:
		DbgIDBIDBFinish();
		EVENT_ERROR(e, DBGERR_FORK, strerror(errno));
		return e;

	default:
		break;
	}

#ifndef sun
	IDBDBGIn = p[0];
	IDBDBGOut = p1[1];
	(void)close(p[1]);
	(void)close(p1[0]);
#else /* !sun */
	IDBDBGIn = IDBDBGOut = p[0];
	(void)close(p[1]);
#endif /* !sun */

	InitRegexIDB();

	IDBBP = NewList();

	return (dbgevent_t *)NULL;
}


extern int AllocPty(int *, char **, char**);
extern int AllocFifo(int *, char **, char**);

dbgevent_t *
InitIDB(int *dfd, int *iofd, char **env)
{
	char *          master;
	char *          slave;
	dbgevent_t *    e;

	/*
	** Try allocating a pty for backend i/o first. If this fails then
	** we use a fifo. Note that using a fifo restricts us to output only.
	*/
	if      
	(
		AllocPty(iofd, &master, &slave) < 0
		&&
		AllocFifo(iofd, &master, &slave) < 0
	)       
		*iofd = -1;

	/* TODO Check if env is really needed */
	if ( (e = StartDebugger(env, slave)) != (dbgevent_t *)NULL )
		return e;

	*dfd = IDBDBGIn;

#ifdef sun
	IODevName = master;
#endif /* sun */

	return (dbgevent_t *)NULL;;
} 

#ifdef sun
/*
** It appears (in Solaris 2.5) that if the slave
** closes its pty before the master has time to read
** the current data, then we get EIO's until the
** slave opens the pty again. The only way to stop this
** seems to be to close and reopen the master side.
*/
int
IDBProgIOHook(int fd)
{       
	if ( IODevName == NULL )
		return -1;

	(void)close(fd);

	if ( (fd = open(IODevName, O_RDWR, 0600)) < 0 )
	{       
		perror(IODevName);
		return -1;
	}

	return fd;
}
#endif /* sun */

dbgevent_t *
DbgIDBInvoke(char *host, int cb, char *proto, char *prog, char *args, char **env, int arch)
{
	dbgevent_t *	e;

	DPRINT("*** DbgInvoke\n");

	MPArch = arch;

	switch ( arch )
	{
#ifdef P4
	case MPARCH_P4MASTER:
		e = IDBInitP4Dbg(host, cb, proto, prog, args, env);
		break;
#endif /* P4 */

	default:
		e = IDBInitDbg(prog);
	}

//	*dbgfd = IDBDBGIn;
	IDBStatus = DBGSTAT_STOPPED;

	return e;
}

/*
** Attach debugger to running process.
*/
dbgevent_t *
DbgIDBAttach(int pid, char *prog, char *args, char **env, int arch)
{
	dbgevent_t *	e;

	DPRINT("*** DbgAttach\n");

	MPArch = arch;

	switch ( arch )
	{
#ifdef P4
	case MPARCH_P4SLAVE:
		e = IDBInitP4DbgAttach(pid, prog, args, env);
		break;
#endif /* P4 */

	default:
		/*e = IDBInitDbgAttach(pid, prog);*/
		break;
	}

//	*dbgfd = IDBDBGIn;
	IDBStatus = DBGSTAT_STOPPED;

	return e;
}

/*
** Signal handler.
*/
void
IDBFinish(int sig)
{
	Shutdown++;

	if ( IDBInit != NULL )
	{
		if ( unlink(IDBInit) < 0 )
			perror(IDBInit);

		free(IDBInit);
	}

	if ( IDBRedirStr != NULL )
		free(IDBRedirStr);
}

/*
** Called if Shutdown > 0 to finalise debugger.
*/
void
DbgIDBIDBFinish(void)
{
	IDBSendCmd("quit");

#ifdef sun
	if ( IODevName != NULL )
		(void)unlink(IODevName); 
#endif /* sun */

}

void
IDBSendCmd(char *fmt, ...)
{
	int	l;
	int	n;
	char *	s;
	char *	p;
	char	buf[BUFSIZ];
	va_list	ap;

	IDBSynced = 0;

	s = (char *)Alloc(strlen(fmt) + 2);
	strcpy(s, fmt);

	if ( fmt[strlen(fmt)-1] != '\n' )
		strcat(s, "\n");

	va_start(ap, fmt);

	vsnprintf(buf, BUFSIZ-1, s, ap);

	va_end(ap);

	l = strlen(buf);
	p = buf;

	DPRINT(">>>%s", buf);

	while ( l > 0 )
	{
		if ( (n = write(IDBDBGOut, p, l)) < 0 )
		{
			perror("IDBSendCmd");
			Free(s);
			return;
		}

		l -= n;
		p += n;
	}

	Free(s);
}

static char *	_dbg_oldline = NULL;
static char *	_dbg_line = NULL;
static int	_dbg_linelen = 0;
static int	_dbg_ready = 0;

/*
** Fill a buffer with all chars up to next newline.
*/
int
IDBFillBuffer(int fd)
{
/*	int		i; */
	int		n;
	int		len;
	char *		p;
	char		buf[BUFSIZ+1];
	static int	_dbg_buflen = 0;
	static char	_dbg_buf[BUFSIZ+1];

	/*
	** If we already have a line, just return.
	*/
	if ( _dbg_ready )
		return 1;

	/*
	** First check if we've already got some data.
	*/
	if ( _dbg_buflen > 0 )
	{
		/*
		** See if it contains a newline.
		*/
		if ( (p = findchar(_dbg_buf, _dbg_buflen, '\n')) != NULL )
		{
			len = p - _dbg_buf + 1;
			_dbg_ready = 1;
		}
		else
			len = _dbg_buflen;

		/*
		** Add it to the current line, or start a new one.
		*/

		if ( _dbg_line == NULL )
		{
			p = _dbg_line = (char *)Alloc(len + 1);
			_dbg_linelen = len;
		}
		else
		{
			_dbg_line = (char *)Resize((void *)_dbg_line, _dbg_linelen + len + 1);
			p = _dbg_line + _dbg_linelen;
			_dbg_linelen += len;
		}

		memcpy(p, _dbg_buf, len);
		p[len] = '\0';

		/*
		** Fix up saved buffer
		*/
		if ( (_dbg_buflen -= len) > 0 )
			memcpy(_dbg_buf, _dbg_buf + len, _dbg_buflen);

		/*
		** Return if we have a complete line.
		*/
		if ( _dbg_ready )
			return 1;
	}

	/*
	** Next, read some data. We can read a whole BUFSIZ here, 'cause
	** if we got this far _dbg_buf musn't have contained a newline,
	** and so was appended to the current line (or was empty).
	*/
	if ( (n = read(fd, buf, BUFSIZ)) <= 0 )
	{
		if ( n < 0 && errno != EINTR )
			perror("IDBFillBuffer");
		else if ( n == 0 )
			fprintf(stderr, "IDBFillBuffer: EOF encountered\n");

		return 0;
	}

	/*
	** Check if we got a newline.
	*/
	if ( (p = findchar(buf, n, '\n')) != NULL )
	{
		len = p - buf + 1;
		_dbg_ready = 1;
	}
	else
		len = n;

	/*
	** Add it to the current line, or start a new one.
	*/

	if ( _dbg_line == NULL )
	{
		p = _dbg_line = (char *)Alloc(len + 1);
		_dbg_linelen = len;
	}
	else
	{
		_dbg_line = (char *)Resize((void *)_dbg_line, _dbg_linelen + len + 1);
		p = _dbg_line + _dbg_linelen;
		_dbg_linelen += len;
	}

	memcpy(p, buf, len);
	p[len] = '\0';

	DPRINT("IDBFillBuffer: line is \"%s\"\n", _dbg_line);

	/*
	** Save any leftovers.
	*/
	if ( (n -= len) > 0 )
	{
		memcpy(_dbg_buf, buf + len, n);
		_dbg_buflen = n;
	}

	return 1;
}

/*
** Get ready to read a new line and free the old one.
*/
char *
IDBFreeBuffer(void)
{
	if ( _dbg_oldline != NULL )
		Free(_dbg_oldline);

	_dbg_oldline = _dbg_line;

	_dbg_line = NULL;
	_dbg_linelen = 0;
	_dbg_ready = 0;

	return _dbg_oldline;
}

/*
** Test if we have a line ready.
*/
int
IDBBufferReady(void)
{
	return _dbg_ready;
}

/*
** Read a complete line. Automatically frees last line.
*/
char *
IDBReadLine(void)
{
	char *	line;

	/*
	** Read data until we have a complete line.
	*/
	while ( !IDBBufferReady() )
	{
		if ( !IDBFillBuffer(IDBDBGIn) )
		{
			IDBStatus= DBGSTAT_DBGERR;
			return NULL;
		}
	}

	/*
	** Remove newline.
	*/
	_dbg_line[_dbg_linelen - 1] = '\0';

	/*
	** Free up resources
	*/
	line = IDBFreeBuffer();

	DPRINT("IDBReadLine: \"%s\"\n", line);

	return line;
}

/*
** Read size bytes. 
**
** NOTE: We do this by reading newline terminated
** chunks until we have enough data. This implies that the
** data must also be newline terminated.
**
** IDBReadData does not automatically free last data read.
*/
char *
IDBReadData(int size)
{
	int	len = 0;
	char *	data = NULL;

	DPRINT("IDBReadData: reading %d bytes\n", size);

	if ( size == 0 )
		return NULL;

	while ( len < size )
	{
		/*
		** Read data until we have a complete line.
		*/
		while ( !IDBBufferReady() )
		{
			if ( !IDBFillBuffer(IDBDBGIn) )
			{
				if ( data != NULL )
					Free(data);

				IDBStatus= DBGSTAT_DBGERR;
				return NULL;
			}
		}

		if ( data == NULL )
			data = (char *)Alloc(_dbg_linelen + 1);
		else
			data = (char *)Resize((void *)data, len + _dbg_linelen + 1);

		memcpy(data + len, _dbg_line, _dbg_linelen);
		len += _dbg_linelen;

		(void)IDBFreeBuffer();
	}

	DPRINT("IDBReadData: read \"%s\"\n", data);

	return data;
}

/*
** Write data to gdb.
*/
int
IDBWriteBuf(char *data, int len)
{
	int	l;
	int	n;

	DPRINT("IDBWriteBuf: writing %d bytes\n", len);

	do
	{
		l = ( len > PIPE_BUF ) ? PIPE_BUF : len;

		if ( (n = write(IDBDBGOut, data, l)) <= 0 )
		{
			fprintf(stderr, "IDBWriteBuf: short data\n");
			return -1;
		}

		data += n;
		len -= n;
	}
	while ( len > 0 );

	/*
	** Send newline to terminate command.
	*/

	(void)write(IDBDBGOut, "\n", 1);

	return 0;
}

/*
** IDBSync Debugger
*/
dbgevent_t *
IDBSync(void)
{
	int		res;
	char *		line;
	dbgevent_t *	e;

	DPRINT("*** IDBSync()\n");

	if ( IDBSynced )
		return (dbgevent_t *)NULL;

	while ( (line = IDBReadLine()) != NULL )
		if ( (res = MatchRegex(RegexPrompt, line)) != 0 )
			break;

	if ( line == NULL )
	{
		EVENT_ERROR(e, DBGERR_DEBUGGER, "reading command");
		return e;
	}

	if ( res < 0 )
	{
		EVENT_ERROR(e, DBGERR_REGEX, RegexPrompt->d_errbuf);
		return e;
	}

	IDBSynced = 1;

	return (dbgevent_t *)NULL;
}

/*
** Set up .gdbinit file. For POE we need to break at main
** otherwise the MPI initialisation does not happen and
** the code will hang.
*/
char *
IDBSetupDbg(int arch, char *slave)
{
	FILE *	fp;
	char *	file;

	if ( (file = tempnam("/tmp", "idb")) == NULL )
		return NULL;

	if ( (fp = fopen(file, "w")) == (FILE *)NULL )
	{
		free(file);
		return NULL;
	}

	fprintf(fp, "set $prompt=\"(gdb)\\n\"\n");

	if ( slave != NULL && *slave != '\0' )
	{
		if ( *slave == '>' )
			IDBRedirStr = strdup(slave);
	}

	fclose(fp);

	MPArch = arch;

	return file;
}

/*
** Check for correct debugger version.
*/
dbgevent_t *
IDBCheckVers(void)
{
	dbgevent_t *	e;

	e = IDBWaitEvent((dbgevent_t *)NULL);

	if ( e->ev_event != DBGEV_VERS )
	{
		if ( e->ev_event != DBGEV_ERROR )
			EVENT_ERROR(e, DBGERR_DEBUGGER, "Bad response from debugger, expected version.");

		return e;
	}

	fprintf(stderr, "Found debugger version %s\n", e->ev_version);

	fprintf(stderr, "Debugger architecture is %s\n\n", e->ev_arch);

	FreeEvent(e);

	IDBSYNC();

	return (dbgevent_t *)NULL;
}

/*
** Initialise debugger, make sure we are using
** the correct version, etc.
*/
dbgevent_t *
IDBInitDbg(char *prog)
{
	dbgevent_t *	e;

	e = IDBWaitEvent((dbgevent_t *)NULL);

	if ( e->ev_event != DBGEV_OK )
		return e;

	FreeEvent(e);

	IDBSendCmd("file %s", prog);

	e = IDBWaitEvent((dbgevent_t *)NULL);

	if ( e->ev_event != DBGEV_OK )
		return e;

	FreeEvent(e);

	/*
	** Check for fortran.
	*/
	IDBSendCmd("break MAIN__");

	e = IDBWaitEvent((dbgevent_t *)NULL);

	if ( e->ev_event == DBGEV_BPSET )
	{
		/*
		** We are Fortran
		*/
		IDBProgMain = "MAIN__";
	}
	else
	{
		if ( e->ev_event == DBGEV_ERROR )
		{
			if ( e->ev_errno != DBGERR_NOFUNC )
				return e;

			FreeEvent(e);

			IDBSendCmd("break main");

			e = IDBWaitEvent((dbgevent_t *)NULL);

			if ( e->ev_event != DBGEV_BPSET )
			{
				if ( e->ev_event == DBGEV_ERROR )
					return e;

				FreeEvent(e);
				EVENT_ERROR(e, DBGERR_DEBUGGER, "Could not set break at main.");
				return e;
			}

			/*
			** We are C
			*/
			IDBProgMain = "main";
		}
		else
		{
			FreeEvent(e);
			EVENT_ERROR(e, DBGERR_DEBUGGER, "Could not set break at main.");
			return e;
		}
	}
	FreeEvent(e);

	if ( IDBRunArgs == NULL )
		IDBRunArgs = strdup("");

	IDBSendCmd("run %s %s", IDBRunArgs, IDBRedirStr != NULL ? IDBRedirStr : "");

	e = IDBWaitEvent((dbgevent_t *)NULL);

	if ( e->ev_event != DBGEV_BPHIT )
	{
		/*
		** Report real error rather than failure message.
		*/

		/*
		FreeEvent(e);
		EVENT_ERROR(e, DBGERR_DEBUGGER, "Program failed to break at main.");
		*/
		return e;
	}

	FreeEvent(DbgIDBDeleteBreak(e->ev_bp->bp_id));

	/*
	** Find out pid.
	*/

	IDBGETPID(IDBPid, e);

	FreeEvent(e);

	IDBSYNC();

	return NewEvent(DBGEV_OK);
}

#ifdef P4
/*
** Initialise debugger using P4.
*/
dbgevent_t *
IDBInitP4Dbg(char *host, int cb, char *proto, char *prog, char *args, char **env)
{
	int		i;
	int		nprocs;
	char *		line;
	char *		pid;
	char **		ap;
	char **		argp;
	dbgevent_t *	e;
	char		pnum[BUFSIZ];
	char		cbstr[BUFSIZ];
	char		dbgsrv[MAXPATHLEN];

	if ( (e = IDBCheckVers()) != (dbgevent_t *)NULL )
		return e;

	IDBSendCmd("file %s", prog);

	e = IDBWaitEvent((dbgevent_t *)NULL);

	if ( e->ev_event != DBGEV_OK )
		return e;

	FreeEvent(e);

	IDBSendCmd("break p4_debug_master_wait");

	e = IDBWaitEvent((dbgevent_t *)NULL);

	if ( e->ev_event != DBGEV_BPSET )
	{
		FreeEvent(e);
		EVENT_ERROR(e, DBGERR_DEBUGGER, "Program not using P4 debug mode.");
		return e;
	}

	FreeEvent(e);

	if ( IDBRunArgs == NULL )
		IDBRunArgs = strdup("");

	IDBSendCmd("run %s %s", IDBRunArgs, IDBRedirStr != NULL ? IDBRedirStr : "");

	e = IDBWaitEvent((dbgevent_t *)NULL);

	if ( e->ev_event != DBGEV_BPHIT )
	{
		FreeEvent(e);
		EVENT_ERROR(e, DBGERR_DEBUGGER, "Program failed to break at p4_debug_master_wait.");
		return e;
	}

	FreeEvent(DbgIDBDeleteBreak(e->ev_bp->bp_id));

	IDBSendCmd("printf \"%%d\\n\", n_pids");

	if ( (line = IDBReadLine()) == NULL )
	{
		EVENT_ERROR(e, DBGERR_DEBUGGER, "getting number of P4 processes");
		return e;
	}

	IDBSYNC();

	nprocs = atoi(line);

#ifdef WINDOWSNT
	snprintf(dbgsrv, MAXPATHLEN-1, "%s", DBGPROG);
#else /* WINDOWSNT */
	snprintf(dbgsrv, MAXPATHLEN-1, "%s/%s", BINDIR, DBGPROG);
#endif /* WINDOWSNT */

	snprintf(cbstr, BUFSIZ-1, "%d", cb);

	for ( i = 0 ; i < nprocs ; i++ )
	{
		/* int v; */
		IDBSendCmd("printf \"%%d\\n\", pid_list[%d]", i);

		if ( (line = IDBReadLine()) == NULL )
		{
			EVENT_ERROR(e, DBGERR_DEBUGGER, "getting P4 pid");
			return e;
		}

		IDBSYNC();

		pid = strdup(line);

		snprintf(pnum, BUFSIZ-1, "%d", i + 1);

		ap = NewArgv
		(
			"dbgsrv",
			"-h", host,
			"-c", cbstr,
			"-a", "p4slave",
			"-i", line,
			"-n", pnum,
			"-p", proto,
			prog,
			NULL
		);

		argp = Str2Argv(args);

		ap = AppendArgv(ap, argp);

		ForkAndExec(dbgsrv, ap, env);

		FreeArgv(ap);
		FreeArgv(argp);
		Free(pid);
	}

	/*
	** Race condition. May not set breakpoints in slaves before
	** we continue master. Sleep a bit here to make this less
	** likely.
	*/
	sleep(3);

	IDBSendCmd("break p4_debug_master");

	e = IDBWaitEvent((dbgevent_t *)NULL);

	if ( e->ev_event != DBGEV_BPSET )
	{
		FreeEvent(e);
		EVENT_ERROR(e, DBGERR_DEBUGGER, "Program not using P4 debug mode.");
		return e;
	}

	FreeEvent(e);

	IDBSendCmd("cont");

	e = IDBWaitEvent((dbgevent_t *)NULL);

	if ( e->ev_event != DBGEV_BPHIT )
	{
		FreeEvent(e);
		EVENT_ERROR(e, DBGERR_DEBUGGER, "Program failed to break at p4_debug_master.");
		return e;
	}

	FreeEvent(DbgIDBDeleteBreak(e->ev_bp->bp_id));

	/*
	** Find out pid.
	*/

	IDBGETPID(IDBPid, e);

	FreeEvent(e);

	return NewEvent(DBGEV_OK);
}

/*
** Attach debugger using P4.
*/
dbgevent_t *
IDBInitP4DbgAttach(int pid, char *prog, char *args, char **env)
{
/*	char **		ap; */
/*	char **		argp; */
	dbgevent_t *	e;
/*	char		dbgsrv[MAXPATHLEN]; */

	if ( (e = IDBCheckVers()) != (dbgevent_t *)NULL )
		return e;

	IDBSendCmd("file %s", prog);

	e = IDBWaitEvent((dbgevent_t *)NULL);

	if ( e->ev_event != DBGEV_OK )
		return e;

	FreeEvent(e);

	IDBSendCmd("attach %d", pid);

	e = IDBWaitEvent((dbgevent_t *)NULL);

	if ( e->ev_event != DBGEV_OK )
		return e;

	FreeEvent(e);

	IDBSendCmd("break p4_debug_slave");

	e = IDBWaitEvent((dbgevent_t *)NULL);

	if ( e->ev_event != DBGEV_BPSET )
	{
		FreeEvent(e);
		EVENT_ERROR(e, DBGERR_DEBUGGER, "Program not using P4 debug mode.");
		return e;
	}

	FreeEvent(e);

	IDBSendCmd("cont");

	e = IDBWaitEvent((dbgevent_t *)NULL);

	if ( e->ev_event != DBGEV_BPHIT )
	{
		FreeEvent(e);
		EVENT_ERROR(e, DBGERR_DEBUGGER, "Program failed to break at p4_debug_slave.");
		return e;
	}

	FreeEvent(DbgIDBDeleteBreak(e->ev_bp->bp_id));

	/*
	** Find out pid.
	*/

	IDBGETPID(IDBPid, e);

	FreeEvent(e);

	return NewEvent(DBGEV_OK);
}
#endif /* P4 */

dbgevent_t *
IDBGetPid(int *pid)
{
	dbgevent_t *	e;

	IDBSendCmd("info threads"); /* works on RedHat */

	e = IDBWaitEvent((dbgevent_t *)NULL);

	if ( e->ev_event != DBGEV_PID )
	{
		EVENT_ERROR(e, DBGERR_DEBUGGER, "pid command not available");
		return e;
	}

	*pid = e->ev_pid;

	FreeEvent(e);

#ifdef notdef
		IDBSendCmd("info program");

		e = IDBWaitEvent((dbgevent_t *)NULL);

		if ( e->ev_event != DBGEV_PROGRAM )
		{
			FreeEvent(e);
			return -1;
		}

		pid = e->event_pid;

		FreeEvent(e);
#endif /* notdef */

	return (dbgevent_t *)NULL;
}

AIF *
BuildAIFVar(char *type, char *data)
{
	if ( strcmp(type, "int") == 0 )
	{
		return IntToAIF(atoi(data));
	}
	else
	{
		return NULL;
	}
}
#endif /* IDB */
