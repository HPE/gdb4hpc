/*
** GDB interface routines
**
** Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
**
*/

#ifndef _DBGREXP_H
#define _DBGREXP_H

#ifdef WINDOWSNT
#include	"regex.h"
#else /* WINDOWSNT */
#include	<regex.h>
#endif /* WINDOWSNT */

enum regexev
{
	/* GDB Regex */
	GDBREX_PROMPT,		/* GDB prompt */
	GDBREX_BPHIT,		/* Breakpoint hit */
	GDBREX_SIGNAL,		/* Signal received */
	GDBREX_EXIT,		/* Program exited with code */
	GDBREX_EXIT2,		/* Program exited normally */
	GDBREX_SHOW,            /* output from show command */
	GDBREX_LINE,		/* Line of source */
	GDBREX_HDR,		/* Header for info breakpoint */
	GDBREX_BPINFO,		/* Line of breakpoint info */
	GDBREX_BPINFO2,		/* Line of breakpoint info (no source info)*/
	GDBREX_BPHITCOUNT,	/* Breakpoint hit count */
	GDBREX_BPDUP,		/* Duplicate breakpoint message */
	GDBREX_BPSET,		/* Breakpoint has been set */
	GDBREX_BPSET2,		/* Breakpoint has been set (no source info) */
	GDBREX_STEP,		/* Single stepping until exit from function */
	GDBREX_STEP2,		/* ...which has no line number information. */
	GDBREX_NOFUNC,		/* No such function */
	GDBREX_NOLINE,		/* No such line */
	GDBREX_OUTOFRANGE,	/* Out of range */
	GDBREX_NOFILE,		/* No such file */
	GDBREX_NOFILEDIR,	/* No such file or directory */
	GDBREX_NOBP,		/* No such breakpoint */
	GDBREX_BADBPARG,	/* Bad argument to breakpoint cmd */
	GDBREX_SIGFRAME,	/* Frame on signal receipt */
	GDBREX_FRAME,		/* Normal frame */
	GDBREX_L0FRAME,		/* Level 0 frame */
	GDBREX_SFRAME,		/* Enter/exit frame (step) */
	GDBREX_IFRAME,		/* Frame with no symbols */
	GDBREX_FIRSTFRAME,	/* First frame message */
	GDBREX_LASTFRAME,	/* Last frame message */
	GDBREX_MINFRAME,	/* Minimum frame */
	GDBREX_NOSTACK,		/* No stack */
	GDBREX_NOSYM,		/* No symbol */
	GDBREX_NOMEM,		/* No such memory location */
	GDBREX_NOTRUN,		/* Program is not running */
	GDBREX_NOSYMS,		/* No debugging symbols */
	GDBREX_NOTEXEC,		/* Not in executable format */
	GDBREX_LOC,		/* Result of info line */
	GDBREX_LOCNOCODE,	/* Result of info line (alternate) */
	GDBREX_VARS,		/* Result of info globals */
	GDBREX_TYPE,		/* GUARD type command */
	GDBREX_DATA,		/* GUARD eval command */
	GDBREX_DATA2,		/* GUARD eval command for unpatched GDB */
	GDBREX_INTRO1,		/* GDB intro message (line 1) */
	GDBREX_INTRO2,		/* GDB intro message (line 2) */
	GDBREX_INTRO3,		/* GDB intro message (line 3) */
	GDBREX_INTRO4,		/* GDB intro message (line 4) */
	GDBREX_INTRO5,		/* GDB intro message (version >= 4.17) */
	GDBREX_INTRO6,		/* GDB intro message (version >= 4.17) */
	GDBREX_INTRO7,		/* GDB intro message (version >= 4.17) */
	GDBREX_INTRO8,		/* GDB intro message (version >= 4.17) */
	GDBREX_VERS,		/* GDB version */
	GDBREX_VERS2,		/* GDB version (alternate) */
	GDBREX_VERS3,		/* GDB version (version >= 4.17) */
	GDBREX_SYMBOLS,		/* Reading symbols... */
	GDBREX_LANG,		/* GDB language */
	GDBREX_SOURCE,		/* Source file is more recent than executable */
	GDBREX_UNDEF,		/* Undefined command */
	GDBREX_PID,		/* Process ID */
	GDBREX_WARN,		/* warning message */
	GDBREX_IGNORE,		/* spurious message */
	GDBREX_ATTACH,          /* attach message */
	GDBREX_ATTACH_FAILED,   /* attach failed message */

