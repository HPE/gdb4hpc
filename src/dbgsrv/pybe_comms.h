// Copyright 2024 Hewlett Packard Enterprise Development LP.
//
// types/functions for communication between dbgsrv and pybe

#pragma once

// this header was written to follow some special constraints. we want both
// dbgsrv and pybe to include a single header for the communication types/fns to
// guarantee that they never go out of sync with each other. but we have a
// problem: for programming convenience, the types defined here make use of low
// level STL containers (vectors, strings) to hold their data. but dbgsrv and
// pybe use different implementations of those containers (libstdc++ vs.
// TINYSTL), so we seemingly can't include the same header on both sides.
//
// to solve this, we templatize everything on the containers and specialize
// those templates as needed, separately, in dbgsrv and pybe. to help with that,
// specialization headers have been created. user code should include those
// files instead of this file, hence:

#ifndef I_HAVE_PERMISSION_TO_INCLUDE_PYBE_COMMS
#error "Do not include pybe_comms.h directly, use a specialization header e.g. pybe_comms_dbgsrv.h"
#endif

#include <cctype>
#include <cerrno>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <netinet/in.h>
#include <signal.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include "pybe_expected.h"

namespace pybe {

// return the signal number that will be emitted when pybe gets the GIL.
inline auto get_gil_signal_number() {
    // TODO: consider if there is a better signal to use, or at least make it configurable by the user.
    //       isn't SIGTRAP used by a few other os facilities?
    return SIGTRAP;
}

inline auto get_gil_signal_name() {
    return "SIGTRAP";
}

// specifies direction for frame modification commands like up/down/frame.
// there is a similar enum (DbgFrameDir) in dbgcmd.h, but to fully isolate
// the pybe API from the dbg API, we make our own here and translate at the API
// boundary as needed.
enum class FrameDirection : int {
    Up,
    Down,
    Absolute,
};

namespace detail {

template <typename String_T, typename StringView_T, template <typename T> typename Vector_T>
struct CommsLowLevelTs {
    // to prevent the template arguments from getting gigantic,
    // we templatize on this struct which holds all the real template args.

    using String = String_T;
    using StringView = StringView_T;

    template <typename T>
    using Vector = Vector_T<T>;
};

using socket_t = int;

template <typename Ts>
class Client_T;

template <typename Ts>
class Server_T;

template <typename Ts>
class Channel_T {
    // a connected socket with an underlying protocol.
    //
    // the client and server sides of the connection each generate a Channel
    // from Client and Server objects respectively. once generated, they can
    // communicate with their send/recv methods.
    //
    // see unit tests for example usage.
public:
    // no ctor: create via Client::connect_with_timeout or Server::accept_with_timeout.
    Channel_T(Channel_T&& expiring);
    Channel_T& operator=(Channel_T&& expiring);
    ~Channel_T();

    // convenience function for use with `send_with_timeout`.
    static uint32_t generate_sequence_number();

    // send a `len`-byte long `message` to the other side. `sequence_number` is
    // a unique token that the corresponding `recv_with_timeout` must be called with.
    // the sequence number can be anything. `generate_sequence_number` can be used for
    // convenience.
    //
    // this function will time out and fail after a small but unspecified length of time spent waiting.
    // the timeout time is on the order of a few hundreds of milliseconds.
    //
    // unlike the usual send syscall, this is atomic. it will never send only a partial message.
    //
    // returns `len` on success or -1 on failure.
    // TODO: an error reporting mechanism
    [[ nodiscard ]]
    ssize_t send_with_timeout(const char* message, size_t len, uint32_t sequence_number);

    // receive a message from the other side. `sequence_number` is the unique token that
    // the corresponding `send_with_timeout` specified.
    //
    // this function will time out and fail after a small but unspecified length of time spent waiting.
    //
    // unlike the usual recv syscall, this is atomic. it will never return only a partial message.
    //
    // returns the received message on success or "" on failure.
    // TODO: an error reporting mechanism
    [[ nodiscard ]]
    Ts::String recv_with_timeout(uint32_t sequence_number);

private:
    friend Expected<Channel_T<Ts>, std::nullptr_t> Client_T<Ts>::connect_with_timeout();
    friend Expected<Channel_T<Ts>, std::nullptr_t> Server_T<Ts>::accept_with_timeout();
    Channel_T(socket_t connected_socket);

    static uint32_t m_next_sequence_number;

