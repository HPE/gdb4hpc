/*********************************************************************************\
 * Debug server protocol definitions.
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * Copyright 2010-2013,2021 Hewlett Packard Enterprise Development LP.
 *********************************************************************************/

#pragma once

#include <unordered_map>

#include "dbgevent.h"
#include "defs.h"

class DebugServerProtocol {
public:
    virtual ~DebugServerProtocol() = default;

    virtual const char * name() = 0;
    virtual int initProto(int procnum, int argc, const char **argv,
        const char *key, const char *host, int port) = 0;
    virtual void endProto() = 0;
    virtual fd_set* getFdSet() = 0;
    virtual void runProto(fd_set *fds) = 0;
    virtual int invokeCallback(const char *chost, int cport, const char *host, 
        int prog, int pnum, int port, dbgevent_t *e) = 0;
    virtual int eventCallback(dbgevent_t *ev) = 0;
    virtual int setupIo(int *fd, int *port) = 0;
    virtual int transferIo(int fd, char *buffer, int size) = 0;
    virtual dbgevent_t* invokeNetwork(int numbe) = 0;
    virtual int sendReply(const char *response) = 0;

    static const std::unordered_map<std::string, DebugServerProtocol *>& getRegistry();

protected:
    static std::unordered_map<std::string, DebugServerProtocol *> registry;
    void registerProtocol();
};

extern int DbgSetSrvProto(const char *);
extern int DbgInitProto(int, int, const char **, const char *, const char *, int);
extern fd_set *DbgGetFDSet(void);
extern void DbgRunProto(fd_set *);
extern void DbgEndProto(void);
extern int DbgInvokeCB(char *, int, char *, int, int, int, dbgevent_t *);
extern int DbgEventCB(dbgevent_t *);
extern int DbgSetupIO(int *, int *);
extern int DbgTransferIO(int, char *, int);
extern dbgevent_t *DbgInvokeNetwork(int);
extern int DbgSendReply(char *);