	/* IDB Regex */
	IDBREX_PROMPT,		/* IDB prompt */
	IDBREX_BPHIT,		/* Breakpoint hit */
	IDBREX_SIGNAL,		/* Signal received */
	IDBREX_EXIT,		/* Program exited with code */
	IDBREX_EXIT2,		/* Program exited normally */
	IDBREX_LINE,		/* Line of source */
	IDBREX_HDR,		/* Header for info breakpoint */
	IDBREX_BPINFO,		/* Line of breakpoint info */
	IDBREX_BPINFO2,		/* Line of breakpoint info (no source info)*/
	IDBREX_BPHITCOUNT,	/* Breakpoint hit count */
	IDBREX_BPDUP,		/* Duplicate breakpoint message */
	IDBREX_BPSET,		/* Breakpoint has been set */
	IDBREX_BPSET2,		/* Breakpoint has been set (no source info) */
	IDBREX_STEP,		/* Single stepping until exit from function */
	IDBREX_STEP2,		/* ...which has no line number information. */
	IDBREX_NOFUNC,		/* No such function */
	IDBREX_NOLINE,		/* No such line */
	IDBREX_OUTOFRANGE,	/* Out of range */
	IDBREX_NOFILE,		/* No such file */
	IDBREX_NOFILEDIR,	/* No such file or directory */
	IDBREX_NOBP,		/* No such breakpoint */
	IDBREX_BADBPARG,	/* Bad argument to breakpoint cmd */
	IDBREX_SIGFRAME,	/* Frame on signal receipt */
	IDBREX_FRAME,		/* Normal frame */
	IDBREX_L0FRAME,		/* Level 0 frame */
	IDBREX_SFRAME,		/* Enter/exit frame (step) */
	IDBREX_IFRAME,		/* Frame with no symbols */
	IDBREX_FIRSTFRAME,	/* First frame message */
	IDBREX_LASTFRAME,	/* Last frame message */
	IDBREX_MINFRAME,	/* Minimum frame */
	IDBREX_NOSTACK,		/* No stack */
	IDBREX_NOSYM,		/* No symbol */
	IDBREX_NOMEM,		/* No such memory location */
	IDBREX_NOTRUN,		/* Program is not running */
	IDBREX_NOSYMS,		/* No debugging symbols */
	IDBREX_NOTEXEC,		/* Not in executable format */
	IDBREX_LOC,		/* Result of info line */
	IDBREX_LOCNOCODE,	/* Result of info line (alternate) */
	IDBREX_VARS,		/* Result of info globals */
	IDBREX_TYPE,		/* GUARD type command */
	IDBREX_DATA,		/* GUARD eval command */
	IDBREX_DATA2,		/* GUARD eval command */
//	IDBREX_INTRO1,		/* IDB intro message (line 1) */
//	IDBREX_INTRO2,		/* IDB intro message (line 2) */
//	IDBREX_INTRO3,		/* IDB intro message (line 3) */
	IDBREX_INTRO4,		/* IDB intro message (line 4) */
	IDBREX_INTRO5,		/* IDB intro message (version >= 4.17) */
//	IDBREX_INTRO6,		/* IDB intro message (version >= 4.17) */
//	IDBREX_INTRO7,		/* IDB intro message (version >= 4.17) */
//	IDBREX_INTRO8,		/* IDB intro message (version >= 4.17) */
	IDBREX_VERS,		/* IDB version */
	IDBREX_VERS2,		/* IDB version (alternate) */
	IDBREX_VERS3,		/* IDB version (version >= 4.17) */
	IDBREX_SYMBOLS,		/* Reading symbols... */
	IDBREX_LANG,		/* IDB language */
	IDBREX_SOURCE,		/* Source file is more recent than executable */
	IDBREX_UNDEF,		/* Undefined command */
	IDBREX_PID,		/* Process ID */
	IDBREX_WARN,		/* warning message */
	IDBREX_IGNORE,		/* spurious message */
//	IDBREX_ATTACH,      /* attach message */
//	IDBREX_ATTACH_FAILED,   /* attach failed message */

