/*********************************************************************************\
 * Permute support.
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

void AddPerm(list_t **lp, int f, int t) {
    perm_t *p;

    if (*lp == (list_t *)NULL) *lp = NewList();

    p = (perm_t *)Alloc(sizeof(perm_t));

    p->p_from = f;
    p->p_to = t;

    AddToList(*lp, (void *)p);
}

void DestroyPerm(list_t *lp) { DestroyList(lp, Free); }

void SetPerm(list_t *lp) { SetList(lp); }

perm_t *GetPerm(list_t *lp) { return (perm_t *)GetListElement(lp); }
