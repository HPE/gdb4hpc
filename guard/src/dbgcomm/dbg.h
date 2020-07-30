/*********************************************************************************\
 * Debugger interface definitions.
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * Copyright 2010-2013,2020 Cray Inc.
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

#ifndef _DBG_H
#define _DBG_H

#include "dbgcmd.h"
#include "dbgevent.h"
#include "dbgformat.h"
#include "defs.h"

/*
**Output routines defined by client/server interface
*/
extern void PrintOutError(const char *, ...) __attribute__((format (printf,1,2)));
extern void PrintOutMsg(const char *, ...) __attribute__((format (printf,1,2)));

#endif /* _DBG_H */
