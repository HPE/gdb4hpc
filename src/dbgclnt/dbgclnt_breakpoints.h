/******************************************************************************\
 * Copyright 2022 Hewlett Packard Enterprise Development LP.
 ******************************************************************************/
#pragma once

#include <algorithm>
#include <string>
#include <vector>
#include <memory>
#include <list>

#include "bitset.h"
#include "dbgbp.h"
#include "dbgevent.h"
#include "defs.h"

class LocaleSet {
    /*
        Maps a set of ranks to a location. Watchpoint (and catchpoint)
    location information gets updated at the time of watchpoint hit, not at
    the time of watchpoint creation. Therefore each rank can have a
    different location listed for the location of a single logical
    watchpoint.
    */
private:
    struct Locale {
        std::unique_ptr<Bitset> group; // All ranks that hit this location
        std::string file; // locale file
        int         line; // locale line
        std::string func; // locale function
    };

    using LocaleList = std::vector<Locale>;
    using const_iterator = LocaleList::const_iterator;

public:
    void add(const Bitset *group, const char *loc_file, int loc_line, const char *loc_func)
    {
        // search the list
        if (loc_file != nullptr) {
            const std::string file{loc_file};
            for (auto&& i : m_loc) {
                if (i.file == loc_file && i.line == loc_line) {
                    // found it!
                    *i.group |= *group;
                    return;
                }
            }
        }
        else {
            const std::string func{loc_func};
            for (auto&& i : m_loc) {
                if (i.func == loc_func) {
                    // found it!
                    *i.group |= *group;
                    return;
                }
            }
        }
        // Not found! Add it.
        m_loc.push_back({
            group->clone(), 
            loc_file ? loc_file : "", 
            loc_line, 
            loc_func ? loc_func : ""
        });
    }

    void replace(const Bitset *group, const char* file, int line, const char* func) {
        for (auto &loc : m_loc) {
            if (loc.group) {
                *loc.group -= *group;
            }
        }
        // moves all empty locations to the end of the list, returns start of
        // chunk full of empty locations
        auto newEnd = std::remove_if(m_loc.begin(), m_loc.end(), [](const auto &l) {
            return l.group->isEmpty();
        });
        m_loc.erase(newEnd, m_loc.end());
        add(group, file, line, func);
    }

    void clear()
    {
        m_loc.clear();
    }

    const_iterator begin() const { return m_loc.begin(); }
    const_iterator end() const { return m_loc.end(); }
    const_iterator cbegin() { return m_loc.cbegin(); }
    const_iterator cend() { return m_loc.cend(); }

    LocaleSet() = default;
    ~LocaleSet()
    {
        clear();
    }

    // Delete copy/move constructors and assignment
    LocaleSet(LocaleSet const&) = delete;
    LocaleSet& operator=(LocaleSet const&) = delete;

private:
    LocaleList m_loc;
};

struct IDSet {
    // Used to map the logical breakpoint number to real gdb backend breakpoint numbers.
    
    IDSet(const Bitset * _group, int _id)
        : id{_id}
    {
        group = _group->clone();
    };

    std::unique_ptr<Bitset> group; // the ranks with this id
    const int id;  // unique bkpt id assigned by each gdb
};

class MappedBreakpoint {
    /*
      A "logical" breakpoint/watchpoint/catchpoint. Maps "logical" (gdb4hpc
    level) breakpoints that live on the gdb4hpc client to "real" or "physical"
    breakpoints that live on the actual ranks.

      A single "logical" breakpoint can correspond to an arbitrary amount of
    "real" breakpoints. For example, in a session with two ranks, one might
    focus on rank 0 and set a breakpoint (a), then focus on rank 1 and set a
    breakpoint (b), and finally focus on all ranks and set a breakpoint (c).
    From the ranks' perspectives, they each have two "real" breakpoints:
    - rank 0: breakpoint 1 (a), breakpoint 2 (c)
    - rank 1: breakpoint 1 (b), breakpoint 2 (c)
      However, from the user of gdb4hpc's perspective, three "logical"
    breakpoints have been set:
    - logical breakpoint 1 -> breakpoint 1 on rank 0 (a)
    - logical breakpoint 2 -> breakpoint 1 on rank 1 (b)
    - logical breakpoint 3 -> breakpoints 2 on ranks 0,1 (c)

      Logical IDs have the scope of individual dbginvoke objects, so two
    dbginvoke objects can each contain their own "breakpoint 1", for example.
    */

    // private types

    struct DispositionSet {
        // Used when checking the disposition of the breakpoint (i.e. enabled, disabled, etc)

        DispositionSet(const Bitset * _group, dbgbp_disp _disp)
            : disp{_disp}
        {
            group = _group->clone();
        }

