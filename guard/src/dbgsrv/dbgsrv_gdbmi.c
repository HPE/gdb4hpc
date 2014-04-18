/*********************************************************************************\
 * GDB interface routines
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

/*
** The Guard GDB/MI interface.
** Now uses the Eclipse SDM libmi interface.
*/

#ifdef HAVE_CONFIG_H
#include	<config.h>
#endif /* HAVE_CONFIG_H */

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/param.h>
#include <sys/wait.h>

#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <signal.h>
#include <stdarg.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <ctype.h>

#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "defs.h"
#include "useful.h"
#include "aif.h"
#include "dbgevent.h"
#include "dbgfn.h"
#include "dbgsrv_libmi2aif.h"
#include "dbgsrv_proto.h"
#include "mparch.h"
#include "expr.h"
#include "dbgsrv_util.h"
#include "dbgsrv_p2p.h"
#include "dbgsrv_defs.h"

#include "cray_tools_be.h"

#include "MI.h"

#define	 GDB_MIN_VERS 7.2
#define GDB_EXPECTED_ARCH "x86_64"

// Enum for supported cuda versions
enum cuda_vers
{
	CUDA_UNKNOWN,
	CUDA_50,
	CUDA_55
};
typedef enum cuda_vers		cuda_vers;

// functions with external visibility
void			SendMICommand(MISession *, MICommand *);

static MISession * 	GDBMISession;
static dbgstat		Status;
static int			Pid = 0;
static int			MPArch;
static char *		RunArgs = NULL;
static dbgbp_t *	CurrBP = (dbgbp_t *)NULL;
static list_t *		BP;
static list_t *		entry_BP;
static list_t *		exit_BP;
static char *		ProgMain;
static int			AsynchEventWaiting = 0;
static MIEvent *	PendingEvent = NULL;
static int			DirectCmdAsyncRestore = 0;

#ifndef ALPS
// These are for sending the target output to the fe
static int			WRITE_fd;
static int			dbgsrv_fd;
#endif

extern appInfo_t	other_app;

/* dbgbackend_sw functions */
static dbgevent_t *	DbgGDBMIInit(int *, int *, char **);
static dbgevent_t *	DbgGDBMIStartup(void);
static int			DbgGDBMIConsume(int);
static int			DbgGDBMIEventReady(void);
static dbgevent_t *	DbgGDBMISetLineBreak(char *, int);
static dbgevent_t *	DbgGDBMISetFuncBreak(char *, char *);
static dbgevent_t *	DbgGDBMISetWatch(int, char *);
static dbgevent_t *	DbgGDBMIDeleteBreak(int);
static dbgevent_t *	DbgGDBMIDisableBreak(int);
static dbgevent_t *	DbgGDBMIEnableBreak(int, int, int);
static dbgevent_t *	DbgGDBMILastBreak(void);
static dbgevent_t *	DbgGDBMIList(char *);
static dbgevent_t *	DbgGDBMIGetSources(void);
static dbgevent_t *	DbgGDBMIGo(void);
static dbgevent_t *	DbgGDBMIFinishFunc(void);
static dbgevent_t *	DbgGDBMISetArgs(char *);
static dbgevent_t *	DbgGDBMIStep(int, int);
static dbgevent_t *	DbgGDBMIKill(void);
static dbgevent_t *	DbgGDBMIMoveFrame(int, int);
static dbgevent_t *	DbgGDBMIShowFrame(int);
static dbgevent_t *	DbgGDBMIGetFrame(void);
static dbgevent_t *	DbgGDBMISetVar(char *, AIF *);
static dbgevent_t *	DbgGDBMIEvalExpr(char *);
static dbgevent_t *	DbgGDBMIGetType(char *, int);
static dbgevent_t *	DbgGDBMIGetVars(char *);
static dbgevent_t *	DbgGDBMIGetScope(char *);
static dbgevent_t *	DbgGDBMIStatus(int);
static dbgevent_t *	DbgGDBMIQuit(int);
static dbgevent_t *	DbgGDBMIInterrupt(void);
static dbgevent_t *	DbgGDBMIInvoke(char *, int, char *, char *, char *, char **, int);
static dbgevent_t *	DbgGDBMIAttach(int, char *, char *, char **, int);
static dbgevent_t *	DbgGDBMIOpenSocket(char *, int);
static dbgevent_t *	DbgGDBMICloseSocket(void);
static void			DbgGDBMIFinish(void);
static int			DbgGDBMIReadyForCmd(void);
static int			DbgGDBMIOK(void);
static int			DbgGDBMIStopped(void);
static dbgevent_t *	DbgGDBMISetDebuggerVar(char *, AIF *);
static dbgevent_t *	DbgGDBMISetSourcePath(char *);
static dbgevent_t *	DbgGDBMIGetSourcePath(void);
static dbgevent_t *	DbgGDBMIGetDebuggerVar(char *);
static dbgevent_t *	DbgGDBMIDirectCmd(int, char **);

/* auxiliary functions */
static dbgevent_t *	InitDbg(char *, char *);
static dbgevent_t *	InitDbgAttach(int, char *);
static dbgevent_t *	WaitEvent(dbgevent_t *e);
static dbgevent_t *	CheckBreak(MICommand *);
static void			Finish(int);
static dbgevent_t *	CheckVers(void);
static void			event_callback(MIEvent *);
static void			console_callback(char *);
static void			log_callback(char *);
static void			target_callback(char *);
static dbgbp_t *	get_dbgbp(MICommand *);
static dbgevent_t *	delete_internal_breakpoints(list_t *);
static dbgevent_t *	setup_gdbmi_environment(void);
static dbgevent_t *	setup_gdbmi_signals(void);
static dbgevent_t *	setup_gdbmi_entry_points(void);
static dbgevent_t *	setup_gdbmi_lastchance_entry_points(void);
static dbgevent_t *	setup_gdbmi_exit_points(void);
static dbgevent_t *	ResetBeforeDirectCmd(void);
static dbgevent_t *	RestoreAfterDirectCmd(void);

/* hashing and slicing functions */
static dbgevent_t *	PerformExtEvalExpr(char *);

/* debugger definition */
dbgbackend_sw	DbgBackendGDBMI =
{
	DbgGDBMIInit,
	DbgGDBMIStartup,
	DbgGDBMIConsume,
	DbgGDBMIEventReady,
	NULL,	// Not implemented ProgIO_hook
	DbgGDBMISetLineBreak,
	DbgGDBMISetFuncBreak,
	DbgGDBMISetWatch,
	DbgGDBMIDeleteBreak,
	DbgGDBMIDisableBreak,
	DbgGDBMIEnableBreak,
	DbgGDBMILastBreak,
	DbgGDBMIList,
	DbgGDBMIGetSources,
	DbgGDBMIGo,
	DbgGDBMIFinishFunc,
	DbgGDBMISetArgs,
	DbgGDBMIStep,
	DbgGDBMIKill,
	DbgGDBMIMoveFrame,
	DbgGDBMIShowFrame,
	DbgGDBMIGetFrame,
	DbgGDBMISetVar,
	DbgGDBMIEvalExpr,
	DbgGDBMIGetType,
	NULL, // DbgGDBMIGetLocation, not used
	DbgGDBMIGetVars,
	DbgGDBMIGetScope,
	DbgGDBMIStatus,
	DbgGDBMIQuit,
	DbgGDBMIInterrupt,
	DbgGDBMIInvoke,
	DbgGDBMIAttach,
	DbgGDBMIFinish,
	DbgGDBMIReadyForCmd,
	DbgGDBMIOK,
	DbgGDBMIStopped,
	DbgGDBMISetDebuggerVar,
	DbgGDBMISetSourcePath,
	DbgGDBMIGetSourcePath,
	DbgGDBMIGetDebuggerVar,
	DbgGDBMIDirectCmd,
	DbgGDBMIOpenSocket,
	DbgGDBMICloseSocket
};

/* internal static functions */

static dbgbp_t *
get_dbgbp(MICommand *cmd)
{
	dbgbp_t *		bp;
	MIBreakpoint *	bpt;
	MIList *		bpts;

	if (cmd == NULL)
		return NULL;
	
	bpts = MIBreakpointGetBreakInsertInfo(cmd);

	if (bpts == NULL) {
		return NULL;
	}
	
	MIListSet(bpts);
	bpt = (MIBreakpoint *)MIListGet(bpts);

	bp = NewBP(bpt->number);

	if (bpt->isWpt)
	{
		if (bpt->isWWpt)
		{
			if (bpt->isRWpt)
				bp->bp_type = DBGBP_AWATCH_TYPE;
			else
				bp->bp_type = DBGBP_WATCH_TYPE;
		} else if (bpt->isRWpt)
		{
			bp->bp_type = DBGBP_RWATCH_TYPE;
		}
	} else
	{
		bp->bp_type = DBGBP_BREAK_TYPE;
	}
	
	bp->bp_enabled = bpt->enabled;
	
	// set the breakpoint disposition
	if (strncmp(bpt->disp, "keep", 4) == 0)
		bp->bp_disp = DBGBP_KEEP_DISP;
	else if (strncmp(bpt->disp, "del", 3) == 0)
		bp->bp_disp = DBGBP_DELETE_DISP;
	else if (strncmp(bpt->disp, "dis", 3) == 0)
		bp->bp_disp = DBGBP_DISABLE_DISP;
	else
		bp->bp_disp = DBGBP_UNKNOWN_DISP;
	
	bp->bp_loc.loc_line = bpt->line;
	if (bpt->file != NULL)
		bp->bp_loc.loc_file = strdup(bpt->file);
	if (bpt->func != NULL)
		bp->bp_loc.loc_func = strdup(bpt->func);
	if (bpt->address != NULL)
		bp->bp_loc.loc_addr = strdup(bpt->address);
		
	DVPRINT("get_dbgbp: bp_id is %d\n", bp->bp_id);
	
	MIListFree(bpts, MIBreakpointFree);
	
	return bp;
}

static dbgevent_t *
delete_internal_breakpoints(list_t *bplist)
{
	dbgevent_t *	e;
	MICommand *		cmd;
	dbgbp_t *		bp;
	
	SetList(bplist);
	
	while( (bp = (dbgbp_t *)GetListElement(bplist)) != (dbgbp_t *)NULL)
	{
		// send the delete command
		cmd = MIBreakDelete(1, &bp->bp_id);
		SendMICommand(GDBMISession, cmd);
		if (!MICommandResultOK(cmd)) 
		{
			MICommandFree(cmd);
			EVENT_ERROR(e, DBGERR_NOBP, "Delete internal breakpoint failed.");
			return e;
		}
		MICommandFree(cmd);
		
		// remove the breakpoint from the internal list and delete it
		RemoveFromList(bplist, (void *)bp);
		FreeBP(bp);
	}
	
	return (dbgevent_t *)NULL;
}

static dbgevent_t *	
setup_gdbmi_environment(void)
{
	dbgevent_t *	e;
	MICommand *		cmd;
	
	cmd = MIGDBSet("confirm", "off");
	SendMICommand(GDBMISession, cmd);
	if (!MICommandResultOK(cmd)) 
	{
		MICommandFree(cmd);
		EVENT_ERROR(e, DBGERR_DEBUGGER, "Gdb set confirm command failed.");
		return e;
	}
	MICommandFree(cmd);
	
	// turn history off
	cmd = MIGDBSet("history size", "0");
	SendMICommand(GDBMISession, cmd);
	if (!MICommandResultOK(cmd)) 
	{
		MICommandFree(cmd);
		EVENT_ERROR(e, DBGERR_DEBUGGER, "Gdb set history size command failed.");
		return e;
	}
	MICommandFree(cmd);
	
	// do not open source files
	cmd = MIGDBSet("source open", "off");
	SendMICommand(GDBMISession, cmd);
	if (!MICommandResultOK(cmd)) 
	{
		MICommandFree(cmd);
		EVENT_ERROR(e, DBGERR_DEBUGGER, "Gdb set source open command failed.");
		return e;
	}
	MICommandFree(cmd);
	
	// we need gdb to set print elements and print repeats to 0 so 
	// that when we resolve the data it gives us the entire contents
	// of the data type.
	cmd = MIGDBSet("print elements", "0");
	SendMICommand(GDBMISession, cmd);
	if (!MICommandResultOK(cmd)) 
	{
		MICommandFree(cmd);
		EVENT_ERROR(e, DBGERR_DEBUGGER, "Gdb set print elements command failed.");
		return e;
	}
	MICommandFree(cmd);
	
	cmd = MIGDBSet("print repeats", "0");
	SendMICommand(GDBMISession, cmd);
	if (!MICommandResultOK(cmd)) 
	{
		MICommandFree(cmd);
		EVENT_ERROR(e, DBGERR_DEBUGGER, "Gdb set print repeats command failed.");
		return e;
	}
	MICommandFree(cmd);
	
	// stop printing strings on the first null character
	cmd = MIGDBSet("print null-stop", "on");
	SendMICommand(GDBMISession, cmd);
	if (!MICommandResultOK(cmd)) 
	{
		MICommandFree(cmd);
		EVENT_ERROR(e, DBGERR_DEBUGGER, "Gdb set print null-stop command failed.");
		return e;
	}
	MICommandFree(cmd);
	
	// demangle c++ names
	cmd = MIGDBSet("print demangle", "on");
	SendMICommand(GDBMISession, cmd);
	if (!MICommandResultOK(cmd)) 
	{
		MICommandFree(cmd);
		EVENT_ERROR(e, DBGERR_DEBUGGER, "Gdb set print demangle command failed.");
		return e;
	}
	MICommandFree(cmd);
	
	cmd = MIGDBSet("print asm-demangle", "on");
	SendMICommand(GDBMISession, cmd);
	if (!MICommandResultOK(cmd)) 
	{
		MICommandFree(cmd);
		EVENT_ERROR(e, DBGERR_DEBUGGER, "Gdb set print asm-demangle command failed.");
		return e;
	}
	MICommandFree(cmd);
	
	// print derived types from the vtable
	cmd = MIGDBSet("print object", "on");
	SendMICommand(GDBMISession, cmd);
	if (!MICommandResultOK(cmd)) 
	{
		MICommandFree(cmd);
		EVENT_ERROR(e, DBGERR_DEBUGGER, "Gdb set print object command failed.");
		return e;
	}
	MICommandFree(cmd);
	
	// limit strings to 200 characters
	cmd = MIGDBSet("print characters", "200");
	SendMICommand(GDBMISession, cmd);
	if (!MICommandResultOK(cmd)) 
	{
		MICommandFree(cmd);
		EVENT_ERROR(e, DBGERR_DEBUGGER, "Gdb set print characters command failed.");
		return e;
	}
	MICommandFree(cmd);
	
	// don't allow pending breakpoints
	cmd = MIGDBSet("breakpoint pending", "off");
	SendMICommand(GDBMISession, cmd);
	if (!MICommandResultOK(cmd)) 
	{
		MICommandFree(cmd);
		EVENT_ERROR(e, DBGERR_DEBUGGER, "Gdb set breakpoint pending command failed.");
		return e;
	}
	MICommandFree(cmd);
	
	// limit backtrace size to 1000
	cmd = MIGDBSet("backtrace limit", "1000");
	SendMICommand(GDBMISession, cmd);
	if (!MICommandResultOK(cmd)) 
	{
		MICommandFree(cmd);
		EVENT_ERROR(e, DBGERR_DEBUGGER, "Gdb set backtrace limit command failed.");
		return e;
	}
	MICommandFree(cmd);
	
	// unwind signals triggered by gdb
	cmd = MIGDBSet("unwindonsignal", "on");
	SendMICommand(GDBMISession, cmd);
	if (!MICommandResultOK(cmd)) 
	{
		MICommandFree(cmd);
		EVENT_ERROR(e, DBGERR_DEBUGGER, "Gdb set unwindonsignal command failed.");
		return e;
	}
	MICommandFree(cmd);
	
	// set can-use-hw-watchpoints to 1
	cmd = MIGDBSet("can-use-hw-watchpoints", "1");
	SendMICommand(GDBMISession, cmd);
	if (!MICommandResultOK(cmd)) 
	{
		MICommandFree(cmd);
		EVENT_ERROR(e, DBGERR_DEBUGGER, "Gdb set can-use-hw-watchpoints command failed.");
		return e;
	}
	MICommandFree(cmd);
	
	// Don't look for auxillary symbol files - this *must* be turned off for gpu's
	if (!gpu_flag)
	{
		cmd = MIGDBSet("mapped-directory", "/dev/null");
		SendMICommand(GDBMISession, cmd);
		if (!MICommandResultOK(cmd)) 
		{
			MICommandFree(cmd);
			EVENT_ERROR(e, DBGERR_DEBUGGER, "Gdb set mapped-directory command failed.");
			return e;
		}
		MICommandFree(cmd);
	}
	
	// Need to set an alternative system root for /dsl and dso lookup if the
	// /dsl directory exists.
	struct stat		st;
	
	if (stat("/dsl", &st) == 0)
	{
		// The directory exists
		cmd = MIGDBSet("solib-absolute-prefix", "/dsl");
		SendMICommand(GDBMISession, cmd);
		if (!MICommandResultOK(cmd)) 
		{
			MICommandFree(cmd);
			EVENT_ERROR(e, DBGERR_DEBUGGER, "Gdb set solib-absolute-prefix command failed.");
			return e;
		}
		MICommandFree(cmd);
	}
	
	// If running with gpu's, turn upc off. If we don't do this, things will break
	if (gpu_flag)
	{
		cmd = MIGDBSet("upcsingle", "1");
		SendMICommand(GDBMISession, cmd);
		if (!MICommandResultOK(cmd)) 
		{
			MICommandFree(cmd);
			EVENT_ERROR(e, DBGERR_DEBUGGER, "Gdb set upcsingle command failed.");
			return e;
		}
		MICommandFree(cmd);
	}
	
	// XXX: This actual does do something with the Allinea branch of gdb.
	// DO NOT REMOVE! If removed, the backend will hang on gdb fatal errors.
	cmd = MIGDBSet("continue-on-fatal-error", "on");
	SendMICommand(GDBMISession, cmd);
	MICommandFree(cmd);
	
	return NULL;
}

