/*********************************************************************************\
 * Debugger MRNet interface routines
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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <errno.h>
#include <inttypes.h>
#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string>
#include <initializer_list>
#include <fstream>

#include "Stream_C.h"
#include "aif.h"
#include "dbg_client.h"
#include "dbg_mrn_fe.h"
#include "defs.h"
#include "list.h"
#include "useful.h"

#ifdef ALPS
#include "common_tools_fe.h"
#endif /* ALPS */

// Static function prototypes
static int DbgMRNInit();
static bitset *DbgMRNSendCmd(dbginvoke_t *, bitset *, const char *);
static proto_msg_set *DbgMRNRecvReply(dbginvoke_t *, bitset *, bitset *);
static void DbgMRNDispatchEvents(dbginvoke_t *, int ev_fd);
static int DbgMRNAttach(dbginvoke_t *, proto_session *, const char *,
                        const char *, const char *, int, int);
static int DbgMRNInvoke(dbginvoke_t *, proto_session *, const char *, char **,
                        const char *, char **, const char *, const char *,
                        char **, const char *, int, int, int);
static int DbgMRNExit(dbginvoke_t *, int);
static void DbgMRNDestroyClient(dbginvoke_t *);
static int DbgAddValgrindFiles(cti_manifest_id_t mid);

// protocol specific startup event callback functions
static int cbMRNInvoke(dbginvoke_t *di, proto_msg_set *set);
static int cbMRNAttach(dbginvoke_t *di, proto_msg_set *set);

dbgclnt_proto DbgClntMRNProtoSW = {
    "mrnet",              /* sw_name */
    DbgMRNInit,           /* sw_init */
    NULL,                 /* sw_finish */
    DbgMRNSendCmd,        /* sw_send_cmd */
    DbgMRNRecvReply,      /* sw_recv_reply */
    NULL,                 /* sw_checkevents */
    DbgMRNDispatchEvents, /* sw_dispatchevents */
    // MRNet does not implement sessions. These should only
    // be used inside of the proxy protocol.
    NULL,               /* sw_create_session */
    NULL,               /* sw_save_session */
    NULL,               /* sw_cleanup_session */
    DbgMRNAttach,       /* sw_attach */
    DbgMRNInvoke,       /* sw_invoke */
    DbgMRNExit,         /* sw_exit */
    DbgMRNDestroyClient /* sw_destroyclnt */
};

static int DbgMRNInit() {
    struct stat st;
    char *mrnet_cfg_dir;
    const char *mrnet_port_base;

    // grab the config dir environment variable
    if ((mrnet_cfg_dir = getenv(GUARD_CFG_DIRECTORY)) == (char *)NULL) {
        PrintOutError(
            "Could not retrieve %s env var! Please check your environment "
            "settings.\n",
            GUARD_CFG_DIRECTORY);
        return 0;
    }

    // try to stat the directory
    if (stat(mrnet_cfg_dir, &st)) {
        // XXX: This gets created in main. This should not fail, but it is good
        // practice just to check
        PrintOutError(
            "Could not find the config directory! Ensure you have write "
            "permissions in the %s path.\n",
            mrnet_cfg_dir);
        return 0;
    }

    // Try to grab the port base environment variable
    if ((mrnet_port_base = getenv(GUARD_MRNET_PORT)) == NULL) {
        // Use the default choice
        mrnet_port_base = DEFAULT_MRNET_PORT_BASE;
    }

    // Set the MRNET_PORT_BASE env var
    setenv("MRNET_PORT_BASE", mrnet_port_base, 1);

    // Set DSL by default unless overridden by an environment variable
    setenv(LIBALPS_ENABLE_DSL, "1", 1);
    char *guard_libalps_enable_dsl = getenv(GUARD_LIBALPS_ENABLE_DSL);
    if (guard_libalps_enable_dsl != NULL) {
        if (strcmp(guard_libalps_enable_dsl, "0") == 0) {
            unsetenv(LIBALPS_ENABLE_DSL);
        }
    }

    return 1;
}

