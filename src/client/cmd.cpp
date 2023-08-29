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
                Command::Func cmd, const char *const help[], const char *desc) {
    if (!Cmds) {
        // create the top level entry
        Cmds = std::make_shared<Command>("", nullptr, proc_optional,
                                         nullptr, nullptr, nullptr);
        CurrentCmds = Cmds.get();
    }

    Cmds->addSubCmd(name, alias, pt, cmd, help, desc);
}

void AddCommand(const char *name, cmdptype pt,
                Command::Func cmd, const char *const help[], const char *desc) {
    return AddCommand(name, nullptr, pt, cmd, help, desc);
}


Command::Command(const std::string& name, const char* alias,
                 cmdptype ptype, Command::Func cmd,
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

Command::Command(Command::CtorArgs&& args)
    : Command(args.name, args.alias, args.ptype, args.fn, args.help, args.desc)
{}

Command::Command(cmdptype ptype, Command::Func defaultCmd) :
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
        auto find_result = findCmd(prefix);
        if (std::holds_alternative<Command::Parts>(find_result)) {
            auto& [parent, argv] = std::get<Command::Parts>(find_result);
            parent->addSubCmd(sub);
        }
        // else silently ignore error. an error message is not useful to the user here.
    }
            
}

void Command::addSubCmd(const char* name, const char* alias, cmdptype ptype,
                        Command::Func cmd,
                        const char* const* help, const char* desc) {
    addSubCmd(std::make_shared<Command>(name, alias, ptype, cmd, help, desc));
}

void Command::addSubCmd(Command::CtorArgs&& args) {
    addSubCmd(std::make_shared<Command>(std::move(args)));
}

