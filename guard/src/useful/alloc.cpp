/*********************************************************************************\
 * Memory routines
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

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "alloc.h"
#include "error.h"

void *Alloc(int size) {
    void *m = malloc(size);

    if (m == (void *)NULL) {
        SetError("Memory allocation failed");
        abort();
    }

    return m;
}

void Asprintf(char **strp, const char *fmt, ...) {
    va_list ap;

    va_start(ap, fmt);

    if (vasprintf(strp, fmt, ap) == -1) {
        SetError("Memory allocation failed");
        abort();
    }

    va_end(ap);
}

void VAsprintf(char **strp, const char *fmt, va_list ap) {
    if (vasprintf(strp, fmt, ap) == -1) {
        SetError("Memory allocation failed");
        abort();
    }
}

void *Resize(void *m, int s) {
    if (m == (void *)NULL) return Alloc(s);

    m = realloc(m, s);

    if (m == (void *)NULL) {
        SetError("Memory allocation failed");
        abort();
    }

    return m;
}

void Free(void *m) {
    if (m == (void *)NULL) {
        if (Argv0 != NULL)
            SetError("%s: Warning: freeing NULL pointer.", Argv0);
        else
            SetError("Warning: freeing NULL pointer.");
        return;
    }

    free(m);
}
