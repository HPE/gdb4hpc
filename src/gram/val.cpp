/*******************************************************************************
 * Copyright 2022-2023 Hewlett Packard Enterprise Development LP.
 ******************************************************************************/

#include "eval.h"
#include "gram.h"
#include <iostream>
#include <cstring>
#include <unistd.h>

// remove these when I pull new_print_cmd
#include "args.h"
#include "cli_funcs.h"
#include "client_settings.h"
#include "dbg_client.h"
#include "../gram/procset.h"
#include "../gram/slice.h"
#include "../gram/blockmap.h"

// clean these up
struct dbgprocset_t;
typedef struct dbgprocset_t dbgprocset_t;
extern dbgprocset_t *FindProcSet(const char *name);
extern dbgprocset_t *CreateTempProcSetSlice(dbgprocset_t *ps, slice_t *ps_slice);
extern void FreeProcSet(dbgprocset_t *s);
extern rec_set_t *CreateRecSetForProcset(const dbgprocset_t *ps);
extern rep_set_t *DbgEvalExpr(rec_set_t *set, const char *exp);
extern void SetOverlaySetForProcSet(dbgprocset_t *s);
extern dbg_over_set_t *GetNextOverlaySetForProcSet(dbgprocset_t *s);
struct g_block_map_t;
typedef struct g_block_map_t g_block_map_t;
extern g_block_map_t *FindBlockMap(const std::string& blockmap);

std::map<std::string, rec_set_t*> IdMap::m_psetMap;

std::pair<dbgprocset_t*,bool> IdMap::getProcset(const std::string& procs) {
    auto brace = procs.find('{',1);
    auto name = std::string{};
    auto slice = std::unique_ptr<slice_t>{};

    if (brace == std::string::npos) {
        name = procs.substr(1);
    } else {
        name = procs.substr(1,brace-1);
        auto endBrace = procs.find('}', brace+1);
        auto sliceArg = procs.substr(brace+1, endBrace-brace-1);
        auto threads = std::string{};


        // pull out the the thread info
        if (auto colon=sliceArg.find(':'); colon != std::string::npos) {
            threads = sliceArg.substr(colon+1);
            sliceArg = sliceArg.substr(0,colon);
        }

        // translate to slice format
        for (size_t i=0; (i=sliceArg.find("..")) != std::string::npos; ++i) {
            sliceArg.replace(i,2,":");
        }
        slice.reset(new slice_t);
        slice->setArgs(sliceArg.c_str());
        if (!threads.empty()) slice->setThreads(threads.c_str());
    }

    auto isTmp = false;
    auto procSet = FindProcSet(name.c_str());

    if (procSet && slice) {
        // check the result
        int n = procSet->s_nproc;
        for (int i=0, r=slice->getRank(); i<r; ++i) {
            auto low = slice->getLow(i), high = slice->getHigh(i);
            if (low < 0 || low > high || (high != INT_MAX && high >= n)) { // INT_MAX is "*"
                throw EvalError("ranks are out of range in " + procs);
            }
        }
        // lookat: how is the memory for this handled?
        procSet = CreateTempProcSetSlice(procSet, slice.get());
        isTmp = true;
    }

    return std::make_pair(procSet, isTmp);

}

g_block_map_t* IdMap::getBlockmapForName(std::string name) {
    return FindBlockMap(name);
}

// verify the at debug variable $name is not already in use.
// Print a warning if printMsg is true.
bool IdMap::idIsAvailable(const std::string& name, bool printMsg) {
    const char* existingType = nullptr;
    if (FindProcSet(name.c_str())) {
        existingType = "procset";
    } else if (FindBlockMap(name)) {
        existingType = "decomposition";
    } else {
        return true;
    }

    if (printMsg) {
        PrintOutError("%s is already defined as a %s.\n", name.c_str(), existingType);
    }

    return false;
}

Value::Value(rec_set_t* rec_set) : m_rec_set(rec_set) {}

Value::~Value() {}

std::string Value::getExpr() const {
    throw EvalError("type is not a program value");
}

void Value::print(std::ostream& os, rec_set_t* rec_set, const std::string& format) {
    os << "Value::print is undefined\n";
}

void Value::addMetaInfo(const std::string& metaInfo) {}

