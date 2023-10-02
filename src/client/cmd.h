/******************************************************************************\
 * Command routine defines.
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * Copyright 2010-2013,2020-2023 Hewlett Packard Enterprise Development LP.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Hewlett Packard Enterprise Development LP.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *
 ******************************************************************************/

#pragma once

// Even though this header doesn't use stuff in these cmd headers, lets
// include them to make cmd.h be the master interface to command defines.
#include "help_dict.h"

// Declarations from these headers are actually used here.
#include <string>
#include "procset.h"
#include "interpreter.h"
#include "command_invoke.h"

// Definine whether or not a command requires an active launch
enum cmdptype { proc_required, proc_optional, proc_ignore };

// The command object links together the name to access a functions,
// the client side function to call and its help information
class CommandNode {
public:
    // standard function
    CommandNode(const std::string& name, const char* alias,
            cmdptype ptype, Command cmd,
            const char* const* help,
            const char* desc);

    // enables nice things like cmd.addSubCmd({.name = "...", .desc = "..."})
    struct CtorArgs {
        std::string name;
        const char* alias = nullptr;
        cmdptype ptype;
        Command fn;
        const char* const* help = nullptr;
        const char* desc = nullptr;
    };

    CommandNode(CtorArgs&& args);

    // root node for a command tree. 
    CommandNode(cmdptype ptype, Command defaultCmd);

    // access properties of the command 
    const char* getAlias() const { return m_alias; }
    const std::string& getShortName() const { return m_name; }
    const std::string& getFullName() const { return m_fullName; }
    const std::vector<std::shared_ptr<CommandNode>>& getSubCmds() const { return  m_subCmds; }
    const char* getDesc() const { return m_desc; }
    const char* const* getHelp() const { return m_help; }    

    // Commands can be be compound, for example "set env" or
    // "show print elements" which is kept as "show" command with
    // a Command object for each sub-command: args, breakpoint, cuda, etc.
    void addSubCmd(std::shared_ptr<CommandNode> sub);

    void addSubCmd(const std::string& name, const char* alias, cmdptype ptype,
                   Command cmd,
                   const char* const* help, const char* desc);
    void addSubCmd(CtorArgs&& args);

    // Locate a command from user input.
    // If a matching command is found, return the command object and the
    // arguments for that command. If not found, return an error describing why.
    struct Parts { CommandNode* command; std::string args; };
    using ErrStr = std::string;
    std::variant<Parts, ErrStr> findCmd(std::string_view cmdLine);

    // run a command with set of arguments
    int execute(Interpreter& intr, dbgprocset_t *s, int argc, const char* const argv[]);

    virtual ~CommandNode();

private:

    std::variant<CommandNode*, ErrStr> findSubCmd(std::string_view cmd);

    std::string m_fullName;       // e.g. "set print elements"
    std::string m_name;           // "elements" 
    const char* m_alias;          // optional 
    cmdptype m_ptype;
    Command m_func;
    const char* const* m_help;
    const char* m_desc;
    std::vector<std::shared_ptr<CommandNode>> m_subCmds;
    std::shared_ptr<CommandNode> m_default;
};

/*
** Function prototypes
*/

extern void QueueCommand(const std::string &command);
extern size_t ExecuteQueuedCommands();

// external access to the help command
extern void PrintHelpEntry(const char *const *);
