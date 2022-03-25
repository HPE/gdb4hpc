// Copyright 2020-2022 Hewlett Packard Enterprise Development LP.

// Classes for defining a sequential stream of data accessed via the AIF interface.
#include "AIFStream.h"
#include "ais.h"
#include <string.h>
#include <stdarg.h>
#include <iomanip>
#include <cmath>

void rd_unimplemented(const char* func) {
    fprintf(stderr, "UNIMPLEMENTED %s\n", func);
}

namespace {
// CachedSlices keep the values when a join operation forces all the
// data to be pulled to down to the front end.   The stream
// can then be processed as usual, or copied for reuse
class CachedSlices : public AIFStream::Slices {
public:
    CachedSlices(std::string&& data);
    CachedSlices(std::vector<std::string>&& data);
    std::string getNextSlice(bool& isFinal) override;
    std::shared_ptr<AIFStream::Slices> clone() const;
private:
    std::vector<std::string> m_data;
    size_t m_idx = 0;
};
CachedSlices::CachedSlices(std::string&& data) {
    m_data.push_back(std::move(data));
}
CachedSlices::CachedSlices(std::vector<std::string>&& data) : m_data{std::move(data)} {}

std::string CachedSlices::getNextSlice(bool& isFinal) {
    if (m_idx < m_data.size()) {
        isFinal = (m_idx+1 == m_data.size());
        return std::move(m_data[m_idx++]);
    } else {
        throw std::runtime_error("result data is truncated");
    }
}
std::shared_ptr<AIFStream::Slices> CachedSlices::clone() const {
    std::vector cpData = m_data; // copy the data slices
    return std::make_shared<CachedSlices>(std::move(cpData));
}

// SharesSlice is a way to reuse stream data by having
// multiple stream objects connect to one master copy.
// This class is the shared single representation.
// Currently, this is used with decompositions when 
// multiple ranks aggregated the same result, though this
// structure could be more widely applied
class SharedSlices : public AIFStream::Slices {
public:
    static std::shared_ptr<SharedSlices> make(std::shared_ptr<Slices> original);
    // clone generates a CloneSlice referencing the
    // shared state.
    std::shared_ptr<AIFStream::Slices> clone() const;

    std::string getNextSlice(bool& isFinal) override;
    std::string getSlice(int idx, bool& isFinal);
private:
    SharedSlices(std::shared_ptr<AIFStream::Slices> original);
    std::shared_ptr<AIFStream::Slices> m_orig;
    std::vector<std::string> m_data;
    bool m_readFinal = false;
    std::vector<size_t> m_idcs;
    std::weak_ptr<SharedSlices> m_self;
};

// CloneSlices is a single "view" into a SharedSlices object.
// It will act like it has a unique copy of the shared data.    
class CloneSlices : public AIFStream::Slices {
public:
    CloneSlices(std::shared_ptr<SharedSlices> parent, int idx);
    std::string getNextSlice(bool& isFinal) override;
    std::shared_ptr<AIFStream::Slices> clone() const;

private:
    std::shared_ptr<SharedSlices> m_parent;
    int m_idx;
};

std::shared_ptr<SharedSlices> SharedSlices::make(std::shared_ptr<Slices> original) {
    auto x = std::shared_ptr<SharedSlices>{new SharedSlices{original}};
    x->m_self = x;  // I need to create shared_ptrs to self 
    return x;
}
SharedSlices::SharedSlices(std::shared_ptr<AIFStream::Slices> original)
    : m_orig(original) {}

std::string SharedSlices::getNextSlice(bool& isFinal) {
    return m_orig->getNextSlice(isFinal);
}
std::shared_ptr<AIFStream::Slices> SharedSlices::clone() const {
    const_cast<SharedSlices*>(this)->m_idcs.push_back(0);
    return std::make_shared<CloneSlices>(m_self.lock(), m_idcs.size()-1);
}
// Retrieve the next slice of data for one of the CloneSlices children
// using this object.
std::string SharedSlices::getSlice(int idx, bool& isFinal) {
    if (auto si=m_idcs[idx]++; si < m_data.size()) {
        // The slice has already been retrived from the back end
        isFinal = (m_readFinal && si+1 == m_data.size());
        if (std::none_of(m_idcs.begin(), m_idcs.end(), [si](auto x) { return x<=si; })) {
            return std::move(m_data[si]); // last use for this one
        } else {
            return m_data[si]; // there is another...
        }
    } else if (!m_readFinal) {
        // we need to fetch the next slice
        m_data.push_back(getNextSlice(m_readFinal));
        isFinal = m_readFinal;
        return m_data.back();
    } else {
        throw std::runtime_error("data is truncated");
    }
}

CloneSlices::CloneSlices(std::shared_ptr<SharedSlices> parent, int idx) :
    m_parent(parent), m_idx(idx) {}
std::string CloneSlices::getNextSlice(bool& isFinal) {
    return m_parent->getSlice(m_idx, isFinal);
}
std::shared_ptr<AIFStream::Slices> CloneSlices::clone() const {
    return m_parent->clone();
}
    
} // namespace

