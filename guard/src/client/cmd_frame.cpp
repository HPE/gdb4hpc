/*********************************************************************************\
 * Stack frame commands.
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * Copyright 2010-2020 Cray Inc.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Cray Inc.,
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
#include <vector>
#include <algorithm>

#include "client.h"
#include "useful.h"

namespace {
/*
backtrace tree

used in cmd_backtrace_cmd (cmd_frame.cpp) to print a pstree-like 
representation of the results of a backtrack command.

copies setname and debugframe_t pointers and does not modify them, but 
duplicates the bitset passed in the constructor which must be freed manually. 
*/
class BTTree {
public:
    BTTree();
    BTTree(const char* setname, const bitset* group, const dbgframe_t* frame);

    void print(std::string indent = "", bool is_last = true) const;
    void insertBacktrace(BTTree &bt);
    void insertChild(const BTTree &bt);
    void removeGroupFromTree(const bitset* group);
    void allocateGroup(int size);
    void freeGroup();
    void freeTreeGroups();
    void condenseTree();

    bool operator==(const BTTree &other) const;
private:
    void printData(const std::string setname, const bitset* group, const dbgframe_t* frame) const;

    const std::string m_setname;
    bitset* m_group;
    const dbgframe_t* m_frame;
    std::vector<BTTree> m_lateral;
    std::vector<BTTree> m_children;
};
}

void PrintFrameSet(char *setname, bitset *group, dbgframe_t *f) {
    PrintMsgWithSetInfo(setname, group, "#%-2d ", f->frame_level);

    if (*f->frame_loc.loc_addr != '\0')
        PrintOutMsg("%s in ", f->frame_loc.loc_addr);

    PrintOutMsg("%s", f->frame_loc.loc_func);

    if (*f->frame_loc.loc_file != '\0')
        PrintOutMsg(" at %s:%d", f->frame_loc.loc_file, f->frame_loc.loc_line);

    PrintOutMsg("\n");
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

        PrintFrameSet(reply->rep_di->d_handle, reply->rep_group, f);
    }

    FreeRepSet(rep_set);

    return 1;
}

static int cmd_backtrace_cmd(rec_set_t *rec_set, int argc, char **argv) {
    int count = 0;
    dbgframe_t *f;

    if (argc >= 2) count = atoi(argv[1]);

    rep_set_t *rep_set = DbgShowFrame(rec_set, count);

    // process error
    ProcessErrorInRepSet(rep_set);

    // turn backtraces into tree format ////////////////////////////////////////

    BTTree root;
    // root is a blank root node which serves to hold multiple trees.
    // it needs a dummy bitset for the recursive group bitset merging to work.
    root.allocateGroup(1);

    std::vector<BTTree> tracePool; // to hold information for cleaning up memory later

    for (auto&& reply : rep_set) {
        // reply sets hold stack frames in "reverse" (deepest first) order, while
        // we want to print them in most shallow (i.e. main) first order.
        std::vector<BTTree> frames;
        
        for (f = EVENT_FRAME(reply->rep_event); f != (dbgframe_t *)NULL; f = f->frame_next) {
            BTTree frame = BTTree(reply->rep_di->d_handle, reply->rep_group, f);

            frames.push_back(frame); // deepest frame will be in the front
        }

        // link frames up
        for (auto it = frames.begin(); it != frames.end(); ++it) {
            if (std::next(it) != frames.end()) {
                std::next(it)->insertChild(*it);
            }
        }

        tracePool.push_back(frames.back()); // save for freeing group bitsets later
        root.insertBacktrace(frames.back()); // insert into root tree and merge common paths
    }

    root.condenseTree();
    root.print();

    // clean up - each tree node had its own group bitset allocated.
    // the original pointers are still stored in tracePool.
    for (auto &path : tracePool) {
        path.freeTreeGroups();
    }
    root.freeGroup();
    FreeRepSet(rep_set);

    return 1;
}

