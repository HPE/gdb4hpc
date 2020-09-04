/******************************************************************************\
 * Command routine defines.
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * Copyright 2010-2013,2020 Hewlett Packard Enterprise Development LP.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Hewlett Packard Enterprise Development LP.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *
 * $HeadURL$
 * $Date$
 * $Rev$
 * $Author$
 *
 ******************************************************************************/

#ifndef _CMD_H
#define _CMD_H

// We must first pull in tye cmd_types.h which declares types used by
// all command definitions.
#include "type_cmd.h"

// Even though this header doesn't use stuff in these cmd headers, lets
// include them to make cmd.h be the master interface to command defines.
#include "class_cmd.h"
#include "help_dict.h"

// Declarations from these headers are actually used here.
#include "client.h"
#include "gram.h"
#include "list.h"

/*
** Global Init command functions defined elsewhere
*/
extern void InitCmd_assign(void);
extern void InitCmd_break(void);
extern void InitCmd_comp(void);
extern void InitCmd_env(void);
extern void InitCmd_frame(void);
extern void InitCmd_func(void);
extern void InitCmd_go(void);
extern void InitCmd_list(void);
extern void InitCmd_passthru(void);
extern void InitCmd_print(void);
extern void InitCmd_proc(void);
extern void InitCmd_procset(void);
extern void InitCmd_source(void);
extern void InitCmd_thread();
extern void InitCmd_step(void);
extern void InitCmd_whatis(void);
extern void InitBuiltinFuncs(void);

/*
 * External command init funcs
 */
extern void InitCommands();
extern void DestroyCommands(void);

/*
** Function prototypes
*/

extern cmdlist_t *NewCommandList(void (*)(const char *), void (*)(const char *),
                                 void (*)(void));
extern void AddCommand(const char *, const char *, cmdptype_t, int, cmdfunc_t,
                       const char *const[], const char *);
extern void AmbiguousAltCmd(cmdlist_t *, const char *);
extern void CmdNotFound(const char *);
extern void DestroyCommandList(cmdlist_t *);
extern void AddAltCommand(cmdlist_t *, const char *, const char *, cmdptype_t,
                          int, cmdfunc_t, const char *const[], const char *);
extern cmdlist_t *SetCommands(cmdlist_t *);
extern void SetDefaultCommand(cmdlist_t *, const char *);
extern cmdtype_t *FindCommand(const char *);
extern cmdtype_t *FindSubCommand(cmdlist_t *, const char *);
extern void ExecuteCommand(const char *);

// The following prototypes are used for adding Class commands (i.e. help, set,
// show, etc)
extern void AddClassCommand(const char *, const char *, cmdptype_t, int,
                            cmdfunc_t, cmdtype_t *(*)(char *),
                            const char *const[], const char *);
extern void AddClassSubCommand(list_t *, const char *, cmdptype_t, int,
                               cmdfunc_t, const char *const[], const char *);
extern int DoClassSubCommand(cmdlist_t *, dbgprocset_t *, cmdtype_t *, int,
                             char *[], int);
extern void AmbiguousClassSubCmd(list_t *, const char *);

// external access to the help command
extern void PrintHelpEntry(char **);
extern int cmd_help_cmd(rec_set_t *, int, char **);

#endif /* ! _CMD_H */
