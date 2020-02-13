/*********************************************************************************\
 * Single step commands.
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * Copyright 2010-2020 Cray Inc.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Cray Inc.,
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
#include <stdlib.h>

#include "client.h"
#include "useful.h"

/*
** This function is able to handle both next and step by examining the first
** character of the command. If its an s, we set in to 1.
*/
static int StepCmd(rec_set_t *rec_set, int argc, char **argv) {
    int in = 0;
    int count = 1;
    rep_set_t *rep_set;

    if (*argv[0] == 's') in++;

    if (argc >= 2) count = atoi(argv[1]);

    DPERFLOG(SetTimerStart("step"));
    if ((rep_set = DbgStep(rec_set, count, in)) != NULL) {
        ProcessErrorInRepSet(rep_set);
        FreeRepSet(rep_set);
        return 0;
    }
    DPERFLOG(SetTimerStopAndLog("step"));

    return 1;
}

void InitCmd_step(void) {
    AddCommand("step", "s", proc_required, 0, (int (*)())StepCmd, cmd_step_help,
               "Step application until it reaches a different source line.");
    AddCommand("next", "n", proc_required, 0, (int (*)())StepCmd, cmd_next_help,
               "Step application, proceeding through subroutine calls.");
}
