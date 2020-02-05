/******************************************************************************\
 * GDB/MI frontend interface routines
 * TODO: This does not support asynchronous commands.
 *
 * © 2013-2019 Cray Inc.	All Rights Reserved.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Cray Inc.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *
 ******************************************************************************/

#ifdef HAVE_CONFIG_H
#include	<config.h>
#endif /* HAVE_CONFIG_H */

#include <limits.h>
#include <stdlib.h>
#include <string.h>

#include "defs.h"
#include "dbgclnt_debugfe.h"
#include "useful.h"

#include "MI.h"

#define	 GDB_MIN_VERS 7.2
#define GDB_EXPECTED_ARCH "x86_64"

/* static prototypes */
static void			DbgInstantiateGDBMI(dbgfrontend_sw *);
static dbgevent_t *	DbgGDBMIInit_fe(dbgfrontend_sw *);
static dbgevent_t *	DbgGDBMIQuit_fe(dbgfrontend_sw *);
static dbgevent_t *	DbgGDBMIList_fe(dbgfrontend_sw *, const char *);
static dbgevent_t * DbgGDBMIGetSources_fe(dbgfrontend_sw *);
static dbgevent_t *	DbgGDBMISetFile_fe(dbgfrontend_sw *, const char *);
static dbgevent_t *	DbgGDBMISetSourcePath_fe(dbgfrontend_sw *, const char *);
static dbgevent_t *	DbgGDBMIGetSourcePath_fe(dbgfrontend_sw *);
static dbgevent_t *	DbgGDBMIGetScope_fe(dbgfrontend_sw *, const char *);
static dbgevent_t *	DbgGDBMICheckStartupFunc_fe(dbgfrontend_sw *);

/* auxiliary functions */
/*
static void		event_callback(MIEvent *);
static void		console_callback(char *);
static void		log_callback(char *);
static void		target_callback(char *);
*/
static dbgevent_t *	setup_gdbmi_environment(MISession *);
static void 		SendMICommand_fe(MISession *sess, MICommand *cmd);


dbgfrontend_sw_create DbgFrontendGDBMICreate =
{
	DbgInstantiateGDBMI
};

static void
DbgInstantiateGDBMI(dbgfrontend_sw *fe_obj)
{
	fe_obj->dbg_session = (dbgfrontend_sess)MISessionNew();
	fe_obj->dbg_init = DbgGDBMIInit_fe;
	fe_obj->dbg_quit = DbgGDBMIQuit_fe;
	fe_obj->dbg_set_file = DbgGDBMISetFile_fe;
	fe_obj->dbg_checkstartupfunc = DbgGDBMICheckStartupFunc_fe;
	fe_obj->dbg_list = DbgGDBMIList_fe;
	fe_obj->dbg_getsources = DbgGDBMIGetSources_fe;
	fe_obj->dbg_setsourcepath = DbgGDBMISetSourcePath_fe;
	fe_obj->dbg_getsourcepath = DbgGDBMIGetSourcePath_fe;
	fe_obj->dbg_getscope = DbgGDBMIGetScope_fe;

	// only set debug for gdbmi if compiled with debug mode
#ifdef DEBUG
	MISessionSetDebug(1);
#else
	MISessionSetDebug(0);
#endif
}

/*
** Send the MI command
*/
static void
SendMICommand_fe(MISession *sess, MICommand *cmd)
{
	char *buf;

	MISessionSendCommand(sess, cmd);
	do {

		if (MISessionProgress(sess) == -1)
		{
			if ((buf = MICommandResultErrorMessage(cmd)) != NULL)
			{
				DPRINT("SendMICommand: Debugger error: %s\n", buf);
				free(buf);
			} else
			{
				DPRINT("SendMICommand: Error from GDB, but no message\n");
			}

			break;
		}

		if (sess->out_fd == -1)
		{
			DPRINT("SendMICommand: sess->out_fd = -1\n");
			break;
		}

	} while (!MICommandCompleted(cmd));
}

