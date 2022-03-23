/*********************************************************************************\
 * Guard debugger
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

#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <math.h>
#include <pwd.h>
#include <setjmp.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <fstream>
#include <filesystem>

#include "client.h"
#include "useful.h"
#include "df.h"

/*
 * External Global Variables
 */
extern char *optarg;
extern int optind;

/*
 * Global Variables
 */
clnt_settings_t Client;  // global settings store - initialized in main

// static globals
static jmp_buf Top;     // Where to go when restoring upon signal
static int debug_flag;  // Debug flag - TODO: not used yet
static int started;     // Is it alright to longjmp?

/*
 * Static Function Prototypes
 */
static void Banner(void);
static void SigHandler(int);
static const char * getDefaultProtocol();
static void printEventLocation(const event_set_t *ev_set, const Bitset *group);
static void readInitCommandsFile();

/*
 * long opts
 */
static const struct option long_opts[] = {
    {"batch", required_argument, 0, 'b'},
    {"keep-going", no_argument, 0, 'k'},
    {"echo", no_argument, 0, 'e'},
    {"help", no_argument, 0, 'h'},
    {"interface", required_argument, 0, 'i'},
    {"output", required_argument, 0, 'o'},
    {"silent", no_argument, 0, 's'},
    {"version", no_argument, 0, 'v'},
    {"xml", no_argument, &(Client.XML_mode), 1},
    {"debug", no_argument, &debug_flag, 1},
    {"no-init", no_argument, 0, 'n'},
    {"launch", required_argument, 0, 'l'},
    {"attach", required_argument, 0, 'j'},
    {"exec", required_argument, 0, 'x'},
    {0, 0, 0, 0}};

static void usage(char *name) {
    PrintOutMsg("Usage: %s [options]\n\n", name);

    PrintOutMsg("Options:\n\n");
    PrintOutMsg("  --help, -h           Display this text.\n");
    PrintOutMsg("  --no-init, -n        Don't execute any commands from the gdb4hpc_init file.\n");
    PrintOutMsg("                       (By default, $HOME/.gdb4hpc/gdb4hpc_init)\n");
    PrintOutMsg("  --silent, -s         Suppress version banner on startup.\n");
    PrintOutMsg("  --version, -v        Display version.\n");
    PrintOutMsg("  --attach=JOBID, -j   Immediately attach to application with JOBID.\n");
    PrintOutMsg("  --exec=CMD, -x       Execute CMD at startup. This option can be used\n");
    PrintOutMsg("                       multiple times. Commands passed here are executed\n");
    PrintOutMsg("                       after commands in gdb4hpc_init.\n");

    PrintOutMsg("\nBatch mode options:\n\n");
    PrintOutMsg("  --batch=FILE, -b     Execute debugger commands from FILE and exit.\n");
    PrintOutMsg("  --output=FILE, -o    Redirect stdout/stderr to FILE. Must be used with --batch.\n");
    PrintOutMsg("  --keep-going, -k     In batch mode, don't exit when an error is encountered.\n");
    PrintOutMsg("  --echo, -e           In batch mode, echo each command before executing it.\n");

    PrintOutMsg("\nFor more information, type \"help\" inside gdb4hpc, or consult the gdb4hpc manpage. (man gdb4hpc)\n");
}

static void Banner(void) {
    PrintOutMsg("gdb4hpc %s - Cray Line Mode Parallel Debugger\n", GUARD_VERSION);
    PrintOutMsg("With Cray Comparative Debugging Technology.\n");
    PrintOutMsg("Copyright 2007-2021 Hewlett Packard Enterprise Development LP.\n");
    PrintOutMsg(
        "Copyright 1996-2016 University of Queensland. All Rights Reserved.\n");
    PrintOutMsg("\n");
    PrintOutMsg("Type \"help\" for a list of commands.\n");
    PrintOutMsg("Type \"help <cmd>\" for detailed help about a command.\n");
    DPRINT("Debug printing is enabled.\n");
}

static const char * getDefaultProtocol() {
    auto envProto = getenv("GDB4HPC_PROTO");
    return envProto ? envProto : DBG_PROTO;
}

static int checkDirPerms(const char *dir) {
    struct stat st;

    if (dir == NULL) return -1;

    // Stat the tmp_dir
    if (stat(dir, &st)) {
        // could not stat the directory
        return -1;
    }

    // check to see if the user id is the same as this directory
    if (st.st_uid == getuid()) {
        // Check the user perms
        return ((st.st_mode & S_IRWXU) ? 0 : -1);
    }

    // check to see if the group id is the same as the directory
    if (st.st_gid == getgid()) {
        // Check the group perms
        return ((st.st_mode & S_IRWXG) ? 0 : -1);
    }

    // Check the other perms
    return ((st.st_mode & S_IRWXO) ? 0 : -1);
}

