/*********************************************************************************\
 * Script commands.
 * 
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * Copyright 2010-2022 Hewlett Packard Enterprise Development LP.
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
#include "compare_util.h"
#include "expression_handle.h"
#include "useful.h"

static cmdlist_t *ScriptCmds;
static cmdlist_t *OldCmds;

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
            printCompareResult(diff->result.get(), diff->difference.get(), *diff->recSet);
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

        PrintOutMsg("Saved assertion scripts:\n");
        for (const auto& [name, script] : script::scripts()) {
            auto runningString = runningScriptName == name
                ? " (running)"
                : ""
            ;
            PrintOutMsg("$%s%s\n", name.c_str(), runningString);
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
        PrintOutMsg("%s", script->summary().c_str());
    }

    return 1;
}

/********************************
 * Regular commands
 ********************************/

static int cmd_build_cmd(rec_set_t *rec_set, int argc, char **argv) {
    val_t *v;
    expr_t *e;

    if (argc < 2) {
        PrintOutError("Not enough command arguments.\n");
        PrintOutError("Expected: build $script_name\n");
        return 0;
    }
    
    // check for debugger variables with given name

    if ((e = ParseExpr(argv[1])) == (expr_t *)NULL) {
        PrintError();
        return 0;
    }

    if (EvalExpr(&v, e, rec_set) < 0) {
        PrintError();
        FreeExpr(e);
        return 0;
    }

    FreeExpr(e);

    if (LVAL_TYPE(v) != lval_internal) {
        PrintOutError(
            "A name must be specified for the assertion script. Names begin "
            "with a \'$\' character.\n");
        return 0;
    }

    switch (RVAL_ACTUAL_TYPE(v)) {
        case rval_proc:
            PrintOutError(
                "A process set has already been defined using this name.\n");
            return 0;
        case rval_blockmap:
            PrintOutError(
                "A decomposition has already been defined using this name.\n");
            return 0;
        case rval_aif:
            PrintOutError(
                "A debugger variable has already been defined using this "
                "name.\n");
            return 0;
        default:
            break;
    }

    // not an existing debugger variable. but is it the name of an existing script?

    auto name = std::string_view(argv[1] + 1); // trim $
    if (name.size() == 0) {
        PrintOutError(
            "A name must be specified for the assertion script. Names begin "
            "with a \'$\' character.\n");
        return 0;
    }

    if (auto existing = script::getScript(name)) {
        PrintOutError("Assertion script $%s has already been defined:\n", name.data());
        PrintOutError("%s\n", existing->string(3ul).c_str());
        return 0;
    }

    // name is passed without $
    g_pendingScript = std::make_unique<script::data::Script>(std::string{name});

    OldCmds = SetCommands(ScriptCmds);
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
    namespace Ex = ExpressionHandleShorthands;

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
 * Helper functions
 ********************************/

static void scriptnotfound(const char *cmd) { CmdNotFound(cmd); }

static void scriptambig(const char *cmd) { AmbiguousAltCmd(ScriptCmds, cmd); }

static void scriptfailed(void) {
    // do nothing
    return;
}

/********************************
 * command init functions
 ********************************/

void InitCmd_script(void) {
    // info commands
    AddInfoCommand("diff", proc_ignore, 0,
                   cmd_info_diff_cmd,
                   cmd_info_diff_help,
                   "Display saved deltas from assertion script execution.");
    AddInfoCommand("script", proc_ignore, 0,
                   cmd_info_script_cmd,
                   cmd_info_script_help,
                   "Display built and active assertion scripts.");

    // regular commands
    AddCommand("build", NULL, proc_ignore, 0, cmd_build_cmd,
               cmd_build_help, "Build an assertion script.");
    AddCommand("start", NULL, proc_ignore, 0, cmd_start_cmd,
               cmd_start_help, "Start executing an assertion script.");
    AddCommand("stop", NULL, proc_ignore, 0, cmd_stop_cmd,
               cmd_stop_help, "Stop the currently executing assertion script.");

    // graph alt commands
    ScriptCmds = NewCommandList(scriptnotfound, scriptambig, scriptfailed);

    AddAltCommand(ScriptCmds, "assert", NULL, proc_ignore, 0,
                  cmd_script_assert_cmd,
                  cmd_script_assert_help, "Create an assertion.");
    AddAltCommand(ScriptCmds, "end", NULL, proc_ignore, 0,
                  cmd_script_end_cmd, cmd_script_end_help,
                  "Finish building the current assertion script.");
    AddAltCommand(ScriptCmds, "help", "h", proc_ignore, 0,
                  cmd_help_cmd, cmd_help_help,
                  "Display help information about build script mode commands.");
    AddAltCommand(ScriptCmds, "set", NULL, proc_ignore, 1,
                  cmd_script_set_cmd, cmd_script_set_help,
                  "Set information about the debugger environment.");
    AddAltCommand(ScriptCmds, "show", NULL, proc_ignore, 1,
                  cmd_script_show_cmd, cmd_script_show_help,
                  "Show information about the debugger environment.");
}

void FinishCmd_graph(void) {
    DestroyCommandList(ScriptCmds);
}
