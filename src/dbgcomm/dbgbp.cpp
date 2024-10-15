/*********************************************************************************\
 * Breakpoint support routines.
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * Copyright 2010-2013,2020-2022 Hewlett Packard Enterprise Development LP.
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

#include "config.h"

#include <stdio.h>
#include <string.h>

#include "dbg.h"
#include "dbgbp.h"
#include "dbgcomm_info.h"
#include "defs.h"
#include "useful.h"

const char *dbgbp_type_str[] = {"breakpoint",     "watchpoint",
                                "acc watchpoint", "read watchpoint",
                                "catchpoint",     "unknown",        
                                NULL};

const char *dbgbp_disp_str[] = {"keep", "del", "dis", "unk", NULL};

/*
** Create a new breakpoint.
*/
dbgbp_t::dbgbp_t(int feId, int beId) :
    bp_fe_id(feId), bp_be_id(beId), bp_ignore(0), bp_hits(0),
    bp_deleted(false), bp_enabled(true), bp_pending(false),
    bp_disp(DBGBP_UNKNOWN_DISP),
    bp_type(DBGBP_UNKNOWN_TYPE),
    bp_comminfo(nullptr) {}


/*
** Create a duplicate breakpoint (or list) for returning in an
** event (as the event is automatically freed).
*/
dbgbp_t *dbgbp_t::clone() const {
    auto b = new dbgbp_t(bp_fe_id, bp_be_id);

    b->bp_ignore = bp_ignore;
    b->bp_deleted = bp_deleted;
    b->bp_enabled = bp_enabled;
    b->bp_pending = bp_pending;
    b->bp_disp = bp_disp;
    b->bp_type = bp_type;
    b->bp_hits = bp_hits;
    b->bp_loc = bp_loc;
    b->bp_exp = bp_exp;
    b->bp_stmt = bp_stmt;
    b->bp_commands = bp_commands;
    b->bp_threadId = bp_threadId;

    b->bp_comminfo = (bp_comminfo ? bp_comminfo->clone() : nullptr);

    return b;
}

/*
** Destroy a breakpoint.
*/
dbgbp_t::~dbgbp_t() {
    delete bp_comminfo;
}

/*
** Remove a breakpoint from the list.
*/
void RemoveBP(std::vector<dbgbp_t*>& bps, int feId, int beId) {
    bps.erase(std::remove_if(bps.begin(), bps.end(),
                             [feId,beId](auto bp) {
                                 if ((feId && bp->bp_fe_id == feId) || (beId && bp->bp_be_id == beId)) {
                                     delete bp; return true; } else { return false; }}),
              bps.end());
}

/*
** Find a breakpoint set on func.
*/
dbgbp_t *FindBPByFunc(std::vector<dbgbp_t*>& bps, char *func) {
    if (!func) return nullptr;
    for (auto bp : bps) {
        if (bp->bp_loc.loc_func != nullptr && (strcmp(func, bp->bp_loc.loc_func) == 0)) return bp;
    }

    return nullptr;
}

/*
** Find a breakpoint by its id.
*/
dbgbp_t *FindBPByID(std::vector<dbgbp_t*>& bps, int feId, int beId) {
    for (auto bp : bps)
        if ((feId && feId == bp->bp_fe_id) || (beId && beId == bp->bp_be_id)) return bp;

    return nullptr;
}

/*
** Return the string representation of the breakpoint type.
*/
const char *BPTypeToStr(dbgbp_type type) { return dbgbp_type_str[type]; }

/*
** Return the string representation of the breakpoint disposition.
*/
const char *BPDispToStr(dbgbp_disp disp) { return dbgbp_disp_str[disp]; }

void DumpBP(dbgbp_t *bp) {
    PrintOutMsg("feId=%d,", bp->bp_fe_id);
    PrintOutMsg("beId=%d,", bp->bp_be_id);
    PrintOutMsg("ign=%s,", bp->bp_ignore ? "true" : "false");
    PrintOutMsg("del=%s,", bp->bp_deleted ? "true" : "false");
    PrintOutMsg("enb=%s,", bp->bp_enabled ? "true" : "false");
    PrintOutMsg("pend=%s,", bp->bp_pending ? "true" : "false");
    PrintOutMsg("disp=%s,", dbgbp_disp_str[bp->bp_disp]);
    PrintOutMsg("type=%s,", dbgbp_type_str[bp->bp_type]);
    PrintOutMsg("file=%s,", 
                bp->bp_loc.loc_file != NULL ? bp->bp_loc.loc_file : "NULL");
    PrintOutMsg("func=%s,",
                bp->bp_loc.loc_func != NULL ? bp->bp_loc.loc_func : "NULL");
    PrintOutMsg("addr=%s,",
                bp->bp_loc.loc_addr != NULL ? bp->bp_loc.loc_addr : "NULL");
    PrintOutMsg("line=%d,", bp->bp_loc.loc_line);
    PrintOutMsg("exp=%s,", bp->bp_exp.c_str());
    PrintOutMsg("hits=%d,", bp->bp_hits);
    PrintOutMsg("stmt=%s,", bp->bp_stmt.c_str());
    PrintOutMsg("\n");
}

dbgloc::dbgloc(const dbgloc &other)
    : loc_file{other.loc_file ? strdup(other.loc_file) : nullptr}
    , loc_func{other.loc_func ? strdup(other.loc_func) : nullptr}
    , loc_addr{other.loc_addr ? strdup(other.loc_addr) : nullptr}
    , loc_line{other.loc_line} {}

dbgloc& dbgloc::operator=(const dbgloc &other) {
    if (this == &other) return *this;

    free(loc_file);
    loc_file = other.loc_file ? strdup(other.loc_file) : nullptr;
    free(loc_func);
    loc_func = other.loc_func ? strdup(other.loc_func) : nullptr;
    free(loc_addr);
    loc_addr = other.loc_addr ? strdup(other.loc_addr) : nullptr;
    loc_line = other.loc_line;
    return *this;
}

bool dbgloc::operator==(const dbgloc &other) const {
    const auto sameCStr = [](const char* lhs, const char* rhs) {
        if (!lhs && !rhs) return true;
        if (!lhs || !rhs) return false;
        return ::strcmp(lhs, rhs) == 0;
    };

    return
        this->loc_line == other.loc_line &&
        sameCStr(this->loc_file, other.loc_file) &&
        sameCStr(this->loc_func, other.loc_func) &&
        sameCStr(this->loc_addr, other.loc_addr)
    ;
}

