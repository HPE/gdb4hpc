/******************************************************************************\
 * client_settings.h - Client settings interface definitions.
 *
 * Copyright 2013-2022 Hewlett Packard Enterprise Development LP.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Hewlett Packard Enterprise Development LP.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *
 ******************************************************************************/

#ifndef _CLIENT_SETTINGS_H
#define _CLIENT_SETTINGS_H

#include "cli_funcs.h"
#include "mi_funcs.h"
#include "dbgclnt_settings.h"
#include "dbgcmd.h"

#include <map>

enum class PendingBrkType { Auto, On, Off };

// This is the global settings object definition
// All settings should go here. If you find any that are missing, add them!
struct clnt_settings : public ClientSettingsInterface {
    int confirm;                                   /* Confirm dangerous commands */
    int quit;                                      /* Quit the debugger? */
    int unsafe;                                    // Attempt to work with a non-mpi application
    int early_entry;                               // Start debugging before continuing to an entry breakpoint
    int non_stop;                                  // Launch applications in non stop mode
    int print_entry_frame;                         /* Print frame locations at startup? */
    bool print_bp_frames;                          /* Print breakpoint frames as they are hit? */
    int print_source_on_step;                      /* Print line after each step? */
    int print_elements;                            /* Number of elements to print in arrays or char * strings */
    bool print_pretty;                             // print indented structures
    bool print_save_last;                          // Save last printed expression in `$`?
    int sync_wait;                                 /* Treat asynchronous cmds as sync? */
    int sync_blocked;                              /* Current blocked on async cmd? */
    const char* proto;                             /* Default debugger protocol */
    const char* backend;                           /* Default debug backend*/
                                                   // XXX: The cli_* streams and fds SHOULD NOT BE SET OUTSIDE OF CLI_FUNCS!!!
    FILE* cli_fpprog; // TODO this should live in cli_stream_t
    std::map<std::string, std::string> launch_env; /* Env vars applied to future job launches, modified by the set env cmd */
    double cmp_lower_eps;                          /* Lower eps limit */
    double cmp_upper_eps;                          /* upper eps limit */
    PendingBrkType brk_pending;                    /* pending breakpoint mode */
    std::string pager;                             // External binary to use for paging large content via PrintOutPagedMsg
    int height;                                    // PrintOutPagedMsg will only page messages with more lines than `height`
    PythonType python;                             // If python mode is enabled

    // methods for ClientSettingsInterface base class
    std::string getProto() override { return proto; }
    std::string getBackend() override { return backend; }
    bool isUnsafe() override { return static_cast<bool>(unsafe); }
    bool isEarlyEntry() override { return static_cast<bool>(early_entry); }
    bool isNonStop() override { return static_cast<bool>(non_stop); }
};
typedef struct clnt_settings clnt_settings_t;

/*
** External global variables defined in main.c
*/
extern clnt_settings_t Client; /* global settings store */

#endif /* _CLIENT_SETTINGS_H */
