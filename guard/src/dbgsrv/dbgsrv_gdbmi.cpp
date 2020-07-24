/*********************************************************************************\
 * GDB interface routines
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * Copyright 2010-2020 Cray Inc.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Cray Inc.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *
 *********************************************************************************/

/*
** The Guard GDB/MI interface.
** Now uses the Eclipse SDM libmi interface.
*/
#include <string>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <arpa/inet.h>
#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <math.h>
#include <netdb.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/param.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string>
#include <fstream>
#include <map>
#include <sstream>
#include <vector>
#include <regex>
#include <iomanip>

#include "MI.h"
#include "aif.h"
#include "common_tools_be.h"
#include "dbgcomm_info.h"
#include "dbgevent.h"
#include "dbgfn.h"
#include "dbgsrv_defs.h"
#include "dbgsrv_libmi2aif.h"
#include "dbgsrv_p2p.h"
#include "dbgsrv_proto.h"
#include "dbgsrv_util.h"
#include "defs.h"
#include "expr.h"
#include "mparch.h"
#include "useful.h"
#include "common_tools_be.h"

static MICommand *CLIBypassc(int cmd_argc, const char *cmd_argv[]) {
    return CLIBypass(cmd_argc, const_cast<char **>(cmd_argv));
}

// Enum for supported gdb versions
enum class GdbVersion { GDB_81, CUDA_101, ROC_GDB};
static GdbVersion gdb_vers = GdbVersion::GDB_81;

// functions with external visibility
void SendMICommand(MISession *, MICommand *);

static MISession *GDBMISession;
static dbgstat Status;
static int Pid = 0;
static int MPArch;
static char *RunArgs = NULL;
static dbgbp_t *CurrBP = (dbgbp_t *)NULL;
static list_t *BP;

static list_t *main_entry_BP;
static list_t *parallel_entry_BP;

static list_t *exit_BP;
static const char *ProgMain;
static int AsynchEventWaiting = 0;
static MIEvent *PendingEvent = NULL;
static int DirectCmdAsyncRestore = 0;

#ifndef ALPS
// These are for sending the target output to the fe
static int WRITE_fd;
static int dbgsrv_fd;
#endif

extern appInfo_t other_app;

/* dbgbackend_sw functions */
namespace {
class DbgGdbMI : public dbgbackend_sw {
public:
    virtual dbgevent_t *init(int *dfd, int *iofd, char **env) override;
    virtual dbgevent_t *startup() override;
    virtual int read(int) override;
    virtual bool bufferReady() override;
    virtual int progioHook(int) override;   //!!! careful - potentially unimplemented
    virtual dbgevent_t *setLineBreak(const int allowPending, const char *file, int lno, const char* threadId, const dbgcomm_info *commInfo) override;
    virtual dbgevent_t *setFuncBreak(const int allowPending, const char *file, const char *func, const char* threadId, const dbgcomm_info *commInfo) override;
    virtual dbgevent_t *setWatch(int type, const char *expr, const dbgcomm_info *commInfo) override;
    virtual dbgevent_t *deleteBreak(int bid) override;
    virtual dbgevent_t *disableBreak(int bid) override;
    virtual dbgevent_t *enableBreak(int bid, bool once, bool del) override;
    virtual dbgevent_t *lastBreak() override;
    virtual dbgevent_t *list(const char *str) override;
    virtual dbgevent_t *getSources() override;
    virtual dbgevent_t *go(bool all) override;
    virtual dbgevent_t *finishFunc() override;
    virtual dbgevent_t *setArgs(const char *arg) override; // set run arguments
    virtual dbgevent_t *step(int const, bool in) override;
    virtual dbgevent_t *kill() override;
    virtual dbgevent_t *moveFrame(int count, bool up, const char* threads) override;

    // If count is not zero then return only that many frames. If count is zero then
    // return an unlimited number of frames. If count is negative return the
    // outermost frames.
    virtual dbgevent_t *showFrame(int count, const char* threads) override;

    virtual dbgevent_t *getFrame(const char* threads) override;
    virtual dbgevent_t *setVar(const char *var, AIF *a) override;
    virtual dbgevent_t *evalExpr(const char *expr, long exprIdx, const char* threads) override;
    virtual dbgevent_t *getType(const char *var, bool isType) override;
    virtual dbgevent_t *getLocation() override;   //!! potentially unimplemented
    virtual dbgevent_t *getVars(const char *file) override;
    virtual dbgevent_t *getScope(const char *location) override;
    virtual dbgevent_t *getStatus(bool getLocation) override;
    virtual dbgevent_t *quit(bool detach) override;
    virtual dbgevent_t *interrupt(bool all) override;
    virtual dbgevent_t *invoke(const char *host, int cp, const char *proto, const char *prog,
                               const char *args, char **env, int arch) override;
    virtual dbgevent_t *attach(int pid, const char *prog, const char *args, char **env, int arch) override;
    virtual void finalize() override;
    virtual bool isReadyForCmd() override;
    virtual bool isOk() override;
    virtual bool isStopped() override;
    virtual dbgevent_t *setDebuggerVar(const char *var, AIF *val) override;
    virtual dbgevent_t *setSourcePath(const char *path) override;
    virtual dbgevent_t *getSourcePath() override;
    virtual dbgevent_t *getDebuggerVar(const char *var) override;

    // Pass command directly to gdb/mi through console command
    virtual dbgevent_t *directCmd(int argc, char **argv) override;
    virtual dbgevent_t *openSocket(const char *opset_name, int opset_size) override;
    virtual dbgevent_t *closeSocket() override;
    virtual dbgevent_t *setThread(const char* threadId) override;
    virtual dbgevent_t *getThreadInfo(const char* threadId) override;
    virtual dbgevent_t *setThreadName(const char* name) override;
    virtual dbgevent_t *findThread(const char* pattern) override;
    virtual dbgevent_t *threadApply(const char* op) override;
    virtual dbgevent_t *setSchedulerLocking(const char* op) override;
    virtual dbgevent_t *getSchedulerLocking() override;
    virtual dbgevent_t *setNonStop(const char* op) override;
    virtual dbgevent_t *getNonStop() override;

private:
    dbgevent_t *initDbg(const char *prog, const char *args);
    dbgevent_t *initDbgAttach(int pid, const char *prog);
    dbgevent_t* setBreak(const int allowPending, const std::string& where,
                         const char* threadSet, const dbgcomm_info* commInfo);
    dbgevent_t* evalExprWithThreads(const char *expr, const char* threads);
    std::vector<int> getThreadSet(const std::string& threadId, std::string* suffix, std::string& error);
    std::string parseThreadApplyOp(const std::string& op, bool& isSilent, bool& isQuiet,
                                   bool &isVerbose);

    std::vector<int> getMultiThreadBreaks(int bid, bool remove);

    template<typename FUNC> void applyMultiThreadBreak(int bid, bool remove, FUNC&& op) {
        auto ids = getMultiThreadBreaks(bid,remove);
        for (auto it = ids.rbegin(); it != ids.rend(); ++it) op(*it);
    }
    std::map<int,std::vector<int>> m_multiThreadBreaks;
};
}

static dbgevent_t *WaitEvent(dbgevent_t *e);
static dbgevent_t *CheckBreak(MICommand *, const dbgcomm_info *);
static void Finish(int);
static void event_callback(MIEvent *);
static void console_callback(char *);
static void log_callback(char *);
static void target_callback(char *);
static void notification_callback(char *, MIList *);
static dbgbp_t *get_dbgbp(MICommand *);
static dbgevent_t *delete_internal_breakpoints(list_t *);
static dbgevent_t *setup_gdbmi_environment(void);
static dbgevent_t *setup_gdbmi_signals(void);
static dbgevent_t *setup_gdbmi_entry_points(void);
static dbgevent_t *setup_gdbmi_main_entry_point(void);
static dbgevent_t *setup_gdbmi_exit_points(void);
static dbgevent_t *ResetBeforeDirectCmd(void);
static dbgevent_t *RestoreAfterDirectCmd(void);
static dbgevent_t *threadEventToFrame(dbgevent_t* threadEvent);
static dbgframe_t *MakeFrameFromMIFrame(MIFrame* miFrame, bool keepAddr=true);

static std::string threadIdsToString(const std::vector<int>& ids);

/* hashing and slicing functions */
static dbgevent_t *PerformExtEvalExpr(const char *);

/* debugger definition */
static DbgGdbMI DbgBackendGDBMI;

dbgbackend_sw* DbgGdbMIGetSingleton() { return &DbgBackendGDBMI; }

static char* MICommandOutputString(MICommand* cmd)
{
    MIListSet(cmd->output->rr->results);
    for (MIResult *	r; (r = (MIResult *)MIListGet(cmd->output->rr->results)); ) {
        if (r->value != NULL) {
            MIString* s = MIValueToString(r->value);
            return strdup(MIStringToCString(s));
        }
    }
    return nullptr;
}

/* internal static functions */

