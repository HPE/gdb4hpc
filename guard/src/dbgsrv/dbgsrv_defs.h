/******************************************************************************\
 * dbgsrv_defs.h - Common defines for the dbgsrv layer
 *
 * Copyright 2012-2013,2015,2020 Cray Inc.
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
 ******************************************************************************/

#ifndef _DBGSRV_DEFS_H
#define _DBGSRV_DEFS_H

extern int Shutdown;
extern int exit_sleep_time;
extern int my_rank;
extern int sock_fd;
extern int gpu_flag;
class GdbVersion;
extern GdbVersion __gdb_vers;
extern int early_entry_flag;
extern int debug_flag;

extern void _dvprint(const char *format, ...);
extern void ResetWDT(void);

#define DVPRINT(...) \
    if (debug_flag) _dvprint(__VA_ARGS__);

#endif /* _DBGSRV_DEFS_H */
