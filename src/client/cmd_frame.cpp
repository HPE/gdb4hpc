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

namespace {
/*
backtrace tree

used in cmd_backtrace_cmd (cmd_frame.cpp) to print a pstree-like 
representation of the results of a backtrace command.

copies setname, debugframe_t, and group pointers and does not modify them.
*/
class BTTree {
public:
    BTTree() = delete;
    BTTree(const BTTree &other) = delete;
    BTTree(BTTree &&other);
    BTTree(const char* setname, const Bitset* group, const dbgframe_t* frame);

    void print(std::string indent = "", bool is_last = true) const;
    void insertBacktrace(BTTree &bt);
    void transferChild(BTTree &bt);
    void condenseTree();
    void orderTree();
    void calculateTreeGroups();

    BTTree& operator=(const BTTree &other) = delete;
    BTTree& operator=(BTTree &&other);
    bool operator==(const BTTree &other) const;
private:
    void printData(const Bitset* group) const;
    size_t lowestRank() const;

    std::string m_setname;
    std::unique_ptr<Bitset> m_dirtyGroup; // newly allocated group that is sometimes needed. owned by the tree.
    const Bitset* m_group;                // original group: not owned by the tree
    const dbgframe_t* m_frame;
    std::vector<BTTree> m_lateral;
    std::vector<BTTree> m_children;
};
}

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

