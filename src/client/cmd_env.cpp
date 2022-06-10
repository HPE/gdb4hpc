/*********************************************************************************\
 * Debugger environment commands.
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * Copyright 2010-2022 Hewlett Packard Enterprise Development LP.
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

#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "client.h"
#include "useful.h"

static int class_set_subcmd_environment_cmd(rec_set_t *rec_set, int argc,
                                            char **argv) {
    int putenv_str_size;
    char *putenv_str;
    char *str;
    char *name;
    char *value;

    if (argc != 3) {
        PrintOutError("Not enough command arguments.\n");
        PrintOutError("Expected: set environment name value\n");
        return 0;
    }

    name = StripQuotes(strdup(argv[1]));
    value = StripQuotes(strdup(argv[2]));

    // we need to ensure that the name is not already set in the list

    SetList(Client.set_env_list);

    // loop through the list
    while ((str = (char *)GetListElement(Client.set_env_list)) !=
           (char *)NULL) {
        // compare only the number of chars in the name, this should get us up
        // to the "=" part
        if (!strncmp(str, name, strlen(name))) {
            // name found, so lets remove the old value
            RemoveFromList(Client.set_env_list, (void *)str);
            Free(str);
            break;
        }
    }

    // need 2 extra chars for "=" and null terminator
    putenv_str_size = strlen(name) + strlen(value) + 2;

    putenv_str = (char *)Alloc(putenv_str_size);

    // snprintf the new string
    snprintf(putenv_str, putenv_str_size, "%s=%s", name, value);

    // add it to the set_env_list
    AddToList(Client.set_env_list, (void *)putenv_str);

    return 1;
}

static int class_unset_subcmd_environment_cmd(rec_set_t *rec_set, int argc,
                                              char **argv) {
    char *str;
    char *name;

    // ensure that there are entries in the list
    if (SizeOfList(Client.set_env_list) < 1) {
        PrintOutError("There are no environment variables set.\n");
        return 1;
    }

    // ensure the user gave us a single name
    if (argc != 2) {
        PrintOutError("Incorrect number of command arguments.\n");
        PrintOutError("Expected: unset environment name\n");
        return 0;
    }

    SetList(Client.set_env_list);

    // this points to the name the user provided
    name = argv[1];

    // loop through the list
    while ((str = (char *)GetListElement(Client.set_env_list)) !=
           (char *)NULL) {
        // compare only the number of chars in the name, this should get us up
        // to the "=" part
        if (!strncmp(str, name, strlen(name))) {
            RemoveFromList(Client.set_env_list, (void *)str);
            Free(str);
            // return here so that we can do error handling below
            return 1;
        }
    }

    // variable name not found, so lets print out an error
    PrintOutError("Environment variable \"%s\" not set.\n", name);

    return 1;
}

static int class_show_subcmd_environment_cmd(rec_set_t *rec_set, int argc,
                                             char **argv) {
    char *str;
    char *name;

    // ensure that there are entries in the list
    if (SizeOfList(Client.set_env_list) < 1) {
        PrintOutError("There are no environment variables set.\n");
        return 1;
    }

    SetList(Client.set_env_list);

    // if the user didn't provide an argument, print out the entire list
    if (argc < 2) {
        // loop through the list
        while ((str = (char *)GetListElement(Client.set_env_list)) !=
               (char *)NULL) {
            PrintOutMsg("%s\n", str);
        }
        return 1;
    }

    // argument was provided, so lets try to print it out

    // this points to the name the user provided
    name = argv[1];

    // loop through the list
    while ((str = (char *)GetListElement(Client.set_env_list)) !=
           (char *)NULL) {
        // compare only the number of chars in the name, this should get us up
        // to the "=" part
        if (!strncmp(str, name, strlen(name))) {
            PrintOutMsg("%s\n", str);
            // we return here so that we can do error handling below
            return 1;
        }
    }

    // variable name not found, so lets print out an error
    PrintOutError("Environment variable \"%s\" not set.\n", name);

    return 1;
}

/*
** This function has external exposure, hence it is not static. Its used in
** the passthru to prevent someone from typing quit and having things die
** ungracefully.
*/
int cmd_quit_cmd(rec_set_t *rec_set, int argc, char **argv) {
    Client.quit = 1;
    DbgMaskInput();

    /* TimerLog is created in cmd_proc */
    DPERFLOG(CloseTimerLog());

    return 1;
}

