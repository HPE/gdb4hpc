/******************************************************************************\
 * Command processing routines.
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * Copyright 2010-2020,2022 Hewlett Packard Enterprise Development LP.
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

#include <deque>
#include <string>

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "client.h"
#include "useful.h"
#include "bitset.h"
#include "dbgevent.h"
#include "mi_funcs.h"
/*
** Internal global variables
*/
static cmdlist_t *CurrentCmds;  /* Current command list */
static list_t *help_class_list; /* Class Command help lookup */
static cmdlist_t *Cmds;         /* Main command list */
static std::deque<std::string> pendingCommandQueue{}; /* Commands queued, waiting to be executed */

/*
** Internal function prototypes
*/
static cmdtype_t *_addcmd(list_t *, const char *, const char *, cmdptype_t, int,
                          cmdfunc_t, const char *const[], const char *);
static cmdhelp_t *_addhelpclasscmd(const char *, cmdtype_t *(*)(char *));
static void _alphabetize_cmd_list(list_t *);
static int _docmd(cmdlist_t *, dbgprocset_t *, cmdtype_t *, int, char *[], int);
static cmdtype_t *_findcmd(cmdlist_t *, const char *);
static void _ambigcmd(list_t *, const char *);
static int _apply_command(dbgprocset_t *, cmdtype_t *, int, char *[]);

/*
 * Command list functions
 */

cmdlist_t *NewCommandList(void (*notfound)(const char *),
                          void (*ambig)(const char *), void (*failed)(void)) {
    cmdlist_t *c;

    c = (cmdlist_t *)Alloc(sizeof(cmdlist_t));
    c->cmd_list = NewList();
    c->cmd_notfound = notfound;
    c->cmd_ambig = ambig;
    c->cmd_failed = failed;
    c->cmd_default = NULL;

    return c;
}

void DestroyCommandList(cmdlist_t *c) {
    if (c == (cmdlist_t *)NULL) return;
    DestroyList(c->cmd_list, Free);
    Free(c);
}

/*
 * AddCommand functions
 */

static cmdtype_t *_addcmd(list_t *l, const char *name, const char *alias,
                          cmdptype_t pt, int sub, cmdfunc_t cmd,
                          const char *const help[], const char *desc) {
    cmdtype_t *c;

    c = (cmdtype_t *)Alloc(sizeof(cmdtype_t));
    c->c_name = name;
    c->c_alias = alias;
    c->c_cmd = cmd;
    c->c_help = (char **)help;
    c->c_desc = desc;
    c->c_ptype = pt;
    c->c_hassub = sub;

    AddToList(l, (void *)c);

    return c;
}

static cmdhelp_t *_addhelpclasscmd(const char *name,
                                   cmdtype_t *(*find)(char *)) {
    cmdhelp_t *c_help;

    c_help = (cmdhelp_t *)Alloc(sizeof(cmdhelp_t));
    c_help->c_name = name;
    c_help->c_find = (cmdtype_t * (*)(char *)) find;

    AddToList(help_class_list, (void *)c_help);

    return c_help;
}

static void _alphabetize_cmd_list(list_t *l) {
    int swap;
    cmdtype_t *c1;
    cmdtype_t *c2;

    // This is a simple bubble sort implementation
    do {
        swap = 0;

        SetList(l);

        // set c1 and c2 to the first and second elements in the list
        c1 = (cmdtype_t *)GetListElement(l);
        c2 = (cmdtype_t *)GetListElement(l);

        // need to ensure c2 is not null, if so we are done
        while (c2 != (cmdtype_t *)NULL) {
            if (strcmp(c1->c_name, c2->c_name) > 0) {
                ++swap;
                SwapWithNextNeighbor(l, (void *)c1);
                // no need to update c1 since it is now in c2's spot in the list
            } else {
                // update c1 to point at c2 since there was no swap
                c1 = c2;
            }

            // update c2 to point at the next element in the list
            c2 = (cmdtype_t *)GetListElement(l);
        }
    } while (swap != 0);
}

void AddCommand(const char *name, const char *alias, cmdptype_t pt, int sub,
                cmdfunc_t cmd, const char *const help[], const char *desc) {
    _addcmd(Cmds->cmd_list, name, alias, pt, sub, cmd, help, desc);
}

void AddAltCommand(cmdlist_t *cmds, const char *name, const char *alias,
                   cmdptype_t pt, int sub, cmdfunc_t cmd,
                   const char *const help[], const char *desc) {
    _addcmd(cmds->cmd_list, name, alias, pt, sub, cmd, help, desc);
}

