/******************************************************************************\
 * Copyright 2021 Hewlett Packard Enterprise Development LP.
 ******************************************************************************/

#include <mutex>
#include <condition_variable>
#include <queue>

#include "zmqnet.hpp"

template <typename T>
class WaitQueue {
public:
    WaitQueue() : m_message_count{0} {};

    T waitForNext();
    std::optional<T> getNextIfAvailable();
    void push(const T &message);
private:
    std::queue<T> m_message_queue;
    mutable std::mutex m_message_queue_lock;
    std::condition_variable m_message_queue_cv;
    int m_message_count;
};

template <typename T>
void WaitQueue<T>::push(const T &message) {
    fprintf(stderr, "wq: pushing item =\n");
    {
        std::scoped_lock<std::mutex> lock(m_message_queue_lock);
        m_message_queue.push(message);
        m_message_count++;
    }
    // scoped like this so we notify any waiting thread *after* releasing the mutex
    m_message_queue_cv.notify_one();
}

template <typename T>
T WaitQueue<T>::waitForNext() {
    fprintf(stderr, "wq: waiting for next item.\n");

    std::unique_lock<std::mutex> lock(m_message_queue_lock);
    m_message_queue_cv.wait(lock, [&](){ return m_message_count > 0; });
    // have the lock now

    auto message = std::move(m_message_queue.back());
    m_message_queue.pop();
    m_message_count--;

    fprintf(stderr, "wq: got message\n");

    return message;

    // lock released upon destruction
}

template <typename T>
std::optional<T> WaitQueue<T>::getNextIfAvailable() {
    fprintf(stderr, "wq: getting next if available.\n");

    std::unique_lock<std::mutex> lock(m_message_queue_lock);
    if (m_message_queue.size() > 0) {
        auto next = std::move(m_message_queue.back());
        m_message_queue.pop();
        m_message_count--;
        return next;
    } else {
        return {};
    }
}

enum class ZMQNetStream : zmqnet::Stream::Id {
    CommandStreamSend,
    CommandStreamReceive,
    EventStream,
    IOStream
};
