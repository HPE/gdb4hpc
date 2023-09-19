/*********************************************************************************\
 * Print command.
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
 *********************************************************************************/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <regex>
#include <sstream>

#include "client.h"
#include "help_dict.h"
#include "useful.h"
#include "gram.h"
#include "interpreter_mi.h"

void ClientSetPrintElements(int elements, rec_set_t* rec_set, bool force) {
    // only do work if we need to
    if (Client.print_elements != elements || force) {
        Client.print_elements = elements;

        // it is valid to call this function with a null rec_set, before a
        // gdb instance is launched
        if (rec_set != nullptr) {
            auto rep_set = DbgSetPrintElements(rec_set, std::to_string(elements).c_str());
            ProcessErrorInRepSet(rep_set);
        }
    }
}

// Print the message from a use of thread apply with it proc-set and thread-set
// info. \a msg is the output from a thread apply operation and will be
// formatted like: Thread 1-4:
//  <result>
// Thread 5 9:
//  <result>
// etc.  and this will be parsed and combined with diName and group to something
// like: $a{0..5:1..4}: <result> $a{0..5:5,9}: <result>
int PrintMultiThreadMsg(const char *diName, const Bitset *group,
                        const std::string &msg) {
    // the form is:
    // Threads 1 3 5-7:
    // <result>
    for (ThreadMsgParser parser(msg); auto res = parser.nextResult();) {
        auto [thread, val] = *res;

        // translate from gdb form to procset form
        for (size_t i = 0; (i = thread.find('-', i)) != std::string::npos; ++i)
            thread.replace(i, 1, "..");
        for (size_t i = 0; (i = thread.find(' ', i)) != std::string::npos; ++i)
            thread.replace(i, 1, ",");

        auto is = std::istringstream{std::move(val)};
        auto line = std::string{};
        while (std::getline(is, line)) {
            line += "\n";
            PrintMsgWithSetAndThreadInfo(diName, group, thread.c_str(),
                                         line.c_str());
        }
    }

    return 0;
}

static int cmd_print_cmd(Command::RunContext&, rec_set_t *rec_set, int argc, const char* const argv[]) {

    if (argc < 2) return 1;

    auto format = std::string{}, args = std::string{};

    // Check for print format modifier
    if (auto slash = strchr(argv[0], '/')) {
        format = std::string{slash+1};
        args = ArgsToString(argv+1, argc-1);
    } else if (argv[1][0] == '/') {
        if (argc < 3) return 1;
        format = std::string{argv[1]+1};
        args = ArgsToString(argv+2, argc-2);
    } else {
        args = ArgsToString(argv+1, argc-1);
    }

    auto cf = format;
    if (auto loc = cf.find("csv"); loc != std::string::npos) cf.erase(loc,3);
    if (auto bad = cf.find_first_not_of("xotz1"); bad != std::string::npos) {
        PrintOutError("Unrecognized format \'%c\'.\n", format[bad]);
        return 0;
    }

    if (Client.print_pretty) format += 'p';

    try {
        auto expr = ParseString(args);
        
        if (auto pe = getenv("PRINT_EXPR"); pe && pe[0] != '0') expr->print();

        {
            auto pager = std::make_unique<cli_pager>();
            auto val = expr->evalTop(rec_set);
            val->print(OutMsgStream(), rec_set, format);
        }
    } catch (EvalErrorAlreadyReported& err) {
        return 1; // this was not reported as the command failing
    } catch (EvalError& err) {
        PrintOutError("%s\n", err.what());
        return 0;
    } catch (std::exception& err) {
        PrintOutError("%s\n", err.what());
        return 0;
    }
        
    return 1;
}

static int cmd_set_print_elements_cmd(Command::RunContext&, rec_set_t *rec_set, int argc, const char* const argv[]) {
    if (argc < 2) {
        PrintOutError("Not enough command arguments.\n");
        PrintOutError("Usage: set print elements <positive integer or \"unlimited\">\n");
        return 0;
    }

    int elements = -1;

    if (strcmp("unlimited", argv[1]) == 0) {
        elements = 0;
    } else {
        try {
            elements = std::stoi(argv[1]);
        } catch (const std::invalid_argument &e) {
            PrintOutError("Error: couldn't convert \"%s\" to an integer. (%s)\n",
                          argv[1], e.what());
            return 0;
        } catch (const std::out_of_range &e) {
            PrintOutError("Error: %s is out of range for an integer. (%s)\n",
                          argv[1], e.what());
            return 0;
        }
    }

    if (elements < 0) {
        PrintOutError("Error: negative values are invalid.\n");
        return 0;
    }

    ClientSetPrintElements(elements, rec_set);

    return 1;
}