static dbgevent_t *	
setup_gdbmi_signals(void)
{
	dbgevent_t *	e;
	MICommand *		cmd;
	
	cmd = CLIHandle("SIG64 nostop noprint pass");
	SendMICommand(GDBMISession, cmd);
	if (!MICommandResultOK(cmd)) {
		MICommandFree(cmd);
		EVENT_ERROR(e, DBGERR_DEBUGGER, "Handle SIG64 command failed.");
		return e;	
	}
	MICommandFree(cmd);

	cmd = CLIHandle("SIG63 nostop noprint pass");
	SendMICommand(GDBMISession, cmd);
	if (!MICommandResultOK(cmd)) {
		MICommandFree(cmd);
		EVENT_ERROR(e, DBGERR_DEBUGGER, "Handle SIG63 command failed.");
		return e;	
	}
	MICommandFree(cmd);
	
	cmd = CLIHandle("SIG39 nostop noprint pass");
	SendMICommand(GDBMISession, cmd);
	if (!MICommandResultOK(cmd)) {
		MICommandFree(cmd);
		EVENT_ERROR(e, DBGERR_DEBUGGER, "Handle SIG39 command failed.");
		return e;	
	}
	MICommandFree(cmd);
	
	cmd = CLIHandle("SIG33 nostop noprint pass");
	SendMICommand(GDBMISession, cmd);
	if (!MICommandResultOK(cmd)) {
		MICommandFree(cmd);
		EVENT_ERROR(e, DBGERR_DEBUGGER, "Handle SIG33 command failed.");
		return e;	
	}
	MICommandFree(cmd);
	
	cmd = CLIHandle("SIG32 nostop noprint pass");
	SendMICommand(GDBMISession, cmd);
	if (!MICommandResultOK(cmd)) {
		MICommandFree(cmd);
		EVENT_ERROR(e, DBGERR_DEBUGGER, "Handle SIG32 command failed.");
		return e;	
	}
	MICommandFree(cmd);
	
	cmd = CLIHandle("SIGUSR1 nostop noprint pass");
	SendMICommand(GDBMISession, cmd);
	if (!MICommandResultOK(cmd)) {
		MICommandFree(cmd);
		EVENT_ERROR(e, DBGERR_DEBUGGER, "Handle SIGUSR1 command failed.");
		return e;	
	}
	MICommandFree(cmd);
	
	cmd = CLIHandle("SIGUSR2 nostop noprint pass");
	SendMICommand(GDBMISession, cmd);
	if (!MICommandResultOK(cmd)) {
		MICommandFree(cmd);
		EVENT_ERROR(e, DBGERR_DEBUGGER, "Handle SIGUSR2 command failed.");
		return e;	
	}
	MICommandFree(cmd);
	
	// We use this signal to interrupt the application. SIGINT is blocked and
	// won't work (I can't figure out why...).
	cmd = CLIHandle("SIGSTOP stop print nopass");
	SendMICommand(GDBMISession, cmd);
	if (!MICommandResultOK(cmd)) {
		MICommandFree(cmd);
		EVENT_ERROR(e, DBGERR_DEBUGGER, "Handle SIGSTOP command failed.");
		return e;	
	}
	MICommandFree(cmd);
	
	cmd = CLIHandle("SIGCONT nostop noprint pass");
	SendMICommand(GDBMISession, cmd);
	if (!MICommandResultOK(cmd)) {
		MICommandFree(cmd);
		EVENT_ERROR(e, DBGERR_DEBUGGER, "Handle SIGCONT command failed.");
		return e;	
	}
	MICommandFree(cmd);
	
	cmd = CLIHandle("SIGTSTP nostop noprint pass");
	SendMICommand(GDBMISession, cmd);
	if (!MICommandResultOK(cmd)) {
		MICommandFree(cmd);
		EVENT_ERROR(e, DBGERR_DEBUGGER, "Handle SIGTSTP command failed.");
		return e;	
	}
	MICommandFree(cmd);
	
	cmd = CLIHandle("SIGCHLD nostop noprint pass");
	SendMICommand(GDBMISession, cmd);
	if (!MICommandResultOK(cmd)) {
		MICommandFree(cmd);
		EVENT_ERROR(e, DBGERR_DEBUGGER, "Handle SIGCHLD command failed.");
		return e;	
	}
	MICommandFree(cmd);
	
	cmd = CLIHandle("SIGPROF nostop noprint pass");
	SendMICommand(GDBMISession, cmd);
	if (!MICommandResultOK(cmd)) {
		MICommandFree(cmd);
		EVENT_ERROR(e, DBGERR_DEBUGGER, "Handle SIGPROF command failed.");
		return e;	
	}
	MICommandFree(cmd);
	
	cmd = CLIHandle("SIGALRM nostop noprint pass");
	SendMICommand(GDBMISession, cmd);
	if (!MICommandResultOK(cmd)) {
		MICommandFree(cmd);
		EVENT_ERROR(e, DBGERR_DEBUGGER, "Handle SIGALRM command failed.");
		return e;	
	}
	MICommandFree(cmd);
	
	cmd = CLIHandle("SIGVTALRM nostop noprint pass");
	SendMICommand(GDBMISession, cmd);
	if (!MICommandResultOK(cmd)) {
		MICommandFree(cmd);
		EVENT_ERROR(e, DBGERR_DEBUGGER, "Handle SIGVTALRM command failed.");
		return e;	
	}
	MICommandFree(cmd);

	return NULL;
}

static dbgevent_t *
setup_gdbmi_entry_points(void)
{
	dbgevent_t *	e;
	MICommand *		cmd;
	dbgbp_t *		bp_res;
	
	// set the lang to c. We always want to do this!!
	cmd = MIGDBSet("lang", "c");
	SendMICommand(GDBMISession, cmd);
	if (!MICommandResultOK(cmd)) 
	{
		MICommandFree(cmd);
		EVENT_ERROR(e, DBGERR_DEBUGGER, "Gdb set lang command failed.");
		return e;
	}
	MICommandFree(cmd);
	
	entry_BP = NewList();
	
	cmd = MIBreakInsert(0, 0, NULL, 0, "*(&MPI_Init)", 0);
	SendMICommand(GDBMISession, cmd);
	// Note we only want to insert the breakpoint into our internal list if the
	// command succeeded. We expect many of these to fail.
	if (MICommandResultOK(cmd)) 
	{
		if ((bp_res = get_dbgbp(cmd)) != NULL)
			AddToList(entry_BP, (void *)bp_res);
	}
	MICommandFree(cmd);
	
	cmd = MIBreakInsert(0, 0, NULL, 0, "athos_mpi_init", 0);
	SendMICommand(GDBMISession, cmd);
	if (MICommandResultOK(cmd)) 
	{
		if ((bp_res = get_dbgbp(cmd)) != NULL)
			AddToList(entry_BP, (void *)bp_res);
	}
	MICommandFree(cmd);
	
	cmd = MIBreakInsert(0, 0, NULL, 0, "MPI_INIT_THREAD__", 0);
	SendMICommand(GDBMISession, cmd);
	if (MICommandResultOK(cmd)) 
	{
		if ((bp_res = get_dbgbp(cmd)) != NULL)
			AddToList(entry_BP, (void *)bp_res);
	}
	MICommandFree(cmd);
	
	cmd = MIBreakInsert(0, 0, NULL, 0, "MPI_INIT__", 0);
	SendMICommand(GDBMISession, cmd);
	if (MICommandResultOK(cmd)) 
	{
		if ((bp_res = get_dbgbp(cmd)) != NULL)
			AddToList(entry_BP, (void *)bp_res);
	}
	MICommandFree(cmd);
	
	cmd = MIBreakInsert(0, 0, NULL, 0, "mpi_init__", 0);
	SendMICommand(GDBMISession, cmd);
	if (MICommandResultOK(cmd)) 
	{
		if ((bp_res = get_dbgbp(cmd)) != NULL)
			AddToList(entry_BP, (void *)bp_res);
	}
	MICommandFree(cmd);
	
	cmd = MIBreakInsert(0, 0, NULL, 0, "mpi_init_thread__", 0);
	SendMICommand(GDBMISession, cmd);
	if (MICommandResultOK(cmd)) 
	{
		if ((bp_res = get_dbgbp(cmd)) != NULL)
			AddToList(entry_BP, (void *)bp_res);
	}
	MICommandFree(cmd);
	
	cmd = MIBreakInsert(0, 0, NULL, 0, "MPI_INIT_THREAD_", 0);
	SendMICommand(GDBMISession, cmd);
	if (MICommandResultOK(cmd)) 
	{
		if ((bp_res = get_dbgbp(cmd)) != NULL)
			AddToList(entry_BP, (void *)bp_res);
	}
	MICommandFree(cmd);
	
	cmd = MIBreakInsert(0, 0, NULL, 0, "MPI_INIT_", 0);
	SendMICommand(GDBMISession, cmd);
	if (MICommandResultOK(cmd)) 
	{
		if ((bp_res = get_dbgbp(cmd)) != NULL)
			AddToList(entry_BP, (void *)bp_res);
	}
	MICommandFree(cmd);
	
	cmd = MIBreakInsert(0, 0, NULL, 0, "mpi_init_thread_", 0);
	SendMICommand(GDBMISession, cmd);
	if (MICommandResultOK(cmd)) 
	{
		if ((bp_res = get_dbgbp(cmd)) != NULL)
			AddToList(entry_BP, (void *)bp_res);
	}
	MICommandFree(cmd);
	
	cmd = MIBreakInsert(0, 0, NULL, 0, "mpi_init_", 0);
	SendMICommand(GDBMISession, cmd);
	if (MICommandResultOK(cmd)) 
	{
		if ((bp_res = get_dbgbp(cmd)) != NULL)
			AddToList(entry_BP, (void *)bp_res);
	}
	MICommandFree(cmd);
	
	cmd = MIBreakInsert(0, 0, NULL, 0, "MPI::Init", 0);
	SendMICommand(GDBMISession, cmd);
	if (MICommandResultOK(cmd)) 
	{
		if ((bp_res = get_dbgbp(cmd)) != NULL)
			AddToList(entry_BP, (void *)bp_res);
	}
	MICommandFree(cmd);
	
	cmd = MIBreakInsert(0, 0, NULL, 0, "*&(MPI_Init_thread)", 0);
	SendMICommand(GDBMISession, cmd);
	if (MICommandResultOK(cmd)) 
	{
		if ((bp_res = get_dbgbp(cmd)) != NULL)
			AddToList(entry_BP, (void *)bp_res);
	}
	MICommandFree(cmd);
	
	cmd = MIBreakInsert(0, 0, NULL, 0, "MPI_Init()", 0);
	SendMICommand(GDBMISession, cmd);
	if (MICommandResultOK(cmd)) 
	{
		if ((bp_res = get_dbgbp(cmd)) != NULL)
			AddToList(entry_BP, (void *)bp_res);
	}
	MICommandFree(cmd);
	
	cmd = MIBreakInsert(0, 0, NULL, 0, "MPI::Init(int&, char**&)", 0);
	SendMICommand(GDBMISession, cmd);
	if (MICommandResultOK(cmd)) 
	{
		if ((bp_res = get_dbgbp(cmd)) != NULL)
			AddToList(entry_BP, (void *)bp_res);
	}
	MICommandFree(cmd);
	
	cmd = MIBreakInsert(0, 0, NULL, 0, "MPI_Init", 0);
	SendMICommand(GDBMISession, cmd);
	if (MICommandResultOK(cmd)) 
	{
		if ((bp_res = get_dbgbp(cmd)) != NULL)
			AddToList(entry_BP, (void *)bp_res);
	}
	MICommandFree(cmd);
	
	cmd = MIBreakInsert(0, 0, NULL, 0, "*&(PMPI_Init)", 0);
	SendMICommand(GDBMISession, cmd);
	if (MICommandResultOK(cmd)) 
	{
		if ((bp_res = get_dbgbp(cmd)) != NULL)
			AddToList(entry_BP, (void *)bp_res);
	}
	MICommandFree(cmd);
	
	cmd = MIBreakInsert(0, 0, NULL, 0, "*&(start_pes)", 0);
	SendMICommand(GDBMISession, cmd);
	if (MICommandResultOK(cmd)) 
	{
		if ((bp_res = get_dbgbp(cmd)) != NULL)
			AddToList(entry_BP, (void *)bp_res);
	}
	MICommandFree(cmd);
	
	cmd = MIBreakInsert(0, 0, NULL, 0, "*&(shmem_init)", 0);
	SendMICommand(GDBMISession, cmd);
	if (MICommandResultOK(cmd)) 
	{
		if ((bp_res = get_dbgbp(cmd)) != NULL)
			AddToList(entry_BP, (void *)bp_res);
	}
	MICommandFree(cmd);
	
	// set the lang back to auto. We always want to do this!!
	cmd = MIGDBSet("lang", "auto");
	SendMICommand(GDBMISession, cmd);
	if (!MICommandResultOK(cmd)) 
	{
		MICommandFree(cmd);
		EVENT_ERROR(e, DBGERR_DEBUGGER, "Gdb set lang command failed.");
		return e;
	}
	MICommandFree(cmd);

	return NULL;
}

static dbgevent_t *
setup_gdbmi_lastchance_entry_points(void)
{
	dbgevent_t *	e;
	MICommand *		cmd;
	dbgbp_t *		bp_res;
	
	// set the lang to c. We always want to do this!!
	cmd = MIGDBSet("lang", "c");
	SendMICommand(GDBMISession, cmd);
	if (!MICommandResultOK(cmd)) 
	{
		MICommandFree(cmd);
		EVENT_ERROR(e, DBGERR_DEBUGGER, "Gdb set lang command failed.");
		return e;
	}
	MICommandFree(cmd);
	
	cmd = MIBreakInsert(0, 0, NULL, 0, "main", 0);
	SendMICommand(GDBMISession, cmd);
	// Note we only want to insert the breakpoint into our internal list if the
	// command succeeded.
	if (MICommandResultOK(cmd)) 
	{
		if ((bp_res = get_dbgbp(cmd)) != NULL)
			AddToList(entry_BP, (void *)bp_res);
	}
	MICommandFree(cmd);
	
	// set the lang back to auto. We always want to do this!!
	cmd = MIGDBSet("lang", "auto");
	SendMICommand(GDBMISession, cmd);
	if (!MICommandResultOK(cmd)) 
	{
		MICommandFree(cmd);
		EVENT_ERROR(e, DBGERR_DEBUGGER, "Gdb set lang command failed.");
		return e;
	}
	MICommandFree(cmd);
	
	return NULL;
}

static dbgevent_t *
setup_gdbmi_exit_points(void)
{
	dbgevent_t *	e;
	MICommand *		cmd;
	dbgbp_t *		bp_res;
	
	// set the lang to c. We always want to do this!!
	cmd = MIGDBSet("lang", "c");
	SendMICommand(GDBMISession, cmd);
	if (!MICommandResultOK(cmd)) 
	{
		MICommandFree(cmd);
		EVENT_ERROR(e, DBGERR_DEBUGGER, "Gdb set lang command failed.");
		return e;
	}
	MICommandFree(cmd);
	
	exit_BP = NewList();
	
	// Try to set a breakpoint at all of the regular exit points.
	// If any command fails, don't worry about it. We expect many of these
	// breakpoints to not exist.
	// FIXME: Figure out what the proper behavior should be. Should we warn if
	//        no exit point is found??
	
	cmd = MIBreakInsert(0, 0, NULL, 0, "*(&exit)", 0);
	SendMICommand(GDBMISession, cmd);
	if (MICommandResultOK(cmd)) 
	{
		if ((bp_res = get_dbgbp(cmd)) != NULL)
		{
			// Change the breakpoint type to an exit point or DBGBP_EXIT_TYPE
			bp_res->bp_type = DBGBP_EXIT_TYPE;
			AddToList(exit_BP, (void *)bp_res);
		}
	}
	MICommandFree(cmd);
	

	cmd = MIBreakInsert(0, 0, NULL, 0, "*(&_exit)", 0);
	SendMICommand(GDBMISession, cmd);
	if (MICommandResultOK(cmd)) 
	{
		if ((bp_res = get_dbgbp(cmd)) != NULL)
		{
			// Change the breakpoint type to an exit point or DBGBP_EXIT_TYPE
			bp_res->bp_type = DBGBP_EXIT_TYPE;
			AddToList(exit_BP, (void *)bp_res);
		}
	}
	MICommandFree(cmd);
	
	cmd = MIBreakInsert(0, 0, NULL, 0, "*(&abort)", 0);
	SendMICommand(GDBMISession, cmd);
	if (MICommandResultOK(cmd)) 
	{
		if ((bp_res = get_dbgbp(cmd)) != NULL)
		{
			// Change the breakpoint type to an exit point or DBGBP_EXIT_TYPE
			bp_res->bp_type = DBGBP_EXIT_TYPE;
			AddToList(exit_BP, (void *)bp_res);
		}
	}
	MICommandFree(cmd);
	
	cmd = MIBreakInsert(0, 0, NULL, 0, "*(&MPIR_Errors_are_fatal)", 0);
	SendMICommand(GDBMISession, cmd);
	if (MICommandResultOK(cmd)) 
	{
		if ((bp_res = get_dbgbp(cmd)) != NULL)
		{
			// Change the breakpoint type to an exit point or DBGBP_EXIT_TYPE
			bp_res->bp_type = DBGBP_EXIT_TYPE;
			AddToList(exit_BP, (void *)bp_res);
		}
	}
	MICommandFree(cmd);
	
	cmd = MIBreakInsert(0, 0, NULL, 0, "*(&MPID_Abort)", 0);
	SendMICommand(GDBMISession, cmd);
	if (MICommandResultOK(cmd)) 
	{
		if ((bp_res = get_dbgbp(cmd)) != NULL)
		{
			// Change the breakpoint type to an exit point or DBGBP_EXIT_TYPE
			bp_res->bp_type = DBGBP_EXIT_TYPE;
			AddToList(exit_BP, (void *)bp_res);
		}
	}
	MICommandFree(cmd);
	
	cmd = MIBreakInsert(0, 0, NULL, 0, "*(&MPI_Abort)", 0);
	SendMICommand(GDBMISession, cmd);
	if (MICommandResultOK(cmd)) 
	{
		if ((bp_res = get_dbgbp(cmd)) != NULL)
		{
			// Change the breakpoint type to an exit point or DBGBP_EXIT_TYPE
			bp_res->bp_type = DBGBP_EXIT_TYPE;
			AddToList(exit_BP, (void *)bp_res);
		}
	}
	MICommandFree(cmd);
	
	cmd = MIBreakInsert(0, 0, NULL, 0, "*(&ompi_mpi_errors_are_fatal_comm_handler)", 0);
	SendMICommand(GDBMISession, cmd);
	if (MICommandResultOK(cmd)) 
	{
		if ((bp_res = get_dbgbp(cmd)) != NULL)
		{
			// Change the breakpoint type to an exit point or DBGBP_EXIT_TYPE
			bp_res->bp_type = DBGBP_EXIT_TYPE;
			AddToList(exit_BP, (void *)bp_res);
		}
	}
	MICommandFree(cmd);
	
	// set the lang back to auto. We always want to do this!!
	cmd = MIGDBSet("lang", "auto");
	SendMICommand(GDBMISession, cmd);
	if (!MICommandResultOK(cmd)) 
	{
		MICommandFree(cmd);
		EVENT_ERROR(e, DBGERR_DEBUGGER, "Gdb set lang command failed.");
		return e;
	}
	MICommandFree(cmd);

	return NULL;
}