        std::unique_ptr<Bitset> group;  // the ranks with this disposition
        const dbgbp_disp disp;  // disposition value
    };

public:

    // public interface

    MappedBreakpoint(int bp_id, const Bitset* group, const dbgbp_t &bp);
    
    int id() const;          // (logical) breakpoint number
    dbgbp_type type() const; // breakpoint type (i.e. breakpoint, watchpoint)
    bool isPending() const;  // true if this is a pending breakpoint

    // Breakpoint info
    const std::string& statement() const; // breakpoint statement
    const std::string& address() const;   // breakpoint address string
    const std::string& file() const;      // breakpoint file name
    const std::string& function() const;  // breakpoint function name
    int line() const;                     // breakpoint line number

    // Watchpoint (and catchpoint) info
    // Watchpoint expression
    const std::string& watchExpression() const;
    // Set of locations and ranks where this watchpoint has hit
    const LocaleSet& watchLocaleSet() const;
    // Add watchpoint location for ranks in group
    void addWatchLocale(const Bitset *group, const char *loc_file, int loc_line, const char *loc_func);
    // Remove all stored watchpoint location data
    void clearWatchLocaleSet();

    // Thread id for thread specific breakpoint. Returns "" if no ID is associated
    const std::string& threadId() const;

    // Ranks bitset where this breakpoint exists
    const Bitset* ranksSet() const;
    // Ranks bitset where this breakpoint is enabled. A subset of ranksSet()
    const Bitset* enabledSet() const;

    // List of structs that map ranks <-> real ids
    const std::list<IDSet>& idSetList() const;
    // List of structs that map ranks <-> dispositions
    const std::list<DispositionSet>& dispSetList() const;

    // Add real breakpoint returned from backend corresponding to group to this logical breakpoint 
    void addBreakpoint(const Bitset *group, const dbgbp_t &breakpoint);
    // Mark this logical breakpoint as enabled on ranks in group
    void markEnabled(const Bitset *group);
    // Mark this logical breakpoint as disabled on ranks in group
    void markDisabled(const Bitset *group);
    // Set the disposition for this breakpoint on ranks in group
    void markDisposition(const Bitset *group, dbgbp_disp disp);
    // Remove all data corresponding to ranks in group from the logical breakpoint.
    // This function can modify internal lists and should not be used while iterating
    // over one.
    void removeGroup(const Bitset *group);

private:
    // private data
    int m_logicalId;
    dbgbp_type m_type;
    bool m_pending;

    // breakpoint locale information
    std::string m_locStmt;
    std::string m_locAddr;
    std::string m_locFile;
    std::string m_locFunc;
    int m_locLine;

    // watchpoint locale information
    std::string m_watchExp;
    LocaleSet m_watchLoc;

    // thread id, if applicable
    std::string m_threadId;

    // All ranks that contain this breakpoint
    std::unique_ptr<Bitset> m_ranksSet;  

    // All ranks where this breakpoint is enabled (a subset of m_ranksSet)
    std::unique_ptr<Bitset> m_enabledSet;

    // List of real ids on backend ranks that this logical breakpoint
    // corresponds to, and on which ranks those real ids live
    std::list<IDSet> m_idList;

    // List of dispositions (i.e. keep, dis, del) on backend ranks that this
    // logical breakpoint corresponds to, and on which ranks those dispositions
    // apply
    std::list<DispositionSet> m_dispList;
};

class BreakpointManager {
    /*
    Manages logical breakpoints as real breakpoints come in one at a time inside
    of backend replies.
    */
public:
    // returns true if there are no breakpoints in the internal list
    bool isEmpty() const;
    // record passed real breakpoint, corresponding to ranks in group, inside of
    // internal logical breakpoint list
    void recordBreakpoint(const dbgbp_t &breakpoint, const Bitset *group);
    // Remove breakpoints with an empty ranksSet() from the internal list
    void removeEmptyBreakpoints();
    // Searches the internal breakpoint list for any pending logical breakpoint
    // that matches both the passed dbgbp and bitset. If found, the part of the
    // breakpoint corresponding to the passed dbgbp is removed and
    // recordBreakpoint is called with the passed breakpoint data. Looking at it
    // another way, this function changes subsets of pending logical breakpoints
    // to new, non-pending logical breakpoints.
    void fixupPendingBreakpoints(const dbgbp_t &bp, const Bitset *group);
    // Returns a pointer to the logical breakpoint in the internal list with a
    // given id. Returns nullptr if none exist.
    MappedBreakpoint* findByLogicalID(int id);
    // Returns a pointer to the logical breakpoint in the internal list that
    // matches the passed dbgbp_t. Returns nullptr if none exist.
    MappedBreakpoint* findByDbgBreakpoint(const dbgbp_t &bp);
    const MappedBreakpoint* findByDbgBreakpoint(const dbgbp_t &bp) const;

