// Copyright 2020-2023 Hewlett Packard Enterprise Development LP.

// Classes for defining a sequential stream of data accessed via the AIF interface.
#include "AIFStream.h"
#include "dbgevent.h"
#include "ais.h"
#include <string.h>
#include <stdarg.h>
#include <iomanip>
#include <cmath>

void rd_unimplemented(const char* func) {
    fprintf(stderr, "UNIMPLEMENTED %s\n", func);
}

namespace {
// CachedChunks keep the values when a join operation forces all the
// data to be pulled to down to the front end.   The stream
// can then be processed as usual, or copied for reuse
class CachedChunks : public AIFStream::Chunks {
public:
    CachedChunks(std::string&& data, bool isPartial);
    CachedChunks(std::vector<std::string>&& data);
    std::string getNextChunk(bool& isFinal) override;
    std::shared_ptr<AIFStream::Chunks> clone() const;
private:
    std::vector<std::string> m_data;
    bool m_isPartial = false;
    size_t m_idx = 0;
};
CachedChunks::CachedChunks(std::string&& data, bool isPartial) : m_isPartial(isPartial) {
    m_data.push_back(std::move(data));
}
CachedChunks::CachedChunks(std::vector<std::string>&& data) : m_data{std::move(data)} {}

std::string CachedChunks::getNextChunk(bool& isFinal) {
    if (m_idx < m_data.size()) {
        isFinal = (!m_isPartial && m_idx+1 == m_data.size());
        return std::move(m_data[m_idx++]);
    } else {
        throw std::runtime_error("result data is truncated");
    }
}
std::shared_ptr<AIFStream::Chunks> CachedChunks::clone() const {
    std::vector cpData = m_data; // copy the data chunks
    return std::make_shared<CachedChunks>(std::move(cpData));
}

// SharesChunk is a way to reuse stream data by having
// multiple stream objects connect to one master copy.
// This class is the shared single representation.
// Currently, this is used with decompositions when 
// multiple ranks aggregated the same result, though this
// structure could be more widely applied
class SharedChunks : public AIFStream::Chunks {
public:
    static std::shared_ptr<SharedChunks> make(std::shared_ptr<Chunks> original);
    // clone generates a CloneChunk referencing the
    // shared state.
    std::shared_ptr<AIFStream::Chunks> clone() const;

    std::string getNextChunk(bool& isFinal) override;
    std::string getChunk(int idx, bool& isFinal);
private:
    SharedChunks(std::shared_ptr<AIFStream::Chunks> original);
    std::string getNextChunkOrig(bool& isFinal);