static void setupCfgDir(const char *dir) {
    struct stat st;

    if (dir == NULL) return;

    // try to stat the directory
    if (stat(dir, &st)) {
        // the directory doesn't exist so we need to create it
        // use perms 755
        if (mkdir(dir, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)) {
            PrintOutError("Could not create config directory %s.\n", dir);
            PrintOutError("Try setting env var %s and try again. Aborting...\n",
                          GUARD_CFG_DIRECTORY);
            exit(1);
        }
    } else {
        // chmod the existing directory to ensure there isn't any funny business
        // going on
        if (chmod(dir, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)) {
            PrintOutError("Could not chmod config directory %s.\n", dir);
            PrintOutError("Try setting env var %s and try again. Aborting...\n",
                          GUARD_CFG_DIRECTORY);
            exit(1);
        }
    }
}

// All environment setup should happen here
// XXX: This function and it's dependencies should be synced with the stuff in
// the proxy_daemon.c source. If you change it here, change it there also.
static void SetupEnvironment(void) {
    char *inst_base;
    char *lib_dir;
    char *libexec_dir;
    char *cfg_dir;

    // Adjust the environment to add our install base to LD_LIBRARY_PATH and
    // PATH of our process
    if ((inst_base = getinstallbase()) == NULL) {
        PrintOutError("Could not locate install base. Aborting...\n");
        exit(1);
    }
    // create the path strings
    Asprintf(&lib_dir, "%s/lib", inst_base);
    Asprintf(&libexec_dir, "%s/libexec", inst_base);

    // Set the environment variables
    adjustLibPath(lib_dir);
    adjustExecPath(libexec_dir);

    // get the cfg dir settings
    if ((cfg_dir = getenv(GUARD_CFG_DIRECTORY)) == NULL) {
        // Not found.
        // We now want to use $HOME since this will contain guard only files
        // like editline history files.
        char *home_dir;

        if ((home_dir = getenv("HOME")) == NULL) {
            PrintOutError("Could not read $HOME. Aborting...\n");
            exit(1);
        }

        // Check if we can write to HOME
        if (checkDirPerms(home_dir)) {
            // We have no where to create a temporary directory...
            PrintOutError("No write permission for $HOME. Aborting...\n");
            exit(1);
        }

        // Create the directory name string
        Asprintf(&cfg_dir, "%s/.gdb4hpc", home_dir);

        // Setup/create this directory
        setupCfgDir(cfg_dir);
        setenv(GUARD_CFG_DIRECTORY, cfg_dir, 1);
        Free(cfg_dir);
    } else {
        // The user set GUARD_CFG_DIRECTORY, so we *ALWAYS* want to use that
        struct passwd *pw;
        char *tmp_dir;

        // Check to see if we can write to this directory
        if (checkDirPerms(cfg_dir)) {
            PrintOutError(
                "Bad permissions on directory specified by env var %s. "
                "Aborting...\n",
                GUARD_CFG_DIRECTORY);
            exit(1);
        }

        // Lets create a unique directory just in case the user gave us some
        // weird location that others can access

        // Get the pw info, this is used in the unique name part of cfg
        // directories
        if ((pw = getpwuid(getuid())) == NULL) {
            PrintOutError("getpwuid() failed. Aborting...\n");
            exit(1);
        }

        tmp_dir = cfg_dir;
        cfg_dir = NULL;
        Asprintf(&cfg_dir, "%s/.gdb4hpc-%s", tmp_dir, pw->pw_name);

        // Setup/create this directory
        setupCfgDir(cfg_dir);
        setenv(GUARD_CFG_DIRECTORY, cfg_dir, 1);
        Free(cfg_dir);
    }

    Free(lib_dir);
    Free(libexec_dir);
    Free(inst_base);
}

static void readInitCommandsFile() {
    const auto filename = std::string{"gdb4hpc_init"};
    if (auto cfgDir = getenv(GUARD_CFG_DIRECTORY)) {
        const auto path = std::string{cfgDir} + "/" + filename;

        // The init file is optional. If it doesn't exist, just return
        if (!std::filesystem::exists(path)) {
            DPRINT("%s not found at %s.\n", filename.c_str(), path.c_str());
            return;
        }

        if (auto stream = std::ifstream(path); !stream.fail()) {
            PrintOutMsg("Initializing from %s\n", path.c_str());
            for (std::string line; std::getline(stream, line); ) {
                DPRINT("Queued command from %s: %s\n", filename.c_str(), line.c_str());
                QueueCommand(line);
            }
        } else {
            PrintOutMsg("Warning: failed to open %s for reading.\n", filename.c_str());
        }
    } else {
        PrintOutMsg("Warning: couldn't find configuration directory to read %s.\n", filename.c_str());
    }
}

