/******************************************************************************\
 * Copyright 2021 Hewlett Packard Enterprise Development LP.
 ******************************************************************************/

#include <algorithm>
#include <cstdlib>
#include <filesystem>

#include "dbg_client.h"
#include "dbgclnt_zmq.h"
#include "dbgclnt_proto_help.h"
#include "defs.h"
#include "bitset.h"
#include "useful.h"

#include "common_tools_fe.h"

// TODO wrap this in #if DEBUG
#if 1
#define LOG(...) fprintf(stderr, __VA_ARGS__)
#else
#define LOG(...) /**/
#endif

// Instantiation of the protocol - accessed via DebugClientProtocol::registry
ZMQNetClientProtocol DbgClntProtoZMQ{};

ZMQNetClientProtocol::ZMQNetClientProtocol()
    : DebugClientProtocol()
    , m_network{nullptr}
    , m_command_result_ready{false}
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

    return 1;
}

bitset* ZMQNetClientProtocol::sendCommand(dbginvoke_t *di, bitset *target,
                             const char *request) {
    LOG("%s\n", __PRETTY_FUNCTION__);

    if (!m_network) {
        PrintOutError("Tried to send a message without a network.\n");
        return nullptr;
    }

    if (!m_command_stream) {
        PrintOutError("Tried to send a message without a command stream.\n");
        return nullptr;
    }

    if (di->d_isShutdown) return nullptr;

    // TODO get/create specific stream for target

    LOG("sending message %s\n", request);

    m_network->sendCommand(*m_command_stream, request);

    return bitset_dup(target);
}

struct proto_msg_set* ZMQNetClientProtocol::receiveReply(dbginvoke_t *di, 
                                            bitset *target, bitset *received) {
    LOG("%s\n", __PRETTY_FUNCTION__);

    // TODO respect target
    // TODO update received

    // wait for result
    LOG("waiting for result...\n");
    
    std::unique_lock<std::mutex> lock(m_command_result_lock);
    m_command_result_cv.wait(lock, [&](){ return m_command_result_ready; });

    auto reply = std::move(m_command_result);
    m_command_result.erase();
    m_command_result_ready = false;
    
    LOG("got reply: %s\n", reply.c_str());

    // TODO process reply

    return nullptr;
}

void ZMQNetClientProtocol::dispatchEvents(dbginvoke_t *di, int ev_fd) {
    LOG("%s\n", __PRETTY_FUNCTION__);
}

int ZMQNetClientProtocol::attach(dbginvoke_t *di, proto_session *sess,
                        const char *app_ident, const char *arch,
                        const char *backend, int gpu_flag, int debug_flag) {
    LOG("%s\n", __PRETTY_FUNCTION__);
    return 0;
}

