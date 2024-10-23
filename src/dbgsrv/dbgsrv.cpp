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

const char* custom_gdb = nullptr;
const char* custom_gdb_data = nullptr;

const struct option long_opts[] = {
    {"callbackHost", required_argument, 0, 'h'},
    {"callbackPort", required_argument, 0, 'c'},
    {"networkProtocol", required_argument, 0, 'p'},
    {"dbgBackend", required_argument, 0, 'e'},
    {"ranknum", required_argument, 0, 'n'},
    {"pid", required_argument, 0, 'i'},
    {"jobkey", required_argument, 0, 'k'},
    {"gpu", no_argument, &gpu_flag, 1},
    {"debug", no_argument, &debug_flag, 1},
    {"non-mpi", no_argument, &non_mpi_flag, 1},
    {"early-entry", no_argument, &early_entry_flag, 1},
    {"non-stop", no_argument, &non_stop_flag, 1},
    {"gdb", required_argument, 0, 'b'},
    {"program", required_argument, 0, 'f'},
    {"vgdb-command-file", required_argument, 0, 'v'},
    {"main-function", required_argument, 0, 'm'},
    {"python", no_argument, &python_flag, 1},
    {0, 0, 0, 0}};

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

void Usage(void);
void Server(void);
void EndServer();
char *MakeStr(int, char *[]);
int SetupProgIO(int *, int *);
int DoProgIO(int *, int);
int AllocPty(int *, char **, char **);
int AllocFifo(int *, char **, char **);

extern int errno;
extern int optind;
extern char *optarg;

