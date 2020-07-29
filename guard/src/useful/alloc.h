/*********************************************************************************\
 * Memory definitions
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * Copyright 2010-2020 Cray Inc.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Cray Inc.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *
 *********************************************************************************/

#ifndef _USEFUL_ALLOC_H
#define _USEFUL_ALLOC_H

#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

extern char *Argv0;

extern void *Alloc(int size);
extern void Asprintf(char **strp, const char *fmt, ...) __attribute__((format (printf,2,3)));
extern void VAsprintf(char **strp, const char *fmt, va_list ap);
extern void *Resize(void *, int);
extern void Free(void *);

#ifdef __cplusplus
}
#endif

#endif /* !_USEFUL_ALLOC_H */
