/******************************************************************************\
 * Copyright 2021-2023 Hewlett Packard Enterprise Development LP.
 ******************************************************************************/

#include <future>
#include <thread>
#include <chrono>

#include "dbg_client.h"
#include "dbgclnt_zmq.h"
#include "dbgclnt_proto_ctiship.h"

#include "useful.h"
#include "signal_util.h"

#include "common_tools_fe.h"

#if DEBUG
#define LOG(...) fprintf(stderr, __VA_ARGS__)
#else
#define LOG(...) /**/
#endif

// Instantiation of the protocol - accessed via DebugClientProtocol::registry
ZMQNetClientProtocol DbgClntProtoZMQ{};

// launch killswitch, used in makeZmqnetFrontend, set by launchSigHandler
static std::atomic<bool> launchKS = false;

static void launchSigHandler(int sig) {
    if (sig == SIGINT) launchKS = true;
}

static std::unique_ptr<zmqnet::AsyncFrontend> makeZmqnetFrontend(
    int num_be, int appId, long int sessionId, long int manifestId, const char* const* leafArgv
){
    auto makeZmqnet = [](
        const std::atomic<bool>* killSwitch,
        int& connected, int appId, long int sessionId,
        long int manifestId, const char* const* leafArgv,
        const cti_hostsList_t* hosts
    ){
        auto connectCallback = [&](int node) {
            if (hosts && node < hosts->numHosts) {
                connected += hosts->hosts[node].numPes;
            } else {
                // not correct, but at least it still shows progress is being made
                connected++;
            }
        };

        auto callbacks = zmqnet::UserCallbacks();
        callbacks.registerCallback<zmqnet::Event::InitialConnection>(connectCallback);

        // might throw on error or interruption. exception handled around zmqnetF.get()
        return std::make_unique<zmqnet::AsyncFrontend>(
            appId, sessionId, manifestId,
            zmqnet::Topology{ .launch = zmqnet::Topology::Packed, .fanout = 32},
            leafArgv, /* leaf env */ nullptr, std::move(callbacks), killSwitch
        );
    };

    launchKS = false;
    auto tmpSigHandler = TmpSignalHandler(SIGINT, launchSigHandler);

    auto connected = int{0};
    auto hosts = std::unique_ptr<cti_hostsList_t, decltype(&cti_destroyHostsList)>
        (cti_getAppHostsPlacement(appId), cti_destroyHostsList);
    auto zmqnetF = std::async(makeZmqnet, &launchKS, std::ref(connected), appId, sessionId, manifestId, leafArgv, hosts.get());

    // in practice, approx. number of seconds of silence until timeout
    auto tries = 300;
    auto envStr = getenv("GDB4HPC_STARTUP_TIMEOUT");
    if (envStr && strtol(envStr, nullptr, 10)) {
        tries = strtol(envStr, nullptr, 10);
        PrintOutMsg("Got a timeout of %d from GDB4HPC_STARTUP_TIMEOUT.\n", tries);
    }
    auto tries_left = tries;
    auto last_connected = -1;

    // wait for network to connect
    using Fs = std::future_status;
    using namespace std::chrono_literals;
    auto dots = 0;

    for (auto status = zmqnetF.wait_for(1s); status != Fs::ready && launchKS == false; status = zmqnetF.wait_for(1s)) {
        if (status == Fs::timeout && last_connected != connected) {
            // a new node connected, reset timeout
            tries_left = tries;
        } else if (status == Fs::timeout && last_connected == connected && --tries_left == 0) {
            // waited, no new ranks connected.
            PrintOutError("Timed out. Cancelling launch. Please wait...\n");
            // this causes the zmqnet thread to eventually throw
            launchKS = true;
        } else if (status == std::future_status::deferred) {
            // other possibilities for status are either ::ready or ::deferred.
            // we should never actually see deferred.
            PrintOutError("Warning: unhandled std::future_status::deferred.\n");
        }

        const auto* enddots = (dots ? "\n" : "");
        if (connected == last_connected && dots != 30) {
            PrintOutMsg(".");
            OutMsgStream().flush();
            ++dots;
        } else if (connected != num_be) {
            // still connecting
            PrintOutMsg("%s%d/%d ranks connected... (timeout in %d seconds)\n",
                        enddots, connected, num_be, tries_left);
            dots = 0;
        } else if (connected != last_connected) {
            // just connected to the last ranks
            PrintOutMsg("%s%d/%d ranks connected\n"
                        "Creating network... (timeout in %d seconds)\n",
                        enddots, connected, num_be, tries_left);
            dots = 0;
        }  else {
            // all connected, still waitint to create network (dots==30)
            PrintOutMsg("\nCreating network... (timeout in %d seconds)\n", tries_left);
            dots = 0;
        }
            
        last_connected = connected;
    };

    if (dots) PrintOutMsg("\n");

    auto zmqnet = std::unique_ptr<zmqnet::AsyncFrontend>();

    try {
        zmqnet = zmqnetF.get();
        if (last_connected != connected) {
            PrintOutMsg("%d/%d ranks connected.\n", connected, num_be);
        }
        zmqnet->removeCallback<zmqnet::Event::InitialConnection>();
    } catch (std::runtime_error& e) {
        PrintOutError("Network creation failed: %s\n", e.what());
    }

    return zmqnet;
}

