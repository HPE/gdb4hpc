/*********************************************************************************\
 * Error definitions.
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * © 2010-2019 Cray Inc.  All Rights Reserved.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Cray Inc.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *
 *********************************************************************************/

#ifndef _USEFUL_ERROR_H
#define _USEFUL_ERROR_H

#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

extern char *Argv0;
extern char *GetError();
extern void SetError(const char *, ...);
extern void SetSysError(const char *, ...);

#ifdef __cplusplus
}
#endif

#endif /* !_USEFUL_ERROR_H */