static int class_maint_subcmd_echo_cmd(rec_set_t *rec_set, int argc,
                                       char **argv) {
    char *echo_str;

    if (argc < 2) return 0;

    // create the message string to echo
    echo_str = ArgsToBuf(&argv[1], argc - 1);

    PrintOutMsg("%s\n", echo_str);

    Free(echo_str);

    return 1;
}

static int class_maint_subcmd_sleep_cmd(rec_set_t *rec_set, int argc,
                                        char **argv) {
    int time = 5;

    if (argc > 1) time = atoi(argv[1]);
    sleep(time);

    return 1;
}

static int cmd_shell_cmd(rec_set_t *rec_set, int argc,
                                         char **argv) {
    FILE *cmd_out;
    char *cmd = NULL;
    char *tmp_cmd;
    char result[BUFSIZ];
    char *line = NULL;
    char *tmp_line = NULL;

    // clear the buffer
    memset((void *)&result[0], '\0', BUFSIZ);

    // Create the command string
    tmp_cmd = ArgsToBuf(&argv[1], argc - 1);

    // Append a redirection of stderr to stdout to the command
    asprintf(&cmd, "%s 2>&1", tmp_cmd);
    Free(tmp_cmd);

    // Open the command for reading
    if ((cmd_out = popen(cmd, "r")) == NULL) {
        PrintOutError("Failed to run command.\n");
        Free(cmd);
        return 0;
    }

    // Done with the cmd string
    Free(cmd);

    // Now read the result
    while (fgets(result, BUFSIZ, cmd_out) != NULL) {
        // copy the result into the line buffer and clear the result buf
        asprintf(&line, "%s", result);
        memset((void *)result, '\0', BUFSIZ);
        // ensure the last character is a newline, otherwise we need to keep
        // reading
        while (line[strlen(line) - 1] != '\n') {
            // read in another line
            if (fgets(result, BUFSIZ, cmd_out) == NULL) {
                // could not read an entire line, so print what we have and
                // return
                PrintOutMsg("%s", line);
                Free(line);
                return 1;
            }
            // save a copy of line
            tmp_line = line;
            line = NULL;
            // concat the two strings together
            asprintf(&line, "%s%s", tmp_line, result);
            // cleanup
            Free(tmp_line);
            memset((void *)result, '\0', BUFSIZ);
        }

        // Print the line and cleanup
        PrintOutMsg("%s", line);
        Free(line);
    }

    // close the file pointer
    pclose(cmd_out);

    return 1;
}

static int class_maint_set_subcmd_earlyentry_cmd(rec_set_t *rec_set, int argc,
                                                 char **argv) {
    const char *s;
    ct_val cv;

    if (argc < 2) {
        PrintOutError("Not enough command arguments.\n");
        PrintOutError("Expected: maint set earlyentry [on|off]\n");
        return 0;
    }

    // only look at the first arg
    s = argv[1];

    switch (ScanString(&cv, &s)) {
        case CT_TRUE:
            Client.early_entry = 1;
            break;
        case CT_FALSE:
            Client.early_entry = 0;
            break;
        default:
            PrintOutError("Encountered something other than on or off.\n");
            PrintOutError(
                "See \"help maint set earlyentry\" for more information.\n");
            return 0;
    }

    return 1;
}

static int class_maint_show_subcmd_earlyentry_cmd(rec_set_t *rec_set, int argc,
                                                  char **argv) {
    if (Client.early_entry)
        PrintOutMsg("Early entry mode enabled.\n");
    else
        PrintOutMsg("Early entry mode disabled.\n");

    return 1;
}