ZMQNetClientProtocol::ZMQNetClientProtocol()
    : DebugClientProtocol()
{ 
    registerProtocol(); 
}

int ZMQNetClientProtocol::init() {
    LOG("%s\n", __PRETTY_FUNCTION__);

    // set libzmqnet install prefix so that zmqnet can find the commnode binary
    auto base = getinstallbase();
    if (!base) {
        PrintOutError("Error in %s: Couldn't detect install base.\n", __PRETTY_FUNCTION__);
        return 0;
    }
    setenv("LIBZMQNET_INSTALL_DIR", base, false);
    free(base);

    return 1;
}

std::unique_ptr<Bitset> ZMQNetClientProtocol::sendCommand(DbgInvoke *di, const Bitset *target,
                             const char *request) {
    LOG("%s\n", __PRETTY_FUNCTION__);

    if (di->isShutdown()) return nullptr;

    return getNetwork<ZMQNetNetwork>(di).sendCommand(target, request);
}

std::variant<ProtoMessageSet, ProtoNothing>
ZMQNetClientProtocol::receiveReply(DbgInvoke *di, const Bitset *target, std::chrono::seconds timeout, Bitset &received) {
    LOG("%s\n", __PRETTY_FUNCTION__);

    if (di->isShutdown()) {
        return {};
    }

    return getNetwork<ZMQNetNetwork>(di).receiveReply(target, timeout, received);
}

int ZMQNetClientProtocol::checkEvents(DbgInvoke *di) {
    // LOG("%s\n", __PRETTY_FUNCTION__);
    return getNetwork<ZMQNetNetwork>(di).checkEvents();
}

void ZMQNetClientProtocol::dispatchEvents(DbgInvoke *di, int ev_fd) {
    // zmqnet doesn't use event file descriptors, so ev_fd will always be -1
    assert(ev_fd == -1);

    auto &net = getNetwork<ZMQNetNetwork>(di);
    
    // dispatch program output
    auto outputLines = net.releaseOutputLines();
    if (outputLines.size() > 0) {
        DbgDispatchProtoOutput(di, outputLines);
    }
    
    // dispatch proto events
    auto messages = net.releaseEvents();
    if (messages.size() > 0) {
        LOG("dispatching\n");
        DbgDispatchProtoEvents(di, messages);
    }
}

int ZMQNetClientProtocol::attach(DbgInvoke *di, const AttachOpts& opts) {
    LOG("%s\n", __PRETTY_FUNCTION__);
    // ZMQNetNetwork::OutputPtr is too many aliases deep to be implicitly
    // convertable from a nullptr, so we'll just make a dummy one to pass.
    auto dummyOutput = ZMQNetNetwork::OutputPtr(nullptr, DbgFreeGenericProgOutp);

    return internalAttach(di, nullptr, 0, std::move(dummyOutput), opts,
                          0, false, false);
}