void AddClassCommand(const char *name, const char *alias, cmdptype_t pt,
                     int sub, cmdfunc_t cmd, cmdtype_t *(*find)(char *),
                     const char *const help[], const char *desc) {
    // register the class command with the regular command list
    _addcmd(Cmds->cmd_list, name, alias, pt, sub, cmd, help, desc);
    // register the class command name in the help list
    _addhelpclasscmd(name, find);
}

void AddClassSubCommand(list_t *l, const char *name, cmdptype_t pt, int sub,
                        cmdfunc_t cmd, const char *const help[],
                        const char *desc) {
    _addcmd(l, name, NULL, pt, sub, cmd, help, desc);
}

/*
 * DoCommand functions
 */

static int _docmd(cmdlist_t *cmd, dbgprocset_t *s, cmdtype_t *c, int argc,
                  char *argv[], int isSubCmd) {
    int res = 0;

    if (!isSubCmd && c->c_hassub)
        res = (*c->c_cmd)(NULL, argc, argv);
    else {
        switch (c->c_ptype) {
            case proc_required:
                if (s == (dbgprocset_t *)NULL || s->s_nproc <= 0) {
                    PrintOutError("No process, use launch/attach first.\n");
                    res = 0;
                } else
                    res = _apply_command(s, c, argc, argv);
                break;

            case proc_optional:
                if (s == (dbgprocset_t *)NULL || s->s_nproc <= 0)
                    res = (*c->c_cmd)(NULL, argc, argv);
                else
                    res = _apply_command(s, c, argc, argv);

                break;

            case proc_ignore:
                res = (*c->c_cmd)(NULL, argc, argv);
                break;

            case proc_optional_mi:
                if (!Client.MI_mode && (!s || s->s_nproc <= 0)) {
                    PrintOutError("No process, use launch/attach first.\n");
                    res = 0;
                } else {
                    res = _apply_command(s, c, argc, argv);
                }
                break;
        }
    }

    if (!res && cmd->cmd_failed != (void (*)(void))NULL) cmd->cmd_failed();

    return res;
}

int DoClassSubCommand(cmdlist_t *cmd_l, dbgprocset_t *s, cmdtype_t *c, int argc,
                      char *argv[], int isSubCmd) {
    return _docmd(cmd_l, s, c, argc, argv, isSubCmd);
}

/*
** Apply a command to each process in a process set.
** Print and compare handle blockmap function
*/
static int _apply_command(dbgprocset_t *s, cmdtype_t *c, int argc,
                          char *argv[]) {
    int res = 1;
    dbgstatus_t *stat;

    // Create a new rec_set for this procset
    auto rec_set = CreateRecSetForProcset(s);

    // Call the command
    if (!(*c->c_cmd)(rec_set, argc, argv)) res = 0;

    // If XML mode is enabled, we need to ask the dbgclient layer if the
    // command we just executed changed the status. If it did, we want to
    // add the status information to the xml output. We also check to see
    // if the command caused the inferior to exit. Inferior exit can only be
    // caused by a command.
    if (Client.XML_mode) {
        if (DbgCheckForStatusChange()) {
            // Status has changed. We need to generate updated status
            // information

            // Query the new status from the backends, note that for this we
            // want to grab the location information
            auto rep_set = DbgStatus(rec_set, 1);

            ProcessErrorInRepSet(rep_set);

            // We need to loop over each di in the rec_set and process it one at
            // a time. This is because we need to create a status block for each
            // di.
            for (auto&& recv_ptr : rec_set) {
                // Start a new status block for this di
                auto status_block = XMLStartStatusBlock(recv_ptr->rec_di.handle().c_str());

                // Process each status message and add it to the status block
                for (auto&& reply : rep_set) {
                    // Ensure the di match
                    if (recv_ptr->rec_di.handle() != reply->rep_di) {
                        continue;
                    }

                    // First create the bitset string
                    auto bitset_str = reply->rep_group->toSetString();

                    // Next create the full set string
                    char *set_str = nullptr;
                    Asprintf(&set_str, "%s%s", reply->rep_di.c_str(),
                             bitset_str.c_str());

                    // point at the status data
                    stat = EVENT_STATUS(reply->rep_event);

                    // The XML status that is printed is based on the status
                    // value
                    XMLAddStatus(&status_block, set_str, stat->stat,
                                 stat->stat_loc);

                    // Done with the set_str
                    Free(set_str);
                }

                // Print the entire status block, this will free the string
                XMLPrintStatusBlock(status_block);
            }

            // Free the reply set
            FreeRepSet(rep_set);

            // Reset the status change since we handled it
            DbgResetStatusChange();
        }
    }

    // Done with the rec_set
    FreeRecSet(rec_set);

    return res;
}

