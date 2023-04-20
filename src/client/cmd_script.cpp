/*********************************************************************************\
 * Script commands.
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
 **********************************************************************************/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <iostream>
#include <iomanip>
#include <variant>
#include <cmath>

#include <arpa/inet.h>
#include <math.h>
#include <netdb.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/param.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include "script_client.h"
#include "script_settings.h"
#include "bitset.h"
#include "client.h"
#include "useful.h"


static std::shared_ptr<Command> ScriptCmds;
static Command* OldCmds = nullptr;

// script object built up by subsequent assert/set/etc commands
static std::unique_ptr<script::data::Script> g_pendingScript = nullptr;

#if DEBUG
// backslash sequences are escape codes to print debug messages in dark grey. if
// output is redirected to a file, they should be removed
#define LOG(msg) \
    std::cerr << "\u001b[30;1m" << __FILE__ << ":" << __LINE__ << "(" << __FUNCTION__ << "): " << msg << "\033[0m" << std::endl
#else
// dummy log message that will be optimized out, but still be syntax/type checked
#define LOG(msg) \
    while (false) std::cerr << msg << std::endl
#endif

/********************************
 * Info class commands
 ********************************/

static int cmd_info_diff_cmd(rec_set_t *rec_set, int argc, char **argv) {
    // this is a subcommand under "info", so argc/argv start at "diff", not "info"

    const auto printSyntax = [](){
        PrintOutError("Expected \"info diff [$script] [<assert id>]\"\n");
    };

    const auto printAssertDiffs = [](const auto& script, const auto& assert) {
        bool printHeader = true;
        for (auto& [diff, count] : script.getCondensedDiffs(assert)) {
            if (printHeader) {
                PrintOutMsg("For assertion %lu: %s\n", assert.id(), assert.string().c_str());
                printHeader = false;
            }
            switch (diff->result) {
            case CompareResult::Pass:
                PrintOutMsg("Comparison is true.\n"); break;
            case CompareResult::Fail:
                PrintOutMsg("Comparison is false.\n"); break;
            case CompareResult::Warn:
                PrintOutMsg("Comparison is warn.\n"); break;
            default:
                PrintOutMsg("Comparison cannot be peformed.\n"); break;
            }
            if (!diff->diff.empty()) {
                PrintOutMsg("The difference delta is:\n");
                PrintOutMsg("%s", diff->diff.c_str());
                            
            }
            //printCompareResult(diff->result.get(), diff->difference.get(), *diff->recSet);
            if (count > 1) {
                PrintOutMsg("(x%lu)\n", count);
            }
            PrintOutMsg("\n");
        }
        return !printHeader; // return true if we printed something
    };

    const auto printScriptDiffs = [&printAssertDiffs](const script::ScriptRunner& script) {
        PrintOutMsg("In assertion script $%s:\n", script.name().c_str());
        for (const auto& assert : script.asserts()) {
            printAssertDiffs(script, assert);
        }
    };

    if (argc == 1) {
        // "info diff": print summary
        PrintOutMsg("Stored assertion deltas:\n");
        for (const auto& [name, script] : script::scripts()) {
            printScriptDiffs(*script);
        }

        return 1;
    }

    // "info diff <name> ..."
    // argc >= 2, need to fetch script

    auto name = std::string_view(argv[1]);

    // scripts are stored without leading $
    if (name.at(0) == '$') {
        name = name.substr(1);
        if (name.length() == 0) {
            PrintOutError("Empty script name passed.\n");
            printSyntax();
            return 0;
        }
    }

    auto script = script::getScript(name);

    if (!script) {
        PrintOutError("Assertion script \"$%s\" doesn't exist.\n", name.data());
        printSyntax();
        return 0;
    }

    if (argc == 2) {
        // "info diff <name>": print summary for single script
        printScriptDiffs(*script);
        return 1;
    }

    // argc >= 3
    // "info diff <name> <assertion id>": print specific assertion's diffs

    size_t id;
    try {
        // std::stoul doesn't catch this...
        if (argv[2][0] == '-') throw std::invalid_argument("Negative");

        id = std::stoul(argv[2]);
    } catch (const std::invalid_argument& e) {
        printSyntax();
        return 0;
    }

    if (auto assert = script->getAssert(id)) {
        if (!printAssertDiffs(*script, *assert)) {
            PrintOutMsg("(None)\n");
        }
    } else {
        PrintOutError("No assertion in $%s with id %lu.\n", script->name().c_str(), id);
        return 0;
    }

    return 1;
}