std::pair<size_t, bool> Value::getNumResults() {
    throw EvalError("Type does not support operations");
}
std::optional<ResultPair> Value::getNextSet(bool rankToRank) {
    throw EvalError("Type does not support operations");
}

void Value::getCommonRanks(ResultPair& ranks) {
    throw EvalError("Type does not support operations");
}
void Value::getCommonSet(ResultPair& ranks) {
    throw EvalError("Type does not support operations");
}


ReplyValue::ReplyValue(rec_set_t* rec_set, const std::string& expr) : Value(rec_set), m_expr(expr) {
    if (!rec_set) {
        throw std::runtime_error("cannot evaluate " + expr + " without a process");
    }
}

ReplyValue::~ReplyValue() {
    for ( auto& [di, grp, aif] : m_replies)
        delete aif;
}

void ReplyValue::addMetaInfo(const std::string& metaInfo) {
    m_expr = metaInfo + m_expr;
}

rep_set_t* ReplyValue::retrieve() const
{
    auto rep_set = DbgEvalExpr(m_rec_set, m_expr.c_str());
    if (!rep_set || rep_set->isEmpty()) {
        FreeRepSet(rep_set);
        //SetError("Could not retrieve contents of val.");
        throw EvalError("Could not retrieve value of " + m_expr);
    } else if (ProcessErrorInRepSet(rep_set)) {
        FreeRepSet(rep_set);
        throw EvalErrorAlreadyReported();
    }

    return rep_set;
}

static std::string getLabel(rec_set_t* rec_set, group_reply* reply, const std::string& format) {
    auto threads = rec_set ? rec_set->getThreads() : nullptr;
    auto diName = reply->rep_di.c_str();

    auto label = GetProcSetString(diName, reply->rep_group.get(), threads);
    if (label.empty()) ;
    else if (format.find("csv") == std::string::npos) label += ": ";
    else label = "\"" + label + "\",";

    return label;
}
    

void ReplyValue::print(std::ostream& os, rec_set_t* rec_set, const std::string& format) {
    // ask the back-end for the values
    auto rep_set = retrieve();

    for ( auto reply : rep_set) {
        // retrieve shouldn't return anything that isn't data, but lets be safe..
        if (reply->rep_event->ev_event != DBGEV_DATA) continue;
        auto label = getLabel(rec_set, reply, format);
        auto aif = EVENT_DATA(reply->rep_event);
        aif->print(os, label, format);
        os << "\n";
    }

    FreeRepSet(rep_set);
}

std::pair<size_t, bool> ReplyValue::getNumResults() {
    auto rep_set = retrieve();
    m_numResults = 0;

    // assemble the information to map a reply into the bits
    // w.r.t the current set of results
    DbgInvoke* prevDi = nullptr;
    Bitset* diRanks = nullptr;
    for (auto reply : rep_set) {
        DbgInvoke* di = reply->rep_di.lock();

        // we are assuming that rep_set segregated by di
        if (di != prevDi) { diRanks = nullptr; prevDi = di; }

        // we want the ranks from this di that were involved in the
        // query, and the relative offset to overlay multiple DI's
        if (!diRanks) {
            auto grp = reply->rep_group->clone();
            diRanks = grp.get();
            m_ranksPerDI[di] = std::make_pair(std::move(grp),m_numResults);
        } else {
            *diRanks |= *reply->rep_group;
        }

        m_numResults += reply->rep_group->numSet();
    }

    // extract the result info from the rep_set before I throw it away
    for (auto reply : rep_set) {
        DbgInvoke* di = reply->rep_di.lock();
        AIFStream* aif = nullptr;

        // lookat: should check this is a data event
        std::swap(aif, EVENT_DATA(reply->rep_event));
        m_replies.push_back(std::make_tuple(di, toRankBits(di, *reply->rep_group), aif));
    }

    delete rep_set;
    return std::make_pair(m_numResults, m_replies.size());
}