static dbgbp_t *get_dbgbp(MICommand *cmd) {
    DVPRINT("*** Dbg %s, %s\n", __FILE__, __func__);
    dbgbp_t *bp;
    MIBreakpoint *bpt;
    MIList *bpts;

    if (cmd == NULL) return NULL;

    bpts = MIBreakpointGetBreakInsertInfo(cmd);

    if (bpts == NULL) {
        return NULL;
    }

    MIListSet(bpts);
    bpt = (MIBreakpoint *)MIListGet(bpts);

    bp = NewBP(bpt->number);

    if (bpt->isWpt) {
        if (bpt->isWWpt) {
            if (bpt->isRWpt)
                bp->bp_type = DBGBP_AWATCH_TYPE;
            else
                bp->bp_type = DBGBP_WATCH_TYPE;
        } else if (bpt->isRWpt) {
            bp->bp_type = DBGBP_RWATCH_TYPE;
        } else {
            bp->bp_type = DBGBP_WATCH_TYPE;
        }
    } else {
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
    if (bpt->file != NULL) bp->bp_loc.loc_file = strdup(bpt->file);
    if (bpt->func != NULL) bp->bp_loc.loc_func = strdup(bpt->func);
    if (bpt->address != NULL) bp->bp_loc.loc_addr = strdup(bpt->address);

    DVPRINT("get_dbgbp: bp_id is %d\n", bp->bp_id);
    //DVPRINT("%s: DumpingBP:", __func__);
    //DumpBP(bp);

    MIListFree(bpts, (void (*)())MIBreakpointFree);

    return bp;
}

static dbgevent_t *delete_internal_breakpoints(list_t *bplist) {
    DVPRINT("*** Dbg %s, %s\n", __FILE__, __func__);
    dbgevent_t *e;
    MICommand *cmd;
    dbgbp_t *bp;

    SetList(bplist);

    while ((bp = (dbgbp_t *)GetListElement(bplist)) != (dbgbp_t *)NULL) {
        // send the delete command
        cmd = MIBreakDelete(1, &bp->bp_id);
        SendMICommand(GDBMISession, cmd);
        if (!MICommandResultOK(cmd)) {
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

static MICommand *_MIGDBSet(const char *var, const char *val) {
    return MIGDBSet(const_cast<char *>(var), const_cast<char *>(val));
}

static void setup_pretty_printing() {
    DVPRINT("*** Dbg %s, %s\n", __FILE__, __func__);
    char* tmpDir = cti_be_getTmpDir();
    if (!tmpDir) {
        DVPRINT("could not get tmpDir");
        return;
    }

    std::string scriptFile = std::string(tmpDir) + "/initpretty" +
                                std::to_string(getpid());
    free(tmpDir);

    std::ofstream script(scriptFile.c_str());
    if (!script) {
        DVPRINT("could not open %s\n", scriptFile.c_str());
        return;
    }

    script << "python\n";
    script << "import sys\n";
    //script << "sys.path.insert(0,'" << prettyDir << "')\n";  (see DbgMRNInvoke)
    script << "from libstdcxx.v6.printers import register_libstdcxx_printers\n";
    script << "register_libstdcxx_printers (None)\n";
    script << "end\n";

    script.close();

    const char* cmd_args[2] = {"source", scriptFile.c_str() };
    MICommand* cmd = CLIBypassc(2, cmd_args);
    SendMICommand(GDBMISession, cmd);
    if (!MICommandResultOK(cmd)) {
        DVPRINT("Loading pretty printers failed\n");
    }
    remove(scriptFile.c_str());
    MICommandFree(cmd);
}

static dbgevent_t *setup_gdbmi_environment(void) {
    DVPRINT("*** Dbg %s, %s\n", __FILE__, __func__);
    dbgevent_t *e;
    MICommand *cmd;

#if 0
	if (debug_flag)
	{
		// set debug infrun mode on
		cmd = _MIGDBSet("debug infrun", "1");
		SendMICommand(GDBMISession, cmd);
		if (!MICommandResultOK(cmd))
		{
			MICommandFree(cmd);
			EVENT_ERROR(e, DBGERR_DEBUGGER, "Gdb set debug infrun command failed.");
			return e;
		}
		MICommandFree(cmd);

		// set debug lin-lwp mode on
		cmd = _MIGDBSet("debug lin-lwp", "1");
		SendMICommand(GDBMISession, cmd);
		if (!MICommandResultOK(cmd))
		{
			MICommandFree(cmd);
			EVENT_ERROR(e, DBGERR_DEBUGGER, "Gdb set debug lin-lwp command failed.");
			return e;
		}
		MICommandFree(cmd);

		// set debug lin-lwp-async mode on
		cmd = _MIGDBSet("debug lin-lwp-async", "1");
		SendMICommand(GDBMISession, cmd);
		if (!MICommandResultOK(cmd))
		{
			MICommandFree(cmd);
			EVENT_ERROR(e, DBGERR_DEBUGGER, "Gdb set debug lin-lwp-async command failed.");
			return e;
		}
		MICommandFree(cmd);
	}
#endif

    cmd = _MIGDBSet("confirm", "off");
    SendMICommand(GDBMISession, cmd);
    if (!MICommandResultOK(cmd)) {
        MICommandFree(cmd);
        EVENT_ERROR(e, DBGERR_DEBUGGER, "Gdb set confirm command failed.");
        return e;
    }
    MICommandFree(cmd);

    // turn history off
    cmd = _MIGDBSet("history size", "0");
    SendMICommand(GDBMISession, cmd);
    if (!MICommandResultOK(cmd)) {
        MICommandFree(cmd);
        EVENT_ERROR(e, DBGERR_DEBUGGER, "Gdb set history size command failed.");
        return e;
    }
    MICommandFree(cmd);

    if (gdb_vers != GdbVersion::ROC_GDB) {
        // do not open source files
        cmd = _MIGDBSet("source open", "off");
        SendMICommand(GDBMISession, cmd);
        if (!MICommandResultOK(cmd)) {
            MICommandFree(cmd);
            EVENT_ERROR(e, DBGERR_DEBUGGER, "Gdb set source open command failed.");
            return e;
        }
        MICommandFree(cmd);
    }
    // we need gdb to set print elements and print repeats to 0 so
    // that when we resolve the data it gives us the entire contents
    // of the data type.
    cmd = _MIGDBSet("print elements", "0");
    SendMICommand(GDBMISession, cmd);
    if (!MICommandResultOK(cmd)) {
        MICommandFree(cmd);
        EVENT_ERROR(e, DBGERR_DEBUGGER,
                    "Gdb set print elements command failed.");
        return e;
    }
    MICommandFree(cmd);

    cmd = _MIGDBSet("print repeats", "0");
    SendMICommand(GDBMISession, cmd);
    if (!MICommandResultOK(cmd)) {
        MICommandFree(cmd);
        EVENT_ERROR(e, DBGERR_DEBUGGER,
                    "Gdb set print repeats command failed.");
        return e;
    }
    MICommandFree(cmd);

    // stop printing strings on the first null character
    cmd = _MIGDBSet("print null-stop", "on");
    SendMICommand(GDBMISession, cmd);
    if (!MICommandResultOK(cmd)) {
        MICommandFree(cmd);
        EVENT_ERROR(e, DBGERR_DEBUGGER,
                    "Gdb set print null-stop command failed.");
        return e;
    }
    MICommandFree(cmd);

    // demangle c++ names
    cmd = _MIGDBSet("print demangle", "on");
    SendMICommand(GDBMISession, cmd);
    if (!MICommandResultOK(cmd)) {
        MICommandFree(cmd);
        EVENT_ERROR(e, DBGERR_DEBUGGER,
                    "Gdb set print demangle command failed.");
        return e;
    }
    MICommandFree(cmd);

    cmd = _MIGDBSet("print asm-demangle", "on");
    SendMICommand(GDBMISession, cmd);
    if (!MICommandResultOK(cmd)) {
        MICommandFree(cmd);
        EVENT_ERROR(e, DBGERR_DEBUGGER,
                    "Gdb set print asm-demangle command failed.");
        return e;
    }
    MICommandFree(cmd);

    // print derived types from the vtable
    cmd = _MIGDBSet("print object", "on");
    SendMICommand(GDBMISession, cmd);
    if (!MICommandResultOK(cmd)) {
        MICommandFree(cmd);
        EVENT_ERROR(e, DBGERR_DEBUGGER, "Gdb set print object command failed.");
        return e;
    }
    MICommandFree(cmd);

    if (gdb_vers != GdbVersion::ROC_GDB) {
        // limit strings to 200 characters
        cmd = _MIGDBSet("print characters", "200");
        SendMICommand(GDBMISession, cmd);
        if (!MICommandResultOK(cmd)) {
            MICommandFree(cmd);
            EVENT_ERROR(e, DBGERR_DEBUGGER,
                        "Gdb set print characters command failed.");
            return e;
        }
        MICommandFree(cmd);
    }

    // don't allow pending breakpoints
    cmd = _MIGDBSet("breakpoint pending", "off");
    SendMICommand(GDBMISession, cmd);
    if (!MICommandResultOK(cmd)) {
        MICommandFree(cmd);
        EVENT_ERROR(e, DBGERR_DEBUGGER,
                    "Gdb set breakpoint pending command failed.");
        return e;
    }
    MICommandFree(cmd);

    // limit backtrace size to 1000
    cmd = _MIGDBSet("backtrace limit", "1000");
    SendMICommand(GDBMISession, cmd);
    if (!MICommandResultOK(cmd)) {
        MICommandFree(cmd);
        EVENT_ERROR(e, DBGERR_DEBUGGER,
                    "Gdb set backtrace limit command failed.");
        return e;
    }
    MICommandFree(cmd);

    // unwind signals triggered by gdb
    cmd = _MIGDBSet("unwindonsignal", "on");
    SendMICommand(GDBMISession, cmd);
    if (!MICommandResultOK(cmd)) {
        MICommandFree(cmd);
        EVENT_ERROR(e, DBGERR_DEBUGGER,
                    "Gdb set unwindonsignal command failed.");
        return e;
    }
    MICommandFree(cmd);

    // set can-use-hw-watchpoints to 1
    cmd = _MIGDBSet("can-use-hw-watchpoints", "1");
    SendMICommand(GDBMISession, cmd);
    if (!MICommandResultOK(cmd)) {
        MICommandFree(cmd);
        EVENT_ERROR(e, DBGERR_DEBUGGER,
                    "Gdb set can-use-hw-watchpoints command failed.");
        return e;
    }
    MICommandFree(cmd);

    // Need to set an alternative system root for /dsl and dso lookup if the
    // /dsl directory exists.
    struct stat st;

    if (stat("/dsl", &st) == 0) {
        // The directory exists
        cmd = _MIGDBSet("solib-absolute-prefix", "/dsl");
        SendMICommand(GDBMISession, cmd);
        if (!MICommandResultOK(cmd)) {
            MICommandFree(cmd);
            EVENT_ERROR(e, DBGERR_DEBUGGER,
                        "Gdb set solib-absolute-prefix command failed.");
            return e;
        }
        MICommandFree(cmd);
    }

    if (gdb_vers != GdbVersion::ROC_GDB) {
        // If running with gpu's, turn upc off. If we don't do this, things will
        // break
        if (gpu_flag) {
            cmd = _MIGDBSet("upcsingle", "1");
            SendMICommand(GDBMISession, cmd);
            if (!MICommandResultOK(cmd)) {
                MICommandFree(cmd);
                EVENT_ERROR(e, DBGERR_DEBUGGER,
                            "Gdb set upcsingle command failed.");
                return e;
            }
            MICommandFree(cmd);
        }
    }

    // XXX: This actual does do something with the Allinea branch of gdb.
    // DO NOT REMOVE! If removed, the backend will hang on gdb fatal errors.
    cmd = _MIGDBSet("continue-on-fatal-error", "1");
    SendMICommand(GDBMISession, cmd);
    MICommandFree(cmd);

    // Set maximum variable size to unlimited to allow gdb to print large values
    cmd = _MIGDBSet("max-value-size", "unlimited");
    SendMICommand(GDBMISession, cmd);
    MICommandFree(cmd);

    // manually set up pretty printing for relocated installs
    setup_pretty_printing();

    // set non-stop mode
    if (non_stop_flag) {
        cmd = _MIGDBSet("non-stop", "on");
        SendMICommand(GDBMISession, cmd);
        if (!MICommandResultOK(cmd)) {
            DVPRINT("setting non-stop mode failed\n");
        }
        MICommandFree(cmd);

        // we need to also set mi-async mode when non-stop mode
        // is on; otherwise commands can hang waiting for all the
        // threads to stop.
        cmd = _MIGDBSet("mi-async", "on");
        SendMICommand(GDBMISession, cmd);
        if (!MICommandResultOK(cmd)) {
            DVPRINT("setting mi-asycn mode failed\n");
        }
        MICommandFree(cmd);
    }

    return NULL;
}

static dbgevent_t *setup_gdbmi_signals(void) {
    DVPRINT("*** Dbg %s, %s\n", __FILE__, __func__);
    dbgevent_t *e;
    MICommand *cmd;

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

static const std::array<std::string, 20> parallel_entry_pt_strs
{
    "*(&MPI_Init)",
    "athos_mpi_init",
    "MPI_INIT_THREAD__",
    "MPI_INIT__",
    "mpi_init__",

    "mpi_init_thread__",
    "MPI_INIT_THREAD_",
    "MPI_INIT_",
    "mpi_init_thread_",
    "mpi_init_",

    "MPI::Init",
    "*&(MPI_Init_thread)",
    "MPI_Init()",
    "MPI::Init(int&)",
    "MPI_Init",

    "PMI2_Init",
    "*&(PMPI_Init)",
    "PMPI_Init",
    "*&(start_pes)",
    "*&(shmem_init)"
};

static dbgevent_t *setup_gdbmi_entry_points(void) {
    DVPRINT("*** Dbg %s, %s\n", __FILE__, __func__);
    dbgevent_t *e;
    MICommand *cmd;

    // set the lang to c. We always want to do this!!
    cmd = _MIGDBSet("lang", "c");
    SendMICommand(GDBMISession, cmd);
    if (!MICommandResultOK(cmd)) {
        MICommandFree(cmd);
        EVENT_ERROR(e, DBGERR_DEBUGGER, "Gdb set lang command failed.");
        return e;
    }
    MICommandFree(cmd);

    parallel_entry_BP = NewList();
    dbgbp_t *bp_res;
    for (auto&& str: parallel_entry_pt_strs) {
        cmd = MIBreakInsert(0, 0, 0, NULL, 0, str.c_str(), 0);
        SendMICommand(GDBMISession, cmd);
        // Note we only want to insert the breakpoint into our internal list if the
        // command succeeded. We expect many of these to fail.
        if (MICommandResultOK(cmd)) {
            if ((bp_res = get_dbgbp(cmd)) != NULL)
                AddToList(parallel_entry_BP, (void *)bp_res);
        }
        MICommandFree(cmd);
    }

    // set the lang back to auto. We always want to do this!!
    cmd = _MIGDBSet("lang", "auto");
    SendMICommand(GDBMISession, cmd);
    if (!MICommandResultOK(cmd)) {
        MICommandFree(cmd);
        EVENT_ERROR(e, DBGERR_DEBUGGER, "Gdb set lang command failed.");
        return e;
    }
    MICommandFree(cmd);

    return NULL;
}

static dbgevent_t *setup_gdbmi_main_entry_point(void) {
    DVPRINT("*** Dbg %s, %s\n", __FILE__, __func__);
    dbgevent_t *e;
    MICommand *cmd;
    dbgbp_t *bp_res;

    main_entry_BP = NewList();

    // set the lang to c. We always want to do this!!
    cmd = _MIGDBSet("lang", "c");
    SendMICommand(GDBMISession, cmd);
    if (!MICommandResultOK(cmd)) {
        MICommandFree(cmd);
        EVENT_ERROR(e, DBGERR_DEBUGGER, "Gdb set lang command failed.");
        return e;
    }
    MICommandFree(cmd);

    cmd = MIBreakInsert(0, 0, 0, NULL, 0, "main", 0);
    SendMICommand(GDBMISession, cmd);
    // Note we only want to insert the breakpoint into our internal list if the
    // command succeeded.
    if (MICommandResultOK(cmd)) {
        if ((bp_res = get_dbgbp(cmd)) != NULL)
            AddToList(main_entry_BP, (void *)bp_res);
    }
    MICommandFree(cmd);

    // set the lang back to auto. We always want to do this!!
    cmd = _MIGDBSet("lang", "auto");
    SendMICommand(GDBMISession, cmd);
    if (!MICommandResultOK(cmd)) {
        MICommandFree(cmd);
        EVENT_ERROR(e, DBGERR_DEBUGGER, "Gdb set lang command failed.");
        return e;
    }
    MICommandFree(cmd);

    return NULL;
}

static dbgevent_t *setup_gdbmi_exit_points(void) {
    DVPRINT("*** Dbg %s, %s\n", __FILE__, __func__);
    dbgevent_t *e;
    MICommand *cmd;
    dbgbp_t *bp_res;

    // set the lang to c. We always want to do this!!
    cmd = _MIGDBSet("lang", "c");
    SendMICommand(GDBMISession, cmd);
    if (!MICommandResultOK(cmd)) {
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

    cmd = MIBreakInsert(0, 0, 0, NULL, 0, "*(&exit)", 0);
    SendMICommand(GDBMISession, cmd);
    if (MICommandResultOK(cmd)) {
        if ((bp_res = get_dbgbp(cmd)) != NULL) {
            // Change the breakpoint type to an exit point or DBGBP_EXIT_TYPE
            bp_res->bp_type = DBGBP_EXIT_TYPE;
            AddToList(exit_BP, (void *)bp_res);
        }
    }
    MICommandFree(cmd);

    cmd = MIBreakInsert(0, 0, 0, NULL, 0, "*(&_exit)", 0);
    SendMICommand(GDBMISession, cmd);
    if (MICommandResultOK(cmd)) {
        if ((bp_res = get_dbgbp(cmd)) != NULL) {
            // Change the breakpoint type to an exit point or DBGBP_EXIT_TYPE
            bp_res->bp_type = DBGBP_EXIT_TYPE;
            AddToList(exit_BP, (void *)bp_res);
        }
    }
    MICommandFree(cmd);

    cmd = MIBreakInsert(0, 0, 0, NULL, 0, "*(&abort)", 0);
    SendMICommand(GDBMISession, cmd);
    if (MICommandResultOK(cmd)) {
        if ((bp_res = get_dbgbp(cmd)) != NULL) {
            // Change the breakpoint type to an exit point or DBGBP_EXIT_TYPE
            bp_res->bp_type = DBGBP_EXIT_TYPE;
            AddToList(exit_BP, (void *)bp_res);
        }
    }
    MICommandFree(cmd);

    cmd = MIBreakInsert(0, 0, 0, NULL, 0, "*(&MPIR_Errors_are_fatal)", 0);
    SendMICommand(GDBMISession, cmd);
    if (MICommandResultOK(cmd)) {
        if ((bp_res = get_dbgbp(cmd)) != NULL) {
            // Change the breakpoint type to an exit point or DBGBP_EXIT_TYPE
            bp_res->bp_type = DBGBP_EXIT_TYPE;
            AddToList(exit_BP, (void *)bp_res);
        }
    }
    MICommandFree(cmd);

    cmd = MIBreakInsert(0, 0, 0, NULL, 0, "*(&MPID_Abort)", 0);
    SendMICommand(GDBMISession, cmd);
    if (MICommandResultOK(cmd)) {
        if ((bp_res = get_dbgbp(cmd)) != NULL) {
            // Change the breakpoint type to an exit point or DBGBP_EXIT_TYPE
            bp_res->bp_type = DBGBP_EXIT_TYPE;
            AddToList(exit_BP, (void *)bp_res);
        }
    }
    MICommandFree(cmd);

    cmd = MIBreakInsert(0, 0, 0, NULL, 0, "*(&MPI_Abort)", 0);
    SendMICommand(GDBMISession, cmd);
    if (MICommandResultOK(cmd)) {
        if ((bp_res = get_dbgbp(cmd)) != NULL) {
            // Change the breakpoint type to an exit point or DBGBP_EXIT_TYPE
            bp_res->bp_type = DBGBP_EXIT_TYPE;
            AddToList(exit_BP, (void *)bp_res);
        }
    }
    MICommandFree(cmd);

    cmd = MIBreakInsert(0, 0, 0, NULL, 0,
                        "*(&ompi_mpi_errors_are_fatal_comm_handler)", 0);
    SendMICommand(GDBMISession, cmd);
    if (MICommandResultOK(cmd)) {
        if ((bp_res = get_dbgbp(cmd)) != NULL) {
            // Change the breakpoint type to an exit point or DBGBP_EXIT_TYPE
            bp_res->bp_type = DBGBP_EXIT_TYPE;
            AddToList(exit_BP, (void *)bp_res);
        }
    }
    MICommandFree(cmd);

    // set the lang back to auto. We always want to do this!!
    cmd = _MIGDBSet("lang", "auto");
    SendMICommand(GDBMISession, cmd);
    if (!MICommandResultOK(cmd)) {
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

dbgevent_t *DbgGdbMI::init(int *dfd, int *iofd, char **env) {
    DVPRINT("*** Dbg %s::%s\n", __FILE__, __func__);
    dbgevent_t *e;
    gdb_vers = GdbVersion::GDB_81;


    /**************DEBUG Start*****************/
    sigset_t debug_mask;
    if (sigprocmask(0, NULL, &debug_mask) < 0) {
        DVPRINT("InitGDBMI(): sigprocmask error!\n");
    } else {
        if (sigismember(&debug_mask, SIGCHLD)) {
            DVPRINT("InitGDBMI(): sigprocmask SIGCHLD is blocked!\n");

            sigemptyset(&debug_mask);
            sigaddset(&debug_mask, SIGCHLD);
            sigprocmask(SIG_UNBLOCK, &debug_mask, NULL);

            DVPRINT("InitGDBMI(): sigprocmask SIGCHLD is unblocked!\n");
        } else {
            DVPRINT("InitGDBMI(): sigprocmask SIGCHLD is NOT blocked!\n");
        }
    }
    /**************DEBUG End*****************/

    GDBMISession = MISessionNew();

    if (debug_flag && !getenv("GDB4HPC_DISABLE_MI_DBG")) {
        MISessionSetDebug(1);
    } else {
        MISessionSetDebug(0);
    }

    MISessionRegisterEventCallback(GDBMISession, event_callback);
    MISessionRegisterConsoleCallback(GDBMISession, console_callback);
    MISessionRegisterLogCallback(GDBMISession, log_callback);
    MISessionRegisterTargetCallback(GDBMISession, target_callback);
    MISessionRegisterNotifyCallback(GDBMISession, notification_callback);
    //set the default gdb type here for both the binary and the share directory,
    //overwrite it if we're using a non standard backend gdb.
    MISessionSetGDBPath(GDBMISession, "gdb-guard");
    char gdb_type[32];
    //set the default value of gdb_type to "gdb"
    sprintf(gdb_type, "gdb");

    // set the gdb path to use standard or gpu specific gdb
    if (gpu_flag) {
        // Need to determine the version of the driver to start the proper gdb
        FILE *vers;
        int num_fields = 0;

        // Open the driver version file, if this fails we are not on a gpu node
        // and should default to using the regular gdb.
        if ((vers = fopen("/proc/driver/nvidia/version", "r")) != NULL) {
            char *vers_cstr = NULL;
            num_fields = fscanf(vers,
                               "NVRM version: NVIDIA UNIX x86_64 Kernel Module%*[ "
                               "\t]%m[0-9.]",
                               &vers_cstr);
            fclose(vers);
            if (num_fields > 0) {
                // We now have the version string, close the vers file
                DVPRINT("Detected Nvidia driver %s\n", vers_cstr);
                // Now strcmp the driver version with a list of supported
                // versions The known driver mappings are:
                // nvidia 319.* ==> CTK 5.5     ; deprecated, unsupported
                // nvidia 340.* ==> CTK 6.5     ; deprecated, unsupported
                // nvidia 346.* ==> CTK 7.0     ; deprecated, unsupported
                // nvidia 352.* ==> CTK 7.5     ; deprecated, unsupported
                // nvidia 361.* ==> CTK 8.0     ; deprecated, unsupported
                // nvidia 384.* ==> CTK 9.0     ; deprecated, unsupported
                // nvidia 390*  ==> CTK 9.1     ; deprecated, unsupported
                // nvidia 440*  ==> CTK 10.2    ; hacking 10.1 to work.
                std::string vers_str(vers_cstr);
                int vers_int = stoi(vers_str.substr(0,3));
                //We're assuming forward compatibility from version 440
                if (vers_int >= 440) {
                    gdb_vers = GdbVersion::CUDA_101;
                }
            }
            if (vers_cstr) {
                free(vers_cstr);
            }
        } else if (access("/usr/sbin/dkms", X_OK ) != -1 ) {
            const size_t size = 1024;
            char linebuf[size];
            char versbuf[32];
            FILE* fp = NULL;
            int num_fields = 0;
            fp = popen("/usr/sbin/dkms --version", "r");
            if (fp != NULL) {
                DVPRINT("popened /usr/sbin/dkms\n");
                while (gdb_vers != GdbVersion::ROC_GDB &&
                       fgets(linebuf, size, fp) != NULL) {
                    num_fields = sscanf(linebuf, "%8s", versbuf);
                    if (num_fields == 1) {
                        if (strncmp(versbuf, "dkms:2.8", 8) == 0) {
                            DVPRINT("Detected AMD driver %s\n", versbuf);
                            gdb_vers = GdbVersion::ROC_GDB;
                        }
                    }
                }
                pclose(fp);
            }
        }

        if (gdb_vers == GdbVersion::GDB_81) {
            // Didn't find any of the gpu driver types; apparently gpu_flag is
            // a global so we need to tell the world that we didn't find the
            // correct gpu debugger. (really, we need to do better variable
            // naming to make this obvious)
            DVPRINT("Failed to determine gpu driver version.\n");
            DVPRINT("Defaulting to non-gpu mode.\n");
            gpu_flag = 0;
        }
        // If GPU mode is on and we're targeting Nvidia hardware, force tmpdir
        // to /tmp. Nvidia utilizes unix pipes that are writen in TMPDIR and
        // this value *MUST* match the value set in the applications
        // environment, otherwise things will hang.
        if (gdb_vers == GdbVersion::CUDA_101) {
            char *old_tmpdir;
            struct stat st;

            // Get the applications setting of TMPDIR
            if ((old_tmpdir = getenv("CTI_OLD_TMPDIR")) != NULL) {
                DVPRINT("Using application TMPDIR setting of %s\n",
                        old_tmpdir);
                if (setenv("TMPDIR", old_tmpdir, 1) < 0) {
                    // failure
                    DVPRINT("setenv failed");
                }
                // ensure the directory exists, otherwise create it
                // try to stat the directory
                if (stat(old_tmpdir, &st) != 0) {
                    // the directory doesn't exist so we need to create
                    // it use perms 755
                    if (mkdir(old_tmpdir, S_IRWXU | S_IRGRP | S_IXGRP |
                                              S_IROTH | S_IXOTH) != 0) {
                        DVPRINT(
                            "Could not create non-existent TMPDIR!\n");
                    } else {
                        DVPRINT("Created TMPDIR at %s\n", old_tmpdir);
                    }
                }
            } else {
                DVPRINT(
                    "CTI_OLD_TMPDIR env var not set. Defaulting to "
                    "/tmp.\n");
                if (setenv("TMPDIR", "/tmp", 1) < 0) {
                    // failure
                    DVPRINT("setenv failed");
                }
            }
        }

        switch (gdb_vers) {
            case GdbVersion::CUDA_101:
                DVPRINT("Using cuda101-gdb-guard\n");
                MISessionSetGDBPath(GDBMISession, "cuda101-gdb-guard");
                // We have defined a different share directory for the
                // cuda gdb 10.1 binary's data-directory:
                sprintf(gdb_type, "cuda-gdb");
                break;
            case GdbVersion::ROC_GDB:
                DVPRINT("Using ROCgdb-guard\n");
                MISessionSetGDBPath(GDBMISession, "ROCgdb-guard");
                // We have defined a different share directory for the
                // ROCgdb binary's data-directory:
                sprintf(gdb_type, "ROCgdb");
                break;
            case GdbVersion::GDB_81:
                DVPRINT("Unsupported driver version.\n");
                DVPRINT("Defaulting to non-gpu mode.\n");
                //gdb_type was initialized to default value of "gdb" after its
                //declaration above...
                gpu_flag = 0;
                break;
        }
    }

    // Get the base dir env var if it was set and use that for data-directory
    char *env_base_dir = getenv(GUARD_INST_DIRECTORY);
    if (env_base_dir != NULL) {
        char *gdb_data_dir;
        Asprintf(&gdb_data_dir, "%s/share/%s", env_base_dir, gdb_type);
        MISessionSetGDBDataDirectory(GDBMISession, gdb_data_dir);
        Free(gdb_data_dir);
    }

    // Ensure PYTHONSTARTUP isn't set
    if (getenv("PYTHONSTARTUP") != NULL) {
        unsetenv("PYTHONSTARTUP");
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
    int progio_pipe[2];

    // We might want to forward output to the frontend
    // Create a pipe to use as a control channel
    if (pipe(progio_pipe)) {
        EVENT_ERROR(e, DBGERR_PIPE,
                    "Could not create a pipe for app stdout redirection.");
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

    if ((e = setup_gdbmi_environment()) != NULL) { return e; }

    DBEPERFLOG(SetTimerStopAndLog("DbgInitBackend: setup_gdbmi_environment"));

    DBEPERFLOG(SetTimerStart("DbgInitBackend: setup_gdbmi_signals"));

    if ((e = setup_gdbmi_signals()) != NULL) { return e; }

    DBEPERFLOG(SetTimerStopAndLog("DbgInitBackend: setup_gdbmi_signals"));

    return (dbgevent_t *)NULL;
}

/*
** Continue to main and show current stack frame.
*/
dbgevent_t *DbgGdbMI::startup(void) {
    DVPRINT("*** Dbg %s, %s\n", __FILE__, __func__);
    dbgevent_t *e;
    MICommand *cmd;
    MIList *frames;
    MIFrame *frame;
    MIEvent *evt;
    dbgbp_t *bp = NULL;
    int finish = 0;
    int attempt = 0;

    // Setup entry points if we are not running with the special early
    // entry mode:
    if (!early_entry_flag) {
        // setup main entry points
        if ((e = setup_gdbmi_main_entry_point()) != NULL) return e;
        // setup parallel entry points
        if ((e = setup_gdbmi_entry_points()) != NULL) return e;

recover_for_slurm:
        // we only want to issue a continue if one of the breakpoint lists is
        // not empty.
        if (!EmptyList(parallel_entry_BP) || !EmptyList(main_entry_BP)) {
            // counter for recover
            ++attempt;
            // Make sure that we're not inside a parallel entry point

            //get all of the frames.
            cmd = MIStackListAllFrames();
            SendMICommand(GDBMISession, cmd);
            if (!MICommandResultOK(cmd)) {
                MICommandFree(cmd);
                EVENT_ERROR(e, DBGERR_DEBUGGER, "MIStackListAllFrames command failed.");
                return e;
            }
            if ((frames = MIGetStackListFramesInfo(cmd)) == NULL) {
                MICommandFree(cmd);
                EVENT_ERROR(e, DBGERR_NOSTACK, "MIGetStackListFramesInfo command failed");
                return e;
            }
            MICommandFree(cmd);

            // parse over the frame list
            for (MIListSet(frames); (frame = (MIFrame *)MIListGet(frames)) != NULL;) {
                if (FindBPByFunc(parallel_entry_BP, frame->func) != NULL) {
                    DVPRINT("%s: found frame in parallel_entry_BP, frame->level "
                            "%d, frame->func %s\n", __func__, frame->level, frame->func);
                    break;
                }
            }
            //if we found a parallel entry point function, select that frame
            if (frame != NULL) {
                //set the frame to the parallel entry point
                cmd = MIStackSelectFrame(frame->level);
                SendMICommand(GDBMISession, cmd);
                if (MICommandResultOK(cmd)) {
                    finish = 1;
                }
                MICommandFree(cmd);

                // free the MIFrame list
                MIListFree(frames, (void (*)())MIFrameFree);

            } else {
                // we are done with the frames list
                MIListFree(frames, (void (*)())MIFrameFree);
                
                // issue a continue command .. this is assuming we're pre-main or
                // pre-parallel entry point.
                cmd = MIExecContinueAll();
                SendMICommand(GDBMISession, cmd);
                if (!MICommandResultOK(cmd)) {
                    EVENT_ERROR(e, DBGERR_DEBUGGER,
                                MICommandResultErrorMessage(cmd));
                    MICommandFree(cmd);
                    return e;
                }
                MICommandFree(cmd);
                // set the status to running
                Status = DBGSTAT_RUNNING;

                // wait for the asynch command to complete
                while (!bufferReady()) {
                    MISessionProgress(GDBMISession);
                }

                // grab the pending asynch
                evt = PendingEvent;

                // set the state back to default
                AsynchEventWaiting = 0;
                PendingEvent = NULL;
                Status = DBGSTAT_STOPPED;

                // slurm will send a SIGSTOP for some reason, try to recover from
                // that
                if (evt->type == MIEventTypeSignal && attempt < 10) {
                    DVPRINT("DbgStartup: Received signal, trying to recover.\n");
                    MIEventFree(evt);
                    evt = NULL;
                    goto recover_for_slurm;
                }

                // ensure we hit the breakpoint
                if (evt->type != MIEventTypeBreakpointHit) {
                    EVENT_ERROR(e, DBGERR_DEBUGGER,
                                "Failed to hit entry breakpoint.");
                    return e;
                }

                // If we hit a main breakpoint, we are done (unset finish).
                // Otherwise check parallel breakpoints
                if ((bp = FindBPByID(main_entry_BP, evt->bkptno)) ==
                    (dbgbp_t *)NULL) {
                    // If we hit a parallel breakpoint, we need to finish, otherwise
                    // unkown
                    if ((bp = FindBPByID(parallel_entry_BP, evt->bkptno)) ==
                        (dbgbp_t *)NULL) {
                        DVPRINT("%s, %s Hit unknown entry breakpoint.", __FILE__,
                                __func__);
                    } else {
                        //hit the known parallel entry breakpoint
                        finish = 1;
                    }
                }

                // Free the event
                MIEventFree(evt);
            }
            // remove all of the entry breakpoints. We are done with them.
            if ((e = delete_internal_breakpoints(main_entry_BP)) != NULL)
                return e;
            if ((e = delete_internal_breakpoints(parallel_entry_BP)) != NULL)
                return e;

            // We want to set MPIR_debug_gate=1. This is an old MPIR standard.
            cmd = _MIGDBSet("lang", "c");
            SendMICommand(GDBMISession, cmd);
            if (!MICommandResultOK(cmd)) {
                MICommandFree(cmd);
                EVENT_ERROR(e, DBGERR_DEBUGGER, "Gdb set lang command failed.");
                return e;
            }
            MICommandFree(cmd);

            // This can fail if the symbol doesn't exist. We want to silently
            // fail.
            cmd = _MIGDBSet("MPIR_debug_gate=1", NULL);
            SendMICommand(GDBMISession, cmd);
            MICommandFree(cmd);

            // set the lang back to auto.
            cmd = _MIGDBSet("lang", "auto");
            SendMICommand(GDBMISession, cmd);
            if (!MICommandResultOK(cmd)) {
                MICommandFree(cmd);
                EVENT_ERROR(e, DBGERR_DEBUGGER, "Gdb set lang command failed.");
                return e;
            }
            MICommandFree(cmd);

            if (finish) {
                // now we want to issue the finish command to get back to main.
                cmd = MIExecFinish();
                SendMICommand(GDBMISession, cmd);
                if (!MICommandResultOK(cmd)) {
                    MICommandFree(cmd);
                    EVENT_ERROR(e, DBGERR_DEBUGGER, "Finish command failed.");
                    return e;
                }
                MICommandFree(cmd);

                // set the status to running
                Status = DBGSTAT_RUNNING;

                // wait for the asynch command to complete
                while (!bufferReady()) MISessionProgress(GDBMISession);

                // grab the pending asynch
                evt = PendingEvent;

                // set the state back to default
                AsynchEventWaiting = 0;
                PendingEvent = NULL;
                Status = DBGSTAT_STOPPED;

                // ensure we finished the function
                if (evt->type != MIEventTypeFunctionFinished &&
                    evt->type != MIEventTypeSuspended) {
                    EVENT_ERROR(e, DBGERR_DEBUGGER,
                                "Failed to finish the entry function.");
                    return e;
                }

                // Free the event
                MIEventFree(evt);
            }
        }
    }

    // Initialize UPC, in case the app uses UPC - this will break gpu debugging
    if (!gpu_flag) {
        const char *cmd_argv[1] = {"upc-init"};
        cmd = CLIBypassc(1, cmd_argv);
        SendMICommand(GDBMISession, cmd);
        MICommandFree(cmd);
    }

    // call the showframe function to ask for the innermost frame
    return showFrame(1, nullptr);
}

/*
** LibMI consumer call. This will have the libmi progress its session
** and perform any necessary callbacks.
*/
int DbgGdbMI::read(int fd) {
    DVPRINT("*** Dbg %s, %s\n", __FILE__, __func__);
    int error = 0;

    /*
    ** We let MI process event callbacks (it's going to call
    ** our event_callback()).
    ** If this fails, or if the out_fd is closed then we should inject
    ** an error event to recover.
    */
    if (MISessionProgress(GDBMISession) == -1) {
        DVPRINT("DbgGDBMIConsume: MISessionProgress returned -1!\n");
        ++error;
    }

    if (GDBMISession->out_fd == -1) {
        DVPRINT("DbgGDBMIConsume: sess->out_fd = -1\n");
        ++error;
    }

    if (error) {
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
bool DbgGdbMI::bufferReady(void) {
    DVPRINT("*** Dbg %s, %s\n", __FILE__, __func__);
    return AsynchEventWaiting; /* Modified by event_callback(). */
}

/*
** Set breakpoint at specified line.
*/
dbgevent_t *DbgGdbMI::setLineBreak(const int allowPending, const char *file, int lno,
                                   const char* threadSet, const dbgcomm_info *commInfo) {
    DVPRINT("*** Dbg %s, %s\n", __FILE__, __func__);
    auto where = std::to_string(lno);
    if (file && file[0]) {
        where = std::string{file} + ":" + where;
    }

    return setBreak(allowPending, where, threadSet, commInfo);
}

/*
** Set breakpoint at start of specified function.
*/
dbgevent_t *DbgGdbMI::setFuncBreak(const int allowPending, const char *file, const char *func,
                                   const char*  threadSet,
                                   const dbgcomm_info *commInfo) {
    DVPRINT("*** Dbg %s, %s\n", __FILE__, __func__);
    assert(func!=nullptr);
    
    std::string where;
    if (file && file[0]) {
        where = std::string(file) + ":" + func;
    } else {
        where = func;
    }

    return setBreak(allowPending, where, threadSet, commInfo);
}

// Common work for setting a function or line breakpoing.   Argument \a where
// contains the proper break expression in either case.
dbgevent_t *DbgGdbMI::setBreak(const int allowPending, const std::string& where,
                               const char*  threadSet,
                               const dbgcomm_info *commInfo) {
    std::vector<int> threads;
    if (threadSet && strlen(threadSet)) {
        std::string error;
        threads = getThreadSet(threadSet, nullptr, error);
        if (!error.empty()) {
            return NewErrorEvent(DBGERR_DEBUGGER, error.c_str());
        }
    }

    // -1 is the gdb/gdbmi indicator for a normal stop on all threads breakpoint
    if (threads.empty()) threads.push_back(-1);

    std::vector<int> bpIds;
    using event_ptr = std::unique_ptr<dbgevent_t, decltype(&FreeEvent)>;

    event_ptr event(nullptr,FreeEvent);

    // gdb only implements a single thread breakpoint, so if have a thread set we will
    // need to create multiple gdb breakpoints and track them.   
    for (auto&& threadId : threads) {

        auto cmd = MIBreakInsert(0, 0, allowPending, nullptr, 0, where.c_str(), threadId);
        SendMICommand(GDBMISession, cmd);

        event_ptr tbEvent(nullptr,FreeEvent);
        
        if (!MICommandResultOK(cmd)) {
            tbEvent.reset(NewErrorEvent(DBGERR_DEBUGGER, MICommandResultErrorMessage(cmd)));

            // Try the basename instead in case the file was compiled locally.
            auto sloc = where.rfind('/');
            if (sloc != std::string::npos) {
                auto where2 = where.substr(sloc+1);

                MICommandFree(cmd);
                cmd = MIBreakInsert(0, 0, allowPending, nullptr, 0, where2.c_str(), threadId);
                SendMICommand(GDBMISession, cmd);

                if (MICommandResultOK(cmd)) {
                    tbEvent.reset();
                }
            }
        }

        if (!tbEvent) {
            tbEvent.reset(CheckBreak(cmd, commInfo));
        }

        MICommandFree(cmd);

        if (!tbEvent) {
            event.reset(NewErrorEvent(DBGERR_DEBUGGER, "Cannot create breakpoint."));
            break;
        } else {
            bpIds.push_back(EVENT_BP(tbEvent)->bp_id);
            if (!event) {
                event = std::move(tbEvent);
            } 
        }
    } // for (threadId)

    if (!event) {
        event.reset(NewErrorEvent(DBGERR_DEBUGGER, "Cannot create breakpoint."));
    }

    if (event->ev_event != DBGEV_BPSET) {
        // failed - undo any breakpoints that may have succeeded
        // note that we haven't yet updated m_multiThreadBreaks so each delete will
        // remove a single breakpoint
        DVPRINT("Insertion failed removing %d breakpoints\n", bpIds.size());
        for (auto it=bpIds.rbegin(); it!=bpIds.rend(); ++it) {
            // may not matter, but undo in LIFO order for neatness sake
            FreeEvent(deleteBreak(*it));
        }
    } else if (bpIds.size() > 1) {
        DVPRINT("adding multiThreadBreaks\n");
        int id = bpIds.front();
        bpIds.erase(bpIds.begin());
        m_multiThreadBreaks.insert(std::make_pair(id, bpIds));

        auto bp = EVENT_BP(event);
        if (bp && threadSet) {
            char* ts = strdup(threadSet);
            std::swap(ts, bp->bp_threadId);
            free(ts);
        }
    }

    return event.release();
}

/*
** Set watchpoint at specified expression.
*/
dbgevent_t *DbgGdbMI::setWatch(int type, const char *expr, const dbgcomm_info *commInfo) {
    DVPRINT("*** Dbg %s, %s expr=%s\n", __FILE__, __func__, expr);
    dbgevent_t *event;
    MICommand *cmd;
    dbgbp_type bp_type = (dbgbp_type)type;
    MIBreakpoint *bpt;
    MIList *bpts;
    int id;

    // set the command based on the type
    switch (bp_type) {
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
        EVENT_ERROR(event, DBGERR_DEBUGGER,
                    "Cannot create internal breakpoint.");
        MICommandFree(cmd);
        return event;
    }

    MIListSet(bpts);
    bpt = (MIBreakpoint *)MIListGet(bpts);

    id = bpt->number;

    MIListFree(bpts, (void (*)())MIBreakpointFree);
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

    event = CheckBreak(cmd, commInfo);

    if (event == NULL) {
        EVENT_ERROR(event, DBGERR_DEBUGGER,
                    "Cannot create internal breakpoint.");
    }

    MICommandFree(cmd);

    return event;
}
// If break bid is the breakpoint that has been applied to a thread list, then DbgGdbMI
// is maintaining the set of *additional* gdb breakpoints - one per thread - otherwise return
// an empty list.   If remove is true, erase the recorded list.
std::vector<int> DbgGdbMI::getMultiThreadBreaks(int bid, bool remove) {
    std::vector<int> ids;
    auto multiLoc = m_multiThreadBreaks.find(bid);
    if (multiLoc != m_multiThreadBreaks.end()) {
        ids = multiLoc->second;
        if (remove) m_multiThreadBreaks.erase(multiLoc);
    }
    return ids;
}

/*
** Delete a breakpoint.
*/
dbgevent_t *DbgGdbMI::deleteBreak(int bid) {
    DVPRINT("*** Dbg %s, %s\n", __FILE__, __func__);

    applyMultiThreadBreak(bid, true, [this](int id) { FreeEvent(this->deleteBreak(id)); });

    auto bp = FindBPByID(BP, bid);

    if (bp == NULL) {
        return NewErrorEvent(DBGERR_NOBP, "Invalid breakpoint id.");
    }

    auto bpid = bid;

    auto cmd = MIBreakDelete(1, &bpid);
    SendMICommand(GDBMISession, cmd);

    if (!MICommandResultOK(cmd)) {
        MICommandFree(cmd);
        return NewErrorEvent(DBGERR_DEBUGGER, "Delete breakpoint failed.");
    }

    RemoveBP(BP, bid);

    auto e = NewEvent(DBGEV_OK);

    MICommandFree(cmd);

    return e;
}

/*
** Disable a breakpoint.
*/
dbgevent_t *DbgGdbMI::disableBreak(int bid) {
    DVPRINT("*** Dbg %s, %s\n", __FILE__, __func__);

    applyMultiThreadBreak(bid, false, [this](int id) { FreeEvent(this->disableBreak(id));});

    auto bp = FindBPByID(BP, bid);
    if (bp == NULL) {
        return NewErrorEvent(DBGERR_NOBP, "Invalid breakpoint id.");
    }

    auto bpid = bid;

    auto cmd = MIBreakDisable(1, &bpid);
    SendMICommand(GDBMISession, cmd);

    if (!MICommandResultOK(cmd)) {
        MICommandFree(cmd);
        return NewErrorEvent(DBGERR_DEBUGGER, "Disable breakpoint failed.");
    }
    MICommandFree(cmd);

    bp->bp_enabled = 0;
    return NewEvent(DBGEV_OK);
}

/*
** Enable a breakpoint.
*/
dbgevent_t *DbgGdbMI::enableBreak(int bid, bool once, bool del) {
    DVPRINT("*** Dbg %s, %s\n", __FILE__, __func__);

    applyMultiThreadBreak(bid, false,
                          [this,once,del](int id) { FreeEvent(this->enableBreak(id,once,del)); });

    auto bp = FindBPByID(BP, bid);
    if (!bp) {
        return NewErrorEvent(DBGERR_NOBP, "Invalid breakpoint id.");
    }

    // cannot set once and del at the same time
    if (once && del) {
        return NewErrorEvent(DBGERR_NOBP, "Invalid arguments.");
    }

    MICommand *cmd = nullptr;
    char cmd_bpid[30];
    const char *cmd_args[3];
    if (once) {
        cmd_args[0] = "enable";
        cmd_args[1] = "once";
        snprintf(cmd_bpid, 30, "%d", bid);
        cmd_args[2] = cmd_bpid;
        cmd = CLIBypassc(3, cmd_args);
    } else if (del) {
        cmd_args[0] = "enable";
        cmd_args[1] = "delete";
        snprintf(cmd_bpid, 30, "%d", bid);
        cmd_args[2] = cmd_bpid;
        cmd = CLIBypassc(3, cmd_args);
    } else {
        // this is needed...
        auto bpid = bid;
        cmd = MIBreakEnable(1, &bpid);
    }

    SendMICommand(GDBMISession, cmd);

    if (!MICommandResultOK(cmd)) {
        MICommandFree(cmd);
        return NewErrorEvent(DBGERR_DEBUGGER, "Enable breakpoint failed.");
    }

    bp->bp_enabled = 1;

    if (once)
        bp->bp_disp = DBGBP_DISABLE_DISP;
    else if (del)
        bp->bp_disp = DBGBP_DELETE_DISP;

    MICommandFree(cmd);

    return NewEvent(DBGEV_OK);
}

/*
** Return id of last breakpoint to be hit.
*/
dbgevent_t *DbgGdbMI::lastBreak() {
    DVPRINT("*** Dbg %s, %s\n", __FILE__, __func__);
    dbgevent_t *e;

    if (!CurrBP) {
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
dbgevent_t *DbgGdbMI::list(const char *str) {
    DVPRINT("*** Dbg %s, %s\n", __FILE__, __func__);
    dbgevent_t *e;
    MICommand *cmd;
    MIList *lines;
    CLILineInfo *line;
    dbglist_t *dbg_list;
    dbglist_t *list_ptr = NULL;
    dbglist_t *list = NULL;

    // We currently turn off opening of source files to avoid performance
    // penalties of accessing the cross mounted file system. But the user
    // explicitly wanted to list the source and our frontend failed, so we need
    // to turn it back on and take the penalty. We turn this feature back off
    // when we are finished. The frontend handles most list commands to avoid
    // these issues, but sometimes it can fail or for the attach case it
    // couldn't find the a.out.
    // TODO: It may be better to do it on the backend rather than the frontend
    // if there are hundreds of thousands of ranks and they are all at different
    // locations.

    if (gdb_vers != GdbVersion::ROC_GDB) {
        cmd = _MIGDBSet("source open", "on");
        SendMICommand(GDBMISession, cmd);
        if (!MICommandResultOK(cmd)) {
            MICommandFree(cmd);
            EVENT_ERROR(e, DBGERR_DEBUGGER, "Gdb set source open command failed.");
            return e;
        }
        MICommandFree(cmd);
    }

    cmd = CLIList(str);
    SendMICommand(GDBMISession, cmd);
    if (!MICommandResultOK(cmd)) {
        EVENT_ERROR(e, DBGERR_DEBUGGER, MICommandResultErrorMessage(cmd));
        MICommandFree(cmd);
        return e;
    }

    lines = CLIGetListInfo(cmd);

    // Convert the MI list of CLILineInfo into a dbglist_t
    for (MIListSet(lines); (line = (CLILineInfo *)MIListGet(lines)) != NULL;) {
        dbg_list = (dbglist_t *)Alloc(sizeof(dbglist_t));
        dbg_list->list_lno = line->line_no;
        dbg_list->list_line = strdup(line->line);
        dbg_list->list_threads = nullptr;
        dbg_list->list_next = (dbglist_t *)NULL;

        if (list == NULL) {
            list = dbg_list;
            list_ptr = dbg_list;
        } else {
            list_ptr->list_next = dbg_list;
            list_ptr = dbg_list;
        }
    }

    MIListFree(lines, (void (*)())CLILineInfoFree);
    MICommandFree(cmd);

    if (gdb_vers != GdbVersion::ROC_GDB) {
        // do not open source files
        cmd = _MIGDBSet("source open", "off");
        SendMICommand(GDBMISession, cmd);
        if (!MICommandResultOK(cmd)) {
            MICommandFree(cmd);
            EVENT_ERROR(e, DBGERR_DEBUGGER, "Gdb set source open command failed.");
            return e;
        }
        MICommandFree(cmd);
    }

    e = NewEvent(DBGEV_LIST);
    e->ev_list = list;

    return e;
}

/*
** Get source files of inferior.
*/
dbgevent_t *DbgGdbMI::getSources() {
    DVPRINT("*** Dbg %s, %s\n", __FILE__, __func__);
    dbgevent_t *e;
    MICommand *cmd;
    MIList *sources;
    MISources *source;
    dbgsources_t *dbg_sources;
    dbgsources_t *sources_ptr = NULL;
    dbgsources_t *head = NULL;

    cmd = MIFileListExecSourceFiles();
    SendMICommand(GDBMISession, cmd);
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

/*
** Start/continue executing program. If we are starting,
** then we must plant a breakpoint at main() so that we
** can find out the process id of the program.
*/
dbgevent_t *DbgGdbMI::go(bool all) {
    DVPRINT("*** Dbg %s, %s\n", __FILE__, __func__);
    dbgevent_t *e;

    if (Status == DBGSTAT_EXIT) {
        EVENT_ERROR(e, DBGERR_AT_EXIT, "");
        return e;
    }

    MICommand *cmd = (all ? MIExecContinueAll() : MIExecContinue());

    SendMICommand(GDBMISession, cmd);

    if (!MICommandResultOK(cmd)) {
        EVENT_ERROR(e, DBGERR_DEBUGGER, MICommandResultErrorMessage(cmd));
        MICommandFree(cmd);
        return e;
    }

    MICommandFree(cmd);

    AsyncCheck((dbgevent_t * (*)(dbgevent_t *)) WaitEvent, (dbgevent_t *)NULL);

    Status = DBGSTAT_RUNNING;

    e = NewEvent(DBGEV_OK);

    return e;
}

/*
** Continue executing program until the current function returns.
*/
dbgevent_t *DbgGdbMI::finishFunc() {
    DVPRINT("*** Dbg %s, %s\n", __FILE__, __func__);
    dbgevent_t *e;

    if (Status == DBGSTAT_EXIT) {
        EVENT_ERROR(e, DBGERR_AT_EXIT, "");
        return e;
    }

    MICommand *cmd = MIExecFinish();

    SendMICommand(GDBMISession, cmd);

    if (!MICommandResultOK(cmd)) {
        EVENT_ERROR(e, DBGERR_DEBUGGER, MICommandResultErrorMessage(cmd));
        MICommandFree(cmd);
        return e;
    }

    MICommandFree(cmd);

    AsyncCheck((dbgevent_t * (*)(dbgevent_t *)) WaitEvent, (dbgevent_t *)NULL);

    Status = DBGSTAT_RUNNING;

    e = NewEvent(DBGEV_OK);

    return e;
}

/*
** Set run arguments.
*/
dbgevent_t *DbgGdbMI::setArgs(const char *arg) {
    DVPRINT("*** Dbg %s, %s\n", __FILE__, __func__);

    if (arg != NULL && *arg != '\0') {
        if (RunArgs != NULL) Free(RunArgs);

        RunArgs = strdup(arg);
    }

    return NewEvent(DBGEV_OK);
}

/*
** Execute count statements. If in == false, do not enter
** function calls.
*/
dbgevent_t *DbgGdbMI::step(int count, bool in) {
    DVPRINT("*** Dbg %s, %s\n", __FILE__, __func__);
    dbgevent_t *e;
    MICommand *cmd;

    if (Status == DBGSTAT_EXIT) {
        EVENT_ERROR(e, DBGERR_AT_EXIT, "");
        return e;
    }

    if (!in)
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

    AsyncCheck((dbgevent_t * (*)(dbgevent_t *)) WaitEvent, (dbgevent_t *)NULL);

    Status = DBGSTAT_RUNNING;

    e = NewEvent(DBGEV_OK);

    return e;
}

/*
** Kill the currently running process.
*/
dbgevent_t *DbgGdbMI::kill() {
    DVPRINT("*** Dbg %s, %s\n", __FILE__, __func__);
    dbgevent_t *e;

#ifndef ALPS
    // Note that if we are on a cray platform, we don't want to send the kill
    // command. The ALPS interface will cleanup for us on the front-end. Killing
    // the application process causes an "unclean" exit in the viewpoint of
    // alps.

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

static dbgframe_t* MakeFrameFromMIFrame(MIFrame* mi, bool keepAddr) {
    dbgloc resLoc = { mi->file, mi->func, keepAddr ? mi->addr : nullptr, mi->line };
    return new dbgframe_t{mi->level, resLoc, nullptr, nullptr, nullptr};
}


/*
** Move up or down count stack frames.
*/
dbgevent_t *DbgGdbMI::moveFrame(int count, bool up, const char* threads) {
    DVPRINT("*** Dbg %s, %s\n", __FILE__, __func__);
    dbgevent_t *e;

    int cur_level;
    int max_depth;
    int frame_number;

    MICommand *cmd;
    MIList *mi_frame_list;
    MIFrame *mi_frame;

    if (threads) {
        std::ostringstream op;
        op << threads << (up ? " up " : " down ") << count;
        return threadEventToFrame(threadApply(op.str().c_str()));
    }

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
    if ((mi_frame = (MIFrame *)MIListGet(mi_frame_list)) == NULL) {
        MIListFree(mi_frame_list, NULL);
        EVENT_ERROR(e, DBGERR_NOSTACK, MICommandResultErrorMessage(cmd));
        MICommandFree(cmd);
        return e;
    }
    // save the level of the current frame
    cur_level = mi_frame->level;

    // free the MIFrame list
    MIListFree(mi_frame_list, (void (*)())MIFrameFree);
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
    if ((max_depth = MIGetStackInfoDepth(cmd)) < 0) {
        EVENT_ERROR(e, DBGERR_NOSTACK, MICommandResultErrorMessage(cmd));
        MICommandFree(cmd);
        return e;
    }
    MICommandFree(cmd);

    // the frame we want to select is based on an offset of the current
    // frame. We must ensure that this is between a min of 0 and a max of
    // max_depth
    switch (up) {
        case 0:
            if ((frame_number = cur_level - count) < 0) {
                EVENT_ERROR(e, DBGERR_LASTFRAME, "");
                return e;
            }
            break;

        default:
            if ((frame_number = cur_level + count) >= max_depth) {
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
    if ((mi_frame = (MIFrame *)MIListGet(mi_frame_list)) == NULL) {
        MIListFree(mi_frame_list, NULL);
        EVENT_ERROR(e, DBGERR_NOSTACK, MICommandResultErrorMessage(cmd));
        MICommandFree(cmd);
        return e;
    }
    MICommandFree(cmd);

    // parse the frame into a dbgframe_t object
    auto result = MakeFrameFromMIFrame(mi_frame);

    // free the MIFrame list
    MIListFree(mi_frame_list, (void (*)())MIFrameFree);

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
dbgevent_t *DbgGdbMI::showFrame(int count, const char* threads) {
    DVPRINT("*** Dbg %s, %s\n", __FILE__, __func__);
    dbgevent_t *e;

    MICommand *cmd;
    MIList *frames;
    MIFrame *frame;

    if (threads) {
        auto op = std::string{threads} + " bt";
        if (count) op += " " + std::to_string(count);
        return threadEventToFrame(threadApply(op.c_str()));
    }

    // We need to do different things depending on the value of count
    if (count < 0) {
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

        if ((depth = MIGetStackInfoDepth(cmd)) == -1) {
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
        cmd = MIStackListFrames(depth + count, depth - 1);
    } else if (count > 0) {
        // if count is positive, we want to ask for the innermost count frames
        // Fix this to be zero offset by subtracting one.
        cmd = MIStackListFrames(0, count - 1);
    } else {
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

    if ((frames = MIGetStackListFramesInfo(cmd)) == NULL) {
        MICommandFree(cmd);
        EVENT_ERROR(e, DBGERR_NOSTACK, "");
        return e;
    }
    MICommandFree(cmd);

    // parse over the frame list
    // increment the frames count each time through
    dbgframe_t *result = nullptr, *tail = nullptr;

    for (MIListSet(frames); (frame = (MIFrame *)MIListGet(frames)) != NULL;) {
        // create the new frame
        auto df = MakeFrameFromMIFrame(frame);
        if (tail) {
            tail->setNext(df);
            tail = df;
        } else {
            result = tail = df;
        }
    }

    // create the event with the frame information
    e = NewEvent(DBGEV_FRAME);
    e->ev_frame = result;

    // free the MIFrame list
    MIListFree(frames, (void (*)())MIFrameFree);

    return e;
}

/*
** Get selected stack frame. This will return information about the current
*frame
** that is selected.
*/
dbgevent_t *DbgGdbMI::getFrame(const char* threads) {
    DVPRINT("*** Dbg %s, %s\n", __FILE__, __func__);
    dbgevent_t *e;

    if (threads) {
        auto op = std::string{threads} + " frame";
        return threadEventToFrame(threadApply(op.c_str()));
    }

    // Get the selected frame
    auto cmd = MIStackInfoFrame();
    SendMICommand(GDBMISession, cmd);

    if (!MICommandResultOK(cmd)) {
        EVENT_ERROR(e, DBGERR_DEBUGGER, MICommandResultErrorMessage(cmd));
        MICommandFree(cmd);
        return e;
    }

    // process the result
    auto mi_frame_list = MIGetFrameInfo(cmd);

    if (mi_frame_list == NULL) {
        EVENT_ERROR(e, DBGERR_NOSTACK, MICommandResultErrorMessage(cmd));
        MICommandFree(cmd);
        return e;
    }

    // get the frame
    MIListSet(mi_frame_list);
    auto mi_frame = (MIFrame *)MIListGet(mi_frame_list);

    if (!mi_frame) {
        MIListFree(mi_frame_list, NULL);
        EVENT_ERROR(e, DBGERR_NOSTACK, MICommandResultErrorMessage(cmd));
        MICommandFree(cmd);
        return e;
    }

    // create the result
    auto result = MakeFrameFromMIFrame(mi_frame);

    // free the MIFrame list
    MIListFree(mi_frame_list, (void (*)())MIFrameFree);
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
dbgevent_t *DbgGdbMI::setVar(const char *var, AIF *a) {
    DVPRINT("*** Dbg %s, %s\n", __FILE__, __func__);
    dbgevent_t *e;
    MICommand *cmd;
    char *data;
    char *expr;
    int len;

    if (a == NULL) {
        EVENT_ERROR(e, DBGERR_SETVAR, "Invalid input. AIF is null.");
        return e;
    }

    if (AIF_LEN(a) <= 0) {
        EVENT_ERROR(e, DBGERR_SETVAR,
                    "Attempt to set var with data of null length.");
        return e;
    }

    // get type info about var to make sure it exists
    if (((e = getType(var, 0))->ev_event) != DBGEV_TYPE) return e;
    FreeEvent(e);

    // convert the aif type to string form
    if (AIFToStr(&data, 0, a)) {
        EVENT_ERROR(e, DBGERR_SETVAR, "Could not convert AIF data to string.");
        return e;
    }

    // create the assignment string
    len = strlen(var) + strlen(data) + 2;
    expr = (char *)Alloc(len);
    snprintf(expr, len, "%s=%s", var, data);

    DVPRINT("DbgSetVar: expr is %s\n", expr);

    // set the var to the val of the aif var
    cmd = _MIGDBSet(expr, NULL);
    SendMICommand(GDBMISession, cmd);

    if (!MICommandResultOK(cmd)) {
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
dbgevent_t *DbgGdbMI::getType(const char *var, bool isType) {
    DVPRINT("*** Dbg %s, %s\n", __FILE__, __func__);
    dbgevent_t *e;
    MICommand *cmd;
    char *type;

    if (isType)
        cmd = CLIPtype(var);
    else
        cmd = CLIWhatis(var);

    SendMICommand(GDBMISession, cmd);
    if (!MICommandResultOK(cmd)) {
        EVENT_ERROR(e, DBGERR_NOSYM, var);
        MICommandFree(cmd);
        return e;
    }

    if (isType)
        type = CLIGetPtypeInfo(cmd);
    else
        type = CLIGetWhatisInfo(cmd);

    if (type == NULL) {
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

dbgevent_t *DbgGdbMI::setThread(const char *threadId) {
    DVPRINT("*** Dbg %s, %s, threadId(%s)\n", __FILE__, __func__, threadId);

    dbgevent_t* e = nullptr;

    char* idEnd = nullptr;
    int threadNo = strtol(threadId, &idEnd, 10);
    if (!idEnd || *idEnd) {
        EVENT_ERROR(e, DBGERR_BADTYPE, "thread command only supports integer thread ids");
        return e;
    }

    MICommand* cmd = MIThreadSelect(threadNo);

    SendMICommand(GDBMISession, cmd);
    if (!MICommandResultOK(cmd)) {
        EVENT_ERROR(e, DBGERR_DEBUGGER, "Gdb set thread failed.");
        MICommandFree(cmd);
        return e;
    }

    MICommandFree(cmd);

    e = NewEvent(DBGEV_OK);

    return e;
}

// remove process specific addresses and LWP ids from the thread ouput that would
// prevent them being reduced in MRNet
static std::string anonymizeThreadInfo(const std::string& val) {
    DVPRINT("*** Dbg %s, %s\n", __FILE__, __func__);
    static std::regex hexValRE(R"(0x[0-9a-f]+( in )?)");  // the extra ( in ) is for the proc counter
    static std::regex lwpRE(R"(Thread.*\(LWP \d+\))");
    static std::regex amdGpuRE(R"(AMDGPU Thread [0-9.,() ]+/[0-9]+)");
    static std::regex multispaceRE(R"(\s\s+)");  // gdb format alignment can stymie aggregation

    std::istringstream is(val);
    std::ostringstream os;

    std::string line;
    for (int lineNo = 0; std::getline(is,line); ++lineNo) {
        if (!lineNo) {
            // heading line
            os << "  Id    Frame\n";
            continue;
        }

        line = std::regex_replace(line, lwpRE, "");
        line = std::regex_replace(line, hexValRE, "");
        line = std::regex_replace(line, amdGpuRE, "AMDGPU");
        line = std::regex_replace(line, multispaceRE, " ");

        os << line << "\n";
    }

    return os.str();
}

namespace {
// Class for taking thread-id, value pairs and assembling id's with the same values
// into equivalence sets.
class ThreadCollector {
public:
    // Add an id,value pair.   *This will throw an exception if \a id is malformed*
    // This function must not be called after the first call to getNextResult.
    void add(std::string id, const std::string& val);

    // Get the next id-list value pair.   Return false if none remain.
    bool getNextResult(std::string& ids, std::string& val);
private:
    using Val2Id = std::map<std::string,std::string>;
    Val2Id m_val2Id;

    using ToSort = std::pair<int,Val2Id::iterator>;
    std::vector<ToSort> m_sortedResult;
    std::vector<ToSort>::iterator m_currentResult;
};

// add an id,value pair for collection
void ThreadCollector::add(std::string id, const std::string& val) {
    if (!m_sortedResult.empty()) {
        DVPRINT("logic error, adding a value to ThreadCollector after extraction has begun.\n");
        return;
    }

    // store the location in val2Id
    auto ins = m_val2Id.insert(std::make_pair(val,id));
    if (ins.second)
        return;

    // duplicate location, merge into the id list
    std::string& ids = ins.first->second;
    if (id.find('*') != std::string::npos) {
        // the default thread indicator for 'info threads' put it at the head of the list
        ids = id + ids;
        return;
    }

    id = id.substr(id.find_first_not_of(" \t")); // strip leading spaces

    // get the last id in the current list
    size_t delimLoc = ids.find_last_of(" -");
    size_t prevIdLoc = (delimLoc == std::string::npos ? 0 : delimLoc+1);

    // determine if this just creates or extends a range
    int prev = std::stoi(ids.substr(prevIdLoc));  // throws if not an int
    int curr = std::stoi(id);
    if (curr != prev+1) {
        // just append to the id list
        ids += " " + id;
    } else if (delimLoc != std::string::npos && ids[delimLoc] == '-') {
        // extending a range
        ids.replace(prevIdLoc,std::string::npos,id);
    } else {
        // creating a new range
        ids += "-" + id;
    }
}

// get the next id-list, value pair of the collected result
bool ThreadCollector::getNextResult(std::string& ids, std::string& val) {
    //DVPRINT("*** Dbg %s, %s\n", __FILE__, __func__); //called too frequently
    if (m_val2Id.empty()) {
        return false;
    }

    if (m_sortedResult.empty()) {
        // first call - sort the entries into ascending order.  We just refer into
        // the map to avoid copies
        for (Val2Id::iterator it = m_val2Id.begin(); it != m_val2Id.end(); ++it) {
            auto id = it->second;
            size_t idLoc = id.find_first_not_of(" \t*");
            int idNo = strtol(id.c_str()+idLoc,nullptr,10);
            m_sortedResult.push_back(ToSort(idNo,it));
        }

        // and sort
        sort(m_sortedResult.begin(), m_sortedResult.end(),
             [](const ToSort& a,const ToSort& b) { return a.first < b.first; });
        m_currentResult = m_sortedResult.begin();
    }

    if (m_currentResult == m_sortedResult.end()) {
        return false;
    }

    // we could potentially avoid the copy for the value, but it would make the code a lot more complicated
    // (the map key is rightly const).
    auto& pr = *m_currentResult++->second;
    ids = move(pr.second);
    val = pr.first;

    return true;
}
}

// identify threads with equivalent frame information to present the information more
// compactly.   Similar threads will be identified by an id list at is:
// id1 id2 id3-id4
static std::string collectThreadInfo(const std::string& val) {
    //DVPRINT("*** Dbg %s, %s\n", __FILE__, __func__); //called too frequently
    std::regex re(R"(^( *\*? *\d+)\s+(.*))");

    ThreadCollector collector;

    std::istringstream is(val);
    std::string line, header;
    for (int lineNo = 0; std::getline(is,line); ++lineNo) {
        if (!lineNo) {
            // heading line
            header = line;
            continue;
        }

        // get the thread-id and location for this line
        std::smatch match;
        if (!std::regex_match(line, match, re)) {
            DVPRINT("did not match line on %d:%s\n", lineNo, line.c_str());;
            // can't parse the line - luckily we can just *not* collect
            return val;
        }

        const std::string& loc = match[2];
        std::string id = match[1];

        try {
            collector.add(id, loc);
        } catch (std::exception&) {
            return val;  // couldn't resolve an id, guess we're not collecting
        }
    }

    // and assemble the result
    std::ostringstream os;
    os << header << "\n";

    for (std::string id, loc; collector.getNextResult(id,loc); ) {
        os << std::setw(7) << std::left << id << " " << loc << "\n";
    }
    return os.str();
}

// translate an extended format thread list into the set of actual thread ids.
// \param threadId the thread list, it recognizes: 1 2 3-4 5..6 "regex" 'regex' in a list
//    using either spaces or commas as a separator
// \param suffix if specified will be assigned and part of \a threadId that follows a
//      thread specification.   If this value is null, any remaining text is an error.
// \param error will be assigned an error message if the list cannot be parsed, or if
//   the name patterns do not match any threads.
// \return the set of individual threads from the list
std::vector<int> DbgGdbMI::getThreadSet(const std::string& threadId, std::string* suffix,
                                        std::string& error) {
    // note that all each of these RE contains a tail that matches the next space or comma
    // in the list  (we add a space to the original string to simplify matching)
    using namespace std::string_literals;
    static auto post = R"((,|(\s+,?)))"s;  // slurp up the list separator
    static auto rangeRE=std::regex{R"(^\s*(\d+)\s*(-|(\.\.))\s*(\d+))" + post};  // 1-2, 1..2
    static auto idRE=std::regex{R"(^\s*(\d+))"s + post}; // 1
    static auto regexRE=std::regex{"^\\s*\"(([^\"\\\\]*\\\\.)*[^\"\\\\]*)\""s + post};  // "re\"g\"ex"
    static auto regexREsq=std::regex{"^\\s*'(([^'\\\\]*\\\\.)*[^'\\\\]*)'"s + post};    // 're\'g\'ex'
    static auto starRE=std::regex{R"(\s*(\*))"s + post};   // * note in parens to match regexRE

    static auto threadFindRE=std::regex{R"((^|\n)Thread\s+(\d+)\s+)"};     // Thread id 
        
    auto hasPattern = false;
    auto toParse = std::string{threadId} + " ";

    std::vector<int> ids;

    for (std::smatch match; !toParse.empty(); toParse = match.suffix().str()) {
        if (std::regex_search(toParse, match, rangeRE)) {
            // range of ids
            auto a = atoi(match[1].str().c_str());
            auto b = atoi(match[4].str().c_str());
            for (auto i=a; i<=b; ++i) {
                ids.push_back(i);
            }
        } else if (std::regex_search(toParse, match, idRE)) {
            // lone id
            auto a = atoi(match[1].str().c_str());
            ids.push_back(a);
        } else if (std::regex_search(toParse, match, regexRE) ||
                   std::regex_search(toParse, match, regexREsq) ||
                   std::regex_search(toParse, match, starRE)) {
            // use thread find to get the matching threads
            hasPattern = true;
            auto pattern = match[1].str();
            if (pattern == "*") pattern = ".*"; // match every thread
            auto ev = findThread(pattern.c_str());

            if (ev->ev_event == DBGEV_THREADINFO && ev->ev_string) {
                const char* findRes = ev->ev_string, *findEnd = findRes + strlen(findRes);
                for (std::cregex_iterator it(findRes,findEnd,threadFindRE), end;
                     it != end; ++it) {
                    ids.push_back(atoi((*it)[2].str().c_str()));
                }
            }
        } else {
            break;              // the next thing isn't parse-able
        }
    }

    if (!toParse.empty()) {
        // remove the space we added to ease parsing.
        toParse.erase(toParse.size()-1);
    }

    if (suffix) {
        *suffix = toParse;
    }

    if (!suffix && !toParse.empty()) {
        error = "junk at end of thread list:\"" + toParse + "\"";
    } else if (hasPattern && ids.empty()) {
        // a just plain empty list is fine - it means all (at least with info threads)
        error = "no threads match the pattern";
    }

    // canonicalize and make unique (thread find will return the same thread multiple times if
    // the regex matches multiple parts of the thread description)
    std::sort(ids.begin(), ids.end());
    ids.erase(std::unique(ids.begin(), ids.end()), ids.end());

    return ids;
}


// get a listing of either all available threads or a subset depending on the arguments
// in \a threadId
dbgevent_t *DbgGdbMI::getThreadInfo(const char *threadId) {
    DVPRINT("*** Dbg %s, %s, threadId(%s)\n", __FILE__, __func__, threadId);

    dbgevent_t* e = nullptr;

    bool doAnonymize = true;
    bool doCollect = true;

    // process the flags first
    static std::regex flagRE{R"(^\s*(-[a-z]+)($|(\s+)))"};
    std::string threadArgs{threadId};

    for (std::smatch match; std::regex_search(threadArgs, match, flagRE); threadArgs=match.suffix().str()) {
        auto f = match[1].str();
        if (f == "-v") {
            doAnonymize = doCollect = false;
        } else if (f == "-l") {
            doCollect = false;
        } else {
            EVENT_ERROR(e, DBGERR_DEBUGGER, "unrecognized flag %s", f.c_str());
            DVPRINT("unrecognized flag %s\n", f.c_str());
        }
    }

    // parse the thread list to get the real thread ids
    std::string error;
    auto ids = getThreadSet(threadArgs.c_str(), nullptr, error);

    if (!error.empty()) {
        EVENT_ERROR(e, DBGERR_DEBUGGER, error.c_str());
        return e;
    }

    // get the arguments ready
    std::vector<std::string> args{"info", "threads"};
    std::transform(ids.begin(), ids.end(), std::back_inserter(args),
                   [](int i) { return std::to_string(i); });

    std::vector<const char*> cargs;
    std::transform(args.begin(), args.end(), std::back_inserter(cargs),
                   [](const std::string& s) { return s.c_str(); });

    // get the result from gdb
    MICommand* cmd = CLIBypassc(cargs.size(), &cargs[0]);
    SendMICommand(GDBMISession, cmd);
    if (!MICommandResultOK(cmd)) {
        EVENT_ERROR(e, DBGERR_DEBUGGER, "Gdb get thread info failed.");
        MICommandFree(cmd);
        return e;
    }

    // reformat the return to be reduced
    if (char* threadInfo = MICommandConsoleOOBMessages(cmd)) {
        std::string info(threadInfo);
        free(threadInfo);

        if (doAnonymize) info = anonymizeThreadInfo(info);
        if (doCollect) info = collectThreadInfo(info);

        e = NewEvent(DBGEV_THREADINFO);
        e->ev_string = strdup(info.c_str());
    } else {
        EVENT_ERROR(e, DBGERR_DEBUGGER, "Gdb get thread info failed.");
    }

    MICommandFree(cmd);
    return e;
}

dbgevent_t *DbgGdbMI::setThreadName(const char* name) {
    DVPRINT("*** Dbg %s, %s\n", __FILE__, __func__);

    const char* cmd_args[3] = { "thread", "name", name };
    MICommand* cmd = CLIBypassc(3, cmd_args);
    SendMICommand(GDBMISession, cmd);

    dbgevent_t* e = nullptr;
    if (!MICommandResultOK(cmd)) {
        EVENT_ERROR(e, DBGERR_DEBUGGER, "Gdb set thread name failed.");
    } else {
        e = NewEvent(DBGEV_OK);
    }

    MICommandFree(cmd);
    return e;
}

dbgevent_t *DbgGdbMI::findThread(const char* pattern) {
    DVPRINT("*** Dbg %s, %s\n", __FILE__, __func__);

    const char* cmd_args[3] = { "thread", "find", pattern };

    MICommand* cmd = CLIBypassc(3, cmd_args);
    SendMICommand(GDBMISession, cmd);

    dbgevent_t* e = nullptr;
    if (!MICommandResultOK(cmd)) {
        EVENT_ERROR(e, DBGERR_DEBUGGER, "Gdb thread find failed.");
        MICommandFree(cmd);
        return e;
    }

    if (char* threadInfo = MICommandConsoleOOBMessages(cmd)) {
        e = NewEvent(DBGEV_THREADINFO);
        e->ev_string = threadInfo;
    } else {
        EVENT_ERROR(e, DBGERR_DEBUGGER, "Gdb thread find failed to get a result.");
    }

    MICommandFree(cmd);
    return e;

}

// parse the command line given to thread apply and extract the flags aimed at gdb4hpc, and return the
// operation string intended for gdb.
//
// the backend gdb doesn't recognize some of the more useful options for apply including:
// -s (silent) suppresses output from threads that produce errors (like a variable not defined)
// -q (quiet) suppresses printing the thread information
// -c (continue) displays error messages but keeps going
// -v (verbose) a gdb4hpc extension - don't strip out hex values
// also if the thread set is "all" we want to add a -ascending flag to keep the output sane.
std::string DbgGdbMI::parseThreadApplyOp(const std::string& op, bool& isSilent, bool& isQuiet, bool &isVerbose) {
    DVPRINT("*** Dbg %s, %s\n", __FILE__, __func__);
    isSilent = isQuiet = isVerbose = false;

    std::istringstream is(op);
    std::ostringstream os;
    std::string arg;

    bool inFlags = true, isAll = false, isAscending = false, isFirst = false;
    while ( is >> arg) {
        bool addArg = true;
        char c = arg[0];
        if (!inFlags) {}
        else if (arg == "all") {
            isAll = true;
        } else if (isdigit(c)) {
        } else if (c != '-') {
            inFlags = false;
            if (isAll && !isAscending) {
                os << " -ascending";
            }
        } else if (arg == "-ascending") {
            isAscending = true;
        } else if (arg == "-s") {
            addArg = false;
            isSilent = true;
        } else if (arg == "-q") {
            addArg = false;
            isQuiet = true;
        } else if (arg == "-c") {
            // kept just for compatibility, this is the default behavior
            addArg = false;
        } else if (arg == "-v") {
            addArg = false;
            isVerbose = true;
        } else {
            return "error:unrecognized flag " + arg;
        }

        if (addArg) {
            os << (isFirst ? "" : " ") << arg;
            isFirst = false;
        }
    }

    if (isAll) {
        return os.str();
    }

    // otherwise, pull find the set of threads
    std::string command, error;
    auto threadIds = getThreadSet(os.str(), &command, error);

    if (!error.empty()) {
        return "error:" + error;
    } else if (threadIds.empty()) {
        return "error:no threads specified for apply";
    } else if (command.empty()) {
        return "error:no command given for thread apply";
    }

    return threadIdsToString(threadIds) + " " + command;
}

// process the output from thread apply using the extended flags
// isSilent: suppress ouput from threads that produce errors (like a variable not defined)
// isQuiet: suppress printing the thread information
// isVerbose: a gdb4hpc extension - don't strip out hex values
static std::string processThreadApplyOutput(const std::string& in, bool isSilent, bool isQuiet, bool isVerbose) {
    DVPRINT("*** Dbg %s, %s\n", __FILE__, __func__);
    std::regex headerRE(R"(^Thread\s+(\d+)\s*\(Thread[^:]*:)");
    std::regex hexRE(R"(0x[0-9a-fA-F]+)");
    std::regex convenienceVarRE(R"(^\$\d+\s*=\s*)");

    std::string line;

    if (!isVerbose) {
        // The collected version of thread apply; we report theads with the same result together
        // in a thead list.
        try {
            std::istringstream is(in);
            ThreadCollector collector;
            std::string id;
            std::ostringstream result;

            while (std::getline(is, line)) {
                std::smatch match;
                if (std::regex_match(line, match, headerRE)) {
                    if (!result.str().empty()) {
                        collector.add(id,result.str());
                        result.str("");
                    }
                    id = std::string(match[1]);
                } else if (!line.empty() && (!isSilent || line[0] != '<')) {
                    line = std::regex_replace(line, hexRE, "");
                    line = std::regex_replace(line, convenienceVarRE, "");
                    result << line << "\n";
                }
            }
            if (!result.str().empty()) {
                collector.add(id, result.str());
            }

            // Print out the thread info - if this format changes, printValWithThreads in cmd_print.cpp
            // would need to changed to match.  (or possibly consider changing to a more structured
            // form for the data)
            std::ostringstream os;
            for (std::string val; collector.getNextResult(id,val); ) {
                if (!isQuiet) {
                    os << "Thread " << id << ":\n";
                }
                os << val;
            }
            return os.str();
        } catch (std::exception&) {
            // we can just fall back on the non-collected version
            DVPRINT("Thread apply collection failed on line %s\n", line.c_str());
        }
    }

    // the non-collected version of thread apply.
    std::istringstream is(in);
    std::ostringstream os;
    std::string header;
    while (std::getline(is, line)) {
        std::smatch match;
        if (std::regex_match(line, match, headerRE)) {
            if (isVerbose) {
                header = match[0];
            } else {
                header = "Thread " + std::string(match[1]) + ":";
            }
        } else if (!line.empty() && (!isSilent || line[0] != '<')) {
            if (!header.empty()) {
                if (!isQuiet) {
                    os << header << "\n";
                }
                header.clear();
            }
            if (!isVerbose) {
                line = std::regex_replace(line, hexRE, "");
                line = std::regex_replace(line, convenienceVarRE, "");
            }
            os << line << "\n";
        }
    }

    return os.str();
}

dbgevent_t *DbgGdbMI::threadApply(const char* inOp) {
    DVPRINT("*** Dbg %s, %s\n", __FILE__, __func__);

    bool isSilent = false, isQuiet = false, isVerbose = false;
    std::string op = parseThreadApplyOp(inOp, isSilent, isQuiet, isVerbose);

    dbgevent_t* e = nullptr;

    if (op.substr(0,6) == "error:") {
        EVENT_ERROR(e, DBGERR_DEBUGGER, op.substr(6).c_str());
        return e;
    }

    const char* cmd_args[3] = { "thread", "apply", op.c_str() };

    MICommand* cmd = CLIBypassc(3, cmd_args);
    SendMICommand(GDBMISession, cmd);

    if (!MICommandResultOK(cmd)) {
        EVENT_ERROR(e, DBGERR_DEBUGGER, "Gdb thread apply failed.");
        MICommandFree(cmd);
        return e;
    }

    if (char* threadInfo = MICommandConsoleOOBMessages(cmd)) {
        std::string info(threadInfo);
        free(threadInfo);

        info = processThreadApplyOutput(info, isSilent, isQuiet, isVerbose);
        e = NewEvent(DBGEV_THREADINFO);
        e->ev_string = strdup(info.c_str());
    } else {
        EVENT_ERROR(e, DBGERR_DEBUGGER, "Gdb thread apply failed to get a result.");
    }

    MICommandFree(cmd);
    return e;
}

dbgevent_t *DbgGdbMI::setSchedulerLocking(const char* mode) {
    DVPRINT("*** Dbg %s, %s\n", __FILE__, __func__);

    const char* cmd_args[3] = { "set", "scheduler-locking", mode };
    MICommand* cmd = CLIBypassc(3, cmd_args);
    SendMICommand(GDBMISession, cmd);

    dbgevent_t* e = nullptr;
    if (!MICommandResultOK(cmd)) {
        EVENT_ERROR(e, DBGERR_DEBUGGER, "Gdb set scheduler-locking failed.");
    } else {
        e = NewEvent(DBGEV_OK);
    }

    MICommandFree(cmd);
    return e;
}

dbgevent_t *DbgGdbMI::getSchedulerLocking() {
    DVPRINT("*** Dbg %s, %s\n", __FILE__, __func__);

    const char* cmd_args[2] = { "show", "scheduler-locking"};
    MICommand* cmd = CLIBypassc(2, cmd_args);
    SendMICommand(GDBMISession, cmd);

    dbgevent_t* e = nullptr;
    if (!MICommandResultOK(cmd)) {
        EVENT_ERROR(e, DBGERR_DEBUGGER, "Gdb get scheduler-locking failed.");
    } else if (char* mode = MICommandConsoleOOBMessages(cmd)) {
        e = NewEvent(DBGEV_THREADINFO);
        e->ev_string = mode;
    } else {
        EVENT_ERROR(e, DBGERR_DEBUGGER, "Gdb get scheduler-locking did not return a value.");
    }

    MICommandFree(cmd);
    return e;
}

dbgevent_t *DbgGdbMI::setNonStop(const char* mode) {
    DVPRINT("*** Dbg %s, %s\n", __FILE__, __func__);

    MICommand* cmd = MICommandNew("-gdb-set", MIResultRecordDONE);
    MICommandAddOption(cmd, "non-stop", mode);
    SendMICommand(GDBMISession, cmd);

    dbgevent_t* e = nullptr;
    if (!MICommandResultOK(cmd)) {
        EVENT_ERROR(e, DBGERR_DEBUGGER, MICommandResultErrorMessage(cmd));
    } else {
        e = NewEvent(DBGEV_OK);
    }

    MICommandFree(cmd);
    return e;
}

dbgevent_t *DbgGdbMI::getNonStop() {
    DVPRINT("*** Dbg %s, %s\n", __FILE__, __func__);

    MICommand* cmd = MICommandNew("-gdb-show", MIResultRecordDONE);
    MICommandAddOption(cmd, "non-stop", nullptr);
    SendMICommand(GDBMISession, cmd);

    dbgevent_t* e = nullptr;
    if (!MICommandResultOK(cmd)) {
        EVENT_ERROR(e, DBGERR_DEBUGGER, "Gdb get non-stop failed.");
    } else if (char* mode = MICommandOutputString(cmd)) {
        e = NewEvent(DBGEV_THREADINFO);
        e->ev_string = mode;
    } else {
        EVENT_ERROR(e, DBGERR_DEBUGGER, "Gdb get non-stop did not get a string");
    }

    MICommandFree(cmd);
    return e;
}


/*
** List variables in the current stack frame
*/
dbgevent_t *DbgGdbMI::getVars(const char *file) {
    DVPRINT("*** Dbg %s, %s\n", __FILE__, __func__);
    dbgevent_t *e;
    dbgvars_t *result = NULL;
    dbgvars_t *cur_var = NULL;
    dbgvars_t *new_var = NULL;

    MICommand *cmd;
    MIList *vars;
    MIArg *var;

    cmd = MIStackListVariables(SIMPLE_VALUES);
    SendMICommand(GDBMISession, cmd);

    if (!MICommandResultOK(cmd)) {
        EVENT_ERROR(e, DBGERR_DEBUGGER, MICommandResultErrorMessage(cmd));
        MICommandFree(cmd);
        return e;
    }

    if ((vars = MIGetStackListVariablesInfo(cmd)) == NULL) {
        EVENT_ERROR(e, DBGERR_DEBUGGER, MICommandResultErrorMessage(cmd));
        MICommandFree(cmd);
        return e;
    }
    MICommandFree(cmd);

    // parse over the vars list
    for (MIListSet(vars); (var = (MIArg *)MIListGet(vars)) != NULL;) {
        // create the new var
        new_var = (dbgvars_t *)Alloc(sizeof(dbgvars_t));

        // first time through we want to set the root var as the result
        if (result == NULL) result = new_var;

        // set the cur_var var_next pointer to the new var if this isn't
        // the first time through
        if (cur_var != NULL) cur_var->var_next = new_var;

        // set the var_next to null
        new_var->var_next = (dbgvars_t *)NULL;

        // set the name
        if (var->name != NULL) {
            DVPRINT("var_name: %s\n", var->name);
            new_var->var_name = strdup(var->name);
        } else {
            DVPRINT("var_name: NULL\n");
            new_var->var_name = (char *)NULL;
        }

        // set the type
        if (var->type != NULL) {
            DVPRINT("var_type: %s\n", var->type);
            new_var->var_type = strdup(var->type);
        } else {
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
    MIListFree(vars, (void (*)())MIArgFree);

    return e;
}

/*
** List local variables relative to a scope
*/
dbgevent_t *DbgGdbMI::getScope(const char *location) {
    DVPRINT("*** Dbg %s, %s\n", __FILE__, __func__);
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

    cmd = CLIInfoScope(location);

    SendMICommand(GDBMISession, cmd);
    if (!MICommandResultOK(cmd)) {
        MICommandFree(cmd);
        EVENT_ERROR(e, DBGERR_DEBUGGER, MICommandResultErrorMessage(cmd));
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
            DVPRINT("var_name: %s\n", name);
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

    // create the event with the var information
    e = NewEvent(DBGEV_VARS);
    e->ev_vars = result;

    // Now we need to get the type information based on the scope. First figure
    // out what function the scope referrs to by setting a breakpoint to get
    // the function name.

    cmd = MIBreakInsert(0, 0, 0, NULL, 0, location, 0);
    SendMICommand(GDBMISession, cmd);

    if (!MICommandResultOK(cmd)) {
        // free existing event
        FreeEvent(e);
        EVENT_ERROR(e, DBGERR_DEBUGGER, MICommandResultErrorMessage(cmd));
        MICommandFree(cmd);
        return e;
    }

    if ((bpts = MIBreakpointGetBreakInsertInfo(cmd)) == NULL) {
        // free existing event
        FreeEvent(e);
        // If we can't get the breakpoint info, we error out
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
    SendMICommand(GDBMISession, cmd);
    // We don't care if the delete breakpoint failed. I think this is okay to
    // do since it won't cause anything to break. This assumption might be
    // wrong though.
    MICommandFree(cmd);

    // Now loop through the vars and get the type for each name
    for (vp = &result; *vp != NULL; vp = &(*vp)->var_next) {
        Asprintf(&var_str, "%s::%s", bpfunc, (*vp)->var_name);
        cmd = CLIWhatis(var_str);
        Free(var_str);

        SendMICommand(GDBMISession, cmd);
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

    return e;
}

/*
** Get debugger status.
*/
dbgevent_t *DbgGdbMI::getStatus(bool getLocation) {
    DVPRINT("*** DbgStatus %s, %s\n", __FILE__, __func__);

    const char* threads = nullptr;  // thread focus unimplemented
    dbgevent_t *e = NewEvent(DBGEV_STAT), *async_e = nullptr;

    // Create the new status object
    dbgstatus_t *s = e->ev_status = (dbgstatus_t *)Alloc(sizeof(dbgstatus_t));

    // Assign the status
    s->stat = Status;

    // If getLocation is 1, we need to populate the location information if its
    // available
    if (getLocation) {
        // Switch on the current Status
        switch (Status) {
            // These should generate location information
            case DBGSTAT_WAITING:
            case DBGSTAT_STOPPED:
            case DBGSTAT_STOPPED_IN_THREAD:
            case DBGSTAT_EXIT:
                // Reuse DbgGDBMIShowFrame to get the bottommost frame
                async_e = showFrame(1, threads);

                if (async_e->ev_frame != NULL) {
                    auto frameLoc = async_e->ev_frame->getLoc();

                    s->stat_loc = (dbgloc_t *)Alloc(sizeof(dbgloc_t));
                    s->stat_loc->loc_file = StrdupOpt(frameLoc.loc_file);
                    s->stat_loc->loc_func = StrdupOpt(frameLoc.loc_func);
                    s->stat_loc->loc_addr = nullptr; // We don't care about the address
                    s->stat_loc->loc_line = frameLoc.loc_line;
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
    } else {
        // Location information not requested
        s->stat_loc = NULL;
    }

    if (Status != DBGSTAT_STOPPED || !non_stop_flag) {
        return e;
    }

    // there's potentially more work to do if we're in non-stop mode
    if (dbgevent_t* threadEvent = getThreadInfo("")) {
        if (threadEvent->ev_event == DBGEV_THREADINFO &&
            strstr(threadEvent->ev_string, "(running)")) {
            s->stat = DBGSTAT_STOPPED_IN_THREAD;
        }
        FreeEvent(threadEvent);
    }

    return e;
}

/*
** Quit debugger.
*/
dbgevent_t *DbgGdbMI::quit(bool detach) {
    DVPRINT("*** Dbg %s, %s\n", __FILE__, __func__);
    dbgevent_t *e = nullptr;
#ifdef ALPS
    MICommand *cmd;
#endif

    /*
    ** Interrupt the process if it is running. This will generate a async event.
    ** We want to grab it and ignore it.
    ** We only want to do this if we are not detaching.
    */
    if (Status == DBGSTAT_RUNNING && !detach) {
        DVPRINT("DbgQuit: Sending SIGSTOP to process.\n");
        ::kill(Pid, SIGSTOP);
        // wait for the asynch command to complete
        while (!bufferReady()) MISessionProgress(GDBMISession);
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
    ** means that when this happens there is a chance that the application will
    *continue
    ** and often cause a segfault. Detatching and then sending SIGSTOP fixes the
    ** issue.
    */

    // If the command completed ok, we need to send SIGSTOP ASAP!!!
    if (MICommandResultOK(cmd) && !detach) {
        ::kill(Pid, SIGSTOP);
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
dbgevent_t *DbgGdbMI::interrupt(bool all) {
    DVPRINT("*** Dbg %s, %s(bool all %s)\n", __FILE__, __func__, all?"true":"false");
    dbgevent_t *e;
    dbgevent_t *async_e;

    const char* threads = nullptr;  // thread focus is unimplemented

    if (Status == DBGSTAT_RUNNING) {
        DVPRINT("RUNNING - sending SIgSTOP\n");
        if (::kill(Pid, SIGSTOP)) {
            switch (errno) {
                case EINVAL:
                    EVENT_ERROR(e, DBGERR_DEBUGGER, "Invalid signal SIGSTOP.");
                    break;
                case EPERM:
                    EVENT_ERROR(e, DBGERR_DEBUGGER,
                                "No permission to send SIGSTOP.");
                    break;
                case ESRCH:
                    EVENT_ERROR(e, DBGERR_DEBUGGER,
                                "No such Pid to send SIGSTOP.");
                    break;
                default:
                    EVENT_ERROR(e, DBGERR_DEBUGGER, "Unknown kill() error.");
                    break;
            }

            return e;
        }

        // wait for the asynch command to complete
        while (!bufferReady()) MISessionProgress(GDBMISession);

        async_e = WaitEvent(NULL);

        if (!all) {
            e = NewEvent(DBGEV_HALT);
            e->ev_loc = (dbgloc_t *)NULL;

            if (async_e->ev_sig_frame != NULL) {
                auto frameLoc = async_e->ev_sig_frame->getLoc();
                e->ev_loc = (dbgloc_t *)Alloc(sizeof(dbgloc_t));
                e->ev_loc->loc_file = StrdupOpt(frameLoc.loc_file);
                e->ev_loc->loc_func = StrdupOpt(frameLoc.loc_func);
                e->ev_loc->loc_addr = nullptr;  // We don't care about the address
                e->ev_loc->loc_line = frameLoc.loc_line;
            }

            FreeEvent(async_e);

            return e;
        }
    }

    if (Status != DBGSTAT_RUNNING || all) {
        DVPRINT("sending interrupt command\n");

        // In non-stop mode we want "halt" to stop the current thread.
        // Calling "interrupt" if the current thread is stopped is harmless
        // so we won't try to avoid the call in all stop mode.
        // Also, adding the interrupt call in the RUNNING state when selecting all
        // because in non-stop the signale will only stop one thread.
        // Note: MIExecInterrupt has a hack that causes it to hang, so using
        // the console command.
        const char* cmdTxt[2] = { "interrupt", "-a" };
        MICommand* cmd = CLIBypassc(all ? 2 : 1,cmdTxt);
        SendMICommand(GDBMISession, cmd);
        if (!MICommandResultOK(cmd)) {
            DVPRINT("MI -exec-interrupt failed\n");
        }
        MICommandFree(cmd);
    }


    // If we are already stopped, we will reuse DbgGDBMIShowFrame to get the
    // bottommost frame
    async_e = showFrame(1, threads);

    e = NewEvent(DBGEV_HALT);
    e->ev_loc = (dbgloc_t *)NULL;

    if (async_e->ev_frame != NULL && async_e->ev_event == DBGEV_FRAME) {
        auto frameLoc = async_e->ev_frame->getLoc();
        e->ev_loc = (dbgloc_t *)Alloc(sizeof(dbgloc_t));
        e->ev_loc->loc_file = StrdupOpt(frameLoc.loc_file);
        e->ev_loc->loc_func = StrdupOpt(frameLoc.loc_func);
        e->ev_loc->loc_addr = nullptr;  // We don't care about the address
        e->ev_loc->loc_line = frameLoc.loc_line;
    }

    FreeEvent(async_e);

    return e;
}

dbgevent_t *DbgGdbMI::invoke(const char *host, int cb, const char *proto,
                             const char *prog, const char *args,
                             char **env, int arch) {
    DVPRINT("*** Dbg %s, %s\n", __FILE__, __func__);
    dbgevent_t *e;

    MPArch = arch;

    e = initDbg(prog, args);

    Status = DBGSTAT_STOPPED;

    AsyncCheck((dbgevent_t * (*)(dbgevent_t *)) WaitEvent, (dbgevent_t *)NULL);

    return e;
}

/*
** Attach debugger to running process.
*/
dbgevent_t *DbgGdbMI::attach(int pid, const char *prog, const char *args,
                             char **env, int arch) {
    DVPRINT("*** Dbg %s, %s\n", __FILE__, __func__);
    dbgevent_t *e;

    MPArch = arch;

    switch (arch) {
        default:
            e = initDbgAttach(pid, prog);
            break;
    }

    Status = DBGSTAT_STOPPED;

    return e;
}

/*
** Called if Shutdown > 0 to finalise debugger.
*/
void DbgGdbMI::finalize() {
    MICommand *cmd;

    cmd = MIGDBExit();
    SendMICommand(GDBMISession, cmd);
    MICommandFree(cmd);
}

/*
** Check if debugger is ready to receive a command.
*/
bool DbgGdbMI::isReadyForCmd() {
    switch (Status) {
        case DBGSTAT_WAITING:
        case DBGSTAT_STOPPED:
        case DBGSTAT_STOPPED_IN_THREAD:
        case DBGSTAT_EXIT:
            return true;
        case DBGSTAT_INITIALISING:
        case DBGSTAT_RUNNING:
        case DBGSTAT_DBGERR:
        case DBGSTAT_INTERR:
            return false;
    }
    return false;
}

/*
** Check that debugger is ready ok
*/
bool DbgGdbMI::isOk() { return (Status != DBGSTAT_DBGERR); }

/*
** Check if debugger is ready to receive a command.
*/
bool DbgGdbMI::isStopped() {
    return (Status == DBGSTAT_STOPPED || Status == DBGSTAT_EXIT);
}

/*
** Set contents of GDB debug variable var.
*/
dbgevent_t *DbgGdbMI::setDebuggerVar(const char *var, AIF *val) {
    DVPRINT("*** Dbg %s, %s\n", __FILE__, __func__);
    MICommand *cmd;
    int intVal;

    AIFToInt(val, &intVal);

    cmd = MICommandNew("-gdb-set", MIResultRecordDONE);
    MICommandAddOption(cmd, var, MIIntToCString(intVal));
    SendMICommand(GDBMISession, cmd);
    MICommandFree(cmd);

    return NewEvent(DBGEV_OK);
}

dbgevent_t *DbgGdbMI::setSourcePath(const char *path) {
    DVPRINT("*** Dbg %s, %s\n", __FILE__, __func__);
    char *pathdirs[2];
    MICommand *cmd;
    MIString *cmd_result;
    char *res;
    dbgevent_t *e;

    pathdirs[0] = const_cast<char *>(path);
    pathdirs[1] = NULL;

    cmd = MIEnvironmentDirectory(0, pathdirs);
    SendMICommand(GDBMISession, cmd);
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

dbgevent_t *DbgGdbMI::getSourcePath() {
    DVPRINT("*** Dbg %s, %s\n", __FILE__, __func__);
    char *pathdirs[1];
    MICommand *cmd;
    MIString *cmd_result;
    char *res;
    dbgevent_t *e;

    pathdirs[0] = NULL;

    cmd = MIEnvironmentDirectory(0, pathdirs);
    SendMICommand(GDBMISession, cmd);
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

/*
** Get contents of GDB debug variable var.
*/
dbgevent_t *DbgGdbMI::getDebuggerVar(const char *var) {
    DVPRINT("*** Dbg %s, %s\n", __FILE__, __func__);
    MICommand *cmd;

    cmd = MICommandNew("-gdb-show", MIResultRecordDONE);
    MICommandAddOption(cmd, var, NULL);
    SendMICommand(GDBMISession, cmd);
    MICommandFree(cmd);

    return NewEvent(DBGEV_OK);
}

static dbgevent_t *ResetBeforeDirectCmd(void) {
    DVPRINT("*** Dbg %s, %s\n", __FILE__, __func__);
    MICommand *cmd;
    dbgevent_t *e;

    // we need gdb to set print elements and print repeats to 0 so
    // that when we resolve the data it gives us the entire contents
    // of the data type.
    cmd = _MIGDBSet("print elements", "200");
    SendMICommand(GDBMISession, cmd);
    if (!MICommandResultOK(cmd)) {
        MICommandFree(cmd);
        EVENT_ERROR(e, DBGERR_DEBUGGER,
                    "Gdb set print elements command failed.");
        return e;
    }
    MICommandFree(cmd);

    cmd = _MIGDBSet("print repeats", "10");
    SendMICommand(GDBMISession, cmd);
    if (!MICommandResultOK(cmd)) {
        MICommandFree(cmd);
        EVENT_ERROR(e, DBGERR_DEBUGGER,
                    "Gdb set print repeats command failed.");
        return e;
    }
    MICommandFree(cmd);

    if (gdb_vers != GdbVersion::ROC_GDB) {
        cmd = _MIGDBSet("source open", "on");
        SendMICommand(GDBMISession, cmd);
        if (!MICommandResultOK(cmd)) {
            MICommandFree(cmd);
            EVENT_ERROR(e, DBGERR_DEBUGGER, "Gdb set source open command failed.");
            return e;
        }
        MICommandFree(cmd);
    }

    return NULL;
}

static dbgevent_t *RestoreAfterDirectCmd(void) {
    DVPRINT("*** Dbg %s, %s\n", __FILE__, __func__);
    MICommand *cmd;
    dbgevent_t *e;

    // we need gdb to set print elements and print repeats to 0 so
    // that when we resolve the data it gives us the entire contents
    // of the data type.
    cmd = _MIGDBSet("print elements", "0");
    SendMICommand(GDBMISession, cmd);
    if (!MICommandResultOK(cmd)) {
        MICommandFree(cmd);
        EVENT_ERROR(e, DBGERR_DEBUGGER,
                    "Gdb set print elements command failed.");
        return e;
    }
    MICommandFree(cmd);

    cmd = _MIGDBSet("print repeats", "0");
    SendMICommand(GDBMISession, cmd);
    if (!MICommandResultOK(cmd)) {
        MICommandFree(cmd);
        EVENT_ERROR(e, DBGERR_DEBUGGER,
                    "Gdb set print repeats command failed.");
        return e;
    }
    MICommandFree(cmd);

    if (gdb_vers != GdbVersion::ROC_GDB) {
        cmd = _MIGDBSet("source open", "off");
        SendMICommand(GDBMISession, cmd);
        if (!MICommandResultOK(cmd)) {
            MICommandFree(cmd);
            EVENT_ERROR(e, DBGERR_DEBUGGER,
                        "Gdb set source open command failed.");
            return e;
        }
        MICommandFree(cmd);
    }

    return NULL;
}

/*
** Pass command directly to gdb/mi through console command
*/
dbgevent_t *DbgGdbMI::directCmd(int cmd_argc, char **cmd_argv) {
    DVPRINT("*** Dbg %s, %s\n", __FILE__, __func__);
    MICommand *cmd;
    dbgevent_t *e;
    dbgevent_t *e2 = NULL;
    // FIXME: Restore after reaching a breakpoint later on!
    int restore = 1;

    // Reset some of the print settings to default, otherwise the user might
    // not get a response
    e = ResetBeforeDirectCmd();

    if (e != NULL) return e;

    cmd = CLIBypass(cmd_argc, cmd_argv);
    SendMICommand(GDBMISession, cmd);

    // Sometimes a command can cause the debugger to start running
    // If so the expected result record from CLIBypass would be wrong.
    // We don't want to use the MICommandResultOK command, rather we want to
    // check to see if it has an error.
    // We also want to update status in certain situations (like ^running)
    switch (MICommandResultClass(cmd)) {
        case MIResultRecordINVALID:
            e = RestoreAfterDirectCmd();
            if (e != NULL) return e;
            EVENT_ERROR(e, DBGERR_DEBUGGER,
                        "Invalid command returned by MI layer.");
            MICommandFree(cmd);
            return e;

        case MIResultRecordRUNNING:
            Status = DBGSTAT_RUNNING;
            // We can't restore if the status changes to running
            restore = 0;
            DirectCmdAsyncRestore = 1;
            // FIXME: Use a custom WaitEvent
            AsyncCheck((dbgevent_t * (*)(dbgevent_t *)) WaitEvent,
                       (dbgevent_t *)NULL);
            break;

        case MIResultRecordERROR:
            e = RestoreAfterDirectCmd();
            if (e != NULL) return e;
            EVENT_ERROR(e, DBGERR_DEBUGGER, MICommandResultErrorMessage(cmd));
            MICommandFree(cmd);
            return e;

        case MIResultRecordEXIT:
            // The user shouldn't be allowed to exit directly!!!
            Status = DBGSTAT_DBGERR;
            EVENT_ERROR(e, DBGERR_DEBUGGER,
                        "GDB reported exit! Things are likely broken.");
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

    if (restore) {
        e2 = RestoreAfterDirectCmd();

        if (e2 != NULL) {
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
dbgevent_t *DbgGdbMI::initDbg(const char *prog, const char *args) {
    DVPRINT("*** Dbg %s, %s\n", __FILE__, __func__);
    dbgevent_t *e;
    MICommand *cmd;
    MIList *bpts;
    MIBreakpoint *bpt;

    CLIInfoProcInfo *procInfo;
    int breakpointNumberAtMain = 0;

    cmd = MICommandNew("-file-exec-and-symbols", MIResultRecordDONE);
    MICommandAddOption(cmd, prog, NULL);
    SendMICommand(GDBMISession, cmd);
    MICommandFree(cmd);

    if (args != NULL) {
        e = setArgs(args);

        if (e->ev_event != DBGEV_OK) return e;

        FreeEvent(e);
    }

    /*
    ** Check for fortran.
    */
    cmd = MIBreakInsert(0, 0, 0, NULL, 0, "MAIN__", 0);
    SendMICommand(GDBMISession, cmd);
    if (MICommandResultOK(cmd)) {
        /*
        ** We are Fortran
        */
        ProgMain = "MAIN__";
    } else {
        MICommandFree(cmd);
        cmd = MIBreakInsert(0, 0, 0, NULL, 0, "main", 0);
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
    MIListFree(bpts, (void (*)())MIBreakpointFree);

    if (RunArgs != NULL) {
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
dbgevent_t *DbgGdbMI::initDbgAttach(int pid, const char *prog) {
    DVPRINT("*** Dbg %s, %s\n", __FILE__, __func__);
    dbgevent_t *e;
    MICommand *cmd;
    CLIInfoProcInfo *procInfo;

    cmd = MITargetAttach(pid);

    DBEPERFLOG(SetTimerStart("DbgAttach: -target-attach"));
    SendMICommand(GDBMISession, cmd);
    DBEPERFLOG(SetTimerStopAndLog("DbgAttach: -target-attach"));

    // Check for success/failure
    if (!MICommandResultOK(cmd)) {
        EVENT_ERROR(e, DBGERR_DEBUGGER, "Attach failed: %s",
                    MICommandResultErrorMessage(cmd));
        MICommandFree(cmd);
        return e;
    }

    MICommandFree(cmd);

    DVPRINT("InitDbgAttach: -target-attach sent.\n");

    /*
    ** -target-attach is a *asynch* command that will generate a MI event.
    ** we must grab it or it will fubar everything else up.
    */
    // grab the "temporary" breakpoint - we want to ignore it
    // this is always generated by an attach command.

    // wait for the asynch command to complete
    while (!bufferReady()) MISessionProgress(GDBMISession);

    DVPRINT("InitDbgAttach: Event ready!\n");

    e = WaitEvent(NULL);
    FreeEvent(e);

    /*
    ** Find out pid.
    */
    cmd = CLIInfoProc();
    SendMICommand(GDBMISession, cmd);
    if (!MICommandResultOK(cmd)) {
        EVENT_ERROR(e, DBGERR_DEBUGGER, "Could not get pid: %s",
                    MICommandResultErrorMessage(cmd));
        MICommandFree(cmd);
        return e;
    }
    procInfo = CLIGetInfoProcInfo(cmd);
    if (procInfo == NULL) {
        EVENT_ERROR(e, DBGERR_DEBUGGER, "Could not get pid: %s",
                    MICommandResultErrorMessage(cmd));
        MICommandFree(cmd);
        return e;
    }
    Pid = procInfo->pid;
    CLIInfoProcInfoFree(procInfo);
    MICommandFree(cmd);

    DVPRINT("InitDbgAttach(): Obtained pid: %d.\n", Pid);

    // setup default exit points
    if ((e = setup_gdbmi_exit_points()) != NULL) return e;

    return NewEvent(DBGEV_OK);
}

/*
** Invoked if there is input from the debugger after
** a non-terminating command (eg. run, step, etc.).
** Checks to see what caused the termination.
*/
static dbgevent_t *WaitEvent(dbgevent_t *e) {
    DVPRINT("*** Dbg %s, %s\n", __FILE__, __func__);
    dbgbp_t *bp = NULL;

    MIEvent *evt = PendingEvent;

    if (e == (dbgevent_t *)NULL) e = NewEvent(DBGEV_NONE);

    AsynchEventWaiting = 0;
    PendingEvent = NULL;

    // check if we should reset due to a direct command triggering the run state
    if (DirectCmdAsyncRestore) {
        DirectCmdAsyncRestore = 0;
        dbgevent_t *e2 = RestoreAfterDirectCmd();
        if (e2 != NULL) {
            FreeEvent(e);
            return e2;
        }
    }

    // check for MI error condition
    if (MIGetError()) {
        e = ChangeEvent(e, DBGEV_ERROR);
        e->ev_errno = DBGERR_DEBUGGER;
        EventErrStr(e, MIGetErrorStr());
        return e;
    }

    // check for null evt
    if (evt == NULL) {
        e = ChangeEvent(e, DBGEV_ERROR);
        e->ev_errno = DBGERR_DEBUGGER;
        EventErrStr(e, "WaitEvent called with NULL PendingEvent!");
        return e;
    }

    switch (evt->type) {
        case MIEventTypeBreakpointHit:
            DVPRINT("MIEventTypeBreakpointHit\n");
            DVPRINT("Breakpoint id is %d\n", evt->bkptno);

            if (Status == DBGSTAT_INITIALISING) {
                bp = NewBP(evt->bkptno);
                e->ev_event = DBGEV_BPHIT;
                e->ev_bp = bp;

                return e;
            }

            // set the status to stopped
            Status = DBGSTAT_STOPPED;

            bp = FindBPByID(BP, evt->bkptno);

            // We want to check for an exit breakpoint if we didn't find it in
            // the normal BP list.
            if (bp == NULL) {
                bp = FindBPByID(exit_BP, evt->bkptno);
                // set the status to exit if we are at an exit breakpoint
                if (bp != NULL) {
                    Status = DBGSTAT_EXIT;
                }
            }

            // If bp is still null, we have a problem.
            if (bp == NULL) {
                e = ChangeEvent(e, DBGEV_ERROR);
                e->ev_errno = DBGERR_DEBUGGER;
                EventErrStr(e, "Hit unknown breakpoint!");
                return e;
            }

            bp->bp_hits++;
            CurrBP = bp;

            // If the stored bp was pending, we need to modify it with the new information
            if (bp->bp_pending && evt->frame != NULL) {
                bp->bp_pending = 0;
                if (evt->frame->file != NULL)
                    bp->bp_loc.loc_file = strdup(evt->frame->file);
                if (evt->frame->func != NULL)
                    bp->bp_loc.loc_func = strdup(evt->frame->func);
                if (evt->frame->addr != NULL)
                    bp->bp_loc.loc_addr = strdup(evt->frame->addr);
                bp->bp_loc.loc_line = evt->frame->line;
            }

            e->ev_event = DBGEV_BPHIT;
            e->ev_bp = DupBP(bp);

            // Check to see if disposition was set to disable or delete
            // We have to do this now because if the bp was set to delete we
            // wouldn't be able to do a DupBP on it...
            // FIXME: This should probably be implemented by setting the
            // bp_deleted
            //         to 1 and cleaning up breakpoints on the next continue.
            switch (bp->bp_disp) {
                case DBGBP_DELETE_DISP:
                    // delete the breakpoint
                    // NOTE: We don't know if the evt->bkptno is in BP or
                    // exit_BP, so we
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
            if (bp == NULL) {
                e = ChangeEvent(e, DBGEV_ERROR);
                e->ev_errno = DBGERR_DEBUGGER;
                EventErrStr(e, "Hit unknown watchpoint!");
                return e;
            }

            bp->bp_hits++;

            // We need to update the breakpoint information with the evt frame.
            // This will change every time the breakpoint is hit.
            // First free the old information
            if (bp->bp_loc.loc_file != NULL) {
                Free(bp->bp_loc.loc_file);
                bp->bp_loc.loc_file = NULL;
            }
            if (bp->bp_loc.loc_func != NULL) {
                Free(bp->bp_loc.loc_func);
                bp->bp_loc.loc_func = NULL;
            }
            if (bp->bp_loc.loc_addr != NULL) {
                Free(bp->bp_loc.loc_addr);
                bp->bp_loc.loc_addr = NULL;
            }
            bp->bp_loc.loc_line = -1;

            // Now update it with the new info
            if (evt->frame != NULL) {
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
            // We have to do this now because if the bp was set to delete we
            // wouldn't be able to do a DupBP on it...
            // FIXME: This should probably be implemented by setting the
            // bp_deleted
            //         to 1 and cleaning up breakpoints on the next continue.
            switch (bp->bp_disp) {
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

        case MIEventTypeWatchpointScope:
            DVPRINT("MIEventTypeWatchpointScope\n");
            DVPRINT("Breakpoint id is %d\n", evt->number);

            bp = FindBPByID(BP, evt->number);

            // If bp is null, we have a problem.
            if (bp == NULL) {
                e = ChangeEvent(e, DBGEV_ERROR);
                e->ev_errno = DBGERR_DEBUGGER;
                EventErrStr(e, "Hit unknown watchpoint!");
                return e;
            }

            // event type: breakpoint deleted
            e->ev_event = DBGEV_BPDEL;
            e->ev_bp = DupBP(bp);

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
                // No need to get the line again since we already have it in the
                // MIFrame information.
                e->ev_step_line = NULL;
                e->ev_step_frame = MakeFrameFromMIFrame(evt->frame, false);
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
                // No need to get the line again since we already have it in the
                // MIFrame information.
                e->ev_step_line = NULL;
                e->ev_step_frame = MakeFrameFromMIFrame(evt->frame, false);
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
                // No need to get the line again since we already have it in the
                // MIFrame information.
                e->ev_step_line = NULL;
                e->ev_step_frame = MakeFrameFromMIFrame(evt->frame, false);
            }

            Status = DBGSTAT_STOPPED;
            MIEventFree(evt);
            return e;

        case MIEventTypeSignal:
            DVPRINT("MIEventTypeSignal %s\n", evt->sigName);

            e->ev_event = DBGEV_SIGNAL;
            e->ev_sig_type =
                (evt->sigName == NULL ? NULL : strdup(evt->sigName));
            e->ev_sig_line = NULL;
            e->ev_sig_frame = (dbgframe_t *)NULL;

            if (evt->frame != NULL) {
                e->ev_sig_frame = MakeFrameFromMIFrame(evt->frame, false);
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
static dbgevent_t *CheckBreak(MICommand *cmd, const dbgcomm_info *commInfo) {
    DVPRINT("*** Dbg %s, %s\n", __FILE__, __func__);
    int id;
    dbgbp_t *bp;
    dbgevent_t *e;
    MIBreakpoint *bpt;
    MIList *bpts;

    bpts = MIBreakpointGetBreakInsertInfo(cmd);

    if (bpts == NULL) {
        return NULL;
    }

    MIListSet(bpts);
    bpt = (MIBreakpoint *)MIListGet(bpts);

    id = bpt->number;

    bp = NewBP(id);

    if (bpt->isWpt) {
        if (bpt->isWWpt) {
            if (bpt->isRWpt)
                bp->bp_type = DBGBP_AWATCH_TYPE;
            else
                bp->bp_type = DBGBP_WATCH_TYPE;
        } else if (bpt->isRWpt) {
            bp->bp_type = DBGBP_RWATCH_TYPE;
        } else {
            bp->bp_type = DBGBP_WATCH_TYPE;
        }
    } else {
        bp->bp_type = DBGBP_BREAK_TYPE;
    }

    bp->bp_enabled = bpt->enabled;

    // set the breakpoint disposition
    if (bpt->disp != NULL) {
        if (strncmp(bpt->disp, "keep", 4) == 0)
            bp->bp_disp = DBGBP_KEEP_DISP;
        else if (strncmp(bpt->disp, "del", 3) == 0)
            bp->bp_disp = DBGBP_DELETE_DISP;
        else if (strncmp(bpt->disp, "dis", 3) == 0)
            bp->bp_disp = DBGBP_DISABLE_DISP;
        else
            bp->bp_disp = DBGBP_UNKNOWN_DISP;
    } else {
        bp->bp_disp = DBGBP_UNKNOWN_DISP;
    }

    // If this breakpoint has multiple locations, fake the information based
    // on the original location.
    if (bpt->hasMultiple) {
        DVPRINT("bpt has multiple locations.\n");
        if (bpt->origLoc != NULL) {
            DVPRINT("bpt original location is present.\n");
            char *loc_str = strdup(bpt->origLoc);
            char *loc_ptr;
            // try to split based on "file:line"
            if ((loc_ptr = strstr(loc_str, ":")) != NULL) {
                DVPRINT("Faking line location\n");
                // set needle to null
                *loc_ptr++ = '\0';
                bp->bp_loc.loc_file = strdup(loc_str);
                bp->bp_loc.loc_line = atoi(loc_ptr);
                Free(loc_str);
            } else {
                DVPRINT("Faking func location\n");
                // The entire string is a function.
                bp->bp_loc.loc_func = loc_str;
            }
        }
    } else {
        bp->bp_loc.loc_line = bpt->line;
        if (bpt->file != NULL) bp->bp_loc.loc_file = strdup(bpt->file);
        if (bpt->func != NULL) bp->bp_loc.loc_func = strdup(bpt->func);
        if (bpt->address != NULL) bp->bp_loc.loc_addr = strdup(bpt->address);
        if (bpt->what != NULL) bp->bp_exp = strdup(bpt->what);
    }

    // Check to see if this is a pending breakpoint
    if (bpt->pending != NULL) {
        bp->bp_pending = 1;
        bp->bp_stmt = strdup(bpt->pending);
    }

    if (bpt->threadId != NULL) {
        bp->bp_threadId = strdup(bpt->threadId);
    }

    if (commInfo) {
        bp->bp_comminfo = commInfo->clone();
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

    MIListFree(bpts, (void (*)())MIBreakpointFree);

    return e;
}

/*
** Signal handler.
*/
static void Finish(int sig) {
    DVPRINT("*** Dbg %s, %s\n", __FILE__, __func__);
    Shutdown++;
}

/*
** Send the MI command
*/
void SendMICommand(MISession *sess, MICommand *cmd) {
    DVPRINT("*** Dbg %s, %s\n", __FILE__, __func__);
    char *buf;

    MISessionSendCommand(sess, cmd);
    do {
        if (MISessionProgress(sess) == -1) {
            if ((buf = MICommandResultErrorMessage(cmd)) != NULL) {
                DVPRINT("SendMICommand: Debugger error: %s\n", buf);
                free(buf);
            } else {
                DVPRINT("SendMICommand: Error from GDB, but no message\n");
            }

            break;
        }

        if (sess->out_fd == -1) {
            DVPRINT("SendMICommand: sess->out_fd = -1\n");
            break;
        }

    } while (!MICommandCompleted(cmd));
}

/* GDBMI callback functions */
static void event_callback(MIEvent *e) {
    DVPRINT("*** Dbg %s, %s, e->type %d\n", __FILE__, __func__, e->type);
    AsynchEventWaiting = 1;
    PendingEvent = e;
}

static void console_callback(char *str) {
    DVPRINT("*** Dbg %s, %s, str %s\n", __FILE__, __func__, str);
}

static void log_callback(char *str) {
    DVPRINT("*** Dbg %s, %s, str %s\n", __FILE__, __func__, str);
}

static void target_callback(char *str) {
    DVPRINT("*** Dbg %s, %s, str %s\n", __FILE__, __func__, str);
}

static void notification_callback(char *className, MIList *results) {
    DVPRINT("NEWGDBMI notification> %s\n", className);
}

/*
**
** ----- hashing and slicing functions -----
**
*/

// rather than coded in DbgGDBMIEvalExpr()
static dbgevent_t *PerformExtEvalExpr(const char *exp) {
    DVPRINT("*** Dbg %s, %s\n", __FILE__, __func__);
    dbgevent_t *e;
    AIF *a;

    AIF *tmp = NULL;

    char var[BUFSIZ] = "";
    char args[BUFSIZ] = "";
    int func_id = 0;
    char sliceinfo[BUFSIZ] = "";

    DVPRINT("*** PerformExtEvalExpr(%s-%d)\n", exp, my_rank);

    ParseDBGExpr(exp, &func_id, sliceinfo, var, args);

    DVPRINT("*** PerformExtEvalExpr(%s-%s-%s)\n", var, args, sliceinfo);

    // We perform hashing separately
    if (func_id == EXPR_HASH_BOB) {
        int hashfunc = 0, precision_lower, precision_upper;
        char hashinfo[BUFSIZ] = "";
        ParseHashArgs(args, &hashfunc, &precision_lower, &precision_upper,
                      hashinfo);

        if ((e = GetAIFVarHash(GDBMISession, var, &a, hashinfo, func_id,
                               precision_lower, precision_upper)) != NULL) {
            return e;
        }

        e = NewEvent(DBGEV_DATA);
        e->ev_data = new AIFStream(a);

        if (debug_flag) {
            char *aif_str = NULL;
            AIFToStr(&aif_str, 0, a);
            DVPRINT("Final AIF size is: %d\n", AIF_LEN(a));
            DVPRINT("Final AIF type is: %s\n", aif_str);
            Free(aif_str);
        }

        return e;
    }

    // GetAIFVar only returns a value if it fails
    AIFStream* aifStream = nullptr;
    if ((e = GetAIFVar(GDBMISession, var, &aifStream, nullptr))) {
        return e;
    }
    
    e = NewEvent(DBGEV_DATA);

    // if slice info found, perform slicing first
    if (strlen(sliceinfo) > 0) {
        int is_revert = 0;

        if (AIFType(a) == AIF_FTN_ARRAY) is_revert = 1;

        e->ev_data = new AIFStream(PerformSlicing(a, sliceinfo, is_revert));
        AIFFree(a);

        if (e->ev_data == NULL) {
            // an error occured.
            FreeEvent(e);
            e = NULL;
            EVENT_ERROR(e, DBGERR_DEBUGGER, "Slicing failed: %s",
                        PerformSlicingErr);

            return e;
        }
    } else {
        e->ev_data = new AIFStream(a);
    }

    // Minh changes here to do p2p mode
    // we should deal with p2p separately
    if (func_id == EXPR_P2P) {
        int is_sender = 0;
        // do a lot of things here
        tmp = PerformP2P(e->ev_data->joinAIF(), args, my_rank, &is_sender);
        if (!is_sender)  // if not sender, send the data back to client
        {
            delete e->ev_data;
            e->ev_data = new AIFStream(tmp);
        } else  // otherwise, just make a empty event
        {
            FreeEvent(e);
            e = NewEvent(DBGEV_OK);
        }
    }
    // Minh change here to do the hashing of data 03/06/2009
    else if (func_id > 0) {
        double lower_bound, upper_bound;

        switch (func_id) {
            case EXPR_SUM:  // sum
                tmp = AIFSum(e->ev_data->joinAIF());
                break;
            case EXPR_PRODUCT:  // product
                tmp = AIFProduct(e->ev_data->joinAIF());
                break;
            case EXPR_AVERAGE:  // average
                tmp = AIFAverage(e->ev_data->joinAIF());
                break;
            case EXPR_MIN:  // min
                tmp = AIFMin(e->ev_data->joinAIF());
                break;
            case EXPR_MAX:  // max
                tmp = AIFMax(e->ev_data->joinAIF());
                break;
            case EXPR_STDEV:  // standard deviation
                tmp = AIFStdev(e->ev_data->joinAIF());
                break;
            case EXPR_COUNTELEM:  // countelem
                ParseCountelemArgs(args, &lower_bound, &upper_bound);
                tmp = AIFCountElem(e->ev_data->joinAIF(), lower_bound, upper_bound);
                break;
            default:
                DVPRINT("*** Unknow reduce operation\n");
                break;
        }

        delete e->ev_data;
        e->ev_data = new AIFStream(tmp);

        DVPRINT("Removed AIFToStr here\n");
    }

    return e;
}

dbgevent_t *DbgGdbMI::evalExpr(const char *exp, long expIdx, const char* threads) {
    DVPRINT("*** Dbg %s, %s(exp %s, rank %d, idx %ld)\n", __FILE__, __func__, exp, my_rank, expIdx);

    // Check whether it has hashing/slicing
    if (strpbrk(exp, "`") || strpbrk(exp, "|")) {
        // TODO: we don't want to parse this if the user escaped the input
        DVPRINT("DbgEvalExpr: Found reduction/slicing info.\n");
        dbgevent_t* e = PerformExtEvalExpr(exp);
        if (e == NULL) {
            e = NewEvent(DBGEV_ERROR);
            e->ev_errno = DBGERR_DEBUGGER;
            EventErrStr(e, "bad data format");
            return e;
        } else if (e->ev_data == nullptr) {
            e = NewEvent(DBGEV_ERROR);
            e->ev_errno = DBGERR_DEBUGGER;
            EventErrStr(e, "NULL data found");
            return e;
        } else {
            DVPRINT("*** out DbgEvalExpr(%s-%d)\n", exp, my_rank);
            return e;
        }
    }

    // track the evaluations that have only returned a portion of result
    static std::map<long,std::unique_ptr<AIFStream>> remainders;

    auto remainderLoc = remainders.find(expIdx);
    if (remainderLoc != remainders.end()) {
        // Return the next slice of data
        bool isFinal;
        dbgev ev = DBGEV_DATA_SLICE;
        auto a = remainderLoc->second->getNextSlice(AIFStream::STEAL, isFinal);
        if (isFinal) {
            DVPRINT("returning the last slice of %l\n", expIdx);
            ev = DBGEV_DATA;
            remainders.erase(remainderLoc);
        } else {
            DVPRINT("Returning the next slice of %l\n", expIdx);
        }

        auto e = NewEvent(ev);
        e->ev_data = new AIFStream(a);
        return e;
    }

    if (threads && threads[0]) {
        // evalExprWithThreads returns null if the result is single threaded, so that case
        // falls through into the normal processing
        if (auto e = evalExprWithThreads(exp, threads)) return e;
    }

    AIFStream* aifStream = nullptr;

    if (dbgevent_t* e = GetAIFVar(GDBMISession, exp, &aifStream, nullptr)) {
        return e;
    }


    dbgev ev = DBGEV_DATA;
    bool isFinal;
    auto  aif = aifStream->getNextSlice(AIFStream::STEAL, isFinal);

    if (isFinal) {
        delete aifStream;
    } else {
        // We are only returning the first slice of the result.   Save the
        // rest for the continuation call.
        DVPRINT("saving remainder of expression %l\n", expIdx);
        ev = DBGEV_DATA_SLICE;
        remainders.insert(std::make_pair(expIdx,aifStream));
    }

    dbgevent_t* e = NewEvent(ev);
    e->ev_data = new AIFStream(aif);
    return e;
}

// handling for expressions that have a defined thread set context.   If the thread set
// resolves to a single thread  we just change the thread context and return 
dbgevent_t* DbgGdbMI::evalExprWithThreads(const char* exp, const char* threads)
{
    std::string error;
    auto threadSet = getThreadSet(threads, nullptr, error);
    if (!error.empty()) {
        return NewErrorEvent(DBGERR_DEBUGGER, error.c_str());
    }

    auto threadStr = threadIdsToString(threadSet);
    if (threadSet.size() == 1) {
        // single thread - just change to the thread and act normally - that is return
        // an AIF structure just like always
        auto e = setThread(threadStr.c_str());
        if (e->ev_event != DBGEV_OK) {
            return e;
        } else {
            FreeEvent(e);
        }
    } else if (threadSet.size() > 1) {
        // multiple threads - we'll switch over to apply, which means this form will
        // just return a string, but in this case there are possible multiple values
        std::string op = threadStr + " p " + exp;
        auto applyEvent = threadApply(op.c_str());
        if (applyEvent->ev_event != DBGEV_THREADINFO) {
            return applyEvent;
        }
        
        // the front end expects data, and most of it will work reasonably well if we
        // return this as string data.   Adding a special header reduces the odds of
        // conflicting with an actual string.
        char* retStr = nullptr;
        asprintf(&retStr, "%s%s", MULTI_THREAD_HDR, applyEvent->ev_u.ev_u_string);
        AIF* aif = StringToAIF(retStr);
        FreeEvent(applyEvent);
        free(retStr);

        dbgevent_t* e = NewEvent(DBGEV_DATA);
        e->ev_data = new AIFStream(aif);
        return e;
    }

    return nullptr; // let the normal eval process either continue or take over
}

dbgevent_t *DbgGdbMI::openSocket(const char *opset_name, int opset_size) {
    DVPRINT("*** Dbg %s, %s\n", __FILE__, __func__);
    int i;

    if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket");
    }

    // load the network info for the other procset
    other_app.id = strdup(opset_name);
    other_app.nrank = opset_size;
    other_app.rank =
        (rankInfoEntry_t **)Alloc(sizeof(rankInfoEntry_t *) * opset_size);
    for (i = 0; i < opset_size; i++)
        other_app.rank[i] = (rankInfoEntry_t *)Alloc(sizeof(rankInfoEntry_t));
    LoadAppInfo(&other_app);

    return NewEvent(DBGEV_OK);
}

dbgevent_t *DbgGdbMI::closeSocket() {
    DVPRINT("*** Dbg %s, %s\n", __FILE__, __func__);
    close(sock_fd);

    // unload other_app
    UnloadAppInfo(&other_app);

    return NewEvent(DBGEV_OK);
}

dbgevent_t *DbgGdbMI::getLocation() {
    DVPRINT("*** Dbg %s, %s\n", __FILE__, __func__);
    // unimplemented, return an error event
    dbgevent_t* e = nullptr;
    EVENT_ERROR(e, DBGERR_DEBUGGER, "debugger function not implemented");
    return e;
}
int DbgGdbMI::progioHook(int) {
    // unimplemented, expecting a status, this has always return -1
    return -1;
}

static std::string threadIdsToString(const std::vector<int>& ids) {
    bool first = true;
    std::ostringstream os;
    for (auto&& id : ids) {
        os << (first ? "" : " ") << id;
        first = false;
    }
    return os.str();
}

// translate a frame result from threadApply into a frame event.    The frame event
// will contain the set of frames for each equivalent thread set
// -- This is based on the strategy of using threadApply to do the aggragation, though
// it may leave us with potentially brittle text at this level.   If in the future gdb/mi
// gives a more efficient means to get frames from multiple threads, the approach could
// be changed to take advantage of it. ---
static dbgevent_t* threadEventToFrame(dbgevent_t* threadEvent) {
    if (threadEvent->ev_event != DBGEV_THREADINFO) {
        return threadEvent;
    }

    static std::regex atRE(R"(#(\d+)\s+(in\s+)?(.*)\s+at (.*):(\d+))");
    static std::regex fromRE(R"(#(\d+)\s+(in\s+)?(.*)\s+from\s+(.*))");

    auto res = std::string{threadEvent->ev_u.ev_u_string};

    dbgframe_t* frame0 = nullptr, *frame0tail = nullptr;

    static auto threadHeadRE=std::regex{R"((^|\n)Thread\s+([0-9 ,-]+):\s*\n)"};
    for (ThreadMsgParser parser(std::move(res)); auto res = parser.nextResult(); ) {
        auto [ thread, val ] = *res;

        // now we will need to scrape the frame information from the response.
        // I will look something like:
        // #0   in nanosleep () from /lib64/libpthread.so.0
        // #1   in std::this_thread::sleep_for<long, std::ratio<1l, 1000l> > (__rtime=...) at /opt/gcc/8.1.0/snos/lib/gcc/x86_64-suse-linux/8.1.0/../../../../include/g++/thread:369
        // #2   in main::$_0::operator() (this=) at bd_dt/C++/cpp_threads.cpp:54
        // so I think every frame is written on one line
        auto is = std::istringstream{std::move(val)};
        auto line = std::string{};

        dbgframe_t *tframe = nullptr, *ttail = nullptr;
        auto threadStr = thread.c_str();

        while (std::getline(is, line)) {
            auto match = std::smatch{};
            dbgframe_t* frame = nullptr;

            if (std::regex_match(line, match, atRE)) {
                auto level = std::stoi(match[1]);
                auto func = match[3].str();
                auto file = match[4].str();
                auto frameLine = std::stoi(match[5]);

                DVPRINT("frame %d IN %s AT FILE %s LINE %d\n", level, func.c_str(), file.c_str(),
                        frameLine);

                auto loc = dbgloc_t{ const_cast<char*>(file.c_str()),
                                     const_cast<char*>(func.c_str()), nullptr, frameLine };
                frame = new dbgframe_t{level, loc, nullptr, threadStr, nullptr};
            } else if (std::regex_match(line, match, fromRE)) {
                auto level = std::stoi(match[1]);
                auto func = match[3].str();
                auto file = match[4].str();
                DVPRINT("frame %d IN %s FROM %s\n", level, func.c_str(), file.c_str());

                auto loc = dbgloc_t{ const_cast<char*>(file.c_str()),
                                     const_cast<char*>(func.c_str()), nullptr, 0 };

                frame = new dbgframe_t{ level, loc, nullptr, threadStr, nullptr};
            } else {
                DVPRINT("could not parse line %s\n", line.c_str());
                continue;
            }

            // we only report threads on the head of each stack
            threadStr = nullptr;

            // add frame to the linked list
            if (!tframe) {
                // begining of thread frame
                tframe = ttail = frame;
                if (!frame0) {
                    // first frame at all
                    frame0 = frame0tail = frame;
                } else {
                    frame0tail->setNextThreadFrame(frame);
                    frame0tail = frame;
                }
            } else {
                ttail->setNext(frame);
                ttail = frame;
            }
        }
    }

    dbgevent_t* frameEvent = nullptr;
    if (frame0) {
        frameEvent = NewEvent(DBGEV_FRAME);
        EVENT_FRAME(frameEvent) = frame0;
    } else {
        frameEvent = NewErrorEvent(DBGERR_DEBUGGER, "could not parse frame result");
    }

    FreeEvent(threadEvent);

    return frameEvent;
}
    
