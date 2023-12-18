/*********************************************************************************\
 * Breakpoint commands.
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

#include <map>
#include <fstream>
#include <sstream>

#include "client.h"
#include "useful.h"
#include "interpreter_mi.h"
#include "script_client.h"
#include "blockmap_client.h"

struct BreakArgs {
    // XXX symbol_or_number should probably be split into two members. emulating old behavior for now...
    std::string symbol_or_number;
    std::string file;
    std::string thread_set;
    PendingBrkType pending_val = PendingBrkType::Auto;
};

static std::optional<BreakArgs> parse_break_args(rec_set_t* rec_set, int argc, const char* const argv[]) {
    auto result = BreakArgs{};

    if (rec_set->getThreads()) result.thread_set = rec_set->getThreads();

    if (argc < 2) {
        PrintOutError("Not enough command arguments.\n");
        PrintOutError("Expected: break main.c:20\n");
        return {};
    } else if (argc >= 4 && strcmp(argv[2],"thread") == 0) {
        result.thread_set = ArgsToString(&argv[3], argc - 3);
    } else if (argc > 2) {
        PrintOutError("Too many command arguments.\n");
        PrintOutError("Expected: break main.c:20\n");
        PrintOutError("or: break main.c:20 thread 2\n");
        return {};
    }

    result.pending_val = Client.brk_pending;
    auto location = std::string_view(argv[1]);

    if (auto colon = location.find(':'); colon != std::string_view::npos) {
        result.file = location.substr(0, colon);
        result.symbol_or_number = location.substr(colon + 1);
    } else {
        result.symbol_or_number = location;
    }

    return result;
}

static rep_set_t send_break_command(rec_set_t *rec_set, const BreakArgs& args) {
    // XXX emulating legacy behavior here... could definitely use a touch up one day
    if (IsInteger(args.symbol_or_number.c_str())) {
        return DbgSetLineBreak(
            rec_set,
            (args.pending_val == PendingBrkType::On),
            !args.file.empty() ? args.file.c_str() : nullptr,
            atoi(args.symbol_or_number.c_str()),
            args.thread_set.c_str());
    } else {
        return DbgSetFuncBreak(
            rec_set,
            (args.pending_val == PendingBrkType::On),
            !args.file.empty() ? args.file.c_str() : nullptr,
            args.symbol_or_number.c_str(),
            args.thread_set.c_str());
    }
}

// returns true on success
static bool do_set_break_console(rec_set_t* rec_set, BreakArgs& args);

static int cmd_break_cmd_console(rec_set_t *rec_set, int argc, const char* const argv[]) {
    auto maybe_args = parse_break_args(rec_set, argc, argv);
    if (!maybe_args) {
        // error message already printed
        return Command::Failure;
    }
    auto& args = *maybe_args;

    return do_set_break_console(rec_set, args) ? Command::Success : Command::Failure;
}

static bool do_set_break_console(rec_set_t* rec_set, BreakArgs& args) {
    auto rep_set = send_break_command(rec_set, args);

    for (auto&& reply : rep_set) {
        // point at the breakpoint data
        auto* bp = EVENT_BP(reply->rep_event);
        auto* di = reply->rep_di.lock();
        auto bpset = di->breakpoints().findByDbgBreakpoint(*bp);
        if (!bpset)  // this is not supposed to happen
        {
            PrintErrorWithSetInfo(reply->rep_di.c_str(), reply->rep_group.get(),
                                  "ERROR: cannot find breakpoint [%d: file %s, "
                                  "line %d] in BPSetList.\n",
                                  bp->bp_fe_id, bp->bp_loc.loc_file,
                                  bp->bp_loc.loc_line);
            continue;
        }

        std::string bpThread;
        if (!bpset->threadId().empty()) {
            bpThread = " thread " + bpset->threadId();
        }

        if (bpset->isPending()) {
            PrintMsgWithSetInfo(reply->rep_di.c_str(), reply->rep_group.get(),
                                "Breakpoint %d: Pending at %s%s.\n",
                                bpset->id(), bpset->statement().c_str(), bpThread.c_str());
        } else if (bpset->line() >= 0) {
            PrintMsgWithSetInfo(reply->rep_di.c_str(), reply->rep_group.get(),
                                "Breakpoint %d: file %s, line %d%s.\n",
                                bpset->id(), bpset->file().c_str(),
                                bpset->line(), bpThread.c_str());
        } else {
            PrintMsgWithSetInfo(reply->rep_di.c_str(), reply->rep_group.get(),
                                "Breakpoint %d in %s%s\n", bpset->id(),
                                bpset->function().c_str(), bpThread.c_str());
        }
    }

    // If we had errors, query the user to set a pending breakpoint if in auto mode
    if (ProcessErrorInRepSet(rep_set) && args.pending_val == PendingBrkType::Auto) {
        // XXX: in gdb, this query defaults to no. but in gdb mi mode, when using the
        // console interpreter fallback, it defaults to yes.
        if (cli_query("Make breakpoint pending on future code object load?", cli_query_default::NO)) {
            // Force pending_val to on and try again
            args.pending_val = PendingBrkType::On;
            return do_set_break_console(rec_set, args);
        }
    }

    return true;
}

static int cmd_break_cmd_mi(MiInterpreter& mi, rec_set_t *rec_set, int argc, const char* const argv[]) {
    auto maybe_args = parse_break_args(rec_set, argc, argv);
    if (!maybe_args) {
        // error message already printed
        return Command::Failure;
    }
    auto& args = *maybe_args;

    auto rep_set = send_break_command(rec_set, args);

    for (auto&& reply : rep_set) {
        // point at the breakpoint data
        auto* bp = EVENT_BP(reply->rep_event);
        auto* di = reply->rep_di.lock();
        auto bpset = di->breakpoints().findByDbgBreakpoint(*bp);
        if (!bpset)  // this is not supposed to happen
        {
            PrintErrorWithSetInfo(reply->rep_di.c_str(), reply->rep_group.get(),
                                  "ERROR: cannot find breakpoint [%d: file %s, "
                                  "line %d] in BPSetList.\n",
                                  bp->bp_fe_id, bp->bp_loc.loc_file,
                                  bp->bp_loc.loc_line);
            continue;
        }

        // XXX: lots of leftover console behavior here, leaving it in for now to match old behavior

        std::string bpThread;
        if (!bpset->threadId().empty()) {
            bpThread = " thread " + bpset->threadId();
        }

        if (bpset->isPending()) {
            PrintMsgWithSetInfo(reply->rep_di.c_str(), reply->rep_group.get(),
                                "Breakpoint %d: Pending at %s%s.\n",
                                bpset->id(), bpset->statement().c_str(), bpThread.c_str());
        }
        else if (bpset->line() >= 0) {
            std::stringstream ss;
            std::string group = reply->rep_group.get()->toSetString();
            auto fullpath = reply->rep_di.lock()->sourcePaths().getFullPath(bpset->file());
            ss << "proc_set=\"" << reply->rep_di.name()
               << "\",group=\"" << group.substr(1,group.length()-2)
               << "\",bkpt={number=\"" << bpset->id()
               << "\",type=\"breakpoint\",disp=\"keep\",enabled=\"y\",addr=\""
               << bpset->address() << "\",func=\"" << bpset->function()
               << "\",file=\"" << bpset->file() <<"\",fullname=\"" << fullpath
               << "\",line=\"" <<bpset->line() << "\"}";
            mi.set_result("%s", ss.str().c_str());
        }
        else {
            PrintMsgWithSetInfo(reply->rep_di.c_str(), reply->rep_group.get(),
                                "Breakpoint %d in %s%s\n", bpset->id(),
                                bpset->function().c_str(), bpThread.c_str());
        }
    }

    return ProcessErrorInRepSet(rep_set) == 0 ? Command::Success : Command::Failure;
}

// returns true on success
static bool do_set_tbreak(rec_set_t *rec_set, BreakArgs& args);

static int cmd_tbreak_cmd(rec_set_t *rec_set, int argc, const char* const argv[]) {
    auto maybe_args = parse_break_args(rec_set, argc, argv);
    if (!maybe_args) {
        // error message already printed
        return Command::Failure;
    }
    auto& args = *maybe_args;

    return do_set_tbreak(rec_set, args) ? Command::Success : Command::Failure;
}

static bool do_set_tbreak(rec_set_t* rec_set, BreakArgs& args) {
    auto rep_set = send_break_command(rec_set, args);

    for (auto&& reply : rep_set) {
        // point at the breakpoint data
        auto* bp = EVENT_BP(reply->rep_event);
        auto* di = reply->rep_di.lock();

        // Now set this to a temporary breakpoint using the enable command for
        // this reply group
        auto new_rec = NewRecSet();
        AddReceiverToSet(new_rec,
                         NewGroupReceiver(*di, reply->rep_group.get()));

        auto bpset = di->breakpoints().findByDbgBreakpoint(*bp);
        if (!bpset)  // this is not supposed to happen
        {
            PrintErrorWithSetInfo(reply->rep_di.c_str(), reply->rep_group.get(),
                                  "ERROR: cannot find breakpoint [%d: file %s, "
                                  "line %d] in BPSetList.\n",
                                  bp->bp_fe_id, bp->bp_loc.loc_file,
                                  bp->bp_loc.loc_line);
            FreeRecSet(new_rec);
            continue;
        }

        if (auto enb_rep = DbgEnableBreak(new_rec, bpset->id(), false, true); enb_rep.hasError()) {
            ProcessErrorInRepSet(enb_rep);
            PrintOutError("ERROR: Could not set temporary breakpoint.\n");
            FreeRecSet(new_rec);
            continue;
        }

        FreeRecSet(new_rec);

        std::string bpThread;
        if (!bpset->threadId().empty()) {
            bpThread = " thread " + bpset->threadId();
        }

        // Print out status
        if (bpset->isPending()) {
            PrintMsgWithSetInfo(reply->rep_di.c_str(), reply->rep_group.get(),
                                "Temporary breakpoint %d: Pending at %s%s.\n",
                                bpset->id(), bpset->statement().c_str(), bpThread.c_str());
        }
        else if (bpset->line() >= 0) {
            PrintMsgWithSetInfo(reply->rep_di.c_str(), reply->rep_group.get(),
                                "Temporary breakpoint %d: file %s, line %d%s.\n",
                                bpset->id(), bpset->file().c_str(),
                                bpset->line(), bpThread.c_str());
        } else {
            PrintMsgWithSetInfo(reply->rep_di.c_str(), reply->rep_group.get(),
                                "Temporary breakpoint %d in %s%s\n",
                                bpset->id(), bpset->function().c_str(),
                                bpThread.c_str());
        }
    }

    // If we had errors, query the user to set a pending breakpoint if in auto mode
    if (ProcessErrorInRepSet(rep_set) && args.pending_val == PendingBrkType::Auto) {
        // XXX: in gdb, this query defaults to no. but in gdb mi mode, when using the
        // console interpreter fallback, it defaults to yes.
        if (cli_query("Make breakpoint pending on future code object load?", cli_query_default::NO)) {
            // Force pending_val to on and try again
            args.pending_val = PendingBrkType::On;
            return do_set_tbreak(rec_set, args);
        }
    }

    return true;
}

static int cmd_watch_cmd(rec_set_t *rec_set, int argc, const char* const argv[]) {

    if (argc < 2) {
        PrintOutError("Not enough command arguments.\n");
        PrintOutError("Expected: watch foo\n");
        return 1;
    }

    int expIdx = 1;

    dbgbp_type bp_type = DBGBP_WATCH_TYPE;
    if (strcmp(argv[1], "-a") == 0) {
        bp_type = DBGBP_AWATCH_TYPE;
        expIdx = 2;
    } else if (strcmp(argv[1], "-r") == 0) {
        bp_type = DBGBP_RWATCH_TYPE;
        expIdx = 2;
    } else if (strcmp(argv[1], "-w") == 0) {
        bp_type = DBGBP_WATCH_TYPE;
        expIdx = 2;
    }

    int n = argc - expIdx;
    if (n < 1) {
        PrintOutError("Not enough command arguments.\n");
        PrintOutError("Expected: watch %s foo\n", argv[1]);
        return 1;
    } else if (n == 2 || (n == 3 && strcmp(argv[expIdx+1], "thread") != 0) || n > 3) {
        PrintOutError("Too many command arguments.\n");
        PrintOutError("Expected: watch foo\n");
        PrintOutError("      or: watch -a foo\n");
        PrintOutError("      or: watch foo thread 2\n");
        return 1;
    }

    std::string exprStr{argv[expIdx]};
    if (n == 3) {
        exprStr = "\"" + exprStr + " thread " + argv[expIdx+2] + "\"";
    }

    char* expression = strdup(exprStr.c_str());

    auto rep_set = DbgSetWatch(rec_set, bp_type, expression);

    Free(expression);

    ProcessErrorInRepSet(rep_set);

    for (auto&& reply : rep_set) {
        // Point at the breakpoint data
        dbgbp_t *bp = EVENT_BP(reply->rep_event);
        auto* di = reply->rep_di.lock();

        if (auto bpset = di->breakpoints().findByDbgBreakpoint(*bp)) {
            switch (bpset->type()) {
            case DBGBP_WATCH_TYPE:
                PrintMsgWithSetInfo(reply->rep_di.c_str(), reply->rep_group.get(),
                                    "Watchpoint %d: Set on variable %s\n",
                                    bpset->id(), bpset->watchExpression().c_str());
                break;
            case DBGBP_AWATCH_TYPE:
                PrintMsgWithSetInfo(
                    reply->rep_di.c_str(), reply->rep_group.get(),
                    "Access Watchpoint %d: Set on variable %s\n",
                    bpset->id(), bpset->watchExpression().c_str());
                break;
            case DBGBP_RWATCH_TYPE:
                PrintMsgWithSetInfo(reply->rep_di.c_str(), reply->rep_group.get(),
                                    "Read Watchpoint %d: Set on variable %s\n",
                                    bpset->id(), bpset->watchExpression().c_str());
                break;
            default:
                PrintMsgWithSetInfo(
                    reply->rep_di.c_str(), reply->rep_group.get(),
                    "Unknown Watchpoint %d: Set on variable %s\n",
                    bpset->id(), bpset->watchExpression().c_str());
            }
        } else {
            PrintErrorWithSetInfo(reply->rep_di.c_str(), reply->rep_group.get(),
                                  "ERROR: cannot find watchpoint [%d: variable "
                                  "%s] in BPSetList.\n",
                                  bp->bp_fe_id, bp->bp_exp.c_str());
        }
    }

    return 1;
}

static int cmd_catch_cmd(rec_set_t *rec_set, int argc, const char* const argv[]) {
    // There is no special reason that we are limited to these events. dbgsrv
    // is set up to send a generic "catch _____" command with CLIBypass, so
    // any valid event type could work. These are just the few that have been
    // verified to work (and not crash the dbgsrv) so far.
    const auto eventTypes = 
        std::vector<std::string>{"throw", "catch", "load", "unload", "fork", 
                                 "vfork", "exec"};
    
    // build a nicely formatted string out of types
    // e.g. "[throw|load|...|exec]"
    auto buildExpected = [&](){
        auto optsStream = std::ostringstream{};
        optsStream << "[";
        for (const auto &type : eventTypes) {
            optsStream << type << "|";
        }
        auto opts = optsStream.str();
        opts[opts.rfind('|')] = ']';

        return opts;
    };

    if (argc < 2) {
        PrintOutError("Not enough command arguments.\n");
        PrintOutError("Expected: catch %s\n", buildExpected().c_str());
        return 0;
    }

    auto event = std::string{argv[1]};
    if (std::find(eventTypes.begin(), eventTypes.end(), event) == eventTypes.end()) {
        PrintOutError("Invalid catchpoint type: %s\n", event.c_str());
        PrintOutError("Expected: catch %s\n", buildExpected().c_str());
        return 0;
    }

    auto regex = std::string{};
    if ((event == "load" || event == "unload") && argc >= 3) {
        // load and unload support regex arguments to match specific libraries
        regex = argv[2];
    }

    auto repSet = DbgSetCatch(rec_set, event, regex);
    ProcessErrorInRepSet(repSet);

    for (auto& reply : repSet) {
        dbgbp_t *bp = EVENT_BP(reply->rep_event);
        auto* di = reply->rep_di.lock();

        if (auto bpSet = di->breakpoints().findByDbgBreakpoint(*bp); bpSet != nullptr) {
            PrintMsgWithSetInfo(reply->rep_di.c_str(), reply->rep_group.get(),
                "Catchpoint %d: Set on %s.\n",
                bpSet->id(), bpSet->watchExpression().c_str());
        } else {
            PrintErrorWithSetInfo(reply->rep_di.c_str(), reply->rep_group.get(),
                "ERROR: cannot find catchpoint [%d: event "
                "%s] in BPSetList.\n", bp->bp_fe_id, bp->bp_exp.c_str());
        }
    }

    return 1;
}

// called via the delete command: "delete checkpoint x"
static int cmd_delete_checkpoint_cmd(rec_set_t *rec_set, int argc, const char* const argv[]) {
    const auto usage = [](){
        PrintOutError("Usage: delete checkpoint <id>\n");
    };
    
    if (argc < 3 || !IsInteger(argv[2])) {
        usage();
        return 0;
    }

    errno = 0;
    // note that we *don't* use parse_breakpoint_id here - checkpoints are distinct from
    // break/catch/watchpoints.
    auto id = strtoul(argv[2], nullptr, 10);
    if (errno) {
        usage();
        return 0;
    }

    if (auto rep_set = DbgDeleteCheckpoint(rec_set, id); rep_set.hasError()) {
        ProcessErrorInRepSet(rep_set);
        return 0;
    }

    return 1;
}

// parse a string that represents a break/catch/watchpoint id and return the id
// number. the input can be a number or the special string "$bpnum" which
// represents the id of the last break/catch/watchpoint that was set. (but not checkpoint)
//
// if the input can't be parsed, returns 0.
//
// note that $bpnum might be -1 or the id of a breakpoint that has been deleted.
// the caller of this function should make sure the returned value is valid.
static int parse_breakpoint_id(std::string_view str) {
    if (str == "$bpnum") return DbgGetBPNum();

    char* leftover;
    const auto parsed_id = ::strtol(str.data(), &leftover, 10);

    if (*leftover) return 0;
    return parsed_id;
}

static int cmd_delete_breakpoint_cmd(rec_set_t* rec_set, int argc, const char* const argv[]) {
    if (!rec_set) {
        PrintOutError("No process, use launch/attach first.\n");
        return Command::Failure;
    }

    auto bp_id = int{0};

    if (argc < 2) {
        /*
        ** delete all
        */
        if (!cli_query("Delete all breakpoints in focus?")) return 1;
    } else {
        bp_id = parse_breakpoint_id(argv[1]);

        if (bp_id <= 0) {
            PrintOutError("Invalid id %d. (from \"%s\")\n", bp_id, argv[1]);
            return Command::Failure;
        }
    }

    // The dbg_client layer handles all the of the details here
    if (auto rep_set = DbgDeleteBreak(rec_set, bp_id); rep_set.hasError()) {
        ProcessErrorInRepSet(rep_set);
        return Command::Failure;
    }
    return Command::Success;
}

