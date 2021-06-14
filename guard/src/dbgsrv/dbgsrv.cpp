/*********************************************************************************\
 * Debug server
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * Copyright 2010-2021 Hewlett Packard Enterprise Development LP.
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
#include <unistd.h>
#include <vector>

#include "aif.h"
#include "dbgevent.h"
#include "dbgfn.h"
#include "dbgsrv_defs.h"
#include "dbgsrv_p2p.h"
#include "dbgsrv_proto.h"
#include "defs.h"
#include "mparch.h"
#include "useful.h"

int debug_flag = 0;
int gpu_flag = 0;
int early_entry_flag = 0;
int non_stop_flag = 0;
const char* custom_gdb = nullptr;
const char* custom_gdb_data = nullptr;

const struct option long_opts[] = {
    {"callbackHost", required_argument, 0, 'h'},
    {"callbackPort", required_argument, 0, 'c'},
    {"networkArch", required_argument, 0, 'a'},
    {"networkProtocol", required_argument, 0, 'p'},
    {"dbgBackend", required_argument, 0, 'e'},
    {"ranknum", required_argument, 0, 'n'},
    {"pid", required_argument, 0, 'i'},
    {"jobkey", required_argument, 0, 'k'},
    {"gpu", no_argument, &gpu_flag, 1},
    {"debug", no_argument, &debug_flag, 1},
    {"early-entry", no_argument, &early_entry_flag, 1},
    {"non-stop", no_argument, &non_stop_flag, 1},
    {"gdb", required_argument, 0, 'b'},
    {"program", required_argument, 0, 'f'},
    {0, 0, 0, 0}};

struct mp_arch MPArchs[] = {
    {"serial", MPARCH_SERIAL},     {"alps", MPARCH_ALPS},
    {"mrnet", MPARCH_MRNET},       {"poe", MPARCH_POE},
    {"mpich", MPARCH_MPICH},       {"mpich-mpd", MPARCH_MPICH_MPD},
    {"mpich-gm", MPARCH_MPICH_GM}, {"mpich-p4", MPARCH_MPICH_P4},
    {"p4", MPARCH_P4MASTER},       {"p4slave", MPARCH_P4SLAVE},
    {"ompi", MPARCH_OMPI},         {"unknown", MPARCH_UNKNOWN}};

#define TIMEOUT_WDT 5000
#define TIMEOUT_PERIOD 1

// Need to define this for any library that is shared between the client and
// server components. We need to be able to hook into the CLI stream in the
// client, so just define this to stdout/stderr in the server.
FILE *out_stream;
FILE *err_stream;

int Shutdown = 0;
int exit_sleep_time = 0;
int WDT = 0;
int my_rank;
int sock_fd;
dbgevent_t *(*AsyncRtn)(dbgevent_t *) = NULL;
dbgevent_t *AsyncEvent;
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
void EndServer(int);
void CheckWDT(void);
char *MakeStr(int, char *[]);
int FindMPArch(char *);
int SetupProgIO(int *, int *);
int DoProgIO(int *, int);
int AllocPty(int *, char **, char **);
int AllocFifo(int *, char **, char **);

extern int errno;
extern int optind;
extern char *optarg;

int main(int argc, char *argv[], char *env[]) {
    int opt_ind = 0;
    char *ps;
    int c;
    int arch = MPARCH_UNKNOWN;
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

    // Set the streams
    out_stream = stdout;
    err_stream = stderr;

    DVPRINT("dbgsrv starting!\n");

    if (argc < 5) Usage();

    Argv0 = argv[0];

    while ((c = getopt_long(argc, argv, "p:h:c:a:n:i:e:k:f:", long_opts,
                            &opt_ind)) != -1) {
        switch (c) {
            case 0:
                // if this is a flag, do nothing
                break;
            case 'a':
                arch = FindMPArch(optarg);
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

            default:
                Usage();
        }
    }

    if (host == NULL) {
        fprintf(stderr, "No host specified.\n");
        Usage();
    }

    if (arch == MPARCH_UNKNOWN) {
        fprintf(stderr, "Unknown architecture.\n");
        Usage();
    }

    if (cb == 0) {
        fprintf(stderr, "No callback specified.\n");
        Usage();
    }

    switch (arch) {
        case MPARCH_POE:
            if ((ps = getenv("MP_CHILD")) == NULL) {
                fprintf(stderr, "AIX_POE: could not find MP_CHILD\n");
                exit(1);
            }

            procnum = atoi(ps);

            break;

        case MPARCH_MPICH:
            if ((ps = getenv("PMI_RANK")) == NULL ||
                sscanf(ps, "%d", &procnum) != 1) {
                fprintf(stderr, "MPICH: could not find process number\n");
                exit(1);
            }

            break;

        case MPARCH_MPICH_MPD:
            if ((ps = getenv("MPD_JRANK")) == NULL ||
                sscanf(ps, "%d", &procnum) != 1) {
                fprintf(stderr, "MPICH_MPD: could not find process number\n");
                exit(1);
            }

            break;

        case MPARCH_MPICH_GM:
            if ((ps = getenv("BPROC_RANK")) == NULL ||
                sscanf(ps, "%d", &procnum) != 1) {
                fprintf(stderr, "MPICH_GM: could not find process number\n");
                exit(1);
            }

            break;

        case MPARCH_MPICH_P4:
            if ((ps = getenv("BPROC_RANK")) == NULL ||
                sscanf(ps, "%d", &procnum) != 1) {
                fprintf(stderr, "MPICH_P4: could not find process number\n");
                exit(1);
            }

            break;

        case MPARCH_P4MASTER:
            procnum = 0;

            break;

        case MPARCH_P4SLAVE:
            if (pid < 0) {
                fprintf(stderr, "P4SLAVE: no pid\n");
                exit(1);
            }

            break;

        case MPARCH_OMPI:
            if ((ps = getenv("OMPI_MCA_ns_nds_vpid")) ==
                    NULL  // for OpenMPI 1.2
                || sscanf(ps, "%d", &procnum) != 1) {
                if ((ps = getenv("OMPI_COMM_WORLD_RANK")) ==
                        NULL  // for OpenMPI 1.3 and above
                    || sscanf(ps, "%d", &procnum) != 1) {
                    fprintf(stderr, "OMPI: could not find process number\n");
                    exit(1);
                }
            }

            break;

        case MPARCH_SERIAL:
            procnum = 0;

            break;

        case MPARCH_ALPS:
            if (pid < 0) {
                fprintf(stderr, "MPARCH_ALPS: no pid\n");
                exit(1);
            }

            break;

        case MPARCH_MRNET:
            break;

        default:
            fprintf(stderr, "MP type not supported\n");
            exit(1);
    }

    // set global my_rank var here
    my_rank = procnum;
    // for P2P
#if 0
	{
		char name[MAXHOSTNAMELEN];
		char netinfo[BUFSIZ];
		int port = my_rank+10000;
		FILE *log = (FILE *)NULL;
		if ( gethostname(name, MAXHOSTNAMELEN-1) < 0 )
		{
			perror("gethostname");
			exit(1);
		}
		sprintf(netinfo, "%d:%s:%d", my_rank, name, port);
		// create a log if debug is turned on
		if ((log = open_log(0, "netinfo")) == (FILE *)NULL)
		{
//			fprintf(stderr, "Could not create dbgsrv logfile.\n");
		} else
		{
			write_log2(log, netinfo);
			close_log(log);
		}
	}
#endif
    // end here

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

    // TODO make this an argvStream

    std::vector<const char *> protoArgv;
    protoArgv.push_back(argv[0]);
    for (; optind < argc; optind++) {
        protoArgv.push_back(argv[optind]);
    }
    protoArgv.push_back(nullptr);

    if ((pnum = DbgInitProto(procnum, protoArgv.size() - 1, protoArgv.data(), jobKey, host, cb)) < 0) {
        fprintf(stderr, "Init of protocol failed\n");
        exit(1);
    }
    DBEPERFLOG(SetTimerStopAndLog("DbgInitProto"));

    // init dbg backend
    DBEPERFLOG(SetTimerStart("DbgInitBackend"));
    if ((e = DbgInit(&DbgFD, &IOBackendFD, env)) != NULL) {
        DbgInvokeCB(host, cb, hname, pnum, procnum, port, e);
        EndServer(1);
    }
    DBEPERFLOG(SetTimerStopAndLog("DbgInitBackend"));

    // init backend i/o
    DBEPERFLOG(SetTimerStart("DbgInitI/O"));
    if (IOBackendFD != -1 && SetupProgIO(&IOClientFD, &port) < 0) {
        fprintf(stderr, "Could not setup program IO channel\n");
        IOBackendFD = -1;
    }
    DBEPERFLOG(SetTimerStopAndLog("DbgInitI/O"));

#ifdef PERFLOG
    gettimeofday(&tv_io, NULL);
#endif

    // attach to or launch the application process
    if (arch == MPARCH_P4SLAVE || arch == MPARCH_ALPS) {
        // attach to application
        DVPRINT("dbgsrv: dbg_attach pid:%d prog:%s\n", pid, prog);
        DBEPERFLOG(SetTimerStart("DbgAttach"));
        e = DbgAttach(pid, prog, nullptr, env, arch);
        DBEPERFLOG(SetTimerStopAndLog("DbgAttach"));
    } else {
        if (procnum < 0 && strncmp(proto, "socket", strlen("socket")) == 0) {
            fprintf(stderr, "Invalid process number\n");
            exit(1);
        }

        e = DbgLaunch(host, cb, proto, prog, nullptr, env, arch);
    }

    if (e->ev_event != DBGEV_OK) {
        fprintf(stderr, "dbgsrv: dbg_invoke failed, error %d = %s\n",
                e->ev_event, e->ev_errstr);
        FreeEvent(e);
        EndServer(1);
    }

#ifdef PERFLOG
    gettimeofday(&tv_end, NULL);
#endif

    /*
    ** Exit if the attach or invoke failed.
    */
    DBEPERFLOG(SetTimerStart("DbgCallBack"));
    if (!DbgInvokeCB(host, cb, hname, pnum, procnum, port, e)) EndServer(1);
    DBEPERFLOG(SetTimerStopAndLog("DbgCallBack"));

    if (e != NULL) FreeEvent(e);

    Server();

    EndServer(0);

    return (0);
}

