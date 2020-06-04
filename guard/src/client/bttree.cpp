#include "bttree.h"
#include "client.h"

#include <algorithm>

BTTree::BTTree() 
    : setname{nullptr}, group{nullptr}, frame{nullptr}
{}

BTTree::BTTree(char* _setname, bitset* _group, dbgframe_t* _frame) 
    : setname{_setname}, group{bitset_dup(_group)}, frame{_frame}
{}

void BTTree::print(std::string base_indent) const {
    std::string child_indent;
    bool has_info = (setname != nullptr && group != nullptr && frame != nullptr);

    if (has_info) {
        PrintOutMsg(base_indent.c_str());
        PrintFrameSet(setname, group, frame);
    }

    if (children.size() == 1 || !has_info) {
        child_indent = "";
    } else {
        child_indent = "  ";
    }

    for (const auto &child : children) {
        child.print(base_indent + child_indent);

        if (!has_info) {
            // seperate root node's children
            PrintOutMsg("\n");
        }
    }
}

std::string BTTree::getFuncName() const {
    if (frame != nullptr) {
        return frame->frame_loc.loc_func;
    } else {
        return "";
    }
}

void BTTree::removeGroupFromTree(bitset* remgroup) {
    bitset_andeqnot(group, remgroup);
    for (auto &child : children) {
        child.removeGroupFromTree(remgroup);
    }
}

bool BTTree::compareData(const BTTree &other) const {
    return std::string(setname) == std::string(other.setname)
        && std::string(frame->frame_loc.loc_addr) == std::string(other.frame->frame_loc.loc_addr);
}

void BTTree::freeTreeGroups() {
    for (auto &child : children) {
        child.freeTreeGroups();
    }

    if (group != nullptr) {
        bitset_free(group);
        group = nullptr;
    }
}

void BTTree::insertBacktrace(BTTree &bt) {
    if (group != nullptr && bt.group != nullptr) {
        PrintOutMsg("(root: %s) (bt: %s)\n",
            bitset_to_set(group), bitset_to_set(bt.group));
    } else {
        PrintOutMsg("A group is null.\n");
    }

    if (children.size() == 0) {
        PrintOutMsg("No children... adding %s to %s.\n", 
            bt.getFuncName().c_str(), getFuncName().c_str());

        // merge bitset at parent
        bitset_oreq(group, bt.group);

        children.push_back(bt);
    } else {
        auto it = std::find_if(children.begin(), children.end(), 
            [&](const BTTree &child) {
                return child.compareData(bt);
            });

        if (it != children.end()) {
            PrintOutMsg("Merging %s under %s.\n", 
                bt.getFuncName().c_str(), getFuncName().c_str());

            // merge bitset at parent
            bitset_oreq(group, bt.group);

            // move down
            if (bt.children.size() != 0) {
                // backtraces will only have one child
                it->insertBacktrace(bt.children.front());
            }
        } else {
            PrintOutMsg("Didn't find anything to merge with. Adding %s under %s.\n", 
                bt.getFuncName().c_str(), getFuncName().c_str());
            
            // cheeky test for if we're in the master root node
            // if we are, we don't want to mess with the group bitsets of other
            // programs.
            if (setname != nullptr) {
                // remove ranks from bitsets on old branches
                for (auto& child : children) {
                    child.removeGroupFromTree(bt.group);
                }
            }

            // merge bitset at parent
            bitset_oreq(group, bt.group);

            children.push_back(bt);
        }
    }
}