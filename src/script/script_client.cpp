/*******************************************************************************
 * Copyright 2022 Hewlett Packard Enterprise Development LP.
 ******************************************************************************/

#include "script_client.h"
#include "cli_funcs.h"
#include "client.h"
#include "dbg_client.h"
#include "onscopeexit.h"
#include <algorithm>
#include <iostream>
#include <iomanip>

#if DEBUG
// backslash sequences are escape codes to print debug messages in dark grey. if
// output is redirected to a file, they should be removed
#define LOG(msg) \
    std::cerr << "\u001b[30;1m" << __FILE__ << ":" << __LINE__ << "(" << __FUNCTION__ << "): " << msg << "\033[0m" << std::endl
#else
// dummy log that will be optimized out but still checked for syntax errors
#define LOG(msg) \
    while (0) { std::cerr << msg; }
#endif

namespace script {

namespace {

// internal table of assertion scripts
ScriptTable g_scripts;
// name of script considered to be the "current" script. the "current" script is
// the one that the main event loop polls for activity
std::string g_currentScriptName;

// assertion script event handler installed in dis on script start. called by dis
// in the client event handling layer. to enforce a limited interface to the currently
// running script, it is accessed via the passed ScriptRunner::EventInterface object
int handleEvent(ScriptRunner::EventInterface scei, DbgInvoke::EventSet& eventSet, const Bitset& replySet);

} // anonymous namespace

using SR = ScriptRunner;

// Current script selection, viewing, and saving ///////////////////////////////

ScriptRunner* getCurrentScript() {
    return getScript(g_currentScriptName);
}

ScriptRunner* setCurrentScript(std::string_view name) {
    LOG("Set current script to " << quoted(name));

    // special case: pass "" to remove current script
    if (name == "") {
        g_currentScriptName = name;
        return nullptr;
    }

    // all other names
    auto script = getScript(name);
    if (script) {
        g_currentScriptName = name;
    }
    return script;
}

ScriptRunner* getScript(std::string_view name) {
    auto it = g_scripts.find(std::string{name});
    if (it != g_scripts.end()) {
        return it->second.get();
    }
    return nullptr;
}

ScriptRunner* saveScript(std::unique_ptr<data::Script>&& script) {
    auto name = script->name();

    LOG("Saving script " << name);
    if (getScript(name)) {
        PrintOutError("A script by the name $%s already exists.\n", name.c_str());
        // a script by this name already exists
        return nullptr;
    }

    g_scripts.emplace(name, std::make_unique<ScriptRunner>(std::move(script)));
    return getScript(name);
}

bool deleteScript(std::string_view name) {
    if (g_currentScriptName == name) {
        // script is running, don't delete it!
        return false;
    }

    g_scripts.erase(std::string{name});

    return true;
}

const ScriptTable& scripts() {
    return g_scripts;
}

// ScriptRunner public api /////////////////////////////////////////////////////

ScriptRunner::ScriptRunner(std::unique_ptr<data::Script>&& script)
    : m_script{std::move(script)} {}

const std::string& ScriptRunner::name() const {
    return m_script->name();
}

std::string ScriptRunner::string() const {
    return m_script->string();
}

std::string ScriptRunner::string(size_t lines) const {
    return m_script->string(lines);
}

const data::Script& ScriptRunner::data() const {
    return *m_script;
}

bool ScriptRunner::install() {
    if (!setBreakpoints()) {
        m_error = "Failed to set some required breakpoints";
        return false;
    }
    LOG("Set breakpoints");

    if (!setEventHandlers()) {
        m_error = "Failed to set event handlers";
        return false;
    }
    LOG("Set event handlers");

    setCurrentScript(name());

    m_oldPrintBpFrames = Client.print_bp_frames;
    m_oldPrintElements = Client.print_elements;

    return true;
}

bool ScriptRunner::uninstall() {
    LOG("Stopping");
    auto success = true;

    if (hasRunToCompletion()) {
        m_completedRunsCount++;
        m_clearResultsOnStartFlag = true;
    }

    if (!restoreOldEventHandlers()) {
        m_error = "Failed to restore original event handlers";
        success = false;
    }

    if (m_script->scriptSettings().haltUponEnd) {
        PrintOutMsg("Sending halt to all involved ranks.\n");
        if (!sendHalt()) {
            m_error = "Failed to send halt to ranks";
            success = false;
        }
    }

    if (!deleteBreakpoints()) {
        m_error = "Failed to delete breakpoints.";
        success = false;
    }

    ClientSetPrintElements(m_oldPrintElements, &m_fullRecSet);
    Client.print_bp_frames = m_oldPrintBpFrames;
    
    if (this == getCurrentScript()) {
        setCurrentScript("");
    }

    PrintOutMsg("Assertion script $%s stopped.\n", name().c_str());

    bool has_results = false;
    for (const auto& [id, rpack] : m_results) {
        has_results |= (rpack.passes > 0);
        has_results |= (rpack.warnings > 0);
        has_results |= (rpack.failures > 0);
        if (has_results) break;
    }
    if (has_results) {
        PrintOutMsg("Use \"info script $%s results\" to view results.\n", name().c_str());
    } else {
        PrintOutMsg("No assertions were evaluated.\n");
    }

    return success;
}

bool ScriptRunner::reset() {
    m_readyFlag = false;
    m_manualFinishFlag = false;

    m_pendingAsserts.clear();
    try {
        for (const auto& assert : m_script->asserts()) {
            m_pendingAsserts.emplace_back(assert);
        }
    } catch (const data::InvalidAssertionError& e) {
        m_error = e.what();
        return false;
    }

    LOG("Created " << m_pendingAsserts.size() << " assert trackers.");
    for ([[maybe_unused]] const auto& tracker : m_pendingAsserts) {
        LOG("Waiting for: " << tracker.string());
    }

    // calculate full recset and cache it for later
    m_fullRecSet.clear();
    for (const auto& tracker : m_pendingAsserts) {
        m_fullRecSet.merge(tracker.createRecSet());
    }

    auto allRanks = RanksMap{};

    // calculate map of dis to ranks involved in assertion script
    for (const auto& tracker : m_pendingAsserts) {
        for (auto& [di, bitset] : tracker.ranksMap()) {
            auto it = allRanks.find(di);
            if (it == allRanks.end()) {
                // new
                allRanks.emplace(std::make_pair(di, bitset->clone()));
            } else {
                // update
                *it->second |= *bitset;
            }
        }
    }

    LOG("ranks involved in script " << name() << ":");
    for ([[maybe_unused]] const auto& [dih, bitset] : allRanks) {
        LOG(dih.name() << ": " << bitset->toSetString());
    }

    m_ranks = std::make_unique<Ranks>(std::move(allRanks));

    if (m_clearResultsOnStartFlag) {
        m_results.clear();
        m_clearResultsOnStartFlag = false;
    }

    return true;
}

bool ScriptRunner::sendContinueHelper(rec_set_t *recSet) {
    if (!recSet) return false;

    // only send continue to ranks that are stopped.

    // copy the list of receivers as intersection operations below can shrink recSet's internal list
    auto receivers = recSet->getReceivers();

    for (const auto* rec : receivers) {
        if (!rec) continue;
        LOG("Sending continue to " << rec->rec_di.handle().name() << ": " << rec->rec_group->toSetString());
        if (const auto* blockedRanks = m_ranks->blocked(rec->rec_di)) {
            group_recv_t tmpRec {
                .rec_di = rec->rec_di,
                .rec_group = blockedRanks->clone()
            };
            recSet->intersectReceiver(&tmpRec);
        }
    }

    if (auto repSet = DbgGo(recSet, true); repSet.hasError()) {
        LOG("go error");
        return false;
    }

    // if there were any asserts that were waiting on part of the location in
    // the released ranks, they need to start waiting all over again
    for (auto& pendingAssert : m_pendingAsserts) {
        for (const auto* group : recSet->getReceivers()) {
            pendingAssert.unnotify(group->rec_di, *group->rec_group);
        }
    }

    m_manualFinishFlag = false;

    return true;
}

bool ScriptRunner::sendContinue() {
    // send continue to all ranks involved in the assertion script  
    
    PrintOutMsg("Sending continue to application...\n");

    if (!sendContinueHelper(&m_fullRecSet)) {
        PrintOutError("Failed to send continue to application.\n");
        return false;
    }

    // update internal stopped ranks map; we just sent continue to everyone, so
    // no one should be stopped any more
    m_ranks->markAllRanksUnblocked();

    LOG("Sent dbgGo (full set)");
    return true;
}

bool ScriptRunner::sendContinue(const Assertion& assert) {
    // send continue to ranks involved in passed assert 

    if (!sendContinueHelper(assert.createRecSet().get())) {
        return false;
    }

    // update internal stopped ranks map
    m_ranks->markRanksUnblocked(assert);

    LOG("Sent dbgGo (assert)");
    return true;
}

bool ScriptRunner::shouldStop() const {
    // all ranks must be stopped so we can send our cleanup 
    // (breakpoints, print elements) commands
    if (!m_ranks->allRanksBlocked()) return false;

    return m_manualFinishFlag;
}

bool ScriptRunner::hasRunToCompletion() const {
    return std::all_of(m_script->asserts().begin(), m_script->asserts().end(),
    [&](const auto& assert){
        return m_results.find(assert.id()) != m_results.end();
    });
}

bool ScriptRunner::allRanksBlocked() const {
    return m_ranks->allRanksBlocked();
}

bool ScriptRunner::allDbgInvokesAtInitialBreakpoint() const {
    return std::all_of(m_pendingAsserts.begin(), m_pendingAsserts.end(),
    [](const auto& assert){
        return std::all_of(assert.breakpointLocations().begin(), assert.breakpointLocations().end(),
        [](const auto& pair) {
            const auto& loc = pair.second;
            if (auto dih = loc->dbgInvoke(); auto* di = dih.lock()) {
                return di->isAtInitialBreakpoint();
            } else {
                return true;
            }
        });
    });
}

bool ScriptRunner::hasReadyAsserts() const {
    return m_readyFlag;
}

bool ScriptRunner::evaluateReadyAsserts() {
    for (auto& assert : m_pendingAsserts) {
        if (!assert.isReady()) continue;

        // XXX: EvalCompare waits until it's substantially lost in the
        // expr.cpp/gram.cpp logic to bother looking at the Client's epsilon values,
        // so we have to set/unset here in order to support per-assertion epsilons

        // keep track of old values
        auto oldLow = Client.cmp_lower_eps;
        auto oldHigh = Client.cmp_upper_eps;

        Client.cmp_lower_eps = assert.settings().error.first;
        Client.cmp_upper_eps = assert.settings().error.second;

        // returned by EvalCompare below
        OnScopeExit cleanupPtrs = [&](){
            // restore error settings
            Client.cmp_lower_eps = oldLow;
            Client.cmp_upper_eps = oldHigh;
        };

        LOG("Evaluating " << assert.id() << ": " << assert.string());
        if (assert.settings().verboseEval) {
            PrintOutMsg("Evaluating assertion %lu: %s\n", assert.id(), assert.string().c_str());
        }

        // Print elements affects what data gets sent back and thus affects
        // compare results. ClientSetPrintElements only does work if the value is
        // different from the current setting, so it's ok to call it every iteration
        if (Client.print_elements != 0) m_oldPrintElements = Client.print_elements;
        ClientSetPrintElements(0, &m_fullRecSet);
            
        auto recSet = assert.createRecSet();
        auto& expr = assert.expressionHandle();

        try {
            auto res = expr.evalCompare(recSet.get());

            auto [ simpleResult, diff] = res->getSimpleResult(recSet.get());

            if (assert.settings().verboseEval) {
                res->print(OutMsgStream(), recSet.get(), "");
            }

            // record the result - update counters
            auto& resultPack = m_results[assert.id()]; // creates if not present

            switch (simpleResult) {
            case CompareResult::Pass:    resultPack.passes++; break;
            case CompareResult::Warn:    resultPack.warnings++; break;
            case CompareResult::Fail:    resultPack.failures++; break;
            case CompareResult::Invalid: break; // not counted
            }

            // record result - update result/diffs
            resultPack.results.emplace_back(Result{.recSet = std::move(recSet),
                                                   .result = simpleResult,
                                                   .diff = std::move(diff)});

            if (assert.settings().stopOnFailure && simpleResult == CompareResult::Fail) {
                PrintOutMsg("Assertion %lu: %s failed. Stopping script.\n", assert.id(), assert.string().c_str());
                m_manualFinishFlag = true;
                break;
            }

        } catch (EvalError& err) {
            PrintOutError("Error in evaluation of \"%s\".\n", assert.string().c_str());
            PrintOutError("%s\n", err.what());
            uninstall();
            return false;
        }
    }

    return true;
}

bool ScriptRunner::releaseReadyAsserts() {
    auto doContinue{true};
    if (shouldStop() && m_script->scriptSettings().haltUponEnd) {
        doContinue = false;
    }
        
    for (auto& assert : m_pendingAsserts) {
        if (assert.isReady()) {
            assert.reset();
            if (doContinue && !sendContinue(assert)) {
                return false;
            }
        }
    }

    m_readyFlag = false;

    return true;
}

const data::Assertion* ScriptRunner::getAssert(size_t id) const {
    if (id > m_script->asserts().size()) {
        return nullptr;
    }
    // ids start at 1
    return &m_script->asserts()[id - 1];
}

std::vector<std::pair<const data::Assertion*, const SR::Result*>> ScriptRunner::getAllDiffs() const {
    std::vector<std::pair<const data::Assertion*, const SR::Result*>> r;

    // build list
    for (const auto& [id, resultPack] : m_results) {
        // ids start at 1
        const auto& assert = m_script->asserts()[id - 1];

        for (const auto& result : resultPack.results) {
            auto simpleResult = result.result;
            if (simpleResult == CompareResult::Warn || simpleResult == CompareResult::Fail) {
                r.emplace_back(&assert, &result);
            }
        }
    }

    // sort by assert id
    std::sort(r.begin(), r.end(), [](const auto& l, const auto& r) {
        return l.first->id() < r.first->id();
    });

    return r;
}

std::vector<std::pair<const SR::Result*, size_t>> ScriptRunner::getCondensedDiffs(const Assertion& assert) const {
    std::vector<std::pair<const SR::Result*, size_t>> stats;

    if (auto it = m_results.find(assert.id()); it != m_results.end()) {
        for (const auto& result : it->second.results) {
            const auto& Pass = CompareResult::Pass;
            const auto& Unkn = CompareResult::Invalid;

            if (result.result != Pass && result.result != Unkn) {
                if (stats.size() == 0) {
                    stats.emplace_back(std::make_pair(&result, 1ul));
                } else {
                    // if this result is the same as the last, just increase the counter
                    if (result == *stats.back().first) {
                        stats.back().second++;
                    } else {
                        stats.emplace_back(std::make_pair(&result, 1ul));
                    }
                }
            }
        }
    }

    return stats;
}

size_t ScriptRunner::numRuns() const {
    return m_completedRunsCount;
}

std::string ScriptRunner::summary() const {
    std::ostringstream ss;

    auto results = resultsView();

    ss << "Results summary for script " << "$" << results.script_name;
    if (auto settings = m_script->settingsString(); settings != "") {
       ss << ", " << settings;
    }
    ss << ":\n\n";

    ss << "Total full runs: " << results.completed_runs << "\n";

    ss << "Total assertion evaluations this run: "
       << results.total_passes
        + results.total_warns
        + results.total_fails
        + results.total_indeterminants
       << "\n";

    ss << "Passed: " << results.total_passes
       << ", Passed with warning: " << results.total_warns
       << ", Failed: " << results.total_fails
       << ", Indeterminate: " << results.total_indeterminants << "\n"
    ;

    // individual assertion results
    ss << "Assertion results:\n\n";
    for (const auto& assert : results.asserts) {
        ss << "Assertion " << assert.id();
        if (auto settings = assert.settingsString(); settings != "") {
            ss << ", " << settings; 
        }
        ss << ":\n";
        ss << assert.string() << "\n";

        size_t passes = 0;
        size_t warnings = 0;
        size_t failures = 0;

        if (auto it = results.results_map.find(assert.id()); it != results.results_map.end()) {
            passes = it->second.passes;
            warnings = it->second.warnings;
            failures = it->second.failures;
        }

        ss << "Passes: " << passes
           << ", Passes with warnings: " << warnings
           << ", Failures: " << failures << "."
           << "\n\n"
        ;
    }

    return ss.str();
}

SR::ResultsView ScriptRunner::resultsView() const {
    auto results = ResultsView{
        .script_name = m_script->name(),
        .total_passes = 0,
        .total_warns = 0,
        .total_fails = 0,
        .total_indeterminants = 0,
        .completed_runs = 0,
        .asserts = m_script->asserts(),
        .results_map = m_results
    };

    for (const auto& [id, resultPack] : m_results) {
        for (const auto& r : resultPack.results) {
            switch (r.result) {
            case CompareResult::Pass: results.total_passes++; break;
            case CompareResult::Warn: results.total_warns++; break;
            case CompareResult::Fail: results.total_fails++; break;
            case CompareResult::Invalid: results.total_indeterminants++; break;
            }
        }
    }

    results.completed_runs = m_completedRunsCount;

    return results;
}

// ScriptRunner event handler api //////////////////////////////////////////////

SR::EventInterface ScriptRunner::getEventInterface() {
    return EventInterface(*this);
}

void ScriptRunner::handleBPHit(const DbgInvoke::EventSet& eventSet, const Bitset& replySet, const dbgbp_t& dbgBp) {
    if (!eventSet.event()) {
        // should never happen
        PrintOutError("Assertion script event handler received event set containing nullptrs");
        return;
    }

    // We don't want the noisy breakpoint output that the old event handler will give us...
    if (Client.print_bp_frames != m_script->scriptSettings().verboseBreakpoints) {
        m_oldPrintBpFrames = Client.print_bp_frames;
    }
    // ...unless the user specifically asks for it
    Client.print_bp_frames = m_script->scriptSettings().verboseBreakpoints;

    if (dbgBp.bp_type == DBGBP_EXIT_TYPE) {
        LOG("DBGBP_EXIT_TYPE");
        handleExit(eventSet, replySet);
        return;
    } else {
        LOG("DBGEV_BPHIT");
        // notify each assert of the breakpoint and check that
        // it's relevant to at least one of them
        bool relevantBreakpoint = false;
        for (auto& assert : m_pendingAsserts) {
            if (assert.BPIsRelevant(eventSet.di(), replySet, dbgBp)) {
                relevantBreakpoint = true;
                auto ready = assert.notify(eventSet.di(), replySet, dbgBp);
                m_readyFlag = std::max(ready, m_readyFlag);
            }
        }
        if (!relevantBreakpoint) {
            // we've hit a breakpoint which no assertion cares about. if this
            // breakpoint is an "interruption", we stop the script. but if
            // all the assertions that needed the stopped ranks are already
            // finished, it's not really an interruption and we can let it slide.
            bool someoneStillNeededIt = false;
            for (const auto& assert : m_pendingAsserts) {
                if (m_results.find(assert.id()) == m_results.end() &&
                    assert.dbgInvokeIsRelevant(eventSet.di(), replySet)) {
                    // found an unfinished assertion that cared about these ranks
                    someoneStillNeededIt = true;
                    break;
                }
            }
            if (someoneStillNeededIt) {
                m_error = "A breakpoint unrelated to the assertion script was hit.";
                return;
            } else {
                // no one needed it anyway... we'll just mark it as blocked
                // below and call it a day.
            }
        }

        // update internal blocked ranks map
        m_ranks->markRanksBlocked(eventSet.di().handle(), replySet);
    }
}

void ScriptRunner::handleExit(const DbgInvoke::EventSet& eventSet, const Bitset& replySet) {
    if (!eventSet.event()) {
        // should never happen
        PrintOutError("Assertion script event handler received event set containing nullptrs");
        return;
    }

    // check that we're actually dealing with an exit event
    switch (eventSet.event()->ev_event) {
        case DBGEV_BPHIT: {
            if (eventSet.event()->ev_bp->bp_type != DBGBP_EXIT_TYPE) {
                LOG("exit ignored (not exit breakpoint)");
                return;
            }
            // ok
            break;
        }

        case DBGEV_EXIT: {
            // ok
            break;
        }

        default: {
            LOG("exit ignored (irrelevant event)");
            return;
        }
    }

    LOG("exit recorded");

    // mark ranks that have exited as blocked
    m_ranks->markRanksBlocked(eventSet.di().handle(), replySet);

    for ([[maybe_unused]] const auto& [dih, ranks] : m_ranks->blockedRanks()) {
        LOG(dih.name() << ": " << ranks->toSetString());
    }
}

int ScriptRunner::passEventToOldHandler(DbgInvoke::EventSet& eventSet, const Bitset& replySet) {
    return m_oldEventHandlers.at(eventSet.di().handle())(eventSet, replySet);
}

// ScriptRunner private helpers ////////////////////////////////////////////////

std::set<DbgInvoke::Handle> ScriptRunner::getDisFromScript() const {
    auto dis = std::set<DbgInvoke::Handle>{};

    for (const auto& assert : m_pendingAsserts) {
        LOG("Getting dis from " << assert.string());
        for (const auto& [bpid, loc] : assert.breakpointLocations()) {
            if (loc->dbgInvoke().lock()) {
                dis.insert(loc->dbgInvoke());
            } else {
                PrintOutMsg("Warning: Expired procset $%s. Ignoring. \n",
                              loc->dbgInvoke().c_str());
            }
        }
    }

    // debug print
    for ([[maybe_unused]] const auto& dih : dis) {
        LOG("Got dis:");
        LOG("- " << dih.name());
    }

    return dis;
}

bool ScriptRunner::setBreakpoints() {
    for (auto& assert : m_pendingAsserts) {
        if (!assert.setBreakpoints()) {
            return false;
        }
    }

    return true; 
}

bool ScriptRunner::deleteBreakpoints() {
    for (auto& assert : m_pendingAsserts) {
        if (!assert.deleteBreakpoints()) {
            return false;
        }
    }

    return true;
}

const data::Assertion& SR::Assertion::data() const {
    return m_assert;
}

const std::vector<std::pair<int, const data::ProgramLocation*>>& SR::Assertion::breakpointLocations() const {
    return m_breakpointLocs;
}

bool ScriptRunner::setEventHandlers() {
    // for each di, install new event handler and keep track of old one
    for (auto dih : getDisFromScript()) {
        if (auto* diptr = dih.lock()) {
            m_oldEventHandlers.insert({dih, diptr->getAsyncEventCallback()});
            diptr->setAsyncEventCallback([this](DbgInvoke::EventSet& e, const Bitset& b) {
                LOG("Passing to handleEvent");
                return handleEvent(this->getEventInterface(), e, b);
            });
        } else {
            LOG("Null handle? " << dih.name());
        }
    }

    return true;
}

bool ScriptRunner::restoreOldEventHandlers() {
    for (auto dih : getDisFromScript()) {
        if (auto diptr = dih.lock()) {
            auto it = m_oldEventHandlers.find(dih);
            if (it != m_oldEventHandlers.end()) {
                diptr->setAsyncEventCallback(it->second);
                LOG("Restored old event handler for " << dih.name());
            } else {
                LOG("No old event handler found for " << dih.name());
            }
        }
    }

    return true;
}

bool ScriptRunner::sendHalt() {
    LOG("Sending halt");
    auto repSet = DbgInterrupt(&m_fullRecSet, true);

    // process error
    if (ProcessErrorInRepSet(repSet)) {
        return false;
    }

    // all ranks are blocked
    m_ranks->markAllRanksBlocked();

    // process location
    for (auto&& reply : repSet) {
        // point at location data
        auto loc = EVENT_LOC(reply->rep_event);
        PrintHaltedLocationSet(reply->rep_di.c_str(), reply->rep_group.get(), loc);
    }

    return true;
}

std::tuple<size_t, size_t, size_t> ScriptRunner::getResultTotals() const {
    size_t passes = 0;
    size_t warns = 0;
    size_t fails = 0;
    for (const auto& [id, resultPack] : m_results) {
        passes += resultPack.passes;
        warns += resultPack.warnings;
        fails += resultPack.failures;
    }
    return {passes, warns, fails};
}

bool SR::Result::operator==(const Result& other) const {
    return ( result == other.result && diff == other.diff);
}

static std::unique_ptr<rec_set_t> createRecSetForLocation(const data::ProgramLocation& loc) {
    LOG("creating recset for " << loc.getLogString());

    auto result = std::make_unique<rec_set_t>();
    if (auto* di = loc.dbgInvoke().lock()) {
        result->addReceiver(NewGroupReceiver(*di, &loc.bitset()));
    } else {
        PrintOutMsg("Warning: expired procset $%s. Ignoring.\n", loc.dbgInvoke().c_str());
    }

    return result;
}

SR::Assertion::Assertion(const data::Assertion& assert)
    : m_assert{assert}
    , m_parsed{assert.parse()} // throws InvalidAssertionError on error
    , m_locs{m_parsed.locations}
{
    for (const auto& loc : m_locs) {
        m_breakpointLocs.emplace_back(-1, &loc);
    }

    reset();
}

bool SR::Assertion::setBreakpoints() {
    auto success = true;

    for (auto& [bpid, loc] : m_breakpointLocs) {
        LOG("Setting breakpoint for " << loc->getLogString());
        auto recSet = createRecSetForLocation(*loc);
        auto repSet = DbgSetLineBreak(recSet.get(), 0, loc->filename().c_str(),
                                      loc->lineNumber(), recSet->getThreads());

        if (ProcessErrorInRepSet(repSet)) {
            LOG("Error setting breakpoint: " << loc->getLogString());
            return false;
        }

        for (const auto& reply : repSet) {
            const auto* dbgBp = EVENT_BP(reply->rep_event);
            auto* di = reply->rep_di.lock();
            const auto* bp = di->breakpoints().findByDbgBreakpoint(*dbgBp);

            if (!bp) {
                LOG("Error setting breakpoint: " << loc->getLogString());
                success = false;
                break;
            }

            if (bpid == -1) {
                bpid = bp->id();
                LOG("Set breakpoint " << di->handle().name() << ": " << bp->id() << ": "
                                      << bp->file() << ":" << bp->line());
            } else if (bpid != bp->id()) {
                LOG("Error setting breakpoint: got two logical ids");
                success = false;
                break;
            }
        }
    }

    return success;
}

bool SR::Assertion::deleteBreakpoints() {
    for (auto& [bpid, loc] : m_breakpointLocs) {
        LOG("Deleting breakpoint " << bpid);

        auto recSet = createRecSetForLocation(*loc);

        // if another expression already deleted our breakpoint, we can stop early
        { // make sure our assumption that our recSet only has one unique di is correct
            auto seen = DbgInvoke::Handle(nullptr);
            for (const auto& rec : recSet.get()) {
                if (!seen.lock()) {
                    seen = rec->rec_di.handle();
                } else if (seen != rec->rec_di.handle()) {
                    LOG("Found multiple dis, can't properly delete "
                        "breakpoints");
                    return false;
                }
            }
        }

        // short circuit if our breakpoint is already gone
        if (recSet->getReceivers().size() == 0)
            continue;

        auto& di = recSet->getReceivers().front()->rec_di;

        if (!di.breakpoints().findByLogicalID(bpid)) {
            LOG("Breakpoint " << bpid << " already deleted.");
            bpid = -1;
            continue;
        }

        // returns nullptr on success
        auto repSet = DbgDeleteBreak(recSet.get(), bpid);

        if (repSet.hasError()) {
            ProcessErrorInRepSet(repSet);
            LOG("Error deleting breakpoint: " << bpid);
            return false;
        }

        LOG("Deleted breakpoint " << bpid);
        bpid = -1;
    }

    return true;
}

bool SR::Assertion::notify(const DbgInvoke& di, const Bitset &bitset, const dbgbp_t& dbgBp) {
    LOG(m_assert.string() << " notified of " << di.handle().name() << ": "
                          << bitset.toSetString() << " at backend breakpoint "
                          << dbgBp.bp_fe_id);
    LOG("bitset before " << m_waitingFor.at(di.handle())->toSetString());

    *m_waitingFor.at(di.handle()) -= bitset;

    LOG("bitset (" << (void*)m_waitingFor.at(di.handle()).get() << ") now "
                   << m_waitingFor.at(di.handle())->toSetString());

    return isReady();
}

void SR::Assertion::unnotify(const DbgInvoke& di, const Bitset &bitset) {
    if (auto it = m_waitingFor.find(di.handle()); it != m_waitingFor.end()) {
        // we are now waiting for these ranks again
        (*it->second |= bitset) &= *m_fullSets.at(di.handle());
    }
}

bool SR::Assertion::isReady() const {
    // return true when all bitsets are empty
    auto foundNotReady = std::any_of(m_waitingFor.begin(), m_waitingFor.end(), [](const auto& pair){
        return pair.second->numSet() > 0;
    });
    LOG("ready? " << !foundNotReady);
    return !foundNotReady;
}

bool SR::Assertion::BPIsRelevant(const DbgInvoke& di, const Bitset& bitset, const dbgbp_t& dbgBp) const {
    auto it = m_waitingFor.find(di.handle());
    if (it == m_waitingFor.end()) {
        // irrelevant di
        return false;
    }

    const auto& dbgLoc = dbgBp.bp_loc;

    for (auto& [bpid, loc] : m_breakpointLocs) {
        if (loc->dbgInvoke() != di.handle()) {
            // irrelevant di
            continue;
        }

        // Changing the breakpoint hit event to potentially contain multiple bp's would
        // be a more robust way to handle multiple breakpoints at the same PC.  
        if (dbgBp.bp_fe_id != bpid && !loc->matches(dbgLoc.loc_file, dbgLoc.loc_line)) {
            continue;
        }

        if (!loc->bitset().hasCommonBits(bitset)) {
            // irrelevant bitset
            continue;
        }

        return true;
    }

    return false;
}

bool SR::Assertion::dbgInvokeIsRelevant(const DbgInvoke& di, const Bitset& bitset) const {
    auto it = m_waitingFor.find(di.handle());
    if (it == m_waitingFor.end()) {
        // irrelevant di
        return false;
    }

    if (!it->second->hasCommonBits(bitset)) {
        // irrelevant bitset
        return false;
    }

    return true;
}

void SR::Assertion::reset() {
    LOG("resetting expression tracker");

    // create di -> bitset map
    m_fullSets.clear();
    for (auto& [bpid, loc] : m_breakpointLocs) {
        auto it = m_fullSets.find(loc->dbgInvoke());
        if (it != m_fullSets.end()) {
            // merge with existing entry
            *it->second |= loc->bitset();
        } else {
            // create new entry
            m_fullSets.emplace(std::make_pair(loc->dbgInvoke(), loc->bitset().clone()));
        }
    }

    m_waitingFor.clear();
    for (const auto& [di, bitset] : m_fullSets) {
        m_waitingFor.emplace(std::make_pair(di, bitset->clone()));
    }

    LOG("wait mapping reset:");
    for ([[maybe_unused]] const auto& [dih, bitset] : m_waitingFor) {
        LOG("di " << dih.name() << " waiting for " << bitset->toSetString());
    }
}

const std::map<DbgInvoke::Handle, std::unique_ptr<Bitset>>&
SR::Assertion::ranksMap() const {
    return m_fullSets;
}

const gram::Expression& SR::Assertion::expressionHandle() const {
    return *m_parsed.expression;
}

std::unique_ptr<rec_set_t> SR::Assertion::createRecSet() const {
    auto result = std::unique_ptr<rec_set_t>();
    for (auto& [bpid, loc] : m_breakpointLocs) {
        if (!result)
            result = createRecSetForLocation(*loc);
        else
            result->merge(createRecSetForLocation(*loc));
    }
    return result;
}

// ScriptRunner private blocked ranks tracking api /////////////////////////////

SR::Ranks::Ranks(RanksMap&& allRanks) 
    : m_involvedRanks{std::move(allRanks)}
{
    reset();
}

void SR::Ranks::reset() {
    LOG("Resetting blocked ranks map");
    m_blockedRanks.clear();

    // figure out who is running and who isn't
    auto tmpRecSet = rec_set_t();
    for (auto& [dih, bitset] : m_involvedRanks) {
        // XXX: Iterators over stl maps return the key as a const object
        // so that the key can't be changed while iterating. Creating a
        // group receiver doesn't change the Handle in a way that would
        // affect hashing, so it's okay to const cast in this case
        auto* diptr = const_cast<DbgInvoke*>(dih.lock());
        if (!diptr) {
            PrintOutMsg("Warning: expired procset $%s. Ignoring.\n", dih.c_str());
            continue;
        }

        auto tmpGroup = new group_recv_t {
            .rec_di = *diptr,
            .rec_group = bitset->clone()
        };
        // takes ownership of tmpGroup
        tmpRecSet.addReceiver(tmpGroup);
    }

    auto repSet = DbgStatus(&tmpRecSet, 0);

    if (ProcessErrorInRepSet(repSet)) {
        // fall back to educated guess
        PrintOutError("Warning: couldn't detect all ranks' statuses.\n");
        PrintOutError("Assertion script will assume everything is stopped to start.\n");
        for (const auto& [di, bitset] : m_involvedRanks) {
            m_blockedRanks.emplace(std::make_pair(di, bitset->clone()));
        }
    } else {
        for (const auto& reply : repSet) {
            if (reply->rep_event->ev_event == DBGEV_STAT) {
                LOG("stat reply: " << reply->rep_di.name() << ": " << reply->rep_group->toSetString() << " " << reply->rep_event->ev_u.ev_u_status->stat);
                const auto& stat = reply->rep_event->ev_u.ev_u_status->stat;
                if (stat == DBGSTAT_STOPPED || stat == DBGSTAT_STOPPED_IN_THREAD) {
                    auto it = m_blockedRanks.find(reply->rep_di);
                    if (it == m_blockedRanks.end()) {
                        // new
                        m_blockedRanks.emplace(std::make_pair(reply->rep_di, reply->rep_group->clone()));
                    } else {
                        // update
                        *it->second |= *reply->rep_group;
                    }
                }
            } else {
                // should never happen
                PrintOutError("Error in detecting assertion starting state.\n");
            }
        }
    }

    LOG("Reset blocked ranks map:");
    for ([[maybe_unused]] const auto& [dih, ranks] : m_blockedRanks) {
        LOG(dih.name() << ": " << ranks->toSetString());
    }
}

void SR::Ranks::markRanksBlocked(const DbgInvoke::Handle& di, const Bitset& ranks) {
    if (auto it = m_blockedRanks.find(di); it != m_blockedRanks.end()) {
        (*it->second |= ranks) &= *m_involvedRanks.at(di);
    }
    LOG("Blocked ranks:");
    for ([[maybe_unused]] const auto& [diHdl, bitset] : m_blockedRanks) {
        LOG(diHdl.name() << ": " << bitset->toSetString());
    }
}

void SR::Ranks::markRanksUnblocked(const DbgInvoke::Handle& di, const Bitset& ranks) {
    if (auto it = m_blockedRanks.find(di); it != m_blockedRanks.end()) {
        *it->second -= ranks;
    }
    LOG("Blocked ranks:");
    for ([[maybe_unused]] const auto& [diHdl, bitset] : m_blockedRanks) {
        LOG(diHdl.name() << ": " << bitset->toSetString());
    }
}

void SR::Ranks::markRanksUnblocked(const Assertion& assert) {
    for (const auto& [di, bitset] : assert.ranksMap()) {
        markRanksUnblocked(di, *bitset);
    }
    LOG("Blocked ranks:");
    for ([[maybe_unused]] const auto& [dih, bitset] : m_blockedRanks) {
        LOG(dih.name() << ": " << bitset->toSetString());
    }
}

void SR::Ranks::markAllRanksBlocked() {
    for (auto& [di, bitset] : m_blockedRanks) {
        *bitset = *m_involvedRanks.at(di);
    }
    LOG("Blocked ranks:");
    for ([[maybe_unused]] const auto& [dih, bitset] : m_blockedRanks) {
        LOG(dih.name() << ": " << bitset->toSetString());
    }
}

void SR::Ranks::markAllRanksUnblocked() {
    for (auto& [di, bitset] : m_blockedRanks) {
        bitset->clear();
    }
    LOG("Blocked ranks:");
    for ([[maybe_unused]] const auto& [dih, bitset] : m_blockedRanks) {
        LOG(dih.name() << ": " << bitset->toSetString());
    }
}

bool SR::Ranks::allRanksBlocked() const {
    LOG("checking for full block...");
    for (const auto& [di, blockedBitset] : m_blockedRanks) {
        LOG("di: " << di.name() << " " << blockedBitset->toSetString());
        if (*blockedBitset != *m_involvedRanks.at(di)) {
            // found a case where not all ranks are blocked
            LOG(di.name() << ": " << blockedBitset->toSetString() << " != " << m_involvedRanks.at(di)->toSetString());
            return false;
        }        
    }

    LOG("full block.");
    return true;
}

Bitset* SR::Ranks::blocked(const DbgInvoke& di) {
    auto it = m_blockedRanks.find(di.handle());
    if (it == m_blockedRanks.end()) {
        return nullptr;
    }
    return it->second.get();
}

const Bitset* SR::Ranks::blocked(const DbgInvoke& di) const {
    // blocked() is const if the returned ptr is cast to const
    return const_cast<Ranks*>(this)->blocked(di);
}

namespace {

int handleEvent(ScriptRunner::EventInterface scei, DbgInvoke::EventSet& eventSet, const Bitset& replySet) {
    if (!eventSet.event()) {
        LOG("Called with nullptr");
        return -1;
    }
    LOG("Event! di: " << eventSet.di().handle().name() << " eventSet: " << eventSet.group().toSetString() << " replySet: " << replySet.toSetString());

    switch (eventSet.event()->ev_event) {
        case DBGEV_EXIT: {
            LOG("DBGEV_EXIT");
            scei.handleExit(eventSet, replySet);
            return scei.passEventToOldHandler(eventSet, replySet);
        }

        case DBGEV_BPHIT: {
            LOG("DBGEV_BPHIT");
            const auto* dbgBp = eventSet.event()->ev_bp;
            if (!dbgBp) {
                // should never happen
                LOG("null dbgBP");
                return -1;
            }

            scei.handleBPHit(eventSet, replySet, *dbgBp);
            return scei.passEventToOldHandler(eventSet, replySet);
        }

        default: {
            // includes DBGEV_ERROR case
            LOG("default");
            return scei.passEventToOldHandler(eventSet, replySet);
        };
    }

    // should never get here
    return -1;
}

} // anonymous namespace

} // namespace script
