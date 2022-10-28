/*********************************************************************************
 * Initialization for gdbmi back-end
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * Copyright 2010-2022 Hewlett Packard Enterprise Development LP.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Hewlett Packard Enterprise Development LP.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *
 *********************************************************************************/
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include "dbgsrv_gdbmi.h"
#include "dbgsrv_defs.h"
#include "dbgevent.h"
#include "defs.h"
#include "alloc.h"
#include "common_tools_be.h"
#include "str.h"
#include "path.h"
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <fstream>
#include <regex>

extern std::function<void(void)> AtServerExit;

static void event_callback(MIEvent *);
static void console_callback(char *);
static void log_callback(char *);
static void target_callback(char *);
static void notification_callback(char *, MIList *);
static void Finish(int);
static dbgbp_t *get_dbgbp(MICommand *);
static bool runRocgdb(MISession*);

void SendMICommand(MISession *, MICommand *);

// Enum for supported gdb versions
enum class GdbVersion { GDB_100, CUDA_101, ROC_GDB, CUSTOM};
static GdbVersion gdb_vers = GdbVersion::GDB_100;

static std::vector<dbgbp_t*> main_entry_BP;
static std::vector<dbgbp_t*> parallel_entry_BP;

static const char* ProgMain = nullptr;

static MICommand *_MIGDBSet(const char *var, const char *val) {
    return MIGDBSet(const_cast<char *>(var), const_cast<char *>(val));
}
static MICommand *CLIBypassc(int cmd_argc, const char *cmd_argv[]) {
    return CLIBypass(cmd_argc, const_cast<char **>(cmd_argv));
}

#ifndef ALPS
// These are for sending the target output to the fe
static int WRITE_fd;
static int dbgsrv_fd;
#endif

DbgGdbMI::Initializer::Initializer(DbgGdbMI& dbg)
    : m_dbg(dbg), m_session(nullptr), m_pid(0)
{}

dbgevent_t *DbgGdbMI::Initializer::init(int *dfd, int *iofd, char **env) {
    dbgevent_t *e;
    gdb_vers = GdbVersion::GDB_100;

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

    m_session = MISessionNew();

    if (debug_flag && !getenv("GDB4HPC_DISABLE_MI_DBG")) {
        MISessionSetDebug(1);
    } else {
        MISessionSetDebug(0);
    }

    MISessionRegisterEventCallback(m_session, event_callback);
    MISessionRegisterConsoleCallback(m_session, console_callback);
    MISessionRegisterLogCallback(m_session, log_callback);
    MISessionRegisterTargetCallback(m_session, target_callback);
    MISessionRegisterNotifyCallback(m_session, notification_callback);

    // set the gdb path to use standard or gpu specific gdb
    if (custom_gdb) {
        DVPRINT("setting gdb path to %s\n", custom_gdb);
        MISessionSetGDBPath(m_session, custom_gdb);
        gdb_vers = GdbVersion::CUSTOM;
        if (custom_gdb_data) {
            MISessionSetGDBDataDirectory(m_session, custom_gdb_data);
        }
    } else if (gpu_flag) {
        // try to initialize a gpu back-end, if this fails, gdb_vers will
        // be set to GDB_100
        initGpu();
    }

    if (gdb_vers == GdbVersion::GDB_100) {
        MISessionSetGDBPath(m_session, "gdb-guard");
        setDataDir("gdb");
    }

    // Ensure PYTHONSTARTUP isn't set
    if (getenv("PYTHONSTARTUP") != NULL) {
        unsetenv("PYTHONSTARTUP");
    }

    DBEPERFLOG(SetTimerStart("DbgInitBackend: MISessionStartLocal"));

    if (MISessionStartLocal(m_session, NULL) < 0) {
        EVENT_ERROR(e, DBGERR_DEBUGGER, "Could not start the debugger.");
        return e;
    }

    DBEPERFLOG(SetTimerStopAndLog("DbgInitBackend: MISessionStartLocal"));

    // this allows dbgsrv to monitor gdb's output fd to find out when we should
    // have libmi service the output.
    *dfd = m_session->out_fd;

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

    DVPRINT("non-alps iofd is %d\n", *iofd);
#else
    // ALPS already forwards the stdout to the frontend for us.
    *iofd = -1;
    DVPRINT("alps iofd is %d\n", *iofd);
#endif

    // set in DbgGdbMI::init
    //Status = DBGSTAT_INITIALISING;

    signal(SIGTERM, Finish);
    signal(SIGHUP, Finish);
    signal(SIGINT, Finish);
    signal(SIGPIPE, Finish);
    signal(SIGQUIT, Finish);

    DBEPERFLOG(SetTimerStart("DbgInitBackend: setup_gdbmi_environment"));

    if ((e = setup_gdbmi_environment()) != NULL) { return e; }

    DBEPERFLOG(SetTimerStopAndLog("DbgInitBackend: setup_gdbmi_environment"));

    DBEPERFLOG(SetTimerStart("DbgInitBackend: setup_gdbmi_signals"));

    if ((e = setup_gdbmi_signals()) != NULL) { return e; }

    DBEPERFLOG(SetTimerStopAndLog("DbgInitBackend: setup_gdbmi_signals"));

    return (dbgevent_t *)NULL;
}

// Initialize the backend when --gpu is given as an argument
void DbgGdbMI::Initializer::initGpu()
{
    //-----------------------------------------------------------------------
    // Need to determine the version of the driver to start the proper gdb
    //-----------------------------------------------------------------------

    if (auto vers = fopen("/proc/driver/nvidia/version", "r")) {
        // We are on an nvidia node.
        char *vers_cstr = nullptr;
        int num_fields = fscanf(vers,
                            "NVRM version: NVIDIA UNIX x86_64 Kernel Module%*[ "
                            "\t]%m[0-9.]",
                            &vers_cstr);
        fclose(vers);
        if (num_fields > 0) {
            // We now have the version string, close the vers file
            DVPRINT("Detected Nvidia driver %s\n", vers_cstr);
            std::string vers_str(vers_cstr);
            int vers_int = stoi(vers_str.substr(0,3));
            //We're assuming forward compatibility from version 440 which is CTK 10.2
            if (vers_int >= 440) {
                gdb_vers = GdbVersion::CUDA_101;
            }
        }
        free(vers_cstr);
    } else if (hasAmdGpus()) {
        gdb_vers = GdbVersion::ROC_GDB;
    }

    if (gdb_vers == GdbVersion::GDB_100) {
        // Didn't find any of the gpu driver types; apparently gpu_flag is
        // a global so we need to tell the world that we didn't find the
        // correct gpu debugger. (really, we need to do better variable
        // naming to make this obvious)
        DVPRINT("Failed to determine gpu driver version.\n");
        DVPRINT("Defaulting to non-gpu mode.\n");
        gpu_flag = 0;
        
    } else if (gdb_vers == GdbVersion::CUDA_101) {
        // If GPU mode is on and we're targeting Nvidia hardware, force tmpdir
        // to /tmp. Nvidia utilizes unix pipes that are writen in TMPDIR and
        // this value *MUST* match the value set in the applications
        // environment, otherwise things will hang.

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

        // set the path and data directory
        DVPRINT("Using cuda-gdb with --cuda-use-lockfile=0\n");
        MISessionSetGDBPath(m_session, "cuda-gdb");
        setDataDir("cuda-gdb");
        MISessionAddGDBArg(m_session, "--cuda-use-lockfile=0");

    } else if (gdb_vers == GdbVersion::ROC_GDB) {
        if (runRocgdb(m_session)) {
            DVPRINT("Using rocgdb from manifest\n");
            DVPRINT("not setting data directory\n");
        } else {
            DVPRINT("rocgdb is unavailable.\n");
            DVPRINT("reverting to non-gpu mode.\n");
            gpu_flag = 0;
            gdb_vers = GdbVersion::GDB_100;
        }
    }
}