// This function maps a group_reply onto a the bits of this specific value.   A rec_set 
// may contain multiple di's and/or may refer to only a subset of the ranks.  Thus
// if you did something like p $a{1,4..5} the returned bit set will be of size 3.
// bit 1 in replyGrp maps to bit 0 or the returned grp, bit 4 maps to bit 1, etc.
std::unique_ptr<Bitset> ReplyValue::toRankBits(DbgInvoke* di, const Bitset& replyGrp) {
    // get ranks in terms of the active ranks
    auto ranks = std::unique_ptr<Bitset>(new DenseBitset(m_numResults));
    auto& [ allRanks, offset] = m_ranksPerDI[di];
    auto locIdx = offset;
    auto it = allRanks->iterator();
    for (size_t i; (i=**it) != Bitset::None; ++*it, ++locIdx) {
        if (replyGrp.test(i)) ranks->set(locIdx);
    }
    return ranks;
}

// This is the inverse of toRankBits.   It takes the group related to the actual results
// and maps it back to the group defined by the given di.   And individual reply never
// straddles multiple di's so we can pass it in as a argument
std::unique_ptr<Bitset> ReplyValue::toReplyBits(DbgInvoke* di, const Bitset& resGrp) {
    auto& [ diRanks, offset ] = m_ranksPerDI[di];
    auto replyGrp = diRanks->clone();
    auto resIdx = offset;
    auto it = diRanks->iterator();
    for (size_t i; (i=**it) != Bitset::None; ++*it, ++resIdx) {
        if (!resGrp.test(resIdx)) replyGrp->unset(i);
    }

    return replyGrp;
}

// get the next reply when this is the first operand
std::optional<ResultPair> ReplyValue::getNextSet(bool rankToRank) {
    if (m_replies.empty()) return std::optional<ResultPair>{};

    // we set the ranks here, but delay setting the aifStream because
    // we don't yet know if it needs to be cloned or not
    auto& [ di, grp, aif ] = m_replies.front();
    ResultPair pr;
    pr.isRankToRank = rankToRank;
    if (rankToRank) pr.ranks = grp->clone();
    
    return pr;
}

// modify the result set as the second operand.   The first operand has
// set the pr.ranks, and we are looking for the first result that
// overlaps.
void ReplyValue::getCommonRanks(ResultPair& pr) {

    for (auto repIt = m_replies.begin(); repIt != m_replies.end(); ++repIt) {
        auto& [ di, repGrp, aif] = *repIt;
        if (pr.isRankToRank) {
            if (!repGrp->hasCommonBits(*pr.ranks)) continue; 

            // at least some ranks are shared.  Note ranks and repGrp are
            // w.r.t the current reply set not computational ranks
            auto origN = pr.ranks->numSet();

            // modify pr.ranks to have the shared set
            *pr.ranks &= *repGrp;
            *repGrp -= *pr.ranks;

            auto newN = pr.ranks->numSet();
            pr.ranksAreEq = (origN == newN);
        }

        if (!pr.isRankToRank || repGrp->isEmpty()) {
            // last result from this reply
            pr.b = aif;
            m_replies.erase(repIt);
            return;
        } else {
            // not every reply matched, it needs to stay around
            // for further matching
            pr.b = aif->clone();
            return;
        }
    }
    throw EvalError("Matching error for binary operation");
}

// final part of the handshake, we're back on the first operand.
// we've established the set of results that match (in result.ranks)
// and now we set result.a with our result and define result.label
void ReplyValue::getCommonSet(ResultPair& result) {
    auto& [ di, grp, aif ] = m_replies.front();
    
    if (!result.isRankToRank || result.ranksAreEq) {
        // The entire remainder of the first reply matches
        result.a = aif;
        m_replies.erase(m_replies.begin());
    } else {
        // not all of the reply matches
        *grp -= *result.ranks;
        result.a = aif->clone();
    }

    // at the moment, we're printing a label based on the
    // ranks in the first operand.   This could be changed
    if (result.isRankToRank) {
        auto replyGrp = toReplyBits(di, *result.ranks);
        result.label = replyGrp->toSetString() + ": ";
    }
}

DecompValue::DecompValue(rec_set_t* rec_set, g_block_map_t*  blockMap,
                         std::unique_ptr<Value>&& val)
    : SingleValue(rec_set), m_blockMap(blockMap), m_val(std::move(val))
{}

void DecompValue::addIndex(const std::string& idx) {
    m_idx += idx;
}

void DecompValue::print(std::ostream& os, rec_set_t* rec_set,
                        const std::string& format) {

    auto aif = getAIF();
    aif->print(os, "", format);
    os << "\n";
}

