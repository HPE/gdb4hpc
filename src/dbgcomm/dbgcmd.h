/******************************************************************************\
 * dbgcmd.h - Command interface defines
 *
 * Copyright 2013-2021 Hewlett Packard Enterprise Development LP.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Hewlett Packard Enterprise Development LP.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *
 ******************************************************************************/

#ifndef _DBGCMD_H
#define _DBGCMD_H

enum dbgcmd {
    DBGCMD_INVOKE,
    DBGCMD_STARTUP,
    DBGCMD_SETLINEBREAK,
    DBGCMD_SETFUNCBREAK,
    DBGCMD_SETWATCH,
    DBGCMD_SETCATCH,
    DBGCMD_DELETEBREAK,
    DBGCMD_DISABLEBREAK,
    DBGCMD_ENABLEBREAK,
    DBGCMD_IGNOREBREAK,
    DBGCMD_CONDITIONBREAK,
    DBGCMD_INFOBREAK,
    DBGCMD_SETCHECKPOINT,
    DBGCMD_RESTORECHECKPOINT,
    DBGCMD_DELETECHECKPOINT,
    DBGCMD_LIST,
    DBGCMD_GETSOURCES,
    DBGCMD_GO,
    DBGCMD_FINISH,
    DBGCMD_SETARGS,
    DBGCMD_STEP,
    DBGCMD_MOVEFRAME,
    DBGCMD_SHOWFRAME,
    DBGCMD_GETFRAME,
    DBGCMD_SETDEBUGGERVAR,
    DBGCMD_SETSOURCEPATH,
    DBGCMD_GETSOURCEPATH,
    DBGCMD_SETPRINTELEMENTS,
    DBGCMD_RESETBEFOREDIRECTCMD,
    DBGCMD_RESTOREAFTERDIRECTCMD,
    DBGCMD_GETDEBUGGERVAR,
    DBGCMD_SETVAR,
    DBGCMD_EVALEXPR,
    DBGCMD_GETTYPE,
    DBGCMD_GETLOCATION,
    DBGCMD_GETVARS,
    DBGCMD_GETSCOPE,
    DBGCMD_DIRECT,
    DBGCMD_STATUS,
    DBGCMD_QUIT,
    DBGCMD_INTERRUPT,
    DBGCMD_OPENSOCKET,
    DBGCMD_CLOSESOCKET,
    DBGCMD_SETCOMMPARAM,  // send parameter to the communication layer
    DBGCMD_CONTINUEEVALEXPR,
    DBGCMD_THREAD,
    DBGCMD_GETTHREADINFO,
    DBGCMD_SETTHREADNAME,
    DBGCMD_FINDTHREAD,
    DBGCMD_THREADAPPLY,
    DBGCMD_SETSCHEDULERLOCKING,
    DBGCMD_GETSCHEDULERLOCKING,
    DBGCMD_SETNONSTOP,
    DBGCMD_GETNONSTOP,
    DBGCMD_OMPD,
    DBGCMD_CUDA,
    DBGCMD_SLEEP,
    DBGCMD_SHELL,
    DBGCMD_GETARGS
};

#endif /* !_DBGCMD_H */