bool DbgGdbMI::Initializer::hasAmdGpus() {
    auto command = "rocminfo";

    auto path = std::unique_ptr<char, decltype(&free)>{pathFind(command, nullptr), free};
    if (!path) {
        DVPRINT("did not find %s in path\n", command);
        return false;
    }

    auto fp = std::unique_ptr<FILE, decltype(&pclose)>{popen(path.get(), "r"), pclose};
    if (!fp) {
        DVPRINT("could not execute %s\n", path.get());
        return false;
    }

    DVPRINT("opened AMD command %s\n", command);
    std::regex gpuLine{R"([dD]evice\s*[tT]ype.*[gG][pP][uU])"};
    size_t numGPUS = 0;
    size_t numLines = 0;
    size_t size = 256;

    char *line = static_cast<char*>(malloc(size));

    while (getline(&line, &size, fp.get()) > 0) {
        if (std::regex_search(line, gpuLine)) {
            ++numGPUS;
            DVPRINT("found gpu %s", line); // line should end in newline
        }
        ++numLines;
    }
    DVPRINT("looked at %zu lines got %zu AMD gpus\n", numLines, numGPUS);

    free(line);

    return (numGPUS > 0);
}

/*
** Continue to main and show current stack frame.
*/
dbgevent_t *DbgGdbMI::Initializer::startup() {
    DVPRINT("*** Dbg %s, %s\n", __FILE__, __func__);

    DVPRINT("non-mpi is %d\n", non_mpi_flag);

    // Setup entry points if we are not running with the special early
    // entry mode:
    if (!early_entry_flag) {
        if (non_mpi_flag) { if (auto e = trampolineToEntryPoint()) return e; }
        else if (auto [entrySet, e] = setEntryBPs(); e) return e;
        else if (!entrySet) DVPRINT("Could not get to entry breakpoint\n");
        else if (auto e = continueToEntryPoint()) return e;
    }

    // Initialize UPC, in case the app uses UPC - this will break gpu debugging
    if (!gpu_flag) {
        const char *cmd_argv[1] = {"upc-init"};
        MICommand* cmd = CLIBypassc(1, cmd_argv);
        SendMICommand(m_session, cmd);
        MICommandFree(cmd);
    }

    // call the showframe function to ask for the innermost frame
    return m_dbg.showFrame(1, nullptr);
}

// When the debug server starts up, some combination of gdb and WLM give control to
// the debug server before symbols have been read.   We have to shake it a bit until
// we can successfully
std::pair<bool,dbgevent_t*> DbgGdbMI::Initializer::setEntryBPs() {
    if (auto x = setEntryBPsL(); x.first || x.second) return x;

    // for cfa6k moved the nexti fall-back above the symbol-file version
    // because symbol-file leaves the symbols as an unresolved linker offset 
    // that gdb should be able to follow but doesn't.
    if (auto [evt, e] = continueToEvent(NEXTI); e) {
        return std::make_pair(false,e);
    }

    if (auto x = setEntryBPsL(); x.first || x.second) return x;
    
    // try loading symbols explicitly
    if (!m_executable.empty()) {
        DVPRINT("could not install initial breakpoints, try symbol load from %s\n",
                m_executable.c_str());
        auto cmd = MIFileSymbolFile(const_cast<char*>(m_executable.c_str()));
        SendMICommand(m_session, cmd);
        if (!MICommandResultOK(cmd)) {
            return std::make_pair(false,
                                  NewErrorEvent(DBGERR_DEBUGGER, "symbol file command failed: %s",
                                                MICommandResultErrorMessage(cmd)));
        }
        MICommandFree(cmd);
    }

    if (auto x = setEntryBPsL(); x.first || x.second) return x;

    // finally try doing a continue to get past the symbol load.  In this case we
    // have to hope the WLM will generate a signal during the process
    if (auto [evt, e] = continueToEvent(CONTINUE); e) {
        return std::make_pair(false,e);
    }

    if (auto x = setEntryBPsL(); x.first || x.second) return x;
        
    DVPRINT("did not install breakpoints\n");

    return std::make_pair(false, nullptr);
}

static void FreeFrames(MIList* f) {  MIListFree(f, (void (*)())MIFrameFree); }