    // each message is prepended by a header for error checking/atomicity.
    // this is transparently added and removed and the user of send/recv_with_timeout
    // will never see it.
    struct Header {
        uint32_t sequence_number;
        uint32_t size_of_rest;
    };

    // this would be nice as a Header member function, but the sizeof(Header)
    // can't be used inside the declaration of Header.
    void serialize_header(const Header& header, char (&buf)[sizeof(Header)]);
    Header deserialize_header(const Ts::String& serialized);

    enum class SendOrRecv { Send, Recv };
    int set_timeout(int milliseconds, SendOrRecv send_or_recv);

    // helpers for {send, recv}_with_timeout, don't use outside of those functions
    [[ nodiscard ]]
    ssize_t send_impl(const char* buf, size_t len);
    [[ nodiscard ]]
    Ts::String recv_impl(size_t len);

    void close();

    socket_t m_channel_fd = -1;
};

template <typename Ts>
uint32_t Channel_T<Ts>::m_next_sequence_number = 0;

template <typename Ts>
Channel_T<Ts>::Channel_T(socket_t connected_socket)
    : m_channel_fd{connected_socket}
{
    // these are timeout values for ::send and ::recv. but note that those syscalls are
    // allowed to send/recv only part of their requested data, time out, and then return
    // success with the truncated data length as the return code. the only time they
    // will return -1 on timeout is if they sent/recvd nothing.
    //
    // we loop over successful sends/recvs until the whole buffer is sent. taken to the
    // logical extreme, this means we might loop for X millisecond periods, sending
    // as little as 1 byte each time until the full message is sent.
    //
    // that's not much of a concern in real life. the point is that these timeouts
    // are not a latency guarantee for Channel_T::{send, recv}, they are more of a
    // "failure granularity".
    //
    // the pybe connection socket is a unix domain socket. the actual network is not
    // touched. so it's safe to set these pretty low. the only failures we really need
    // to worry about in theory is kernel buffers filling up and the speed of pybe
    // building up the data.

    set_timeout(500, SendOrRecv::Send);
    set_timeout(500, SendOrRecv::Recv);
}

template <typename Ts>
Channel_T<Ts>::Channel_T(Channel_T<Ts>&& expiring)
    : m_channel_fd{-1}
{
    *this = std::move(expiring);
}

template <typename Ts>
Channel_T<Ts>& Channel_T<Ts>::operator=(Channel_T<Ts>&& expiring) {
    close();
    std::swap(m_channel_fd, expiring.m_channel_fd);
    return *this;
}

template <typename Ts>
Channel_T<Ts>::~Channel_T() {
    close();
}

template <typename Ts>
ssize_t Channel_T<Ts>::send_with_timeout(const char* message, size_t len, uint32_t seq) {
    // send header
    auto header = Header{
        .sequence_number = seq,
        .size_of_rest = (uint32_t)len, // TODO address this narrowing conversion
    };
    char serialized_header_buf[sizeof(Header)];
    serialize_header(header, serialized_header_buf);

    // send header
    if (send_impl(serialized_header_buf, sizeof(serialized_header_buf)) == -1) return -1;

    // send message
    return send_impl(message, len);
}

template <typename Ts>
ssize_t Channel_T<Ts>::send_impl(const char* buf, size_t len) {
    auto bytes_remaining = len;

    // 8 is arbitrary, maybe there's a better number
    for (size_t attempts_left = 8; attempts_left > 0 && bytes_remaining > 0; attempts_left--) {
        // MSG_NOSIGNAL: don't generate SIGPIPE, still return EPIPE
        auto r = ::send(m_channel_fd, &buf[len - bytes_remaining], bytes_remaining, MSG_NOSIGNAL);

        if (r > 0) {
            bytes_remaining -= r;
            attempts_left++; // a successful attempt shouldn't count against us
            // now loop and send the rest.
        } else {
            switch (errno) {
                // send man page says both EWOULDBLOCK and EAGAIN should be checked for, but on
                // some machines they are the same and give a duplicate case error
                #if EWOULDBLOCK != EAGAIN
                case EWOULDBLOCK: [[ fallthrough ]];
                #endif
                case EAGAIN: return -1; // timed out without sending anything, give up
                case EINTR: break; // break out of switch, fall through loop and try again

                // not expecting these
                case EACCES: return -1; // TODO return better errors
                case EALREADY: return -1;
                case EBADF: return -1;
                case ECONNRESET: return -1;
                case EDESTADDRREQ: return -1;
                case EFAULT: return -1;
                case EINVAL: return -1;
                case EISCONN: return -1;
                case EMSGSIZE: return -1;
                case ENOBUFS: return -1;
                case ENOMEM: return -1;
                case ENOTCONN: return -1;
                case ENOTSOCK: return -1;
                case EOPNOTSUPP: return -1;
                case EPIPE: return -1;

                default: return -1; // TODO return better error
            }
        }
    }

    return len - bytes_remaining; // TODO return better error
}

template <typename Ts>
Ts::String Channel_T<Ts>::recv_with_timeout(uint32_t sequence_number) {
    // TODO better errors
    const auto serialized_header = recv_impl(sizeof(Header));
    if (serialized_header == "") return "";
    const auto header = deserialize_header(serialized_header);
    const auto message = recv_impl(header.size_of_rest);

    // check the seq num *after* the message so that we don't leave half a
    // protocal packet in the queue
    if (header.sequence_number != sequence_number) return "";
    return message;
}

template <typename Ts>
Ts::String Channel_T<Ts>::recv_impl(size_t len) {
    auto recv_buf = (typename Ts::template Vector<char>){};
    recv_buf.reserve(len);

    auto result = (typename Ts::String){};

    size_t bytes_remaining = len;

    // 8 is arbitrary, maybe there's a better number
    for (size_t attempts_left = 8; attempts_left > 0 && bytes_remaining > 0; attempts_left--) {
        auto r = ::recv(m_channel_fd, recv_buf.data(), len, 0);

        if (r > 0) {
            // we use this somewhat strange .append form
            // because both std::string and tinystl::string have it
            // and it won't stop early on null bytes
            //
            // XXX: would be nice if we didn't have to copy at all though...
            result.append(recv_buf.data(), recv_buf.data() + r);

            bytes_remaining -= r;
            attempts_left++; // a successful attempt shouldn't count against us
            // now loop and recv the rest.
        } else {
            switch (errno) {
                // recv man page says both EWOULDBLOCK and EAGAIN should be checked for, but on
                // some machines they are the same and give a duplicate case error
                #if EWOULDBLOCK != EAGAIN
                case EWOULDBLOCK: [[ fallthrough ]];
                #endif
                case EAGAIN: return ""; // timed out without receiving anything, give up

                case EINTR: break; // break out of switch, fall through loop and try again

                // not expecting these
                case EBADF: return ""; // TODO better error message
                case ECONNREFUSED: return "";
                case EFAULT: return "";
                case EINVAL: return "";
                case ENOMEM: return "";
                case ENOTCONN: return "";
                case ENOTSOCK: return "";

                default: return ""; // TODO return better error
            }
        }
    }

    return result;
}

template <typename Ts>
uint32_t Channel_T<Ts>::generate_sequence_number() {
    return m_next_sequence_number++;
}

template <typename Ts>
void Channel_T<Ts>::serialize_header(const Header& header, char (&buf)[sizeof(Header)]) {
    // convert to network byte order
    auto network_header = header;
    network_header.sequence_number = ::htonl(network_header.sequence_number);
    network_header.size_of_rest = ::htonl(network_header.size_of_rest);

    ::memcpy(buf, (char*)&network_header, sizeof(Header));
}

template <typename Ts>
Channel_T<Ts>::Header Channel_T<Ts>::deserialize_header(const Ts::String& serialized) {
    if (serialized.size() != sizeof(Header)) {
        // TODO
    }

    auto result = Header{};
    ::memcpy(&result, serialized.data(), sizeof(Header));

    // convert to host byte order
    result.sequence_number = ::ntohl(result.sequence_number);
    result.size_of_rest = ::ntohl(result.size_of_rest);

    return result;
}

template <typename Ts>
void Channel_T<Ts>::close() {
    // might be -1 if we were moved from
    if (m_channel_fd >= 0) {
        // TODO check return value?
        ::close(m_channel_fd);
    }
    m_channel_fd = -1;
}

template <typename Ts>
int Channel_T<Ts>::set_timeout(int milliseconds, SendOrRecv send_or_recv) {
    if (milliseconds <= 0) {
        // we don't support infinite waits
        return -1; // TODO better error
    }

    // assuming tv_sec is a integer type and we get the automatic math.floor
    const auto timeout = timeval {
        .tv_sec = milliseconds / 1000,
        .tv_usec = (milliseconds % 1000) * 1000,
    };

    const auto option = send_or_recv == SendOrRecv::Send ? SO_SNDTIMEO : SO_RCVTIMEO;
    if (setsockopt(m_channel_fd, SOL_SOCKET, option, &timeout, sizeof(timeout))) {
        return -1; // TODO better error
    }

    return 0;
}

template <typename Ts>
class Client_T {
    // peer of a Server. once a connection is set up, a Channel will be generated.
public:
    // create a client that points at `endpoint`.
    // `endpoint` must begin with '@' and be unique among all Clients on the system,
    // including those in other proceseses.
    // the partner server must use the same `endpoint`.
    //
    // after creation, the Client is not yet connected. to connect,
    // call `connect_with_timeout` in sync with the matching Server's `accept_with_timeout`.
    static Expected<Client_T, std::nullptr_t> make(Ts::StringView endpoint);

