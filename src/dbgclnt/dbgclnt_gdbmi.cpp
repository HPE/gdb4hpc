/******************************************************************************\
 * GDB/MI frontend interface routines
 * TODO: This does not support asynchronous commands.
 *
 * Copyright 2013-2024 Hewlett Packard Enterprise Development LP.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Hewlett Packard Enterprise Development LP.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *
 ******************************************************************************/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <limits.h>
#include <stdlib.h>
#include <string.h>

#include "MI.h"
#include "dbgclnt_debugfe.h"
#include "defs.h"
#include "useful.h"

DbgFE::DbgFE(const char* gdbPath) {

    m_session = MISessionNew();
    
    // only set debug for gdbmi if compiled with debug mode
#ifdef DEBUG
    MISessionSetDebug(1);
#else
    MISessionSetDebug(0);
#endif

    // set the gdb path to use gdb-guard
    MISessionSetGDBPath(m_session, gdbPath ? gdbPath : "gdb-guard");

    // Get the base dir env var and setup data-dir if the env var is set
    char *env_base_dir = getenv(GUARD_INST_DIRECTORY);
    if (env_base_dir != NULL) {
        char *data_dir;
        Asprintf(&data_dir, "%s/share/gdb", env_base_dir);
        MISessionSetGDBDataDirectory(m_session, data_dir);
        Free(data_dir);
    }

    if (MISessionStartLocal(m_session, NULL) < 0) {
        return;
    }

    if (auto e = setup_gdbmi_environment()) {
        FreeEvent(e);
        return;
    }
}

/*
** Send the MI command
*/
void DbgFE::sendCmd(MICommand *cmd) {

    MISessionSendCommand(m_session, cmd);
    do {
        if (MISessionProgress(m_session) == -1) {
            if (auto buf = MICommandResultErrorMessage(cmd)) {
                DPRINT("SendMICommand: Debugger error: %s\n", buf);
                free(buf);
            } else {
                DPRINT("SendMICommand: Error from GDB, but no message\n");
            }

            break;
        }

        if (m_session->out_fd == -1) {
            DPRINT("SendMICommand: m_session->out_fd = -1\n");
            break;
        }

    } while (!MICommandCompleted(cmd));
}

dbgevent_t *DbgFE::setup_gdbmi_environment() {
    dbgevent_t *e;

    auto cmd = MIGDBSet("confirm", "off");
    sendCmd(cmd);
    if (!MICommandResultOK(cmd)) {
        MICommandFree(cmd);
        EVENT_ERROR(e, DBGERR_DEBUGGER, "Gdb set confirm command failed.");
        return e;
    }
    MICommandFree(cmd);

    // stop printing strings on the first null character
    cmd = MIGDBSet("print null-stop", "on");
    sendCmd(cmd);
    if (!MICommandResultOK(cmd)) {
        MICommandFree(cmd);
        EVENT_ERROR(e, DBGERR_DEBUGGER,
                    "Gdb set print null-stop command failed.");
        return e;
    }
    MICommandFree(cmd);

    // demangle c++ names
    cmd = MIGDBSet("print demangle", "on");
    sendCmd(cmd);
    if (!MICommandResultOK(cmd)) {
        MICommandFree(cmd);
        EVENT_ERROR(e, DBGERR_DEBUGGER,
                    "Gdb set print demangle command failed.");
        return e;
    }
    MICommandFree(cmd);

    cmd = MIGDBSet("print asm-demangle", "on");
    sendCmd(cmd);
    if (!MICommandResultOK(cmd)) {
        MICommandFree(cmd);
        EVENT_ERROR(e, DBGERR_DEBUGGER,
                    "Gdb set print asm-demangle command failed.");
        return e;
    }
    MICommandFree(cmd);

    // print derived types from the vtable
    cmd = MIGDBSet("print object", "on");
    sendCmd(cmd);
    if (!MICommandResultOK(cmd)) {
        MICommandFree(cmd);
        EVENT_ERROR(e, DBGERR_DEBUGGER, "Gdb set print object command failed.");
        return e;
    }
    MICommandFree(cmd);

    // limit backtrace size to 1000
    cmd = MIGDBSet("backtrace limit", "1000");
    sendCmd(cmd);
    if (!MICommandResultOK(cmd)) {
        MICommandFree(cmd);
        EVENT_ERROR(e, DBGERR_DEBUGGER,
                    "Gdb set backtrace limit command failed.");
        return e;
    }
    MICommandFree(cmd);

    // unwind signals triggered by gdb
    cmd = MIGDBSet("unwindonsignal", "on");
    sendCmd(cmd);
    if (!MICommandResultOK(cmd)) {
        MICommandFree(cmd);
        EVENT_ERROR(e, DBGERR_DEBUGGER,
                    "Gdb set unwindonsignal command failed.");
        return e;
    }
    MICommandFree(cmd);

    return NULL;
}

