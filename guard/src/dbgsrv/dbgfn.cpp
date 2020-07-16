/*********************************************************************************\
 * Server dispatch routines
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * Copyright 2010-2013,2020 Cray Inc.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Cray Inc.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *
 * $HeadURL$
 * $Date$
 * $Rev$
 * $Author$
 *
 *********************************************************************************/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <stdlib.h>
#include <string.h>

#include "dbgfn.h"
#include "dbgsrv_defs.h"
#include "defs.h"

/*
** Setup debug backend defs
*/
struct backend_tab {
    const char *backend_name;
    dbgbackend_sw *backend_def;
};
typedef struct backend_tab backend_tab;

// external debug backend defs
#if defined(GDB_BACKEND)
extern dbgbackend_sw DbgBackendGDB;
#elif defined(GDBMI_BACKEND)
extern dbgbackend_sw* DbgGdbMIGetSingleton();
#elif defined(IDB_BACKEND)
extern dbgbackend_sw DbgBackendIDB;
#endif

// table of backend defs
static backend_tab DbgBackendTab[] = {
#if defined(GDB_BACKEND)
    {"gdb", &DbgBackendGDB},
#elif defined(GDBMI_BACKEND)
    {"gdbmi", DbgGdbMIGetSingleton()},
#elif defined(IDB_BACKEND)
    {"idb", &DbgBackendIDB},
#endif
    {NULL, (dbgbackend_sw *)NULL}};

/*
** Set default debug backend
** This defines the current debug backend in use
*/
#if defined(GDB_BACKEND)
static dbgbackend_sw *DbgBackendSW = &DbgBackendGDB; /* Default to GDB */
#elif defined(GDBMI_BACKEND)
static dbgbackend_sw *DbgBackendSW = DbgGdbMIGetSingleton(); /* Default to GDBMI */
#elif defined(IDB_BACKEND)
static dbgbackend_sw *DbgBackendSW = &DbgBackendIDB; /* Default to IDB */
#endif

/*
** The following functions are used to startup/init the debug backend and not
** send acutal commands to the backend.
*/

// NOTE: Only one protocol/debug backend can be active. The debug server is
//       designed to talk to only one client, whereas the client is designed to
//       talk to many different debug servers. Hence why the client handles
//       multiple protocols at once, and the server doesn't.

int DbgSetBackend(const char *backend) {
    backend_tab *p;

    for (p = DbgBackendTab; p->backend_name != NULL; p++) {
        if (strcmp(p->backend_name, backend) == 0) {
            // Set the current backend
            DbgBackendSW = p->backend_def;
            return 0;
        }
    }

    return 1;
}

dbgevent_t *DbgInit(int *dfd, int *iofd, char **env) {
    ResetWDT();
    return DbgBackendSW->init(dfd, iofd, env);
}

dbgevent_t *DbgAttach(int pid, const char *prog, const char *args, char **env,
                      int arch) {
    ResetWDT();
    return DbgBackendSW->attach(pid, prog, args, env, arch);
}

dbgevent_t *DbgInvoke(const char *host, int cb, const char *proto,
                      const char *prog, const char *args, char **env,
                      int arch) {
    ResetWDT();
    return DbgBackendSW->invoke(host, cb, proto, prog, args, env, arch);
}

void DbgFinalize(void) {
    ResetWDT();
    return DbgBackendSW->finalize();
}

int DbgProgIOHook(int fd) {
    ResetWDT();
    return DbgBackendSW->progioHook(fd);
}

int DbgRead(int fd) {
    ResetWDT();
    return DbgBackendSW->read(fd);
}

int DbgBufferReady(void) {
    ResetWDT();
    return DbgBackendSW->bufferReady();
}

/*
** The following macros are used below - note that the debug backend needs to be
** started and init'ed before calling these.
*/

#define DBG_CHECK()                                               \
    if (!DbgBackendSW->isOk()) {                                 \
        dbgevent_t* e;                                            \
        EVENT_ERROR(e, DBGERR_DEBUGGER, "debugger not available"); \
        return e;                                                  \
    }

