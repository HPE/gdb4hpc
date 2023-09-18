/******************************************************************************\
 * Copyright 2021 Hewlett Packard Enterprise Development LP.
 ******************************************************************************/
#include "dbg_zmq_network.h"

#include "lrunle_bitset.h"
#include "bitset_convert.h"

#include "dbg_client.h"
#include "signal_util.h"

#include <poll.h>

#ifdef DEBUG
#define LOG(...) fprintf(stderr, __VA_ARGS__)
#else
#define LOG(...) /**/
#endif

namespace {

struct UnpackedMessage {
    std::unique_ptr<char, decltype(&std::free)> data;
    uint64_t hash;
    std::unique_ptr<DenseBitset> bitset;
};

UnpackedMessage unpackMessageReader(const ZmqnetMessagePackage::ZmqnetMessage::Reader &reader) {
    LOG("unpacking: \n");
    LOG("\tdata: %s\n", reader.getData().cStr());
    LOG("\thash: %lu\n", reader.getHash());
    auto bitsetPtr = reader.getBitset().asChars().begin();
    auto lrunbitset = std::make_unique<LRunLEBitset>(bitsetPtr);
    LOG("\tbitset: %s\n", lrunbitset->toSetString().c_str());
    auto densebitset = bitset::createDenseFrom(*lrunbitset);

    return {
        .data = std::unique_ptr<char, decltype(&std::free)>(strndup(reader.getData().cStr(), reader.getData().size()), std::free), 
        .hash = reader.getHash(), 
        .bitset = std::move(densebitset)
    };
}

} // anonymous namespace

ZMQNetNetwork::ZMQNetNetwork(std::unique_ptr<zmqnet::AsyncFrontend> &&frontend, OutputPtr &&output, std::unique_ptr<Bitset> &&allRanks)
    : m_network{std::move(frontend)}
    , m_output_watcher{std::move(output)}
    , m_stream_table(*m_network, std::move(allRanks))
{
    // Generate an error if a dbgsrv exits unexpectedly
    m_network->registerCallback<zmqnet::Event::LeafExit>([this](int rank) {
        // rank will be 0-indexed, so add 1 for size argument
        auto bitset = std::make_unique<SparseBitset>(rank + 1);
        bitset->set(rank);

        // generate an error event. the message will have rank info prepended
        // by the async callback handling functions
        auto error = NewErrorEvent(DBGERR_INTERNAL,
            "Remote debugger disappeared. This is a bug. Please report it!",
            rank
        );

        // Async event callbacks want messages in soc proto form.
        // EventToSocProto free()'s error.
        auto socProto = EventToSocProto(error);

        m_stream_table.injectAsyncEvent(ProtoMessage(std::move(bitset), std::move(socProto)));
    });
}

std::unique_ptr<Bitset> ZMQNetNetwork::sendCommand(const Bitset *target, const char *request) {
    LOG("%s\n", __PRETTY_FUNCTION__);

    if (!m_network) {
        PrintOutError("Tried to send a message without a network.\n");
        return nullptr;
    }

    LOG("sending message %s\n", request);

    auto &commandStream = m_stream_table.getCommandStream(target);
    commandStream.sendCommand(request, strlen(request) + 1); // include null terminator

    return target->clone();
}

std::variant<ProtoMessageSet, ProtoNothing>
ZMQNetNetwork::receiveReply(const Bitset *target, std::chrono::seconds timeout, Bitset &received) {
    LOG("%s\n", __PRETTY_FUNCTION__);

    // wait for result
    LOG("waiting for result...\n");

    auto &stream = m_stream_table.getCommandStream(target);
    auto result = stream.extractResult(timeout);

    // if we received a result, update received bitset
    if (auto* message_set = std::get_if<ProtoMessageSet>(&result)) {
        for (const auto& res : *message_set) {
            received |= *res.getBitset();
        }
    }

    return result;
}

int ZMQNetNetwork::checkEvents() {
    // LOG("%s\n", __PRETTY_FUNCTION__);
    return (m_stream_table.hasAsyncEvent() || m_output_watcher.hasOutput()) ? 1 : 0;
}

ProtoMessageSet ZMQNetNetwork::releaseEvents() {
    // LOG("%s\n", __PRETTY_FUNCTION__);
    return m_stream_table.releaseAsyncEvents();
}

