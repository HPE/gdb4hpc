/*********************************************************************************\
 * Stack frame commands.
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * Copyright 2010-2020 Hewlett Packard Enterprise Development LP.
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
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <cstring>
#include <vector>
#include <algorithm>

#include "client.h"
#include "useful.h"

namespace {
/*
backtrace tree

used in cmd_backtrace_cmd (cmd_frame.cpp) to print a pstree-like 
representation of the results of a backtrack command.

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

static int movecmd(rec_set_t *rec_set, int argc, char **argv) {
    int up = 0;
    int count = 1;
    dbgframe_t *f;

    if (*argv[0] == 'u') up++;

    if (argc >= 2) count = atoi(argv[1]);

    rep_set_t *rep_set;

    rep_set = DbgMoveFrame(rec_set, count, up);

    // process error
    ProcessErrorInRepSet(rep_set);

    // process frames
    for (auto&& reply : rep_set) {
        // Point at the frame data
        f = EVENT_FRAME(reply->rep_event);

        PrintFrameSet(reply->rep_di.c_str(), reply->rep_group.get(), f);
    }

    FreeRepSet(rep_set);

    return 1;
}

static int cmd_backtrace_cmd(rec_set_t *rec_set, int argc, char **argv) {
    int count = 0;

    if (argc >= 2) count = atoi(argv[1]);

    rep_set_t *rep_set = DbgShowFrame(rec_set, count);

    // process error
    ProcessErrorInRepSet(rep_set);

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
        
        for (const dbgframe_t* f = EVENT_FRAME(reply->rep_event); f != nullptr; f = f->getNext()) {
            // deepest (farthest from main) frame will be in the front
            frames.push_back(BTTree(reply->rep_di.c_str(), reply->rep_group.get(), f));
            auto loc = f->getLoc();

            if(Client.MI_mode){
                if (!ss.str().empty()){
                    ss<<",";
                }else{
                    ss<<"[";
                }
                ss << "frame={level=\"" << f->getLevel() << "\",addr=\"" << loc.loc_addr
                << "\",func=\"" << loc.loc_func << "\",file=\"" << loc.loc_file
                << "\",fullname=\""<<loc.loc_file<<"\",line=\""<<loc.loc_line<<"\"}";
            }
        }

        // link frames up
        for (auto it = frames.begin(); it != frames.end(); ++it) {
            if (std::next(it) != frames.end()) {
                std::next(it)->transferChild(*it);
            }
        }

        root.insertBacktrace(frames.back()); // insert into root tree and merge common paths

        if (Client.MI_mode){
            if (ss.str().empty()){
                ss<<"[";
            }
            ss<<"]";
            PrintOutMIResult(MIResult::DONE, "proc_set=\"%s\",stack=%s\n(gdb)\n",reply->rep_di.c_str(),ss.str().c_str());
        }
    }

    root.condenseTree();
    root.calculateTreeGroups();
    root.orderTree();

    if(!Client.MI_mode){
        root.print();
    }

    FreeRepSet(rep_set);

    return 1;
}

static int cmd_frame_cmd(rec_set_t *rec_set, int argc, char **argv) {

    bool verbose = false;
    if (argc == 2 && std::string{argv[1]} == "-v") verbose = true;
    else if (argc > 1) {
        PrintOutError("unrecognized argument expected [-v]\n");
        return 1;
    }

    auto rep_set = DbgGetFrame(rec_set, verbose);

    // process error
    ProcessErrorInRepSet(rep_set);

    if (Client.MI_mode && !rep_set->isEmpty()) {
        auto reply = rep_set->getFirstReply();
        auto f = EVENT_FRAME(reply->rep_event);
        auto loc = f->getLoc();
        PrintOutMIResult(MIResult::DONE,
                        "proc_set=\"%s\",frame={level=\"%d\",addr=\"%s\","
                        "func=\"%s\",file=\"%s\",fullname=\"%s\",line=\"%d\"}\n",
                         reply->rep_di.c_str(), f->getLevel(), loc.loc_addr,
                         loc.loc_func, loc.loc_file, loc.loc_file, loc.loc_line);

    } else {
        // process frame - there should only be one
        for (auto&& reply : rep_set) {
            // Point at the frame data
            auto fp = EVENT_FRAME(reply->rep_event);
            PrintFrameSet(reply->rep_di.c_str(), reply->rep_group.get(), fp);
        }
    }

    FreeRepSet(rep_set);

    return 1;
}

void InitCmd_frame(void) {
    AddCommand("up", NULL, proc_required, 0, movecmd, cmd_up_help,
               "Move up one or more stack frames.");
    AddCommand("down", NULL, proc_required, 0, movecmd,
               cmd_down_help, "Move down one or more stack frames.");
    AddCommand("backtrace", "bt", proc_required, 0,
               cmd_backtrace_cmd, cmd_backtrace_help,
               "Print backtrace of all stack frames.");
    AddCommand("frame", NULL, proc_required, 0, cmd_frame_cmd,
               cmd_frame_help, "Print the currently selected stack frame.");
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
        // since there is an invisible master root node, every backtrack tree is
        // indented by 3 spaces by default. we want it to look like we're printing
        // multiple tress instead of one big tree.
        PrintOutMsg((indent + nodeMark).substr(3).c_str());
        printData(m_dirtyGroup ? m_dirtyGroup.get() : m_group);
        for (auto &lat : m_lateral) {
            if (is_last) {
                nodeMark = "   ";
            } else {
                nodeMark = "|  ";
            }

            PrintOutMsg((indent + nodeMark).substr(3).c_str());
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