static int cmd_delete_cmd(rec_set_t *rec_set, int argc, const char* const argv[]) {
    // FIXME: this function needs to know about a lot of different systems and is not
    // limited to breakpoints any more. to reduce header/namespace pollution in
    // cmd_break.cpp, this function could probably be moved to a new cmd_delete.cpp.

    // in cmd_script.cpp
    extern int delete_script_cmd(int argc, const char* const argv[]);
    // in cmd_blockmap.cpp
    extern int delete_decomp_cmd(int argc, const char* const argv[]);

    // figure out what we're deleting
    using namespace std::string_literals;
    if (argc >= 2 && argv[1] && "checkpoint"s == argv[1]) {
        return cmd_delete_checkpoint_cmd(rec_set, argc, argv);
    } else if (argc >= 2 && *argv[1] == '$') {
        // an argument that starts with a $ could be an assertion script, decomposition,
        // $bpnum or client side convenience variable

        auto name = std::string_view{argv[1]};

        if (name == "$bpnum") {
            // special breakpoint value $bpnum
            return cmd_delete_breakpoint_cmd(rec_set, argc, argv);
        } else if (script::getScript(name.substr(1))) {
            return delete_script_cmd(argc, argv);
        } else if (FindBlockMap(argv[1])) {
            return delete_decomp_cmd(argc, argv);
        }

        // TODO client side variables

        PrintOutError("Can't delete unknown item: %s\n", argv[1]);
        return Command::Failure;
    } else {
        // otherwise assume breakpoint
        return cmd_delete_breakpoint_cmd(rec_set, argc, argv);
    }
}

