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

// Instantiation of the protocol - accessed via DebugClientProtocol::registry
ZMQNetClientProtocol DbgClntProtoZMQ{};

// Helper class for building argv for cti and zmqnet
class ArgvStream {
public:
    ArgvStream() = default;
    template <typename... Args>
    ArgvStream(Args&&... args);

    const std::vector<const char *> c_strs() const {
        std::vector<const char *> c_strs;
        std::transform(m_argv.begin(), m_argv.end(), 
            std::back_inserter(c_strs), [](const std::string &s){return s.c_str();});
        c_strs.push_back(nullptr);
        return c_strs;
    };

    void clear() {m_argv.clear();}

    friend ArgvStream& operator<<(ArgvStream& ab, const std::string &str);
    friend ArgvStream& operator<<(ArgvStream& ab, const char *cstr);
    friend ArgvStream& operator<<(ArgvStream& ab, char *cstr);
    friend ArgvStream& operator<<(ArgvStream& ab, const char *argv[]);
    friend ArgvStream& operator<<(ArgvStream& ab, char *argv[]);
    template <typename T>
      friend ArgvStream& operator<<(ArgvStream& ab, const T &t);
private:
    template <typename T, typename... Args> void push(const T &t, Args&&... args);
    void push();

    std::vector<std::string> m_argv;
};

template <typename... Args>
ArgvStream::ArgvStream(Args&&... args) {
    push(args...);
}

ArgvStream& operator<<(ArgvStream& ab, const std::string &str) {
    ab.m_argv.push_back(str);
    return ab;
}

ArgvStream& operator<<(ArgvStream& ab, const char *cstr) {
    ab.m_argv.push_back(std::string{cstr});
    return ab;
}

ArgvStream& operator<<(ArgvStream& ab, char *cstr) {
    ab.m_argv.push_back(std::string{cstr});
    return ab;
}

// argv must end with a nullptr
ArgvStream& operator<<(ArgvStream& ab, const char *argv[]) {
    for (int i = 0; argv != nullptr && argv[i] != nullptr; i++) {
        ab.m_argv.push_back(std::string{argv[i]});
    }
    return ab;
}

// argv must end with a nullptr
ArgvStream& operator<<(ArgvStream& ab, char *argv[]) {
    for (int i = 0; argv != nullptr && argv[i] != nullptr; i++) {
        ab.m_argv.push_back(std::string{argv[i]});
    }
    return ab;
}

// try to_string if no overload is found
template <typename T>
ArgvStream& operator<<(ArgvStream& ab, const T &t) {
    ab.m_argv.push_back(std::to_string(t));
    return ab;
}

template <typename T, typename... Args>
void ArgvStream::push(const T &t, Args&&... args) {
    *this << t;
    push(args...);
}

void ArgvStream::push() {
    // end variadic arg recursion
}

ZMQNetClientProtocol::ZMQNetClientProtocol()
    : DebugClientProtocol()
    , m_network{nullptr}
{ 
    registerProtocol(); 
}

int ZMQNetClientProtocol::init() {
    PrintOutMsg("%s\n", __PRETTY_FUNCTION__);

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
    PrintOutMsg("%s\n", __PRETTY_FUNCTION__);

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

    PrintOutMsg("sending message %s\n", request);

    m_network->sendCommand(*m_command_stream, request);

    return bitset_dup(target);
}

struct proto_msg_set* ZMQNetClientProtocol::receiveReply(dbginvoke_t *di, 
                                            bitset *target, bitset *received) {
    PrintOutMsg("%s\n", __PRETTY_FUNCTION__);

    // TODO respect target
    // TODO update received

    // wait for result
    PrintOutMsg("waiting for result...\n");
    auto reply = m_result_queue.waitForNext();
    PrintOutMsg("got reply: %s\n", reply.asBytes().begin());

    return nullptr;
}

void ZMQNetClientProtocol::dispatchEvents(dbginvoke_t *di, int ev_fd) {
    PrintOutMsg("%s\n", __PRETTY_FUNCTION__);
}

