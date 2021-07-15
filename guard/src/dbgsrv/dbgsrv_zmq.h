/******************************************************************************\
 * Copyright 2021 Hewlett Packard Enterprise Development LP.
 ******************************************************************************/
#ifndef _DBGSRV_ZMQ_H
#define _DBGSRV_ZMQ_H

#include <future>
#include <functional>
#include <type_traits>

#include "dbgsrv_proto.h"
#include "dbg_zmq.h"
#include "dbgsrv_cmds.h"
#include "lrunle_bitset.h"

class ZMQNetServerProtocol : public DebugServerProtocol {
public:
    ZMQNetServerProtocol();

    const char * name() override;
    int initProto(int &procnum, int &pid, int argc, const char * const * argv,
        const char *key, const char *host, int port) override;
    void endProto() override;
    bool checkEvents() const override;
    void runProto(fd_set *fds) override;
    int invokeCallback(const char *chost, int cport, const char *host, int prog,
        int pnum, int port, dbgevent_t *e) override;
    int eventCallback(dbgevent_t *ev) override;
    int setupIo(int *fd, int *port) override;
    int transferIo(int fd, char *buffer, int size) override;
    dbgevent_t* invokeNetwork(int numbe) override;
    int sendReply(const char *response) override;
private:
    using ResultPtr = std::unique_ptr<char, decltype(&std::free)>;
    using CommandDispatchFunc = std::packaged_task<ResultPtr (const dbgcomm_info*)>;

    // take a result and use capnp to serialize it
    zmqnet::ByteArray packResult(ResultPtr result);

    std::unique_ptr<zmqnet::AsyncBackend> m_network;

    std::unique_ptr<CommandDispatchFunc> m_pending_command;
    mutable std::mutex m_command_lock;

    // bitset related
    size_t m_rank;
    std::unique_ptr<LRunLEBitset> m_rank_bitset;
    Bitset::Serialized m_rank_serialized;
    size_t m_rank_serialized_size;
};

// used as a handle in dbgsrv_proto.cpp to make the linker include this class
void ZMQServerProtocol_link() {};

#endif // _DBGSRV_ZMQ_H