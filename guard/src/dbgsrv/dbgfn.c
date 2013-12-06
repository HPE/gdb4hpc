/*
** Server dispatch routines
**
** Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
**
*/

#ifdef HAVE_CONFIG_H
#include	<config.h>
#endif /* HAVE_CONFIG_H */

#include	<string.h>
#include	<stdlib.h>

#include	"defs.h"
#include	"dbgsrv_defs.h"
#include	"dbgfn.h"

static char *	_backend_err_str = NULL;

/*
** Setup debug backend defs
*/
struct backend_tab
{
	char *			backend_name;
	dbgbackend_sw *	backend_def;
};
typedef struct backend_tab		backend_tab;

// external debug backend defs
#if defined(GDB_BACKEND)
extern dbgbackend_sw	DbgBackendGDB;
#elif defined(GDBMI_BACKEND)
extern dbgbackend_sw	DbgBackendGDBMI;
#elif defined(IDB_BACKEND)
extern dbgbackend_sw	DbgBackendIDB;
#endif

// table of backend defs
static backend_tab DbgBackendTab[] =
{
#if defined(GDB_BACKEND)
	{	"gdb", 		&DbgBackendGDB },
#elif defined(GDBMI_BACKEND)
	{	"gdbmi",	&DbgBackendGDBMI },
#elif defined(IDB_BACKEND)
	{	"idb", 		&DbgBackendIDB },
#endif
	{	NULL,		(dbgbackend_sw *)NULL }
};

/*
** Set default debug backend
** This defines the current debug backend in use
*/
#if defined(GDB_BACKEND)
static dbgbackend_sw * DbgBackendSW = &DbgBackendGDB;		/* Default to GDB */
#elif defined(GDBMI_BACKEND)
static dbgbackend_sw * DbgBackendSW = &DbgBackendGDBMI;	/* Default to GDBMI */
#elif defined(IDB_BACKEND)
static dbgbackend_sw * DbgBackendSW = &DbgBackendIDB;		/* Default to IDB */
#endif

/*
** The following functions are used to startup/init the debug backend and not
** send acutal commands to the backend.
*/

// NOTE: Only one protocol/debug backend can be active. The debug server is
//       designed to talk to only one client, whereas the client is designed to
//       talk to many different debug servers. Hence why the client handles
//       multiple protocols at once, and the server doesn't.

int
DbgSetBackend(char *backend)
{
	backend_tab *	p;

	for ( p = DbgBackendTab ; p->backend_name != NULL ; p++ )
	{
		if ( strcmp(p->backend_name, backend) == 0 )
		{
			// Set the current backend
			DbgBackendSW = p->backend_def;
			return 0;
		}
	}

	return 1;
}

// Used to ensure the function is implemented
#define DBG_IMP(e, f) \
	if ( f == NULL ) \
	{ \
		EVENT_ERROR(e, DBGERR_DEBUGGER, "debugger function not implemented"); \
		return e; \
	}
	
#define DBG_SIMPLE_IMP(f) \
	if ( f == NULL ) \
	{ \
		return -1; \
	}
	
#define DBG_VOID_IMP(f) \
	if ( f == NULL ) \
	{ \
		return; \
	}

dbgevent_t *
DbgInit(int *dfd, int *iofd, char **env)
{
	dbgevent_t *	e;

	ResetWDT();
	
	DBG_IMP(e, DbgBackendSW->dbg_init);

	return DbgBackendSW->dbg_init(dfd, iofd, env);
}

dbgevent_t *
DbgAttach(int pid, char *prog, char *args, char **env, int arch)
{
	dbgevent_t *	e;

	ResetWDT();
	
	DBG_IMP(e, DbgBackendSW->dbg_attach);

	return DbgBackendSW->dbg_attach(pid, prog, args, env, arch);
}

dbgevent_t *
DbgInvoke(char *host, int cb, char *proto, char *prog, char *args, char **env, int arch)
{
	dbgevent_t *	e;

	ResetWDT();
	
	DBG_IMP(e, DbgBackendSW->dbg_invoke);

	return DbgBackendSW->dbg_invoke(host, cb, proto, prog, args, env, arch);
}