    std::shared_ptr<AIFStream::Chunks> m_orig;
    std::vector<std::string> m_data;
    bool m_readFinal = false;
    std::vector<size_t> m_idcs;
    std::weak_ptr<SharedChunks> m_self;
};

// CloneChunks is a single "view" into a SharedChunks object.
// It will act like it has a unique copy of the shared data.    
class CloneChunks : public AIFStream::Chunks {
public:
    CloneChunks(std::shared_ptr<SharedChunks> parent, int idx);
    std::string getNextChunk(bool& isFinal) override;
    std::shared_ptr<AIFStream::Chunks> clone() const;

private:
    std::shared_ptr<SharedChunks> m_parent;
    int m_idx;
};

std::shared_ptr<SharedChunks> SharedChunks::make(std::shared_ptr<Chunks> original) {
    auto x = std::shared_ptr<SharedChunks>{new SharedChunks{original}};
    x->m_self = x;  // I need to create shared_ptrs to self 
    return x;
}
SharedChunks::SharedChunks(std::shared_ptr<AIFStream::Chunks> original)
    : m_orig(original)
{
    // reserve an index slot for the SharedChunks object itself
    m_idcs.push_back(0);
}

std::string SharedChunks::getNextChunkOrig(bool& isFinal) {
    return m_orig->getNextChunk(isFinal);
}

std::string SharedChunks::getNextChunk(bool& isFinal) {
    // index 0 is reserved for use by the ShareChunks object itself
    return getChunk(0, isFinal);
}

std::shared_ptr<AIFStream::Chunks> SharedChunks::clone() const {
    const_cast<SharedChunks*>(this)->m_idcs.push_back(0);
    return std::make_shared<CloneChunks>(m_self.lock(), m_idcs.size()-1);
}
// Retrieve the next chunk of data for one of the CloneChunks children
// using this object.
std::string SharedChunks::getChunk(int idx, bool& isFinal) {
    if (auto si=m_idcs[idx]++; si < m_data.size()) {
        // The chunk has already been retrived from the back end
        isFinal = (m_readFinal && si+1 == m_data.size());
        if (std::none_of(m_idcs.begin(), m_idcs.end(), [si](auto x) { return x<=si; })) {
            return std::move(m_data[si]); // last use for this one
        } else {
            return m_data[si]; // there is another...
        }
    } else if (!m_readFinal) {
        // we need to fetch the next chunk
        m_data.push_back(getNextChunkOrig(m_readFinal));
        isFinal = m_readFinal;
        return m_data.back();
    } else {
        throw std::runtime_error("data is truncated");
    }
}

CloneChunks::CloneChunks(std::shared_ptr<SharedChunks> parent, int idx) :
    m_parent(parent), m_idx(idx) {}
std::string CloneChunks::getNextChunk(bool& isFinal) {
    return m_parent->getChunk(m_idx, isFinal);
}
std::shared_ptr<AIFStream::Chunks> CloneChunks::clone() const {
    return m_parent->clone();
}

struct NullOp { template <class T> T operator()(T x) { return x; } };

// concatenate a series of streams into a single visitor
class ConcatVisitor : public ais::OpVisitor<NullOp> {
public:
    ConcatVisitor(ais::Visitor& out) : ais::OpVisitor<NullOp>{NullOp(), out} {}
    void eof() override {}
};

// a vstream that concatenates the output of a series of streams
class ConcatVStream : public ais::VStream {
public:
    ConcatVStream(std::vector<std::shared_ptr<ais::VStream> > subStrs) :
        m_strs(std::move(subStrs)) {}
    void apply(ais::Visitor& v) override;

private:
    std::vector<std::shared_ptr<ais::VStream> > m_strs;
};

void ConcatVStream::apply(ais::Visitor& v) {
    ConcatVisitor cv{v};
    for (auto subStr : m_strs) {
        subStr->apply(cv);
    }
    v.eof();
}
    
} // namespace

std::shared_ptr<AIFStream::Chunks> AIFStream::Chunks::clone() const {
    throw std::runtime_error("clone is not supported on stream data");
}
 

AIFStream::AIFStream(std::string&& data, bool isPartial) :
    m_chunks{new CachedChunks{std::move(data), isPartial}} {
}


AIFStream::AIFStream(std::vector<std::string>&& data) :
    m_chunks{new CachedChunks{std::move(data)}} {}

AIFStream::AIFStream(std::shared_ptr<Chunks> chunks) : m_chunks(chunks) {}

AIFStream::AIFStream(std::shared_ptr<ais::VStream> vstream) :
    m_vstream(vstream) {}


// concatenate a set of streams
AIFStream::AIFStream(const std::vector<AIFStream*>& subStrs) {
    auto streams = std::vector<std::shared_ptr<ais::VStream>>{};
    auto chunks = std::vector<std::string>{};

    for (auto subStr : subStrs ) {
        if (auto vs = subStr->getVStreamPtr()) {
            streams.push_back(vs);
        } else if (subStr->m_chunks) {
            bool isFinal;
            chunks.emplace_back(subStr->getNextChunk(isFinal));
        }
    }

    if (!chunks.empty()) {
        m_chunks.reset(new CachedChunks{std::move(chunks)});
    } else {
        m_vstream.reset(new ConcatVStream(streams));
    }
}
AIFStream::~AIFStream() {}

// Get the next chunk of AIF data.   Set isFinal to true if this is the last
// chunk of the data object.   The returned AIF is owned by this object and
// is guaranteed to exist until the next call to getNextChunk.
// FIXME - oh just make this a shared pointer for god's sake.
std::string AIFStream::getNextChunk(bool& isFinal)
{
    if (m_chunks) {
        return m_chunks->getNextChunk(isFinal);
    } else {
        throw std::runtime_error("trying to read an uninitialized data stream");
    }

}