static int class_maint_set_subcmd_unsafe_cmd(rec_set_t *rec_set, int argc,
                                             char **argv) {
    const char *s;
    ct_val cv;

    if (argc < 2) {
        PrintOutError("Not enough command arguments.\n");
        PrintOutError("Expected: maint set unsafe [on|off]\n");
        return 0;
    }

    // only look at the first arg
    s = argv[1];

    switch (ScanString(&cv, &s)) {
        case CT_TRUE:
            Client.unsafe = 1;
            break;
        case CT_FALSE:
            Client.unsafe = 0;
            break;
        default:
            PrintOutError("Encountered something other than on or off.\n");
            PrintOutError(
                "See \"help maint set unsafe\" for more information.\n");
            return 0;
    }

    return 1;
}

static int class_maint_show_subcmd_unsafe_cmd(rec_set_t *rec_set, int argc,
                                              char **argv) {
    if (Client.unsafe)
        PrintOutMsg("Unsafe mode enabled.\n");
    else
        PrintOutMsg("Unsafe mode disabled.\n");

    return 1;
}

static int class_maint_set_subcmd_sync_cmd(rec_set_t *rec_set, int argc,
                                           char **argv) {
    const char *s;
    ct_val cv;

    if (argc < 2) {
        PrintOutError("Not enough command arguments.\n");
        PrintOutError("Expected: maint set sync [on|off]\n");
        return 0;
    }

    // only look at the first arg
    s = argv[1];

    switch (ScanString(&cv, &s)) {
        case CT_TRUE:
            Client.sync_wait = 1;
            break;
        case CT_FALSE:
            // Possibly reset if we are currently blocking
            // Note that this is probably unecessary since I don't know how this
            // command could possibly execute without being unblocked, but it is
            // good practice I suppose.
            if (Client.sync_blocked) {
                DbgUnmaskInput();
                Client.sync_blocked = 0;
            }
            Client.sync_wait = 0;
            break;
        default:
            PrintOutError("Encountered something other than on or off.\n");
            PrintOutError(
                "See \"help maint set sync\" for more information.\n");
            return 0;
    }

    return 1;
}

static int class_maint_show_subcmd_sync_cmd(rec_set_t *rec_set, int argc,
                                            char **argv) {
    if (Client.sync_wait)
        PrintOutMsg("Synchronous event mode enabled.\n");
    else
        PrintOutMsg("Synchronous event mode disabled.\n");

    return 1;
}

static int class_info_subcmd_status_cmd(rec_set_t *rec_set, int argc,
                                        char **argv) {
    rep_set_t *rep_set;
    dbgstatus_t *s;

    // We do not want to get the location info with this status command
    rep_set = DbgStatus(rec_set, 0);

    ProcessErrorInRepSet(rep_set);

    for (auto&& reply : rep_set) {
        // point at the status data
        s = EVENT_STATUS(reply->rep_event);

        switch (s->stat) {
            case DBGSTAT_INITIALISING:
                PrintMsgWithSetInfo(reply->rep_di->d_handle, reply->rep_group.get(),
                                    "The debugger is initialising.\n");
                break;

            case DBGSTAT_WAITING:
                PrintMsgWithSetInfo(reply->rep_di->d_handle, reply->rep_group.get(),
                                    "The debugger is waiting for a command.\n");
                break;

            case DBGSTAT_RUNNING:
                PrintMsgWithSetInfo(reply->rep_di->d_handle, reply->rep_group.get(),
                                    "The application is running.\n");
                break;

            case DBGSTAT_STOPPED:
                PrintMsgWithSetInfo(reply->rep_di->d_handle, reply->rep_group.get(),
                                    "The application is stopped.\n");
                break;

            case DBGSTAT_STOPPED_IN_THREAD:
                PrintMsgWithSetInfo(reply->rep_di->d_handle, reply->rep_group.get(),
                                    "The application is stopped in a thread while in non-stop mode.\n");
                break;

            case DBGSTAT_EXIT:
                PrintMsgWithSetInfo(
                    reply->rep_di->d_handle, reply->rep_group.get(),
                    "The application is at an exit breakpoint.\n");
                break;

            case DBGSTAT_DBGERR:
            case DBGSTAT_INTERR:
                PrintErrorWithSetInfo(
                    reply->rep_di->d_handle, reply->rep_group.get(),
                    "The debugger is reporting an error status.\n");
                break;
        }
    }

    FreeRepSet(rep_set);

    return 1;
}

