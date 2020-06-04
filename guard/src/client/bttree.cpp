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

    if (setname != nullptr
    && group != nullptr
    && frame != nullptr) {
        PrintOutMsg(base_indent.c_str());
        PrintFrameSet(setname, group, frame);
    }

    if (children.size() == 1) {
        child_indent = "";
    } else {
        child_indent = "  ";
    }

    for (const auto &child : children) {
        child.print(base_indent + child_indent);
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
    // todo: improve comparison
    return getFuncName() == other.getFuncName();
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
            
            PrintOutMsg("(%s) and (%s)\n",
                bitset_to_set(bt.group), bitset_to_set(group));

            // merge bitset at parent
            bitset_oreq(group, bt.group);

            // move down
            if (bt.children.size() != 0) {
                // paths will only have one child
                it->insertBacktrace(bt.children.front());
            }
        } else {
            PrintOutMsg("Didn't find anything to merge with. Adding %s under %s.\n", 
                bt.getFuncName().c_str(), getFuncName().c_str());
            
            // remove ranks from bitsets on old branch
            for (auto& child : children) {
                child.removeGroupFromTree(bt.group);
            }

            // merge bitset at parent
            bitset_oreq(group, bt.group);

            children.push_back(bt);
        }
    }
}