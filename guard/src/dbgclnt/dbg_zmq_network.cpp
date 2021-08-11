/******************************************************************************\
 * Copyright 2021 Hewlett Packard Enterprise Development LP.
 ******************************************************************************/
#include "dbg_zmq_network.h"

#include "lrunle_bitset.h"
#include "bitset_convert.h"

#include "dbg_client.h"

#if DEBUG
#define LOG(...) fprintf(stderr, __VA_ARGS__)
#else
#define LOG(...) /**/
#endif

ZMQNetNetwork::ZMQNetNetwork(std::unique_ptr<zmqnet::AsyncFrontend> &&frontend)
    : m_network{std::move(frontend)}
    , m_broadcast_command_stream(*m_network, (ZMQNetStream)ZMQNetStream::CommandStream, m_network->getAllNodesCommunicator())
{
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
            "libzmqnetagg.so", "run", zmqnet::sync::Timeout{m_stream_timeout_ms}));
    }
}

std::unique_ptr<Bitset> ZMQNetNetwork::sendCommand(const Bitset *target, const char *request) {
    LOG("%s\n", __PRETTY_FUNCTION__);

    if (!m_network) {
        PrintOutError("Tried to send a message without a network.\n");
        return nullptr;
    }

    // TODO get/create specific stream for target

    LOG("sending message %s\n", request);

    m_broadcast_command_stream.sendCommand(request);

    return target->clone();
}

std::optional<ProtoMessageSet> ZMQNetNetwork::receiveReply(const Bitset *target, Bitset &received) {
    LOG("%s\n", __PRETTY_FUNCTION__);

    // TODO respect target

    // wait for result
    LOG("waiting for result...\n");
    
    zmqnet::ByteArray reply = m_broadcast_command_stream.extractResult();
    
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

int ZMQNetNetwork::checkEvents() {
    // LOG("%s\n", __PRETTY_FUNCTION__);
    return m_event_queue.hasEvent() ? 1 : 0;
}

ProtoMessageSet ZMQNetNetwork::releaseEvents() {
    LOG("%s\n", __PRETTY_FUNCTION__);
    auto messages = ProtoMessageSet{};

    while (auto optionalEventData = m_event_queue.getNext()) {
        auto reader = getReaderFromData(*optionalEventData);
        auto pack = reader.getRoot<ZmqnetMessagePackage>();

        LOG("got reader\n");

        for (const auto &message : pack.getMessages()) {
            auto unpacked = unpackMessageReader(message);
            messages.emplace_back(std::move(unpacked.bitset), std::move(unpacked.data));
        }
    }
    
    return messages;
}

int ZMQNetNetwork::exit() {
    LOG("%s\n", __PRETTY_FUNCTION__);

    if (m_network) m_network->runExit();

    return 0;
}

capnp::UnalignedFlatArrayMessageReader ZMQNetNetwork::getReaderFromData(const zmqnet::ByteArray &data) const {
    assert(data.size() != 0);
    assert(data.size() % sizeof(capnp::word) == 0);
    auto words_ptr = reinterpret_cast<capnp::word const *>(data.begin());
    auto words_size = data.size() / sizeof(capnp::word);
    auto words_array = kj::arrayPtr(words_ptr, words_size);

    return capnp::UnalignedFlatArrayMessageReader(words_array);
}

ZMQNetNetwork::UnpackedMessage ZMQNetNetwork::unpackMessageReader(const ZmqnetMessagePackage::ZmqnetMessage::Reader &reader) const {
    LOG("unpacking: \n");
    LOG("\tdata: %s\n", reader.getData().cStr());
    LOG("\thash: %lu\n", reader.getHash());
    auto bitsetPtr = reader.getBitset().asChars().begin();
    auto lrunbitset = std::make_unique<LRunLEBitset>(bitsetPtr);
    LOG("\tbitset: %s\n", bitset->toSet().c_str());
    auto densebitset = bitset::createDenseFrom(*lrunbitset);

    return {
        .data = std::unique_ptr<char, decltype(&std::free)>(strndup(reader.getData().cStr(), reader.getData().size()), std::free), 
        .hash = reader.getHash(), 
        .bitset = std::move(densebitset)
    };
}

bool ZMQNetNetwork::EventQueue::hasEvent() {
    std::scoped_lock<std::mutex> lock(m_queue_lock);
    return m_queue.size() > 0;
}

std::optional<zmqnet::ByteArray> ZMQNetNetwork::EventQueue::getNext() {
    std::scoped_lock<std::mutex> lock(m_queue_lock);

    if (m_queue.size() > 0) {
        auto result = std::move(m_queue.front()); 
        m_queue.pop();
        return result; 
    } else {
        return {};
    }
}

void ZMQNetNetwork::EventQueue::enqueue(zmqnet::ByteArray &&event) {
    std::scoped_lock<std::mutex> lock(m_queue_lock);

    m_queue.push(std::move(event));
}

ZMQNetNetwork::PsuedoSyncStream::PsuedoSyncStream(zmqnet::AsyncFrontend &parentNetwork, zmqnet::Stream::Id streamId, zmqnet::Communicator communicator) 
    : m_parentNetwork{parentNetwork}
    , m_resultReady{false}
{
    // create "sync" stream
    const auto command_callback = [&](zmqnet::Stream::Id const &streamId, zmqnet::ByteArrayPtr resultPtr) {
        // TODO check errors
        LOG("command_callback: %d %s\n", streamId, result.begin());

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

ZMQNetNetwork::PsuedoSyncStream::PsuedoSyncStream(zmqnet::AsyncFrontend &parentNetwork, ZMQNetStream streamId, zmqnet::Communicator communicator) 
    : PsuedoSyncStream(parentNetwork, static_cast<zmqnet::Stream::Id>(streamId), communicator)
{}

void ZMQNetNetwork::PsuedoSyncStream::sendCommand(const std::string &command) {
    if (!m_stream) {
        throw std::runtime_error("PsuedoSyncStream tried to send a command with a null stream");
    }

    m_parentNetwork.sendCommand(*m_stream, command);
}

zmqnet::ByteArray ZMQNetNetwork::PsuedoSyncStream::extractResult() {
    std::unique_lock<std::mutex> lock(m_resultLock);
    m_resultCV.wait(lock, [&]() { return m_resultReady; });

    auto reply = std::move(m_result);
    m_resultReady = false;

    return reply;
}