// Force the full stream state to be pulled from the back end, potentially
// for re-use.
AIFStream* AIFStream::join() {
    if (m_chunks && !dynamic_cast<CachedChunks*>(m_chunks.get())) {
        std::vector<std::string> data;
        for (bool isFinal = false; !isFinal; ) {
            data.push_back(m_chunks->getNextChunk(isFinal));
        }
        m_chunks.reset(new CachedChunks{std::move(data)});
    }
    return this;
}

AIFStream* AIFStream::clone()
{
    if (m_chunks) {
        if (m_chunks && !std::dynamic_pointer_cast<SharedChunks>(m_chunks)) {
            m_chunks = SharedChunks::make(m_chunks);
        }
        return new AIFStream(m_chunks->clone());
    } else if (m_vstream) {
        return new AIFStream(m_vstream);
    } else {
        throw std::runtime_error("cannot clone an empty data stream");
    }
}

namespace {

// Low level interface for reading primitives off of the data stream.
// Note.   This is currently implemented just dropping the string
// representations, which is far from optimal, but made debugging the
// process very easy.   I expect to redo with cap 'n proto sooner
// or later.
class AIFStreamReadBuffer : public ais::ReadBuffer {
public:
    AIFStreamReadBuffer(std::shared_ptr<AIFStream::Chunks> chunks) :
        m_chunks(chunks), m_final(false) {}
    int readInt() override;
    long readLong() override;
    long long readLongLong() override;
    float readFloat() override;
    double readDouble() override;
    long double readLongDouble() override;
    std::string readString() override;
    bool hasMoreData() override;

private:
    void getMore();
    void checkForError();

    template<typename  T>
    inline T read() {
        auto c = m_in.peek();
        if (c == EOF) {
            getMore();
        }
        checkForError();
        auto val = T{};
        m_in >> val;
        return val;
    }
    
    std::shared_ptr<AIFStream::Chunks> m_chunks;
    bool m_final;
    std::istringstream m_in;
};



void AIFStreamReadBuffer::getMore() {
    if (!m_final) {
        m_in.str(m_chunks->getNextChunk(m_final));
        m_in.clear();
    } else {
        throw std::runtime_error("result data is truncated");
    }
}

void AIFStreamReadBuffer::checkForError() {
    auto c = m_in.peek();
    if (c == ' ') {
        c = m_in.get();
        c = m_in.peek();
    }
    if (c == '@') {
        // An error, e.g. <optimized out> sent from the back-end
        c = m_in.get();
        auto err = std::string{};
        m_in >> std::quoted(err);
        throw ais::GdbError(err);
    }
}


int AIFStreamReadBuffer::readInt() { return read<int>(); }
long AIFStreamReadBuffer::readLong() { return read<long>(); }
long long AIFStreamReadBuffer::readLongLong() { return read<long long>(); }
float AIFStreamReadBuffer::readFloat() { return read<float>(); }
double AIFStreamReadBuffer::readDouble() { return read<double>(); }
long double AIFStreamReadBuffer::readLongDouble() { return read<long double>(); }
std::string AIFStreamReadBuffer::readString() {
    checkForError();
    
    auto val = std::string{};
    while (!(m_in >> std::quoted(val))) getMore();
    return val;
}

bool AIFStreamReadBuffer::hasMoreData() {
    return !m_final || (m_in && m_in.peek() != EOF);
}
} // anonymous namespace

// creates a new aifstream and transfers ownership of the AIF retrieved from
// getNextChunk. 
// preserves truncation info in the new stream.
AIFStream* AIFStream::createStreamFromNextChunk(bool &isFinal) {
    // getNextChunk will set internal truncation data
    try {
        auto chunk = getNextChunk(isFinal);
        return new AIFStream(std::move(chunk), !isFinal);
    } catch (dbgevent_t* e) {
        throw e;
    } catch (...) {
        throw NewErrorEvent(DBGERR_INTERNAL, "value evaluation failed");
    }
}

