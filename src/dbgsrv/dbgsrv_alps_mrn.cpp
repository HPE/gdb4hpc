/*********************************************************************************\
 * dbgsrv_alps.c - A wrapper program to launch guard dbgsrv's onto the compute
 *		 nodes.
 *
 * Copyright 2011-2021 Hewlett Packard Enterprise Development LP.
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

#include <getopt.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fstream>
#include <vector>

#include "common_tools_be.h"
#include "defs.h"
#include "useful.h"
#include "unit_tests.h"

#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"

#define DBGSRV "dbgsrv"
#define PID_BUFSIZ 64

#define DBGSRV_OPT_PID "-i"
#define DBGSRV_OPT_HOST "-h"
#define DBGSRV_OPT_PORT "-c"
#define DBGSRV_OPT_ARCH "-a"
#define DBGSRV_OPT_PROTO "-p"
#define DBGSRV_OPT_DBG "-e"
#define DBGSRV_OPT_PROC "-n"
#define DBGSRV_OPT_PROG "-f"
#define DBGSRV_OPT_GPU "--gpu"
#define DBGSRV_OPT_DEBUG "--debug"
#define DBGSRV_OPT_NON_MPI "--non-mpi"
#define DBGSRV_OPT_EARLY_ENTRY "--early-entry"
#define DBGSRV_OPT_NON_STOP "--non-stop"
#define DBGSRV_OPT_GDB "--gdb"

/* #define UDASRV "udaserver"
#define UDASRV_ARGC 2

#define UDASRV_ARGV_NAME 0
#define UDASRV_ARGV_NULL 1

#define UDASRV_ENV_SERVICE "UDA_SERVICE"
#define UDASRV_ENV_PLUGIN "UDA_PLUGIN_LIBRARY"

#define UDA_SERVICE "/uda_test"
#define UDA_PLUGIN "uda-plugin-cray-upc.so"*/

static int debug_flag = 0;
static int gpu_flag = 0;
static int early_entry_flag = 0;
static int non_stop_flag = 0;
static int non_mpi_flag = 0;

const struct option long_opts[] = {
    {"callbackHost", required_argument, 0, 'h'},
    {"callbackPort", required_argument, 0, 'c'},
    {"networkArch", required_argument, 0, 'a'},
    {"networkProtocol", required_argument, 0, 'p'},
    {"dbgBackend", required_argument, 0, 'e'},
    {"help", no_argument, 0, 'x'},
    {"gpu", no_argument, &gpu_flag, 1},
    {"debug", no_argument, &debug_flag, 1},
    {"non-mpi", no_argument, &non_mpi_flag, 1},
    {"early-entry", no_argument, &early_entry_flag, 1},
    {"non-stop", no_argument, &non_stop_flag, 1},
    {"gdb", required_argument, 0, 'b'},
    {"program", required_argument, 0, 'f'},
    {0, 0, 0, 0}};

void DVPRINT(const char *format, ...) {
    va_list arg_list;

    if (!debug_flag) return;

    // setup the va_list
    va_start(arg_list, format);

    // vfprintf the line we recieved.
    vfprintf(stderr, format, arg_list);

    // end the va_list
    va_end(arg_list);
}

void usage(char *name) {
    fprintf(stdout, "Usage: %s [OPTIONS]...\n", name);
    fprintf(stdout, "Launch guard debug servers on compute nodes.\n");
    fprintf(stdout, "\t-h, --callbackHost     Front end hostname\n");
    fprintf(stdout, "\t-c, --callbackPort     Front end callback port\n");
    fprintf(stdout, "\t-a, --networkArch      Network architecture\n");
    fprintf(stdout, "\t-p, --networkProtocol  Network protocol\n");
    fprintf(stdout, "\t-e, --dbgBackend       Debugging program backend\n");
    fprintf(stdout, "\t    --gpu              Turn on gpu mode.\n");
    fprintf(stdout, "\t    --early-entry      Turn on early entry mode.\n");
    fprintf(stdout, "\t    --debug            Turn on debug logging.\n");
    fprintf(stdout, "\t    --non-mpi          Start a non-mpi application.\n");
    fprintf(stdout, "\t-x, --help             Display this text and exit\n");
    fprintf(stdout, "\t-f, --program          Program that the debugger will launch/attach to.\n");
}

