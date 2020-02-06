/*********************************************************************************\
 * Line number routines.
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
 *********************************************************************************/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <stdio.h>
#include <string.h>

#include "defs.h"
#include "gram.h"
#include "useful.h"

line_t *FindLine(list_t *, int);

line_t *NewLine(int lno) {
    line_t *l;

    l = (line_t *)Alloc(sizeof(line_t));

    l->l_lno = lno;
    l->l_bp = -1;
    l->l_proc = -1;
    l->l_sync = -1;
    l->l_val = -1;
    l->l_ref = 0;
    l->l_bs = NULL;

    return l;
}

line_t *AddLine(list_t **lp, int lno) {
    line_t *l;

    if (*lp == (list_t *)NULL) *lp = NewList();

    if ((l = FindLine(*lp, lno)) == (line_t *)NULL) {
        l = NewLine(lno);
        AddToList(*lp, (void *)l);
    }

    l->l_ref++;

    return l;
}

line_t *FindLine(list_t *lp, int lno) {
    line_t *l;

    SetList(lp);

    while ((l = (line_t *)GetListElement(lp)) != (line_t *)NULL)
        if (l->l_lno == lno) return l;

    return (line_t *)NULL;
}

void DestroyLine(line_t *l) {
    l->l_ref--;
    if (l->l_ref >= 0) return;

    if (l->l_bs != NULL) bitset_free(l->l_bs);

    Free(l);
}
