/*******************************************************************************
 * Copyright 2022-2023 Hewlett Packard Enterprise Development LP.
 ******************************************************************************/

#pragma once

#include <string>
#include <memory>
#include <map>
#include <vector>
#include "dbginvoke.h"
#include "dbg_client.h"
#include "ais.h"

class AIFStream;
class Bitset;
class DbgInvoke;

// Basic error type thrown by grammar and value 
class EvalError : public std::exception {
public:
    EvalError(std::string err) : m_err(std::move(err)) {}
    const char* what() const noexcept override { return m_err.c_str(); }
private:
    std::string m_err;
};

class ParseError : public EvalError {
public:
    ParseError(std::string err) : EvalError(std::move(err)) {}
};

class EvalErrorAlreadyReported : public EvalError {
public:
    EvalErrorAlreadyReported() : EvalError("") {}
    const char* what() const noexcept override { return "error was already printed"; }
};

class rec_set_t;

struct dbgprocset_t;
typedef struct dbgprocset_t dbgprocset_t;

struct BlockMap;
typedef struct BlockMap BlockMap;

struct group_reply;
typedef struct group_reply group_reply_t;

class Value;

// Map for accessing the globally stored procSets and blockmaps.
// 
class IdMap {
public:
    static std::pair<dbgprocset_t*,bool> getProcset(const std::string& procs);

    static BlockMap* getBlockmapForName(std::string name);

    static bool idIsAvailable(const std::string& name, bool printMsg);

    // `cloneClientValue` looks up the stored client variable with given `name`.
    // if found, the function clones it and returns the clone. if not found,
    // it returns nullptr.
    static std::unique_ptr<Value> cloneClientValue(std::string_view name);

    // `setClientValue` the given `value` as `name`.
    //
    // since values are single use stream objects, this function clones
    // (pickles) `value` into storage. since cloning a value is only well
    // defined on an unused value, `value` must not have been evaluated yet.
    //
    // this function calls `value.pickle()`, which might have side effects, so
    // `value` is a mutable reference.
    static void setClientValue(std::string_view name, Value& value);
private:
    // return true if `name` does not refer to an existing client value
    static bool clientValueIdIsAvailable(std::string_view name);

    static std::map<std::string, std::unique_ptr<Value>> m_client_values;
};

    
// struct for negotiating a common set of ranks for a binary operation
struct ResultPair {
    std::unique_ptr<Bitset> ranks = std::unique_ptr<Bitset>{};
    bool ranksAreEq = false;
    bool isRankToRank = true;
    AIFStream* a = nullptr;
    AIFStream* b = nullptr;
    std::string label = "";
};

// visitor for AIS based operations on a value
class ValueVisitor : public ais::Visitor {
public:
    virtual void beginReply(const std::string& label) = 0;
};

//---------------------------------------------------------------------------
// The Value type holds the value of an expression which is retrieved or 
// computed on demand.   The Expression and Value trees have some similarities,
// but the Expression tree is wedded directly to the expression *syntax*,
// while the Value type contains *semantics*.   So at evaluation time
// a DSym will resolve to a Procset or Blockmap value.
//
// Not that Value types are evaluated lazily, so they can be built
// up via recursion down the evaluation tree and then a print call
// on the top level Value drives the full evalation.
class Value {
public:
    Value(rec_set_t* rec_set);
    virtual ~Value();

    // `pickle` returns a pickled version of the value. a pickled value should
    // be safe to store long term and not depend on any backend state. a pickled
    // value should itself implement `pickle`.
    //
    // note that pickling can have side effects (e.g. a PendingReplyValue will do a
    // remote fetch to retrieve the value to be pickled), and for that reason
    // `pickle` is not a const function.
    //
    // XXX: a better api might be a `void pickle()` with side effects, and a const `clone()`.
    //
    // not every value type can be pickled, and the default implementation of
    // `pickle` throws EvalError.
    virtual std::unique_ptr<Value> pickle();

    virtual rec_set_t* getRecSet() const { return m_rec_set; }
    virtual std::string getExpr() const;

    // primary interface for driving the evaluation of the value, it printed
    // out to os
    virtual void print(std::ostream& os, rec_set_t* rec_set, const std::string& format) = 0;