AIFStream* DecompValue::getAIF() {
    if (m_aif)
        return m_aif;

    auto val = dynamic_cast<ReplyValue*>(m_val.get());
    if (!val) {
        throw EvalError("Decomposition must refer to a progam value");
    }

    if (!m_idx.empty()) {
        auto os = std::ostringstream{};
        os << "[[|" << GetBlockMapDefinition(m_blockMap) << m_idx << "]]";
        val->addMetaInfo(os.str());
    }

    auto rep_set = val->retrieve();

    auto aif = construct_blockmap_data(m_blockMap, m_idx, val->getRecSet(), rep_set);
    return m_aif = aif;
}

DistributedBlockMapValue::DistributedBlockMapValue(rec_set_t* rec_set, g_block_map_t*  blockMap,
                                                   std::unique_ptr<Value>&& val)
    : ReplyValue(rec_set, val->getExpr()), m_blockMap(blockMap), m_val(std::move(val))
{}

rep_set_t* DistributedBlockMapValue::retrieve() const {
    auto val = dynamic_cast<ReplyValue*>(m_val.get());
    if (!val) {
        throw EvalError("Decomposition must refer to a progam value");
    }

    return val->retrieve();
}

void DistributedBlockMapValue::print(std::ostream& os, rec_set_t* rec_set,
                                     const std::string& format) {

    auto rep_set = retrieve();
    for ( auto reply : rep_set) {
        auto label = getLabel(m_rec_set, reply, format);
        auto aif = EVENT_DATA(reply->rep_event);

        os << label;
        PrintWithBlockMap(aif, m_blockMap, os);
        os << "\n";
    }

    FreeRepSet(rep_set);
}


AIFValue::AIFValue(rec_set_t* rec_set, const std::string& expr, AIFStream* aif)
    : SingleValue(rec_set), m_expr(expr) {
    m_aif = aif;
}

AIFStream* AIFValue::getAIF() {
    return m_aif;
}

void AIFValue::print(std::ostream& os, rec_set_t* rec_set, const std::string& format) {
    m_aif->print(os, "", format);
    os << "\n";
}

std::string AIFValue::getExpr() const { return m_expr; }

SingleValue::SingleValue(rec_set_t* rec_set) : Value(rec_set) {}
SingleValue::~SingleValue() { delete m_aif; }

std::pair<size_t, bool> SingleValue::getNumResults() { return std::make_pair(1,true); }
std::optional<ResultPair> SingleValue::getNextSet(bool rankToRank) {
    if (m_processed) return std::optional<ResultPair>{};
    m_processed = true;
    // the rankToRank argument is the *intent* to be rank-to-rank, but a single value
    // cannot be rank to rank.
    return ResultPair{.isRankToRank=false};
}
void SingleValue::getCommonRanks(ResultPair& result) {
    // as a second argument, match everyone
    result.ranksAreEq = true;
    //result.b = m_aif->clone();  // clone does not work for decomp -- revisit
    result.b = getAIF();
    m_aif = nullptr;
}
void SingleValue::getCommonSet(ResultPair& result) {
    //result.a = m_aif->clone();  // revisit
    result.a = getAIF();
    m_aif = nullptr;

    // lookat: I should be able to get a label for the second operand
    // if a ReplyValue
    result.label = "";
}


ProcsetValue::ProcsetValue(rec_set_t* rec_set, dbgprocset_t* tmpPs, const std::string& threads)
    : Value(rec_set), m_tmpProcset(tmpPs), m_threads(threads)
{
    if (!rec_set) {
        throw std::runtime_error("null rec_set");
    }
}

ProcsetValue::~ProcsetValue() {
    FreeProcSet(m_tmpProcset);
}

void ProcsetValue::print(std::ostream& os, rec_set_t* rec_set, const std::string& format) {
    os << m_rec_set << "\n";
}

std::pair<DbgInvoke::Handle,std::unique_ptr<Bitset>>
ProcsetValue::getProcSet() {

    auto rcvs = m_rec_set->getReceivers();
    if (rcvs.size() != 1) {
        throw EvalError("Procset must be on a single app launch.");
    }
    auto rcv = rcvs[0];
    return std::make_pair(rcv->rec_di.handle(), rcv->rec_group->clone());
}

std::string ProcsetValue::getThreads() const {
    return m_threads;
}