std::shared_ptr<AIFStream::Slices> AIFStream::Slices::clone() const {
    throw std::runtime_error("clone is not supported on stream data");
}
 

AIFStream::AIFStream(std::string&& data, bool isPartial) :
    m_slices{new CachedSlices{std::move(data)}} {
    /* isPartial was never used */
}


AIFStream::AIFStream(std::vector<std::string>&& data) :
    m_slices{new CachedSlices{std::move(data)}} {}

AIFStream::AIFStream(std::shared_ptr<Slices> slices) : m_slices(slices) {}

AIFStream::AIFStream(std::shared_ptr<ais::VStream> vstream) :
    m_vstream(vstream) {}

AIFStream::~AIFStream() {}

// Get the next slice of AIF data.   Set isFinal to true if this is the last
// slice of the data object.   The returned AIF is owned by this object and
// is guaranteed to exist until the next call to getNextSlice.
// FIXME - oh just make this a shared pointer for god's sake.
std::string AIFStream::getNextSlice(bool& isFinal)
{
    if (m_slices) {
        return m_slices->getNextSlice(isFinal);
    } else {
        throw std::runtime_error("trying to read an uninitialized data stream");
    }

}

// Force the full stream state to be pulled from the back end, potentially
// for re-use.
AIFStream* AIFStream::join() {
    if (m_slices && !dynamic_cast<CachedSlices*>(m_slices.get())) {
        std::vector<std::string> data;
        for (bool isFinal = false; !isFinal; ) {
            data.push_back(m_slices->getNextSlice(isFinal));
        }
        m_slices.reset(new CachedSlices{std::move(data)});
    }
    return this;
}

AIFStream* AIFStream::clone()
{
    // There's only one channel for retrieval from the back end, so
    // join first.
    join();

    // and clone
    if (m_slices) {
        return new AIFStream(m_slices->clone());
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
    AIFStreamReadBuffer(std::shared_ptr<AIFStream::Slices> slices) :
        m_slices(slices), m_final(false) {}
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

    template<typename  T>
    inline T read() {
        auto c = m_in.peek();
        if (c == EOF) {
            getMore();
            c = m_in.peek();
        }
        if (c == '@') {
            // An error, e.g. <optimized out> sent from the back-end
            c = m_in.get();
            auto err = std::string{};
            m_in >> std::quoted(err);
            throw ais::GdbError(err);
        }
        
        auto val = T{};
        m_in >> val;
        return val;
    }
    
    std::shared_ptr<AIFStream::Slices> m_slices;
    bool m_final;
    std::istringstream m_in;
};



void AIFStreamReadBuffer::getMore() {
    if (!m_final) {
        m_in.str(m_slices->getNextSlice(m_final));
        m_in.clear();
    } else {
        throw std::runtime_error("result data is truncated");
    }
}

int AIFStreamReadBuffer::readInt() { return read<int>(); }
long AIFStreamReadBuffer::readLong() { return read<long>(); }
long long AIFStreamReadBuffer::readLongLong() { return read<long long>(); }
float AIFStreamReadBuffer::readFloat() { return read<float>(); }
double AIFStreamReadBuffer::readDouble() { return read<double>(); }
long double AIFStreamReadBuffer::readLongDouble() { return read<long double>(); }
std::string AIFStreamReadBuffer::readString() {
    auto val = std::string{};
    while (!(m_in >> std::quoted(val))) getMore();
    return val;
}

bool AIFStreamReadBuffer::hasMoreData() {
    return !m_final || (m_in && m_in.peek() != EOF);
}
} // anonymous namespace

