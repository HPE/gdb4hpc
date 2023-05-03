/*********************************************************************************\
 * Variable defines.
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

#ifndef _VAR_H
#define _VAR_H

#include "slice.h"

struct var_t {
    struct dbgprocset_t *v_procset; /* process number */
    int v_indiv;      /*extention for individual process TODO: Remove*/
    char *v_var;      /* name of var */
    char *v_fname;    /* file name */
    int v_lno;        /* line number */
    slice_t *v_slice; /* array slice (optional) */
    int v_internal;   /* var is an internal variable */

#define v_pname v_procset->s_name /* procset name */
};
typedef struct var_t var_t;

var_t *NewVar(void);
void DestroyVar(var_t *);

#endif /* !_VAR_H */
