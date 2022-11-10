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
#include <future>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

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

namespace {
// Class for creating a sub-shell with pipe connections and a asynchronous reading thread
class SubShell {
public:
    SubShell();
    ~SubShell();
    //void send(int argc, char **argv);
    void send(const std::string& command);
private:
    bool init();
    void close();

    static void readThread(int fid);

    int m_shellPid = -1;
    int m_toShell = -1;
    int m_fromShell = -1;
    std::future<void> m_readThread;
};

// gdb4hpc level commands to handle a shell as a mode
class ShellMode {
public:
    static void init();
    static void enter();
private:
    static void notfound(const char *cmd) { CmdNotFound(cmd); }
    static void ambig(const char *cmd) { AmbiguousAltCmd(m_cmds, cmd); }
    static void failed() { cli_dec_prompt(); SetCommands(m_oldCmds); }

    static int sendToShell(rec_set_t *rec_set, int argc, char**argv);
    static int exit(rec_set_t* rec_set, int argc, char **argv);
    static int suspend(rec_set_t* rec_set, int argc, char **argv);

    static cmdlist_t *m_cmds;
    static cmdlist_t *m_oldCmds;

    static std::unique_ptr<SubShell> m_subshell;
};
}

static int cmd_shell_cmd(rec_set_t *rec_set, int argc, char **argv) {
    // If remote flag, run shell on remote nodes and return
    // Otherwise run locally
    if (argc < 2) {
        ShellMode::enter();
        return 0;
    }
    
    if (argc < 2) {
        PrintOutError("Usage shell <command>\n");
        return 0;
    }
            
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
    auto cmd = ArgsToString(&argv[1], argc - 1);
    cmd += " 2>&1";

    // Open the command for reading
    auto cmd_out = popen(cmd.c_str(), "r");
    if (!cmd_out) {
        PrintOutError("Failed to run command.\n");
        return 0;
    }

    char line[BUFSIZ];
    while (fgets(line, BUFSIZ, cmd_out)) {
        PrintOutMsg("%s", line);
    }
    
    pclose(cmd_out);
    return 1;
}

static int cmd_pipe_cmd(rec_set_t *rec_set, int argc, char **argv) {

    if (argc < 2) {
        PrintOutError("No command given.\n");
        return 0;
    }

    int argIdx = 1;
    auto delim = "|";
    if (strcmp(argv[1],"-d") == 0) {
        if (argc > 2) {
            delim = argv[2];
            argIdx = 3;
        } else {
            PrintOutError("-d needs a delimiter.\n");
            return 0;
        }
    }

    auto gdbCommand = std::string{};
    auto shellCommand = std::string{};

    // | or delim could show up in the *middle* of an argument, so scan each 
    // argument
    for ( ; argIdx < argc; ++argIdx) {
        auto arg = argv[argIdx];
        if (auto d = strstr(arg, delim)) {
            if (d != arg) gdbCommand.append(arg, size_t(d-arg));
            shellCommand = std::string{d+strlen(delim)};
            break;
        } else {
            gdbCommand.append(arg);
            gdbCommand += " ";
        }
    }
    for (++argIdx; argIdx < argc; ++argIdx) {
        if (!shellCommand.empty()) shellCommand += " ";
        shellCommand += argv[argIdx];
    }

    if (gdbCommand.empty()) {
        PrintOutError("Gdb command is required before %s.\n", delim);
        return 0;
    } else if (!gdbCommand.empty() && gdbCommand.back() == ' ') {
        gdbCommand.pop_back();
    }

    if (shellCommand.empty()) {
        PrintOutError("Shell command is required after %s.\n", delim);
    }

    auto piper = cli_pipe(shellCommand);

    if (!piper.ok()) {
        // Piper down!!
        PrintOutError("Could not execute pipe command: %s.\n",
                      piper.getErr().c_str());
        return 0;
    }

    ExecuteCommand(gdbCommand.c_str());

    return 1;
}

