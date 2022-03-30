/*******************************************************************************
 * Copyright 2022 Hewlett Packard Enterprise Development LP.
 ******************************************************************************/
#pragma once

// classes that read assertion data classes (assert_data.h) and run an assertion
// script with them. classes here know how to talk to the backends and are the
// classes that actually issue continue/breakpoint/etc commands.

#include "assert_data.h"
#include "bitset.h"
#include "dbgclnt_breakpoints.h"
#include "dbg_client.h"
#include "dbginvoke.h"
#include "expression_handle.h"
#include "val.h"
#include "gram.h"

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
    using RanksMap = std::map<const dbginvoke*, std::unique_ptr<Bitset>, dbginvokeMapper>;
    class EventInterface; friend EventInterface;

private:
    class Expression;
    class Assertion;
    struct Result;
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
    // reset the internal state of the assertion script
    void reset();
    // send a continue command to all ranks involved in the assertion script
    // that aren't already running according to the ScriptRunner's internal
    // tracking. this serves as the initial "start running" function. returns
    // true if continues were successfully sent
    bool sendContinue();
    // returns true if the assertion script is considered finished and should be
    // stopped. the definition of finished depends on the options that the user
    // specified when writing the script
    bool finished() const;
    // returns true if all ranks are blocked (stopped or exited). if no asserts
    // can be evaluated and released, it is impossible to make progress in the
    // assertion script and it should be stopped
    bool allRanksBlocked() const;

    // returns true if any asserts are ready (all associated ranks are stopped
    // at the correct location) to be evaluated
    bool hasReadyAsserts() const;
    // evaluates and stores results of any ready asserts
    bool evaluateReadyAsserts();
    // sends a continue command to any ready asserts
    bool releaseReadyAsserts();

    const Assertion* getAssert(size_t id) const;
    const auto& asserts() const { return m_pendingAsserts; }
    // return a list of all stored diffs sorted by the diffs' ids
    const std::vector<std::pair<const Assertion*, const Result*>> getAllDiffs() const;

    bool hasError() const { return m_error != ""; }
    std::string getError() const { return m_error; }
    // return summary of assertion results
    std::string summary() const;

private:
    // returns an EventInterface object that will be passed to the public event 
    // handling function to enforce a limited API of the three functions below
    EventInterface getEventInterface();
    // notify internal tracking structures that ranks corresponding to the event
    // set and bitset have hit dbgBp
    void handleBPHit(event_set& eventSet, const Bitset& bitset, const dbgbp_t& dbgBp);
    // notify internal tracking structures that ranks corresponding to the event
    // set and bitset have exitted or hit an exit breakpoint
    void handleExit(event_set& eventSet, const Bitset& bitset);
    // pass the given event to the event handler which the script runner replaced
    // and return the result
    int passEventToOldHandler(event_set& eventSet, const Bitset& replySet);
    // create a mapping from all dis involved in the assertion script to a bitset
    // of all of its ranks involved in the script

    std::set<dbginvoke*, dbginvokeMapper> getDisFromScript() const;
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
    std::vector<Assertion> m_pendingAsserts;
    std::unique_ptr<Ranks> m_ranks;
    std::map<const dbginvoke*, clnt_ev_callback_func, dbginvokeMapper> m_oldEventHandlers;
    rec_set_t m_fullRecSet;

    std::string m_error = "";
    bool m_readyFlag = false;
    bool m_manualFinishFlag = false;

    int m_oldPrintElements = 0;
    bool m_oldPrintBpFrames = false;
};

class ScriptRunner::Expression {
    // Holds a data::Expression, keeps track of program state, and notifies when
    // the program state matches the location of the held data::expression. When the
    // program state matches, a compare call can be made by the ScriptRunner.
public:
    Expression(const data::Expression& expression);

    // return the data::Expression which the object is tracking
    const auto& expression() const { return m_expression; }

    // Create/delete breakpoints needed to evaluate this expression. returns true on success.
    bool setBreakpoints();
    bool deleteBreakpoints();
    // notify the object of new ranks that have stopped. if passed di does not
    // match a di from the original expression, this function is a no-op. returns
    // the result of isReady().
    bool notify(const dbginvoke& di, const Bitset& bitset, const dbgbp_t& dbgBp);
    // notify the object that any internal state kept about the passed ranks is
    // no longer valid.
    void unnotify(const dbginvoke& di, const Bitset& bitset);
    // returns true when all ranks from the original expression have been passed
    // through notify() with correct breakpoint locations.
    bool isReady() const;
    // check if a breakpoint event is relevant to the internal expression. if it
    // is, returns the MappedBreakpoint that matches it. if it isn't, returns nullptr
    const MappedBreakpoint* BPIsRelevant(const dbginvoke& di, const Bitset& bitset, const dbgbp_t& dbgBp) const;
    // check if a set of ranks is relevant to the internal expression.
    bool procsetIsRelevant(const dbginvoke& di, const Bitset& bitset) const;
    // reset internal state to be waiting on all ranks
    void reset();
    // return a map mapping dis to a bitsets representing the set of ranks that
    // are involved in this assertion
    const auto& ranksMap() const { return m_fullSets; }
    // return a newly created rec_set_t including every di/rank mentioned in the expression
    std::unique_ptr<rec_set_t> createRecSet() const;