int main(int argc, char *argv[], char *env[]) {
    int opt_ind = 0;
    int c;
    int cb = 0;
    char *hname = NULL;
    char *host = NULL;
    int procnum = -1;
    int port = -1;
    int pnum;
    int pid = -1;
    const char *proto = NULL;
    const char *backend = DBG_BACKEND;
    const char *prog = NULL;
    const char *jobKey = NULL;
    dbgevent_t *e = nullptr;
    char *vgdb_command_file = nullptr;
    char *main_function = nullptr;

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
#ifdef PERFLOG
    gettimeofday(&tv_srv, NULL);
#endif

#ifdef DEBUG
    // hardcode the debug_flag to 1 if built with debug turned on
    debug_flag = 1;
#endif
    free(const_cast<char*>(custom_gdb));
    custom_gdb = nullptr;

    free(const_cast<char*>(custom_gdb_data));
    custom_gdb_data = nullptr;

    DVPRINT("dbgsrv starting!\n");

    if (argc < 5) Usage();

    Argv0 = argv[0];

    while ((c = getopt_long(argc, argv, "p:h:c:n:i:e:k:f:v:", long_opts,
                            &opt_ind)) != -1) {
        switch (c) {
            case 0:
                // if this is a flag, do nothing
                break;

            case 'c':
                cb = atoi(optarg);
                break;

            case 'n':
                procnum = atoi(optarg);
                break;

            case 'i':
                pid = atoi(optarg);
                break;

            case 'e':
                backend = optarg;

                if (DbgSetBackend(backend)) {
                    fprintf(stderr, "No such debug backend \"%s\"\n", backend);
                    exit(1);
                }
                break;

            case 'h':
                host = optarg;
                break;

            case 'p':
                proto = optarg;

                if (DbgSetSrvProto(proto)) {
                    fprintf(stderr, "No such protocol \"%s\"\n", proto);
                    exit(1);
                }
                break;

            case 'k':
                jobKey = optarg;
                break;

            case 'b':
                DVPRINT("--gdb option is %s\n", optarg);
                if (char* colon = strchr(optarg, ':')) {
                    custom_gdb = strndup(optarg, colon-optarg);
                    custom_gdb_data = strdup(colon+1);
                } else {
                    custom_gdb = strdup(optarg);
                    custom_gdb_data = nullptr;
                }
                DVPRINT("custom_gdb = %s, custom_gdb_data = %s\n", custom_gdb,
                        custom_gdb_data ? custom_gdb_data : "nullptr");
                break;

            case 'f':
                if (optarg == nullptr || *optarg == '\0') {
                    fprintf(stderr, "--program passed without an argument.\n");
                    return 1;
                }
                prog = strdup(optarg);
                DVPRINT("program name: %s\n", prog);
                break;

            case 'v':
                if (optarg == nullptr || *optarg == '\0') {
                    fprintf(stderr, "--vgdb-command-file passed without an argument.\n");
                    return 1;
                }
                vgdb_command_file = strdup(optarg);
                DVPRINT("vgdb-command-file: %s\n", optarg);
                break;

            case 'm':
                if (optarg == nullptr || *optarg == '\0') {
                    fprintf(stderr, "--main-function passed without an argument.\n");
                    return 1;
                }
                main_function = strdup(optarg);
                DVPRINT("main-function: %s\n", optarg);
                break;

            default:
                Usage();
        }
    }

    if (host == NULL) {
        fprintf(stderr, "No host specified.\n");
        Usage();
    }

    if (cb == 0) {
        fprintf(stderr, "No callback specified.\n");
        Usage();
    }

#ifdef PERFLOG
    gettimeofday(&tv_init, NULL);
#endif

#ifdef BE_PERFLOG
    char *envDir;
    char logfile[PATH_MAX];

    if ((envDir = getenv(DBG_LOG_ENV_VAR)) == (char *)NULL) {
        // user didn't set the environment variable, so lets
        // default to tmp
        envDir = "/tmp";
    }

    // create the fullpath string to the log file using PATH_MAX plus a null
    // term
    snprintf(logfile, PATH_MAX + 1, "%s/ccdb_perf_backend_%d.log", envDir,
             procnum);

    DBEPERFLOG(CreateTimerLog(logfile));
#endif

    // init dbg protocol
    DBEPERFLOG(SetTimerStart("DbgInitProto"));

    // we create new argc/argv specifically for the init proto function. argv[0] 
    // should be the binary name ("dbgsrv") and the rest will be any leftover positional
    // arguments not processed by getopt above. getopt rearranges argv so that unprocessed
    // arguments are left at the end, and sets the global optind to point to the
    // first unprocessed one.

    ArgvStream protoArgv;
    protoArgv << argv[0];
    for (; optind < argc; optind++) {
        protoArgv << argv[optind];
    }

    // if procnum and pid are not already set, initProto sets them.
    if ((pnum = DbgInitProto(procnum, pid, protoArgv.argc(), protoArgv.c_strs().data(), jobKey, host, cb)) < 0) {
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
    DBEPERFLOG(SetTimerStopAndLog("DbgInitProto"));

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

    // init backend i/o
    DBEPERFLOG(SetTimerStart("DbgInitI/O"));
    if (IOBackendFD != -1 && SetupProgIO(&IOClientFD, &port) < 0) {
        DVPRINT("Could not setup program IO channel\n");
        IOBackendFD = -1;
    }
    DBEPERFLOG(SetTimerStopAndLog("DbgInitI/O"));

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
        if (vgdb_command_file == nullptr) {
            DVPRINT("dbgsrv: dbg_attach pid:%d prog:%s main_function:%s\n", pid, prog,
                (main_function) ? main_function : "(none detected)");
            DBEPERFLOG(SetTimerStart("DbgAttach"));
            e = DbgAttach(pid, prog, nullptr, env, main_function);
            DBEPERFLOG(SetTimerStopAndLog("DbgAttach"));

        // attach to remote GDB server
        } else {

            // Read connection instructions from VGDB command file
            DVPRINT("dbgsrv: reading VGDB command file at %s\n", vgdb_command_file);

            auto vgdbCommand = std::string{};
            { auto commandStream = std::ifstream{vgdb_command_file};
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

#ifdef PERFLOG
    gettimeofday(&tv_end, NULL);
#endif

    /*
    ** Exit if the attach or invoke failed.
    */
    DBEPERFLOG(SetTimerStart("DbgCallBack"));
    if (!DbgInvokeCB(host, cb, hname, pnum, procnum, port, e)) {
        return 1;
    }
    DBEPERFLOG(SetTimerStopAndLog("DbgCallBack"));

    if (e != NULL) FreeEvent(e);

    Server();
    return 0;
}

void Usage(void) {
    fprintf(stderr,
            "usage: dbgsrv -h host -c cb -f program [-p proto] [proto args]\n");
    exit(1);
}

void Server(void) {
    int fd = -1;

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
                ** Only do this if backend i/o is connected
                */
                if (IOBackendFD >= 0) {
                    /*
                    ** Check to see if there is any data from
                    ** the backend. If DoProgIO() fails then all
                    ** bets are off.
                    */
                    if (FD_ISSET(IOBackendFD, readfds)) {
                        FD_CLR(IOBackendFD, readfds);

                        if (!DoProgIO(&IOBackendFD, fd)) IOBackendFD = -1;
                    }

                    /*
                    ** Data on IOClientFD means that the client
                    ** has connected to our socket. Accept
                    ** the connection, then get rid of IOClientFD.
                    */
                    if (IOClientFD >= 0 && FD_ISSET(IOClientFD, readfds)) {
                        FD_CLR(IOClientFD, readfds);
#ifdef _AIX
                        if ((fd = accept(IOClientFD, (struct sockaddr *)NULL,
                                         (size_t *)NULL)) < 0)
#else
                        if ((fd = accept(IOClientFD, (struct sockaddr *)NULL,
                                         0)) < 0)
#endif
                            perror("accept");

                        close(IOClientFD);
                        IOClientFD = -1;
                        DVPRINT("dbgsrv: sent accept!\n");
                    }
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
** Create and bind a socket for the client to connect to.
*/
int SetupProgIO(int *cfd, int *port) { return DbgSetupIO(cfd, port); }

/*
** Transfer data from backend to socket connected to client.
*/
int DoProgIO(int *in, int out) {
    int n;
    char buf[BUFSIZ];

    if ((n = read(*in, buf, BUFSIZ)) < 0) {
        if (errno == EIO) {
            if ((*in = DbgProgIOHook(*in)) < 0) return 0;

            return 1;
        }

        perror("DoProgIO: read");
        return 0;
    }

    if (DbgTransferIO(out, buf, n) < 0) return 0;

    return 1;
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
