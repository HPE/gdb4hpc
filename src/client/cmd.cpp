/******************************************************************************\
 * Command processing routines.
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * Copyright 2010-2023 Hewlett Packard Enterprise Development LP.
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
static std::shared_ptr<Command> Cmds;
static Command* CurrentCmds = nullptr;  /* Current command list */
static std::deque<std::string> pendingCommandQueue{}; /* Commands queued, waiting to be executed */

void AddCommand(const char *name, const char *alias, cmdptype pt,
                cmdfunc_t cmd, const char *const help[], const char *desc) {
    if (!Cmds) {
        // create the top level entry
        Cmds = std::make_shared<Command>("", nullptr, proc_optional,
                                         nullptr, nullptr, nullptr);
        CurrentCmds = Cmds.get();
    }

    Cmds->addSubCmd(name, alias, pt, cmd, help, desc);
}

void AddCommand(const char *name, cmdptype pt,
                cmdfunc_t cmd, const char *const help[], const char *desc) {
    return AddCommand(name, nullptr, pt, cmd, help, desc);
}


Command::Command(const std::string& name, const char* alias,
                 cmdptype ptype, cmdfunc_t cmd,
                 const char* const* help,
                 const char* desc) :
    m_fullName(name), m_alias(alias), m_ptype(ptype),
    m_func(cmd), m_help(help), m_desc(desc)
{
    if (auto spc = name.find_last_of(' '); spc == std::string::npos) {
        m_name = name;
    } else {
        m_name = name.substr(spc+1);
    }
}


Command::Command(cmdptype ptype, cmdfunc_t defaultCmd) :
    m_ptype(ptype), m_func(nullptr), m_help(nullptr), m_desc(nullptr)
{
    if (defaultCmd) {
        m_default = std::make_shared<Command>("", nullptr, proc_optional, defaultCmd,
                                              nullptr, nullptr);
    }
}

Command::~Command() {}

void Command::addSubCmd( std::shared_ptr<Command> sub) {

    auto spc = sub->m_fullName.find_last_of(" ");
    if ( spc == std::string::npos ||
         (!m_fullName.empty() && sub->m_fullName.rfind(m_fullName,0) == 0)) {
        auto loc = std::lower_bound(m_subCmds.begin(), m_subCmds.end(), sub,
                                    [](auto a, auto b) {
                                        return a->getShortName() < b->getShortName();
                                    });
        m_subCmds.insert(loc, sub);
    } else {
        auto prefix = sub->m_fullName.substr(0, spc);
        if (auto [parent, empty] = findCmd(prefix); parent) {
            parent->addSubCmd(sub);
        }
    }
            
}

void Command::addSubCmd(const char* name, const char* alias, cmdptype ptype,
                        cmdfunc_t cmd,
                        const char* const* help, const char* desc) {
    addSubCmd(std::make_shared<Command>(name, alias, ptype, cmd, help, desc));
}

// execute the command given a procset and arguments
int Command::execute(dbgprocset_t *s, int argc, char *argv[]) {

    if (m_ptype == proc_required ||
        (m_ptype == proc_optional_mi && !Client.MI_mode)) {
        if (!s || s->s_nproc <= 0) {
            PrintOutError("No process, use launch/attach first.\n");
            return 0;
        }
    }


    if (!m_func && m_subCmds.empty()) {
        PrintOutError("Command is not runnable.\n");
        return 0;
    } else if (!m_func && argc == 1) {
        // class command - list the sub commands
        for (auto c : m_subCmds) {
            auto name = c->getFullName();
            auto desc = c->getDesc();
            if (desc && !name.empty()) {
                PrintOutMsg("%-25s%s\n", name.c_str(), desc);
            }
        }
        return 1;
    } else if (!m_func) {
        PrintOutError("Undefined %s command: \"%s\".  Try \"help %s\".\n",
                      m_fullName.c_str(), argv[1], m_fullName.c_str());
        return 0;
    }
                      
    auto rec_set = (m_ptype == proc_ignore ? nullptr :
                    CreateRecSetForProcset(s) // handles nullptr
                    );

    auto res = (*m_func)(rec_set, argc, argv);
    if (!res) {
        failed();
    }

    if (Client.XML_mode) postProcessXML(rec_set);

    // Done with the rec_set
    FreeRecSet(rec_set);

    return res;
}


