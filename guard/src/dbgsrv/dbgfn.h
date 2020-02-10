/*********************************************************************************\
 * Debug support routine definitions
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * © 2011-2020 Cray Inc.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Cray Inc.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *
 *********************************************************************************/

#ifndef _DBGFN_H
#define _DBGFN_H

#include "aif.h"
#include "dbgevent.h"

class dbgcomm_info;

#define ASYNC_FORCE (dbgevent_t * (*)(dbgevent_t *)) - 1

extern void AsyncCheck(dbgevent_t *(*)(dbgevent_t *), dbgevent_t *);

struct dbgbackend_sw {
    dbgevent_t *(*dbg_init)(int *, int *, char **);
    dbgevent_t *(*dbg_startup)(void);
    int (*dbg_read)(int);
    int (*dbg_bufferready)(void);
    int (*dbg_progio_hook)(int);
    dbgevent_t *(*dbg_setlinebreak)(const char *, int, const dbgcomm_info *);
    dbgevent_t *(*dbg_setfuncbreak)(const char *, char *, const dbgcomm_info *);
    dbgevent_t *(*dbg_setwatch)(int, const char *, const dbgcomm_info *);
    dbgevent_t *(*dbg_deletebreak)(int);
    dbgevent_t *(*dbg_disablebreak)(int);
    dbgevent_t *(*dbg_enablebreak)(int, int, int);
    dbgevent_t *(*dbg_lastbreak)(void);
    dbgevent_t *(*dbg_list)(const char *);
    dbgevent_t *(*dbg_getsources)(void);
    dbgevent_t *(*dbg_go)(void);
    dbgevent_t *(*dbg_finishfunc)(void);
    dbgevent_t *(*dbg_setargs)(const char *);
    dbgevent_t *(*dbg_step)(int, int);
    dbgevent_t *(*dbg_kill)(void);
    dbgevent_t *(*dbg_moveframe)(int, int);
    dbgevent_t *(*dbg_showframe)(int);
    dbgevent_t *(*dbg_getframe)(void);
    dbgevent_t *(*dbg_setvar)(const char *, AIF *);
    dbgevent_t *(*dbg_evalexpr)(const char *);
    dbgevent_t *(*dbg_gettype)(const char *, int);
    dbgevent_t *(*dbg_getlocation)(void);
    dbgevent_t *(*dbg_getvars)(const char *);
    dbgevent_t *(*dbg_getscope)(const char *);
    dbgevent_t *(*dbg_status)(int);
    dbgevent_t *(*dbg_quit)(int);
    dbgevent_t *(*dbg_interrupt)(void);
    dbgevent_t *(*dbg_invoke)(const char *, int, const char *, const char *,
                              const char *, char **, int);
    dbgevent_t *(*dbg_attach)(int, const char *, const char *, char **, int);
    void (*dbg_finish)(void);
    int (*dbg_readyforcmd)(void);
    int (*dbg_ok)(void);
    int (*dbg_stopped)(void);
    dbgevent_t *(*dbg_setdebuggervar)(const char *, AIF *);
    dbgevent_t *(*dbg_setsourcepath)(const char *);
    dbgevent_t *(*dbg_getsourcepath)(void);
    dbgevent_t *(*dbg_getdebuggervar)(const char *);
    dbgevent_t *(*dbg_directcmd)(int, char **);
    dbgevent_t *(*dbg_opensocket)(const char *, int);
    dbgevent_t *(*dbg_closesocket)(void);
};
typedef struct dbgbackend_sw dbgbackend_sw;

extern int DbgSetBackend(const char *);
extern dbgevent_t *DbgInit(int *, int *, char **);
extern dbgevent_t *DbgAttach(int, const char *, const char *, char **, int);
extern dbgevent_t *DbgInvoke(const char *, int, const char *, const char *,
                             const char *, char **, int);
extern void DbgFinish(void);
extern int DbgProgIOHook(int);
extern int DbgRead(int);
extern int DbgBufferReady(void);
extern dbgevent_t *DbgStartup(void);
extern dbgevent_t *DbgSetLineBreak(char *, int, const dbgcomm_info *);
extern dbgevent_t *DbgSetFuncBreak(char *, char *, const dbgcomm_info *);
extern dbgevent_t *DbgSetWatch(int, char *, const dbgcomm_info *);
extern dbgevent_t *DbgDeleteBreak(int);
extern dbgevent_t *DbgDisableBreak(int);
extern dbgevent_t *DbgEnableBreak(int, int, int);
extern dbgevent_t *DbgShowBreak(void);
extern dbgevent_t *DbgLastBreak(void);
extern dbgevent_t *DbgList(char *);
extern dbgevent_t *DbgGetSources(void);
extern dbgevent_t *DbgGo(void);
extern dbgevent_t *DbgFinishFunc(void);
extern dbgevent_t *DbgSetArgs(char *);
extern dbgevent_t *DbgStep(int, int);
extern dbgevent_t *DbgKill(void);
extern dbgevent_t *DbgMoveFrame(int, int);
extern dbgevent_t *DbgShowFrame(int);
extern dbgevent_t *DbgGetFrame(void);
extern dbgevent_t *DbgSetVar(char *, AIF *);
extern dbgevent_t *DbgSetDebuggerVar(char *, AIF *);
extern dbgevent_t *DbgSetSourcePath(char *);
extern dbgevent_t *DbgGetSourcePath(void);
extern dbgevent_t *DbgEvalExpr(char *);
extern dbgevent_t *DbgGetType(char *, int);
extern dbgevent_t *DbgGetLocation(void);
extern dbgevent_t *DbgGetVars(char *);
extern dbgevent_t *DbgGetScope(char *);
extern dbgevent_t *DbgGetDebuggerVar(char *);
extern dbgevent_t *DbgDirectCmd(int, char **);
extern dbgevent_t *DbgStatus(int);
extern dbgevent_t *DbgGetEvent(void);
extern dbgevent_t *DbgQuit(int);
extern dbgevent_t *DbgInterrupt(void);
extern dbgevent_t *DbgOpenSocket(char *, int);
extern dbgevent_t *DbgCloseSocket(void);

#endif /* !_DBGFN_H */