/*
 * FindCommand functions
 */

static cmdtype_t *_findcmd(cmdlist_t *cmds, const char *cmd) {
    int cnt = 0;
    cmdtype_t *c;
    cmdtype_t *cp = NULL;

    auto cmdlen = strlen(cmd);
    if (auto slash = strchr(cmd, '/')) cmdlen = (slash-cmd);

    SetList(cmds->cmd_list);

    while ((c = (cmdtype_t *)GetListElement(cmds->cmd_list)) !=
           (cmdtype_t *)NULL) {
        if (c->c_alias && strlen(c->c_alias) == cmdlen && strncmp(cmd, c->c_alias, cmdlen) == 0) return c;

        if (strncmp(cmd, c->c_name, cmdlen) == 0 && c->c_cmd != NULL) {
            cnt++;
            cp = c;
            if (strlen(c->c_name) == cmdlen) {
                cnt = 1;  // an exact match takes precedence over a subset
                break;
            }
        }
    }

    if (cnt > 1) {
        cmds->cmd_ambig(cmd);
        return (cmdtype_t *)NULL;
    }

    if (cnt == 0) {
        // Check to see if there is a default command set, if so return that
        if (cmds->cmd_default != NULL) return cmds->cmd_default;

        cmds->cmd_notfound(cmd);
        return (cmdtype_t *)NULL;
    }

    return cp;
}

static cmdtype_t *FindHelpClassCommand(char **cmd_argv) {
    cmdhelp_t *c;
    char *cmd = cmd_argv[0];

    SetList(help_class_list);

    while ((c = (cmdhelp_t *)GetListElement(help_class_list)) !=
           (cmdhelp_t *)NULL) {
        if (c->c_name != NULL && strcmp(cmd, c->c_name) == 0) break;
    }

    if (c != (cmdhelp_t *)NULL)
        return (*c->c_find)(cmd_argv[1]);
    else
        return _findcmd(CurrentCmds, cmd);
}

cmdtype_t *FindCommand(const char *cmd) { return _findcmd(CurrentCmds, cmd); }

cmdtype_t *FindSubCommand(cmdlist_t *cmd_l, const char *cmd) {
    return _findcmd(cmd_l, cmd);
}

/*
 * functions which define default command not found behavior
 */

void CmdNotFound(const char *cmd) {
    PrintOutError("Undefined command: \"%s\".  Try \"help\".\n", cmd);
}

/*
 * functions which define default ambiguous command behavior
 */

static void _ambigcmd(list_t *cmds, const char *cmd) {
    int cnt = 0;
    cmdtype_t *c;
    list_t *l = NewList();

    SetList(cmds);

    while ((c = (cmdtype_t *)GetListElement(cmds)) != (cmdtype_t *)NULL) {
        if (strncmp(cmd, c->c_name, strlen(cmd)) == 0 && c->c_cmd != NULL) {
            AddToList(l, (void *)c);
            cnt++;
        }
    }

    SetList(l);

    while ((c = (cmdtype_t *)GetListElement(l)) != (cmdtype_t *)NULL) {
        PrintOutError("%s", c->c_name);

        if (--cnt > 0) PrintOutError(", ");
    }

    PrintOutError("\n");

    DestroyList(l, (void (*)(void *))NULL);
}

static void AmbiguousCmd(const char *cmd) {
    PrintOutError("Ambiguous command \"%s\": ", cmd);
    _ambigcmd(CurrentCmds->cmd_list, cmd);
}

void AmbiguousAltCmd(cmdlist_t *cmds, const char *cmd) {
    PrintOutError("Ambiguous command \"%s\": ", cmd);
    _ambigcmd(cmds->cmd_list, cmd);
}

void AmbiguousClassSubCmd(list_t *l, const char *cmd) { _ambigcmd(l, cmd); }

/*
 * functions for getting and setting commands
 */

cmdlist_t *SetCommands(cmdlist_t *cmds) {
    cmdlist_t *old = CurrentCmds;

    CurrentCmds = cmds;
    return old;
}

void SetDefaultCommand(cmdlist_t *cmds, const char *cmd) {
    if (cmds == NULL) return;

    cmds->cmd_default = FindSubCommand(cmds, cmd);
}

