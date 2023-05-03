/*********************************************************************************\
 * Breapoint routine definitions
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * Copyright 2010-2013,2020-2021 Hewlett Packard Enterprise Development LP.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Hewlett Packard Enterprise Development LP.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *
 *********************************************************************************/

#ifndef _DBGBP_H
#define _DBGBP_H

#include "dbgloc.h"
#include <string>

enum dbgbp_type {
    DBGBP_BREAK_TYPE,
    DBGBP_WATCH_TYPE,
    DBGBP_AWATCH_TYPE,
    DBGBP_RWATCH_TYPE,
    DBGBP_CATCH_TYPE,
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
    dbgbp_t(int feId, int beId);
    ~dbgbp_t();
    dbgbp_t(const dbgbp_t&) = delete;
    dbgbp_t* operator=(const dbgbp_t&) = delete;

    dbgbp_t* clone() const;
    
    int bp_fe_id;
    int bp_be_id;
    int bp_ignore;
    int bp_hits;
    bool bp_deleted;
    bool bp_enabled;
    bool bp_pending;
    dbgbp_disp bp_disp;
    dbgbp_type bp_type;
    dbgloc_t bp_loc;
    std::string bp_exp;
    std::string bp_threadId;
    std::string bp_stmt;
    std::vector<std::string> bp_commands;
    dbgcomm_info *bp_comminfo;  // owned by this object
};

extern void DumpBP(dbgbp_t *bp);
extern void RemoveBP(std::vector<dbgbp_t*>&, int feId, int beId);
extern dbgbp_t *FindBPByFunc(std::vector<dbgbp_t*>&, char *);
extern dbgbp_t *FindBPByID(std::vector<dbgbp_t*>&, int feId, int beId);
extern const char *BPTypeToStr(dbgbp_type);
extern const char *BPDispToStr(dbgbp_disp);

#endif /* !_DBGBP_H */
