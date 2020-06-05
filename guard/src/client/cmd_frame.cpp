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
    BTTree(char* setname, bitset* group, dbgframe_t* frame);

    void print(std::string indent = "", bool is_last = true) const;
    void insertBacktrace(BTTree &bt);
    void insertChild(const BTTree &bt);
    void removeGroupFromTree(bitset* group);
    void allocateGroup(int size);
    void freeGroup();
    void freeTreeGroups();
    bool compareData(const BTTree &other) const;
    void condenseTree();
private:
    void printData(char* setname, bitset* group, dbgframe_t* frame) const;

    char* _setname;
    bitset* _group;
    dbgframe_t* _frame;
    std::vector<BTTree> _lateral;
    std::vector<BTTree> _children;
};

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
        BTTree path;
        BTTree child;
        bool available_child = false;
        
        for (f = EVENT_FRAME(reply->rep_event); f != (dbgframe_t *)NULL; f = f->frame_next) {
            path = BTTree(reply->rep_di->d_handle, reply->rep_group, f);

            if (available_child == true) {
                path.insertChild(child);
            }

            child = path;
            available_child = true;
        }

        tracePool.push_back(path); // save for freeing group bitsets later
        root.insertBacktrace(path); // insert into root tree and merge common paths
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
    : _setname{nullptr}, _group{nullptr}, _frame{nullptr}
{}

BTTree::BTTree(char* setname, bitset* group, dbgframe_t* frame) 
    : _setname{setname}, _group{bitset_dup(group)}, _frame{frame}
{}

void BTTree::print(std::string indent, bool is_last) const {
    bool has_info = (_setname != nullptr && _group != nullptr && _frame != nullptr);

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
        printData(_setname, _group, _frame);
        for (auto &lat : _lateral) {
            if (is_last) {
                nodeMark = "   ";
            } else {
                nodeMark = "|  ";
            }

            PrintOutMsg((indent + nodeMark).substr(3).c_str());
            printData(lat._setname, lat._group, lat._frame);
        }

        if (is_last) {
            indent += "   ";
        } else {
            indent += "|  ";
        }

        if (_children.size() == 0) {
            // extra spaces to make sure substr(3) never crashes
            PrintOutMsg("%s\n", (indent + "   ").substr(3).c_str());
        } else {
            PrintOutMsg("%s\n", (indent + "|  ").substr(3).c_str());
        }
    }

    for (unsigned int i = 0; i < _children.size(); i++) {
        _children[i].print(indent, i == _children.size() - 1);
    }
}

void BTTree::removeGroupFromTree(bitset* remgroup) {
    bitset_andeqnot(_group, remgroup);
    for (auto &child : _children) {
        child.removeGroupFromTree(remgroup);
    }
}

bool BTTree::compareData(const BTTree &other) const {
    return std::string(_setname) == std::string(other._setname)
        && std::string(_frame->frame_loc.loc_addr) == std::string(other._frame->frame_loc.loc_addr);
}

void BTTree::freeTreeGroups() {
    for (auto &child : _children) {
        child.freeTreeGroups();
    }

    if (_group != nullptr) {
        freeGroup();
    }
}

void BTTree::insertBacktrace(BTTree &bt) {
    if (_children.size() == 0) {
        // no children; nothing to merge with; just add here.
        bitset_oreq(_group, bt._group);
        _children.push_back(bt);
    } else {
        // look for a matching child node
        auto it = std::find_if(_children.begin(), _children.end(), 
            [&](const BTTree &child) {
                return child.compareData(bt);
            });

        if (it != _children.end()) {
            // match found; merge it and move down
            bitset_oreq(_group, bt._group);
            if (bt._children.size() != 0) {
                // backtraces will only have one child
                it->insertBacktrace(bt._children.front());
            }
        } else {
            // match not found; add a new branch

            // test for if we're in the master root node
            // if we are, we don't want to mess with the _group bitsets of other
            // programs.
            if (_setname != nullptr) {
                // remove ranks from bitsets on old branches
                for (auto& child : _children) {
                    child.removeGroupFromTree(bt._group);
                }
            }

            bitset_oreq(_group, bt._group);
            _children.push_back(bt);
        }
    }
}

void BTTree::insertChild(const BTTree &t) {
    _children.push_back(t);
}

void BTTree::allocateGroup(int size) {
    _group = bitset_new(size);
}

void BTTree::freeGroup() {
    if (_group != nullptr) {
        bitset_free(_group);
        _group = nullptr;
    }
}

void BTTree::printData(char* sn, bitset* gr, dbgframe_t* f) const {
    PrintMsgWithSetInfo(sn, gr, "#%-2d ", f->frame_level);

    PrintOutMsg("%s", f->frame_loc.loc_func);

    if (*f->frame_loc.loc_file != '\0')
        PrintOutMsg(" at %s:%d", f->frame_loc.loc_file, f->frame_loc.loc_line);

    PrintOutMsg("\n");
}

void BTTree::condenseTree() {
    for (auto &child : _children) {
        child.condenseTree();
    }

    // don't do this if we are master root; master root only servers as a holder
    // for unique backtraces as children
    if (_children.size() == 1 && _setname != nullptr) {
        BTTree& child = _children.front();
        _lateral.push_back(child);
        _lateral.back()._lateral.clear();

        for (auto &nested : child._lateral) {
            _lateral.push_back(nested);
        }

        child._lateral.clear();
        _children = child._children;
    }
}