static int cmd_info_script_cmd(rec_set_t *rec_set, int argc, char **argv) {
    // this is a subcommand under "info", so argc/argv start at "script", not "info"

    const auto printSyntax = [](){
        PrintOutError("Expected \"info script $name\"\n");
    };

    if (argc < 2) {
        auto runningScriptName = script::getCurrentScript()
            ? script::getCurrentScript()->name()
            : ""
        ;
        std::stringstream ss;
        PrintOutMsg("Saved assertion scripts:\n");
        for (const auto& [name, script] : script::scripts()) {
            auto runningString = runningScriptName == name
                ? " (running)"
                : ""
            ;
            if (Client.MI_mode){
                if (ss.tellp()!=0){
                    ss<<",";
                }else{
                    ss<<"[";
                }
                ss<<"{name=\""<<name<<"\",running=\""<<runningString<<"\"}";
            }else{
                PrintOutMsg("$%s%s\n", name.c_str(), runningString);
            }
        }
        if (Client.MI_mode){
            if (ss.tellp()==0)
                ss<<"[";
            ss<<"]";
            PrintOutMIResult(MIResult::DONE, "scripts=%s\n%s",ss.str().c_str(),GetMIEpilogue());
        }
        return 1;
    }

    auto name = std::string_view(argv[1]);

    // scripts are stored without leading $
    if (name.at(0) == '$') {
        name = name.substr(1);
        if (name.length() == 0) {
            PrintOutError("Empty script name passed.\n");
            printSyntax();
            return 0;
        }
    }

    auto script = script::getScript(name);

    if (!script) {
        PrintOutError("Assertion script \"$%s\" doesn't exist.\n", name.data());
        printSyntax();
        return 0;
    }

    if (argc < 3 || std::string_view(argv[2]) != "results") {
        PrintOutMsg("Assertion script $%s:\n", script->name().c_str());
        auto maxDigitLen = static_cast<int>(std::log10(script->asserts().size())) + 1;
        for (const auto& assert : script->asserts()) {
            auto ss = std::ostringstream{};
            ss << "Assertion " << std::setw(maxDigitLen) << assert.id() << ": "
               << assert.string()
            ;
            PrintOutMsg("%s\n", ss.str().c_str());
        }
        PrintOutMsg("Use \"info script $%s results\" for results.\n", script->name().c_str());
    } else {
        // detailed summary
        // summary string has correct newlines
        if (Client.MI_mode){
            PrintOutMIResult(MIResult::DONE, "script_result=%s\n%s",script->summary().c_str(),GetMIEpilogue());
        }
        else{
            PrintOutMsg("%s", script->summary().c_str());
        }
    }

    return 1;
}

/********************************
 * Regular commands
 ********************************/

static int cmd_build_cmd(rec_set_t *rec_set, int argc, char **argv) {
    if (argc < 2) {
        PrintOutError("Not enough command arguments.\n");
        PrintOutError("Expected: build $script_name\n");
        return 0;
    }

    if (auto existing = script::getScript(argv[1]+1)) {
        PrintOutError("Assertion script %s has already been defined:\n", argv[1]);
        PrintOutError("%s\n", existing->string(3ul).c_str());
        return 0;
    }

    // check for debugger variables with given name
    if (!IdMap::idIsAvailable(argv[1], true)) {
        return 0;
    }

    // name is passed without $
    auto name = std::string{argv[1]+1};
    g_pendingScript = std::make_unique<script::data::Script>(name);

    OldCmds = SetCommands(ScriptCmds.get());
    cli_inc_prompt();

    return 1;
}