int main(int argc, char *argv[]) {
    int c;
    double ulp;
    int opt_ind = 0;
    cli_stream_args_t *cli_args = NULL;  // CLI args
    char *in_file = NULL;
    char *out_file = NULL;
    bool should_read_init = true; // read gdb4hpc_init?

    // Ensure the environment has been setup, otherwise set it up and re-exec
    // This is required because otherwise dlopen's will be fubared.
    if (getenv(GUARD_ENV_SETUP) == NULL) {
        char *exe_path;

        SetupEnvironment();
        // Done. Set the proper environment variable so that we don't have an
        // infinite loop here.
        setenv(GUARD_ENV_SETUP, "true", 1);
        exe_path = getexepath();
        execve(exe_path, argv, environ);
    }

    // Initialize the settings to the default values
    Client.confirm = 1;               // Confirm dangerous commands?
    Client.quit = 0;                  // Quit the debugger?
    Client.interactive = 1;           // Interactive mode?
    Client.silent = 0;                // Print banner?
    Client.echo = false;              // Echo each command in batch mode?
    Client.keep_going = false;        // Ignore errors in batch mode?
    Client.unsafe = 0;                // Be unsafe?
    Client.early_entry = 0;           // Enter at pmi_init?
    Client.print_entry_frame = 1;     // Print frame locations at startup?
    Client.print_source_on_step = 1;  // Print source line on each step?
    Client.print_elements = 0;        // Elements to print in arrays or char * strings
    Client.sync_wait = 0;             // Treat asynchronous cmds as sync?
    Client.sync_blocked = 0;          // Current blocked on async cmd?
    Client.proto = getDefaultProtocol(); // Default debugger protocol
    Client.arch = DBG_ARCH;           // Default starter achitecture
    Client.backend = DBG_BACKEND;     // Default debug backend
    Client.XML_mode = 0;              // XML output mode
    Client.set_env_list = NewList();  // Env vars from set env cmd
    // find out the ulp for this machine
    ulp = nextafter(0.0, 0.1);
    Client.cmp_lower_eps = ulp;
    Client.cmp_upper_eps = ulp;
    Client.brk_pending = PendingBrkType::Auto;  // Allow pending breakpoints?

    /*
    ** Set signal handlers.
    ** Note that this MUST happen before the InitCLI function is called,
    *otherwise
    ** things like editline/readline won't register the signal handlers
    *properly.
    ** In the future, if a signal handler needs to be added it should happen
    *early
    ** on before InitCLI.
    */
    DbgHandleSignal(SIGINT, SigHandler, DBG_SIG_MASKABLE);
    DbgHandleSignal(SIGHUP, SigHandler, DBG_SIG_NOMASK);
    DbgHandleSignal(SIGTERM, SigHandler, DBG_SIG_NOMASK);
    DbgHandleSignal(SIGQUIT, SigHandler, DBG_SIG_NOMASK);
    // we want to ignore SIGPIPE
    DbgHandleSignal(SIGPIPE, SIG_IGN, DBG_SIG_NOMASK);

    while ((c = getopt_long(argc, argv, "b:j:l:x:nkehi:o:sv", long_opts, &opt_ind)) !=
           -1) {
        switch (c) {
            case 0:
                // if this is a flag, do nothing
                break;
            case 'b': {
                char *stringp;

                if (optarg == NULL) {
                    PrintOutError(
                        "Error: --batch provided, but missing arguments.\n");
                    exit(1);
                }

                // Set the mode and create the cli args
                Client.cli_mode = CLI_FILE_STREAM;
                stringp = strdup(optarg);
                in_file = strdup(StripQuotes(stringp));
                Free(stringp);

                // Turn confirm off, we will not be able to call fgetc on stdin
                // with batch. If this is needed, rework the query function
                // to call an external cli function that is cli stream
                // dependent.
                Client.confirm = 0;

                break;
            }
            case 'j': {
                if (optarg == nullptr) {
                    PrintOutError(
                        "Error: --attach provided, but missing arguments.\n");
                    exit(1);
                }

                std::ostringstream ss;
                ss << "attach $job " << optarg;
                QueueCommand(ss.str());

                break;
            }
            case 'l': {
                if (optarg == nullptr) {
                    PrintOutError(
                        "Error: --launch provided, but missing arguments.\n");
                    exit(1);
                }

                auto ranksEnv = getenv(GDB4HPC_DEFAULT_NUM_RANKS);
                auto ranks = ranksEnv ? ranksEnv : "1";

                std::ostringstream ss;
                ss << "launch $job{" << ranks << "} " << optarg;
                QueueCommand(ss.str());

                break;
            }
            case 'x': {
                if (optarg == nullptr) {
                    PrintOutError(
                        "Error: --exec provided, but missing arguments.\n");
                    exit(1);
                }
                QueueCommand(optarg);
                break;
            }
            case 'n': {
                should_read_init = false;
                break;
            }
            case 'k': {
                Client.keep_going = true;
                break;
            }
            case 'e': {
                Client.echo = true;
                break;
            }
            case 'i': {
                char *stringp, *argstr, *token;
                char *hoststr, *portstr, *errpstr;

                if (optarg == NULL) {
                    PrintOutError(
                        "Error: --interface provided, but missing "
                        "arguments.\n");
                    exit(1);
                }

                // Setup things. Note that strsep modifies the string, so it
                // shouldn't use optarg directly.
                argstr = strdup(optarg);
                stringp = argstr;

                // First parse the host argument
                // Make sure its not null or that it doesn't contain a null
                // terminator
                if ((token = strsep(&stringp, ":")) == NULL || *token == '\0') {
                    PrintOutError(
                        "Error:    Encountered problem with host argument.\n");
                    PrintOutError(
                        "Expected: --interface host:IOPort:ERRPort\n");
                    PrintOutError("Found:    --interface %s\n", optarg);
                    exit(1);
                }
                hoststr = token;
                // Next parse the IO port argument
                // Make sure its not null or that it doesn't contain a null
                // terminator
                if ((token = strsep(&stringp, ":")) == NULL || *token == '\0') {
                    PrintOutError(
                        "Error:    Encountered problem with IOPort "
                        "argument.\n");
                    PrintOutError(
                        "Expected: --interface host:IOPort:ERRPort\n");
                    PrintOutError("Found:    --interface %s\n", optarg);
                    exit(1);
                }
                portstr = token;
                // The final argument is what is left in stringp
                // Make sure this isn't pointing at the null terminator
                if (*stringp == '\0') {
                    PrintOutError(
                        "Error:    Encountered problem with ERRPort "
                        "argument.\n");
                    PrintOutError(
                        "Expected: --interface host:IOPort:ERRPort\n");
                    PrintOutError("Found:    --interface %s\n", optarg);
                    exit(1);
                }
                errpstr = stringp;

                // Set the default CLI and args to socket type
                Client.cli_mode = CLI_SOCKET_STREAM;
                cli_args =
                    NewCLIArgs(CLI_SOCKET_STREAM, hoststr, portstr, errpstr);
                // Turn confirm off, we will not be able to call fgetc on stdin
                // through a socket. If this is needed, rework the query
                // function to call an external cli function that is cli stream
                // dependent.
                Client.confirm = 0;
                // Set silent to 1, we don't want to print out banner
                // This should be the case when output is XML
                Client.silent = 1;

                // Cleanup original strsep string that we strdup'ed.
                Free(argstr);

                break;
            }
            case 'o': {
                char *stringp;

                if (optarg == NULL) {
                    PrintOutError(
                        "Error: --output provided, but missing arguments.\n");
                    exit(1);
                }

                // Set the mode and create the cli args
                Client.cli_mode = CLI_FILE_STREAM;
                stringp = strdup(optarg);
                out_file = strdup(StripQuotes(stringp));
                Free(stringp);

                break;
            }
            case 's':
                // Set silent to 1, we don't want to print out banner
                Client.silent = 1;
                break;
            case 'v':
                PrintOutMsg("gdb4hpc-%s\n", GUARD_VERSION);
                exit(0);
            case 'h':
                usage(argv[0]);
                exit(0);
            default:
                usage(argv[0]);
                exit(1);
        }
    }

    // Check if we should use the default CLI, if one of the above options
    // didn't create cli_args, then we know we should use the default. We now
    // also create the CLI_FILE_STREAM CLI here so that an output file can be
    // used with the batch mode
    if (cli_args == NULL) {
        if (Client.cli_mode == CLI_FILE_STREAM) {
            // Ensure that there is an in_file, this is a requirement.
            if (in_file == NULL) {
                PrintOutError(
                    "Error: Cannot use --output argument without --batch "
                    "argument.\n");
                exit(1);
            }
            cli_args = NewCLIArgs(CLI_FILE_STREAM, in_file, out_file, out_file);
            Free(in_file);
            if (out_file != NULL) Free(out_file);
        } else {
            // Use the default CLI

            // Check to see if stdin is a tty, if not set the default CLI to
            // notty
            if (!isatty(STDIN_FILENO)) {
                // Set the default stream to notty
                Client.cli_mode = CLI_NOTTY_STREAM;  // CLI mode
            } else {
                // Set the default stream to tty
                Client.cli_mode = CLI_TTY_STREAM;  // CLI mode
            }
            // Create the CLI args
            cli_args = NewCLIArgs(Client.cli_mode, stdin, stdout, stderr);
        }
    }

    registerProtos();
    if (DbgInit() < 0) {
        PrintOutError("Could not initialise debugger.\n");
        exit(1);
    }

    InitVal();
    InitProcSet();
    InitCommands();
    InitDF();
    InitCLI(Client.cli_mode, cli_args);

    if (!Client.silent) {
        Banner();
    }

    // Read gdb4hpc_init file
    if (should_read_init)
        readInitCommandsFile();

    // Initial reset of the stream, for good measure.
    // If we don't do this, the prompt will be missing when using the editline
    // library. I think this is due to some weirdness with the Banner() call.
    cli_reset_stream();

    (void)setjmp(Top);
    started = 1;

    // Execute commands that have been queued at startup (e.g. immediate attach)
    ExecuteQueuedCommands();

    // Main event loop
    for (;;) {
        // Check this first in case we got here after a signal arrived, that
        // might have set Client.quit.
        if (Client.quit) break;

        // Check to see if we should block on an async event
        if (Client.sync_wait) {
            // get current async pending status
            int astatus = DbgCheckForAsyncPending();

            // If we are currently blocking on async command and the async
            // pending is false, we can unblock.
            if (Client.sync_blocked && !astatus) {
                // Block signals here because there is a race condition
                // otherwise
                HoldSignals();
                DbgUnmaskInput();
                Client.sync_blocked = 0;
                ReleaseSignals();

                // If we are not currently blocking on an async command and the
                // async pending is true, we need to block
            } else if (!Client.sync_blocked && astatus) {
                // Block signals here because there is a race condition
                // otherwise
                HoldSignals();
                DbgMaskInput();
                Client.sync_blocked = 1;
                ReleaseSignals();
            }
        }

        switch (DbgCheckEvents()) {
            case -1:
                Client.quit = 1;

            case 0:
                continue;

            default:
                DbgDispatchEvents();
        }
    }

    // Ignore signals during cleanup
    DbgHandleSignal(SIGINT, SIG_IGN, DBG_SIG_NOMASK);
    DbgHandleSignal(SIGHUP, SIG_IGN, DBG_SIG_NOMASK);
    DbgHandleSignal(SIGTERM, SIG_IGN, DBG_SIG_NOMASK);
    DbgHandleSignal(SIGQUIT, SIG_IGN, DBG_SIG_NOMASK);

    FinishDF();
    EndAll();
    DbgFinalize();
    FinishProcSet();
    FinishFunc();
    FinishVal();
    DestroyCommands();
    DestroyCLI();

    // Reset signal dispositions that used SigHandler
    DbgHandleSignal(SIGINT, SIG_DFL, DBG_SIG_NOMASK);
    DbgHandleSignal(SIGHUP, SIG_DFL, DBG_SIG_NOMASK);
    DbgHandleSignal(SIGTERM, SIG_DFL, DBG_SIG_NOMASK);
    DbgHandleSignal(SIGQUIT, SIG_DFL, DBG_SIG_NOMASK);

    exit(0);
}

