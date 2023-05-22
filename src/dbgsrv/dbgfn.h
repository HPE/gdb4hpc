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
#include "dbgcmd.h"

class dbgcomm_info;

extern void AsyncCheck(dbgevent_t *(*)(dbgevent_t *), dbgevent_t *);

extern int DbgSetBackend(const char *);
extern dbgevent_t *DbgInit(int *, int *, char **);
extern dbgevent_t *DbgAttach(int, const char *, const char *, char **);
extern dbgevent_t *DbgLaunch(const char *, int, const char *, const char *,
                             const char *, char **);
extern dbgevent_t *DbgTargetRemote(const char *);
extern void DbgFinalize(void);
extern int DbgProgIOHook(int);
extern int DbgRead(int);
extern int DbgBufferReady(void);
extern dbgevent_t *DbgStartup(void);
extern dbgevent_t *DbgSetLineBreak(int id, int, const char *, int, const char*, const dbgcomm_info *);
extern dbgevent_t *DbgSetFuncBreak(int id, int, const char *, const char *, const char*, const dbgcomm_info *);
extern dbgevent_t *DbgSetWatch(int id, int, const char *, const dbgcomm_info *);
extern dbgevent_t *DbgSetCatch(int id, const std::string &event, const std::string &regex, const dbgcomm_info *ci);
extern dbgevent_t *DbgSetCheckpoint();
extern dbgevent_t *DbgRestoreCheckpoint(unsigned int id);
extern dbgevent_t *DbgDeleteCheckpoint(unsigned int id);
extern dbgevent_t *DbgDeleteBreak(int);
extern dbgevent_t *DbgDisableBreak(int);
extern dbgevent_t *DbgEnableBreak(int, bool, bool);
extern dbgevent_t *DbgIgnoreBreak(int, int);
extern dbgevent_t *DbgConditionBreak(int, const std::string& cond);
extern dbgevent_t *DbgShowBreak(void);
extern dbgevent_t *DbgInfoBreak(void);
extern dbgevent_t *DbgList(const char *);
extern dbgevent_t *DbgGetSources(void);
extern dbgevent_t *DbgGo(bool all, const char* threads);
extern dbgevent_t *DbgFinishFunc(const char* threads);
extern dbgevent_t *DbgSetArgs(const char *);
extern dbgevent_t *DbgStep(int, int, const char*);
extern dbgevent_t *DbgMoveFrame(int, DbgFrameDir dir, const char*);
extern dbgevent_t *DbgShowFrame(int, const char*);
extern dbgevent_t *DbgGetFrame(bool, const char*);
extern dbgevent_t *DbgSetVar(const char *, const char* val, const char* threads);
extern dbgevent_t *DbgSetDebuggerVar(const char *, const char *val);
extern dbgevent_t *DbgSetSourcePath(DbgDirOp, const char *);
extern dbgevent_t *DbgGetSourcePath(void);
extern dbgevent_t *DbgSetPrintElements(const char* elements);
extern dbgevent_t *DbgResetBeforeDirectCmd();
extern dbgevent_t *DbgRestoreAfterDirectCmd();
extern dbgevent_t *DbgEvalExpr(const char *, long, const char*);
extern dbgevent_t *DbgGetType(const char *, bool, const char*);
extern dbgevent_t *DbgGetLocation(void);
extern dbgevent_t *DbgGetVars(const char*);
extern dbgevent_t *DbgGetArgs(const char*);
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
extern dbgevent_t *DbgSleep(int seconds);
extern dbgevent_t *DbgShell(int argc, char **argv);
extern dbgevent_t *DbgOpenSocket(const char *, int);
extern dbgevent_t *DbgCloseSocket(void);

#endif /* !_DBGFN_H */
