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
#include "client.h"
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

    // Passed to command implementations. Contains information about the context that the
    // command was run under.
    struct RunContext {
        // The interpreter that initiated the command. It is valid for this value to be nullptr.
        Interpreter* caller = nullptr;

        // ...room to grow. TODO consider moving some clnt_settings here?
    };

    // Basic type for client functions. Return value is `int` for legacy reasons, but
    // implementations should prefer to return Success or Failure only.
    using Func = std::function<int(RunContext&, rec_set_t*, int, const char* const[])>;

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

class ConsoleCommandsFactory {
public:
    static std::shared_ptr<Command> make();
private:
    // internal or base commands that don't live in cmd_*.cpp files.
    // e.g. the base "set" node which all other set commands are under.
    static void insert_class_commands(Command& root);

    // these are defined in their respective cmd_*.cpp files
    static void insert_interpreter_commands(Command& root);
    static void insert_frame_commands(Command& root);
    static void insert_step_commands(Command& root);
    static void insert_passthru_commands(Command& root);
    static void insert_script_commands(Command& root);
    static void insert_blockmap_commands(Command& root);
    static void insert_env_commands(Command& root);
    static void insert_python_commands(Command& root);
    static void insert_assign_commands(Command& root);
    static void insert_checkpoint_commands(Command& root);
    static void insert_source_commands(Command& root);
    static void insert_whatis_commands(Command& root);
    static void insert_break_commands(Command& root);
    static void insert_proc_commands(Command& root);
    static void insert_list_commands(Command& root);
    static void insert_iostat_commands(Command& root);
    static void insert_threads_commands(Command& root);
    static void insert_print_commands(Command& root);
    static void insert_comp_commands(Command& root);
    static void insert_go_commands(Command& root);
    static void insert_procset_commands(Command& root);
};

class MiCommandsFactory {
public:
    static std::shared_ptr<Command> make();
private:
    // internal or base commands that don't live in cmd_*.cpp files.
    // e.g. the base "set" node which all other set commands are under.
    static void insert_class_commands(Command& root);

    // these are defined in their respective cmd_*.cpp files
    static void insert_interpreter_commands(Command& root);
    static void insert_frame_commands(Command& root);
    static void insert_step_commands(Command& root);
    static void insert_whatis_commands(Command& root);
    static void insert_blockmap_commands(Command& root);
    static void insert_break_commands(Command& root);
    static void insert_list_commands(Command& root);
    static void insert_threads_commands(Command& root);
    static void insert_script_commands(Command& root);
    static void insert_print_commands(Command& root);
    static void insert_env_commands(Command& root);
    static void insert_proc_commands(Command& root);
    static void insert_comp_commands(Command& root);
    static void insert_go_commands(Command& root);
    static void insert_procset_commands(Command& root);
};


/*
** Global Init command functions defined elsewhere
*/
extern void InitCmd_func();
extern void InitCmd_proc();
extern void InitCmd_thread();

void FinishCmd_blockmap();

/*
 * External command init funcs
 */
extern void InitCommands();
extern void DestroyCommands();

/*
** Function prototypes
*/

extern void AddCommand(const char * cmd, const char * alias,
                           cmdptype, Command::Func,
                           const char *const[], const char *);

extern void AddCommand(const char * cmd, cmdptype, Command::Func,
                           const char *const[], const char *);

extern Command* SetCommands(Command *);

extern void QueueCommand(const std::string &command);
extern size_t ExecuteQueuedCommands();
extern void ExecuteCommand(const char *);

// external access to the help command
extern void PrintHelpEntry(const char *const *);

namespace shim {
// these functions are temporary and will be removed when the new interpreters
// implementation is done. they are needed to allow the interpreters
// implementation access to internal cmd.cpp values to better switch between the
// new and old implementations.
//
// TODO remove the need for all shim:: functions

// get current legacy command tree
Command* GetCommands();

} // namespace shim

#endif /* ! _CMD_H */