std::vector<std::string> ZMQNetNetwork::releaseOutputLines() {
    return m_output_watcher.releaseOutputLines();
}

int ZMQNetNetwork::syncExit() {
    LOG("%s\n", __PRETTY_FUNCTION__);

    if (m_network) {
        m_network->removeCallback<zmqnet::Event::LeafExit>();
        m_network->runExit();
    }

    return 0;
}

int ZMQNetNetwork::forceExit() {
    LOG("%s\n", __PRETTY_FUNCTION__);

    if (m_network) {
        m_network->removeCallback<zmqnet::Event::LeafExit>();
        m_network->forceDisconnect();
    }

    return 0;
}

ZMQNetNetwork::StreamTable::CommandStream::CommandStream(zmqnet::AsyncFrontend &parentNetwork, zmqnet::Stream::Id streamId, zmqnet::Communicator communicator) 
    : m_parentNetwork{parentNetwork}
{
    // create "sync" stream
    const auto command_callback = [&](zmqnet::Stream::Id const &, zmqnet::ByteArrayPtr resultPtr) {
        LOG("command_callback: %d %s\n", streamId, resultPtr.begin());

        auto result =
            zmqnet::make_ByteArray(resultPtr.begin(), resultPtr.size());
        {
            std::scoped_lock<std::mutex> lock(m_shared_result.mutex);
            // destroys any data that was in the command result previously
            m_shared_result.data = std::move(result);
            m_shared_result.results_received += 1;
        }

        // notify a thread waiting for the result, if present
        m_shared_result.cv.notify_one();
    };

    parentNetwork.registerStreamCallback(streamId, command_callback);
    m_stream = std::make_unique<zmqnet::Stream>(
        parentNetwork.registerStream(communicator, streamId, "libzmqnetagg.so",
                                     "run", zmqnet::sync::WaitForAll{}));
}

ZMQNetNetwork::StreamTable::CommandStream::CommandStream(zmqnet::AsyncFrontend &parentNetwork, ZMQNetStream streamId, zmqnet::Communicator communicator) 
    : CommandStream(parentNetwork, static_cast<zmqnet::Stream::Id>(streamId), communicator)
{}

void ZMQNetNetwork::StreamTable::CommandStream::sendCommand(const char *command, size_t len) {
    if (!m_stream) {
        throw std::runtime_error("CommandStream tried to send a command with a null stream");
    }

    LOG("CommandStream %d: sending \"%s\" of length %lu\n", m_stream->id, command, len);

    m_parentNetwork.sendCommand(*m_stream, command, len);
    m_commands_sent += 1;
}

std::variant<ProtoMessageSet, ProtoNothing>
ZMQNetNetwork::StreamTable::CommandStream::extractResult(std::chrono::seconds timeout) {
    LOG("CommandStream %d: extracting... (%lus timeout)\n", m_stream->id, timeout.count());
    return internalExtractResult(/* cancellable */ true, timeout);
}

ProtoMessageSet ZMQNetNetwork::StreamTable::CommandStream::extractResultWait() {
    // XXX: Use extractResult() if possible. This function should be removed. It only
    // exists now because of its use in new command stream creation. Right now, it's
    // impossible to recover from an error (cancels are errors) during that process.

    LOG("CommandStream %d: extracting (uncancellable)...\n", m_stream->id);
    // if cancellable is false and no timeout is used,
    // the only way for internalExtractResult to return is with a result.
    return std::get<ProtoMessageSet>(
        internalExtractResult(/* cancellable */ false, /* no timeout */ std::chrono::seconds(-1))
    );
}

// for now, having one shared bool for all command streams is okay because the main
// thread sequentially operates on only one command stream at a time.
std::atomic<bool> g_extract_result_cancel_flag = false;

void cancelExtractResult(int signal) {
    if (signal == SIGINT) g_extract_result_cancel_flag = true;
}