    // used for pushing additional meta-info (like array length) for the 
    // back-end gdb
    virtual void addMetaInfo(const std::string& metaInfo);

    // apply a general operation to the value
    virtual void apply(rec_set_t* rec_set, ValueVisitor& visitor);

    //-- interface for negotiating a common set of ranks with a pair of replies --
    // the next four functions all throw an exception for any value types that
    // don't have an AIFStream(s) form.
    // get the number of results and if there is a unique value
    virtual std::pair<size_t, bool> getNumResults();

    // get the next available result from the first operand - includes the ranks over
    // which is defined.   result.ranks is of size numResults
    virtual std::optional<ResultPair> getNextSet(bool rankToRank);

    // find a result on the second operand that includes at least one rank in common
    // update result.ranks with the common ranks, set result.ranksAreEq if result.ranks 
    // is unchanged, and sets result.b with the the data stream of the second operand
    virtual void getCommonRanks(ResultPair& result);
    // finalizes the handshake by setting result.a to the data stream of the first operand,
    // updates the first value to mark the ranks result.ranks as DONE, and sets
    // result.label to the procset values from the common ranks (which are in result.ranks)
    virtual void getCommonSet(ResultPair& result);

protected:
    rec_set_t* m_rec_set;
};

class ReplyValue : public Value {
    // This class implements print/apply/getNumResults/getNextSet/getCommonRanks/getCommonSet
    // for anything that can produce a rep_set.
public:
    ReplyValue(rec_set_t* rec_set);
    virtual ~ReplyValue();

    // ReplyValue will call this function to obtain a rep_set to process.
    // called by print, apply, and getNumResults.
    virtual rep_set_t retrieve() const = 0;

    virtual void print(std::ostream& os, rec_set_t* rec_set, const std::string& format) override;
    virtual void apply(rec_set_t* rec_set, ValueVisitor& visitor) override;

    // functions for iterating through the matched ranks of a pair of values
    std::pair<size_t, bool> getNumResults() override;
    std::optional<ResultPair> getNextSet(bool rankToRank) override;
    void getCommonRanks(ResultPair& result) override;
    void getCommonSet(ResultPair& result) override;

private:
    std::unique_ptr<Bitset> toRankBits(DbgInvoke* di, const Bitset& grp);
    std::unique_ptr<Bitset> toReplyBits(DbgInvoke* di, const Bitset& resGrp);

    size_t m_numResults = 0;
    std::vector<std::tuple<DbgInvoke*, std::unique_ptr<Bitset>,AIFStream*>> m_replies;
    std::map<DbgInvoke*, std::pair<std::unique_ptr<Bitset>, size_t>> m_ranksPerDI;
};

class ClientValue : public Value {
    // a ClientValue is an initially empty value with a name that references
    // something in IdMap. the lookup happens when the value is used. if at that
    // time an attempt is made to use a ClientValue with a name that does not
    // refer to a value, EvalError is thrown.
    //
    // if the initial lookup is successful, ClientValue stores the retrieved
    // value and forwards all future calls to it.
public:
    ClientValue(std::string_view name);
    std::unique_ptr<Value> pickle() override;

    std::string getName() const;

    rec_set_t* getRecSet() const override;

    // note the lack of getExpr() and addMetaInfo(). those are used for
    // constructing expressions to send to the backend to evaluate. since a
    // client value's name is only meaningful on the client side, ClientValue
    // doesn't participate in expression building for the backend.
    //
    // however, it might be possible to support this use case by printing the
    // Value to a string and then sending it.

    std::pair<size_t, bool> getNumResults() override;
    std::optional<ResultPair> getNextSet(bool rankToRank) override;
    void getCommonRanks(ResultPair& result) override;
    void getCommonSet(ResultPair& result) override;

    void print(std::ostream& os, rec_set_t* rec_set, const std::string& format) override;
    void apply(rec_set_t* rec_set, ValueVisitor& visitor) override;

private:
    // look up the value associated with the stored name, clone it, cache it,
    // and return a reference to it. future calls will return a reference to the
    // cached value.
    //
    // if no value with the given name exists, throws EvalError.
    Value& instantiate();

