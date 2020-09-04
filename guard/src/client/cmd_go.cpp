/*********************************************************************************\
 * Execution control commands.
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * Copyright 2010-2020 Hewlett Packard Enterprise Development LP.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Hewlett Packard Enterprise Development LP.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *
 *********************************************************************************/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "client.h"
#include "useful.h"

static int cmd_continue_cmd(rec_set_t *rec_set, int argc, char **argv) {
    rep_set_t *rep_set;

    DPERFLOG(SetTimerStart("cont"));

    bool continueAll = false;
    if (argc == 2 && strcmp(argv[1],"-a") == 0) {
        continueAll = true;
    } else if (argc > 1) {
        PrintOutError("unrecognized command arguments.\n");
        PrintOutError("expected continue or continue -a.\n");
        return 0;
    }
        
    if ((rep_set = DbgGo(rec_set, continueAll)) != NULL) {
        ProcessErrorInRepSet(rep_set);
        FreeRepSet(rep_set);
        return 0;
    }

    DPERFLOG(SetTimerStopAndLog("cont"));

    return 1;
}

static int cmd_finish_cmd(rec_set_t *rec_set, int argc, char **argv) {
    rep_set_t *rep_set;

    DPERFLOG(SetTimerStart("finish"));

    if ((rep_set = DbgFinishFunc(rec_set)) != NULL) {
        ProcessErrorInRepSet(rep_set);
        FreeRepSet(rep_set);
        return 0;
    }

    DPERFLOG(SetTimerStopAndLog("finish"));

    return 1;
}

void InitCmd_go(void) {
    AddCommand("continue", "c", proc_required, 0, cmd_continue_cmd,
               cmd_continue_help, "Continue execution of application.");
    AddCommand(
        "finish", NULL, proc_required, 0, cmd_finish_cmd,
        cmd_finish_help,
        "Execute application until each rank's current function returns.");
}