    Client_T(Client_T&& expiring);
    Client_T(const Client_T& other) = delete;
    ~Client_T();

    // connect to the partner server and generate a Channel on success.
    // a partner server must exist with the same `endpoint` that this client was created with.
    // after successfully generating a Channel, the Client can not be used again.
    //
    // this function will timeout and fail after a short, unspecified amount of time.
    // the timeout time is on the order of a few seconds.
    //
    // TODO: error reporting mechanism
    Expected<Channel_T<Ts>, std::nullptr_t> connect_with_timeout();

private:
    explicit Client_T() = default;
    void close();
    socket_t m_socket;
    Ts::String m_path;
};

template <typename Ts>
Expected<Client_T<Ts>, std::nullptr_t> Client_T<Ts>::make(Ts::StringView endpoint) {
    auto result = Client_T<Ts>{};

    result.m_socket = ::socket(AF_UNIX, SOCK_STREAM, 0);
    if (result.m_socket == -1) {
        // TODO better error
        return nullptr;
    }

    result.m_path = (typename Ts::String){endpoint.data(), endpoint.size()};

    return result;
}

template <typename Ts>
Client_T<Ts>::Client_T(Client_T&& expiring)
    : m_socket{-1}
    , m_path{std::move(expiring.m_path)}
{
    std::swap(m_socket, expiring.m_socket);
}

template <typename Ts>
Client_T<Ts>::~Client_T() {
    close();
}

template <typename Ts>
void Client_T<Ts>::close() {
    // might be -1 after connect_with_timeout or if we were moved from
    if (m_socket >= 0) {
        ::close(m_socket);
    }
    m_socket = -1;
}

template <typename Ts>
Expected<Channel_T<Ts>, std::nullptr_t> Client_T<Ts>::connect_with_timeout() {
    sockaddr_un addr;
    addr.sun_family = AF_UNIX;
    ::memset(addr.sun_path, 0, sizeof(addr.sun_path));
    // TODO bounds check this copy
    ::memcpy(addr.sun_path, m_path.c_str(), m_path.size());

    if (addr.sun_path[0] == '@') {
        // setting the first byte to zero indicates that the path is
        // in an abstract namespace - it doesn't depend on the filesystem
        // and won't be present there. it still has to be OS-unique, we
        // assume dbgsrv did that work an gave us a unique path already.
        addr.sun_path[0] = '\0';
    } else {
        // TODO
        return nullptr;
    }

    // temporarily set the socket to non-blocking so we can implement the timeout
    {
        int flags;
        if ((flags = ::fcntl(m_socket, F_GETFL, nullptr) == -1)) {
            return nullptr;
        }
        flags |= O_NONBLOCK;
        if (::fcntl(m_socket, F_SETFL, flags) == -1) {
            return nullptr;
        }
    }

    if (::connect(m_socket, (sockaddr*)&addr, sizeof(addr))) {
        // TODO better error
        return nullptr;
    }

    // since we set the socket to non blocking, the connect action is "in
    // motion" in the background. we can check that it's done by waiting for the
    // socket to be writeable
    pollfd fds[] = {{
        .fd = m_socket,
        .events = POLLOUT,
        .revents = 0,
    }};

    while (true) {
        // ten seconds
        const auto poll_r =::poll(fds, sizeof(fds)/sizeof(pollfd), 10000);
        if (poll_r > 0) break;

        // handle error
        if (poll_r == 0) return nullptr; // timed out
        switch (errno) {
            case EINTR: break; // signal interrupt, try again

            // not expecting to actually encounter any of these
            case EINVAL: return nullptr;
            case ENOMEM: return nullptr;
            case EFAULT: return nullptr;
            default: return nullptr;
        }
    }

    {   // success, now set it back to blocking
        int flags;
        if ((flags = ::fcntl(m_socket, F_GETFL, nullptr) == -1)) {
            return nullptr;
        }
        flags &= ~O_NONBLOCK;
        if (::fcntl(m_socket, F_SETFL, flags) == -1) {
            return nullptr;
        }
    }

    auto their_fd = m_socket;
    m_socket = -1; // new channel owns the fd

    return Channel_T<Ts>(their_fd);
}

template <typename Ts>
class Server_T {
    // peer of a Client. once a connection is set up, a Channel will be generated.
public:
    // create a server that listens for a connection on `endpoint`.
    // `endpoint` must begin with '@' and be unique among all Servers on the system,
    // including those in other processes.
    // the parter client must use the same 'endpoint'.
    //
    // after creation, the Server is not yet connected. to connect,
    // call `accept_with_timeout` in sync with the matching Client's `connect_with_timeout`.
    pybe::Expected<Server_T<Ts>, std::nullptr_t> static make(Ts::StringView endpoint);
    Server_T(Server_T&& expiring);
    ~Server_T();