// creates a new aifstream and transfers ownership of the AIF retrieved from
// getNextSlice(STEAL).
// preserves truncation info in the new stream.
AIFStream* AIFStream::createStreamFromNextSlice(bool &isFinal) {
    // getNextSlice will set internal truncation data
    auto slice = getNextSlice(isFinal);
    return new AIFStream(std::move(slice), !isFinal);
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


AIFStream* AIFStream::fromMultiThreadResult(const char* val) {
    return new AIFStream{
        ais::SingleValueStream<ais::MultiThreadResult,std::string>::make(val)};
}

// Little adapter that takes C++ stream style input and sends it
// to a C file handle
class FileAdapter : public std::streambuf
{
public:
    FileAdapter(FILE* f) : m_f(f) {}
    std::streamsize xsputn(const char* s, std::streamsize n) override;
    int overflow(int c = EOF) override;
private:
    FILE* m_f;
};

std::streamsize FileAdapter::xsputn(const char* s, std::streamsize n) { return fwrite(s,1,n,m_f); }
int FileAdapter::overflow(int c) { return putc(c, m_f); }

int AIFStream::print(FILE* f)
{
    return print(f,"");
}


int AIFStream::print(FILE* f, const std::string& label)
{
    FileAdapter fa(f);
    std::ostream os(&fa);
    try {
        ais::print(getVStream(), label, os);
        return 0;
    } catch (...) {
        return -1;
    }
}

void AIFStream::print(std::ostream& os) {
    ais::print(getVStream(), "", os);
}

template <class T>
inline AIFStream* AIFStreamFromBinaryOp(AIFStream* a, AIFStream* b, T op) {
    auto strOp = [](const std::string& a, const std::string& b) { return a; };
    auto binOp = new ais::FBinaryOperator{op, strOp};
    auto vs = ais::makeBinaryOpStream(a->getVStreamPtr(),
                                      b->getVStreamPtr(),
                                      std::unique_ptr<ais::BinaryOperator>(binOp));
    return new AIFStream{vs};
}

AIFStream* AIFStream::add(AIFStream* a, AIFStream* b) {
    return AIFStreamFromBinaryOp(a,b,[](auto x, auto y) { return x+y; });
}
AIFStream* AIFStream::sub(AIFStream* a, AIFStream* b) {
    return AIFStreamFromBinaryOp(a,b,[](auto x, auto y) { return x-y; });
}
AIFStream* AIFStream::mul(AIFStream* a, AIFStream* b) {
    return AIFStreamFromBinaryOp(a,b,[](auto x, auto y) { return x*y; });
}
AIFStream* AIFStream::div(AIFStream* a, AIFStream* b) {
    return AIFStreamFromBinaryOp(a,b,[](auto x, auto y) { if (y != 0) return x/y; else return y/1; });
}
AIFStream* AIFStream::rem(AIFStream* a, AIFStream* b) {
    return AIFStreamFromBinaryOp(a,b,[](auto x, auto y) {
                                         if constexpr ( std::is_integral<decltype(x)>::value) {
                                                 return x % y;
                                             } else {
                                             return fmod(x,y);
                                         }});
}
AIFStream* AIFStream::And(AIFStream* a, AIFStream* b) {
    return AIFStreamFromBinaryOp(a,b,[](auto x, auto y) { return x && y; });
}
AIFStream* AIFStream::Or(AIFStream* a, AIFStream* b) {
    return AIFStreamFromBinaryOp(a,b,[](auto x, auto y) { return x || y; });
}
AIFStream* AIFStream::neg(AIFStream* a) {
    auto cloneA = new AIFStream(std::move(*a));
    auto negOp = [](auto x) {
                     if constexpr (std::is_arithmetic_v<decltype(x)>) return -x; else return x;};
    auto negStream = makeUnaryOpStream(cloneA->getVStreamPtr(), negOp);
    return new AIFStream(negStream);
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
        else if (eq && !ltx && !gtx) *res = 0;
        else if (ltw && !lt && !gt) *res = -2;   // ambiguity if ltw && gtw  (otherwise !gtx)
        else if (lt && !eq && !gtx) *res = -1;
        else if (gtw && !gt && !ltx) *res = 2;
        else if (gt && !eq && !ltx) *res = 1;
        else *res = 1;  // not ideal, but this might cause the least trouble.
        return 0;
    } catch (std::exception& e) {
        errMsg = e.what();
        *res = 3;
        return -1;
    }
}

AIFStream::AIFStream(std::shared_ptr<ais::DecompIterator> blockIter,
                     const std::vector<AIFStream*>& data)
{
    // If different parts of the decomposition have the same result, then
    // data will contain duplicates.  We arrange to have multiple
    // readers access them
    using SlicesPtr = std::shared_ptr<AIFStream::Slices>;
    auto smap = std::map<AIFStream*,SlicesPtr>{};

    // gather the set of data objects, create the SharedSlices the
    // second time we see a data set.
    for (auto it = data.begin(); it != data.end(); ++it) {
        auto slices = (*it)->m_slices;        
        if ( auto [loc,added] = smap.insert(std::pair{*it,slices}); !added) {
            if (!dynamic_cast<SharedSlices*>(loc->second.get())) {
                loc->second = SharedSlices::make(slices);
            }
        }
    }

    // Replace any duplicated results with CloneSlices
    auto subReaders = std::vector<std::unique_ptr<ais::Reader>>{};
    for (auto it = data.begin(); it != data.end(); ++it) {
        auto slices = smap[*it];
        if (dynamic_cast<SharedSlices*>(slices.get())) {
            slices = slices->clone();
        }

        auto buffer = std::make_shared<AIFStreamReadBuffer>(slices);
        subReaders.push_back(std::make_unique<ais::BufferReader>(buffer));
    }

    m_reader.reset(new ais::DecompReader{std::move(subReaders), blockIter});
}

std::shared_ptr<ais::VStream> AIFStream::getVStreamPtr() {
    if (m_vstream) return m_vstream;
    
    if (!m_reader) {
        auto buffer = std::make_shared<AIFStreamReadBuffer>(m_slices);
        m_reader = std::make_unique<ais::BufferReader>(buffer);
    }

    m_vstream.reset(new ais::ReaderStream(std::move(m_reader)));
    return m_vstream;
}


ais::VStream& AIFStream::getVStream() {
    return *getVStreamPtr();
}
