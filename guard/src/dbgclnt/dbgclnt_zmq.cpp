/******************************************************************************\
 * Copyright 2021 Hewlett Packard Enterprise Development LP.
 ******************************************************************************/

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
    return 0;
}

int ZMQNetClientProtocol::launch(dbginvoke_t *di, proto_session *sess, const char *prog,
                        const char * const *argv, const char *arch, const char * const *arch_args,
                        const char *arch_input, const char *backend, const char* gdb, char **env,
                        const char *cwd, int gpu_flag, int debug_flag,
                        int early_entry_flag, bool isNonStop) {
    LOG("%s\n", __PRETTY_FUNCTION__);

    if (!di) {
        PrintOutError("Tried to launch ZMQNet with a null di\n");
        return 1;
    }

    PrintOutMsg("Starting application, please wait...\n");

    // launch n ranks of app being debugged ////////////////////////////////////

    int prog_outp = -1;
    if ((di->d_proto_outp = DbgCreateGenericProgOutp()) != nullptr) {
        prog_outp = di->d_proto_outp->p_o_wfd;
    }

    auto launcherArgs = ArgvStream("-n", di->d_nproc, arch_args, prog, argv);
    if ((di->d_appid = cti_launchAppBarrier(launcherArgs.c_strs().data(), prog_outp, prog_outp, arch_input, cwd, env)) == 0) {
        PrintOutError("Application launch failed.\n");

        // TODO: dump error messages
    }

    PrintOutMsg("Launched application...\n");

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
        "-e", backend,
        "-f", prog
    );
    if (gpu_flag)         dbgsrvArgs << "--gpu";
    if (debug_flag)       dbgsrvArgs << "--debug";
    if (early_entry_flag) dbgsrvArgs << "--early-entry";
    if (isNonStop)        dbgsrvArgs << "--non-stop";
    if (gdb && serverGdb) dbgsrvArgs << "--gdb" << *serverGdb;

#if DEBUG
    LOG("zmqnet argv:\n");
    for (const auto cstr : dbgsrvArgs.c_strs()) {
        LOG("%s ", cstr);
    }
    LOG("\n");
#endif
    // TODO enable failure callback ?

    auto num_be = di->d_nproc;
    di->d_bitset_full = std::make_unique<DenseBitset>(num_be);
    di->d_bitset_full->invert();

    di->d_bitset_launched = std::make_unique<DenseBitset>(num_be);

    // TODO environment variables
    auto zmqnet = std::make_unique<zmqnet::AsyncFrontend>(di->d_appid, session_id, manifest_id, 
        zmqnet::Topology{ .launch = zmqnet::Topology::Packed, .fanout = 4},
        dbgsrvArgs.c_strs().data()  
    );
    di->d_proto_net = std::make_unique<ZMQNetNetwork>(std::move(zmqnet), di->d_bitset_full->clone());

    PrintOutMsg("Created network...\n");

    // send invoke
    auto invoke = CommandToSocProto(DBGCMD_INVOKE, num_be);
    sendCommand(di, di->d_bitset_full.get(), invoke.c_str());

    // wait for response
    auto received{DenseBitset(num_be)};
    receiveReply(di, di->d_bitset_full.get(), received);

    // TODO check received

    // TODO set program output stream handler

    PrintOutMsg("Connected to application...\n");

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
