/*********************************************************************************\
 * GDB interface routines
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * Copyright 2010-2023 Hewlett Packard Enterprise Development LP.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Hewlett Packard Enterprise Development LP.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *
 *********************************************************************************/

/*
** The Guard GDB/MI interface.
** Now uses the Eclipse SDM libmi interface.
*/
#include <string>

#include "config.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <dlfcn.h>
#include <string>
#include <fstream>
#include <future>
#include <functional>
#include <map>
#include <vector>
#include <regex>
#include <iomanip>
#include <filesystem>

#include "common_tools_be.h"

#include "dbgcomm_info.h"
#include "dbgevent.h"
#include "dbgsrv_gdbmi.h"
#include "dbgsrv_defs.h"
#include "dbgsrv_libmi2aif.h"
#include "dbgsrv_p2p.h"
#include "dbgsrv_proto.h"
#include "dbgsrv_util.h"
#include "defs.h"
#include "useful.h"
#include "mi_utils.h"

#include "pybe_comms.h"

static dbgevent_t* checkForPythonError(MICommand& cmd, const std::string& result);
static std::optional<dbgloc> parsePythonLocation(const std::string& line);
static dbgframe_t* parsePythonBacktrace(const std::string& text);
static dbgframe_t* parsePythonMoveFrame(const std::string& text);

static MICommand *CLIBypassc(int cmd_argc, const char *cmd_argv[]) {
    return CLIBypass(cmd_argc, const_cast<char **>(cmd_argv));
}

static MISession *GDBMISession;
static dbgstat Status;
static dbgbp_t *CurrBP = (dbgbp_t *)NULL;
static std::vector<dbgbp_t*> BP;

static int AsynchEventWaiting = 0;
static MIEvent *PendingEvent = NULL;

// true if the debugger is actually in non-stop mode, the variable non_stop_mode
// just indicates it was *requested*
static bool isInNonStopMode = false;

// set by DbgSetPrintElements
int clientPrintElements = 0;

// gdbmode keeps independent "print elements" and "print repeats" setting
static int directPrintElements = 200;
static std::string directPrintRepeats = "10";

#ifndef ALPS
// These are for sending the target output to the fe
static int WRITE_fd;
static int dbgsrv_fd;
#endif

extern appInfo_t other_app;

static dbgevent_t *WaitEvent(dbgevent_t *e);
static dbgevent_t *WaitOmpdEvent(dbgevent_t *e);
static dbgevent_t *CheckBreak(int feId, MICommand *, const dbgcomm_info *, int& beId);

static dbgevent_t *threadEventToFrame(dbgevent_t* threadEvent);
static dbgframe_t *MakeFrameFromMIFrame(MIFrame* miFrame, bool keepAddr=false);

static std::string threadIdsToString(const std::vector<int>& ids);
static void ThreadAsyncCheck(dbgevent_t *(*)(dbgevent_t *) , dbgevent_t *);

enum free_cmd_type { NO_FREE_CMD, FREE_CMD };
static dbgevent_t *NewMIErrorEvent(MICommand* cmd, free_cmd_type = NO_FREE_CMD, dbgerr=DBGERR_DEBUGGER);

static const char* unknownBPErrorMsg = "Hit unknown breakpoint!";

/* debugger definition */
static DbgGdbMI DbgBackendGDBMI;

// number of threads still running that were set off by background commands in thread apply
static size_t numBackgroundThreads = 0;

DbgGdbMI* DbgGdbMIGetSingleton() { return &DbgBackendGDBMI; }

DbgGdbMI::DbgGdbMI() : m_initializer(new Initializer(*this)),
                       m_ompdIsInitialized(false), m_ompdIsAvail(false) {
    if (auto * be = cti_be_getFileDir()) {
        DVPRINT("got the file directory %s\n", be);
        m_beFileDir = be;
        free(be);
    }
}

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


static MICommand *_MIGDBSet(const char *var, const char *val) {
    return MIGDBSet(const_cast<char *>(var), const_cast<char *>(val));
}
/*
**
** ----- DbgGdbMI functions -----
**
*/

dbgevent_t *DbgGdbMI::init(int *dfd, int *iofd, char **env) {
    DVPRINT("*** Dbg %s::%s\n", __FILE__, __func__);

    Status = DBGSTAT_INITIALISING;

    auto event = m_initializer->init(dfd, iofd, env);
    GDBMISession = m_initializer->getMISession();

    return event;
}

/*
** Continue to main and show current stack frame.
*/
dbgevent_t *DbgGdbMI::startup(void) {
    DVPRINT("*** Dbg %s, %s\n", __FILE__, __func__);
    return m_initializer->startup();
}

/*
** LibMI consumer call. This will have the libmi progress its session
** and perform any necessary callbacks.
*/
int DbgGdbMI::read(int) {
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
    // DVPRINT("*** Dbg %s, %s\n", __FILE__, __func__);
    return AsynchEventWaiting; /* Modified by event_callback(). */
}

/*
** Set breakpoint at specified line.
*/
dbgevent_t *DbgGdbMI::setLineBreak(int feId,
                                   const int allowPending, const char *file, int lno,
                                   const char* threadSet, const dbgcomm_info *commInfo) {
    DVPRINT("*** Dbg %s, %s\n", __FILE__, __func__);
    auto where = std::to_string(lno);
    if (file && file[0]) {
        where = std::string{file} + ":" + where;
    } else if (auto frameEvt = getFrame(false, threadSet)) {
        if (frameEvt->ev_event == DBGEV_FRAME) {
            if (auto frameFile = frameEvt->ev_frame->getLoc().loc_file) {
                where = std::string{frameFile} + ":" + where;
            }
        }
        FreeEvent(frameEvt);
    }
        

    return setBreak(feId, allowPending, where, threadSet, commInfo);
}

/*
** Set breakpoint at start of specified function.
*/
dbgevent_t *DbgGdbMI::setFuncBreak(int feId,
                                   const int allowPending, const char *file, const char *func,
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

    return setBreak(feId, allowPending, where, threadSet, commInfo);
}

// Common work for setting a function or line breakpoing.   Argument \a where
// contains the proper break expression in either case.
dbgevent_t *DbgGdbMI::setBreak(int feId,
                               const int allowPending, const std::string& where,
                               const char*  threadSet,
                               const dbgcomm_info *commInfo) {
    std::vector<int> threads;
    if (threadSet && strlen(threadSet)) {
        std::string error;
        threads = getThreadSet(threadSet, nullptr, error);
        if (!error.empty()) {
            return NewErrorEvent(DBGERR_DEBUGGER, "%s", error.c_str());
        }
    }

    if (char* objdump = pathFind("cuobjdump", nullptr)) {
        DVPRINT("found cuobjdump at %s\n", objdump);
        free(objdump);
    } else {
        DVPRINT("did not find cuobjdump, PATH is %s\n", getenv("PATH"));
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
            tbEvent.reset(NewMIErrorEvent(cmd));

            // Try the basename instead in case the file was compiled locally.
            auto where2 = std::filesystem::path(where).filename();
            if (where2 != where) {
                MICommandFree(cmd);
                cmd = MIBreakInsert(0, 0, allowPending, nullptr, 0, where2.c_str(), threadId);
                SendMICommand(GDBMISession, cmd);
                if (MICommandResultOK(cmd)) tbEvent.reset();
            }
        }
        int beId = 0;
        if (!tbEvent) {
            tbEvent.reset(CheckBreak(feId, cmd, commInfo, beId));
        }

        MICommandFree(cmd);

        if (!tbEvent) {
            event.reset(NewErrorEvent(DBGERR_DEBUGGER, "Cannot create breakpoint."));
            break;
        } else {
            bpIds.push_back(beId);
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
        DVPRINT("Insertion failed removing %zu breakpoints\n", bpIds.size());
        for (auto it=bpIds.rbegin(); it!=bpIds.rend(); ++it) {
            // may not matter, but undo in LIFO order for neatness sake
            FreeEvent(deleteBreak(*it));
        }

        return event.release();
    } 

    // add the thread info if this is a thread specific breakpoint
    if (threadSet && !threads.empty() && threads[0] != -1) {
        auto bp = EVENT_BP(event);
        bp->bp_threadId = threadSet;
    } 

    return event.release();
}

/*
** Set watchpoint at specified expression.
*/
dbgevent_t *DbgGdbMI::setWatch(int feId, int type, const char *expr, const dbgcomm_info *commInfo) {
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

    if (!MICommandResultOK(cmd)) return NewMIErrorEvent(cmd, FREE_CMD);

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

    if (!MICommandResultOK(cmd)) return NewMIErrorEvent(cmd, FREE_CMD);

    // Now check the breakpoint. It has all the info we need.

    int beId;
    event = CheckBreak(feId, cmd, commInfo, beId);

    if (event == NULL) {
        EVENT_ERROR(event, DBGERR_DEBUGGER,
                    "Cannot create internal breakpoint.");
    }

    MICommandFree(cmd);

    return event;
}

dbgevent_t *DbgGdbMI::setCatch(int feId, const std::string &event,
                               const std::string &regex, const dbgcomm_info *commInfo) {
    DVPRINT("*** Dbg %s, %s event=%s regex=\"%s\" \n", __FILE__, __func__, event.c_str(), regex.c_str());
    
    using CommandPtr = std::unique_ptr<MICommand, decltype(&MICommandFree)>;

    int id; // breakpoint id

    // Send catch command and determine catchpoint id
    {
        // MI doesn't directly support most catch commands, but they work fine with CLIBypass
        CommandPtr CLICatchCommand(nullptr, MICommandFree);

        if ((event == "load" || event == "unload") && regex != "") {
            // load/unload support regexes to specify libraries to monitor
            const char* argv[] = {"catch", event.c_str(), regex.c_str()};
            CLICatchCommand = CommandPtr(CLIBypassc(3, argv), MICommandFree);
        } else {
            const char* argv[] = {"catch", event.c_str()};
            CLICatchCommand = CommandPtr(CLIBypassc(2, argv), MICommandFree);
        }

        // Since we're using Bypass, MI only reports a synchronous ^done after
        // the command is over, no breakpoint info is included. But before that, 
        // a "breakpoint created" async notification is generated with the info 
        // we need. We set a callback here to catch that notification. 

        // Restore old callback when we're done
        const auto oldCallback = GDBMISession->notify_callback;
        const auto oldCallbackCtx = GDBMISession->notify_callback_ctx;
        auto restoreCallbackOnExit = OnScopeExit([&](){
            MISessionRegisterNotifyCallback(GDBMISession, oldCallback, oldCallbackCtx);
        });

        MIList* breakPoints = nullptr;

        // Set custom callback
        MISessionRegisterNotifyCallback(GDBMISession, [](void* list, char *className, MIList *results) {
            *static_cast<MIList**>(list) = MIBreakpointGetBreakNotifyInfo(className, results);
        }, &breakPoints);

        // Send command
        SendMICommand(GDBMISession, CLICatchCommand.get()); // our callback happens here
        if (!MICommandResultOK(CLICatchCommand.get())) {
            return NewMIErrorEvent(CLICatchCommand.get());
        }

        if (breakPoints == nullptr) {
            dbgevent_t *errorEvent;
            EVENT_ERROR(errorEvent, DBGERR_DEBUGGER,
                "Cannot create catchpoint. (Couldn't fetch newly inserted catchpoint info)");
            return errorEvent;
        }

        if (MIListIsEmpty(breakPoints)) {
            dbgevent_t *errorEvent;
            EVENT_ERROR(errorEvent, DBGERR_DEBUGGER,
                "Cannot create catchpoint. (Empty breakpoint list)");
            return errorEvent;
        }

        MIListSet(breakPoints);
        id = ((MIBreakpoint *)MIListGet(breakPoints))->number;
        MIListFree(breakPoints, (void (*)())MIBreakpointFree);
    }

    // Now send the MIBreakInfo
    auto breakInfoCommand = CommandPtr(MIBreakInfo(id), MICommandFree);

    SendMICommand(GDBMISession, breakInfoCommand.get());
    if (!MICommandResultOK(breakInfoCommand.get())) {
        return NewMIErrorEvent(breakInfoCommand.get());
    }

    // Now check the breakpoint. It has all the info we need.
    int beId = 0;
    auto dbgEvent = CheckBreak(feId, breakInfoCommand.get(), commInfo, beId);

    if (dbgEvent == nullptr) {
        dbgevent_t *errorEvent;
        EVENT_ERROR(errorEvent, DBGERR_DEBUGGER,
            "Cannot create internal catchpoint. (Failed to record new breakpoint)");
        return errorEvent;
    }

    return dbgEvent;
}