/*
** Attach to user program and build an MRNet tree.
*/
static int DbgMRNAttach(dbginvoke_t *di, proto_session *sess,
                        const char *app_ident, const char *arch,
                        const char *backend, int gpu_flag, int debug_flag) {
    char *root = NULL;
    char *cfg_dir;
    char topology_name[MAXPATHLEN];
    char connections[MAXPATHLEN];
    int num_be;
    int rtn;
    MRNet_Topology *mrn_topology;
    mrnet_type *network = NULL;
    char *request = NULL;
    char *env_var_str;
    // Set this to the number of environment variables we need to ship over.
    char *env_vars[5] = {NULL, NULL, NULL, NULL, NULL};
    int env_var_cnt = 0;
    int i;
    // init this to the default timeout
    unsigned long int mrnet_timeout = GUARD_DEFAULT_TIMEOUT;
    char **ap = NULL;
    int event_fd;
    int *ev_fd_ptr;
    int dbg = debug_flag;
    char **machine_hosts = NULL;
    char **tmp;
    cti_manifest_id_t mid;
    cti_session_id_t sid;
    cti_wlm_type_t wlm;
    char *env_base_dir = NULL;

#ifdef DEBUG
    dbg = 1;
#endif

    // ensure the dbginvoke_t object was passed down
    if (di == NULL) goto attach_error;

    // Set the initial protocol event callback to use our custom event handler
    // for launching
    di->d_proto_ev_callback = cbMRNAttach;

    // get mrnet configuration information
    if ((cfg_dir = getenv(GUARD_CFG_DIRECTORY)) == (char *)NULL) {
        PrintOutError(
            "ERROR: Cannot find %s. Please set MRNet environment variables!\n",
            GUARD_CFG_DIRECTORY);
        goto attach_error;
    }
    snprintf(topology_name, MAXPATHLEN - 1, "%s/%s", cfg_dir, MRNET_TOPOLOGY);
    snprintf(connections, MAXPATHLEN - 1, "%s/%s", cfg_dir,
             MRNET_ATTACH_CONNECTIONS);

    /*
    ** using the tools interface, we can interact with the compute nodes
    *directly
    ** this means that there is no longer any need for a starter application on
    *the
    ** front end.
    */
    PrintOutMsg("Attaching to application, please wait...\n");

    // if PERFLOG is turned on, we need to create a string to the location to
    // create the log file.
#ifdef PERFLOG
    char *envDir;
    char logfile[PATH_MAX];

    if ((envDir = getenv(DBG_LOG_ENV_VAR)) == (char *)NULL) {
        // user didn't set the environment variable, so lets
        // read the HOME variable
        if ((envDir = getenv("HOME")) == (char *)NULL) {
            // couldn't read HOME, so lets default to tmp
            envDir = "/tmp";
        }
    }

    // create the fullpath string to the log file using PATH_MAX plus a null
    // term
    snprintf(logfile, PATH_MAX + 1, "%s/mrnet_perf_frontend.log", envDir);

    DPERFLOG(CreateTimerLog(logfile));
    DPERFLOG(SetLogInfo("Attaching to processes\n"));
    DPERFLOG(SetTimerStart("Attach total"));
#endif /* PERFLOG */

    // get the wlm type
    wlm = cti_current_wlm();

    switch (wlm) {
        case CTI_WLM_SLURM: {
            const char *sep;
            uint32_t jobid;
            uint32_t stepid;
            char *eptr;

            // find '.' in app_ident
            if ((sep = strchr(app_ident, '.')) == NULL) {
                PrintOutError(
                    "Invalid slurm job step argument. Expected "
                    "<job_id>.<step_id>\n");
                goto attach_error;
            }
            // turn it into null term and point past
            //*sep++ = '\0';
            ++sep;  // point to second digit (no need to modify string, strtol
                    // will stop at '.'

            // convert the jobid
            errno = 0;
            jobid = (uint32_t)strtoul(app_ident, &eptr, 10);

            // check for error
            if ((errno == ERANGE && (jobid == ULONG_MAX)) ||
                (errno != 0 && jobid == 0)) {
                PrintOutError("Invalid slurm jobid argument.\n");
                goto attach_error;
            }

            // check for invalid input
            if (eptr == app_ident) {
                PrintOutError("Invalid slurm jobid argument.\n");
                goto attach_error;
            }

            // convert the stepid
            errno = 0;
            stepid = (uint32_t)strtoul(sep, &eptr, 10);

            // check for error
            if ((errno == ERANGE && (stepid == ULONG_MAX)) ||
                (errno != 0 && stepid == 0)) {
                PrintOutError("Invalid slurm stepid argument.\n");
                goto attach_error;
            }

            // check for invalid input
            if (eptr == sep) {
                PrintOutError("Invalid slurm stepid argument.\n");
                goto attach_error;
            }

            // Register the slurm job step with the tool interface
            cti_slurm_ops_t *ops = NULL;
            cti_wlm_type_t ret = cti_open_ops((void **)&ops);
            if (ret != CTI_WLM_SLURM) {
                PrintOutError("cti_open_ops returned invalid wlm!\n");
                goto attach_error;
            }
            if ((di->d_appid = ops->registerJobStep(jobid, stepid)) == 0) {
                PrintOutError("CTI error: %s\n", cti_error_str());
                PrintOutError("Failed to register slurm job step with CTI.\n");
                goto attach_error;
            }
        } break;
        case CTI_WLM_SSH: {
            pid_t launcher_pid;
            char *eptr;

            // convert the string to an int
            errno = 0;
            launcher_pid = strtol(app_ident, &eptr, 10);

            // check for error
            if ((launcher_pid < 0) || (errno != 0)) {
                PrintOutError("Invalid launcher pid argument.\n");
                goto attach_error;
            }

            // check for invalid input
            if (eptr == app_ident) {
                PrintOutError("Invalid launcher pid argument.\n");
                goto attach_error;
            }

            // Register the pid with the tool interface
            cti_ssh_ops_t *ops = NULL;
            cti_wlm_type_t ret = cti_open_ops((void **)&ops);
            if (ret != CTI_WLM_SSH) {
                PrintOutError("cti_open_ops returned invalid wlm!\n");
                goto attach_error;
            }
            if ((di->d_appid = ops->registerJob(launcher_pid)) == 0) {
                PrintOutError("CTI error: %s\n", cti_error_str());
                PrintOutError("Failed to register launcher pid with CTI.\n");
                goto attach_error;
            }
        } break;
        default:
            PrintOutError("Unsupported wlm %s\n", cti_wlm_type_toString(wlm));
            goto attach_error;
    }

    // TODO: If we really wanted to, we could attach to the existing aprun
    // process here and redirect its /proc stdout and stderr to our output
    // stream handler. Slurm should be trivial to implement this with sattach.
    // If this is ever done, put it in the CTI library.

    // Create a new session based on the appid. We need to remember this for
    // later
    if ((sid = cti_createSession(di->d_appid)) == 0) {
        PrintOutError("CTI error: %s\n", cti_error_str());
        PrintOutError("Failed to create a cti_session_id_t.\n");
        goto attach_error;
    }

    // Create a manifest to hold the stuff we want to ship
    if ((mid = cti_createManifest(sid)) == 0) {
        PrintOutError("CTI error: %s\n", cti_error_str());
        PrintOutError("Failed to create a cti_manifest_id_t.\n");
        goto attach_error;
    }

    // Add gdb-guard to the manifest
    if (cti_addManifestBinary(mid, "gdb-guard")) {
        PrintOutError("CTI error: %s\n", cti_error_str());
        PrintOutError("Failed to add gdb-guard to manifest.\n");
        goto attach_error;
    }

    // This checks the setting of gpu_flag to figure out if cuda-gdb should be
    // sent over
    if (gpu_flag) {
        // The backend code will determine which version of cuda-gdb to use
        if (cti_addManifestBinary(mid, "cuda101-gdb-guard")) {
            PrintOutError("CTI error: %s\n", cti_error_str());
            PrintOutError("Failed to add cuda101-gdb-guard to manifest.\n");
            goto attach_error;
        } else if (cti_addManifestBinary(mid, "ROCgdb-guard")) {
            PrintOutError("CTI error: %s\n", cti_error_str());
            PrintOutError("Failed to add ROCgdb-guard to manifest.\n");
            goto attach_error;
        }
    }

    // Add dbgsrv to the manifest
    if (cti_addManifestBinary(mid, "dbgsrv")) {
        PrintOutError("CTI error: %s\n", cti_error_str());
        PrintOutError("Failed to add dbgsrv to manifest.\n");
        goto attach_error;
    }

#if 1  // RWM
       // send over the uda plugin - only if gpu mode is currently turned off
    if (!gpu_flag) {
        if (cti_addManifestLibrary(mid, "uda-plugin-cray-upc.so")) {
            PrintOutError("CTI error: %s\n", cti_error_str());
            PrintOutError("Failed to transfer uda-plugin-cray-upc.so.\n");
            goto attach_error;
        }
    }
// End upc addition
#endif

#if 0  // use with strace for libmi
	if (cti_addManifestBinary(mid, "strace"))
	{
		PrintOutError("CTI error: %s\n", cti_error_str());
		PrintOutError("Failed to add strace to manifest.\n");

		goto attach_error;
	}
#endif

    // Add libthread_db otherwise gdb will be messed up with thread
    // support
    if (cti_addManifestLibrary(mid, "libthread_db.so.1")) {
        PrintOutError("CTI error: %s\n", cti_error_str());
        PrintOutError("Failed to add libthread_db.so.1 to manifest.\n");
        goto attach_error;
    }

#ifdef BE_VALGRIND

    if (DbgAddValgrindFiles(mid))
        goto attach_error;

#endif

    // Add the aggregation filter to the manifest
    if (cti_addManifestLibrary(mid, AGGREGATION_FILTER)) {
        PrintOutError("CTI error: %s\n", cti_error_str());
        PrintOutError("Failed to add %s to manifest.\n", AGGREGATION_FILTER);
        goto attach_error;
    }

    PrintOutMsg("Creating MRNet communication network...\n");

    // get the number of processes associated with this application
    if ((di->d_nproc = cti_getNumAppPEs(di->d_appid)) == 0) {
        PrintOutError("CTI error: %s\n", cti_error_str());
        PrintOutError("Failed to retrieve nproc.\n");
        goto attach_error;
    }

    // create the root hostname for mrnet
    if ((root = cti_getHostname()) == NULL) {
        PrintOutError("CTI error: %s\n", cti_error_str());
        PrintOutError("Failed to retrieve node hostname.\n");
        goto attach_error;
    }

    DPRINT("Application has apid[%d]\n", di->d_appid);
    DPRINT("Root has a hostname of [%s]\n", root);

    // get a list of the nid hostnames for topology creation
    if ((machine_hosts = cti_getAppHostsList(di->d_appid)) == NULL) {
        PrintOutError("CTI error: %s\n", cti_error_str());
        PrintOutError("Failed to retrieve nid hostname list.\n");
        goto attach_error;
    }

#ifdef DEBUG
    DPRINT("Application has the following nid hostnames list:\n");
    tmp = machine_hosts;
    while (*tmp != (char *)NULL) {
        DPRINT("%s ", *tmp++);
    }
    DPRINT("\n");
#endif /* DEBUG */

    // create the topology file used to create the mrnet front end network
    if (create_mrnet_topology(di->d_nproc, root, machine_hosts, 1)) {
        PrintOutError(
            "ERROR: DbgMRNAttach failed to create MRNet topology file!\n");
        goto attach_error;
    }

    // cleanup
    Free(root);
    root = NULL;
    tmp = machine_hosts;
    while (*tmp != NULL) {
        Free(*tmp++);
    }
    Free(machine_hosts);
    machine_hosts = NULL;

    DPRINT("MRNet is going to create a tree with topology[%s]\n",
           topology_name);

    DPERFLOG(SetTimerStart("Create mrnet network and ship dependencies"));

    // We let MRNet send the contents of the manifest for us
    network = mrn_fe_create_network_no_be(topology_name, &mrn_topology,
                                          di->d_appid, mid);
    if (network == NULL) {
        // error already printed
        goto attach_error;
    }

    DPERFLOG(SetTimerStopAndLog("Create mrnet network and ship dependencies"));

    DPRINT("MRNet network initialized.\n");

    // create the backend attach file which the compute nodes will use to hook
    // up to the network
    if (create_parents_info(mrn_topology, di->d_nproc) < 1) {
        PrintOutError(
            "ERROR: DbgMRNAttach cannot create infomation for MRNet "
            "attachment!\n");
        goto attach_error;
    }

    // Test if MRNet sent the manifest, if so create a new one.
    // MRNet will not ship the manifest if there are no commnodes spawned.
    if (!cti_manifestIsValid(mid)) {
        // Create a new manifest to hold the rest of the stuff we want to ship
        if ((mid = cti_createManifest(sid)) == 0) {
            PrintOutError("CTI error: %s\n", cti_error_str());
            PrintOutError("Failed to create a cti_manifest_id_t.\n");
            goto attach_error;
        }
    }

    // Add the be_attach.dat file to the manifest
    if (cti_addManifestFile(mid, connections)) {
        PrintOutError("CTI error: %s\n", cti_error_str());
        PrintOutError(
            "Failed to add backend attach information to manifest.\n");
        goto attach_error;
    }

    // Transfer and execute dbgsrv_alps with call line info:
    // dbgsrv_alps -h <cbhost> -c <cbport> -a <arch> -p <proto> -e <backend>
    // first create the argv for dbgsrv_alps
    // This now also depends on gpu_flag
    if (dbg) {
        if (gpu_flag) {
            ap = NewArgv("-h", "mrn", "-c", "-1", "-a", arch, "-p",
                         di->d_proto->sw_name, "-e", backend, "--gpu",
                         "--debug", NULL);
        } else {
            ap = NewArgv("-h", "mrn", "-c", "-1", "-a", arch, "-p",
                         di->d_proto->sw_name, "-e", backend, "--debug", NULL);
        }
    } else {
        if (gpu_flag) {
            ap = NewArgv("-h", "mrn", "-c", "-1", "-a", arch, "-p",
                         di->d_proto->sw_name, "-e", backend, "--gpu", NULL);
        } else {
            ap = NewArgv("-h", "mrn", "-c", "-1", "-a", arch, "-p",
                         di->d_proto->sw_name, "-e", backend, NULL);
        }
    }

#ifdef DEBUG
    DPRINT("Starting dbgsrv_alps with argv: \n");
    tmp = ap;
    while (*tmp != (char *)NULL) {
        DPRINT("%s ", *tmp++);
    }
    DPRINT("\n");

    // set dbg to 1 if debug is on
    dbg = 1;
#endif /* DEBUG */

    // For the attach case, we might need to send over the current value of
    // GUARD_MRNET_STARTUP_TIMEOUT in case it isn't set in the environment of
    // the application. Note that we want the environment inside the cray tool
    // interface to be set to this value.
    if ((env_var_str = getenv(GUARD_MRNET_STARTUP_TIMEOUT)) != NULL) {
        Asprintf(&env_vars[env_var_cnt++], "%s=%s", GUARD_MRNET_STARTUP_TIMEOUT,
                 env_var_str);
    }

    // ditto for GUARD_ADDITIONAL_SLEEP, we want to use the actual env var that
    // inside the cray tool interface again.
    if ((env_var_str = getenv(GUARD_ADDITIONAL_SLEEP)) != NULL) {
        Asprintf(&env_vars[env_var_cnt++], "%s=%s", GUARD_ADDITIONAL_SLEEP,
                 env_var_str);
    }

    // We should set the DBG_LOG_ENV_VAR only if debug flag is true
    if (dbg) {
        if ((env_var_str = getenv(DBG_LOG_ENV_VAR)) != NULL) {
            Asprintf(&env_vars[env_var_cnt++], "%s=%s", DBG_LOG_ENV_VAR,
                     env_var_str);
        }
    }

    // Forward GUARD_INST_DIRECTORY if set
    if ((env_base_dir = getenv(GUARD_INST_DIRECTORY)) != NULL) {
        Asprintf(&env_vars[env_var_cnt++], "%s=%s", GUARD_INST_DIRECTORY,
                 env_base_dir);
    }

    DPRINT("Launching debug servers onto compute nodes.\n");

    DPERFLOG(SetTimerStart("mrnet backend attach"));

    // Set CTI debug env var if debug is on
    if (dbg) {
        if (cti_setAttribute(CTI_DEBUG, "1")) {
            PrintOutError("Setting CTI_DEBUG failed: %s\n", cti_error_str());
        }
    }

    if (cti_execToolDaemon(mid, "dbgsrv_alps", (const char *const *)ap,
                           (const char *const *)env_vars)) {
        PrintOutError("CTI error: %s\n", cti_error_str());
        PrintOutError("Failed to launch dbgsrv_alps onto compute nodes!\n");
        goto attach_error;
    }

    // done with the argv array and env_vars
    FreeArgv(ap);
    ap = NULL;
    for (i = 0; i < env_var_cnt; ++i) {
        Free(env_vars[i]);
    }
    env_var_cnt = 0;

    // wait for the debug servers to connect to the mrnet tree
    rtn = mrn_fe_attach_be(network, di->d_nproc, mrnet_timeout);
    if (rtn) {
        if (rtn > 0) {
            PrintOutError(
                "ERROR: DbgMRNAttach failed to attach dbgsrvs before "
                "timeout!\n");
        } else {
            PrintOutError(
                "ERROR: DbgMRNAttach had null network information!\n");
        }
        goto attach_error;
    }

    DPERFLOG(SetTimerStopAndLog("mrnet backend attach"));

    DPRINT("Debug server launch completed.\n");
    DPRINT(
        "MRNet is going to create communication streams for the debugging "
        "network...\n");

    if (mrn_fe_create_streams_and_load_filter(network, AGGREGATION_FILTER,
                                              AGGREGATION_FUNC)) {
        PrintOutError(
            "ERROR: DbgMRNAttach failed to load filter functions and create "
            "streams for MRNet!\n");
        goto attach_error;
    }

    DPRINT("Debugging network startup completed.\n");

    PrintOutMsg("Finalizing setup...\n");

    mrn_fe_enable_failure_callback(network, di);

    if ((num_be = mrn_fe_get_num_of_be(network)) != di->d_nproc) {
        PrintOutError(
            "ERROR: MRNet has an incorrect number of debug servers.\n");
        PrintOutError("\tExpected [%d] found [%d]\n", di->d_nproc, num_be);
        goto attach_error;
    }

    DPRINT("num_be is %d\n", num_be);

    di->d_bitset_full = bitset_new(num_be);
    bitset_invert(di->d_bitset_full);

    di->d_bitset_launched = bitset_new(num_be);

    di->d_proto_net = network;

    DPRINT("DbgMRNAttach: MRNet initialization succeded! network:[%p]\n",
           network);

    // create the d_evfds list for the di
    di->d_evfds = NewList();

    // setup the event notification fd for the io stream
    event_fd = mrn_fe_get_io_notificationfd(network);

    // create an int to hold the io stream fd. Note that this will get free'ed
    // later on when the dbg invoke gets cleaned up.
    ev_fd_ptr = (int *)Alloc(sizeof(int));
    *ev_fd_ptr = event_fd;

    // Add the io_stream fd to the d_evfds list
    // This allows DbgCheckEvents to select on MRNet protocol events
    AddToList(di->d_evfds, (void *)ev_fd_ptr);

    // setup the event notification fd for the event stream
    event_fd = mrn_fe_get_event_notificationfd(network);

    // create an int to hold the event stream fd.
    ev_fd_ptr = (int *)Alloc(sizeof(int));
    *ev_fd_ptr = event_fd;

    // Add the event_stream fd to the d_evfds list
    // This allows DbgCheckEvents to select on MRNet protocol events
    AddToList(di->d_evfds, (void *)ev_fd_ptr);

    /*broadcast an INVOKE command to complete the attach callback*/
    request = CommandToSocProto(DBGCMD_INVOKE, num_be);

    if (mrn_fe_setup_streams(network, request)) {
        PrintOutError(
            "ERROR: MRNet cannot setup broadcast channels with dbgsrvs!\n");
        goto attach_error;
    }

    // wait for the DBG_OK response from the dbgsrv's indicating they have
    // finished starting up

    DPRINT("MRNet setup finalized.\n");
    DPRINT("Waiting for dbgsrv callbacks...\n");

    // final cleanup
    Free(request);
    request = NULL;

    DPERFLOG(SetTimerStart("Attach finalize"));

    // Here we want to wait for the dbginvoke to start as long as the callback
    // is still valid
    while (di->d_started == 0) {
        if (WaitForProtoStartupEvent(di) == -1) {
            PrintOutError("Could not receive launch callbacks!\n");
            goto attach_error;
        }
    }

    DPERFLOG(SetTimerStopAndLog("Attach finalize"));

    // All done. Everything is ready to go at this point.

    DPERFLOG(SetTimerStopAndLog("Attach total"));
    DPRINT("DbgMRNAttach completed!\n");

    return 0;

attach_error:

    if (di->d_appid != 0) {
        cti_deregisterApp(di->d_appid);
        di->d_appid = 0;
    }

    if (network != NULL) {
        mrn_fe_delete_network(network);
        // clear the network pointer
        di->d_proto_net = NULL;
    }

    if (ap != NULL) FreeArgv(ap);

    if (root != NULL) Free(root);

    if (machine_hosts != NULL) {
        tmp = machine_hosts;
        while (*tmp != NULL) {
            Free(*tmp++);
        }
        Free(machine_hosts);
    }

    for (i = 0; i < env_var_cnt; ++i) {
        Free(env_vars[i]);
    }

    if (request != NULL) Free(request);

    return 1;
}

