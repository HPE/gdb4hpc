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
#include "procset.h"
#include "slice.h"
#include "blockmap_client.h"

// clean these up
struct dbgprocset_t;
typedef struct dbgprocset_t dbgprocset_t;
extern dbgprocset_t *FindProcSet(const char *name);
extern dbgprocset_t *CreateTempProcSetSlice(dbgprocset_t *ps, slice_t *ps_slice);
extern void FreeProcSet(dbgprocset_t *s);
extern rec_set_t *CreateRecSetForProcset(const dbgprocset_t *ps);
extern void SetOverlaySetForProcSet(dbgprocset_t *s);
extern dbg_over_set_t *GetNextOverlaySetForProcSet(dbgprocset_t *s);
struct BlockMap;
typedef struct BlockMap BlockMap;
extern BlockMap *FindBlockMap(const std::string& blockmap);

std::map<std::string, std::unique_ptr<Value>> IdMap::m_client_values;

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

BlockMap* IdMap::getBlockmapForName(std::string name) {
    return FindBlockMap(name);
}

bool IdMap::clientValueIdIsAvailable(std::string_view name) {
    return !m_client_values.contains(std::string{name});
}

std::unique_ptr<Value> IdMap::cloneClientValue(std::string_view name) {
    const auto key = std::string{name};
    if (auto it = m_client_values.find(key); it != m_client_values.end()) {
        return it->second->pickle();
    }
    return nullptr;
}

void IdMap::setClientValue(std::string_view name, Value& value) {
    const auto key = std::string{name};
    m_client_values[key] = value.pickle();
}