/*
**
** ----- dbgbackend_sw functions -----
**
*/

static dbgevent_t *
DbgGDBMIInit(int *dfd, int *iofd, char **env)
{
	dbgevent_t *	e;
	
	DVPRINT("*** DbgGDBMIInit\n");

	GDBMISession = MISessionNew();

	if (debug_flag)
		MISessionSetDebug(1);
	else
		MISessionSetDebug(0);

	MISessionRegisterEventCallback(GDBMISession, event_callback);
	MISessionRegisterConsoleCallback(GDBMISession, console_callback);
	MISessionRegisterLogCallback(GDBMISession, log_callback);
	MISessionRegisterTargetCallback(GDBMISession, target_callback);

	// set the gdb path to use gdb-guard or cuda-gdb-guard
	if (gpu_flag)
	{
		// Need to determine the version of the driver to start the proper gdb
		FILE *		vers;
		char *		vers_str = NULL;
		cuda_vers	vers_num = CUDA_UNKNOWN;
		
		// Open the driver version file, if this fails we are not on a gpu node
		// and should default to using the regular gdb.
		if ((vers = fopen("/proc/driver/nvidia/version", "r")) == NULL)
		{
			DVPRINT("Failed to open /proc/driver/nvidia/version\n");
			DVPRINT("Defaulting to non-gpu mode.\n");
			MISessionSetGDBPath(GDBMISession, "gdb-guard");
			gpu_flag = 0;
		} else
		{
			// We can open the driver version file, so now we need to grab the
			// driver number
			if (fscanf(vers, "NVRM version: NVIDIA UNIX x86_64 Kernel Module%*[ \t]%m[0-9.]", &vers_str) == 0)
			{
				// Failed to determine driver version!!!!
				fclose(vers);
				DVPRINT("Failed to determine driver version.\n");
				DVPRINT("Defaulting to non-gpu mode.\n");
				MISessionSetGDBPath(GDBMISession, "gdb-guard");
				gpu_flag = 0;
			} else
			{
				// We now have the version string, close the vers file
				fclose(vers);
			
				DVPRINT("Detected Nvidia driver %s\n", vers_str);
				
				// Now strcmp the driver version with a list of supported versions
				if (!strncmp(vers_str, "319.82", 6))
				{
					vers_num = CUDA_55;
				} else if (!strncmp(vers_str, "319.72", 6))
				{
					vers_num = CUDA_55;
				} else if (!strncmp(vers_str, "319.62", 6))
				{
					vers_num = CUDA_55;
				} else if (!strncmp(vers_str, "319.37", 6))
				{
					vers_num = CUDA_55;
				} else if (!strncmp(vers_str, "319.15", 6))
				{
					vers_num = CUDA_55;
				} else if (!strncmp(vers_str, "304.47.05", 9))
				{
					vers_num = CUDA_50;
				} else if (!strncmp(vers_str, "304.47.04", 9))
				{
					vers_num = CUDA_50;
				} else if (!strncmp(vers_str, "304.47.01", 9))
				{
					vers_num = CUDA_50;
				} else if (!strncmp(vers_str, "304.47", 6))
				{
					vers_num = CUDA_50;
				} else if (!strncmp(vers_str, "304.37", 6))
				{
					vers_num = CUDA_50;
				} else if (!strncmp(vers_str, "304.24", 6))
				{
					vers_num = CUDA_50;
				} else if (!strncmp(vers_str, "304.08", 6))
				{
					vers_num = CUDA_UNKNOWN;
				}
				
				// force tmpdir to /tmp if GPU mode is on.
				// This is a stupid Nvidia requirement. They utilize unix pipes
				// that are writen in TMPDIR and this value *MUST* match the value
				// set in the applications environment otherwise things will hang.
				if (vers_num != CUDA_UNKNOWN)
				{
					char *			old_tmpdir;
					struct stat		st;
					
					// Get the applications setting of TMPDIR
					if ((old_tmpdir = getenv("CRAYTOOL_OLD_TMPDIR")) != NULL)
					{
						DVPRINT("Using application TMPDIR setting of %s\n", old_tmpdir);
						if (setenv("TMPDIR", old_tmpdir, 1) < 0)
						{
							// failure
							DVPRINT("setenv failed");
						}
						// ensure the directory exists, otherwise create it
						// try to stat the directory
						if (stat(old_tmpdir, &st) != 0)
						{
							// the directory doesn't exist so we need to create it
							// use perms 755
							if (mkdir(old_tmpdir, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH) != 0)
							{
								DVPRINT("Could not create non-existent TMPDIR!\n");
							} else
							{
								DVPRINT("Created TMPDIR at %s\n", old_tmpdir);
							}
						}
					} else
					{
						DVPRINT("CRAYTOOL_OLD_TMPDIR env var not set. Defaulting to /tmp.\n");
						if (setenv("TMPDIR", "/tmp", 1) < 0)
						{
							// failure
							DVPRINT("setenv failed");
						}
					}
				}
				
				switch (vers_num)
				{
					case CUDA_50:
						DVPRINT("Using cuda50-gdb-guard\n");
						MISessionSetGDBPath(GDBMISession, "cuda50-gdb-guard");
						break;
					case CUDA_55:
						DVPRINT("Using cuda55-gdb-guard\n");
						MISessionSetGDBPath(GDBMISession, "cuda55-gdb-guard");
						break;
					case CUDA_UNKNOWN:
						DVPRINT("Unsupported driver version.\n");
						DVPRINT("Defaulting to non-gpu mode.\n");
						MISessionSetGDBPath(GDBMISession, "gdb-guard");
						gpu_flag = 0;
						break;
				}
			}
		}
	} else
	{
		MISessionSetGDBPath(GDBMISession, "gdb-guard");
	}

	DBEPERFLOG(SetTimerStart("DbgInitBackend: MISessionStartLocal"));

	if (MISessionStartLocal(GDBMISession, NULL) < 0) {
		EVENT_ERROR(e, DBGERR_DEBUGGER, "Could not start the debugger.");
		return e;
	}
	
	DBEPERFLOG(SetTimerStopAndLog("DbgInitBackend: MISessionStartLocal"));
	
	// this allows dbgsrv to monitor gdb's output fd to find out when we should
	// have libmi service the output.
	*dfd = GDBMISession->out_fd;
	
#ifndef ALPS
	int	progio_pipe[2];
	
	// We might want to forward output to the frontend
	// Create a pipe to use as a control channel
	if (pipe(progio_pipe))
	{
		EVENT_ERROR(e, DBGERR_PIPE, "Could not create a pipe for app stdout redirection.");
		return e;
	}
	// set libmi's target callback end of the pipe
	WRITE_fd = progio_pipe[1];
	dbgsrv_fd = progio_pipe[0];
	*iofd = dbgsrv_fd;
#else
	// ALPS already forwards the stdout to the frontend for us.
	*iofd = -1;
#endif

	Status = DBGSTAT_INITIALISING;

	signal(SIGTERM, Finish);
	signal(SIGHUP, Finish);
	signal(SIGINT, Finish);
	signal(SIGPIPE, Finish);
	signal(SIGQUIT, Finish);

	BP = NewList();

	DBEPERFLOG(SetTimerStart("DbgInitBackend: setup_gdbmi_environment"));
	
	if ((e = setup_gdbmi_environment()) != NULL)
		return e;

	DBEPERFLOG(SetTimerStopAndLog("DbgInitBackend: setup_gdbmi_environment"));
	
	DBEPERFLOG(SetTimerStart("DbgInitBackend: setup_gdbmi_signals"));
	
	if ((e = setup_gdbmi_signals()) != NULL)
		return e;
	
	DBEPERFLOG(SetTimerStopAndLog("DbgInitBackend: setup_gdbmi_signals"));

	return (dbgevent_t *)NULL;
}

/*
** Continue to main and show current stack frame.
*/
static dbgevent_t *
DbgGDBMIStartup(void)
{
	dbgevent_t *	e;
	MICommand *		cmd;
	MIEvent * 		evt;
	dbgbp_t *		bp = NULL;
	int				finish = 1;

	DVPRINT("*** DbgStartup\n");
	
	// setup default entry points
	if ((e = setup_gdbmi_entry_points()) != NULL)
		return e;
		
	// if the entry_BP list is empty, we want to setup the last chance entry
	// breakpoints. If this still fails to set a breakpoint, then we are unable
	// to release the application from the startup barrier. The user will have
	// to intervene manually. We also do not want to call the finish command.
	// The last chance entry points place us directly into main.
	if ( EmptyList(entry_BP) )
	{
		if ((e = setup_gdbmi_lastchance_entry_points()) != NULL)
			return e;
		
		finish = 0;
	}
		
	// we only want to issue a continue if the entry_BP list is not empty.
	if ( !EmptyList(entry_BP))
	{
		// issue a continue command
		cmd = MIExecContinue();
		SendMICommand(GDBMISession, cmd);
		if (!MICommandResultOK(cmd)) {
			EVENT_ERROR(e, DBGERR_DEBUGGER, MICommandResultErrorMessage(cmd));
			MICommandFree(cmd);
			return e;	
		}
		MICommandFree(cmd);

		// set the status to running
		Status = DBGSTAT_RUNNING;
		
		// wait for the asynch command to complete
		while (!DbgGDBMIEventReady())
			MISessionProgress(GDBMISession);
			
		// grab the pending asynch 
		evt = PendingEvent;
		
		// set the state back to default
		AsynchEventWaiting = 0;
		PendingEvent = NULL;
		Status = DBGSTAT_STOPPED;
		
		// ensure we hit the breakpoint
		if (evt->type != MIEventTypeBreakpointHit)
		{
			EVENT_ERROR(e, DBGERR_DEBUGGER, "Failed to hit entry breakpoint.");
			return e;
		}
		
		// ensure we hit a known entry breakpoint
		if ((bp=FindBPByID(entry_BP, evt->bkptno)) == (dbgbp_t *)NULL)
		{
			EVENT_ERROR(e, DBGERR_DEBUGGER, "Hit unknown entry breakpoint.");
			return e;
		}
		
		// Free the event
		MIEventFree(evt);
		
		// remove all of the entry breakpoints. We are done with them.
		if ((e = delete_internal_breakpoints(entry_BP)) != NULL)
			return e;
			
		// We want to set MPIR_debug_gate=1. This is an old MPIR standard.
		cmd = MIGDBSet("lang", "c");
		SendMICommand(GDBMISession, cmd);
		if (!MICommandResultOK(cmd)) 
		{
			MICommandFree(cmd);
			EVENT_ERROR(e, DBGERR_DEBUGGER, "Gdb set lang command failed.");
			return e;
		}
		MICommandFree(cmd);
		
		// This can fail if the symbol doesn't exist. We want to silently fail.
		cmd = MIGDBSet("MPIR_debug_gate=1", NULL);
		SendMICommand(GDBMISession, cmd);
		MICommandFree(cmd);
		
		// set the lang back to auto.
		cmd = MIGDBSet("lang", "auto");
		SendMICommand(GDBMISession, cmd);
		if (!MICommandResultOK(cmd)) 
		{
			MICommandFree(cmd);
			EVENT_ERROR(e, DBGERR_DEBUGGER, "Gdb set lang command failed.");
			return e;
		}
		MICommandFree(cmd);
		
		if (finish)
		{
			// now we want to issue the finish command to get back to main.
			cmd = MIExecFinish();
			SendMICommand(GDBMISession, cmd);
			if (!MICommandResultOK(cmd)) 
			{
				MICommandFree(cmd);
				EVENT_ERROR(e, DBGERR_DEBUGGER, "Finish command failed.");
				return e;
			}
			MICommandFree(cmd);
			
			// set the status to running
			Status = DBGSTAT_RUNNING;
			
			// wait for the asynch command to complete
			while (!DbgGDBMIEventReady())
				MISessionProgress(GDBMISession);
			
			// grab the pending asynch 
			evt = PendingEvent;
		
			// set the state back to default
			AsynchEventWaiting = 0;
			PendingEvent = NULL;
			Status = DBGSTAT_STOPPED;
		
			// ensure we finished the function
			if (evt->type != MIEventTypeFunctionFinished)
			{
				EVENT_ERROR(e, DBGERR_DEBUGGER, "Failed to finish the entry function.");
				return e;
			}
		
			// Free the event
			MIEventFree(evt);
		}
	}
	
	// Initialize UPC, in case the app uses UPC - this will break gpu debugging
	if (!gpu_flag)
	{
		char *cmd_argv[1] = { "upc-init" };
		cmd = CLIBypass(1, cmd_argv);
		SendMICommand(GDBMISession, cmd);
		MICommandFree(cmd);
	}

	// call the showframe function with unlimited frame count and return
	return DbgGDBMIShowFrame(0);
}

/*
** LibMI consumer call. This will have the libmi progress its session
** and perform any necessary callbacks.
*/
static int
DbgGDBMIConsume(int fd)
{
	int		error = 0;

	/*
	** We let MI process event callbacks (it's going to call
	** our event_callback()).
	** If this fails, or if the out_fd is closed then we should inject
	** an error event to recover.
	*/
	if (MISessionProgress(GDBMISession) == -1)
	{
		DVPRINT("DbgGDBMIConsume: MISessionProgress returned -1!\n");
		++error;
	}
	
	if (GDBMISession->out_fd == -1) 
	{
		DVPRINT("DbgGDBMIConsume: sess->out_fd = -1\n");
		++error;
	}
	
	if (error)
	{
		// Trigger the AsynchEventWaiting condition. WaitEvent will detect that
		// MIErrorNum is set and will return an error event.
		AsynchEventWaiting = 1;
		PendingEvent = NULL;
		
		return 0;
	}
	
	return 1;
}

/*
** The main Server loop of dbgsrv.c calls this function to check whether we 
** are ready to process the registered asynch event or not.
*/
static int
DbgGDBMIEventReady(void)
{
	return AsynchEventWaiting; /* Modified by event_callback(). */
}

/*
** Set breakpoint at specified line.
*/
static dbgevent_t *
DbgGDBMISetLineBreak(char *file, int lno)
{
	dbgevent_t *	event;
	MICommand *		cmd;
	char *			where;
	char *			opt2;
	int				tmp=lno, len=0;

	DVPRINT("*** DbgSetLineBreak\n");

	// determine digit length of integer
	do {
		++len;
	} while (tmp /= 10);

	if (file == NULL || *file == '\0')
	{
		// 1 for null term
		++len;
		where = Alloc(len);
		snprintf(where, len, "%d", lno);
	} else
	{
		// strlen of file plus null term and :
		len += strlen(file) + 2;
		where = Alloc(len);
		snprintf(where, len, "%s:%d", file, lno);
	}

	cmd = MIBreakInsert(0, 0, NULL, 0, where, 0);
	SendMICommand(GDBMISession, cmd);

	if (!MICommandResultOK(cmd)) 
	{
		EVENT_ERROR(event, DBGERR_DEBUGGER, MICommandResultErrorMessage(cmd));
		MICommandFree(cmd);
		Free(where);
		
		// If this error's out, try using the basename of the file instead
		// XXX: This will not work properly if there are two source files in the
		//      application with the same name. The full path based on the source
		//      tree should be used in that case.
		if (file != NULL && *file != '\0')
		{
			opt2 = basename(file);
			where = Alloc(len);
			snprintf(where, len, "%s:%d", opt2, lno);
			
			cmd = MIBreakInsert(0, 0, NULL, 0, where, 0);
			SendMICommand(GDBMISession, cmd);
			
			if (!MICommandResultOK(cmd)) 
			{
				MICommandFree(cmd);
				Free(where);
				// return the original error event
				return event;
			}
			
			// This worked, so free the error event and continue on with the
			// original code
			FreeEvent(event);
		} else
		{
			return event;
		}
	}
	
	event = CheckBreak(cmd);
	
	if (event == NULL) 
	{
		EVENT_ERROR(event, DBGERR_DEBUGGER, "Cannot create internal breakpoint.");
	}
	
	Free(where);
	MICommandFree(cmd);
	
	return event;
}