static dbgevent_t *
setup_gdbmi_environment(MISession *sess)
{
	dbgevent_t *	e;
	MICommand *		cmd;

	cmd = MIGDBSet("confirm", "off");
	SendMICommand_fe(sess, cmd);
	if (!MICommandResultOK(cmd))
	{
		MICommandFree(cmd);
		EVENT_ERROR(e, DBGERR_DEBUGGER, "Gdb set confirm command failed.");
		return e;
	}
	MICommandFree(cmd);

	// stop printing strings on the first null character
	cmd = MIGDBSet("print null-stop", "on");
	SendMICommand_fe(sess, cmd);
	if (!MICommandResultOK(cmd))
	{
		MICommandFree(cmd);
		EVENT_ERROR(e, DBGERR_DEBUGGER, "Gdb set print null-stop command failed.");
		return e;
	}
	MICommandFree(cmd);

	// demangle c++ names
	cmd = MIGDBSet("print demangle", "on");
	SendMICommand_fe(sess, cmd);
	if (!MICommandResultOK(cmd))
	{
		MICommandFree(cmd);
		EVENT_ERROR(e, DBGERR_DEBUGGER, "Gdb set print demangle command failed.");
		return e;
	}
	MICommandFree(cmd);

	cmd = MIGDBSet("print asm-demangle", "on");
	SendMICommand_fe(sess, cmd);
	if (!MICommandResultOK(cmd))
	{
		MICommandFree(cmd);
		EVENT_ERROR(e, DBGERR_DEBUGGER, "Gdb set print asm-demangle command failed.");
		return e;
	}
	MICommandFree(cmd);

	// print derived types from the vtable
	cmd = MIGDBSet("print object", "on");
	SendMICommand_fe(sess, cmd);
	if (!MICommandResultOK(cmd))
	{
		MICommandFree(cmd);
		EVENT_ERROR(e, DBGERR_DEBUGGER, "Gdb set print object command failed.");
		return e;
	}
	MICommandFree(cmd);

	// limit backtrace size to 1000
	cmd = MIGDBSet("backtrace limit", "1000");
	SendMICommand_fe(sess, cmd);
	if (!MICommandResultOK(cmd))
	{
		MICommandFree(cmd);
		EVENT_ERROR(e, DBGERR_DEBUGGER, "Gdb set backtrace limit command failed.");
		return e;
	}
	MICommandFree(cmd);

	// unwind signals triggered by gdb
	cmd = MIGDBSet("unwindonsignal", "on");
	SendMICommand_fe(sess, cmd);
	if (!MICommandResultOK(cmd))
	{
		MICommandFree(cmd);
		EVENT_ERROR(e, DBGERR_DEBUGGER, "Gdb set unwindonsignal command failed.");
		return e;
	}
	MICommandFree(cmd);

	// always continue on fatal errors
	cmd = MIGDBSet("continue-on-fatal-error", "on");
	SendMICommand_fe(sess, cmd);
	if (!MICommandResultOK(cmd))
	{
		MICommandFree(cmd);
		EVENT_ERROR(e, DBGERR_DEBUGGER, "Gdb set continue-on-fatal-error command failed.");
		return e;
	}
	MICommandFree(cmd);

	return NULL;
}

static dbgevent_t *
DbgGDBMIInit_fe(dbgfrontend_sw *fe_obj)
{
	dbgevent_t *	e;
	MISession * 	sess = (MISession *)fe_obj->dbg_session;

	DPRINT("*** DbgGDBMIInit_fe\n");

	/*MISessionRegisterEventCallback(sess, event_callback);
	MISessionRegisterConsoleCallback(sess, console_callback);
	MISessionRegisterLogCallback(sess, log_callback);
	MISessionRegisterTargetCallback(sess, target_callback);*/

	// set the gdb path to use gdb-guard
	MISessionSetGDBPath(sess, "gdb-guard");

	// Get the base dir env var and setup data-dir if the env var is set
	char *env_base_dir = getenv(GUARD_INST_DIRECTORY);
	if (env_base_dir != NULL) {
		char *			data_dir;
		Asprintf(&data_dir, "%s/share/gdb", env_base_dir);
		MISessionSetGDBDataDirectory(sess, data_dir);
		Free(data_dir);
	}

	if (MISessionStartLocal(sess, NULL) < 0)
	{
		EVENT_ERROR(e, DBGERR_DEBUGGER, "Could not start the debugger.");
		return e;
	}

	if ((e = setup_gdbmi_environment(sess)) != NULL)
		return e;

	// no need to startup debugger, we are not attaching onto anything.
	fe_obj->dbg_status = DBGSTAT_STOPPED;

	return (dbgevent_t *)NULL;
}

