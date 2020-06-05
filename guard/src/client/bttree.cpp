#include "bttree.h"
#include "client.h"

#include <algorithm>
#include <sstream>

BTTree::BTTree() 
    : setname{nullptr}, group{nullptr}, frame{nullptr}
{}

BTTree::BTTree(char* _setname, bitset* _group, dbgframe_t* _frame) 
    : setname{_setname}, group{bitset_dup(_group)}, frame{_frame}
{}

void BTTree::print(std::string indent, bool is_last) const {
    bool has_info = (setname != nullptr && group != nullptr && frame != nullptr);

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
        printData(setname, group, frame);
        for (auto &lat : lateral) {
            if (is_last) {
                nodeMark = "   ";
            } else {
                nodeMark = "|  ";
            }

            PrintOutMsg((indent + nodeMark).substr(3).c_str());
            printData(lat.setname, lat.group, lat.frame);
        }

        if (is_last) {
            indent += "   ";
        } else {
            indent += "|  ";
        }
        
        if (children.size() == 0) {
            // extra spaces to make sure substr(3) never crashes
            PrintOutMsg("%s\n", (indent + "   ").substr(3).c_str());
        } else {
            PrintOutMsg("%s\n", (indent + "|  ").substr(3).c_str());
        }
    }

    for (unsigned int i = 0; i < children.size(); i++) {
        children[i].print(indent, i == children.size() - 1);
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
        freeGroup();
    }
}

void BTTree::insertBacktrace(BTTree &bt) {
    if (children.size() == 0) {
        // merge bitset at parent
        bitset_oreq(group, bt.group);

        children.push_back(bt);
    } else {
        auto it = std::find_if(children.begin(), children.end(), 
            [&](const BTTree &child) {
                return child.compareData(bt);
            });

        if (it != children.end()) {
            // merge bitset at parent
            bitset_oreq(group, bt.group);

            // move down
            if (bt.children.size() != 0) {
                // backtraces will only have one child
                it->insertBacktrace(bt.children.front());
            }
        } else {
            // test for if we're in the master root node
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

void BTTree::insertChild(const BTTree &t) {
    children.push_back(t);
}

void BTTree::allocateGroup(int size) {
    group = bitset_new(size);
}

void BTTree::freeGroup() {
    if (group != nullptr) {
        bitset_free(group);
        group = nullptr;
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
    for (auto &child : children) {
        child.condenseTree();
    }

    // don't do this if we are master root
    if (children.size() == 1 && setname != nullptr) {
        BTTree& child = children.front();
        lateral.push_back(child);
        lateral.back().lateral.clear();

        for (auto &nested : child.lateral) {
            lateral.push_back(nested);
        }

        child.lateral.clear();
        children = child.children;
    }
}
