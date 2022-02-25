/*********************************************************************************\
 * Graph variable support.
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

#ifndef _GVAR_H
#define _GVAR_H

#define VAR_INT 1
#define VAR_DOUBLE 2
#define VAR_STRING 3

#define GRAPH_LOWER "Lower Limit"
#define GRAPH_UPPER "Upper Limit"
#define GRAPH_RELATIVE "Relative"
#define GRAPH_HALT "Halt"
#define GRAPH_STOP "Stop"
#define GRAPH_DFBLOCK "DF Block"

struct graphvar {
    int gv_type;

    union {
        double gv_u_dval;
        int gv_u_ival;
        char *gv_u_sval;
    } gv_u;
#define gv_dval gv_u.gv_u_dval
#define gv_ival gv_u.gv_u_ival
#define gv_sval gv_u.gv_u_sval
};
typedef struct graphvar graphvar_t;

extern void InitGraphVars(void);
extern void FinishGraphVars(void);
extern void SetGraphVar(const char *, int, void *);
extern void GetGraphVar(const char *, void *);

#endif /* !_GVAR_H */