void
DbgFinish(void)
{
	ResetWDT();
	
	DBG_VOID_IMP(DbgBackendSW->dbg_finish);
	
	return DbgBackendSW->dbg_finish();
}

int
DbgProgIOHook(int fd)
{
	ResetWDT();
	
	DBG_SIMPLE_IMP(DbgBackendSW->dbg_progio_hook);
	
	return DbgBackendSW->dbg_progio_hook(fd);
}

int
DbgRead(int fd)
{
	ResetWDT();
	
	DBG_SIMPLE_IMP(DbgBackendSW->dbg_read);
	
	return DbgBackendSW->dbg_read(fd);
}

int
DbgBufferReady(void)
{
	ResetWDT();
	
	DBG_SIMPLE_IMP(DbgBackendSW->dbg_bufferready);
	
	return DbgBackendSW->dbg_bufferready();
}

/*
** The following macros are used below - note that the debug backend needs to be
** started and init'ed before calling these.
*/

#define DBG_CHECK(e) \
	if ( !DbgBackendSW->dbg_ok() ) \
	{ \
		EVENT_ERROR(e, DBGERR_DEBUGGER, "debugger not available"); \
		return e; \
	}

#define DBG_READY(e) \
	DBG_CHECK(e); \
	\
	if ( !DbgBackendSW->dbg_readyforcmd() ) \
	{ \
		EVENT_ERROR(e, DBGERR_ISRUNNING, ""); \
		return e; \
	}

#define DBG_STOPPED(e) \
	DBG_READY(e); \
	\
	if ( !DbgBackendSW->dbg_stopped() ) \
	{ \
		EVENT_ERROR(e, DBGERR_ISRUNNING, ""); \
		return e; \
	}

/*
** These functions will send actual commands to the debug backend.
*/

dbgevent_t *
DbgStartup(void)
{
	dbgevent_t *	e;

	ResetWDT();

	DBG_READY(e);

	DBG_IMP(e, DbgBackendSW->dbg_startup);

	return DbgBackendSW->dbg_startup();
}

dbgevent_t *
DbgSetLineBreak(char *file, int line)
{
	dbgevent_t *	e;

	ResetWDT();

	DBG_READY(e);
	
	DBG_IMP(e, DbgBackendSW->dbg_setlinebreak);

	return DbgBackendSW->dbg_setlinebreak(file, line);
}

dbgevent_t *
DbgSetFuncBreak(char *file, char *func)
{
	dbgevent_t *	e;

	ResetWDT();

	DBG_READY(e);
	
	DBG_IMP(e, DbgBackendSW->dbg_setfuncbreak);

	return DbgBackendSW->dbg_setfuncbreak(file, func);
}

dbgevent_t *
DbgSetWatch(int type, char *expr)
{
	dbgevent_t *	e;

	ResetWDT();

	DBG_READY(e);
	
	DBG_IMP(e, DbgBackendSW->dbg_setwatch);

	return DbgBackendSW->dbg_setwatch(type, expr);
}

dbgevent_t *
DbgDeleteBreak(int bp)
{
	dbgevent_t *	e;

	ResetWDT();

	DBG_READY(e);
	
	DBG_IMP(e, DbgBackendSW->dbg_deletebreak);

	return DbgBackendSW->dbg_deletebreak(bp);
}

dbgevent_t *
DbgDisableBreak(int bp)
{
	dbgevent_t *	e;

	ResetWDT();

	DBG_READY(e);
	
	DBG_IMP(e, DbgBackendSW->dbg_disablebreak);

	return DbgBackendSW->dbg_disablebreak(bp);
}

dbgevent_t *
DbgEnableBreak(int bp, int once, int del)
{
	dbgevent_t *	e;

	ResetWDT();

	DBG_READY(e);
	
	DBG_IMP(e, DbgBackendSW->dbg_enablebreak);

	return DbgBackendSW->dbg_enablebreak(bp, once, del);
}

dbgevent_t *
DbgLastBreak(void)
{
	dbgevent_t *	e;

	ResetWDT();

	DBG_READY(e);
	
	DBG_IMP(e, DbgBackendSW->dbg_lastbreak);

	return DbgBackendSW->dbg_lastbreak();
}