static int cmd_frame_cmd(rec_set_t *rec_set, int argc, char **argv) {
    rep_set_t *rep_set;
    dbgframe_t *fp;

    rep_set = DbgGetFrame(rec_set);

    // process error
    ProcessErrorInRepSet(rep_set);

    // process frame - there should only be one
    for (auto&& reply : rep_set) {
        // Point at the frame data
        fp = EVENT_FRAME(reply->rep_event);

        PrintFrameSet(reply->rep_di->d_handle, reply->rep_group, fp);
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

BTTree::BTTree() 
    : m_setname{""}, m_group{nullptr}, m_frame{nullptr}
{}

BTTree::BTTree(const char* setname, const bitset* group, const dbgframe_t* frame) 
    : m_setname(setname), m_group{bitset_dup(group)}, m_frame{frame}
{}

void BTTree::print(std::string indent, bool is_last) const {
    bool has_info = (m_group != nullptr && m_frame != nullptr);

    std::string nodeMark = "|_ ";

    if (has_info) { // skip the root node
        if (is_last) {
            nodeMark = "\\_ ";
        }

        // substr(3) is a hack to shave off the first 3 characters of the tree.
        // since there is an invisible master root node, every backtrack tree is
        // indended by 3 spaces by default. we want it to look like we're printing
        // multiple tress instead of one big tree.
        PrintOutMsg((indent + nodeMark).substr(3).c_str());
        printData(m_setname, m_group, m_frame);
        for (auto &lat : m_lateral) {
            if (is_last) {
                nodeMark = "   ";
            } else {
                nodeMark = "|  ";
            }

            PrintOutMsg((indent + nodeMark).substr(3).c_str());
            printData(lat.m_setname, lat.m_group, lat.m_frame);
        }

        if (is_last) {
            indent += "   ";
        } else {
            indent += "|  ";
        }

        if (m_children.size() == 0) {
            // extra spaces to make sure substr(3) never throws and exception
            PrintOutMsg("%s\n", (indent + "   ").substr(3).c_str());
        } else {
            PrintOutMsg("%s\n", (indent + "|  ").substr(3).c_str());
        }
    }

    for (size_t i = 0; i < m_children.size(); ++i) {
        m_children[i].print(indent, i == m_children.size() - 1);
    }
}

void BTTree::removeGroupFromTree(const bitset* remgroup) {
    bitset_andeqnot(m_group, remgroup);
    for (auto &child : m_children) {
        child.removeGroupFromTree(remgroup);
    }
}

void BTTree::freeTreeGroups() {
    for (auto &child : m_children) {
        child.freeTreeGroups();
    }

    if (m_group != nullptr) {
        freeGroup();
    }
}

void BTTree::insertBacktrace(BTTree &bt) {
    if (m_children.size() == 0) {
        // no children; nothing to merge with; just add here.
        bitset_oreq(m_group, bt.m_group);
        m_children.push_back(bt);
    } else {
        // look for a matching child node
        auto it = std::find_if(m_children.begin(), m_children.end(), 
            [&](const BTTree &child) {
                return child == bt;
            });

        if (it != m_children.end()) {
            // match found; merge it and move down
            bitset_oreq(m_group, bt.m_group);
            if (bt.m_children.size() != 0) {
                // backtraces will only have one child
                it->insertBacktrace(bt.m_children.front());
            }
        } else {
            // match not found; add a new branch

            // test for if we're in the master root node
            // if we are, we don't want to mess with the m_group bitsets of other
            // programs.
            if (m_frame != nullptr) {
                // remove ranks from bitsets on old branches
                for (auto& child : m_children) {
                    child.removeGroupFromTree(bt.m_group);
                }
            }

            bitset_oreq(m_group, bt.m_group);
            m_children.push_back(bt);
        }
    }
}

void BTTree::insertChild(const BTTree &t) {
    m_children.push_back(t);
}

void BTTree::allocateGroup(int size) {
    m_group = bitset_new(size);
}

void BTTree::freeGroup() {
    if (m_group != nullptr) {
        bitset_free(m_group);
        m_group = nullptr;
    }
}

void BTTree::printData(const std::string sn, const bitset* gr, const dbgframe_t* f) const {
    PrintMsgWithSetInfo(sn.c_str(), gr, "#%-2d ", f->frame_level);

    PrintOutMsg("%s", f->frame_loc.loc_func);

    if (*f->frame_loc.loc_file != '\0')
        PrintOutMsg(" at %s:%d", f->frame_loc.loc_file, f->frame_loc.loc_line);

    PrintOutMsg("\n");
}

void BTTree::condenseTree() {
    for (auto &child : m_children) {
        child.condenseTree();
    }

    // don't do this if we are master root; master root only servers as a holder
    // for unique backtraces as children
    if (m_children.size() == 1 && m_frame != nullptr) {
        BTTree& child = m_children.front();
        m_lateral.push_back(child);
        m_lateral.back().m_lateral.clear();

        for (auto &nested : child.m_lateral) {
            m_lateral.push_back(nested);
        }

        child.m_lateral.clear();
        m_children = std::vector<BTTree>(child.m_children);
    }
}

bool BTTree::operator==(const BTTree &other) const {
    return m_setname == other.m_setname
        && std::string(m_frame->frame_loc.loc_addr) == std::string(other.m_frame->frame_loc.loc_addr);
}
