/*********************************************************************************\
 * HDF handling routines
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * © 2010-2013 Cray Inc.  All Rights Reserved.
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
 **********************************************************************************/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#ifdef HDF

#include <mfhdf.h>

#include "aif.h"
#include "client.h"
#include "defs.h"
#include "useful.h"

void HDFDump(char *file, char *var, AIF *a) {
    int i;
    int32 stat;
    int32 rank;
    int32 type;
    int32 *sizes;
    int32 *start;
    int32 *edges;
    int *min;
    int *max;
    char *btype;
    hentry_t *h;
    static int32 sd_id;
    static int32 sds_id;
    static htable_t *ntab = (htable_t *)NULL;

    if (AIFType(a) != AIF_ARRAY) {
        PrintOutError("HDFDump: Data type must be an array\n");
        return;
    }

    if (ntab == (htable_t *)NULL) ntab = HashCreate(7);

    FDSArrayInfo(AIF_FORMAT(a), &rank, &btype, NULL);

    switch (FDSType(btype)) {
        case AIF_INTEGER:
            type = (FDSTypeSize(btype) > 4) ? DFNT_INT64 : DFNT_INT32;
            break;

        case AIF_FLOATING:
            type = (FDSTypeSize(btype) > 4) ? DFNT_FLOAT64 : DFNT_FLOAT32;
            break;
    }

    FDSArrayBounds(AIF_FORMAT(a), rank, &min, &max, &sizes);

    start = (int32 *)Alloc(sizeof(int32) * rank);
    edges = (int32 *)Alloc(sizeof(int32) * rank);

    for (i = 0; i < rank; i++) {
        start[i] = 0;
        edges[i] = sizes[i];
    }

    if (HashSearch(ntab, file) == (hentry_t *)NULL) {
        sd_id = SDstart(file, DFACC_CREATE);
        HashInsert(ntab, file, (void *)NULL);
    } else
        sd_id = SDstart(file, DFACC_RDWR);

    sds_id = SDcreate(sd_id, var, type, rank, sizes);

    if (SDwritedata(sds_id, start, NULL, edges, (VOIDP)AIF_DATA(a)) >= FAIL)
        HEprint(Client.cli_out_stream, 1);

    SDendaccess(sds_id);

    SDend(sd_id);

    Free(min);
    Free(max);
    Free(sizes);
    Free(start);
    Free(edges);
}

#endif /* HDF */
