/*
** Memory definitions
**
** Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
**
*/

#ifndef _USEFUL_ALLOC_H
#define _USEFUL_ALLOC_H

#include <stdarg.h>

extern char *	Argv0;

extern void *	Alloc(int size);
extern void		Asprintf(char **strp, const char *fmt, ...);
extern void		VAsprintf(char **strp, const char *fmt, va_list ap);
extern void *	Resize(void *, int);
extern void		Free(void *);

#endif /* !_USEFUL_ALLOC_H */