/*
** Compatible callback routine for attaching with mrnet.
*/
static int cbMRNAttach(dbginvoke_t *di, proto_msg_set *set) {
    proto_message *msg;
    dbgevent_t *e = NULL;
    int err = 0;

    if (set == NULL) return 0;

    while ((msg = NextProtoMsgInSet(set)) != NULL) {
        // convert this protocol msg string to an event
        e = SocProtoToEvent(msg->message);

        bitset_oreq(di->d_bitset_launched, msg->group);

        if (e->ev_event != DBGEV_OK) {
            err = 1;
            PrintOutError("Received unknown event %s.\n",
                          DbgevToStr(e->ev_event));
        }

        // Done with the event, free it
        FreeEvent(e);
    }

    if (err) return 1;

    DPRINT("cbMRNAttach received responses [%s]!\n",
           bitset_to_str(di->d_bitset_launched));

    // Check if we have received a message from every rank
    if (bitset_eq(di->d_bitset_launched, di->d_bitset_full)) {
        // Reset the protocol event callback to the dbgclient default
        di->d_proto_ev_callback = DbgDispatchProtoEvents;

        // Set the started bit in the di
        di->d_started = 1;

        DPERFLOG(SetTimerStopAndLog("stream_init"));

        DPRINT("dbgsrv callback complete.\n");
        DPRINT("Releasing application from the startup barrier...\n");

        PrintOutMsg("Attach complete.\n");
    }

    return 0;
}

