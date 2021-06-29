/*********************************************************************************\
 * Process Set Definitions.
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * Copyright 2010-2014,2020 Hewlett Packard Enterprise Development LP.
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

#ifndef _PROCSET_H
#define _PROCSET_H

#include "dense_bitset.h"
#include "dbg_client.h"

/*
** The following describes each type of procset.
**
** DBGSET_INVOKE:
**   This procset is used for invoked applications. It is created when calling
**   the launch or attach commands. All responses provided by the debugger will
**   be in terms of the handle provided for this procset. The dbg_invoke
**   structure holds onto this handle name and responses use that when creating
**   the print messages. The lifetime of this procset is permament until the
**   kill/release command is called on the original procset. At that point all
**   existing defsets that reference this procset are marked invalid.
**
** DBGSET_DEFSET:
**   This procset is used when calling the defset command. These procsets are
**   permanent until explicitly removed. Multiple dbginvoke_t can belong to
**   this type of procset. The over_set_list provides a pointer to each di in
**   the procset along with the mask which defines which ranks belong to it. The
**   offset is used to determine where the di ranks should start counting from.
**   The first di in the procset starts from offset 0, the second di starts from
**   the number of ranks set in the first di, and so on.
**
** DBGSET_TEMP:
**   This procset is used in the expr parser layer and is created for process
**   targeted commands. The lifetime of this procset is the same as the val_t.
**   Once the val_t is destroyed, this procset is removed. The user should never
**   be allowed to access this type of procset from the command line. This is
**   also used for the focus command. In that case, process targeted commands
**   cannot be used with the temp procset.
**
** DBGSET_INTERNAL:
**
**   This procset is used as an internal procset, most notably the $all procset.
**   The lifetime of this procset is the entire duration of the debugger
**   instance. It should never be free'ed except when calling the final tear
**   down on exit.
*/
enum dbgset_type { DBGSET_INVOKE, DBGSET_DEFSET, DBGSET_TEMP, DBGSET_INTERNAL };
typedef enum dbgset_type dbgset_type;

/*
** A procset can point at multiple dbginvoke_t (di) objects. For each di, the
** procset can point at only a subset of all ranks. An overlay set (over_set)
** is used to keep track of which ranks the procset is pointing at for the di.
**
** The offset only comes into play with DBGSET_DEFSET to keep track of which
** index in the procset maps onto which dbginvoke_t for targeted commands.
*/
struct dbg_over_set_t {
    dbginvoke_t* s_di;                    // pointer to dbginvoke
    std::unique_ptr<Bitset> s_di_bs;      // mask for bitset in di, this is the "full set" for this
                                          // overlay set
    int s_ps_offset;                      // offset for this overset, used when multiple oversets
                                          // come into play with defset calculate which index
                                          // corresponds to which over_set
};
typedef struct dbg_over_set_t dbg_over_set_t;

/*
** A procset consists of one or more overlay sets.
*/
struct dbgprocset_t {
    char* s_name;        // name of procset
    dbgset_type s_type;  // type of procset
    int s_nproc;         // number of ranks in procset
    int s_init;          // Used for DBGSET_INVOKE, if the set was finalized
    int s_num_sets;  // Used to keep track of the number of over_sets, each one
                     // corresponding to a unique di
    list_t* s_over_set_list;  // list of dbg_over_set_t
    char* s_threads;
};
typedef struct dbgprocset_t dbgprocset_t;

dbgprocset_t *FindProcSet(char *procsetName);
bool CheckProcSet(hentry_t* sym);
void SetProcSetThreads(dbgprocset_t* procSet, const char* threads);

#endif /* !_PROCSET_H */