/*
** Set a.out inferior file for symbol reading.
*/
static dbgevent_t *
DbgGDBMISetFile_fe(dbgfrontend_sw *fe_obj, const char *file)
{
	dbgevent_t *	e;
	MICommand *		cmd;
	MISession *		sess = (MISession *)fe_obj->dbg_session;

	DPRINT("*** DbgGDBMISetFile_fe\n");

	cmd = MIFileExecAndSymbols(file);
	SendMICommand_fe(sess, cmd);
	if (!MICommandResultOK(cmd))
	{
		EVENT_ERROR(e, DBGERR_DEBUGGER, MICommandResultErrorMessage(cmd));
		MICommandFree(cmd);
		return e;
	}

	MICommandFree(cmd);

	return NewEvent(DBGEV_OK);
}

static dbgevent_t *
DbgGDBMIQuit_fe(dbgfrontend_sw *fe_obj)
{
	MICommand *	cmd;
	MISession *	sess = (MISession *)fe_obj->dbg_session;

	DPRINT("*** DbgGDBMIQuit_fe\n");

	cmd = MIGDBExit();
	SendMICommand_fe(sess, cmd);
	MICommandFree(cmd);

	return NewEvent(DBGEV_OK);;
}

/*
** List source code.
**
** GDB does not have a MI command for this; we use the CLI command.
*/
static dbgevent_t *
DbgGDBMIList_fe(dbgfrontend_sw *fe_obj, const char *str)
{
	dbgevent_t *	e;
	MICommand *		cmd;
	MISession *		sess = (MISession *)fe_obj->dbg_session;
	MIList *		lines;
	CLILineInfo *	line;
	dbglist_t *		line_list;
	dbglist_t *		list_ptr = NULL;
	dbglist_t *		list = NULL;


	DPRINT("*** DbgGDBMIList_fe\n");

	cmd = CLIList(str);
	SendMICommand_fe(sess, cmd);
	if (!MICommandResultOK(cmd))
	{
		EVENT_ERROR(e, DBGERR_DEBUGGER, MICommandResultErrorMessage(cmd));
		MICommandFree(cmd);
		return e;
	}

	lines = CLIGetListInfo(cmd);

	// Convert the MI list of CLILineInfo into a dbglist_t
	for (MIListSet(lines); (line = (CLILineInfo *)MIListGet(lines)) != NULL; )
	{
		line_list = (dbglist_t *)Alloc(sizeof(dbglist_t));
		line_list->list_lno = line->line_no;
		line_list->list_line = strdup(line->line);
		line_list->list_next = (dbglist_t *)NULL;

		if (list == NULL)
		{
			list = line_list;
			list_ptr = line_list;
		} else
		{
			list_ptr->list_next = line_list;
			list_ptr = line_list;
		}
	}

	MIListFree(lines, (void (*)())CLILineInfoFree);
	MICommandFree(cmd);

	e = NewEvent(DBGEV_LIST);
	e->ev_list = list;

	return e;
}

