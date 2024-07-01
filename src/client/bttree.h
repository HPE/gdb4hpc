// Copyright 2023-2024 Hewlett Packard Enterprise Development LP.
//
// Backtrace Tree
//
// used in backtrace commands to print a pstree-like representation of the results.
//
// copies setname, debugframe_t, and group pointers and does not modify them.

#include <string>
#include <vector>

#include "gdb4_bitset.h"
#include "dbgevent.h"
#include "dbg_client.h"

class BTTree {
public:
    BTTree() = delete;
    BTTree(const BTTree &other) = delete;
    BTTree(BTTree &&other);

    // make a node
    BTTree(const char* setname, const Bitset* group, const dbgframe_t* frame);

    // construct an entire tree from a rep_set.
    // the rep_set must contain DBGEV_FRAME data.
    //
    // the passed `rep_set` is not modified - it's not marked const because
    // rep_set.begin() is not const. (see comments around rep_set.begin() for why)
    static BTTree fromFrames(rep_set_t& rep_set);

    void print() const;
    void insertBacktrace(BTTree &&bt);
    void transferChild(BTTree &&bt);
    void condenseTree();
    void orderTree();
    void calculateTreeGroups();

    BTTree& operator=(const BTTree &other) = delete;
    BTTree& operator=(BTTree &&other);
    bool shouldMerge(const BTTree &other) const;
private:
    void printData(const Bitset* group) const;
    size_t lowestRank() const;
    const Bitset& group() const;
    void mergeGroup(const Bitset& group);
    void innerPrint(std::string indent, bool is_last) const;

    std::string m_setname;
    std::unique_ptr<Bitset> m_dirtyGroup; // newly allocated group that is sometimes needed. owned by the tree.
    const Bitset* m_group;                // original group: not owned by the tree
    const dbgframe_t* m_frame;
    std::vector<BTTree> m_lateral;
    std::vector<BTTree> m_children;
};
