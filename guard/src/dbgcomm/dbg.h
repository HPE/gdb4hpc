/*
** Debugger interface definitions.
**
** Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
**
*/

#ifndef _DBG_H
#define _DBG_H

#include "defs.h"
#include "dbgcmd.h"
#include "dbgevent.h"
#include "dbgformat.h"

/* 
**Output routines defined by client/server interface
*/
extern void	PrintOutError(char *, ...);
extern void	PrintOutMsg(char *, ...);

#endif /* _DBG_H */
