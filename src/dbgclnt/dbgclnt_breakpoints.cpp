/******************************************************************************\
 * Copyright 2022 Hewlett Packard Enterprise Development LP.
 ******************************************************************************/
#include "dbgclnt_breakpoints.h"

#include <algorithm>
#include <stdexcept>
#include <iostream>
#include <sstream>
#include <set>

#include <string.h>
#include <assert.h>

#include "dbginvoke.h"

MappedBreakpoint::MappedBreakpoint(int bp_id, const Bitset *group, const dbgbp_t &bp)
    : m_logicalId{bp_id},
      m_type{bp.bp_type},
      m_pending(bp.bp_pending && bp.bp_stmt != nullptr),
      m_locStmt(m_pending ? bp.bp_stmt : ""),
      m_locAddr(bp.bp_loc.loc_addr ? bp.bp_loc.loc_addr : ""),
      m_locFile{""},
      m_locFunc{""},
      m_locLine{bp.bp_loc.loc_line},
      m_watchExp(bp.bp_exp ? bp.bp_exp : ""),
      m_threadId(bp.bp_threadId ? bp.bp_threadId : ""),
      m_ranksSet(group->clone()),
      m_enabledSet(bp.bp_enabled ? group->clone() : nullptr),
      m_idList{},
      m_dispList{}
{
    // Add the group and disposition to the disp list
    m_dispList.emplace_back(group, bp.bp_disp);

    // Set the id unique to the rank
    m_idList.emplace_back(group, bp.bp_fe_id);

    if (m_locLine >= 0) {
        m_locFile = bp.bp_loc.loc_file;
        m_locFunc = bp.bp_loc.loc_func;
    }
}

int MappedBreakpoint::id() const {
    return m_logicalId;
}

dbgbp_type MappedBreakpoint::type() const {
    return m_type;
}

bool MappedBreakpoint::isPending() const {
    return m_pending;
}

const std::string& MappedBreakpoint::statement() const {
    return m_locStmt;
}

const std::string& MappedBreakpoint::address() const {
    return m_locAddr;
}

const std::string& MappedBreakpoint::file() const {
    return m_locFile;
}

const std::string& MappedBreakpoint::function() const {
    return m_locFunc;
}

int MappedBreakpoint::line() const {
    return m_locLine;
}

const std::string& MappedBreakpoint::watchExpression() const {
    return m_watchExp;
}

const LocaleSet& MappedBreakpoint::watchLocaleSet() const {
    return m_watchLoc;
}

void MappedBreakpoint::addWatchLocale(const Bitset *group, const char *loc_file, int loc_line, const char *loc_func) {
    m_watchLoc.add(group, loc_file, loc_line, loc_func);
}

void MappedBreakpoint::clearWatchLocaleSet() {
    m_watchLoc.clear();
};

const std::string& MappedBreakpoint::threadId() const {
    return m_threadId;
}

const Bitset* MappedBreakpoint::ranksSet() const {
    return m_ranksSet.get();
}

const Bitset* MappedBreakpoint::enabledSet() const {
    return m_enabledSet.get();
}

const std::list<IDSet>& MappedBreakpoint::idSetList() const {
    return m_idList;
}

const std::list<MappedBreakpoint::DispositionSet>& MappedBreakpoint::dispSetList() const {
    return m_dispList;
}

void MappedBreakpoint::addBreakpoint(const Bitset *group, const dbgbp_t &bp) {
    assert(m_ranksSet != nullptr);
    assert(group != nullptr);

    // Add this group into the bp_set group
    *m_ranksSet |= *group;

    // Add internal gdb id to the bp_id_list
    auto found_id_set = std::find_if(m_idList.begin(), m_idList.end(),
    [&](const auto &id_set) {
        return id_set.id == bp.bp_fe_id;
    });

    if (found_id_set == m_idList.end()) {
        // no existing internal gdb id found, add it.
        m_idList.emplace_back(group, bp.bp_fe_id);
    } else {
        // existing internal gdb id found, add group to existing id_set
        *found_id_set->group |= *group;
    }

    // Add this group to the enabled Bitset if enabled
    if (bp.bp_enabled) {
        markEnabled(group);
    }

    // Set the disposition for the group
    markDisposition(group, bp.bp_disp);
}

