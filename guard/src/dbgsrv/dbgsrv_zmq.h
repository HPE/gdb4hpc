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

class ZMQNetServerProtocol : public DebugServerProtocol {
public:
    ZMQNetServerProtocol();

    const char * name() override;
    int initProto(int procnum, int argc, const char **argv,
        const char *key, const char *host, int port) override;
    void endProto() override;
    fd_set* getFdSet() override;
    void runProto(fd_set *fds) override;
    int invokeCallback(const char *chost, int cport, const char *host, int prog,
        int pnum, int port, dbgevent_t *e) override;
    int eventCallback(dbgevent_t *ev) override;
    int setupIo(int *fd, int *port) override;
    int transferIo(int fd, char *buffer, int size) override;
    dbgevent_t* invokeNetwork(int numbe) override;
    int sendReply(const char *response) override;
private:
    // event flag functions return true on success, false on failure. a failure
    // means that the main/zmqnet threads will probably desync and therefore 
    // the debugger should shut down.
    bool incEventFlag();
    bool decEventFlag();

    std::unique_ptr<zmqnet::AsyncBackend> m_network;

    using CommandDispatchFunc = std::packaged_task<std::unique_ptr<char, decltype(&std::free)> (const dbgcomm_info*)>;
    std::unique_ptr<CommandDispatchFunc> m_pending_command;

    std::mutex m_command_lock;

    fd_set m_fds;
    int m_event_pipe[2];
};

// used as a handle in dbgsrv_proto.cpp to make the linker include this class
void ZMQServerProtocol_link() {};

#endif // _DBGSRV_ZMQ_H