void Usage(void) {
    fprintf(stderr,
            "usage: dbgsrv -h host -c cb -a arch -f program [-p proto] [proto args]\n");
    exit(1);
}

void Server(void) {
    int fd = -1;
    int dtbsz = min(FD_SETSIZE, getdtablesize());
    fd_set *readfds;
    dbgevent_t *e;
    struct timeval tv;

    DVPRINT("dbgsrv: Server() main loop has started!\n");
    DVPRINT("dtbsz: %d\n", dtbsz);

    for (;;) {
        DVPRINT("dbgsrv: Main loop\n");
        // add the backend protocol file descriptor(s) to the set
        readfds = DbgGetFDSet();

        // add the debugger output file descriptor to the set
        if (DbgFD >= 0) FD_SET(DbgFD, readfds);

        /*
        ** If IOBackendFD >= 0 then we are reading the target output and
        ** the debugger has been told to use read it. We should read from the
        ** master side.
        */
        if (IOBackendFD >= 0) {
            FD_SET(IOBackendFD, readfds);

            /*
            ** IOClientFD is just used to set up the socket
            ** to the client. Once this has been done,
            ** it is set to -1 and not used again.
            */
            if (IOClientFD >= 0) FD_SET(IOClientFD, readfds);
        }

        // Wait for an asynchronous event if a asynch handler has been
        // installed
        if (AsyncRtn != NULL) {
            if (AsyncRtn == ASYNC_FORCE || (DbgBufferReady() > 0)) {
                if (AsyncRtn != ASYNC_FORCE)
                    e = (*AsyncRtn)(AsyncEvent);
                else
                    e = AsyncEvent;

                DVPRINT("Handle asynch callback.\n");
                DbgEventCB(e);
                DVPRINT("Asynch callback finished.\n");

                AsyncRtn = NULL;
            }
        }

        /*
        ** Must initialise timeout here, 'cause Linux
        ** trashes it.
        */
        tv.tv_sec = TIMEOUT_PERIOD;
        tv.tv_usec = 0;

        switch (select(dtbsz, readfds, NULL, NULL, &tv)) {
            case -1:
                if (errno == EINTR) {
                    if (Shutdown) break;

                    continue;
                }

                perror("select");

                return;
            case 0:
                CheckWDT();
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

void EndServer(int stat) {
    DVPRINT("EndServer called\n");
    DbgFinalize();
    DbgEndProto();

    // If exit_sleep_time is not zero, we want to sleep before calling exit.
    // This is useful when there is an automatic cleanup mechanism and we wish
    // to use that instead of attempting to cleanup ourselves.
    if (exit_sleep_time) {
        DVPRINT("Sleeping until we are killed off...\n");
        sleep(exit_sleep_time);  // to prevent frontend detects network failures
    }

    DVPRINT("Exiting...\n");

    exit(stat);
}

/*
** Reset watch dog timer
*/
void ResetWDT(void) { WDT = 0; }

/*
** Check if WDT has expired. If it has, then server should exit
** after current command. Otherwise, just increment counter.
*/
void CheckWDT(void) {
// We don't want the watchdog timer to kill us if we are running on a Cray. The
// Toolhelper is always guaranteed to clean us up.
#ifndef ALPS
    if (WDT++ < TIMEOUT_WDT)
#endif
        return;

    /*
    ** Stop process if it's running. We should detach in the case and attempt
    ** cleanup.
    */
    (void)DbgQuit(1);

    /*
    ** Shutdown server
    */
    Shutdown++;
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
** Convert a string into MP architecture type.
*/
int FindMPArch(char *arch) {
    struct mp_arch *mp;

    for (mp = MPArchs; mp->mp_arch != MPARCH_UNKNOWN; mp++) {
        if (strcmp(mp->mp_str, arch) == 0) {
            return mp->mp_arch;
        }
    }

    return MPARCH_UNKNOWN;
}

/*
** Enable async reading from debugger.
*/
void AsyncCheck(dbgevent_t *(*rtn)(dbgevent_t *), dbgevent_t *event) {
    if (AsyncRtn != NULL) /* XXX */
        return;

    AsyncRtn = rtn;
    AsyncEvent = event;
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

#ifndef GDBMI_BACKEND
// The below interface is depricated with the arrival of gdbmi.

/*
** Allocate a pseudo terminal. fd is a file descriptor
** for the master side of the pty. slave is the name of
** the slave side.
*/
int AllocPty(int *fd, char **master, char **slave) {
#ifdef HAVE_OPENPTY
    int fdmaster;
    int fdslave;
#if defined(__APPLE__) || defined(__linux__)
    static char name[BUFSIZ];

    if (openpty(&fdmaster, &fdslave, name, NULL, NULL) < 0) return -1;

    *fd = fdmaster;
    *slave = name;
#else  /* __APPLE__ || __linux__ */
    if (openpty(&fdmaster, &fdslave, NULL, NULL, NULL) < 0) return -1;

    *fd = fdmaster;
    *slave = ttyname(fdslave);
#endif /* __APPLE__ || __linux__ */

    *master = ttyname(fdmaster);

#elif defined(sun) || defined(WINDOWSNT) || defined(__linux)
    int sfd;

    *master = "/dev/ptmx";

    if ((*fd = open(*master, O_RDWR)) < 0) {
        *slave = NULL;
        return -1;
    }

    grantpt(*fd);
    unlockpt(*fd);
    *slave = ptsname(*fd);

#if defined(sun)
    /*
    ** Push terminal emulation modules. This is needed to
    ** enable canonical processing on the pty.
    */
    sfd = open(*slave, O_RDWR);
    ioctl(sfd, I_PUSH, "ptem");
    ioctl(sfd, I_PUSH, "ldterm");
#endif /* defined(sun) */

#elif defined(_AIX)
    *master = "/dev/ptc";

    if ((*fd = open(*master, O_RDWR)) < 0) return -1;

    *slave = ttyname(*fd);
#else
    int n1;
    int n2;
    char c1;
    char c2;
    int found = 0;
    char *tty = strdup("/dev/ptyxx");

    *slave = NULL;

    for (n2 = 0; !found && (c2 = "pqrstuvwxyz"[n2++]) != '\0';) {
        for (n1 = 0; (c1 = "0123456789abcdef"[n1++]) != '\0';) {
            tty[8] = c2;
            tty[9] = c1;

            if ((*fd = open(tty, O_RDWR, 0600)) >= 0) {
                found = 1;
                break;
            }
        }
    }

    if (*fd < 0) return -1;

    *master = tty;
    *slave = strdup(tty);
    (*slave)[5] = 't';
#endif /* ! HAVE_OPENPTY */

    return 0;
}

/*
** Setup a fifo for backend output.
*/
int AllocFifo(int *fd, char **master, char **slave) {
    if ((*master = tempnam("/tmp", "fifo")) == NULL)
        // if ( (*master = mkstemp("fifo")) == NULL )
        return -1;

    if (mkfifo(*master, 0600) < 0) {
        free(*master);
        return -1;
    }

    if ((*fd = open(*master, O_RDONLY | O_NDELAY)) < 0) {
        unlink(*master);
        free(*master);
        return -1;
    }

    *slave = (char *)Alloc(strlen(*master) + 2);
    strcpy(*slave, ">");
    strcat((*slave) + 1, *master);

    return 0;
}
#endif /* GDBMI_BACKEND */

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
