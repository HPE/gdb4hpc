/*********************************************************************************\
 * Graph variable support.
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
 **********************************************************************************/

#ifndef _GVAR_H
#define _GVAR_H

#define VAR_INT 1
#define VAR_DOUBLE 2
#define VAR_STRING 3

#define GRAPH_LOWER "Lower Limit"
#define GRAPH_UPPER "Upper Limit"
#define GRAPH_RELATIVE "Relative"
#define GRAPH_FORCE "Force"
#define GRAPH_HALT "Halt"
#define GRAPH_STOP "Stop"
#define GRAPH_DISPLAY "Display"
#define GRAPH_OUTPUT "Output"
#define GRAPH_LOGGING "Logging"
#define GRAPH_LOGFILE "Log File"
#define GRAPH_DFBLOCK "DF Block"
#define GRAPH_DFMODE "DF Mode"
#define GRAPH_REDUCE "Reduce"
#define GRAPH_HASH_LOWER "Lower hashing precision"
#define GRAPH_HASH_UPPER "Upper hashing precision"
#define GRAPH_HISTOGRAM_NUMBINS "Histogram number of bins"
#define GRAPH_HISTOGRAM_START "Histogram domain start"
#define GRAPH_HISTOGRAM_END "Histogram domain end"
#define GRAPH_LOWER_BOUND "Element counting lower bound"
#define GRAPH_UPPER_BOUND "Element counting upper bound"
#define GRAPH_P2PMODE "P2P Comparison"

enum dfmode { DF_DEFAULT, DF_SYNC, DF_ASYNC };
typedef enum dfmode dfmode_t;

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
