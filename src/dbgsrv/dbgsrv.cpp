/*********************************************************************************\
 * Debug server
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

#include "config.h"

#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/param.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <thread>
#include <unistd.h>
#include <vector>
#include <fstream>

#include "dbgevent.h"
#include "dbgfn.h"
#include "dbgsrv_defs.h"
#include "dbgsrv_p2p.h"
#include "dbgsrv_proto.h"
#include "defs.h"
#include "useful.h"

#include "unittest_drivers.h"

#include "common_tools_be.h"

int debug_flag = 0;
int gpu_flag = 0;
int early_entry_flag = 0;
int non_stop_flag = 0;
int non_mpi_flag = 0;
int python_flag = 0;
int gdbinit_flag = 0;

const char* custom_gdb = nullptr;
const char* custom_gdb_data = nullptr;

int Shutdown = 0;
int my_rank;
int sock_fd;
int DbgFD;
int IOBackendFD = -1;
int IOClientFD;
// use for P2P
appInfo_t other_app;

#ifdef PERFLOG
struct timeval tv_srv;
struct timeval tv_para;
struct timeval tv_arch;
struct timeval tv_init;
struct timeval tv_io;
struct timeval tv_end;
#endif

void Server(void);
void EndServer();
char *MakeStr(int, char *[]);
int SetupProgIO(int *, int *);
int DoProgIO(int *, int);
int AllocPty(int *, char **, char **);
int AllocFifo(int *, char **, char **);

extern int errno;
extern int optind;

const struct option long_opts[] = {
    {"proto", required_argument, 0, 'p'},
    {"backend", required_argument, 0, 'e'},
    {"gpu", no_argument, 0, 'g'},
    {"debug", no_argument, 0, 'd'},
    {"non-mpi", no_argument, 0, 'x'},
    {"early-entry", no_argument, 0, 'y'},
    {"non-stop", no_argument, 0, 'z'},
    {"gdb", required_argument, 0, 'b'},
    {"program", required_argument, 0, 'f'},
    {"vgdb-command-file", required_argument, 0, 'v'},
    {"main-function", required_argument, 0, 'm'},
    {"python", no_argument, 0, 'P'},
    {"gdbinit", no_argument, 0, 'n'},
    {0, 0, 0, 0}
};

static std::string get_optstring() {
    return optstring_from_longopts(long_opts, OptstringScanningMode::PermuteArgv, /*differentiate_missing_arg*/ false);
}

TEST_CASE("dbgsrv long opts are valid") {
    CHECK_NOTHROW(get_optstring());
}

struct DbgsrvArgv {
    std::string proto;
    bool gpu = false;
    bool debug = false;
    bool non_mpi = false;
    bool early_entry = false;
    bool non_stop = false;
    std::string custom_gdb;
    std::string program;
    std::string vgdb_command_file;
    std::string main_function;
    bool python = false;
    bool gdbinit = false;
    ArgvStream proto_argv;
};

DbgsrvArgv parse_dbgsrv_argv(int argc, char* argv[]) {
    DbgsrvArgv args = {};

    const auto optstring = get_optstring();

    int c;
    int opt_ind = 0;
    extern char *optarg;

    while ((c = getopt_long(argc, argv, optstring.c_str(), long_opts, &opt_ind)) != -1) {
        switch (c) {
            case 'p':
                args.proto = optarg;
                break;
            case 'g':
                args.gpu = true;
                break;
            case 'd':
                args.debug = true;
                break;
            case 'x':
                args.non_mpi = true;
                break;
            case 'y':
                args.early_entry = true;
                break;
            case 'z':
                args.non_stop = true;
                break;
            case 'b':
                args.custom_gdb = optarg;
                break;
            case 'm':
                args.main_function = optarg;
                break;
            case 'f':
                args.program = optarg;
                break;
            case 'v':
                args.vgdb_command_file = optarg;
                break;
            case 'P':
                args.python = true;
                break;
            case 'n':
                args.gdbinit = true;
                break;
            default:
                throw std::runtime_error(std::string{"Invalid argument:"} + char(c));
        }
    }

    // we create new argc/argv specifically for the init proto function. argv[0]
    // should be the binary name ("dbgsrv") and the rest will be any leftover positional
    // arguments not processed by getopt above. getopt rearranges argv so that unprocessed
    // arguments are left at the end, and sets the global optind to point to the
    // first unprocessed one.

    args.proto_argv << argv[0];
    for (; optind < argc; optind++) {
        args.proto_argv << argv[optind];
    }

    // validate that the required options are set
    if (args.proto.empty()) {
        // XXX: obviously since we only use zmqnet, this isn't *actually* required,
        // but enough of the dbgsrv layers still do require *something* that we keep it for now.
        throw std::runtime_error("Protocol (-p/--proto) not specified");
    }

    return args;
}

