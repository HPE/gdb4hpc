/*******************************************************************************
 * Copyright 2022 Hewlett Packard Enterprise Development LP.
 ******************************************************************************/

#include "script_client.h"
#include "compare_util.h"
#include "cli_funcs.h"
#include "client.h"
#include "dbg_client.h"
#include "gram.h"
#include "procset.h"
#include "val.h"
#include "gram.h"
#include "onscopeexit.h"

#include <algorithm>

#if DEBUG
#include <iostream>
#include <iomanip>
// backslash sequences are escape codes to print debug messages in dark grey. if
// output is redirected to a file, they should be removed
#define LOG(msg) \
    std::cerr << "\u001b[30;1m" << __FILE__ << ":" << __LINE__ << "(" << __FUNCTION__ << "): " << msg << "\033[0m" << std::endl
#else 
#define LOG(msg)
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
// running script, it is access via the passed ScriptRunner::EventInterface object
int handleEvent(ScriptRunner::EventInterface scei, event_set* eventSet, const Bitset* replySet);

} // anonymous namespace

using SR = ScriptRunner;

// Current script selection, viewing, and saving ///////////////////////////////

ScriptRunner* getCurrentScript() {
    return getScript(g_currentScriptName);
}

ScriptRunner* setCurrentScript(const char* name) {
    return setCurrentScript(std::string{name});
}

ScriptRunner* setCurrentScript(const std::string_view& name) {
    return setCurrentScript(std::string(name.data(), name.length()));
}

ScriptRunner* setCurrentScript(const std::string& name) {
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

ScriptRunner* getScript(const char* name) {
    return getScript(std::string{name});
}

ScriptRunner* getScript(const std::string_view& name) {
    return getScript(std::string(name.data(), name.length()));
}

ScriptRunner* getScript(const std::string& name) {
    auto it = g_scripts.find(name);
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

const ScriptTable& scripts() {
    return g_scripts;
}

// ScriptRunner public api /////////////////////////////////////////////////////

ScriptRunner::ScriptRunner(std::unique_ptr<data::Script>&& script)
    : m_script{std::move(script)}
{
    LOG("Creating script runner");

    for (const auto& assert : m_script->asserts()) {
        m_pendingAsserts.emplace_back(assert);
    }

    LOG("Created " << m_pendingAsserts.size() << " assert trackers.");
    for ([[maybe_unused]] const auto& tracker : m_pendingAsserts) {
        LOG("Waiting for: " << tracker.leftHandSide().expression().string() << " and " << tracker.rightHandSide().expression().string());
    }

    // calculate full recset and cache it for later
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
                allRanks.emplace(std::make_pair(di, std::move(bitset)));
            } else {
                // update
                *it->second |= *bitset;
            }
        }
    }

    // debug print
    LOG("ranks involved in script " << name() << ":");
    for ([[maybe_unused]] const auto& [di, bitset] : allRanks) {
        LOG(di->d_handle << ": " << bitset->toSetString());
    }

    m_ranks = std::make_unique<Ranks>(std::move(allRanks));
}

const std::string& ScriptRunner::name() const {
    return m_script->name();
}

std::string ScriptRunner::string() const {
    return m_script->string();
}

std::string ScriptRunner::string(size_t lines) const {
    return m_script->string(lines);
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

    if (Client.XML_mode) {
        auto [passes, warns, fails] = getResultTotals();
        PrintOutMsg(XML_RESULT, m_script->name().c_str(), 0lu, passes, warns, fails);
    }
    
    PrintOutMsg("Assertion script $%s stopped.\n", name().c_str());
    return success;
}

void ScriptRunner::reset() {
    m_readyFlag = false;
    // create map to keep track of which ranks are stopped or exited
    m_ranks->reset();
    for (auto& assert : m_pendingAsserts) {
        assert.reset();
    }
}