dbgevent_t *
DbgList(char *str)
{
	dbgevent_t *	e;

	ResetWDT();

	DBG_READY(e);
	
	DBG_IMP(e, DbgBackendSW->dbg_list);

	return DbgBackendSW->dbg_list(str);
}

dbgevent_t *
DbgGetSources(void)
{
	dbgevent_t *	e;
	
	ResetWDT();
	
	DBG_READY(e);
	
	DBG_IMP(e, DbgBackendSW->dbg_getsources);
	
	return DbgBackendSW->dbg_getsources();
}

dbgevent_t *
DbgGo(void)
{
	dbgevent_t *	e;

	ResetWDT();

	DBG_READY(e);
	
	DBG_IMP(e, DbgBackendSW->dbg_go);

	return DbgBackendSW->dbg_go();
}

dbgevent_t *
DbgFinishFunc(void)
{
	dbgevent_t *	e;

	ResetWDT();

	DBG_READY(e);
	
	DBG_IMP(e, DbgBackendSW->dbg_finishfunc);

	return DbgBackendSW->dbg_finishfunc();
}

dbgevent_t *
DbgSetArgs(char *args)
{
	dbgevent_t *	e;

	ResetWDT();

	DBG_READY(e);
	
	DBG_IMP(e, DbgBackendSW->dbg_setargs);

	return DbgBackendSW->dbg_setargs(args);
}

dbgevent_t *
DbgStep(int count, int in)
{
	dbgevent_t *	e;

	ResetWDT();

	DBG_STOPPED(e);
	
	DBG_IMP(e, DbgBackendSW->dbg_step);

	return DbgBackendSW->dbg_step(count, in);
}

dbgevent_t *
DbgKill(void)
{
	dbgevent_t *	e;

	ResetWDT();

	DBG_READY(e);
	
	DBG_IMP(e, DbgBackendSW->dbg_kill);

	return DbgBackendSW->dbg_kill();
}

dbgevent_t *
DbgMoveFrame(int count, int up)
{
	dbgevent_t *	e;

	ResetWDT();

	DBG_READY(e);
	
	DBG_IMP(e, DbgBackendSW->dbg_moveframe);

	return DbgBackendSW->dbg_moveframe(count, up);
}

// If count is 0, then an unlimited number of frames should be displayed.
dbgevent_t *
DbgShowFrame(int count)
{
	dbgevent_t *	e;

	ResetWDT();

	DBG_READY(e);
	
	DBG_IMP(e, DbgBackendSW->dbg_showframe);

	return DbgBackendSW->dbg_showframe(count);
}

dbgevent_t *
DbgGetFrame(void)
{
	dbgevent_t *	e;

	ResetWDT();

	DBG_READY(e);
	
	DBG_IMP(e, DbgBackendSW->dbg_getframe);

	return DbgBackendSW->dbg_getframe();
}

dbgevent_t *
DbgSetDebuggerVar(char *var, AIF *val)
{
	dbgevent_t *	e;

	ResetWDT();

	DBG_READY(e);
	
	DBG_IMP(e, DbgBackendSW->dbg_setdebuggervar);

	return DbgBackendSW->dbg_setdebuggervar(var, val);
}

dbgevent_t *
DbgSetSourcePath(char *path)
{
	dbgevent_t *	e;

	ResetWDT();

	DBG_READY(e);

	DBG_IMP(e, DbgBackendSW->dbg_setsourcepath);

	return DbgBackendSW->dbg_setsourcepath(path);
}

dbgevent_t *
DbgGetSourcePath()
{
	dbgevent_t *	e;

	ResetWDT();

	DBG_READY(e);
	
	DBG_IMP(e, DbgBackendSW->dbg_getsourcepath);

	return DbgBackendSW->dbg_getsourcepath();
}

dbgevent_t *
DbgGetDebuggerVar(char *var)
{
	dbgevent_t *	e;

	ResetWDT();

	DBG_READY(e);
	
	DBG_IMP(e, DbgBackendSW->dbg_getdebuggervar);

	return DbgBackendSW->dbg_getdebuggervar(var);
}

