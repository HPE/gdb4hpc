/******************************************************************************\
 * dbgcmd.h - Command interface defines
 *
 * Copyright 2013 Cray Inc.	All Rights Reserved.
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
 ******************************************************************************/

#ifndef _DBGCMD_H
#define _DBGCMD_H

#include "aif.h"

enum dbgcmd {
    DBGCMD_INVOKE,
    DBGCMD_STARTUP,
    DBGCMD_SETLINEBREAK,
    DBGCMD_SETFUNCBREAK,
    DBGCMD_SETWATCH,
    DBGCMD_DELETEBREAK,
    DBGCMD_DISABLEBREAK,
    DBGCMD_ENABLEBREAK,
    DBGCMD_LASTBREAK,
    DBGCMD_LIST,
    DBGCMD_GETSOURCES,
    DBGCMD_GO,
    DBGCMD_FINISH,
    DBGCMD_SETARGS,
    DBGCMD_STEP,
    DBGCMD_KILL,
    DBGCMD_MOVEFRAME,
    DBGCMD_SHOWFRAME,
    DBGCMD_GETFRAME,
    DBGCMD_SETDEBUGGERVAR,
    DBGCMD_SETSOURCEPATH,
    DBGCMD_GETSOURCEPATH,
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
    DBGCMD_GETSCHEDULERLOCKING
    
};
typedef enum dbgcmd dbgcmd;

struct cmdargs_invoke {
    int numbe;
};
typedef struct cmdargs_invoke cmdargs_invoke;

struct cmdargs_setlinebreak {
    char *file;
    int lno;
};
typedef struct cmdargs_setlinebreak cmdargs_setlinebreak;

struct cmdargs_setfuncbreak {
    char *file;
    char *func;
};
typedef struct cmdargs_setfuncbreak cmdargs_setfuncbreak;

struct cmdargs_setwatch {
    int type;
    char *expr;
};
typedef struct cmdargs_setwatch cmdargs_setwatch;

struct cmdargs_deletebreak {
    int bpid;
};
typedef struct cmdargs_deletebreak cmdargs_deletebreak;

struct cmdargs_disablebreak {
    int bpid;
};
typedef struct cmdargs_disablebreak cmdargs_disablebreak;

struct cmdargs_enablebreak {
    int bpid;
    int once;
    int del;
};
typedef struct cmdargs_enablebreak cmdargs_enablebreak;

struct cmdargs_list {
    char *list_expr;
};
typedef struct cmdargs_list cmdargs_list;

struct cmdargs_setargs {
    char *args;
};
typedef struct cmdargs_setargs cmdargs_setargs;

struct cmdargs_step {
    int count;
    int into;
};
typedef struct cmdargs_step cmdargs_step;

struct cmdargs_moveframe {
    int count;
    int up_direction;
};
typedef struct cmdargs_moveframe cmdargs_moveframe;

struct cmdargs_showframe {
    int count;
};
typedef struct cmdargs_showframe cmdargs_showframe;

struct cmdargs_setdebuggervar {
    char *var;
    AIF *val;
};
typedef struct cmdargs_setdebuggervar cmdargs_setdebuggervar;

struct cmdargs_setsourcepath {
    char *path;
};
typedef struct cmdargs_setsourcepath cmdargs_setsourcepath;

struct cmdargs_getdebuggervar {
    char *var;
};
typedef struct cmdargs_getdebuggervar cmdargs_getdebuggervar;

struct cmdargs_setvar {
    char *var;
    AIF *val;
};
typedef struct cmdargs_setvar cmdargs_setvar;

struct cmdargs_evalexpr {
    long exprIdx;
    char *expr;
};
typedef struct cmdargs_evalexpr cmdargs_evalexpr;

struct cmdargs_gettype {
    char *sym;
    int isType;
};
typedef struct cmdargs_gettype cmdargs_gettype;

struct cmdargs_getvars {
    // TODO: I don't think this is used anymore
    char *file;
};
typedef struct cmdargs_getvars cmdargs_getvars;

struct cmdargs_getscope {
    char *location;
};
typedef struct cmdargs_getscope cmdargs_getscope;

struct cmdargs_direct {
    int cmd_argc;
    char **cmd_argv;
};
typedef struct cmdargs_direct cmdargs_direct;

struct cmdargs_status {
    int getLocation;
};
typedef struct cmdargs_status cmdargs_status;

struct cmdargs_string {
    const char* str;
};

struct cmdargs_quit {
    int detach;
};
typedef struct cmdargs_quit cmdargs_quit;

struct cmdargs_opensocket {
    char *opset_name;
    int opset_size;
};
typedef struct cmdargs_opensocket cmdargs_opensocket;

struct dbgcommand_t {
    dbgcmd command;