/*
** Invoke user program and build an MRNet tree.
*/
static int DbgMRNInvoke(dbginvoke_t *di, proto_session *sess, const char *prog,
                        char **argv, const char *arch, char **arch_args,
                        const char *arch_input, const char *backend, char **env,
                        const char *cwd, int gpu_flag, int debug_flag,
                        int early_entry_flag) {
    char sn[80];
    char *root = NULL;
    char *cfg_dir;
    char topology_name[MAXPATHLEN];
    char connections[MAXPATHLEN];
    int num_be;
    int rtn;
    MRNet_Topology *mrn_topology;
    mrnet_type *network = NULL;
    char *request = NULL;
    char **ap = NULL;
    char **tmp;
    int event_fd;
    int *ev_fd_ptr;
    g_progoutp_t *po = NULL;
    int prog_outp = -1;
    unsigned long int mrnet_timeout = GUARD_DEFAULT_TIMEOUT;
    char *env_var_str;
    int dbg = debug_flag;
    char **machine_hosts = NULL;
    cti_manifest_id_t mid;
    cti_session_id_t sid;
    char **args = NULL;
    char *env_base_dir = NULL;

#ifdef DEBUG
    dbg = 1;
#endif

    // ensure the dbginvoke_t object was passed down
    if (di == NULL) goto invoke_error;

    // Set the initial protocol event callback to use our custom event handler
    // for launching
    di->d_proto_ev_callback = cbMRNInvoke;

    // get mrnet configuration information
    if ((cfg_dir = getenv(GUARD_CFG_DIRECTORY)) == (char *)NULL) {
        PrintOutError(
            "ERROR: Cannot find %s. Please set MRNet environment variables!\n",
            GUARD_CFG_DIRECTORY);
        goto invoke_error;
    }
    snprintf(topology_name, MAXPATHLEN - 1, "%s/%s", cfg_dir, MRNET_TOPOLOGY);
    snprintf(connections, MAXPATHLEN - 1, "%s/%s", cfg_dir,
             MRNET_ATTACH_CONNECTIONS);

    /*
    ** using the tools interface, we can interact with the compute nodes
    *directly
    ** this means that there is no longer any need for a starter application on
    *the
    ** front end.
    */
    PrintOutMsg("Starting application, please wait...\n");

    // if PERFLOG is turned on, we need to create a string to the location to
    // create the log file.
#ifdef PERFLOG
    char *envDir;
    char logfile[PATH_MAX];

    if ((envDir = getenv(DBG_LOG_ENV_VAR)) == (char *)NULL) {
        // user didn't set the environment variable, so lets
        // read the HOME variable
        if ((envDir = getenv("HOME")) == (char *)NULL) {
            // couldn't read HOME, so lets default to tmp
            envDir = "/tmp";
        }
    }

    // create the fullpath string to the log file using PATH_MAX plus a null
    // term
    snprintf(logfile, PATH_MAX + 1, "%s/mrnet_perf_frontend.log", envDir);

    DPERFLOG(CreateTimerLog(logfile));
    DPERFLOG(SetLogInfo("Invoking %d processes, GPU is %s\n", di->d_nproc,
                        gpu_flag ? "on" : "off"));
    DPERFLOG(SetTimerStart("Invoke total"));
#endif /* PERFLOG */

    // create the argv for the app launcher note that here argv[0] should not be
    // the program name this is to address the fact that guard should be
    // ignorant of the starter applications real name.
    snprintf(sn, sizeof(sn), "%d", di->d_nproc);
#ifdef DEBUG
    ap = NewArgv("-n", sn, NULL);
#else
    // we want the application to be silent when not in debug mode.
    // ap = NewArgv("--quiet", "-n", sn, NULL);
    ap = NewArgv("-n", sn, NULL);
#endif /* DEBUG */

    // add the launcher args if present
    if (arch_args != NULL) {
        tmp = &arch_args[0];
        while (*tmp != (char *)NULL) {
            ap = AppendArgs(ap, *tmp++, NULL);
        }
    }

    ap = AppendArgs(ap, prog, NULL);  // and the prog
    ap = AppendArgv(ap, argv);        // plus its args

#ifdef DEBUG
    DPRINT("application launcher args: ");
    tmp = ap;
    while (*tmp != (char *)NULL) {
        DPRINT("%s ", *tmp++);
    }
    DPRINT("\n");
#endif /* DEBUG */

    // create the output stream to redirect stdout/stderr to
    // Note that this will be a g_progoutp_t in this proto definition
    if ((di->d_proto_outp = DbgCreateGenericProgOutp()) != NULL) {
        po = (g_progoutp_t *)di->d_proto_outp;
        prog_outp = po->p_o_wfd;
        // Set the protocol program output stream callback
        di->d_proto_outp_callback = DbgDispatchProtoOutput;
    }

    // Try to read the GUARD_MRNET_STARTUP_TIMEOUT and set the appropriate
    // CTI environment variables for the application.
    if ((env_var_str = getenv(GUARD_MRNET_STARTUP_TIMEOUT)) != NULL) {
        mrnet_timeout = strtoul(env_var_str, NULL, 10);
        // set the pmi_attribs parser timeout to the same value as mrnet
        // timeout.
        if (cti_setAttribute(CTI_PMI_FOPEN_TIMEOUT, env_var_str)) {
            PrintOutError("Setting CTI_PMI_FOPEN_TIMEOUT failed: %s\n",
                          cti_error_str());
        }
    }

    // ditto for GUARD_ADDITIONAL_SLEEP, we want to use the actual env var that
    // inside the cray tool interface again.
    if ((env_var_str = getenv(GUARD_ADDITIONAL_SLEEP)) != NULL) {
        if (cti_setAttribute(CTI_PMI_FOPEN_TIMEOUT, env_var_str)) {
            PrintOutError("Setting CTI_PMI_FOPEN_TIMEOUT failed: %s\n",
                          cti_error_str());
        }
    }

    DPERFLOG(SetTimerStart("application launch"));

    // launch the application using the tool interface
    // Note that the prog_outp is the WRITE END OF THE PIPE!!!!
    if ((di->d_appid = cti_launchAppBarrier((const char *const *)ap, prog_outp,
                                            prog_outp, arch_input, cwd,
                                            (const char *const *)env)) == 0) {
        int n = 0, r;
        char *read_buf;

        PrintOutError("Application launch failed.\n");

        // Check to see if we can read from the p_o_rfd fd. If we can, lets
        // print each line out as an error since it might contain useful
        // information for the user.
        if (ioctl(po->p_o_rfd, FIONREAD, &n) == 0) {
            if (n != 0) {
                // There is stuff for us to read.
                read_buf = (char *)Alloc(n + 1);
                memset(read_buf, '\0', n + 1);
                r = read(po->p_o_rfd, read_buf, n);
                if (r > 0) {
                    // TODO: Make the GUI parse for this since it is important.
                    PrintOutError(
                        "\nThe following error was returned from the "
                        "application launcher:\n%s\n",
                        read_buf);
                } else {
                    PrintOutError("CTI error: %s\n", cti_error_str());
                }
                Free(read_buf);
            } else {
                PrintOutError("CTI error: %s\n", cti_error_str());
            }
        } else {
            PrintOutError("CTI error: %s\n", cti_error_str());
        }
        goto invoke_error;
    }

    DPERFLOG(SetTimerStopAndLog("application launch"));

    // done with this ap
    FreeArgv(ap);
    ap = NULL;

    // ship over the binaries to the compute nodes

    // Create a new session based on the appid. We need to remember this for
    // later
    if ((sid = cti_createSession(di->d_appid)) == 0) {
        PrintOutError("CTI error: %s\n", cti_error_str());
        PrintOutError("Failed to create a cti_session_id_t.\n");
        goto invoke_error;
    }

    // Create a new manifest to hold the stuff we want to ship
    if ((mid = cti_createManifest(sid)) == 0) {
        PrintOutError("CTI error: %s\n", cti_error_str());
        PrintOutError("Failed to create a cti_manifest_id_t.\n");
        goto invoke_error;
    }

    // first add gdb-guard to manifest
    if (cti_addManifestBinary(mid, "gdb-guard")) {
        PrintOutError("CTI error: %s\n", cti_error_str());
        PrintOutError("Failed to add gdb-guard to manifest.\n");
        goto invoke_error;
    }

    // This checks the setting of gpu_flag to figure out if cuda-gdb should be
    // sent over
    if (gpu_flag) {
        // The backend code will determine which version of cuda-gdb to use
        if (cti_addManifestBinary(mid, "cuda101-gdb-guard")) {
            PrintOutError("CTI error: %s\n", cti_error_str());
            PrintOutError("Failed to add cuda101-gdb-guard to manifest.\n");
            goto invoke_error;
        } else if (cti_addManifestBinary(mid, "ROCgdb-guard")) {
            PrintOutError("CTI error: %s\n", cti_error_str());
            PrintOutError("Failed to add ROCgdb-guard to manifest.\n");
            goto invoke_error;
        }
    }

    // add the dbgsrv
    if (cti_addManifestBinary(mid, "dbgsrv")) {
        PrintOutError("CTI error: %s\n", cti_error_str());
        PrintOutError("Failed to add dbgsrv to manifest.\n");
        goto invoke_error;
    }

#if 1  // RWM
       // send over the uda plugin - only if gpu mode is currently turned off
    if (!gpu_flag) {
        if (cti_addManifestLibrary(mid, "uda-plugin-cray-upc.so")) {
            PrintOutError("CTI error: %s\n", cti_error_str());
            PrintOutError(
                "Failed to add uda-plugin-cray-upc.so to manifest.\n");
            goto invoke_error;
        }
    }
// End upc addition
#endif

#if 0  // use with strace for libmi
	if (cti_addManifestBinary(mid, "strace"))
	{
		PrintOutError("CTI error: %s\n", cti_error_str());
		PrintOutError("Failed to add strace to manifest.\n");

		goto invoke_error;
	}
#endif

    // Add libthread_db otherwise gdb will be messed up with thread
    // support
    if (cti_addManifestLibrary(mid, "libthread_db.so.1")) {
        PrintOutError("CTI error: %s\n", cti_error_str());
        PrintOutError("Failed to add libthread_db.so.1 to manifest.\n");
        goto invoke_error;
    }

#ifdef BE_VALGRIND
    // Add valgrind files to the manifest
    if (DbgAddValgrindFiles(mid)) {
        goto invoke_error;
    }
#endif

    // add the aggregation filter
    if (cti_addManifestLibrary(mid, AGGREGATION_FILTER)) {
        PrintOutError("CTI error: %s\n", cti_error_str());
        PrintOutError("Failed to add %s to manifest.\n", AGGREGATION_FILTER);
        goto invoke_error;
    }

    DPRINT("Application launch initialized.\n");
    PrintOutMsg("Creating MRNet communication network...\n");

    DPRINT("MRNet is creating a tree that the dbgsrvs can attach to.\n");

    // create the root hostname for mrnet
    if ((root = cti_getHostname()) == NULL) {
        PrintOutError("CTI error: %s\n", cti_error_str());
        PrintOutError("Failed to retrieve node hostname.\n");
        goto invoke_error;
    }

    DPRINT("Application has apid[%d]\n", (int)di->d_appid);
    DPRINT("Root has a hostname of [%s]\n", root);

    // get a list of the nid hostnames for topology creation
    if ((machine_hosts = cti_getAppHostsList(di->d_appid)) == (char **)NULL) {
        PrintOutError("CTI error: %s\n", cti_error_str());
        PrintOutError("Failed to retrieve nid hostname list.\n");
        goto invoke_error;
    }

#ifdef DEBUG
    DPRINT("Application has the following nid hostnames list:\n");
    tmp = machine_hosts;
    while (*tmp != (char *)NULL) {
        DPRINT("%s ", *tmp++);
    }
    DPRINT("\n");
#endif /* DEBUG */

    // create the topology file used to create the mrnet front end network
    if (create_mrnet_topology(di->d_nproc, root, machine_hosts, 1)) {
        PrintOutError(
            "ERROR: DbgMRNInvoke failed to create MRNet topology file!\n");
        goto invoke_error;
    }

    // cleanup
    Free(root);
    root = NULL;
    tmp = machine_hosts;
    while (*tmp != NULL) {
        Free(*tmp++);
    }
    Free(machine_hosts);
    machine_hosts = NULL;

    DPRINT("MRNet is going to create a tree with topology[%s]\n",
           topology_name);

    DPERFLOG(SetTimerStart("Create mrnet network and ship dependencies"));

    network = mrn_fe_create_network_no_be(topology_name, &mrn_topology,
                                          di->d_appid, mid);
    if (network == NULL) {
        // error already printed
        goto invoke_error;
    }

    DPERFLOG(SetTimerStopAndLog("Create mrnet network and ship dependencies"));

    // create the backend attach file which the compute nodes will use to hook
    // up to the network
    if (create_parents_info(mrn_topology, di->d_nproc) < 1) {
        PrintOutError(
            "ERROR: DbgMRNInvoke cannot create infomation for MRNet "
            "attachment!\n");
        goto invoke_error;
    }

    // Test if MRNet sent the manifest, if so create a new one.
    // MRNet will not ship the manifest if there are no commnodes spawned.
    if (!cti_manifestIsValid(mid)) {
        // Create a new manifest to hold the rest of the stuff we want to ship
        if ((mid = cti_createManifest(sid)) == 0) {
            PrintOutError("CTI error: %s\n", cti_error_str());
            PrintOutError("Failed to create a cti_manifest_id_t.\n");
            goto invoke_error;
        }
    }

    // Add the be_attach.dat file to the manifest
    if (cti_addManifestFile(mid, connections)) {
        PrintOutError("CTI error: %s\n", cti_error_str());
        PrintOutError("Failed to transfer backend attach information.\n");
        goto invoke_error;
    }

    // Add the pretty printers
    /* JLV 5/14/20 Commenting this out because the server side can find the printers file locally.
       Since this is just for the local install, it's better keep fewer deltas between a dev system
       and an installed one.   Keeping the code in place though, because this area is ticklish and 
       we may someday just need to push the file in every system.

    if (getenv("GDB4HPC_INSTALL_DIR")) {
        // this is a local install, ship over the pretty-printing via the
        // manifest
        std::string pdir(getenv("GDB4HPC_INSTALL_DIR"));
        pdir += "/share/gdb/python/libstdcxx/v6/";
        for (auto s : { "__init__.py", "printers.py" }) {
            auto f = pdir + s;
            if (cti_addManifestFile(mid, f.c_str())) {
                 PrintOutError("CTI warning: %s\n", cti_error_str());
                 PrintOutError("Could not add pretty print file %s to manifest\n",
                               f.c_str());
            }
        }
    }
    */  

    DPRINT("MRNet network initialized.\n");
    DPRINT("Launching debug servers onto compute nodes.\n");

    // Transfer and execute dbgsrv_alps with call line info:
    // dbgsrv_alps -h <cbhost> -c <cbport> -a <arch> -p <proto> -e <backend>
    // prog first create the argv for dbgsrv_alps This now also depends on
    // gpu_flag This now adds the early entry flag...
    // TODO: Clean this ugly thing up
    if (dbg) {
        if (gpu_flag) {
            if (early_entry_flag) {
                ap = NewArgv("-h", "mrn", "-c", "-1", "-a", arch, "-p",
                             di->d_proto->sw_name, "-e", backend, "--gpu",
                             "--debug", "--early-entry", "--", prog, NULL);
            } else {
                ap = NewArgv("-h", "mrn", "-c", "-1", "-a", arch, "-p",
                             di->d_proto->sw_name, "-e", backend, "--gpu",
                             "--debug", "--", prog, NULL);
            }
        } else {
            if (early_entry_flag) {
                ap = NewArgv("-h", "mrn", "-c", "-1", "-a", arch, "-p",
                             di->d_proto->sw_name, "-e", backend, "--debug",
                             "--early-entry", "--", prog, NULL);
            } else {
                ap = NewArgv("-h", "mrn", "-c", "-1", "-a", arch, "-p",
                             di->d_proto->sw_name, "-e", backend, "--debug",
                             "--", prog, NULL);
            }
        }
    } else {
        if (gpu_flag) {
            if (early_entry_flag) {
                ap = NewArgv("-h", "mrn", "-c", "-1", "-a", arch, "-p",
                             di->d_proto->sw_name, "-e", backend, "--gpu",
                             "--early-entry", "--", prog, NULL);
            } else {
                ap = NewArgv("-h", "mrn", "-c", "-1", "-a", arch, "-p",
                             di->d_proto->sw_name, "-e", backend, "--gpu", "--",
                             prog, NULL);
            }
        } else {
            if (early_entry_flag) {
                ap = NewArgv("-h", "mrn", "-c", "-1", "-a", arch, "-p",
                             di->d_proto->sw_name, "-e", backend,
                             "--early-entry", "--", prog, NULL);
            } else {
                ap = NewArgv("-h", "mrn", "-c", "-1", "-a", arch, "-p",
                             di->d_proto->sw_name, "-e", backend, "--", prog,
                             NULL);
            }
        }
    }

#ifdef DEBUG
    DPRINT("Starting dbgsrv_alps with argv: \n");
    tmp = ap;
    while (*tmp != (char *)NULL) {
        DPRINT("%s ", *tmp++);
    }
    DPRINT("\n");

    // set dbg to 1 if debug is on
    dbg = 1;
#endif /* DEBUG */

    // Forward GUARD_INST_DIRECTORY if set
    if ((env_base_dir = getenv(GUARD_INST_DIRECTORY)) != NULL) {
        args = (char **)malloc(2 * sizeof(char *));
        Asprintf(&args[0], "%s=%s", GUARD_INST_DIRECTORY, env_base_dir);
        args[1] = (char *)NULL;
    }

    DPERFLOG(SetTimerStart("mrnet backend attach"));

    // Set CTI debug env var if debug is on
    if (dbg) {
        if (cti_setAttribute(CTI_DEBUG, "1")) {
            PrintOutError("Setting CTI_DEBUG failed: %s\n", cti_error_str());
        }
    }

    // launch dbgsrv_alps onto the compute nodes
    if (cti_execToolDaemon(mid, "dbgsrv_alps", (const char *const *)ap,
                           (const char *const *)args)) {
        PrintOutError("CTI error: %s\n", cti_error_str());
        PrintOutError("Failed to launch dbgsrv_alps onto compute nodes!\n");
        if (args != NULL) {
            free(args[0]);
            free(args);
            args = NULL;
        }
        goto invoke_error;
    }

    if (args != NULL) {
        free(args[0]);
        free(args);
        args = NULL;
    }

    // done with the argv array
    FreeArgv(ap);
    ap = NULL;

    // wait for the debug servers to connect to the mrnet tree
    rtn = mrn_fe_attach_be(network, di->d_nproc, mrnet_timeout);
    if (rtn) {
        if (rtn > 0) {
            PrintOutError(
                "ERROR: DbgMRNInvoke failed to attach dbgsrvs before "
                "timeout!\n");
        } else {
            PrintOutError(
                "ERROR: DbgMRNInvoke had null network information!\n");
        }

        goto invoke_error;
    }

    DPERFLOG(SetTimerStopAndLog("mrnet backend attach"));

    DPRINT("Debug server launch completed.\n");
    DPRINT(
        "MRNet is going to create communication streams for the debugging "
        "network...\n");

    if (mrn_fe_create_streams_and_load_filter(network, AGGREGATION_FILTER,
                                              AGGREGATION_FUNC)) {
        PrintOutError(
            "ERROR: DbgMRNInvoke failed to load filter functions and create "
            "streams for MRNet!\n");
        goto invoke_error;
    }

    DPRINT("Debugging network startup completed.\n");
    PrintOutMsg("Finalizing setup...\n");

    mrn_fe_enable_failure_callback(network, di);

    if ((num_be = mrn_fe_get_num_of_be(network)) != di->d_nproc) {
        PrintOutError(
            "ERROR: MRNet has an incorrect number of debug servers.\n");
        PrintOutError("\tExpected [%d] found [%d]\n", di->d_nproc, num_be);
        goto invoke_error;
    }

    DPRINT("num_be is %d\n", num_be);

    // Create the full bitset based on the number of ranks
    di->d_bitset_full = bitset_new(num_be);
    bitset_invert(di->d_bitset_full);

    // Create the empty launched bitset
    di->d_bitset_launched = bitset_new(num_be);

    // Set the network object in the dbginvoke
    di->d_proto_net = network;

    DPRINT("DbgMRNInvoke: MRNet initialization succeded! network:[%p]\n",
           network);

    // create the d_evfds list for the di
    di->d_evfds = NewList();

    // setup the event notification fd for the io stream
    event_fd = mrn_fe_get_io_notificationfd(network);

    // create an int to hold the io stream fd. Note that this will get free'ed
    // later on when the dbg invoke gets cleaned up.
    ev_fd_ptr = (int *)Alloc(sizeof(int));
    *ev_fd_ptr = event_fd;

    // Add the io_stream fd to the d_evfds list
    // This allows DbgCheckEvents to select on MRNet protocol events
    AddToList(di->d_evfds, (void *)ev_fd_ptr);

    // setup the event notification fd for the event stream
    event_fd = mrn_fe_get_event_notificationfd(network);

    // create an int to hold the event stream fd.
    ev_fd_ptr = (int *)Alloc(sizeof(int));
    *ev_fd_ptr = event_fd;

    // Add the event_stream fd to the d_evfds list
    // This allows DbgCheckEvents to select on MRNet protocol events
    AddToList(di->d_evfds, (void *)ev_fd_ptr);

    // broadcast an INVOKE command to complete the invoke callback
    request = CommandToSocProto(DBGCMD_INVOKE, num_be);

    if (mrn_fe_setup_streams(network, request)) {
        PrintOutError(
            "ERROR: MRNet cannot setup broadcast channels with dbgsrvs!\n");
        goto invoke_error;
    }

    // wait for the DBG_OK response from the dbgsrv's indicating they have
    // finished starting up

    DPRINT("MRNet setup finalized.\n");
    DPRINT("Waiting for dbgsrv callbacks...\n");

    // final cleanup
    Free(request);
    request = NULL;

    DPERFLOG(SetTimerStart("Launch finalize"));

    // Here we want to wait for the dbginvoke to start as long as the callback
    // is still valid
    while (di->d_started == 0) {
        if (WaitForProtoStartupEvent(di) == -1) {
            PrintOutError("Could not receive launch callbacks!\n");
            goto invoke_error;
        }
    }

    DPERFLOG(SetTimerStopAndLog("Launch finalize"));

    // setup the program output stream handler if we are handling output - Note
    // we need to do this after the startup event completes. Otherwise the proxy
    // might segfault...
    if (po != NULL) {
        ev_fd_ptr = (int *)Alloc(sizeof(int));
        *ev_fd_ptr = po->p_o_rfd;

        // Add the p_o_rfd to the d_evfds list
        // This allows DbgCheckEvents to select on application output events
        AddToList(di->d_evfds, (void *)ev_fd_ptr);
    }

    // All done. Everything is ready to go at this point.

    DPERFLOG(SetTimerStopAndLog("Invoke total"));
    DPRINT("DbgMRNInvoke completed!\n");

    return 0;

invoke_error:

    if (ap != NULL) FreeArgv(ap);

    if (di->d_appid != 0) {
        cti_killApp(di->d_appid, SIGKILL);
        cti_deregisterApp(di->d_appid);
        di->d_appid = 0;
    }

    if (network != NULL) {
        mrn_fe_delete_network(network);
        // clear the network pointer
        di->d_proto_net = NULL;
    }

    if (machine_hosts != NULL) {
        tmp = machine_hosts;
        while (*tmp != NULL) {
            Free(*tmp++);
        }
        Free(machine_hosts);
    }

    if (request != NULL) Free(request);

    return 1;
}