dbgevent_t *
DbgSetVar(char *var, AIF *data)
{
	dbgevent_t *	e;

	ResetWDT();

	DBG_READY(e);
	
	DBG_IMP(e, DbgBackendSW->dbg_setvar);

	return DbgBackendSW->dbg_setvar(var, data);
}

dbgevent_t *
DbgEvalExpr(char *expr)
{
	dbgevent_t *	e;

	ResetWDT();

	DBG_READY(e);
	
	DBG_IMP(e, DbgBackendSW->dbg_evalexpr);

	return DbgBackendSW->dbg_evalexpr(expr);
}

dbgevent_t *
DbgGetType(char *var, int isType)
{
	dbgevent_t *	e;

	ResetWDT();

	DBG_READY(e);
	
	DBG_IMP(e, DbgBackendSW->dbg_gettype);

	return DbgBackendSW->dbg_gettype(var, isType);
}

dbgevent_t *
DbgGetLocation(void)
{
	dbgevent_t *	e;

	ResetWDT();

	DBG_READY(e);
	
	DBG_IMP(e, DbgBackendSW->dbg_getlocation);

	return DbgBackendSW->dbg_getlocation();
}

dbgevent_t *
DbgOpenSocket(char *opset_name, int opset_size)
{
	dbgevent_t *	e;

	ResetWDT();

	DBG_READY(e);
	
	DBG_IMP(e, DbgBackendSW->dbg_opensocket);

	return DbgBackendSW->dbg_opensocket(opset_name, opset_size);
}

dbgevent_t *
DbgCloseSocket(void)
{
	dbgevent_t *	e;

	ResetWDT();

	DBG_READY(e);
	
	DBG_IMP(e, DbgBackendSW->dbg_closesocket);

	return DbgBackendSW->dbg_closesocket();
}

dbgevent_t *
DbgGetVars(char *file)
{
	dbgevent_t *	e;

	ResetWDT();

	DBG_READY(e);
	
	DBG_IMP(e, DbgBackendSW->dbg_getvars);

	return DbgBackendSW->dbg_getvars(file);
}

dbgevent_t *
DbgGetScope(char *location)
{
	dbgevent_t *	e;

	ResetWDT();

	DBG_READY(e);
	
	DBG_IMP(e, DbgBackendSW->dbg_getscope);

	return DbgBackendSW->dbg_getscope(location);
}

dbgevent_t *
DbgDirectCmd(int cmd_argc, char **cmd_argv)
{
	dbgevent_t *	e;

	ResetWDT();

	DBG_READY(e);
	
	DBG_IMP(e, DbgBackendSW->dbg_directcmd);

	return DbgBackendSW->dbg_directcmd(cmd_argc, cmd_argv);
}

dbgevent_t *
DbgStatus(int getLocation)
{
	dbgevent_t *	e;

	ResetWDT();

	DBG_CHECK(e);
	
	DBG_IMP(e, DbgBackendSW->dbg_status);

	return DbgBackendSW->dbg_status(getLocation);
}

dbgevent_t *
DbgQuit(int detach)
{
	dbgevent_t *	e;
	
	Shutdown++;
	
	DBG_CHECK(e);
	
	DBG_IMP(e, DbgBackendSW->dbg_quit);

	return DbgBackendSW->dbg_quit(detach);
}

dbgevent_t *
DbgInterrupt(void)
{
	dbgevent_t *	e;

	ResetWDT();

	DBG_CHECK(e);
	
	DBG_IMP(e, DbgBackendSW->dbg_interrupt);

	return DbgBackendSW->dbg_interrupt();
}

void
DbgSetBackendError(char *err)
{
	if (err == NULL)
	{
		return;	
	}
	
	if (_backend_err_str != NULL)
	{
		free(_backend_err_str);
	}
	
	_backend_err_str = strdup(err);
}

// Make sure to free the string if calling this!
char *
DbgGetBackendError(void)
{
	char *	rtn;
	
	rtn = _backend_err_str;
	
	_backend_err_str = NULL;
	
	return rtn;
}

