/******************************************************************************\
 * dbgcmd.c - Command interface
 *
 * Copyright 2013-2020 Cray Inc.	All Rights Reserved.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Cray Inc.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *
 ******************************************************************************/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include "dbg.h"
#include "defs.h"
#include "useful.h"

dbgcommand_t *NewCommand(dbgcmd ct) {
    dbgcommand_t *c;

    c = (dbgcommand_t *)Alloc(sizeof(dbgcommand_t));
    c->command = ct;

    return c;
}

static void FreeCmdArgsInvoke(cmdargs_invoke *arg) {
    if (arg == NULL) return;

    Free(arg);
}

static void FreeCmdArgsSetLineBreak(cmdargs_setlinebreak *arg) {
    if (arg == NULL) return;

    Free(arg->file);

    Free(arg);
}

static void FreeCmdArgsSetFuncBreak(cmdargs_setfuncbreak *arg) {
    if (arg == NULL) return;

    Free(arg->file);
    Free(arg->func);

    Free(arg);
}

static void FreeCmdArgsSetWatch(cmdargs_setwatch *arg) {
    if (arg == NULL) return;

    Free(arg->expr);

    Free(arg);
}

static void FreeCmdArgsDeleteBreak(cmdargs_deletebreak *arg) {
    if (arg == NULL) return;

    Free(arg);
}

static void FreeCmdArgsDisableBreak(cmdargs_disablebreak *arg) {
    if (arg == NULL) return;

    Free(arg);
}

static void FreeCmdArgsEnableBreak(cmdargs_enablebreak *arg) {
    if (arg == NULL) return;

    Free(arg);
}

static void FreeCmdArgsList(cmdargs_list *arg) {
    if (arg == NULL) return;

    Free(arg);
}

static void FreeCmdArgsSetArgs(cmdargs_setargs *arg) {
    if (arg == NULL) return;

    Free(arg->args);

    Free(arg);
}

static void FreeCmdArgsStep(cmdargs_step *arg) {
    if (arg == NULL) return;

    Free(arg);
}

static void FreeCmdArgsMoveFrame(cmdargs_moveframe *arg) {
    if (arg == NULL) return;

    Free(arg);
}

static void FreeCmdArgsShowFrame(cmdargs_showframe *arg) {
    if (arg == NULL) return;

    Free(arg);
}

static void FreeCmdArgsSetDebuggerVar(cmdargs_setdebuggervar *arg) {
    if (arg == NULL) return;

    Free(arg->var);
    AIFFree(arg->val);

    Free(arg);
}

static void FreeCmdArgsSetSourcePath(cmdargs_setsourcepath *arg) {
    if (arg == NULL) return;

    Free(arg->path);

    Free(arg);
}

static void FreeCmdArgsGetDebuggerVar(cmdargs_getdebuggervar *arg) {
    if (arg == NULL) return;

    Free(arg->var);

    Free(arg);
}

static void FreeCmdArgsSetVar(cmdargs_setvar *arg) {
    if (arg == NULL) return;

    Free(arg->var);
    AIFFree(arg->val);

    Free(arg);
}

static void FreeCmdArgsEvalExpr(cmdargs_evalexpr *arg) {
    if (arg == NULL) return;

    Free(arg->expr);

    Free(arg);
}

static void FreeCmdArgsGetType(cmdargs_gettype *arg) {
    if (arg == NULL) return;

    Free(arg->sym);

    Free(arg);
}

static void FreeCmdArgsGetVars(cmdargs_getvars *arg) {
    if (arg == NULL) return;

    Free(arg->file);

    Free(arg);
}

static void FreeCmdArgsGetScope(cmdargs_getscope *arg) {
    if (arg == NULL) return;

    Free(arg->location);

    Free(arg);
}

static void FreeCmdArgsDirect(cmdargs_direct *arg) {
    int i;

    if (arg == NULL) return;

    for (i = 0; i < arg->cmd_argc; ++i) {
        Free(arg->cmd_argv[i]);
    }

    Free(arg->cmd_argv);

    Free(arg);
}

static void FreeCmdArgsStatus(cmdargs_status *arg) {
    if (arg == NULL) return;

    Free(arg);
}

static void FreeCmdArgsQuit(cmdargs_quit *arg) {
    if (arg == NULL) return;

    Free(arg);
}

static void FreeCmdArgsOpenSocket(cmdargs_opensocket *arg) {
    if (arg == NULL) return;

    Free(arg->opset_name);

    Free(arg);
}

static void FreeCmdArgsString(cmdargs_string *arg) {
    if (arg) {
        Free(const_cast<char*>(arg->str));
        Free(arg);
    }
}

