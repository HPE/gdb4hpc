/*********************************************************************************\
 * Variable defines.
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

#ifndef _VAR_H
#define _VAR_H

#include "line.h"
#include "list.h"
#include "slice.h"

struct var_t {
    struct dbgprocset_t *v_procset; /* process number */
    int v_indiv;      /*extention for individual process TODO: Remove*/
    char *v_var;      /* name of var */
    char *v_fname;    /* file name */
    int v_lno;        /* line number */
    slice_t *v_slice; /* array slice (optional) */
    list_t *v_perm;   /* array permute (optional) */
    int v_internal;   /* var is an internal variable */
    /*
    ** v_line[0] is used to store breakpoint information for the var
    ** when building graph. If v_proc == NULL,
    ** then this var represents a collection of processes and v_line
    ** holds breakpoint information for each process.
    */
    line_t *v_line; /* break point line number */

#define v_pname v_procset->s_name /* procset name */
#define v_bp v_line->l_bp         /* break point id */
#define v_pid v_line->l_proc      /* proc once bp reached */
#define v_sync v_line->l_sync     /* rendezvous for procs */
#define v_value v_line->l_val     /* rendezvous for procs */
#define v_bs v_line->l_bs         /* rendezvous for procs */
};
typedef struct var_t var_t;

var_t *NewVar(void);
void DestroyVar(var_t *);

#endif /* !_VAR_H */
