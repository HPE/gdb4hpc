/******************************************************************************\
 * Copyright 2021 Hewlett Packard Enterprise Development LP.
 ******************************************************************************/

#include <algorithm>
#include <cstdlib>
#include <filesystem>

#include "dbg_client.h"
#include "dbgclnt_zmq.h"
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

    if (!m_command_send_stream) {
        PrintOutError("Tried to send a message without a command stream.\n");
        return nullptr;
    }

    if (di->d_isShutdown) return nullptr;

    // TODO get/create specific stream for target

    PrintOutMsg("sending message %s\n", request);

    m_network->sendCommand(*m_command_send_stream, request);

    return bitset_dup(target);
}

struct proto_msg_set* ZMQNetClientProtocol::receiveReply(dbginvoke_t *di, 
                                            bitset *target, bitset *received) {
    PrintOutMsg("%s\n", __PRETTY_FUNCTION__);

    // TODO respect target
    // TODO update received

    // request reply be fetched and sent to result queue
    m_network->sendCommand(*m_command_receive_stream, "ping");

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
    auto serverGdb = std::string{};
    if (gdb) {
        auto gdbBin = std::string{gdb};
        auto colon = gdbBin.find(':');
        auto dataDir = std::string{};
        if (colon != std::string::npos) {
            dataDir = gdbBin.substr(colon+1);
            gdbBin = gdbBin.substr(0,colon);
            if (!dataDir.empty() && dataDir[0] != '/') {
                PrintOutError("adding a dataDir to the manifest is not supported.\n");
                return 1;
            }
        }

        if (gdbBin.empty()) {
            PrintOutError("--gdb executable name is empty\n");
            return 1;
        } else if (gdbBin[0] == '/') {
            serverGdb = gdbBin;
        } else {
            if (cti_addManifestBinary(manifest_id, gdbBin.c_str())) {
                PrintOutError("Failed to add %s to manifest.\n", gdbBin.c_str());
                PrintOutError("CTI error: %s\n", cti_error_str());
                return 1;
            }
            serverGdb = std::filesystem::path(gdbBin).filename();
        }

        if (!dataDir.empty()) {
            serverGdb += ":";
            serverGdb += dataDir;
        }
    } else if (cti_addManifestBinary(manifest_id, "gdb-guard")) {
        PrintOutError("Failed to add gdb-guard to manifest.\n");
        PrintOutError("CTI error: %s\n", cti_error_str());
        return 1;
    }

    // TODO gpu_flag check

    if (cti_addManifestBinary(manifest_id, "dbgsrv")) {
        PrintOutError("CTI error: %s\n", cti_error_str());
        PrintOutError("Failed to add dbgsrv to manifest.\n");
        return 1;
    }

    if (cti_addManifestBinary(manifest_id, "dbgsrv_alps")) {
        PrintOutError("CTI error: %s\n", cti_error_str());
        PrintOutError("Failed to add dbgsrv-alps to manifest.\n");
        return 1;
    }

    // send over the uda plugin - only if gpu mode is currently turned off
    if (!gpu_flag) {
        if (cti_addManifestLibrary(manifest_id, "uda-plugin-cray-upc.so")) {
            PrintOutError(
                "Failed to add uda-plugin-cray-upc.so to manifest.\n");
            PrintOutError("CTI error: %s\n", cti_error_str());
            return 1;
        }
    }

    // TODO ompd

    if (cti_addManifestLibrary(manifest_id, "libthread_db.so.1")) {
        PrintOutError("CTI error: %s\n", cti_error_str());
        PrintOutError("Failed to add libthread_db.so.1 to manifest.\n");
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
        "-h", "zmq",
        "-c", "1",
        "-a", arch,
        "-p", "zmqnet",
        "-e", backend,
        "-f", prog
    );
    if (gpu_flag)         dbgsrvAlpsArgs << "--gpu";
    if (debug_flag)       dbgsrvAlpsArgs << "--debug";
    if (early_entry_flag) dbgsrvAlpsArgs << "--early-entry";
    if (isNonStop)        dbgsrvAlpsArgs << "--non-stop";
    if (gdb)              dbgsrvAlpsArgs << "--gdb" << serverGdb;

    PrintOutMsg("zmqnet argv:\n");
    for (const auto cstr : dbgsrvAlpsArgs.c_strs()) {
        PrintOutMsg("%s ", cstr);
    }
    PrintOutMsg("\n");

    // TODO environment variables

    m_network = std::make_unique<zmqnet::AsyncFrontend>(di->d_appid, session_id, manifest_id, 
        zmqnet::Topology{ .launch = zmqnet::Topology::Fcfs, .fanout = 1},
        dbgsrvAlpsArgs.c_strs().data()  
    );

    { // create send command stream
            const auto command_send_callback = [&](zmqnet::Stream::Id const& streamId, zmqnet::ByteArrayPtr result) {
            // TODO check errors
            fprintf(stderr, "command_send_callback: %d %s\n", streamId, result.asBytes().begin());
        };

        // TODO this is ugly!
        const auto streamId = static_cast<zmqnet::Stream::Id>(ZMQNetStream::CommandStreamSend);
        m_network->registerStreamCallback(streamId, command_send_callback);
        auto allNodesCommunicator = m_network->getAllNodesCommunicator();
        m_command_send_stream = std::make_unique<zmqnet::Stream>(m_network->registerStream(allNodesCommunicator, streamId,
            "libzmqnetagg.so", "run", zmqnet::sync::WaitForAll{}));
    }

    { // create recieve command stream
        const auto command_receive_callback = [&](zmqnet::Stream::Id const &streamId, zmqnet::ByteArrayPtr result) {
            // push result to result queue
            fprintf(stderr, "command_receive_callback: %d %s\n", streamId, result.asBytes().begin());
            m_result_queue.push(result);
            fprintf(stderr, "successfully pushed result: %d %s\n", streamId, result.asBytes().begin());
        };

        // TODO this is ugly!
        const auto streamId = static_cast<zmqnet::Stream::Id>(ZMQNetStream::CommandStreamReceive);
        m_network->registerStreamCallback(streamId, command_receive_callback);
        auto allNodesCommunicator = m_network->getAllNodesCommunicator();
        m_command_receive_stream = std::make_unique<zmqnet::Stream>(m_network->registerStream(allNodesCommunicator, streamId,
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
