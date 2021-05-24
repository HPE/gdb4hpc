/*********************************************************************************\
 * Initialization for gdbmi back-end
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * Copyright 2010-2020 Hewlett Packard Enterprise Development LP.
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
enum class GdbVersion { GDB_81, CUDA_101, ROC_GDB, CUSTOM};
static GdbVersion gdb_vers = GdbVersion::GDB_81;

static list_t *main_entry_BP;
static list_t *parallel_entry_BP;

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
    : m_dbg(dbg), m_session(nullptr), m_exitBP(nullptr), m_pid(0)
{}

dbgevent_t *DbgGdbMI::Initializer::init(int *dfd, int *iofd, char **env) {
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
        // be set to GdbVersion::GDB_81
        initGpu();
    }

    if (gdb_vers == GdbVersion::GDB_81) {
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
#else
    // ALPS already forwards the stdout to the frontend for us.
    *iofd = -1;
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
        // we may be on a AMDnode.
        // The dkms check seems a little suspect, but the worst side effect might
        // just be that we run with rocgdb on the backend and lose some fortran
        // support.
        if (auto fp = popen("/usr/sbin/dkms --version", "r")) {
            DVPRINT("popened /usr/sbin/dkms\n");
            const size_t size = 1024;
            char linebuf[size];
            while ( fgets(linebuf, size, fp)) {
                if (strncmp(linebuf, "dkms:2.8", 8) == 0) {
                    DVPRINT("Detected AMD driver %s\n", linebuf);
                    gdb_vers = GdbVersion::ROC_GDB;
                    break;
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
        DVPRINT("Using cuda101-gdb-guard\n");
        MISessionSetGDBPath(m_session, "cuda101-gdb-guard");
        setDataDir("cuda-gdb");

    } else if (gdb_vers == GdbVersion::ROC_GDB) {
        if (runRocgdb(m_session)) {
            DVPRINT("Using rocgdb from manifest\n");
            DVPRINT("not setting data directory\n");
        } else {
            DVPRINT("rocgdb is unavailable.\n");
            DVPRINT("reverting to non-gpu mode.\n");
            gpu_flag = 0;
            gdb_vers = GdbVersion::GDB_81;
        }
    }
}

/*
** Continue to main and show current stack frame.
*/
dbgevent_t *DbgGdbMI::Initializer::startup() {
    DVPRINT("*** Dbg %s, %s\n", __FILE__, __func__);

    // Setup entry points if we are not running with the special early
    // entry mode:
    if (!early_entry_flag) {
        if (dbgevent_t *e = continueToEntryPoint()) return e;
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

// When the debug server starts up, some WLM's give control at a well defined entry point
// like WLM_Init, but some variations give control earlier in startup process.   This function
// continues the process, if necessary, until we are actually in the user program.
// \return nullptr on success
dbgevent_t *DbgGdbMI::Initializer::continueToEntryPoint() {
    DVPRINT("*** Dbg %s, %s\n", __FILE__, __func__);

    dbgevent_t *e = nullptr;
    
    // setup main entry points
    if ((e = setup_gdbmi_main_entry_point()) != NULL) return e;
    // setup parallel entry points
    if ((e = setup_gdbmi_entry_points()) != NULL) return e;
    
    // we only want to issue a continue if one of the breakpoint lists is
    // not empty.
    if (EmptyList(parallel_entry_BP) && EmptyList(main_entry_BP)) {
        return nullptr;
    }

    bool finish = false;
    bool atEntry = false;

    for (int attempt=0; !atEntry && attempt < 10; ++attempt) {
        // Make sure that we're not inside a parallel entry point

        //get all of the frames.
        MICommand* cmd = MIStackListAllFrames();
        SendMICommand(m_session, cmd);
        if (!MICommandResultOK(cmd)) {
            MICommandFree(cmd);
            EVENT_ERROR(e, DBGERR_DEBUGGER, "MIStackListAllFrames command failed.");
            return e;
        }

        MIList *frames = MIGetStackListFramesInfo(cmd);
        if (!frames) {
            MICommandFree(cmd);
            EVENT_ERROR(e, DBGERR_NOSTACK, "MIGetStackListFramesInfo command failed");
            return e;
        }
        MICommandFree(cmd);

        // parse over the frame list
        MIFrame* frame = nullptr;
        for (MIListSet(frames); (frame = (MIFrame *)MIListGet(frames)) != NULL;) {
            if (FindBPByFunc(parallel_entry_BP, frame->func) != NULL) {
                DVPRINT("%s: found frame in parallel_entry_BP, frame->level "
                        "%d, frame->func %s\n", __func__, frame->level, frame->func);
                break;
            }
        }
        // free the MIFrame list
        MIListFree(frames, (void (*)())MIFrameFree);

        //if we found a parallel entry point function, select that frame
        if (frame) {
            //set the frame to the parallel entry point
            cmd = MIStackSelectFrame(frame->level);
            SendMICommand(m_session, cmd);
            if (MICommandResultOK(cmd)) {
                finish = true;
            }
            MICommandFree(cmd);

            atEntry = true;
            break;
        }
                
        // issue a continue command .. this is assuming we're pre-main or
        // pre-parallel entry point.
        DVPRINT("Trying to continue to start breakpoint attempt = %d\n", attempt);
                
        cmd = MIExecContinueAll();
        SendMICommand(m_session, cmd);
        if (!MICommandResultOK(cmd)) {
            EVENT_ERROR(e, DBGERR_DEBUGGER,
                        MICommandResultErrorMessage(cmd));
            MICommandFree(cmd);
            return e;
        }
        MICommandFree(cmd);

        MIEvent *evt = m_dbg.waitForAsyncEvent();

        if (evt->type == MIEventTypeBreakpointHit) {
            // If we hit a main breakpoint, we are done (unset finish).
            // Otherwise check parallel breakpoints
            if (dbgbp_t *bp = FindBPByID(main_entry_BP, evt->bkptno)) {
                atEntry = true;
            } else if ((bp = FindBPByID(parallel_entry_BP, evt->bkptno))) {
                // If we hit a parallel breakpoint, we need to finish
                atEntry = finish = true;
            } else {
                DVPRINT("%s, %s Hit unknown entry breakpoint.", __FILE__, __func__);
            }
        } else if (evt->type == MIEventTypeSignal) {
            // slurm will send a SIGSTOP for some reason, try to recover from
            // that
            DVPRINT("DbgStartup: Received signal %s (%s), trying to recover.\n",
                    evt->sigName, evt->sigMeaning);
            MIEventFree(evt);

            if (gdb_vers == GdbVersion::ROC_GDB) {
                // rocgdb triggers a signal without stopping the process.
                // if this just goes back to the top of loop, the MIStackListAllFrames
                // will fail.   One could pose this without the special casing by
                // dealing with failure there, but I don't know enough about every
                // situation that could cause ListAllFrames to fail to assure
                // myself it would be waiting forever.  So cowardly, just changing the
                // handling for roc.
                DVPRINT("waiting for another event\n");
                evt = m_dbg.waitForAsyncEvent();
                DVPRINT("Got event type %d\n", evt->type);
                MIEventFree(evt);
            }
            evt = NULL;
            continue;  // for attempt
        }

        // Free the event
        MIEventFree(evt);
        break;
        
    } // for attempt

    if (!atEntry) {
        EVENT_ERROR(e, DBGERR_DEBUGGER, "Failed to hit entry breakpoint.");
        return e;
    }

    // remove all of the entry breakpoints. We are done with them.
    if ((e = delete_internal_breakpoints(main_entry_BP)) != NULL)
        return e;
    if ((e = delete_internal_breakpoints(parallel_entry_BP)) != NULL)
        return e;

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
        cmd = MIExecFinish();
        SendMICommand(m_session, cmd);
        if (!MICommandResultOK(cmd)) {
            MICommandFree(cmd);
            EVENT_ERROR(e, DBGERR_DEBUGGER, "Finish command failed.");
            return e;
        }
        MICommandFree(cmd);

        MIEvent *evt = m_dbg.waitForAsyncEvent();

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

    return nullptr;
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

dbgevent_t *DbgGdbMI::Initializer::setup_gdbmi_entry_points() {
    DVPRINT("*** Dbg %s, %s\n", __FILE__, __func__);
    dbgevent_t *e;
    MICommand *cmd;

    // set the lang to c. We always want to do this!!
    cmd = _MIGDBSet("lang", "c");
    SendMICommand(m_session, cmd);
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
        SendMICommand(m_session, cmd);
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
    SendMICommand(m_session, cmd);
    if (!MICommandResultOK(cmd)) {
        MICommandFree(cmd);
        EVENT_ERROR(e, DBGERR_DEBUGGER, "Gdb set lang command failed.");
        return e;
    }
    MICommandFree(cmd);

    return nullptr;
}

dbgevent_t *DbgGdbMI::Initializer::setup_gdbmi_main_entry_point() {
    DVPRINT("*** Dbg %s, %s\n", __FILE__, __func__);
    dbgevent_t *e;
    MICommand *cmd;
    dbgbp_t *bp_res;

    main_entry_BP = NewList();

    // set the lang to c. We always want to do this!!
    cmd = _MIGDBSet("lang", "c");
    SendMICommand(m_session, cmd);
    if (!MICommandResultOK(cmd)) {
        MICommandFree(cmd);
        EVENT_ERROR(e, DBGERR_DEBUGGER, "Gdb set lang command failed.");
        return e;
    }
    MICommandFree(cmd);

    cmd = MIBreakInsert(0, 0, 0, NULL, 0, "main", 0);
    SendMICommand(m_session, cmd);
    // Note we only want to insert the breakpoint into our internal list if the
    // command succeeded.
    if (MICommandResultOK(cmd)) {
        if ((bp_res = get_dbgbp(cmd)) != NULL)
            AddToList(main_entry_BP, (void *)bp_res);
    }
    MICommandFree(cmd);

    // set the lang back to auto. We always want to do this!!
    cmd = _MIGDBSet("lang", "auto");
    SendMICommand(m_session, cmd);
    if (!MICommandResultOK(cmd)) {
        MICommandFree(cmd);
        EVENT_ERROR(e, DBGERR_DEBUGGER, "Gdb set lang command failed.");
        return e;
    }
    MICommandFree(cmd);

    return NULL;
}

dbgevent_t *DbgGdbMI::Initializer::setup_gdbmi_exit_points() {
    DVPRINT("*** Dbg %s, %s\n", __FILE__, __func__);
    dbgevent_t *e;
    MICommand *cmd;
    dbgbp_t *bp_res;

    // set the lang to c. We always want to do this!!
    cmd = _MIGDBSet("lang", "c");
    SendMICommand(m_session, cmd);
    if (!MICommandResultOK(cmd)) {
        MICommandFree(cmd);
        EVENT_ERROR(e, DBGERR_DEBUGGER, "Gdb set lang command failed.");
        return e;
    }
    MICommandFree(cmd);

    m_exitBP = NewList();

    // Try to set a breakpoint at all of the regular exit points.
    // If any command fails, don't worry about it. We expect many of these
    // breakpoints to not exist.
    // FIXME: Figure out what the proper behavior should be. Should we warn if
    //        no exit point is found??

    cmd = MIBreakInsert(0, 0, 0, NULL, 0, "*(&exit)", 0);
    SendMICommand(m_session, cmd);
    if (MICommandResultOK(cmd)) {
        if ((bp_res = get_dbgbp(cmd)) != NULL) {
            // Change the breakpoint type to an exit point or DBGBP_EXIT_TYPE
            bp_res->bp_type = DBGBP_EXIT_TYPE;
            AddToList(m_exitBP, (void *)bp_res);
        }
    }
    MICommandFree(cmd);

    cmd = MIBreakInsert(0, 0, 0, NULL, 0, "*(&_exit)", 0);
    SendMICommand(m_session, cmd);
    if (MICommandResultOK(cmd)) {
        if ((bp_res = get_dbgbp(cmd)) != NULL) {
            // Change the breakpoint type to an exit point or DBGBP_EXIT_TYPE
            bp_res->bp_type = DBGBP_EXIT_TYPE;
            AddToList(m_exitBP, (void *)bp_res);
        }
    }
    MICommandFree(cmd);

    cmd = MIBreakInsert(0, 0, 0, NULL, 0, "*(&abort)", 0);
    SendMICommand(m_session, cmd);
    if (MICommandResultOK(cmd)) {
        if ((bp_res = get_dbgbp(cmd)) != NULL) {
            // Change the breakpoint type to an exit point or DBGBP_EXIT_TYPE
            bp_res->bp_type = DBGBP_EXIT_TYPE;
            AddToList(m_exitBP, (void *)bp_res);
        }
    }
    MICommandFree(cmd);

    cmd = MIBreakInsert(0, 0, 0, NULL, 0, "*(&MPIR_Errors_are_fatal)", 0);
    SendMICommand(m_session, cmd);
    if (MICommandResultOK(cmd)) {
        if ((bp_res = get_dbgbp(cmd)) != NULL) {
            // Change the breakpoint type to an exit point or DBGBP_EXIT_TYPE
            bp_res->bp_type = DBGBP_EXIT_TYPE;
            AddToList(m_exitBP, (void *)bp_res);
        }
    }
    MICommandFree(cmd);

    cmd = MIBreakInsert(0, 0, 0, NULL, 0, "*(&MPID_Abort)", 0);
    SendMICommand(m_session, cmd);
    if (MICommandResultOK(cmd)) {
        if ((bp_res = get_dbgbp(cmd)) != NULL) {
            // Change the breakpoint type to an exit point or DBGBP_EXIT_TYPE
            bp_res->bp_type = DBGBP_EXIT_TYPE;
            AddToList(m_exitBP, (void *)bp_res);
        }
    }
    MICommandFree(cmd);

    cmd = MIBreakInsert(0, 0, 0, NULL, 0, "*(&MPI_Abort)", 0);
    SendMICommand(m_session, cmd);
    if (MICommandResultOK(cmd)) {
        if ((bp_res = get_dbgbp(cmd)) != NULL) {
            // Change the breakpoint type to an exit point or DBGBP_EXIT_TYPE
            bp_res->bp_type = DBGBP_EXIT_TYPE;
            AddToList(m_exitBP, (void *)bp_res);
        }
    }
    MICommandFree(cmd);

    cmd = MIBreakInsert(0, 0, 0, NULL, 0,
                        "*(&ompi_mpi_errors_are_fatal_comm_handler)", 0);
    SendMICommand(m_session, cmd);
    if (MICommandResultOK(cmd)) {
        if ((bp_res = get_dbgbp(cmd)) != NULL) {
            // Change the breakpoint type to an exit point or DBGBP_EXIT_TYPE
            bp_res->bp_type = DBGBP_EXIT_TYPE;
            AddToList(m_exitBP, (void *)bp_res);
        }
    }
    MICommandFree(cmd);

    // set the lang back to auto. We always want to do this!!
    cmd = _MIGDBSet("lang", "auto");
    SendMICommand(m_session, cmd);
    if (!MICommandResultOK(cmd)) {
        MICommandFree(cmd);
        EVENT_ERROR(e, DBGERR_DEBUGGER, "Gdb set lang command failed.");
        return e;
    }
    MICommandFree(cmd);

    return NULL;
}

/*
** Initialise debugger, make sure we are using
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

    return NewEvent(DBGEV_OK);
}


/*
** Attach debugger. Added specifically for Cray XT / ALPS, but it might
** work for other platforms as well.
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
    cmd = CLIInfoProc();
    SendMICommand(m_session, cmd);
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
    m_pid = procInfo->pid;
    CLIInfoProcInfoFree(procInfo);
    MICommandFree(cmd);

    DVPRINT("InitDbgAttach(): Obtained pid: %d.\n", m_pid);

    // setup default exit points
    if ((e = setup_gdbmi_exit_points()) != NULL) return e;

    if (gdb_vers == GdbVersion::ROC_GDB && EmptyList(m_exitBP)) {
        // This debugger currently stops *before* reading symbols.
        // In the future this be moved to startup along with
        // setting up the exit points.  Unless there is some unknown
        // reason the exit breakpoints need be set up before startup..
        DVPRINT("Continuing for ROC\n");
        auto cmd = MIExecContinue();
        SendMICommand(m_session, cmd);
        if (!MICommandResultOK(cmd)) {
            EVENT_ERROR(e, DBGERR_DEBUGGER, "Continue failed: %s",
                        MICommandResultErrorMessage(cmd));
            MICommandFree(cmd);
            return e;
        }
        MICommandFree(cmd);
        e = m_dbg.waitAndProcessAsyncEvent();
        DVPRINT("InitDbgAttach: continued to the real entry!\n");
        FreeEvent(e);

        if ((e = setup_gdbmi_exit_points()) != NULL) return e;
    }

    return NewEvent(DBGEV_OK);
}

dbgevent_t *DbgGdbMI::Initializer::setup_gdbmi_environment() {
    DVPRINT("*** Dbg %s, %s\n", __FILE__, __func__);
    dbgevent_t *e;
    MICommand *cmd;

#if 0
	if (debug_flag)
	{
		// set debug infrun mode on
		cmd = _MIGDBSet("debug infrun", "1");
		SendMICommand(m_session, cmd);
		if (!MICommandResultOK(cmd))
		{
			MICommandFree(cmd);
			EVENT_ERROR(e, DBGERR_DEBUGGER, "Gdb set debug infrun command failed.");
			return e;
		}
		MICommandFree(cmd);

		// set debug lin-lwp mode on
		cmd = _MIGDBSet("debug lin-lwp", "1");
		SendMICommand(m_session, cmd);
		if (!MICommandResultOK(cmd))
		{
			MICommandFree(cmd);
			EVENT_ERROR(e, DBGERR_DEBUGGER, "Gdb set debug lin-lwp command failed.");
			return e;
		}
		MICommandFree(cmd);

		// set debug lin-lwp-async mode on
		cmd = _MIGDBSet("debug lin-lwp-async", "1");
		SendMICommand(m_session, cmd);
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
            MICommandFree(cmd);
            EVENT_ERROR(e, DBGERR_DEBUGGER,
                        "Gdb set print characters command failed.");
            return e;
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

    if (gdb_vers != GdbVersion::ROC_GDB && gdb_vers != GdbVersion::CUSTOM) {
        // If running with gpu's, turn upc off. If we don't do this, things will
        // break
        if (gpu_flag) {
            cmd = _MIGDBSet("upcsingle", "1");
            SendMICommand(m_session, cmd);
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
    SendMICommand(m_session, cmd);
    MICommandFree(cmd);

    // Set maximum variable size to unlimited to allow gdb to print large values
    cmd = _MIGDBSet("max-value-size", "unlimited");
    SendMICommand(m_session, cmd);
    MICommandFree(cmd);

    // manually set up pretty printing for relocated installs
    setup_pretty_printing();

    // set non-stop mode
    auto isInNonStopMode = false;
    if (non_stop_flag) {
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
    dbgevent_t *e;
    MICommand *cmd;

    cmd = CLIHandle("SIG64 nostop noprint pass");
    SendMICommand(m_session, cmd);
    if (!MICommandResultOK(cmd)) {
        MICommandFree(cmd);
        EVENT_ERROR(e, DBGERR_DEBUGGER, "Handle SIG64 command failed.");
        return e;
    }
    MICommandFree(cmd);

    cmd = CLIHandle("SIG63 nostop noprint pass");
    SendMICommand(m_session, cmd);
    if (!MICommandResultOK(cmd)) {
        MICommandFree(cmd);
        EVENT_ERROR(e, DBGERR_DEBUGGER, "Handle SIG63 command failed.");
        return e;
    }
    MICommandFree(cmd);

    cmd = CLIHandle("SIG39 nostop noprint pass");
    SendMICommand(m_session, cmd);
    if (!MICommandResultOK(cmd)) {
        MICommandFree(cmd);
        EVENT_ERROR(e, DBGERR_DEBUGGER, "Handle SIG39 command failed.");
        return e;
    }
    MICommandFree(cmd);

    cmd = CLIHandle("SIG33 nostop noprint pass");
    SendMICommand(m_session, cmd);
    if (!MICommandResultOK(cmd)) {
        MICommandFree(cmd);
        EVENT_ERROR(e, DBGERR_DEBUGGER, "Handle SIG33 command failed.");
        return e;
    }
    MICommandFree(cmd);

    cmd = CLIHandle("SIG32 nostop noprint pass");
    SendMICommand(m_session, cmd);
    if (!MICommandResultOK(cmd)) {
        MICommandFree(cmd);
        EVENT_ERROR(e, DBGERR_DEBUGGER, "Handle SIG32 command failed.");
        return e;
    }
    MICommandFree(cmd);

    cmd = CLIHandle("SIGUSR1 nostop noprint pass");
    SendMICommand(m_session, cmd);
    if (!MICommandResultOK(cmd)) {
        MICommandFree(cmd);
        EVENT_ERROR(e, DBGERR_DEBUGGER, "Handle SIGUSR1 command failed.");
        return e;
    }
    MICommandFree(cmd);

    cmd = CLIHandle("SIGUSR2 nostop noprint pass");
    SendMICommand(m_session, cmd);
    if (!MICommandResultOK(cmd)) {
        MICommandFree(cmd);
        EVENT_ERROR(e, DBGERR_DEBUGGER, "Handle SIGUSR2 command failed.");
        return e;
    }
    MICommandFree(cmd);

    // We use this signal to interrupt the application. SIGINT is blocked and
    // won't work (I can't figure out why...).
    cmd = CLIHandle("SIGSTOP stop print nopass");
    SendMICommand(m_session, cmd);
    if (!MICommandResultOK(cmd)) {
        MICommandFree(cmd);
        EVENT_ERROR(e, DBGERR_DEBUGGER, "Handle SIGSTOP command failed.");
        return e;
    }
    MICommandFree(cmd);

    cmd = CLIHandle("SIGCONT nostop noprint pass");
    SendMICommand(m_session, cmd);
    if (!MICommandResultOK(cmd)) {
        MICommandFree(cmd);
        EVENT_ERROR(e, DBGERR_DEBUGGER, "Handle SIGCONT command failed.");
        return e;
    }
    MICommandFree(cmd);

    cmd = CLIHandle("SIGTSTP nostop noprint pass");
    SendMICommand(m_session, cmd);
    if (!MICommandResultOK(cmd)) {
        MICommandFree(cmd);
        EVENT_ERROR(e, DBGERR_DEBUGGER, "Handle SIGTSTP command failed.");
        return e;
    }
    MICommandFree(cmd);

    cmd = CLIHandle("SIGCHLD nostop noprint pass");
    SendMICommand(m_session, cmd);
    if (!MICommandResultOK(cmd)) {
        MICommandFree(cmd);
        EVENT_ERROR(e, DBGERR_DEBUGGER, "Handle SIGCHLD command failed.");
        return e;
    }
    MICommandFree(cmd);

    cmd = CLIHandle("SIGPROF nostop noprint pass");
    SendMICommand(m_session, cmd);
    if (!MICommandResultOK(cmd)) {
        MICommandFree(cmd);
        EVENT_ERROR(e, DBGERR_DEBUGGER, "Handle SIGPROF command failed.");
        return e;
    }
    MICommandFree(cmd);

    cmd = CLIHandle("SIGALRM nostop noprint pass");
    SendMICommand(m_session, cmd);
    if (!MICommandResultOK(cmd)) {
        MICommandFree(cmd);
        EVENT_ERROR(e, DBGERR_DEBUGGER, "Handle SIGALRM command failed.");
        return e;
    }
    MICommandFree(cmd);

    cmd = CLIHandle("SIGVTALRM nostop noprint pass");
    SendMICommand(m_session, cmd);
    if (!MICommandResultOK(cmd)) {
        MICommandFree(cmd);
        EVENT_ERROR(e, DBGERR_DEBUGGER, "Handle SIGVTALRM command failed.");
        return e;
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

dbgevent_t *DbgGdbMI::Initializer::delete_internal_breakpoints(list_t *bplist) {
    DVPRINT("*** Dbg %s, %s\n", __FILE__, __func__);
    dbgevent_t *e;
    MICommand *cmd;
    dbgbp_t *bp;

    SetList(bplist);

    while ((bp = (dbgbp_t *)GetListElement(bplist)) != (dbgbp_t *)NULL) {
        // send the delete command
        cmd = MIBreakDelete(1, &bp->bp_id);
        SendMICommand(m_session, cmd);
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
    dbgevent_t *e = nullptr;
#ifdef ALPS
    MICommand *cmd;
#endif

    /*
    ** Interrupt the process if it is running. This will generate a async event.
    ** We want to grab it and ignore it.
    ** We only want to do this if we are not detaching.
    */
    if (m_dbg.isRunning() && !detach) {
        DVPRINT("DbgQuit: Sending SIGSTOP to process.\n");
        ::kill(m_pid, SIGSTOP);
        // wait for the asynch command to complete
        e = m_dbg.waitAndProcessAsyncEvent();
        FreeEvent(e);
    }

    DVPRINT("DbgQuit: Detaching from process.\n");

    cmd = MITargetDetach(m_pid);

    SendMICommand(m_session, cmd);

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
        ::kill(m_pid, SIGSTOP);
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
dbgevent_t *DbgGdbMI::Initializer::interrupt(bool all) {
    DVPRINT("*** Dbg %s, %s(bool all %s)\n", __FILE__, __func__, all?"true":"false");
    dbgevent_t *e;
    dbgevent_t *async_e;

    const char* threads = nullptr;  // thread focus is unimplemented

    if (m_dbg.isRunning()) {
        DVPRINT("RUNNING - sending SIGSTOP\n");
        if (::kill(m_pid, SIGSTOP)) {
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