/*
** Compatible callback routine for invoking with mrnet.
*/
static int cbMRNInvoke(dbginvoke_t *di, proto_msg_set *set) {
    proto_message *msg;
    dbgevent_t *e = NULL;
    int err = 0;

    if (set == NULL) return 0;

    while ((msg = NextProtoMsgInSet(set)) != NULL) {
        // convert this protocol msg string to an event
        e = SocProtoToEvent(msg->message);

        bitset_oreq(di->d_bitset_launched, msg->group);

        if (e->ev_event != DBGEV_OK) {
            err = 1;
            PrintOutError("Received unknown event %s.\n",
                          DbgevToStr(e->ev_event));
        }

        // Done with the event, free it
        FreeEvent(e);
    }

    if (err) return 1;

    DPRINT("cbMRNInvoke received responses [%s]!\n",
           bitset_to_str(di->d_bitset_launched));

    // Check if we have received a message from every rank
    if (bitset_eq(di->d_bitset_launched, di->d_bitset_full)) {
        // Reset the protocol event callback to the dbgclient default
        di->d_proto_ev_callback = DbgDispatchProtoEvents;

        // Set the started bit in the di
        di->d_started = 1;

        DPERFLOG(SetTimerStopAndLog("stream_init"));

        DPRINT("dbgsrv callback complete.\n");
        DPRINT("Releasing application from the startup barrier...\n");

#ifdef ALPS
        // tell CTI to release the application from the startup barrier
        if (cti_releaseAppBarrier(di->d_appid)) {
            PrintOutError("CTI error: %s\n", cti_error_str());
            PrintOutError(
                "Could not release application from the startup barrier.\n");
            cti_killApp(di->d_appid, SIGKILL);
            return 1;
        }
#endif

        PrintOutMsg("Launch complete.\n");
    }

    return 0;
}

