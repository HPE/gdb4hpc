/*********************************************************************************\
 * Function routines.
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

#ifndef _FUNC_H
#define _FUNC_H

#include "hash.h"
#include "list.h"
#include "val.h"

#define ARG_SYM 1
#define ARG_VAL 2
#define ARG_INT 3
#define ARG_GRAPH 4

struct arg_io {
    int io_inp;
    int io_out;
    list_t *io_vlist;
};

struct arg_t {
    int a_type;
    union {
        hentry_t *a_u_sym;
        val_t *a_u_val;
        int a_u_int;
        struct arg_io a_u_io;
    } a_u;
#define a_sym a_u.a_u_sym
#define a_val a_u.a_u_val
#define a_int a_u.a_u_int
#define a_inp a_u.a_u_io.io_inp
#define a_out a_u.a_u_io.io_out
#define a_vlist a_u.a_u_io.io_vlist
};
typedef struct arg_t arg_t;

struct func_t {
    hentry_t *f_sym;
    list_t *f_args;
    list_t *f_actual;
    struct expr_t *f_def;
    char *f_strdef;
    int (*f_builtin)(list_t *, val_t **);
};
typedef struct func_t func_t;

extern int FuncDef;
extern list_t *DbgFuncs;
#endif /* !_FUNC_H */