#define DBG_READY()                          \
    DBG_CHECK();                             \
                                              \
    if (!DbgBackendSW->isReadyForCmd()) {   \
        dbgevent_t* e;                      \
        EVENT_ERROR(e, DBGERR_ISRUNNING, ""); \
        return e;                             \
    }

#define DBG_STOPPED()                        \
    DBG_READY();                             \
                                              \
    if (!DbgBackendSW->isStopped()) {       \
        dbgevent_t* e;                      \
        EVENT_ERROR(e, DBGERR_ISRUNNING, ""); \
        return e;                             \
    }

/*
** These functions will send actual commands to the debug backend.
*/

dbgevent_t *DbgStartup(void) {
    ResetWDT();
    DBG_READY();
    return DbgBackendSW->startup();
}

dbgevent_t *DbgSetLineBreak(int allowPending, const char *file, int line,
                            const char* threadSet, const dbgcomm_info *ci) {
    ResetWDT();
    DBG_READY();
    return DbgBackendSW->setLineBreak(allowPending, file, line, threadSet, ci);
}

dbgevent_t *DbgSetFuncBreak(int allowPending, const char *file, const char *func,
                            const char* threadSet, const dbgcomm_info *ci) {
    ResetWDT();
    DBG_READY();
    return DbgBackendSW->setFuncBreak(allowPending, file, func, threadSet, ci);
}

dbgevent_t *DbgSetWatch(int type, const char *expr, const dbgcomm_info *ci) {
    ResetWDT();
    DBG_READY();
    return DbgBackendSW->setWatch(type, expr, ci);
}

dbgevent_t *DbgDeleteBreak(int bp) {
    ResetWDT();
    DBG_READY();
    return DbgBackendSW->deleteBreak(bp);
}

dbgevent_t *DbgDisableBreak(int bp) {
    ResetWDT();
    DBG_READY();
    return DbgBackendSW->disableBreak(bp);
}

dbgevent_t *DbgEnableBreak(int bp, bool once, bool del) {
    ResetWDT();
    DBG_READY();
    return DbgBackendSW->enableBreak(bp, once, del);
}

dbgevent_t *DbgLastBreak(void) {
    ResetWDT();
    DBG_READY();
    return DbgBackendSW->lastBreak();
}

dbgevent_t *DbgList(const char *str) {
    ResetWDT();
    DBG_READY();
    return DbgBackendSW->list(str);
}

dbgevent_t *DbgGetSources() {
    ResetWDT();
    DBG_READY();
    return DbgBackendSW->getSources();
}

dbgevent_t *DbgGo(bool all) {
    ResetWDT();
    DBG_READY();
    return DbgBackendSW->go(all);
}

dbgevent_t *DbgFinishFunc(void) {
    ResetWDT();
    DBG_READY();
    return DbgBackendSW->finishFunc();
}

dbgevent_t *DbgSetArgs(const char *args) {
    ResetWDT();
    DBG_READY();
    return DbgBackendSW->setArgs(args);
}

dbgevent_t *DbgStep(int count, int in) {
    ResetWDT();
    DBG_STOPPED();
    return DbgBackendSW->step(count, in);
}

dbgevent_t *DbgKill(void) {
    ResetWDT();
    DBG_READY();
    return DbgBackendSW->kill();
}

dbgevent_t *DbgMoveFrame(int count, bool up) {
    ResetWDT();
    DBG_READY();
    return DbgBackendSW->moveFrame(count, up);
}

// If count is 0, then an unlimited number of frames should be displayed.
dbgevent_t *DbgShowFrame(int count, const char* threads) {
    ResetWDT();
    DBG_READY();
    return DbgBackendSW->showFrame(count, threads);
}

dbgevent_t *DbgGetFrame(const char* threads) {
    ResetWDT();
    DBG_READY();
    return DbgBackendSW->getFrame(threads);
}

dbgevent_t *DbgSetDebuggerVar(const char *var, AIF *val) {
    ResetWDT();
    DBG_READY();
    return DbgBackendSW->setDebuggerVar(var, val);
}