void ShellMode::init() {
    m_cmds = NewCommandList(notfound, ambig, failed);
    AddAltCommand(m_cmds, "_default_cmd", nullptr, proc_optional, 0,
                  sendToShell, dummy_help, "");
    AddAltCommand(m_cmds, "exit", nullptr, proc_optional, 0,
                  exit, dummy_help, "");
    AddAltCommand(m_cmds, "suspend", nullptr, proc_optional, 0,
                  suspend, dummy_help, "");
    SetDefaultCommand(m_cmds, "_default_cmd");
}

cmdlist_t *ShellMode::m_cmds = nullptr;
cmdlist_t *ShellMode::m_oldCmds = nullptr;
std::unique_ptr<SubShell> ShellMode::m_subshell;

int ShellMode::sendToShell(rec_set_t *rec_set, int argc, char**argv) {
    if (!m_subshell)
        m_subshell.reset(new SubShell);

    auto msg = ArgsToString(argv, argc);
    m_subshell->send(msg);
    // wait a little bit to give the output a chance to finish,
    // otherwise the prompt can get messed up
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    return 1;
}

// Start shell mode
void ShellMode::enter()
{
    PrintOutMsg("Entering shell mode, use exit or suspend to leave.\n");
    m_oldCmds = SetCommands(m_cmds);
    cli_inc_prompt();
}

// Leave shell mode
int ShellMode::exit(rec_set_t *rec_set, int argc, char**argv) {
    PrintOutMsg("\nExiting shell mode.\n");
    m_subshell.reset();
    cli_dec_prompt();
    SetCommands(m_oldCmds);
    return 1;
}

int ShellMode::suspend(rec_set_t *rec_set, int argc, char**argv) {
    PrintOutMsg("\nReturning to gdb4hpc.\n");
    cli_dec_prompt();
    SetCommands(m_oldCmds);
    return 1;
}


SubShell::SubShell() {}
SubShell::~SubShell() { try { close(); } catch (...) {}  }

void SubShell::close() {
    if (m_shellPid <= 0) return;

    // try to exit the shell gracefully
    send("exit");

    // if we don't wait for the sub-shell process, it will stick around as zombie
    int iters=4;
    for (int iter=1; ::waitpid(m_shellPid, nullptr, WNOHANG)==0 && iter<=iters; ++iter) {
        if (iter < iters) ::sleep(1);
        else ::kill(m_shellPid, SIGKILL);
    }

    m_shellPid = m_toShell = m_fromShell = -1;
    if (m_readThread.valid()) {
        m_readThread.wait();
    }
}

// send a command to the sub-shell process.   Any output is picked up the
// readThread and printed to the console
void SubShell::send(const std::string& command) {
    if (m_toShell < 0) {
        init();
    }
    if (m_toShell < 0) {
        PrintOutError("sub shell is not connected\n");
        return;
    }

    ::write(m_toShell, command.c_str(), command.size());
    char lf = '\n';
    ::write(m_toShell, &lf, 1);
}

