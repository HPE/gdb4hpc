/*********************************************************************************\
 * Error handling routines
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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defs.h"
#include "error.h"

// external error printing function
// extern void	PrintOutError(char *, ...);
extern int errno;

char *Argv0 = NULL;

static char _lasterr[BUFSIZ];

char *GetError() { return _lasterr; }

void SetError(const char *fmt, ...) {
    va_list args;

    va_start(args, fmt);

    vsnprintf(_lasterr, sizeof(_lasterr), fmt, args);

    va_end(args);
}

void SetSysError(const char *fmt, ...) {
    va_list args;
    char buf[BUFSIZ];

    va_start(args, fmt);

    vsnprintf(buf, sizeof(buf), fmt, args);
    const auto n = sizeof(_lasterr);
    if (snprintf(_lasterr, n, "%s: %s", buf, strerror(errno)) >= int(n)) { /* ignore truncation */ }
    va_end(args);
}