std::variant<ProtoMessageSet, ProtoNothing>
ZMQNetNetwork::StreamTable::CommandStream::internalExtractResult(bool cancellable, std::chrono::seconds timeout) {
    LOG("CommandStream %d: internalExtractResult (cancellable == %d) (timeout = %lds)...\n",
        m_stream->id, cancellable, timeout.count()
    );

    // set up cancellation criteria

    g_extract_result_cancel_flag = false;
    auto signal_handler = cancellable ? std::make_unique<TmpSignalHandler>(SIGINT, cancelExtractResult) : nullptr;

    using namespace std::chrono_literals;
    auto time_elapsed = 0ms;
    auto wait_interval = 250ms;

    auto stopWaiting = [&](){
        return (m_commands_sent == m_shared_result.results_received) // message arrived
            || g_extract_result_cancel_flag                          // user cancelled wait
            || (timeout.count() > 0 && time_elapsed >= timeout);     // timed out
    };

    // wait for result or cancellation

    // this lock should be held for the remainder of the function (except when in wait_for)
    // so that the zmqnet callback doesn't change the result out from under us
    std::unique_lock<std::mutex> lock(m_shared_result.mutex);

    while (!m_shared_result.cv.wait_for(lock, wait_interval, stopWaiting)) {
        time_elapsed += wait_interval;
        LOG("waiting on cv... (sent %lu == recvd %lu) (%lums passed)\n",
            m_commands_sent, m_shared_result.results_received.load(), time_elapsed.count()
        );
    }

    // handle result

    if (m_commands_sent == m_shared_result.results_received) {
        // the result got here
        LOG("done waiting. got result\n");

        // unpack data and construct proto message set
        auto reader = getReaderFromData(m_shared_result.data);
        auto pack = reader.getRoot<ZmqnetMessagePackage>();
        auto result = ProtoMessageSet();

        for (const auto message : pack.getMessages()) {
            auto unpacked = unpackMessageReader(message);
            result.emplace_back(std::move(unpacked.bitset), std::move(unpacked.data));
        }

        return result;
    } else if (time_elapsed >= wait_interval) {
        // timed out
        LOG("done waiting. timed out\n");
        return ProtoNothing(ProtoNothing::Why::TimedOut);
    } else {
        // the user cancelled the operation
        LOG("done waiting. user cancelled wait.\n");
        return ProtoNothing(ProtoNothing::Why::CancelledByUser);
    }
}

ZMQNetNetwork::StreamTable::StreamTable(
    zmqnet::AsyncFrontend &parentNetwork, std::unique_ptr<Bitset> &&allRanks)
    : m_parentNetwork{parentNetwork}
    , m_internalCommandStream(m_parentNetwork, ZMQNetStream::InternalCommandStream, m_parentNetwork.getAllNodesCommunicator())
{
    // store all nodes bitset
    m_bitset_pool.emplace_back(std::move(allRanks));

    // create broadcast command stream
    m_command_streams.emplace(
        m_bitset_pool.back().get(),
        std::make_unique<CommandStream>(m_parentNetwork, ZMQNetStream::BroadcastCommandStream, m_parentNetwork.getAllNodesCommunicator())
    );

    // create broadcast event stream
    m_event_streams.emplace(
        m_bitset_pool.back().get(),
        std::make_unique<EventStream>(m_parentNetwork, ZMQNetStream::BroadcastEventStream, m_parentNetwork.getAllNodesCommunicator(), m_event_queue)
    );
}

ZMQNetNetwork::StreamTable::CommandStream& ZMQNetNetwork::StreamTable::getCommandStream(const Bitset *target) {
    // check if we already have a stream for this target
    auto it = m_command_streams.find(target);
    if (it != m_command_streams.end()) 
        return *(it->second);

    // none found, create new stream
    auto [commandStreamId, eventStreamId] = generateStreamIdPair();
    LOG("Creating new command/event stream %d/%d...\n", commandStreamId, eventStreamId);

    // register new communicator
    auto comm = m_parentNetwork.registerCommunicator(target->toSet());

    // send stream create command to backend
    capnp::MallocMessageBuilder builder;
    auto root = builder.initRoot<InternalCommand>();
    auto command = root.initNewStreamCmd();
    command.setCommandStreamId(commandStreamId);
    command.setEventStreamId(eventStreamId);

    auto words = capnp::messageToFlatArray(builder);
    auto bytes = words.forceReleaseAsBytes();
    m_internalCommandStream.sendCommand(bytes.asChars().begin(), bytes.size());
    m_internalCommandStream.extractResultWait(); // ignoring result; just waiting for a ping

    // create bitset -> command stream pair
    m_bitset_pool.emplace_back(target->clone());
    m_command_streams.emplace(
        m_bitset_pool.back().get(), 
        std::make_unique<CommandStream>(m_parentNetwork, commandStreamId, comm)
    );

    // create bitset -> event stream pair
    m_event_streams.emplace(
        m_bitset_pool.back().get(),
        std::make_unique<EventStream>(m_parentNetwork, eventStreamId, comm, m_event_queue)
    );

    return *m_command_streams.at(target);
}