int ZMQNetClientProtocol::attach(dbginvoke_t *di, proto_session *sess,
                        const char *app_ident, const char *arch,
                        const char *backend, int gpu_flag, int debug_flag) {
    PrintOutMsg("%s\n", __PRETTY_FUNCTION__);
    return 0;
}

int ZMQNetClientProtocol::launch(dbginvoke_t *di, proto_session *sess, const char *prog,
                        char **argv, const char *arch, char **arch_args,
                        const char *arch_input, const char *backend, const char* gdb, char **env,
                        const char *cwd, int gpu_flag, int debug_flag,
                        int early_entry_flag, bool isNonStop) {
    PrintOutMsg("%s\n", __PRETTY_FUNCTION__);

    PrintOutMsg("Starting application, please wait...\n");

    // launch n ranks of app being debugged ////////////////////////////////////

    int prog_outp = -1;
    if ((di->d_proto_outp = DbgCreateGenericProgOutp()) != nullptr) {
        prog_outp = ((g_progoutp_t *)di->d_proto_outp)->p_o_wfd;
    }

    auto launcherArgs = ArgvStream("-n", di->d_nproc, arch_args, prog, argv);
    if ((di->d_appid = cti_launchAppBarrier(launcherArgs.c_strs().data(), prog_outp, prog_outp, arch_input, cwd, env)) == 0) {
        PrintOutError("Application launch failed.\n");

        // TODO: dump error messages
    }

    PrintOutMsg("launched app\n");

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

    PrintOutMsg("added files\n");

    // create network //////////////////////////////////////////////////////////

    auto dbgsrvAlpsArgs = ArgvStream(
        "dbgsrv_alps",
        "-h", "zmq",   // meaningless for zmqnet, but required to be non-null by dbgsrv
        "-c", "1",     // meaningless for zmqnet, but required to be non-null by dbgsrv
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

    PrintOutMsg("zmqnet argv:\n");
    for (const auto cstr : dbgsrvAlpsArgs.c_strs()) {
        PrintOutMsg("%s ", cstr);
    }
    PrintOutMsg("\n");

    // TODO environment variables

    m_network = std::make_unique<zmqnet::AsyncFrontend>(di->d_appid, session_id, manifest_id, 
        zmqnet::Topology{ .launch = zmqnet::Topology::Fcfs, .fanout = 4},
        dbgsrvAlpsArgs.c_strs().data()  
    );

    { // create send command stream
        const auto command_callback = [&](zmqnet::Stream::Id const& streamId, zmqnet::ByteArrayPtr result) {
            // TODO check errors
            fprintf(stderr, "command_callback: %d %s\n", streamId, result.asBytes().begin());

            m_result_queue.push(result);
            fprintf(stderr, "successfully pushed result: %d %s\n", streamId, result.asBytes().begin());
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

    PrintOutMsg("Made it out!\n");

    return 0;
}

void ZMQNetClientProtocol::destroyClient(dbginvoke *di) {
    PrintOutMsg("%s\n", __PRETTY_FUNCTION__);
}

int ZMQNetClientProtocol::exit(dbginvoke_t *di, int detach) {
    PrintOutMsg("%s\n", __PRETTY_FUNCTION__);

    if (m_network) m_network->runExit();

    if (!detach) {
        // send a kill signal using the CTI
        if (cti_killApp(di->d_appid, SIGKILL)) {
            PrintOutError("Failed to kill application process!\n");
            PrintOutError("CTI error: %s\n", cti_error_str());
            return -1;
        }
    }

    cti_deregisterApp(di->d_appid);

    di->d_appid = 0;

    return 0;
}

int ZMQNetAttachHandler::handleEvent(dbginvoke_t *di, proto_msg_set *set) {
    PrintOutMsg("ZMQNetAttachHandler::handleEvent\n");
    return 0;
}

int ZMQNetLaunchHandler::handleEvent(dbginvoke_t *di, proto_msg_set *set) {
    PrintOutMsg("ZMQNetAttachHandler::handleEvent\n");
    return 0;
}
