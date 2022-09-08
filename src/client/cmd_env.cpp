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

static int cmd_set_environment_cmd(rec_set_t *rec_set, int argc,
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

static int cmd_unset_environment_cmd(rec_set_t *rec_set, int argc,
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

static int cmd_show_environment_cmd(rec_set_t *rec_set, int argc,
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

static int cmd_maint_echo_cmd(rec_set_t *rec_set, int argc,
                                       char **argv) {
    char *echo_str;

    if (argc < 2) return 0;

    // create the message string to echo
    echo_str = ArgsToBuf(&argv[1], argc - 1);

    PrintOutMsg("%s\n", echo_str);

    Free(echo_str);

    return 1;
}

static int cmd_maint_sleep_cmd(rec_set_t *rec_set, int argc,
                                        char **argv) {
    int time = 5;

    if (argc > 1) time = atoi(argv[1]);
    sleep(time);

    return 1;
}

static int shell_remote(rec_set_t *rec_set, int argc, char **argv){
    auto rep_set = DbgShell(rec_set, argc, argv);
    ProcessErrorInRepSet(rep_set);
    for (auto&& reply : rep_set){
        auto ev = reply->rep_event->ev_event;
        if(ev != DBGEV_STRING){
            PrintOutError("unexpected result type.\n");
            continue;
        }
        auto is = std::istringstream{reply->rep_event->ev_u.ev_u_string};
        auto line = std::string{};
        while(std::getline(is, line)){
            PrintMsgWithSetInfo(reply->rep_di.c_str(), reply->rep_group.get(), "%s\n", line.c_str());
        }
    }
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

    // If remote flag, run shell on remote nodes and return
    // Otherwise run locally
    if(!strcmp(argv[1], "-r")) {
        if (argc > 2) {
            return shell_remote(rec_set, argc-2, argv+2);
        } else {
            PrintOutError("No command given for remote shell.\n");
            return 0;
        }
    }

    // Local implementation below

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

static int cmd_maint_set_earlyentry_cmd(rec_set_t *rec_set, int argc,
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

static int cmd_maint_show_earlyentry_cmd(rec_set_t *rec_set, int argc,
                                                  char **argv) {
    if (Client.early_entry)
        PrintOutMsg("Early entry mode enabled.\n");
    else
        PrintOutMsg("Early entry mode disabled.\n");

    return 1;
}

static int cmd_maint_set_unsafe_cmd(rec_set_t *rec_set, int argc,
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

static int cmd_maint_show_unsafe_cmd(rec_set_t *rec_set, int argc,
                                              char **argv) {
    if (Client.unsafe)
        PrintOutMsg("Unsafe mode enabled.\n");
    else
        PrintOutMsg("Unsafe mode disabled.\n");

    return 1;
}

static int cmd_maint_set_sync_cmd(rec_set_t *rec_set, int argc,
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

static int cmd_maint_show_sync_cmd(rec_set_t *rec_set, int argc,
                                            char **argv) {
    if (Client.sync_wait)
        PrintOutMsg("Synchronous event mode enabled.\n");
    else
        PrintOutMsg("Synchronous event mode disabled.\n");

    return 1;
}

static int cmd_info_status_cmd(rec_set_t *rec_set, int argc,
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
                PrintMsgWithSetInfo(reply->rep_di.c_str(), reply->rep_group.get(),
                                    "The debugger is initialising.\n");
                break;

            case DBGSTAT_WAITING:
                PrintMsgWithSetInfo(reply->rep_di.c_str(), reply->rep_group.get(),
                                    "The debugger is waiting for a command.\n");
                break;

            case DBGSTAT_RUNNING:
                PrintMsgWithSetInfo(reply->rep_di.c_str(), reply->rep_group.get(),
                                    "The application is running.\n");
                break;

            case DBGSTAT_STOPPED:
                PrintMsgWithSetInfo(reply->rep_di.c_str(), reply->rep_group.get(),
                                    "The application is stopped.\n");
                break;

            case DBGSTAT_STOPPED_IN_THREAD:
                PrintMsgWithSetInfo(reply->rep_di.c_str(), reply->rep_group.get(),
                                    "The application is stopped in a thread while in non-stop mode.\n");
                break;

            case DBGSTAT_EXIT:
                PrintMsgWithSetInfo(
                    reply->rep_di.c_str(), reply->rep_group.get(),
                    "The application is at an exit breakpoint.\n");
                break;

            case DBGSTAT_DBGERR:
            case DBGSTAT_INTERR:
                PrintErrorWithSetInfo(
                    reply->rep_di.c_str(), reply->rep_group.get(),
                    "The debugger is reporting an error status.\n");
                break;
        }
    }

    FreeRepSet(rep_set);

    return 1;
}

static int cmd_set_directory_cmd(rec_set_t *rec_set, int argc,
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

        PrintMsgWithSetInfo(reply->rep_di.c_str(), reply->rep_group.get(),
                            "Source directory search path set to: %s\n",
                            new_path);
    }

    FreeRepSet(rep_set);

    return 1;
}

static int cmd_show_directories_cmd(rec_set_t *rec_set, int argc,
                                             char **argv) {
    rep_set_t *rep_set;
    char *path;

    rep_set = DbgGetSourcePath(rec_set);

    ProcessErrorInRepSet(rep_set);

    for (auto&& reply : rep_set) {
        path = EVENT_SOURCE_PATH(reply->rep_event);

        PrintMsgWithSetInfo(reply->rep_di.c_str(), reply->rep_group.get(),
                            "Source directories searched: %s\n", path);
    }

    FreeRepSet(rep_set);

    return 1;
}

static int cmd_show_version_cmd(rec_set_t *rec_set, int argc,
                                         char **argv) {
    PrintOutMsg("Version %s\n", GUARD_VERSION);
    return 1;
}

static int class_show_sub_cmd_prompt_cmd(rec_set_t *rec_set, int argc,
                                        char **argv) {
    if (!Client.MI_mode) {
        PrintOutMsg("dbg all>\n");
    } else {
        PrintOutMIResult(MIResult::DONE, "value=\"(gdb)\"\n");
    }
    return 1;
}


static int cmd_set_error_cmd(rec_set_t *rec_set, int argc,
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

static int cmd_show_error_cmd(rec_set_t *rec_set, int argc,
                                       char **argv) {
    PrintOutMsg("Warning epsilon: ");
    PrintOutMsg("%.16lg", Client.cmp_lower_eps);
    PrintOutMsg("\nError epsilon: ");
    PrintOutMsg("%.16lg", Client.cmp_upper_eps);
    PrintOutMsg("\n");

    return 1;
}

// Check if given string is found in vector of strings
static bool check_string_in_vector(std::vector<std::string> v, std::string s){
    // return true if string is present
    return (std::find(v.begin(), v.end(), s) != v.end());
}

static bool get_on_off(char *val){
    return (strcmp(val, "on") == 0);
}

// Set output redirection for logging purposes
static int cmd_set_logging_cmd(rec_set_t *rec_set, int argc, char **argv){

    // Constant vector of valid arguments
    const static std::vector<std::string> valid_args = {"on", "off"};
    // Reference to the cli_logging_t which allows us to modify logging behavior
    cli_logging_t& logger = GetLogging();
    static bool enabled_bool;
    static bool invalid_arg_bool;

    if(argc < 2 || argc > 3){
        PrintOutError("Invalid number of arguments!\n");
        return 0;
    }

    if(argc == 3){
        enabled_bool = get_on_off(argv[2]);
        invalid_arg_bool = !check_string_in_vector(valid_args, argv[2]);
        // Check that argv[1] isnt file, handles wrong arguments independently
        if(invalid_arg_bool && strcmp(argv[1], "file")){
            PrintOutError("Error: Expected usage \"set logging %s [on | off]\"\n", argv[1]);
            return 0;
        }
    }
        
    // enable or disable logging functionality
    if(strcmp(argv[1], "enabled") == 0){
        
        // default to enabling logging if argument isnt passed
        if(!argv[2] || enabled_bool){
            logger.setEnabled(true);
            PrintOutMsg("Copying output to gdb4hpc.txt.\n");
        } else {
            logger.setEnabled(false);
            PrintOutMsg("Logging disabled.\n");
        }
    
    }

    // change logging output file
    else if(strcmp(argv[1], "file") == 0){
        if(argv[2]){
            if(!logger.isEnabled()){
                PrintOutError("Error: Please enable logging first.\n");
                return 0;
            }
            logger.setFile(argv[2]);
        } else {
            PrintOutError("Error: Expected usage \"set logging file <file>\".\n");
        }
    }

    // set logging to overwrite current logfile instead of append
    else if(strcmp(argv[1], "overwrite") == 0){
        if(!argv[2] || enabled_bool){
            logger.setOverwrite(true);
        } else {
            logger.setOverwrite(false);
        }
    }

    // Set stdout to entirely redirect to log file and not print to terminal
    else if(strcmp(argv[1], "redirect") == 0){
        if(!argv[2] || enabled_bool){
            if(!logger.isEnabled()){
                PrintOutError("Error: Please enable logging first.\n");
                return 0;
            }
            PrintOutMsg("Redirecting output to logfile.\n");
            logger.setRedirect(true);
        } else {
            logger.setRedirect(false);
        }
    } else {
        // if invalid command
        PrintOutMsg("Undefined Set logging command \"%s\": . Try \"help set logging\"\n", argv[1]);
        return 0;
    }
        
    return 1;
}

static int cmd_set_print_entryframe_cmd(rec_set_t *rec_set, int argc,
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

static int cmd_show_print_entryframe_cmd(rec_set_t *rec_set, int argc,
                                                  char **argv) {
    if (Client.print_entry_frame)
        PrintOutMsg("print entry-frame enabled.\n");
    else
        PrintOutMsg("print entry-frame disabled.\n");

    return 1;
}

static int cmd_set_print_sourceonstep_cmd(rec_set_t *rec_set, int argc,
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

static int cmd_show_print_sourceonstep_cmd(rec_set_t *rec_set, int argc,
                                                  char **argv) {
    if (Client.print_source_on_step)
        PrintOutMsg("print source-on-step enabled.\n");
    else
        PrintOutMsg("print source-on-step disabled.\n");

    return 1;
}

static int class_new_ui_cmd(rec_set_t* rec_set, int argc, char**argv) {
    if (argc != 3) {
        PrintOutError("Wrong number of arguments to new-ui interpreter tty.\n");
        return 0;
    }

    if (strcmp(argv[1], "mi2") != 0 && strcmp(argv[1], "mi") != 0) {
        PrintOutError("Only mi ui is supported.\n");
        return 0;
    }

    auto tty = argv[2];
    auto cli_args = NewCLIArgs(CLI_FILE_STREAM, tty, tty, tty);
    SetNewCLIStream(CLI_FILE_STREAM, interpreter_type::MI, cli_args, 0);

    // Set back to the default stream since this make be part of a sequence of
    // -iex arguments
    SetDefaultCLIStream();

    return 1;
}

static int cmd_set_inferior_tty_cmd(rec_set_t* rec_set, int argc, char** argv) {
    mi_log("in cmd_set_inferior_tty_cmd\n");
    
    const char* tty = nullptr;
    if (argc == 2) {
        tty = argv[1];
    } else if (argc != 1) {
        PrintOutError("Unexpected arguments, expected [TTY].\n");
        return 1;
    }

    if (!SetProgramOutputFile(tty)) {
        PrintOutError("Could not open %s for output.\n", tty ? tty : "default location");
        return 1;
    }

    PrintOutOk();
    return 0;
}

static int cmd_interpreter_exec_cmd(rec_set_t* rec_set, int argc, char** argv) {
    if (argc < 3) {
        PrintOutError("Usage %s console|mi <command>.\n", argv[0]);
        return 1;
    }

    if (strcmp(argv[1],"console") != 0) {
        PrintOutError("Only console is supported.\n");
        return 1;
    }

    if (!Client.MI_mode) {
        PrintOutError("Only supported from MI mode.\n");
        return 1;
    }

    Client.MI_mode = false;

    PrintOutMsg("~\"");

    auto command = std::string{};
    if (argc > 3 || argv[2][0] != '"') {
        command = ArgsToString(argv+2, argc-2);
    } else {
        auto len = strlen(argv[2]);
        command = std::string{argv[2]+1, std::max(size_t(2),len)-2};
    }
    ExecuteCommand(command.c_str());

    PrintOutMsg("\"\n");

    Client.MI_mode = true;

    PrintOutOk();

    return 0;
}

void InitCmd_env(void) {
    // regular commands
    AddCommand("quit", "q", proc_ignore, 0, cmd_quit_cmd,
               cmd_quit_help, "Exit the debugger.");
    
    AddCommand("shell", nullptr, proc_optional, 0,
                cmd_shell_cmd,
                cmd_shell_help, "Run a shell command.");

    AddCommand("interpreter-exec", nullptr, proc_optional, 0,
               cmd_interpreter_exec_cmd, nullptr, nullptr); // undocumented

    // maint commands
    AddMaintCommand("echo", proc_ignore, 0,
                    cmd_maint_echo_cmd,
                    cmd_maint_echo_help, "Echo a string.");

    AddMaintCommand("system", proc_ignore, 0,
                cmd_shell_cmd,
                cmd_shell_help, "Run a shell command.");

    AddMaintCommand("sleep", proc_ignore, 0,
                    cmd_maint_sleep_cmd,
                    cmd_maint_sleep_help, "Pause the debugger.");
    

    // info commands
    AddInfoCommand("status", proc_required, 0,
                   cmd_info_status_cmd,
                   cmd_info_status_help,
                   "Display the status of the debug servers.");

    // set commands
    AddSetCommand("directory", proc_required, 0,
                  cmd_set_directory_cmd,
                  cmd_set_directory_help,
                  "Add directory to search path for source files.");
    AddSetCommand("error", proc_ignore, 0,
                  cmd_set_error_cmd,
                  cmd_set_error_help,
                  "Set warn and error epsilon for floating point comparison.");
    AddSetCommand("environment", proc_ignore, 0,
                  cmd_set_environment_cmd,
                  cmd_set_environment_help,
                  "Set environment variable value for launching applications.");
    AddSetCommand("logging", proc_ignore, 0,
                  cmd_set_logging_cmd,
                  cmd_set_logging_help,
                  "Redirect client side output.");

    // set print commands
    AddSetPrintCommand(
        "entry-frame", proc_ignore, 0,
        cmd_set_print_entryframe_cmd,
        cmd_set_print_entryframe_help,
        "Enable/disable printing of entry frames on launch/attach.");
    AddSetPrintCommand(
        "source-on-step", proc_ignore, 0,
        cmd_set_print_sourceonstep_cmd,
        cmd_set_print_sourceonstep_help,
        "Enable/disable printing of source on step/next.");

    // maint set commands
    AddMaintSetCommand("earlyentry", proc_ignore, 0,
                       cmd_maint_set_earlyentry_cmd,
                       cmd_maint_set_earlyentry_help,
                       "Enable early entry mode.");
    AddMaintSetCommand(
        "unsafe", proc_ignore, 0, cmd_maint_set_unsafe_cmd,
        cmd_maint_set_unsafe_help, "Enable unsafe functionality.");
    AddMaintSetCommand(
        "sync", proc_ignore, 0, cmd_maint_set_sync_cmd,
        cmd_maint_set_sync_help, "Enable synchronous event mode.");

    // show commands
    AddShowCommand("directories", proc_required, 0,
                   cmd_show_directories_cmd,
                   cmd_show_directories_help,
                   "Show current search path for finding source files.");
    AddShowCommand(
        "error", proc_ignore, 0, cmd_show_error_cmd,
        cmd_show_error_help,
        "Show warn and error epsilon for floating point comparison.");
    AddShowCommand("environment", proc_ignore, 0,
                   cmd_show_environment_cmd,
                   cmd_show_environment_help,
                   "Show environment variables that were set.");
    AddShowCommand(
        "version", proc_ignore, 0, cmd_show_version_cmd,
        cmd_show_version_help, "Show the debugger version number.");

    AddShowCommand(
        "prompt", proc_ignore, 0, class_show_sub_cmd_prompt_cmd,
        cmd_show_prompt_help, "Show the prompt.");

    // show print commands
    AddShowPrintCommand("entry-frame", proc_ignore, 0,
                        cmd_show_print_entryframe_cmd,
                        cmd_show_print_entryframe_help,
                        "Show setting of print entry-frame mode.");
    AddShowPrintCommand("source-on-step", proc_ignore, 0,
                        cmd_show_print_sourceonstep_cmd,
                        cmd_show_print_sourceonstep_help,
                        "Show setting of print source-on-step mode.");

    // maint show commands
    AddMaintShowCommand("earlyentry", proc_ignore, 0,
                        cmd_maint_show_earlyentry_cmd,
                        cmd_maint_show_earlyentry_help,
                        "Show setting of early entry mode.");
    AddMaintShowCommand(
        "unsafe", proc_ignore, 0, cmd_maint_show_unsafe_cmd,
        cmd_maint_show_unsafe_help, "Show setting of unsafe mode.");
    AddMaintShowCommand("sync", proc_ignore, 0,
                        cmd_maint_show_sync_cmd,
                        cmd_maint_show_sync_help,
                        "Show setting of synchronous event mode.");

    // unset commands
    AddUnsetCommand("environment", proc_ignore, 0,
                    cmd_unset_environment_cmd,
                    cmd_unset_environment_help,
                    "Remove a previously set environment variable.");

    // MI related commands - not documented
    AddCommand("new-ui", nullptr, proc_ignore, 0, class_new_ui_cmd, nullptr, nullptr);

    AddSetCommand("inferior-tty", proc_ignore, 0, cmd_set_inferior_tty_cmd,
                  nullptr,  "set the tty for the application i/o");
               
}
