/*********************************************************************************\
 * Block Map routines.
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * Copyright 2010-2013,2020 Hewlett Packard Enterprise Development LP.
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

#ifndef _GBLOCKMAP_H
#define _GBLOCKMAP_H

#include "aif.h"
#include "dbg_client.h"
#include "hash.h"
#include "list.h"
#include "procset.h"
#include "var.h"

struct g_block_map_t {
    hentry_t *m_sym;
    var_t *m_actual;
    int m_debug;
    int *m_distribute;       // store the mode of the block, cyclic
    int *m_data;             // store the upperbound of the dimensions
    int *m_procs;            // store the process grid
    int *m_blockcyclic_val;  // store block-cyclic value
    int *m_dim_order;        // store dimension order of mapping
    int m_rank;              // store the rank of the blockmap function
    int (*m_builtin)(list_t *);

    /*
    ** Fields filled in when map is being evaluated.
    */
    int m_current;
    AIFIndex *m_ix;
};
typedef struct g_block_map_t g_block_map_t;

extern void SetBlockMap(void);
extern g_block_map_t *GetBlockMap(void);
extern g_block_map_t *FindBlockMap(char *);
extern int CheckBlockMap(hentry_t *);
extern int VerifyBlockMap(hentry_t *);
extern g_block_map_t *NewBlockMap(hentry_t *);
extern void DestroyBlockMap(g_block_map_t *);
extern AIF *ApplyBlockMap(g_block_map_t *, char *, int, AIF **, const char *);
extern void PrintBlockMap(FILE *, g_block_map_t *);
extern void FinishBlockMap(void);
extern void BlockMapToString(g_block_map_t *, var_t *, int, char *);
extern AIF *construct_blockmap_data(g_block_map_t *bm, dbgprocset_t *,
                                    int array_size, rep_set_t *replies);

#endif /* !_GBLOCKMAP_H */