int ZMQNetClientProtocol::internalAttach(DbgInvoke *di,
    const char *prog, uint64_t appid, ZMQNetNetwork::OutputPtr &&output,
    const AttachOpts& opts, int early_entry_flag, bool isNonStop, bool isNonMpi)
{
    LOG("%s\n", __PRETTY_FUNCTION__);

    // figure out our appid
    if (appid != 0) {
        di->setAppId(appid);
    } else if (opts.app_ident.empty()) {
        PrintOutError("Can't parse appid from nullptr.\n");
        return 1;
    } else {
        di->setAppId(parseAndRegisterAppId(opts.app_ident.c_str()));
        if (di->appId() == 0) {
            PrintOutError("Failed to parse appid.\n");
            return 1;
        }
    }

    // figure out how many ranks there are
    if (di->numRanks() == 0) {
        if (di->setNumRanks(cti_getNumAppPEs(di->appId())); di->numRanks() == 0) {
            PrintOutError("Failed to retrieve nproc.\n");
            PrintOutError("CTI error: %s\n", cti_error_str());
            return 1;
        }
    }

    // ship required backend files /////////////////////////////////////////////

    const auto session_id = cti_createSession(di->appId());
    if (session_id == 0) {
        PrintOutError("Failed to create CTI session.\n");
        PrintOutError("CTI error: %s\n", cti_error_str());
        return 1;
    }

    const auto manifest_id = cti_createManifest(session_id);
    if (manifest_id == 0) {
        PrintOutError("Failed to create CTI manifest\n");
        PrintOutError("CTI error: %s\n", cti_error_str());
        return 1;
    }

    auto serverGdb = shipGdb(manifest_id, opts.gdb, opts.gdb_data_dir);
    if (!serverGdb) {
        PrintOutError("Error shipping gdb.\n");
        return 1;
    }

    if (opts.gpu_flag && opts.gdb.empty() && !shipGpuGdbs(manifest_id)) {
        PrintOutError("Error shipping gpu gdbs.\n");
        return 1;
    }

    // dbgsrv is shipped here
    if (!shipCommonFiles(manifest_id, opts.gpu_flag)) {
        PrintOutError("Error shipping common files.\n");
        return 1;
    }

    if (!shipOmpd(manifest_id)) {
        PrintOutMsg("Error shipping OMPD files. OMPD support will not be available.\n");
    }

    // TODO support --with-valgrind

    if (cti_addManifestLibrary(manifest_id, "libzmqnetagg.so")) {
        PrintOutError("CTI error: %s\n", cti_error_str());
        PrintOutError("Failed to add %s to manifest.\n", "libzmqnetagg.so");
        return 1;
    }

    LOG("added files\n");

    // create network //////////////////////////////////////////////////////////

    auto dbgsrvArgs = ArgvStream(
        "dbgsrv",
        "-h", "zmq",   // meaningless for zmqnet, but required to be non-null by dbgsrv
        "-c", "1",     // meaningless for zmqnet, but required to be non-zero by dbgsrv
        "-p", "zmqnet",
        "-e", opts.backend.c_str()
    );
    if (prog)             dbgsrvArgs << "-f" << prog;
    if (opts.gpu_flag)    dbgsrvArgs << "--gpu";
    if (opts.debug_flag)  dbgsrvArgs << "--debug";
    if (early_entry_flag) dbgsrvArgs << "--early-entry";
    if (isNonStop)        dbgsrvArgs << "--non-stop";
    if (isNonMpi)         dbgsrvArgs << "--non-mpi";
    if (!opts.gdb.empty() && serverGdb) dbgsrvArgs << "--gdb" << *serverGdb;
    if (!opts.vgdb_file.empty()) dbgsrvArgs << "--vgdb-command-file" << opts.vgdb_file;

#if DEBUG
    LOG("zmqnet argv:\n");
    for (const auto cstr : dbgsrvArgs.c_strs()) {
        LOG("%s ", cstr);
    }
    LOG("\n");
#endif
    auto num_be = di->numRanks();
    di->setFullBitset(std::make_unique<DenseBitset>(num_be, DenseBitset::InitialState::Full));
    di->setLaunchedBitset(std::make_unique<DenseBitset>(num_be));

    auto zmqnet = makeZmqnetFrontend(num_be, di->appId(), session_id, manifest_id, dbgsrvArgs.c_strs().data());

    if (!zmqnet) {
        // error message printed in makeZmqnetFrontend
        cti_destroySession(session_id);
        return 1;
    }

    di->setNetwork(std::make_unique<ZMQNetNetwork>(std::move(zmqnet), std::move(output),
                                                   di->fullBitset()->clone()));

    PrintOutMsg("Created network...\n");

    // send invoke
    auto invoke = CommandToSocProto(DBGCMD_INVOKE, num_be);
    sendCommand(di, di->fullBitset(), invoke.c_str());

    // wait for response
    auto received{DenseBitset(num_be)};
    receiveReply(di, di->fullBitset(), /* no timeout */ std::chrono::seconds(-1), received);

    if (!received.isFull()) {
        PrintOutError("Error: not all ranks received startup message. Attempting to clean up...\n");
        forceExit(di);
        return 1;
    }

    PrintOutMsg("Connected to application...\n");

    di->setStarted();

    return 0;
}