/*
** Set breakpoint at start of specified function.
*/
static dbgevent_t *
DbgGDBMISetFuncBreak(char *file, char *func)
{
	dbgevent_t *	event;
	MICommand *		cmd;
	char *			where;
	char *			opt2;
	int				len = 0;

	DVPRINT("*** DbgSetFuncBreak\n");

	if (file == NULL || *file == '\0')
	{
		where = strdup(func);
	} else
	{
		len = strlen(file) + strlen(func) + 2; // 2 for null term and :
		where = Alloc(len);
		snprintf(where, len, "%s:%s", file, func);
	}
	
	cmd = MIBreakInsert(0, 0, NULL, 0, where, 0);
	SendMICommand(GDBMISession, cmd);

	if (!MICommandResultOK(cmd)) 
	{
		EVENT_ERROR(event, DBGERR_DEBUGGER, MICommandResultErrorMessage(cmd));
		MICommandFree(cmd);
		Free(where);
		
		// If this error's out, try using the basename of the file instead
		// XXX: This will not work properly if there are two source files in the
		//      application with the same name. The full path based on the source
		//      tree should be used in that case.
		if (file != NULL && *file != '\0')
		{
			opt2 = basename(file);
			where = Alloc(len);
			snprintf(where, len, "%s:%s", opt2, func);
			
			cmd = MIBreakInsert(0, 0, NULL, 0, where, 0);
			SendMICommand(GDBMISession, cmd);
			
			if (!MICommandResultOK(cmd)) 
			{
				MICommandFree(cmd);
				Free(where);
				// return the original error event
				return event;
			}
			
			// This worked, so free the error event and continue on with the
			// original code
			FreeEvent(event);
		} else
		{
			return event;
		}
	}
	
	event = CheckBreak(cmd);
	
	if (event == NULL) {
		EVENT_ERROR(event, DBGERR_DEBUGGER, "Cannot create internal breakpoint.");
	}
	
	Free(where);
	MICommandFree(cmd);
	
	return event;
}

/*
** Set watchpoint at specified expression.
*/
static dbgevent_t *
DbgGDBMISetWatch(int type, char *expr)
{
	dbgevent_t *	event;
	MICommand *		cmd;
	dbgbp_type		bp_type = (dbgbp_type)type;
	MIBreakpoint *	bpt;
	MIList *		bpts;
	int				id;

	DVPRINT("*** DbgSetWatch\n");
	
	// set the command based on the type
	switch (bp_type)
	{
		case DBGBP_WATCH_TYPE:
			cmd = MIBreakWatch(expr, 0, 0);
			break;
		case DBGBP_AWATCH_TYPE:
			cmd = MIBreakWatch(expr, 1, 0);
			break;
		case DBGBP_RWATCH_TYPE:
			cmd = MIBreakWatch(expr, 0, 1);
			break;
		default:
			EVENT_ERROR(event, DBGERR_NOSYM, expr);
			return event;
	}
	
	SendMICommand(GDBMISession, cmd);

	if (!MICommandResultOK(cmd)) {
		EVENT_ERROR(event, DBGERR_DEBUGGER, MICommandResultErrorMessage(cmd));
		MICommandFree(cmd);
		return event;
	}
	
	/*
	** The MIBreakWatch command returns a limited amount of information when
	** inserting the watchpoint. Much less info is returned than the MIBreakInsert
	** command. We need to first process the returned info ourselves to get the
	** breakpoint id, and then call MIBreakInfo with this id.
	*/

	bpts = MIBreakpointGetBreakInsertInfo(cmd);

	if (bpts == NULL) {
		EVENT_ERROR(event, DBGERR_DEBUGGER, "Cannot create internal breakpoint.");
		MICommandFree(cmd);
		return event;
	}
	
	MIListSet(bpts);
	bpt = (MIBreakpoint *)MIListGet(bpts);
	
	id = bpt->number;
	
	MIListFree(bpts, MIBreakpointFree);
	MICommandFree(cmd);
	
	// Now send the MIBreakInfo command
	cmd = MIBreakInfo(id);
	
	SendMICommand(GDBMISession, cmd);
	
	if (!MICommandResultOK(cmd)) {
		EVENT_ERROR(event, DBGERR_DEBUGGER, MICommandResultErrorMessage(cmd));
		MICommandFree(cmd);
		return event;
	}
	
	// Now check the breakpoint. It has all the info we need.

	event = CheckBreak(cmd);
	
	if (event == NULL)
	{
		EVENT_ERROR(event, DBGERR_DEBUGGER, "Cannot create internal breakpoint.");
	}
	
	MICommandFree(cmd);
	
	return event;
}

/*
** Delete a breakpoint.
*/
static dbgevent_t *
DbgGDBMIDeleteBreak(int bid)
{
	dbgevent_t *	e;
	MICommand *		cmd;
	dbgbp_t *		bp;
	int				bpid;

	DVPRINT("*** DbgDeleteBreak\n");
	
	bp = FindBPByID(BP, bid);
	
	if ( bp == NULL)
	{
		EVENT_ERROR(e, DBGERR_NOBP, "Invalid breakpoint id.");
		return e;
	}

	bpid = bid;

	cmd = MIBreakDelete(1, &bpid);
	SendMICommand(GDBMISession, cmd);

	if (!MICommandResultOK(cmd)) 
	{
		MICommandFree(cmd);
		EVENT_ERROR(e, DBGERR_DEBUGGER, "Delete breakpoint failed.");
		return e;	
	}
	
	RemoveBP(BP, bid);

	e = NewEvent(DBGEV_OK);
	
	MICommandFree(cmd);

	return e;
}

/*
** Disable a breakpoint.
*/
static dbgevent_t *
DbgGDBMIDisableBreak(int bid)
{
	dbgevent_t *	e;
	MICommand *		cmd;
	dbgbp_t *		bp;
	int				bpid;

	DVPRINT("*** DbgDisableBreak\n");

	bp = FindBPByID(BP, bid);
	
	if ( bp == NULL)
	{
		EVENT_ERROR(e, DBGERR_NOBP, "Invalid breakpoint id.");
		return e;
	}

	bpid = bid;

	cmd = MIBreakDisable(1, &bpid);
	SendMICommand(GDBMISession, cmd);

	if (!MICommandResultOK(cmd)) 
	{
		MICommandFree(cmd);
		EVENT_ERROR(e, DBGERR_DEBUGGER, "Disable breakpoint failed.");
		return e;	
	}
	
	bp->bp_enabled = 0;

	e = NewEvent(DBGEV_OK);
	
	MICommandFree(cmd);

	return e;
}

/*
** Enable a breakpoint.
*/
static dbgevent_t *
DbgGDBMIEnableBreak(int bid, int once, int del)
{
	dbgevent_t *	e;
	MICommand *		cmd;
	char *			cmd_args[3];
	char 			cmd_bpid[30];
	dbgbp_t *		bp;
	int				bpid;

	DVPRINT("*** DbgEnableBreak\n");

	bp = FindBPByID(BP, bid);
	
	if ( bp == NULL )
	{
		EVENT_ERROR(e, DBGERR_NOBP, "Invalid breakpoint id.");
		return e;
	}
	
	// cannot set once and del at the same time
	if ( once && del )
	{
		EVENT_ERROR(e, DBGERR_NOBP, "Invalid arguments.");
		return e;
	}

	if (once)
	{
		cmd_args[0] = "enable";
		cmd_args[1] = "once";
		snprintf(cmd_bpid, 30, "%d", bid);
		cmd_args[2] = cmd_bpid;
		cmd = CLIBypass(3, cmd_args);
	} else if (del)
	{
		cmd_args[0] = "enable";
		cmd_args[1] = "delete";
		snprintf(cmd_bpid, 30, "%d", bid);
		cmd_args[2] = cmd_bpid;
		cmd = CLIBypass(3, cmd_args);
	} else
	{
		// this is needed...
		bpid = bid;
		cmd = MIBreakEnable(1, &bpid);
	}
	
	SendMICommand(GDBMISession, cmd);

	if (!MICommandResultOK(cmd)) 
	{
		MICommandFree(cmd);
		EVENT_ERROR(e, DBGERR_DEBUGGER, "Enable breakpoint failed.");
		return e;	
	}
	
	bp->bp_enabled = 1;
	
	if (once)
		bp->bp_disp = DBGBP_DISABLE_DISP;
	else if (del)
		bp->bp_disp = DBGBP_DELETE_DISP;

	e = NewEvent(DBGEV_OK);
	
	MICommandFree(cmd);

	return e;
}

/*
** Return id of last breakpoint to be hit.
*/
static dbgevent_t *
DbgGDBMILastBreak(void)
{
	dbgevent_t *	e;

	if ( CurrBP == (dbgbp_t *)NULL )
	{
		EVENT_ERROR(e, DBGERR_NOBP, "No breakpoints registered.");
		return e;
	}

	e = NewEvent(DBGEV_BPINFO);
	e->ev_bp = CurrBP;

	return e;
}

/*
** List source code.
**
** GDB does not have a MI command for this; we use the CLI command.
*/
static dbgevent_t *
DbgGDBMIList(char *str)
{
	dbgevent_t *	e;
	MICommand *		cmd;
	MIList *		lines;
	CLILineInfo *	line;
	dbglist_t *		this;
	dbglist_t *		list_ptr = NULL;
	dbglist_t *		list = NULL;

	DVPRINT("*** DbgList\n");
	
	// We currently turn off opening of source files to avoid performance 
	// penalties of accessing the cross mounted file system. But the user 
	// explicitly wanted to list the source and our frontend failed, so we need
	// to turn it back on and take the penalty. We turn this feature back off
	// when we are finished. The frontend handles most list commands to avoid
	// these issues, but sometimes it can fail or for the attach case it couldn't
	// find the a.out. 
	// TODO: It may be better to do it on the backend rather than the frontend
	// if there are hundreds of thousands of ranks and they are all at different
	// locations.
	
	cmd = MIGDBSet("source open", "on");
	SendMICommand(GDBMISession, cmd);
	if (!MICommandResultOK(cmd)) 
	{
		MICommandFree(cmd);
		EVENT_ERROR(e, DBGERR_DEBUGGER, "Gdb set source open command failed.");
		return e;
	}
	MICommandFree(cmd);

	cmd = CLIList(str);
	SendMICommand(GDBMISession, cmd);
	if (!MICommandResultOK(cmd)) {
		EVENT_ERROR(e, DBGERR_DEBUGGER, MICommandResultErrorMessage(cmd));
		MICommandFree(cmd);
		return e;
	}

	lines = CLIGetListInfo(cmd);
	
	// Convert the MI list of CLILineInfo into a dbglist_t
	for (MIListSet(lines); (line = (CLILineInfo *)MIListGet(lines)) != NULL; ) 
	{
		this = (dbglist_t *)Alloc(sizeof(dbglist_t));
		this->list_lno = line->line_no;
		this->list_line = strdup(line->line);
		this->list_next = (dbglist_t *)NULL;

		if (list == NULL)
		{
			list = this;
			list_ptr = this;
		} else
		{
			list_ptr->list_next = this;
			list_ptr = this;
		}
	}
	
	MIListFree(lines, CLILineInfoFree);
	MICommandFree(cmd);
	
	// do not open source files
	cmd = MIGDBSet("source open", "off");
	SendMICommand(GDBMISession, cmd);
	if (!MICommandResultOK(cmd)) 
	{
		MICommandFree(cmd);
		EVENT_ERROR(e, DBGERR_DEBUGGER, "Gdb set source open command failed.");
		return e;
	}
	MICommandFree(cmd);

	e = NewEvent(DBGEV_LIST);
	e->ev_list = list;

	return e;
}

/*
** Get source files of inferior.
*/
static dbgevent_t *
DbgGDBMIGetSources(void)
{
	dbgevent_t *	e;
	MICommand *		cmd;
	MIList *		sources;
	MISources *		source;
	dbgsources_t *	this;
	dbgsources_t *	sources_ptr = NULL;
	dbgsources_t *	head = NULL;

	DVPRINT("*** DbgGetSources\n");

	cmd = MIFileListExecSourceFiles();
	SendMICommand(GDBMISession, cmd);
	if (!MICommandResultOK(cmd)) {
		EVENT_ERROR(e, DBGERR_DEBUGGER, MICommandResultErrorMessage(cmd));
		MICommandFree(cmd);
		return e;
	}

	sources = MIGetFileListExecSourceFilesInfo(cmd);
	
	// Convert the MI list of MISources into a dbgsources_t
	for (MIListSet(sources); (source = (MISources *)MIListGet(sources)) != NULL; ) 
	{
		if (source->fullname == NULL)
			continue;
	
		this = (dbgsources_t *)Alloc(sizeof(dbgsources_t));
		
		// Ensure that this source file actually exists
		this->sources_file = realpath(source->fullname, NULL);
		if (this->sources_file == NULL)
		{
			Free(this);
			continue;
		}
		
		this->sources_next = (dbgsources_t *)NULL;

		if (head == NULL)
		{
			head = this;
			sources_ptr = this;
		} else
		{
			sources_ptr->sources_next = this;
			sources_ptr = this;
		}
	}
	
	MIListFree(sources, MISourcesFree);
	MICommandFree(cmd);

	e = NewEvent(DBGEV_SOURCES);
	e->ev_sources = head;

	return e;
}

/*
** Start/continue executing program. If we are starting,
** then we must plant a breakpoint at main() so that we 
** can find out the process id of the program.
*/
static dbgevent_t *
DbgGDBMIGo(void)
{
	dbgevent_t *	e;
	MICommand *		cmd;

	DVPRINT("*** DbgGo()\n");
	
	if (Status == DBGSTAT_EXIT)
	{
		EVENT_ERROR(e, DBGERR_AT_EXIT, "");
		return e;
	}

	cmd = MIExecContinue();

	SendMICommand(GDBMISession, cmd);
	
	if (!MICommandResultOK(cmd)) {
		EVENT_ERROR(e, DBGERR_DEBUGGER, MICommandResultErrorMessage(cmd));
		MICommandFree(cmd);
		return e;	
	}

	MICommandFree(cmd);

	AsyncCheck(WaitEvent, (dbgevent_t *)NULL);

	Status = DBGSTAT_RUNNING;
	
	e = NewEvent(DBGEV_OK);

	return e;
}

/*
** Continue executing program until the current function returns.
*/
static dbgevent_t *
DbgGDBMIFinishFunc(void)
{
	dbgevent_t *	e;
	MICommand *		cmd;

	DVPRINT("*** DbgFinishFunc()\n");
	
	if (Status == DBGSTAT_EXIT)
	{
		EVENT_ERROR(e, DBGERR_AT_EXIT, "");
		return e;
	}

	cmd = MIExecFinish();

	SendMICommand(GDBMISession, cmd);
	
	if (!MICommandResultOK(cmd)) {
		EVENT_ERROR(e, DBGERR_DEBUGGER, MICommandResultErrorMessage(cmd));
		MICommandFree(cmd);
		return e;	
	}

	MICommandFree(cmd);

	AsyncCheck(WaitEvent, (dbgevent_t *)NULL);

	Status = DBGSTAT_RUNNING;
	
	e = NewEvent(DBGEV_OK);

	return e;
}

/*
** Set run arguments.
*/
static dbgevent_t *
DbgGDBMISetArgs(char *arg)
{
	DVPRINT("*** DbgSetArgs\n");

	if ( arg != NULL && *arg != '\0' )
	{
		if ( RunArgs != NULL )
			Free(RunArgs);

		RunArgs = strdup(arg);
	}

	return NewEvent(DBGEV_OK);
}

/*
** Execute count statements. If in == 0, do not enter
** function calls.
*/
static dbgevent_t *
DbgGDBMIStep(int count, int in)
{
	dbgevent_t *	e;
	MICommand *		cmd;

	DVPRINT("*** DbgStep()\n");
	
	if (Status == DBGSTAT_EXIT)
	{
		EVENT_ERROR(e, DBGERR_AT_EXIT, "");
		return e;
	}

	if ( !in )
		cmd = count > 1 ? MIExecNext(count) : MIExecNext(1);
	else
		cmd = count > 1 ? MIExecStep(count) : MIExecStep(1);

	SendMICommand(GDBMISession, cmd);

	if (!MICommandResultOK(cmd)) {
		EVENT_ERROR(e, DBGERR_DEBUGGER, MICommandResultErrorMessage(cmd));
		MICommandFree(cmd);
		return e;
	}

	MICommandFree(cmd);

	AsyncCheck(WaitEvent, (dbgevent_t *)NULL);

	Status = DBGSTAT_RUNNING;
	
	e = NewEvent(DBGEV_OK);

	return e;
}

/*
** Kill the currently running process.
*/
static dbgevent_t *
DbgGDBMIKill(void)
{
	dbgevent_t *	e;

	DVPRINT("*** DbgKill\n");

#ifndef ALPS	
	// Note that if we are on a cray platform, we don't want to send the kill
	// command. The ALPS interface will cleanup for us on the front-end. Killing
	// the application process causes an "unclean" exit in the viewpoint of alps.

	MICommand *cmd = MICommandNew("kill", MIResultRecordDONE);

	SendMICommand(GDBMISession, cmd);

	if (!MICommandResultOK(cmd)) {
		EVENT_ERROR(e, DBGERR_DEBUGGER, MICommandResultErrorMessage(cmd));
		MICommandFree(cmd);
		Status = DBGSTAT_WAITING;
		return e;	
	}

	Pid = 0;
	MICommandFree(cmd);
#endif

	e = NewEvent(DBGEV_OK);
	Status = DBGSTAT_WAITING;

	return e;
}