/*
 * Help functions defined below
 */

int cmd_help_cmd(rec_set_t *rec_set, int argc, char **argv) {
    cmdtype_t *c;

    if (argc < 2) {
        SetList(CurrentCmds->cmd_list);

        while ((c = (cmdtype_t *)GetListElement(CurrentCmds->cmd_list)) !=
               (cmdtype_t *)NULL)
            if (c->c_cmd != NULL && c->c_desc != NULL)
                PrintOutMsg("%-15s %s\n", c->c_name, c->c_desc);

        return 1;
    }

    if (argc > 2)
        c = FindHelpClassCommand(&argv[1]);
    else
        c = _findcmd(CurrentCmds, argv[1]);

    if (c == (cmdtype_t *)NULL) return 0;

    if (c->c_help == (char **)NULL) return 0;

    PrintHelpEntry(c->c_help);

    return 1;
}

void PrintHelpEntry(char **helpStrArray) {
    char **cur_line;

    if (helpStrArray == NULL) return;

    // print initial newline
    PrintOutMsg("\n");

    // iterate over the array of strings
    cur_line = helpStrArray;
    while (*cur_line) {
        PrintOutMsg("%s\n", *cur_line++);
    }

    // print final newline
    PrintOutMsg("\n");

    return;
}

/*
 * command initialization
 */

void InitCommands() {
    // init the internal command lists
    Cmds = NewCommandList(CmdNotFound, AmbiguousCmd, (void (*)(void))NULL);
    help_class_list = NewList();

    // init the class commands
    InitCmd_class();

    // add the help command
    AddCommand("help", "h", proc_ignore, 0, cmd_help_cmd,
               cmd_help_help, "Display help information about commands.");

    // Begin init of every other command
    InitCmd_assign();
    InitCmd_blockmap();
    InitCmd_break();
    InitCmd_checkpoint();
    InitCmd_comp();
    InitCmd_env();
    InitCmd_iostat();
    InitCmd_frame();
    InitCmd_go();
    InitCmd_script();
    InitCmd_list();
    InitCmd_passthru();
    InitCmd_print();
    InitCmd_proc();
    InitCmd_procset();
    InitCmd_source();
    InitCmd_step();
    InitCmd_thread();
    InitCmd_whatis();
#ifdef USE_PYTHON    
    InitCmd_python();
#endif    


    // alphabetize the command list
    _alphabetize_cmd_list(Cmds->cmd_list);

    // alphabetize the class command lists
    _alphabetize_cmd_list(InfoCmds->cmd_list);
    _alphabetize_cmd_list(MaintCmds->cmd_list);
    _alphabetize_cmd_list(MaintSetCmds->cmd_list);
    _alphabetize_cmd_list(MaintShowCmds->cmd_list);
    _alphabetize_cmd_list(SetCmds->cmd_list);
    _alphabetize_cmd_list(ShowCmds->cmd_list);
    _alphabetize_cmd_list(UnsetCmds->cmd_list);
    _alphabetize_cmd_list(UsageCmds->cmd_list);

    (void)SetCommands(Cmds);
};

void DestroyCommands(void) {
    // destroy the internal help class list
    if (help_class_list != (list_t *)NULL) {
        DestroyList(help_class_list, Free);
    }

    FinishCmd_graph();
    FinishCmd_blockmap();
    DestroyCommandList(Cmds);
    DestroyCmd_class();
}

// queue a command to later be executed by ExecuteQueuedCommands
void QueueCommand(const std::string &command) {
    // Don't queue pure whitespace
    if (command.find_first_not_of(" \t\n\v\f\r") != std::string::npos) {
        pendingCommandQueue.push_back(command);
    }
}

// execute all commands in the command queue managed by QueueCommand
size_t ExecuteQueuedCommands() {
    const auto r = pendingCommandQueue.size();

    if (r == 0) return 0;
    
    // preserve prompt
    cli_save_stream();

    while (pendingCommandQueue.size()) {
        auto command = std::move(pendingCommandQueue.front());
        pendingCommandQueue.pop_front();
        ExecuteCommand(command.c_str());
    }
    
    cli_restore_stream();

    return r;
}

/*
** Execute command - used to parse a char buf into a command and execute it
*/