static int cmd_disable_cmd(rec_set_t *rec_set, int argc, const char* const argv[]) {
    if (argc < 2) {
        PrintOutError("Usage: disable <break/catch/watchpoint id>\n");
        return Command::Failure;
    }

    const auto bp_id = parse_breakpoint_id(argv[1]);
    if (bp_id <= 0) {
        PrintOutError("Invalid id %d. (from \"%s\"\n", bp_id, argv[1]);
        return Command::Failure;
    }

    // The dbg_client layer handles all the of the details here
    if (auto rep_set = DbgDisableBreak(rec_set, bp_id); rep_set.hasError()) {
        ProcessErrorInRepSet(rep_set);
        return 0;
    }

    return 1;
}

static int cmd_enable_cmd(rec_set_t *rec_set, int argc, const char* const argv[]) {
    int argp = 1;
    int once = 0;
    int delete_cmd = 0;
    int bp_id = 0;

    // FIXME: Currently GDB has no way to turn a breakpoint that was marked as
    //         delete or disable back to keep. We could solve this by internaly
    //         deleting the original breakpoint and creating a new breakpoint
    //         but this would involve modifying the IDSet objects in the
    //         bpset. I don't want to do that right now, and besides doing so
    //         would make us "better" than gdb.

    while (argp < argc) {
        if (strcmp(argv[argp], "-once") == 0) {
            ++argp;
            ++once;
        } else if (strcmp(argv[argp], "-delete") == 0) {
            ++argp;
            ++delete_cmd;
        } else {
            bp_id = parse_breakpoint_id(argv[argp]);
            break;
        }
    }

    if (bp_id <= 0) {
        PrintOutError("Invalid id: %d. (from \"%s\")\n", bp_id, argv[argp]);
        return Command::Failure;
    }

    // you cannot set once and delete at the same time
    if (once && delete_cmd) {
        PrintOutError(
            "The -once and -delete arguments cannot be used at the same "
            "time.\n");
        return Command::Failure;
    }

    // The dbg_client layer handles all the of the details here
    if (auto rep_set = DbgEnableBreak(rec_set, bp_id, once, delete_cmd); rep_set.hasError()) {
        ProcessErrorInRepSet(rep_set);
        return Command::Failure;
    }

    return Command::Success;
}