/*
** Move up or down count stack frames.
*/
static dbgevent_t *
DbgGDBMIMoveFrame(int count, int up)
{
	dbgevent_t *		e;
	dbgframe_t *		result = NULL;
	
	int			cur_level;
	int			max_depth;
	int			frame_number;
	
	MICommand *		cmd;
	MIList *		mi_frame_list;
	MIFrame *		mi_frame;

	DVPRINT("*** DbgMoveFrame\n");

	// first get the level of the current frame
	cmd = MIStackInfoFrame();
	SendMICommand(GDBMISession, cmd);

	if (!MICommandResultOK(cmd)) {
		EVENT_ERROR(e, DBGERR_DEBUGGER, MICommandResultErrorMessage(cmd));
		MICommandFree(cmd);
		return e;
	}
	
	// process the result
	mi_frame_list = MIGetFrameInfo(cmd);

	if (mi_frame_list == NULL) {
		EVENT_ERROR(e, DBGERR_NOSTACK, MICommandResultErrorMessage(cmd));
		MICommandFree(cmd);
		return e;
	}
	
	// get the frame
	MIListSet(mi_frame_list);
	if ((mi_frame = (MIFrame *)MIListGet(mi_frame_list)) == NULL)
	{
		MIListFree(mi_frame_list, NULL);
		EVENT_ERROR(e, DBGERR_NOSTACK, MICommandResultErrorMessage(cmd));
		MICommandFree(cmd);
		return e;
	}
	// save the level of the current frame
	cur_level = mi_frame->level;
	
	// free the MIFrame list
	MIListFree(mi_frame_list, MIFrameFree);
	MICommandFree(cmd);
	
	// now we need to get the depth of the stack
	cmd = MIStackInfoDepth();
	SendMICommand(GDBMISession, cmd);
	
	if (!MICommandResultOK(cmd)) {
		EVENT_ERROR(e, DBGERR_DEBUGGER, MICommandResultErrorMessage(cmd));
		MICommandFree(cmd);
		return e;
	}
	
	// process the result
	if ((max_depth = MIGetStackInfoDepth(cmd)) < 0)
	{
		EVENT_ERROR(e, DBGERR_NOSTACK, MICommandResultErrorMessage(cmd));
		MICommandFree(cmd);
		return e;
	}
	MICommandFree(cmd);
	
	// the frame we want to select is based on an offset of the current
	// frame. We must ensure that this is between a min of 0 and a max of
	// max_depth
	switch (up)
	{
		case 0:
			if ((frame_number = cur_level - count) < 0)
			{
				EVENT_ERROR(e, DBGERR_LASTFRAME, "");
				return e;
			}
			break;
			
		default:
			if ((frame_number = cur_level + count) >= max_depth)
			{
				EVENT_ERROR(e, DBGERR_FIRSTFRAME, "");
				return e;
			}
	}
	
	// now we know which frame we should select
	cmd = MIStackSelectFrame(frame_number);
	SendMICommand(GDBMISession, cmd);
	MICommandFree(cmd);
	
	// now query information about the frame we changed to and return it
	cmd = MIStackInfoFrame();
	SendMICommand(GDBMISession, cmd);

	if (!MICommandResultOK(cmd)) {
		EVENT_ERROR(e, DBGERR_DEBUGGER, MICommandResultErrorMessage(cmd));
		MICommandFree(cmd);
		return e;
	}
	
	// process the result
	mi_frame_list = MIGetFrameInfo(cmd);

	if (mi_frame_list == NULL) {
		EVENT_ERROR(e, DBGERR_NOSTACK, MICommandResultErrorMessage(cmd));
		MICommandFree(cmd);
		return e;
	}
	
	// get the frame
	MIListSet(mi_frame_list);
	if ((mi_frame = (MIFrame *)MIListGet(mi_frame_list)) == NULL)
	{
		MIListFree(mi_frame_list, NULL);
		EVENT_ERROR(e, DBGERR_NOSTACK, MICommandResultErrorMessage(cmd));
		MICommandFree(cmd);
		return e;
	}
	MICommandFree(cmd);
	
	// parse the frame into a dbgframe_t object
	result = (dbgframe_t *)Alloc(sizeof(dbgframe_t));

	// set the next to null and the level to its level
	result->frame_next = (dbgframe_t *)NULL;
	result->frame_level = mi_frame->level;

	// set frame address
	if (mi_frame->addr != NULL)
	{
		DVPRINT("frame_addr: %s\n", mi_frame->addr);
		result->frame_loc.loc_addr = strdup(mi_frame->addr);
	} else
	{
		DVPRINT("frame_addr: NULL\n");
		result->frame_loc.loc_addr = (char *)NULL;
	}
		
	// set frame function
	if (mi_frame->func != NULL)
	{
		DVPRINT("frame_func: %s\n", mi_frame->func);
		result->frame_loc.loc_func = strdup(mi_frame->func);
	} else
	{
		DVPRINT("frame_func: NULL\n");
		result->frame_loc.loc_func = (char *)NULL;
	}
		
	// set frame file
	if (mi_frame->file != NULL)
	{
		DVPRINT("frame_file: %s\n", mi_frame->file);
		result->frame_loc.loc_file = strdup(mi_frame->file);
	} else
	{
		DVPRINT("frame_file: NULL\n");
		result->frame_loc.loc_file = (char *)NULL;
	}
	
	// set frame line
	DVPRINT("frame_line: %d\n", mi_frame->line);
	result->frame_loc.loc_line = mi_frame->line;
	
	// free the MIFrame list
	MIListFree(mi_frame_list, MIFrameFree);
	
	// create the event with the frame information
	e = NewEvent(DBGEV_FRAME);
	e->ev_frame = result;

	return e;
}

/*
** Show current stack frames.
** If count is not zero then return only that many frames. If count is zero then
** return an unlimited number of frames. If count is negative return the
** outermost frames.
*/
static dbgevent_t *
DbgGDBMIShowFrame(int count)
{
	dbgevent_t *	e;
	dbgframe_t *	result = NULL;
	dbgframe_t *	new_f = NULL;
	dbgframe_t *	old_f = NULL;
	
	MICommand * cmd;
	MIList * frames;
	MIFrame * frame;

	DVPRINT("*** DbgShowFrame\n");
	
	// We need to do different things depending on the value of count
	if (count < 0)
	{
		// if count is negative, we want to ask for the outermost frames
		// We need to figure out the depth of the stack and ask for depth-count
		// to depth frames.
		int depth;
		
		// Get the depth of the stack
		cmd = MIStackInfoDepth();
	
		SendMICommand(GDBMISession, cmd);

		if (!MICommandResultOK(cmd)) {
			EVENT_ERROR(e, DBGERR_DEBUGGER, MICommandResultErrorMessage(cmd));
			MICommandFree(cmd);
			return e;
		}
		
		if ((depth = MIGetStackInfoDepth(cmd)) == -1)
		{
			MICommandFree(cmd);
			EVENT_ERROR(e, DBGERR_NOSTACK, "");
			return e;
		}
		MICommandFree(cmd);
		
		// Ask for the outermost count frames
		// Remember that count is a negative number, so adding is really 
		// subtracting a positive. 
		// Note that depth is *not* zero offset, so we need to subtract one from
		// it when asking for the high. We don't need to fix for the low.
		cmd = MIStackListFrames(depth+count, depth-1);
	} else if (count > 0)
	{
		// if count is positive, we want to ask for the innermost count frames
		// Fix this to be zero offset by subtracting one.
		cmd = MIStackListFrames(0, count-1);
	} else
	{
		// if count is zero, ask for all the frames
		cmd = MIStackListAllFrames();
	}
	
	// Send the command that was created by the above logic.
	SendMICommand(GDBMISession, cmd);

	if (!MICommandResultOK(cmd)) {
		EVENT_ERROR(e, DBGERR_DEBUGGER, MICommandResultErrorMessage(cmd));
		MICommandFree(cmd);
		return e;
	}

	if ((frames = MIGetStackListFramesInfo(cmd))== NULL) 
	{
		MICommandFree(cmd);
		EVENT_ERROR(e, DBGERR_NOSTACK, "");
		return e;
	}
	MICommandFree(cmd);

	// parse over the frame list
	// increment the frames count each time through
	for (MIListSet(frames); (frame = (MIFrame *)MIListGet(frames)) != NULL; )
	{		
		// create the new frame
		new_f = (dbgframe_t *)Alloc(sizeof(dbgframe_t));

		// first time through we want to set the root frame as the result
		if (result == NULL)
			result = new_f;

		// if this is not the first time through, set the old frames next frame pointer to
		// this new frame
		if (old_f != NULL)
			old_f->frame_next = new_f;

		// set the next to null and the level to its level
		new_f->frame_next = (dbgframe_t *)NULL;
		new_f->frame_level = frame->level;

		// set frame address
		if (frame->addr != NULL)
		{
			DVPRINT("frame_addr: %s\n", frame->addr);
			new_f->frame_loc.loc_addr = strdup(frame->addr);
		} else
		{
			DVPRINT("frame_addr: NULL\n");
			new_f->frame_loc.loc_addr = (char *)NULL;
		}
		
		// set frame function
		if (frame->func != NULL)
		{
			DVPRINT("frame_func: %s\n", frame->func);
			new_f->frame_loc.loc_func = strdup(frame->func);
		} else
		{
			DVPRINT("frame_func: NULL\n");
			new_f->frame_loc.loc_func = (char *)NULL;
		}
		
		// set frame file
		if (frame->file != NULL)
		{
			DVPRINT("frame_file: %s\n", frame->file);
			new_f->frame_loc.loc_file = strdup(frame->file);
		} else
		{
			DVPRINT("frame_file: NULL\n");
			new_f->frame_loc.loc_file = (char *)NULL;
		}
		
		// set frame line
		DVPRINT("frame_line: %d\n", frame->line);
		new_f->frame_loc.loc_line = frame->line;

		old_f = new_f;
	}

	// create the event with the frame information
	e = NewEvent(DBGEV_FRAME);
	e->ev_frame = result;
	
	// free the MIFrame list
	MIListFree(frames, MIFrameFree);

	return e;
}

/*
** Get selected stack frame. This will return information about the current frame
** that is selected.
*/
static dbgevent_t *
DbgGDBMIGetFrame(void)
{
	dbgevent_t *	e;
	dbgframe_t *	result = NULL;
	
	MICommand * cmd;
	MIList * 	mi_frame_list;
	MIFrame *	mi_frame;

	DVPRINT("*** DbgShowFrame\n");
	
	// Get the selected frame
	cmd = MIStackInfoFrame();
	SendMICommand(GDBMISession, cmd);

	if (!MICommandResultOK(cmd)) {
		EVENT_ERROR(e, DBGERR_DEBUGGER, MICommandResultErrorMessage(cmd));
		MICommandFree(cmd);
		return e;
	}
	
	// process the result
	mi_frame_list = MIGetFrameInfo(cmd);

	if (mi_frame_list == NULL) {
		EVENT_ERROR(e, DBGERR_NOSTACK, MICommandResultErrorMessage(cmd));
		MICommandFree(cmd);
		return e;
	}
	
	// get the frame
	MIListSet(mi_frame_list);
	if ((mi_frame = (MIFrame *)MIListGet(mi_frame_list)) == NULL)
	{
		MIListFree(mi_frame_list, NULL);
		EVENT_ERROR(e, DBGERR_NOSTACK, MICommandResultErrorMessage(cmd));
		MICommandFree(cmd);
		return e;
	}
	
	// create the result
	result = (dbgframe_t *)Alloc(sizeof(dbgframe_t));
	memset((void *)result, 0, sizeof(dbgframe_t));
	
	// set frame level
	result->frame_level = mi_frame->level;
	
	// set frame address
	if (mi_frame->addr != NULL)
	{
		DVPRINT("frame_addr: %s\n", mi_frame->addr);
		result->frame_loc.loc_addr = strdup(mi_frame->addr);
	}
		
	// set frame function
	if (mi_frame->func != NULL)
	{
		DVPRINT("frame_func: %s\n", mi_frame->func);
		result->frame_loc.loc_func = strdup(mi_frame->func);
	}
		
	// set frame file
	if (mi_frame->file != NULL)
	{
		DVPRINT("frame_file: %s\n", mi_frame->file);
		result->frame_loc.loc_file = strdup(mi_frame->file);
	}
		
	// set frame line
	DVPRINT("frame_line: %d\n", mi_frame->line);
	result->frame_loc.loc_line = mi_frame->line;
	
	// free the MIFrame list
	MIListFree(mi_frame_list, MIFrameFree);
	MICommandFree(cmd);
	
	// create the event with the frame information
	e = NewEvent(DBGEV_FRAME);
	e->ev_frame = result;

	return e;
}

/*
** Set contents of variable var.
**
** This function is still messed up. We should be implementing this through
** -data-write-memory-bytes if we are going to modify program data using aif
** types. Right now we just convert to string and let gdb handle it. I think
** this is a better implementation (andrewg).
*/
static dbgevent_t *
DbgGDBMISetVar(char *var, AIF *a)
{
	dbgevent_t *	e;
	MICommand *		cmd;
	char *			data;
	char *			expr;
	int				len;
	
	DVPRINT("*** DbgSetVar\n");
	
	if ( a == NULL)
	{
		EVENT_ERROR(e, DBGERR_SETVAR, "Invalid input. AIF is null.");
		return e;
	}
	
	if ( AIF_LEN(a) <= 0)
	{
		EVENT_ERROR(e, DBGERR_SETVAR, "Attempt to set var with data of null length.");
		return e;
	}
	
	// get type info about var to make sure it exists
	if (((e = DbgGDBMIGetType(var, 0))->ev_event) != DBGEV_TYPE)
		return e;
	FreeEvent(e);
	
	// convert the aif type to string form
	if (AIFToStr(&data, 0, a))
	{
		EVENT_ERROR(e, DBGERR_SETVAR, "Could not convert AIF data to string.");
		return e;
	}
	
	// create the assignment string
	len = strlen(var) + strlen(data) + 2;
	expr = Alloc(len);
	snprintf(expr, len, "%s=%s", var, data);
	
	DVPRINT("DbgSetVar: expr is %s\n", expr);
	
	// set the var to the val of the aif var
	cmd = MIGDBSet(expr, NULL);
	SendMICommand(GDBMISession, cmd);
	
	if (!MICommandResultOK(cmd))
	{
		EVENT_ERROR(e, DBGERR_SETVAR, MICommandResultErrorMessage(cmd));
		MICommandFree(cmd);
		return e;
	}
	MICommandFree(cmd);
	Free(expr);
	
	return NewEvent(DBGEV_OK);
}

/*
** Find type of variable.
** If isType is true, then we are finding type information for a type.
*/
static dbgevent_t *
DbgGDBMIGetType(char *var, int isType)
{
	dbgevent_t *	e;
	MICommand *	cmd;
	char *		type;

	DVPRINT("*** DbgGetType(%s, %d)\n", var, isType);

	if (isType)
		cmd = CLIPtype(var);
	else
		cmd = CLIWhatis(var);
	
	SendMICommand(GDBMISession, cmd);
	if (!MICommandResultOK(cmd))
	{
		EVENT_ERROR(e, DBGERR_NOSYM, var);
		MICommandFree(cmd);
		return e;
	}
	
	if (isType)
		type = CLIGetPtypeInfo(cmd);
	else
		type = CLIGetWhatisInfo(cmd);
	
	if (type == NULL)
	{
		EVENT_ERROR(e, DBGERR_NOSYM, var);
		MICommandFree(cmd);
		return e;
	}
	MICommandFree(cmd);

	e = NewEvent(DBGEV_TYPE);
	e->ev_type = strdup(type);

	Free(type);

	return e;
}

/*
** List variables in the current stack frame
*/
static dbgevent_t *
DbgGDBMIGetVars(char *file)
{
	dbgevent_t *	e;
	dbgvars_t *	result = NULL;
	dbgvars_t *	cur_var = NULL;
	dbgvars_t *	new_var = NULL;
	
	MICommand *	cmd;
	MIList *	vars;
	MIArg *	 var;

	DVPRINT("*** DbgGetVars\n");

	cmd = MIStackListVariables(SIMPLE_VALUES);
	SendMICommand(GDBMISession, cmd);

	if (!MICommandResultOK(cmd)) {
		EVENT_ERROR(e, DBGERR_DEBUGGER, MICommandResultErrorMessage(cmd));
		MICommandFree(cmd);
		return e;
	}

	if ((vars = MIGetStackListVariablesInfo(cmd))== NULL)
	{
		EVENT_ERROR(e, DBGERR_DEBUGGER, MICommandResultErrorMessage(cmd));
		MICommandFree(cmd);
		return e;
	}
	MICommandFree(cmd);
	
	// parse over the vars list
	for (MIListSet(vars); (var = (MIArg *)MIListGet(vars)) != NULL; ) 
	{
		// create the new var
		new_var = (dbgvars_t *)Alloc(sizeof(dbgvars_t));
		
		// first time through we want to set the root var as the result
		if (result == NULL)
			result = new_var;
		
		// set the cur_var var_next pointer to the new var if this isn't
		// the first time through
		if (cur_var != NULL)
			cur_var->var_next = new_var;
			
		// set the var_next to null
		new_var->var_next = (dbgvars_t *)NULL;
		
		// set the name
		if (var->name != NULL)
		{
			DVPRINT("var_name: %s\n", var->name);
			new_var->var_name = strdup(var->name);
		} else
		{
			DVPRINT("var_name: NULL\n");
			new_var->var_name = (char *)NULL;
		}
		
		// set the type
		if (var->type != NULL)
		{
			DVPRINT("var_type: %s\n", var->type);
			new_var->var_type = strdup(var->type);
		} else
		{
			DVPRINT("var_type: NULL\n");
			new_var->var_type = NULL;
		}
		
		// set the cur_var pointer to the new_var
		cur_var = new_var;
	}
	
	// create the event with the var information
	e = NewEvent(DBGEV_VARS);
	e->ev_vars = result;
	
	// free the MIArg list
	MIListFree(vars, MIArgFree);
	
	return e;
}

