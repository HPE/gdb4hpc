/*******************************************************************************
 * Copyright 2022 Hewlett Packard Enterprise Development LP.
 ******************************************************************************/
#pragma once

// classes that read assertion data classes (script_data.h) and run an assertion
// script with them. classes here know how to talk to the backends and are the
// classes that actually issue continue/breakpoint/etc commands.

#include "script_data.h"
#include "bitset.h"
#include "dbgclnt_breakpoints.h"
#include "dbg_client.h"
#include "dbginvoke.h"

#include <deque>
#include <functional>
#include <string>
#include <map>
#include <memory>
#include <vector>

namespace script {

class ScriptRunner {
    // Runs an assertion script. Sends start/stop/compare/etc commands to the
    // backend and keeps track of breakpoints that have been hit and assertions
    // that are ready to be evaluated
public:
    using RanksMap = std::map<DbgInvoke::Handle, std::unique_ptr<Bitset>>;
    class EventInterface; friend EventInterface;

private:
    class Expression;
    class Assertion;
    class Ranks;
    struct Result {
        std::unique_ptr<rec_set_t> recSet;
        CompareResult result;
        std::string diff;

        // return true if two results, in particular their stored difference values,
        // are the same
        bool operator==(const Result& other) const;
    };

    struct ResultPack {
        std::vector<Result> results;
        size_t passes = 0;
        size_t warnings = 0;
        size_t failures = 0;
    };

private:
    class Expression;
    class Assertion;
    class Ranks;
public:
    ScriptRunner(std::unique_ptr<data::Script>&& script);

    ScriptRunner(const ScriptRunner&) = delete;
    ScriptRunner& operator=(const ScriptRunner&) = delete;

    // return the name of the underlying assertion script (without a leading $)
    const std::string& name() const;
    // return the entire assertion script as a string
    std::string string() const;
    // return the first n lines of the assertion script as a string
    std::string string(size_t lines) const;

    // install assertion script event handlers to appropriate dis and set
    // breakpoints. changes the global "current" script to this one. returns
    // true on success
    bool install();
    // uninstall assertion script event handlers and delete set breakpoints. if
    // this script is the global "current" script, it will be removed. returns
    // true on success
    bool uninstall();
    // reset the internal state of the assertion script. returns true on success
    bool reset();
    // send a continue command to all ranks involved in the assertion script
    // that aren't already running according to the ScriptRunner's internal
    // tracking. this serves as the initial "start running" function. returns
    // true if continues were successfully sent
    bool sendContinue();
    // returns true if the assertion script should be stopped and uninstalled. the
    // conditions for stopping depend on the settings used when the user initially writes
    // the script. as a rule of thumb, a script should be stopped when every assertion has
    // at least one result, or the user set an assert to stop on failure, and it has
    // failed.
    bool shouldStop() const;
    // returns true if the assertion script has evaluated each assertion at least once.
    bool hasRunToCompletion() const;
    // returns true if all ranks are blocked (stopped or exited). if no asserts
    // can be evaluated and released, it is impossible to make progress in the
    // assertion script and it should be stopped
    bool allRanksBlocked() const;
    // returns true if all dbginvokes involved in the assertion script are at their
    // initial breakpoint.
    bool allDbgInvokesAtInitialBreakpoint() const;

    // returns true if any asserts are ready (all associated ranks are stopped
    // at the correct location) to be evaluated
    bool hasReadyAsserts() const;
    // evaluates and stores results of any ready asserts
    bool evaluateReadyAsserts();
    // sends a continue command to any ready asserts
    bool releaseReadyAsserts();

    const data::Assertion* getAssert(size_t id) const;
    const auto& asserts() const { return m_pendingAsserts; }
    // return a list of all stored diffs sorted by the diffs' ids
    std::vector<std::pair<const data::Assertion*, const Result*>> getAllDiffs() const;
    // returns a list of all results results tagged with the amount of times they repeat in a row
    std::vector<std::pair<const Result*, size_t>> getCondensedDiffs(const Assertion& assert) const;
    // return number of times that this script hasRunToCompletion().
    size_t numRuns() const;

    bool hasError() const { return m_error != ""; }
    std::string getError() const { return m_error; }
    // return textual summary of assertion results
    std::string summary() const;

    struct ResultsView {
        std::string script_name;

        size_t total_passes;
        size_t total_warns;
        size_t total_fails;
        size_t total_indeterminants;

        size_t completed_runs;