/*
** Apply a command to each process in a process set.
** Print and compare handle blockmap function
*/
void Command::postProcessXML(rec_set_t *rec_set) {

    // If XML mode is enabled, we need to ask the dbgclient layer if the
    // command we just executed changed the status. If it did, we want to
    // add the status information to the xml output. We also check to see
    // if the command caused the inferior to exit. Inferior exit can only be
    // caused by a command.
    if (!DbgCheckForStatusChange()) return;

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
            auto stat = EVENT_STATUS(reply->rep_event);

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

void Command::failed() {}

Command *SetCommands(Command *cmds) {
    auto old = CurrentCmds;
    CurrentCmds = cmds;
    return old;
}

/*
 * Help functions defined below
 */

int cmd_help_cmd(rec_set_t *rec_set, int argc, char **argv) {
    if (argc < 2) {
        for (auto c : CurrentCmds->getSubCmds()) {
            auto name = c->getShortName();
            auto desc = c->getDesc();
            if (desc && !name.empty()) {
                PrintOutMsg("%-15s %s\n", name.c_str(), desc);
            }
        }

        return 1;
    }

    auto cmdStr = ArgsToString(argv+1, argc-1);
    auto [cmd, err] = CurrentCmds->findCmd(cmdStr);

    if (cmd && cmd->getHelp()) {
        PrintHelpEntry(cmd->getHelp());
        return 1;
    } else if (cmd) {
        PrintOutError("No help for %s\n", cmdStr.c_str());
    } else {
        PrintOutError("%s\n", err.c_str());
    }

    return 0;
}


void PrintHelpEntry(const char *const * helpStrArray) {
    if (helpStrArray == NULL) return;

    // print initial newline
    PrintOutMsg("\n");

    // iterate over the array of strings
    auto cur_line = helpStrArray;
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
    // init the class commands
    InitCmd_class();

    // add the help command
    AddCommand("help", "h", proc_ignore, cmd_help_cmd,
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

    SetCommands(Cmds.get());
};

void DestroyCommands(void) {
    // destroy the internal help class list
    FinishCmd_blockmap();
    Cmds.reset();
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

    while (*line && std::isspace(*line)) ++line;
    if (!*line || *line == '#') return;
    
    int seq = 0;

    dbgprocset_t *s = CurrentProcSet();
    auto exiter = OnScopeExit([](){DPRINT("Exit ExecuteCommand\n");});

    DPRINT("In ExecuteCommand\n");

    if (!line) return;

    auto cmd = strdup(line);

    // If xml mode is enabled, we need to pull apart the xml tag we received
    if (Client.XML_mode) {
        int num_read = 0;
        if ((sscanf(line, XML_REQUEST_START, &seq, &num_read)) < 1) {
            PrintOutError("XML request parse error.\n");
            cli_set_error();
            return;
        }

        Free(cmd);
        cmd = strdup(line + num_read);

        auto haystack = cmd;
        // If we don't find the end needle the first time, its an error
        if ((haystack = strstr(haystack, XML_REQUEST_END)) == NULL) {
            PrintOutError("XML request parse error.\n");
            cli_set_error();
            Free(cmd);
            return;
        }
        auto needle = haystack;

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
    }

    DPRINT("ExecuteCommand: cmd is \"%s\"\n", cmd);


    // Inform the cli layer that we are starting to execute a command
    if (!Client.MI_mode){
        if (!cli_start_response(seq)) {
            // Sequence number mismatch, silently fail so next command does not have interference
            Free(cmd);
            return;
        }
    }

    auto [c,args] = CurrentCmds->findCmd(cmd);
    if (!c) {
        // command not found
        Free(cmd);
        // Inform the cli layer that we are done executing a command
        cli_end_response();
        // set an error for the cli stream
        cli_set_error();
        return;
    }

    auto argv = ArgvStream{};
    argv.parseAndAppendArgs(c->getShortName() + " " + args);

    // do the command
    if (!c->execute(s, argv.argc(), const_cast<char**>(argv.c_strs().data()))) {
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


// find a command from the command line argument.  Return the command and the
// arguments if a matching function can be found
std::pair<Command*, std::string> Command::findCmd(const std::string& cmd) {
    auto* cp = CurrentCmds;

    const auto spcSlash = "/ \t\n";
    const auto spc = spcSlash+1;

    auto err = std::string{};
    auto end = size_t{0};

    for (auto loc=size_t{0}; loc < cmd.size(); loc=end) {
        if ((loc = cmd.find_first_not_of(spc, loc)) == std::string::npos) break;
        if (!std::isalpha(cmd[loc])) break;
        if ((end = cmd.find_first_of(spcSlash, loc)) == std::string::npos) end = cmd.size();

        if (auto [subCp,subErr] = cp->findSubCmd(cmd.substr(loc,end-loc)); subCp) {
            cp = subCp;
        } else {
            end = loc;
            err = subErr;
            break;
        }
    }
    if (cp != CurrentCmds) {
        end = (cp->getShortName().empty() ? 0 : cmd.find_first_not_of(spc, end));
        return std::make_pair(cp, cmd.substr(std::min(cmd.size(),end)));
    } else {
        PrintOutError("%s\n", err.c_str());
        return std::pair<Command*, std::string>{nullptr, ""};
    }
}

std::pair<Command*,std::string> Command::findSubCmd(const std::string& cmd) {
    Command* cp = nullptr;
    std::vector<std::string> matches;
    
    for (auto subCmd : m_subCmds) {
        const auto sub = subCmd->getShortName();
        if (cmd == sub) {
            cp = subCmd.get();
            matches.clear();
            break;
        } else if (subCmd->m_alias && cmd == subCmd->m_alias) {
            cp = subCmd.get();
            matches.clear();
            break;
        } else if (!m_default && sub.rfind(cmd,0) == 0) {
            cp = subCmd.get();
            matches.push_back(sub);
        }
    }

    if (matches.size() > 1) {
        auto os = std::ostringstream{};
        os << "command " << cmd << " is ambiguous:";
        for (auto m : matches) os << " " << m;
        return std::make_pair(nullptr, os.str());
    } else if (cp) {
        return std::make_pair(cp,"");
    } else if (m_default) {
        return std::make_pair(m_default.get(),"");
    } else {
        auto err = "unknown command: " + cmd;
        return std::make_pair(nullptr, err);
    }
}