void ExecuteCommand(const char *line) {
    int seq = 0;
    int num_read = 0;
    char *cmd = NULL;
    char *haystack = NULL;
    char *needle = NULL;
    auto argv = ArgvStream{};
    cmdtype_t *c = NULL;
    dbgprocset_t *s = CurrentProcSet();
    auto exiter = OnScopeExit([](){DPRINT("Exit ExecuteCommand\n");});

    DPRINT("In ExecuteCommand\n");

    if (line == NULL) return;

    cmd = strdup(line);

    // If xml mode is enabled, we need to pull apart the xml tag we received
    if (Client.XML_mode) {
        if ((sscanf(line, XML_REQUEST_START, &seq, &num_read)) < 1) {
            PrintOutError("XML request parse error.\n");
            cli_set_error();
            return;
        }

        Free(cmd);
        cmd = strdup(line + num_read);

        haystack = cmd;
        // If we don't find the end needle the first time, its an error
        if ((haystack = strstr(haystack, XML_REQUEST_END)) == NULL) {
            PrintOutError("XML request parse error.\n");
            cli_set_error();
            Free(cmd);
            return;
        }
        needle = haystack;

        // Make sure that the user didn't type the needle in their command.
        // Its unlikely that they would do this, but we should still guard
        // against it.
        while (strlen(haystack) > strlen(XML_REQUEST_END)) {
            haystack += strlen(XML_REQUEST_END);

            while ((haystack = strstr(haystack, XML_REQUEST_END)) != NULL) {
                needle = haystack;
            }
        }

        // turn the start of the needle into a newline followed by a null term
        *needle = '\n';
        *(needle + 1) = '\0';
        DPRINT("ExecuteCommand: seq is %d\n", seq);
    } else if (Client.MI_mode) {
        //Convert mi to cli command
        try {
            auto cliCmd = convertMiToCli(cmd);
            free(cmd);

            if (cliCmd == "ok") {
                PrintOutOk();
                return;
            } else if (cliCmd.rfind("ok,", 0) == 0) {
                PrintOutMIResult(MIResult::DONE, "%s\n%s", cliCmd.substr(3).c_str(),GetMIEpilogue());
                return;
            } else if (cliCmd.rfind("error,", 0) == 0) {
                PrintOutError("%s", cliCmd.substr(6).c_str());
                return;
            }
            cmd = strdup(cliCmd.c_str());
        } catch (std::string msg) {
            PrintOutError("%s\n", msg.c_str());
            free(cmd);
            return;
        }

        std::string cmd_str = cmd;

        if (cmd_str.substr(0, cmd_str.find(" ")) == "-exec-run"){
            PrintOutMIResult(MIResult::CONTINUATION, "^running\n%s", GetMIEpilogue());
            free(cmd);
            return;
        }else if (cmd_str.substr(0, cmd_str.find(" ")) == "focus"){
            PrintOutMIResult(MIResult::CONTINUATION, "^running\n%s", GetMIEpilogue());
        }
    }

    DPRINT("ExecuteCommand: cmd is \"%s\"\n", cmd);

    argv.parseAndAppendArgs(cmd);

    // Inform the cli layer that we are starting to execute a command
    if (!Client.MI_mode){
        if (!cli_start_response(seq)) {
            // Sequence number mismatch, silently fail so next command does not have interference
            Free(cmd);
            return;
        }
    }
    // These are not considered to be errors
    if (argv.argc() == 0 || argv[0] == "" || argv[0][0] == '#') {
        Free(cmd);
        // Inform the cli layer that we are done executing a command
        cli_end_response();
        return;
    }

    if ((c = _findcmd(CurrentCmds, argv[0].c_str())) == (cmdtype_t *)NULL) {
        // command not found
        Free(cmd);
        // Inform the cli layer that we are done executing a command
        cli_end_response();
        // set an error for the cli stream
        cli_set_error();
        return;
    }

    // do the command
    if (!_docmd(CurrentCmds, s, c, argv.argc(), const_cast<char**>(argv.c_strs().data()), 0)) {
        Free(cmd);
        // Inform the cli layer that we are done executing a command
        cli_end_response();
        // set an error for the cli stream
        cli_set_error();
        return;
    }

    // Inform the cli layer that we are done executing a command
    cli_end_response();
    if (Client.MI_mode){
        if (strstr(cmd, "-exec-continue")){
            PrintOutMIResult(MIResult::CONTINUATION, "^running\n%s", GetMIEpilogue());
        } else if (strstr(cmd, "focus")){
            PrintOutOk();
        }
    }

    // If Quit is true after executing the command, remove the input handler.
    if (Client.quit) {
        DbgHandleInput(-1, nullptr, NULL);
    }

    Free(cmd);

    DPRINT("ExecuteCommand: Command ok!\n");

    return;
}