    std::unique_ptr<Value> m_referenced_value;
    std::string m_name;
};

class ClientValueAssignment : public Value {
    // a ClientValueAssignment holds a name (assignee) and a value source. when
    // ClientValueAssignment is used, it will attempt to save the source value
    // as the given name. If that's not possible (e.g. the source value doesn't
    // implement Value::pickle), it throws EvalError.
    //
    // in the gdb expression language, assignments produce a value. e.g.
    //   print $foo = 2 -> 2
    //   print ($foo = 2) + 2 -> 4
    // so this class can also be used as a value.
  public:
    ClientValueAssignment(std::unique_ptr<ClientValue>&& assignee, std::unique_ptr<Value>&& source);

    std::unique_ptr<Value> pickle() override;
    void print(std::ostream& os, rec_set_t* rec_set, const std::string& format) override;
    void apply(rec_set_t* rec_set, ValueVisitor& visitor) override;
    void doAssignment();

    std::pair<size_t, bool> getNumResults() override;
    std::optional<ResultPair> getNextSet(bool rankToRank) override;
    void getCommonRanks(ResultPair& result) override;
    void getCommonSet(ResultPair& result) override;
private:
    std::unique_ptr<ClientValue> m_assignee;
    std::unique_ptr<Value> m_source;
};

class PendingReplyValue : public ReplyValue {
    // The basic result for the result of an evaluation from the back-end gdb's.
    // The result is the aggregated set of different simple results (as AIFStream's)
    // paired with the set of ranks for each.
    //
    // an alternate name for this class could be "ServerValue" - the opposite of a ClientValue.
public:
    PendingReplyValue(rec_set_t* rec_set, const std::string& expr);
    std::unique_ptr<Value> pickle() override;

    virtual void addMetaInfo(const std::string& metaInfo) override;
    rec_set_t* getRecSet() const { return m_rec_set; }
    std::string getExpr() const override { return m_expr; }

    rep_set_t retrieve() const override;

private:
    std::string m_expr;
};

class PickledReplyValue : public ReplyValue {
    // PickledReplyValue is a container for a rep_set.
    // note that since it is plain old data detached from its original reply,
    // Value::getRecSet() returns nullptr.
public:
    PickledReplyValue(rep_set_t&& rep_set);
    std::unique_ptr<Value> pickle() override;
    rep_set_t retrieve() const override;

private:
    rep_set_t m_rep_set;
};

// Base class for debugger results that are a single unique result, see
// DecompValue for the classic example.
class SingleValue : public Value {
public:
    SingleValue(rec_set_t* rec_set);
    ~SingleValue() override;

    // functions for iterating through the matched ranks of a pair of values
    std::pair<size_t, bool> getNumResults() override;
    std::optional<ResultPair> getNextSet(bool rankToRank) override;
    void getCommonRanks(ResultPair& result) override;
    void getCommonSet(ResultPair& result) override;

protected:
    virtual AIFStream* getAIF() = 0;
    AIFStream* m_aif = nullptr;
    bool m_processed = false;
};
  
// A single array decomposed across multiple ranks
class DecompValue : public SingleValue {
public:
    DecompValue(rec_set_t* rec_set, BlockMap*  blockMap,
                std::unique_ptr<Value>&& val);

    std::unique_ptr<Value> pickle() override;

    virtual void print(std::ostream& os, rec_set_t* rec_set, const std::string& format) override;

    void addIndex(const std::string& idx);

protected:
    virtual AIFStream* getAIF() override;

private:
    rec_set_t& getRecSet();
    rep_set_t getReply();

    BlockMap* m_blockMap;
    std::unique_ptr<Value> m_val;
    std::string m_idx;
};

// Like DecompValue, but caches all information needed to recreate a decomposition
// AIFStream without needing to make a call to the backend.
class PickledDecompValue : public SingleValue {
public:
    PickledDecompValue(BlockMap* blockmap, PickledRecSet&& rec_set, rep_set_t&& pickled_reply, std::string_view idx);

    std::unique_ptr<Value> pickle() override;

    void print(std::ostream& os, rec_set_t* rec_set, const std::string& fmt) override;
private:
    AIFStream* getAIF() override;