/*
** Get source files from inferior.
*/
static dbgevent_t *
DbgGDBMIGetSources_fe(dbgfrontend_sw *fe_obj)
{
	dbgevent_t *	e;
	MICommand *		cmd;
	MISession *		sess = (MISession *)fe_obj->dbg_session;
	MIList *		sources;
	MISources *		source;
	dbgsources_t *	dbg_sources;
	dbgsources_t *	sources_ptr = NULL;
	dbgsources_t *	head = NULL;

	DPRINT("*** DbgGDBMIGetSources_fe\n");

	cmd = MIFileListExecSourceFiles();

	SendMICommand_fe(sess, cmd);
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

		dbg_sources = (dbgsources_t *)Alloc(sizeof(dbgsources_t));

		DPRINT("DbgGDBMIGetSources_fe: fullname is %s\n", source->fullname);

		// Ensure that this source file actually exists
		dbg_sources->sources_file = realpath(source->fullname, NULL);
		if (dbg_sources->sources_file == NULL)
		{
			Free(dbg_sources);
			continue;
		}

		dbg_sources->sources_next = (dbgsources_t *)NULL;

		if (head == NULL)
		{
			head = dbg_sources;
			sources_ptr = dbg_sources;
		} else
		{
			sources_ptr->sources_next = dbg_sources;
			sources_ptr = dbg_sources;
		}
	}

	MIListFree(sources, (void (*)())MISourcesFree);
	MICommandFree(cmd);

	e = NewEvent(DBGEV_SOURCES);
	e->ev_sources = head;

	return e;
}

