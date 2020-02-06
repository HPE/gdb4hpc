/******************************************************************************\
 * Frontend debugger interface routines
 *
 * © 2013-2019 Cray Inc.	All Rights Reserved.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Cray Inc.,
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

// Static prototypes
static void FreeSetLocation(set_location *);

/*
** Setup debug frontend defs
*/
struct frontend_tab {
    const char *frontend_name;
    dbgfrontend_sw_create *frontend_create;
};
typedef struct frontend_tab frontend_tab;

// external debug backend defs
#if defined(GDBMI_BACKEND)
extern dbgfrontend_sw_create DbgFrontendGDBMICreate;
#endif

// table of backend defs
static frontend_tab DbgFrontendTab[] = {
#if defined(GDBMI_BACKEND)
    {"gdbmi", &DbgFrontendGDBMICreate},
#endif
    {NULL, (dbgfrontend_sw_create *)NULL}};

dbgfrontend_sw *DbgInstantiateFE(const char *debug_def) {
    frontend_tab *p;
    dbgfrontend_sw *fe_obj;

    for (p = DbgFrontendTab; p->frontend_name != NULL; ++p) {
        if (strcmp(p->frontend_name, debug_def) == 0) {
            // allocate a new object
            fe_obj = (dbgfrontend_sw *)Alloc(sizeof(dbgfrontend_sw));
            memset(fe_obj, 0, sizeof(dbgfrontend_sw));

            // Set the stuff we are responsible for
            fe_obj->dbg_name = p->frontend_name;
            fe_obj->dbg_status = DBGSTAT_INITIALISING;
            fe_obj->dbg_list_locs = NewList();

            // instantiate the object usings its create function, this function
            // *MUST* be provided
            p->frontend_create->dbg_instantiate(fe_obj);

            // init the frontend if it has an init function defined
            if (fe_obj->dbg_init != NULL) {
                fe_obj->dbg_init(fe_obj);
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

    // Free the list state
    DestroyList(fe_obj->dbg_list_locs, (void (*)(void *))FreeSetLocation);

    // cleanup memory
    Free(fe_obj);

    return e;
}

/*
** The following functions are used to handle state information from DbgList
** commands.
*/

int AddListStateLocation(dbgfrontend_sw *fe_obj, bitset *set, char *str) {
    set_location *new_loc;

    if (fe_obj == NULL || fe_obj->dbg_list_locs == NULL || set == NULL ||
        str == NULL) {
        return -1;
    }

    new_loc = (set_location *)Alloc(sizeof(set_location));
    new_loc->set = bitset_dup(set);
    new_loc->loc_str = strdup(str);
    new_loc->count = 0;

    AddToList(fe_obj->dbg_list_locs, new_loc);

    // Set dbg_list_isPending to 1
    fe_obj->dbg_list_isPending = 1;

    return 0;
}

set_location *GetListStateLocation(dbgfrontend_sw *fe_obj) {
    if (fe_obj == NULL || fe_obj->dbg_list_locs == NULL) return NULL;

    return (set_location *)GetListElement(fe_obj->dbg_list_locs);
}

void SetListStateLocation(dbgfrontend_sw *fe_obj) {
    if (fe_obj == NULL || fe_obj->dbg_list_locs == NULL) return;

    SetList(fe_obj->dbg_list_locs);
}

void ClearListState(dbgfrontend_sw *fe_obj) {
    set_location *loc;

    if (fe_obj == NULL || fe_obj->dbg_list_locs == NULL) return;

    // set dbg_list_isPending to 0
    fe_obj->dbg_list_isPending = 0;

    // free the list locations
    while ((loc = (set_location *)RemoveFirst(fe_obj->dbg_list_locs)) != NULL) {
        FreeSetLocation(loc);
    }
}

static void FreeSetLocation(set_location *set_loc) {
    bitset_free(set_loc->set);
    Free(set_loc->loc_str);
    Free(set_loc);
}

// Used to ensure the function is implemented
#define DBG_IMP(e, f)                                                         \
    if (f == NULL) {                                                          \
        EVENT_ERROR(e, DBGERR_DEBUGGER, "debugger function not implemented"); \
        return e;                                                             \
    }

dbgevent_t *DbgFEInit(dbgfrontend_sw *fe_obj) {
    dbgevent_t *e;

    DBG_IMP(e, fe_obj->dbg_init);

    return fe_obj->dbg_init(fe_obj);
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

dbgevent_t *DbgFEList(dbgfrontend_sw *fe_obj, const char *str) {
    dbgevent_t *e;

    DBG_IMP(e, fe_obj->dbg_list);

    return fe_obj->dbg_list(fe_obj, str);
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