int main(int argc, char *argv[], char *env[]) {
    dbgevent_t *e = nullptr;

    // short circuit for unit tests
    if (argc >= 2 && strcmp(argv[1], "--has-tests") == 0) {
        return hasUnitTests();
    }

    if (argc >= 2 && strcmp(argv[1], "--test") == 0) {
        argv[1] = argv[0];
        return runUnitTests(argc - 1, &argv[1]);
    }

    if (const auto* seconds = ::getenv("GDB4HPC_DBGSRV_STARTUP_SLEEP")) {
        std::this_thread::sleep_for(std::chrono::seconds(::atoi(seconds)));
    }

#ifdef DEBUG
    // hardcode the debug_flag to 1 if built with debug turned on
    debug_flag = 1;
#endif

    DVPRINT("dbgsrv starting!\n");

    // parse args - invalidates argv
    auto args = parse_dbgsrv_argv(argc, argv);

    // apply args
    // FIXME: a little too many globals... eventual clean up tracked in CPE-9445
    debug_flag = args.debug;
    early_entry_flag = args.early_entry;
    non_stop_flag = args.non_stop;
    non_mpi_flag = args.non_mpi;
    python_flag = args.python;
    gdbinit_flag = args.gdbinit;

    if (DbgSetSrvProto(args.proto.c_str())) {
        fprintf(stderr, "No such protocol \"%s\"\n", args.proto.c_str());
        exit(1);
    }

    if (!args.custom_gdb.empty()) {
        DVPRINT("--gdb option is %s\n", args.custom_gdb.c_str());
        if (const char* colon = strchr(args.custom_gdb.c_str(), ':')) {
            custom_gdb = strndup(args.custom_gdb.c_str(), colon - args.custom_gdb.c_str());
            custom_gdb_data = strdup(colon+1);
        } else {
            custom_gdb = strdup(args.custom_gdb.c_str());
            custom_gdb_data = nullptr;
        }
        DVPRINT("custom_gdb = %s, custom_gdb_data = %s\n", custom_gdb,
                custom_gdb_data ? custom_gdb_data : "nullptr");
    }

    // InitProto sets procnum and pid
    int procnum = -1;
    int pid = -1;

    if (DbgInitProto(procnum, pid, args.proto_argv.argc(), args.proto_argv.c_strs().data()) < 0) {
        fprintf(stderr, "Init of protocol failed\n");
        return 1;
    }
    // procnum and pid must be set by the end of DbgInitProto
    if (procnum < 0) {
        fprintf(stderr, "no procnum (rank) after DbgInitProto.\n");
        return 1;
    }
    if (pid < 0) {
        fprintf(stderr, "no pid after DbgInitProto.\n");
        return 1;
    }

    // set global my_rank
    my_rank = procnum;

    // create log file
    auto log = std::unique_ptr<FILE, decltype(&close_log)>(nullptr, close_log);
    if (debug_flag) {
        auto nid = cti_be_getNodeHostname();
        log.reset(create_log(procnum, nid));
        if (log) {
            hook_stdoe(log.get());
        } else {
            fprintf(stderr, "Warning: Could not create log file on %s rank %d.\n", nid, procnum);
        }
        free(nid);
    }

    // ensure things are cleaned up on exit
    auto endServerOnExit = OnScopeExit(&EndServer);

    // init dbg backend
    DBEPERFLOG(SetTimerStart("DbgInitBackend"));
    if ((e = DbgInit(&DbgFD, &IOBackendFD, env))) {
        // This used to exit at this point, but since creating the protocol
        // has succeeded, we can return more information by continuing into
        // server
        DVPRINT("DbgInit failed\n");
    }
    DBEPERFLOG(SetTimerStopAndLog("DbgInitBackend"));

#ifdef PERFLOG
    gettimeofday(&tv_io, NULL);
#endif

    // attach to or launch the application process
    if (e && e->ev_event != DBGEV_OK) {
        // intentionally empty
        // Initializing the network connection succeeded but
        // initializing gdb failed; we are continuing into the
        // server loop so we can send a real failure message.
    } else {

        // attach to application directly
        if (args.vgdb_command_file.empty()) {
            DVPRINT("dbgsrv: dbg_attach pid:%d prog:%s main_function:%s\n", pid, args.program.c_str(), args.main_function.c_str());
            DBEPERFLOG(SetTimerStart("DbgAttach"));
            e = DbgAttach(pid, args.program.c_str(), nullptr, env, args.main_function);
            DBEPERFLOG(SetTimerStopAndLog("DbgAttach"));

        // attach to remote GDB server
        } else {

            // Read connection instructions from VGDB command file
            DVPRINT("dbgsrv: reading VGDB command file at %s\n", args.vgdb_command_file.c_str());

            auto vgdbCommand = std::string{};
            { auto commandStream = std::ifstream{args.vgdb_command_file};
                auto line = std::string{};
                while (std::getline(commandStream, line)) {
                    try {

                        // Parse line <rank> <command>
                        auto first_space = line.find(" ");
                        if (first_space == std::string::npos) {
                            throw std::exception();
                        }
                        auto rank = std::stoi(line.substr(0, first_space));

                        // Check for this rank
                        if (rank == my_rank) {
                            vgdbCommand = line.substr(first_space + 1);
                            break;
                        }
                    } catch (...) {
                        fprintf(stderr, "Failed to parse VGDB line: '%s'\n", line.c_str());
                        return 1;
                    }
                }
            }

            // Check command line
            if (vgdbCommand.empty()) {
                fprintf(stderr, "Failed to find rank %d in VGDB command file\n", my_rank);
                return 1;
            }

            // Run remote attach
            DVPRINT("dbgsrv: rank %d '%s'\n", my_rank, vgdbCommand.c_str());
            DBEPERFLOG(SetTimerStart("DbgTargetRemote"));
            e = DbgTargetRemote(vgdbCommand.c_str());
            DBEPERFLOG(SetTimerStopAndLog("DbgTargetRemote"));
        }
    }

    if (e->ev_event != DBGEV_OK) {
        DVPRINT("dbgsrv: dbg_invoke failed, error %d = %s\n", e->ev_event, e->ev_errstr);
    }

    if (e != NULL) FreeEvent(e);

    Server();
    return 0;
}

