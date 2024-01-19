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

    // function signature for text completion functions
    using CompletionFn =
        std::function<Interpreter::Completions(std::string_view /*partialWord*/, std::string_view /*buffer*/, const Interpreter&, const dbgprocset_t*, const ArgvStream&)>;

    // enables nice things like cmd.addSubCmd({.name = "...", .desc = "..."})
    struct CtorArgs {
        std::string name;
        const char* alias = nullptr;
        cmdptype ptype;
        Command fn;
        CompletionFn completion = nullptr;
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

    // similar to `findCmd`, but more lenient. `findCmdBestMatch` will search
    // the command tree iterating through `line` word by word until either the
    // end of the `line` is reached or a mismatch/ambiguity is found. e.g.
    // findCmdBestMatch("set print this_subcmd_doesnt_exist") returns the node
    // corresponding to "print" instead of an error.
    Parts findCmdBestMatch(std::string_view line);

    // using this node's completion function, generate completions for
    // `partialWord` which is a substring of `buffer`. some extra context is
    // provided for the completion function via `intr`, `procset` (might be
    // nullptr), and `argv`.
    //
    // if this node doesn't have a completion function, a default behavior of
    // matching the command node's subcommands is used.
    Interpreter::Completions completeText(
        std::string_view partialWord, std::string_view buffer,
        const Interpreter& intr, const dbgprocset_t *procset, const ArgvStream& argv);

    // run a command with set of arguments
    int execute(Interpreter& intr, dbgprocset_t *s, int argc, const char* const argv[]);

    virtual ~CommandNode();

private:

    struct Traversal;
    Traversal traverse(std::string_view line);
    std::variant<CommandNode*, ErrStr> findSubCmd(std::string_view cmd);

    std::string m_fullName;       // e.g. "set print elements"
    std::string m_name;           // "elements" 
    const char* m_alias;          // optional 
    cmdptype m_ptype;
    Command m_func;
    CompletionFn m_completionFn = nullptr;
    const char* const* m_help;
    const char* m_desc;
    std::vector<std::shared_ptr<CommandNode>> m_subCmds;
    std::shared_ptr<CommandNode> m_default;
};

// external access to the help command
extern void PrintHelpEntry(const char *const *);
