/******************************************************************************\
 * Copyright (c) 1996-2012 by Monash Uni.
 * Copyright 2010-2013,2021 Hewlett Packard Enterprise Development LP.
 ******************************************************************************/

#include "dbgsrv_zmq.h"
#include "dbgsrv_defs.h"

// Proto definition
ZMQNetServerProtocol DbgSrvProtoZMQNet;

const char *ZMQNetServerProtocol::name() {
    return "zmqnet";
}

// Callbacks
int ZMQNetServerProtocol::invokeCallback(char *chost, int cport, char *host, int prog,
                             int pnum, int port, dbgevent_t *e) {
    // we have nothing to do here
    DVPRINT("ZMQNetServerProtocol::invokeCallback\n");
    return 1;
}

int ZMQNetServerProtocol::eventCallback(dbgevent_t *ev) {
    DVPRINT("ZMQNetServerProtocol::eventCallback\n");
    return 0;
}

int ZMQNetServerProtocol::initProto(int procnum, int argc, char **argv,
                              const char *key, const char *host, int port) {
    DVPRINT("ZMQNetServerProtocol::initProto\n");
    return 0;
}

void ZMQNetServerProtocol::endProto() {
    DVPRINT("ZMQNetServerProtocol::endProto\n");
}

fd_set *ZMQNetServerProtocol::getFdSet() {
    DVPRINT("ZMQNetServerProtocol::getFdSet\n");
    return nullptr;
}

int ZMQNetServerProtocol::setupIo(int *fd, int *port) { 
    DVPRINT("ZMQNetServerProtocol::setupIo\n");
    return 0; 
}

int ZMQNetServerProtocol::transferIo(int fd, char *buffer, int size) {
    DVPRINT("ZMQNetServerProtocol::transferIo\n");
    return 0;
}

void ZMQNetServerProtocol::runProto(fd_set *fds) {
    DVPRINT("ZMQNetServerProtocol::runProto\n");
}

dbgevent_t *ZMQNetServerProtocol::invokeNetwork(int numbe) {
    DVPRINT("ZMQNetServerProtocol::invokeNetwork\n");
    return nullptr;
}

int ZMQNetServerProtocol::sendReply(char *response) {
    DVPRINT("ZMQNetServerProtocol::sendReply\n");
    return 0;
}
