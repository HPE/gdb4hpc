/*
** Error definitions.
**
** Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
**
*/

#ifndef _USEFUL_ERROR_H
#define _USEFUL_ERROR_H

#include	<stdarg.h>

extern char *	Argv0;
extern char *	GetError();
extern void	SetError(char *, ...);
extern void	SetSysError(char *, ...);

#endif /* !_USEFUL_ERROR_H */