static int class_set_subcmd_directory_cmd(rec_set_t *rec_set, int argc,
                                          char **argv) {
    rep_set_t *rep_set;
    char *new_path;

    if (argc < 2) {
        PrintOutError("Not enough command arguments.\n");
        PrintOutError("Expected: set directory /path/to/dir\n");
        return 0;
    }

    rep_set = DbgSetSourcePath(rec_set, argv[1]);

    ProcessErrorInRepSet(rep_set);

    for (auto&& reply : rep_set) {
        new_path = EVENT_SOURCE_PATH(reply->rep_event);

        PrintMsgWithSetInfo(reply->rep_di->d_handle, reply->rep_group.get(),
                            "Source directory search path set to: %s\n",
                            new_path);
    }

    FreeRepSet(rep_set);

    return 1;
}

static int class_show_subcmd_directories_cmd(rec_set_t *rec_set, int argc,
                                             char **argv) {
    rep_set_t *rep_set;
    char *path;

    rep_set = DbgGetSourcePath(rec_set);

    ProcessErrorInRepSet(rep_set);

    for (auto&& reply : rep_set) {
        path = EVENT_SOURCE_PATH(reply->rep_event);

        PrintMsgWithSetInfo(reply->rep_di->d_handle, reply->rep_group.get(),
                            "Source directories searched: %s\n", path);
    }

    FreeRepSet(rep_set);

    return 1;
}

static int class_show_subcmd_version_cmd(rec_set_t *rec_set, int argc,
                                         char **argv) {
    PrintOutMsg("Version %s\n", GUARD_VERSION);
    return 1;
}

static int class_set_subcmd_error_cmd(rec_set_t *rec_set, int argc,
                                      char **argv) {
    const char *s;
    char *buf;
    long double lower;
    long double upper;
    long double ulp;
    ct_val cv;

    if (argc < 3) {
        PrintOutError("Not enough command arguments.\n");
        PrintOutError("Expected: set error [warn] [error]\n");
        PrintOutError("Example command: set error 1e-16 2e-15\n");
        return 0;
    }

    buf = ArgsToBuf(&argv[1], argc - 1);

    s = buf;

    switch (ScanString(&cv, &s)) {
        case CT_FLOAT:
            lower = cv.cv_float;
            break;
        case CT_INT:
            lower = (long double)cv.cv_int;
            break;
        default:
            PrintOutError(
                "Encountered a non-numeric value for the warning epsilon "
                "argument.\n");
            PrintOutError("Must be a numeric value. Example: 1e-16\n");
            Free(buf);
            return 0;
    }

    switch (ScanString(&cv, &s)) {
        case CT_FLOAT:
            upper = cv.cv_float;
            break;
        case CT_INT:
            upper = (long double)cv.cv_int;
            break;
        default:
            PrintOutError(
                "Encountered a non-numeric value for the error epsilon "
                "argument.\n");
            PrintOutError("Must be a numeric value. Example: 2e-15\n");
            Free(buf);
            return 0;
    }

    // Done with buf
    Free(buf);

    // ensure upper and lower are ever so slightly greater than zero.
    if ((lower == 0.0) || (upper == 0.0)) {
        ulp = nextafterl(
            0.0, 0.1);  // get the unit of least precision for this machine
        if (lower == 0.0) lower = ulp;
        if (upper == 0.0) upper = ulp;
    }

    if (lower > upper) {
        PrintOutError("The lower value must be less than the upper value.\n");
        PrintOutError("See \"help set error\" for more information.\n");
        return 0;
    }

    Client.cmp_lower_eps = lower;
    Client.cmp_upper_eps = upper;

    return 1;
}