/*
**********************************************************
**********************************************************
************ WRAPPERS FOR MRNet ROUTINES *****************
**********************************************************
**********************************************************
*/

static void DbgMRNDispatchEvents(dbginvoke_t *di, int ev_fd) {
    mrnet_type *network = (mrnet_type *)di->d_proto_net;
    g_progoutp_t *po = (g_progoutp_t *)di->d_proto_outp;
    proto_msg_set *io_res;
    proto_msg_set *ev_res;
    proto_message *msg;
    char *set_str;

    // dispatch application output if started
    // Note that the proxy would segfault here if we tried to handle program
    // output before the startup event completes...
    if (di->d_started && (po != NULL) && (po->p_o_rfd == ev_fd)) {
        // Call the protocol specific callback function to handle the output on
        // the stream
        (*(di->d_proto_outp_callback))(di, po->p_o_stream);
        return;
    }

    // dispatch io
    // FIXME: This isn't used on a Cray, but if it is this should be calling
    // a callback registered in the dbginvoke instead of handling it here. It's
    // not appropriate to handle client events in this layer.
    if (di->d_started &&
        ((io_res = mrn_fe_dispatch_io(network, ev_fd)) != NULL)) {
        // process each io message in the list
        while ((msg = NextProtoMsgInSet(io_res)) != NULL) {
            set_str = bitset_to_set(msg->group);
            PrintOutMsg("IO from %s: %s\n", set_str, msg->message);
            Free(set_str);
        }

        FreeProtoMsgSet(io_res);
        return;
    }

    // dispatch events
    if ((ev_res = mrn_fe_dispatch_event(network, ev_fd)) != NULL) {
        // Call the protocol specific callback function to handle this event msg
        (*(di->d_proto_ev_callback))(di, ev_res);

        // Free the message set
        FreeProtoMsgSet(ev_res);
        return;
    }
}

