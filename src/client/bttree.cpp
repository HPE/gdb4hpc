// Copyright 2023-2024 Hewlett Packard Enterprise Development LP.

#include "bttree.h"
#include "dense_bitset.h"
#include "procset.h"

BTTree::BTTree(const char* setname, const Bitset* group, const dbgframe_t* frame) 
    : m_setname(setname), m_dirtyGroup{nullptr}, m_group{group}, m_frame{frame}
{}

BTTree BTTree::fromFrames(rep_set_t& rep_set) {
    auto dummySet = std::make_unique<DenseBitset>(1);
    BTTree root("", dummySet.get(), nullptr);
    // root is a blank root node which serves to hold multiple trees.
    // it needs a dummy bitset for the recursive group bitset merging to work.

    for (const auto& reply : rep_set) {
        if (!reply->rep_event || reply->rep_event->ev_event != DBGEV_FRAME) {
            // FIXME: silent failure is pretty bad...
            continue;
        }

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
                std::next(it)->transferChild(std::move(*it));
            }
        }

        root.insertBacktrace(std::move(frames.back())); // insert into root tree and merge common paths
    }

    root.condenseTree();
    root.calculateTreeGroups();
    root.orderTree();

    return root;
}

BTTree::BTTree(BTTree &&other)
    : m_setname{std::move(other.m_setname)}, m_dirtyGroup{std::move(other.m_dirtyGroup)}
    , m_group{std::move(other.m_group)}, m_frame{std::move(other.m_frame)}
    , m_lateral{std::move(other.m_lateral)}, m_children{std::move(other.m_children)}
{
    other.m_group = nullptr;
}

void BTTree::print() const {
    innerPrint("", true);
}

void BTTree::innerPrint(std::string indent, bool is_last) const {
    bool has_info = (m_group != nullptr && m_frame != nullptr);

    std::string nodeMark = "|_ ";

    if (has_info) { // skip the root node
        if (is_last) {
            nodeMark = "\\_ ";
        }

        // substr(3) is a hack to shave off the first 3 characters of the tree.
        // since there is an invisible master root node, every backtrace tree is
        // indented by 3 spaces by default. we want it to look like we're printing
        // multiple trees instead of one big tree.
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
        m_children[i].innerPrint(indent, i == m_children.size() - 1);
    }
}

void BTTree::insertBacktrace(BTTree &&bt) {
    if (m_children.size() == 0) {
        // no children; nothing to merge with; just add here.
        m_children.push_back(std::move(bt));
    } else {
        // look for a matching child node
        auto it = std::find_if(m_children.begin(), m_children.end(), 
            [&](const BTTree &child) {
                return child.shouldMerge(bt);
            });

        if (it != m_children.end()) {
            // match found; merge it and move down
            it->mergeGroup(bt.group());
            if (bt.m_children.size() != 0) {
                // backtraces will only have one child
                it->insertBacktrace(std::move(bt.m_children.front()));
            }
        } else {
            // match not found; add a new branch
            m_children.push_back(std::move(bt));
        }
    }
}

void BTTree::transferChild(BTTree &&t) {
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

const Bitset& BTTree::group() const {
    if (m_dirtyGroup) return *m_dirtyGroup;
    return *m_group;
}

void BTTree::mergeGroup(const Bitset& group) {
    if (!m_dirtyGroup) {
        m_dirtyGroup = m_group->clone();
    }
    *m_dirtyGroup |= group;
}

void BTTree::calculateTreeGroups() {
    for (auto &child : m_children) {
        child.calculateTreeGroups();
    }

    // leaf nodes will have pointers to the groups they were given at creation time.

    if (m_children.size() != 0 && !m_dirtyGroup) {
        for (const auto &child : m_children) {
            mergeGroup(child.group());
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

bool BTTree::shouldMerge(const BTTree &other) const {
    // this function checks if two nodes should be merged in the backtrace view. it is
    // about merging that is beneficial to the user in a backtrace view instead of strict
    // bit-by-bit merging.
    //
    // a node should merge with another node if it refers to the same di and, as best as we can tell,
    // its location is the same. if two nodes are in the same function, but on
    // different lines, they are different. we differ on line numbers so we can produce
    // outputs like this:
    //
    // one{0..9}: #3 main at cpp_bt.cpp:35
    // |
    // |_ one{0..4}: #2 branch at cpp_bt.cpp:47
    // |  one{0..4}: #1 left at cpp_bt.cpp:77
    // |  ...
    // |
    // \_ one{5..9}: #2 branch at cpp_bt.cpp:49
    //    one{5..9}: #1 right at cpp_bt.cpp:81
    //    ...
    //
    // all frame #2 paths are in branch, but ranks are in different locations within branch.
    // this is shown as different line numbers in frame 2. it's informative to the user
    // to make this distinction.
    //
    // what about frames without debug info/line numbers? in the past, we split on the address,
    // but that used to lead to unhelpful output, especially when dealing with dlopened functions:
    //
    // a{0..2}: #1 main
    // |
    // |_ a{0}: #0 dlopened_function
    // |
    // |_ a{1}: #0 dlopened_function
    // |
    // \_ a{2}: #0 dlopened_function
    //
    // see PE-37929.
    //
    // so now we merge on function names in this case. we only fall back to addresses
    // when absolutely no other information is available, which is rare. merging on function
    // names alone can lead to false positives in the edge case that we have multiple static
    // functions with the same name in different files, all without file/line info.
    // dlopen'd functions are more common than this edge case, so we accept the potential
    // false positive match.

    // check di name
    if (m_setname != other.m_setname) return false;

    const auto& a = m_frame->getLoc();
    const auto& b = other.m_frame->getLoc();

    // try the function name first
    if (a.loc_func && b.loc_func) {
        // see if we have the complete set of data (fn + file + line)
        if (a.loc_file && a.loc_line && b.loc_file && b.loc_line) {
            return (a.loc_line == b.loc_line)
              && (::strcmp(a.loc_file, b.loc_file) == 0);
        }

        // we have incomplete data, just check the function name
        return ::strcmp(a.loc_func, b.loc_func) == 0;
    }

    // missing function names, try addresses
    if (a.loc_addr && b.loc_addr) {
        return ::strcmp(a.loc_addr, b.loc_addr) == 0;
    } else {
        // missing addresses too. with absolutely nothing to go on, just return false
        // so they show as separate branches in the backtrace tree
        return false;
    }
}
