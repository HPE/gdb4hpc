#ifndef _BTTREE_H
#define _BTTREE_H

#include <string>
#include <vector>

#include "bitset"
#include "client.h"

struct BTTree {
    BTTree();
    BTTree(char* setname, bitset* group, dbgframe_t* frame);

    char* setname;
    bitset* group;
    dbgframe_t* frame;
    std::vector<BTTree> children;

    void print(std::string indent = "") const;
    void removeGroupFromTree(bitset* group);
    void insertBacktrace(BTTree &bt);
    std::string getFuncName() const;
    bool compareData(const BTTree &other) const;
    void freeTreeGroups();
};

#endif /* _BTTREE_H */
