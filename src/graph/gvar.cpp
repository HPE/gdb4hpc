/*********************************************************************************\
 * Guard Compiler
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
 **********************************************************************************/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include "gvar.h"

#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "hash.h"
#include "expr.h"

#define MAX_GRAPHVARS 7 /* 2^7 = 128 */

static htable_t *GraphVarTab = (htable_t *)NULL;

void InitGraphVars(void) {
    double ulp;
    int zero, one;

    if (GraphVarTab != (htable_t *)NULL) return;

    ulp = nextafter(0.0,
                    0.1);  // get the unit of least precision for this machine
    zero = 0;
    one = 1;

    GraphVarTab = HashCreate(MAX_GRAPHVARS);

    SetGraphVar(GRAPH_LOWER, VAR_DOUBLE, (void *)&ulp);
    SetGraphVar(GRAPH_UPPER, VAR_DOUBLE, (void *)&ulp);
    SetGraphVar(GRAPH_RELATIVE, VAR_INT, (void *)&zero);
    SetGraphVar(GRAPH_HALT, VAR_INT, (void *)&one);
    SetGraphVar(GRAPH_STOP, VAR_INT, (void *)&one);
    SetGraphVar(GRAPH_DFBLOCK, VAR_INT, (void *)&zero);
}

// Removing va_args until this function is actually called with
// variable arguments
void SetGraphVar(const char *var, int type, void *val) {
    graphvar_t *gv;
    hentry_t *h = (hentry_t *)NULL;

    InitGraphVars();

    if ((h = HashSearch(GraphVarTab, var)) == (hentry_t *)NULL)
        gv = (graphvar_t *)malloc(sizeof(graphvar_t));
    else
        gv = (graphvar_t *)h->h_data;

    switch (type) {
        case VAR_DOUBLE:
            gv->gv_dval = *(double *)(val);
            break;

        case VAR_INT:
            gv->gv_ival = *(int *)(val);
            break;

        case VAR_STRING:
            if (h != (hentry_t *)NULL && gv->gv_type == VAR_STRING)
                free(gv->gv_sval);

            gv->gv_sval = strdup((char *)val);
            break;
    }

    gv->gv_type = type;

    if (h == (hentry_t *)NULL) HashInsert(GraphVarTab, var, (void *)gv);
}

// andrewg added on 8/10/10
// If you are just going to do a memcpy on the guy you pass in,
// you shouldn't use varargs.
void GetGraphVar(const char *var, void *val) {
    int len = 0;
    graphvar_t *gv;
    hentry_t *h;

    InitGraphVars();

    if ((h = HashSearch(GraphVarTab, var)) == (hentry_t *)NULL) return;

    gv = (graphvar_t *)h->h_data;

    switch (gv->gv_type) {
        case VAR_DOUBLE:
            len = sizeof(double);
            break;

        case VAR_INT:
            len = sizeof(int);
            break;

        case VAR_STRING:
            len = sizeof(char *);
            break;

        default:
            break;
    }

    if (val == (void *)NULL) return;

    memcpy(val, (void *)&gv->gv_u, len);
}

void _destroy_gvar(char *var, void *v) {
    graphvar_t *gv = (graphvar_t *)v;

    if (gv == (graphvar_t *)NULL) return;

    if (gv->gv_type == VAR_STRING) free(gv->gv_sval);

    free(v);
}

void FinishGraphVars(void) { HashDestroy(GraphVarTab, _destroy_gvar); }
