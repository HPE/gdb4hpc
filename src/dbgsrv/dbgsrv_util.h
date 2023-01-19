/*********************************************************************************\
 * dbgserv_util.h
 *
 * Copyright 2011-2013,2020-2022 Hewlett Packard Enterprise Development LP.
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
 *********************************************************************************/

#ifndef _DBGSRV_UTIL_H
#define _DBGSRV_UTIL_H

#include "useful.h"

typedef struct blockmap_info {
    int rank;
    int num_procs;
    int m_distribute[10];
    int m_blockcyclic_val[10];
    int m_data[10];
    int m_procs[10];
    int m_dim_order[10];
    int order_map[10];
    char isym[BUFSIZ];
    double a_lower;
    double a_upper;
} blockmap_info;

extern char *PerformSlicingErr;

void ParseDBGExpr(const char *, char *, char *, char *);

// Get the size of the virtual data segment of the current process
// in megabytes.
unsigned long GetProcSize();


#endif /* !_DBGSRV_UTIL_H */