/*
** List local variables relative to a scope
*/
static dbgevent_t *
DbgGDBMIGetScope(char *location)
{
	MICommand * 	cmd;
	MIList *		vars;
	char *			name;
	dbgvars_t *		result = NULL;
	dbgvars_t **	vp = NULL;
	MIList *		bpts;
	MIBreakpoint *	bpt;
	int				bpid;
	char *			bpfunc;
	char *			var_str;
	char *			type;
	dbgevent_t *	e;
	
	DVPRINT("*** DbgGetScope\n");
	
	cmd = CLIInfoScope(location);
	
	SendMICommand(GDBMISession, cmd);
	if (!MICommandResultOK(cmd)) 
	{
		MICommandFree(cmd);
		EVENT_ERROR(e, DBGERR_DEBUGGER, MICommandResultErrorMessage(cmd));
		return e;
	}
	
	// Get the names of the vars
	if ((vars = CLIGetVariablesFromInfoScopeInfo(cmd)) == NULL)
	{
		EVENT_ERROR(e, DBGERR_DEBUGGER, MICommandResultErrorMessage(cmd));
		MICommandFree(cmd);
		return e;
	}
	MICommandFree(cmd);
	
	// Create the dbgvars_t for each of the named variables
	vp = &result;
	for (MIListSet(vars); (name = (char *)MIListGet(vars)) != NULL; ) 
	{
		// create the new var
		*vp = (dbgvars_t *)Alloc(sizeof(dbgvars_t));
		memset(*vp, 0, sizeof(dbgvars_t));
		
		// set the name
		if (name != NULL)
		{
			DVPRINT("var_name: %s\n", name);
			(*vp)->var_name = strdup(name);
		} else
		{
			// We do not want to include null names
			Free(*vp);
			*vp = NULL;
			continue;
		}
		// set the var_next to null
		(*vp)->var_next = (dbgvars_t *)NULL;
		
		// increment the var pointer
		vp = &(*vp)->var_next;
	}
	
	// free the name list
	MIListFree(vars, Free);
	
	// create the event with the var information
	e = NewEvent(DBGEV_VARS);
	e->ev_vars = result;
	
	// Now we need to get the type information based on the scope. First figure
	// out what function the scope referrs to by setting a breakpoint to get
	// the function name.
	
	cmd = MIBreakInsert(0, 0, NULL, 0, location, 0);
	SendMICommand(GDBMISession, cmd);

	if (!MICommandResultOK(cmd)) 
	{
		// free existing event
		FreeEvent(e);
		EVENT_ERROR(e, DBGERR_DEBUGGER, MICommandResultErrorMessage(cmd));
		MICommandFree(cmd);
		return e;
	}
	
	if ((bpts = MIBreakpointGetBreakInsertInfo(cmd)) == NULL)
	{
		// free existing event
		FreeEvent(e);
		// If we can't get the breakpoint info, we error out
		EVENT_ERROR(e, DBGERR_DEBUGGER, "Could not obtain breakpoint information for scope.");
		MICommandFree(cmd);
		return e;
	}
	
	MIListSet(bpts);
	bpt = (MIBreakpoint *)MIListGet(bpts);
	
	bpid = bpt->number;
	
	// Make sure that there is a function, otherwise whatis will fail. We need
	// the function scope in order for this to work
	if (bpt->func == NULL)
	{
		// free existing event
		FreeEvent(e);
		EVENT_ERROR(e, DBGERR_DEBUGGER, "Could not obtain function locale for scope.");
		MICommandFree(cmd);
		return e;
	}
	
	bpfunc = strdup(bpt->func);
	
	MIListFree(bpts, MIBreakpointFree);
	MICommandFree(cmd);
	
	// Delete the breakpoint, we have the information we need
	cmd = MIBreakDelete(1, &bpid);
	SendMICommand(GDBMISession, cmd);
	// We don't care if the delete breakpoint failed. I think this is okay to
	// do since it won't cause anything to break. This assumption might be
	// wrong though.
	MICommandFree(cmd);
	
	// Now loop through the vars and get the type for each name
	for (vp = &result; *vp != NULL; vp = &(*vp)->var_next)
	{
		Asprintf(&var_str, "%s::%s", bpfunc, (*vp)->var_name);
		cmd = CLIWhatis(var_str);
		Free(var_str);
		
		SendMICommand(GDBMISession, cmd);
		if (!MICommandResultOK(cmd))
		{
			// If there was a problem, continue
			MICommandFree(cmd);
			continue;
		}
		
		if ((type = CLIGetWhatisInfo(cmd)) == NULL)
		{
			MICommandFree(cmd);
			continue;
		}
		MICommandFree(cmd);
		
		// I don't know if the strdup is actually necessary, dbgsrv was doing it
		// for the whatis command though.
		(*vp)->var_type = strdup(type);
		Free(type);
	}
	
	return e;
}

/*
** Get debugger status.
*/
static dbgevent_t *
DbgGDBMIStatus(int getLocation)
{
	dbgevent_t *	e;
	dbgevent_t *	async_e;
	dbgstatus_t *	s;

	e = NewEvent(DBGEV_STAT);

	DVPRINT("*** DbgStatus\n");
	
	// Create the new status object
	s = e->ev_status = (dbgstatus_t *)Alloc(sizeof(dbgstatus_t));
	
	// Assign the status
	s->stat = Status;
	
	// If getLocation is 1, we need to populate the location information if its
	// available
	if (getLocation)
	{
		// Switch on the current Status
		switch (Status)
		{
			// These should generate location information
			case DBGSTAT_WAITING:
			case DBGSTAT_STOPPED:
			case DBGSTAT_EXIT:
				// Reuse DbgGDBMIShowFrame to get the bottommost frame
				async_e = DbgGDBMIShowFrame(1);
			
				if (async_e->ev_frame != NULL) 
				{
					s->stat_loc = (dbgloc_t *)Alloc(sizeof(dbgloc_t));
					s->stat_loc->loc_file = (async_e->ev_frame->frame_loc.loc_file == NULL ? NULL : strdup(async_e->ev_frame->frame_loc.loc_file));
					s->stat_loc->loc_func = (async_e->ev_frame->frame_loc.loc_func == NULL ? NULL : strdup(async_e->ev_frame->frame_loc.loc_func));
					s->stat_loc->loc_addr = NULL; // We don't care about the address
					s->stat_loc->loc_line = async_e->ev_frame->frame_loc.loc_line;
				}
			
				FreeEvent(async_e);
				break;
		
			// These do not generate location information
			case DBGSTAT_INITIALISING:
			case DBGSTAT_RUNNING:
			case DBGSTAT_DBGERR:
			case DBGSTAT_INTERR:
				s->stat_loc = NULL;
		}
	} else
	{
		// Location information not requested
		s->stat_loc = NULL;
	}

	return e;
}

/*
** Quit debugger.
*/
static dbgevent_t *
DbgGDBMIQuit(int detach)
{
	dbgevent_t *	e = NULL;
#ifdef ALPS
	MICommand *		cmd;
#endif

	DVPRINT("*** DbgQuit\n");

	/*
	** Interrupt the process if it is running. This will generate a async event.
	** We want to grab it and ignore it.
	** We only want to do this if we are not detaching.
	*/
	if ( Status == DBGSTAT_RUNNING && !detach )
	{
		DVPRINT("DbgQuit: Sending SIGSTOP to process.\n");
		(void)kill(Pid, SIGSTOP);
		// wait for the asynch command to complete
		while (!DbgGDBMIEventReady())
			MISessionProgress(GDBMISession);
		e = WaitEvent(NULL);
		FreeEvent(e);
	}
	
	DVPRINT("DbgQuit: Detaching from process.\n");
	
	cmd = MITargetDetach(Pid);
	
	SendMICommand(GDBMISession, cmd);

#ifdef ALPS
	/*
	** On Cray systems there is a nasty race condition where alps kills off
	** the toolhelper daemons (i.e. gdb) before the application processes. This
	** means that when this happens there is a chance that the application will continue
	** and often cause a segfault. Detatching and then sending SIGSTOP fixes the
	** issue.
	*/
	
	// If the command completed ok, we need to send SIGSTOP ASAP!!!
	if (MICommandResultOK(cmd) && !detach) 
	{
		(void)kill(Pid, SIGSTOP);
		DVPRINT("DbgQuit: Sent process SIGSTOP.\n");
	}
#endif

	MICommandFree(cmd);

	DVPRINT("DbgQuit: Signal shutdown state.\n");
	
	// signal the shutdown state
	Shutdown++;

	return NewEvent(DBGEV_OK);
}

/*
** Interrupt the debugger.
*/
static dbgevent_t *
DbgGDBMIInterrupt(void)
{
	dbgevent_t *	e;
	dbgevent_t *	async_e;
	
	DVPRINT("*** DbgInterrupt\n");

	if ( Status == DBGSTAT_RUNNING )
	{
		if (kill(Pid, SIGSTOP))
		{
			switch(errno)
			{
				case EINVAL:
					EVENT_ERROR(e, DBGERR_DEBUGGER, "Invalid signal SIGSTOP.");
					break;
				case EPERM:
					EVENT_ERROR(e, DBGERR_DEBUGGER, "No permission to send SIGSTOP.");
					break;
				case ESRCH:
					EVENT_ERROR(e, DBGERR_DEBUGGER, "No such Pid to send SIGSTOP.");
					break;
				default:
					EVENT_ERROR(e, DBGERR_DEBUGGER, "Unknown kill() error.");
					break;
			}
			
			return e;
		}
		
		// wait for the asynch command to complete
		while (!DbgGDBMIEventReady())
			MISessionProgress(GDBMISession);
			
		async_e = WaitEvent(NULL);
			
		e = NewEvent(DBGEV_HALT);
		e->ev_loc = (dbgloc_t *)NULL;
		
		if (async_e->ev_sig_frame != NULL) {
			e->ev_loc = (dbgloc_t *)Alloc(sizeof(dbgloc_t));
			e->ev_loc->loc_file = (async_e->ev_sig_frame->frame_loc.loc_file == NULL ? NULL : strdup(async_e->ev_sig_frame->frame_loc.loc_file));
			e->ev_loc->loc_func = (async_e->ev_sig_frame->frame_loc.loc_func == NULL ? NULL : strdup(async_e->ev_sig_frame->frame_loc.loc_func));
			e->ev_loc->loc_addr = NULL; // We don't care about the address
			e->ev_loc->loc_line = async_e->ev_sig_frame->frame_loc.loc_line;
		}
		
		FreeEvent(async_e);
		
		return e;
	}
	
	// If we are already stopped, we will reuse DbgGDBMIShowFrame to get the 
	// bottommost frame
	async_e = DbgGDBMIShowFrame(1);
	
	e = NewEvent(DBGEV_HALT);
	e->ev_loc = (dbgloc_t *)NULL;
	
	if (async_e->ev_frame != NULL) {
		e->ev_loc = (dbgloc_t *)Alloc(sizeof(dbgloc_t));
		e->ev_loc->loc_file = (async_e->ev_frame->frame_loc.loc_file == NULL ? NULL : strdup(async_e->ev_frame->frame_loc.loc_file));
		e->ev_loc->loc_func = (async_e->ev_frame->frame_loc.loc_func == NULL ? NULL : strdup(async_e->ev_frame->frame_loc.loc_func));
		e->ev_loc->loc_addr = NULL; // We don't care about the address
		e->ev_loc->loc_line = async_e->ev_frame->frame_loc.loc_line;
	}
	
	FreeEvent(async_e);

	return e;
}

static dbgevent_t *
DbgGDBMIInvoke(char *host, int cb, char *proto, char *prog, char *args, char **env, int arch)
{
	dbgevent_t *	e;

	DVPRINT("*** DbgInvoke\n");

	MPArch = arch;

	e = InitDbg(prog, args);

	Status = DBGSTAT_STOPPED;

	AsyncCheck(WaitEvent, (dbgevent_t *)NULL);

	return e;
}

/*
** Attach debugger to running process.
*/
static dbgevent_t *
DbgGDBMIAttach(int pid, char *prog, char *args, char **env, int arch)
{
	dbgevent_t *	e;

	DVPRINT("*** DbgAttach\n");

	MPArch = arch;

	switch ( arch )
	{
	default:
		e = InitDbgAttach(pid, prog);
		break;
	}

	Status = DBGSTAT_STOPPED;

	return e;
}

/*
** Called if Shutdown > 0 to finalise debugger.
*/
static void
DbgGDBMIFinish(void)
{
	MICommand *cmd;

	cmd = MIGDBExit();
	SendMICommand(GDBMISession, cmd);
	MICommandFree(cmd);
}

/*
** Check if debugger is ready to receive a command.
*/
static int
DbgGDBMIReadyForCmd(void)
{
	return (int)(Status == DBGSTAT_WAITING || Status == DBGSTAT_STOPPED || Status == DBGSTAT_EXIT);
}

/*
** Check that debugger is ready ok
*/
static int
DbgGDBMIOK(void)
{
	return (int)(Status != DBGSTAT_DBGERR);
}

/*
** Check if debugger is ready to receive a command.
*/
static int
DbgGDBMIStopped(void)
{
	return (int)(Status == DBGSTAT_STOPPED || Status == DBGSTAT_EXIT);
}

/*
** Set contents of GDB debug variable var.
*/
static dbgevent_t *
DbgGDBMISetDebuggerVar(char *var, AIF *val)
{
	MICommand *	cmd;
	int 		intVal;

	DVPRINT("*** DbgSetDebuggerVar\n");

	AIFToInt(val, &intVal);

	cmd = MICommandNew("-gdb-set", MIResultRecordDONE);
	MICommandAddOption(cmd, var, MIIntToCString(intVal));
	SendMICommand(GDBMISession, cmd);
	MICommandFree(cmd);

	return NewEvent(DBGEV_OK);
}

static dbgevent_t *
DbgGDBMISetSourcePath(char *path)
{
	char *			pathdirs[2];
	MICommand * 	cmd;
	MIString *		cmd_result;
	char *			res;
	dbgevent_t *	e;

	DVPRINT("*** DbgSetSourcePath\n");
	
	pathdirs[0] = path;
	pathdirs[1] = NULL;
	
	cmd = MIEnvironmentDirectory(0, pathdirs);
	SendMICommand(GDBMISession, cmd);
	if (!MICommandResultOK(cmd)) 
	{
		MICommandFree(cmd);
		EVENT_ERROR(e, DBGERR_DEBUGGER, "Gdb -environment-directory command failed.");
		return e;
	}
	
	cmd_result = MIGetEnvironmentDirectoryInfo(cmd);
	
	e = NewEvent(DBGEV_SOURCEPATH);
	res = MIStringToCString(cmd_result);
	e->ev_source_path = (res == NULL) ? NULL : strdup(res);
	
	MICommandFree(cmd);
	MIStringFree(cmd_result);

	return e;
}

static dbgevent_t *
DbgGDBMIGetSourcePath(void)
{
	char *			pathdirs[1];
	MICommand * 	cmd;
	MIString *		cmd_result;
	char *			res;
	dbgevent_t *	e;

	DVPRINT("*** DbgSetSourcePath\n");
	
	pathdirs[0] = NULL;
	
	cmd = MIEnvironmentDirectory(0, pathdirs);
	SendMICommand(GDBMISession, cmd);
	if (!MICommandResultOK(cmd)) 
	{
		MICommandFree(cmd);
		EVENT_ERROR(e, DBGERR_DEBUGGER, "Gdb -environment-directory command failed.");
		return e;
	}
	
	cmd_result = MIGetEnvironmentDirectoryInfo(cmd);
	
	e = NewEvent(DBGEV_SOURCEPATH);
	res = MIStringToCString(cmd_result);
	e->ev_source_path = (res == NULL) ? NULL : strdup(res);
	
	MICommandFree(cmd);
	MIStringFree(cmd_result);

	return e;
}

/*
** Get contents of GDB debug variable var.
*/
static dbgevent_t *
DbgGDBMIGetDebuggerVar(char *var)
{
	MICommand *	cmd;

	DVPRINT("*** DbgGetDebuggerVar\n");

	cmd = MICommandNew("-gdb-show", MIResultRecordDONE);
	MICommandAddOption(cmd, var, NULL);
	SendMICommand(GDBMISession, cmd);
	MICommandFree(cmd);

	return NewEvent(DBGEV_OK);
}

static dbgevent_t *
ResetBeforeDirectCmd(void)
{
	MICommand *		cmd;
	dbgevent_t *	e;

	// we need gdb to set print elements and print repeats to 0 so 
	// that when we resolve the data it gives us the entire contents
	// of the data type.
	cmd = MIGDBSet("print elements", "200");
	SendMICommand(GDBMISession, cmd);
	if (!MICommandResultOK(cmd)) 
	{
		MICommandFree(cmd);
		EVENT_ERROR(e, DBGERR_DEBUGGER, "Gdb set print elements command failed.");
		return e;
	}
	MICommandFree(cmd);
	
	cmd = MIGDBSet("print repeats", "10");
	SendMICommand(GDBMISession, cmd);
	if (!MICommandResultOK(cmd)) 
	{
		MICommandFree(cmd);
		EVENT_ERROR(e, DBGERR_DEBUGGER, "Gdb set print repeats command failed.");
		return e;
	}
	MICommandFree(cmd);
	
	cmd = MIGDBSet("source open", "on");
	SendMICommand(GDBMISession, cmd);
	if (!MICommandResultOK(cmd)) 
	{
		MICommandFree(cmd);
		EVENT_ERROR(e, DBGERR_DEBUGGER, "Gdb set source open command failed.");
		return e;
	}
	MICommandFree(cmd);
	
	return NULL;
}