static int cmd_ignore_cmd(rec_set_t *rec_set, int argc, const char* const argv[]) {
    if (argc != 3) {
        PrintOutError("Arguments required <break/catch/watchpoint id> <ignore-count>.\n");
        return Command::Failure;
    }

    int bp_id = parse_breakpoint_id(argv[1]);
    if (bp_id <= 0) {
        PrintOutError("Invalid id %d. (from \"%s\").\n", bp_id, argv[1]);
        return Command::Failure;
    }

    char* ep;
    int n = strtol(argv[2], &ep, 10);
    if (n < 0 || *ep) {
        PrintOutError("Invalid ignore count %s.\n", argv[2]);
        return Command::Failure;
    }

    // The dbg_client layer handles all the of the details here
    if (auto rep_set = DbgIgnoreBreak(rec_set, bp_id, n); rep_set.hasError()) {
        ProcessErrorInRepSet(rep_set);
        return Command::Failure;
    }

    return Command::Success;
}

static int cmd_condition_cmd(rec_set_t *rec_set, int argc, const char* const argv[]) {
    if (argc < 2) {
        PrintOutError("Breakpoint number required.\n");
    }

    int bp_id = parse_breakpoint_id(argv[1]);
    if (bp_id <= 0) {
        PrintOutError("Invalid id %d. (from \"%s\")\n", bp_id, argv[1]);
        return Command::Failure;
    }

    auto cond = ArgsToString(argv+2, argc-2);

    // The dbg_client layer handles all the of the details here
    if (auto rep_set = DbgConditionBreak(rec_set, bp_id, cond); rep_set.hasError()) {
        ProcessErrorInRepSet(rep_set);
        return Command::Failure;
    }

    return Command::Success;
}

