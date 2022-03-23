/******************************************************************************\
 * dbgsrv_defs.h - Common defines for the dbgsrv layer
 *
 * Copyright 2012-2013,2015,2020 Hewlett Packard Enterprise Development LP.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Hewlett Packard Enterprise Development LP.,
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
extern int my_rank;
extern int sock_fd;
extern int gpu_flag;
extern int early_entry_flag;
extern int debug_flag;
extern int non_mpi_flag;
extern int non_stop_flag;
extern const char* custom_gdb;
extern const char* custom_gdb_data;

extern void _dvprint(const char *format, ...) __attribute__((format (printf,1,2)));
extern void ResetWDT(void);

#define DVPRINT(...) \
    (debug_flag ? _dvprint(__VA_ARGS__) : (void)0)

#endif /* _DBGSRV_DEFS_H */
