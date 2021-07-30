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
    // by the time initProto is done, procnum and pid must hold valid (non-negative)
    // values. some protocols set them inside the function, and some have them
    // set before this function is called.
    virtual int initProto(int &procnum, int &pid, int argc, const char * const *argv,
        const char *key, const char *host, int port) = 0;
    virtual void endProto() = 0;

    // Two ways to notify the main thread that runProto should be called are
    // supported. In order of precedence:
    // 1. getFdSet(). The main thread calls getFdSet to get a set of file
    //    descriptors and then calls select on them. If any supplied fd
    //    becomes ready for reading, that is a signal to call runProto. If a
    //    protocol doesn't support event fds, this function should return
    //    nullptr.
    // 2. checkEvents(). If, after calling select on the getFdSet set (if
    //    provided) no fds are available for reading, the main thread will call
    //    checkEvents. If it returns true, the main thread will call runProto.
    //    If a protocol doesn't support using this function, the function should
    //    return false.
    //
    //    There are circumstances where the main thread will call runProto even
    //    without a notification from the protocol. This happens whenever a
    //    top layer event, such as output from gdb, happens. Thus, a call to
    //    runProto does not necessarily mean a protocol event is ready, only
    //    that the main thread is happy to let the protocol have its turn if it
    //    wants to. Therefore, runProto must do its own checking to verify that
    //    something is indeed ready to go.

    virtual fd_set* getFdSet() { return nullptr; };
    virtual bool checkEvents() const { return false; };
    
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

int DbgSetSrvProto(const char *);
int DbgInitProto(int&, int&, int, const char * const *, const char *, const char *, int);
fd_set *DbgGetFdSet(int dbgFd, int ioClientFd, int ioBackendFd);
int DbgCheckEvents(fd_set *);
void DbgRunProto(fd_set *);
void DbgEndProto();
int DbgInvokeCB(char *, int, char *, int, int, int, dbgevent_t *);
int DbgEventCB(dbgevent_t *);
int DbgSetupIO(int *, int *);
int DbgTransferIO(int, char *, int);
dbgevent_t *DbgInvokeNetwork(int);
int DbgSendReply(const char *);