static int class_show_subcmd_error_cmd(rec_set_t *rec_set, int argc,
                                       char **argv) {
    PrintOutMsg("Warning epsilon: ");
    PrintOutMsg("%.16lg", Client.cmp_lower_eps);
    PrintOutMsg("\nError epsilon: ");
    PrintOutMsg("%.16lg", Client.cmp_upper_eps);
    PrintOutMsg("\n");

    return 1;
}

static int class_set_print_subcmd_entryframe_cmd(rec_set_t *rec_set, int argc,
                                                 char **argv) {
    const char *s;
    int entryframe_flag;
    ct_val cv;

    if (argc < 2) {
        PrintOutError("Not enough command arguments.\n");
        PrintOutError("Expected: set print entry-frame [true|false]\n");
        return 0;
    }

    // only look at the first arg
    s = argv[1];

    switch (ScanString(&cv, &s)) {
        case CT_TRUE:
            entryframe_flag = 1;
            break;

        case CT_FALSE:
            entryframe_flag = 0;
            break;

        default:
            PrintOutError("Encountered something other than true or false.\n");
            PrintOutError(
                "See \"help set print entry-frame\" for more information.\n");
            return 0;
    }

    Client.print_entry_frame = entryframe_flag;

    return 1;
}

static int class_show_print_subcmd_entryframe_cmd(rec_set_t *rec_set, int argc,
                                                  char **argv) {
    if (Client.print_entry_frame)
        PrintOutMsg("print entry-frame enabled.\n");
    else
        PrintOutMsg("print entry-frame disabled.\n");

    return 1;
}

static int class_set_print_subcmd_sourceonstep_cmd(rec_set_t *rec_set, int argc,
                                                 char **argv) {
    const char *s;
    int sourceonstep_flag;
    ct_val cv;

    if (argc < 2) {
        PrintOutError("Not enough command arguments.\n");
        PrintOutError("Expected: set print source-on-step [true|false]\n");
        return 0;
    }

    // only look at the first arg
    s = argv[1];

    switch (ScanString(&cv, &s)) {
        case CT_TRUE:
            sourceonstep_flag = 1;
            break;

        case CT_FALSE:
            sourceonstep_flag = 0;
            break;

        default:
            PrintOutError("Encountered something other than true or false.\n");
            PrintOutError(
                "See \"help set print source-on-step\" for more information.\n");
            return 0;
    }

    Client.print_source_on_step = sourceonstep_flag;

    return 1;
}

static int class_show_print_subcmd_sourceonstep_cmd(rec_set_t *rec_set, int argc,
                                                  char **argv) {
    if (Client.print_source_on_step)
        PrintOutMsg("print source-on-step enabled.\n");
    else
        PrintOutMsg("print source-on-step disabled.\n");

    return 1;
}