// get an integer value from an AIF that is supposed to be an integral value
int AIFStream::toInt(int& val) {
    // work off a clone so the stream can be reconsumed
    auto x = std::unique_ptr<AIFStream>(clone());    

    try {
        auto gotVal = bool{false};
        auto f = ais::FVisitor([&val, &gotVal](auto v) {
                                   if constexpr (std::is_arithmetic<decltype(v)>::value) {
                                           val = v;
                                           gotVal = true;
                                       }});
        x->getVStream().apply(f);
        return gotVal ? 0 : -1;
    } catch (std::exception& err) {
        return -1;
    }
}

int AIFStream::isZero(int& val)
{
    // I think this one is always cloned directly
    try {
        val = true;
        auto f = ais::FVisitor(
            [&val](auto v) {
                if constexpr (std::is_arithmetic<decltype(v)>::value) {
                        val = val && !v;
                } else {
                    val = val && v.empty();
                }});
        getVStream().apply(f);
        return 0;
    } catch (std::exception& err) {
        return -1;
    }
}

AIFStream* AIFStream::fromInt(int val) {
    return new AIFStream{ais::SingleValueStream<ais::Int,int>::make(val)};
}

AIFStream* AIFStream::fromDouble(double val) {
    return new AIFStream{ais::SingleValueStream<ais::Double,double>::make(val)};
}

AIFStream* AIFStream::fromString(const char* val) {
    return fromString(std::string{val});
}

AIFStream* AIFStream::fromString(const std::string& val) {
    return new AIFStream{ais::SingleValueStream<ais::String,std::string>::make(val)};
}

AIFStream* AIFStream::fromLiteral(const std::string& val) {
    return new AIFStream{ais::SingleValueStream<ais::Literal,std::string>::make(val)};
}

void AIFStream::print(std::ostream& os) {
    ais::print(getVStream(), "", "", os);
}

void AIFStream::print(std::ostream& os, const std::string& label, const std::string& format) {
    ais::print(getVStream(), label, format, os);
}


template <class T>
inline AIFStream* AIFStreamFromBinaryOp(AIFStream* a, AIFStream* b, T op) {
    auto strOp = [](const std::string& a, const std::string&) { return a; };
    auto binOp = new ais::FBinaryOperator{op, strOp};
    auto vs = ais::makeBinaryOpStream(a->getVStreamPtr(),
                                      b->getVStreamPtr(),
                                      std::unique_ptr<ais::BinaryOperator>(binOp));
    return new AIFStream{vs};
}

AIFStream* AIFStream::fromBinOp(AIFStream* a, const std::string& op, AIFStream* b) {
    if (op == "+") {
        return AIFStreamFromBinaryOp(a,b,[](auto x, auto y) { return x+y; });
    } else if (op == "-") {
        return AIFStreamFromBinaryOp(a,b,[](auto x, auto y) { return x-y; });
    } else if (op == "*") {
        return AIFStreamFromBinaryOp(a,b,[](auto x, auto y) { return x*y; });
    } else if (op == "/") {
        return AIFStreamFromBinaryOp(a,b,[](auto x, auto y) { if (y != 0) return x/y; else return y/1; });
    } else if (op == "%") {
        return AIFStreamFromBinaryOp(a,b,[](auto x, auto y) {
                                             if constexpr ( std::is_integral<decltype(x)>::value) {
                                                     return x % y;
                                                 } else {
                                                 return fmod(x,y);
                                             }});
    } else if (op == "&&") {
        return AIFStreamFromBinaryOp(a,b,[](auto x, auto y) { return x && y; });
    } else if (op == "||") {
        return AIFStreamFromBinaryOp(a,b,[](auto x, auto y) { return x || y; });
    } else {
        return nullptr;
    }
}

AIFStream* AIFStream::fromUnaryOp(const std::string& op, AIFStream* a) {
    if (op == "-") {
        // templatize this when there is more than one supported operator
        auto cloneA = new AIFStream(std::move(*a));
        auto negOp = [](auto x) {
                         if constexpr (std::is_arithmetic_v<decltype(x)>) return -x; else return x;};
        auto negStream = makeUnaryOpStream(cloneA->getVStreamPtr(), negOp);
        return new AIFStream(negStream);
    } else {
        return nullptr;
    }
}