namespace {
    // class for caching the hit and ignore count info from the back end
    class BPCnts {
    public:
        BPCnts() {}
        ~BPCnts() {};
        void retrieve(rec_set_t* rec_set);
        std::string get(const std::string &handle, const Bitset *group, int id);

        struct IgnoreSet {
            std::unique_ptr<Bitset> group;
            int ignore_count;
        };
        std::vector<IgnoreSet> get_ignore_counts(const std::string &handle, int bp_fe_id);
    private:
        rep_set_t rep_set;
    };
}

// Retrieve the hit count and ignore information from the back end.  We don't/can't
// work this from the front end because the is no guarantee the program will get
// the same number of stops on each back end. 
void BPCnts::retrieve(rec_set_t* rec_set) {
    if (!rec_set) return;
    rep_set = DbgInfoBreakpoints(rec_set);
}

// get the extended status for breakpoint id.
// the count information has already been retrieved from the back end
std::string BPCnts::get(const std::string &handle, const Bitset *group, int id) {
    if (rep_set.isEmpty()) return "";

    std::map<std::pair<int,int>, std::unique_ptr<Bitset>> cnts;

    auto formatHit = [](std::ostringstream& res, int cnt) {
        res << "    already hit " << cnt << (cnt != 1 ? " times" : " time"); };
    auto formatIgnore = [](std::ostringstream& res, int ignore) {
        res << "    " << "ignore next " << ignore << (ignore != 1 ? " hits" : " hit"); };

    auto res = std::ostringstream{};
    for (auto&& reply : rep_set) {
        auto rep_group = reply->rep_group.get();
        if (reply->rep_di.name() != handle || !group->hasCommonBits(*rep_group)) {
            continue;
        }
                
        const char* str = reply->rep_event->ev_u.ev_u_string;
        auto is = std::istringstream(str);
        int bp, cnt, ignore;
        while ((is >> bp >> cnt >> ignore) && bp != id) {}
        if (bp != id) continue;

        if (*group == *rep_group) {
            // If they are all the same, then we can just add 0 or 1 lines as the
            // procset info is already printed
            if (cnt > 0) formatHit(res, cnt);
            if (ignore > 0) formatIgnore(res, ignore);
            if (cnt > 0 || ignore > 0) res << "\n";
            cnts.clear();
            break;
        }

        // otherwise, we have either gotten multiple replies or some of the
        // ranks didn't return a result (probably RUNNING, but possibly dead))
        std::pair<int,int> key(cnt,ignore);
        if (auto loc = cnts.find(key); loc != cnts.end()) {
            *loc->second |= *rep_group;
            *loc->second &= *group;
        } else {
            auto igrp = bitset::fromAnd(*group, *rep_group);
            cnts.insert(std::make_pair(key, std::move(igrp)));
        }
    }

    // Otherwise, we have gotten either multiple results or just a subset of the
    // ranks reported (the remainder may be running)
    for (auto& [pr, grp] : cnts) {
        auto [cnt,ignore] = pr;
        // note the grp != group check is for the case were this breakpoint has the same
        // values across all ranks, but a different breakpoint had varying results
        if (cnts.size() > 1 || ((cnt || ignore) && *grp != *group)) {
            res << "    " << GetProcSetString(handle.c_str(), grp.get(), "") << ":";
        }
        if (cnts.size() > 1 || cnt) formatHit(res, cnt);
        if (ignore) formatIgnore(res, ignore);
        if (cnts.size() > 1 || cnt || ignore) res << "\n";
    }
                        
    return res.str();
}

std::vector<BPCnts::IgnoreSet>
BPCnts::get_ignore_counts(const std::string &proc_name, int requested_bp) {
    if (rep_set.isEmpty()) return {};

    auto result = std::vector<BPCnts::IgnoreSet>{};

    for (const auto* reply : rep_set) {
        if (reply->rep_di.name() != proc_name) continue;

        auto is = std::istringstream(reply->rep_event->ev_u.ev_u_string);
        // the backend is aware of frontend ids and reports the result with frontend ids,
        // the name "be_bp" means "the frontend breakpoint id as reported by the backend"
        int be_bp, hit_count, ignore_count;
        while ((is >> be_bp >> hit_count >> ignore_count) && be_bp != requested_bp) {}
        if (be_bp != requested_bp) continue;

        if (ignore_count > 0) {
            result.emplace_back(BPCnts::IgnoreSet{
                .group = reply->rep_group->clone(),
                .ignore_count = ignore_count,
            });
        }
    }

    return result;
}