        const std::vector<data::Assertion>& asserts;
        // assert id -> results
        const std::map<int, ResultPack>& results_map;
    };
    // return a data summary of assertion results. the results point to valid
    // memory only as long as the parent is valid.
    ResultsView resultsView() const;

private:
    // returns an EventInterface object that will be passed to the public event 
    // handling function to enforce a limited API of the three functions below
    EventInterface getEventInterface();
    // notify internal tracking structures that ranks corresponding to the event
    // set and bitset have hit dbgBp
    void handleBPHit(const DbgInvoke::EventSet& eventSet, const Bitset& bitset, const dbgbp_t& dbgBp);
    // notify internal tracking structures that ranks corresponding to the event
    // set and bitset have exitted or hit an exit breakpoint
    void handleExit(const DbgInvoke::EventSet& eventSet, const Bitset& bitset);
    // pass the given event to the event handler which the script runner replaced
    // and return the result
    int passEventToOldHandler(DbgInvoke::EventSet& eventSet, const Bitset& replySet);
    // create a mapping from all dis involved in the assertion script to a bitset
    // of all of its ranks involved in the script

    std::set<DbgInvoke::Handle> getDisFromScript() const;
    // helper for sendContinue() and sendContinue(assert)
    bool sendContinueHelper(rec_set_t* recSet);
    // send a continue command to ranks involved in the passed assert that aren't
    // already stopped according to the ScriptRunner's internal tracking
    bool sendContinue(const Assertion& assert);
    // communicate with the backends and set/delete all breakpoints that are relevant
    // to the assertion script. returns true on success
    bool setBreakpoints();
    bool deleteBreakpoints();
    // for every di involved in the assertion script, swap out their event
    // handlers and replace them with event handlers that will update the
    // internals of the assertion script. returns true on success
    bool setEventHandlers();
    // restore di event handlers to what they were before the call to
    // setEventHandlers
    bool restoreOldEventHandlers();
    // send DbgHalt to all involved ranks. returns true on success
    bool sendHalt();
    std::tuple<size_t, size_t, size_t> getResultTotals() const;

    std::unique_ptr<data::Script> m_script;

    // assert id -> results
    std::map<int, ResultPack> m_results;

    std::vector<Assertion> m_pendingAsserts;
    std::unique_ptr<Ranks> m_ranks;
    std::map<DbgInvoke::Handle, DbgInvoke::AsyncEventCallback> m_oldEventHandlers;
    rec_set_t m_fullRecSet;

    std::string m_error = "";
    bool m_readyFlag = false;
    bool m_manualFinishFlag = false;
    bool m_clearResultsOnStartFlag = false;

    int m_oldPrintElements = 0;
    bool m_oldPrintBpFrames = false;

    size_t m_completedRunsCount = 0;
};

class ScriptRunner::Assertion {
    // Holds a data::Assertion. Sets breakpoints, keeps track of program state relative to
    // the data:Assertion, and keeps tracks of assertion evaluation results.
public:
    Assertion(const data::Assertion& assert);
    Assertion(Assertion&& other) = default;

    auto id() const { return m_assert.id(); }

    // Create/delete breakpoints needed to evaluate this expression. returns true on success.
    bool setBreakpoints();
    bool deleteBreakpoints();

    // return a vector of locations that have to do with this assertion, and which breakpoints
    // they map to.
    const std::vector<std::pair<int, const data::ProgramLocation*>>& breakpointLocations() const;

    // notify the object of new ranks that have stopped. before notifying, BPIsRelevant
    // should be used to check if the event matters to this assertion. returns the result
    // of isReady().
    bool notify(const DbgInvoke& di, const Bitset& bitset, const dbgbp_t& dbgBp);

    // notify the object that any internal state kept about the passed ranks is
    // no longer valid.
    void unnotify(const DbgInvoke& di, const Bitset& bitset);

    // returns true when breakpoint events for all ranks from the original expression have
    // been passed through notify().
    bool isReady() const;

    // check if a breakpoint event is relevant to the internal expression. if it
    bool BPIsRelevant(const DbgInvoke& di, const Bitset& bitset, const dbgbp_t& dbgBp) const;

    // check if a set of ranks is relevant to the internal expression.
    bool dbgInvokeIsRelevant(const DbgInvoke& di, const Bitset& bitset) const;

    // reset internal state to be waiting on all ranks.
    void reset();

    // return a map mapping dis to a bitsets representing the set of ranks that
    // are involved in this assertion.
    const std::map<DbgInvoke::Handle, std::unique_ptr<Bitset>>& ranksMap() const;

    // return a newly created rec_set_t including every di/rank mentioned in the expression.
    std::unique_ptr<rec_set_t> createRecSet() const;

