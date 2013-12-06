/*
** Argument definitions.
**
** Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
**
*/

#ifndef _USEFUL_ARGS_H
#define _USEFUL_ARGS_H

extern char **	Str2Argv(char *);
extern char **	NewArgv(char *, ...);
extern char **	CopyArgv(char **);
extern char **	AppendArgv(char **, char **);
extern char **	AppendArgs(char **, ...);
extern char *	ArgsToBuf(char **, int);
extern void	FreeArgv(char **);

#endif /* !_USEFUL_ARGS_H */