    // accept a connection from the partner client and generate a Channel on success.
    //
    // this function will timeout and fail after a short, unspecified amount of time.
    // the timeout time is on the order of a few seconds.
    //
    // TODO: error reporting mechanism
    pybe::Expected<Channel_T<Ts>, std::nullptr_t> accept_with_timeout();

private:
    explicit Server_T() = default;

    void close();

    pybe::detail::socket_t m_socket = -1;
    Ts::String m_path;
};

template <typename Ts>
Server_T<Ts>::Server_T(Server_T<Ts>&& expiring)
    : m_socket{-1}
    , m_path{std::move(expiring.m_path)}
{
    std::swap(m_socket, expiring.m_socket);
}

template <typename Ts>
pybe::Expected<Server_T<Ts>, std::nullptr_t> Server_T<Ts>::make(Ts::StringView path) {
    auto socket = ::socket(AF_UNIX, SOCK_STREAM, 0);
    if (socket == -1) {
        // TODO better error
        return nullptr;
    }

    sockaddr_un addr;
    addr.sun_family = AF_UNIX;
    ::memset(addr.sun_path, 0, sizeof(addr.sun_path));
    ::memcpy(addr.sun_path, path.data(), path.size());

    if (addr.sun_path[0] == '@') {
        // setting the first byte to zero indicates that the path is
        // in an abstract namespace - it doesn't depend on the filesystem
        // and won't be present there. it still has to be OS-unique, we
        // assume dbgsrv did that work an gave us a unique path already.
        addr.sun_path[0] = '\0';
    } else {
        // TODO
        return nullptr;
    }

    if (::bind(socket, (sockaddr*)&addr, sizeof(addr))) {
        // TODO better error
        return nullptr;
    }

    // the 2nd parameter specifies the length of the connection
    // queue. attempts from another process to `connect` will be
    // successful as long as there is room in the queue. once the
    // queue is full, the other process's `connect` blocks.
    //
    // each server is only expected to have one client,
    // so 0 (one connection, no queue) seems fine.
    // but more thought should be given there.
    //
    // TODO: determine the correct queue size
    if (::listen(socket, 0)) {
        // TODO better error
        return nullptr;
    }

    auto result = Server_T<Ts>{};

    result.m_socket = socket;
    result.m_path = path;

    return result;
}

template <typename Ts>
Server_T<Ts>::~Server_T() {
    close();
}

template <typename Ts>
void Server_T<Ts>::close() {
    // might be -1 if we were moved from
    if (m_socket >= 0) {
        ::close(m_socket);
    }
    m_socket = -1;
}

template <typename Ts>
pybe::Expected<Channel_T<Ts>, std::nullptr_t> Server_T<Ts>::accept_with_timeout() {
    pollfd fds[] = {{
        .fd = m_socket,
        .events = POLLIN,
        .revents = 0,
    }};

    while (true) {
        // ten seconds
        const auto poll_r =::poll(fds, sizeof(fds)/sizeof(pollfd), 10000);
        if (poll_r > 0) break;

        // handle error
        if (poll_r == 0) return nullptr; // timed out
        switch (errno) {
            case EINTR: break; // signal interrupt, try again

            // not expecting to actually encounter any of these
            case EINVAL: return nullptr;
            case ENOMEM: return nullptr;
            case EFAULT: return nullptr;
            default: return nullptr;
        }
    }

    auto conn_fd = ::accept(m_socket, nullptr, nullptr);
    if (conn_fd == -1) {
        // TODO better error message
        return nullptr;
    }
    return Channel_T<Ts>(conn_fd);
}

template <typename Ts> struct Location_T;
template <typename Ts> struct Frame_T;
template <typename Ts> struct Backtrace_T;
template <typename Ts> struct Breakpoint_T;
template <typename Ts> struct Exception_T;

template <typename Ts>
class WireMsgBuilder_T {
    // helper class to serialize groups of pybe plain-ol-data structs
public:
    enum class Type : int {
        Int = 1,
        String,

