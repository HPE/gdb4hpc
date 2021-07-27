/******************************************************************************\
 * Copyright 2021 Hewlett Packard Enterprise Development LP.
 ******************************************************************************/

#include <algorithm>
#include <cstdlib>
#include <filesystem>

#include "dbg_client.h"
#include "dbgclnt_zmq.h"
#include "dbgclnt_proto_ctiship.h"
#include "defs.h"
#include "lrunle_bitset.h"
#include "useful.h"

#include "dbg_zmq.capnp.h"
#include "capnp/serialize.h"

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
    free(base);

    return 1;
}

std::unique_ptr<Bitset> ZMQNetClientProtocol::sendCommand(dbginvoke_t *di, const Bitset *target,
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

    return target->clone();
}

std::optional<ProtoMessageSet> ZMQNetClientProtocol::receiveReply(dbginvoke_t *di, 
                                            const Bitset *target, Bitset &received) {
    LOG("%s\n", __PRETTY_FUNCTION__);

    if (di->d_isShutdown) {
        return {};
    }

    // TODO respect target

    // wait for result
    LOG("waiting for result...\n");
    
    std::unique_lock<std::mutex> lock(m_command_result_lock);
    m_command_result_cv.wait(lock, [&](){ return m_command_result_ready; });

    auto reply = std::move(m_command_result);
    m_command_result_ready = false;
    
    auto reader = getReaderFromData(reply);
    auto pack = reader.getRoot<ZmqnetMessagePackage>();
    auto result = ProtoMessageSet();

    for (const auto message : pack.getMessages()) {
        auto unpacked = unpackMessageReader(message);
    
        received |= *unpacked.bitset;

        result.emplace_back(std::move(unpacked.bitset), std::move(unpacked.data));
    }

    return result;
}

int ZMQNetClientProtocol::checkEvents(dbginvoke_t *di) {
    // LOG("%s\n", __PRETTY_FUNCTION__);
    return m_event_queue.hasEvent() ? 1 : 0;
}

void ZMQNetClientProtocol::dispatchEvents(dbginvoke_t *di, int ev_fd) {
    // zmqnet doesn't use event file descriptors, so ev_fd will always be -1
    assert(ev_fd == -1);

    auto messages = ProtoMessageSet{};

    while (auto optionalEventData = m_event_queue.getNext()) {
        auto eventData = std::move(*optionalEventData);

        auto reader = getReaderFromData(eventData);
        auto pack = reader.getRoot<ZmqnetMessagePackage>();

        LOG("got reader\n");

        for (const auto &message : pack.getMessages()) {
            auto unpacked = unpackMessageReader(message);
            messages.emplace_back(std::move(unpacked.bitset), std::move(unpacked.data));
        }
    }
    
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

    // This is rolled into the protocol with zmqnet
    di->d_proto_net = nullptr;

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

    // TODO environment variables

    m_network = std::make_unique<zmqnet::AsyncFrontend>(di->d_appid, session_id, manifest_id, 
        zmqnet::Topology{ .launch = zmqnet::Topology::Packed, .fanout = 4},
        dbgsrvArgs.c_strs().data()  
    );

    { // create send command stream
        const auto command_callback = [&](zmqnet::Stream::Id const& streamId, zmqnet::ByteArrayPtr result) {
            // TODO check errors
            LOG("command_callback: %d %s\n", streamId, result.begin());

            std::unique_lock<std::mutex> lock(m_command_result_lock);

            // copies data and destroys any data that was in the command result previously
            m_command_result = zmqnet::make_ByteArray(result.begin(), result.size());

            // notify a thread waiting for the result, if present
            m_command_result_ready = true;
            lock.unlock();
            m_command_result_cv.notify_one();
        };

        const auto streamId = static_cast<zmqnet::Stream::Id>(ZMQNetStream::CommandStream);
        m_network->registerStreamCallback(streamId, command_callback);
        auto allNodesCommunicator = m_network->getAllNodesCommunicator();
        m_command_stream = std::make_unique<zmqnet::Stream>(m_network->registerStream(allNodesCommunicator, streamId,
            "libzmqnetagg.so", "run", zmqnet::sync::WaitForAll{}));
    }

    { // create async event stream
        const auto event_callback = [&](zmqnet::Stream::Id const& streamId, zmqnet::ByteArrayPtr result) {
            LOG("event_callback %d %s\n", streamId, result.begin());

            m_event_queue.enqueue(zmqnet::make_ByteArray(result.begin(), result.size()));
            LOG("event_callback %d, successfully enqueued\n", streamId);
        };

        const auto streamId = static_cast<zmqnet::Stream::Id>(ZMQNetStream::EventStream);
        m_network->registerStreamCallback(streamId, event_callback);
        auto allNodesCommunicator = m_network->getAllNodesCommunicator();
        m_event_stream = std::make_unique<zmqnet::Stream>(m_network->registerStream(allNodesCommunicator, streamId,
            "libzmqnetagg.so", "run", zmqnet::sync::Timeout{400}));
    }

    PrintOutMsg("Created network...\n");

    // TODO enable failure callback ?

    auto num_be = di->d_nproc;
    di->d_bitset_full = std::make_unique<DenseBitset>(num_be);
    di->d_bitset_full->invert();

    di->d_bitset_launched = std::make_unique<DenseBitset>(num_be);

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
}

