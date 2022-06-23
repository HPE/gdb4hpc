/******************************************************************************\
 * Copyright 2021-2022 Hewlett Packard Enterprise Development LP.
 ******************************************************************************/

#include <future>
#include <thread>
#include <chrono>

#include "dbg_client.h"
#include "dbgclnt_zmq.h"
#include "dbgclnt_proto_ctiship.h"

#include "useful.h"

#include "common_tools_fe.h"

#if DEBUG
#define LOG(...) fprintf(stderr, __VA_ARGS__)
#else
#define LOG(...) /**/
#endif

// Instantiation of the protocol - accessed via DebugClientProtocol::registry
ZMQNetClientProtocol DbgClntProtoZMQ{};

static std::unique_ptr<zmqnet::AsyncFrontend> makeZmqnetFrontend(int num_be,
    int appId, long int sessionId, long int manifestId, const char* const* leafArgv)
{
    using namespace std::chrono_literals;

    auto last_connected = 0;
    auto connected = 0;

    // in practice, approx. number of seconds of silence until timeout
    auto tries = 300;
    auto envStr = getenv("GDB4HPC_STARTUP_TIMEOUT");
    if (envStr && strtol(envStr, nullptr, 10)) {
        tries = strtol(envStr, nullptr, 10);
        PrintOutMsg("Got a timeout of %d from GDB4HPC_STARTUP_TIMEOUT.\n", tries);
    }
    auto tries_left = tries;

    auto makeZmqnet = [&connected](
        std::promise<std::unique_ptr<zmqnet::AsyncFrontend>>&& zmqnetP, int appId,
        long int sessionId, long int manifestId, const char* const* leafArgv)
    {
        auto connectCallback = [&](int node) {
            connected++;
        };

        auto callbacks = zmqnet::UserCallbacks();
        callbacks.registerCallback<zmqnet::Event::InitialConnection>(connectCallback);

        zmqnetP.set_value(
            std::make_unique<zmqnet::AsyncFrontend>(appId, sessionId, manifestId,
                zmqnet::Topology{ .launch = zmqnet::Topology::Packed, .fanout = 4},
                leafArgv, /* leaf env */ nullptr, std::move(callbacks)
            )
        );
    };

    // a simple std::async(...) future won't work for us here,
    // because the std::async future will implicitly wait for its internal
    // thread's completion on scope exit, meaning we couldn't return early after a timeout.
    auto zmqnetP  = std::promise<std::unique_ptr<zmqnet::AsyncFrontend>>{};
    auto zmqnetF  = zmqnetP.get_future();
    auto zmqnetTh = std::thread(
        makeZmqnet, std::move(zmqnetP), appId, sessionId, manifestId, leafArgv
    );

    // wait for network to connect
    using Fs = std::future_status;
    for (auto status = zmqnetF.wait_for(1s); status != Fs::ready; status = zmqnetF.wait_for(1s)) {
        if (status == Fs::timeout && last_connected != connected) {
            // a new node connected, reset timeout
            tries_left = tries;
        } else if (status == Fs::timeout && last_connected == connected && --tries_left == 0) {
            // waited, no new ranks connected.
            PrintOutError("Timed out.\n");
            // XXX: the thread runs forever, stuck waiting for nodes to connect.
            // but we have no way to stop it with the current zmqnet api.
            zmqnetTh.detach();
            return nullptr;
        } else if (status == std::future_status::deferred) {
            // other possibilities for status are either ::ready or ::deferred.
            // we should never actually see deferred.
            PrintOutError("Warning: unhandled std::future_status::deferred.\n");
        }
        PrintOutMsg("%d/%d ranks connected... (timeout in %d seconds)\n", connected, num_be, tries_left);
        last_connected = connected;
    };

    PrintOutMsg("%d/%d ranks connected.\n", connected, num_be);

    auto zmqnet = zmqnetF.get();
    zmqnet->removeCallback<zmqnet::Event::InitialConnection>();
    zmqnetTh.join();

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

std::unique_ptr<Bitset> ZMQNetClientProtocol::sendCommand(dbginvoke_t *di, const Bitset *target,
                             const char *request) {
    LOG("%s\n", __PRETTY_FUNCTION__);

    if (di->d_isShutdown) return nullptr;

    return getNetwork<ZMQNetNetwork>(di).sendCommand(target, request);
}

std::optional<ProtoMessageSet> ZMQNetClientProtocol::receiveReply(dbginvoke_t *di, 
                                            const Bitset *target, Bitset &received) {
    LOG("%s\n", __PRETTY_FUNCTION__);

    if (di->d_isShutdown) {
        return {};
    }

    return getNetwork<ZMQNetNetwork>(di).receiveReply(target, received);
}

int ZMQNetClientProtocol::checkEvents(dbginvoke_t *di) {
    // LOG("%s\n", __PRETTY_FUNCTION__);
    return getNetwork<ZMQNetNetwork>(di).checkEvents();
}

void ZMQNetClientProtocol::dispatchEvents(dbginvoke_t *di, int ev_fd) {
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

int ZMQNetClientProtocol::attach(dbginvoke_t *di, proto_session *sess,
                        const char *app_ident, const char *arch,
                        const char *backend, int gpu_flag, int debug_flag) {
    LOG("%s\n", __PRETTY_FUNCTION__);
    // ZMQNetNetwork::OutputPtr is too many aliases deep to be implicitly
    // convertable from a nullptr, so we'll just make a dummy one to pass.
    auto dummyOutput = ZMQNetNetwork::OutputPtr(nullptr, DbgFreeGenericProgOutp);

    return internalAttach(di, sess, nullptr, app_ident, 0, std::move(dummyOutput), 
                   arch, backend, nullptr, gpu_flag, debug_flag, 0, false, false);
}

int ZMQNetClientProtocol::internalAttach(dbginvoke_t *di, proto_session *sess,
    const char *prog, const char *app_ident, uint64_t appid, ZMQNetNetwork::OutputPtr &&output,
    const char *arch, const char *backend, const char* gdb, int gpu_flag, int debug_flag,
    int early_entry_flag, bool isNonStop, bool isNonMpi)
{
    LOG("%s\n", __PRETTY_FUNCTION__);

    // figure out our appid
    if (appid != 0) {
        di->d_appid = appid;
    } else if (app_ident == nullptr) {
        PrintOutError("Can't parse appid from nullptr.\n");
        return 1;
    } else {
        di->d_appid = parseAndRegisterAppId(app_ident); 
        if (di->d_appid == 0) {
            PrintOutError("Failed to parse appid.\n");
            return 1;
        }
    }

    // figure out how many ranks there are
    if (di->d_nproc == 0) {
        if ((di->d_nproc = cti_getNumAppPEs(di->d_appid)) == 0) {
            PrintOutError("Failed to retrieve nproc.\n");
            PrintOutError("CTI error: %s\n", cti_error_str());
            return 1;
        }
    }

    // ship required backend files /////////////////////////////////////////////

    const auto session_id = cti_createSession(di->d_appid);
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

    auto serverGdb = shipGdb(manifest_id, gdb);
    if (!serverGdb) {
        PrintOutError("Error shipping gdb.\n");
        return 1;
    }

    if (gpu_flag && !gdb && !shipGpuGdbs(manifest_id)) {
        PrintOutError("Error shipping gpu gdbs.\n");
        return 1;
    }

    // dbgsrv is shipped here
    if (!shipCommonFiles(manifest_id, gpu_flag)) {
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
        "-a", arch,
        "-p", "zmqnet",
        "-e", backend
    );
    if (prog)             dbgsrvArgs << "-f" << prog;
    if (gpu_flag)         dbgsrvArgs << "--gpu";
    if (debug_flag)       dbgsrvArgs << "--debug";
    if (early_entry_flag) dbgsrvArgs << "--early-entry";
    if (isNonStop)        dbgsrvArgs << "--non-stop";
    if (isNonMpi)         dbgsrvArgs << "--non-mpi";
    if (gdb && serverGdb) dbgsrvArgs << "--gdb" << *serverGdb;

#if DEBUG
    LOG("zmqnet argv:\n");
    for (const auto cstr : dbgsrvArgs.c_strs()) {
        LOG("%s ", cstr);
    }
    LOG("\n");
#endif
    auto num_be = di->d_nproc;
    di->d_bitset_full = std::make_unique<DenseBitset>(num_be);
    di->d_bitset_full->invert();

    di->d_bitset_launched = std::make_unique<DenseBitset>(num_be);

    auto zmqnet = makeZmqnetFrontend(num_be, di->d_appid, session_id, manifest_id, dbgsrvArgs.c_strs().data());

    if (!zmqnet) {
        // error message printed in makeZmqnetFrontend
        cti_destroySession(session_id);
        return 1;
    }

    di->d_proto_net = std::make_unique<ZMQNetNetwork>(std::move(zmqnet), std::move(output), di->d_bitset_full->clone());

    PrintOutMsg("Created network...\n");

    // send invoke
    auto invoke = CommandToSocProto(DBGCMD_INVOKE, num_be);
    sendCommand(di, di->d_bitset_full.get(), invoke.c_str());

    // wait for response
    auto received{DenseBitset(num_be)};
    receiveReply(di, di->d_bitset_full.get(), received);

    if (!received.isFull()) {
        PrintOutError("Error: not all ranks received startup message. Attempting to clean up...\n");
        exit(di, 0);
        return 1;
    }

    PrintOutMsg("Connected to application...\n");

    di->d_started = 1;

    return 0;
}

int ZMQNetClientProtocol::launch(dbginvoke_t *di, proto_session *sess, const char *prog,
                        const char * const *argv, const char *arch, const char * const *arch_args,
                        const char *arch_input, const char *backend, const char* gdb, const char * const *env,
                        const char *cwd, int gpu_flag, int debug_flag, int non_mpi_flag,
                        int early_entry_flag, bool isNonStop) {
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

    auto launcherArgs = ArgvStream("-n", di->d_nproc, arch_args, prog, argv);
    if ((di->d_appid = cti_launchAppBarrier(launcherArgs.c_strs().data(), output->p_o_wfd, output->p_o_wfd, arch_input, cwd, env)) == 0) {
        PrintOutError("Failed to launch CTI app.\n");
        PrintOutError("CTI error: %s\n", cti_error_str());
        return 1;
    }

    PrintOutMsg("Launched application...\n");

    // attach to launched application
    if (internalAttach(di, sess, prog, nullptr, di->d_appid, std::move(output), arch, backend, gdb, gpu_flag, debug_flag, early_entry_flag, isNonStop, non_mpi_flag)) {
        PrintOutError("Failed to attach to application. Cleaning up...\n");

        if (di->d_appid != 0) {
            cti_releaseAppBarrier(di->d_appid);
            cti_killApp(di->d_appid, SIGKILL);
            cti_deregisterApp(di->d_appid);
            di->d_appid = 0;
        }

        return 1;
    }

    // tell CTI to release the application from the startup barrier
    if (cti_releaseAppBarrier(di->d_appid)) {
        PrintOutError(
            "Could not release application from the startup barrier.\n");
        PrintOutError("CTI error: %s\n", cti_error_str());
        cti_killApp(di->d_appid, SIGKILL);
        di->d_appid = 0;
        return 1;
    }

    return 0;
}

void ZMQNetClientProtocol::destroyClient(dbginvoke *di) {
    LOG("%s\n", __PRETTY_FUNCTION__);

    if (di->d_proto_net) {
        exit(di, 0);
    }
}

int ZMQNetClientProtocol::exit(dbginvoke_t *di, int detach) {
    LOG("%s\n", __PRETTY_FUNCTION__);

    if (!di) {
        PrintOutError("Tried to exit with null di.\n");
        return -1;
    }

    getNetwork<ZMQNetNetwork>(di).exit();
    di->d_proto_net.reset();

    return DebugClientProtocol::exit(di, detach);
}
