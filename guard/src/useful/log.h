/*********************************************************************************\
 * log.h - Header file for the log interface.
 *
 * Copyright 2011-2020 Cray Inc.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Cray Inc.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *
 *********************************************************************************/

#ifndef _LOG_H
#define _LOG_H

#include <stdio.h>

#define DBG_LOG_ENV_VAR "CRAY_DBG_LOG_DIR"

#ifdef __cplusplus
extern "C" {
#endif

FILE *create_log(int, const char *);
FILE *open_log(int, const char *);
int write_log(FILE *, const char *);
int write_log2(FILE *, const char *);
int close_log(FILE *);
int hook_stdoe(FILE *);
void print_backtrace(FILE *log, const char *message);

#ifdef __cplusplus
}
#endif

#endif /* _LOG_H */