static int cmd_start_cmd(rec_set_t *rec_set, int argc, char **argv) {
    if (argc < 2) {
        PrintOutError("Expected \"start $script_name\"\n");
        return 0;
    }

    auto name = std::string_view(argv[1]);

    // scripts are stored without leading $
    if (name.at(0) == '$') {
        name = name.substr(1);
        if (name.length() == 0) {
            PrintOutError("Empty script name passed.\n");
            return 0;
        }
    }

    auto script = script::getScript(name);

    if (!script) {
        PrintOutError("Assertion script \"$%s\" doesn't exist.\n", name.data());
        return 0;
    }

    // detect the case where a user runs an assertion script for a second time without
    // restarting the processes involved
    if (script->hasRunToCompletion() && !script->allDbgInvokesAtInitialBreakpoint()) {
        auto prompt =
            std::string{"Start $"} + script->name() +
            "? Program state is different than the initial first run, so results "
            "might be unexpected.\nYou might want to restart your app with the run "
            "command first.";

        if (!cli_query(prompt.c_str(), cli_query_default::NO, cli_query_default::YES)) {
            PrintOutMsg("Cancelled.\n");
            return 1;
        }
    }

    if (!script->reset()) {
        PrintOutError("Failed to set up script $%s: (%s)\n", script->name().c_str(), script->getError().c_str());
        PrintOutError("Cancelling execution.\n");
        return 0;
    }
    LOG("Successful script reset");

    if (!script->install()) {
        PrintOutError("Failed to set up script $%s: (%s)\n", script->name().c_str(), script->getError().c_str());
        PrintOutError("Cancelling execution.\n");
        script->uninstall();
        return 0;
    }

    // send go
    if (!script->sendContinue()) {
        PrintOutError("Failed to start script $%s. Cancelling script execution.\n",
            script->name().c_str());
        return 0;
    }
    LOG("Sent go");

    return 1;
}

static int cmd_stop_cmd(rec_set_t *rec_set, int argc, char **argv) {
    if (auto* script = script::getCurrentScript()) {
        script->uninstall();
    } else {
        PrintOutMsg("No assertion script is currently running.\n");
    }
    return 0;
}

/********************************
 * Alt commands
 ********************************/

static int cmd_script_assert_cmd(rec_set_t *rec_set, int argc, char **argv) {
    if (!g_pendingScript) {
        PrintOutError("Tried to add an assert to a nonexistant script.");
        return 0;
    }

    auto& currentScript = *g_pendingScript;

    try {
        // Add assert to current pending assertion script. Throws
        // InvalidAssertionError if expression is not fit to be used in an
        // assertion script (e.g. not a compare, missing location data)
        currentScript.newAssert(ArgvStream(&argv[1]).asString());
    } catch (const script::data::InvalidAssertionError& e) {
        PrintOutError("Error: %s.\n", e.what());
        return 0;
    }

    return 1;
}

static int cmd_script_end_cmd(rec_set_t *rec_set, int argc, char **argv) {
    // should never be false...
    if (g_pendingScript) {
        auto name = g_pendingScript->name();
        script::saveScript(std::move(g_pendingScript));
        PrintOutMsg("Assertion script $%s compiled.\n", name.c_str());
    } else {
        // ...but just in case
        PrintOutError("Tried to compile a nonexistent assertion script.\n");
    }
    cli_dec_prompt();
    SetCommands(OldCmds);
    return 1;
}

static void printTableEntry(const std::string& name, const script::settings::Item& state, const std::string& scope, const std::string& desc) {
    std::stringstream out;
    
    struct Widths {
        size_t name;
        size_t scope;
    };

    // calculate spacing for columns
    constexpr const auto widths = []() {
        Widths r{0, 0};
        for (const auto& sett : script::settings::infoTable()) {
            r.name = std::max(r.name, std::string_view(sett.name).size());
            r.scope = std::max(r.scope, std::string_view(sett.scope).size());
        }
        r.name += 1;
        r.scope += 1;
        return r;
    }();

    out << std::left
        << std::setw(widths.name) << name
        << std::setw(widths.scope) << scope
        << std::setw(11) << script::settings::shortString(state)
        << desc
    ;
    PrintOutMsg("%s\n", out.str().c_str());
}

static void printSettingsTable() {
    PrintOutMsg("Current settings:\n");
    // header
    printTableEntry("Name", "Currently", "Scope", "Description");
    for (const auto& sett : script::settings::infoTable()) {
        auto current = g_pendingScript->getSetting(sett.name);
        printTableEntry(sett.name, current, sett.scope, sett.description);
    }
}