bool ScriptRunner::sendContinueHelper(rec_set_t *recSet) {
    if (!recSet) return false;

    // only send continue to ranks that are stopped.

    // copy the list of receivers as intersection operations below can shrink recSet's internal list
    auto receivers = recSet->getReceivers();

    for (const auto* rec : receivers) {
        if (!rec) continue;
        LOG("Sending continue to " << rec->rec_di->d_handle << ": " << rec->rec_group->toSetString());
        if (const auto* blockedRanks = m_ranks->blocked(*rec->rec_di)) {
            group_recv_t tmpRec {
                .rec_di = rec->rec_di,
                .rec_group = blockedRanks->clone()
            };
            recSet->intersectReceiver(&tmpRec);
        }
    }

    // returns nullptr on success
    if (auto* repSet = DbgGo(recSet, true); repSet) {
        LOG("go error");
        FreeRepSet(repSet);
        return false;
    }

    // if there were any asserts that were waiting on part of the location in
    // the released ranks, they need to start waiting all over again
    for (auto& pendingAssert : m_pendingAsserts) {
        for (const auto* group : recSet->getReceivers()) {
            pendingAssert.unnotify(*group->rec_di, *group->rec_group);
        }
    }
    
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

    if (Client.XML_mode) {
        for (const auto& [di, ranks] : m_ranks->ranks()) {
            std::string setStr = di->d_handle + ranks->toSetString();
            XMLPrintStartGraphMsg(di->d_handle, setStr.c_str());
        }
    }

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

bool ScriptRunner::finished() const {
    // all ranks must be stopped so we can send our cleanup 
    // (breakpoints, print elements) commands
    if (!m_ranks->allRanksBlocked()) return false;

    if (m_manualFinishFlag) return true;
    
    for (const auto& assert : m_pendingAsserts) {
        if (assert.isFinished() == false) {
            return false;
        }
    }

    return true;
}

bool ScriptRunner::allRanksBlocked() const {
    return m_ranks->allRanksBlocked();
}

bool ScriptRunner::hasReadyAsserts() const {
    return m_readyFlag;
}

bool ScriptRunner::evaluateReadyAsserts() {
    for (auto& assert : m_pendingAsserts) {
        if (assert.isReady()) {
            // XXX: EvalCompare waits until it's substantially lost in the
            // expr.cpp/gram.cpp logic to bother looking at the Client's epsilon values,
            // so we have to set/unset here in order to support per-assertion epsilons

            // keep track of old values
            auto oldLow = Client.cmp_lower_eps;
            auto oldHigh = Client.cmp_upper_eps;

            Client.cmp_lower_eps = assert.settings().error.first;
            Client.cmp_upper_eps = assert.settings().error.second;

            // returned by EvalCompare below
            val_t* rawResult = nullptr;
            val_t* rawDifference = nullptr;

            OnScopeExit cleanupPtrs = [&](){
                DestroyVal(rawResult);     // nullptr is fine
                DestroyVal(rawDifference); // nullptr is fine

                // restore error settings
                Client.cmp_lower_eps = oldLow;
                Client.cmp_upper_eps = oldHigh;
            };

            if (assert.settings().verboseEval) {
                PrintOutMsg("Evaluating assertion %lu: %s\n", assert.id(), assert.string().c_str());
            }

            // Print elements affects what data gets sent back and thus affects
            // compare results. ClientSetPrintElements only does work if the value is
            // different from the current setting, so it's ok to call it every iteration
            if (Client.print_elements != 0) m_oldPrintElements = Client.print_elements;
            ClientSetPrintElements(0, &m_fullRecSet);
            
            auto recSet = assert.createRecSet();
            auto expr = assert.expressionHandle();

            if (!expr || EvalCompare(&rawResult, &rawDifference, expr->raw(), recSet.get()) < 0 || !rawResult) {
                PrintOutError("Error in evaluation of \"%s\".\n", assert.string().c_str());
                PrintError();
                uninstall();
                return false;
            }

            // wrap in smart pointers
            using ValPtr = std::unique_ptr<val_t, decltype(&DestroyVal)>;
            auto result = ValPtr(rawResult, DestroyVal); 
            auto difference = ValPtr(rawDifference, DestroyVal);
            rawResult = nullptr;
            rawDifference = nullptr;

            if (assert.settings().verboseEval) {
                printCompareResult(result.get(), difference.get(), *recSet);
            }

            auto simpleResult = simpleCompareResult(*result);
            assert.recordResult(std::move(result), std::move(difference));

            if (Client.XML_mode) {
                auto [passes, warns, fails] = assert.getResultTotals();
                PrintOutLine(XML_RESULT, name().c_str(), assert.id(), passes, warns, fails);
            }

            if (assert.settings().stopOnFailure && simpleResult == CompareResult::Fail) {
                PrintOutMsg("Assertion %lu: %s failed. Stopping script.\n", assert.id(), assert.string().c_str());
                m_manualFinishFlag = true;
                break;
            }
        }
    }

    return true;
}

bool ScriptRunner::releaseReadyAsserts() {
    for (auto& assert : m_pendingAsserts) {
        if (assert.isReady()) {
            assert.reset();
            if (!sendContinue(assert)) {
                return false;
            }
        }
    }

    m_readyFlag = false;

    return true;
}

const SR::Assertion* ScriptRunner::getAssert(size_t id) const {
    auto it = std::find_if(m_pendingAsserts.begin(), m_pendingAsserts.end(), [id](const auto& a) {
        return a.id() == id;
    });
    return (it != m_pendingAsserts.end()) ? &(*it) : nullptr;
}

const std::vector<std::pair<const SR::Assertion*, const SR::Result*>> ScriptRunner::getAllDiffs() const {
    std::vector<std::pair<const SR::Assertion*, const SR::Result*>> r;

    // build list
    for (const auto& assert : asserts()) {
        for (const auto& result : assert.results()) {
            auto simpleResult = simpleCompareResult(*result.result);
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

std::string ScriptRunner::summary() const {
    std::ostringstream ss;
    ss << "Results summary for script " << "$" << m_script->name();

    if (auto settings = m_script->settingsString(); settings != "") {
       ss << ", " << settings;
    }
    ss << ":\n\n";

    // totals
    size_t pass = 0;
    size_t warn = 0;
    size_t fail = 0;
    size_t indet = 0;
    for (const auto& assert : m_pendingAsserts) {
        for (const auto& result : assert.results()) {
            switch (simpleCompareResult(*result.result)) {
            case CompareResult::Pass: pass++; break;
            case CompareResult::Warn: warn++; break;
            case CompareResult::Fail: fail++; break;            
            case CompareResult::Unknown: indet++; break;
            }
        }
    }

    ss << "Total assertion evaluations: " << pass + warn + fail + indet << "\n";
    ss << "Passed: " << pass 
       << ", Passed with warning: " << warn 
       << ", Failed: " << fail 
       << ", Indeterminate: " << indet << "\n"
    ;

    // individual assertion results
    ss << "Assertion results:\n\n";
    for (const auto& assert : m_pendingAsserts) {
        ss << "Assertion " << assert.id();
        if (auto settings = assert.settingsString(); settings != "") {
            ss << ", " << settings; 
        }
        ss << ":\n";
        ss << assert.string() << "\n"
           << assert.summary() << "\n\n"
        ;
    }

    return ss.str();
}

// ScriptRunner event handler api //////////////////////////////////////////////

SR::EventInterface ScriptRunner::getEventInterface() {
    return EventInterface(*this);
}

void ScriptRunner::handleBPHit(event_set& eventSet, const Bitset& replySet, const dbgbp_t& dbgBp) {
    if (!eventSet.e_ev || !eventSet.e_di) {
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
            if (assert.BPIsRelevant(*eventSet.e_di, replySet, dbgBp)) {
                relevantBreakpoint = true;
            }
            // notify after checking relevance, as notify will reduce
            // the set of ranks that BPIsRelevant is checking for
            auto ready = assert.notify(*eventSet.e_di, replySet, dbgBp);
            m_readyFlag = std::max(ready, m_readyFlag);
        }
        if (!relevantBreakpoint) {
            // we've hit a breakpoint which no assertion cares about. if this
            // breakpoint is an "interruption", we stop the script. but if
            // all the assertions that needed the stopped ranks are already
            // finished, it's not really an interruption and we can let it slide.
            bool someoneStillNeededIt = false;
            for (const auto& assert : m_pendingAsserts) {
                if (!assert.isFinished() && assert.procsetIsRelevant(*eventSet.e_di, replySet)) {
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
        m_ranks->markRanksBlocked(*eventSet.e_di, replySet);
    }
}

void ScriptRunner::handleExit(event_set& eventSet, const Bitset& replySet) {
    if (!eventSet.e_ev || !eventSet.e_di) {
        // should never happen
        PrintOutError("Assertion script event handler received event set containing nullptrs");
        return;
    }

    // check that we're actually dealing with an exit event
    switch (eventSet.e_ev->ev_event) {
        case DBGEV_BPHIT: {
            if (eventSet.e_ev->ev_bp->bp_type != DBGBP_EXIT_TYPE) {
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
    m_ranks->markRanksBlocked(*eventSet.e_di, replySet);

    for ([[maybe_unused]] const auto& [di, ranks] : m_ranks->blockedRanks()) {
        LOG(di->d_handle << ": " << ranks->toSetString());
    }
}

int ScriptRunner::passEventToOldHandler(event_set& eventSet, const Bitset& replySet) {
    return m_oldEventHandlers.at(eventSet.e_di)(&eventSet, &replySet);
}

// ScriptRunner private helpers ////////////////////////////////////////////////

std::set<dbginvoke*, dbginvokeMapper> ScriptRunner::getDisFromScript() const {
    auto dis = std::set<dbginvoke*, dbginvokeMapper>{};
    
    const auto getDiFromExpression = [&](const data::Expression& e) {
        forEachOverlaySet(*e.procset(), [&](const auto* os) {
            if (os->s_di) dis.insert(os->s_di);
        });
    };
    
    for (const auto& assert : m_script->asserts()) {
        LOG("Extracting dis from " << assert.leftHandSide().string());
        getDiFromExpression(assert.leftHandSide());
        LOG("Extracting dis from " << assert.rightHandSide().string());
        getDiFromExpression(assert.rightHandSide());
    }

    // debug print
    for ([[maybe_unused]] const auto& di : dis) {
        LOG("Got dis:");
        LOG("- " << di->d_handle);
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

bool ScriptRunner::setEventHandlers() {
    // for each di, install new event handler and keep track of old one
    for (auto* di : getDisFromScript()) {
        m_oldEventHandlers.insert({di, di->d_clnt_ev_callback});
        di->d_clnt_ev_callback = [this](event_set* e, const Bitset* b) {
            return handleEvent(this->getEventInterface(), e, b);
        };
    }

    return true;
}

bool ScriptRunner::restoreOldEventHandlers() {
    for (auto* di : getDisFromScript()) {
        auto it = m_oldEventHandlers.find(di);
        if (it != m_oldEventHandlers.end()) {
            di->d_clnt_ev_callback = it->second;
            LOG("Restored old event handler for " << di->d_handle);
        } else {
            LOG("No old event handler found for " << di->d_handle);
        }
    }

    return true;
}

bool ScriptRunner::sendHalt() {
    LOG("Sending halt");
    auto* repSet = DbgInterrupt(&m_fullRecSet, true);

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
        PrintHaltedLocationSet(reply->rep_di->d_handle, reply->rep_group.get(), loc);
    }

    FreeRepSet(repSet);

    return true;
}

std::tuple<size_t, size_t, size_t> ScriptRunner::getResultTotals() const {
    size_t passes = 0;
    size_t warns = 0;
    size_t fails = 0;
    for (const auto& assert : m_pendingAsserts) {
        auto [aPasses, aWarns, aFails] = assert.getResultTotals();
        passes += aPasses;
        warns += aWarns;
        fails += aFails;
    }
    return {passes, warns, fails};
}

// ScriptRunner::Expression ////////////////////////////////////////////////////

SR::Expression::Expression(const data::Expression& expression)
    : m_expression{expression}
    , m_breakPointId{-1}
{
    // assemble overlay sets into a mapping from a di to a single bitset
    forEachOverlaySet(*m_expression.procset(), [&](const auto* os) {
        auto it = m_fullSets.find(os->s_di);
        if (it != m_fullSets.end()) {
            // in theory, procsets shouldn't contain multiple overlay sets for
            // the same di, but just in case...

            auto newBitset = sliceBitset(*os, &m_expression.procsetSlice());
            *it->second |= *newBitset;
        } else {
            // create new entry

            auto newBitset = sliceBitset(*os, &m_expression.procsetSlice());
            m_fullSets.emplace(std::make_pair(os->s_di, std::move(newBitset)));
        }
    });
}

bool SR::Expression::setBreakpoints() {
    const auto& e = expression();
    auto success = true;

    LOG("Setting breakpoint " << e.location().string());
    auto recSet = createRecSet();
    auto repSet = DbgSetLineBreak(recSet.get(), 0, e.location().filename().c_str(), 
            e.location().lineNumber(), recSet->getThreads());

    if (ProcessErrorInRepSet(repSet)) {
        LOG("Error setting breakpoint: " << e.location().string());
        FreeRepSet(repSet);
        return false;
    }

    for (const auto& reply : repSet) {
        const auto* dbgBp = EVENT_BP(reply->rep_event);
        const auto* bp = reply->rep_di->d_breakpoints.findByDbgBreakpoint(*dbgBp);

        if (!bp) {
            LOG("Error setting breakpoint: " << e.location().string());
            success = false;
            break;
        }

        if (m_breakPointId == -1) {
            m_breakPointId = bp->id();
        } else if (m_breakPointId != bp->id()) {
            LOG("Error setting breakpoint: got two logical ids");
            success = false;
            break;
        }
    }

    FreeRepSet(repSet);

    return success;
}

bool SR::Expression::deleteBreakpoints() {
    if (m_breakPointId == -1) return true;

    LOG("Deleting breakpoint " << m_breakPointId);
    
    auto recSet = createRecSet();

    // if another expression already deleted our breakpoint, we can stop early
    {   // make sure our assumption that our recSet only has one unique di is correct
        dbginvoke_t* seen = nullptr;
        for (const auto& rec : recSet.get()) {
            if (!seen) {
                seen = rec->rec_di;
            } else if (seen != rec->rec_di) {
                LOG("Found multiple dis, can't properly delete breakpoints");
                return false;
            }
        }
    }

    // short circuit if our breakpoint is already gone
    if (recSet->getReceivers().size() == 0) return true;
    if (!recSet->getReceivers().front()->rec_di->d_breakpoints.findByLogicalID(m_breakPointId)) {
        LOG("Breakpoint " << m_breakPointId << " already deleted.");
        m_breakPointId = -1;
        return true;
    }
    
    // returns nullptr on success
    auto repSet = DbgDeleteBreak(recSet.get(), m_breakPointId);

    if (repSet) {
        ProcessErrorInRepSet(repSet);
        LOG("Error deleting breakpoint: " << m_breakPointId);
        FreeRepSet(repSet);
        return false;
    }

    LOG("Deleted breakpoint " << m_breakPointId);
    m_breakPointId = -1;
    FreeRepSet(repSet);

    return true;
}

bool SR::Expression::notify(const dbginvoke& di, const Bitset &bitset, const dbgbp_t& dbgBp) {
    LOG(m_expression.string() << " notified of " << di.d_handle << ": " << bitset.toSetString() << " at backend breakpoint " << dbgBp.bp_id);

    if (BPIsRelevant(di, bitset, dbgBp)) {
        LOG("id match: " << BPIsRelevant(di, bitset, dbgBp)->id());
        LOG("bitset before " << m_waitingFor.at(&di)->toSetString());
        *m_waitingFor.at(&di) -= bitset;
        LOG("bitset (" << (void*)m_waitingFor.at(&di).get() << ") now " << m_waitingFor.at(&di)->toSetString());
    } else {
        LOG("id mismatch");
    }

    return isReady();
}

void SR::Expression::unnotify(const dbginvoke& di, const Bitset &bitset) {
    if (auto it = m_waitingFor.find(&di); it != m_waitingFor.end()) {
        // we are now waiting for these ranks again
        (*it->second |= bitset) &= *m_fullSets.at(&di);
    }   
}

bool SR::Expression::isReady() const {
    // return true when all bitsets are empty
    auto foundNotReady = std::any_of(m_waitingFor.begin(), m_waitingFor.end(), [](const auto& pair){
        return pair.second->numSet() > 0;
    });
    LOG("ready? " << !foundNotReady);
    return !foundNotReady;
}

const MappedBreakpoint* SR::Expression::BPIsRelevant(const dbginvoke& di, const Bitset& bitset, const dbgbp_t& dbgBp) const {
    auto it = m_waitingFor.find(&di);
    if (it == m_waitingFor.end()) {
        // irrelevant di
        return nullptr;
    }

    if (!it->second->hasCommonBits(bitset)) {
        // irrelevant bitset
        return nullptr;
    }

    const auto* mappedBP = di.d_breakpoints.findByDbgBreakpoint(dbgBp);
    if (!mappedBP || mappedBP->id() != m_breakPointId) {
        // irrelevant BP
        return nullptr;
    }

    return mappedBP;
}

bool SR::Expression::procsetIsRelevant(const dbginvoke& di, const Bitset& bitset) const {
    auto it = m_waitingFor.find(&di);
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

void SR::Expression::reset() {
    LOG("resetting expression tracker");

    m_waitingFor.clear();
    for (const auto& [di, bitset] : m_fullSets) {
        m_waitingFor.emplace(std::make_pair(di, bitset->clone()));
    }

    LOG("wait mapping reset:");
    for ([[maybe_unused]] const auto& [di, bitset] : m_waitingFor) {
        LOG("di " << di->d_handle << " waiting for " << bitset->toSetString());
    }
}

std::unique_ptr<rec_set_t> SR::Expression::createRecSet() const {
    // an expression's recset is based on the expression text and thus never changes
    
    if (!m_cachedRecSet) {
        LOG("creating recset for " << string());
        if (expression().procsetSlice().getRank() == 0) {
            LOG("no slice, returning CreateRecSetForProcset");
            return std::unique_ptr<rec_set_t>(CreateRecSetForProcset(expression().procset()));
        }

        LOG("slice");
        // slice up the procset
        m_cachedRecSet = std::make_unique<rec_set_t>();

        forEachOverlaySet(*expression().procset(), [&](const auto* os){
            auto bs = sliceBitset(*os, &expression().procsetSlice());
            if (!bs->isEmpty()) {
                LOG("adding receiver " << bs->toSetString());
                m_cachedRecSet->addReceiver(NewGroupReceiver(os->s_di, std::move(bs)));
            }
        });
    }

    return m_cachedRecSet->clone();
}

bool SR::Result::operator==(const Result& other) const {
    if (simpleCompareResult(*result) != simpleCompareResult(*other.result)) {
        return false;
    }

    const auto& Pass = CompareResult::Pass;
    const auto& Unknown = CompareResult::Unknown;

    // both results known to be the same
    if (simpleCompareResult(*result) == Pass || simpleCompareResult(*result) == Unknown) {
        return true;
    }

    if (RetrieveContents(difference.get(), recSet.get())) return false;
    if (RetrieveContents(other.difference.get(), other.recSet.get())) return false;

    // results are warn/fail; compare the two results' diffs
    if (RVAL_TYPE(difference) != RVAL_TYPE(other.difference)) {
        return false;
    }

    switch (difference->val_rval_type) {
        case rval_aif: {
            auto* aisL = difference->rval_u.rval_aif;
            auto* aisR = other.difference->rval_u.rval_aif;
            return AIFStream::compareEqual(*aisL, *aisR);
        }

        case rval_reply: {
            auto* repSetL = difference->rval_u.rval_reply;
            auto* repSetR = other.difference->rval_u.rval_reply;

            if (repSetL->getNumReplies() != repSetR->getNumReplies()) {
                return false;
            }

            for (size_t i = 0; i < repSetL->getNumReplies(); i++) {
                auto* repL = repSetL->getReplies()[i];
                auto* repR = repSetR->getReplies()[i];

                if (repL->rep_di != repR->rep_di) return false;
                if (*repL->rep_group != *repR->rep_group) return false;

                auto* aisL = repL->rep_event->ev_u.ev_u_data;
                auto* aisR = repR->rep_event->ev_u.ev_u_data;
                if (!AIFStream::compareEqual(*aisL, *aisR) != 0) return false;
            }

            return true;
        }

        default:
            return false;
    }

}

SR::Assertion::Assertion(const data::Assertion& assert)
    : m_assert{assert}
    , m_lhs(assert.leftHandSide())
    , m_op{assert.op()}
    , m_rhs(assert.rightHandSide())
    , m_passes{0}
    , m_warns{0}
    , m_fails{0}
{}

bool SR::Assertion::setBreakpoints() {
    return m_lhs.setBreakpoints() && m_rhs.setBreakpoints();
}

bool SR::Assertion::deleteBreakpoints() {
    return m_lhs.deleteBreakpoints() && m_rhs.deleteBreakpoints();
}

bool SR::Assertion::notify(const dbginvoke& di, const Bitset &bitset, const dbgbp_t& dbgBp) {
    auto lhsReady = m_lhs.notify(di, bitset, dbgBp);
    auto rhsReady = m_rhs.notify(di, bitset, dbgBp);
    return lhsReady && rhsReady;
}

void SR::Assertion::unnotify(const dbginvoke& di, const Bitset &bitset) {
    m_lhs.unnotify(di, bitset);
    m_rhs.unnotify(di, bitset);
}

bool SR::Assertion::isReady() const {
    return m_lhs.isReady() && m_rhs.isReady();
}

bool SR::Assertion::isFinished() const {
    LOG(string() << ": " << (m_results.size() != 0));
    return m_results.size() != 0;
}

bool SR::Assertion::BPIsRelevant(const dbginvoke& di, const Bitset& bitset, const dbgbp_t& dbgBp) const {
    return m_lhs.BPIsRelevant(di, bitset, dbgBp) || m_rhs.BPIsRelevant(di, bitset, dbgBp);
}

bool SR::Assertion::procsetIsRelevant(const dbginvoke& di, const Bitset& bitset) const {
    return m_lhs.procsetIsRelevant(di, bitset) || m_rhs.procsetIsRelevant(di, bitset);
}

void SR::Assertion::reset() {
    m_lhs.reset();
    m_rhs.reset();
}

std::map<const dbginvoke*, std::unique_ptr<Bitset>, dbginvokeMapper>
SR::Assertion::ranksMap() const {
    auto r = std::map<const dbginvoke*, std::unique_ptr<Bitset>, dbginvokeMapper>{};
    
    // initialize with lhs
    for (const auto& [di, bitset] : m_lhs.ranksMap()) {
        r.emplace(std::make_pair(di, bitset->clone()));
    }

    // merge rhs
    for (const auto& [di, bitset] : m_rhs.ranksMap()) {
        auto it = r.find(di);
        if (it == r.end()) {
            // new entry
            r.emplace(std::make_pair(di, bitset->clone()));
        } else {
            // existing entry
            *it->second |= *bitset;
        }
    }

    return r;
}

std::string SR::Assertion::summary() const {
    std::ostringstream ss;

    size_t passes = 0;
    size_t warns = 0;
    size_t failures = 0;

    for (const auto& res : m_results) {
        switch (simpleCompareResult(*res.result)) {
            case CompareResult::Pass: passes++; break;
            case CompareResult::Warn: warns++; break;
            case CompareResult::Fail: failures++; break;
            case CompareResult::Unknown: break; // we don't show these in the summary
        }
    }

    ss << "Passes: " << passes
       << ", Passes with warnings: " << warns
       << ", Failures: " << failures << ".";

    return ss.str();
}

void SR::Assertion::recordResult(ValPtr&& res, ValPtr&& diff) {
    if (!res) return;
    LOG("Recording assert");

    // keep track of running totals
    switch (simpleCompareResult(*res)) {
        case CompareResult::Pass: m_passes++; break;
        case CompareResult::Warn: m_warns++; break;
        case CompareResult::Fail: m_fails++; break;
        case CompareResult::Unknown: break; // ignore
    };

    m_results.emplace_back(Result {
        .recSet = createRecSet(),
        .result = std::move(res),
        .difference = std::move(diff)
    });
}

std::tuple<size_t, size_t, size_t> SR::Assertion::getResultTotals() const {
    return {m_passes, m_warns, m_fails};
}

const std::vector<SR::Result>& SR::Assertion::results() const {
    return m_results;
}

std::vector<std::pair<const SR::Result*, size_t>> SR::Assertion::getCondensedDiffs() const {
    std::vector<std::pair<const SR::Result*, size_t>> stats;

    for (const auto& result : m_results) {
        const auto& Pass = CompareResult::Pass;
        const auto& Unkn = CompareResult::Unknown;

        if (simpleCompareResult(*result.result) != Pass && simpleCompareResult(*result.result) != Unkn) {
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

    return stats;
}

std::unique_ptr<ExpressionHandle> SR::Assertion::expressionHandle() const {
    // XXX: probably can construct an expr_t in place rather than re-parse...
    std::ostringstream ss;

    ss << m_lhs.evalString() << " "
       << m_op.string() << " "
       << m_rhs.evalString()
    ;

    LOG("exprString: " << ss.str());
    try {
        return std::make_unique<ExpressionHandle>(ss.str());
    } catch (const ExpressionHandle::ParseError& e) {
        // should never happen
        PrintOutError("Error parsing assertion: %s\n", ss.str().c_str());
        PrintOutError("%s\n", e.what());
        return nullptr;
    }
}

std::unique_ptr<rec_set_t> SR::Assertion::createRecSet() const {
    // an assertion's recset is based on the assertion text and thus never changes
    if (!m_cachedRecSet) {
        m_cachedRecSet = m_lhs.createRecSet();
        m_cachedRecSet->merge(m_rhs.createRecSet());
    }

    return m_cachedRecSet->clone();
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
    for (const auto& [di, bitset] : m_involvedRanks) {
        auto tmpGroup = new group_recv_t{
            .rec_di = const_cast<dbginvoke*>(di),
            .rec_group = bitset->clone()
        };
        // takes ownership of tmpGroup
        tmpRecSet.addReceiver(tmpGroup);
    }

    auto repSet = DbgStatus(&tmpRecSet, 0);

    if (ProcessErrorInRepSet(repSet)) {
        // fall back to educated guess
        ProcessErrorInRepSet(repSet);
        PrintOutError("Warning: couldn't detect all ranks' statuses.\n");
        PrintOutError("Assertion script will assume everything is stopped to start.\n");
        for (const auto& [di, bitset] : m_involvedRanks) {
            m_blockedRanks.emplace(std::make_pair(di, bitset->clone()));
        }
    } else {
        for (const auto& reply : repSet) {
            if (reply->rep_event->ev_event == DBGEV_STAT) {
                LOG("stat reply: " << reply->rep_di->d_handle << ": " << reply->rep_group->toSetString() << " " << reply->rep_event->ev_u.ev_u_status->stat);
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

    FreeRepSet(repSet);

    LOG("Reset blocked ranks map:");
    for ([[maybe_unused]] const auto& [di, ranks] : m_blockedRanks) {
        LOG(di->d_handle << ": " << ranks->toSetString());
    }
}

void SR::Ranks::markRanksBlocked(const dbginvoke& di, const Bitset& ranks) {
    if (auto it = m_blockedRanks.find(&di); it != m_blockedRanks.end()) {
        (*it->second |= ranks) &= *m_involvedRanks.at(&di);
    }
    LOG("Blocked ranks:");
    for ([[maybe_unused]] const auto& [di, bitset] : m_blockedRanks) {
        LOG(di->d_handle << ": " << bitset->toSetString());
    }
}

void SR::Ranks::markRanksUnblocked(const dbginvoke& di, const Bitset& ranks) {
    if (auto it = m_blockedRanks.find(&di); it != m_blockedRanks.end()) {
        *it->second -= ranks;
    }
    LOG("Blocked ranks:");
    for ([[maybe_unused]] const auto& [di, bitset] : m_blockedRanks) {
        LOG(di->d_handle << ": " << bitset->toSetString());
    }
}

void SR::Ranks::markRanksUnblocked(const Assertion& assert) {
    for (const auto& [di, bitset] : assert.ranksMap()) {
        markRanksUnblocked(*di, *bitset);
    }
    LOG("Blocked ranks:");
    for ([[maybe_unused]] const auto& [di, bitset] : m_blockedRanks) {
        LOG(di->d_handle << ": " << bitset->toSetString());
    }
}

void SR::Ranks::markAllRanksBlocked() {
    for (auto& [di, bitset] : m_blockedRanks) {
        *bitset = *m_involvedRanks.at(di);
    }
    LOG("Blocked ranks:");
    for ([[maybe_unused]] const auto& [di, bitset] : m_blockedRanks) {
        LOG(di->d_handle << ": " << bitset->toSetString());
    }
}

void SR::Ranks::markAllRanksUnblocked() {
    for (auto& [di, bitset] : m_blockedRanks) {
        bitset->clear();
    }
    LOG("Blocked ranks:");
    for ([[maybe_unused]] const auto& [di, bitset] : m_blockedRanks) {
        LOG(di->d_handle << ": " << bitset->toSetString());
    }
}

bool SR::Ranks::allRanksBlocked() const {
    LOG("checking for full block...");
    for (const auto& [di, blockedBitset] : m_blockedRanks) {
        LOG("di: " << di->d_handle << " " << blockedBitset->toSetString());
        if (*blockedBitset != *m_involvedRanks.at(di)) {
            // found a case where not all ranks are blocked
            LOG(di->d_handle << ": " << blockedBitset->toSetString() << " != " << m_involvedRanks.at(di)->toSetString());
            return false;
        }        
    }

    LOG("full block.");
    return true;
}

Bitset* SR::Ranks::blocked(const dbginvoke& di) {
    auto it = m_blockedRanks.find(&di);
    if (it == m_blockedRanks.end()) {
        return nullptr;
    }
    return it->second.get();
}

const Bitset* SR::Ranks::blocked(const dbginvoke& di) const {
    // blocked() is const if the returned ptr is cast to const
    return const_cast<Ranks*>(this)->blocked(di);
}

namespace {

int handleEvent(ScriptRunner::EventInterface scei, event_set* eventSet, const Bitset* replySet) {
    if (!eventSet || !eventSet->e_ev || !replySet) {
        LOG("Called with nullptr (" << eventSet << ") (" << eventSet->e_ev << ") (" << replySet << ")");
        return -1;
    }
    LOG("Event! di: " << eventSet->e_di->d_handle << " eventSet: " << eventSet->e_group->toSetString() << " replySet: " << replySet->toSetString());

    switch (eventSet->e_ev->ev_event) {
        case DBGEV_EXIT: {
            LOG("DBGEV_EXIT");
            scei.handleExit(*eventSet, *replySet);
            return scei.passEventToOldHandler(*eventSet, *replySet);
        }

        case DBGEV_BPHIT: {
            LOG("DBGEV_BPHIT");
            const auto* dbgBp = eventSet->e_ev->ev_bp;
            if (!dbgBp) {
                // should never happen
                LOG("null dbgBP");
                return -1;
            }

            scei.handleBPHit(*eventSet, *replySet, *dbgBp);
            return scei.passEventToOldHandler(*eventSet, *replySet);
        }

        default: {
            // includes DBGEV_ERROR case
            LOG("default");
            return scei.passEventToOldHandler(*eventSet, *replySet);
        };
    }

    // should never get here
    return -1;
}

} // anonymous namespace

} // namespace script
