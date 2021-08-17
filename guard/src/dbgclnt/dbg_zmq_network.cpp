/******************************************************************************\
 * Copyright 2021 Hewlett Packard Enterprise Development LP.
 ******************************************************************************/
#include "dbg_zmq_network.h"

#include "lrunle_bitset.h"
#include "bitset_convert.h"

#include "dbg_client.h"

#include <poll.h>

#if DEBUG
#define LOG(...) fprintf(stderr, __VA_ARGS__)
#else
#define LOG(...) /**/
#endif

ZMQNetNetwork::ZMQNetNetwork(std::unique_ptr<zmqnet::AsyncFrontend> &&frontend, OutputPtr &&output, std::unique_ptr<Bitset> &&allRanks)
    : m_network{std::move(frontend)}
    , m_output_watcher{std::move(output)}
    , m_stream_table(*m_network, std::move(allRanks))
{}

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

std::optional<ProtoMessageSet> ZMQNetNetwork::receiveReply(const Bitset *target, Bitset &received) {
    LOG("%s\n", __PRETTY_FUNCTION__);

    // wait for result
    LOG("waiting for result...\n");
    
    auto &stream = m_stream_table.getCommandStream(target);
    zmqnet::ByteArray reply = stream.extractResult();

    // unpack data
    auto reader = getReaderFromData(reply);
    auto pack = reader.getRoot<ZmqnetMessagePackage>();
    auto result = ProtoMessageSet();

    // construct proto message set, update received
    for (const auto message : pack.getMessages()) {
        auto unpacked = unpackMessageReader(message);
    
        received |= *unpacked.bitset;

        result.emplace_back(std::move(unpacked.bitset), std::move(unpacked.data));
    }

    return result;
}

int ZMQNetNetwork::checkEvents() {
    // LOG("%s\n", __PRETTY_FUNCTION__);
    return m_stream_table.hasAsyncEvent() || m_output_watcher.hasOutput() ? 1 : 0;
}

ProtoMessageSet ZMQNetNetwork::releaseEvents() {
    // LOG("%s\n", __PRETTY_FUNCTION__);
    auto messages = ProtoMessageSet{};

    if (auto events = m_stream_table.releaseAsyncEvents(); events.size() != 0) {
        for (const auto &eventData : events) {
            auto reader = getReaderFromData(eventData);
            auto pack = reader.getRoot<ZmqnetMessagePackage>();

            for (const auto &message : pack.getMessages()) {
                auto unpacked = unpackMessageReader(message);
                messages.emplace_back(std::move(unpacked.bitset), std::move(unpacked.data));
            }
        }
    }
    
    return messages;
}

std::vector<std::string> ZMQNetNetwork::releaseOutputLines() {
    return m_output_watcher.releaseOutputLines();
}

int ZMQNetNetwork::exit() {
    LOG("%s\n", __PRETTY_FUNCTION__);

    if (m_network) m_network->runExit();

    return 0;
}

ZMQNetNetwork::UnpackedMessage ZMQNetNetwork::unpackMessageReader(const ZmqnetMessagePackage::ZmqnetMessage::Reader &reader) const {
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

ZMQNetNetwork::StreamTable::CommandStream::CommandStream(zmqnet::AsyncFrontend &parentNetwork, zmqnet::Stream::Id streamId, zmqnet::Communicator communicator) 
    : m_parentNetwork{parentNetwork}
    , m_resultReady{false}
{
    // create "sync" stream
    const auto command_callback = [&](zmqnet::Stream::Id const &streamId, zmqnet::ByteArrayPtr resultPtr) {
        LOG("command_callback: %d %s\n", streamId, resultPtr.begin());

        auto result =
            zmqnet::make_ByteArray(resultPtr.begin(), resultPtr.size());
        {
            std::scoped_lock<std::mutex> lock(m_resultLock);
            // destroys any data that was in the command result previously
            m_result = std::move(result);
            m_resultReady = true;
        }

        // notify a thread waiting for the result, if present
        m_resultCV.notify_one();
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
}

zmqnet::ByteArray ZMQNetNetwork::StreamTable::CommandStream::extractResult() {
    LOG("CommandStream %d: extracting...\n", m_stream->id);
    std::unique_lock<std::mutex> lock(m_resultLock);
    m_resultCV.wait(lock, [&]() { return m_resultReady; });

    auto reply = std::move(m_result);
    m_resultReady = false;

    return reply;
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
    m_internalCommandStream.extractResult(); // ignoring result; just waiting for a ping

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

std::vector<zmqnet::ByteArray> ZMQNetNetwork::StreamTable::releaseAsyncEvents() {
    return m_event_queue.getAll();
}

ZMQNetNetwork::StreamTable::EventStream::EventStream(zmqnet::AsyncFrontend &parentNetwork, zmqnet::Stream::Id streamId, zmqnet::Communicator communicator, EventQueue &parentQueue)
    : m_parentNetwork{parentNetwork}
    , m_parentQueue{parentQueue}
{
    // create async event stream
    const auto event_callback = [this](zmqnet::Stream::Id const& streamId, zmqnet::ByteArrayPtr result) {
        LOG("event_callback %d %s\n", streamId, result.begin());

        m_parentQueue.enqueue(zmqnet::make_ByteArray(result.begin(), result.size()));
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
    auto watchOutput = [this](){
        const int timeout{1000}; // milliseconds

        // single element array
        pollfd fds[1] = {{
            .fd = m_output->p_o_rfd,
            .events = POLLIN,
            .revents = 0
        }};

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
                    fprintf(stderr, "An error occured while watching program output.\n");
                    fprintf(stderr, "error text: %s\n", strerror(errno));
                    break;
                }
                
                default:
                    // should never get here
                    break;
            }
        }
    };

    // start watcher thread
    m_watcher_thread = std::thread(watchOutput);
}

ZMQNetNetwork::OutputWatcher::~OutputWatcher() {
    LOG("shutting down output watcher thread\n");
    m_keep_going = false;
    m_watcher_thread.join();
}

bool ZMQNetNetwork::OutputWatcher::hasOutput() const {
    return m_output_queue.hasItem();
}

std::vector<std::string> ZMQNetNetwork::OutputWatcher::releaseOutputLines() {
    return m_output_queue.getAll();
}