BlockmapValue::BlockmapValue(rec_set_t* rec_set, const std::string& name)
    : Value(rec_set), m_name(name) {}

void BlockmapValue::print(std::ostream& os, rec_set_t* rec_set, const std::string& format) {
}

g_block_map_t* BlockmapValue::getBlockMap() const { return FindBlockMap(m_name); }

BinaryFuncValue::BinaryFuncValue(std::unique_ptr<Value>&& a,
                                 const std::string& op,
                                 std::unique_ptr<Value>&& b) :
    Value(a->getRecSet()), m_a(std::move(a)), m_op(op), m_b(std::move(b))
{}

void BinaryFuncValue::print(std::ostream& os, rec_set_t* rec_set, const std::string& format) {
    // check if the values can be matched up.
    auto [ NA, uniqueNA ] = m_a->getNumResults();
    auto [ NB, uniqueNB ] = m_b->getNumResults();

    if ( NA == NB || (uniqueNA && uniqueNB)) {
        // we can do a rank by rank operation
        while ( auto pr = m_a->getNextSet(NA == NB)) {
            m_b->getCommonRanks(*pr);
            m_a->getCommonSet(*pr);
            printL(os, pr->label, pr->a, pr->b, format);
        }
    } else {
        throw EvalError("Operands of " + m_op + " cannot be matched by rank.");
    }
}

AIFStream* BinaryFuncValue::getAIFStream(AIFStream* a, AIFStream* b) const {
    if (auto str = AIFStream::fromBinOp(a, m_op, b)) {
        return str;
    } else {
        delete a;
        delete b;
        throw EvalError("operator " + m_op + " is not supported");
    }

}

void BinaryFuncValue::printL(std::ostream& os, const std::string& label,
                             AIFStream* a, AIFStream* b,
                             const std::string& format) const {
    auto aif = getAIFStream(a,b);
    aif->print(os, label, format);
    os << "\n";
    delete aif;
}

std::pair<size_t, bool> BinaryFuncValue::getNumResults() {
    auto [ ra, ua ] = m_a->getNumResults();
    auto [ rb, ub ] = m_b->getNumResults();
    if (ra == rb) {
        return std::make_pair( ra, std::max(ua,ub));
    } else if ( ua && ub) {
        return std::make_pair(1,true);
    } else {
        throw EvalError("Operands of " + m_op + " cannot be matched by rank.");
    }
}

std::optional<ResultPair> BinaryFuncValue::getNextSet(bool rankToRank) {
    if (!rankToRank) {
        if (m_processed) return std::optional<ResultPair>{};
        m_processed = true;
        return ResultPair{.isRankToRank=false};
    }
        
    if (m_unprocessed) {
        return ResultPair{.ranks = m_unprocessed->ranks->clone(),
                              .isRankToRank = rankToRank};
    }

    auto res = m_a->getNextSet(rankToRank);
    if (!res) return res;       // done processing

    m_b->getCommonRanks(*res);
    m_a->getCommonSet(*res);

    // to get the common ranks for our two operands, we had to push ahead
    // of the normal process so we have a pair of AIFStreams that we'll
    // need to cache for getCommonSet
    m_unprocessed.swap(res);

    return ResultPair{.ranks = m_unprocessed->ranks->clone(),
            .isRankToRank = true};
}

