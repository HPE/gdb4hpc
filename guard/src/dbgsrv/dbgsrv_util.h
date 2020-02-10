/*********************************************************************************\
 * dbgserv_util.h
 *
 * © 2011-2013,2020 Cray Inc.
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

#ifndef _DBGSRV_UTIL_H
#define _DBGSRV_UTIL_H

#include "aif.h"
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

void ParseDBGExpr(const char *, int *, char *, char *, char *);
void ParseHashArgs(char *, int *, int *, int *, char *);
void ParseCountelemArgs(char *, double *, double *);
void ParseHistogramArgs(char *, int *, double *, double *);
void ParseBlockMapArgs(char *, blockmap_info *);
void ParseP2PArgs(char *, char *, int *, blockmap_info *, blockmap_info *);

AIF *PerformSlicing(AIF *, char *, int);
AIF *PerformHash(AIF *, char *, int, int, int);
AIF *PerformHashMI(char *, char *, arrayDimension_t *, char *, int, int, int);

void SliceSocketSend(int, AIF *);
AIF *SliceSocketReceive(void);
AIF *PerformP2P(AIF *, char *, int, int *);

#endif /* !_DBGSRV_UTIL_H */
