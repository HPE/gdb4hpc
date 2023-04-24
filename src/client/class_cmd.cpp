/******************************************************************************\
 * class_cmd.c - Class Command processing routines
 *
 * Copyright 2011-2023 Hewlett Packard Enterprise Development LP.
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "client.h"


/*
 * Command classes are defined below - External visibility for custom command
 * lists
 */


/*
 * Init functions for command classes - External visibility
 */

void InitCmd_class(void) {
    // class commands
    AddCommand(
        "info", "i", proc_ignore, nullptr,
        class_info_help,
        "Display information about the application being debugged.");
    AddCommand("maint", proc_ignore, nullptr,
               class_maint_help,
               "Commands for use by debugger maintainers.");
    AddCommand("set", proc_ignore, nullptr,
               class_set_help,
               "Set information about the debugger environment.");
    AddCommand("show", proc_ignore, nullptr,
               class_show_help,
               "Show information about the debugger environment.");
    AddCommand("unset", proc_ignore, nullptr,
               class_unset_help,
               "Unset information in the debugger environment.");

    // Set class commands
    AddCommand("set print", proc_ignore, nullptr,
               class_set_print_help,
               "Set information about how things get printed.");
    AddCommand("set breakpoint", proc_ignore, nullptr,
               class_set_break_help,
               "Set information about breakpoints.");

    // Show class commands
    AddCommand("show print", proc_ignore, nullptr,
               class_show_print_help,
               "Show information about how things get printed.");
    AddCommand("show breakpoint", proc_ignore, nullptr,
               class_show_break_help,
               "Show information about breakpoints.");

    // Maint class commands
    AddCommand("maint set", proc_ignore, nullptr,
               class_maint_set_help,
               "Set information about the debugger environment.");
    AddCommand("maint show", proc_ignore, nullptr,
               class_maint_show_help,
               "Show information about the debugger environment.");
}

