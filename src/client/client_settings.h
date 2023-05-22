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

#include <map>

enum class PendingBrkType {
    Auto,
    On,
    Off
};

// This is the global settings object definition
// All settings should go here. If you find any that are missing, add them!
struct clnt_settings : public ClientSettingsInterface {
    int confirm;                                   /* Confirm dangerous commands */
    int quit;                                      /* Quit the debugger? */
    int interactive;                               /* Interactive mode? */
    int silent;                                    /* Print banner? */
    bool echo;                                     /* Echo each command in batch mode? */
    bool keep_going;                               /* Ignore errors in batch mode? */
    int unsafe;                                    // Attempt to work with a non-mpi application
    int early_entry;                               // Start debugging before continuing to an entry breakpoint
    int non_stop;                                  // Launch applications in non stop mode
    int print_entry_frame;                         /* Print frame locations at startup? */
    bool print_bp_frames;                          /* Print breakpoint frames as they are hit? */
    int print_source_on_step;                      /* Print line after each step? */
    int print_elements;                            /* Number of elements to print in arrays or char * strings */
    bool print_pretty;                             // print indented structures
    int sync_wait;                                 /* Treat asynchronous cmds as sync? */
    int sync_blocked;                              /* Current blocked on async cmd? */
    const char* proto;                             /* Default debugger protocol */
    const char* backend;                           /* Default debug backend*/
    cli_type_t cli_mode;                           /* CLI mode */
                                                   // XXX: The cli_* streams and fds SHOULD NOT BE SET OUTSIDE OF CLI_FUNCS!!!
    cli_out_t* cli_out;                            /* output stream-like object */
    FILE* cli_fpprog;
    int XML_mode;                                  /* XML output mode */
    bool MI_mode;                                  /* MI output mode */
    std::map<std::string, std::string> launch_env; /* Env vars applied to future job launches, modified by the set env cmd */
    double cmp_lower_eps;                          /* Lower eps limit */
    double cmp_upper_eps;                          /* upper eps limit */
    PendingBrkType brk_pending;                    /* pending breakpoint mode */

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