// When the debug server starts up, some WLM's give control at a well defined entry point
// like WLM_Init, but some variations give control earlier in startup process.   This function
// continues the process, if necessary, until we are actually at one one of the entry
// breakpoints
dbgevent_t *DbgGdbMI::Initializer::continueToEntryPoint() {
    DVPRINT("*** Dbg %s, %s\n", __FILE__, __func__);

    dbgevent_t *e = nullptr;
    
    bool atEntry = false;
    bool finish = false;

    for (int attempt=0; !atEntry && attempt < 10; ++attempt) {
        DVPRINT("continueToEntryPoint attempt: %d\n", attempt);

        MIFrame* f = nullptr, *prevF = nullptr, *frame = nullptr;
        std::unique_ptr<MIList, decltype(&FreeFrames)> frames(nullptr,&FreeFrames);

        // Make sure that we're not inside a parallel entry point
        // get all of the frames.
        MICommand* cmd = MIStackListAllFrames();
        SendMICommand(m_session, cmd);
        if (MICommandResultOK(cmd)) {
            frames.reset(MIGetStackListFramesInfo(cmd));
            if (!frames) {
                MICommandFree(cmd);
                EVENT_ERROR(e, DBGERR_NOSTACK, "MIGetStackListFramesInfo command failed");
                return e;
            }
            MICommandFree(cmd);

            // parse over the frame list
            for (MIListSet(frames.get()); (f = (MIFrame *)MIListGet(frames.get())); prevF=f) {
                // we're processing the frame bottom up, so keep going to look for 
                // another entry breakpoint higher in the stack.  In particular,
                // we might have gotten control of the process after main had already
                // started.
                if (FindBPByFunc(main_entry_BP, f->func)) {
                    if (prevF) {
                        // finish the frame below
                        frame = prevF;
                        finish = true;
                    } else {
                        // just stopped in main
                        frame = f;
                        finish = false;
                    }
                } else if (FindBPByFunc(parallel_entry_BP, f->func)) {
                    frame = f;
                    finish = true;
                }
            }
        }

        //if we found a parallel entry point function, select that frame
        if (frame) {
            DVPRINT("continueToEntryPoint: found entry point %s finish=%s\n",
                    frame->func, finish ? "true" : "false");

            //set the frame to the parallel entry point
            auto cmd = MIStackSelectFrame(frame->level);
            SendMICommand(m_session, cmd);
            MICommandFree(cmd);
            atEntry = true;
            break;
        }
        
        // issue a continue command .. this is assuming we're pre-main or
        // pre-parallel entry point.
        DVPRINT("continueToEntryPoint: trying to continue to start breakpoint attempt = %d\n", attempt);
        if (auto [evt,e] = continueToEvent(); e) return e;
        else if (evt->type == MIEventTypeBreakpointHit) {
            DVPRINT("got breakpoint hit event\n");
            // If we hit a main breakpoint, we are done (unset finish).
            // Otherwise check parallel breakpoints
            if (dbgbp_t *bp = FindBPByID(main_entry_BP, 0, evt->bkptno)) {
                atEntry = true;
                finish = false;
            } else if ((bp = FindBPByID(parallel_entry_BP, 0, evt->bkptno))) {
                // If we hit a parallel breakpoint, we need to finish
                atEntry = finish = true;
            } else {
                DVPRINT("%s, %s Hit unknown entry breakpoint.", __FILE__, __func__);
            }
        } else if (evt->type == MIEventTypeSignal || evt->type == MIEventTypeSteppingRange) {
            // slurm will send a SIGSTOP for some reason, try to recover from
            // that
            DVPRINT("DbgStartup: Received signal %s (%s), trying to recover.\n",
                    evt->sigName, evt->sigMeaning);
            continue;  // for attempt
        } else {
            // shouldn't ever happen in theory, but log it just in case
            DVPRINT("got event type %d, ignoring...\n", evt->type);
        }

        break;
        
    } // for (attempt)

    if (!atEntry) {
        EVENT_ERROR(e, DBGERR_DEBUGGER, "Failed to hit entry breakpoint.");
        return e;
    }

    // remove all of the entry breakpoints. We are done with them.
    if ((e = delete_internal_breakpoints(main_entry_BP)) != NULL)
        return e;
    if ((e = delete_internal_breakpoints(parallel_entry_BP)) != NULL)
        return e;

    // observed on kay (pbs) the breakpoint hit notification got mixed in with a bunch of
    // library load events, so flush events instead of getting an "unknown breakpoint"
    // error
    auto flushed = m_dbg.flushAsyncEvents();
    DVPRINT("Flushed %d events.\n", flushed);

    // We want to set MPIR_debug_gate=1. This is an old MPIR standard.
    MICommand* cmd = _MIGDBSet("lang", "c");
    SendMICommand(m_session, cmd);
    if (!MICommandResultOK(cmd)) {
        MICommandFree(cmd);
        EVENT_ERROR(e, DBGERR_DEBUGGER, "Gdb set lang command failed.");
        return e;
    }
    MICommandFree(cmd);

    // This can fail if the symbol doesn't exist. We want to silently
    // fail.
    cmd = _MIGDBSet("MPIR_debug_gate=1", NULL);
    SendMICommand(m_session, cmd);
    MICommandFree(cmd);

    // set the lang back to auto.
    cmd = _MIGDBSet("lang", "auto");
    SendMICommand(m_session, cmd);
    if (!MICommandResultOK(cmd)) {
        MICommandFree(cmd);
        EVENT_ERROR(e, DBGERR_DEBUGGER, "Gdb set lang command failed.");
        return e;
    }
    MICommandFree(cmd);

    if (finish) {
        // now we want to issue the finish command to get back to main.
        DVPRINT("calling finish on initial frame\n");
        if (auto [evt, e] = continueToEvent(FINISH); e) {
            return e;
        } else if (evt->type != MIEventTypeFunctionFinished &&
                 evt->type != MIEventTypeSuspended) {
            // we didn't finish the function
            return NewErrorEvent(DBGERR_DEBUGGER,
                                 "Failed to finish the entry function.");
        }
    }

    return nullptr;
}

