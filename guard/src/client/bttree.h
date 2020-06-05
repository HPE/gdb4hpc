#ifndef _BTTREE_H
#define _BTTREE_H

#include <string>
#include <vector>

#include "bitset"
#include "client.h"

// backtrace tree
//
// used in cmd_backtrace_cmd to print a pstree-like representation of the
// results of a backtrack command.
//
// copies setname and debugframe_t pointers and does not modify them, but 
// duplicates the bitset passed in the constructor which must be freed when done. 
class BTTree {
public:
    BTTree();
    BTTree(char* setname, bitset* group, dbgframe_t* frame);

    void print(std::string indent = "", bool is_last = true) const;
    void removeGroupFromTree(bitset* group);
    void insertBacktrace(BTTree &bt);
    void insertChild(const BTTree &bt);
    void allocateGroup(int size);
    void freeGroup();
    bool compareData(const BTTree &other) const;
    void freeTreeGroups();
    void condenseTree();
private:
    void printData(char* setname, bitset* group, dbgframe_t* frame) const;

    char* setname;
    bitset* group;
    dbgframe_t* frame;
    std::vector<BTTree> lateral;
    std::vector<BTTree> children;
};

#endif /* _BTTREE_H */
