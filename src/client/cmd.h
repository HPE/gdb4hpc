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
 * $HeadURL$
 * $Date$
 * $Rev$
 * $Author$
 *
 ******************************************************************************/

#ifndef _CMD_H
#define _CMD_H

// Even though this header doesn't use stuff in these cmd headers, lets
// include them to make cmd.h be the master interface to command defines.
#include "help_dict.h"

// Declarations from these headers are actually used here.
#include <string>
#include "procset.h"
#include "interpreter.h"

#include <functional>

// Definine whether or not a command requires an active launch
enum cmdptype { proc_required, proc_optional, proc_ignore };

// The command object links together the name to access a functions,
// the client side function to call and its help information
class Command {
public:
    // a more readable return value for command implementations. TODO consider promoting
    // these to a full fledged enum class. if it was done, it would require a lot of
    // noise to change the return type of all command functions.
    static constexpr auto Success = 1;
    static constexpr auto Failure = 0;

    struct RunContext;

    // Basic generic function type for client functions. Return value is `int` for legacy
    // reasons, but implementations should prefer to return Success or Failure only.
    using Func = std::function<int(RunContext&, rec_set_t*, int, const char* const[])>;

    // Each command gets passed a RunContext. This allows for commands that can modify
    // state in their calling interpreter (e.g. gdbmode, -var-create).
    struct RunContext {
        // The interpreter that initiated the command. It is valid for this value to be nullptr.
        Interpreter* caller = nullptr;
        // ...room to grow.
    };

    // Since RunContext is completely generic and optional, writing commands that modify
    // their calling interpreter would be annoying (dynamic_casts everywhere). We provide
    // a more concrete context type for that. To get the more concrete context type to the
    // the command implementation that wants to use it, we point the initial command
    // function to a wrapper (called specify, below) that lowers the context and bails out
    // if it fails. See cmd_frame.cpp for a simple example of this in action.
    template <typename INTR>
    struct LoweredContext {
        INTR& caller;
    };

    template <typename INTR>
    static std::optional<LoweredContext<INTR>> lower_context(Command::RunContext& context) {
        auto* concrete = dynamic_cast<INTR*>(context.caller);
        if (concrete) return LoweredContext<INTR>{ .caller = *concrete };
        return std::nullopt;
    }

    template <typename INTR>
    using LoweredCmdFunc = std::function<int (LoweredContext<INTR>&, rec_set_t*, int, const char* const [])>;

    template <typename INTR>
    static auto specify(LoweredCmdFunc<INTR> fn) {
        return [fn](Command::RunContext& ctx, rec_set_t* rec_set, int argc, const char* const argv[]) {
            if (auto lowered = lower_context<INTR>(ctx)) {
                return fn(*lowered, rec_set, argc, argv);
            }
            // this error should only be reachable if, say, an mi-only command
            // is attached to a console interpreter command tree.
            PrintOutError("Invalid interpreter.\n");
            return Command::Failure;
        };
    }

    // standard function
    Command(const std::string& name, const char* alias,
            cmdptype ptype, Func cmd,
            const char* const* help,
            const char* desc);

    // enables nice things like cmd.addSubCmd({.name = "...", .desc = "..."})
    struct CtorArgs {
        std::string name;
        const char* alias = nullptr;
        cmdptype ptype;
        Func fn;
        const char* const* help = nullptr;
        const char* desc = nullptr;
    };

    Command(CtorArgs&& args);

    // root node for a command tree. 
    Command(cmdptype ptype, Func defaultCmd);

    // access properties of the command 
    const char* getAlias() const { return m_alias; }
    const std::string& getShortName() const { return m_name; }
    const std::string& getFullName() const { return m_fullName; }
    const std::vector<std::shared_ptr<Command>>& getSubCmds() const { return  m_subCmds; }
    const char* getDesc() const { return m_desc; }
    const char* const* getHelp() const { return m_help; }    

    // Commands can be be compound, for example "set env" or
    // "show print elements" which is kept as "show" command with
    // a Command object for each sub-command: args, breakpoint, cuda, etc.
    void addSubCmd(std::shared_ptr<Command> sub);

    void addSubCmd(const std::string& name, const char* alias, cmdptype ptype,
                   Func cmd,
                   const char* const* help, const char* desc);
    void addSubCmd(CtorArgs&& args);

    // Locate a command from user input.
    // If a matching command is found, return the command object and the
    // arguments for that command. If not found, return an error describing why.
    struct Parts { Command* command; std::string args; };
    using ErrStr = std::string;
    std::variant<Parts, ErrStr> findCmd(std::string_view cmdLine);

    // run a command with set of arguments
    int execute(RunContext& ctx, dbgprocset_t *s, int argc, const char* const argv[]);

    virtual void failed();
    virtual ~Command();

private:

    std::variant<Command*, ErrStr> findSubCmd(std::string_view cmd);

    std::string m_fullName;       // e.g. "set print elements"
    std::string m_name;           // "elements" 
    const char* m_alias;          // optional 
    cmdptype m_ptype;
    Func m_func;
    const char* const* m_help;
    const char* m_desc;
    std::vector<std::shared_ptr<Command>> m_subCmds;
    std::shared_ptr<Command> m_default;
};

/*
** Function prototypes
*/

extern void QueueCommand(const std::string &command);
extern size_t ExecuteQueuedCommands();

// external access to the help command
extern void PrintHelpEntry(const char *const *);

#endif /* ! _CMD_H */