// Class for accumulating differenced during a comparison operation.
// In the future we could provide implementations to return the
// largest differences, or do some statistics, but for version 1.0
// we'll just return the first 100 differences.
class AIFStream::Difference::Impl : public ais::Difference {
public:
    void add(const std::string& idx, long int a, long int b) override;
    void add(const std::string& idx, long double a, long double b) override;
    void add(const std::string& idx, const std::string& a, const std::string& b) override;
    std::string getResult() override;
private:
    const char* doPrint();
    static const int MAX_VALS = 100;
    std::ostringstream m_difference;
    int m_numVals = 0;
};

const char* AIFStream::Difference::Impl::doPrint() {
    if (++m_numVals <= MAX_VALS)
        return m_numVals == 1 ? "" : "\n";
    else
        return nullptr;
}
    

void AIFStream::Difference::Impl::add(const std::string& idx, long int a, long int b) {
    if (auto p = doPrint()) m_difference << p << idx << (idx.empty() ? "" : ":") << b-a;
}
void AIFStream::Difference::Impl::add(const std::string& idx, long double a, long double b) {
    if (auto p = doPrint()) m_difference << p << idx << (idx.empty() ? "" : ":") << b-a;
}

void AIFStream::Difference::Impl::add(const std::string& idx,
                                      const std::string& a, const std::string& b) {
    if (auto p = doPrint()) {
        m_difference << p << idx << (idx.empty() ? "" : ":") << a << " vs " << b;
    }
}

std::string AIFStream::Difference::Impl::getResult() {
    if (m_numVals > MAX_VALS) {
        m_difference << "\n... and " << m_numVals - MAX_VALS << " more...";
        m_numVals = MAX_VALS; // don't want to add the "and more" every time it's called
    }
    return m_difference.str();
}

AIFStream::Difference::Difference() : m_impl(new Impl) {}
AIFStream::Difference::~Difference() = default;
std::string AIFStream::Difference::getResult() { return m_impl->getResult(); }
AIFStream* AIFStream::Difference::getResultAIF() { return fromLiteral(getResult()); }
AIFStream::Difference::Impl& AIFStream::Difference::getAis() { return *m_impl; }

int AIFStream::compare(AIFStream* a, AIFStream* b, int* res, Difference& diff,
                       double* lo, double* hi, std::string& errMsg)
{
    auto& valA = a->getVStream();
    auto& valB = b->getVStream();

    try {
        auto cmp = ais::compare(valA, valB, diff.getAis(), lo ? *lo : 0.0, hi ? *hi : 0.0);
        using namespace ais;

        bool lt = cmp[LT], ltw = cmp[WARN_LT], eq = cmp[EQ],
            gt = cmp[GT], gtw = cmp[WARN_GT], nc = cmp[NOT_COMPARABLE];

        bool ltx = (lt || ltw);
        bool gtx = (gt || gtw);

        // LOOKAT: this return value is used to report <, <=, >, >=, but it doesn't
        // account for it being a combination of multiple values, so there isn't a value
        // for less than AND greater than
        if (nc) *res = 3;
        else if (eq && !ltx && !gtx) *res = 0; // {1, 1} == {1, 1}
        else if (ltw && !lt && !gt) *res = -2; // {.99, 1, 1.01} ~= {1, 1, 1} // ambiguity if ltw && gtw  (otherwise !gtx)
        else if (lt && !gtx) *res = -1;        // {0, 1} < {1, 1}
        else if (gtw && !gt && !ltx) *res = 2; // {1.01, 1} ~>= {1, 1}
        else if (gt && !ltx) *res = 1;         // {2, 1} > {1, 1}
        else *res = 1;  // not ideal, but this might cause the least trouble.
        return 0;
    } catch (std::exception& e) {
        errMsg = e.what();
        *res = 3;
        return -1;
    }
}