int ZMQNetClientProtocol::exit(dbginvoke_t *di, int detach) {
    LOG("%s\n", __PRETTY_FUNCTION__);

    if (!di) {
        PrintOutError("Tried to exit with null di.\n");
        return -1;
    }

    if (m_network) m_network->runExit();

    PrintOutMsg("Disconnected from network...\n");
    
    PrintOutMsg("Finishing up...\n");
    return DebugClientProtocol::exit(di, detach);
}

capnp::UnalignedFlatArrayMessageReader ZMQNetClientProtocol::getReaderFromData(const zmqnet::ByteArray &data) const {
    assert(data.size() != 0);
    assert(data.size() % sizeof(capnp::word) == 0);
    auto words_ptr = reinterpret_cast<capnp::word const *>(data.begin());
    auto words_size = data.size() / sizeof(capnp::word);
    auto words_array = kj::arrayPtr(words_ptr, words_size);

    return capnp::UnalignedFlatArrayMessageReader(words_array);
}

ZMQNetClientProtocol::UnpackedMessage ZMQNetClientProtocol::unpackMessageReader(const ZmqnetMessagePackage::ZmqnetMessage::Reader &reader) const {
    LOG("unpacking: \n");
    LOG("\tdata: %s\n", reader.getData().cStr());
    LOG("\thash: %lu\n", reader.getHash());
    auto bitsetPtr = reader.getBitset().asChars().begin();
    auto bitset = std::make_unique<LRunLEBitset>(bitsetPtr);
    LOG("\tbitset: %s\n", bitset->toSet().c_str());

    return {
        .data = std::unique_ptr<char, decltype(&std::free)>(strndup(reader.getData().cStr(), reader.getData().size()), std::free), 
        .hash = reader.getHash(), 
        .bitset = std::move(bitset)
    };
}

bool ZMQNetClientProtocol::EventQueue::hasEvent() {
    std::scoped_lock<std::mutex> lock(m_queue_lock);
    return m_queue.size() > 0;
}

std::optional<zmqnet::ByteArray> ZMQNetClientProtocol::EventQueue::getNext() {
    std::scoped_lock<std::mutex> lock(m_queue_lock);

    if (m_queue.size() > 0) {
        auto result = std::move(m_queue.front()); 
        m_queue.pop();
        return result; 
    } else {
        return {};
    }
}

void ZMQNetClientProtocol::EventQueue::enqueue(zmqnet::ByteArray &&event) {
    std::scoped_lock<std::mutex> lock(m_queue_lock);

    m_queue.push(std::move(event));
}