void InitCmd_env(void) {
    // regular commands
    AddCommand("quit", "q", proc_ignore, 0, cmd_quit_cmd,
               cmd_quit_help, "Exit the debugger.");
    
    AddCommand("shell", nullptr, proc_ignore, 0,
                cmd_shell_cmd,
                cmd_shell_help, "Run a shell command.");

    // maint commands
    AddMaintCommand("echo", proc_ignore, 0,
                    class_maint_subcmd_echo_cmd,
                    class_maint_subcmd_echo_help, "Echo a string.");

    AddMaintCommand("system", proc_ignore, 0,
                cmd_shell_cmd,
                cmd_shell_help, "Run a shell command.");

    AddMaintCommand("sleep", proc_ignore, 0,
                    class_maint_subcmd_sleep_cmd,
                    class_maint_subcmd_sleep_help, "Pause the debugger.");
    

    // info commands
    AddInfoCommand("status", proc_required, 0,
                   class_info_subcmd_status_cmd,
                   class_info_subcmd_status_help,
                   "Display the status of the debug servers.");

    // set commands
    AddSetCommand("directory", proc_required, 0,
                  class_set_subcmd_directory_cmd,
                  class_set_subcmd_directory_help,
                  "Add directory to search path for source files.");
    AddSetCommand("error", proc_ignore, 0,
                  class_set_subcmd_error_cmd,
                  class_set_subcmd_error_help,
                  "Set warn and error epsilon for floating point comparison.");
    AddSetCommand("environment", proc_ignore, 0,
                  class_set_subcmd_environment_cmd,
                  class_set_subcmd_environment_help,
                  "Set environment variable value for launching applications.");

    // set print commands
    AddSetPrintCommand(
        "entry-frame", proc_ignore, 0,
        class_set_print_subcmd_entryframe_cmd,
        class_set_print_subcmd_entryframe_help,
        "Enable/disable printing of entry frames on launch/attach.");
    AddSetPrintCommand(
        "source-on-step", proc_ignore, 0,
        class_set_print_subcmd_sourceonstep_cmd,
        class_set_print_subcmd_sourceonstep_help,
        "Enable/disable printing of source on step/next.");

    // maint set commands
    AddMaintSetCommand("earlyentry", proc_ignore, 0,
                       class_maint_set_subcmd_earlyentry_cmd,
                       class_maint_set_subcmd_earlyentry_help,
                       "Enable early entry mode.");
    AddMaintSetCommand(
        "unsafe", proc_ignore, 0, class_maint_set_subcmd_unsafe_cmd,
        class_maint_set_subcmd_unsafe_help, "Enable unsafe functionality.");
    AddMaintSetCommand(
        "sync", proc_ignore, 0, class_maint_set_subcmd_sync_cmd,
        class_maint_set_subcmd_sync_help, "Enable synchronous event mode.");

    // show commands
    AddShowCommand("directories", proc_required, 0,
                   class_show_subcmd_directories_cmd,
                   class_show_subcmd_directories_help,
                   "Show current search path for finding source files.");
    AddShowCommand(
        "error", proc_ignore, 0, class_show_subcmd_error_cmd,
        class_show_subcmd_error_help,
        "Show warn and error epsilon for floating point comparison.");
    AddShowCommand("environment", proc_ignore, 0,
                   class_show_subcmd_environment_cmd,
                   class_show_subcmd_environment_help,
                   "Show environment variables that were set.");
    AddShowCommand(
        "version", proc_ignore, 0, class_show_subcmd_version_cmd,
        class_show_subcmd_version_help, "Show the debugger version number.");

    // show print commands
    AddShowPrintCommand("entry-frame", proc_ignore, 0,
                        class_show_print_subcmd_entryframe_cmd,
                        class_show_print_subcmd_entryframe_help,
                        "Show setting of print entry-frame mode.");
    AddShowPrintCommand("source-on-step", proc_ignore, 0,
                        class_show_print_subcmd_sourceonstep_cmd,
                        class_show_print_subcmd_sourceonstep_help,
                        "Show setting of print source-on-step mode.");

    // maint show commands
    AddMaintShowCommand("earlyentry", proc_ignore, 0,
                        class_maint_show_subcmd_earlyentry_cmd,
                        class_maint_show_subcmd_earlyentry_help,
                        "Show setting of early entry mode.");
    AddMaintShowCommand(
        "unsafe", proc_ignore, 0, class_maint_show_subcmd_unsafe_cmd,
        class_maint_show_subcmd_unsafe_help, "Show setting of unsafe mode.");
    AddMaintShowCommand("sync", proc_ignore, 0,
                        class_maint_show_subcmd_sync_cmd,
                        class_maint_show_subcmd_sync_help,
                        "Show setting of synchronous event mode.");

    // unset commands
    AddUnsetCommand("environment", proc_ignore, 0,
                    class_unset_subcmd_environment_cmd,
                    class_unset_subcmd_environment_help,
                    "Remove a previously set environment variable.");
}