static int cmd_show_print_elements_cmd(Command::RunContext&, rec_set_t *, int, const char* const []) {
    if (Client.print_elements == 0) {
        PrintOutMsg("Limit on string chars or array elements to print is unlimited.\n");
    } else {
        PrintOutMsg("Limit on string chars or array elements to print is %d.\n", Client.print_elements);
    }

    return 1;
}

static int cmd_set_print_pretty_cmd(Command::RunContext&, rec_set_t *, int argc, const char* const argv[]) {
    if (argc < 2) {
        PrintOutError("Not enough command arguments.\n");
        PrintOutError("Usage: set print pretty on|off\n");
        return 0;
    }

    if (strcmp(argv[1],"on")==0) {
        Client.print_pretty = true;
    } else if (strcmp(argv[1],"off")==0) {
        Client.print_pretty = false;
    } else {
        PrintOutError("Unrecognized option %s, use \"on\" or \"off\".\n", argv[1]);
        return 0;
    }

    return 1;
}

static int cmd_enable_pretty_printing_mi(Command::RunContext&, rec_set_t *, int, const char* const []) {
    Client.print_pretty = true;
    return Command::Success;
}

static int cmd_show_print_pretty_cmd(Command::RunContext&, rec_set_t *, int, const char* const []) {
    PrintOutMsg("Pretty printing is %s.\n", Client.print_pretty ? "on" : "off");
    return 1;
}

// remove new lines and escape quotes so output can be added to an MI response
std::string FormatMIValue(std::string origValue) {
    auto value = std::move(origValue);
    while (!value.empty() && value.back() == '\n') value.pop_back();
    for (std::size_t pos=0;(pos=value.find_first_of("\"\'\\\n",pos)) != std::string::npos; ) {
        if(value[pos]=='\n'){
            value.replace(pos, 1, "\\n");
            pos=pos+2;
        }else{
            value.insert(pos,"\\");
            pos=pos+2;
        }
    }
    return value;
}
    
// retrieve the value of an expresison for the MI context.
// return an empty string if it fails (typically a variable out of scope)
std::string GetMiValue(const std::string& expression, const std::string& ps_str)
{
    try {
        auto expr = ParseString(expression);
        auto ps = FindProcSet(ps_str.c_str());
        auto rec_set = std::unique_ptr<rec_set_t>{CreateRecSetForProcset(ps)};

        auto val = expr->evalTop(rec_set.get());

        auto os = std::ostringstream{};
        
        val->print(os, rec_set.get(), (Client.print_pretty?"p":""));

        return FormatMIValue(os.str());
    } catch (std::exception& err) {}

    return "";
}

static void insert_setshow_commands(Command& root, std::string set, std::string show) {
    root.addSubCmd(
        set + " print", nullptr, proc_ignore,
        nullptr, class_set_print_help,
        "Set information about how things get printed.");

    root.addSubCmd(
        show + " print", nullptr, proc_ignore,
        nullptr, class_show_print_help,
        "Show information about how things get printed.");

    root.addSubCmd(
        set + " print elements", nullptr, proc_optional, cmd_set_print_elements_cmd,
        cmd_set_print_elements_help,
        "Specify number of elements to be printed in arrays or c-strings.");

    root.addSubCmd(
        show + " print elements", nullptr, proc_optional, cmd_show_print_elements_cmd,
        cmd_show_print_elements_help,
        "Show number of elements to print when an array is printed.");

    root.addSubCmd(
        set + " print pretty", nullptr, proc_optional, cmd_set_print_pretty_cmd,
        cmd_set_print_pretty_help,
        "Enable or disable pretty printing of structures.");

    root.addSubCmd(
        show + " print pretty", nullptr, proc_optional, cmd_show_print_pretty_cmd,
        cmd_show_print_pretty_help,
        "Show if pretty printing of structures is enabled.");
}

void ConsoleCommandsFactory::insert_print_commands(Command& root) {
    insert_setshow_commands(root, "set", "show");
    root.addSubCmd("print", "p", proc_optional, cmd_print_cmd, cmd_print_help,
               "Print the value of an expression.");
}

void MiCommandsFactory::insert_print_commands(Command& root) {
    root.addSubCmd({.name = "-enable-pretty-printing", .ptype = proc_optional, .fn = cmd_enable_pretty_printing_mi });
    insert_setshow_commands(root, "-gdb-set", "-gdb-show");
}