void MappedBreakpoint::markEnabled(const Bitset *group) {
    assert (m_enabledSet != nullptr);
    assert (group != nullptr);
    *m_enabledSet |= *group;
}

void MappedBreakpoint::markDisabled(const Bitset *group) {
    assert (m_enabledSet != nullptr);
    assert (group != nullptr);
    *m_enabledSet -= *group;
}

void MappedBreakpoint::markDisposition(const Bitset *group, dbgbp_disp bp_disp) {
    assert (group != nullptr);

    bool found = false;
    for (const auto &disp_set : m_dispList) {
        if (disp_set.disp == bp_disp) {
            // This matches the requested disposition
            found = true;
            *disp_set.group |= *group;
        } else {
            // This doesn't match the requested disposition, so remove the group
            // We do this in case there is any overlap from a previous set
            // disposition command on a set.
            *disp_set.group -= *group;
        }
    }
    // Remove empty sets from list
    m_dispList.remove_if([](const DispositionSet &disp_set){ return disp_set.group->isEmpty(); });

    // If we didn't find the disp_set, we need to create a new entry and add it
    // to the list
    if (!found) {
        m_dispList.emplace_back(group, bp_disp);
    }
}

void MappedBreakpoint::removeGroup(const Bitset *group) {
    // Remove the group from the global bp list
    *m_ranksSet -= *group;

    // Remove the group from the bp_id list
    for (auto &id_set : m_idList) {
        *id_set.group -= *group;
    }
    // remove empty sets from list
    m_idList.remove_if([](const IDSet &id_set) {
        return id_set.group->isEmpty();
    });

    // Remove the group from the enabled list
    *m_enabledSet -= *group;

    // Remove the group from the disp list
    for (const auto &disp_set : m_dispList) {
        // Try to remove the group from this set
        *disp_set.group -= *group;
    }
    // Remove any empty sets from the list
    m_dispList.remove_if([](const MappedBreakpoint::DispositionSet &disp_set) {
        return disp_set.group->isEmpty();
    });
}

bool BreakpointManager::isEmpty() const {
    return m_breakpoints.empty();
}

void BreakpointManager::recordBreakpoint(const dbgbp_t &bp, const Bitset *group) {
    assert(group != nullptr);

    if (auto set = findByDbgBreakpoint(bp)) {
        // This is a known breakpoint
        // update the bp_set with this group and info from the bp for disp and id
        set->addBreakpoint(group, bp);
    } else {
        // create a new breakpoint for this group based on the bp
        if (bp.bp_fe_id >= m_nextID) m_nextID = bp.bp_fe_id + 1;
        m_breakpoints.emplace_back(bp.bp_fe_id, group, bp);
    }
}

void BreakpointManager::removeEmptyBreakpoints() {
    // remove breakpoints with empty ranksSet()s
    m_breakpoints.remove_if([](const MappedBreakpoint &p){ return p.ranksSet()->isEmpty(); });
}

