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

#ifndef _ASSIGN_H
#define _ASSIGN_H

#include "list.h"
#include "var.h"

struct assign_t {
    char *a_lstr;
    char *a_rstr;
    var_t *a_rvar;
};
typedef struct assign_t assign_t;

extern assign_t *AddAssign(list_t *, char *, char *, var_t *);
extern void DestroyAssign(assign_t *);
#endif /* !_ASSIGN_H */