int ZMQNetClientProtocol::launch(dbginvoke_t *di, proto_session *sess, const char *prog,
                        char **argv, const char *arch, char **arch_args,
                        const char *arch_input, const char *backend, const char* gdb, char **env,
                        const char *cwd, int gpu_flag, int debug_flag,
                        int early_entry_flag, bool isNonStop) {
    LOG("%s\n", __PRETTY_FUNCTION__);

    if (!di) {
        PrintOutError("Tried to launch ZMQNet with a null di\n");
        return 1;
    }

    // This is rolled into the protocol with zmqnet
    di->d_proto_net = nullptr;

    LOG("Starting application, please wait...\n");

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

    LOG("launched app\n");

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

    // user can specify a custom gdb binary and a (full path) datadir
    auto serverGdb = shipGdb(manifest_id, gdb);
    if (!serverGdb) {
        PrintOutError("Error shipping gdb.\n");
        return 1;
    }

    if (gpu_flag && !gdb && !shipGpuGdbs(manifest_id)) {
        PrintOutError("Error shipping gpu gdbs.\n");
        return 1;
    }

    if (!shipCommonFiles(manifest_id, gpu_flag)) {
        PrintOutError("Error shipping common files.\n");
        return 1;
    }

    if (!shipOmpd(manifest_id)) {
        PrintOutMsg("Error shipping OMPD files. OMPD support will not be available.\n");
    }

    if (cti_addManifestBinary(manifest_id, "dbgsrv_alps")) {
        PrintOutError("CTI error: %s\n", cti_error_str());
        PrintOutError("Failed to add dbgsrv-alps to manifest.\n");
        return 1;
    }

    // TODO support --with-valgrind

    if (cti_addManifestLibrary(manifest_id, "libzmqnetagg.so")) {
        PrintOutError("CTI error: %s\n", cti_error_str());
        PrintOutError("Failed to add %s to manifest.\n", "libzmqnetagg.so");
        return 1;
    }

    LOG("added files\n");

    // create network //////////////////////////////////////////////////////////

    auto dbgsrvAlpsArgs = ArgvStream(
        "dbgsrv_alps",
        "-h", "zmq",   // meaningless for zmqnet, but required to be non-null by dbgsrv
        "-c", "1",     // meaningless for zmqnet, but required to be non-zero by dbgsrv
        "-a", arch,
        "-p", "zmqnet",
        "-e", backend,
        "-f", prog
    );
    if (gpu_flag)         dbgsrvAlpsArgs << "--gpu";
    if (debug_flag)       dbgsrvAlpsArgs << "--debug";
    if (early_entry_flag) dbgsrvAlpsArgs << "--early-entry";
    if (isNonStop)        dbgsrvAlpsArgs << "--non-stop";
    if (gdb && serverGdb) dbgsrvAlpsArgs << "--gdb" << *serverGdb;

    LOG("zmqnet argv:\n");
    for (const auto cstr : dbgsrvAlpsArgs.c_strs()) {
        LOG("%s ", cstr);
    }
    LOG("\n");

    // TODO environment variables

    m_network = std::make_unique<zmqnet::AsyncFrontend>(di->d_appid, session_id, manifest_id, 
        zmqnet::Topology{ .launch = zmqnet::Topology::Fcfs, .fanout = 4},
        dbgsrvAlpsArgs.c_strs().data()  
    );

    { // create send command stream
        const auto command_callback = [&](zmqnet::Stream::Id const& streamId, zmqnet::ByteArrayPtr result) {
            // TODO check errors
            fprintf(stderr, "command_callback: %d %s\n", streamId, result.asBytes().begin());

            std::unique_lock<std::mutex> lock(m_command_result_lock);
            m_command_result = std::string{result.asChars().begin(), result.size()};
            m_command_result_ready = true;
            lock.unlock();
            m_command_result_cv.notify_one();
        };

        // TODO this is ugly!
        const auto streamId = static_cast<zmqnet::Stream::Id>(ZMQNetStream::CommandStream);
        m_network->registerStreamCallback(streamId, command_callback);
        auto allNodesCommunicator = m_network->getAllNodesCommunicator();
        m_command_stream = std::make_unique<zmqnet::Stream>(m_network->registerStream(allNodesCommunicator, streamId,
            "libzmqnetagg.so", "run", zmqnet::sync::WaitForAll{}));
    }

    // TODO enable failure callback ?

    auto num_be = di->d_nproc;
    di->d_bitset_full = bitset_new(num_be);
    bitset_invert(di->d_bitset_full);

    di->d_bitset_launched = bitset_new(num_be);

    // TODO setup event notification fd for io?
    // TODO setup event notification fd for event?

    // TODO send INVOKE

    // TODO wait for response

    // TODO set program output stream handler

    LOG("Made it out!\n");

    return 0;
}

void ZMQNetClientProtocol::destroyClient(dbginvoke *di) {
    LOG("%s\n", __PRETTY_FUNCTION__);
}

int ZMQNetClientProtocol::exit(dbginvoke_t *di, int detach) {
    LOG("%s\n", __PRETTY_FUNCTION__);

    if (!di) {
        PrintOutError("Tried to exit with null di.\n");
        return -1;
    }

    if (m_network) m_network->runExit();

    return DebugClientProtocol::exit(di, detach);
}

int ZMQNetAttachHandler::handleEvent(dbginvoke_t *di, proto_msg_set *set) {
    LOG("ZMQNetAttachHandler::handleEvent\n");
    return 0;
}

int ZMQNetLaunchHandler::handleEvent(dbginvoke_t *di, proto_msg_set *set) {
    LOG("ZMQNetAttachHandler::handleEvent\n");
    return 0;
}