        Location,
        Frame,
        Backtrace,
        Breakpoint,
        Exception,
    };

    Ts::String msg;

    void push(int num);
    void push(const Ts::String& string);

    void push(const Location_T<Ts>& frame);
    void push(const Frame_T<Ts>& frame);
    void push(const Backtrace_T<Ts>& frame);
    void push(const Breakpoint_T<Ts>& breakpoint);
    void push(const Exception_T<Ts>& exception);

private:
    // used internally by the push methods
    void push_type(Type type);

};

template <typename Ts>
Ts::String to_string(int num);

template <typename Ts>
void WireMsgBuilder_T<Ts>::push_type(WireMsgBuilder_T<Ts>::Type type) {
    // we mark types with '^' to make debugging serialized messages easier,
    // it serves no actual data storage/parsing purpose
    this->msg.append("^");
    this->msg.append(detail::to_string<Ts>(static_cast<int>(type)).c_str());
}

template <typename Ts>
void WireMsgBuilder_T<Ts>::push(int num) {
    push_type(Type::Int);
    this->msg.append(" ");
    this->msg.append(detail::to_string<Ts>(num).c_str());
    this->msg.append(" ");
}

template <typename Ts>
void WireMsgBuilder_T<Ts>::push(const Ts::String& string) {
    push_type(Type::String);
    this->msg.append(" ");
    // note that this size push includes the type identifier for the int
    push(string.size());
    this->msg.append(string.c_str());
    this->msg.append(" ");
}

template <typename Ts>
void WireMsgBuilder_T<Ts>::push(const Location_T<Ts>& loc) {
    push_type(Type::Location);
    this->msg.append(" ");

    push(loc.function_name);
    push(loc.file_name);
    push(loc.line_number);
}

template <typename Ts>
void WireMsgBuilder_T<Ts>::push(const Frame_T<Ts>& frame) {
    push_type(Type::Frame);
    this->msg.append(" ");

    push(frame.level);
    push(frame.location);
}

template <typename Ts>
void WireMsgBuilder_T<Ts>::push(const Backtrace_T<Ts>& bt) {
    push_type(Type::Backtrace);
    this->msg.append(" ");

    push(bt.frames.size());

    for (const auto& frame : bt.frames) {
        push(frame);
    }
}

template <typename Ts>
void WireMsgBuilder_T<Ts>::push(const Breakpoint_T<Ts>& bp) {
    push_type(Type::Breakpoint);
    this->msg.append(" ");

    push(bp.feid);
    push(bp.location);
}

template <typename Ts>
void WireMsgBuilder_T<Ts>::push(const Exception_T<Ts>& exp) {
    push_type(Type::Exception);
    this->msg.append(" ");

    push(exp.type_name);
    push(exp.str);
    push(exp.backtrace);
    push(static_cast<int>(exp.fatal));
}

// deserialization helpers

template <typename Ts>
void consume_whitespace(const char*& cursor);

template <typename Ts>
bool consume_type(const char*& cursor, typename WireMsgBuilder_T<Ts>::Type type);

template <typename Ts>
Expected<int, std::nullptr_t> parse_int(const char*& cursor);

template <typename Ts>
Expected<typename Ts::String, std::nullptr_t> parse_string(const char*& cursor);

template <typename Ts>
struct Location_T {
    Ts::String function_name;
    Ts::String file_name;
    int line_number;