static int cmd_info_breakpoints_cmd_console(rec_set_t *rec_set, int argc, const char* const argv[]) {
    int bid = 0;
    std::stringstream ss;

    if (argc > 1) {
        if (!IsInteger(argv[1])) {
            PrintOutError("Numeric argument required (breakpoint number).\n");
            return Command::Failure;
        }

        bid = atoi(argv[1]);
    }

    // Print an entry in the breakpoint table with proper spacing
    auto printTableEntry = [](const std::string &handle, const Bitset *group, 
        const std::string &id, const std::string &type, const std::string &disp, 
        const std::string &enabled, const std::string &extra) 
    {
        auto procset = handle;
        if (group) procset += group->toSetString();
        std::ostringstream out;

        out << std::left 
            << std::setw(16) << procset
            << std::setw(4) << id
            << std::setw(15) << type
            << std::setw(5) << disp
            << std::setw(4) << enabled
            << extra
        ;

        PrintOutMsg("%s\n", out.str().c_str());
    };

    BPCnts bpcnts;
    bpcnts.retrieve(rec_set);

    auto printBP = [printTableEntry, &bpcnts](const MappedBreakpoint& bpset,
                                              const std::string &handle, const Bitset *group, 
                                              const std::string &disp,
                                              const std::string &enabled) {
        auto id = std::to_string(bpset.id());
        auto type = BPTypeToStr(bpset.type());

        auto bpThread = std::string{};
        if (!bpset.threadId().empty())
            bpThread = " thread " + bpset.threadId();

        // The info we print is dependent on the type of breakpoint and the
        // enabled/disabled setting and disposition
        auto location = std::ostringstream{};

        switch (bpset.type()) {
        case DBGBP_WATCH_TYPE:
        case DBGBP_AWATCH_TYPE:
        case DBGBP_RWATCH_TYPE:
        case DBGBP_CATCH_TYPE:
            location << bpset.watchExpression() << bpThread;
            break;

        case DBGBP_BREAK_TYPE:
        default:
            if (bpset.isPending()) {
                location << "at " << bpset.statement() << bpThread << " <PENDING>";
            } else if (bpset.line() > 0) {
                location << "in " << bpset.function() << " at "
                         << bpset.file() << ":" << bpset.line() << bpThread;
            } else {
                location << "in " << bpset.function() << bpThread;
            }
            break;
        }

        if (!bpset.condition().empty()) {
            location << " if " << bpset.condition();
        }
        
        printTableEntry(handle, group, id, type, disp, enabled, location.str());
        if (auto hitInfo = bpcnts.get(handle, group, bpset.id()); !hitInfo.empty()) {
            PrintOutMsg("%s", hitInfo.c_str());
        }
    };

    // Print header
    printTableEntry("Procset", nullptr, "Num", "Type", "Disp", "Enb", "What");

    // Need to loop over each di in the rec_set
    for (auto&& recv_ptr : rec_set) {
        if (recv_ptr->rec_di.breakpoints().isEmpty()) {
            PrintMsgWithSetInfo(recv_ptr->rec_di.handle().c_str(), recv_ptr->rec_group.get(),
                                "No breakpoints or watchpoints.\n");
            continue;
        }

        if (bid > 0 && recv_ptr->rec_di.breakpoints().findByLogicalID(bid) == nullptr) {
            PrintErrorWithSetInfo(
                recv_ptr->rec_di.handle().c_str(), recv_ptr->rec_group.get(),
                "No breakpoint or watchpoint number %d.\n", bid);
            continue;
        }

        for (const auto &bpset : recv_ptr->rec_di.breakpoints()) {
            if (bid > 0 && bpset.id() != bid) continue;

            auto ranks = bitset::fromAnd(*bpset.ranksSet(), *recv_ptr->rec_group);
            if (ranks->isEmpty()) continue;

            // loop over the disposition list
            for (const auto &disp_set : bpset.dispSetList()) {
                auto disp_group = bitset::fromAnd(*disp_set.group, *ranks);
                auto enabled_group = bitset::fromAnd(*bpset.enabledSet(), *disp_group);
                auto disabled_group = disp_group->clone();
                *disabled_group -= *enabled_group;

                for (auto isEnabled = 2; isEnabled--; ) {
                    const auto* group = isEnabled ? enabled_group.get() : disabled_group.get();
                    if (group->isEmpty()) continue;

                    auto enabled = std::string{isEnabled ? "y" : "n"};
                    auto disp = BPDispToStr(disp_set.disp);

                    printBP(bpset, recv_ptr->rec_di.handle().name(),
                            group, disp, enabled);
                } // for (isEnabled)
            }
            if (bid >= 0 && bpset.id() == bid) break;
        } // for (const auto &bpset : recv_ptr->rec_di.breakpoints())
    } // for (auto&& recv_ptr : rec_set) {

    return Command::Success;
}

static int cmd_info_breakpoints_cmd_mi(MiInterpreter& mi, rec_set_t *rec_set, int argc, const char* const argv[]) {
    if (!rec_set) {
        mi.set_result("BreakpointTable={body=[]}");
        return Command::Success;
    }

    int bid = 0;
    std::stringstream ss;

    if (argc > 1) {
        if (!IsInteger(argv[1])) {
            PrintOutError("Numeric argument required (breakpoint number).\n");
            return Command::Failure;
        }

        bid = atoi(argv[1]);
    }

    BPCnts bpcnts;
    bpcnts.retrieve(rec_set);

    // Add entry to breakpoint table string for MI mode
    auto addMiEntry = [&ss]( const MappedBreakpoint& bpset,
                             const std::string &disp, 
                             const std::string &enabled,
                             const std::string &fullpath)
    {
        auto id = bpset.id();
        auto type = BPTypeToStr(bpset.type());
        auto func = bpset.function();
        auto file = bpset.file();
        auto line = bpset.line();
        
        if (!ss.str().empty()){
            ss<<",";
        }

        ss <<"bkpt={number=\""<<id
           <<"\",type=\""<<type
           <<"\",disp=\""<<disp
           <<"\",enabled=\""<<enabled
           <<"\",func=\""<<func
           <<"\",file=\""<<file
           <<"\",fullname=\""<<fullpath<<"\""
           <<",line=\""<<line<<"\"}";
    };

    // Need to loop over each di in the rec_set
    for (auto&& recv_ptr : rec_set) {
        if (bid > 0 && recv_ptr->rec_di.breakpoints().findByLogicalID(bid) == nullptr) {
            PrintErrorWithSetInfo(
                recv_ptr->rec_di.handle().c_str(), recv_ptr->rec_group.get(),
                "No breakpoint or watchpoint number %d.\n", bid);
            continue;
        }

        for (const auto &bpset : recv_ptr->rec_di.breakpoints()) {
            if (bid > 0 && bpset.id() != bid) continue;

            auto ranks = bitset::fromAnd(*bpset.ranksSet(), *recv_ptr->rec_group);
            if (ranks->isEmpty()) continue;

            // loop over the disposition list
            for (const auto &disp_set : bpset.dispSetList()) {
                auto disp_group = bitset::fromAnd(*disp_set.group, *ranks);
                auto enabled_group = bitset::fromAnd(*bpset.enabledSet(), *disp_group);
                auto disabled_group = disp_group->clone();
                *disabled_group -= *enabled_group;

                for (auto isEnabled = 2; isEnabled--; ) {
                    const auto* group = isEnabled ? enabled_group.get() : disabled_group.get();
                    if (group->isEmpty()) continue;

                    auto enabled = std::string{isEnabled ? "y" : "n"};
                    auto disp = BPDispToStr(disp_set.disp);

                    addMiEntry(bpset,disp,enabled,
                               recv_ptr->rec_di.sourcePaths().getFullPath(bpset.file()));
                } // for (isEnabled)
            }
            if (bid >= 0 && bpset.id() == bid) break;
        } // for (const auto &bpset : recv_ptr->rec_di.breakpoints())

        std::string group = recv_ptr->rec_group.get()->toSetString();
        mi.set_result("proc_set=\"%s\", group=\"%s\",BreakpointTable={body=[%s]}",
                            recv_ptr->rec_di.handle().c_str(),group.substr(1,group.length()-2).c_str(),
                            ss.str().c_str());
        // clear ss stream for next recv_ptr 
        ss.str("");

    } // for (auto&& recv_ptr : rec_set) {

    return Command::Success;
}