bool ZMQNetNetwork::StreamTable::hasAsyncEvent() {
    return m_event_queue.hasItem();
}

void ZMQNetNetwork::StreamTable::injectAsyncEvent(ProtoMessage&& expiring) {
    m_event_queue.enqueue(std::move(expiring));
}

ProtoMessageSet ZMQNetNetwork::StreamTable::releaseAsyncEvents() {
    return m_event_queue.getAll();
}

ZMQNetNetwork::StreamTable::EventStream::EventStream(zmqnet::AsyncFrontend &parentNetwork, zmqnet::Stream::Id streamId, zmqnet::Communicator communicator, EventQueue &parentQueue)
    : m_parentNetwork{parentNetwork}
    , m_parentQueue{parentQueue}
{
    // create async event stream
    const auto event_callback = [this](zmqnet::Stream::Id const&, zmqnet::ByteArrayPtr result) {
        LOG("event_callback %d %s\n", streamId, result.begin());

        // unpack and enqueue
        auto data = zmqnet::make_ByteArray(result.begin(), result.size());
        auto reader = getReaderFromData(data);
        auto pack = reader.getRoot<ZmqnetMessagePackage>();
        for (const auto& msg : pack.getMessages()) {
            auto unpacked = unpackMessageReader(msg);
            m_parentQueue.enqueue(ProtoMessage(std::move(unpacked.bitset), std::move(unpacked.data)));
        }

        LOG("event_callback %d, successfully enqueued\n", streamId);
    };

    m_parentNetwork.registerStreamCallback(streamId, event_callback);
    m_stream = std::make_unique<zmqnet::Stream>(m_parentNetwork.registerStream(communicator, streamId,
        "libzmqnetagg.so", "run", zmqnet::sync::Timeout{m_stream_timeout_ms}));
}

ZMQNetNetwork::StreamTable::EventStream::EventStream(zmqnet::AsyncFrontend &parentNetwork, ZMQNetStream streamId, zmqnet::Communicator communicator, EventQueue &parentQueue) 
    : EventStream(parentNetwork, static_cast<zmqnet::Stream::Id>(streamId), communicator, parentQueue)
{}

ZMQNetNetwork::OutputWatcher::OutputWatcher(OutputPtr &&output)
    : m_output{std::move(output)}
    , m_keep_going{true}
{
    // we might get a nullptr for output, e.g. when attaching instead of launching
    if (!m_output) return;

    auto watchOutput = [this](){
        const int timeout{1000}; // milliseconds

        // single element array
        pollfd fds[1] = {{
            .fd = m_output->p_o_rfd,
            .events = POLLIN,
            .revents = 0
        }};

        try {
            while (m_keep_going) {
                switch (poll(fds, 1, timeout)) {
                    case 1: {
                        // read as much as we can
                        char line[BUFSIZ];
                        while (fgets(line, BUFSIZ, m_output->p_o_stream)) {
                            m_output_queue.enqueue(std::string{line});
                        }
                        break;
                    }

                    case 0: {
                        // keep waiting
                        break;
                    }

                    case -1: {
                        if (errno == EINTR) {
                            // signals are ok, just try again
                        } else {
                            fprintf(stderr, "An error occurred while watching program output.\n");
                            fprintf(stderr, "error text: %s\n", strerror(errno));
                        }
                        break;
                    }
                    
                    default:
                        // should never get here
                        break;
                }
            }
        } catch (...) {
            fprintf(stderr, "WARNING: An exception in the program output collection thread occurred.\nProgram output will no longer be displayed.\n");
        }
    };

    // start watcher thread
    m_watcher_thread = std::thread(watchOutput);
}

ZMQNetNetwork::OutputWatcher::~OutputWatcher() {
    LOG("shutting down output watcher thread\n");
    m_keep_going = false;
    if (m_watcher_thread.joinable()) m_watcher_thread.join();
}

bool ZMQNetNetwork::OutputWatcher::hasOutput() const {
    return m_output_queue.hasItem();
}

std::vector<std::string> ZMQNetNetwork::OutputWatcher::releaseOutputLines() {
    return m_output_queue.getAll();
}