    Ts::String serialize() const;
    static Expected<Location_T, std::nullptr_t> deserialize(const char*& data, size_t len);

    bool operator==(const Location_T& other) const = default;
    bool operator!=(const Location_T& other) const = default;
};

template <typename Ts>
Ts::String Location_T<Ts>::serialize() const
{
    auto result = detail::WireMsgBuilder_T<Ts>{};
    result.push(*this);
    return std::move(result.msg);
}

template <typename Ts>
Expected<Location_T<Ts>, std::nullptr_t> Location_T<Ts>::deserialize(const char*& cursor, size_t) {
    auto result = Location_T{};

    // TODO bounds check with size_t param

    if (!detail::consume_type<Ts>(cursor, detail::WireMsgBuilder_T<Ts>::Type::Location)) return nullptr;

    if (auto maybe_str = detail::parse_string<Ts>(cursor)) {
        result.function_name = *maybe_str;
    } else {
        return nullptr;
    }

    if (auto maybe_str = detail::parse_string<Ts>(cursor)) {
        result.file_name = *maybe_str;
    } else {
        return nullptr;
    }

    if (auto maybe_int = detail::parse_int<Ts>(cursor)) {
        result.line_number = *maybe_int;
    } else {
        return nullptr;
    }

    return result;
}

template <typename Ts>
struct Frame_T {
    // a single function frame