// verify the at debug variable $name is not already in use.
// Print a warning if printMsg is true.
bool IdMap::idIsAvailable(const std::string& name, bool printMsg) {
    const char* existingType = nullptr;
    if (FindProcSet(name.c_str())) {
        existingType = "procset";
    } else if (FindBlockMap(name)) {
        existingType = "decomposition";
    } else if (!clientValueIdIsAvailable(name)) {
        existingType = "client variable";
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

std::unique_ptr<Value> Value::pickle() {
    throw EvalError("Value cannot be saved");
}

std::string Value::getExpr() const {
    throw EvalError("type is not a program value");
}

void Value::print(std::ostream& os, rec_set_t*, const std::string&) {
    os << "Value::print is undefined\n";
}

void Value::addMetaInfo(const std::string&) {}

void Value::apply(rec_set_t*, ValueVisitor& visitor) {
    auto os = std::ostringstream{};
    print(os, nullptr, "");
    visitor(os.str());
}

std::pair<size_t, bool> Value::getNumResults() {
    throw EvalError("Type does not support operations");
}
std::optional<ResultPair> Value::getNextSet(bool) {
    throw EvalError("Type does not support operations");
}

void Value::getCommonRanks(ResultPair&) {
    throw EvalError("Type does not support operations");
}
void Value::getCommonSet(ResultPair&) {
    throw EvalError("Type does not support operations");
}

ReplyValue::ReplyValue(rec_set_t* rec_set)
    : Value(rec_set)
{}

ReplyValue::~ReplyValue() {
    for (auto& [di, grp, aif] : m_replies)
        delete aif;
}

static std::string getLabel(rec_set_t* rec_set, group_reply* reply, const std::string& format) {
    auto threads = rec_set ? rec_set->getThreads() : nullptr;
    auto diName = reply->rep_di.c_str();

    auto label = GetProcSetString(diName, reply->rep_group.get(), threads);
    if (label.empty()) ;
    else if (format == "procset") {}
    else if (format.find("csv") == std::string::npos) label += ": ";
    else label = "\"" + label + "\",";

    return label;
}

void ReplyValue::print(std::ostream& os, rec_set_t* rec_set, const std::string& format) {
    auto rep_set = retrieve();

    for (auto reply : rep_set) {
        // retrieve shouldn't return anything that isn't data, but lets be safe..
        if (reply->rep_event->ev_event != DBGEV_DATA) continue;
        auto label = getLabel(rec_set, reply, format);
        auto aif = EVENT_DATA(reply->rep_event);
        aif->print(os, label, format);
        os << "\n";
    }
}

void ReplyValue::apply(rec_set_t* rec_set, ValueVisitor& visitor) {
    auto rep_set = retrieve();

    for ( auto reply : rep_set) {
        // retrieve shouldn't return anything that isn't data, but lets be safe..
        if (reply->rep_event->ev_event != DBGEV_DATA) continue;
        auto label = getLabel(rec_set, reply, "procset");
        if (auto aif = EVENT_DATA(reply->rep_event)) {
            visitor.beginReply(label);
            aif->getVStream().apply(visitor);
        }
    }
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

    return std::make_pair(m_numResults, m_replies.size() == 1);
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

PendingReplyValue::PendingReplyValue(rec_set_t* rec_set, const std::string& expr)
    : ReplyValue(rec_set), m_expr(expr)
{
    if (!rec_set) {
        throw std::runtime_error("cannot evaluate " + expr + " without a process");
    }
}

std::unique_ptr<Value> PendingReplyValue::pickle() {
    auto reply = retrieve();
    reply.pickle();
    return std::make_unique<PickledReplyValue>(std::move(reply));
}

void PendingReplyValue::addMetaInfo(const std::string& metaInfo) {
    m_expr = metaInfo + m_expr;
}

rep_set_t PendingReplyValue::retrieve() const
{
    auto rep_set = DbgEvalExpr(m_rec_set, m_expr.c_str());
    if (rep_set.isEmpty()) {
        throw EvalError("Could not retrieve value of " + m_expr);
    } else if (ProcessErrorInRepSet(rep_set)) {
        throw EvalErrorAlreadyReported();
    }

    return rep_set;
}

PickledReplyValue::PickledReplyValue(rep_set_t&& rep_set)
    : ReplyValue(nullptr)
    , m_rep_set{std::move(rep_set)}
{}

std::unique_ptr<Value> PickledReplyValue::pickle() {
    // We assume that the original rep_set was fully joined and that we don't
    // have to join this one
    return std::make_unique<PickledReplyValue>(m_rep_set.clone());
}

rep_set_t PickledReplyValue::retrieve() const {
    return m_rep_set.clone();
}

DecompValue::DecompValue(rec_set_t* rec_set, BlockMap*  blockMap,
                         std::unique_ptr<Value>&& val)
    : SingleValue(rec_set), m_blockMap(blockMap), m_val(std::move(val))
{}

void DecompValue::addIndex(const std::string& idx) {
    m_idx += idx;
}

void DecompValue::print(std::ostream& os, rec_set_t*,
                        const std::string& format) {

    auto aif = getAIF();
    aif->print(os, "", format);
    os << "\n";
}

AIFStream* DecompValue::getAIF() {
    if (m_aif)
        return m_aif;

    auto val = dynamic_cast<PendingReplyValue*>(m_val.get());
    if (!val) {
        throw EvalError("Decomposition must refer to a progam value");
    }

    if (!m_idx.empty()) {
        auto os = std::ostringstream{};
        os << "[[|(" << m_blockMap->getDefinition() << ")" << m_idx << "]]";
        val->addMetaInfo(os.str());
    }

    auto rep_set = val->retrieve();

    auto aif = construct_blockmap_data(m_blockMap, m_idx, val->getRecSet(), rep_set);
    return m_aif = aif;
}

DistributedBlockMapValue::DistributedBlockMapValue(rec_set_t* rec_set, BlockMap*  blockMap,
                                                   std::unique_ptr<Value>&& val)
    : ReplyValue(rec_set), m_blockMap(blockMap), m_val(std::move(val))
{}

rep_set_t DistributedBlockMapValue::retrieve() const {
    auto val = dynamic_cast<PendingReplyValue*>(m_val.get());
    if (!val) {
        throw EvalError("Decomposition must refer to a progam value");
    }

    return val->retrieve();
}

void DistributedBlockMapValue::print(std::ostream& os, rec_set_t*,
                                     const std::string& format) {

    auto rep_set = retrieve();
    for ( auto reply : rep_set) {
        auto label = getLabel(m_rec_set, reply, format);
        auto aif = EVENT_DATA(reply->rep_event);

        os << label;
        PrintWithBlockMap(aif, m_blockMap, os);
        os << "\n";
    }
}


AIFValue::AIFValue(rec_set_t* rec_set, const std::string& expr, AIFStream* aif)
    : SingleValue(rec_set), m_expr(expr) {
    m_aif = aif;
}

std::unique_ptr<Value> AIFValue::pickle() {
    // return a copy of the AIFValue with no rec_set.
    return std::make_unique<AIFValue>(nullptr, m_expr, m_aif->clone());
}

AIFStream* AIFValue::getAIF() {
    return m_aif;
}

void AIFValue::print(std::ostream& os, rec_set_t*, const std::string& format) {
    m_aif->print(os, "", format);
    os << "\n";
}

std::string AIFValue::getExpr() const { return m_expr; }

SingleValue::SingleValue(rec_set_t* rec_set) : Value(rec_set) {}
SingleValue::~SingleValue() { delete m_aif; }

std::pair<size_t, bool> SingleValue::getNumResults() { return std::make_pair(1,true); }
std::optional<ResultPair> SingleValue::getNextSet(bool) {
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

void ProcsetValue::print(std::ostream& os, rec_set_t*, const std::string&) {
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

void BlockmapValue::print(std::ostream& , rec_set_t*, const std::string&) {}

BlockMap* BlockmapValue::getBlockMap() const { return FindBlockMap(m_name); }

BinaryFuncValue::BinaryFuncValue(std::unique_ptr<Value>&& a,
                                 const std::string& op,
                                 std::unique_ptr<Value>&& b) :
    Value(a->getRecSet()), m_a(std::move(a)), m_op(op), m_b(std::move(b))
{}

std::unique_ptr<Value> BinaryFuncValue::pickle() {
    return std::make_unique<BinaryFuncValue>(m_a->pickle(), m_op, m_b->pickle());
}

void BinaryFuncValue::print(std::ostream& os, rec_set_t*, const std::string& format) {
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
        auto extra = (m_op == "==") ? " (use the compare command)" : "";
        throw EvalError("operator " + m_op + " is not supported" + extra);
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

    if (m_unprocessed->ranks) {
        return ResultPair{.ranks = m_unprocessed->ranks->clone(),
                              .isRankToRank = true};
    } else {
        return ResultPair{.isRankToRank=false};
    }
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
    // get nextSet cached the value in m_unprocessed
    if (!m_unprocessed) {
        // was known to be !rankToRank a priori
        result.label = "";
        result.a = getSingleResult();
    } else if (!result.isRankToRank) {
        // single match, we can consume the result
        auto& r = *m_unprocessed;
        result.a = getAIFStream(r.a, r.b);
        result.label = "";
        m_unprocessed.reset();
    } else if (result.ranksAreEq) {
        // complete matching, we can consume the result
        auto& r = *m_unprocessed;
        result.a = getAIFStream(r.a, r.b);
        result.label = std::move(r.label);
        m_unprocessed.reset();
    } else {
        // we've only used a portion of the used ranks
        auto& r = *m_unprocessed;
        result.a = getAIFStream(r.a->clone(), r.b->clone());
        *r.ranks -= *result.ranks;
        result.label = "not the right label";
    }
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
    Value(a->getRecSet()), m_a(std::move(a)), m_op(op), m_b(std::move(b)), m_should_label_diffs{true} {
}

static void getDiffStr(std::ostream &os, bool should_label_diffs,
                       const std::string &label, const std::string &diffStr) {

    if (!should_label_diffs) {
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

void ComparisonValue::setLabelDiffs(bool should_label) {
    m_should_label_diffs = should_label;
}

void ComparisonValue::print(std::ostream& os, rec_set_t*, const std::string&) {
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
ComparisonValue::getSimpleResult(rec_set_t*) {
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
                if (printDelta) getDiffStr(deltaSS, m_should_label_diffs, pr->label, diff.getResult());
                break;
            case CompareResult::Warn:
                warn = (warn ? combineLabels(*warn,pr->label) : pr->label);
                if (printDelta) getDiffStr(deltaSS, m_should_label_diffs, pr->label, diff.getResult());
                break;
            case CompareResult::Invalid:
                throw EvalError(errMsg.c_str());
            }
        }
    } else {
        throw EvalError("Cannot compare mismatched values.");
    }
}

ClientValue::ClientValue(std::string_view name)
    : Value(nullptr)
    , m_name{name}
{}

std::string ClientValue::getName() const {
    return m_name;
}

Value& ClientValue::instantiate() {
    if (!m_referenced_value) {
        m_referenced_value = IdMap::cloneClientValue(m_name);
        if (!m_referenced_value) {
            throw EvalError("Not a client side variable: " + m_name);
        }
    }

    return *m_referenced_value;
}

std::unique_ptr<Value> ClientValue::pickle() {
    // we instantiate the value and pickle it rather than simply pickling the
    // ClientValue. otherwise, statements like $foo = $bar would get weird,
    // storing names instead of values.
    return instantiate().pickle();
}

rec_set_t* ClientValue::getRecSet() const {
    // before the ClientValue is saved, it has no rec_set, and anything saved in
    // IdMap is saved without a rec_set.
    return nullptr;
}

std::pair<size_t, bool> ClientValue::getNumResults() {
    return instantiate().getNumResults();
}

std::optional<ResultPair> ClientValue::getNextSet(bool rankToRank) {
    // no instiantiation here. in the Value API, only getNumResults is expected to have side effects
    if (!m_referenced_value) throw EvalError("Not a client side variable: " + m_name);
    return m_referenced_value->getNextSet(rankToRank);
}

void ClientValue::getCommonRanks(ResultPair& result) {
    // no instiantiation here. in the Value API, only getNumResults is expected to have side effects
    if (!m_referenced_value) throw EvalError("Not a client side variable: " + m_name);
    m_referenced_value->getCommonRanks(result);
}

void ClientValue::getCommonSet(ResultPair& result) {
    // no instiantiation here. in the Value API, only getNumResults is expected to have side effects
    if (!m_referenced_value) throw EvalError("Not a client side variable: " + m_name);
    m_referenced_value->getCommonSet(result);
}

void ClientValue::print(std::ostream& os, rec_set_t* rec_set, const std::string& format) {
    instantiate().print(os, rec_set, format);
}

void ClientValue::apply(rec_set_t* rec_set, ValueVisitor& visitor) {
    instantiate().apply(rec_set, visitor);
}

ClientValueAssignment::ClientValueAssignment(std::unique_ptr<ClientValue>&& assignee, std::unique_ptr<Value>&& source)
    : Value(source->getRecSet())
    , m_assignee{std::move(assignee)}
    , m_source{std::move(source)}
{}

std::unique_ptr<Value> ClientValueAssignment::pickle() {
    doAssignment();
    return m_assignee->pickle();
}

void ClientValueAssignment::print(std::ostream& os, rec_set_t* rec_set, const std::string& format) {
    doAssignment();
    m_assignee->print(os, rec_set, format);
}

void ClientValueAssignment::apply(rec_set_t* rec_set, ValueVisitor& visitor) {
    doAssignment();
    m_assignee->apply(rec_set, visitor);
}

std::pair<size_t, bool> ClientValueAssignment::getNumResults() {
    doAssignment();
    return m_assignee->getNumResults();
}

std::optional<ResultPair> ClientValueAssignment::getNextSet(bool rankToRank) {
    // no assignment here, only getNumResults is expected to have side effects
    return m_assignee->getNextSet(rankToRank);
}

void ClientValueAssignment::getCommonRanks(ResultPair& result) {
    // no assignment here, only getNumResults is expected to have side effects
    return m_assignee->getCommonRanks(result);
}

void ClientValueAssignment::getCommonSet(ResultPair& result) {
    // no assignment here, only getNumResults is expected to have side effects
    return m_assignee->getCommonSet(result);
}

void ClientValueAssignment::doAssignment() {
    IdMap::setClientValue(m_assignee->getName(), *m_source);
}