static int movecmd(Command::RunContext&, rec_set_t *rec_set, int argc, const char* const argv[]) {
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

static int cmd_backtrace_console(Command::RunContext&, rec_set_t *rec_set, int argc, const char* const argv[]) {
    int count = 0;
    if (argc >= 2) count = atoi(argv[1]);
    auto rep_set = DbgShowFrame(rec_set, count);

    // process error - even with an error, continue and print what we can
    (void)ProcessErrorInRepSet(rep_set);

    // turn backtraces into tree format ////////////////////////////////////////

    auto dummySet = std::make_unique<DenseBitset>(1);
    BTTree root("", dummySet.get(), nullptr);
    // root is a blank root node which serves to hold multiple trees.
    // it needs a dummy bitset for the recursive group bitset merging to work.

    for (const auto& reply : rep_set) {
        // reply sets hold stack frames in "reverse" (deepest first) order, while
        // we want to print them in most shallow (i.e. main) first order.
        std::vector<BTTree> frames;

        std::stringstream ss;
        int n = 0;

        for (const dbgframe_t* f = EVENT_FRAME(reply->rep_event); f; f = f->getNext(), ++n) {
            // deepest (farthest from main) frame will be in the front
            frames.push_back(BTTree(reply->rep_di.c_str(), reply->rep_group.get(), f));
            auto loc = f->getLoc();
        }

        // link frames up
        for (auto it = frames.begin(); it != frames.end(); ++it) {
            if (std::next(it) != frames.end()) {
                std::next(it)->transferChild(*it);
            }
        }

        root.insertBacktrace(frames.back()); // insert into root tree and merge common paths
    }

    root.condenseTree();
    root.calculateTreeGroups();
    root.orderTree();

    {
        auto pager = cli_pager{};
        root.print();
    }

    return Command::Success;
}

static int cmd_backtrace_mi(Command::LoweredContext<MiInterpreter>& ctx, rec_set_t *rec_set, int argc, const char* const argv[]) {
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
        ctx.caller.set_result("proc_set=\"%s\",group=\"%s\",stack=%s",
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

static int cmd_frame_console(Command::RunContext&, rec_set_t *rec_set, int argc, const char* const argv[]) {
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

static int cmd_frame_mi(Command::LoweredContext<MiInterpreter>& ctx, rec_set_t *rec_set, int argc, const char* const argv[]) {
    auto maybe_frame_args = cmd_frame_parse_args(argc, argv);
    if (!maybe_frame_args) return Command::Failure; // error already emitted in parse
    auto& frame_args = *maybe_frame_args;

    auto rep_set = DbgGetFrame(rec_set, frame_args.verbose);

    if (ProcessErrorInRepSet(rep_set)) return Command::Failure;

    for (auto&& reply : rep_set) {
        auto f = EVENT_FRAME(reply->rep_event);
        auto loc = f->getLoc();
        std::string group = reply->rep_group.get()->toSetString();
        ctx.caller.set_result(
            "proc_set=\"%s\",group=\"%s\",frame={level=\"%d\",addr=\"%s\","
            "func=\"%s\",file=\"%s\",fullname=\"%s\",line=\"%d\"}",
            reply->rep_di.c_str(), group.substr(1, group.length() - 2).c_str(),
            f->getLevel(), loc.loc_addr, loc.loc_func, loc.loc_file,
            loc.loc_file, loc.loc_line);
        break; // just print the first (XXX: seems wrong?)
    }

    return Command::Success;
}

void ConsoleCommandsFactory::insert_frame_commands(Command& root) {
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

    // select-frame is undocumented
    root.addSubCmd({
        .name = "select-frame", .ptype = proc_required, .fn = movecmd });
}

void MiCommandsFactory::insert_frame_commands(Command& root) {
    const auto lower = Command::specify<MiInterpreter>;

    root.addSubCmd({
        .name = "-stack-info-frame", .ptype = proc_required, .fn = lower(cmd_frame_mi) });
    root.addSubCmd({
        .name = "-stack-list-frames", .ptype = proc_required, .fn = lower(cmd_backtrace_mi) });
    root.addSubCmd({
        .name = "-stack-select-frame", .ptype = proc_required, .fn = movecmd });
}

////////////////////////////////////////////////////////////////////////////////
// BTTree
////////////////////////////////////////////////////////////////////////////////

BTTree::BTTree(const char* setname, const Bitset* group, const dbgframe_t* frame) 
    : m_setname(setname), m_dirtyGroup{nullptr}, m_group{group}, m_frame{frame}
{}

BTTree::BTTree(BTTree &&other)
    : m_setname{std::move(other.m_setname)}, m_dirtyGroup{std::move(other.m_dirtyGroup)}
    , m_group{std::move(other.m_group)}, m_frame{std::move(other.m_frame)}
    , m_lateral{std::move(other.m_lateral)}, m_children{std::move(other.m_children)}
{
    other.m_group = nullptr;
}

void BTTree::print(std::string indent, bool is_last) const {
    bool has_info = (m_group != nullptr && m_frame != nullptr);

    std::string nodeMark = "|_ ";

    if (has_info) { // skip the root node
        if (is_last) {
            nodeMark = "\\_ ";
        }

        // substr(3) is a hack to shave off the first 3 characters of the tree.
        // since there is an invisible master root node, every backtrace tree is
        // indented by 3 spaces by default. we want it to look like we're printing
        // multiple tress instead of one big tree.
        PrintOutMsg("%s", (indent + nodeMark).substr(3).c_str());
        printData(m_dirtyGroup ? m_dirtyGroup.get() : m_group);
        for (auto &lat : m_lateral) {
            if (is_last) {
                nodeMark = "   ";
            } else {
                nodeMark = "|  ";
            }

            PrintOutMsg("%s", (indent + nodeMark).substr(3).c_str());
            lat.printData(m_dirtyGroup ? m_dirtyGroup.get() : m_group);
        }

        if (is_last) {
            indent += "   ";
        } else {
            indent += "|  ";
        }

        if (m_children.size() == 0) {
            // extra spaces to make sure substr(3) never throws an exception
            PrintOutMsg("%s\n", (indent + "   ").substr(3).c_str());
        } else {
            PrintOutMsg("%s\n", (indent + "|  ").substr(3).c_str());
        }
    }

    for (size_t i = 0; i < m_children.size(); ++i) {
        m_children[i].print(indent, i == m_children.size() - 1);
    }
}

void BTTree::insertBacktrace(BTTree &bt) {
    if (m_children.size() == 0) {
        // no children; nothing to merge with; just add here.
        m_children.push_back(std::move(bt));
    } else {
        // look for a matching child node
        auto it = std::find_if(m_children.begin(), m_children.end(), 
            [&](const BTTree &child) {
                return child == bt;
            });

        if (it != m_children.end()) {
            // match found; merge it and move down
            if (bt.m_children.size() != 0) {
                // backtraces will only have one child
                it->insertBacktrace(bt.m_children.front());
            }
        } else {
            // match not found; add a new branch
            m_children.push_back(std::move(bt));
        }
    }
}

void BTTree::transferChild(BTTree &t) {
    m_children.push_back(std::move(t));
}

void BTTree::printData(const Bitset* group) const {
    if (group == nullptr || m_frame == nullptr) {
        return;
    }

    PrintMsgWithSetInfo(m_setname.c_str(), group, "#%-2d ", m_frame->getLevel());

    const auto &loc = m_frame->getLoc();
    PrintOutMsg("%s", loc.loc_func);

    if (*loc.loc_file != '\0')
        PrintOutMsg(" at %s:%d", loc.loc_file, loc.loc_line);

    PrintOutMsg("\n");
}

void BTTree::condenseTree() {
    for (auto &child : m_children) {
        child.condenseTree();
    }

    // don't do this if we are master root; master root only serves as a holder
    // for unique backtraces as children
    if (m_children.size() == 1 && m_frame != nullptr) {
        BTTree& child = m_children.front();

        // put child into lateral list
        m_lateral.push_back(std::move(child));

        // inherit child's children
        m_children = std::move(m_lateral.back().m_children);
        
        // add any lateral children the child had to our lateral list
        for (auto &nested : m_lateral.back().m_lateral) {
            m_lateral.push_back(std::move(nested));
        }
    }
}

void BTTree::orderTree() {
    // sort each node's children in ascending order according to the lowest
    // rank in their set. this method is useful for making the tree's output
    // deterministic instead of subject to the order that data comes in.

    auto rankCompare = [](const BTTree &lhs, const BTTree &rhs) {
        return lhs.lowestRank() < rhs.lowestRank();
    };

    std::sort(m_children.begin(), m_children.end(), rankCompare);

    for (auto &child : m_children) {
        child.orderTree();
    }
}

size_t BTTree::lowestRank() const {
    if (m_dirtyGroup) {
        return m_dirtyGroup->firstSet();
    } else {
        return m_group->firstSet();
    }
}

void BTTree::calculateTreeGroups() {
    for (auto &child : m_children) {
        child.calculateTreeGroups();
    }

    // leaf nodes will have pointers to the groups they were given at creation time.

    if (m_children.size() != 0 && !m_dirtyGroup) {
        m_dirtyGroup = std::make_unique<DenseBitset>(m_children.front().m_group->sizeInBits());
    
        for (const auto &child : m_children) {
            if (child.m_dirtyGroup) {
                *m_dirtyGroup |= *child.m_dirtyGroup;
            } else {
                *m_dirtyGroup |= *child.m_group;
            }
        }
    }

}

BTTree& BTTree::operator=(BTTree &&other) {
    m_setname = std::move(other.m_setname);
    m_dirtyGroup = std::move(other.m_dirtyGroup);
    m_group = std::move(other.m_group);
    m_frame = std::move(other.m_frame);
    m_lateral = std::move(other.m_lateral);
    m_children = std::move(other.m_children);
    other.m_group = nullptr;

    return *this;
}

bool BTTree::operator==(const BTTree &other) const {
    return m_setname == other.m_setname
        && std::strcmp(m_frame->getLoc().loc_addr, other.m_frame->getLoc().loc_addr) == 0;
}
