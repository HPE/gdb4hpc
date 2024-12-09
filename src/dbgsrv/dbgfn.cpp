/*********************************************************************************\
 * Server dispatch routines
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * Copyright 2010-2013,2020-2022 Hewlett Packard Enterprise Development LP.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Hewlett Packard Enterprise Development LP.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *
 *********************************************************************************/

#include "config.h"

#include <stdlib.h>
#include <string.h>

#include "dbgfn.h"
#include "dbgsrv_defs.h"
#include "defs.h"
#include "dbgsrv_gdbmi.h"

/*
** Setup debug backend defs
*/
struct backend_tab {
    const char *backend_name;
    DbgGdbMI *backend_def;
};
typedef struct backend_tab backend_tab;

// external debug backend def
extern DbgGdbMI* DbgGdbMIGetSingleton();

// table of backend defs
static backend_tab DbgBackendTab[] = {
    {"gdbmi", DbgGdbMIGetSingleton()},
    {nullptr, nullptr}};

/*
** Set default debug backend
** This defines the current debug backend in use
*/
static DbgGdbMI *DbgBackendSW = DbgGdbMIGetSingleton();

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
    return DbgBackendSW->init(dfd, iofd, env);
}

dbgevent_t *DbgAttach(int pid, const char *prog, const char *args, char **env, const char *main_function) {
    return DbgBackendSW->attach(pid, prog, args, env, main_function);
}

dbgevent_t *DbgLaunch(const char *host, int cb, const char *proto,
                      const char *prog, const char *args, char **env, const char *main_function) {
    return DbgBackendSW->launch(host, cb, proto, prog, args, env, main_function);
}

dbgevent_t *DbgTargetRemote(const char* target_remote) {
    return DbgBackendSW->targetRemote(target_remote);
}

void DbgFinalize(void) {
    return DbgBackendSW->finalize();
}

int DbgProgIOHook(int fd) {
    return DbgBackendSW->progioHook(fd);
}

int DbgRead(int fd) {
    return DbgBackendSW->read(fd);
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
    DBG_READY();
    return DbgBackendSW->startup();
}

dbgevent_t *DbgSetLineBreak(int id, int allowPending, const char *file, int line,
                            const char* threadSet, const dbgcomm_info *ci) {
    DBG_READY();
    if (DbgBackendSW->isPython()) return DbgBackendSW->pyBESetLineBreakpoint(id, file, line);
    return DbgBackendSW->setLineBreak(id, allowPending, file, line, threadSet, ci);
}

dbgevent_t *DbgSetFuncBreak(int id, int allowPending, const char *file, const char *func,
                            const char* threadSet, const dbgcomm_info *ci) {
    DBG_READY();
    if (DbgBackendSW->isPython()) return DbgBackendSW->pyBESetFuncBreakpoint(id, file, func);
    return DbgBackendSW->setFuncBreak(id, allowPending, file, func, threadSet, ci);
}

dbgevent_t *DbgSetWatch(int id, int type, const char *expr, const dbgcomm_info *ci) {
    DBG_READY();
    return DbgBackendSW->setWatch(id, type, expr, ci);
}

dbgevent_t *DbgSetCatch(int id, const std::string &event, const std::string &regex, const dbgcomm_info *ci) {
    DBG_READY();
    return DbgBackendSW->setCatch(id, event, regex, ci);
}

dbgevent_t *DbgSetCheckpoint() {
    DBG_READY();
    return DbgBackendSW->setCheckpoint();
}

dbgevent_t *DbgRestoreCheckpoint(unsigned int id) {
    DBG_READY();
    return DbgBackendSW->restoreCheckpoint(id);
}

dbgevent_t *DbgDeleteCheckpoint(unsigned int id) {
    DBG_READY();
    return DbgBackendSW->deleteCheckpoint(id);
}

dbgevent_t *DbgDeleteBreak(int bp) {
    DBG_READY();
    if (DbgBackendSW->isPython()) return DbgBackendSW->pyBEDeleteBreakpoint(bp);
    return DbgBackendSW->deleteBreak(bp);
}

dbgevent_t *DbgDisableBreak(int bp) {
    DBG_READY();
    return DbgBackendSW->disableBreak(bp);
}

dbgevent_t *DbgEnableBreak(int bp, bool once, bool del) {
    DBG_READY();
    return DbgBackendSW->enableBreak(bp, once, del);
}