// execute the command given a procset and arguments
int Command::execute(Command::RunContext& ctx, dbgprocset_t *s, int argc, const char * const argv[]) {

    if (m_ptype == proc_required && (!s || s->s_nproc <= 0)) {
        PrintOutError("No process, use launch/attach first.\n");
        return 0;
    }


    if (!m_func && m_subCmds.empty()) {
        if (m_help) PrintHelpEntry(m_help);
        else PrintOutError("Command is not runnable.\n");
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

    // pass nullptr if the command is a proc_ignore command or if the passed procset
    // doesn't have any processes in it. that can happen when the user is running commands
    // before/after launching/killing any/all apps (the $all procset is passed in but is empty)
    const auto pass_null_rec_set = (m_ptype == proc_ignore) || (s && s->s_nproc <= 0);
    auto rec_set = (pass_null_rec_set ? nullptr :
                    CreateRecSetForProcset(s) // handles nullptr
                    );

    auto res = m_func(ctx, rec_set, argc, argv);
    if (!res) {
        failed();
    }

    // Done with the rec_set
    FreeRecSet(rec_set);

    return res;
}

void Command::failed() {}

Command *SetCommands(Command *cmds) {
    auto old = CurrentCmds;
    CurrentCmds = cmds;
    return old;
}

// TODO remove the need for all shim:: functions
namespace shim {

Command *GetCommands() {
    return CurrentCmds;
}

} // namespace shim

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

    // Begin init of every other command
    InitCmd_break();
    InitCmd_comp();
    InitCmd_env();
    InitCmd_go();
    InitCmd_print();
    InitCmd_proc();
    InitCmd_procset();
    InitCmd_thread();

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

    dbgprocset_t *s = CurrentProcSet();
    auto exiter = OnScopeExit([](){DPRINT("Exit ExecuteCommand\n");});

    DPRINT("In ExecuteCommand\n");

    if (!line) return;

    auto cmd = strdup(line);

    if (Client.MI_mode) {
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

    auto find_result = CurrentCmds->findCmd(cmd);

    if (auto* err = std::get_if<Command::ErrStr>(&find_result)) {
        // command not found
        PrintOutError("%s\n", err->c_str());
        Free(cmd);
        // set an error for the cli stream
        cli_set_error();
        return;
    }

    static_assert(std::variant_size_v<decltype(find_result)> == 2, "Assuming there are only two alternatives");

    auto [c,args] = std::get<Command::Parts>(find_result);
    if (!c) {
        // command not found
        Free(cmd);
        // set an error for the cli stream
        cli_set_error();
        return;
    }

    auto argv = ArgvStream{};
    argv.parseAndAppendArgs(c->getShortName() + " " + args);

    auto null_context = Command::RunContext{};

    // do the command
    if (!c->execute(null_context, s, argv.argc(), argv.c_strs().data())) {
        Free(cmd);
        // set an error for the cli stream
        cli_set_error();
        return;
    }

    // Inform the cli layer that we are done executing a command
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


// Locate a command from user input.
// If a matching command is found, return the command object and the
// arguments for that command. If not found, return an error describing why.
std::variant<Command::Parts, Command::ErrStr> Command::findCmd(std::string_view cmd) {
    auto* cp = this;

    const auto spcSlash = "/ \t\n";
    const auto spc = spcSlash+1;

    auto err = std::string{};
    auto end = size_t{0};

    for (auto loc=size_t{0}; loc < cmd.size(); loc=end) {
        if ((loc = cmd.find_first_not_of(spc, loc)) == std::string::npos) break;
        if (!(std::isalpha(cmd[loc]) || cmd[loc] == '-')) break;
        if ((end = cmd.find_first_of(spcSlash, loc)) == std::string::npos) end = cmd.size();

        if (auto subRes = cp->findSubCmd(cmd.substr(loc,end-loc)); subRes.index()) {
            end = loc;
            err = std::get<1>(subRes);
            break;
        } else if (std::get<0>(subRes)) {
            cp = std::get<0>(subRes);
        } else {
            end = loc;
            break;
        }
    }

    if (!err.empty()) {
        return err;
    } else {
        end = (cp->getShortName().empty() ? 0 : cmd.find_first_not_of(spc, end));
        return Parts{ .command = cp, .args = std::string{cmd.substr(std::min(cmd.size(),end))} };
    }
}

std::variant<Command*, Command::ErrStr> Command::findSubCmd(std::string_view cmd) {
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
        return os.str();
    } else if (cp) {
        return cp;
    } else if (m_default) {
        return m_default.get();
    } else if (m_func) {
        return static_cast<Command*>(nullptr);
    } else {
        auto err = std::string{"unknown command: "};
        err.append(cmd);
        return err;
    }
}

std::shared_ptr<Command> ConsoleCommandsFactory::make() {
    auto root = std::make_shared<Command>("", nullptr, proc_optional, nullptr, nullptr, nullptr);

    root->addSubCmd({
        .name = "info", .alias = "i", .ptype = proc_ignore,
        .fn = nullptr, .help = class_info_help,
        .desc = "Display information about the application being debugged."});

    insert_interpreter_commands(*root);
    insert_frame_commands(*root);
    insert_step_commands(*root);
    insert_passthru_commands(*root);
    insert_script_commands(*root);
    insert_blockmap_commands(*root);
    insert_env_commands(*root);
    insert_python_commands(*root);
    insert_assign_commands(*root);
    insert_checkpoint_commands(*root);
    insert_source_commands(*root);
    insert_whatis_commands(*root);
    insert_break_commands(*root);
    insert_proc_commands(*root);
    insert_list_commands(*root);
    insert_iostat_commands(*root);
    insert_threads_commands(*root);

    // insert a fake help command so we get the niceness of the command tree
    // matching "h", "he", etc. to "help". printing the actual help requires
    // context only available in the interpreter itself, so the interpreter
    // will catch a match to this command and short circuit to the help output.
    auto dummy_help = [](Command::RunContext& c, rec_set_t* r, int argc, const char* const argv[]) -> int {
        return Command::Failure;
    };
    root->addSubCmd({
        .name = "help", .alias = "h", .ptype = proc_ignore,
        .fn = dummy_help, .help = cmd_help_help,
        .desc = "Display help information about commands."});

    return root;
}

std::shared_ptr<Command> MiCommandsFactory::make() {
    auto root = std::make_shared<Command>("", nullptr, proc_optional, nullptr, nullptr, nullptr);

    insert_interpreter_commands(*root);
    insert_frame_commands(*root);
    insert_step_commands(*root);
    insert_whatis_commands(*root);
    insert_blockmap_commands(*root);
    insert_break_commands(*root);
    insert_script_commands(*root);
    insert_list_commands(*root);
    insert_threads_commands(*root);

    // insert a fake help command so we get the niceness of the command tree
    // matching "h", "he", etc. to "help". printing the actual help requires
    // context only available in the interpreter itself, so the interpreter
    // will catch a match to this command and short circuit to the help output.
    auto dummy_help = [](Command::RunContext& c, rec_set_t* r, int argc, const char* const argv[]) -> int {
        return Command::Failure;
    };
    root->addSubCmd({
        .name = "-help", .ptype = proc_ignore,
        .fn = dummy_help, .help = cmd_help_help,
        .desc = "Display help information about commands."});

    return root;
}
