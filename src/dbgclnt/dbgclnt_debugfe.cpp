/******************************************************************************\
 * Frontend debugger interface routines
 *
 * Copyright 2013-2019, 2020 Hewlett Packard Enterprise Development LP.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Hewlett Packard Enterprise Development LP.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *
 ******************************************************************************/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <string.h>

#include "dbgclnt_debugfe.h"
#include "defs.h"
#include "useful.h"

/*
** Setup debug frontend defs
*/
struct frontend_tab {
    const char *frontend_name;
    dbgfrontend_sw_create *frontend_create;
};
typedef struct frontend_tab frontend_tab;

// external debug backend def
extern dbgfrontend_sw_create DbgFrontendGDBMICreate;

// table of backend defs
static frontend_tab DbgFrontendTab[] = {
    {"gdbmi", &DbgFrontendGDBMICreate},
    {NULL, (dbgfrontend_sw_create *)NULL}};

dbgfrontend_sw *DbgInstantiateFE(const char *debug_def, const char* gdbPath) {
    for (auto p = DbgFrontendTab; p->frontend_name != NULL; ++p) {
        if (strcmp(p->frontend_name, debug_def) == 0) {
            // allocate a new object
            auto fe_obj = new dbgfrontend_sw{ .dbg_name = p->frontend_name };

            // instantiate the object usings its create function, this function
            // *MUST* be provided
            p->frontend_create->dbg_instantiate(fe_obj);

            // init the frontend if it has an init function defined
            if (fe_obj->dbg_init != NULL) {
                if (auto e = fe_obj->dbg_init(fe_obj, gdbPath)) {
                    // the event has always been ignored
                    FreeEvent(e);
                }
            }

            // return the object
            return fe_obj;
        }
    }

    return (dbgfrontend_sw *)NULL;
}

dbgevent_t *DbgDestroyFE(dbgfrontend_sw *fe_obj) {
    dbgevent_t *e;

    // if a quit function isn't implemented, just fail silently so that we can
    // cleanup memory.
    if (fe_obj->dbg_quit != NULL) {
        e = fe_obj->dbg_quit(fe_obj);
    } else {
        e = NewEvent(DBGEV_OK);
    }

    // cleanup memory
    delete fe_obj;

    return e;
}

/*
** The following functions are used to handle state information from DbgList
** commands.
*/

int AddListStateLocation(dbgfrontend_sw *fe_obj, const Bitset *set, const char* threads, const char *str) {
    if (!fe_obj || !set || !str) {
        return -1;
    }


    set_location new_loc = {
        .set = set->clone(),
        .threads = std::unique_ptr<char,decltype(&std::free)>(StrdupOpt(threads), &std::free),
        .loc_str = std::unique_ptr<char,decltype(&std::free)>(strdup(str), &std::free),
        .count = 0};

    fe_obj->dbg_list_locs.push_back(std::move(new_loc));

    // Set dbg_list_isPending to 1
    fe_obj->dbg_list_isPending = 1;

    return 0;
}

std::vector<set_location>& GetListStateLocations(dbgfrontend_sw& fe_obj) {
    return fe_obj.dbg_list_locs;
}


void ClearListState(dbgfrontend_sw *fe_obj) {
    if (!fe_obj) return;

    // set dbg_list_isPending to 0
    fe_obj->dbg_list_isPending = 0;

    fe_obj->dbg_list_locs.clear();
}

// Used to ensure the function is implemented
#define DBG_IMP(e, f)                                                         \
    if (f == NULL) {                                                          \
        EVENT_ERROR(e, DBGERR_DEBUGGER, "debugger function not implemented"); \
        return e;                                                             \
    }

dbgevent_t *DbgFESetFile(dbgfrontend_sw *fe_obj, const char *file) {
    dbgevent_t *e;

    DBG_IMP(e, fe_obj->dbg_set_file);

    return fe_obj->dbg_set_file(fe_obj, file);
}

dbgevent_t *DbgFECheckStartupFunc(dbgfrontend_sw *fe_obj) {
    dbgevent_t *e;

    DBG_IMP(e, fe_obj->dbg_checkstartupfunc);

    return fe_obj->dbg_checkstartupfunc(fe_obj);
}

dbgevent_t *DbgFEList(dbgfrontend_sw *fe_obj, const char* threads, const char *str) {
    dbgevent_t *e;

    DBG_IMP(e, fe_obj->dbg_list);

    return fe_obj->dbg_list(fe_obj, threads, str);
}

dbgevent_t *DbgFEGetSources(dbgfrontend_sw *fe_obj) {
    dbgevent_t *e;

    DBG_IMP(e, fe_obj->dbg_getsources);

    return fe_obj->dbg_getsources(fe_obj);
}

dbgevent_t *DbgFESetSourcePath(dbgfrontend_sw *fe_obj, const char *path) {
    dbgevent_t *e;

    DBG_IMP(e, fe_obj->dbg_setsourcepath);

    return fe_obj->dbg_setsourcepath(fe_obj, path);
}

dbgevent_t *DbgFEGetSourcePath(dbgfrontend_sw *fe_obj) {
    dbgevent_t *e;

    DBG_IMP(e, fe_obj->dbg_getsourcepath);

    return fe_obj->dbg_getsourcepath(fe_obj);
}

dbgevent_t *DbgFEGetScope(dbgfrontend_sw *fe_obj, char *location) {
    dbgevent_t *e;

    DBG_IMP(e, fe_obj->dbg_getscope);

    return fe_obj->dbg_getscope(fe_obj, location);
}