void ForceQuit(void) {
    Client.quit = 1;
    // Force any running graphs to finish
    FinishDF();
}

static void SigHandler(int sig) {
    dbgprocset_t *s;
    rec_set_t *rec_set;
    rep_set_t *rep_set;
    dbgloc_t *halt_loc;

    DPRINT("\n***In SigHandler\n");

    /*
    ** Stop the active graph if there is one.
    */
    if (ActiveDF()) {
        ExitDF();
    }

    // Any signals that should cause gdb4hpc to quit should be handled here.
    if (sig == SIGHUP || sig == SIGTERM || sig == SIGQUIT) {
        Client.quit = 1;

        if (started) longjmp(Top, sig);
    }

    s = CurrentProcSet();

    // Create a rec_set for this procset
    rec_set = CreateRecSetForProcset(s);

    // Call the interupt command
    rep_set = DbgInterrupt(rec_set,true);

    // process error
    ProcessErrorInRepSet(rep_set);

    // process location
    for (auto&& reply : rep_set) {
        halt_loc = EVENT_LOC(reply->rep_event);
        PrintHaltedLocationSet(reply->rep_di->d_handle, reply->rep_group.get(),
                               halt_loc);
    }

    FreeRepSet(rep_set);
    FreeRecSet(rec_set);

    // unmask the signal
    DbgUnmaskSignal(sig);

    if (started) {
        // Reset the stream to default
        SetDefaultCLIStream();
        longjmp(Top, sig);
    }
}

