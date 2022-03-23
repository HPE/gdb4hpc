/*********************************************************************************\
 * Debug support routine definitions
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * Copyright 2011-2022 Hewlett Packard Enterprise Development LP.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Hewlett Packard Enterprise Development LP.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *
 *********************************************************************************/

#ifndef _DBGFN_H
#define _DBGFN_H

#include "dbgevent.h"

class dbgcomm_info;

extern void AsyncCheck(dbgevent_t *(*)(dbgevent_t *), dbgevent_t *);

class dbgbackend_sw {
public:
    virtual dbgevent_t *init(int *dfd, int *iofd, char **env) = 0;
    virtual dbgevent_t *startup() = 0;
    virtual int read(int) = 0;
    virtual bool bufferReady() = 0;
    virtual int progioHook(int) = 0;   //!!! careful - potentially unimplemented
    virtual dbgevent_t *setLineBreak(const int, const char *file, int lno, const char* threadSet, const dbgcomm_info *commInfo) = 0;
    virtual dbgevent_t *setFuncBreak(const int, const char *file, const char *func, const char* threadSet, const dbgcomm_info *commInfo) = 0;
    virtual dbgevent_t *setWatch(int type, const char *expr, const dbgcomm_info *commInfo) = 0;
    virtual dbgevent_t *setCatch(const std::string &event, const std::string &regex, const dbgcomm_info *commInfo) = 0;
    virtual dbgevent_t *setCheckpoint() = 0;
    virtual dbgevent_t *restoreCheckpoint(unsigned int id) = 0;
    virtual dbgevent_t *deleteCheckpoint(unsigned int id) = 0;
    virtual dbgevent_t *deleteBreak(int bid) = 0;
    virtual dbgevent_t *disableBreak(int bid) = 0;
    virtual dbgevent_t *enableBreak(int bid, bool once, bool del) = 0;
    virtual dbgevent_t *lastBreak() = 0;
    virtual dbgevent_t *list(const char *str) = 0;
    virtual dbgevent_t *getSources() = 0;
    virtual dbgevent_t *go(bool all, const char* threads) = 0;
    virtual dbgevent_t *finishFunc(const char* threads) = 0;
    virtual dbgevent_t *setArgs(const char *arg) = 0; // set run arguments
    virtual dbgevent_t *step(int const, bool in, const char*) = 0;
    virtual dbgevent_t *kill() = 0;
    virtual dbgevent_t *moveFrame(int count, bool up, const char* threads) = 0;

    // If count is not zero then return only that many frames. If count is zero then
    // return an unlimited number of frames. If count is negative return the
    // outermost frames.
    virtual dbgevent_t *showFrame(int count, const char* threads) = 0;

    virtual dbgevent_t *getFrame(const char* threads) = 0;
    virtual dbgevent_t *setVar(const char *var, const char *a, const char* threads) = 0;
    virtual dbgevent_t *evalExpr(const char *expr, long exprIdx, const char* threads) = 0;
    virtual dbgevent_t *getType(const char *var, bool isType, const char* threads) = 0;
    virtual dbgevent_t *getLocation() = 0;   //!! potentially unimplemented
    virtual dbgevent_t *getVars(const char* threads) = 0;
    virtual dbgevent_t *getScope(const char *location) = 0;
    virtual dbgevent_t *getStatus(bool getLocation) = 0;
    virtual dbgevent_t *quit(bool detach) = 0;
    virtual dbgevent_t *interrupt(bool all) = 0;
    virtual dbgevent_t *launch(const char *host, int cp, const char *proto, const char *prog,
                               const char *args, char **env, int arch) = 0;
    virtual dbgevent_t *attach(int pid, const char *prog, const char *args, char **env, int arch) = 0;
    virtual void finalize() = 0;
    virtual bool isReadyForCmd() = 0;
    virtual bool isOk() = 0;
    virtual bool isStopped() = 0;
    virtual dbgevent_t *setDebuggerVar(const char *var, const char *val) = 0;
    virtual dbgevent_t *setSourcePath(const char *path) = 0;
    virtual dbgevent_t *getSourcePath() = 0;
    virtual dbgevent_t *getDebuggerVar(const char *var) = 0;
    virtual dbgevent_t *setPrintElements(const char *elements) = 0;
    virtual dbgevent_t *resetBeforeDirectCmd() = 0;
    virtual dbgevent_t *restoreAfterDirectCmd() = 0;