static int cmd_set_break_pending_cmd(int argc, const char* const argv[]) {


    if (argc < 2) {
        PrintOutError("Not enough command arguments.\n");
        PrintOutError("Expected: set breakpoint pending [auto|on|off]\n");
        return 0;
    }

    // only look at the first arg
    auto s = std::string{argv[1]};

    PendingBrkType pending_val;

    if (s == "auto") pending_val = PendingBrkType::Auto;
    else if (s == "on") pending_val = PendingBrkType::On;
    else if (s == "off") pending_val = PendingBrkType::Off;
    else {
        PrintOutError("Encountered something other than auto or on or off.\n");
        PrintOutError("See \"help set breakpoint pending\" for more information.\n");
        return 0;
    }

    Client.brk_pending = pending_val;
    return 1;
}

static int cmd_show_break_pending_cmd() {
    if (Client.brk_pending == PendingBrkType::Auto)
        PrintOutMsg("Allow setting of pending breakpoints will ask user first. (auto)\n");
    else if (Client.brk_pending == PendingBrkType::On)
        PrintOutMsg("Allow setting of pending breakpoints enabled.\n");
    else
        PrintOutMsg("Allow setting of pending breakpoints disabled.\n");

    return 1;
}

// given a breakpoint/watchpoint/catchpoint (`mapped_bp`), current
// `ignore_counts`, and the procset that the breakpoint is on (`proc_name`),
// return a string of commands that could be used to reconstruct it.
//
// the returned commands assume that a job with the same name as `proc_name` has already
// been launched and that it was launched with the same number of ranks as it was
// originally.
//
// on failure, the function returns an empty string.
static std::string reconstruct_breakpoint_commands(const MappedBreakpoint& mapped_bp,
                                                   BPCnts& ignore_counts,
                                                   const std::string& proc_name)
{
    auto ss = std::ostringstream{};

    // focus only on ranks where this breakpoint was set
    ss << "focus $" << proc_name << mapped_bp.ranksSet()->toSetString() << std::endl;

    // breakpoint command
    switch (mapped_bp.type()) {
        case DBGBP_BREAK_TYPE: {
            ss << "break";
            if (mapped_bp.line() > 0 && !mapped_bp.file().empty()) {
                ss << " " << mapped_bp.file() << ":" << mapped_bp.line();
            } else if (!mapped_bp.function().empty()) {
                ss << " " << mapped_bp.function();
            } else if (!mapped_bp.address().empty()) {
                ss << " *" << mapped_bp.address();
            } else {
                // failure: no location
                return "";
            }

            break;
        }

        case DBGBP_WATCH_TYPE: {
            ss << "watch " << mapped_bp.watchExpression();
            break;
        }

        case DBGBP_CATCH_TYPE: {
            // catchpoints store their parameter in .watchExpression. this is a human
            // readable string and not always appropriate for directly using in a catch command.
            // we need to transform certain ones back into their command versions.
            auto type = mapped_bp.watchExpression();

            const auto type_map = std::vector<std::pair<std::string, std::string>> {
                {"exception throw", "throw"},
                {"exception catch", "catch"},
                {"load of library matching", "load"},
                {"load of library", "load"},
                {"unload of library matching", "unload"},
                {"unload of library", "unload"},
            };
            // others don't need any transformation

            for (const auto& [find, replace] : type_map) {
                if (type.starts_with(find)) {
                    type.replace(0, find.size(), replace);
                    break;
                }
            }

            ss << "catch " << type;
            break;
        }

        default: {
            // failure: unimplemented bp type
            return "";
        }
    }

    // append thread to breakpoint command
    if (!mapped_bp.threadId().empty()) {
        ss << " thread " << mapped_bp.threadId();
    }

    // end breakpoint command
    ss << std::endl;

    // rest of the function is setting ignore/enabled/disposition/condition

    // condition
    if (!mapped_bp.condition().empty()) {
        // XXX: PE-51457: conditions are stored client side on a logical
        // breakpoint but can differ on each physical breakpoint on the
        // backend. saving the condition this way is wrong in some
        // situations (see ticket) but is the best we can do for now.
        ss << "condition $bpnum " << mapped_bp.condition() << std::endl;
    }

    // enabled/disabled
    if (*mapped_bp.ranksSet() != *mapped_bp.enabledSet()) {
        auto disabled_set = mapped_bp.enabledSet()->clone();
        disabled_set->invert();

        ss << "focus $" << proc_name << disabled_set->toSetString() << std::endl;
        ss << "disable $bpnum" << std::endl;
    }

    // dispositions
    for (const auto& disp_set : mapped_bp.dispSetList()) {
        if (disp_set.disp != DBGBP_KEEP_DISP) {
            ss << "focus $" << proc_name << disp_set.group->toSetString() << std::endl;
            // assuming the only other two options are keep and delete
            const auto option = disp_set.disp == DBGBP_DISABLE_DISP ? "-once" : "-delete";
            ss << "enable " << option << " $bpnum" << std::endl;
        }
    }

    // ignore counts
    for (const auto& ignore_set : ignore_counts.get_ignore_counts(proc_name, mapped_bp.id())) {
        ss << "focus $" << proc_name << ignore_set.group->toSetString() << std::endl;
        ss << "ignore $bpnum " << ignore_set.ignore_count << std::endl;
    }

    return ss.str();
}