	/* GDBMI Regex */
	GDBMIREX_PROMPT,		/* GDB prompt */
	GDBMIREX_BPHIT,		/* Breakpoint hit */
	GDBMIREX_SIGNAL,		/* Signal received */
	GDBMIREX_EXIT,		/* Program exited with code */
	GDBMIREX_EXIT2,		/* Program exited normally */
	GDBMIREX_LINE,		/* Line of source */
	GDBMIREX_BPINFO,		/* Line of breakpoint info */
	GDBMIREX_BPINFO2,		/* Line of breakpoint info (no source info)*/
	GDBMIREX_BPHITCOUNT,	/* Breakpoint hit count */
	GDBMIREX_BPDUP,		/* Duplicate breakpoint message */
	GDBMIREX_BPSET,		/* Breakpoint has been set */
	GDBMIREX_STEP,		/* Single stepping until exit from function */
	GDBMIREX_STEP2,		/* ...which has no line number information. */
	GDBMIREX_NOFUNC,		/* No such function */
	GDBMIREX_NOLINE,		/* No such line */
	GDBMIREX_OUTOFRANGE,	/* Out of range */
	GDBMIREX_NOFILE,		/* No such file */
	GDBMIREX_NOFILEDIR,	/* No such file or directory */
	GDBMIREX_NOBP,		/* No such breakpoint */
	GDBMIREX_BADBPARG,	/* Bad argument to breakpoint cmd */
	GDBMIREX_SIGFRAME,	/* Frame on signal receipt */
	GDBMIREX_FRAME,		/* Normal frame */
	GDBMIREX_L0FRAME,		/* Level 0 frame */
	GDBMIREX_SFRAME,		/* Enter/exit frame (step) */
	GDBMIREX_IFRAME,		/* Frame with no symbols */
	GDBMIREX_FIRSTFRAME,	/* First frame message */
	GDBMIREX_LASTFRAME,	/* Last frame message */
	GDBMIREX_MINFRAME,	/* Minimum frame */
	GDBMIREX_NOSTACK,		/* No stack */
	GDBMIREX_NOSYM,		/* No symbol */
	GDBMIREX_NOMEM,		/* No such memory location */
	GDBMIREX_NOTRUN,		/* Program is not running */
	GDBMIREX_NOSYMS,		/* No debugging symbols */
	GDBMIREX_NOTEXEC,		/* Not in executable format */
	GDBMIREX_LOC,		/* Result of info line */
	GDBMIREX_LOCNOCODE,	/* Result of info line (alternate) */
	GDBMIREX_VARS,		/* Result of info globals */
	GDBMIREX_TYPE,		/* GUARD type command */
	GDBMIREX_DATA,		/* GUARD eval command */
	GDBMIREX_DATA2,		/* GUARD eval command for unpatched GDB */
	GDBMIREX_INTRO,		/* GDB intro message (version >= 4.17) */
	GDBMIREX_VERS,		/* GDB version (version >= 4.17) */
	GDBMIREX_SYMBOLS,		/* Reading symbols... */
	GDBMIREX_LANG,		/* GDB language */
	GDBMIREX_SOURCE,		/* Source file is more recent than executable */
	GDBMIREX_UNDEF,		/* Undefined command */
	GDBMIREX_PID,		/* Process ID */
	GDBMIREX_WARN,		/* warning message */
	GDBMIREX_IGNORE,		/* spurious message */
	GDBMIREX_ATTACH,      /* attach message */
	GDBMIREX_ATTACH_FAILED,   /* attach failed message */
	GDBMIREX_DONE,   /* ^done */
	GDBMIREX_RUNNING,   /* ^running */

	REX_NULL
};
typedef enum regexev	regexev_t;

struct dbgrexp
{
	char *		d_pattern;		/* pattern to be matched */
	char *		d_string;		/* matched string */
	int		d_flags;		/* regexec flags */
	int		d_nmatch;		/* number of subexpressions */
	regexev_t	d_event;		/* event trigger */
	int		d_res;			/* result of regexec */
	regex_t *	d_regex;		/* regex structure */
	regmatch_t *	d_match;		/* array of matches */
	char		d_errbuf[BUFSIZ];	/* error string */
};
typedef struct dbgrexp	dbgrexp_t;

extern dbgrexp_t *	RegexError;
extern dbgrexp_t *	RegexPrompt;

extern void		InitRegexGDB(void);
extern void		InitRegexIDB(void);
extern void		InitRegexGDBMI(void);
extern dbgrexp_t *	AddRegex(char *, int, int, regexev_t);
extern dbgrexp_t *	MatchRegexList(char *);
extern void		FreeRegex(dbgrexp_t *);
void			RegexGetError(dbgrexp_t *);
extern int		MatchRegex(dbgrexp_t *, char *);
extern char *		MatchToStr(dbgrexp_t *, int);
extern int		MatchToInt(dbgrexp_t *, int, int);

#ifdef DEBUG
extern char *	GetRegexEvent(regexev_t);
#endif /* DEBUG */

#endif /* !_DBGREXP_H */
