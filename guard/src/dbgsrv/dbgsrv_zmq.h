/******************************************************************************\
 * Copyright 2021 Hewlett Packard Enterprise Development LP.
 ******************************************************************************/
#ifndef _DBGSRV_ZMQ_H
#define _DBGSRV_ZMQ_H

#include "dbgsrv_proto.h"

class ZMQNetServerProtocol : public DebugServerProtocol {
public:
    ZMQNetServerProtocol() : DebugServerProtocol() { registerProtocol(); };

    const char * name() override;
    int initProto(int procnum, int argc, char **argv,
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
};

// used as a handle in dbgsrv_proto.cpp to make the linker include this class
void ZMQServerProtocol_link() {};

#endif // _DBGSRV_ZMQ_H