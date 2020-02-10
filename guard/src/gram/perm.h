/*********************************************************************************\
 * Permute defines.
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

#ifndef _PERM_H
#define _PERM_H

#include "list.h"

struct perm_t {
    int p_from;
    int p_to;
};
typedef struct perm_t perm_t;

void AddPerm(list_t **, int, int);
void DestroyPerm(list_t *);
void SetPerm(list_t *);
perm_t *GetPerm(list_t *);

#endif /* !_PERM_H */