// This binary value is the second term of some larger expression,
// This function find ranks that are a subset of pr.ranks
// then stores our result stream into pr.b and updates pr.ranks
// with the common ranks (and potentiall sets pr.ranksAreEq)
void BinaryFuncValue::getCommonRanks(ResultPair& pr) {
    // Get the common ranks of pr and m_a
    if (!pr.isRankToRank) {
        pr.b = getSingleResult();
        return;
    }
        
    ResultPair loc{.isRankToRank = true};

    auto eqA = false;
    AIFStream* a = nullptr;
    if (m_unprocessed) {
        // algorithmically, we expect that that any
        // ranks left over from the previous call to getCommonRanks
        // are a subset of pr.ranks
        eqA = (*m_unprocessed->ranks == *pr.ranks);
        loc.ranks = std::move(m_unprocessed->ranks);
        a = m_unprocessed->a;
        m_unprocessed.reset();
    } else {
        // if we don't have any ranks left over from the
        // previous getCommonRanks, just get common ranks with the
        // usual function
        loc.ranks = pr.ranks->clone();
        m_a->getCommonRanks(loc);
        eqA = loc.ranksAreEq;

        // "reverse the polarity" so we can treat those
        // common ranks like the output from m_a->getNextSet.
        a = loc.b;
        loc.b = nullptr;
    } 

    // now we "reverse the polarity" so we can treat those
    // common ranks like the output from m_a->getNextSet.
    auto ranksA = std::unique_ptr<Bitset>{};
    ranksA = loc.ranks->clone();
    
    m_b->getCommonRanks(loc);  // make loc.ranks the common ranks and set loc.b
    auto b = loc.b;

    // so a and b are the AIFStreams of your operators and loc.ranks are
    // the common set, and loc.ranksAreEq if the A and B sets are the same
    if (eqA && loc.ranksAreEq) {
        pr.ranksAreEq = true;
        pr.b = getAIFStream(a, b);
        return;
    } else if (loc.ranksAreEq) {
        // all computed ranks of a are consumed
        pr.ranksAreEq = false;
        *pr.ranks &= *loc.ranks;
        pr.b = getAIFStream(a, b);
    } else {
        // some ranks of A are left over for next call
        *ranksA -= *loc.ranks;
        m_unprocessed.emplace();
        m_unprocessed->ranks = std::move(ranksA);
        m_unprocessed->a = a;

        pr.ranksAreEq = false;
        *pr.ranks &= *loc.ranks;
        pr.b = getAIFStream(a->clone(), b);
    } 
}

void BinaryFuncValue::getCommonSet(ResultPair& result) {
    if (!result.isRankToRank) {
        result.a = getSingleResult();
        result.label = "";
        return;
    }

    // get nextSet cached the value in m_unprocessed
    auto& r = *m_unprocessed;

    if (result.ranksAreEq) {
        result.a = getAIFStream(r.a, r.b);
        result.label = std::move(r.label);
        m_unprocessed.reset();
        return;
    }

    // we've only used a portion of the used ranks
    result.a = getAIFStream(r.a->clone(), r.b->clone());
    *r.ranks -= *result.ranks;
    result.label = "not the right label";
}

AIFStream* BinaryFuncValue::getSingleResult() {
    if (auto loc = m_a->getNextSet(false)) {
        m_b->getCommonRanks(*loc);
        m_a->getCommonSet(*loc);
        return getAIFStream(loc->a, loc->b);
    } else {
        throw EvalError("Unable to get result from first operand.");
    }
}

ComparisonValue::ComparisonValue(std::unique_ptr<Value>&& a, const std::string& op,
                                 std::unique_ptr<Value>&& b) :
    Value(a->getRecSet()), m_a(std::move(a)), m_op(op), m_b(std::move(b)) {
    if (op == "=") m_op = "==";
}

static void getDiffStr(std::ostream& os,
                       const std::string& label, const std::string& diffStr) {

    if (Client.MI_mode) {
        os << diffStr;
        return;
    } else if (label.empty()) {
        os << diffStr << "\n";
        return;
    }

    // insert label info each line of the result
    os << label;
    for (auto c : diffStr) {
        if (c == '\n') os << "\n" << label;
        else os << c;
    }
    os << "\n";
}

void ComparisonValue::print(std::ostream& os, rec_set_t* rec_set, const std::string& format) {
    std::optional<std::string> passed, failed, warn;
    auto deltaSS = std::ostringstream{};

    getFullResult(passed, failed, warn, deltaSS);

    auto printDelta = (m_op != "!=");
    auto sep = "\n";
    if (printDelta) {
        // match previous behavior
        if ((failed && failed->find('{') == std::string::npos) ||
            (warn && warn->find('{') == std::string::npos)) sep = " ";
    }

    if (passed) os << *passed << "Comparison is true.\n";
    if (failed) os << *failed << "Comparison is false." << sep;
    if (warn) os << *warn << "Comparison is warn." << sep;

    if (printDelta && (failed || warn)) {
        os << "The difference delta is:\n";
        os << deltaSS.str();
    }
 
}