// Launch strategy that does not rely on the WLM launch barrier.
// When this is called, gdb4hpc has launched the program
// dbgsrv_trampoline instead of the actual application.
// dbgsrv_trampoline waits for gdb to connect to it before
// exec's the application which gives gdb4hpc a chance to
// get a stop set in main.
dbgevent_t *DbgGdbMI::Initializer::trampolineToEntryPoint() {
    DVPRINT("*** Dbg %s, %s\n", __FILE__, __func__);

    // similar to the logic in the setEntryBPs, though in this case I
    // saw the similar problem in pegasus.  The remedy is the same in that
    // nexti gives the tiny fillip to initiate the link process.
    if (auto [evt, e] = continueToEvent(NEXTI); e) {
        return e;
    }

    DVPRINT("Load symbol table for %s\n", m_executable.c_str());
    auto cmd = MIFileSymbolFile(const_cast<char*>(m_executable.c_str()));
    SendMICommand(m_session, cmd);
    if (!MICommandResultOK(cmd)) {
        return NewErrorEvent(DBGERR_DEBUGGER, "symbol file command failed: %s",
                             MICommandResultErrorMessage(cmd));
    }
    MICommandFree(cmd);

    // dbgsrv_trampoline calls connectedHook once a second.  Since we don't
    // know what state we're starting in, and catch exec is also a nebulous
    // state we just catch the bus at connectHook.
    DVPRINT("set breakpoint in connectHook\n");
    cmd = MIBreakInsert(0, 0, 0, NULL, 0, "connectHook", 0);
    SendMICommand(m_session, cmd);
    if (MICommandResultOK(cmd)) {
        std::vector<int> connectBPs;
        if (auto bpts = MIBreakpointGetBreakInsertInfo(cmd)) {
            MIListSet(bpts);
            if (auto bpt = (MIBreakpoint *)MIListGet(bpts))
                connectBPs.push_back(bpt->number);
        }
        MICommandFree(cmd);

        auto [evt,e] = continueToEvent();
        if (e) {
            DVPRINT("continue command failed \"%s\"\n", DbgEventErrorStr(e));
            FreeEvent(e);
        }
        
        DVPRINT("delete the connectHookBP\n");
        for (auto bp : connectBPs) {
            cmd = MIBreakDelete(1, &bp);
            SendMICommand(m_session, cmd);
            if (!MICommandResultOK(cmd)) DVPRINT("Delete breakpoint failed\n");
            MICommandFree(cmd);
        }
    } else {
        // The odds of the remaining steps working if we can't set a breakpoint
        // is a little remote, but we'll give them a chance anyway
        MICommandFree(cmd);
    }

    DVPRINT("Get the real application\n");
    cmd = MIDataEvaluateExpression("theApplication");
    SendMICommand(m_session, cmd);
    if (!MICommandResultOK(cmd)) {
        DVPRINT("Getting real name of application failed\n");
        return NewErrorEvent(DBGERR_DEBUGGER, "Get application path failed:%s",
                             MICommandResultErrorMessage(cmd));
    }

    std::string application;
    if (auto app = MIGetDataEvaluateExpressionInfo(cmd)) {
        if (const auto beg = strchr(app,'"')) {
            if (const auto end = strchr(beg+1, '"')) {
                application.assign(beg+1, end-beg-1);
            }
        }
    }
    MICommandFree(cmd);

    if (application.empty()) {
        return NewErrorEvent(DBGERR_DEBUGGER, "Could not get application path");
    }

    DVPRINT("Got application %s\n", application.c_str());



    DVPRINT("Set connected in %s\n", m_executable.c_str());
    // Tell dbgsrv_trampoline that we have connected
    cmd = MIDataEvaluateExpression("dbgsrv_trampoline_connected=1");
    SendMICommand(m_session, cmd);
    if (!MICommandResultOK(cmd)) {
        DVPRINT("Setting value to connected failed\n");
        return NewErrorEvent(DBGERR_DEBUGGER, "Setting value to connected failed:%s",
                             MICommandResultErrorMessage(cmd));
    }
    MICommandFree(cmd);

    const char* catchExec[] = { "catch", "exec" };
    cmd = CLIBypassc(2, catchExec);
    SendMICommand(m_session, cmd);
    if (!MICommandResultOK(cmd)) {
        DVPRINT("Setting value to connected failed\n");
        return NewErrorEvent(DBGERR_DEBUGGER, "Setting value to connected failed:%s",
                             MICommandResultErrorMessage(cmd));
    }
    MICommandFree(cmd);

    
    // Like continueToEntryPoint, the road gets a little bumpy here and
    // we might get some signals before the exec actually happens.
    // I had tried out setting a "catch exec" here, but that *seems* to be
    // unnecessary.  If we just let it go, gdb will naturally recreate the
    // entry breakpoints after the exec.
    int numSigs = 0, maxNumSigs = 5;
    for (int numSigs=0; numSigs<5; ++numSigs) {
        if (auto [evt,e] = continueToEvent(); e) {
            DVPRINT("continue command failed \"%s\"\n"
                    "- in this case I think because we're at the exec\n",
                    DbgEventErrorStr(e));
            FreeEvent(e);
            break;
        } else if (evt->type != MIEventTypeSignal) {
            DVPRINT("bounced to a breakpoint\n");
            break;
        }
    }

    if (numSigs == maxNumSigs) {
        return NewErrorEvent(DBGERR_DEBUGGER, "could not launch in non-MPI mode");
    }

    DVPRINT("delete catchpoint - expected to be 2\n");
    int catchBP[1] = { 2 };
    cmd = MIBreakDelete(1, catchBP);
    SendMICommand(m_session, cmd);
    if (!MICommandResultOK(cmd)) {
        DVPRINT("Delete breakpoint failed\n");
    }
    MICommandFree(cmd);

    DVPRINT("Setting executable to \"%s\"\n", application.c_str());
    m_executable = application;

    cmd = MIFileSymbolFile(const_cast<char*>(m_executable.c_str()));
    SendMICommand(m_session, cmd);
    if (!MICommandResultOK(cmd)) {
        DVPRINT("Reading symbols failed: %s\n",
                MICommandResultErrorMessage(cmd));
    }
    MICommandFree(cmd);

    DVPRINT("set entry bps\n");
    if (auto [ entrySet, e] = setEntryBPs(); e) return e;
    else if (!entrySet) {
        DVPRINT("Could not set entry breakpoint\n");
    }

    auto e = continueToEntryPoint();
    if (!e) m_ignoreSigstop = m_isNonMPI = true;
    return e;
}