static dbgevent_t *
DbgGDBMISetSourcePath_fe(dbgfrontend_sw *fe_obj, const char *path)
{
	MISession *		sess = (MISession *)fe_obj->dbg_session;
	char *			pathdirs[2];
	MICommand * 	cmd;
	MIString *		cmd_result;
	char *			res;
	dbgevent_t *	e;

	DPRINT("*** DbgGDBMISetSourcePath_fe\n");

	pathdirs[0] = const_cast<char*>(path);
	pathdirs[1] = NULL;

	cmd = MIEnvironmentDirectory(0, pathdirs);

	SendMICommand_fe(sess, cmd);
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
DbgGDBMIGetSourcePath_fe(dbgfrontend_sw *fe_obj)
{
	MISession *		sess = (MISession *)fe_obj->dbg_session;
	char *			pathdirs[1];
	MICommand * 	cmd;
	MIString *		cmd_result;
	char *			res;
	dbgevent_t *	e;

	DPRINT("*** DbgGDBMIGetSourcePath_fe\n");

	pathdirs[0] = NULL;

	cmd = MIEnvironmentDirectory(0, pathdirs);

	SendMICommand_fe(sess, cmd);
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
DbgGDBMIGetScope_fe(dbgfrontend_sw *fe_obj, const char *location)
{
	MISession *		sess = (MISession *)fe_obj->dbg_session;
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

	DPRINT("*** DbgGDBMIGetScope_fe\n");

	cmd = CLIInfoScope(location);

	SendMICommand_fe(sess, cmd);
	if (!MICommandResultOK(cmd))
	{
		EVENT_ERROR(e, DBGERR_DEBUGGER, MICommandResultErrorMessage(cmd));
		MICommandFree(cmd);
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
			DPRINT("var_name: %s\n", name);
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
	MIListFree(vars, (void (*)())Free);

	// Create the event to hold the var information
	e = NewEvent(DBGEV_VARS);
	e->ev_vars = result;

	// Now we need to get the type information based on the scope. First figure
	// out what function the scope referrs to by setting a breakpoint to get
	// the function name.

	cmd = MIBreakInsert(0, 0, NULL, 0, location, 0);
	SendMICommand_fe(sess, cmd);

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
		// If we can't get the breakpoint info, we error out
		// free existing event
		FreeEvent(e);
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

	MIListFree(bpts, (void (*)())MIBreakpointFree);
	MICommandFree(cmd);

	// Delete the breakpoint, we have the information we need
	cmd = MIBreakDelete(1, &bpid);
	SendMICommand_fe(sess, cmd);
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

		SendMICommand_fe(sess, cmd);
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

	// create the event with the var information
	e = NewEvent(DBGEV_VARS);
	e->ev_vars = result;

	return e;
}

static dbgevent_t *
DbgGDBMICheckStartupFunc_fe(dbgfrontend_sw *fe_obj)
{
	MISession *		sess = (MISession *)fe_obj->dbg_session;
	MICommand * 	cmd;
	dbgevent_t *	e;
	int				found = 0;

	DPRINT("*** DbgGDBMICheckStartupFunc_fe\n");

	// On Cray systems, we require PMI_Init to exist otherwise the pmi_attribs
	// file will never be created and the user will hang upon startup.
	cmd = CLIWhatis("PMI_Init");

	SendMICommand_fe(sess, cmd);
	if (MICommandResultOK(cmd))
	{
		found = 1;
	}
	MICommandFree(cmd);

	// We should also check for other startup function names as well
	if (!found)
	{
		cmd = CLIWhatis("MPI_Init");
		SendMICommand_fe(sess, cmd);
		if (MICommandResultOK(cmd))
		{
			found = 1;
		}
		MICommandFree(cmd);
	}

	if(!found){
		cmd = CLIWhatis("__pgas_runtime_error_checking");
		SendMICommand_fe(sess, cmd);
		if(MICommandResultOK(cmd)){
			found = 1;
		}
		MICommandFree(cmd);
	}

	if (!found)
	{
		cmd = CLIWhatis("PMI2_Init");
		SendMICommand_fe(sess, cmd);
		if (MICommandResultOK(cmd))
		{
			found = 1;
		}
		MICommandFree(cmd);
	}

	if (!found)
	{
		cmd = CLIWhatis("MPI_INIT_THREAD__");
		SendMICommand_fe(sess, cmd);
		if (MICommandResultOK(cmd))
		{
			found = 1;
		}
		MICommandFree(cmd);
	}

	if (!found)
	{
		cmd = CLIWhatis("MPI_INIT__");
		SendMICommand_fe(sess, cmd);
		if (MICommandResultOK(cmd))
		{
			found = 1;
		}
		MICommandFree(cmd);
	}

	if (!found)
	{
		cmd = CLIWhatis("mpi_init__");
		SendMICommand_fe(sess, cmd);
		if (MICommandResultOK(cmd))
		{
			found = 1;
		}
		MICommandFree(cmd);
	}

	if (!found)
	{
		cmd = CLIWhatis("mpi_init_thread__");
		SendMICommand_fe(sess, cmd);
		if (MICommandResultOK(cmd))
		{
			found = 1;
		}
		MICommandFree(cmd);
	}

	if (!found)
	{
		cmd = CLIWhatis("MPI_INIT_THREAD_");
		SendMICommand_fe(sess, cmd);
		if (MICommandResultOK(cmd))
		{
			found = 1;
		}
		MICommandFree(cmd);
	}

	if (!found)
	{
		cmd = CLIWhatis("MPI_INIT_");
		SendMICommand_fe(sess, cmd);
		if (MICommandResultOK(cmd))
		{
			found = 1;
		}
		MICommandFree(cmd);
	}

	if (!found)
	{
		cmd = CLIWhatis("mpi_init_thread_");
		SendMICommand_fe(sess, cmd);
		if (MICommandResultOK(cmd))
		{
			found = 1;
		}
		MICommandFree(cmd);
	}

	if (!found)
	{
		cmd = CLIWhatis("mpi_init_");
		SendMICommand_fe(sess, cmd);
		if (MICommandResultOK(cmd))
		{
			found = 1;
		}
		MICommandFree(cmd);
	}

	if (!found)
	{
		cmd = CLIWhatis("MPI::Init");
		SendMICommand_fe(sess, cmd);
		if (MICommandResultOK(cmd))
		{
			found = 1;
		}
		MICommandFree(cmd);
	}

	if (!found)
	{
		cmd = CLIWhatis("PMPI_Init");
		SendMICommand_fe(sess, cmd);
		if (MICommandResultOK(cmd))
		{
			found = 1;
		}
		MICommandFree(cmd);
	}

	if (!found)
	{
		cmd = CLIWhatis("start_pes");
		SendMICommand_fe(sess, cmd);
		if (MICommandResultOK(cmd))
		{
			found = 1;
		}
		MICommandFree(cmd);
	}

	if (!found)
	{
		cmd = CLIWhatis("shmem_init");
		SendMICommand_fe(sess, cmd);
		if (MICommandResultOK(cmd))
		{
			found = 1;
		}
		MICommandFree(cmd);
	}

	// If we get here, no known startup function was found.
	if (!found)
	{
		EVENT_ERROR(e, DBGERR_DEBUGGER, "Missing PMI_Init symbol!");
		return e;
	}

	// startup function was found
	return NewEvent(DBGEV_OK);;
}