// Prints program location after a step event is received
static void printEventLocation(const event_set_t *ev_set, const Bitset *group) {
    if (!ev_set || !ev_set->e_ev) {
        return;
    }

    const dbgframe_t *frame = ev_set->e_ev->ev_step.step_frame;
    bool frame_changed = ev_set->e_ev->ev_step.step_frame_changed;

    if (frame) {
        const auto &frameLoc = frame->getLoc();

        // Print location info
        if (frameLoc.loc_file && frameLoc.loc_func && frameLoc.loc_line > 0) {
            // See if we can get the source line
            auto lineArg = std::string{frameLoc.loc_file} + ":" + std::to_string(frameLoc.loc_line);
            lineArg += "," + lineArg; // filename:xx,filename:xx
            dbgevent_t *listResult = nullptr;

            if (ev_set->e_di->d_frontend) {
                listResult = DbgFEList(ev_set->e_di->d_frontend, nullptr, lineArg.c_str());
            }

            if (listResult && listResult->ev_event == DBGEV_LIST && listResult->ev_list->list_lno == frameLoc.loc_line) {
                // Print the source line

                // Print a frame if the last step event changed functions
                if (frame_changed) {
                    PrintMsgWithSetInfo(
                        ev_set->e_di->d_handle, ev_set->e_group.get(),
                        "%s at %s:%d\n", frameLoc.loc_func, frameLoc.loc_file, frameLoc.loc_line
                    );
                }

                auto line = std::string{listResult->ev_list->list_line};
                formatSourceLine(line);
                
                PrintMsgWithSetInfo(
                    ev_set->e_di->d_handle, ev_set->e_group.get(),
                    "%d\t%s\n", frameLoc.loc_line, line.c_str()
                );
            } else {
                // For some reason we didn't get the right line, just print the frame
                PrintMsgWithSetInfo(
                    ev_set->e_di->d_handle, ev_set->e_group.get(),
                    "%s at %s:%d\n", frameLoc.loc_func, frameLoc.loc_file, frameLoc.loc_line
                );
            }

            FreeEvent(listResult);
        } else if (frameLoc.loc_func) {
            // No file/line, just print function name
            PrintMsgWithSetInfo(
                ev_set->e_di->d_handle, ev_set->e_group.get(), "in %s\n", frameLoc.loc_func);
        } else {
            // Shouldn't ever happen
            PrintMsgWithSetInfo(ev_set->e_di->d_handle, ev_set->e_group.get(),
                "Step finished with no location information.\n"
            );
        }
    } else {
        // Got a null frame? This should never happen
        PrintMsgWithSetInfo(ev_set->e_di->d_handle, ev_set->e_group.get(),
            "Step finished with no location information.\n"
        );
    }
}