void BreakpointManager::fixupPendingBreakpoints(const dbgbp_t &bp, const Bitset *group) {
    assert (group != nullptr);

    std::unique_ptr<Bitset> search_group = group->clone();
    do {
        // find matching logical breakpoint that matches the passed-in data:
        // dbgbp_t, group, and search_group.
        // Return a tuple containing a pointer to the logical breakpoint if it's
        // found (nullptr otherwise) and the bitset corresponding to the bitset
        // that should be removed from the logical breakpoint below
        const auto findMatchingLBP =
            [](const dbgbp_t &bp, const Bitset *group,
               const std::unique_ptr<Bitset> &search_group,
               std::list<MappedBreakpoint> &set_list)
            -> std::tuple<MappedBreakpoint *, std::unique_ptr<Bitset>> 
        {
            std::unique_ptr<Bitset> remove_group = nullptr;
            for (auto &bp_set : set_list) {
                // check to see if this bp_set is pending
                if (bp_set.isPending()) {
                    // see if this bp_id is in the id list and matches our search group
                    auto found_id_set = std::find_if(bp_set.idSetList().begin(), bp_set.idSetList().end(), 
                    [&](const auto &id_set) {
                        return id_set.id == bp.bp_fe_id && search_group->hasCommonBits(*id_set.group);
                    });

                    // See if we found a matching bp_id for group
                    if (found_id_set != bp_set.idSetList().end()) {
                        remove_group = bitset::fromAnd(*search_group, *found_id_set->group);
                        return std::tuple(&bp_set, std::move(remove_group));
                    }
                }
            }

            return std::tuple(nullptr, std::move(remove_group));
        };

        auto [bp_set, remove_group] = findMatchingLBP(bp, group, search_group, m_breakpoints);

        // See if we found a matching bp_set, if so we need to clean it up
        if (bp_set) {
            assert(remove_group != nullptr);
            bp_set->removeGroup(remove_group.get());
            removeEmptyBreakpoints();
            recordBreakpoint(bp, remove_group.get());
            *search_group -= *remove_group;
        } else {
            // no other pending breakpoints found, we are done
            return;
        }
    } while(!search_group->isEmpty());
}

MappedBreakpoint* BreakpointManager::findByLogicalID(int id) {
    if (id < 1) return nullptr;

    auto it = std::find_if(m_breakpoints.begin(), m_breakpoints.end(), 
    [=](const auto &mbp) {
        return mbp.id() == id;
    });

    return it != m_breakpoints.end() ? &(*it) : nullptr;
}

MappedBreakpoint* BreakpointManager::findByDbgBreakpoint(const dbgbp_t &bp) {
    return findByLogicalID(bp.bp_fe_id);
}

const MappedBreakpoint* BreakpointManager::findByDbgBreakpoint(const dbgbp_t &bp) const {
    // findByDbgBreakpoint is const as long as you don't mess with the returned ptr
    return const_cast<BreakpointManager*>(this)->findByDbgBreakpoint(bp);
}

MappedCheckpoint::MappedCheckpoint(unsigned int id, const std::vector<std::pair<const Bitset*, const dbgcheckpoint_t*>> &checkpointData, std::optional<unsigned long> dbgcpId) 
    : m_id{id}
{
    if (checkpointData.size() == 0) {
        throw std::runtime_error("Tried to create a MappedCheckpoint with no checkpointData");
    }

    for (const auto &[group, dbcp] : checkpointData) {
        if (group == nullptr || dbcp == nullptr) {
            throw std::runtime_error("Tried to create a MappedCheckpoing with checkpointData that contained nullptr");
        }
    }

    // we assume all bitsets are the same size
    m_allRanks = checkpointData.front().first->clone();

    for (const auto &[group, dbcp] : checkpointData) {
        *m_allRanks |= *group;
        // copy in passed dbgcheckpoint_t data. overwrite passed backend ID if specified
        m_idsMap.emplace_back(group, dbgcpId ? *dbgcpId : dbcp->id);
        m_locations.add(group, dbcp->location.loc_file, dbcp->location.loc_line, dbcp->location.loc_func);
    }

    m_current = m_allRanks->clone();
    m_current->clear();
}

MappedCheckpoint::MappedCheckpoint(unsigned int id, const std::vector<std::pair<const Bitset*, const dbgcheckpoint_t*>> &checkpointData)
    : MappedCheckpoint(id, checkpointData, {})
{}

unsigned int MappedCheckpoint::id() const {
    return m_id;
}

LocaleSet& MappedCheckpoint::locations() {
    return m_locations;
}

const LocaleSet& MappedCheckpoint::locations() const {
    return m_locations;
}