static bitset *DbgMRNSendCmd(dbginvoke_t *di, bitset *target,
                             const char *request) {
    proto_network network = di->d_proto_net;
    bitset *received = NULL;

    // send command only if not already in a shutdown state
    if (di->d_isShutdown) {
        return NULL;
    }

    MRNet_Stream *newEventStream = NULL;
    mrn_fe_send_command((mrnet_type *)network, target, request,
                        &newEventStream);

    received = bitset_new(bitset_size(target));

    DPRINT("INFO: Sending cmd to set[%s]\n", bitset_to_set(target));

    if (newEventStream) {
        int eventFd = stream_get_DataNotificationFd(newEventStream);

        int *eventFdPtr = (int *)Alloc(sizeof(int));
        *eventFdPtr = eventFd;
        AddToList(di->d_evfds, eventFdPtr);
    }

    return received;
}

// Here received means the bitset that represents the ranks we sent the initial
// command to. Change this to something that makes more sense.
static proto_msg_set *DbgMRNRecvReply(dbginvoke_t *di, bitset *target,
                                      bitset *received) {
    proto_network network = di->d_proto_net;
    int full = 0;
    proto_msg_set *msg_set = NULL;
    proto_message *msg;
    proto_msg_set *result = NULL;

    // recv reply only if not already in a shutdown state
    if (di->d_isShutdown) {
        return NULL;
    }

    while (!full) {
        if ((msg_set = mrn_fe_recv_reply((mrnet_type *)network, target)) ==
            NULL) {
            PrintOutError("Could not recv reply from MRNet network.\n");
            ResetProtoMsgSet(result);
            return result;
        }

        // If this is the first message set, then use this as our return type,
        // otherwise merge the set with our return type.
        if (result == NULL) {
            result = msg_set;
            // process the msg_set to get the received set
            while ((msg = NextProtoMsgInSet(msg_set)) != NULL) {
                // add this group to the received group
                bitset_oreq(received, msg->group);
            }
        } else {
            // add each message to the result
            while ((msg = RemoveProtoMsgInSet(msg_set)) != NULL) {
                // add this group to the received group
                bitset_oreq(received, msg->group);
                AddProtoMsgSet(result, msg);
            }
        }

        DPRINT("INFO: Recieved replies from set[%s]\n",
               bitset_to_str(received));

        // Check to see if we have received all responses
        full = bitset_eq(target, received);
    }

    ResetProtoMsgSet(result);

    return result;
}

