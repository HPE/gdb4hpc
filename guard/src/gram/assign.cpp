/*********************************************************************************\
 * Support for graph assigns.
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * © 2010-2013,2020 Cray Inc.
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

list_t *_assign_list = (list_t *)NULL;

assign_t *AddAssign(list_t *l, char *lval, char *rval, var_t *v) {
    assign_t *a;

    a = (assign_t *)Alloc(sizeof(assign_t));

    a->a_lstr = strdup(lval);
    a->a_rstr = strdup(rval);
    a->a_rvar = v;

    AddToList(l, (void *)a);

    return a;
}

void DestroyAssign(assign_t *a) {
    Free(a->a_lstr);
    Free(a->a_rstr);
    Free(a);
}