const Bitset& MappedCheckpoint::ranksSet() const {
    return *m_allRanks;
}

const std::list<IDSet>& MappedCheckpoint::idSetList() const {
    return m_idsMap;
}

const Bitset& MappedCheckpoint::currentSet() const {
    return *m_current;
}

void MappedCheckpoint::markCurrent(const Bitset &group) {
    *m_current |= group;
}

void MappedCheckpoint::unmarkCurrent(const Bitset &group) {
    *m_current -= group;
}

void MappedCheckpoint::removeGroup(const Bitset &group) {
    *m_allRanks -= group;

    // Remove the group from the id list
    for (auto &idSet : m_idsMap) {
        *idSet.group -= group;
    }
    // remove empty sets from list
    m_idsMap.remove_if([](const IDSet &id_set) {
        return id_set.group->isEmpty();
    });
}

CheckpointManager::CheckpointManager()
    : m_error{"no error"}
{}

unsigned int CheckpointManager::recordCheckpoint(unsigned int id, const std::vector<std::pair<const Bitset*, const dbgcheckpoint_t*>> &checkpointData) {
    // add checkpoint to internal checkpoint list, return true on success
    const auto makeOne = [&](unsigned int theId, const std::vector<std::pair<const Bitset*, const dbgcheckpoint_t*>> &theCheckpointData, std::optional<unsigned long> cpid) {
        try {
            m_checkpoints.emplace_back(theId, theCheckpointData, cpid);
        } catch (const std::runtime_error &e) {
            std::ostringstream err;
            err << "Failed to construct checkpoint from passed data ("
                << e.what() << ")"
            ;
            m_error = err.str();
            return false;
        }
        return true;
    };

    unsigned int successfullyMade{0};

    // on a given rank, the first checkpoint created actually produces two
    // usable checkpoints: the original process and the newly created process.
    // we need to reflect this in our client side data.

    auto shouldMakeFirst = std::any_of(checkpointData.begin(), checkpointData.end(),
        [](const auto &pair) {
            if (!pair.second) return false;
            return pair.second->id == 1;
    });

    // extra checkpoint for first checkpoint case
    if (shouldMakeFirst) {
        // first checkpoint always has real id of 0
        if (makeOne(id, checkpointData, 0)) {
            successfullyMade++;
            id++;
            m_checkpoints.back().markCurrent(m_checkpoints.back().ranksSet());
        }
    }

    // base case
    if (makeOne(id, checkpointData, {})) {
        successfullyMade++;
    }

    return successfullyMade;
}

const MappedCheckpoint* CheckpointManager::newest() const {
    return m_checkpoints.size() > 0 ? &m_checkpoints.back() : nullptr;
}

MappedCheckpoint* CheckpointManager::find(unsigned int id) {
    auto it = std::find_if(m_checkpoints.begin(), m_checkpoints.end(),
    [&](const auto &mcp) {
        return mcp.id() == id;
    });
    return it != m_checkpoints.end() ? &(*it) : nullptr;
}

void CheckpointManager::removeEmpty() {
    // remove checkpoints with empty ranksSet()s
    m_checkpoints.remove_if([](const auto &p){ return p.ranksSet().isEmpty(); });
}

void CheckpointManager::removeSingular() {
    std::set<unsigned int> removeIds; 
    
    for (const auto &checkp : m_checkpoints) {
        bool foundMatch = false;
        for (const auto &other : m_checkpoints) {
            if (checkp.id() == other.id()) continue;
            foundMatch = std::max(foundMatch, checkp.ranksSet().hasCommonBits(other.ranksSet()));
            if (foundMatch) break;
        }

        if (!foundMatch) removeIds.insert(checkp.id());
    }

    m_checkpoints.remove_if([&](const auto &cp) {
        return removeIds.count(cp.id()) != 0;
    });
}

size_t CheckpointManager::size() const {
    return m_checkpoints.size();
}

const std::string& CheckpointManager::getError() const {
    return m_error;
}
