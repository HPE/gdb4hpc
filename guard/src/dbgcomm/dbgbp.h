/*********************************************************************************\
 * Breapoint routine definitions
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * Copyright 2010-2013,2020 Cray Inc.
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

#ifndef _DBGBP_H
#define _DBGBP_H

#include "dbgloc.h"
#include "list.h"

enum dbgbp_type {
    DBGBP_BREAK_TYPE,
    DBGBP_WATCH_TYPE,
    DBGBP_AWATCH_TYPE,
    DBGBP_RWATCH_TYPE,
    DBGBP_EXIT_TYPE,
    DBGBP_UNKNOWN_TYPE
};
typedef enum dbgbp_type dbgbp_type;

enum dbgbp_disp {
    DBGBP_KEEP_DISP,
    DBGBP_DELETE_DISP,
    DBGBP_DISABLE_DISP,
    DBGBP_UNKNOWN_DISP
};
typedef enum dbgbp_disp dbgbp_disp;

class dbgcomm_info;

struct dbgbp_t {
    int bp_id;
    int bp_ignore;
    int bp_deleted;
    int bp_enabled;
    int bp_pending;
    dbgbp_disp bp_disp;
    dbgbp_type bp_type;
    dbgloc_t bp_loc;
    char *bp_exp;  // Should this be a part of bp_loc?? Its used to hold the
                   // watchpoint expression.
    int bp_threadId;
    int bp_hits;
    char *bp_stmt;
    char **bp_commands;
    dbgcomm_info *bp_comminfo;  // owned by this object
    struct dbgbp_t *bp_next;
};
typedef struct dbgbp_t dbgbp_t;

extern dbgbp_t *NewBP(int);
extern void DumpBP(dbgbp_t *bp);
extern void CleanupBP(list_t *);
extern void FreeBP(dbgbp_t *);
extern dbgbp_t *DupBP(dbgbp_t *);
extern void RemoveBP(list_t *, int);
extern dbgbp_t *FindBPByFunc(list_t *, char *);
extern dbgbp_t *FindBPByID(list_t *, int);
extern dbgbp_t *GenBPList(list_t *);
extern const char *BPTypeToStr(dbgbp_type);
extern const char *BPDispToStr(dbgbp_disp);

#endif /* !_DBGBP_H */