bool SubShell::init() {
    char* shellCmd = getenv("SHELL");
    if (!shellCmd) {
        PrintOutError("$SHELL is not defined.\n");
        return false;
    }

    // create the pipes the flow is
    // gdb4hpc -> left -> subshell -> right -> readThread -> gdb4hpc output
    int leftPipes[2], rightPipes[2];
    if (::pipe(leftPipes) < 0) {
        PrintOutError("Pipe creation failed.\n");
        return false;
    }

    if (::pipe(rightPipes) < 0) {
        ::close(leftPipes[0]);
        ::close(leftPipes[1]);
        PrintOutError("Pipe creation failed.\n");
        return false;
    }

    if (auto cmdPid = fork(); cmdPid < 0) {
        PrintOutError("fork failed.\n");
        ::close(leftPipes[0]);  ::close(leftPipes[1]);
        ::close(rightPipes[0]); ::close(rightPipes[1]);
        return false;
    } else if (cmdPid == 0) {
        // child will be the sub-shell set up the communication and
        // start up the subshell process
        ::close(leftPipes[1]);
        ::close(rightPipes[0]);
        ::dup2(leftPipes[0], STDIN_FILENO);
        ::dup2(rightPipes[1], STDOUT_FILENO);
        ::dup2(rightPipes[1], STDERR_FILENO);
        char* args[] = { shellCmd, nullptr };
        ::execvp(shellCmd, args);
        return false;
    } else {
        // parent, I should have pipes to send and receive 
        m_shellPid = cmdPid;

        ::close(leftPipes[0]);
        ::close(rightPipes[1]);
        m_toShell = leftPipes[1];
        m_fromShell = rightPipes[0];

        if (::fcntl(m_fromShell, F_SETFL, O_NONBLOCK) < 0) {
            PrintOutError("could not set pipe to O_NONBLOCK\n");
            return false;
        }

        // create a thread to wait for output from the subshell
        m_readThread = std::async( std::launch::async, readThread, m_fromShell);
        return true;
    }
}

// process that runs in a separate thread reading the results from the sub-shell
void SubShell::readThread(int fid) {
    char line[BUFSIZ];
    for (int nb; (nb = read(fid, line, BUFSIZ-1)) != 0; ) {
        if (nb > 0) { 
            line[nb] = '\0';
            PrintOutMsg(line);
        } else if (nb < 0 && errno != EAGAIN) {
            PrintOutError("read error\n");
            return;
        } else {
            // There may be a more elegant way than just checking for output 10
            // time second, but I had trouble getting select to work without
            // blocking.
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
    return;
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
        PrintOutMIResult(MIResult::DONE, "value=\"(gdb) \"\n");
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
            PrintOutMsg("Copying output to %s.\n", logger.getFile().c_str());
        } else {
            logger.setEnabled(false);
            PrintOutMsg("Logging disabled.\n");
        }
    
    }

    // change logging output file
    else if(strcmp(argv[1], "file") == 0){
        if(argv[2]){
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
    Client.keep_going=true;

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
        return 0;
    }

    if (!SetProgramOutputFile(tty)) {
        PrintOutError("Could not open %s for output.\n", tty ? tty : "default location");
        return 0;
    }

    PrintOutOk();
    return 1;
}

static int cmd_interpreter_exec_cmd(rec_set_t* rec_set, int argc, char** argv) {
    if (argc < 3) {
        PrintOutError("Usage %s console|mi <command>.\n", argv[0]);
        return 0;
    }

    if (strcmp(argv[1],"console") != 0) {
        PrintOutError("Only console is supported.\n");
        return 0;
    }

    if (!Client.MI_mode) {
        PrintOutError("Only supported from MI mode.\n");
        return 0;
    }

    Client.MI_mode = false;

    auto command = std::string{};
    if (argc > 3 || argv[2][0] != '"') {
        command = ArgsToString(argv+2, argc-2);
    } else {
        auto len = strlen(argv[2]);
        command = std::string{argv[2]+1, std::max(size_t(2),len)-2};
    }
    mi_log("\nExecuting %s\n", command.c_str());
    {
        PrintOutCliToMI mi;         // toggles output printing
        ExecuteCommand(command.c_str());
    }

    Client.MI_mode = true;

    PrintOutOk();

    return 1;
}

void InitCmd_env(void) {
    // regular commands
    AddCommand("quit", "q", proc_ignore, 0, cmd_quit_cmd,
               cmd_quit_help, "Exit the debugger.");
    
    AddCommand("shell", nullptr, proc_optional, 0,
                cmd_shell_cmd,
                cmd_shell_help, "Run a shell command.");

    AddCommand("pipe", nullptr, proc_optional, 0,
               cmd_pipe_cmd,
               cmd_pipe_help, "Send the output of a gdb command to a shell command.");


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

    ShellMode::init();
}