void FreeCommand(dbgcommand_t *cmd) {
    if (cmd == NULL) return;

    switch (cmd->command) {
        case DBGCMD_INVOKE:
            FreeCmdArgsInvoke(cmd->cmd_arg_invoke);
            break;

        case DBGCMD_SETLINEBREAK:
            FreeCmdArgsSetLineBreak(cmd->cmd_arg_setlinebreak);
            break;

        case DBGCMD_SETFUNCBREAK:
            FreeCmdArgsSetFuncBreak(cmd->cmd_arg_setfuncbreak);
            break;

        case DBGCMD_SETWATCH:
            FreeCmdArgsSetWatch(cmd->cmd_arg_setwatch);
            break;

        case DBGCMD_DELETEBREAK:
            FreeCmdArgsDeleteBreak(cmd->cmd_arg_deletebreak);
            break;

        case DBGCMD_DISABLEBREAK:
            FreeCmdArgsDisableBreak(cmd->cmd_arg_disablebreak);
            break;

        case DBGCMD_ENABLEBREAK:
            FreeCmdArgsEnableBreak(cmd->cmd_arg_enablebreak);
            break;

        case DBGCMD_LIST:
            FreeCmdArgsList(cmd->cmd_arg_list);
            break;

        case DBGCMD_SETARGS:
            FreeCmdArgsSetArgs(cmd->cmd_arg_setargs);
            break;

        case DBGCMD_STEP:
            FreeCmdArgsStep(cmd->cmd_arg_step);
            break;

        case DBGCMD_MOVEFRAME:
            FreeCmdArgsMoveFrame(cmd->cmd_arg_moveframe);
            break;

        case DBGCMD_SHOWFRAME:
            FreeCmdArgsShowFrame(cmd->cmd_arg_showframe);
            break;

        case DBGCMD_SETDEBUGGERVAR:
            FreeCmdArgsSetDebuggerVar(cmd->cmd_arg_setdebuggervar);
            break;

        case DBGCMD_SETSOURCEPATH:
            FreeCmdArgsSetSourcePath(cmd->cmd_arg_setsourcepath);
            break;

        case DBGCMD_GETDEBUGGERVAR:
            FreeCmdArgsGetDebuggerVar(cmd->cmd_arg_getdebuggervar);
            break;

        case DBGCMD_SETVAR:
            FreeCmdArgsSetVar(cmd->cmd_arg_setvar);
            break;

        case DBGCMD_EVALEXPR:
        case DBGCMD_CONTINUEEVALEXPR:
            FreeCmdArgsEvalExpr(cmd->cmd_arg_evalexpr);
            break;

        case DBGCMD_GETTYPE:
            FreeCmdArgsGetType(cmd->cmd_arg_gettype);
            break;

        case DBGCMD_GETVARS:
            FreeCmdArgsGetVars(cmd->cmd_arg_getvars);
            break;

        case DBGCMD_GETSCOPE:
            FreeCmdArgsGetScope(cmd->cmd_arg_getscope);
            break;

        case DBGCMD_DIRECT:
            FreeCmdArgsDirect(cmd->cmd_arg_direct);
            break;

        case DBGCMD_STATUS:
            FreeCmdArgsStatus(cmd->cmd_arg_status);
            break;

        case DBGCMD_QUIT:
            FreeCmdArgsQuit(cmd->cmd_arg_quit);
            break;

        case DBGCMD_OPENSOCKET:
            FreeCmdArgsOpenSocket(cmd->cmd_arg_opensocket);
            break;

        case DBGCMD_THREAD:
        case DBGCMD_GETTHREADINFO:
        case DBGCMD_SETTHREADNAME:
        case DBGCMD_FINDTHREAD:
        case DBGCMD_THREADAPPLY:
        case DBGCMD_SETSCHEDULERLOCKING:
        case DBGCMD_SETNONSTOP:
            FreeCmdArgsString(cmd->cmd_u.cmd_u_string);
            break;

        // Commands without arguments go here
        case DBGCMD_STARTUP:
        case DBGCMD_LASTBREAK:
        case DBGCMD_GETSOURCES:
        case DBGCMD_GO:
        case DBGCMD_FINISH:
        case DBGCMD_KILL:
        case DBGCMD_GETFRAME:
        case DBGCMD_GETSOURCEPATH:
        case DBGCMD_GETLOCATION:
        case DBGCMD_INTERRUPT:
        case DBGCMD_CLOSESOCKET:
        case DBGCMD_SETCOMMPARAM:
        case DBGCMD_GETSCHEDULERLOCKING:
        case DBGCMD_GETNONSTOP:
            break;
    }

    Free(cmd);
}
