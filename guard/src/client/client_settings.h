/******************************************************************************\
 * client_settings.h - Client settings interface definitions.
 *
 * © 2013-2019 Cray Inc.  All Rights Reserved.
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
 ******************************************************************************/

#ifndef _CLIENT_SETTINGS_H
#define _CLIENT_SETTINGS_H

#include    "aif.h"
#include    "cli_funcs.h"

// This is the global settings object definition
// All settings should go here. If you find any that are missing, add them!
struct  clnt_settings
{
    int                 confirm;            /* Confirm dangerous commands */
    int                 quit;               /* Quit the debugger? */
    int                 interactive;        /* Interactive mode? */
    int                 silent;             /* Be quiet? */
    int                 unsafe;             /* Be unsafe? */
    int                 early_entry;        /* Enter at pmi_init? */
    int                 print_entry_frame;  /* Print frame locations at startup? */
    int                 sync_wait;          /* Treat asynchronous cmds as sync? */
    int                 sync_blocked;       /* Current blocked on async cmd? */
    char *              proto;              /* Default debugger protocol */
    char *              arch;               /* Default starter achitecture */
    char *              backend;            /* Default debug backend*/
    cli_type_t          cli_mode;           /* CLI mode */
    // XXX: The cli_* streams and fds SHOULD NOT BE SET OUTSIDE OF CLI_FUNCS!!!
    FILE *              cli_in_stream;      /* input stream */
    int                 cli_in_fd;          /* input fd */
    FILE *              cli_out_stream;     /* output stream */
    int                 cli_out_fd;         /* output fd */
    FILE *              cli_err_stream;     /* error stream */
    int                 cli_err_fd;         /* error fd */
    int                 XML_mode;           /* XML output mode */
    list_t *            set_env_list;       /* Env vars from set env cmd */
    AIF *               cmp_lower_eps;      /* Lower eps limit */
    AIF *               cmp_upper_eps;      /* upper eps limit */
};
typedef struct clnt_settings        clnt_settings_t;

/*
** External global variables defined in main.c
*/
extern clnt_settings_t  Client;     /* global settings store */

#endif /* _CLIENT_SETTINGS_H */