/*
** Set a.out inferior file for symbol reading.
*/
dbgevent_t *DbgFE::setFile(const char *file) {
    dbgevent_t *e;

    DPRINT("*** DbgFESetFile_fe\n");

    auto cmd = MIFileExecAndSymbols(file);
    sendCmd(cmd);
    if (!MICommandResultOK(cmd)) {
        EVENT_ERROR(e, DBGERR_DEBUGGER, MICommandResultErrorMessage(cmd));
        MICommandFree(cmd);
        return e;
    }

    MICommandFree(cmd);

    return NewEvent(DBGEV_OK);
}

dbgevent_t *DbgFE::quit() {
    DPRINT("*** DbgFEQuit_fe\n");

    auto cmd = MIGDBExit();
    sendCmd(cmd);
    MICommandFree(cmd);

    return NewEvent(DBGEV_OK);
    ;
}

/*
** List source code.
**
** GDB does not have a MI command for this; we use the CLI command.
*/
dbgevent_t *DbgFE::list(const char* threads, const char *str) {
    dbgevent_t *e;
    MICommand *cmd;
    MIList *lines;
    CLILineInfo *line;
    dbglist_t *line_list;
    dbglist_t *list_ptr = NULL;
    dbglist_t *list = NULL;

    DPRINT("*** DbgFE::list_fe\n");

    cmd = CLIList(str);
    sendCmd(cmd);
    if (!MICommandResultOK(cmd)) {
        EVENT_ERROR(e, DBGERR_DEBUGGER, MICommandResultErrorMessage(cmd));
        MICommandFree(cmd);
        return e;
    }

    lines = CLIGetListInfo(cmd);

    // Convert the MI list of CLILineInfo into a dbglist_t
    for (MIListSet(lines); (line = (CLILineInfo *)MIListGet(lines)) != NULL;) {
        line_list = (dbglist_t *)Alloc(sizeof(dbglist_t));
        line_list->list_lno = line->line_no;
        line_list->list_line = strdup(line->line);
        line_list->list_threads = StrdupOpt(threads);
        line_list->list_next = (dbglist_t *)NULL;

        if (list == NULL) {
            list = line_list;
            list_ptr = line_list;
        } else {
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

dbgevent_t* DbgFE::list(const Location& loc) {
    return list(loc.threads.get(), loc.loc_str.get());
}

/*
** Get source files from inferior.
*/
dbgevent_t *DbgFE::getSources() {
    dbgevent_t *e;
    MIList *sources;
    MISources *source;
    dbgsources_t *dbg_sources;
    dbgsources_t *sources_ptr = NULL;
    dbgsources_t *head = NULL;

    DPRINT("*** DbgFE::getSources_fe\n");

    auto cmd = MIFileListExecSourceFiles();

    sendCmd(cmd);
    if (!MICommandResultOK(cmd)) {
        EVENT_ERROR(e, DBGERR_DEBUGGER, MICommandResultErrorMessage(cmd));
        MICommandFree(cmd);
        return e;
    }

    sources = MIGetFileListExecSourceFilesInfo(cmd);

    // Convert the MI list of MISources into a dbgsources_t
    for (MIListSet(sources);
         (source = (MISources *)MIListGet(sources)) != NULL;) {
        if (source->fullname == NULL) continue;

        dbg_sources = (dbgsources_t *)Alloc(sizeof(dbgsources_t));

        DPRINT("DbgFE::getSources_fe: fullname is %s\n", source->fullname);

        // Ensure that this source file actually exists
        dbg_sources->sources_file = realpath(source->fullname, NULL);
        if (dbg_sources->sources_file == NULL) {
            Free(dbg_sources);
            continue;
        }

        dbg_sources->sources_next = (dbgsources_t *)NULL;

        if (head == NULL) {
            head = dbg_sources;
            sources_ptr = dbg_sources;
        } else {
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

dbgevent_t *DbgFE::setSourcePath(DbgDirOp op, const char *path) {
    DPRINT("*** DbgFE::setSourcePath_fe\n");

    if (op == DbgDirOp::ADD || op == DbgDirOp::SET) {
        auto reset = (op == DbgDirOp::SET || !path || !strlen(path) ? 1 : 0);

        char* pathdirs[2] = { (reset ? nullptr : const_cast<char *>(path)), nullptr };

        auto cmd = MIEnvironmentDirectory(reset, pathdirs);

        sendCmd(cmd);
        if (!MICommandResultOK(cmd)) {
            MICommandFree(cmd);
            return NewErrorEvent(DBGERR_DEBUGGER,
                                 "Gdb -environment-directory command failed.");
        }

        auto cmd_result = MIGetEnvironmentDirectoryInfo(cmd);

        auto e = NewEvent(DBGEV_SOURCEPATH);
        auto res = MIStringToCString(cmd_result);
        e->ev_source_path = (res == NULL) ? NULL : strdup(res);

        MICommandFree(cmd);
        MIStringFree(cmd_result);

        return e;
    }

    if (op == DbgDirOp::SUB) {
        auto cmd = MIGDBSet("substitute-path", path);
        sendCmd(cmd);
        if (!MICommandResultOK(cmd)) {
            MICommandFree(cmd);
            return NewErrorEvent(DBGERR_DEBUGGER,
                                 "Gdb set substitute-path command failed.");
        }
        MICommandFree(cmd);
    } else if (op == DbgDirOp::UNSET_SUB) {
        
        auto cmd = MICommandNew("unset", MIResultRecordDONE);
        MICommandAddOption(cmd, "substitute-path", path && strlen(path) ? path : nullptr);
        sendCmd(cmd);
        if (!MICommandResultOK(cmd)) {
            MICommandFree(cmd);
            return NewErrorEvent(DBGERR_DEBUGGER,
                                 "Gdb unset substitute-path command failed.");
        }
        MICommandFree(cmd);
    }

    // Op DbgDirOp::SHOW_SUB just falls through to here.
    // All operations return the new path
    auto cmd = MICommandNew("-gdb-show", MIResultRecordDONE);
    MICommandAddOption(cmd, "substitute-path", nullptr);
    sendCmd(cmd);
    if (!MICommandResultOK(cmd)) {
        MICommandFree(cmd);
        return NewErrorEvent(DBGERR_DEBUGGER,
                             "Gdb set substitute-path command failed.");
    }
    auto e = NewEvent(DBGEV_SOURCEPATH);
    auto res = MICommandConsoleOOBMessages(cmd);
    e->ev_source_path = (res == NULL) ? strdup("n/a") : strdup(res);

    MICommandFree(cmd);
    
    return e;
}

dbgevent_t *DbgFE::getSourcePath() {
    char *pathdirs[1];
    MICommand *cmd;
    MIString *cmd_result;
    char *res;
    dbgevent_t *e;

    DPRINT("*** DbgFE::getSourcePath_fe\n");

    pathdirs[0] = NULL;

    cmd = MIEnvironmentDirectory(0, pathdirs);

    sendCmd(cmd);
    if (!MICommandResultOK(cmd)) {
        MICommandFree(cmd);
        EVENT_ERROR(e, DBGERR_DEBUGGER,
                    "Gdb -environment-directory command failed.");
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

dbgevent_t *DbgFE::getScope(const char *location) {
    MICommand *cmd;
    MIList *vars;
    char *name;
    dbgvars_t *result = NULL;
    dbgvars_t **vp = NULL;
    MIList *bpts;
    MIBreakpoint *bpt;
    int bpid;
    char *bpfunc;
    char *var_str;
    char *type;
    dbgevent_t *e;

    DPRINT("*** DbgFE::getScope_fe\n");

    cmd = CLIInfoScope(location);

    sendCmd(cmd);
    if (!MICommandResultOK(cmd)) {
        EVENT_ERROR(e, DBGERR_DEBUGGER, MICommandResultErrorMessage(cmd));
        MICommandFree(cmd);
        return e;
    }

    // Get the names of the vars
    if ((vars = CLIGetVariablesFromInfoScopeInfo(cmd)) == NULL) {
        EVENT_ERROR(e, DBGERR_DEBUGGER, MICommandResultErrorMessage(cmd));
        MICommandFree(cmd);
        return e;
    }
    MICommandFree(cmd);

    // Create the dbgvars_t for each of the named variables
    vp = &result;
    for (MIListSet(vars); (name = (char *)MIListGet(vars)) != NULL;) {
        // create the new var
        *vp = (dbgvars_t *)Alloc(sizeof(dbgvars_t));
        memset(*vp, 0, sizeof(dbgvars_t));

        // set the name
        if (name != NULL) {
            DPRINT("var_name: %s\n", name);
            (*vp)->var_name = strdup(name);
        } else {
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

    cmd = MIBreakInsert(0, 0, 0, NULL, 0, location, 0);
    sendCmd(cmd);

    if (!MICommandResultOK(cmd)) {
        // free existing event
        FreeEvent(e);
        EVENT_ERROR(e, DBGERR_DEBUGGER, MICommandResultErrorMessage(cmd));
        MICommandFree(cmd);
        return e;
    }

    if ((bpts = MIBreakpointGetBreakInsertInfo(cmd)) == NULL) {
        // If we can't get the breakpoint info, we error out
        // free existing event
        FreeEvent(e);
        EVENT_ERROR(e, DBGERR_DEBUGGER,
                    "Could not obtain breakpoint information for scope.");
        MICommandFree(cmd);
        return e;
    }

    MIListSet(bpts);
    bpt = (MIBreakpoint *)MIListGet(bpts);

    bpid = bpt->number;

    // Make sure that there is a function, otherwise whatis will fail. We need
    // the function scope in order for this to work
    if (bpt->func == NULL) {
        // free existing event
        FreeEvent(e);
        EVENT_ERROR(e, DBGERR_DEBUGGER,
                    "Could not obtain function locale for scope.");
        MICommandFree(cmd);
        return e;
    }

    bpfunc = strdup(bpt->func);

    MIListFree(bpts, (void (*)())MIBreakpointFree);
    MICommandFree(cmd);

    // Delete the breakpoint, we have the information we need
    cmd = MIBreakDelete(1, &bpid);
    sendCmd(cmd);
    // We don't care if the delete breakpoint failed. I think this is okay to
    // do since it won't cause anything to break. This assumption might be
    // wrong though.
    MICommandFree(cmd);

    // Now loop through the vars and get the type for each name
    for (vp = &result; *vp != NULL; vp = &(*vp)->var_next) {
        Asprintf(&var_str, "%s::%s", bpfunc, (*vp)->var_name);
        cmd = CLIWhatis(var_str);
        Free(var_str);

        sendCmd(cmd);
        if (!MICommandResultOK(cmd)) {
            // If there was a problem, continue
            MICommandFree(cmd);
            continue;
        }

        if ((type = CLIGetWhatisInfo(cmd)) == NULL) {
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

dbgevent_t* DbgFE::getFunctionSymbolNames() {
    auto cmd = MISymbolInfoFunctions(
        /* include nondebug */ true,
        /* type filter */ nullptr,
        /* name filter */ nullptr, 
        /* max results */ -1
    );
    auto cmd_cleanup = OnScopeExit([&](){ MICommandFree(cmd); });
    dbgevent_t* result = nullptr;

    sendCmd(cmd);
    if (!MICommandResultOK(cmd)) {
        EVENT_ERROR(result, DBGERR_DEBUGGER, MICommandResultErrorMessage(cmd));
        return result;
    }

    result = NewEvent(DBGEV_SYMBOLS);

    auto* mi_symbols = MIGetSymbolInfo(cmd);
    auto mi_symbols_cleanup = OnScopeExit([&](){ MISymbolInfoFree(mi_symbols); });
    if (!mi_symbols) {
        FreeEvent(result);
        EVENT_ERROR(result, DBGERR_DEBUGGER, "Failed to extract symbol info");
        return result;
    }

    auto dbg_symbols = new dbgsymbols_t{};

    MIListSet(mi_symbols->debug);
    for (MIFileSymbols* fs; (fs = static_cast<MIFileSymbols*>(MIListGet(mi_symbols->debug))); ) {
        MIListSet(fs->symbols);
        for (MISymbol* s; (s = static_cast<MISymbol*>(MIListGet(fs->symbols))); ) {
            dbg_symbols->debug.emplace_back(s->name);
        }
    }

    MIListSet(mi_symbols->nondebug);
    for (MISymbol* s; (s = static_cast<MISymbol*>(MIListGet(mi_symbols->nondebug))); ) {
        dbg_symbols->nondebug.emplace_back(s->name);
    }

    result->ev_u.ev_u_symbols = dbg_symbols;

    return result;
}

static const std::array<std::string, 16> parallel_entry_pt_strs
{
    "PMI_Init",
    "MPI_Init",
    "__pgas_runtime_error_checking",
    "PMI2_Init",
    "MPI_INIT_THREAD__",

    "MPI_INIT__",
    "mpi_init__",
    "mpi_init_thread__",
    "MPI_INIT_THREAD_",
    "MPI_INIT_",

    "mpi_init_thread_",
    "mpi_init_",
    "MPI::Init",
    "PMPI_Init",
    "start_pes",

    "shmem_init"
};

dbgevent_t *DbgFE::checkStartupFunc() {
    DPRINT("*** DbgFECheckStartupFunc_fe\n");

    // set the lang to c. We always want to do this!!
    auto cmd = MIGDBSet("lang", "c");
    sendCmd(cmd);
    if (!MICommandResultOK(cmd)) {
        MICommandFree(cmd);
        return NewErrorEvent(DBGERR_DEBUGGER, "Gdb set lang command failed.");
    }
    MICommandFree(cmd);

    auto foundBP = false;
    
    for (auto&& str: parallel_entry_pt_strs) {
        cmd = MIBreakInsert(0, 0, 0, NULL, 0, str.c_str(), 0);
        sendCmd(cmd);
        // We expect many of these to fail - only one has to exist
        if (!MICommandResultOK(cmd)) {
            MICommandFree(cmd);
            continue;
        }
        // undo the breakpoint
        auto bpts = MIBreakpointGetBreakInsertInfo(cmd);
        MIListSet(bpts);
        for (MIBreakpoint* bp; (bp = (MIBreakpoint *)MIListGet(bpts)); ) {
            auto id = bp->number;
            auto delCmd = MIBreakDelete(1, &id);
            sendCmd(delCmd);
            if (!MICommandResultOK(delCmd)) {
                MICommandFree(delCmd);
                return NewErrorEvent(DBGERR_NOBP, "Delete internal breakpoint failed.");
            }
            MICommandFree(delCmd);
        }

        MIListFree(bpts, (void (*)()) MIBreakpointFree);
        MICommandFree(cmd);
        foundBP = true;
        break;
    }

    // set the lang back to auto. We always want to do this!!
    cmd = MIGDBSet("lang", "auto");
    sendCmd(cmd);
    if (!MICommandResultOK(cmd)) {
        MICommandFree(cmd);
        return NewErrorEvent(DBGERR_DEBUGGER, "Gdb set lang command failed.");
    }
    MICommandFree(cmd);

    if (!foundBP) {
        // If we get here, no known startup function was found.
        return NewErrorEvent(DBGERR_DEBUGGER, "Missing PMI or MPI entrypoint symbol!");
    }
    
    // startup function was found
    return NewEvent(DBGEV_OK);
}

void DbgFE::addListStateLocation(const Bitset *set, const char* threads, const char *str) {
    if (!set || !str) return;
    
    Location loc = {
        .set = set->clone(),
        .threads = std::unique_ptr<char,decltype(&std::free)>(StrdupOpt(threads), &std::free),
        .loc_str = std::unique_ptr<char,decltype(&std::free)>(strdup(str), &std::free),
        .count = 0};

    m_listLocs.push_back(std::move(loc));
    m_listIsPending = true;
}

void DbgFE::clearListState() {
    m_listLocs.clear();
    m_listIsPending = false;
}

DbgFE::~DbgFE() { if (m_session) MISessionFree(m_session); }