    Ts::String serialize() const;
    static Expected<Frame_T, std::nullptr_t> deserialize(const char*& data, size_t len);

    bool operator==(const Frame_T& other) const = default;
    bool operator!=(const Frame_T& other) const = default;

    int level;
    Location_T<Ts> location;
};

template <typename Ts>
Ts::String Frame_T<Ts>::serialize() const {
    auto result = detail::WireMsgBuilder_T<Ts>{};
    result.push(*this);
    return std::move(result.msg);
}

template <typename Ts>
Expected<Frame_T<Ts>, std::nullptr_t> Frame_T<Ts>::deserialize(const char*& cursor, size_t len) {
    auto result = Frame_T{};

    // TODO bounds check with size_t param

    if (!detail::consume_type<Ts>(cursor, detail::WireMsgBuilder_T<Ts>::Type::Frame)) return nullptr;

    if (auto maybe_level = detail::parse_int<Ts>(cursor)) {
        result.level = *maybe_level;
    } else {
        return nullptr;
    }

    if (auto maybe_loc = Location_T<Ts>::deserialize(cursor, len)) {
        result.location = *maybe_loc;
    } else {
        return nullptr;
    }

    return result;
}

template <typename Ts>
struct Backtrace_T {
    // a backtrace composed of n frames. the first frame in the list is the deepest (farthest from main)
    // TODO: verify this claim. i am only 90% sure. :-)

    Ts::String serialize() const;
    static Expected<Backtrace_T, std::nullptr_t> deserialize(const char*& data, size_t len);

    // TINYSTL::vector does not have an operator== defined, so we define Backtrace_T== ourselves
    bool operator==(const Backtrace_T& other) const;
    bool operator!=(const Backtrace_T& other) const = default;

    Ts::template Vector<Frame_T<Ts>> frames;
};

template <typename Ts>
Ts::String Backtrace_T<Ts>::serialize() const {
    auto result = detail::WireMsgBuilder_T<Ts>{};
    result.push(*this);
    return std::move(result.msg);
}

template <typename Ts>
Expected<Backtrace_T<Ts>, std::nullptr_t> Backtrace_T<Ts>::deserialize(const char*& cursor, size_t len) {
    auto result = Backtrace_T{};

    // TODO bounds check with size_t param

    if (!detail::consume_type<Ts>(cursor, detail::WireMsgBuilder_T<Ts>::Type::Backtrace)) return nullptr;

    if (auto maybe_int = detail::parse_int<Ts>(cursor)) {
        auto& num_frames = *maybe_int;

        for (int i = 0; i < num_frames; i++) {
            if (auto maybe_frame = Frame_T<Ts>::deserialize(cursor, len)) {
                result.frames.emplace_back(std::move(*maybe_frame));
            } else {
                return nullptr;
            }
        }

        return result;
    } else {
        return nullptr;
    }
}

template <typename Ts>
bool Backtrace_T<Ts>::operator==(const Backtrace_T<Ts>& other) const {
    if (this->frames.size() != other.frames.size()) return false;
    for (size_t i = 0; i < this->frames.size(); i++) {
        if (this->frames[i] != other.frames[i]) return false;
    }
    return true;
}

template <typename Ts>
struct Breakpoint_T {
    // a pybe breakpoint

    Ts::String serialize() const;
    static Expected<Breakpoint_T, std::nullptr_t> deserialize(const char*& data, size_t len);

    bool operator==(const Breakpoint_T& other) const = default;