    // return the full expression as originally typed
    std::string string() const { return m_expression.string(); }
    // return portion of the expression appropriate for passing to a compare evaluation
    std::string evalString() const { return m_expression.evalString(); }

private:
    const data::Expression& m_expression;
    std::map<const dbginvoke*, std::unique_ptr<Bitset>, dbginvokeMapper> m_fullSets;
    std::map<const dbginvoke*, std::unique_ptr<Bitset>, dbginvokeMapper> m_waitingFor;
    int m_breakPointId;
    mutable std::unique_ptr<rec_set_t> m_cachedRecSet;
};

using ValPtr = std::unique_ptr<val_t, decltype(&DestroyVal)>;

struct ScriptRunner::Result {
    std::unique_ptr<rec_set_t> recSet;
    ValPtr result;
    ValPtr difference;

    // return true if two results, in particular their stored difference values,
    // are the same
    bool operator==(const Result& other) const;
};

class ScriptRunner::Assertion {
    // Holds two instances of Expression and a binary operation. supports
    // the same operations as Expression, forwarding each call to the
    // underlying expressions. when the program state is ready to evaluate the
    // assertion, Assertion can generate the ExpressionHandle object for use
    // with EvalCompare etc

public:
    Assertion(const data::Assertion& assert);
    Assertion(Assertion&& other) = default;

    const auto& leftHandSide() const { return m_lhs; }
    const auto& rightHandSide() const { return m_rhs; }

    // forward to underlying instances Expression and return the logical
    // and/or (depending on the operation) of the results if applicable

    auto id() const { return m_assert.id(); }
    bool setBreakpoints();
    bool deleteBreakpoints();
    bool notify(const dbginvoke& di, const Bitset& bitset, const dbgbp_t& dbgBp);
    void unnotify(const dbginvoke& di, const Bitset& bitset);
    bool isReady() const;
    bool isFinished() const;
    bool BPIsRelevant(const dbginvoke& di, const Bitset& bitset, const dbgbp_t& dbgBp) const;
    bool procsetIsRelevant(const dbginvoke& di, const Bitset& bitset) const;
    void reset();
    RanksMap ranksMap() const;
    std::unique_ptr<rec_set_t> createRecSet() const;
    const auto& settings() const { return m_assert.settings(); }

    std::unique_ptr<ExpressionHandle> expressionHandle() const;
    std::string string() const { return m_assert.string(); };
    std::string settingsString() const { return m_assert.settingsString(); }
    std::string summary() const;

    // result storage

    // takes ownership of both pointers
    void recordResult(ValPtr&& result, ValPtr&& difference);
    std::tuple<size_t, size_t, size_t> getResultTotals() const;
    const std::vector<Result>& results() const;
    std::vector<std::pair<const Result*, size_t>> getCondensedDiffs() const;

private:
    const data::Assertion& m_assert;
    Expression m_lhs;
    data::Op m_op;
    Expression m_rhs;

    size_t m_passes;
    size_t m_warns;
    size_t m_fails;
    std::vector<Result> m_results;
    mutable std::unique_ptr<rec_set_t> m_cachedRecSet;
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
    // queires backend for status (running, stopped) of all involved ranks and
    // sets internal tracking structures accordingly
    void reset();
    // mark ranks in di as blocked
    void markRanksBlocked(const dbginvoke& di, const Bitset& ranks);
    // mark ranks in di as unblocked
    void markRanksUnblocked(const dbginvoke& di, const Bitset& ranks);
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
    Bitset* blocked(const dbginvoke& di);
    const Bitset* blocked(const dbginvoke& di) const;
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

    void handleBPHit(event_set& eventSet, const Bitset& bitset, const dbgbp_t& dbgBp) {
        m_scriptRunner.handleBPHit(eventSet, bitset, dbgBp);
    }
    void handleExit(event_set& eventSet, const Bitset& bitset) {
        m_scriptRunner.handleExit(eventSet, bitset);
    }
    int passEventToOldHandler(event_set& eventSet, const Bitset& replySet) {
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