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

// Definine whether or not a command requires an active launch
enum cmdptype { proc_required, proc_optional, proc_optional_mi, proc_ignore };

// Basic type for client functions
using cmdfunc_t = int (*)(rec_set_t*, int, char**);

// The command object links together the name to access a functions,
// the client side function to call and its help information
class Command {
public:
    // standard function
    Command(const std::string& name, const char* alias,
            cmdptype ptype, cmdfunc_t cmd,
            const char* const* help,
            const char* desc);

    // root node for a command tree. 
    Command(cmdptype ptype, cmdfunc_t defaultCmd);

    // access properties of the command 
    const std::string& getShortName() const { return m_name; }
    const std::string& getFullName() const { return m_fullName; }
    const std::vector<std::shared_ptr<Command>>& getSubCmds() const { return  m_subCmds; }
    const char* getDesc() const { return m_desc; }
    const char* const* getHelp() const { return m_help; }    

    // Commands can be be compound, for example "set env" or
    // "show print elements" which is kept as "show" command with
    // a Command object for each sub-command: args, breakpoint, cuda, etc.
    void addSubCmd(std::shared_ptr<Command> sub);

    void addSubCmd(const char* name, const char* alias, cmdptype ptype,
                   cmdfunc_t cmd,
                   const char* const* help, const char* desc);

    // Locate a command from user input.
    // If a matching command is found, return the command object and the
    // arguments for that command. If not found, return an error describing why.
    struct Parts { Command* command; std::string args; };
    using ErrStr = std::string;
    std::variant<Parts, ErrStr> findCmd(std::string_view cmdLine);

    // run a command with set of arguments
    int execute(dbgprocset_t *s, int argc, char *argv[]);

    virtual void failed();
    virtual ~Command();

private:

    std::variant<Command*, ErrStr> findSubCmd(std::string_view cmd);

    std::string m_fullName;       // e.g. "set print elements"
    std::string m_name;           // "elements" 
    const char* m_alias;          // optional 
    cmdptype m_ptype;
    cmdfunc_t m_func;
    const char* const* m_help;
    const char* m_desc;
    std::vector<std::shared_ptr<Command>> m_subCmds;
    std::shared_ptr<Command> m_default;
};


/*
** Global Init command functions defined elsewhere
*/
extern void InitCmd_assign();
extern void InitCmd_blockmap();
extern void InitCmd_break();
extern void InitCmd_checkpoint();
extern void InitCmd_comp();
extern void InitCmd_env();
extern void InitCmd_iostat();
extern void InitCmd_frame();
extern void InitCmd_func();
extern void InitCmd_go();
extern void InitCmd_script();
extern void InitCmd_list();
extern void InitCmd_passthru();
extern void InitCmd_print();
extern void InitCmd_proc();
extern void InitCmd_procset();
extern void InitCmd_python();
extern void InitCmd_source();
extern void InitCmd_thread();
extern void InitCmd_step();
extern void InitCmd_whatis();
extern void InitCmd_class();

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
                           cmdptype, cmdfunc_t,
                           const char *const[], const char *);

extern void AddCommand(const char * cmd, cmdptype, cmdfunc_t,
                           const char *const[], const char *);

extern Command* SetCommands(Command *);

extern void QueueCommand(const std::string &command);
extern size_t ExecuteQueuedCommands();
extern void ExecuteCommand(const char *);

// external access to the help command
extern void PrintHelpEntry(const char *const *);
extern int cmd_help_cmd(rec_set_t *, int, char **);

#endif /* ! _CMD_H */