static dbgevent_t *
RestoreAfterDirectCmd(void)
{
	MICommand *		cmd;
	dbgevent_t *	e;

	// we need gdb to set print elements and print repeats to 0 so 
	// that when we resolve the data it gives us the entire contents
	// of the data type.
	cmd = MIGDBSet("print elements", "0");
	SendMICommand(GDBMISession, cmd);
	if (!MICommandResultOK(cmd)) 
	{
		MICommandFree(cmd);
		EVENT_ERROR(e, DBGERR_DEBUGGER, "Gdb set print elements command failed.");
		return e;
	}
	MICommandFree(cmd);
	
	cmd = MIGDBSet("print repeats", "0");
	SendMICommand(GDBMISession, cmd);
	if (!MICommandResultOK(cmd)) 
	{
		MICommandFree(cmd);
		EVENT_ERROR(e, DBGERR_DEBUGGER, "Gdb set print repeats command failed.");
		return e;
	}
	MICommandFree(cmd);
	
	cmd = MIGDBSet("source open", "off");
	SendMICommand(GDBMISession, cmd);
	if (!MICommandResultOK(cmd)) 
	{
		MICommandFree(cmd);
		EVENT_ERROR(e, DBGERR_DEBUGGER, "Gdb set source open command failed.");
		return e;
	}
	MICommandFree(cmd);
	
	return NULL;
}

/*
** Pass command directly to gdb/mi through console command
*/
static dbgevent_t *
DbgGDBMIDirectCmd(int cmd_argc, char **cmd_argv)
{
	MICommand *		cmd;
	dbgevent_t *	e;
	dbgevent_t *	e2 = NULL;
	// FIXME: Restore after reaching a breakpoint later on!
	int				restore = 1;

	DVPRINT("*** DbgDirectCmd\n");
	
	// Reset some of the print settings to default, otherwise the user might
	// not get a response
	e = ResetBeforeDirectCmd();
	
	if (e!=NULL)
		return e;

	cmd = CLIBypass(cmd_argc, cmd_argv);
	SendMICommand(GDBMISession, cmd);
	
	// Sometimes a command can cause the debugger to start running
	// If so the expected result record from CLIBypass would be wrong.
	// We don't want to use the MICommandResultOK command, rather we want to
	// check to see if it has an error.
	// We also want to update status in certain situations (like ^running)
	switch (MICommandResultClass(cmd))
	{
		case MIResultRecordINVALID:
			e = RestoreAfterDirectCmd();
			if (e!=NULL)
				return e;
			EVENT_ERROR(e, DBGERR_DEBUGGER, "Invalid command returned by MI layer.");
			MICommandFree(cmd);
			return e;
			
		case MIResultRecordRUNNING:
			Status = DBGSTAT_RUNNING;
			// We can't restore if the status changes to running
			restore = 0;
			DirectCmdAsyncRestore = 1;
			// FIXME: Use a custom WaitEvent
			AsyncCheck(WaitEvent, (dbgevent_t *)NULL);
			break;
			
		case MIResultRecordERROR:
			e = RestoreAfterDirectCmd();
			if (e!=NULL)
				return e;
			EVENT_ERROR(e, DBGERR_DEBUGGER, MICommandResultErrorMessage(cmd));
			MICommandFree(cmd);
			return e;
			
		case MIResultRecordEXIT:
			// The user shouldn't be allowed to exit directly!!!
			Status = DBGSTAT_DBGERR;
			EVENT_ERROR(e, DBGERR_DEBUGGER, "GDB reported exit! Things are likely broken.");
			MICommandFree(cmd);
			return e;
			
		case MIResultRecordCONNECTED:
			// TODO: How to handle this case.
		case MIResultRecordDONE:
		default:
			// Command is ok - do nothing here
			break;
	}
	
	e = NewEvent(DBGEV_CMD);
	e->ev_cmd = MICommandConsoleOOBMessages(cmd);
	
	MICommandFree(cmd);
	
	if (restore)
	{
		e2 = RestoreAfterDirectCmd();
		
		if (e2!=NULL)
		{
			FreeEvent(e);
			return e2;
		}
	}

	return e;
}



/*
**
** ----- auxiliary functions -----
**
*/



/*
** Initialise debugger, make sure we are using
** the correct version, etc.
*/
static dbgevent_t *
InitDbg(char *prog, char *args)
{
	dbgevent_t *	e;
	MICommand *		cmd;
	MIList *		bpts;
	MIBreakpoint *	bpt;

	CLIInfoProcInfo *procInfo;
	int breakpointNumberAtMain = 0;

	if ( (e = CheckVers()) != (dbgevent_t *)NULL )
		return e;

	cmd = MICommandNew("-file-exec-and-symbols", MIResultRecordDONE);
	MICommandAddOption(cmd, prog, NULL);
	SendMICommand(GDBMISession, cmd);
	MICommandFree(cmd);

	if ( args != NULL )
	{
		e = DbgGDBMISetArgs(args);

		if ( e->ev_event != DBGEV_OK )
			return e;
			
		FreeEvent(e);
	}

	/*
	** Check for fortran.
	*/
	cmd = MIBreakInsert(0, 0, NULL, 0, "MAIN__", 0);
	SendMICommand(GDBMISession, cmd);
	if (MICommandResultOK(cmd)) {
		/*
		** We are Fortran
		*/
		ProgMain = "MAIN__";
	} else {
		MICommandFree(cmd);
		cmd = MIBreakInsert(0, 0, NULL, 0, "main", 0);
		SendMICommand(GDBMISession, cmd);
		if (!MICommandResultOK(cmd)) {
			MICommandFree(cmd);
			EVENT_ERROR(e, DBGERR_DEBUGGER, "Could not set break at main.");
			return e;
		}
		/*
		** We are C
		*/
		ProgMain = "main";

		DVPRINT("InitDbg(): Breapoint set at main.\n");
	}
	bpts = MIBreakpointGetBreakInsertInfo(cmd);
	MICommandFree(cmd);

	if (bpts == NULL) {
		EVENT_ERROR(e, DBGERR_DEBUGGER, "Could not set break at main.");
		return e;
	}

	MIListSet(bpts);
	bpt = (MIBreakpoint *)MIListGet(bpts);
	breakpointNumberAtMain = bpt->number;
	MIListFree(bpts, MIBreakpointFree);

	if ( RunArgs != NULL ) {
		cmd = MICommandNew("-exec-arguments", MIResultRecordDONE);
		MICommandAddOption(cmd, RunArgs, NULL);
		SendMICommand(GDBMISession, cmd);
		if (!MICommandResultOK(cmd)) {
			MICommandFree(cmd);
			EVENT_ERROR(e, DBGERR_DEBUGGER, "Could not set program arguments.");
			return e;
		}
		MICommandFree(cmd);
	}

	cmd = MIExecRun();
	SendMICommand(GDBMISession, cmd);
	if (!MICommandResultOK(cmd)) {
		MICommandFree(cmd);
		EVENT_ERROR(e, DBGERR_DEBUGGER, "Could not run program.");
		return e;
	}
	MICommandFree(cmd);

	cmd = MIBreakDelete(1, &breakpointNumberAtMain);
	SendMICommand(GDBMISession, cmd);
	if (!MICommandResultOK(cmd)) {
		MICommandFree(cmd);
		EVENT_ERROR(e, DBGERR_DEBUGGER, "Could not delete breakpoint at main.");
		return e;
	}
	MICommandFree(cmd);

	/*
	** Find out pid.
	*/
	cmd = CLIInfoProc();
	SendMICommand(GDBMISession, cmd);
	if (!MICommandResultOK(cmd)) {
		MICommandFree(cmd);
		EVENT_ERROR(e, DBGERR_DEBUGGER, "Could not get pid.");
		return e;
	}
	procInfo = CLIGetInfoProcInfo(cmd);
	if (procInfo == NULL) {
		MICommandFree(cmd);
		EVENT_ERROR(e, DBGERR_DEBUGGER, "Could not get pid.");
		return e;
	}
	Pid = procInfo->pid;
	CLIInfoProcInfoFree(procInfo);
	MICommandFree(cmd);

	DVPRINT("InitDbg(): ProgMain=%s Pid=%d.\n", ProgMain, Pid);

	return NewEvent(DBGEV_OK);
}

/*
** Attach debugger. Added specifically for Cray XT / ALPS, but it might
** work for other platforms as well.
*/	
static dbgevent_t *
InitDbgAttach(int pid, char *prog)
{	
	dbgevent_t *		e;
	MICommand *			cmd;
	CLIInfoProcInfo *	procInfo;
	
	DVPRINT("*** InitDbgAttach\n");

	DBEPERFLOG(SetTimerStart("DbgAttach: CheckVers"));
	if ((e = CheckVers()) != (dbgevent_t *)NULL)
		return e;
	DBEPERFLOG(SetTimerStopAndLog("DbgAttach: CheckVers"));

	cmd = MITargetAttach(pid);
	
	DBEPERFLOG(SetTimerStart("DbgAttach: -target-attach"));
	SendMICommand(GDBMISession, cmd);
	DBEPERFLOG(SetTimerStopAndLog("DbgAttach: -target-attach"));

	// Check for success/failure
	if (!MICommandResultOK(cmd)) 
	{
		EVENT_ERROR(e, DBGERR_DEBUGGER, "Attach failed: %s", MICommandResultErrorMessage(cmd));
		MICommandFree(cmd);
		return e;
	}

	MICommandFree(cmd);
	
	/*
	** -target-attach is a *asynch* command that will generate a MI event.
	** we must grab it or it will fubar everything else up.
	*/
	// grab the "temporary" breakpoint - we want to ignore it
	// this is always generated by an attach command.
	e = WaitEvent(NULL);
	FreeEvent(e);
	
	/*
	** Find out pid.
	*/
	cmd = CLIInfoProc();
	SendMICommand(GDBMISession, cmd);
	if (!MICommandResultOK(cmd)) {
		EVENT_ERROR(e, DBGERR_DEBUGGER, "Could not get pid: %s", MICommandResultErrorMessage(cmd));
		MICommandFree(cmd);
		return e;
	}
	procInfo = CLIGetInfoProcInfo(cmd);
	if (procInfo == NULL) {
		EVENT_ERROR(e, DBGERR_DEBUGGER, "Could not get pid: %s", MICommandResultErrorMessage(cmd));
		MICommandFree(cmd);
		return e;
	}
	Pid = procInfo->pid;
	CLIInfoProcInfoFree(procInfo);
	MICommandFree(cmd);

	DVPRINT("InitDbgAttach(): Obtained pid: %d.\n", Pid);
	
	// setup default exit points
	if ((e = setup_gdbmi_exit_points()) != NULL)
		return e;

	return NewEvent(DBGEV_OK);
}

/*
** Invoked if there is input from the debugger after
** a non-terminating command (eg. run, step, etc.).
** Checks to see what caused the termination.
*/
static dbgevent_t *
WaitEvent(dbgevent_t *e)
{
	dbgbp_t *	bp = NULL;

	MIEvent * evt = PendingEvent;

	DVPRINT("*** WaitEvent\n");

	if ( e == (dbgevent_t *)NULL )
		e = NewEvent(DBGEV_NONE);

	AsynchEventWaiting = 0;
	PendingEvent = NULL;
	
	// check if we should reset due to a direct command triggering the run state
	if (DirectCmdAsyncRestore)
	{
		DirectCmdAsyncRestore = 0;
		dbgevent_t * e2 = RestoreAfterDirectCmd();
		if (e2!=NULL)
		{
			FreeEvent(e);
			return e2;
		}
	}
	
	// check for MI error condition
	if (MIGetError())
	{
		e = ChangeEvent(e, DBGEV_ERROR);
		e->ev_errno = DBGERR_DEBUGGER;
		EventErrStr(e, MIGetErrorStr());
		return e;
	}
	
	// check for null evt
	if (evt == NULL)
	{
		e = ChangeEvent(e, DBGEV_ERROR);
		e->ev_errno = DBGERR_DEBUGGER;
		EventErrStr(e, "WaitEvent called with NULL PendingEvent!");
		return e;
	}

	switch (evt->type)
	{
	case MIEventTypeBreakpointHit:
		DVPRINT("MIEventTypeBreakpointHit\n");
		DVPRINT("Breakpoint id is %d\n", evt->bkptno);

		if ( Status == DBGSTAT_INITIALISING )
		{
			bp = NewBP(evt->bkptno);
			e->ev_event = DBGEV_BPHIT;
			e->ev_bp = bp;

			return e;
		}
		
		// set the status to stopped
		Status = DBGSTAT_STOPPED;
		
		bp = FindBPByID(BP, evt->bkptno);
		
		// We want to check for an exit breakpoint if we didn't find it in the
		// normal BP list.
		if ( bp == NULL)
		{
			bp = FindBPByID(exit_BP, evt->bkptno);
			// set the status to exit if we are at an exit breakpoint
			if (bp != NULL)
			{
				Status = DBGSTAT_EXIT;
			}
		}

		// If bp is still null, we have a problem.
		if ( bp == NULL )
		{
			e = ChangeEvent(e, DBGEV_ERROR);
			e->ev_errno = DBGERR_DEBUGGER;
			EventErrStr(e, "Hit unknown breakpoint!");
			return e;
		}

		bp->bp_hits++;
		CurrBP = bp;

		e->ev_event = DBGEV_BPHIT;
		e->ev_bp = DupBP(bp);
		
		// Check to see if disposition was set to disable or delete
		// We have to do this now because if the bp was set to delete we wouldn't
		// be able to do a DupBP on it...
		// FIXME: This should probably be implemented by setting the bp_deleted
		//         to 1 and cleaning up breakpoints on the next continue.
		switch (bp->bp_disp)
		{
			case DBGBP_DELETE_DISP:
				// delete the breakpoint
				// NOTE: We don't know if the evt->bkptno is in BP or exit_BP, so we
				//       try to remove from both. Its okay to do this.
				RemoveBP(BP, evt->bkptno);
				RemoveBP(exit_BP, evt->bkptno);
				CurrBP = NULL;
				break;
			case DBGBP_DISABLE_DISP:
				// disable the breakpoint
				bp->bp_enabled = 0;
				break;
			default:
				// do nothing
				break;
		}

		MIEventFree(evt);
		return e;
		
	case MIEventTypeWatchpointTrigger:
		DVPRINT("MIEventTypeWatchpointTrigger\n");
		DVPRINT("Breakpoint id is %d\n", evt->number);
		
		bp = FindBPByID(BP, evt->number);

		// If bp is null, we have a problem.
		if ( bp == NULL )
		{
			e = ChangeEvent(e, DBGEV_ERROR);
			e->ev_errno = DBGERR_DEBUGGER;
			EventErrStr(e, "Hit unknown watchpoint!");
			return e;
		}

		bp->bp_hits++;
		
		// We need to update the breakpoint information with the evt frame.
		// This will change every time the breakpoint is hit.
		// First free the old information
		if (bp->bp_loc.loc_file != NULL)
		{
			Free(bp->bp_loc.loc_file);
			bp->bp_loc.loc_file = NULL;
		}
		if (bp->bp_loc.loc_func != NULL)
		{
			Free(bp->bp_loc.loc_func);
			bp->bp_loc.loc_func = NULL;
		}
		if (bp->bp_loc.loc_addr != NULL)
		{
			Free(bp->bp_loc.loc_addr);
			bp->bp_loc.loc_addr = NULL;
		}
		bp->bp_loc.loc_line = -1;
		
		// Now update it with the new info
		if (evt->frame != NULL)
		{
			if (evt->frame->file != NULL)
				bp->bp_loc.loc_file = strdup(evt->frame->file);
			if (evt->frame->func != NULL)
				bp->bp_loc.loc_func = strdup(evt->frame->func);
			if (evt->frame->addr != NULL)
				bp->bp_loc.loc_addr = strdup(evt->frame->addr);
			bp->bp_loc.loc_line = evt->frame->line;
		}
		
		CurrBP = bp;

		e->ev_event = DBGEV_BPHIT;
		e->ev_bp = DupBP(bp);
		
		// Check to see if disposition was set to disable or delete
		// We have to do this now because if the bp was set to delete we wouldn't
		// be able to do a DupBP on it...
		// FIXME: This should probably be implemented by setting the bp_deleted
		//         to 1 and cleaning up breakpoints on the next continue.
		switch (bp->bp_disp)
		{
			case DBGBP_DELETE_DISP:
				// delete the watchpoint
				RemoveBP(BP, evt->bkptno);
				CurrBP = NULL;
				break;
			case DBGBP_DISABLE_DISP:
				// disable the watchpoint
				bp->bp_enabled = 0;
				break;
			default:
				// do nothing
				break;
		}

		Status = DBGSTAT_STOPPED;
		MIEventFree(evt);
		return e;

	case MIEventTypeSuspended:
		DVPRINT("MIEventTypeSuspended\n");
		
		// FIXME: This should be using its own event type
		e->ev_event = DBGEV_STEP;
		e->ev_step_frame = (dbgframe_t *)NULL;
		e->ev_step_line = (char *)NULL;

		if (evt->frame != NULL) {
			e->ev_step_lno = evt->frame->line;
			// No need to get the line again since we already have it in the MIFrame information.
			e->ev_step_line = NULL;
			e->ev_step_frame = (dbgframe_t *)Alloc(sizeof(dbgframe_t));
			e->ev_step_frame->frame_level = evt->frame->level;
			e->ev_step_frame->frame_loc.loc_file = (evt->frame->file == NULL ? NULL : strdup(evt->frame->file));
			e->ev_step_frame->frame_loc.loc_func = (evt->frame->func == NULL ? NULL : strdup(evt->frame->func));
			e->ev_step_frame->frame_loc.loc_addr = NULL; // We don't care about the address
			e->ev_step_frame->frame_loc.loc_line = evt->frame->line;
			e->ev_step_frame->frame_next = NULL;
		}
		
		Status = DBGSTAT_STOPPED;
		MIEventFree(evt);
		return e;

	case MIEventTypeFunctionFinished:
		DVPRINT("MIEventTypeFunctionFinished\n");
		
		// FIXME: This should be using its own event type
		e->ev_event = DBGEV_STEP;
		e->ev_step_frame = (dbgframe_t *)NULL;
		e->ev_step_line = (char *)NULL;

		if (evt->frame != NULL) {
			e->ev_step_lno = evt->frame->line;
			// No need to get the line again since we already have it in the MIFrame information.
			e->ev_step_line = NULL;
			e->ev_step_frame = (dbgframe_t *)Alloc(sizeof(dbgframe_t));
			e->ev_step_frame->frame_level = evt->frame->level;
			e->ev_step_frame->frame_loc.loc_file = (evt->frame->file == NULL ? NULL : strdup(evt->frame->file));
			e->ev_step_frame->frame_loc.loc_func = (evt->frame->func == NULL ? NULL : strdup(evt->frame->func));
			e->ev_step_frame->frame_loc.loc_addr = NULL; // We don't care about the address
			e->ev_step_frame->frame_loc.loc_line = evt->frame->line;
			e->ev_step_frame->frame_next = NULL;
		}

		Status = DBGSTAT_STOPPED;
		MIEventFree(evt);
		return e;

	case MIEventTypeSteppingRange:
		DVPRINT("MIEventTypeSteppingRange\n");

		e->ev_event = DBGEV_STEP;
		e->ev_step_frame = (dbgframe_t *)NULL;
		e->ev_step_line = (char *)NULL;

		if (evt->frame != NULL) {
			e->ev_step_lno = evt->frame->line;
			// No need to get the line again since we already have it in the MIFrame information.
			e->ev_step_line = NULL;
			e->ev_step_frame = (dbgframe_t *)Alloc(sizeof(dbgframe_t));
			e->ev_step_frame->frame_level = evt->frame->level;
			e->ev_step_frame->frame_loc.loc_file = (evt->frame->file == NULL ? NULL : strdup(evt->frame->file));
			e->ev_step_frame->frame_loc.loc_func = (evt->frame->func == NULL ? NULL : strdup(evt->frame->func));
			e->ev_step_frame->frame_loc.loc_addr = NULL; // We don't care about the address
			e->ev_step_frame->frame_loc.loc_line = evt->frame->line;
			e->ev_step_frame->frame_next = NULL;
		}

		Status = DBGSTAT_STOPPED;
		MIEventFree(evt);
		return e;

	case MIEventTypeSignal:
		DVPRINT("MIEventTypeSignal %s\n", evt->sigName);

		e->ev_event = DBGEV_SIGNAL;
		e->ev_sig_type = (evt->sigName == NULL ? NULL : strdup(evt->sigName));
		e->ev_sig_line = NULL;
		e->ev_sig_frame = (dbgframe_t *)NULL;
		
		if (evt->frame != NULL) {
			e->ev_sig_frame = (dbgframe_t *)Alloc(sizeof(dbgframe_t));
			e->ev_sig_frame->frame_level = evt->frame->level;
			e->ev_sig_frame->frame_loc.loc_file = (evt->frame->file == NULL ? NULL : strdup(evt->frame->file));
			e->ev_sig_frame->frame_loc.loc_func = (evt->frame->func == NULL ? NULL : strdup(evt->frame->func));
			e->ev_sig_frame->frame_loc.loc_addr = NULL; // We don't care about the address
			e->ev_sig_frame->frame_loc.loc_line = evt->frame->line;
			e->ev_sig_frame->frame_next = NULL;
		}

		Status = DBGSTAT_STOPPED;
		MIEventFree(evt);
		return e;

	case MIEventTypeInferiorSignalExit:
		DVPRINT("MIEventTypeInferiorSignalExit\n");
		MIEventFree(evt);
		return e;

	case MIEventTypeInferiorExit:
		DVPRINT("MIEventTypeInferiorExit\n");
		e->ev_event = DBGEV_EXIT;
		e->ev_exit = 0;
		Status = DBGSTAT_WAITING;
		Pid = 0;
		MIEventFree(evt);
		return e;

	default:
		DVPRINT("MIEventTypeDefault\n");
		e = ChangeEvent(e, DBGEV_ERROR);
		e->ev_errno = DBGERR_DEBUGGER;
		EventErrStr(e, "unknown event");
		MIEventFree(evt);
		return e;
	}
	/* NOT REACHED */
}