dbgevent_t *DbgGdbMI::setCheckpoint() {
    DVPRINT("*** Dbg %s:%s\n", __FILE__, __func__);
    using CommandPtr = std::unique_ptr<MICommand, decltype(&MICommandFree)>;

    // send checkpoint command
    const char* argv[] = {"checkpoint"};
    CommandPtr cliCheckpointCmd(CLIBypassc(1, argv), MICommandFree);

    SendMICommand(GDBMISession, cliCheckpointCmd.get());
    if (!MICommandResultOK(cliCheckpointCmd.get())) {
        return NewMIErrorEvent(cliCheckpointCmd.get());
    }

    // figure out new ID; returned message will be in the form
    // "checkpoint N: fork returned pid XXXXX."
    unsigned long checkpointId = 0;
    {
        using CharPtr = std::unique_ptr<char, decltype(&std::free)>;
        // fetch result message
        CharPtr resultStr(MICommandConsoleOOBMessages(cliCheckpointCmd.get()), &std::free);
        if (resultStr == nullptr) {
            return NewErrorEvent(DBGERR_DEBUGGER, "Failed to get checkpoint ID (null result)");
        }
        DVPRINT("Got checkpoint result \"%s\"\n", resultStr.get());

        // parse id
        const char *s = resultStr.get() + strnlen("checkpoint ", strlen(resultStr.get()));
        errno = 0;
        checkpointId = strtoul(s, nullptr, 10);
        if (errno != 0) {
            dbgevent_t *errorEvent;
            EVENT_ERROR(errorEvent, DBGERR_DEBUGGER, "Failed to parse checkpoint ID (errno = %d)", errno);
            return errorEvent;
        }
        DVPRINT("Parsed checkpoint id: %lu\n", checkpointId);
    }
    
    // figure out where we are so we can tell the frontend
    auto frameEvent = getFrame(false, nullptr);
    if (!frameEvent || !frameEvent->ev_frame) {
        dbgevent_t *errorEvent;
        EVENT_ERROR(errorEvent, DBGERR_DEBUGGER, "Failed to get current frame (null event)");
        return errorEvent;
    } else if (frameEvent->ev_event == DBGEV_ERROR) {
        // pass error up
        return frameEvent;
    }

    // package up frame and id
    dbgevent_t* checkpointEvent = NewEvent(DBGEV_CHECKPOINT);
    checkpointEvent->ev_checkpoint = new dbgcheckpoint_t {
        .id = checkpointId,
        .location{frameEvent->ev_frame->getLoc()}
    };
    
    FreeEvent(frameEvent);

    return checkpointEvent;
}

dbgevent_t *DbgGdbMI::restoreCheckpoint(unsigned int id) {
    DVPRINT("*** Dbg %s:%s\n", __FILE__, __func__);
    using CommandPtr = std::unique_ptr<MICommand, decltype(&MICommandFree)>;

    auto idStr = std::to_string(id);
    const char* argv[] = {"restart", idStr.c_str()};
    CommandPtr cliCheckpointCmd(CLIBypassc(2, argv), MICommandFree);

    SendMICommand(GDBMISession, cliCheckpointCmd.get());
    if (!MICommandResultOK(cliCheckpointCmd.get())) {
        return NewMIErrorEvent(cliCheckpointCmd.get());
    }

    Status = DBGSTAT_STOPPED;

    return NewEvent(DBGEV_OK);
}

dbgevent_t *DbgGdbMI::deleteCheckpoint(unsigned int id) {
    DVPRINT("*** Dbg %s:%s\n", __FILE__, __func__);
    using CommandPtr = std::unique_ptr<MICommand, decltype(&MICommandFree)>;

    auto idStr = std::to_string(id);
    const char* argv[] = {"delete", "checkpoint", idStr.c_str()};
    CommandPtr cliCheckpointCmd(CLIBypassc(3, argv), MICommandFree);

    SendMICommand(GDBMISession, cliCheckpointCmd.get());
    if (!MICommandResultOK(cliCheckpointCmd.get())) {
        return NewMIErrorEvent(cliCheckpointCmd.get());
    }

    return NewEvent(DBGEV_OK);
}

/*
** Delete a breakpoint.
*/
dbgevent_t *DbgGdbMI::deleteBreak(int feBid) {
    DVPRINT("*** Dbg %s, %s\n", __FILE__, __func__);

    auto found = false;
    while (auto bp = FindBPByID(BP, feBid, 0)) {
        found = true;
        auto bpid = bp->bp_be_id;

        auto cmd = MIBreakDelete(1, &bpid);
        SendMICommand(GDBMISession, cmd);

        if (!MICommandResultOK(cmd)) {
            MICommandFree(cmd);
            return NewErrorEvent(DBGERR_DEBUGGER, "Delete breakpoint failed.");
        }

        MICommandFree(cmd);
        RemoveBP(BP, 0, bpid);
    }

    if (!found) {
        return NewErrorEvent(DBGERR_NOBP, "Invalid breakpoint id.");
    }

    return NewEvent(DBGEV_OK);
}