/*
** DBG_EXIT: Tell server to quit immediately.
** If detach is true, then we detach from application instead of killing it.
*/
static int DbgMRNExit(dbginvoke_t *di, int detach) {
    mrnet_type *network = (mrnet_type *)di->d_proto_net;
    g_progoutp_t *po = (g_progoutp_t *)di->d_proto_outp;

    // shutdown mrnet only if not already in a shutdown state
    if (!di->d_isShutdown) {
        mrn_fe_delete_network(network);
    }

    // clear the network pointer
    di->d_proto_net = NULL;

#ifdef ALPS
    if (!detach) {
        // send a kill signal using the CTI
        if (cti_killApp(di->d_appid, SIGKILL)) {
            PrintOutError("CTI error: %s\n", cti_error_str());
            PrintOutError("Failed to kill application process!\n");
            return -1;
        }
    }

    cti_deregisterApp(di->d_appid);

    di->d_appid = 0;
#endif

    // Destroy the program output object
    DbgFreeGenericProgOutp(po);

    // clear the program output object
    di->d_proto_outp = NULL;

    return 0;
}

static void DbgMRNDestroyClient(dbginvoke_t *di) {
    mrnet_type *network = (mrnet_type *)di->d_proto_net;
    g_progoutp_t *po;

    // Check to see if the network type has been destroyed
    if (network != NULL) {
        // call the exit function to force the application to quit
        DbgMRNExit(di, 0);
    }

    // need to set po now since DbgMRNExit can modify it
    po = (g_progoutp_t *)di->d_proto_outp;

    // Check to see if the output stream has been destroyed
    if (po != NULL) {
        // Destroy the program output object
        DbgFreeGenericProgOutp(po);

        // clear the program output object
        di->d_proto_outp = NULL;
    }
}

static int DbgAddValgrindFiles(cti_manifest_id_t mid)
{
    std::string suppressionFile;
    bool hasSuppressionFile = false;

    // Add valgrind to the manifest
    if (int err = cti_addManifestBinary(mid, "valgrind")) {
        PrintOutError("CTI error: %s\n", cti_error_str());
        PrintOutError("Failed to add valgrind to manifest.\n");
        return err;
    }

    // And the suppression file if there is one
    suppressionFile = std::string(getenv("HOME")) + "/gdb4hpc_be_suppressions.txt";
    if (std::ifstream(suppressionFile.c_str())) hasSuppressionFile = true;

    if (hasSuppressionFile) {
        if (int err = cti_addManifestFile(mid, suppressionFile.c_str())) {
            PrintOutError("CTI error: %s\n", cti_error_str());
            PrintOutError("Failed to add valgrind suppression file %s to manifest.\n", suppressionFile.c_str());
            return err;
        }
    } else {
        // Print out the file we're looking for so the dev doesn't need to look it up
        PrintOutError("no suppression file found at: %s\n", suppressionFile.c_str());
    }

    return 0;
}