/*
** Check that breakpoint command has succeded and
** extract appropriate information. Returns breakpoint
** id in bid. Adds to breakpoint list if necessary.
*/
static dbgevent_t *
CheckBreak(MICommand *cmd)
{
	int				id;
	dbgbp_t *		bp;
	dbgevent_t *	e;
	MIBreakpoint *	bpt;
	MIList *		bpts;
	
	DVPRINT("*** CheckBreak\n");

	bpts = MIBreakpointGetBreakInsertInfo(cmd);

	if (bpts == NULL) {
		return NULL;
	}
	
	MIListSet(bpts);
	bpt = (MIBreakpoint *)MIListGet(bpts);

	id = bpt->number;

	bp = NewBP(id);

	if (bpt->isWpt)
	{
		if (bpt->isWWpt)
		{
			if (bpt->isRWpt)
				bp->bp_type = DBGBP_AWATCH_TYPE;
			else
				bp->bp_type = DBGBP_WATCH_TYPE;
		} else if (bpt->isRWpt)
		{
			bp->bp_type = DBGBP_RWATCH_TYPE;
		}
	} else
	{
		bp->bp_type = DBGBP_BREAK_TYPE;
	}
	
	bp->bp_enabled = bpt->enabled;
	
	// set the breakpoint disposition
	if (bpt->disp != NULL)
	{
		if (strncmp(bpt->disp, "keep", 4) == 0)
			bp->bp_disp = DBGBP_KEEP_DISP;
		else if (strncmp(bpt->disp, "del", 3) == 0)
			bp->bp_disp = DBGBP_DELETE_DISP;
		else if (strncmp(bpt->disp, "dis", 3) == 0)
			bp->bp_disp = DBGBP_DISABLE_DISP;
		else
			bp->bp_disp = DBGBP_UNKNOWN_DISP;
	} else
	{
		bp->bp_disp = DBGBP_UNKNOWN_DISP;
	}
	
	// If this breakpoint has multiple locations, fake the information based
	// on the original location.
	if (bpt->hasMultiple)
	{
		DVPRINT("bpt has multiple locations.\n");
		if (bpt->origLoc != NULL)
		{
			DVPRINT("bpt original location is present.\n");
			char *loc_str = strdup(bpt->origLoc);
			char *loc_ptr;
			// try to split based on "file:line"
			if ((loc_ptr = strstr(loc_str, ":")) != NULL)
			{
				DVPRINT("Faking line location\n");
				// set needle to null
				*loc_ptr++ = '\0';
				bp->bp_loc.loc_file = strdup(loc_str);
				bp->bp_loc.loc_line = atoi(loc_ptr);
				Free(loc_str);
			} else
			{
				DVPRINT("Faking func location\n");
				// The entire string is a function.
				bp->bp_loc.loc_func = loc_str;
			}
		}
	} else
	{
		bp->bp_loc.loc_line = bpt->line;
		if (bpt->file != NULL)
			bp->bp_loc.loc_file = strdup(bpt->file);
		if (bpt->func != NULL)
			bp->bp_loc.loc_func = strdup(bpt->func);
		if (bpt->address != NULL)
			bp->bp_loc.loc_addr = strdup(bpt->address);
		if (bpt->what != NULL)
			bp->bp_exp = strdup(bpt->what);
	}
	
	/*
	** Link a copy of the breakpoint onto BP.
	*/
	AddToList(BP, (void *)bp);

	/*
	** Now create a fake event and make it
	** look like a BPSET.
	*/
	e = NewEvent(DBGEV_BPSET);
	e->ev_bp = DupBP(bp);

	MIListFree(bpts, MIBreakpointFree);

	return e;
}

/*
** Signal handler.
*/
static void
Finish(int sig)
{
	DVPRINT("*** In Finish.\n");
	Shutdown++;
}

/*
** Check for correct debugger version.
*/
static dbgevent_t *
CheckVers(void)
{
	dbgevent_t *	e = NULL;
	MICommand * 	cmd;
	double			gdb_version = -1.0;
	char *			gdb_arch = NULL;
	int				vers_err = 0;
	int				arch_err = 0;
	
	DVPRINT("*** CheckVers\n");

	cmd = MIGDBVersion();
	SendMICommand(GDBMISession, cmd);
	if (MICommandResultOK(cmd)) {
		gdb_version = CLIGetGDBVersion(cmd);
		gdb_arch = CLIGetGDBArch(cmd);
	}
	
	DVPRINT("CheckVers: Debugger vers=%f arch=%s\n", gdb_version, gdb_arch);

	MICommandFree(cmd);
	
	if (gdb_version < GDB_MIN_VERS)
		++vers_err;
		
	if (gdb_arch == NULL)
	{
		gdb_arch = strdup("unknown");
		++arch_err;
	} else if (strncmp(gdb_arch, GDB_EXPECTED_ARCH, strlen(GDB_EXPECTED_ARCH)))
		++arch_err;
	
	if (arch_err && vers_err)
	{
		EVENT_ERROR(e, DBGERR_DEBUGGER, "Invalid gdb version %f and arch %s. Expected at least version %f and arch %s.", 
						gdb_version, 
						gdb_arch, 
						GDB_MIN_VERS, 
						GDB_EXPECTED_ARCH);
	} else if (arch_err)
	{
		EVENT_ERROR(e, DBGERR_DEBUGGER, "Invalid gdb arch %s. Expected arch %s.", gdb_arch, GDB_EXPECTED_ARCH);
	} else if (vers_err)
	{
		EVENT_ERROR(e, DBGERR_DEBUGGER, "Invalid gdb version %f. Expected at least version %f.", gdb_version, GDB_MIN_VERS);
	}

	return e;
}

/*
** Send the MI command
*/
void
SendMICommand(MISession *sess, MICommand *cmd)
{
	char *buf;

	MISessionSendCommand(sess, cmd);
	do {

		if (MISessionProgress(sess) == -1)
		{
			if ((buf = MICommandResultErrorMessage(cmd)) != NULL)
			{
				DVPRINT("SendMICommand: Debugger error: %s\n", buf);
				free(buf);
			} else 
			{
				DVPRINT("SendMICommand: Error from GDB, but no message\n");
			}

			break;
		}

		if (sess->out_fd == -1) 
		{
			DVPRINT("SendMICommand: sess->out_fd = -1\n");
			break;
		}

	} while (!MICommandCompleted(cmd));

}

/* GDBMI callback functions */
static void
event_callback(MIEvent *e)
{
	DVPRINT("NEWGDBMI event> %d\n", e->type);
	AsynchEventWaiting = 1;
	PendingEvent = e;
}

static void
console_callback(char *str)
{
	DVPRINT("NEWGDBMI console> %s\n", str);
}

static void
log_callback(char *str)
{
	DVPRINT("NEWGDBMI log> %s\n", str);
}

static void
target_callback(char *str)
{
	
	DVPRINT("NEWGDBMI target> %s\n", str);
	
}



/*
**
** ----- hashing and slicing functions -----
**
*/

// rather than coded in DbgGDBMIEvalExpr()
static dbgevent_t *
PerformExtEvalExpr(char *exp)
{
	dbgevent_t *	e;
	AIF *	a;

	AIF *		tmp = NULL;

	char		var[BUFSIZ]="";
	char		args[BUFSIZ]="";
	char *		type;
	int			func_id = 0;
	char		sliceinfo[BUFSIZ]="";

	DVPRINT("*** PerformExtEvalExpr(%s-%d)\n", exp, my_rank);

	ParseDBGExpr(exp, &func_id, sliceinfo, var, args);

	DVPRINT("*** PerformExtEvalExpr(%s-%s-%s)\n", var, args, sliceinfo);

	// We perform hashing separately
	if (func_id == EXPR_HASH_BOB)
	{
		int hashfunc=0, precision_lower, precision_upper;
		char hashinfo[BUFSIZ]="";
		ParseHashArgs(args, &hashfunc, &precision_lower, &precision_upper, hashinfo);
		
		if ((e = GetAIFVarHash(GDBMISession, var, &a, hashinfo, func_id, precision_lower, precision_upper)) != NULL)
		{
			return e;
		}
		
		e = NewEvent(DBGEV_DATA);
		e->ev_data = a;

		if (debug_flag)
		{
			char *aif_str = NULL;
			AIFToStr(&aif_str, 0, a);
			DVPRINT("Final AIF size is: %d\n", AIF_LEN(a));
			DVPRINT("Final AIF type is: %s\n", aif_str);
			Free(aif_str);
		}
			
		return e;
	}

	if ((e = GetAIFVar(GDBMISession, var, &a, &type)) != NULL)
	{
		return e;
	}
	
	e = NewEvent(DBGEV_DATA);

	// if slice info found, perform slicing first
	if (strlen(sliceinfo) > 0)
	{
			int is_revert = 0;

			if ( AIFType(a) == AIF_FTN_ARRAY )
				is_revert = 1;

			e->ev_data = PerformSlicing(a, sliceinfo, is_revert);
			AIFFree(a);
				
			if (e->ev_data == NULL)
			{
				// an error occured.
				FreeEvent(e);
				e = NULL;
				EVENT_ERROR(e, DBGERR_DEBUGGER, "Slicing failed: %s", PerformSlicingErr);
				
				return e;
			}
	} else
	{
		e->ev_data = a;
	}

	// Minh changes here to do p2p mode
	// we should deal with p2p separately
	if (func_id == EXPR_P2P)
	{
		int is_sender = 0;
		//do a lot of things here
		tmp = PerformP2P(e->ev_data, args, my_rank, &is_sender);
		if (!is_sender)	// if not sender, send the data back to client
		{
			AIFFree(e->ev_data);
			e->ev_data = tmp;
		} else		// otherwise, just make a empty event
		{
			FreeEvent(e);
			e = NewEvent(DBGEV_OK);
		}
	}
	// Minh change here to do the hashing of data 03/06/2009
	else if (func_id > 0)
	{
		double lower_bound, upper_bound;
		
		switch (func_id) 
		{
			case EXPR_SUM:		// sum
				tmp = AIFSum(e->ev_data);
				break;
			case EXPR_PRODUCT:	// product
				tmp = AIFProduct(e->ev_data);
				break;
			case EXPR_AVERAGE:	// average
				tmp = AIFAverage(e->ev_data);
				break;
			case EXPR_MIN:	// min
				tmp = AIFMin(e->ev_data);
				break;
			case EXPR_MAX:	// max
				tmp = AIFMax(e->ev_data);
				break;
			case EXPR_STDEV:	// standard deviation
				tmp = AIFStdev(e->ev_data);
				break;
			case EXPR_COUNTELEM:	// countelem
				ParseCountelemArgs(args, &lower_bound, &upper_bound);
				tmp = AIFCountElem(e->ev_data, lower_bound, upper_bound);
				break;
			default:
				DVPRINT("*** Unknow reduce operation\n");
				break;
		}

		AIFFree(e->ev_data);
		e->ev_data = tmp;
		
		if (debug_flag)
		{
			char *aif_str = NULL;
			AIFToStr(&aif_str, 0, a);
			DVPRINT("Final AIF size is: %d\n", AIF_LEN(a));
			DVPRINT("Final AIF type is:\n %s\n", aif_str);
			Free(aif_str);
		}
	}

	return e;
}

dbgevent_t *
DbgGDBMIEvalExpr(char *exp)
{
	char *			type;
	AIF *			a;
	dbgevent_t *	e;

	DVPRINT("*** DbgEvalExpr(%s-%d)\n", exp, my_rank);

	// Check whether it has hashing/slicing
	if (strpbrk(exp, "`") || strpbrk(exp, "|")) 
	{
		// TODO: we don't want to parse this if the user escaped the input	
		DVPRINT("DbgEvalExpr: Found reduction/slicing info.\n");
		e = PerformExtEvalExpr(exp);
		if (e == NULL) {
			e = NewEvent(DBGEV_ERROR);
			e->ev_errno = DBGERR_DEBUGGER;
			EventErrStr(e, "bad data format");
			return e;
		}
		else if (e->ev_data == (AIF*)NULL) {
			e = NewEvent(DBGEV_ERROR);
			e->ev_errno = DBGERR_DEBUGGER;
			EventErrStr(e, "NULL data found");
			return e;
		}
		else
		{
			DVPRINT("*** out DbgEvalExpr(%s-%d)\n", exp, my_rank);
			return e;
		}
	}

	if ((e = GetAIFVar(GDBMISession, exp, &a, &type)) != NULL)
	{
		return e;
	}
	
	e = NewEvent(DBGEV_NONE);
	e->ev_event = DBGEV_DATA;
	e->ev_data = a;

	if (debug_flag)
	{
		char *aif_str = NULL;
		AIFToStr(&aif_str, 0, a);
		DVPRINT("Final AIF size is: %d\n", AIF_LEN(a));
		DVPRINT("Final AIF type is: %s\n", aif_str);
		Free(aif_str);
	}

	return e;
}

dbgevent_t *
DbgGDBMIOpenSocket(char *opset_name, int opset_size)
{
	int i;

	if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("Socket");
	}
	
	// load the network info for the other procset
	other_app.id = strdup(opset_name);
	other_app.nrank = opset_size;
	other_app.rank = (rankInfoEntry_t **)Alloc(sizeof(rankInfoEntry_t *)*opset_size);
	for (i=0; i<opset_size; i++)
		other_app.rank[i] = (rankInfoEntry_t *)Alloc(sizeof(rankInfoEntry_t));
	LoadAppInfo(&other_app);

	return NewEvent(DBGEV_OK);
}

dbgevent_t *
DbgGDBMICloseSocket(void)
{
	close(sock_fd);

	// unload other_app
	UnloadAppInfo(&other_app);

	return NewEvent(DBGEV_OK);
}