int ZMQNetClientProtocol::launch(DbgInvoke *di, const LaunchOpts& opts) {
    LOG("%s\n", __PRETTY_FUNCTION__);

    if (!di) {
        PrintOutError("Tried to launch ZMQNet with a null di\n");
        return 1;
    }

    PrintOutMsg("Starting application, please wait...\n");

    // launch n ranks of app being debugged ////////////////////////////////////

    // program output pipe container.
    // this will eventually be given to the ZMQNetNetwork object.
    auto output = ZMQNetNetwork::OutputPtr(DbgCreateGenericProgOutp(), DbgFreeGenericProgOutp);

    const auto optStr = [](const std::string& str) {
                            return str.empty() ? nullptr : str.c_str();
                        };

    auto launcherArgs = ArgvStream{};

    // Non-MPMD launches will use rank count from launch command
    // MPMD rank counts must be specified for each subjob
    if (opts.job_opts.size() == 1) {
        launcherArgs << "-n" << di->numRanks();
    }
    auto first = true;
    for (auto&& job_opt : opts.job_opts) {
        if (first) {
            first = false;
        } else {
            launcherArgs << ":";
        }
        launcherArgs << job_opt.launcher_args.c_strs().data() << job_opt.app_argv.c_strs().data();
    }

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

        auto arch_input = optStr(opts.input_file_path);
        auto cwd = optStr(opts.cwd);
        auto env = opts.env.c_strs();

        di->setAppId(cti_launchAppBarrier(launcherArgs.c_strs().data(), output->p_o_wfd,
                                          output->p_o_wfd, arch_input, cwd, env.data()));

        // Notify the message thread that launch has completed and that it can exit its wait
        { auto guard = std::lock_guard<std::mutex>{waitMtx};
            applicationLaunched = true;
        }
        waitCv.notify_all();
        waitFuture.get();
    }

    if (di->appId() == 0) {
        PrintOutError("Failed to launch CTI app.\n");
        PrintOutError("CTI error: %s\n", cti_error_str());
        return 1;
    }

    PrintOutMsg("Launched application...\n");

    auto attachOpts = AttachOpts{.proto = "",
                                 .app_ident = "",
                                 .backend = opts.backend_name,
                                 .gdb = opts.gdb_path,
                                 .gdb_data_dir = opts.gdb_data_dir,
                                 .vgdb_file = "",
                                 .gpu_flag = bool(opts.gpu_flag),
                                 .debug_flag = bool(opts.debug_flag) };
                            

    // attach to launched application
    auto prog = (opts.job_opts.size() == 1)
        ? opts.job_opts[0].app_argv[0].c_str()
        : nullptr;
    auto non_mpi = (opts.job_opts.size() == 1)
        ? opts.job_opts[0].non_mpi_flag
        : false;
    if (internalAttach(di, prog, di->appId(), std::move(output), attachOpts,
                       opts.early_entry_flag, opts.non_stop_flag, non_mpi)) {
        PrintOutError("Failed to attach to application. Cleaning up...\n");

        if (di->appId() != 0) {
            cti_releaseAppBarrier(di->appId());
            cti_killApp(di->appId(), SIGKILL);
            cti_deregisterApp(di->appId());
            di->setAppId(0);
        }

        return 1;
    }

    // tell CTI to release the application from the startup barrier
    if (cti_releaseAppBarrier(di->appId())) {
        PrintOutError(
            "Could not release application from the startup barrier.\n");
        PrintOutError("CTI error: %s\n", cti_error_str());
        cti_killApp(di->appId(), SIGKILL);
        di->setAppId(0);
        return 1;
    }

    return 0;
}

int ZMQNetClientProtocol::syncExit(DbgInvoke *di) {
    LOG("%s\n", __PRETTY_FUNCTION__);

    if (!di) {
        PrintOutError("Tried to exit with null di.\n");
        return -1;
    }

    auto network_exit_result = getNetwork<ZMQNetNetwork>(di).syncExit();
    di->setNetwork(nullptr);

    return network_exit_result;
}

int ZMQNetClientProtocol::forceExit(DbgInvoke *di) {
    LOG("%s\n", __PRETTY_FUNCTION__);

    if (!di) {
        PrintOutError("Tried to force exit with null di.\n");
        return -1;
    }

    auto network_exit_result = getNetwork<ZMQNetNetwork>(di).forceExit();
    di->setNetwork(nullptr);

    return network_exit_result;
}
