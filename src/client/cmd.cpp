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
#include "interpreter_mi.h"

CommandNode::CommandNode(const std::string& name, const char* alias,
                 cmdptype ptype, Command cmd,
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

CommandNode::CommandNode(CommandNode::CtorArgs&& args)
    : CommandNode(args.name, args.alias, args.ptype, args.fn, args.help, args.desc)
{}

CommandNode::CommandNode(cmdptype ptype, Command defaultCmd) :
    m_ptype(ptype), m_func(nullptr), m_help(nullptr), m_desc(nullptr)
{
    if (defaultCmd) {
        m_default = std::make_shared<CommandNode>("", nullptr, proc_optional, defaultCmd,
                                              nullptr, nullptr);
    }
}

CommandNode::~CommandNode() {}

void CommandNode::addSubCmd( std::shared_ptr<CommandNode> sub) {

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
        if (std::holds_alternative<CommandNode::Parts>(find_result)) {
            auto& [parent, argv] = std::get<CommandNode::Parts>(find_result);
            parent->addSubCmd(sub);
        }
        // else silently ignore error. an error message is not useful to the user here.
    }
            
}

void CommandNode::addSubCmd(const std::string& name, const char* alias, cmdptype ptype,
                        Command cmd,
                        const char* const* help, const char* desc) {
    addSubCmd(std::make_shared<CommandNode>(name, alias, ptype, cmd, help, desc));
}

void CommandNode::addSubCmd(CommandNode::CtorArgs&& args) {
    addSubCmd(std::make_shared<CommandNode>(std::move(args)));
}

static std::unique_ptr<rec_set_t> getRecSetForCommand(cmdptype ptype, dbgprocset_t* procset) {
    // pass nullptr if the command is a proc_ignore command or if the passed procset
    // doesn't have any processes in it. that can happen when the user is running commands
    // before/after launching/killing any/all apps (the $all procset is passed in but is empty)
    const auto pass_null_rec_set = (ptype == proc_ignore) || (procset && procset->s_nproc <= 0);

    // CreateRecSetForProcset will handle a nullptr input correctly and return nullptr
    auto* rec_set = pass_null_rec_set ? nullptr : CreateRecSetForProcset(procset);

    return std::unique_ptr<rec_set_t>(rec_set);
}

// execute the command given a procset and arguments
int CommandNode::execute(Interpreter& intr, dbgprocset_t *s, int argc, const char * const argv[]) {
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

    return m_func(intr, getRecSetForCommand(m_ptype, s).get(), argc, argv);
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

// Locate a command from user input.
// If a matching command is found, return the command object and the
// arguments for that command. If not found, return an error describing why.
std::variant<CommandNode::Parts, CommandNode::ErrStr> CommandNode::findCmd(std::string_view cmd) {
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

std::variant<CommandNode*, CommandNode::ErrStr> CommandNode::findSubCmd(std::string_view cmd) {
    CommandNode* cp = nullptr;
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
        return static_cast<CommandNode*>(nullptr);
    } else {
        auto err = std::string{"unknown command: "};
        err.append(cmd);
        return err;
    }
}