    // All command argument data structures go here
    // Note that some commands don't have any arguments, its okay to omit these
    // from the union.
    union {
        cmdargs_invoke *cmd_u_invoke;
        cmdargs_setlinebreak *cmd_u_setlinebreak;
        cmdargs_setfuncbreak *cmd_u_setfuncbreak;
        cmdargs_setwatch *cmd_u_setwatch;
        cmdargs_deletebreak *cmd_u_deletebreak;
        cmdargs_disablebreak *cmd_u_disablebreak;
        cmdargs_enablebreak *cmd_u_enablebreak;
        cmdargs_list *cmd_u_list;
        cmdargs_setargs *cmd_u_setargs;
        cmdargs_step *cmd_u_step;
        cmdargs_moveframe *cmd_u_moveframe;
        cmdargs_showframe *cmd_u_showframe;
        cmdargs_setdebuggervar *cmd_u_setdebuggervar;
        cmdargs_setsourcepath *cmd_u_setsourcepath;
        cmdargs_getdebuggervar *cmd_u_getdebuggervar;
        cmdargs_setvar *cmd_u_setvar;
        cmdargs_evalexpr *cmd_u_evalexpr;
        cmdargs_gettype *cmd_u_gettype;
        cmdargs_getvars *cmd_u_getvars;
        cmdargs_getscope *cmd_u_getscope;
        cmdargs_direct *cmd_u_direct;
        cmdargs_status *cmd_u_status;
        cmdargs_string * cmd_u_string;
        cmdargs_quit *cmd_u_quit;
        cmdargs_opensocket *cmd_u_opensocket;
    } cmd_u;
#define cmd_arg_invoke cmd_u.cmd_u_invoke
#define cmd_invoke_numbe cmd_u.cmd_u_invoke->numbe
#define cmd_arg_setlinebreak cmd_u.cmd_u_setlinebreak
#define cmd_setlinebreak_file cmd_u.cmd_u_setlinebreak->file
#define cmd_setlinebreak_lno cmd_u.cmd_u_setlinebreak->lno
#define cmd_arg_setfuncbreak cmd_u.cmd_u_setfuncbreak
#define cmd_setfuncbreak_file cmd_u.cmd_u_setfuncbreak->file
#define cmd_setfuncbreak_func cmd_u.cmd_u_setfuncbreak->func
#define cmd_arg_setwatch cmd_u.cmd_u_setwatch
#define cmd_setwatch_type cmd_u.cmd_u_setwatch->type
#define cmd_setwatch_expr cmd_u.cmd_u_setwatch->expr
#define cmd_arg_deletebreak cmd_u.cmd_u_deletebreak
#define cmd_deletebreak_bpid cmd_u.cmd_u_deletebreak->bpid
#define cmd_arg_disablebreak cmd_u.cmd_u_disablebreak
#define cmd_disablebreak_bpid cmd_u.cmd_u_disablebreak->bpid
#define cmd_arg_enablebreak cmd_u.cmd_u_enablebreak
#define cmd_enablebreak_bpid cmd_u.cmd_u_enablebreak->bpid
#define cmd_enablebreak_once cmd_u.cmd_u_enablebreak->once
#define cmd_enablebreak_del cmd_u.cmd_u_enablebreak->del
#define cmd_arg_list cmd_u.cmd_u_list
#define cmd_list_expr cmd_u.cmd_u_list->list_expr
#define cmd_arg_setargs cmd_u.cmd_u_setargs
#define cmd_setargs_args cmd_u.cmd_u_setargs->args
#define cmd_arg_step cmd_u.cmd_u_step
#define cmd_step_count cmd_u.cmd_u_step->count
#define cmd_step_into cmd_u.cmd_u_step->into
#define cmd_arg_moveframe cmd_u.cmd_u_moveframe
#define cmd_moveframe_count cmd_u.cmd_u_moveframe->count
#define cmd_moveframe_direction cmd_u.cmd_u_moveframe->up_direction
#define cmd_arg_showframe cmd_u.cmd_u_showframe
#define cmd_showframe_count cmd_u.cmd_u_showframe->count
#define cmd_arg_setdebuggervar cmd_u.cmd_u_setdebuggervar
#define cmd_setdebuggervar_var cmd_u.cmd_u_setdebuggervar->var
#define cmd_setdebuggervar_val cmd_u.cmd_u_setdebuggervar->val
#define cmd_arg_setsourcepath cmd_u.cmd_u_setsourcepath
#define cmd_setsourcepath_path cmd_u.cmd_u_setsourcepath->path
#define cmd_arg_getdebuggervar cmd_u.cmd_u_getdebuggervar
#define cmd_getdebuggervar_var cmd_u.cmd_u_getdebuggervar->var
#define cmd_arg_setvar cmd_u.cmd_u_setvar
#define cmd_setvar_var cmd_u.cmd_u_setvar->var
#define cmd_setvar_val cmd_u.cmd_u_setvar->val
#define cmd_arg_evalexpr cmd_u.cmd_u_evalexpr
#define cmd_evalexpr_expr cmd_u.cmd_u_evalexpr->expr
#define cmd_arg_gettype cmd_u.cmd_u_gettype
#define cmd_gettype_sym cmd_u.cmd_u_gettype->sym
#define cmd_gettype_isType cmd_u.cmd_u_gettype->isType
#define cmd_arg_getvars cmd_u.cmd_u_getvars
#define cmd_getvars_file cmd_u.cmd_u_getvars->file
#define cmd_arg_getscope cmd_u.cmd_u_getscope
#define cmd_getscope_location cmd_u.cmd_u_getscope->location
#define cmd_arg_direct cmd_u.cmd_u_direct
#define cmd_direct_argc cmd_u.cmd_u_direct->cmd_argc
#define cmd_direct_argv cmd_u.cmd_u_direct->cmd_argv
#define cmd_arg_status cmd_u.cmd_u_status
#define cmd_status_getLocation cmd_u.cmd_u_status->getLocation
#define cmd_arg_quit cmd_u.cmd_u_quit
#define cmd_quit_detach cmd_u.cmd_u_quit->detach
#define cmd_arg_opensocket cmd_u.cmd_u_opensocket
#define cmd_opensocket_name cmd_u.cmd_u_opensocket->opset_name
#define cmd_opensocket_size cmd_u.cmd_u_opensocket->opset_size
};
typedef struct dbgcommand_t dbgcommand_t;

extern dbgcommand_t *NewCommand(dbgcmd);
extern void FreeCommand(dbgcommand_t *);

#endif /* !_DBGCMD_H */