    // return the original expression in a form that can be used with EvalCompare.
    const gram::Expression& expressionHandle() const;

    // return a human readable representation of the assert expression, as it was
    // originally typed.
    std::string string() const { return m_assert.string(); };
    const auto& settings() const { return m_assert.settings(); }
    // return a human readable string of settings applied to this assert that differ from
    // default assertion settings.
    std::string settingsString() const { return m_assert.settingsString(); }

private:
    const data::Assertion &m_assert;
    data::Assertion::ParsedAssertion m_parsed;

    // convenience member. reference to m_parsed.locations
    std::vector<data::ProgramLocation>& m_locs;

    // <breakpoint id, ProgramLocation>
    // pointers to ProgramLocations inside of m_locs
    std::vector<std::pair<int, const data::ProgramLocation*>> m_breakpointLocs;

    std::map<DbgInvoke::Handle, std::unique_ptr<Bitset>> m_fullSets;
    std::map<DbgInvoke::Handle, std::unique_ptr<Bitset>> m_waitingFor;
};

class ScriptRunner::Ranks {
    // Keeps track of ranks that are "blocked" (stopped or exited) for use in
    // detecting deadlocks in an assertion script.
public:
    // initialize internal structure which keeps track of which ranks are running
    // and which ranks are blocked. queries the backend for initial state
    Ranks(RanksMap&& allRanks);
    // returns internal ranks map
    const auto& ranks() const { return m_involvedRanks; }
    const auto& blockedRanks() const { return m_blockedRanks; }
    // queires backend for status (running, stopped) of all involved ranks and
    // sets internal tracking structures accordingly
    void reset();
    // mark ranks in di as blocked
    void markRanksBlocked(const DbgInvoke::Handle& di, const Bitset& ranks);
    // mark ranks in di as unblocked
    void markRanksUnblocked(const DbgInvoke::Handle& di, const Bitset& ranks);
    // mark ranks associated with assert as unblocked
    void markRanksUnblocked(const Assertion& assert);
    // mark all ranks as blocked
    void markAllRanksBlocked();
    // mark all ranks as unblocked
    void markAllRanksUnblocked();
    // returns true if all ranks are blocked (stopped or exited). if no asserts
    // can be evaluated and released, it is impossible to make progress in the
    // assertion script and it should be stopped
    bool allRanksBlocked() const;
    // returns the bitset corresponding to di or nullptr if it doesn't exist
    Bitset* blocked(const DbgInvoke& di);
    const Bitset* blocked(const DbgInvoke& di) const;
private:
    RanksMap m_involvedRanks;
    RanksMap m_blockedRanks;
};

class ScriptRunner::EventInterface  {
    // enforces a limited api for a ScriptRunner in the public event handling function
public:
    EventInterface(ScriptRunner& sc)
        : m_scriptRunner{sc}
    {}

    void handleBPHit(const DbgInvoke::EventSet& eventSet, const Bitset& bitset, const dbgbp_t& dbgBp) {
        m_scriptRunner.handleBPHit(eventSet, bitset, dbgBp);
    }
    void handleExit(const DbgInvoke::EventSet& eventSet, const Bitset& bitset) {
        m_scriptRunner.handleExit(eventSet, bitset);
    }
    int passEventToOldHandler(DbgInvoke::EventSet& eventSet, const Bitset& replySet) {
        return m_scriptRunner.passEventToOldHandler(eventSet, replySet);
    }
private:
    ScriptRunner& m_scriptRunner;
};

// there should only be one script running at a time. the application should use
// these functions to get at the current script

ScriptRunner* getCurrentScript();

// Set the current script to a script previously saved to the internal script
// table with saveScript(). returns ScriptRunner for specified script if one is
// found, nullptr otherwise. pass "" to set current script to none.
ScriptRunner* setCurrentScript(const char* name);
ScriptRunner* setCurrentScript(const std::string_view& name);
ScriptRunner* setCurrentScript(const std::string& name);

// look up script by name (no leading $) and return it if found. returns nullptr
// otherwise.
ScriptRunner* getScript(const char* name);
ScriptRunner* getScript(const std::string_view& name);
ScriptRunner* getScript(const std::string& name);

// save a script into the internal script table. returns the newly created
// ScriptRunner on success and nullptr on failure.
ScriptRunner* saveScript(std::unique_ptr<data::Script>&& script);

using ScriptTable = std::map<std::string, std::unique_ptr<ScriptRunner>>;

// return the table of all scripts
const ScriptTable& scripts();

} // namespace script