int main(int argc, char **argv) {
    // dbgsrv arguments
    int opt_ind = 0;
    int c;
    char *callbackHost = NULL;
    char *callbackPort = NULL;
    char *networkArch = NULL;
    char *networkProto = NULL;
    char *dbgBackend = NULL;
    char *prog = NULL;
    char* gdb = nullptr;
    char pidbuff[PID_BUFSIZ];
    // node specific info
    cti_pidList_t *myPids;
    // dbgsrv info
    int i;
    char **tmp;
    pid_t mypid;
/* #if 1
    // TODO: UPC addition here
    // udaserver info
    // char **		udasrv_argv;
    char *scratch;
    char *uda_str;
    int len;
// End UPC addition
#endif*/

    if (argc >= 2 && strcmp(argv[1], "--has-tests") == 0) {
        return hasUnitTests();
    }

    if (argc >= 2 && strcmp(argv[1], "--test") == 0) {
        argv[1] = argv[0];
        return runUnitTests(argc - 1, &argv[1]);
    }

#ifdef DEBUG
    debug_flag = 1;
#endif

    if (debug_flag) {
        // print out the argv
        DVPRINT("dbgsrv_alps argv: \n");
        tmp = argv;
        while (*tmp != NULL) {
            DVPRINT("%s ", *tmp++);
        }
        DVPRINT("\n");
    }

    DVPRINT("Parsing longopts...\n");

    while ((c = getopt_long(argc, argv, "h:c:a:p:e:f:x", long_opts, &opt_ind)) !=
           -1) {
        switch (c) {
            case 0:
                // if this is a flag, do nothing
                break;
            case 'h':
                if (optarg == NULL || *optarg == '\0') {
                    fprintf(stderr, "Invalid callback hostname.\n");
                    return 1;
                }
                callbackHost = strdup(optarg);
                DVPRINT("callbackHost: %s\n", callbackHost);
                break;

            case 'c':
                if (optarg == NULL || *optarg == '\0') {
                    fprintf(stderr, "Invalid callback port number.\n");
                    return 1;
                }
                callbackPort = strdup(optarg);
                DVPRINT("callbackPort: %s\n", callbackPort);
                break;

            case 'a':
                if (optarg == NULL || *optarg == '\0') {
                    fprintf(stderr, "Invalid network architecture.\n");
                    return 1;
                }
                networkArch = strdup(optarg);
                DVPRINT("networkArch: %s\n", networkArch);
                break;

            case 'p':
                if (optarg == NULL || *optarg == '\0') {
                    fprintf(stderr, "Invalid network protocol.\n");
                    return 1;
                }
                networkProto = strdup(optarg);
                DVPRINT("networkProtocol: %s\n", networkProto);
                break;

            case 'e':
                if (optarg == NULL || *optarg == '\0') {
                    fprintf(stderr, "Invalid debug backend.\n");
                    return 1;
                }
                dbgBackend = strdup(optarg);
                DVPRINT("dbgBackend: %s\n", dbgBackend);
                break;

            case 'x':
                usage(argv[0]);
                return 1;

            case 'b':
                fprintf(stderr, "in dbgsrv_alps_mrn.cpp: got --gdb=%s\n", optarg);
                gdb = strdup(optarg);
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
                usage(argv[0]);
                return 1;
        }
    }

    DVPRINT("Done parsing longopts.\n");

    if (debug_flag) { 
        fprintf(stderr, "optind: %d\n", optind);
        fprintf(stderr, "argv is now: ");
        // print out the argv
        auto tmp = argv;
        while (*tmp != nullptr) {
            fprintf(stderr, "%s ", *tmp++);
        }
        fprintf(stderr, "\n"); 
    }

/* #define CRAY_CCE_UPC 1
#ifdef CRAY_CCE_UPC
    // This only works with non-gpu mode
    if (!gpu_flag) {
        // Tell gdb where to find the UPC plugin
        scratch = cti_be_getLibDir();
        if (!scratch) {
            fprintf(stderr, "cti_be_getLibDir failed\n");
            return 1;
        }

        len = strlen(scratch) + strlen(UDA_PLUGIN) + 2;
        if ((uda_str = (char *)malloc(len)) == (void *)0) {
            fprintf(stderr, "UDA_PLUGIN malloc failed.\n");
            return 1;
        }
        snprintf(uda_str, len, "%s/%s", scratch, UDA_PLUGIN);

        if (setenv("UDA_GUPC_PLUGIN_LIBRARY", uda_str, 1) < 0) {
            // failure
            fprintf(stderr, "UDA_GUPC_PLUGIN_LIBRARY setenv failed");
            return 1;
        }
        free(uda_str);
    }
#endif

#if 0
// TODO: UPC addition here

	// calloc memory for the dbgsrv argv array
	if ((udasrv_argv = calloc(UDASRV_ARGC, sizeof(char *))) == (void *)0)
	{
		fprintf(stderr, "calloc failed.\n");
		return 1;
	}

	// set the static members of dbgsrv_argv
	udasrv_argv[UDASRV_ARGV_NAME] = strdup(UDASRV);
	udasrv_argv[UDASRV_ARGV_NULL] = NULL;

	// get the value of TMPDIR, this should be set by dlaunch
	scratch = getenv("TMPDIR");
	if (!scratch)
		scratch = "/tmp";

	len = strlen(scratch) + strlen(UDA_SERVICE) + 1;

	if ((uda_str = malloc(len)) == (void *)0)
	{
		fprintf(stderr, "malloc failed.\n");
		return 1;
	}

	snprintf(uda_str, len, "%s%s", scratch, UDA_SERVICE);

	// set the actual environment variable
	if (setenv(UDASRV_ENV_SERVICE, uda_str, 1) < 0)
	{
		// failure
		fprintf(stderr, "setenv failed");
		return 1;
	}

	// set the actual environment variable
	if (setenv(UDASRV_ENV_PLUGIN, UDA_PLUGIN, 1) < 0)
	{
		// failure
		fprintf(stderr, "setenv failed");
		return 1;
	}

	free(uda_str);

#ifdef DEBUG
	FILE *log = NULL;
	// create a log if debug is turned on
	if ((log = create_log(0, "udaserver")) == NULL)
	{
		fprintf(stderr, "Could not create udasrv logfile.\n");
	}
#endif

	// fork and exec
	mypid = fork();

	// error case
	if (mypid < 0)
	{
		fprintf(stderr, "Fatal fork error.\n");
		return 1;
	}

	// child case
	if (mypid == 0)
	{
#ifdef DEBUG
		// if debug is on, hook into the logfile
		if (log != NULL)
			hook_stdoe(log);
#endif
		// exec dbgsrv
		execvp(UDASRV, udasrv_argv);

		// exec shouldn't return
		fprintf(stderr, "Return from exec.\n");
		exit(1);
	}

// End UPC addition
#endif*/

    // get the app pids for this node
    if ((myPids = cti_be_findAppPids()) == NULL) {
        fprintf(stderr, "findAppPids failed.\n");
        return 1;
    }
    DVPRINT("Number of app pid_t's detected on this node: %d\n",
            myPids->numPids);

    // set the static members of dbgsrv_argv
    auto dbgsrv_argv =
        ArgvStream(DBGSRV, DBGSRV_OPT_HOST, callbackHost, DBGSRV_OPT_PORT,
                callbackPort, DBGSRV_OPT_ARCH, networkArch, DBGSRV_OPT_PROTO,
                networkProto, DBGSRV_OPT_DBG, dbgBackend);

    if (gpu_flag)         dbgsrv_argv << DBGSRV_OPT_GPU;
    if (debug_flag)       dbgsrv_argv << DBGSRV_OPT_DEBUG;
    if (non_mpi_flag)     dbgsrv_argv << DBGSRV_OPT_NON_MPI;
    if (early_entry_flag) dbgsrv_argv << DBGSRV_OPT_EARLY_ENTRY;
    if (non_stop_flag)    dbgsrv_argv << DBGSRV_OPT_NON_STOP;
    if (gdb)              dbgsrv_argv << DBGSRV_OPT_GDB << gdb;
    if (prog)             dbgsrv_argv << DBGSRV_OPT_PROG << prog;

    // forward any non-processed positional arguments to dbgsrv.
    // optind is set by getopt and points to the first unprocessed non-option
    // argument. getopt also rearranges argv so that all unprocessed non-option
    // arguments are at the end of argv, so we can just read until the end.
    for(; optind < argc; optind++) { 
        dbgsrv_argv << argv[optind];
    }

    if (debug_flag) {
        DVPRINT("dbgsrv argv is: \n");
        DVPRINT("%s\n", dbgsrv_argv.asString().c_str());
    }

    // start numPids dbgsrv instances.
    for (i = 0; i < myPids->numPids; ++i) {
        auto this_argv = ArgvStream(dbgsrv_argv);
        // write this instances rank into the pidbuff and create the str
        snprintf(pidbuff, PID_BUFSIZ, "%d", myPids->pids[i].rank);
        this_argv << DBGSRV_OPT_PROC << pidbuff;
        DVPRINT("app rank: %s\n", pidbuff);
        memset(pidbuff, 0, PID_BUFSIZ);

        // write this instances pid into the pidbuff and create the str
        snprintf(pidbuff, PID_BUFSIZ, "%d", myPids->pids[i].pid);
        this_argv << DBGSRV_OPT_PID << pidbuff;
        DVPRINT("app pid: %s\n", pidbuff);
        memset(pidbuff, 0, PID_BUFSIZ);

        if (debug_flag) {
            DVPRINT("Starting dbgsrv with argv: \n");
            DVPRINT("%s\n", this_argv.asString().c_str());
        }

#ifdef BE_VALGRIND
        FILE *valgrind_log = NULL;
        char valgrind_log_str[BUFSIZ];

        // create a log if valgrind is turned on
        if ((valgrind_log = create_log(i, "valgrind")) == NULL) {
            fprintf(stderr, "Could not create valgrind logfile.\n");
        }
        snprintf(valgrind_log_str, BUFSIZ, "--log-fd=%d", fileno(valgrind_log));

        // create an argv for valgrind
        auto valgrind_argv =
            ArgvStream("valgrind", "--read-var-info=yes", "--track-origins=yes",
                    "--leak-check=full", "--show-leak-kinds=definite",
                    "--gen-suppressions=all", valgrind_log_str);

        if (char* fileDir = cti_be_getFileDir()) {
            std::string suppressions{ std::string(fileDir) + "/" + GDB4HPC_BE_SUPPRESSIONS};
            if (std::ifstream(suppressions)) {
                suppressions = "--suppressions=" + suppressions;
                valgrind_argv << suppressions.c_str();
            }
            free(fileDir);
        }

        // Add additional valgrind options
        if (char* envArgs = getenv("GDB4HPC_VALGRIND_ARGS")) {
            valgrind_argv.parseAndAppendArgs(envArgs);
        }

        valgrind_argv << this_argv;
#endif

        // fork and exec -- the final time through we don't want to fork since
        // we only want numPids processes to exist.
        if (i < (myPids->numPids - 1)) {
            mypid = fork();
        } else {
            DVPRINT("Done forking dbgsrv...\n");
            DVPRINT("%d dbgsrv's forked in total.\n", i);
            DVPRINT("%d dbgsrv's launched in total.\n", i + 1);
            mypid = 1;
        }

        // error case
        if (mypid < 0) {
            fprintf(stderr, "Fatal fork error.\n");

            // cleanup pid list
            cti_be_destroyPidList(myPids);
#ifdef BE_VALGRIND
            // close valgrind logfile
            close_log(valgrind_log);
#endif

            return 1;
        }

        // child case - We want the child to be responsible for starting the n
        // processes. Otherwise WLM's such as slurm will fail since the WLM sees
        // the parent exit and assumes the srun is complete.
        if (mypid == 0) {
            continue;
        }

        // parent case, or final time through

        DVPRINT("Exec'ing dbgsrv...\n");
#ifdef BE_VALGRIND
        // exec valgrind
        execvp("valgrind", const_cast<char**>(valgrind_argv.c_strs().data()));
#else
        // exec dbgsrv
        execvp(DBGSRV, const_cast<char**>(this_argv.c_strs().data()));
#endif

        // exec shouldn't return
        fprintf(stderr, "Return from exec.\n");
        exit(1);
    }

    // shouldn't get here
    fprintf(stderr, "dbgsrv_alps_mrn: At impossible part of code!\n");

    // cleanup strdup'ed stuff
    Free(callbackHost);
    Free(callbackPort);
    Free(networkArch);
    Free(networkProto);
    Free(dbgBackend);
    if (prog != NULL) {
        Free(prog);
    }
    free(gdb);

    // cleanup pid list
    cti_be_destroyPidList(myPids);

    return 1;
}
