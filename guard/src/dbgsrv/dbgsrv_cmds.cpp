/*********************************************************************************\
 * Server commands dispatch routines
 *
 * Copyright (c) 1996-2012 by Monash Uni.
 *
 * Copyright 2013,2020 Cray Inc.
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

#include "dbg.h"
#include "dbgfn.h"
#include "dbgsrv_cmds.h"
#include "dbgsrv_defs.h"
#include "dbgsrv_proto.h"
#include "dbgsrv_util.h"
#include "useful.h"

#define CHECK_ARGS(e, cargs)                                     \
    if (cargs == NULL) {                                         \
        EVENT_ERROR(e, DBGERR_INTERNAL,                          \
                    "Received command without valid arguments"); \
        break;                                                   \
    }

void dbgsrv_dispatch_cmds(char *msg, const dbgcomm_info *ci) {
    dbgcommand_t *cmd;
    dbgevent_t *e = NULL;

    DVPRINT("dbgsrv_dispatch_cmds received: %s\n", msg);

    // Convert the network string to a command and free the msg
    cmd = SocProtoToCommand(msg);
    Free(msg);

    bool trackSize = false;
    unsigned long size0 = 0;

    // switch on the command
    switch (cmd->command) {
        case DBGCMD_INVOKE:
            CHECK_ARGS(e, cmd->cmd_arg_invoke);
            e = DbgInvokeNetwork(cmd->cmd_invoke_numbe);
            break;

        case DBGCMD_STARTUP:
            e = DbgStartup();
            break;

        case DBGCMD_SETLINEBREAK:
            CHECK_ARGS(e, cmd->cmd_arg_setlinebreak);
            e = DbgSetLineBreak(cmd->cmd_setlinebreak_file,
                                cmd->cmd_setlinebreak_lno, ci);
            break;

        case DBGCMD_SETFUNCBREAK:
            CHECK_ARGS(e, cmd->cmd_arg_setfuncbreak);
            e = DbgSetFuncBreak(cmd->cmd_setfuncbreak_file,
                                cmd->cmd_setfuncbreak_func, ci);
            break;

        case DBGCMD_SETWATCH:
            CHECK_ARGS(e, cmd->cmd_arg_setwatch);
            e = DbgSetWatch(cmd->cmd_setwatch_type, cmd->cmd_setwatch_expr, ci);
            break;

        case DBGCMD_DELETEBREAK:
            CHECK_ARGS(e, cmd->cmd_arg_deletebreak);
            e = DbgDeleteBreak(cmd->cmd_deletebreak_bpid);
            break;

        case DBGCMD_DISABLEBREAK:
            CHECK_ARGS(e, cmd->cmd_arg_disablebreak);
            e = DbgDisableBreak(cmd->cmd_disablebreak_bpid);
            break;

        case DBGCMD_ENABLEBREAK:
            CHECK_ARGS(e, cmd->cmd_arg_enablebreak);
            e = DbgEnableBreak(cmd->cmd_enablebreak_bpid,
                               cmd->cmd_enablebreak_once,
                               cmd->cmd_enablebreak_del);
            break;

        case DBGCMD_LASTBREAK:
            e = DbgLastBreak();
            break;

        case DBGCMD_LIST:
            CHECK_ARGS(e, cmd->cmd_arg_list);
            e = DbgList(cmd->cmd_list_expr);
            break;

        case DBGCMD_GETSOURCES:
            e = DbgGetSources();
            break;

        case DBGCMD_GO:
            e = DbgGo();
            break;

        case DBGCMD_FINISH:
            e = DbgFinishFunc();
            break;

        case DBGCMD_SETARGS:
            CHECK_ARGS(e, cmd->cmd_arg_setargs);
            e = DbgSetArgs(cmd->cmd_setargs_args);
            break;

        case DBGCMD_STEP:
            CHECK_ARGS(e, cmd->cmd_arg_step);
            e = DbgStep(cmd->cmd_step_count, cmd->cmd_step_into);
            break;

        case DBGCMD_KILL:
            e = DbgKill();
            break;

        case DBGCMD_MOVEFRAME:
            CHECK_ARGS(e, cmd->cmd_arg_moveframe);
            e = DbgMoveFrame(cmd->cmd_moveframe_count,
                             cmd->cmd_moveframe_direction);
            break;

        case DBGCMD_SHOWFRAME:
            CHECK_ARGS(e, cmd->cmd_arg_showframe);
            e = DbgShowFrame(cmd->cmd_showframe_count);
            break;

        case DBGCMD_GETFRAME:
            e = DbgGetFrame();
            break;

        case DBGCMD_SETDEBUGGERVAR:
            CHECK_ARGS(e, cmd->cmd_arg_setdebuggervar);
            e = DbgSetDebuggerVar(cmd->cmd_setdebuggervar_var,
                                  cmd->cmd_setdebuggervar_val);
            break;

        case DBGCMD_SETSOURCEPATH:
            CHECK_ARGS(e, cmd->cmd_arg_setsourcepath);
            e = DbgSetSourcePath(cmd->cmd_setsourcepath_path);
            break;

        case DBGCMD_GETSOURCEPATH:
            e = DbgGetSourcePath();
            break;

        case DBGCMD_GETDEBUGGERVAR:
            CHECK_ARGS(e, cmd->cmd_arg_getdebuggervar);
            e = DbgGetDebuggerVar(cmd->cmd_getdebuggervar_var);
            break;

        case DBGCMD_SETVAR:
            CHECK_ARGS(e, cmd->cmd_arg_setvar);
            e = DbgSetVar(cmd->cmd_setvar_var, cmd->cmd_setvar_val);
            break;

        case DBGCMD_EVALEXPR:
        case DBGCMD_CONTINUEEVALEXPR:
            CHECK_ARGS(e, cmd->cmd_arg_evalexpr);
            size0 = GetProcSize();
            trackSize = true;
            e = DbgEvalExpr(cmd->cmd_evalexpr_expr, cmd->cmd_u.cmd_u_evalexpr->exprIdx);
            break;

        case DBGCMD_GETTYPE:
            CHECK_ARGS(e, cmd->cmd_arg_gettype);
            e = DbgGetType(cmd->cmd_gettype_sym, cmd->cmd_gettype_isType);
            break;

        case DBGCMD_GETLOCATION:
            e = DbgGetLocation();
            break;

        case DBGCMD_GETVARS:
            CHECK_ARGS(e, cmd->cmd_arg_getvars);
            e = DbgGetVars(cmd->cmd_getvars_file);
            break;

        case DBGCMD_GETSCOPE:
            CHECK_ARGS(e, cmd->cmd_arg_getscope);
            e = DbgGetScope(cmd->cmd_getscope_location);
            break;

        case DBGCMD_STATUS:
            CHECK_ARGS(e, cmd->cmd_arg_status);
            e = DbgStatus(cmd->cmd_status_getLocation);
            break;

        case DBGCMD_DIRECT:
            CHECK_ARGS(e, cmd->cmd_arg_direct);
            e = DbgDirectCmd(cmd->cmd_direct_argc, cmd->cmd_direct_argv);
            break;

        case DBGCMD_QUIT:
            CHECK_ARGS(e, cmd->cmd_arg_quit);
            e = DbgQuit(cmd->cmd_quit_detach);
            break;

        case DBGCMD_INTERRUPT:
            e = DbgInterrupt();
            break;

        case DBGCMD_OPENSOCKET:
            CHECK_ARGS(e, cmd->cmd_arg_opensocket);
            e = DbgOpenSocket(cmd->cmd_opensocket_name,
                              cmd->cmd_opensocket_size);
            break;

        case DBGCMD_CLOSESOCKET:
            e = DbgCloseSocket();
            break;

        case DBGCMD_SETCOMMPARAM:
            e = NULL;  // this command should be caught by the communication
                       // level
            break;

        case DBGCMD_THREAD:
            CHECK_ARGS(e, cmd->cmd_u.cmd_u_thread->threadId);
            e = DbgSetThread(cmd->cmd_u.cmd_u_thread->threadId);
            break;

        case DBGCMD_GETTHREADINFO:
            e = DbgGetThreadInfo(cmd->cmd_u.cmd_u_thread->threadId);
            break;
            
        case DBGCMD_SETTHREADNAME:
            CHECK_ARGS(e, cmd->cmd_u.cmd_u_thread->threadId);
            e = DbgSetThreadName(cmd->cmd_u.cmd_u_thread->threadId);
            break;

        case DBGCMD_FINDTHREAD:
            e = DbgFindThread(cmd->cmd_u.cmd_u_thread->threadId);
            break;

        case DBGCMD_THREADAPPLY:
            DVPRINT("in DBGCMD_THREADAPPLY\n");
            e = DbgThreadApply(cmd->cmd_u.cmd_u_thread->threadId);
            break;
    }

    FreeCommand(cmd);

    unsigned long size1 = 0, size2 = 0;
    if (trackSize)
        size1 = GetProcSize();

    // Convert the event to network protocol and send it
    // TODO: remove the null event check here when the DbgInvokeNetwork function
    // gets fixed for MRNet. Every command should be returning a response.

    if (!e)
        return;


    if (char* resp = EventToSocProto(e)) {
        DbgSendReply(resp);
    }

    if (trackSize) {
        size2 = GetProcSize();
        DVPRINT("Sizes at dispatcher %lu %lu %lu\n", size0, size1, size2);
    }
}