// This is called each time an asynchronous reply is recieved. The entire event
// set which contains the aggregated event is passed in as ev_set and the
// individual reply that was just received is passed in as reply_set.
int EventCB(event_set_t *ev_set, const Bitset *reply_set) {
    std::string bpThread;

    // need to do "special" things if we are currently reading from stdin
    // for readline support.
    cli_save_stream();

    DASSERT_NOT_NULL(ev_set);
    DPRINT(
        "INFO[EventCB]: received event type:[%s], ev_bitset:%s, reply_set:%s "
        "from:%s\n",
        EventToStr(ev_set->e_ev), ev_set->e_group->toSetString().c_str(),
        reply_set->toSetString().c_str(), ev_set->e_di->d_handle);

    switch (ev_set->e_ev->ev_event) {
        case DBGEV_EXIT:
            if (ev_set->e_ev->ev_exit == 0) {
                PrintMsgWithSetInfo(ev_set->e_di->d_handle, ev_set->e_group.get(),
                                    "Program exited normally.\n");
            } else {
                PrintMsgWithSetInfo(ev_set->e_di->d_handle, ev_set->e_group.get(),
                                    "Program exited with code 0%o.\n",
                                    ev_set->e_ev->ev_exit);
            }
            break;

        case DBGEV_BPHIT: {
            // ensure the recv bp has a corresponding bp_set. If not, this is an
            // unknown breakpoint. If its unknown either its a breakpoint
            // encountered from ending the graph (graph breakpoints are hidden)
            // or an exit breakpoint that was set by the dbg servers.
            auto bpset = ev_set->e_di->d_breakpoints.findByDbgBreakpoint(*ev_set->e_ev->ev_bp);

            if (!bpset) {
                if (ev_set->e_ev->ev_bp->bp_type == DBGBP_EXIT_TYPE) {
                    PrintMsgWithSetInfo(
                        ev_set->e_di->d_handle, ev_set->e_group.get(),
                        "The application has reached an exit breakpoint.\n");
                } else {
                    if (ev_set->e_ev->ev_bp->bp_loc.loc_line > 0) {
                        PrintMsgWithSetInfo(
                            ev_set->e_di->d_handle, ev_set->e_group.get(),
                            "In %s at %s:%d\n",
                            ev_set->e_ev->ev_bp->bp_loc.loc_func,
                            ev_set->e_ev->ev_bp->bp_loc.loc_file,
                            ev_set->e_ev->ev_bp->bp_loc.loc_line);
                    } else {
                        PrintMsgWithSetInfo(
                            ev_set->e_di->d_handle, ev_set->e_group.get(), "In %s\n",
                            ev_set->e_ev->ev_bp->bp_loc.loc_func);
                    }
                }
                break;
            }

            // What we print out is based on if we are at a watchpoint or
            // breakpoint
            // TODO: What should this default to? Right now I am having it
            // default to the breakpoint message.
            if (!bpset->threadId().empty()) bpThread = " thread " + bpset->threadId();

            switch (bpset->type()) {
                case DBGBP_WATCH_TYPE:
                case DBGBP_AWATCH_TYPE:
                case DBGBP_RWATCH_TYPE:
                    // Print out the updated locale information for the watchpoint.
                    // This was updated in EventCB before calling this function.
                    for (const auto &watch_loc : bpset->watchLocaleSet()) {
                        PrintMsgWithSetInfo(
                            ev_set->e_di->d_handle, watch_loc.group.get(),
                            "Watchpoint %d, variable expression: %s\n",
                            bpset->id(), bpset->watchExpression().c_str());
                        if (watch_loc.line > 0) {
                            PrintMsgWithSetInfo(
                                ev_set->e_di->d_handle, watch_loc.group.get(),
                                "In %s at %s:%d\n", watch_loc.func.c_str(),
                                watch_loc.file.c_str(), watch_loc.line);
                        } else {
                            PrintMsgWithSetInfo(
                                ev_set->e_di->d_handle, watch_loc.group.get(),
                                "In %s\n", watch_loc.func.c_str());
                        }
                    }
                    // clear out the watchpoint locale to avoid printing multiple times
                    bpset->clearWatchLocaleSet();
                    break;

                case DBGBP_BREAK_TYPE:

                    if (bpset->line() > 0) {
                        PrintMsgWithSetInfo(
                            ev_set->e_di->d_handle, ev_set->e_group.get(),
                            "Breakpoint %d, %s at %s:%d%s\n", bpset->id(),
                            bpset->function().c_str(), bpset->file().c_str(),
                            bpset->line(), bpThread.c_str());
                    } else {
                        PrintMsgWithSetInfo(ev_set->e_di->d_handle,
                                            ev_set->e_group.get(),
                                            "Breakpoint %d, in %s%s\n",
                                            bpset->id(), bpset->function().c_str(),
                                            bpThread.c_str());
                    }
                    break;

                case DBGBP_CATCH_TYPE:
                    if (!bpset->watchExpression().empty()) {
                        PrintMsgWithSetInfo(
                            ev_set->e_di->d_handle, ev_set->e_group.get(),
                            "Catchpoint %d (%s)\n",
                            bpset->id(), bpset->watchExpression().c_str());
                    } else {
                        PrintMsgWithSetInfo(ev_set->e_di->d_handle,
                            ev_set->e_group.get(),
                            "Catchpoint %d\n",
                            bpset->id());
                    }
                    break;

                default:
                    if (bpset->line() > 0) {
                        PrintMsgWithSetInfo(
                            ev_set->e_di->d_handle, ev_set->e_group.get(),
                            "Unknown break type %d, %s at %s:%d\n",
                            bpset->id(), bpset->function().c_str(),
                            bpset->file().c_str(), bpset->line());
                    } else {
                        PrintMsgWithSetInfo(ev_set->e_di->d_handle,
                                            ev_set->e_group.get(),
                                            "Unknown break type %d, in %s\n",
                                            bpset->id(), bpset->function().c_str());
                    }
                    break;
            }

            break;
        }

        case DBGEV_BPDEL: {
            // ensure the recv bp has a cooresponding bp_set. If not, this is an
            // unknown breakpoint. If its unknown either its a breakpoint
            // encountered from ending the graph (graph breakpoints are hidden)
            // or an exit breakpoint that was set by the dbg servers.
            auto bpset = ev_set->e_di->d_breakpoints.findByDbgBreakpoint(*ev_set->e_ev->ev_bp);

            if (!bpset) {
                PrintErrorWithSetInfo(
                    ev_set->e_di->d_handle, ev_set->e_group.get(),
                    "ERROR: cannot find breakpoint in BPSetList.\n");
                return 0;
            }

            // ranks removed from breakpoint bitset in
            // src/dbgclnt/dbg_client.cpp

            // What we print out is based on if we are at a watchpoint or
            // breakpoint
            switch (bpset->type()) {
                case DBGBP_WATCH_TYPE:
                case DBGBP_AWATCH_TYPE:
                case DBGBP_RWATCH_TYPE:
                    // Print out the updated locale information for the watchpoint.
                    // This was updated in EventCB before calling this function.
                    for (const auto &watch_loc : bpset->watchLocaleSet()) {
                        PrintMsgWithSetInfo(
                            ev_set->e_di->d_handle, watch_loc.group.get(),
                            "Watchpoint %d deleted because the program has left "
                            "the block in which its expression is valid.\n",
                            bpset->id());
                        if (watch_loc.line > 0) {
                            PrintMsgWithSetInfo(
                                ev_set->e_di->d_handle, watch_loc.group.get(),
                                "In %s at %s:%d\n", watch_loc.func.c_str(),
                                watch_loc.file.c_str(), watch_loc.line);
                        } else {
                            PrintMsgWithSetInfo(
                                ev_set->e_di->d_handle, watch_loc.group.get(),
                                "In %s\n", watch_loc.func.c_str());
                        }
                    }
                    // clear out the watchpoint locale to avoid printing multiple times
                    bpset->clearWatchLocaleSet();
                    break;

                case DBGBP_BREAK_TYPE:
                    if (bpset->line() > 0) {
                        PrintMsgWithSetInfo(
                            ev_set->e_di->d_handle, ev_set->e_group.get(),
                            "Breakpoint %d deleted, %s at %s:%d\n",
                            bpset->id(), bpset->function().c_str(),
                            bpset->file().c_str(), bpset->line());
                    } else {
                        PrintMsgWithSetInfo(ev_set->e_di->d_handle,
                                            ev_set->e_group.get(),
                                            "Breakpoint %d deleted, in %s\n",
                                            bpset->id(), bpset->function().c_str());
                    }
                    break;

                case DBGBP_CATCH_TYPE:
                    if (!bpset->watchExpression().empty()) {
                        PrintMsgWithSetInfo(
                            ev_set->e_di->d_handle, ev_set->e_group.get(),
                            "Catchpoint %d deleted (%s)\n",
                            bpset->id(), bpset->watchExpression().c_str());
                    } else {
                        PrintMsgWithSetInfo(ev_set->e_di->d_handle,
                            ev_set->e_group.get(),
                            "Catchpoint %d deleted.\n",
                            bpset->id());
                    }
                    break;

                default:
                    if (bpset->line() > 0) {
                        PrintMsgWithSetInfo(
                            ev_set->e_di->d_handle, ev_set->e_group.get(),
                            "Unknown break type %d deleted, %s at %s:%d\n",
                            bpset->id(), bpset->function().c_str(),
                            bpset->file().c_str(), bpset->line());
                    } else {
                        PrintMsgWithSetInfo(
                            ev_set->e_di->d_handle, ev_set->e_group.get(),
                            "Unknown break type %d deleted, in %s\n",
                            bpset->id(), bpset->function().c_str());
                    }
                    break;
            }

            break;
        }

        case DBGEV_SIGNAL:
            PrintMsgWithSetInfo(ev_set->e_di->d_handle, ev_set->e_group.get(),
                                "Program received signal %s.\n",
                                ev_set->e_ev->ev_sig_type);

            if (ev_set->e_ev->ev_sig_frame != (dbgframe_t *)NULL) {
                const auto &frameLoc = ev_set->e_ev->ev_sig_frame->getLoc();
                if (frameLoc.loc_file != NULL) {
                    PrintMsgWithSetInfo(
                        ev_set->e_di->d_handle, ev_set->e_group.get(),
                        "In %s at %s:%d\n",
                        frameLoc.loc_func, frameLoc.loc_file, frameLoc.loc_line);
                } else {
                    PrintMsgWithSetInfo(
                        ev_set->e_di->d_handle, ev_set->e_group.get(), "In %s\n",
                        frameLoc.loc_func);
                }
            } else if (ev_set->e_ev->ev_sig_line != NULL) {
                PrintMsgWithSetInfo(ev_set->e_di->d_handle, ev_set->e_group.get(),
                                    "%s\n", ev_set->e_ev->ev_sig_line);
            }

            break;

        case DBGEV_STEP:
            if (Client.print_source_on_step)
                printEventLocation(ev_set, reply_set);
            break;

        case DBGEV_ERROR:
            PrintErrorWithSetInfo(ev_set->e_di->d_handle, ev_set->e_group.get(),
                                  "%s\n", DbgEventErrorStr(ev_set->e_ev));
            break;

        case DBGEV_OK:
            break;

        default:
            PrintErrorWithSetInfo(ev_set->e_di->d_handle, ev_set->e_group.get(),
                                  "Can't print event %d\n",
                                  ev_set->e_ev->ev_event);
            break;
    }

    // If XML mode is enabled, we need to ask the dbgclient layer if the event
    // we just received changed the status. If it did, we want to add the status
    // information to the xml output.
    if (Client.XML_mode) {
        // used for checking status
        rec_set_t *rec_set;
        rep_set_t *rep_set;
        char *set_str;
        dbgstatus_t *s;
        char *status_block;

        if (DbgCheckForStatusChange()) {
            // Status has changed. We need to generate updated status
            // information - this only impacts a single di

            // create the rec_set - FOR THE ENTIRE DI!!!
            // Bugfix for BUG 814782
            rec_set = NewRecSet();
            DASSERT_NOT_NULL(ev_set->e_di);
            AddReceiverToSet(
                rec_set,
                NewGroupReceiver(ev_set->e_di, ev_set->e_di->d_bitset_full.get()));

            // Query the new status from the backends, note that for this we
            // want to grab the location information
            rep_set = DbgStatus(rec_set, 1);

            ProcessErrorInRepSet(rep_set);

            // Start a new status block
            status_block = XMLStartStatusBlock(ev_set->e_di->d_handle);

            // Process each status message and add it to the status block
            for (auto&& reply : rep_set) {
                // First create the bitset string
                auto bitset_str = reply->rep_group->toSetString();

                // Create the set string
                Asprintf(&set_str, "%s%s", reply->rep_di->d_handle, bitset_str.c_str());

                // point at the status data
                s = EVENT_STATUS(reply->rep_event);

                // The XML status that is printed is based on the status value
                XMLAddStatus(&status_block, set_str, s->stat, s->stat_loc);

                // Done with the set_str
                Free(set_str);
            }

            // Print the entire status block, this will free the string
            XMLPrintStatusBlock(status_block);

            // Free the reply set
            FreeRepSet(rep_set);

            // Done with the rec_set
            FreeRecSet(rec_set);

            // Reset the status change since we handled it
            DbgResetStatusChange();
        }
    }

    cli_restore_stream();

    return 1;
}