void Server(void) {
    DVPRINT("dbgsrv: Server() main loop has started!\n");

    for (;;) {
        for (auto* ev : DbgReleaseAsyncEvents()) {
            if (ev && ev->ev_event != DBGEV_NONE) {
                DVPRINT("Handle asynch callback.\n");
                // assuming DbgEventCB will free ev
                DbgEventCB(ev);
                DVPRINT("Asynch callback finished.\n");
            } else {
                // XXX: emulating legacy behavior. surely it would be better to just pass the
                // DBGEV_NONE to DbgEventCB and let it be handled there...
                DVPRINT("Ignoring null asynch event\n");
                FreeEvent(ev); // nullptr is safe here
            }
        }

        // get common and backend protocol file descriptors
        auto readfds = DbgGetFdSet(DbgFD, IOClientFD, IOBackendFD);

        switch (DbgCheckEvents(readfds)) {
            case -1:
                if (errno == EINTR) {
                    if (Shutdown) break;

                    continue;
                }

                perror("select");

                return;
            case 0:
                break;

            default:
                /*
                ** Check for responses from debugger.
                ** Note: non RPC fd's must be cleared from
                ** readfds before invoking svc_getreqset()
                ** otherwise problems occur.
                */

                /*
                ** Check for messages from gdb.
                */
                if (DbgFD >= 0 && FD_ISSET(DbgFD, readfds)) {
                    FD_CLR(DbgFD, readfds);

                    DVPRINT("Handle gdb output.\n");
                    if (!DbgRead(DbgFD)) {
                        (void)close(DbgFD);
                        DbgFD = -1;
                    }
                    DVPRINT("Done handling gdb output.\n");
                }

                /*
                ** Anything else will be debug proto calls.
                */
                DbgRunProto(readfds);
        }

        if (Shutdown) break;
    }
}

void EndServer() {
    // Called on scope exit from main()
    DVPRINT("EndServer called\n");

    // Put network into exit state. Network is not necessarily completely gone after this.
    DbgEndProto();

    // Disconnect from the inferior
    DbgFinalize();

    DVPRINT("Returning from EndServer...\n");
}

/*
** Turn argument list into a string.
*/
char *MakeStr(int argc, char *argv[]) {
    int i;
    int len = 0;
    char *s;

    if (argc == 0) return NULL;

    for (i = 0; i < argc; i++) len += strlen(argv[i]);

    s = (char *)Alloc(len + argc);

    *s = '\0';

    for (i = 0; i < argc; i++) {
        strcat(s, argv[i]);

        if (i < argc - 1) strcat(s, " ");
    }

    return s;
}

/*
** fprintf a line to stderr.
*/
void PrintOutError(const char *format, ...) {
    va_list arg_list;

    // vfprintf the line we recieved
    vfprintf(stderr, format, arg_list);

    // end the va_list
    va_end(arg_list);
}

/*
** fprintf a line to stdout.
*/
void PrintOutMsg(const char *format, ...) {
    va_list arg_list;

    // vfprintf the line we recieved
    vfprintf(stdout, format, arg_list);

    // end the va_list
    va_end(arg_list);
}

/*
** debug print routine that depends on debug_flag variable
*/
void _dvprint(const char *format, ...) {
    va_list arg_list;

    if (!debug_flag) return;

    struct timeval tv;
    while (gettimeofday( &tv, NULL ) == -1 ) {}

    fprintf(stderr, "%ld.%06ld: ", tv.tv_sec % 1000, tv.tv_usec);

    // setup the va_list
    va_start(arg_list, format);

    // vfprintf the line we recieved.
    vfprintf(stderr, format, arg_list);

    // end the va_list
    va_end(arg_list);
}