    BlockMap* m_blockmap;
    rep_set_t m_pickled_reply;
    PickledRecSet m_rec_set;
    std::string m_idx;
};

// A value based on a fully distribued block map.   It acts like a reply, but the
// blockMap detemines the layout and size
class DistributedBlockMapValue : public ReplyValue {
public:
    DistributedBlockMapValue(rec_set_t* rec_set, BlockMap*  blockMap,
                             std::unique_ptr<Value>&& val);

    std::unique_ptr<Value> pickle() override;

    virtual void print(std::ostream& os, rec_set_t* rec_set,
                       const std::string& format) override;

    rep_set_t retrieve() const override;

private:
    BlockMap* m_blockMap;
    std::unique_ptr<Value> m_val;
};

// A value that is defined on the front end, support constant ints, doubles, or strings
class AIFValue : public SingleValue {
public:
    AIFValue(rec_set_t* rec_set, const std::string& expr, AIFStream* aif); // takes control of aif.
    std::unique_ptr<Value> pickle() override;

    void print(std::ostream& os, rec_set_t* rec_set, const std::string& format) override;
    std::string getExpr() const override;

protected:
    AIFStream* getAIF() override;
    std::string m_expr;
private:
};

// A value that is the result of applying an operator to two other values
class BinaryFuncValue : public Value {
public:
    BinaryFuncValue(std::unique_ptr<Value>&& a, const std::string& op,
                    std::unique_ptr<Value>&& b);
    std::unique_ptr<Value> pickle() override;

    void print(std::ostream& os, rec_set_t* rec_set, const std::string& format) override;

    std::pair<size_t, bool> getNumResults() override;
    std::optional<ResultPair> getNextSet(bool rankToRank) override;
    void getCommonRanks(ResultPair& result) override;
    void getCommonSet(ResultPair& result) override;

protected:
    AIFStream* getAIFStream(AIFStream* a, AIFStream* b) const;
    void printL(std::ostream& os, const std::string& label,
                AIFStream* a, AIFStream* b, const std::string& format) const;
    AIFStream* getSingleResult();

    std::unique_ptr<Value> m_a;
    std::string m_op;
    std::unique_ptr<Value> m_b;
    std::optional<ResultPair> m_unprocessed;
    bool m_processed = false;
};

// A procset like $a or $a{1,4..6}.  The value refers to the procset itself
class ProcsetValue : public Value {
public:
    ProcsetValue(rec_set_t* rec_set, dbgprocset_t* tmpProcset, const std::string& threads);
    ~ProcsetValue() override;

    std::pair<DbgInvoke::Handle,std::unique_ptr<Bitset>> getProcSet();
    dbgprocset_t* getProcset();
    std::string getThreads() const;
    
    virtual void print(std::ostream& os, rec_set_t* rec_set, const std::string& format) override;
private:
    dbgprocset_t* m_tmpProcset;
    std::string m_threads;
};

// A blockmap (decomposition) variable, i.e it refers to the blockmap
// as an object.
class BlockmapValue : public Value {
public:
    BlockmapValue(rec_set_t* rec_set, const std::string& name);
    virtual void print(std::ostream& os, rec_set_t* rec_set, const std::string& format) override;
    BlockMap* getBlockMap() const;
private:
    std::string m_name;
};

// The result of a comparison
class ComparisonValue : public Value {
public:
    ComparisonValue(std::unique_ptr<Value>&& a, const std::string& op,
                    std::unique_ptr<Value>&& b);

    void setLabelDiffs(bool should_lable);
    void print(std::ostream& os, rec_set_t* rec_set, const std::string& format) override;

    // Get the passed,failed and the diffference output if applicable
    std::pair<CompareResult, std::string> getSimpleResult(rec_set_t* rec_set);

private:
    void getFullResult(std::optional<std::string>& passedSet,
                       std::optional<std::string>& failed,
                       std::optional<std::string>& warn,
                       std::ostream& delta);
    
    std::unique_ptr<Value> m_a;
    std::string m_op;
    std::unique_ptr<Value> m_b;
    bool m_should_label_diffs;
};
