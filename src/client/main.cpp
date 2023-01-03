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

#include "unittest_drivers.h"

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
#include "script_client.h"
#include "useful.h"

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
static void readInitCommandsFile();
static void interfaceOption(const char* opt, cli_stream_args_t*& cli_args);

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
    {"interpreter", required_argument, 0, 'I'},
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
    PrintOutMsg("Copyright 2007-2022 Hewlett Packard Enterprise Development LP.\n");
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

    if (argc >= 2 && strcmp(argv[1], "--has-tests") == 0) {
        return hasUnitTests();
    }

    if (argc >= 2 && strcmp(argv[1], "--test") == 0) {
        argv[1] = argv[0];
        return runUnitTests(argc - 1, &argv[1]);
    }

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
    Client.non_stop = 1;              // Launch applications in non stop mode?
    Client.print_entry_frame = 1;     // Print frame locations at startup?
    Client.print_bp_frames = true;    // Print breakpoint frames as they are hit?
    Client.print_source_on_step = 1;  // Print source line on each step?
    Client.print_elements = 0;        // Elements to print in arrays or char * strings
    Client.print_pretty = false;      // Print indented structures
    Client.sync_wait = 0;             // Treat asynchronous cmds as sync?
    Client.sync_blocked = 0;          // Current blocked on async cmd?
    Client.proto = getDefaultProtocol(); // Default debugger protocol
    Client.arch = DBG_ARCH;           // Default starter achitecture
    Client.backend = DBG_BACKEND;     // Default debug backend
    Client.cli_fpprog = nullptr;      // only set to override the stream setting
    Client.XML_mode = 0;              // XML output mode
    Client.MI_mode = false;               // MI output mode
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
                if (strncmp(optarg, "ex=", 3) == 0) {
                    QueueCommand(optarg+3);
                } else {
                    interfaceOption(optarg, cli_args);
                }
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
            case 'I': {
                if (strcmp(optarg, "mi") == 0) {
                    Client.MI_mode = true;
                } else {
                    PrintOutError("Unrecognized interpreter: %s\n", optarg);
                }
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
            if (!Client.interactive || !isatty(STDIN_FILENO)) {
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
        exit(1);
    }

    InitProcSet();
    InitCommands();

    auto interpreter = interpreter_type::CLI;
    if (Client.XML_mode) interpreter = interpreter_type::XML;
    else if (Client.MI_mode) interpreter = interpreter_type::MI;

    InitCLI(Client.cli_mode, interpreter, cli_args);

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

    EndAll();
    DbgFinalize();
    FinishProcSet();
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
}

static void SigHandler(int sig) {
    dbgprocset_t *s;
    rec_set_t *rec_set;
    rep_set_t *rep_set;
    dbgloc_t *halt_loc;

    DPRINT("\n***In SigHandler\n");

    /*
    ** Stop the active script if there is one.
    */
    if (auto script = script::getCurrentScript()) {
        script->uninstall();
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
        PrintHaltedLocationSet(reply->rep_di.c_str(), reply->rep_group.get(),
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

// Handle the old -i/--interface option which is used for ccdb to
// specify a socket.   
void interfaceOption(const char* opt, cli_stream_args_t*& cli_args)
{
    if (opt && opt[0] == '=' && strstr(opt, "mi")) {
        Client.MI_mode = true;
        Client.silent = true;
        Client.interactive = false;
        Client.confirm = false;
        return;
    }

    // Setup things. Note that strsep modifies the string, so it
    // shouldn't use optarg directly.
    char* argstr = strdup(opt);
    char* stringp = argstr;

    // First parse the host argument
    // Make sure its not null or that it doesn't contain a null
    // terminator
    char* token;
    if ((token = strsep(&stringp, ":")) == NULL || *token == '\0') {
        PrintOutError(
            "Error:    Encountered problem with host argument.\n");
        PrintOutError(
            "Expected: --interface host:IOPort:ERRPort\n");
        PrintOutError("Found:    --interface %s\n", optarg);
        exit(1);
    }
    char* hoststr = token;
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
    char* portstr = token;
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
    char* errpstr = stringp;

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
}
