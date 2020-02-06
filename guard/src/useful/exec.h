/*********************************************************************************\
 * Exec support defs
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

#ifndef _USEFUL_EXEC_H
#define _USEFUL_EXEC_H

#ifdef __cplusplus
extern "C" {
#endif

extern int CheckStatus(int);
extern void Exec(char *, char **, char **);
extern int ForkAndExec(char *, char **, char **);
extern void Reap(int);

#ifdef __cplusplus
}
#endif

#endif /* !_USEFUL_EXEC_H */