dbgevent_t *DbgIgnoreBreak(int bp, int n) {
    DBG_READY();
    return DbgBackendSW->ignoreBreak(bp, n);
}

dbgevent_t *DbgConditionBreak(int bp, const std::string& cond) {
    DBG_READY();
    return DbgBackendSW->conditionBreak(bp, cond);
}


dbgevent_t *DbgInfoBreak(void) {
    DBG_READY();
    return DbgBackendSW->infoBreak();
}

dbgevent_t *DbgList(const char *str) {
    DBG_READY();
    return DbgBackendSW->list(str);
}

dbgevent_t *DbgGetSources() {
    DBG_READY();
    return DbgBackendSW->getSources();
}

dbgevent_t *DbgGo(bool all, const char* threads) {
    DBG_READY();
    return DbgBackendSW->go(all, threads);
}

dbgevent_t *DbgFinishFunc(const char* threads) {
    DBG_READY();
    if (DbgBackendSW->isPython()) return DbgBackendSW->pyBEFinish();
    return DbgBackendSW->finishFunc(threads);
}

dbgevent_t *DbgSetArgs(const char *args) {
    DBG_READY();
    return DbgBackendSW->setArgs(args);
}

dbgevent_t *DbgStep(int count, int in, const char* threads) {
    DBG_STOPPED();
    if (DbgBackendSW->isPython()) return DbgBackendSW->pyBEStep(static_cast<bool>(in));
    return DbgBackendSW->step(count, in, threads);
}

dbgevent_t *DbgMoveFrame(int count, DbgFrameDir dir, const char* threads) {
    DBG_READY();
    if (DbgBackendSW->isPython()) return DbgBackendSW->pyBEMoveFrame(count, dir);
    return DbgBackendSW->moveFrame(count, dir, threads);
}

// If count is 0, then an unlimited number of frames should be displayed.
dbgevent_t *DbgShowFrame(int count, const char* threads) {
    DBG_READY();
    if (DbgBackendSW->isPython()) return DbgBackendSW->pyBEBacktrace();
    return DbgBackendSW->showElfFrames(count, threads);
}

dbgevent_t *DbgGetFrame(bool verbose, const char* threads) {
    DBG_READY();
    if (DbgBackendSW->isPython()) return DbgBackendSW->pyBEGetFrame();
    return DbgBackendSW->getFrame(verbose, threads);
}

dbgevent_t *DbgSetDebuggerVar(const char *var, const char *val) {
    DBG_READY();
    return DbgBackendSW->setDebuggerVar(var, val);
}

dbgevent_t *DbgSetSourcePath(DbgDirOp op, const char *path) {
    DBG_READY();
    return DbgBackendSW->setSourcePath(op, path);
}

dbgevent_t *DbgGetSourcePath() {
    DBG_READY();
    return DbgBackendSW->getSourcePath();
}

dbgevent_t *DbgGetDebuggerVar(const char *var) {
    DBG_READY();
    return DbgBackendSW->getDebuggerVar(var);
}

dbgevent_t *DbgSetPrintElements(const char *elements) {
    DBG_READY();
    return DbgBackendSW->setPrintElements(elements);
}

dbgevent_t *DbgResetBeforeDirectCmd() {
    DBG_READY();
    return DbgBackendSW->resetBeforeDirectCmd();
}

dbgevent_t *DbgRestoreAfterDirectCmd() {
    DBG_READY();
    return DbgBackendSW->restoreAfterDirectCmd();
}

dbgevent_t *DbgSetVar(const char *var, const char* val, const char* threads) {
    DBG_READY();
    return DbgBackendSW->setVar(var, val, threads);
}

dbgevent_t *DbgEvalExpr(const char *expr, long exprCnt, const char* threads) {
    DBG_READY();
    if (DbgBackendSW->isPython()) return DbgBackendSW->pyBEEvalExpr(expr);
    return DbgBackendSW->evalExpr(expr, exprCnt, threads);
}

dbgevent_t *DbgGetType(const char *var, bool isType, const char* threads) {
    DBG_READY();
    return DbgBackendSW->getType(var, isType, threads);
}

dbgevent_t *DbgGetLocation(void) {
    DBG_READY();
    return DbgBackendSW->getLocation();
}