/*
** Disable a breakpoint.
*/
dbgevent_t *DbgGdbMI::disableBreak(int feId) {
    DVPRINT("*** Dbg %s, %s\n", __FILE__, __func__);

    if (auto e = infoBreak()) {
        FreeEvent(e);
    }

    auto bp = FindBPByID(BP, feId, 0);
    if (!bp) {
        return NewErrorEvent(DBGERR_NOBP, "Invalid breakpoint id.");
    }

    auto bpid = bp->bp_be_id;

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

    auto bp = FindBPByID(BP, bid, 0);
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
        snprintf(cmd_bpid, 30, "%d", bp->bp_be_id);
        cmd_args[2] = cmd_bpid;
        cmd = CLIBypassc(3, cmd_args);
    } else if (del) {
        cmd_args[0] = "enable";
        cmd_args[1] = "delete";
        snprintf(cmd_bpid, 30, "%d", bp->bp_be_id);
        cmd_args[2] = cmd_bpid;
        cmd = CLIBypassc(3, cmd_args);
    } else {
        // this is needed...
        auto bpid = bp->bp_be_id;
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

// ignore the next n occurrences of a breakpoint 
dbgevent_t *DbgGdbMI::ignoreBreak(int feId, int n) {
    DVPRINT("*** Dbg %s, %s\n", __FILE__, __func__);

    auto bp = FindBPByID(BP, feId, 0);
    if (!bp) {
        return NewErrorEvent(DBGERR_NOBP, "Invalid breakpoint id.");
    }

    auto bid = bp->bp_be_id;
    auto cmd = MIBreakAfter(1, &bid, n);
    SendMICommand(GDBMISession, cmd);
    if (!MICommandResultOK(cmd)) {
        MICommandFree(cmd);
        return NewErrorEvent(DBGERR_DEBUGGER, "Ignore breakpoint failed.");
    }

    MICommandFree(cmd);
    return NewEvent(DBGEV_OK);
}

// ignore the next n occurrences of a breakpoint 
dbgevent_t *DbgGdbMI::conditionBreak(int feId, const std::string& cond) {
    DVPRINT("*** Dbg %s, %s %d %s\n", __FILE__, __func__, feId, cond.c_str());

    auto bp = FindBPByID(BP, feId, 0);
    if (!bp) {
        return NewErrorEvent(DBGERR_NOBP, "Invalid breakpoint id.");
    }

    auto bid = bp->bp_be_id;
    auto cmd = MIBreakCondition( 1, &bid, const_cast<char*>(cond.c_str()));
    SendMICommand(GDBMISession, cmd);

    if (!MICommandResultOK(cmd)) {
        MICommandFree(cmd);
        return NewErrorEvent(DBGERR_DEBUGGER, "Condition breakpoint failed.");
    }

    MICommandFree(cmd);
    return NewEvent(DBGEV_OK);
}


/*
** Return breakpoint information.
*/
dbgevent_t *DbgGdbMI::infoBreak() {
    DVPRINT("*** Dbg %s, %s\n", __FILE__, __func__);

    auto res = std::ostringstream{};

    for (auto bp : BP) {
        auto cmd = MIBreakInfo(bp->bp_be_id);
        SendMICommand(GDBMISession, cmd);
        if (!MICommandResultOK(cmd)) {
            DVPRINT("MIBreakInfo %d failed\n", bp->bp_be_id);
            MICommandFree(cmd);
            continue;
        }

        auto bpts = MIBreakpointGetBreakInsertInfo(cmd);
        if (!bpts) {
            DVPRINT("bpts is null\n");
        }

        MIListSet(bpts);
        while (auto bpt = (MIBreakpoint *)MIListGet(bpts)) {
            if (auto bp = FindBPByID(BP, 0, bpt->number)) {
                if (auto id = bp->bp_fe_id; id > 0) {
                    res << id << " " << bpt->times << " " << bpt->ignore << "\n";
                }
            }
        }

        MIListFree(bpts, (void (*)())MIBreakpointFree);
        MICommandFree(cmd);
    }

    auto e = NewEvent(DBGEV_STRING);
    e->ev_string = strdup(res.str().c_str());

    return e;
}

void DbgGdbMI::setSourceOpen(bool doOpen) {
    m_initializer->setSourceOpen(doOpen);
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

    // note this option does not exist in newer versions of gdb
    setSourceOpen(true);

    cmd = CLIList(str);
    SendMICommand(GDBMISession, cmd);
    if (!MICommandResultOK(cmd)) return NewMIErrorEvent(cmd, FREE_CMD);

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

    // do not open source files
    setSourceOpen(false);

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
    if (!MICommandResultOK(cmd)) return NewMIErrorEvent(cmd, FREE_CMD);

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
dbgevent_t *DbgGdbMI::go(bool all, const char* threads) {
    DVPRINT("*** Dbg %s, %s\n", __FILE__, __func__);

    if (Status == DBGSTAT_EXIT) {
        return NewErrorEvent(DBGERR_AT_EXIT, "");
    }

    if (!all && threads && isInNonStopMode) {
        // in non-stop mode continue just runs the current thread.   So apply it
        // to the active threads
        auto command = std::string{threads} + " c&";  // use background apply for async functions
        auto applyEvent = threadApply(command.c_str());
        if (applyEvent->ev_event == DBGEV_ERROR) {
            return applyEvent;
        }
        FreeEvent(applyEvent);  // nothing interesting with background calls.
    } else {
        // normal continue starts all threads unless scheduler locking is enabled.
        auto cmd = (all ? MIExecContinueAll() : MIExecContinue());

        SendMICommand(GDBMISession, cmd);

        if (!MICommandResultOK(cmd)) return NewMIErrorEvent(cmd, FREE_CMD);

        MICommandFree(cmd);
    }

    // set up catch for the async response.
    ThreadAsyncCheck(WaitEvent, nullptr);

    Status = DBGSTAT_RUNNING;

    return NewEvent(DBGEV_OK);
}

/*
** Continue executing program until the current function returns.
*/
dbgevent_t *DbgGdbMI::finishFunc(const char* threads) {
    DVPRINT("*** Dbg %s, %s\n", __FILE__, __func__);

    if (Status == DBGSTAT_EXIT) {
        return NewErrorEvent(DBGERR_AT_EXIT, "");
    }

    if (threads) {
        auto command = std::string{threads} + " finish&";
        auto applyEvent = threadApply(command.c_str());
        if (applyEvent->ev_event == DBGEV_ERROR) {
            return applyEvent;
        }
        FreeEvent(applyEvent);
    } else {
        MICommand *cmd = MIExecFinish();

        SendMICommand(GDBMISession, cmd);

        if (!MICommandResultOK(cmd)) return NewMIErrorEvent(cmd, FREE_CMD);

        MICommandFree(cmd);
    }

    ThreadAsyncCheck(WaitEvent, nullptr);

    Status = DBGSTAT_RUNNING;

    return NewEvent(DBGEV_OK);
}

/*
** Set run arguments.
*/
dbgevent_t *DbgGdbMI::setArgs(const char *arg) {
    DVPRINT("*** Dbg %s, %s\n", __FILE__, __func__);
    return m_initializer->setArgs(arg);
}

/*
** Execute count statements. If in == false, do not enter
** function calls.
*/
dbgevent_t *DbgGdbMI::step(int count, bool in, const char* threads) {
    DVPRINT("*** Dbg %s, %s\n", __FILE__, __func__);

    if (Status == DBGSTAT_EXIT) {
        return NewErrorEvent(DBGERR_AT_EXIT, "");
    }

    if (count < 1) count = 1;

    if (threads) {
        // gdb does not respect the --thread argument to -exec-next so we'll use apply
        auto os = std::ostringstream{};
        os << threads << " " << (in ? "s" : "n");
        if (count > 1) os << " " << count;
        os << "&";              // the call is asynchronous

        auto e = threadApply(os.str().c_str());

        if (e->ev_event == DBGEV_ERROR)
            return e;

        // otherwise it doesn't return anything interesting
        FreeEvent(e);
    } else {
        // normal next/step.   If there are multiple threads, the behavior will
        // depend on the non-stop and scheduler-locking modes.
        auto cmd = (in ? MIExecStep(count) : MIExecNext(count));
        SendMICommand(GDBMISession, cmd);

        if (!MICommandResultOK(cmd)) return NewMIErrorEvent(cmd, FREE_CMD);

        MICommandFree(cmd);
    }

    // register the method for catching the response when it comes back
    ThreadAsyncCheck(WaitEvent, nullptr);

    Status = DBGSTAT_RUNNING;

    auto e = NewEvent(DBGEV_OK);

    return e;
}

static dbgframe_t* MakeFrameFromMIFrame(MIFrame* mi, bool keepAddr) {
    dbgloc resLoc{StrdupOpt(mi->file), StrdupOpt(mi->func),
        keepAddr ? StrdupOpt(mi->addr) : nullptr, mi->line};
    return new dbgframe_t{mi->level, resLoc, nullptr, nullptr, nullptr};
}


/*
** Move up or down count stack frames.
*/
dbgevent_t *DbgGdbMI::moveFrame(int count, DbgFrameDir dir, const char* threads) {
    DVPRINT("*** Dbg %s, %s\n", __FILE__, __func__);
    dbgevent_t *e;

    int cur_level;
    int max_depth;
    int frame_number = 0;

    MICommand *cmd;
    MIList *mi_frame_list;
    MIFrame *mi_frame;

    if (threads) {
        std::ostringstream op;
        auto dirStr = "";
        switch (dir) {
        case DbgFrameDir::UP: dirStr = " up"; break;
        case DbgFrameDir::DOWN: dirStr = " down"; break;
        case DbgFrameDir::ABSOLUTE: dirStr = " select-frame"; break;
        };
        op << threads << dirStr << count;
        return threadEventToFrame(threadApply(op.str().c_str()));
    }

    // first get the level of the current frame
    cmd = MIStackInfoFrame();
    SendMICommand(GDBMISession, cmd);

    if (!MICommandResultOK(cmd)) return NewMIErrorEvent(cmd, FREE_CMD);

    // process the result
    mi_frame_list = MIGetFrameInfo(cmd);

    if (mi_frame_list == NULL) return NewMIErrorEvent(cmd, FREE_CMD);

    // get the frame
    MIListSet(mi_frame_list);
    if ((mi_frame = (MIFrame *)MIListGet(mi_frame_list)) == NULL) {
        MIListFree(mi_frame_list, NULL);
        return NewMIErrorEvent(cmd, FREE_CMD, DBGERR_NOSTACK);
    }
    // save the level of the current frame
    cur_level = mi_frame->level;

    // free the MIFrame list
    MIListFree(mi_frame_list, (void (*)())MIFrameFree);
    MICommandFree(cmd);

    // now we need to get the depth of the stack
    cmd = MIStackInfoDepth();
    SendMICommand(GDBMISession, cmd);

    if (!MICommandResultOK(cmd)) return NewMIErrorEvent(cmd, FREE_CMD);

    // process the result
    if ((max_depth = MIGetStackInfoDepth(cmd)) < 0) {
        return NewMIErrorEvent(cmd, FREE_CMD, DBGERR_NOSTACK);
    }
    MICommandFree(cmd);

    // the frame we want to select is based on an offset of the current
    // frame. We must ensure that this is between a min of 0 and a max of
    // max_depth
    switch (dir) {
    case DbgFrameDir::DOWN:
        if ((frame_number = cur_level - count) < 0) {
            EVENT_ERROR(e, DBGERR_LASTFRAME, "");
            return e;
        }
        break;

    case DbgFrameDir::UP:
        if ((frame_number = cur_level + count) >= max_depth) {
            EVENT_ERROR(e, DBGERR_FIRSTFRAME, "");
            return e;
        }
        break;

    case DbgFrameDir::ABSOLUTE:
        if ( (frame_number = count) >= max_depth) {
            EVENT_ERROR(e, DBGERR_FIRSTFRAME, "");
            return e;
        }
        break;
    }

    // now we know which frame we should select
    cmd = MIStackSelectFrame(frame_number);
    SendMICommand(GDBMISession, cmd);
    MICommandFree(cmd);

    // now query information about the frame we changed to and return it
    cmd = MIStackInfoFrame();
    SendMICommand(GDBMISession, cmd);

    if (!MICommandResultOK(cmd)) return NewMIErrorEvent(cmd, FREE_CMD);

    // process the result
    mi_frame_list = MIGetFrameInfo(cmd);

    if (mi_frame_list == NULL) return NewMIErrorEvent(cmd, FREE_CMD, DBGERR_NOSTACK);

    // get the frame
    MIListSet(mi_frame_list);
    if ((mi_frame = (MIFrame *)MIListGet(mi_frame_list)) == NULL) {
        MIListFree(mi_frame_list, NULL);
        return NewMIErrorEvent(cmd, FREE_CMD, DBGERR_NOSTACK);
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
** Show current ELF stack frames.
** If count is not zero then return only that many frames. If count is zero then
** return an unlimited number of frames. If count is negative return the
** outermost frames.
*/
dbgevent_t* DbgGdbMI::showElfFrames(int count, const char* threads) {
    DVPRINT("*** Dbg %s, %s %d %s\n", __FILE__, __func__, count, threads ? threads : "");
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

        if (!MICommandResultOK(cmd)) return NewMIErrorEvent(cmd, FREE_CMD);

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
        if (MICommandResultClass(cmd) == MIResultRecordRUNNING) {
            DVPRINT("MIStackListFrames is RUNNING\n");
            e = NewErrorEvent(DBGERR_ISRUNNING, "Process is running");
        } else {
            DVPRINT("MIStackListFrames FAILED\n");
            e = NewMIErrorEvent(cmd);
        }
        MICommandFree(cmd);
        return e;
    }

    DVPRINT("MIStackListFrames succeeded\n");

    if ((frames = MIGetStackListFramesInfo(cmd)) == NULL) {
        DVPRINT("MIGetStackListFramesInfo failed\n");
        MICommandFree(cmd);
        EVENT_ERROR(e, DBGERR_NOSTACK, "");
        return e;
    }
    MICommandFree(cmd);

    // parse over the frame list
    // increment the frames count each time through
    dbgframe_t *result = nullptr, *tail = nullptr;

    for (MIListSet(frames); (frame = (MIFrame *)MIListGet(frames)) != NULL;) {
        // create the new frame (the "treeing" process relies on the frame address)
        auto df = MakeFrameFromMIFrame(frame, true);
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

dbgevent_t *DbgGdbMI::showFrame(int count, const char* threads) {
    return isPython() ? pythonBEBacktrace() : showElfFrames(count, threads);
}

/*
** Get selected stack frame. This will return information about the current
*frame
** that is selected.
*/
dbgevent_t *DbgGdbMI::getFrame(bool verbose, const char* threads) {
    DVPRINT("*** Dbg %s, %s\n", __FILE__, __func__);
    dbgevent_t *e;

    if (threads && threads[0]) {
        auto op = std::string{threads} + " frame";
        return threadEventToFrame(threadApply(op.c_str()));
    }

    // Get the selected frame
    auto cmd = MIStackInfoFrame();
    SendMICommand(GDBMISession, cmd);

    if (!MICommandResultOK(cmd)) return NewMIErrorEvent(cmd, FREE_CMD);

    // process the result
    auto mi_frame_list = MIGetFrameInfo(cmd);

    if (mi_frame_list == NULL) return NewMIErrorEvent(cmd, FREE_CMD, DBGERR_NOSTACK);

    // get the frame
    MIListSet(mi_frame_list);
    auto mi_frame = (MIFrame *)MIListGet(mi_frame_list);

    if (!mi_frame) {
        MIListFree(mi_frame_list, NULL);
        return NewMIErrorEvent(cmd, FREE_CMD, DBGERR_NOSTACK);
    }

    // create the result
    auto result = MakeFrameFromMIFrame(mi_frame, verbose);

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

dbgevent_t *DbgGdbMI::setVar(const char *var, const char* val, const char* threads) {
    DVPRINT("*** Dbg %s, %s\n", __FILE__, __func__);

    // get type info about var to make sure it exists
    if (!threads) {
        // for multiple threads the variable may of different type or not exist
        // in every thread - let thread apply return an error information.
        auto e = getType(var, 0, threads);
        if (e->ev_event != DBGEV_TYPE) {
            return e;
        } else {
            FreeEvent(e);
        }
    }

    // create the assignment string
    auto expr = std::string{var} + "=" + val;
    DVPRINT("DbgSetVar: expr is %s\n", expr.c_str());

    if (threads) {
        auto tcommand = std::string{threads} + " set " + expr;
        DVPRINT("DbgSetVar: arguments to apply are %s\n", tcommand.c_str());

        auto e = threadApply(tcommand.c_str());
        if (e->ev_event == DBGEV_ERROR) {
            return e;
        } else {
            FreeEvent(e);
        }
    } else {
        // set the var to the val of the aif var
        auto cmd = _MIGDBSet(expr.c_str(), NULL);
        SendMICommand(GDBMISession, cmd);

        if (!MICommandResultOK(cmd)) return NewMIErrorEvent(cmd, FREE_CMD, DBGERR_SETVAR);
        MICommandFree(cmd);
    }

    return NewEvent(DBGEV_OK);
}


/*
** Find type of variable.
** If isType is true, then we are finding type information for a type.
*/
dbgevent_t *DbgGdbMI::getType(const char *var, bool isType, const char* threads) {
    DVPRINT("*** Dbg %s, %s\n", __FILE__, __func__);

    if (threads && !isType) {
        // threads could be looking at variables in different scopes
        auto command = std::string{threads} + " whatis " + var;
        return threadApply(command.c_str());
    }

    auto cmd = (isType ? CLIPtype(var) : CLIWhatis(var));

    SendMICommand(GDBMISession, cmd);
    if (!MICommandResultOK(cmd)) {
        auto e = NewErrorEvent(DBGERR_NOSYM, "%s", var);
        MICommandFree(cmd);
        return e;
    }

    auto type = (isType ? CLIGetPtypeInfo(cmd) : CLIGetWhatisInfo(cmd));
    if (!type) {
        auto e = NewErrorEvent(DBGERR_NOSYM, "%s", var);
        MICommandFree(cmd);
        return e;
    }
    MICommandFree(cmd);

    auto e = NewEvent(DBGEV_TYPE);
    e->ev_type = strdup(type);

    free(type);

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
    static std::regex amdGpuRE(R"(AMDGPU ((Thread)|(Wave)) [0-9:.,() ]+/[0-9]+)");
    static std::regex multispaceRE(R"(\s\s+)");  // gdb format alignment can stymie aggregation
    static std::regex intArgRE(R"(=\d+)");
    
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
        if (std::smatch match; std::regex_search(line, match, amdGpuRE)) {
            // On AMDGPU's remove integral arguments too.  The motiviation is that openMP is
            // now adding an integer argument that is *different* for every single thread.
            line.replace(match[0].first, match[0].second, "AMDGPU");
            line = std::regex_replace(line, intArgRE, "=");
        }
        line = std::regex_replace(line, multispaceRE, " ");

        os << line << "\n";
    }

    return os.str();
}

static std::string anonymizeCudaThreadInfo(const std::string& val) {
    DVPRINT("*** Dbg %s, %s\n", __FILE__, __func__);

    static std::regex hdrRE{R"((\s*\S+\s+\S+).*)"};
    static std::regex kernelRE{R"(^\s*Kernel .*)"};

    // BlockId TheadIdx (to) BlockIdx ThreadIdx Count VirtualPC Filename Line
    static std::regex threadRE{R"((\*?\s*[0-9(),]+\s+[0-9(),]+)\s+\S+\s+\S+\s+(\d+)\s+0x[[:xdigit:]]+\s+(.*))"};
     
    auto inStr = std::istringstream{val};
    auto line = std::string{};

    auto os = std::ostringstream{};
    os << "Showing one thread per location; use -v for full list.\n";

    auto locMap = std::map<std::string, std::pair<std::string,int>>{};

    auto printMap = [&os, &locMap]() {
         for (auto& [loc, pr] : locMap) {
             os << pr.first << " " << std::setw(6) << pr.second << std::setw(0)
                << " " << loc << "\n";
         }};

    bool readHdr = false;
    std::smatch match;
    while (std::getline(inStr, line)) {
        if (!readHdr) {
            if (line.find("BlockIdx") == std::string::npos &&
                line.find("Virtual") == std::string::npos) {
                DVPRINT("unrecognized header \"%s\"\n", line.c_str());
                return val;
            } else {
                readHdr = true;
                if (std::regex_match(line, match, hdrRE)) {
                    os << match[1].str() << "   Count    Location\n";
                }
            }
        } else if (std::regex_match(line, kernelRE)) {
            printMap();
            os << line << "\n";
        } else if (std::regex_match(line, match, threadRE)) {
            auto t = match[1].str();    // * (0,0,0)  (0,0,0)
            auto n = std::stoi(std::string{match[2]});   // count
            auto loc = std::string{match[3]}; // filename + line
            if (auto [it,added] = locMap.insert(std::make_pair(loc,std::make_pair(t,n))); !added) {
                it->second.second += n;
            } else if (t[0] == '*') {
                it->second.first = t; // prefer the thread marked as default
            }
        } else {
            // unrecognized line
            DVPRINT("unrecognized line \"%s\"\n", line.c_str());
            return val;
        }
    }

    printMap();

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
        if (!id.empty() && id[0] != '*') id = " " + id;
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
//
// numThreads will contain the number of threads involved, note that it will return 0 for the
// thread set "all".   (currently that information is not used when thread apply is called
// directly by the user, and other contexts don't use "all")
std::string DbgGdbMI::parseThreadApplyOp(const std::string& op, bool& isSilent, bool& isQuiet, bool &isVerbose, size_t& numThreads) {
    DVPRINT("*** Dbg %s, %s\n", __FILE__, __func__);
    isSilent = isQuiet = isVerbose = false;

    std::istringstream is(op);
    std::ostringstream os;
    std::string arg;

    bool inFlags = true, isAll = false, isAscending = false, isFirst = false;
    numThreads = 0;

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

    numThreads = threadIds.size();

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
    std::regex headerRE(R"(^Thread\s+(\d+)\s*\((AMDGPU\s*)?Thread[^:]*:)");
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

    bool isSilent = false, isQuiet = false, isVerbose = false; size_t numThreads;
    std::string op = parseThreadApplyOp(inOp, isSilent, isQuiet, isVerbose, numThreads);

    dbgevent_t* e = nullptr;

    if (op.substr(0,6) == "error:") {
        EVENT_ERROR(e, DBGERR_DEBUGGER, op.substr(6).c_str());
        return e;
    }

    auto isBackground = false;
    auto lastCharPos = op.find_last_not_of(" \t");
    if (lastCharPos != std::string::npos && op[lastCharPos] == '&') isBackground = true;

    const char* cmd_args[3] = { "thread", "apply", op.c_str() };

    MICommand* cmd = CLIBypassc(3, cmd_args);
    SendMICommand(GDBMISession, cmd);

    if (isBackground) {
        // The command will not contain the actual result yet.
        numBackgroundThreads = numThreads;
        MICommandFree(cmd);
        e = NewEvent(DBGEV_THREADINFO);
        e->ev_string = strdup("applied in background");
        return e;
    }
    numBackgroundThreads = 0;

    if (!MICommandResultOK(cmd)) {
        using namespace std::string_literals;
        char * errStr = MICommandResultErrorMessage(cmd);
        auto fullError = "Thread apply failed. (gdb: "s + (errStr ? errStr : "No error message given") + ")";

        EVENT_ERROR(e, DBGERR_DEBUGGER, "%s", fullError.c_str());
        MICommandFree(cmd);
        free(errStr);
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
        e = NewMIErrorEvent(cmd);
    } else {
        // current compilers can't change this state after launch, but update the
        // mode if this ever succeeds
        isInNonStopMode = (strcasecmp(mode, "on") == 0);
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

// ompd prints the thread info in a little different format; in particular the thread indices
// in ompd simple indices so we probably want them displayed - we just filter out the
// LWP numbers and hex addresses that mess up aggregation
static std::string anonymizeOmpdThreadInfo(const std::string& val) {
    static std::regex threadId{R"(^(Thread\s+\d+)\s*\(\d+\)(.*))"};
    static std::regex threadId2{R"(^(\[.* thread\s+\d+)\s+\(Thread.*\(LWP.*\)\)(.*))"};
    static std::regex hexValRE(R"(0x[0-9a-f]+)");  // the extra ( in ) is for the proc counter

     std::istringstream is(val);
     std::ostringstream os;

     for (std::string line; std::getline(is,line); ) {
         line = std::regex_replace(line, threadId, "$1$2");
         line = std::regex_replace(line, threadId2, "$1$2");
         line = std::regex_replace(line, hexValRE, "");
         os << line << "\n";
     }

     return os.str();
}

// OpenMP debugger functions
dbgevent_t *DbgGdbMI::ompd(const char* inOpp) {
    DVPRINT("*** Dbg %s, %s\n", __FILE__, __func__);

    if (auto e = initOmpd()) return e;  // initOmpd returns nullptr on success

    static std::regex verboseRE{R"(^\s*-v\s*)"};
    auto isVerbose = bool{false};
    auto opp = std::string{inOpp};

    auto match = std::smatch{};
    if (std::regex_search(opp, match, verboseRE)) {
        isVerbose = true;
        opp = match.suffix().str();
    }

    const char* cmd_args[2] = { "ompd", opp.c_str() };
    if (opp == "help") {
        cmd_args[0] = "help";
        cmd_args[1] = "ompd";
    }

    MICommand* cmd = CLIBypassc(2, cmd_args);
    SendMICommand(GDBMISession, cmd);

    bool isRunning = false;

    dbgevent_t* e = nullptr;
    if (MICommandResultClass(cmd) == MIResultRecordRUNNING) {
        // ompd init runs aynchronously after it sets a temporary breakpoint
        isRunning = true;
        e = NewEvent(DBGEV_OK);
    } else if (!MICommandResultOK(cmd)) {
        const char* msg = MICommandResultErrorMessage(cmd);
        if (!msg) msg = "ompd failed.";
        EVENT_ERROR(e, DBGERR_DEBUGGER, "%s", msg);
    } else if (char* result = MICommandConsoleOOBMessages(cmd)) {
        e = NewEvent(DBGEV_THREADINFO);

        if (isVerbose || opp != "threads") {
            e->ev_string = result;
        } else {
            auto cresult = anonymizeOmpdThreadInfo(result);
            free(result);
            e->ev_string = strdup(cresult.c_str());
        }

    } else {
        EVENT_ERROR(e, DBGERR_DEBUGGER, "Gdb ompd did not return a value.");
    }

    MICommandFree(cmd);

    if (isRunning) {
        DVPRINT("RUNNING : calling ThreadAsyncCheck\n");
        ThreadAsyncCheck(WaitOmpdEvent, nullptr);
        Status = DBGSTAT_RUNNING;
    }
    return e;
}

static dbgevent_t *WaitOmpdEvent(dbgevent_t* inEvent) {
    DVPRINT("*** Dbg %s, %s\n", __FILE__, __func__);
    auto e = WaitEvent(inEvent);

    // ompd init set an internal breakpoint and runs to it, so gdb4hpc is going to get
    // an event on a breakpoint is doesn't know anything about.
    if (e && e->ev_event == DBGEV_ERROR && strcmp(e->ev_errstr, unknownBPErrorMsg) == 0) {
        FreeEvent(e);
        e = NewEvent(DBGEV_OK);
    }

    return e;
}

// initialize teh ompd plugin if it hasn't been already - return an error event if
// initialization fails
dbgevent_t* DbgGdbMI::initOmpd() {
    if (!m_ompdIsInitialized) {
        m_ompdIsInitialized = true;
        m_ompdIsAvail = initOmpdL();
    }

    if (m_ompdIsAvail) return nullptr;
    else return NewErrorEvent(DBGERR_NOTIMP, "could not start ompd plugin");
}

bool DbgGdbMI::initOmpdL() {
    DVPRINT("initialized ompd\n");
    if (m_beFileDir.empty()) return false;

    auto initFile = m_beFileDir;
    initFile += "/__init__.py";

    const char* cmd_args[2] = {"source", initFile.c_str() };
    MICommand* cmd = CLIBypassc(2, cmd_args);
    SendMICommand(GDBMISession, cmd);

    bool ok = true;
    if (!MICommandResultOK(cmd)) {
        DVPRINT("Loading ompd plugin failed\n");
        ok = false;
    }
    MICommandFree(cmd);
    return ok;
}

// Cuda debugger functions
dbgevent_t *DbgGdbMI::cuda(const char* inOp) {
    DVPRINT("*** Dbg %s, %s \"%s\"\n", __FILE__, __func__, inOp);

    if (auto e = initOmpd()) return e;  // initOmpd returns nullptr on success

    static std::regex verboseRE{R"(\s*-v\b)"};
    auto isVerbose = bool{false};
    auto op = std::string{inOp};

    auto match = std::smatch{};
    if (std::regex_search(op, match, verboseRE)) {
        isVerbose = true;
        op = match.prefix().str() + match.suffix().str();
    }

    static const std::vector<std::string> subCmds{"help", "info", "set", "show"};

    auto cudaOp = "cuda " + op;
    auto cmd_args = ArgvStream{};
    cmd_args.parseAndAppendArgs(cudaOp.c_str());

    // cuda gdb puts "cuda" after the sub commands, e.g. "help set cuda cuda-variable" 
    for (size_t i = 0;
         i < cmd_args.argc() - 1 && std::find(subCmds.begin(), subCmds.end(), cmd_args[i+1]) != subCmds.end();
         ++i) {
        std::swap(cmd_args[i], cmd_args[i+1]);
    }
    auto argc = cmd_args.argc();
    
    MICommand* cmd = CLIBypass(argc, const_cast<char**>(cmd_args.c_strs().data()));
    SendMICommand(GDBMISession, cmd);

    bool isRunning = false;

    dbgevent_t* e = nullptr;
    if (MICommandResultClass(cmd) == MIResultRecordRUNNING) {
        // ompd init runs aynchronously after it sets a temporary breakpoint
        isRunning = true;
        e = NewEvent(DBGEV_OK);
    } else if (!MICommandResultOK(cmd)) {
        const char* msg = MICommandResultErrorMessage(cmd);
        if (!msg) msg = "cuda command failed.";
        e = NewErrorEvent(DBGERR_DEBUGGER, "%s", msg);
    } else if (char* result = MICommandConsoleOOBMessages(cmd)) {
        e = NewEvent(DBGEV_THREADINFO);

        DVPRINT("--- CUDA result op = \"%s\"\n", op.c_str()); 
        if (isVerbose || op != "info threads") {
            e->ev_string = result;
        } else {
            auto cresult = anonymizeCudaThreadInfo(result);
            free(result);
            e->ev_string = strdup(cresult.c_str());
        }

    } else {
        EVENT_ERROR(e, DBGERR_DEBUGGER, "Gdb cuda did not return a value.");
    }

    MICommandFree(cmd);

    if (isRunning) {
        DVPRINT("RUNNING : calling ThreadAsyncCheck\n");
        ThreadAsyncCheck(WaitOmpdEvent, nullptr);
        Status = DBGSTAT_RUNNING;
    }
    return e;
}

dbgevent_t *DbgGdbMI::sleep(int seconds) {
    DVPRINT("Sleeping for %d seconds...\n", seconds);
    ::sleep(seconds);
    DVPRINT("OK!\n");
    return NewEvent(DBGEV_OK);
}

dbgevent_t *DbgGdbMI::shell(int argc, char **argv){
    DVPRINT("Shell Invoked\n");

    // Create the command string
    auto cmd = ArgsToString(&argv[0], argc);
    cmd += " 2>&1";

    // Open the command for reading
    DVPRINT("sending command: \"%s\"\n", cmd.c_str());

    auto cmd_out = popen(cmd.c_str(), "r");
    if (!cmd_out) {
        DVPRINT("Failed to run command.\n");
        return NewErrorEvent(DBGERR_PIPE, "Could not open pipe on back end.");
    }

    // Now read the result
    const size_t buf_size = BUFSIZ;
    char buf[buf_size];
    size_t bytes_read = 0;

    std::string output = "";

    for (bytes_read = ::fread(buf, 1, buf_size, cmd_out); bytes_read == buf_size; bytes_read = ::fread(buf, 1, buf_size, cmd_out)) {
        output.append(buf, bytes_read);
    }

    // fread returns a number < buf_size (breaks the loop) upon either error or eof.
    if (::ferror(cmd_out)) {
        ::pclose(cmd_out);
        return NewErrorEvent(DBGERR_DEBUGGER, "Failed to read shell output");
    }

    // no error, must have been eof. a-ok.
    // copy last one into output
    output.append(buf, bytes_read);
    ::pclose(cmd_out);

    // XXX: since ev_string is a null terminated string, it can't encode null bytes.
    // transform them into '\x00' so they get shown to the user.
    for (size_t i = output.find('\0', 0); i != std::string::npos; i = output.find('\0', i)) {
        output.replace(i, 1, "\\x00");
    }

    auto e = NewEvent(DBGEV_STRING);
    e->ev_string = ::strdup(output.c_str());

    return e;
}

dbgevent_t *DbgGdbMI::signal(char const* signame) {

    using CommandPtr = std::unique_ptr<MICommand, decltype(&MICommandFree)>;

    DVPRINT("Sending signal %s\n", signame);

    // Create queued signal command
    const char* argv[] = {"queue-signal", signame};
    CommandPtr signalCmd(CLIBypassc(2, argv), MICommandFree);

    // Send queued signal
    SendMICommand(GDBMISession, signalCmd.get());
    if (!MICommandResultOK(signalCmd.get())) {
        DVPRINT("Signal command failed\n");
        auto msg = MICommandResultErrorMessage(signalCmd.get());
        return NewErrorEvent(DBGERR_DEBUGGER, "%s", (msg) ? msg : "Signal failed");
    }

    // Report success
    auto e = NewEvent(DBGEV_STRING);
    auto resultStr = std::string{"Queued signal "} + signame;
    e->ev_string = ::strdup(resultStr.c_str());

    return e;
}

dbgevent_t* DbgGdbMI::pythonBacktrace() {
    DVPRINT("pythonBacktrace\n");

    const char* argv[1] = { "py-bt" };
    auto cmd = std::unique_ptr<MICommand, decltype(&MICommandFree)>(CLIBypassc(1, argv), MICommandFree);
    SendMICommand(GDBMISession, cmd.get());

    auto result = MallocedCharPtr(MICommandConsoleOOBMessages(cmd.get()));

    if (auto* err = checkForPythonError(*cmd, result.get_string())) {
        return err;
    }

    if (auto* frames = parsePythonBacktrace(result.get_string())) {
        auto* e = NewEvent(DBGEV_FRAME);
        e->ev_frame = frames;
        return e;
    } else {
        return NewErrorEvent(DBGERR_DEBUGGER, "Failed to parse py-bt result");
    }
}

dbgevent_t* DbgGdbMI::pythonList(const std::string& arg) {
    DVPRINT("pythonList\n");

    const char* argv[2] = { "py-list", arg.c_str() };
    auto cmd = std::unique_ptr<MICommand, decltype(&MICommandFree)>(CLIBypassc(2, argv), MICommandFree);
    SendMICommand(GDBMISession, cmd.get());

    auto result = MallocedCharPtr(MICommandConsoleOOBMessages(cmd.get()));

    if (auto* err = checkForPythonError(*cmd, result.get_string())) {
        return err;
    }

    auto* e = NewEvent(DBGEV_STRING);
    e->ev_string = result.release();
    return e;
}

dbgevent_t* DbgGdbMI::pythonMoveFrame(DbgFrameDir dir) {
    DVPRINT("pythonMoveFrame\n");

    auto cmd = [&]() {
        const char* up_argv[1] = { "py-up" };
        const char* down_argv[1] = { "py-down" };
        switch (dir) {
            case DbgFrameDir::UP: return make_MICmd(CLIBypassc(1, up_argv));
            case DbgFrameDir::DOWN: return make_MICmd(CLIBypassc(1, down_argv));
            // frontend will ensure ABSOLUTE is never passed
            case DbgFrameDir::ABSOLUTE: return make_MICmd(nullptr);
        }
        return make_MICmd(nullptr); // unreachable
    }();

    if (!cmd) {
        return NewErrorEvent(DBGERR_DEBUGGER, "Only py-up and py-down are supported");
    }

    SendMICommand(GDBMISession, cmd.get());

    auto result = MallocedCharPtr(MICommandConsoleOOBMessages(cmd.get()));

    if (auto* err = checkForPythonError(*cmd, result.get_string())) {
        return err;
    }

    if (auto* frame = parsePythonMoveFrame(result.get_string())) {
        auto* e = NewEvent(DBGEV_FRAME);
        e->ev_frame = frame;
        return e;
    } else {
        return NewErrorEvent(DBGERR_DEBUGGER, "Failed to parse python frame");
    }
}

dbgevent_t* DbgGdbMI::pythonPrint(const std::string& name) {
    DVPRINT("pythonPrint %s\n", name.c_str());

    const char* argv[2] = { "py-print", name.c_str() };
    auto cmd = std::unique_ptr<MICommand, decltype(&MICommandFree)>(CLIBypassc(2, argv), MICommandFree);

    SendMICommand(GDBMISession, cmd.get());

    auto result = MallocedCharPtr(MICommandConsoleOOBMessages(cmd.get()));

    if (auto* err = checkForPythonError(*cmd, result.get_string())) {
        return err;
    }

    auto* e = NewEvent(DBGEV_STRING);
    e->ev_string = result.release();
    return e;
}

dbgevent_t* DbgGdbMI::pythonLocals() {
    DVPRINT("pythonLocals\n");

    const char* argv[1] = { "py-locals" };
    auto cmd = std::unique_ptr<MICommand, decltype(&MICommandFree)>(CLIBypassc(1, argv), MICommandFree);

    SendMICommand(GDBMISession, cmd.get());

    auto result = MallocedCharPtr(MICommandConsoleOOBMessages(cmd.get()));

    if (auto* err = checkForPythonError(*cmd, result.get_string())) {
        return err;
    }

    auto* e = NewEvent(DBGEV_STRING);
    e->ev_string = result.release();
    return e;
}

Pybe& DbgGdbMI::ensurePybe() {
    if (m_pybe) return *m_pybe; // no work to do

    if (!m_initializer || !m_initializer->getMISession()) {
        throw NewErrorEvent(DBGERR_INTERNAL, "Failed to connect to python.");
    }

    try {
        m_pybe = std::make_unique<Pybe>(*m_initializer->getMISession());
        return *m_pybe;
    } catch (...) {
        // turn python mode off so the user can use normal versions of debugger commands.
        // it's assumed that the error message we just caught will notify the user of this change.
        setPython(false);
        throw;
    }
}

dbgevent_t* DbgGdbMI::pythonBEGetPython() {
    auto e = NewEvent(DBGEV_STRING);
    e->ev_string = isPython() ? ::strdup("on") : ::strdup("off");
    return e;
};

dbgevent_t* DbgGdbMI::pythonBESetPython(PythonType setting) {
    if (setting != PythonType::On && setting != PythonType::Off) {
        throw NewErrorEvent(DBGERR_DEBUGGER, "Expected \"on\" or \"off\".");
    }
    setPython(setting == PythonType::On);
    return NewEvent(DBGEV_OK);
};

dbgevent_t* DbgGdbMI::pythonBEInject() {
    DVPRINT("pythonBEInject\n");
    ensurePybe(); // throws on failure
    return NewEvent(DBGEV_OK);
}

dbgevent_t* DbgGdbMI::pythonBEGetGIL() {
    DVPRINT("PythonBEGetGIL\n");
    ensurePybe().trivialize_gil(); // throws on error
    return NewEvent(DBGEV_OK);
}

dbgevent_t* DbgGdbMI::pythonBEAttach() {
    DVPRINT("PythonBEAttach\n");

    auto cmd = make_MICmd(MIDataEvaluateExpression("(char*)Py_GetVersion()"));
    SendMICommand(GDBMISession, cmd.get());
    if (MICommandResultOK(cmd.get())) {
        DVPRINT("enabling python\n");
        auto& pybe = ensurePybe();
        if (pybe.get_python_minor_version() <= 6) {
            return NewErrorEvent( DBGERR_DEBUGGER,
                "Attach is not supported on python versions <= 3.6. Debugging "
                "will continue, but python features are disabled.");
        }
        pybe.trivialize_gil();
        setPython(true);
        return NewEvent(DBGEV_OK);
    } else {
        DVPRINT("not python\n");
        // the string will not actually be printed
        auto* e = NewEvent(DBGEV_STRING);
        e->ev_u.ev_u_string = ::strdup("Not a python interpreter");
        return e;
    }
}

bool DbgGdbMI::isPython() const {
    return m_isPython;
}

void DbgGdbMI::setPython(bool isPython) {
    m_isPython = isPython;
}

// TODO name this better
static dbgframe_t* pybe_backtrace_to_dbgframe(const pybe::Backtrace& bt) {
    int current_level = 0;
    auto* first_frame = static_cast<dbgframe_t*>(nullptr); // "first" meaning "deepest"
    auto* last_frame = static_cast<dbgframe_t*>(nullptr);  // "last" meaning "shallowest" (e.g. main)

    for (const auto& py_frame : bt.frames) {
        // dbgloc owns the ptrs given to it
        auto location = dbgloc(::strdup(py_frame.file_name.c_str()),
                               ::strdup(py_frame.function_name.c_str()),
                               ::strdup("") /* address */,
                               py_frame.line_number);
        auto frame = new dbgframe_t(current_level, location, nullptr, nullptr, nullptr);
        current_level++;

        if (!first_frame) {
            first_frame = frame;
            last_frame = frame;
        } else {
            last_frame->setNext(frame);
            last_frame = frame;
        }
    }

    if (first_frame == nullptr) {
        // if the user requests a backtrace before python has started, we won't
        // get any frames. in that case give the user something informative
        auto location = dbgloc(nullptr, ::strdup("No python backtrace"), nullptr, 0);
        first_frame = new dbgframe_t(0, location, nullptr, nullptr, nullptr);
    }

    return first_frame;
}

dbgevent_t* DbgGdbMI::pythonBEBacktrace() {
    DVPRINT("pythonBEBacktrace\n");

    auto gil = ensurePybe().coerce_gil();
    auto bt = gil.get_backtrace();

    auto* e = NewEvent(DBGEV_FRAME);
    e->ev_frame = pybe_backtrace_to_dbgframe(bt);

    return e;
}

// given a completed command and its console output, check for python errors.
// if an error is detected, it will be returned as an error event. otherwise,
// returns nullptr. an empty `output` is considered an error.
static dbgevent_t* checkForPythonError(MICommand& cmd, const std::string& output) {
    if (!MICommandResultOK(&cmd)) {
        auto err_buf = MallocedCharPtr(MICommandResultErrorMessage(&cmd));
        auto err = err_buf.get_view();

        // if the python extension hasn't been loaded, expect
        // 'Undefined command: "py-...".  Try "help".'
        if (err.find("Undefined command") != std::string_view::npos) {
            return NewErrorEvent(DBGERR_DEBUGGER, "Python gdb extensions are not loaded");
        }

        return NewErrorEvent(DBGERR_DEBUGGER, "Unknown python command error");
    }

    if (output.empty()) {
        return NewErrorEvent(DBGERR_DEBUGGER, "Python command had no result");
    }

    // If python extensions are loaded, MI still considers some python "failures"
    // as successful and it will pass the ResultOK check above. We have
    // to examine the output for error messages. We keep the matching very
    // specific so that we (hopefully) don't accidentally match a successful
    // result that happens to have the error text in it.
    //
    // in cpp regex, ^/$ match the start/end of the whole string, not lines

    // py-bt/py-up/py-down
    static const auto frame1 = std::regex(R"((^|\n)Unable to locate python frame($|\n))");

    // py-list
    static const auto frame2 = std::regex(R"((^|\n)Unable to locate gdb frame for python bytecode interpreter($|\n))");

    // py-up/py-down
    static const auto older_frame = std::regex(R"((^|\n)Unable to find an older python frame($|\n))");
    static const auto newer_frame = std::regex(R"((^|\n)Unable to find a newer python frame($|\n))");

    auto match = std::smatch{};

    if (std::regex_search(output, match, frame1) || std::regex_search(output, match, frame2)) {
        return NewErrorEvent(DBGERR_DEBUGGER, "Unable to locate python frame");
    }

    if (std::regex_search(output, match, older_frame)) {
        return NewErrorEvent(DBGERR_DEBUGGER, "Unable to locate an older python frame");
    }
    if (std::regex_search(output, match, newer_frame)) {
        return NewErrorEvent(DBGERR_DEBUGGER, "Unable to locate a newer python frame");
    }

    return nullptr;
}

// turn the text output of py-bt into a linked list of dbgframe_t.
static dbgframe_t* parsePythonBacktrace(const std::string& text) {
    std::istringstream ss(text);
    std::string line;

    int current_level = 0;
    auto* first_frame = static_cast<dbgframe_t*>(nullptr); // "first" meaning "deepest"
    auto* last_frame = static_cast<dbgframe_t*>(nullptr);  // "last" meaning "shallowest" (e.g. main)

    while (std::getline(ss, line, '\n')) {
        DVPRINT("parsePythonBacktrace: %s\n", line.c_str());

        if (auto location = parsePythonLocation(line)) {
            auto* frame = new dbgframe_t(current_level, *location, nullptr, nullptr, nullptr);

            current_level++;

            if (!first_frame) {
                first_frame = frame;
                last_frame = frame;
            } else {
                last_frame->setNext(frame);
                last_frame = frame;
            }
        }
    }

    return first_frame;
}

static dbgframe_t* parsePythonMoveFrame(const std::string& text) {
    std::istringstream ss(text);
    std::string line;

    // the result should be on the first line, but scan until we get a match, just in case

    while (std::getline(ss, line, '\n')) {
        DVPRINT("parsePythonMoveFrame: %s\n", line.c_str());

        // extract the frame level. move frame command results will start with #\d+.
        if (line.size() < 2) continue; // can't extract frame level
        auto level = ::atoi(line.c_str() + 1);

        if (auto location = parsePythonLocation(line)) {
            // we only need one frame, so return
            return new dbgframe_t(level, *location, nullptr, nullptr, nullptr);
        }
    }

    return nullptr;
}

// parse a dbgloc from a single line of a py-bt/py-up/py-down command
static std::optional<dbgloc> parsePythonLocation(const std::string& line) {
    // expecting frame lines in the form of:
    //
    // Traceback (most recent call first):
    //   <built-in method __build_class__ of module object at remote 0x7fffed9266f0>
    //   File "/lib/python3.12/functools.py", line 347, in <module>
    //     class partialmethod(object):
    //   File "<frozen importlib._bootstrap>", line 488, in _call_with_frames_removed
    //   File "/py-mpi/./main.py", line 1, in <module>
    //     from mpi4py import MPI
    //
    // if we catch the python interpreter during garbage collecting, we just get a single frame:
    // Traceback (most recent call first):
    //   Garbage-collecting
    //
    // this function is also used for the results of py-up and py-down, which produce
    // results like:
    //
    // #3 <built-in method sleep of module object at remote 0x7fb51fead1f0>
    // #6 Frame 0x7fb520f28020, for file /py-mpi/main.py, line 43, in <module> ()
    // #6 Frame 0x7fb520f28078, for file /py-mpi/main.py, line 34, in main (num=6)
    // #6 Frame 0x7fb520f28078, for file /py-mpi/main.py, line 34, in main (nums=[[0, 1, 19...(truncated)

    static const std::regex builtin(R"(<built-in method (\S+) of module object at remote (0x[0-9a-f]+)>)");
    static const std::regex file1(R":(File "(.*)", line (\d+), in (\S+)):");
    static const std::regex file2(R"(file (\S+), line (\d+), in (\S+))");
    static const std::regex garbage(R"(Garbage-collecting$)");

    auto match = std::smatch{};
    auto location = std::optional<dbgloc>{};

    // FIXME: dbgloc_t is a very old type that accepts char*s and owns them.
    //        so there is way more string copying than necessary below...

    if (std::regex_search(line, match, builtin)) {
        // since we use regex_search here, we match both
        // "<built-in method..." and "#6 <built-in method..."
        location = dbgloc();
        location->loc_func = ::strdup(match[1].str().c_str());
        // an address is present in the line, but we leave it out so results aggregate better.
        // the address isn't useful to the user.
    } else if (std::regex_search(line, match, file1)) {
        location = dbgloc();
        location->loc_file = ::strdup(match[1].str().c_str());
        location->loc_line = ::atoi(match[2].str().c_str());
        location->loc_func = ::strdup(match[3].str().c_str());
    } else if (std::regex_search(line, match, file2)) {
        location = dbgloc();
        location->loc_file = ::strdup(match[1].str().c_str());
        location->loc_line = ::atoi(match[2].str().c_str());
        location->loc_func = ::strdup(match[3].str().c_str());
    } else if (std::regex_search(line, match, garbage)) {
        // python doesn't give us any info other than "Garbage-collecting"
        location = dbgloc();
        location->loc_func = ::strdup("Garbage-collecting");
    } else {
        DVPRINT("parsePythonLocation: No match\n");
    }

    return location;
}

/*
** List variables in the current stack frame
*/
dbgevent_t *DbgGdbMI::getVars(const char* threads) {
    DVPRINT("*** Dbg %s, %s %s\n", __FILE__, __func__,  threads ? threads : "");

    if (threads) {
        auto command = std::string{threads} + " info locals";
        return threadApply(command.c_str());
    }

    auto cmd = MIStackListVariables(SIMPLE_VALUES);
    SendMICommand(GDBMISession, cmd);

    if (!MICommandResultOK(cmd)) return NewMIErrorEvent(cmd, FREE_CMD);

    auto vars = MIGetStackListVariablesInfo(cmd);
    if (!vars) return NewMIErrorEvent(cmd, FREE_CMD);

    MICommandFree(cmd);

    dbgvars_t *result = nullptr;
    dbgvars_t *cur_var = nullptr;

    // parse over the vars list
    MIArg *var;
    for (MIListSet(vars); (var = (MIArg *)MIListGet(vars)) != NULL;) {

        // create the new var
        auto new_var = (dbgvars_t *)Alloc(sizeof(dbgvars_t));

        // first time through we want to set the root var as the result
        if (result == nullptr) result = new_var;

        // set the cur_var var_next pointer to the new var if this isn't
        // the first time through
        if (cur_var != nullptr) cur_var->var_next = new_var;

        // set the var_next to null
        new_var->var_next = (dbgvars_t *)nullptr;

        // set the name
        if (var->name != nullptr) {
            DVPRINT("var_name: %s\n", var->name);
            new_var->var_name = strdup(var->name);
        } else {
            DVPRINT("var_name: NULL\n");
            new_var->var_name = nullptr;
        }

        // set the type
        if (var->type != nullptr) {
            DVPRINT("var_type: %s\n", var->type);
            new_var->var_type = strdup(var->type);
        } else {
            DVPRINT("var_type: NULL\n");
            new_var->var_type = nullptr;
        }

        // set the cur_var pointer to the new_var
        cur_var = new_var;
    }

    // create the event with the var information
    auto e = NewEvent(DBGEV_VARS);
    e->ev_vars = result;

    // free the MIArg list
    MIListFree(vars, (void (*)())MIArgFree);

    return e;
}

/*
** List variables in the current stack frame
*/
dbgevent_t *DbgGdbMI::getArgs(const char* threads) {
    DVPRINT("*** Dbg %s, %s %s\n", __FILE__, __func__,  threads ? threads : "");

    if (threads) {
        auto command = std::string{threads} + " info args";
        return threadApply(command.c_str());
    }

    // first get the level of the current frame
    auto cmd = MIStackInfoFrame();
    SendMICommand(GDBMISession, cmd);
    if (!MICommandResultOK(cmd)) return NewMIErrorEvent(cmd, FREE_CMD);

    auto mi_frame_list = MIGetFrameInfo(cmd);
    if (!mi_frame_list) return NewMIErrorEvent(cmd, FREE_CMD, DBGERR_NOSTACK);

    MIListSet(mi_frame_list);
    auto mi_frame = (MIFrame *)MIListGet(mi_frame_list);
    auto level = mi_frame->level;

    MICommandFree(cmd);
    MIListFree(mi_frame_list, (void (*)())MIFrameFree);

    DVPRINT("got stack level %d, calling info args\n", level);

    cmd = MIStackListArguments(SIMPLE_VALUES, level, level);
    SendMICommand(GDBMISession, cmd);

    if (!MICommandResultOK(cmd)) return NewMIErrorEvent(cmd, FREE_CMD);

    mi_frame_list = MIGetStackListArgumentsInfo(cmd);
    if (!mi_frame_list) return NewMIErrorEvent(cmd, FREE_CMD, DBGERR_NOSTACK);

    auto resultStr = std::ostringstream{};
    bool first = true;

    static auto hexValRE = std::regex{R"(0x[1-9a-f][[:xdigit:]]+)"};

    MIListSet(mi_frame_list);
    for (MIFrame* frame; (frame = (MIFrame*)MIListGet(mi_frame_list)); ) {
        DVPRINT("GET A FRAME %s\n", frame->func ? frame->func : "nofunc" );

        if (auto args = frame->args) {
            MIListSet(args);
            while (auto arg = (MIArg*)MIListGet(args)) {
                if (!arg->name) continue;
                if (first) first = false;
                else resultStr << "\n";

                if (arg->type) resultStr << arg->type << " ";
                resultStr << arg->name;

                if (arg->value) {
                    resultStr << " = " << std::regex_replace(arg->value, hexValRE, "...");
                }
            }
        }
    }

    MIListFree(mi_frame_list, (void (*)())MIFrameFree);
    MICommandFree(cmd);

    auto e = NewEvent(DBGEV_STRING);
    e->ev_string = strdup(resultStr.str().c_str());
    
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
    if (!MICommandResultOK(cmd)) return NewMIErrorEvent(cmd, FREE_CMD);

    // Get the names of the vars
    if (!(vars = CLIGetVariablesFromInfoScopeInfo(cmd))) {
        return NewMIErrorEvent(cmd, FREE_CMD);
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
    // out what function the scope refers to by setting a breakpoint to get
    // the function name.

    cmd = MIBreakInsert(0, 0, 0, NULL, 0, location, 0);
    SendMICommand(GDBMISession, cmd);

    if (!MICommandResultOK(cmd)) {
        // free existing event
        FreeEvent(e);
        return NewMIErrorEvent(cmd, FREE_CMD);
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
                async_e = showElfFrames(1, threads);

                if (async_e->ev_frame != NULL) {
                    const auto &frameLoc = async_e->ev_frame->getLoc();

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
    return m_initializer->quit(detach);
}

/*
** Interrupt the debugger.
*/
dbgevent_t *DbgGdbMI::interrupt(bool all) {
    auto* e = m_initializer->interrupt(all);
    if (isPython()) ensurePybe().trivialize_gil();
    return e;
}

dbgevent_t *DbgGdbMI::launch(const char *, int, const char *,
                             const char *prog, const char *args,
                             char **, const char *main_function) {
    DVPRINT("*** Dbg %s, %s\n", __FILE__, __func__);
    dbgevent_t *e;

    e = m_initializer->initDbg(prog, args, main_function);

    Status = DBGSTAT_STOPPED;

    AsyncCheck(WaitEvent, nullptr);

    return e;
}

/*
** Attach debugger to running process.
*/
dbgevent_t *DbgGdbMI::attach(int pid, const char *prog, const char *, char **, const char *main_function) {
    DVPRINT("*** Dbg %s, %s\n", __FILE__, __func__);
    dbgevent_t *e;

    e = m_initializer->initDbgAttach(pid, prog, main_function);

    Status = DBGSTAT_STOPPED;

    return e;
}

/*
** Attach debugger to GDB server using provided command.
*/
dbgevent_t *DbgGdbMI::targetRemote(const char* target_remote) {
    DVPRINT("*** Dbg %s, %s\n", __FILE__, __func__);
    dbgevent_t *e;

    e = m_initializer->initDbgTargetRemote(target_remote);

    // Check for live VGDB connection
    // Exited instances will update State to Running to block commands
    (void)getStatus(false);
    DVPRINT("DbgGdbMI::targetRemote: inferior status %d\n", Status);

    return e;
}

/*
** Called if Shutdown > 0 to finalise debugger.
*/
void DbgGdbMI::finalize() {
    MICommand *cmd;

    DVPRINT("DbgGdbMI::finalize: sending exit.\n");
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
dbgevent_t *DbgGdbMI::setDebuggerVar(const char *var, const char *val) {
    DVPRINT("*** Dbg %s, %s\n", __FILE__, __func__);
    MICommand *cmd;


    cmd = MICommandNew("-gdb-set", MIResultRecordDONE);
    MICommandAddOption(cmd, var, val);
    SendMICommand(GDBMISession, cmd);
    MICommandFree(cmd);

    return NewEvent(DBGEV_OK);
}

dbgevent_t *DbgGdbMI::setSourcePath(DbgDirOp op, const char *path) {
    DVPRINT("*** Dbg %s, %s\n", __FILE__, __func__);

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

    auto val = getDebuggerVarL(var);
    auto ev = NewEvent(DBGEV_STRING);
    ev->ev_string = strdup(val.c_str());
    return ev;
}

std::string DbgGdbMI::getDebuggerVarL(const char* var) {
    auto res = std::string{};
    auto cmd = MICommandNew("-gdb-show", MIResultRecordDONE);
    MICommandAddOption(cmd, var, NULL);
    SendMICommand(GDBMISession, cmd);
    if (MICommandResultOK(cmd)) {
        res = MICommandOutputString(cmd);
    }
    MICommandFree(cmd);

    return res;
}


dbgevent_t *DbgGdbMI::setPrintElements(const char *elements) {
    DVPRINT("*** Dbg %s, %s %s\n", __FILE__, __func__, elements);

    auto elems = atoi(elements);

    // update cached value
    clientPrintElements = elems;

    // update debugger value
    return setPrintElementsL(elems);
}

dbgevent_t *DbgGdbMI::setPrintElementsL(int elems) {
    // update debugger value
    return setDebuggerVar("print elements", std::to_string(elems).c_str());
}

int DbgGdbMI::getPrintElementsL() {
    auto str = getDebuggerVarL("print elements");
    return atoi(str.c_str());
}

// This is called before entering gdbmode and before passthru commands are
// executed.
// We want gdbmode to appear like a freshly-started gdb instance would, so we
// reset some printing settings to gdb defaults.
dbgevent_t *DbgGdbMI::resetBeforeDirectCmd() {
    // this function always returns NewEvent(DBGEV_OK),
    // so we just need to clean it up
    DVPRINT("DbgGdbMI::resetBeforeDirectCmd\n");
    auto e = setPrintElementsL(directPrintElements);
    FreeEvent(e);

    // also returns NewEvent(DBGEV_OK)
    e = setDebuggerVar("print repeats", directPrintRepeats.c_str());
    FreeEvent(e);

    setSourceOpen(true);

    return NewEvent(DBGEV_OK);
}

// Called upon exit from gdbmode and after a passthru command is finished
dbgevent_t *DbgGdbMI::restoreAfterDirectCmd() {
    DVPRINT("In DbgGdbMI::restoreAfterDirectCmd\n");

    directPrintElements = getPrintElementsL();
    
    // this function always returns NewEvent(DBGEV_OK), so we just need to clean it up
    auto e = setPrintElementsL(clientPrintElements);
    FreeEvent(e);

    directPrintRepeats = getDebuggerVarL("print repeats");
        
    // also returns NewEvent(DBGEV_OK)
    e = setDebuggerVar("print repeats", "0");
    FreeEvent(e);

    setSourceOpen(false);

    return NewEvent(DBGEV_OK);
}

/*
** Pass command directly to gdb/mi through console command
*/
dbgevent_t *DbgGdbMI::directCmd(int cmd_argc, char **cmd_argv) {
    DVPRINT("*** Dbg %s, %s\n", __FILE__, __func__);
    MICommand *cmd;
    dbgevent_t *e;
    auto isMI = false;

    if (strcmp(cmd_argv[0],"interpreter-exec") != 0) {
        cmd = CLIBypass(cmd_argc, cmd_argv);
    } else if (cmd_argc > 2 && strstr(cmd_argv[1],"mi")) {
        isMI = true;
        cmd = MICommandNew(cmd_argv[2],MIResultRecordDONE);
        for (int i=3; i<cmd_argc; ++i)
            MICommandAddOption(cmd, cmd_argv[i], nullptr);
    } else {
        cmd = CLIBypass(cmd_argc-2, cmd_argv+2);
    }
   
    SendMICommand(GDBMISession, cmd);

    // Sometimes a command can cause the debugger to start running
    // If so the expected result record from CLIBypass would be wrong.
    // We don't want to use the MICommandResultOK command, rather we want to
    // check to see if it has an error.
    // We also want to update status in certain situations (like ^running)
    switch (MICommandResultClass(cmd)) {
        case MIResultRecordINVALID:
            EVENT_ERROR(e, DBGERR_DEBUGGER,
                        "Invalid command returned by MI layer.");
            MICommandFree(cmd);
            return e;

        case MIResultRecordRUNNING:
            // FIXME: Use a custom WaitEvent
            AsyncCheck(WaitEvent, nullptr);
            break;

        case MIResultRecordERROR:
            return NewMIErrorEvent(cmd, FREE_CMD);

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
    if (!isMI) {
        e->ev_cmd = MICommandConsoleOOBMessages(cmd);
    } else if (auto rr = MICommandResult(cmd)) {
        if (auto miStr = MIResultRecordToString(rr)) {
            e->ev_cmd = strdup(MIStringToCString(miStr));
            MIStringFree(miStr);
        }
    }

    MICommandFree(cmd);

    return e;
}

/*
** Invoked if there is input from the debugger after
** a non-terminating command (eg. run, step, etc.).
** Checks to see what caused the termination.
*/
static dbgevent_t *WaitEvent(dbgevent_t *e) {
    return DbgBackendGDBMI.waitEvent(e);
}

dbgevent_t* DbgGdbMI::waitEvent(dbgevent_t* e) {

    DVPRINT("*** Dbg %s, %s\n", __FILE__, __func__);

    MISessionRegisterThreadStoppedCallback(GDBMISession, nullptr);
    numBackgroundThreads = 0;

    dbgbp_t *bp = NULL;
    MIEvent *evt = PendingEvent;
    
    static auto lastKnownFrame = std::make_unique<dbgframe_t>(
        0, dbgloc_t{nullptr, nullptr, nullptr, 0}, nullptr, nullptr, nullptr
    );

    if (e == (dbgevent_t *)NULL) e = NewEvent(DBGEV_NONE);

    AsynchEventWaiting = 0;
    PendingEvent = NULL;

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

    std::unique_ptr<MIEvent, decltype(&MIEventFree)> evtHolder(evt, &MIEventFree);

    switch (evt->type) {
        case MIEventTypeCatchpointHit:
            DVPRINT("MIEventTypeCatchpointHit, falling through to WatchpointTrigger (and then Breakpoint hit)\n");
            [[fallthrough]];
        case MIEventTypeWatchpointTrigger:
            DVPRINT("MIEventTypeWatchpointHit, falling through to BreakpointHit\n");
            [[fallthrough]];
        case MIEventTypeBreakpointHit: {
            DVPRINT("MIEventTypeBreakpointHit\n");

            int idNumber = 0;
            if (evt->type == MIEventTypeBreakpointHit || evt->type == MIEventTypeCatchpointHit) {
                // breakpoint and catchpoint fill "bkptno"
                idNumber = evt->bkptno;
            } else {
                // watchpoints fill "number"
                idNumber = evt->number;
            }
            
            DVPRINT("Breakpoint id is %d\n", idNumber);

            // short circuit during the process of finding the starting breakpoint
            if (Status == DBGSTAT_INITIALISING) {
                bp = new dbgbp_t(0, idNumber); // internal BP, so no front end id
                e->ev_event = DBGEV_BPHIT;
                e->ev_bp = bp;
                return e;
            }

            Status = DBGSTAT_STOPPED;

            bp = FindBPByID(BP, 0, idNumber); 
            auto exitBPs = m_initializer->getExitBPs();

            DVPRINT("BPMgr: Looking for BP be:%d\n", idNumber);

            // We want to check for an exit breakpoint if we didn't find it in
            // the normal BP list.
            if (bp == nullptr) {
                bp = FindBPByID(exitBPs, 0, idNumber);
                // set the status to exit if we are at an exit breakpoint
                if (bp != nullptr) {
                    Status = DBGSTAT_EXIT;
                }
            }

            // If bp is still null, we have a problem.
            if (bp == nullptr) {
                DVPRINT("BPMgr: Could not find be breakpoint %d\n", idNumber);
                e = ChangeEvent(e, DBGEV_ERROR);
                e->ev_errno = DBGERR_DEBUGGER;
                EventErrStr(e, unknownBPErrorMsg);
                return e;
            }

            DVPRINT("BPMgr: Got BP fe:%d\n", bp->bp_fe_id);

            bp->bp_hits++;
            CurrBP = bp;

            // In the case of pending breakpoints, watchpoints, and catchpoints,
            // we need to update the breakpoint information with the evt frame.
            // This will change every time the breakpoint is hit.
            if (bp->bp_pending || bp->bp_type == DBGBP_WATCH_TYPE || bp->bp_type == DBGBP_CATCH_TYPE) {
                // First free the old information if present

                // XXX: the dbgloc_t in dbgbp_t manages its own memory and we
                // really shouldn't be reaching in like this...
                free(bp->bp_loc.loc_file);
                free(bp->bp_loc.loc_func);
                free(bp->bp_loc.loc_addr);
                bp->bp_loc.loc_file = nullptr;
                bp->bp_loc.loc_func = nullptr;
                bp->bp_loc.loc_addr = nullptr;
                bp->bp_loc.loc_line = -1;

                // Now update it with the new info
                if (evt->frame != nullptr) {
                    bp->bp_pending = 0; // this is fine on watch/catchpoints too
                    if (evt->frame->file != nullptr)
                        bp->bp_loc.loc_file = strdup(evt->frame->file);
                    if (evt->frame->func != nullptr)
                        bp->bp_loc.loc_func = strdup(evt->frame->func);
                    if (evt->frame->addr != nullptr)
                        bp->bp_loc.loc_addr = strdup(evt->frame->addr);
                    bp->bp_loc.loc_line = evt->frame->line;
                }
            }

            if (evt->frame && Status != DBGSTAT_INITIALISING) {
                lastKnownFrame.reset(MakeFrameFromMIFrame(evt->frame));
            }

            e->ev_event = DBGEV_BPHIT;
            e->ev_bp = bp->clone();

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
                    RemoveBP(BP, 0, evt->bkptno);
                    RemoveBP(exitBPs, 0, evt->bkptno);
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

            return e;
        }
        
        case MIEventTypeWatchpointScope:
            DVPRINT("MIEventTypeWatchpointScope\n");
            DVPRINT("Breakpoint id is %d\n", evt->number);

            bp = FindBPByID(BP, 0, evt->number);

            // If bp is null, we have a problem.
            if (bp == NULL) {
                e = ChangeEvent(e, DBGEV_ERROR);
                e->ev_errno = DBGERR_DEBUGGER;
                EventErrStr(e, "Hit unknown watchpoint!");
                return e;
            }

            // event type: breakpoint deleted
            e->ev_event = DBGEV_BPDEL;
            e->ev_bp = bp->clone();

            Status = DBGSTAT_STOPPED;
            return e;

        case MIEventTypeSuspended:
            DVPRINT("MIEventTypeSuspended, falling through to FunctionFinished, then SteppingRange\n");
            [[fallthrough]];
        case MIEventTypeFunctionFinished:
            DVPRINT("MIEventTypeFunctionFinished, falling through to SteppingRange\n");
            [[fallthrough]];
        case MIEventTypeSteppingRange:
            DVPRINT("MIEventTypeSteppingRange\n");
            e->ev_event = DBGEV_STEP;
            e->ev_step.step_frame_changed = false;
            e->ev_step.step_frame = nullptr;
            e->ev_step.step_threadId = evt->threadId;

            if (evt->frame) {
                e->ev_step.step_frame = MakeFrameFromMIFrame(evt->frame, false);

                auto oldFunc = lastKnownFrame->getLoc().loc_func;
                auto newFunc = e->ev_step.step_frame->getLoc().loc_func;
                
                e->ev_step.step_frame_changed = 
                    oldFunc && newFunc && strcmp(oldFunc, newFunc) != 0;

                // Don't cache our frame during startup or we will think we changed
                // frames on the very first step because _start or similar will be cached
                if (Status != DBGSTAT_INITIALISING) {
                    lastKnownFrame.reset(e->ev_step.step_frame->clone());
                }
            }

            Status = DBGSTAT_STOPPED;
            return e;

        case MIEventTypeSignal:
            DVPRINT("MIEventTypeSignal %s\n", evt->sigName);

            if (strcmp(evt->sigName,"SIGSTOP") == 0 && m_initializer->ignoreSigstop()) {
                DVPRINT("ignoring SIGSTOP\n");
                auto goEvent = go(true,nullptr);
                auto went = (goEvent && goEvent->ev_event == DBGEV_OK);
                FreeEvent(goEvent);
                if (went) return e; // still DBGEV_NONE
            }

            e->ev_event = DBGEV_SIGNAL;
            e->ev_sig_type =
                (evt->sigName == NULL ? NULL : strdup(evt->sigName));
            e->ev_sig_line = NULL;
            e->ev_sig_frame = (dbgframe_t *)NULL;

            if (evt->frame != NULL) {
                e->ev_sig_frame = MakeFrameFromMIFrame(evt->frame, false);
            }

            Status = DBGSTAT_STOPPED;
            return e;

        case MIEventTypeInferiorSignalExit:
            DVPRINT("MIEventTypeInferiorSignalExit\n");
            return e;

        case MIEventTypeInferiorExit:
            DVPRINT("MIEventTypeInferiorExit\n");
            e->ev_event = DBGEV_EXIT;
            e->ev_exit = 0;
            Status = DBGSTAT_WAITING;
            m_initializer->resetPId();
            return e;

        default:
            DVPRINT("MIEventTypeDefault\n");
            e = ChangeEvent(e, DBGEV_ERROR);
            e->ev_errno = DBGERR_DEBUGGER;
            EventErrStr(e, "unknown event");
            return e;
    }
    /* NOT REACHED */
}

/*
** Check that breakpoint command has succeded and
** extract appropriate information. Returns breakpoint
** id in bid. Adds to breakpoint list if necessary.
*/
static dbgevent_t *CheckBreak(int feId, MICommand *cmd, const dbgcomm_info *commInfo, int& beId) {
    DVPRINT("*** Dbg %s, %s\n", __FILE__, __func__);
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

    beId = bpt->number;

    bp = new dbgbp_t(feId, beId);

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
    } else if (bpt->catchType != nullptr) {
        bp->bp_type = DBGBP_CATCH_TYPE;
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
        // libmi might tuck the address in either the origLoc or func member
        DVPRINT("bpt has multiple locations.\n");
        if (bpt->origLoc || bpt->func) {
            DVPRINT("bpt original location is present.\n");
            char *loc_str = strdup(bpt->origLoc ? bpt->origLoc : bpt->func);
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
        if (bpt->what != NULL) {
            bp->bp_exp = strdup(bpt->what);
        } else if (bpt->catchType != NULL) {
            // some catch points (e.g. exec) don't report a "what" field but a
            // "catchpoint" field.
            bp->bp_exp = strdup(bpt->catchType);
        }
    }

    // Check to see if this is a pending breakpoint
    if (bpt->pending != NULL) {
        bp->bp_pending = 1;
        bp->bp_stmt = strdup(bpt->pending);
    }

    if (bpt->threadId != NULL) {
        bp->bp_threadId.assign(bpt->threadId);
    }

    if (commInfo) {
        bp->bp_comminfo = commInfo->clone();
    }

    /*
    ** Link a copy of the breakpoint onto BP.
    */
    DVPRINT("BPMgr: Adding breakpoint fe:%d, be:%d **\n", bp->bp_fe_id, bp->bp_be_id);
    BP.push_back(bp);

    /*
    ** Now create a fake event and make it
    ** look like a BPSET.
    */
    e = NewEvent(DBGEV_BPSET);
    e->ev_bp = bp->clone();

    MIListFree(bpts, (void (*)())MIBreakpointFree);

    return e;
}

// Top level call for evaluating a program expression and returning the data
dbgevent_t *DbgGdbMI::evalExpr(const char *exp, long expIdx, const char* threads) {
    DVPRINT("*** Dbg %s, %s(exp %s, rank %d, idx %ld)\n", __FILE__, __func__, exp, my_rank, expIdx);

    // track the evaluations that have only returned a portion of result
    static std::map<long,std::unique_ptr<AIFStream>> remainders;

    auto remainderLoc = remainders.find(expIdx);
    if (remainderLoc != remainders.end()) {
        // Return the next slice of data
        try {
            bool isFinal;
            dbgev ev = DBGEV_DATA_CHUNK;
            AIFStream* returnStream = remainderLoc->second->createStreamFromNextChunk(isFinal);
            if (returnStream) {
                if (isFinal) {
                    DVPRINT("returning the last slice of %ld\n", expIdx);
                    ev = DBGEV_DATA;
                    remainders.erase(remainderLoc);
                } else {
                    DVPRINT("Returning the next slice of %ld\n", expIdx);
                }
            } else {
                DVPRINT("getNextSlice failed");
                return NewErrorEvent(DBGERR_DEBUGGER, "array evaluation failed");
            }

            auto e = NewEvent(ev);
            e->ev_data = returnStream;
            return e;
        } catch (dbgevent_t* e) {
            remainders.erase(remainderLoc);
            return e;
        }
    }

    if (threads && threads[0]) {
        // evalExprWithThreads returns null if the result is single threaded, so that case
        // falls through into the normal processing
        if (auto e = evalExprWithThreads(exp, threads)) return e;
    }

    AIFStream* aifStream = GetAIFVar(GDBMISession, exp);

    dbgev ev = DBGEV_DATA;
    bool isFinal;
    AIFStream* returnStream = nullptr;
    try {
        returnStream = aifStream->createStreamFromNextChunk(isFinal);
    } catch (dbgevent_t* err) {
        DVPRINT("Get value failed\n");
        delete aifStream;
        return err;
    }

    if (!returnStream) {
        DVPRINT("getNextChunk failed\n");
        delete aifStream;
        return NewErrorEvent(DBGERR_DEBUGGER, "array evaluation failed");
    }

    if (isFinal) {
        delete aifStream;
    } else {
        // We are only returning the first chunk of the result.   Save the
        // rest for the continuation call.
        DVPRINT("saving remainder of expression %ld\n", expIdx);
        ev = DBGEV_DATA_CHUNK;
        remainders.insert(std::make_pair(expIdx,aifStream));
    }

    dbgevent_t* e = NewEvent(ev);
    e->ev_data = returnStream;
    return e;
}

// handling for expressions that have a defined thread set context.   If the thread set
// resolves to a single thread  we just change the thread context and return
dbgevent_t* DbgGdbMI::evalExprWithThreads(const char* exp, const char* threads)
{
    std::string error;
    auto threadSet = getThreadSet(threads, nullptr, error);
    if (!error.empty()) {
        return NewErrorEvent(DBGERR_DEBUGGER, "%s", error.c_str());
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
        AIFStream* aif = AIFStream::fromMultiThreadResult(applyEvent->ev_u.ev_u_string);
        FreeEvent(applyEvent);

        dbgevent_t* e = NewEvent(DBGEV_DATA);
        e->ev_data = aif;
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
// -- This is based on the strategy of using threadApply to do the aggregation, though
// it may leave us with potentially brittle text at this level.   If in the future gdb/mi
// gives a more efficient means to get frames from multiple threads, the approach could
// be changed to take advantage of it. ---
static dbgevent_t* threadEventToFrame(dbgevent_t* threadEvent) {
    if (threadEvent->ev_event != DBGEV_THREADINFO) {
        return threadEvent;
    }

    static std::regex atRE{R"(#(\d+)\s+(in\s+)?(.*)\s+at (.*):(\d+))"};
    static std::regex fromRE{R"(#(\d+)\s+(in\s+)?(.*)\s+from\s+(.*))"};
    static std::regex runningRE{R"(<.* is running.*>)"};

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

                auto loc = dbgloc_t{ strdup(file.c_str()),
                                     strdup(func.c_str()), nullptr, frameLine };
                frame = new dbgframe_t{level, loc, nullptr, threadStr, nullptr};
            } else if (std::regex_match(line, match, fromRE)) {
                auto level = std::stoi(match[1]);
                auto func = match[3].str();
                auto file = match[4].str();
                DVPRINT("frame %d IN %s FROM %s\n", level, func.c_str(), file.c_str());

                auto loc = dbgloc_t{ strdup(file.c_str()),
                                     strdup(func.c_str()), nullptr, 0 };

                frame = new dbgframe_t{ level, loc, nullptr, threadStr, nullptr};
            } else if (std::regex_match(line, match, runningRE)) {
                auto loc = dbgloc_t{ strdup(""), strdup("<running>"), nullptr, 0 };
                frame = new dbgframe_t{ 0, loc, nullptr, threadStr, nullptr};
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

// Callback function registered with libMI.
// This function is registered when gdb4hpc starts multiple background thread operations
// (e.g. next, step, continue) in non-stop mode.    This function is celled whenever libMI
// detects that a thread has stopped.
// We are using the global variable numBackgroundThreads to track how many background threads
// are still running.
// As per the libMI interface, returning non-zero indicates that libMI can stop waiting for
// more responses.
static int thread_stopped_callback(int) {
    if (numBackgroundThreads && !--numBackgroundThreads) {
        return 1;  // we've heard back from all threads
    }
    else {
        return 0;
    }
}

// Register a function with the top level server loop to tell it that we are waiting for an
// asynchronous reponse.   If the global variable numBackgroundThreads is non-zero, then
// we are running in non-stop mode and are expected numBackgroundThreads separate responses from
// gdb as those threads stop.  (i.e they've completed their "next" or "step").
// Once we've gotten the single response, or the expected multiple responses from gdb, the
// server look will call \a rtn with the event \a event.
static void ThreadAsyncCheck(dbgevent_t *(*rtn)(dbgevent_t *), dbgevent_t *event) {
    auto cb = numBackgroundThreads ? thread_stopped_callback : nullptr;
    MISessionRegisterThreadStoppedCallback(GDBMISession, cb);
    AsyncCheck(rtn, event);
}

MIEvent* DbgGdbMI::waitForAsyncEvent()
{
    // set the status to running
    Status = DBGSTAT_RUNNING;

    // wait for the asynch command to complete
    while (!bufferReady()) {
        MISessionProgress(GDBMISession);
    }

    // grab the pending asynch
    MIEvent* evt = nullptr;
    std::swap(PendingEvent, evt);

    // set the state back to default
    AsynchEventWaiting = 0;
    Status = DBGSTAT_STOPPED;

    return evt;
}

dbgevent_t* DbgGdbMI::waitAndProcessAsyncEvent()
{
    while (!bufferReady()) MISessionProgress(GDBMISession);
    return WaitEvent(nullptr);
}

// keep taking async events from MI until none are waiting.
// events are immediately discarded without being processed.
// returns amount of events flushed.
int DbgGdbMI::flushAsyncEvents() {
    Status = DBGSTAT_RUNNING;

    int flushed = 0;

    while (bufferReady()) {
        flushed++;
        DVPRINT("Flushed event with type %d\n", PendingEvent->type);
        MIEventFree(PendingEvent);
        PendingEvent = nullptr;
        AsynchEventWaiting = 0;

        // see if we get another
        MISessionProgress(GDBMISession);
    }

    AsynchEventWaiting = 0;
    Status = DBGSTAT_STOPPED;

    return flushed;
}

void DbgGdbMI::setAsyncEvent(MIEvent* e) {
    AsynchEventWaiting = 1;
    std::swap(PendingEvent, e);
    if (e) MIEventFree(e);
}

bool DbgGdbMI::isRunning() const { return Status == DBGSTAT_RUNNING; }
bool DbgGdbMI::isAtExit() const { return Status == DBGSTAT_EXIT; }
void DbgGdbMI::setIsInNonStopMode(bool nonStop) { isInNonStopMode = nonStop; }

void DbgGdbMI::sendCmd(MICommand* cmd) {
    SendMICommand(GDBMISession, cmd);
}
 
static dbgevent_t *NewMIErrorEvent(MICommand* cmd, free_cmd_type doFree, dbgerr err) {
    auto e = NewErrorEvent(err, "%s", MICommandResultErrorMessage(cmd));
    if (doFree == FREE_CMD)
        MICommandFree(cmd);
    return e;
}
    
