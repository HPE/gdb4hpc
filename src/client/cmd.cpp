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
static std::deque<std::string> pendingCommandQueue{}; /* Commands queued, waiting to be executed */

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

void Command::addSubCmd(const std::string& name, const char* alias, cmdptype ptype,
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
        DbgInterpret(command.c_str());
    }
    
    cli_restore_stream();

    return r;
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

    insert_class_commands(*root);
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
    insert_print_commands(*root);
    insert_comp_commands(*root);
    insert_go_commands(*root);
    insert_procset_commands(*root);

    return root;
}

static void insert_setshow_commands(Command& root, std::string set, std::string show) {
    root.addSubCmd(set, nullptr, proc_ignore,
                   nullptr, class_set_help,
                   "Set information about the debugger environment.");

    root.addSubCmd(show, nullptr, proc_ignore,
                   nullptr, class_show_help,
                   "Show information about the debugger environment.");
}

void ConsoleCommandsFactory::insert_class_commands(Command& root) {
    // insert a fake help command so we get the niceness of the command tree
    // matching "h", "he", etc. to "help". printing the actual help requires
    // context only available in the interpreter itself, so the interpreter
    // will catch a match to this command and short circuit to the help output.
    auto dummy_help = [](Command::RunContext&, rec_set_t*, int, const char* const []) -> int {
        return Command::Failure;
    };
    root.addSubCmd("help", "h", proc_ignore,
                    dummy_help, cmd_help_help,
                    "Display help information about commands.");

    root.addSubCmd("info", "i", proc_ignore,
                   nullptr, class_info_help,
                   "Display information about the application being debugged.");

    root.addSubCmd("maint", nullptr, proc_ignore,
                   nullptr, class_maint_help,
                   "Commands for use by debugger maintainers.");

    root.addSubCmd("maint set", nullptr, proc_ignore,
                   nullptr, class_maint_set_help,
                   "Set information about the debugger environment.");

    root.addSubCmd("maint show", nullptr, proc_ignore,
                   nullptr, class_maint_show_help,
                   "Show information about the debugger environment.");

    root.addSubCmd("unset", nullptr, proc_ignore,
                   nullptr, class_unset_help,
                   "Unset information in the debugger environment.");

    insert_setshow_commands(root, "set", "show");
}

std::shared_ptr<Command> MiCommandsFactory::make() {
    auto root = std::make_shared<Command>("", nullptr, proc_optional, nullptr, nullptr, nullptr);

    insert_class_commands(*root);
    insert_dummy_commands(*root);
    insert_mi_commands(*root);
    insert_interpreter_commands(*root);
    insert_frame_commands(*root);
    insert_step_commands(*root);
    insert_whatis_commands(*root);
    insert_blockmap_commands(*root);
    insert_break_commands(*root);
    insert_script_commands(*root);
    insert_list_commands(*root);
    insert_threads_commands(*root);
    insert_print_commands(*root);
    insert_env_commands(*root);
    insert_proc_commands(*root);
    insert_comp_commands(*root);
    insert_go_commands(*root);
    insert_procset_commands(*root);

    return root;
}

void MiCommandsFactory::insert_class_commands(Command& root) {
    // insert a fake help command so we get the niceness of the command tree
    // matching "h", "he", etc. to "help". printing the actual help requires
    // context only available in the interpreter itself, so the interpreter
    // will catch a match to this command and short circuit to the help output.
    auto dummy_help = [](Command::RunContext&, rec_set_t*, int, const char* const []) -> int {
        return Command::Failure;
    };
    root.addSubCmd({
        .name = "-help", .ptype = proc_ignore,
        .fn = dummy_help, .help = cmd_help_help,
        .desc = "Display help information about commands."});

    insert_setshow_commands(root, "-gdb-set", "-gdb-show");
}

void MiCommandsFactory::insert_dummy_commands(Command& root) {
    const auto read_from_dummy_map = [](Command::RunContext&, rec_set_t*, int, const char* const argv[]) -> int {
        static const std::map<std::string, std::string> dummy_map = {
             {"-data-list-register-names", "register-names=[]"},
             {"-enable-frame-filters", ""},
             {"-list-features", "features=[]"},
             {"-list-target-features", "features=[async]"},
        };

        // assume it will never miss because we only enter this function if we have a
        // known command
        auto [key, value] = *dummy_map.find(argv[0]);

        if (value.empty()) {
            PrintOutOk();
        } else {
            PrintOutMIResult(MIResult::DONE, "%s\n%s", value.c_str(), GetMIEpilogue());
        }

        return Command::Success;
    };

    root.addSubCmd({ .name = "-data-list-register-names", .ptype = proc_ignore, .fn = read_from_dummy_map });
    root.addSubCmd({ .name = "-enable-frame-filters", .ptype = proc_ignore, .fn = read_from_dummy_map });
    root.addSubCmd({ .name = "-list-features", .ptype = proc_ignore, .fn = read_from_dummy_map });
    root.addSubCmd({ .name = "-list-target-features", .ptype = proc_ignore, .fn = read_from_dummy_map });
}