dbgevent_t *DbgOpenSocket(const char *opset_name, int opset_size) {
    DBG_READY();
    return DbgBackendSW->openSocket(opset_name, opset_size);
}

dbgevent_t *DbgCloseSocket(void) {
    DBG_READY();
    return DbgBackendSW->closeSocket();
}

dbgevent_t *DbgGetVars(const char* threads) {
    DBG_READY();
    return DbgBackendSW->getVars(threads);
}

dbgevent_t *DbgGetArgs(const char* threads) {
    DBG_READY();
    return DbgBackendSW->getArgs(threads);
}


dbgevent_t *DbgGetScope(const char *location) {
    DBG_READY();
    return DbgBackendSW->getScope(location);
}

dbgevent_t* DbgSetThread(const char* threadId) {
    DBG_READY();
    return DbgBackendSW->setThread(threadId);
}

dbgevent_t* DbgGetThreadInfo(const char* threadId, bool anonymize, bool collect) {
    DBG_READY();
    return DbgBackendSW->getThreadInfo(threadId, anonymize, collect);
}

dbgevent_t *DbgSetThreadName(const char* name) {
    DBG_READY();
    return DbgBackendSW->setThreadName(name);
}

dbgevent_t *DbgFindThread(const char* pattern)
{
    DBG_READY();
    return DbgBackendSW->findThread(pattern);
}

dbgevent_t *DbgThreadApply(const char* op)
{
    DBG_READY();
    return DbgBackendSW->threadApply(op);
}


dbgevent_t *DbgSetSchedulerLocking(const char* mode)
{
    DBG_READY();
    return DbgBackendSW->setSchedulerLocking(mode);
}

dbgevent_t *DbgGetSchedulerLocking()
{
    DBG_READY();
    return DbgBackendSW->getSchedulerLocking();
}

dbgevent_t *DbgSetNonStop(const char* mode)
{
    DBG_READY();
    return DbgBackendSW->setNonStop(mode);
}

dbgevent_t *DbgGetNonStop()
{
    DBG_READY();
    return DbgBackendSW->getNonStop();
}

dbgevent_t *DbgOmpd(const char* op)
{
    DBG_READY();
    return DbgBackendSW->ompd(op);
}

dbgevent_t *DbgCuda(const char* op)
{
    DBG_READY();
    return DbgBackendSW->cuda(op);
}

dbgevent_t *DbgSleep(int seconds) {
    DBG_READY();
    return DbgBackendSW->sleep(seconds);
}

dbgevent_t *DbgShell(int argc, char **argv){
    DBG_READY();
    return DbgBackendSW->shell(argc, argv);
}

dbgevent_t *DbgSignal(char const* signame) {
    DBG_READY();
    return DbgBackendSW->signal(signame);
}

dbgevent_t *DbgPyBEGetPython() {
    DBG_READY();
    return DbgBackendSW->pyBEGetPython();
}

dbgevent_t *DbgPyBESetPython(PyMode setting) {
    DBG_READY();
    return DbgBackendSW->pyBESetPython(setting);
}

dbgevent_t *DbgPyBEInject() {
    DBG_READY();
    return DbgBackendSW->pyBEInject();
}

dbgevent_t *DbgPyBEGetGIL() {
    DBG_READY();
    return DbgBackendSW->pyBEGetGIL();
}

dbgevent_t *DbgPyBEAttach() {
    DBG_READY();
    return DbgBackendSW->pyBEAttach();
}

std::vector<dbgevent_t*> DbgReleaseAsyncEvents() {
    // we don't need to check DBG_READY or DBG_CHECK here. if the backend needs to
    // use libmi to process its async events, it is its responsibility to handle
    // if the app is running etc.
    return DbgBackendSW->releaseAsyncEvents();
}

dbgevent_t *DbgDirectCmd(int cmd_argc, char **cmd_argv) {
    DBG_READY();
    return DbgBackendSW->directCmd(cmd_argc, cmd_argv);
}

dbgevent_t *DbgStatus(int getLocation) {
    DBG_CHECK();
    return DbgBackendSW->getStatus(getLocation);
}

dbgevent_t *DbgQuit(bool detach) {
    Shutdown++;
    DBG_CHECK();
    return DbgBackendSW->quit(detach);
}

dbgevent_t *DbgInterrupt(bool all) {
    DBG_CHECK();
    return DbgBackendSW->interrupt(all);
}