CompareResult
AIFStream::compareOp(AIFStream* a, const std::string& op, AIFStream* b,
                     Difference& diff,
                     double* lo, double* hi, std::string& errMsg)
{
    const auto PASS = CompareResult::Pass, FAIL = CompareResult::Fail,
        WARN = CompareResult::Warn, INVALID = CompareResult::Invalid;

    auto& valA = a->getVStream();
    auto& valB = b->getVStream();

    try {
        auto cmp = ais::compare(valA, valB, diff.getAis(), lo ? *lo : 0.0, hi ? *hi : 0.0);
        using namespace ais;

        bool lt = cmp[LT], ltw = cmp[WARN_LT], eq = cmp[EQ],
            gt = cmp[GT], gtw = cmp[WARN_GT], nc = cmp[NOT_COMPARABLE];

        bool ltx = (lt || ltw);
        bool gtx = (gt || gtw);

        // LOOKAT: this return value is used to report <, <=, >, >=, but it doesn't
        // account for it being a combination of multiple values, so there isn't a value
        // for less than AND greater than

        if (nc) {
            errMsg = "Comparison cannot be applied to this type.";
            return INVALID;
        }
        else if (op == "==" || op == "=") {
            if (lt || gt) return FAIL;
            else if (ltw || gtw) return WARN;
            else return PASS;
        } else if (op == "!=") {
            return (lt || gt ? PASS : FAIL);  // is this correct w.r.t ltw and gtw
        } else if (op == "<") {
            return (!eq && !gtx ? PASS : FAIL);
        } else if (op == "<=") {
            return (!gtx ? PASS : FAIL);
        } else if (op == ">=") {
            return (!ltx ? PASS : FAIL);
        } else if (op == ">") {
            return (!eq && !ltx ? PASS : FAIL);
        } else {
            return INVALID;
        }
    } catch (std::exception& e) {
        errMsg = e.what();
        return INVALID;
    }
}


bool AIFStream::compareEqual(AIFStream& a, AIFStream& b) {
    AIFStream::Difference diff; // unused
    std::string error;          // unused
    int res = -1;

    AIFStream::compare(&a, &b, &res, diff, nullptr, nullptr, error);

    return res == 0;
};

AIFStream::AIFStream(std::shared_ptr<ais::DecompIterator> blockIter,
                     const std::vector<AIFStream*>& data)
{
    // If different parts of the decomposition have the same result, then
    // data will contain duplicates.  We arrange to have multiple
    // readers access them
    using ChunksPtr = std::shared_ptr<AIFStream::Chunks>;
    auto smap = std::map<AIFStream*,ChunksPtr>{};

    // gather the set of data objects, create the SharedChunks the
    // second time we see a data set.
    for (auto it = data.begin(); it != data.end(); ++it) {
        auto chunks = (*it)->m_chunks;        
        if ( auto [loc,added] = smap.insert(std::pair{*it,chunks}); !added) {
            if (!dynamic_cast<SharedChunks*>(loc->second.get())) {
                loc->second = SharedChunks::make(chunks);
            }
        }
    }

    // Replace any duplicated results with CloneChunks
    auto subReaders = std::vector<std::unique_ptr<ais::Reader>>{};
    for (auto it = data.begin(); it != data.end(); ++it) {
        auto chunks = smap[*it];
        if (dynamic_cast<SharedChunks*>(chunks.get())) {
            chunks = chunks->clone();
        }

        auto buffer = std::make_shared<AIFStreamReadBuffer>(chunks);
        subReaders.push_back(std::make_unique<ais::BufferReader>(buffer));
    }

    m_reader.reset(new ais::DecompReader{std::move(subReaders), blockIter});
}

std::shared_ptr<ais::VStream> AIFStream::getVStreamPtr() {
    if (m_vstream) return m_vstream;
    
    if (!m_reader) {
        auto buffer = std::make_shared<AIFStreamReadBuffer>(m_chunks);
        m_reader = std::make_unique<ais::BufferReader>(buffer);
    }

    m_vstream.reset(new ais::ReaderStream(std::move(m_reader)));
    return m_vstream;
}


ais::VStream& AIFStream::getVStream() {
    return *getVStreamPtr();
}
