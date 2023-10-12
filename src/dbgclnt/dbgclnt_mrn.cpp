/*********************************************************************************\
 * Debugger MRNet interface routines
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
#include <filesystem>
#include <future>
#include <condition_variable>

#include "dbg_client.h"
#include "dbg_mrn_network.h"
#include "defs.h"
#include "useful.h"
#include "dbgclnt_mrn.h"
#include "dbgclnt_proto_ctiship.h"

// Instantiation of the protocol - accessed via DebugClientProtocol::registry
MRNClientProtocol DbgClntProtoMRN{};

int MRNClientProtocol::init() {
    struct stat st;
    char *mrnet_cfg_dir;
    const char *mrnet_port_base;

    // grab the config dir environment variable
    if ((mrnet_cfg_dir = getenv(GUARD_CFG_DIRECTORY)) == (char *)nullptr) {
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
    if ((mrnet_port_base = getenv(GUARD_MRNET_PORT)) == nullptr) {
        // Use the default choice
        mrnet_port_base = DEFAULT_MRNET_PORT_BASE;
    }

    // Set the MRNET_PORT_BASE env var
    setenv("MRNET_PORT_BASE", mrnet_port_base, 1);

    // Adjust PATH to prepend MRNet libexec directory
    auto appendPath = [](const std::string& newDir) {
        auto oldPath = ::getenv("PATH") ? std::string{":"} + ::getenv("PATH") : "";
        auto newPath = newDir + oldPath;
        ::setenv("PATH", newPath.c_str(), 1);
    };

    // Prepend MRNet libexec directory for commnode
    if (auto mrnet_install_dir = ::getenv("MRNET_INSTALL_DIR")) {
        appendPath(std::string{mrnet_install_dir} + "/libexec");

    // Use default installation (MRNET_ROOT_DIR set at configure time)
    } else if (auto e = std::error_code(); std::filesystem::is_directory(MRNET_ROOT_DIR, e)) {
        appendPath(std::string{MRNET_ROOT_DIR} + "/libexec");

    // Print warning
    } else {
        PrintOutError("MRNet install location was not found in "
            "environment. Tool launch may fail. Load the `cray-mrnet` "
            "module, or set MRNET_INSTALL_DIR to the system MRNet "
            "installation path\n");
    }

    // Set DSL by default unless overridden by an environment variable
    setenv(LIBALPS_ENABLE_DSL, "1", 1);
    char *guard_libalps_enable_dsl = getenv(GUARD_LIBALPS_ENABLE_DSL);
    if (guard_libalps_enable_dsl != nullptr) {
        if (strcmp(guard_libalps_enable_dsl, "0") == 0) {
            unsetenv(LIBALPS_ENABLE_DSL);
        }
    }

    return 1;
}

bool MRNClientProtocol::sendCommand(DbgInvoke *di, const Bitset *target,
                                       const char *request) {
    auto &network = getNetwork<MRNetNetwork>(di);

    // send command only if not already in a shutdown state
    if (di->isShutdown()) {
        return false;
    }

    MRN::Stream *newEventStream = nullptr;
    network.send_command(target, request, &newEventStream);

    DPRINT("INFO: Sending cmd to set[%s]\n", target->toSetString().c_str());

    if (newEventStream) {
        int eventFd = newEventStream->get_DataNotificationFd();
        di->addEventFd(eventFd);
    }

    return true;
}

std::variant<ProtoMessageSet, ProtoNothing>
MRNClientProtocol::receiveReply(DbgInvoke *di, const Bitset *target, std::chrono::seconds) {
    auto &network = getNetwork<MRNetNetwork>(di);
    int full = 0;

    // recv reply only if not already in a shutdown state
    if (di->isShutdown()) {
        return ProtoNothing();
    }

    std::optional<ProtoMessageSet> result{};
    auto received = target->clone();
    received->clear();

    while (!full) {
        auto msg_set = network.recv_reply(target);
        if (!msg_set) {
            PrintOutError("Could not recv reply from MRNet network.\n");
            if (result) return std::move(*result);
            return ProtoNothing();
        }

        // If this is the first message set, then use this as our return type,
        // otherwise merge the set with our return type.
        if (!result) {
            result = std::move(msg_set);
            // process the pmsg_set to get the received set
            for (const auto &pmsg : *result) {
                // add this group to the received group
                *received |= *pmsg.getBitset();
            }
        } else {
            // add each message to the result
            for (auto &pmsg : *msg_set) {
                // add this group to the received group
                *received |= *pmsg.getBitset();
                result->emplace_back(std::move(pmsg));
            }
        }

        DPRINT("INFO: Recieved replies from set[%s]\n",
               received->toString().c_str());

        // Check to see if we have received all responses
        full = *target == *received;
    }

    if (result) return std::move(*result);
    return ProtoNothing();
}

void MRNClientProtocol::dispatchEvents(DbgInvoke *di, int ev_fd) {
    auto &network = getNetwork<MRNetNetwork>(di);
    auto* po = di->protoOutput();

    // dispatch application output if started
    if (di->isStarted() && (po != nullptr) && (po->p_o_rfd == ev_fd)) {
        // Call the protocol specific callback function to handle the output on
        // the stream
        DbgDispatchProtoOutput(di, po->p_o_stream);
        return;
    }

    // dispatch io
    // FIXME: This isn't used on a Cray, but if it is this should be calling
    // a callback registered in the dbginvoke instead of handling it here. It's
    // not appropriate to handle client events in this layer.
    {
        auto io_res = network.dispatch_io(ev_fd);
        if (di->isStarted() && io_res) {
            // process each io message in the list
            for (const auto &msg : *io_res) {
                auto msg_bitset = msg.getBitset();
                auto set_str = msg_bitset ? msg_bitset->toString() : "0:00";
                PrintOutMsg("IO from %s: %s\n", set_str.c_str(), msg.getMessage());
            }
            return;
        }
    }

    {
        auto ev_res = network.dispatch_event(ev_fd);
        // dispatch events
        if (ev_res) {
            // Call the protocol specific callback function to handle this event msg
            handleProtocolEvent(di, *ev_res);
            return;
        }
    }
}

int MRNClientProtocol::attach(DbgInvoke *di, const AttachOpts& opts) {
    char *root = nullptr;
    char *cfg_dir;
    char topology_name[MAXPATHLEN];
    char connections[MAXPATHLEN];
    int num_be;
    int rtn;
    MRN::NetworkTopology *mrn_topology = nullptr;
    std::unique_ptr<MRNetNetwork> network = nullptr;
    char *env_var_str;
    std::string request;
    // Set this to the number of environment variables we need to ship over.
    char *env_vars[5] = {nullptr, nullptr, nullptr, nullptr, nullptr};
    int env_var_cnt = 0;
    int i;
    // init this to the default timeout
    unsigned long int mrnet_timeout = GUARD_DEFAULT_TIMEOUT;
    auto argv = ArgvStream{};
    int event_fd;
    int dbg = opts.debug_flag;
    char **machine_hosts = nullptr;
    char **tmp;
    cti_manifest_id_t mid;
    cti_session_id_t sid;
    char *env_base_dir = nullptr;
    std::optional<std::string> serverGdb = {};
    auto backend = opts.backend.c_str();
    
#ifdef DEBUG
    dbg = 1;
#endif

    // ensure the dbginvoke_t object was passed down
    if (di == nullptr) goto attach_error;

    // Set the initial protocol event callback to use our custom event handler
    // for launching
    setProtocolEventHandler(new MRNAttachHandler());

    // get mrnet configuration information
    if ((cfg_dir = getenv(GUARD_CFG_DIRECTORY)) == (char *)nullptr) {
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

    if ((envDir = getenv(DBG_LOG_ENV_VAR)) == (char *)nullptr) {
        // user didn't set the environment variable, so lets
        // read the HOME variable
        if ((envDir = getenv("HOME")) == (char *)nullptr) {
            // couldn't read HOME, so lets default to tmp
            envDir = "/tmp";
        }
    }

    // create the fullpath string to the log file using PATH_MAX plus a nullptr
    // term
    snprintf(logfile, PATH_MAX + 1, "%s/mrnet_perf_frontend.log", envDir);

    DPERFLOG(CreateTimerLog(logfile));
    DPERFLOG(SetLogInfo("Attaching to processes\n"));
    DPERFLOG(SetTimerStart("Attach total"));
#endif /* PERFLOG */

    if (di->setAppId(parseAndRegisterAppId(opts.app_ident.c_str())); di->appId() == 0) {
        PrintOutError("Failed to parse appid.\n");
        goto attach_error;
    }

    // TODO: If we really wanted to, we could attach to the existing aprun
    // process here and redirect its /proc stdout and stderr to our output
    // stream handler. Slurm should be trivial to implement this with sattach.
    // If this is ever done, put it in the CTI library.

    // Create a new session based on the appid. We need to remember this for
    // later
    if ((sid = cti_createSession(di->appId())) == 0) {
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

    // first add gdb-guard to manifest
    serverGdb = shipGdb(mid, opts.gdb, opts.gdb_data_dir);
    if (!serverGdb) {
        PrintOutError("Error shipping gdb.\n");
        goto attach_error;
    }

    // This checks the setting of gpu_flag to figure out if cuda-gdb and rocgdb
    // sent over
    if (opts.gpu_flag && opts.gdb.empty()) {
        if (!shipGpuGdbs(mid))
            goto attach_error;
    }

    if (!shipCommonFiles(mid, opts.gpu_flag)) {
        PrintOutError("Error shipping comming files.\n");
        goto attach_error;
    }

    if (!shipOmpd(mid)) {
        PrintOutMsg("Failed to add OMPD support files to manifest. \
OMPD support will not be available.\n");
    }

#ifdef BE_VALGRIND

    if (addValgrindFiles(mid))
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
    if (di->setNumRanks(cti_getNumAppPEs(di->appId())); di->numRanks() == 0) {
        PrintOutError("CTI error: %s\n", cti_error_str());
        PrintOutError("Failed to retrieve nproc.\n");
        goto attach_error;
    }

    // create the root hostname for mrnet
    if ((root = cti_getHostname()) == nullptr) {
        PrintOutError("CTI error: %s\n", cti_error_str());
        PrintOutError("Failed to retrieve node hostname.\n");
        goto attach_error;
    }

    DPRINT("Application has apid[%lu]\n", di->appId());
    DPRINT("Root has a hostname of [%s]\n", root);

    // get a list of the nid hostnames for topology creation
    if ((machine_hosts = cti_getAppHostsList(di->appId())) == nullptr) {
        PrintOutError("CTI error: %s\n", cti_error_str());
        PrintOutError("Failed to retrieve nid hostname list.\n");
        goto attach_error;
    }

#ifdef DEBUG
    DPRINT("Application has the following nid hostnames list:\n");
    tmp = machine_hosts;
    while (*tmp != (char *)nullptr) {
        DPRINT("%s ", *tmp++);
    }
    DPRINT("\n");
#endif /* DEBUG */

    // create the topology file used to create the mrnet front end network
    if (create_mrnet_topology(di->numRanks(), root, machine_hosts, 1)) {
        PrintOutError(
            "ERROR: DbgMRNAttach failed to create MRNet topology file!\n");
        goto attach_error;
    }

    // cleanup
    Free(root);
    root = nullptr;
    tmp = machine_hosts;
    while (*tmp != nullptr) {
        Free(*tmp++);
    }
    Free(machine_hosts);
    machine_hosts = nullptr;

    DPRINT("MRNet is going to create a tree with topology[%s]\n",
           topology_name);

    DPERFLOG(SetTimerStart("Create mrnet network and ship dependencies"));

    // We let MRNet send the contents of the manifest for us
    network = std::unique_ptr<MRNetNetwork>
        (MRNetNetwork::create_network_no_be(topology_name, mrn_topology, di->appId(), mid));
    
    if (network == nullptr) {
        // error already printed
        goto attach_error;
    }

    DPERFLOG(SetTimerStopAndLog("Create mrnet network and ship dependencies"));

    DPRINT("MRNet network initialized.\n");

    // create the backend attach file which the compute nodes will use to hook
    // up to the network
    if (create_parents_info(mrn_topology, di->numRanks()) < 1) {
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
    // dbgsrv_alps -h <cbhost> -c <cbport> -p <proto> -e <backend>
    // first create the argv for dbgsrv_alps
    // This now also depends on gpu_flag
    if (dbg) {
        if (opts.gpu_flag) {
            argv = ArgvStream("-h", "mrn", "-c", "-1", "-p",
                              di->proto().name(), "-e", backend, "--gpu",
                              "--debug");
        } else {
            argv = ArgvStream("-h", "mrn", "-c", "-1", "-p",
                              di->proto().name(), "-e", backend, "--debug");
        }
    } else {
        if (opts.gpu_flag) {
            argv = ArgvStream("-h", "mrn", "-c", "-1", "-p",
                              di->proto().name(), "-e", backend, "--gpu");
        } else {
            argv = ArgvStream("-h", "mrn", "-c", "-1", "-p",
                         di->proto().name(), "-e", backend);
        }
    }

    if (!opts.gdb.empty() && serverGdb) argv << "--gdb" << *serverGdb;
    if (!opts.vgdb_file.empty()) argv << "--vgdb-command-file" << opts.vgdb_file;

#ifdef DEBUG
    DPRINT("Starting dbgsrv_alps with argv: \n\t%s\n", argv.asString().c_str());

    // set dbg to 1 if debug is on
    dbg = 1;
#endif /* DEBUG */

    // For the attach case, we might need to send over the current value of
    // GUARD_MRNET_STARTUP_TIMEOUT in case it isn't set in the environment of
    // the application. Note that we want the environment inside the cray tool
    // interface to be set to this value.
    if ((env_var_str = getenv(GUARD_MRNET_STARTUP_TIMEOUT)) != nullptr) {
        Asprintf(&env_vars[env_var_cnt++], "%s=%s", GUARD_MRNET_STARTUP_TIMEOUT,
                 env_var_str);
    }

    // ditto for GUARD_ADDITIONAL_SLEEP, we want to use the actual env var that
    // inside the cray tool interface again.
    if ((env_var_str = getenv(GUARD_ADDITIONAL_SLEEP)) != nullptr) {
        Asprintf(&env_vars[env_var_cnt++], "%s=%s", GUARD_ADDITIONAL_SLEEP,
                 env_var_str);
    }

    // We should set the DBG_LOG_ENV_VAR only if debug flag is true
    if (dbg) {
        if ((env_var_str = getenv(DBG_LOG_ENV_VAR)) != nullptr) {
            Asprintf(&env_vars[env_var_cnt++], "%s=%s", DBG_LOG_ENV_VAR,
                     env_var_str);
        }
    }

    // Forward GUARD_INST_DIRECTORY if set
    if ((env_base_dir = getenv(GUARD_INST_DIRECTORY)) != nullptr) {
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

    if (cti_execToolDaemon(mid, "dbgsrv_alps", argv.c_strs().data(),
                           (const char *const *)env_vars)) {
        PrintOutError("CTI error: %s\n", cti_error_str());
        PrintOutError("Failed to launch dbgsrv_alps onto compute nodes!\n");
        goto attach_error;
    }

    // done with env_vars
    for (i = 0; i < env_var_cnt; ++i) {
        Free(env_vars[i]);
    }
    env_var_cnt = 0;

    // wait for the debug servers to connect to the mrnet tree
    rtn = network->attach_be(di->numRanks(), mrnet_timeout);
    if (rtn) {
        if (rtn > 0) {
            PrintOutError(
                "ERROR: DbgMRNAttach failed to attach dbgsrvs before "
                "timeout!\n");
        } else {
            PrintOutError(
                "ERROR: DbgMRNAttach had nullptr network information!\n");
        }
        goto attach_error;
    }

    DPERFLOG(SetTimerStopAndLog("mrnet backend attach"));

    DPRINT("Debug server launch completed.\n");
    DPRINT(
        "MRNet is going to create communication streams for the debugging "
        "network...\n");

    if (network->create_streams_and_load_filter(AGGREGATION_FILTER,
                                              AGGREGATION_FUNC)) {
        PrintOutError(
            "ERROR: DbgMRNAttach failed to load filter functions and create "
            "streams for MRNet!\n");
        goto attach_error;
    }

    DPRINT("Debugging network startup completed.\n");

    PrintOutMsg("Finalizing setup...\n");

    network->enable_failure_callback(di);

    if ((num_be = network->get_num_of_be()) != di->numRanks()) {
        PrintOutError(
            "ERROR: MRNet has an incorrect number of debug servers.\n");
        PrintOutError("\tExpected [%d] found [%d]\n", di->numRanks(), num_be);
        goto attach_error;
    }

    DPRINT("num_be is %d\n", num_be);

    di->setFullBitset(std::make_unique<DenseBitset>(num_be, DenseBitset::InitialState::Full));
    di->setLaunchedBitset(std::make_unique<DenseBitset>(num_be));

    DPRINT("DbgMRNAttach: MRNet initialization succeded! network:[%p]\n",
           network.get());

    // create the d_evfds list for the di
    di->initializeEventFds();

    // setup the event notification fd for the io stream
    event_fd = network->get_io_notificationfd();

    // Add the io_stream fd to the d_evfds list
    // This allows DbgCheckEvents to select on MRNet protocol events
    di->addEventFd(event_fd);

    // setup the event notification fd for the event stream
    event_fd = network->get_event_notificationfd();

    // Add the event_stream fd to the d_evfds list
    // This allows DbgCheckEvents to select on MRNet protocol events
    di->addEventFd(event_fd);

    /*broadcast an INVOKE command to complete the attach callback*/
    request = CommandToSocProto(DBGCMD_INVOKE, num_be);

    if (network->setup_streams(request.c_str())) {
        PrintOutError(
            "ERROR: MRNet cannot setup broadcast channels with dbgsrvs!\n");
        goto attach_error;
    }

    // hand the network over to the di
    di->setNetwork(std::move(network));

    // wait for the DBG_OK response from the dbgsrv's indicating they have
    // finished starting up

    DPRINT("MRNet setup finalized.\n");
    DPRINT("Waiting for dbgsrv callbacks...\n");

    // final cleanup

    DPERFLOG(SetTimerStart("Attach finalize"));

    // Here we want to wait for the dbginvoke to start as long as the callback
    // is still valid
    while (!di->isStarted()) {
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

    if (di->appId() != 0) {
        cti_deregisterApp(di->appId());
        di->setAppId(0);
    }

    if (root != nullptr) Free(root);

    if (machine_hosts != nullptr) {
        tmp = machine_hosts;
        while (*tmp != nullptr) {
            Free(*tmp++);
        }
        Free(machine_hosts);
    }

    for (i = 0; i < env_var_cnt; ++i) {
        Free(env_vars[i]);
    }

    return 1;
}

int MRNClientProtocol::launch(DbgInvoke *di, const LaunchOpts& opts) {
    char sn[80];
    char *root = nullptr;
    char *cfg_dir;
    char topology_name[MAXPATHLEN];
    char connections[MAXPATHLEN];
    int num_be;
    int rtn;
    MRN::NetworkTopology *mrn_topology = nullptr;
    std::unique_ptr<MRNetNetwork> network = nullptr;
    auto app_argv = ArgvStream{};
    auto dbgsrv_alps_argv = ArgvStream{};
    char **tmp;
    std::string request;
    int event_fd;
    g_progoutp_t *po = nullptr;
    int prog_outp = -1;
    unsigned long int mrnet_timeout = GUARD_DEFAULT_TIMEOUT;
    char *env_var_str;
    int dbg = opts.debug_flag;
    char **machine_hosts = nullptr;
    cti_manifest_id_t mid;
    cti_session_id_t sid;
    char **args = nullptr;
    char *env_base_dir = nullptr;
    std::optional<std::string> serverGdb = {};

    auto backend = opts.backend_name;

    const auto optStr = [](const std::string& str) {
                            return str != "" ? str.c_str() : nullptr;
                        };

#ifdef DEBUG
    dbg = 1;
#endif
    // ensure the dbginvoke_t object was passed down
    if (di == nullptr) goto invoke_error;

    // Set the initial protocol event callback to use our custom event handler
    // for launching
    setProtocolEventHandler(new MRNLaunchHandler());

    // get mrnet configuration information
    if ((cfg_dir = getenv(GUARD_CFG_DIRECTORY)) == (char *)nullptr) {
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

    if ((envDir = getenv(DBG_LOG_ENV_VAR)) == (char *)nullptr) {
        // user didn't set the environment variable, so lets
        // read the HOME variable
        if ((envDir = getenv("HOME")) == (char *)nullptr) {
            // couldn't read HOME, so lets default to tmp
            envDir = "/tmp";
        }
    }

    // create the fullpath string to the log file using PATH_MAX plus a nullptr
    // term
    snprintf(logfile, PATH_MAX + 1, "%s/mrnet_perf_frontend.log", envDir);

    DPERFLOG(CreateTimerLog(logfile));
    DPERFLOG(SetLogInfo("Invoking %d processes, GPU is %s\n", di->numRanks(),
                        gpu_flag ? "on" : "off"));
    DPERFLOG(SetTimerStart("Invoke total"));
#endif /* PERFLOG */

    // create the argv for the app launcher note that here argv[0] should not be
    // the program name this is to address the fact that guard should be
    // ignorant of the starter applications real name.
    snprintf(sn, sizeof(sn), "%d", di->numRanks());

    // Non-MPMD launches will use rank count from launch command
    // MPMD rank counts must be specified for each subjob
    if (opts.job_opts.size() == 1) {
        app_argv << "-n" << di->numRanks();
    }
    { auto first = true;
        for (auto&& job_opt : opts.job_opts) {
            if (first) {
                first = false;
            } else {
               app_argv << ":";
            }
            app_argv << job_opt.launcher_args << job_opt.app_argv;
        }
    }

#ifdef DEBUG
    DPRINT("application launcher args: ");
    DPRINT("%s\n", app_argv.asString().c_str());
#endif /* DEBUG */

    // create the output stream to redirect stdout/stderr to
    // Note that this will be a g_progoutp_t in this proto definition
    if (di->initializeProtoOutput(); di->protoOutput() != nullptr) {
        po = di->protoOutput();
        prog_outp = po->p_o_wfd;
    }

    // Try to read the GUARD_MRNET_STARTUP_TIMEOUT and set the appropriate
    // CTI environment variables for the application.
    if ((env_var_str = getenv(GUARD_MRNET_STARTUP_TIMEOUT)) != nullptr) {
        mrnet_timeout = strtoul(env_var_str, nullptr, 10);
        // set the pmi_attribs parser timeout to the same value as mrnet
        // timeout.
        if (cti_setAttribute(CTI_PMI_FOPEN_TIMEOUT, env_var_str)) {
            PrintOutError("Setting CTI_PMI_FOPEN_TIMEOUT failed: %s\n",
                          cti_error_str());
        }
    }

    // ditto for GUARD_ADDITIONAL_SLEEP, we want to use the actual env var that
    // inside the cray tool interface again.
    if ((env_var_str = getenv(GUARD_ADDITIONAL_SLEEP)) != nullptr) {
        if (cti_setAttribute(CTI_PMI_FOPEN_TIMEOUT, env_var_str)) {
            PrintOutError("Setting CTI_PMI_FOPEN_TIMEOUT failed: %s\n",
                          cti_error_str());
        }
    }

    DPERFLOG(SetTimerStart("application launch"));

    // Print an informational message that launch is still pending if WLM hasn't
    // launched job within a few seconds.
    { auto waitCv = std::condition_variable{};
        auto waitMtx = std::mutex{};
        auto applicationLaunched = false;
        auto waitFuture = std::async(std::launch::async,
            [&waitCv, &waitMtx, &applicationLaunched] {

            // Wait before printing message
            auto lock = std::unique_lock<std::mutex>{waitMtx};
            if (!waitCv.wait_for(lock, std::chrono::seconds{10}, [&applicationLaunched] {
                return applicationLaunched; })) {

                PrintOutMsg("Job was submitted, but launch is still pending.\n"
                    "  Check your workload manager queue for schedule or resource conflicts.\n"
                    "  GDB4hpc will continue to wait.\n");
            }
        });

        // launch the application using the tool interface
        // Note that the prog_outp is the WRITE END OF THE PIPE!!!!
        di->setAppId(cti_launchAppBarrier(app_argv.c_strs().data(), prog_outp, prog_outp,
                                          optStr(opts.input_file_path),
                                          optStr(opts.cwd),
                                          opts.env.c_strs().data()));

        // Notify the message thread that launch has completed and that it can exit its wait
        { auto guard = std::lock_guard<std::mutex>{waitMtx};
            applicationLaunched = true;
        }
        waitCv.notify_all();
        waitFuture.get();
    }

    if (di->appId() == 0) {
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
    PrintOutMsg("Launched application...\n");

    // ship over the binaries to the compute nodes

    // Create a new session based on the appid. We need to remember this for
    // later
    if ((sid = cti_createSession(di->appId())) == 0) {
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
    serverGdb = shipGdb(mid, opts.gdb_path, opts.gdb_data_dir);
    if (!serverGdb) {
        PrintOutError("Error shipping gdb.\n");
        goto invoke_error;
    }

    // This checks the setting of gpu_flag to figure out if cuda-gdb should be
    // sent over
    if (opts.gpu_flag && opts.gdb_path.empty()) {
        if (!shipGpuGdbs(mid)) {
            goto invoke_error;
        }
    }

    if (!shipCommonFiles(mid, opts.gpu_flag)) {
        PrintOutError("Error shipping common files.\n");
        goto invoke_error;
    }

    if (!shipOmpd(mid)) {
        PrintOutMsg("Failed to add OMPD support files to manifest. \
OMPD support will not be available.\n");
    }

#ifdef BE_VALGRIND
    // Add valgrind files to the manifest
    if (addValgrindFiles(mid)) {
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
    if ((root = cti_getHostname()) == nullptr) {
        PrintOutError("CTI error: %s\n", cti_error_str());
        PrintOutError("Failed to retrieve node hostname.\n");
        goto invoke_error;
    }

    DPRINT("Application has apid[%d]\n", (int)di->appId());
    DPRINT("Root has a hostname of [%s]\n", root);

    // get a list of the nid hostnames for topology creation
    if ((machine_hosts = cti_getAppHostsList(di->appId())) == (char **)nullptr) {
        PrintOutError("CTI error: %s\n", cti_error_str());
        PrintOutError("Failed to retrieve nid hostname list.\n");
        goto invoke_error;
    }

#ifdef DEBUG
    DPRINT("Application has the following nid hostnames list:\n");
    tmp = machine_hosts;
    while (*tmp != (char *)nullptr) {
        DPRINT("%s ", *tmp++);
    }
    DPRINT("\n");
#endif /* DEBUG */

    // create the topology file used to create the mrnet front end network
    if (create_mrnet_topology(di->numRanks(), root, machine_hosts, 1)) {
        PrintOutError(
            "ERROR: DbgMRNInvoke failed to create MRNet topology file!\n");
        goto invoke_error;
    }

    // cleanup
    Free(root);
    root = nullptr;
    tmp = machine_hosts;
    while (*tmp != nullptr) {
        Free(*tmp++);
    }
    Free(machine_hosts);
    machine_hosts = nullptr;

    DPRINT("MRNet is going to create a tree with topology[%s]\n",
           topology_name);

    DPERFLOG(SetTimerStart("Create mrnet network and ship dependencies"));

    network = std::unique_ptr<MRNetNetwork>
        (MRNetNetwork::create_network_no_be(topology_name, mrn_topology, di->appId(), mid));
    
    if (network == nullptr) {
        // error already printed
        goto invoke_error;
    }

    DPERFLOG(SetTimerStopAndLog("Create mrnet network and ship dependencies"));

    // create the backend attach file which the compute nodes will use to hook
    // up to the network
    if (create_parents_info(mrn_topology, di->numRanks()) < 1) {
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
    // dbgsrv_alps -h <cbhost> -c <cbport> -p <proto> -e <backend>
    // prog first create the argv for dbgsrv_alps This now also depends on
    // gpu_flag This now adds the early entry flag...

    dbgsrv_alps_argv = ArgvStream("-h", "mrn", "-c", "-1", "-p",
                                  di->proto().name(), "-e", backend);
    if (opts.gpu_flag)    dbgsrv_alps_argv << "--gpu";
    if (dbg)              dbgsrv_alps_argv << "--debug";
    if (opts.early_entry_flag) dbgsrv_alps_argv << "--early-entry";
    if (opts.non_stop_flag)        dbgsrv_alps_argv << "--non-stop";
    if (!opts.gdb_path.empty() && serverGdb) dbgsrv_alps_argv << "--gdb" << *serverGdb;
    if (opts.job_opts.size() == 1) {
        if (opts.job_opts[0].non_mpi_flag)     dbgsrv_alps_argv << "--non-mpi";
        dbgsrv_alps_argv << "-f" << opts.job_opts[0].app_argv[0];
    }

#ifdef DEBUG
    DPRINT("Starting dbgsrv_alps with argv: \n");
    DPRINT("%s\n", dbgsrv_alps_argv.asString().c_str());

    // set dbg to 1 if debug is on
    dbg = 1;
#endif /* DEBUG */

    // Forward GUARD_INST_DIRECTORY if set
    if ((env_base_dir = getenv(GUARD_INST_DIRECTORY)) != nullptr) {
        args = (char **)malloc(2 * sizeof(char *));
        Asprintf(&args[0], "%s=%s", GUARD_INST_DIRECTORY, env_base_dir);
        args[1] = (char *)nullptr;
    }

    DPERFLOG(SetTimerStart("mrnet backend attach"));

    // Set CTI debug env var if debug is on
    if (dbg) {
        if (cti_setAttribute(CTI_DEBUG, "1")) {
            PrintOutError("Setting CTI_DEBUG failed: %s\n", cti_error_str());
        }
    }

    // launch dbgsrv_alps onto the compute nodes
    if (cti_execToolDaemon(mid, "dbgsrv_alps", dbgsrv_alps_argv.c_strs().data(),
                           (const char *const *)args)) {
        PrintOutError("CTI error: %s\n", cti_error_str());
        PrintOutError("Failed to launch dbgsrv_alps onto compute nodes!\n");
        if (args != nullptr) {
            free(args[0]);
            free(args);
            args = nullptr;
        }
        goto invoke_error;
    }

    if (args != nullptr) {
        free(args[0]);
        free(args);
        args = nullptr;
    }

    // wait for the debug servers to connect to the mrnet tree
    rtn = network->attach_be(di->numRanks(), mrnet_timeout);
    if (rtn) {
        if (rtn > 0) {
            PrintOutError(
                "ERROR: DbgMRNInvoke failed to attach dbgsrvs before "
                "timeout!\n");
        } else {
            PrintOutError(
                "ERROR: DbgMRNInvoke had nullptr network information!\n");
        }

        goto invoke_error;
    }

    DPERFLOG(SetTimerStopAndLog("mrnet backend attach"));

    DPRINT("Debug server launch completed.\n");
    DPRINT(
        "MRNet is going to create communication streams for the debugging "
        "network...\n");

    if (network->create_streams_and_load_filter(AGGREGATION_FILTER,
                                              AGGREGATION_FUNC)) {
        PrintOutError(
            "ERROR: DbgMRNInvoke failed to load filter functions and create "
            "streams for MRNet!\n");
        goto invoke_error;
    }

    DPRINT("Debugging network startup completed.\n");
    PrintOutMsg("Finalizing setup...\n");

    network->enable_failure_callback(di);

    if ((num_be = network->get_num_of_be()) != di->numRanks()) {
        PrintOutError(
            "ERROR: MRNet has an incorrect number of debug servers.\n");
        PrintOutError("\tExpected [%d] found [%d]\n", di->numRanks(), num_be);
        goto invoke_error;
    }

    DPRINT("num_be is %d\n", num_be);

    di->setFullBitset(std::make_unique<DenseBitset>(num_be, DenseBitset::InitialState::Full));
    di->setLaunchedBitset(std::make_unique<DenseBitset>(num_be));

    DPRINT("DbgMRNInvoke: MRNet initialization succeded! network:[%p]\n",
           network.get());

    // create the d_evfds list for the di
    di->initializeEventFds();

    // setup the event notification fd for the io stream
    event_fd = network->get_io_notificationfd();

    // Add the io_stream fd to the d_evfds list
    // This allows DbgCheckEvents to select on MRNet protocol events
    di->addEventFd(event_fd);

    // setup the event notification fd for the event stream
    event_fd = network->get_event_notificationfd();

    // Add the event_stream fd to the d_evfds list
    // This allows DbgCheckEvents to select on MRNet protocol events
    di->addEventFd(event_fd);

    // broadcast an INVOKE command to complete the invoke callback
    request = CommandToSocProto(DBGCMD_INVOKE, num_be);

    if (network->setup_streams(request.c_str())) {
        PrintOutError(
            "ERROR: MRNet cannot setup broadcast channels with dbgsrvs!\n");
        goto invoke_error;
    }

    // hand the network over to the di
    di->setNetwork(std::move(network));

    // wait for the DBG_OK response from the dbgsrv's indicating they have
    // finished starting up

    DPRINT("MRNet setup finalized.\n");
    DPRINT("Waiting for dbgsrv callbacks...\n");

    // final cleanup

    DPERFLOG(SetTimerStart("Launch finalize"));

    // Here we want to wait for the dbginvoke to start as long as the callback
    // is still valid
    while (!di->isStarted()) {
        if (WaitForProtoStartupEvent(di) == -1) {
            PrintOutError("Could not receive launch callbacks!\n");
            goto invoke_error;
        }
    }

    DPERFLOG(SetTimerStopAndLog("Launch finalize"));

    // setup the program output stream handler if we are handling output
    if (po != nullptr) {
        // Add the p_o_rfd to the d_evfds list
        // This allows DbgCheckEvents to select on application output events
        di->addEventFd(po->p_o_rfd);
    }

    // All done. Everything is ready to go at this point.

    DPERFLOG(SetTimerStopAndLog("Invoke total"));
    DPRINT("DbgMRNInvoke completed!\n");

    return 0;

invoke_error:

    if (di->appId() != 0) {
        cti_killApp(di->appId(), SIGKILL);
        cti_deregisterApp(di->appId());
        di->setAppId(0);
    }

    if (machine_hosts != nullptr) {
        tmp = machine_hosts;
        while (*tmp != nullptr) {
            Free(*tmp++);
        }
        Free(machine_hosts);
    }

    return 1;
}

int MRNClientProtocol::syncExit(DbgInvoke *di) {
    if (!di) {
        PrintOutError("Tried to exit with null di.\n");
        return -1;
    }

    // shutdown mrnet only if not already in a shutdown state
    if (!di->isShutdown()) {
        di->setNetwork(nullptr);
    }

    return 0;
}

int MRNClientProtocol::forceExit(DbgInvoke *di) {
    // mrnet doesn't support forceExit
    return syncExit(di);
}

#ifdef BE_VALGRIND
int MRNClientProtocol::addValgrindFiles(cti_manifest_id_t mid)
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
    suppressionFile = std::string(getenv("HOME")) + "/" + GDB4HPC_BE_SUPPRESSIONS;
    if (std::ifstream(suppressionFile.c_str())) hasSuppressionFile = true;

    if (hasSuppressionFile) {
        if (int err = cti_addManifestFile(mid, suppressionFile.c_str())) {
            PrintOutError("CTI error: %s\n", cti_error_str());
            PrintOutError("Failed to add valgrind suppression file %s to manifest.\n", suppressionFile.c_str());
            return err;
        }
    } else {
        // Print out the file we're looking for so the dev doesn't need to look it up
        PrintOutMsg("no suppression file found at: %s\n", suppressionFile.c_str());
    }

    return 0;
}
#endif

/*
** Compatible callback routine for attaching with mrnet.
*/
int MRNAttachHandler::handleEvent(DbgInvoke *di, const ProtoMessageSet &set) {
    dbgevent_t *e = nullptr;
    int err = 0;

    for (const auto &msg : set) {
        // convert this protocol msg string to an event
        e = SocProtoToEvent(msg.getMessage());

        di->orLaunchedBitset(*msg.getBitset());

        if (e->ev_event != DBGEV_OK) {
            err = 1;
            PrintOutError("Received unknown event %s.\n",
                          DbgevToStr(e->ev_event));
        }

        // Done with the event, free it
        FreeEvent(e);
    }

    if (err) return 1;

    DPRINT("cbMRNAttach received responses [%s]!\n", di->launchedBitset()->toString().c_str());

    // Check if we have received a message from every rank
    if (*di->launchedBitset() == *di->fullBitset()) {
        // Reset the protocol event callback to the dbgclient default
        di->proto().resetProtocolEventHandler();

        // Set the started bit in the di
        di->setStarted();

        DPERFLOG(SetTimerStopAndLog("stream_init"));

        DPRINT("dbgsrv callback complete.\n");
        DPRINT("Releasing application from the startup barrier...\n");

        PrintOutMsg("Attach complete.\n");
    }

    return 0;
}

/*
** Compatible callback routine for invoking with mrnet.
*/
int MRNLaunchHandler::handleEvent(DbgInvoke *di, const ProtoMessageSet &set) {
    dbgevent_t *e = nullptr;
    int err = 0;

    for (const auto &msg : set) {
        // convert this protocol msg string to an event
        e = SocProtoToEvent(msg.getMessage());

        di->orLaunchedBitset(*msg.getBitset());

        if (e->ev_event != DBGEV_OK) {
            err = 1;
            PrintOutError("Received unknown event %s.\n",
                          DbgevToStr(e->ev_event));
        }

        // Done with the event, free it
        FreeEvent(e);
    }

    if (err) return 1;

    DPRINT("cbMRNInvoke received responses [%s]!\n", di->launchedBitset()->toString().c_str());

    // Check if we have received a message from every rank
    if (*di->launchedBitset() == *di->fullBitset()) {
        // Reset the protocol event callback to the dbgclient default
        di->proto().resetProtocolEventHandler();

        // Set the started bit in the di
        di->setStarted();

        DPERFLOG(SetTimerStopAndLog("stream_init"));

        DPRINT("dbgsrv callback complete.\n");
        DPRINT("Releasing application from the startup barrier...\n");

#ifdef ALPS
        // tell CTI to release the application from the startup barrier
        if (cti_releaseAppBarrier(di->appId())) {
            PrintOutError("CTI error: %s\n", cti_error_str());
            PrintOutError(
                "Could not release application from the startup barrier.\n");
            cti_killApp(di->appId(), SIGKILL);
            return 1;
        }
#endif
    }

    return 0;
}
