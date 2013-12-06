/*
** Exec support defs
**
** Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
**
*/

#ifndef _USEFUL_EXEC_H
#define _USEFUL_EXEC_H

extern int      CheckStatus(int);
extern void	Exec(char *, char **, char **);
extern int	ForkAndExec(char *, char **, char **);
extern void	Reap(int);

#endif /* !_USEFUL_EXEC_H */