// also called from cmd_utils.cpp
int save_all_breakpoints(rec_set_t* rec_set, std::ostream& os) {
    auto result = Command::Success;
    auto should_refocus = false;

    auto ignore_counts = BPCnts{};
    ignore_counts.retrieve(rec_set);

    for (const auto& rec : rec_set) {
        for (const auto& mapped_bp : rec->rec_di.breakpoints()) {
            const auto command = reconstruct_breakpoint_commands(mapped_bp, ignore_counts,
                                                                 rec->rec_di.handle().name());
            os << command << std::endl;
            if (command.empty()) {
                PrintOutError("Warning: Failed to save breakpoint %s::%d.\n",
                            rec->rec_di.handle().c_str(), mapped_bp.id());
                result = Command::Failure;
                continue;
            }

            // the reconstructed breakpoint commands include a focus command.
            // make sure to reset the focus afterwards.
            should_refocus = true;
        }
    }

    // XXX: we have no way to know if this was actually the focus state when the
    // user started the source command.
    if (should_refocus) os << "focus $all" << std::endl;

    return result;
}

static int save_breakpoints_cmd(rec_set_t* rec_set, int argc, const char* const argv[]) {
    // filename required. argc starts counting from "breakpoints" in "set breakpoints <filename>".
    if (argc < 2) {
        PrintOutError("Usage: save breakpoints [filename]\n");
        return Command::Failure;
    }

    auto outfile = std::ofstream{argv[1]};
    if (!outfile) {
        PrintOutError("Failed to open %s for writing.\n", argv[1]);
        return Command::Failure;
    }

    if ((save_all_breakpoints(rec_set, outfile) == Command::Success) && outfile) {
        PrintOutMsg("Saved breakpoints to %s. Use the source command to load them.\n", argv[1]);
        return Command::Success;
    } else {
        // error already printed
        return Command::Failure;
    }
}

void insert_setshow_commands(CommandNode& root, std::string set, std::string show) {
    root.addSubCmd(
        set + " breakpoint", nullptr, proc_ignore,
         nullptr, class_set_break_help,
         "Set information about breakpoints.");

    root.addSubCmd(
        set + " breakpoint pending", nullptr, proc_ignore,
        cmd_set_break_pending_cmd,
        cmd_set_break_pending_help,
        "Set behavior for pending breakpoints.");

    root.addSubCmd(
        show + " breakpoint", nullptr, proc_ignore,
         nullptr, class_show_break_help,
         "Show information about breakpoints.");

    root.addSubCmd(
        show + " breakpoint pending", nullptr, proc_ignore,
        cmd_show_break_pending_cmd,
        cmd_show_break_pending_help,
        "Show behavior for pending breakpoints.");
}

void ConsoleCommandsFactory::insert_break_commands(CommandNode& root) {
    root.addSubCmd(
        "info breakpoints", nullptr, proc_required,
        cmd_info_breakpoints_cmd_console, cmd_info_breakpoints_help,
        "Show status of breakpoints.");

    root.addSubCmd(
        "break", "b", proc_required, cmd_break_cmd_console,
        cmd_break_help, "Set breakpoint at specified line or function.");
    root.addSubCmd(
        "delete", "d", proc_optional, cmd_delete_cmd,
        cmd_delete_help, "Delete breakpoints and other items.");
    root.addSubCmd(
        "disable", nullptr, proc_required, cmd_disable_cmd,
        cmd_disable_help, "Disable a breakpoint.");
    root.addSubCmd(
        "enable", nullptr, proc_required, cmd_enable_cmd,
        cmd_enable_help, "Enable a breakpoint.");
    root.addSubCmd(
        "ignore", nullptr, proc_required, cmd_ignore_cmd,
        cmd_ignore_help, "Ignore a breakpoint a number of times.");
    root.addSubCmd(
        "condition", nullptr, proc_required, cmd_condition_cmd,
        cmd_condition_help, "Stop only if a condition is met.");

    root.addSubCmd(
        "tbreak", nullptr, proc_required, cmd_tbreak_cmd, cmd_tbreak_help,
        "Set a temporary breakpoint at specified line or function.");
    root.addSubCmd(
        "watch", nullptr, proc_required, cmd_watch_cmd,
        cmd_watch_help, "Set watchpoint on specified expression.");
    root.addSubCmd(
        "catch", nullptr, proc_required, cmd_catch_cmd, 
        cmd_catch_help, "Set catchpoint on specified event.");

    root.addSubCmd(
        "save breakpoints", nullptr, proc_required, save_breakpoints_cmd,
        cmd_save_breakpoints_help, "Save breakpoints to a file.");

    insert_setshow_commands(root, "set", "show");
}

void MiCommandsFactory::insert_break_commands(CommandNode& root) {
    root.addSubCmd({
        .name = "-break-list", .ptype = proc_optional,
        .fn = cmd_info_breakpoints_cmd_mi });

    // note that the only breakpoint function that had an "if (Client.MI_mode)" was
    // cmd_break_cmd, so I'm mapping the rest to their console versions for now.
    // this will probably change later as we add more mi-specific behavior.

    root.addSubCmd({
        .name = "-break-insert", .ptype = proc_required,
        .fn = cmd_break_cmd_mi });
    root.addSubCmd({
        .name = "-break-delete", .ptype = proc_required, .fn = cmd_delete_cmd });
    root.addSubCmd({
        .name = "-break-disable", .ptype = proc_required, .fn = cmd_disable_cmd });
    root.addSubCmd({
        .name = "-break-enable", .ptype = proc_required, .fn = cmd_enable_cmd });
    root.addSubCmd({
        .name = "-break-after", .ptype = proc_required, .fn = cmd_ignore_cmd });
    root.addSubCmd({
        .name = "-break-condition", .ptype = proc_required, .fn = cmd_condition_cmd });

    insert_setshow_commands(root, "-gdb-set", "-gdb-show");
}