    int feid;
    Location_T<Ts> location;
};

template <typename Ts>
Ts::String Breakpoint_T<Ts>::serialize() const {
    auto result = detail::WireMsgBuilder_T<Ts>{};
    result.push(*this);
    return std::move(result.msg);
}

template <typename Ts>
Expected<Breakpoint_T<Ts>, std::nullptr_t> Breakpoint_T<Ts>::deserialize(const char*& cursor, size_t len) {
    auto result = Breakpoint_T{};

    // TODO bounds check with size_t param

    if (!detail::consume_type<Ts>(cursor, detail::WireMsgBuilder_T<Ts>::Type::Breakpoint)) return nullptr;

    if (auto maybe_int = parse_int<Ts>(cursor)) {
        result.feid = *maybe_int;
    } else {
        return nullptr;
    }

    if (auto maybe_loc = Location_T<Ts>::deserialize(cursor, len)) {
        result.location = *maybe_loc;
    } else {
        return nullptr;
    }

    return result;
}

template <typename Ts>
struct Exception_T {
    // a python exception with a backtrace

    Ts::String serialize() const;
    static Expected<Exception_T, std::nullptr_t> deserialize(const char*& data, size_t len);

    bool operator==(const Exception_T& other) const = default;

    // type(exc).__name__
    Ts::String type_name;
    // str(exc)
    Ts::String str;
    Backtrace_T<Ts> backtrace;
    // if true, python is about to exit
    bool fatal;
};

template <typename Ts>
Ts::String Exception_T<Ts>::serialize() const {
    auto result = detail::WireMsgBuilder_T<Ts>{};
    result.push(*this);
    return std::move(result.msg);
}

template <typename Ts>
Expected<Exception_T<Ts>, std::nullptr_t> Exception_T<Ts>::deserialize(const char*& cursor, size_t len) {
    auto result = Exception_T{};

    // TODO bounds check with size_t param

    if (!detail::consume_type<Ts>(cursor, detail::WireMsgBuilder_T<Ts>::Type::Exception)) return nullptr;

    if (auto maybe_str = detail::parse_string<Ts>(cursor)) {
        result.type_name = std::move(*maybe_str);
    } else {
        return nullptr;
    }

    if (auto maybe_str = detail::parse_string<Ts>(cursor)) {
        result.str = std::move(*maybe_str);
    } else {
        return nullptr;
    }

    if (auto maybe_bt = Backtrace_T<Ts>::deserialize(cursor, len)) {
        result.backtrace = std::move(*maybe_bt);
    } else {
        return nullptr;
    }

    if (auto maybe_fatal = parse_int<Ts>(cursor)) {
        result.fatal = *maybe_fatal;
    }

    return result;
}

// [de]serializing helpers

template <typename Ts>
Ts::String to_string(int num) {
    auto result = (typename Ts::String){};

    auto len = ::snprintf(nullptr, 0, "%d", num) + 1;
    auto buf = new char[len];
    ::snprintf(buf, len, "%d", num);
    // XXX: is there a way to do this without the copy?
    result.append(buf);
    delete[] buf;

    return result;
}

template <typename Ts>
void consume_whitespace(const char*& cursor) {
    // doesn't actually depend on Ts, but included anyway for consistency
    while (::isspace(*cursor)) cursor++;
}

template <typename Ts>
bool consume_type(const char*& cursor, typename WireMsgBuilder_T<Ts>::Type type) {
    consume_whitespace<Ts>(cursor);

    // move past '^'
    if (*cursor == '^') {
        cursor++;
    } else {
        return false;
    }

    const auto type_str = detail::to_string<Ts>(static_cast<int>(type));
    if (::strncmp(cursor, type_str.c_str(), type_str.size()) == 0) {
        cursor += type_str.size();
        return true;
    } else {
        return false;
    }
}

template <typename Ts>
Expected<int, std::nullptr_t> parse_int(const char*& cursor) {
    if (!detail::consume_type<Ts>(cursor, WireMsgBuilder_T<Ts>::Type::Int)) return nullptr;

    // strtol consumes whitespace for us
    char* end = nullptr;
    auto value = static_cast<int>(::strtol(cursor, &end, 10));
    cursor = end;
    return value;
}

template <typename Ts>
Expected<typename Ts::String, std::nullptr_t> parse_string(const char*& cursor) {
    if (!detail::consume_type<Ts>(cursor, WireMsgBuilder_T<Ts>::Type::String)) return nullptr;

    consume_whitespace<Ts>(cursor);

    if (auto maybe_len = parse_int<Ts>(cursor)) {
        auto& len = *maybe_len;
        if (len < 0) return nullptr;

        cursor++; // we know there will only be one space between the
                  // len and the string - the string might start with whitespace
        auto result = typename Ts::String(cursor, len);
        cursor += len;
        return result;
    } else {
        return nullptr;
    }
}

} // namespace detail
} // namespace pybe
