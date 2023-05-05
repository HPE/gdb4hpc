/******************************************************************************\
 * Frontend debugger interface definition
 * TODO: This does not support asynchronous commands.
 *
 * Copyright 2013, 2020 Hewlett Packard Enterprise Development LP.
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
 ******************************************************************************/

#ifndef _DBGCLNT_DEBUGFE_H
#define _DBGCLNT_DEBUGFE_H

#include "dense_bitset.h"
#include "dbgevent.h"

typedef void *dbgfrontend_sess;

struct set_location {
    std::unique_ptr<Bitset> set;
    std::unique_ptr<char, decltype(&std::free)> threads;
    std::unique_ptr<char, decltype(&std::free)> loc_str;
    int count;
};

struct dbgfrontend_sw {
    const char *dbg_name = nullptr;
    dbgfrontend_sess dbg_session = nullptr;
    dbgstat dbg_status = DBGSTAT_INITIALISING;
    int dbg_list_isPending = 0;
    std::vector<set_location> dbg_list_locs;

    dbgevent_t *(*dbg_init)(struct dbgfrontend_sw *, const char* gdbPath);
    dbgevent_t *(*dbg_quit)(struct dbgfrontend_sw *);
    dbgevent_t *(*dbg_set_file)(struct dbgfrontend_sw *, const char *);
    dbgevent_t *(*dbg_checkstartupfunc)(struct dbgfrontend_sw *);
    dbgevent_t *(*dbg_list)(struct dbgfrontend_sw *, const char*, const char *);
    dbgevent_t *(*dbg_getsources)(struct dbgfrontend_sw *);
    dbgevent_t *(*dbg_setsourcepath)(struct dbgfrontend_sw *, const char *);
    dbgevent_t *(*dbg_getsourcepath)(struct dbgfrontend_sw *);
    dbgevent_t *(*dbg_getscope)(struct dbgfrontend_sw *, const char *);
};
typedef struct dbgfrontend_sw dbgfrontend_sw;

// Used by debug frontend definitions to set their instantiation functions
struct dbgfrontend_sw_create {
    void (*dbg_instantiate)(dbgfrontend_sw *);
};
typedef struct dbgfrontend_sw_create dbgfrontend_sw_create;

// Functions used for managing state information for DbgList commands
extern int AddListStateLocation(dbgfrontend_sw *, const Bitset*, const char*, const char *);

extern std::vector<set_location>& GetListStateLocations(dbgfrontend_sw&);
extern void ClearListState(dbgfrontend_sw *);

// Interface functions
extern dbgfrontend_sw *DbgInstantiateFE(const char *, const char* gdb_path);
extern dbgevent_t *DbgDestroyFE(dbgfrontend_sw *);
extern dbgevent_t *DbgFESetFile(dbgfrontend_sw *, const char *);
extern dbgevent_t *DbgFECheckStartupFunc(dbgfrontend_sw *);
extern dbgevent_t *DbgFEList(dbgfrontend_sw *, const char*, const char *);
extern dbgevent_t *DbgFEGetSources(dbgfrontend_sw *);
extern dbgevent_t *DbgFESetSourcePath(dbgfrontend_sw *, const char *);
extern dbgevent_t *DbgFEGetSourcePath(dbgfrontend_sw *);
extern dbgevent_t *DbgFEGetScope(dbgfrontend_sw *, char *);

#endif /* !_DBGCLNT_DEBUGFE_H */
