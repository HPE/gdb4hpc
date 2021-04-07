/******************************************************************************\
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 * Copyright 2010-2021 Hewlett Packard Enterprise Development LP.
 ******************************************************************************/
#ifndef _DBGSRV_ZMQ_H
#define _DBGSRV_ZMQ_H

#include "dbgsrv_proto.h"

class ZMQNetServerProtocol : public DebugServerProtocol {
public:
    const char * name() override;
    int initProto(int procnum, int argc, char **argv,
        const char *key, const char *host, int port) override;
    void endProto() override;
    fd_set* getFdSet() override;
    void runProto(fd_set *fds) override;
    int invokeCallback(char *chost, int cport, char *host, int prog,
        int pnum, int port, dbgevent_t *e) override;
    int eventCallback(dbgevent_t *ev) override;
    int setupIo(int *fd, int *port) override;
    int transferIo(int fd, char *buffer, int size) override;
    dbgevent_t* invokeNetwork(int numbe) override;
    int sendReply(char *response) override;
};

#endif // _DBGSRV_ZMQ_H