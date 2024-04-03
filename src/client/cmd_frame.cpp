/*********************************************************************************\
 * Stack frame commands.
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

#include <stdio.h>
#include <string>
#include <cstring>
#include <sstream>
#include <vector>
#include <algorithm>
#include <iostream>

#include "client.h"
#include "useful.h"

#include "interpreter_mi.h"
#include "bttree.h"

void PrintFrameSet(const char *setname, Bitset *group, const dbgframe_t *frame) {

    for (auto f = frame; f ; f = f->getNextThreadFrame()) {

        PrintMsgWithSetAndThreadInfo(setname, group, f->getThreads(), "#%-2d ", f->getLevel());

        const auto &loc = f->getLoc();
        if (*loc.loc_addr != '\0')
            PrintOutMsg("%s in ", loc.loc_addr);

        PrintOutMsg("%s", loc.loc_func);

        if (*loc.loc_file != '\0')
            PrintOutMsg(" at %s:%d", loc.loc_file, loc.loc_line);

        PrintOutMsg("\n");
    }
}

static int movecmd(rec_set_t *rec_set, int argc, const char* const argv[]) {
    int count = 1;

    auto dir = [&](){
        // "up", "down", "select-frame" (undocumented) , and "-stack-select-frame" (undocumented)
        // are all routed to movecmd. check the first letter to determine what operation we're doing.
        switch (*argv[0]) {
            case 'u': return DbgFrameDir::UP;
            case 'd': return DbgFrameDir::DOWN;
            // else "select-frame" or "-stack-select-frame"
            default: return DbgFrameDir::ABSOLUTE;
        }
    }();

    if (argc >= 2) count = atoi(argv[1]);

    auto rep_set = DbgMoveFrame(rec_set, count, dir);

    // process error
    ProcessErrorInRepSet(rep_set);

    // process frames
    for (auto&& reply : rep_set) {
        // Point at the frame data
        auto* f = EVENT_FRAME(reply->rep_event);

        PrintFrameSet(reply->rep_di.c_str(), reply->rep_group.get(), f);
    }

    return Command::Success;
}

static int cmd_backtrace_console(rec_set_t *rec_set, int argc, const char* const argv[]) {
    int count = 0;
    if (argc >= 2) count = atoi(argv[1]);
    auto rep_set = DbgShowFrame(rec_set, count);

    // process error - even with an error, continue and print what we can
    (void)ProcessErrorInRepSet(rep_set);

    {
        auto pager = cli_pager{};
        auto root = BTTree::fromFrames(rep_set);
        root.print();
    }

    return Command::Success;
}

static int cmd_backtrace_mi(MiInterpreter& mi, rec_set_t *rec_set, int argc, const char* const argv[]) {
    int count = 0;
    if (argc >= 2) count = atoi(argv[1]);

    auto rep_set = DbgShowFrame(rec_set, count);

    // process error - even with an error, continue and print what we can
    (void)ProcessErrorInRepSet(rep_set);

    for (const auto& reply : rep_set) {
        std::stringstream ss;
        int n = 0;

        for (const dbgframe_t* f = EVENT_FRAME(reply->rep_event); f; f = f->getNext(), ++n) {
            // deepest (farthest from main) frame will be in the front
            auto loc = f->getLoc();

            ss << (n ? "," : "[");
            ss << "frame={level=\"" << f->getLevel() << "\","
               << (f->getLevel() ? "" : mi_dummyFrameAddr)
               << "func=\"" << loc.loc_func << "\",file=\"" << loc.loc_file
               << "\",fullname=\""<<loc.loc_file<<"\",line=\""<<loc.loc_line<<"\"}";
        }

        if (ss.str().empty()){
            ss<<"[";
        }
        ss<<"]";
        std::string group = reply->rep_group.get()->toSetString();
        mi.set_result("proc_set=\"%s\",group=\"%s\",stack=%s",
                         reply->rep_di.c_str(), group.substr(1,group.length()-2).c_str(),
                         ss.str().c_str());
        // XXX: while splitting cmd_backtrace_cmd into console and mi functions,
        // I noticed that this is printing ^done, but proceeding with the loop
        // anyway, potentially printing ^done multiple times? leaving as is for
        // now...
    }

    return Command::Success;
}

struct FrameArgs {
    bool verbose = false;
};

static std::optional<FrameArgs> cmd_frame_parse_args(int argc, const char* const argv[]) {
    auto result = FrameArgs{};
    if (argc == 2 && std::string{argv[1]} == "-v") result.verbose = true;
    else if (argc > 1) {
        PrintOutError("Unrecognized argument, expected \"-v\"\n");
        return {};
    }
    return result;
}

static int cmd_frame_console(rec_set_t *rec_set, int argc, const char* const argv[]) {
    auto maybe_frame_args = cmd_frame_parse_args(argc, argv);
    if (!maybe_frame_args) return Command::Failure; // error already emitted in parse
    auto& frame_args = *maybe_frame_args;

    auto rep_set = DbgGetFrame(rec_set, frame_args.verbose);

    // process error - even on error, continue and print what we can
    (void)ProcessErrorInRepSet(rep_set);

    // process frame
    for (auto&& reply : rep_set) {
        // Point at the frame data
        auto fp = EVENT_FRAME(reply->rep_event);
        PrintFrameSet(reply->rep_di.c_str(), reply->rep_group.get(), fp);
    }

    return Command::Success;
}

static int cmd_frame_mi(MiInterpreter& mi, rec_set_t *rec_set, int argc, const char* const argv[]) {
    auto maybe_frame_args = cmd_frame_parse_args(argc, argv);
    if (!maybe_frame_args) return Command::Failure; // error already emitted in parse
    auto& frame_args = *maybe_frame_args;

    auto rep_set = DbgGetFrame(rec_set, frame_args.verbose);

    if (ProcessErrorInRepSet(rep_set)) return Command::Failure;

    for (auto&& reply : rep_set) {
        auto f = EVENT_FRAME(reply->rep_event);
        auto loc = f->getLoc();
        std::string group = reply->rep_group.get()->toSetString();
        mi.set_result(
            "proc_set=\"%s\",group=\"%s\",frame={level=\"%d\",addr=\"%s\","
            "func=\"%s\",file=\"%s\",fullname=\"%s\",line=\"%d\"}",
            reply->rep_di.c_str(), group.substr(1, group.length() - 2).c_str(),
            f->getLevel(), loc.loc_addr, loc.loc_func, loc.loc_file,
            loc.loc_file, loc.loc_line);
        break; // just print the first (XXX: seems wrong?)
    }

    return Command::Success;
}

static int cmd_maint_cached_frames(rec_set_t* rec_set) {
    // show all the frames currently held in the frame caches of `rec_set`
    for (const auto* recv : rec_set) {
        if (auto* cached_frames = recv->rec_di.getCachedFrames()) {
            for (const auto& loc : *cached_frames) {
                PrintOutMsg("%s:%d %s @ %s\n", loc.loc_file, loc.loc_line, loc.loc_func, loc.loc_addr);
            }
        }
    }

    return Command::Success;
}

void ConsoleCommandsFactory::insert_frame_commands(CommandNode& root) {
    root.addSubCmd({
        .name = "frame", .ptype = proc_required,
        .fn = cmd_frame_console, .help = cmd_frame_help,
        .desc = "Print the currently selected stack frame."});
    root.addSubCmd({
        .name = "backtrace", .alias = "bt", .ptype = proc_required,
        .fn = cmd_backtrace_console, .help = cmd_backtrace_help,
        .desc = "Print backtrace of all stack frames."});
    root.addSubCmd({
        .name = "up", .ptype = proc_required,
        .fn = movecmd, .help = cmd_up_help,
        .desc = "Move up one or more stack frames."});
    root.addSubCmd({
        .name = "down", .ptype = proc_required,
        .fn = movecmd, .help = cmd_down_help,
        .desc = "Move down one or more stack frames."});
    root.addSubCmd({
        .name = "maint list-cached-frames", .ptype = proc_required,
        .fn = cmd_maint_cached_frames, .help = nullptr,
        .desc = "Show cached frame data."});

    // select-frame is undocumented
    root.addSubCmd({
        .name = "select-frame", .ptype = proc_required, .fn = movecmd });
}

void MiCommandsFactory::insert_frame_commands(CommandNode& root) {
    root.addSubCmd({
        .name = "-stack-info-frame", .ptype = proc_required, .fn = cmd_frame_mi });
    root.addSubCmd({
        .name = "-stack-list-frames", .ptype = proc_required, .fn = cmd_backtrace_mi });
    root.addSubCmd({
        .name = "-stack-select-frame", .ptype = proc_required, .fn = movecmd });
}