std::pair<CompareResult, std::string>
ComparisonValue::getSimpleResult(rec_set_t* rec_set) {
    std::optional<std::string> passed, failed, warn;
    auto deltaSS = std::ostringstream{};

    try {
        getFullResult(passed, failed, warn, deltaSS);

        auto res = CompareResult::Pass;
        if (failed) res = CompareResult::Fail;
        else if (warn) res = CompareResult::Warn;

        return std::make_pair(res, std::move(deltaSS.str()));

    } catch (EvalError& err) {
        return std::make_pair(CompareResult::Invalid, std::string(err.what()));
    }
}

// combine two pieces of procset notation into a single combined procset
static std::string combineLabels(const std::string& labelA, const std::string &labelB){
    if (labelA.find('}', labelA.find('{')) ==  std::string::npos ||
        labelB.find('}', labelB.find('{')) == std::string::npos) return labelA;

    std::ostringstream os;
    const auto* a = labelA.c_str(), *b = labelB.c_str();

    // preamble
    for ( ; *a && *a != '{'; ++a) os << *a;
    b = strchr(b,'{') + 1;
    if (!*a) return ""; else os << *(a++);

    // prime the pump - b is normally the value not used in the previous
    // iteration in the loop
    char *nextA = nullptr, *nextB = nullptr;
    int bval = strtol(b,&nextB,10);
    if (nextB == b) return "" ; else b = nextB;

    int lastVal = -2; bool inRange = false;

    const char end = '}';
    while (true) {
        int aval = INT_MAX;
        if (*a != end) {
            aval = strtol(a,&nextA,10);
            if (nextA == a) return ""; else a = nextA;
        }
            
        if (bval < aval) {
            // a,aval is the next in order
            std::swap(a,b);
            std::swap(aval,bval);
        }

        if (aval == lastVal+1) {
            inRange = true;
        } else if (inRange) {
            os << ".." << lastVal << "," << aval;
            inRange = false;
        } else {
            os << aval;
        }

        if (a[0]=='.' && a[1]=='.') {
            // we don't write out now, the other label may
            // extend the range
            inRange = true;
            lastVal = strtol(a+2,&nextA,10);
            a = nextA;
        } else {
            lastVal = aval;
        }

        if (*a == ',') ++a;
        else if (*a == end) {
            if (*b == end) break;
        } else {
            return "";
        }
    }
    // the last value was not written out
    if (bval == lastVal+1) os << ".." << bval;
    else {
        if (inRange) os << ".." << lastVal;
        if (bval != INT_MAX) os << "," << bval;
    }

    // } and everything after
    for ( ; *a; ++a) os << *a;

    return os.str();
}

void ComparisonValue::getFullResult(std::optional<std::string>& passed,
                                    std::optional<std::string>& failed,
                                    std::optional<std::string>& warn,
                                    std::ostream& deltaSS)
{
    auto lowerEps = Client.cmp_lower_eps, upperEps = Client.cmp_upper_eps;
    
    auto printDelta = (m_op != "!=");

    // check if the values can be matched up.
    auto [ NA, uniqueNA ] = m_a->getNumResults();
    auto [ NB, uniqueNB ] = m_b->getNumResults();

    if ( NA == NB || (uniqueNA && uniqueNB)) {
        // we can do a rank by rank operation
        while ( auto pr = m_a->getNextSet(NA == NB)) {
            m_b->getCommonRanks(*pr);
            m_a->getCommonSet(*pr);

            AIFStream::Difference diff;
            auto errMsg = std::string{};
            auto res = AIFStream::compareOp(pr->a, m_op, pr->b, diff, &lowerEps, &upperEps, errMsg);

            delete pr->a;
            delete pr->b;
            pr->a = pr->b = nullptr;

            switch (res) {
            case CompareResult::Pass:
                passed = (passed ? combineLabels(*passed,pr->label) : pr->label);
                break;

            case CompareResult::Fail:
                failed = (failed ? combineLabels(*failed,pr->label) : pr->label);
                if (printDelta) getDiffStr(deltaSS, pr->label, diff.getResult());
                break;
            case CompareResult::Warn:
                warn = (warn ? combineLabels(*warn,pr->label) : pr->label);
                if (printDelta) getDiffStr(deltaSS, pr->label, diff.getResult());
                break;
            case CompareResult::Invalid:
                throw EvalError(errMsg.c_str());
            }
        }
    } else {
        throw EvalError("Cannot compare mismatched values.");
    }

    //os << "Don't forget MI!!!!\n";
}