static int cmd_script_set_cmd(rec_set_t *rec_set, int argc, char **argv) {
    if (!g_pendingScript) {
        // should never happen
        PrintOutError("Tried to set script setting with null script\n");
        return 0;
    }
    
    if (argc < 2) {
        printSettingsTable();
        return 1;
    }

    auto setting = g_pendingScript->getSetting(argv[1]);
    if (std::holds_alternative<nullptr_t>(setting)) {
        PrintOutError("No such assertion script setting: %s\n", argv[1]);
        PrintOutMsg("View list of available settings by issuing the set command with no arguments.\n");
        return 0;
    }

    const auto printArgSyntax = [](const auto& item){
        if (std::holds_alternative<bool*>(item)) {
            PrintOutError("Specify either \"true\", \"false\", \"1\", \"0\", \"on\" or \"off\".\n");
            return;
        } else if (std::holds_alternative<std::pair<double, double>*>(item)) {
            PrintOutError("Specify two floating point numbers separated by spaces.\n");
            return;
        }

        PrintOutError("Invalid syntax\n"); // should never happen
    };

    if (argc < 3) {
        printArgSyntax(setting);
        return 0;
    }

    if (const auto* b = std::get_if<bool*>(&setting)) {
        // parse bool
        auto& boo = **b;
    
        auto arg = std::string{argv[2]};
        // standardize to all lowercase
        std::transform(arg.begin(), arg.end(), arg.begin(), [](auto c){ return std::tolower(c); });
        
        if (arg == "true" || arg == "1" || arg == "on") {
            boo = true;
        } else if (arg == "false" || arg == "0" || arg == "off") {
            boo = false;
        } else {
            printArgSyntax(setting);
            return 0;
        }
        return 1;
    } else if (const auto *pdd = std::get_if<std::pair<double, double>*>(&setting)) {
        // parse pair<double, double>
        auto& pair = **pdd;

        if (argc < 4) {
            printArgSyntax(setting);
            return 0;
        }

        double low, high;

        try {
            low = std::stod(argv[2]);
            high = std::stod(argv[3]);
        } catch (const std::invalid_argument &ia) {
            printArgSyntax(setting);
            return 0;
        }

        pair.first = low;
        pair.second = high;
        return 1;
    }

    PrintOutError("Unhandled type for setting: %s\n", argv[1]); // should never happen

    return 1;
}

static int cmd_script_show_cmd(rec_set_t *rec_set, int argc, char **argv) {
    if (!g_pendingScript) {
        // should never happen
        PrintOutError("Tried to set a script setting with null script\n");
        return 0;
    }
    
    if (argc < 2) {
        printSettingsTable();
        return 1;
    }

    auto setting = g_pendingScript->getSetting(argv[1]);
    if (std::holds_alternative<nullptr_t>(setting)) {
        PrintOutError("No such assertion script setting: %s\n", argv[1]);
        PrintOutMsg("View list of available settings by issuing the show command with no arguments.\n");
        return 0;
    }

    auto info = script::settings::findInfo(argv[1]);

    if (std::holds_alternative<nullptr_t>(info.defaultt)) {
        // should never happen after check above
        return 0;
    }

    PrintOutMsg("%s: %s\n", argv[1], info.description);
    PrintOutMsg("Current value: %s.\n", script::settings::longString(setting).c_str());

    return 1;
}


/********************************
 * command init functions
 ********************************/

void InitCmd_script(void) {
    // info commands
    AddCommand("info diff", proc_ignore,
                   cmd_info_diff_cmd,
                   cmd_info_diff_help,
                   "Display saved deltas from assertion script execution.");
    AddCommand("info script", proc_ignore,
                   cmd_info_script_cmd,
                   cmd_info_script_help,
                   "Display built and active assertion scripts.");

    // regular commands
    AddCommand("build", NULL, proc_ignore, cmd_build_cmd,
               cmd_build_help, "Build an assertion script.");
    AddCommand("start", NULL, proc_ignore, cmd_start_cmd,
               cmd_start_help, "Start executing an assertion script.");
    AddCommand("stop", NULL, proc_ignore, cmd_stop_cmd,
               cmd_stop_help, "Stop the currently executing assertion script.");

    // graph alt commands
    ScriptCmds = std::make_shared<Command>(proc_ignore, nullptr);
    ScriptCmds->addSubCmd("assert", NULL, proc_ignore,
                  cmd_script_assert_cmd,
                  cmd_script_assert_help, "Create an assertion.");
    ScriptCmds->addSubCmd("end", NULL, proc_ignore,
                  cmd_script_end_cmd, cmd_script_end_help,
                  "Finish building the current assertion script.");
    ScriptCmds->addSubCmd("help", "h", proc_ignore,
                  cmd_help_cmd, cmd_help_help,
                  "Display help information about build script mode commands.");
    ScriptCmds->addSubCmd("set", NULL, proc_ignore,
                  cmd_script_set_cmd, cmd_script_set_help,
                  "Set information about the debugger environment.");
    ScriptCmds->addSubCmd("show", NULL, proc_ignore,
                  cmd_script_show_cmd, cmd_script_show_help,
                  "Show information about the debugger environment.");
}