    int getNextID() const { return m_nextID; }

    // allow range based iteration
    auto begin() { return m_breakpoints.begin(); }
    auto end() { return m_breakpoints.end(); }
    auto cbegin() const { return m_breakpoints.cbegin(); }
    auto cend() const { return m_breakpoints.cend(); }

private:
    int m_nextID = 1;
    std::list<MappedBreakpoint> m_breakpoints;
};

class MappedCheckpoint {
    /*
      A logical checkpoint corresponding to a checkpoint on an arbitrary amount
    of ranks. Each dbginvoke has a list of checkpoints. Checkpoints are given a
    client side id with the guarantee (enforced outside of this class) that all
    logical checkpoints with the same id, across any amount of dis, were created
    at the same time.

      A logical checkpoint can correspond to any subset of ranks on a di, and
    once constructed, that subset never changes. Because of this, a checkpoint
    must be constructed with a list of all ranks and locations at once.

      A logical checkpoint stores a set of locations corresponding to where that
    checkpoint was last known to be on each of the ranks it exists on. This
    information is only (externally) updated when the current checkpoint changes
    (the restart command is used). Locations returned by locations() are
    therefore only snapshots of a certain point in time and are not guaranteed
    to match the current frames.
    */
public:
    MappedCheckpoint(unsigned int id, const std::vector<std::pair<const Bitset*, const dbgcheckpoint_t*>> &checkpointData, std::optional<unsigned long> dbgcpId);
    MappedCheckpoint(unsigned int id, const std::vector<std::pair<const Bitset*, const dbgcheckpoint_t*>> &checkpointData);

    // logical id
    unsigned int id() const;

    // where this checkpoint is on each rank
    LocaleSet& locations();
    const LocaleSet& locations() const;

    // ranks where this checkpoint exists
    const Bitset& ranksSet() const;
    // ranks where this checkpoint is current. Always a subset of ranksSet()
    const std::list<IDSet>& idSetList() const;

    // set of ranks where this checkpoint is current
    const Bitset& currentSet() const;
    // mark this checkpoint as the current checkpoint on ranks in group
    void markCurrent(const Bitset &group);
    // unmark this checkpoint as the current checkpoint on ranks in group
    void unmarkCurrent(const Bitset &group);

    // disassociate group with this checkpoint. This function can modify internal
    // lists and should not be used while iterating over one.
    void removeGroup(const Bitset &group);

private:
    // client side id
    const unsigned int m_id; 
    // ranks where this checkpoint exists
    std::unique_ptr<Bitset> m_allRanks;
    // ranks where this checkpoint is current. 
    // should always be a subset of m_allRanks.
    std::unique_ptr<Bitset> m_current;  
    // list mapping real backend checkpoint ids to which ranks they are on
    std::list<IDSet> m_idsMap;
    // maps checkpoint locations to which ranks they are on
    LocaleSet m_locations;
};

class CheckpointManager {
    /*
      Manages a list of checkpoints and helps to keep relationships between
    client side checkpoint data correct.
    */
public:
    CheckpointManager();

    // record a new checkpoint with given id that maps to checkpoints on groups 
    // passed in checkpointData
    unsigned int recordCheckpoint(unsigned int id, const std::vector<std::pair<const Bitset*, const dbgcheckpoint_t*>> &checkpointData);
    
    // return the most recently added checkpoint. Returns nullptr if there are
    // no checkpoints.
    const MappedCheckpoint* newest() const;
    // return the checkpoint with the given id. Returns nullptr if there is no match.
    MappedCheckpoint* find(unsigned int id);

    // remove empty checkpoints from internal checkpoint list
    void removeEmpty();
    // remove any checkpoints that are the only ones remaning on their procset
    // from internal checkpoint list.
    // for example, given the following checkpoints:
    // - checkpoint 1 on $a{0..1}
    // - checkpoint 2 on $a{2..3}
    // - checkpoint 3 on $a{2..5}
    // checkpoint 1 would be removed.
    void removeSingular();
    // return size of interal breakpoint list- the number of known checkpoints
    size_t size() const;
    // return internal error string, updated any time an internal error happens
    const std::string& getError() const;

    // allow range based iteration
    auto begin() { return m_checkpoints.begin(); }
    auto end() { return m_checkpoints.end(); }
    auto cbegin() const { return m_checkpoints.cbegin(); }
    auto cend() const { return m_checkpoints.cend(); }

private:
    std::list<MappedCheckpoint> m_checkpoints; // internal checkpoint list
    std::string m_error; // updated upon internal error
};