/* GDBMI callback functions */
static void event_callback(MIEvent *e) {
    DVPRINT("*** Dbg %s, %s, e->type %d\n", __FILE__, __func__, e->type);
    DbgGdbMI::setAsyncEvent(e);
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
** Signal handler.
*/
static void Finish(int sig) {
    DVPRINT("*** Dbg %s, %s\n", __FILE__, __func__);
    Shutdown++;
}

static const std::array<std::string, 21> parallel_entry_pt_strs
{
    "main",
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

static const std::array<std::string, 6> exit_pt_strs =
{
    "exit",
    "_exit",
    "abort",
    "MPIR_Errors_are_fatal",
    "MPID_Abort",
    "ompi_mpi_errors_are_fatal_comm_handler"
};

bool DbgGdbMI::Initializer::entryBPsAreSet() {
    return !main_entry_BP.empty() || !parallel_entry_BP.empty();
}

std::pair<bool, dbgevent_t *> DbgGdbMI::Initializer::setEntryBPsL() {
    DVPRINT("*** Dbg %s, %s\n", __FILE__, __func__);

    // this function may be called more than once
    if (entryBPsAreSet()) {
        return std::make_pair(true, nullptr);  // already set
    }

    // set the lang to c. We always want to do this!!
    auto cmd = _MIGDBSet("lang", "c");
    SendMICommand(m_session, cmd);
    if (!MICommandResultOK(cmd)) {
        MICommandFree(cmd);
        return std::make_pair(false,
                              NewErrorEvent(DBGERR_DEBUGGER, "Gdb set lang command failed."));
    }
    MICommandFree(cmd);

    // set up the parallel entry breakpoints
    for (auto&& str: parallel_entry_pt_strs) {
        cmd = MIBreakInsert(0, 0, 0, NULL, 0, str.c_str(), 0);
        SendMICommand(m_session, cmd);
        // Note we only want to insert the breakpoint into our internal list if the
        // command succeeded. We expect many of these to fail.
        if (MICommandResultOK(cmd)) {
            if (auto bp_res = get_dbgbp(cmd)) {
                auto& bps = (str == "main" ? main_entry_BP : parallel_entry_BP);
                bps.push_back(bp_res);
            }
        }
        MICommandFree(cmd);
    }

    // set up the exit breakpoints
    if (m_exitBP.empty()) {
        // install the breakpoints as potentially pending since the exit point
        // might be in a shared library
        for (auto&& str : exit_pt_strs) {
            cmd = MIBreakInsert(0, 0, 1, NULL, 0, str.c_str(), 0);
            SendMICommand(m_session, cmd);
            // Note we only want to insert the breakpoint into our internal list if the
            // command succeeded. We expect many of these to fail.
            if (MICommandResultOK(cmd)) {
                if (auto bp_res = get_dbgbp(cmd)) {
                    bp_res->bp_type = DBGBP_EXIT_TYPE;
                    m_exitBP.push_back(bp_res);
                }
            }
            MICommandFree(cmd);
        }
    }

    // set the lang back to auto. We always want to do this!!
    cmd = _MIGDBSet("lang", "auto");
    SendMICommand(m_session, cmd);
    if (!MICommandResultOK(cmd)) {
        MICommandFree(cmd);
        return std::make_pair(false,
                              NewErrorEvent(DBGERR_DEBUGGER, "Gdb set lang command failed."));
    }
    MICommandFree(cmd);

    auto areSet = entryBPsAreSet();
    DVPRINT("setting initial breakpoints %s\n", areSet ? "succeeded\n" : "failed\n");
    return std::make_pair(areSet, nullptr);
}

/*
** Initialize debugger, make sure we are using
** the correct version, etc.
*/
dbgevent_t *DbgGdbMI::Initializer::initDbg(const char *prog, const char *args) {
    DVPRINT("*** Dbg %s, %s\n", __FILE__, __func__);
    dbgevent_t *e;
    MICommand *cmd;
    MIList *bpts;
    MIBreakpoint *bpt;

    CLIInfoProcInfo *procInfo;
    int breakpointNumberAtMain = 0;

    cmd = MICommandNew("-file-exec-and-symbols", MIResultRecordDONE);
    MICommandAddOption(cmd, prog, NULL);
    SendMICommand(m_session, cmd);
    MICommandFree(cmd);

    if (args != NULL) {
        e = m_dbg.setArgs(args);

        if (e->ev_event != DBGEV_OK) return e;

        FreeEvent(e);
    }

    /*
    ** Check for fortran.
    */
    cmd = MIBreakInsert(0, 0, 0, NULL, 0, "MAIN__", 0);
    SendMICommand(m_session, cmd);
    if (MICommandResultOK(cmd)) {
        /*
        ** We are Fortran
        */
        ProgMain = "MAIN__";
    } else {
        MICommandFree(cmd);
        cmd = MIBreakInsert(0, 0, 0, NULL, 0, "main", 0);
        SendMICommand(m_session, cmd);
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

    if (m_runArgs) {
        cmd = MICommandNew("-exec-arguments", MIResultRecordDONE);
        MICommandAddOption(cmd, m_runArgs, NULL);
        SendMICommand(m_session, cmd);
        if (!MICommandResultOK(cmd)) {
            MICommandFree(cmd);
            EVENT_ERROR(e, DBGERR_DEBUGGER, "Could not set program arguments.");
            return e;
        }
        MICommandFree(cmd);
    }

    cmd = MIExecRun();
    SendMICommand(m_session, cmd);
    if (!MICommandResultOK(cmd)) {
        MICommandFree(cmd);
        EVENT_ERROR(e, DBGERR_DEBUGGER, "Could not run program.");
        return e;
    }
    MICommandFree(cmd);

    cmd = MIBreakDelete(1, &breakpointNumberAtMain);
    SendMICommand(m_session, cmd);
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
    SendMICommand(m_session, cmd);
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
    m_pid = procInfo->pid;
    CLIInfoProcInfoFree(procInfo);
    MICommandFree(cmd);

    DVPRINT("InitDbg(): ProgMain=%s Pid=%d.\n", ProgMain, m_pid);
    if (prog) m_executable = prog;
    return NewEvent(DBGEV_OK);
}


/*
** Attach debugger. Added specifically for Cray XT / ALPS, but it works
** for other platforms as well.
*/
dbgevent_t *DbgGdbMI::Initializer::initDbgAttach(int pid, const char *prog) {
    DVPRINT("*** Dbg %s, %s\n", __FILE__, __func__);
    dbgevent_t *e;
    MICommand *cmd;
    CLIInfoProcInfo *procInfo;

    cmd = MITargetAttach(pid);

    DBEPERFLOG(SetTimerStart("DbgAttach: -target-attach"));
    SendMICommand(m_session, cmd);
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
    e = m_dbg.waitAndProcessAsyncEvent();

    DVPRINT("InitDbgAttach: Event ready!\n");
    FreeEvent(e);


    /*
    ** Find out pid.
    */
    if (gdb_vers != GdbVersion::CUDA_101) {
        // The info proc call here breaks cuda-gdb.   It will start reporting
        // "Not supported on this target." errors.   I'm not sure about the
        // real need for it on other debuggers, but I'll leave it in.
        cmd = CLIInfoProc();
        SendMICommand(m_session, cmd);
        if (MICommandResultOK(cmd) && (procInfo = CLIGetInfoProcInfo(cmd))) {
            m_pid = procInfo->pid;
            CLIInfoProcInfoFree(procInfo);
        } else {
            DVPRINT("info proc failed - assuming we've attached\n");
            m_pid = pid;
        }
        MICommandFree(cmd);
    } else {
        DVPRINT("NOT CALLING INFO PROC\n");
        m_pid = pid;
    }

    DVPRINT("InitDbgAttach(): Obtained pid: %d.\n", m_pid);
    if (prog) m_executable = prog;
    return NewEvent(DBGEV_OK);
}

dbgevent_t *DbgGdbMI::Initializer::setup_gdbmi_environment() {
    DVPRINT("*** Dbg %s, %s\n", __FILE__, __func__);
    dbgevent_t *e;
    MICommand *cmd;

    cmd = _MIGDBSet("confirm", "off");
    SendMICommand(m_session, cmd);
    if (!MICommandResultOK(cmd)) {
        MICommandFree(cmd);
        EVENT_ERROR(e, DBGERR_DEBUGGER, "Gdb set confirm command failed.");
        return e;
    }
    MICommandFree(cmd);

    // turn history off
    cmd = _MIGDBSet("history size", "0");
    SendMICommand(m_session, cmd);
    if (!MICommandResultOK(cmd)) {
        MICommandFree(cmd);
        EVENT_ERROR(e, DBGERR_DEBUGGER, "Gdb set history size command failed.");
        return e;
    }
    MICommandFree(cmd);

    // do not open source files
    setSourceOpen(false);

    // we need gdb to set print elements and print repeats to 0 so
    // that when we resolve the data it gives us the entire contents
    // of the data type.
    cmd = _MIGDBSet("print elements", "0");
    SendMICommand(m_session, cmd);
    if (!MICommandResultOK(cmd)) {
        MICommandFree(cmd);
        EVENT_ERROR(e, DBGERR_DEBUGGER,
                    "Gdb set print elements command failed.");
        return e;
    }
    MICommandFree(cmd);

    cmd = _MIGDBSet("print repeats", "0");
    SendMICommand(m_session, cmd);
    if (!MICommandResultOK(cmd)) {
        MICommandFree(cmd);
        EVENT_ERROR(e, DBGERR_DEBUGGER,
                    "Gdb set print repeats command failed.");
        return e;
    }
    MICommandFree(cmd);

    // stop printing strings on the first null character
    cmd = _MIGDBSet("print null-stop", "on");
    SendMICommand(m_session, cmd);
    if (!MICommandResultOK(cmd)) {
        MICommandFree(cmd);
        EVENT_ERROR(e, DBGERR_DEBUGGER,
                    "Gdb set print null-stop command failed.");
        return e;
    }
    MICommandFree(cmd);

    // demangle c++ names
    cmd = _MIGDBSet("print demangle", "on");
    SendMICommand(m_session, cmd);
    if (!MICommandResultOK(cmd)) {
        MICommandFree(cmd);
        EVENT_ERROR(e, DBGERR_DEBUGGER,
                    "Gdb set print demangle command failed.");
        return e;
    }
    MICommandFree(cmd);

    cmd = _MIGDBSet("print asm-demangle", "on");
    SendMICommand(m_session, cmd);
    if (!MICommandResultOK(cmd)) {
        MICommandFree(cmd);
        EVENT_ERROR(e, DBGERR_DEBUGGER,
                    "Gdb set print asm-demangle command failed.");
        return e;
    }
    MICommandFree(cmd);

    // print derived types from the vtable
    cmd = _MIGDBSet("print object", "on");
    SendMICommand(m_session, cmd);
    if (!MICommandResultOK(cmd)) {
        MICommandFree(cmd);
        EVENT_ERROR(e, DBGERR_DEBUGGER, "Gdb set print object command failed.");
        return e;
    }
    MICommandFree(cmd);

    if (gdb_vers != GdbVersion::ROC_GDB && gdb_vers != GdbVersion::CUSTOM) {
        // limit strings to 200 characters
        cmd = _MIGDBSet("print characters", "200");
        SendMICommand(m_session, cmd);
        if (!MICommandResultOK(cmd)) {
            // support depends on the version of the debugger, but we don't
            // need to quit if the setting isn't available.
            DVPRINT("Gdb set print characters command failed.\n");
        }
        MICommandFree(cmd);
    }

    // don't allow pending breakpoints
    cmd = _MIGDBSet("breakpoint pending", "off");
    SendMICommand(m_session, cmd);
    if (!MICommandResultOK(cmd)) {
        MICommandFree(cmd);
        EVENT_ERROR(e, DBGERR_DEBUGGER,
                    "Gdb set breakpoint pending command failed.");
        return e;
    }
    MICommandFree(cmd);

    // limit backtrace size to 1000
    cmd = _MIGDBSet("backtrace limit", "1000");
    SendMICommand(m_session, cmd);
    if (!MICommandResultOK(cmd)) {
        MICommandFree(cmd);
        EVENT_ERROR(e, DBGERR_DEBUGGER,
                    "Gdb set backtrace limit command failed.");
        return e;
    }
    MICommandFree(cmd);

    // unwind signals triggered by gdb
    cmd = _MIGDBSet("unwindonsignal", "on");
    SendMICommand(m_session, cmd);
    if (!MICommandResultOK(cmd)) {
        MICommandFree(cmd);
        EVENT_ERROR(e, DBGERR_DEBUGGER,
                    "Gdb set unwindonsignal command failed.");
        return e;
    }
    MICommandFree(cmd);

    // set can-use-hw-watchpoints to 1
    cmd = _MIGDBSet("can-use-hw-watchpoints", "1");
    SendMICommand(m_session, cmd);
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
        SendMICommand(m_session, cmd);
        if (!MICommandResultOK(cmd)) {
            MICommandFree(cmd);
            EVENT_ERROR(e, DBGERR_DEBUGGER,
                        "Gdb set solib-absolute-prefix command failed.");
            return e;
        }
        MICommandFree(cmd);
    }

    if (gdb_vers == GdbVersion::CUDA_101) {
        // If running with gpu's, turn upc off. If we don't do this, things will
        // break, but it is not supported on rocgdb, so it's just a ddt-cuda-gdb thing.
        // Other version of cuda-gdb don't support it so don't quit if the call fails
        if (gpu_flag) {
            cmd = _MIGDBSet("upcsingle", "1");
            SendMICommand(m_session, cmd);
            if (!MICommandResultOK(cmd)) {
                DVPRINT("Gdb set upcsingle command failed.\n");
            }
            MICommandFree(cmd);
        }
    }

    // Set maximum variable size to unlimited to allow gdb to print large values
    cmd = _MIGDBSet("max-value-size", "unlimited");
    SendMICommand(m_session, cmd);
    MICommandFree(cmd);

    // manually set up pretty printing for relocated installs
    setup_pretty_printing();

    // set non-stop mode -- disabled for cuda-gdb because non-stop is the default,
    // and cuda-gdb doesn't handle it well
    auto isInNonStopMode = false;
    if (non_stop_flag && gdb_vers != GdbVersion::CUDA_101 &&
        (!custom_gdb || !strstr(custom_gdb,"cuda")) ) {
        cmd = _MIGDBSet("non-stop", "on");
        SendMICommand(m_session, cmd);
        if (!MICommandResultOK(cmd)) {
            DVPRINT("setting non-stop mode failed\n");
        }
        MICommandFree(cmd);

        // we need to also set mi-async mode when non-stop mode
        // is on; otherwise commands can hang waiting for all the
        // threads to stop.
        cmd = _MIGDBSet("mi-async", "on");
        SendMICommand(m_session, cmd);
        if (!MICommandResultOK(cmd)) {
            DVPRINT("setting mi-asycn mode failed\n");
        } else {
            isInNonStopMode = true;
        }
            
        MICommandFree(cmd);
    }
    m_dbg.setIsInNonStopMode(isInNonStopMode);

    return NULL;
}

dbgevent_t *DbgGdbMI::Initializer::setup_gdbmi_signals() {
    DVPRINT("*** Dbg %s, %s\n", __FILE__, __func__);
    
    auto nostops = std::vector<std::string>{"SIG64", "SIG63", "SIG39","SIG33", "SIG32",
                                            "SIGUSR1", "SIGUSR2", "SIGCONT", "SIGTSTP",
                                            "SIGCHLD","SIGPROF", "SIGALRM", "SIGVTALRM",
                                            "SIGURG" };

    for ( auto&& sig : nostops ) {
        auto str = sig + " nostop noprint pass";
        auto cmd = CLIHandle(str.c_str());
        SendMICommand(m_session, cmd);
        if (!MICommandResultOK(cmd)) {
            MICommandFree(cmd);
            return NewErrorEvent(DBGERR_DEBUGGER, "%s command failed.", str.c_str());
        }
        MICommandFree(cmd);
    }

    // We use this signal to interrupt the application. SIGINT is blocked and
    // won't work (I can't figure out why...).
    auto cmd = CLIHandle("SIGSTOP stop print nopass");
    SendMICommand(m_session, cmd);
    if (!MICommandResultOK(cmd)) {
        MICommandFree(cmd);
        return NewErrorEvent(DBGERR_DEBUGGER, "Handle SIGSTOP command failed.");
    }
    MICommandFree(cmd);

    return NULL;
}

void DbgGdbMI::Initializer::setup_pretty_printing() {
    DVPRINT("*** Dbg %s, %s\n", __FILE__, __func__);
    char* tmpDir = cti_be_getTmpDir();
    if (!tmpDir) {
        DVPRINT("could not get tmpDir");
        return;
    }

    /*
       -- holding off on adding this since the paths are different on centos and 
          seles --
    const char* safe_path_args[] = { "add-auto-load-safe-path", "/cm/shared/opt" };
    auto cmd = CLIBypassc(2, safe_path_args);
    SendMICommand(m_session, cmd);
    if (!MICommandResultOK(cmd)) {
        DVPRINT("Settting auto-load-save-path\n");
    }
    MICommandFree(cmd);
    cmd = nullptr;
    */

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
    auto cmd = CLIBypassc(2, cmd_args);
    SendMICommand(m_session, cmd);
    if (!MICommandResultOK(cmd)) {
        DVPRINT("Loading pretty printers failed\n");
    }
    remove(scriptFile.c_str());
    MICommandFree(cmd);
}

// Turn the source open mode on or off.
// Turning off source file opening is a performance optimization available in
// some versions of gdb.
void DbgGdbMI::Initializer::setSourceOpen(bool doOpen) {
    // the function does not exist in newer versions of gdb
    // in the future, I'd expect a more sophisticated way to handle gdb versioning..
    if (gdb_vers == GdbVersion::ROC_GDB || gdb_vers == GdbVersion::CUSTOM) return;

    const char* mode = (doOpen ? "on" : "off");
    auto cmd = _MIGDBSet("source open", mode);
    SendMICommand(m_session, cmd);

    if (!MICommandResultOK(cmd)) {
        // it's not critical that this call work.
        DVPRINT("set source open %s mode failed\n", mode);
    }
    MICommandFree(cmd);
}

dbgevent_t *DbgGdbMI::Initializer::delete_internal_breakpoints(std::vector<dbgbp_t*>& bplist) {
    DVPRINT("*** Dbg %s, %s\n", __FILE__, __func__);

    dbgevent_t *e = nullptr;

    while (!bplist.empty()) {
        auto bp = bplist.back();
        bplist.pop_back();
        
        // send the delete command
        auto cmd = MIBreakDelete(1, &bp->bp_be_id);
        SendMICommand(m_session, cmd);
        if (!MICommandResultOK(cmd)) {
            if (!e) e = NewErrorEvent(DBGERR_NOBP, "Delete internal breakpoint failed.");
        }
        MICommandFree(cmd);

        // remove the breakpoint from the internal list and delete it
        delete bp;
    }

    return e;
}

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

    bp = new dbgbp_t(0, bpt->number);

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

    DVPRINT("get_dbgbp: bp_be_id is %d\n", bp->bp_be_id);
    //DVPRINT("%s: DumpingBP:", __func__);
    //DumpBP(bp);

    MIListFree(bpts, (void (*)())MIBreakpointFree);

    return bp;
}

/*
** Set run arguments.
*/
dbgevent_t *DbgGdbMI::Initializer::setArgs(const char *arg) {
    if (arg && *arg != '\0') {
        free(m_runArgs);
        m_runArgs = strdup(arg);
    }

    return NewEvent(DBGEV_OK);
}

/*
** Quit debugger.
*/
dbgevent_t *DbgGdbMI::Initializer::quit(bool detach) {
    DVPRINT("*** Dbg %s, %s\n", __FILE__, __func__);
    /*
    ** Interrupt the process if it is running. This will generate an async event.
    ** We want to grab it and ignore it.
    */

    if (m_dbg.isRunning()) {
        DVPRINT("DbgQuit: Stopping program.\n");

        // gdbmi will catch this, generating an async event.
        // gdbmi will stop the inferior in response to the event,
        // but the inferior will not actually get the signal yet.
        sendAndCatchSigstop();

        if (!m_session || m_session->out_fd == -1) {
            DVPRINT("gdb is probably already dead, not waiting for an event\n");
        } else {
            auto e = m_dbg.waitAndProcessAsyncEvent();
            FreeEvent(e);
        }
    }

    // Even if we are detaching, we stop the process so that it will not finish
    // while the network cleans itself up, resulting in the WLM killing the whole
    // network before the client knows.
    // We let the client kill (or continue in the detach case) the process
    // via CTI once network cleanup is done.
    if (!sendAndDeliverSigstop()) {
        DVPRINT("WARNING: failed to send a SIGSTOP to the inferior for shutdown.\n");
    }

    return NewEvent(DBGEV_OK);
}

/*
** Interrupt the debugger.
*/
dbgevent_t *DbgGdbMI::Initializer::interrupt(bool all) {
    DVPRINT("*** Dbg %s, %s(bool all %s)\n", __FILE__, __func__, all?"true":"false");
    dbgevent_t *e;
    dbgevent_t *async_e;

    const char* threads = nullptr;  // thread focus is unimplemented

    if (m_dbg.isRunning()) {
        DVPRINT("RUNNING - sending SIGSTOP\n");
        if (sendAndCatchSigstop()) {
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

        // wait for the async command to complete
        async_e = m_dbg.waitAndProcessAsyncEvent();

        if (!all) {
            e = NewEvent(DBGEV_HALT);
            e->ev_loc = (dbgloc_t *)NULL;

            if (async_e->ev_sig_frame != NULL) {
                const auto &frameLoc = async_e->ev_sig_frame->getLoc();
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

    if (all || !m_dbg.isRunning()) {
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
        SendMICommand(m_session, cmd);
        if (!MICommandResultOK(cmd)) {
            DVPRINT("MI -exec-interrupt failed\n");
        }
        MICommandFree(cmd);
    }


    // If we are already stopped, we will reuse DbgGDBMIShowFrame to get the
    // bottommost frame
    async_e = m_dbg.showFrame(1, threads);

    e = NewEvent(DBGEV_HALT);
    e->ev_loc = (dbgloc_t *)NULL;

    if (async_e->ev_frame != NULL && async_e->ev_event == DBGEV_FRAME) {
        const auto &frameLoc = async_e->ev_frame->getLoc();
        e->ev_loc = (dbgloc_t *)Alloc(sizeof(dbgloc_t));
        e->ev_loc->loc_file = StrdupOpt(frameLoc.loc_file);
        e->ev_loc->loc_func = StrdupOpt(frameLoc.loc_func);
        e->ev_loc->loc_addr = nullptr;  // We don't care about the address
        e->ev_loc->loc_line = frameLoc.loc_line;
    }

    FreeEvent(async_e);

    return e;
}

void DbgGdbMI::Initializer::setDataDir(const char* gdbType) {
    if (auto baseDir = getenv(GUARD_INST_DIRECTORY)) {
        auto dataDir = std::string{baseDir} + "/share/" + gdbType;
        MISessionSetGDBDataDirectory(m_session, dataDir.c_str());
    }
}

// Get rocgdb out of the manifest and setup MI to call it.   Return false if
// it is not available.
static bool runRocgdb(MISession* session) {

    if (auto binDir = cti_be_getBinDir()) {
        auto rocgdb = std::string{binDir} + "/rocgdb";
        free(binDir);

        if (access(rocgdb.c_str(), X_OK) == 0) {
            DVPRINT("found rocgdb at %s\n", rocgdb.c_str());
            MISessionSetGDBPath(session, rocgdb.c_str());
            return true;
        } else {
            DVPRINT("could not find rocgdb in %s\n", binDir);
        }
    }

    return false;
}

// Execute a non-synchronous command and wait for an event
// return the MIEvent and a dbgevent if there is an error.
std::pair<DbgGdbMI::Initializer::MIEventPtr,dbgevent_t*>
DbgGdbMI::Initializer::continueToEvent(ContinueType ctype) {
    // it's possible that another stop signal has happened while we were
    // already stopped. we flush those old events before listening again
    // so our call to waitForAsyncEvent doesn't pick up a stale event
    m_dbg.flushAsyncEvents();

    // send the command
    MICommand* cmd = nullptr;
    switch (ctype) {
    case CONTINUE: cmd = MIExecContinueAll(); break;
    case NEXTI: cmd = MICommandNew("-exec-next-instruction", MIResultRecordRUNNING); break;
    case FINISH: cmd = MIExecFinish(); break;
    }

    if (!cmd) {
        auto e = NewErrorEvent(DBGERR_DEBUGGER,"command creating failed");
        return std::make_pair(MIEventPtr{nullptr,MIEventFree},e);
    }
    
    SendMICommand(m_session, cmd);

    if (!MICommandResultOK(cmd)) {
        auto e = NewErrorEvent(DBGERR_DEBUGGER, "%s", MICommandResultErrorMessage(cmd));
        MICommandFree(cmd);
        return std::make_pair(MIEventPtr{nullptr,MIEventFree},e);
    }
    MICommandFree(cmd);

    DVPRINT("wait for aysnc event\n");
    MIEvent *evt = m_dbg.waitForAsyncEvent();
    DVPRINT("got event of type %d\n", evt->type);

    return std::make_pair(MIEventPtr{evt,MIEventFree}, nullptr);
}

// This little wart of logic was prompted by cfa6k that needs to run mpi programs
// with --non-mpi probably because the mpi library is stripped so the usual
// start-up barrier drops gdb into a stop with no readable stack.
//
// So --non-mpi avoids the usual MPI breakpoints, but MPI_Init is still generating
// a SIGSTOP at a point with no usuable stack.  So we skip over it and everything's
// copacetic.
// Except that gdb4hpc uses SIGSTOP to halt the process.   But those calls are all
// inside this class, so we just rest the flag before call to 'kill' so the
// gdb4hpc server handles the signal like usual.
//
// This strategy could be used all the time, but to limit exposure, we'll just
// use it in non-mpi mode for now.  (a lot of the complexity of continueToEntryPoint
// is related to spurious stop signals)
bool DbgGdbMI::Initializer::ignoreSigstop() {
    if (!m_isNonMPI) return false;
    else if (m_ignoreSigstop) return true;
    else { m_ignoreSigstop = true; return false; }
}

int DbgGdbMI::Initializer::sendAndCatchSigstop() {
    m_ignoreSigstop = false;
    return ::kill(m_pid, SIGSTOP);
}

[[nodiscard]] bool DbgGdbMI::Initializer::sendAndDeliverSigstop() {
    DVPRINT("Sending a SIGSTOP directly to the inferior.\n");

    // "SignalInfo" is a misnomer. This function is the equivalent of the command
    // "signal SIGSTOP", which just sends a SIGSTOP without gathering any "info"
    MICommand* cmd = CLISignalInfo("SIGSTOP");
    if (!cmd) return false;

    // XXX: CLISignalInfo will set the cmd up expecting an MIResultRecordDONE (^done).
    // The signal command actually generates a MIResultRecordRUNNING (^running),
    // even after a SIGSTOP.
    cmd->expected_class = MIResultRecordRUNNING;

    SendMICommand(m_session, cmd);
    auto ok = static_cast<bool>(MICommandResultOK(cmd));

    MICommandFree(cmd);

    return ok;
}
