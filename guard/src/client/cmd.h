/*
** Command routine defines.
**
** Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
**
*/

#ifndef _CMD_H
#define _CMD_H

// We must first pull in tye cmd_types.h which declares types used by
// all command definitions.
#include	"type_cmd.h"

// Even though this header doesn't use stuff in these cmd headers, lets
// include them to make cmd.h be the master interface to command defines.
#include	"help_dict.h"
#include	"class_cmd.h"

// Declarations from these headers are actually used here.
#include	"gram.h"
#include	"list.h"
#include	"client.h"

/*
** Global Init command functions defined elsewhere
*/
extern void	InitCmd_assign(void);
extern void	InitCmd_break(void);
extern void	InitCmd_comp(void);
extern void	InitCmd_env(void);
extern void	InitCmd_frame(void);
extern void	InitCmd_func(void);
extern void	InitCmd_go(void);
extern void	InitCmd_list(void);
extern void	InitCmd_passthru(void);
extern void	InitCmd_print(void);
extern void	InitCmd_proc(void);
extern void	InitCmd_procset(void);
extern void	InitCmd_source(void);
extern void	InitCmd_step(void);
extern void	InitCmd_whatis(void);
extern void	InitBuiltinFuncs(void);

/*
 * External command init funcs
 */
extern void			InitCommands();
extern void			DestroyCommands(void);

/*
** Function prototypes
*/
extern cmdlist_t *	NewCommandList(void (*)(char *), void (*)(char *), void (*)(void));
extern void			AddCommand(char *, char *, cmdptype_t, int, int (*)(), const char *[], char *);
extern void			AmbiguousAltCmd(cmdlist_t *, char *);
extern void			CmdNotFound(char *);
extern void			DestroyCommandList(cmdlist_t *);
extern void			AddAltCommand(cmdlist_t *, char *, char *, cmdptype_t , int , int (*)(), const char *[], char *);
extern cmdlist_t *	SetCommands(cmdlist_t *);
extern void			SetDefaultCommand(cmdlist_t *, char *);
extern cmdtype_t *	FindCommand(char *);
extern cmdtype_t *	FindSubCommand(cmdlist_t *, char *);
extern void			ExecuteCommand(char *);

// The following prototypes are used for adding Class commands (i.e. help, set, show, etc)
extern void			AddClassCommand(char *, char *, cmdptype_t, int, int (*)(), cmdtype_t *(*)(char *), const char *[], char *);
extern void			AddClassSubCommand(list_t *, char *, cmdptype_t, int, int (*)(), const char *[], char *);
extern int			DoClassSubCommand(cmdlist_t *, dbgprocset_t *, cmdtype_t *, int, char *[], int);
extern void			AmbiguousClassSubCmd(list_t *, char *);

// external access to the help command
extern void			PrintHelpEntry(char **);
extern int			cmd_help_cmd(rec_set_t *, int, char **);

#endif /* ! _CMD_H */