dbgevent_t *DbgSetSourcePath(const char *path) {
    ResetWDT();
    DBG_READY();
    return DbgBackendSW->setSourcePath(path);
}

dbgevent_t *DbgGetSourcePath() {
    ResetWDT();
    DBG_READY();
    return DbgBackendSW->getSourcePath();
}

dbgevent_t *DbgGetDebuggerVar(const char *var) {
    ResetWDT();
    DBG_READY();
    return DbgBackendSW->getDebuggerVar(var);
}

dbgevent_t *DbgSetVar(const char *var, AIF *data) {
    ResetWDT();
    DBG_READY();
    return DbgBackendSW->setVar(var, data);
}

dbgevent_t *DbgEvalExpr(const char *expr, long exprCnt, const char* threads) {
    ResetWDT();
    DBG_READY();
    return DbgBackendSW->evalExpr(expr, exprCnt, threads);
}

dbgevent_t *DbgGetType(const char *var, bool isType) {
    ResetWDT();
    DBG_READY();
    return DbgBackendSW->getType(var, isType);
}

dbgevent_t *DbgGetLocation(void) {
    ResetWDT();
    DBG_READY();
    return DbgBackendSW->getLocation();
}

dbgevent_t *DbgOpenSocket(const char *opset_name, int opset_size) {
    ResetWDT();
    DBG_READY();
    return DbgBackendSW->openSocket(opset_name, opset_size);
}

dbgevent_t *DbgCloseSocket(void) {
    ResetWDT();
    DBG_READY();
    return DbgBackendSW->closeSocket();
}

dbgevent_t *DbgGetVars(const char *file) {
    ResetWDT();
    DBG_READY();
    return DbgBackendSW->getVars(file);
}

dbgevent_t *DbgGetScope(const char *location) {
    ResetWDT();
    DBG_READY();
    return DbgBackendSW->getScope(location);
}

dbgevent_t* DbgSetThread(const char* threadId) {
    ResetWDT();
    DBG_READY();
    return DbgBackendSW->setThread(threadId);
}

dbgevent_t* DbgGetThreadInfo(const char* threadId) {
    ResetWDT();
    DBG_READY();
    return DbgBackendSW->getThreadInfo(threadId);
}

dbgevent_t *DbgSetThreadName(const char* name) {
    ResetWDT();
    DBG_READY();
    return DbgBackendSW->setThreadName(name);
}

dbgevent_t *DbgFindThread(const char* pattern)
{
    ResetWDT();
    DBG_READY();
    return DbgBackendSW->findThread(pattern);
}

dbgevent_t *DbgThreadApply(const char* op)
{
    ResetWDT();
    DBG_READY();
    return DbgBackendSW->threadApply(op);
}

dbgevent_t *DbgSetSchedulerLocking(const char* mode)
{
    ResetWDT();
    DBG_READY();
    return DbgBackendSW->setSchedulerLocking(mode);
}

dbgevent_t *DbgGetSchedulerLocking()
{
    ResetWDT();
    DBG_READY();
    return DbgBackendSW->getSchedulerLocking();
}

dbgevent_t *DbgSetNonStop(const char* mode)
{
    ResetWDT();
    DBG_READY();
    return DbgBackendSW->setNonStop(mode);
}

dbgevent_t *DbgGetNonStop()
{
    ResetWDT();
    DBG_READY();
    return DbgBackendSW->getNonStop();
}


dbgevent_t *DbgDirectCmd(int cmd_argc, char **cmd_argv) {
    ResetWDT();
    DBG_READY();
    return DbgBackendSW->directCmd(cmd_argc, cmd_argv);
}

dbgevent_t *DbgStatus(int getLocation) {
    ResetWDT();
    DBG_CHECK();
    return DbgBackendSW->getStatus(getLocation);
}

dbgevent_t *DbgQuit(bool detach) {
    Shutdown++;
    DBG_CHECK();
    return DbgBackendSW->quit(detach);
}

dbgevent_t *DbgInterrupt(bool all) {
    ResetWDT();
    DBG_CHECK();
    return DbgBackendSW->interrupt(all);
}