    // Pass command directly to gdb/mi through console command
    virtual dbgevent_t *directCmd(int argc, char **argv) = 0;
    virtual dbgevent_t *openSocket(const char *opset_name, int opset_size) = 0;
    virtual dbgevent_t *closeSocket() = 0;
    virtual dbgevent_t *setThread(const char* threadId) = 0;
    virtual dbgevent_t *getThreadInfo(const char* threadId) = 0;
    virtual dbgevent_t *setThreadName(const char* name) = 0;
    virtual dbgevent_t *findThread(const char* pattern) = 0;
    virtual dbgevent_t *threadApply(const char* op) = 0;
    virtual dbgevent_t *setSchedulerLocking(const char* mode) = 0;
    virtual dbgevent_t *getSchedulerLocking() = 0;
    virtual dbgevent_t *setNonStop(const char* mode) = 0;
    virtual dbgevent_t *getNonStop() = 0;
    virtual dbgevent_t *ompd(const char* op) = 0;
    virtual dbgevent_t *cuda(const char* op) = 0;

};

extern int DbgSetBackend(const char *);
extern dbgevent_t *DbgInit(int *, int *, char **);
extern dbgevent_t *DbgAttach(int, const char *, const char *, char **, int);
extern dbgevent_t *DbgLaunch(const char *, int, const char *, const char *,
                             const char *, char **, int);
extern void DbgFinalize(void);
extern int DbgProgIOHook(int);
extern int DbgRead(int);
extern int DbgBufferReady(void);
extern dbgevent_t *DbgStartup(void);
extern dbgevent_t *DbgSetLineBreak(int, const char *, int, const char*, const dbgcomm_info *);
extern dbgevent_t *DbgSetFuncBreak(int, const char *, const char *, const char*, const dbgcomm_info *);
extern dbgevent_t *DbgSetWatch(int, const char *, const dbgcomm_info *);
extern dbgevent_t *DbgSetCatch(const std::string &event, const std::string &regex, const dbgcomm_info *ci);
extern dbgevent_t *DbgSetCheckpoint();
extern dbgevent_t *DbgRestoreCheckpoint(unsigned int id);
extern dbgevent_t *DbgDeleteCheckpoint(unsigned int id);
extern dbgevent_t *DbgDeleteBreak(int);
extern dbgevent_t *DbgDisableBreak(int);
extern dbgevent_t *DbgEnableBreak(int, bool, bool);
extern dbgevent_t *DbgShowBreak(void);
extern dbgevent_t *DbgLastBreak(void);
extern dbgevent_t *DbgList(const char *);
extern dbgevent_t *DbgGetSources(void);
extern dbgevent_t *DbgGo(bool all, const char* threads);
extern dbgevent_t *DbgFinishFunc(const char* threads);
extern dbgevent_t *DbgSetArgs(const char *);
extern dbgevent_t *DbgStep(int, int, const char*);
extern dbgevent_t *DbgKill(void);
extern dbgevent_t *DbgMoveFrame(int, bool, const char*);
extern dbgevent_t *DbgShowFrame(int, const char*);
extern dbgevent_t *DbgGetFrame(const char*);
extern dbgevent_t *DbgSetVar(const char *, const char* val, const char* threads);
extern dbgevent_t *DbgSetDebuggerVar(const char *, const char *val);
extern dbgevent_t *DbgSetSourcePath(const char *);
extern dbgevent_t *DbgGetSourcePath(void);
extern dbgevent_t *DbgSetPrintElements(const char* elements);
extern dbgevent_t *DbgResetBeforeDirectCmd();
extern dbgevent_t *DbgRestoreAfterDirectCmd();
extern dbgevent_t *DbgEvalExpr(const char *, long, const char*);
extern dbgevent_t *DbgGetType(const char *, bool, const char*);
extern dbgevent_t *DbgGetLocation(void);
extern dbgevent_t *DbgGetVars(const char*);
extern dbgevent_t *DbgGetScope(const char *);
extern dbgevent_t *DbgGetDebuggerVar(const char *);
extern dbgevent_t *DbgDirectCmd(int, char **);
extern dbgevent_t *DbgStatus(int);
extern dbgevent_t *DbgGetEvent(void);
extern dbgevent_t *DbgQuit(bool);
extern dbgevent_t *DbgInterrupt(bool all);
extern dbgevent_t *DbgSetThread(const char* threadId);
extern dbgevent_t *DbgGetThreadInfo(const char* threadId);
extern dbgevent_t *DbgSetThreadName(const char* name);
extern dbgevent_t *DbgFindThread(const char* pattern);
extern dbgevent_t *DbgThreadApply(const char* op);
extern dbgevent_t *DbgSetSchedulerLocking(const char* mode);
extern dbgevent_t *DbgGetSchedulerLocking();
extern dbgevent_t *DbgSetNonStop(const char* mode);
extern dbgevent_t *DbgGetNonStop();
extern dbgevent_t *DbgOmpd(const char* op);
extern dbgevent_t *DbgCuda(const char* op);
extern dbgevent_t *DbgOpenSocket(const char *, int);
extern dbgevent_t *DbgCloseSocket(void);

#endif /* !_DBGFN_H */
