/******************************************************************************
 * libmi to aif conversion routines
 *
 * Copyright 2011-2023 Hewlett Packard Enterprise Development LP.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Hewlett Packard Enterprise Development LP.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *
 ******************************************************************************/

#include "config.h"

#include <iomanip>
#include <regex>
#include <cctype>
#include <algorithm>
#include <limits>

#include "dbgevent.h"
#include "dbgsrv_defs.h"
#include "dbgsrv_libmi2aif.h"
#include "defs.h"
#include "parse.h"
#include "MIOOBRecord.h"
#include "blockmap.h"
#include "mi_utils.h"

#include "unittest_macros.h"

// Used to fix infinite loop issue for recursive calls to whatis
extern int clientPrintElements;

const std::string ANONYMOUS_NAMESPACE = "(anonymous namespace)::";

namespace { class SubArray; }

class LazyMIVar {
    // handle for an MIVar which defers var, type, and expression calculations
private:
    Mi2Aif& m_mi;

    std::shared_ptr<MIVar> m_var;
    std::shared_ptr<ais::Type> m_type;
    std::string m_expr;
    std::shared_ptr<SubArray> m_subArray;

public:
    LazyMIVar(Mi2Aif& mi, std::string expr);
    LazyMIVar(Mi2Aif& mi, std::shared_ptr<MIVar> var);

    std::shared_ptr<MIVar> getVar();
    std::shared_ptr<ais::Type> getType();
    std::string getExpr();

    char *getArrayDataString();

private:
    std::string arrayToFnCall(const std::string& expr);
    std::string getGdbType(const std::string& expr);
    std::string sendWhatIs(const std::string& expr);
    void getSliceType(const std::string& expr, const std::string& metaInfo);
    std::string getSharedPtrType(const std::string& type);
    std::string getUniquePtrType(const std::string& type);    
};

namespace {
// Class that handles the special slicing duties related to implementing
// from..to in for C/C++ containers
class SubArray {
public:
    virtual ~SubArray() {}
    static std::pair<std::string,bool> removeRngs(Mi2Aif& mi, const std::string& expr);
    std::shared_ptr<ais::Type> getType() { return m_type; }
    virtual char* getArrayDataString() = 0;
    virtual const  mdspan& getSpan() = 0;
    virtual bool mayHaveOffset() const { return false; };
    
protected:
    SubArray(Mi2Aif& mi, const std::string& expr);
    const static int UNDEFINED = INT_MIN;

    void getRngs(const std::vector<std::string>& idcs);
    int getIdx(const std::string& s, int defVal) const;
    void resolveOpenRngs();


    Mi2Aif& m_mi;
    std::string m_baseExpr;
    std::vector<std::pair<int,int>> m_rngs;
    std::shared_ptr<ais::Type> m_type;
    std::unique_ptr<mdspan> m_span;
};

class CSubArray : public SubArray {
public:
    CSubArray(Mi2Aif& mi, const std::string& expr);
    char* getArrayDataString() override;
    const mdspan& getSpan() override;

private:
    void makeArrayType(std::shared_ptr<ais::Type> baseType,
                       const std::vector<std::pair<int,int>>& rngs);

    std::vector<std::string> parse(const std::string& type);

};

class KokkosSubArray : public SubArray {
public:
    static std::unique_ptr<KokkosSubArray> get(Mi2Aif& mi, const std::string& expr,
                                               const std::string& metaInfo,
                                               std::shared_ptr<ais::KokkosView> view);
    char* getArrayDataString() override;
    const mdspan& getSpan() override;

private:
    KokkosSubArray(Mi2Aif& mi, std::shared_ptr<ais::KokkosView> type, const std::string& expr, const std::string& baseExpr);
    void parse(const std::string& type);
    static bool getViewLens(Mi2Aif& mi, const std::string& expr, ais::KokkosView& view);
    void makeArrayType(std::shared_ptr<ais::Type> baseType,
                       const std::vector<std::pair<int,int>>& rngs);

    std::shared_ptr<ais::KokkosView> m_kv;
};

class RajaSubArray : public SubArray {
public:
    static std::unique_ptr<RajaSubArray> get(Mi2Aif& mi, const std::string& expr1,
                                             std::string_view meta_info,
                                             const std::shared_ptr<ais::RajaView> raja_view);
                                         
    char* getArrayDataString() override;
    const mdspan& getSpan() override;
    bool mayHaveOffset() const override;

    struct Tester;
    friend Tester;
private:
    // throws std::runtime_error on failure
    RajaSubArray(Mi2Aif& mi, const std::string& base_expr, const std::string& expr,
        std::string_view meta_info, const std::shared_ptr<ais::RajaView> view);

    struct RajaIndex {
        int position;
        int index;
        const char* text = nullptr;
        size_t text_len = 0;
    };

    static std::vector<RajaIndex> generateIndices(const std::vector<std::pair<int, int>>& ranges,
                                                  const mdspan& span);

    std::string m_base_expr;
    const std::shared_ptr<ais::RajaView> m_view;
};

struct RajaSubArray::Tester {
    // exposes private members of RajaSubArray for unit tests
    static constexpr auto generateIndices = &RajaSubArray::generateIndices;
};

}

// Implementation for translating primitive data objects into a data to be sent
// across the pipe.
// Note that the current implementation is just based on ostringstream, which
// is great for debugging but is definitely non-optimal.   It is expected this
// will be replaced with a cap'n proto based implementation at some point in the
// future.
class WriteBuffer : public ais::WriteBuffer {
public:
    WriteBuffer() {}
    void writeInt(int val) override;
    void writeLong(long val) override;
    void writeLongLong(long long val) override;
    void writeFloat(float val) override;
    void writeDouble(double val) override;
    void writeLongDouble(long double val) override;
    void writeString(const std::string& str) override;
    void writeError(const std::string& str) override;
    void writePointer(const std::string& type, void* val) override;

    std::string exportResult();
protected:
    std::ostringstream m_out;
};

class MIWriteBuffer : public WriteBuffer {
public:
    MIWriteBuffer(Mi2Aif& mi) : m_mi(mi) {}
    void writePointer(const std::string& type, void* val) override;
private:
    Mi2Aif& m_mi;
};

namespace {

// Data result reported by gdb.
// In the basic form, this is just the string reported on gdb on the
// "print expression", but large arrays be broken into multiple
// "bite-sized" pieces via the Chunker object.
class GdbResult : public ais::InterpretStream {
public:
    GdbResult(const char* buf) : m_buffer{buf}, m_pos{buf}, m_truncater(clientPrintElements) {}
    ~GdbResult() = default;
    const char* get() override { return m_pos; }
    void setPos(const char* pos) override { m_pos = pos; }
    ais::Truncater& truncater() override { return m_truncater; }

    GdbResult(const GdbResult&) = delete;
    GdbResult& operator=(const GdbResult&) = delete;
    
private:
    const char* m_buffer;
    const char* m_pos;
    ais::Truncater m_truncater;
};

// Chunks implementation where the data is created on demand by
// using gdb/mi to evaluate an expression.
class EvalChunks : public AIFStream::Chunks {
public:
    EvalChunks(MISession* session, const std::string& var);
    std::string getNextChunk(bool& isFinal) override;
private:
    void initChunker();
    Mi2Aif m_mi2aif;
    std::string m_var;
    std::unique_ptr<Chunker> m_chunker;
};

} //namespace

// global debug event set by the evaluator
std::unique_ptr<dbgevent_t, decltype(&FreeEvent)> dbg_error{nullptr, FreeEvent};
void throwDbgError(const std::string& what) {
    DVPRINT("%s\n", what.c_str());
    if (!dbg_error)
        dbg_error.reset(NewErrorEvent(DBGERR_INTERNAL, "%s", what.c_str()));
    throw dbg_error.release();
}

// This is the "main" function that should be called in this interface...
// Note the deferred evaluation; the AIFStream may throw an evaluation failure
// when the data is accessed.
AIFStream *GetAIFVar(MISession *session, const char *var) {
    // The actual evaluation is deferred until the AIFStream request
    // the next (i.e. first) chunk.
    if (!var) throw NewErrorEvent(DBGERR_INTERNAL, "null expression");
    auto chunks = std::make_shared<EvalChunks>(session, var);
    return new AIFStream(chunks);
}

// quote typename with spaces to pass to MI
static std::string quoteId(std::string var)
{
    if (var.find(' ') != std::string::npos && var.find('"') == std::string::npos) {
        var = "\"" + var + "\"";
    }
    return var;
}

static void DeleteMIVar(MISession* session, MIVar* mivar)
{
    if (mivar) {
        auto cmd = MIVarDelete(mivar->name);
        SendMICommand(session, cmd);
        MICommandFree(cmd);
        MIVarFree(mivar);
    }
}

static bool typeIsReference(const char* str) {
    if (!str) return false;

    // check for the most common form, e.g. "int&", "double &&"
    for (auto s = str+strlen(str); s != str; ) {
        if (auto c = *--s; c == '&') return true;
        else if (!std::isspace(c)) break;
    }

    // check for the reference to array form, e.g. "int (&)[4]", "double (&&)[4][4]"
    {
        const static std::regex array_ref_re(R"(^.* \(&&?\)(\[\d+\])+)");
        if (std::regex_match(str, array_ref_re)) return true;
    }

    return false;
}

LazyMIVar::LazyMIVar(Mi2Aif& mi, std::string expr) 
    : m_mi{mi}, m_expr{std::move(expr)}
{}

LazyMIVar::LazyMIVar(Mi2Aif& mi, std::shared_ptr<MIVar> var) 
    : m_mi{mi}, m_var{var}
{}

std::string LazyMIVar::arrayToFnCall(const std::string& funcExpr) {
    if (funcExpr.find('[') != std::string::npos) {
        DVPRINT("fixing up \"%s\"; retrying as a function call: ", funcExpr.c_str());
        auto newFuncExpr = funcExpr;
        for (size_t p; (p=newFuncExpr.find("][")) != std::string::npos; ) {
            newFuncExpr.replace(p,2,1,',');
        }
        std::replace(newFuncExpr.begin(), newFuncExpr.end(), '[', '(');
        std::replace(newFuncExpr.begin(), newFuncExpr.end(), ']', ')');

        DVPRINT("%s\n", newFuncExpr.c_str());
        return newFuncExpr;
    }

    // passed expression not possibly a function call
    return "";
}

std::shared_ptr<MIVar> LazyMIVar::getVar() {
    if (m_var) return m_var;

    if (m_expr.empty()) {
        throwDbgError("No expression to create variable with.");
    }

    m_var = m_mi.createMIVar(m_expr.c_str());

    if (!m_var) {
        // the frontend can't tell if "(12)" is a C function call or a
        // fortran array index and thus sends function calls to us
        // as "myFunc[12]" instead of "myFunc(12)"

        if (auto fnCallExpr = arrayToFnCall(m_expr); !fnCallExpr.empty()) {
            dbg_error.reset(); // set in createMIVar above
            m_var = m_mi.createMIVar(fnCallExpr.c_str());
        }
    }

    if (!m_var) {
        throwDbgError("CreateMIVar returned null.\n");
    }

    return m_var;
}

std::string LazyMIVar::getGdbType(const std::string& exp) {
    try {
        auto what = sendWhatIs(exp);
        try {
            sendWhatIs(what);
        } catch (...) {
            // typedef's have confused gdb into a "what" that it doesn't understand
            // try ptype on expression for a second opinion
            DVPRINT("whatis gave an unknown type %s\n", what.c_str());
            auto alias = m_mi.getPTypeAlias(exp, true, true);
            if (!alias.empty() && alias != what) {
                DVPRINT("using ptype type %s\n", alias.c_str());
                what = alias;
            }
        }

        // we need to dig deeper to tease out the dynamic type of a smart pointer
        if (what.starts_with("std::shared_ptr<")) what = getSharedPtrType(what);
        else if (what.starts_with("std::unique_ptr<")) what = getUniquePtrType(what);

        return what;

    } catch (...) {
        // the frontend can't tell if "(12)" is a C function call or a
        // fortran array index and thus sends function calls to us
        // as "myFunc[12]" instead of "myFunc(12)"
        if (auto fnCallExpr = arrayToFnCall(exp); !fnCallExpr.empty()) {
            return sendWhatIs(fnCallExpr);
        } else {
            throw;
        }
    }
}

static std::string getSmartPtrType(const std::string& exprType,
                                   const std::string& realType)
{
    static const auto realTypeRE = std::regex{R"(/\*\s*real type\s*=\s*(.*\S)\s*\*\s*\*/)"};
    auto match = std::smatch{};
    if (std::regex_match(realType, match, realTypeRE)) {
        auto ptrBeg = exprType.find("<");
        auto ptrEnd = findExprMatch(exprType, ptrBeg);
        
        auto rval = exprType.substr(0,ptrBeg+1) + match[1].str() + exprType.substr(ptrEnd);
        DVPRINT("derived type is %s\n", rval.c_str());
        return rval;

    }

    return exprType;
}


// attempt to find the real type of a shared_ptr 
std::string LazyMIVar::getSharedPtrType(const std::string& exprType) {
    auto retVal = exprType;
    try {
        auto ptrBeg = exprType.find("<");
        auto ptrEnd = findExprMatch(exprType, ptrBeg);
        auto ptrType = exprType.substr(ptrBeg+1, ptrEnd-ptrBeg-1);

        auto val = std::unique_ptr<char, decltype(&std::free)>(getArrayDataString(), std::free);;
        if (!val) return exprType;

        if (auto addr = strstr(val.get(), "0x")) {
            auto os = std::ostringstream{};
            os << "(" << ptrType << "*)" << addr;
            auto subType = sendWhatIs(os.str());

            retVal = getSmartPtrType(exprType, subType);
        }

    } catch (...) {}  
    
    
    return retVal;
}

std::string LazyMIVar::getUniquePtrType(const std::string& exprType) {
    auto retVal = exprType;
    try {
        auto val = std::unique_ptr<char, decltype(&std::free)>(getArrayDataString(), std::free);;
        if (!val) return retVal;

        static const auto typeValRE = std::regex{R"(.*std::unique_ptr<(.*)>\s+containing\s+(0x.*))"};
        auto tvMatch = std::cmatch{};
        if (!std::regex_match(val.get(), tvMatch, typeValRE)) return retVal;

        auto os = std::ostringstream{};
        os << "(" << tvMatch[1].str() << "*)" << tvMatch[2].str();
        auto ptrType = sendWhatIs(os.str());
        retVal = getSmartPtrType(exprType, ptrType);
    } catch (...) {}

    return retVal;
        
}

// Use whatis commands to determine the gdb type of `exp`. Accounts
// for the frontend's confusion with ftn array indices and c function calls.
// If no type is found, returns an empty string.
std::string LazyMIVar::sendWhatIs(const std::string& exp) {
    auto cmd = CLIWhatis(exp.c_str());
    SendMICommand(m_mi.getSession(), cmd);

    if (MICommandResultOK(cmd)) {
        auto typeStr = CLIGetWhatisInfo(cmd);
        MICommandFree(cmd);
        if (!typeStr) return std::string{""};
        std::string r = typeStr;
        free(typeStr);
        return r;
    } else {
        auto msg = MICommandResultErrorMessage(cmd);
        auto err = std::string{msg};
        MICommandFree(cmd);
        free(msg);
        throw err;
    }
}

std::shared_ptr<ais::Type> LazyMIVar::getType() {
    static std::regex anonymousRE(R"((const)? *class)");

    if (m_type) return m_type;

    if (m_var) {
        return m_type = TypeParser::getType(m_mi, m_var.get(), nullptr);
    } 

    if (m_expr.empty()) {
        throwDbgError("No expression to create variable with.");
    }

    // strip meta info if present
    auto origExpr = m_expr;
    auto [expr1, metaInfo] = m_mi.extractMetaInfo(m_expr.c_str());

    // strip out .. expressions, for fortran we can just replace
    // with :, for C we collapse we will insert a SubArray object
    // to handle the slicing
    auto [expr,isFtn] = SubArray::removeRngs(m_mi, expr1);
    if (isFtn) m_expr = expr;  // may now have :'s

    auto exprType = std::string{};
    try {
        exprType = getGdbType(expr);
    } catch (std::string err) {
        if (!dbg_error) {
            if (err.find("No symbol") != std::string::npos) {
                DVPRINT("Whatis %s failed; no symbol.\n", expr.c_str());
                dbg_error.reset(NewErrorEvent(DBGERR_NOSYM, "%s", expr.c_str()));
            } else {
                DVPRINT("Whatis %s failed: %s.\n", expr.c_str(), err.c_str());
                dbg_error.reset(NewErrorEvent(DBGERR_DEBUGGER, "%s: %s",
                                              expr.c_str(), err.c_str()));
            }
        }
        throw dbg_error.release();
    }

    if (std::regex_match(exprType, anonymousRE) && getVar()) {
        // unnamed C++ class type, I'll have to work from the expression
        return getType();
    }

    m_type = TypeParser::getType(m_mi, nullptr, exprType.c_str());
    DVPRINT("Got type for %s: %s\n", origExpr.c_str(), m_type->getTypeName().c_str());

    if (!isFtn) {
        // check if we're dealing with a sliced expression. this could be a C array slice
        // or special treatment for kokkos/raja views
        try {
            if (auto kv = std::dynamic_pointer_cast<ais::KokkosView>(m_type)) {
                // this sets kv's view lengths even in the non-slice case
                m_subArray = KokkosSubArray::get(m_mi, expr1, metaInfo, kv);
            } else if (auto rv = std::dynamic_pointer_cast<ais::RajaView>(m_type)) {
                m_subArray = RajaSubArray::get(m_mi, expr1, metaInfo, rv);
            } else if (expr1.find("..") != std::string::npos) {
                m_subArray.reset(new CSubArray(m_mi, expr1));
            }
        } catch (const std::exception& err) {
            DVPRINT("Caught exception parsing the SubArray: %s\n", err.what());
        } catch (...) {
            DVPRINT("Caught unknown exception parsing a SubArray\n");
        }

        if (m_subArray) {
            DVPRINT("expression is a SubArray\n");
            m_type = m_subArray->getType();
        }
    }

    if (metaInfo.empty())
        return m_type;

    // if we have a pointer with length meta info, we can apply it now
    if (metaInfo[0] == '@') {
        if (auto ptrType = std::dynamic_pointer_cast<ais::Pointer>(m_type)) {
            DVPRINT("Found metainfo to apply: %s\n", metaInfo.c_str());
            auto lengthened = std::string{"("} + expr + ")[0]" + metaInfo;
            try {
                if (auto appliedType = getGdbType(lengthened); !appliedType.empty()) {
                    m_type = TypeParser::getType(m_mi, nullptr, appliedType.c_str());
                }
            } catch (...) {}
        }
    } else if (metaInfo[0] == '|') {
        getSliceType(expr, metaInfo);
    }
    
    return m_type;
}

std::string LazyMIVar::getExpr() {
    if (!m_expr.empty()) return m_expr;

    if (!m_var || !m_var->expr) {
        throwDbgError("No var to derive expression from.");
    }

    m_expr = m_var->expr;
    return m_expr;
}

namespace {
// class for returning only a portion of a an array.  This version
// is used to return only the sliced version of a decomposed array
// Note: the underlying type isn't necessarily an array, or an
// array of the right dimension so it would be too hard to filter
// at the gdb extraction level
class SubArrayFilter : public ais::FilteredArray::Filter {
public:
    SubArrayFilter(std::unique_ptr<BlockMap>&& blockMap,
                   const std::string& rngStr,
                   bool isFortran, int idx0);
    int getLen() override;
    bool doWrite() override;

private:
    std::unique_ptr<BlockMap> m_blockMap;
    int m_len;
    int m_repeat;
    std::vector<std::pair<int,int>> m_subRngs;
    std::vector<int> m_idcs;
    BlockMap::Iterator m_it;
};

SubArrayFilter::SubArrayFilter(std::unique_ptr<BlockMap>&& blockMap,
                               const std::string& rngStr,
                               bool isFortran, int index0) 
    : m_blockMap(std::move(blockMap)), m_len(0),
      m_repeat(0), m_it(m_blockMap.get())
{
    auto bm = m_blockMap.get();
    m_len = 0;
    m_it.begin(isFortran, index0);

    m_subRngs = bm->getIndexRng(rngStr);
    if (isFortran) {
        // m_subRngs and m_it are ordered from slowest to fastest
        // and zero based
        std::reverse(m_subRngs.begin(), m_subRngs.end());
        for (auto& [lo,hi] : m_subRngs) {
            lo -= index0;
            hi -= index0;
        }
    }
    auto dim = m_it.getDim();
    m_idcs.resize(dim);
    
    for (; m_it.next(); ) {
        if (m_it.streamIdx != my_rank) continue;
        m_it.getIdcs(m_idcs);

        int i=0;
        for ( ; i < dim; ++i) {
            auto idx = m_idcs[i];
            auto [lo,hi] = m_subRngs[i];
            if (i+1 < dim) {
                if (idx < lo || idx > hi) break;
            } else if (auto ihi=idx+m_it.len-1; idx <= hi && ihi >= lo) {
                m_len += std::min(hi,ihi) - std::max(lo,idx) + 1;
            }
        }
    }

    m_it.reset();
}

int SubArrayFilter::getLen() { return m_len; }
bool SubArrayFilter::doWrite() {
    if (m_repeat--) {
        ++m_idcs.back();
    } else {
        for (; m_it.next(); ) {
            if (m_it.streamIdx != my_rank) continue;
            m_it.getIdcs(m_idcs);
            m_repeat = m_it.len - 1;
            break;
        }
    }

    int dim = m_idcs.size();
    for (int i=0; i<dim; ++i) {
        auto idx = m_idcs[i];
        auto [lo,hi] = m_subRngs[i];
        if (idx < lo || idx > hi)  return false;
    }
    return true;
}
}

void LazyMIVar::getSliceType(const std::string& expr,
                             const std::string& metaInfo)
{
    // break meta info info blockmap and index parts
    auto i = findExprMatch(metaInfo,1);
    auto bmDef = metaInfo.substr(2,i-2);

    if (metaInfo[i]) ++i;

    // and the indices
    auto idxBeg = i;
    if (metaInfo[i] == '(') {
        if ((i = metaInfo.find(')',i)) != std::string::npos)
            ++i;
    } else {
        i = metaInfo.find(',',i);
    }
    auto idx = metaInfo.substr(idxBeg,
                               (i==std::string::npos) ? std::string::npos : i-idxBeg);

    DVPRINT("Blockmap %s, Indices are %s\n", bmDef.c_str(), idx.c_str());


    if (auto array = std::dynamic_pointer_cast<ais::Array>(m_type)) {
        auto bm = BlockMap::fromDefinition(bmDef);
        auto isFortran = array->isFortran();
        auto filter = std::make_shared<SubArrayFilter>(std::move(bm), idx,
                                                       isFortran, array->getIndex0());
        m_type = std::make_shared<ais::FilteredArray>(array, filter);
    }
    m_expr = expr;
}

// Handle ".." range expressions.  For fortran we can just replace them with ':' but
// for C/C++ collapse [from..to] to [from].   Return true in .second if it
// was a fortran range.
std::pair<std::string,bool> SubArray::removeRngs(Mi2Aif& mi, const std::string& expr) {
    auto isFtnArray = false;
    auto retVal = std::make_pair(expr, false);
    
    if (expr.back() == ')') {
        auto s = expr;
        if (auto beg = findExprMatchT(s.rbegin(), s.rend()); beg != s.rend()) {
            s.erase(beg.base()-1, s.end());
        }

        // if s == expr, the call to .getType in the block would cause infinite
        // recursion because .getType calls removeRngs.
        if (s != expr) {
            try {
                LazyMIVar var{mi, s};
                auto type = var.getType();
                if (dynamic_cast<ais::KokkosView*>(type.get()))
                    return std::make_pair(s,false);
                else if (dynamic_cast<ais::RajaView*>(type.get()))
                    return std::make_pair(s, false);
                else if (dynamic_cast<ais::FtnArray*>(type.get()))
                    isFtnArray = true;
                else
                    return retVal;
            } catch (dbgevent_t* evt) {
                FreeEvent(evt);
                return retVal;
            }
        }
    }

    if (expr.find("..") == std::string::npos) return retVal;

    auto s = expr;

    if (isFtnArray) {
        // fortran understands ranges via ':'
        for (size_t loc = 0; (loc = s.find("..",loc)) != std::string::npos; ) {
            s.replace(loc,2,":");
        }
        return std::make_pair(s,true);
    }

    // fix open ended ranges, i.e. replace .. with 0..
    auto openRngRE = std::regex{R"(([[(])\s*\.\.)"};
    s = std::regex_replace(s, openRngRE, "$010..");

    // For C/C++ arrays, reduce the expression to single elements by removing the
    // "..endIdx" portion
    auto loc = size_t{0};
    
    while ((loc = s.find("..",loc)) != std::string::npos) {
        auto end = loc;
        for ( ; end < s.size() && s[end] != ',' && s[end] != ')' && s[end] != ']'; ++end) {
            if (s[end] == '(' || s[end] == '[') end = findExprMatch(s,end);
        }
        s.replace(loc, end-loc, "");
    }

    return std::make_pair(s,false);
}

SubArray::SubArray(Mi2Aif& mi, const std::string&) : m_mi(mi) {}


CSubArray::CSubArray(Mi2Aif& mi, const std::string& expr) : SubArray(mi, expr) {
    // get the element type
    auto idcs = parse(expr);
    auto baseTypeStr = m_baseExpr;
    for (size_t i=0, n=idcs.size(); i<n; ++i) baseTypeStr += "[0]";

    auto baseVar = LazyMIVar(mi, baseTypeStr);
    getRngs(idcs);
    resolveOpenRngs();
    makeArrayType(baseVar.getType(), m_rngs);

    DVPRINT("Got SubArray type: %s\n", m_type->getTypeName().c_str());
}

std::vector<std::string> CSubArray::parse(const std::string& expr) {
    auto idcs = std::vector<std::string>{};
    auto level = 0;
    auto end = size_t{};
    auto i = expr.size();
    for ( ; i--; ) {
        auto c = expr[i];
        if (c == ']') {
            if (++level == 1) end = i;
        } else if (c == '[') {
            if (!--level) idcs.push_back(expr.substr(i+1, end-i-1));
        } else if (!level && !isspace(c)) {
            break;
        }
    }

    m_baseExpr = expr.substr(0,i+1);
    std::reverse(idcs.begin(), idcs.end());
    return idcs;
}

const mdspan& CSubArray::getSpan() {
    if (m_span) return *m_span;

    auto arrVar = LazyMIVar(m_mi, m_baseExpr);
    auto arrType = arrVar.getType();

    auto sizes = std::vector<size_t>{};
    while (auto atype = dynamic_cast<ais::Array*>(arrType.get())) {
        sizes.push_back(atype->getLen());
        arrType = atype->getElemTypePtr();
    }

    auto n = sizes.size();
    auto strides = std::vector<size_t>{n, 1};
    if (n) {
        for (int i=n-1; i--; ) strides[i] = strides[i+1]*sizes[i+1];
    }

    m_span.reset(new mdspan{sizes, strides});
    return *m_span;
}

void SubArray::resolveOpenRngs() {
    // m_rngs are listed from slowest to fastest
    // note that we're putting off getting the span if we don't need it.
    auto i = size_t{0};
    for (auto& [lo,hi] : m_rngs) {
        if (lo == UNDEFINED) {
            if (mayHaveOffset()) {
                const auto& span = getSpan();
                lo = span.offset(i);
            } else {
                lo = 0;
            }
        }
        if (hi == UNDEFINED) {
            const auto& span = getSpan();
            hi = span.size(i) - 1 + span.offset(i);
        }
        ++i;
    }
}

void CSubArray::makeArrayType(std::shared_ptr<ais::Type> baseType,
                             const std::vector<std::pair<int,int>>& rngs) {
    m_type = baseType;

    // rngs are listed from slowest varying to fastest varying
    for (auto rit = rngs.rbegin(); rit != rngs.rend(); ++rit) {
        auto [lower, upper] = *rit;
        if (upper > lower) {
            if (dynamic_cast<ais::Char*>(m_type.get())) {
                m_type = std::make_shared<ais::CharArray>();
            } else {
                auto arr = std::make_shared<ais::Array>(m_type, upper-lower+1);
                arr->setBounds(upper-lower+1, 0);
                m_type = arr;
            }
        }
    }
}

void SubArray::getRngs(const std::vector<std::string>& idcs) {

    for (const auto& idx : idcs) {
        auto rng = std::pair<int,int>{};
        if (auto dot = idx.find(".."); dot == std::string::npos) {
            rng.first = rng.second = getIdx(idx, 0);
        } else {
            auto defaultLower = (mayHaveOffset() ? UNDEFINED : 0);
            rng.first = getIdx(idx.substr(0,dot), defaultLower);
            rng.second = getIdx(idx.substr(dot+2),UNDEFINED);
            if (rng.second >= 0 && rng.second < rng.first)
                rng.second = rng.first;
        }
        DVPRINT("Got range %d %d\n", rng.first, rng.second);
        m_rngs.push_back(rng);
    }

    resolveOpenRngs();
}

int SubArray::getIdx(const std::string& expr, int defVal) const {
    if (std::ranges::all_of(expr, [](char c) { return std::isspace(c); })) {
        return defVal;
    } else if (std::ranges::all_of(expr.begin(), expr.end(), [](char c) {return std::isdigit(c);})) {
        return std::stoi(expr);
    } else if (auto res = EvalExpression(m_mi.getSession(), expr.c_str())) {
        auto resStr = std::string{res};
        free(res);
        return std::stoi(resStr);
    } else {
        throw std::runtime_error("evaluation failed");
    }
}

char* CSubArray::getArrayDataString() {
    if (m_rngs.empty()) {
        LazyMIVar evar(m_mi, m_baseExpr);
        return evar.getArrayDataString();
    }
    
    auto os = std::ostringstream{};
    auto idx = std::vector<int>{};
    auto idcs = m_rngs;
    auto inDim = idcs.size();

    auto repeat = idcs.back().second-idcs.back().first+1;
    std::vector<std::string> nests(inDim);
    size_t brck0 = 0;
    for (size_t i=0; i+1 < inDim; ++i) {
        if (idcs[i].first < idcs[i].second) ++brck0;
    }
    for (size_t i=0; i < inDim; ++i) {
        nests.emplace_back(brck0,'{');
        if (brck0 && idcs[i].first < idcs[i].second) --brck0;
    }

    os << nests[0];
    
    while (true) {
        std::ostringstream expr;
        expr << m_baseExpr;
        for (auto [lo,hi] : idcs) expr << '[' << lo << ']';
        if (repeat > 1) expr << '@' << repeat;

        LazyMIVar evar(m_mi, expr.str());

        if (auto res1 = evar.getArrayDataString()) {
            os << res1;
            free(res1);
        }

        int i = inDim-1;
        for ( ; i--; ) {
            auto& idx = idcs[i];
            if (idx.first < idx.second) {
                ++idx.first;
                os << ',' << nests[i+1];
                break;
            } else {
                auto rng = m_rngs[i];
                idx.first = rng.first;
                if (rng.first != rng.second) os << '}';
            }
        }

        if (i < 0) break;
    }

    return strdup(os.str().c_str());
}

std::unique_ptr<KokkosSubArray>
KokkosSubArray::get(Mi2Aif& mi, const std::string& expr,
                    const std::string& metaInfo,
                    std::shared_ptr<ais::KokkosView> view) {

    auto retVal = std::unique_ptr<KokkosSubArray>{};

    if (expr.back() == ')') {
        // range view
        auto loc = findExprMatchT(expr.rbegin(), expr.rend());
        auto baseExpr = std::string{expr.begin(), loc.base()-1};
        if (getViewLens(mi, baseExpr, *view)) {
            retVal.reset(new KokkosSubArray{mi, view, expr, baseExpr});
        }
    } else if (getViewLens(mi, expr, *view) && !metaInfo.empty()) {
        // decomposition, assume the entire contents
        auto os = std::ostringstream{};
        os << expr;
        auto i = 0;
        auto span = view->getSpan();
        for (auto len : span.sizes()) os << (i++ ? ',' : '(') << "0.." << len-1;
        os << ')';
        retVal.reset(new KokkosSubArray{mi, view, os.str(), expr});
    }

    return retVal;
}

// Create the type for sending back to the front-end
void KokkosSubArray::makeArrayType(std::shared_ptr<ais::Type> baseType,
                             const std::vector<std::pair<int,int>>& rngs) {

    m_type = baseType;

    // rngs are listed from slowest varying to fastest varying
    for (auto rit = rngs.rbegin(); rit != rngs.rend(); ++rit) {
        auto [lower, upper] = *rit;
        if (upper > lower) {
            auto arr = std::make_shared<ais::Array>(m_type, upper-lower+1);
            arr->setBounds(upper-lower+1, 0);
            m_type = arr;
        }
    }
}


// Retrieve - or try to retrieve - the length and stride information
// of a KokkosView from the debugger.   Most of the interface is
// inlined, but just enough seems to be available to infer the dimensions
bool KokkosSubArray::getViewLens(Mi2Aif& mi, const std::string& expr,
                                 ais::KokkosView& view) {
    auto dim = view.dim();
    auto lens = std::vector<size_t>{}, strides = std::vector<size_t>{};

    auto span = size_t{};

    try {
        auto sess = mi.getSession();
        auto q = expr + ".span()";
        if (auto res = EvalExpression(sess, q.c_str())) {
            span = std::stol(res);
            free(res);
        } else {
            return false;
        }

        q = expr + ".span_is_contiguous()";
        if (auto res = EvalExpression(sess, q.c_str())) {
            if (res[0] != 't' && res[0] != 'T' && res[0] != '1') {
                free(res);
                return false;
            }
            free(res);
        } else {
            return false;
        }

        for (int i = 0; i < dim; ++i) {
            q = expr + ".stride_" + std::to_string(i) + "()";
            if (auto res = EvalExpression(sess, q.c_str())) {
                auto stride = std::stol(res);
                free(res);
                if (stride > 0) strides.push_back(stride);
                else return false;
            } else {
                return false;
            }
        }
    } catch (...) {
        return false;
    }

    if (!span) return false;

    // infer the lengths from strides and span
    for (auto i=0; i<dim; ++i) {
        auto stride = strides[i];
        auto nextStride = span;
        for (auto& s : strides) {
            if (s > stride && s < nextStride) nextStride = s;
        }
        lens.push_back(nextStride / stride);
    }

    view.getSpan().setSizes(lens, strides);
    return true;
}

KokkosSubArray::KokkosSubArray(Mi2Aif& mi, std::shared_ptr<ais::KokkosView> type,
                               const std::string& expr, const std::string& baseExpr) :
    SubArray(mi, expr), m_kv(type)
{
    m_span.reset(new mdspan{type->getSpan()});
    m_baseExpr = baseExpr;
    parse(expr);
    makeArrayType(type->elemType(), m_rngs);
}

void KokkosSubArray::parse(const std::string& expr) {
    auto idcs = std::vector<std::string>{};
    auto paren = expr.find_last_of('(');
    m_baseExpr = expr.substr(0,paren);
    for (auto loc = paren; loc != std::string::npos; ) {
        auto end = expr.find_first_of(",)", loc+1);
        if (end != std::string::npos) {
            idcs.push_back(expr.substr(loc+1,end-loc-1));
        }
        loc = end;
    }
    getRngs(idcs);
}

const mdspan& KokkosSubArray::getSpan() { return *m_span; }

// extract the data values from a kokkos view and form into a single
// array with the output ordering
char* KokkosSubArray::getArrayDataString()
{
    const auto& span = m_kv->getSpan();
    const auto& strides = span.strides();
    const auto& lens = span.sizes();
    
    auto dim = lens.size();
    if (m_rngs.size() != dim) {
        throwDbgError("range expression is not of same dimensions as view");
    }

    // Get the size of the sub-range and the limits of the smallest
    // rectangle of data I can query that will contain it.
    auto idx0 = size_t{0}, idx1 = size_t{0}, totalLen = size_t{1};
    auto rngLens = std::vector<size_t>{};
    for (auto i=size_t{0}; i<dim; ++i) {
        m_rngs[i].first = std::max(m_rngs[i].first,0);
        m_rngs[i].second = std::min(m_rngs[i].second,int(lens[i]-1));
        auto len = m_rngs[i].second - m_rngs[i].first + 1;
        rngLens.push_back(len);
        totalLen *= len;
        idx0 += m_rngs[i].first * strides[i];
        idx1 += m_rngs[i].second * strides[i];
    }

    // Get the data from the process, in some cases this may include a lot
    // of values we aren't using but we have to sacrifice memory for time
    // because extracting one at a time if far too slow
    auto expr = m_baseExpr + ".data()[" + std::to_string(idx0) + "]@"
        + std::to_string(idx1-idx0+1);
    
    LazyMIVar evar(m_mi, expr);
    auto fullRes = evar.getArrayDataString();
    if (!fullRes) {
        DVPRINT("Could not read value\n");
        return nullptr;
    }

    auto fullResHolder = std::unique_ptr<char, decltype(&free)>{fullRes, free};

    // build up a table mapping the full results into our sub-range
    // aside from being a superset of the data we're after, it is also
    // not necessarily indexed in the same order
    auto strideMap = std::vector<std::pair<size_t,size_t>>{};
    for (size_t i=0; i<dim; ++i) strideMap.emplace_back(strides[i],i);
    std::sort(strideMap.begin(), strideMap.end());
    std::reverse(strideMap.begin(), strideMap.end());

    std::vector<size_t> rngMap(dim,1);
    for (auto i=dim; --i; ) rngMap[i-1] = rngMap[i] * rngLens[i];

    auto numFound = size_t{0};
    auto elems = std::vector<const char*>{totalLen};

    const char* s = fullRes;
    if (*s == '{') ++s;

    // iterate over the elements in the gdb output, store references
    // to the ones we are keeping in printout order
    for (size_t fullIdx=0; s; ++fullIdx) {

        // map into the subrange
        auto rngIdx = size_t{0};
        auto remIdx = fullIdx;
        bool inRng = true;
        for ( auto [stride, dir] : strideMap) {
            auto ri = remIdx / stride;
            if (ri >= rngLens[dir]) { inRng = false; break; }
            rngIdx += ri * rngMap[dir];
            remIdx %= stride;
        }

        if (inRng) {
            ++numFound;
            elems[rngIdx] = s;
        }

        // possible complex values
        if (*s == '{') s = findExprMatch(s);

        if ((s = strchr(s,','))) ++s;
    }

    if (numFound != totalLen) {
        DVPRINT("Did not find all elements\n");
        return nullptr;
    }

    // these are from slowest to fastest
    auto outLens = rngLens;
    std::erase(outLens,1);
    std::ranges::reverse(outLens); // not fastest to slowest

    // and put the results into a string.   
    auto open = "{", sep = ",", close = "}";

    auto os = std::ostringstream{};
    for (size_t i=0; i<totalLen; ++i) {
        auto elem = elems[i];

        auto elemEnd = strchr(elem,',');
        if (!elemEnd) elemEnd = strchr(elem,'}');

        auto elemVal = std::string_view{elem, size_t(elemEnd-elem)};

        auto j = i;
        for (auto len : outLens) {
            if (j % len) break;
            else os << open;
            j /= len;
        }

        os << elemVal;

        j = i+1;
        for (auto len : outLens) {
            if (j % len) { os << sep; break; }
            else os << close;
            j /= len;
        }
    }


    // we could potentially avoid copying the data (twice) by
    // changing this interface to supply a custom ais::InterpretStream instead
    return strdup(os.str().c_str());
}

std::unique_ptr<RajaSubArray>
RajaSubArray::get(Mi2Aif& mi, const std::string& expr,
                  std::string_view meta_info,
                  const std::shared_ptr<ais::RajaView> raja_view) {
    // prepare to get view bounds from gdb
    auto paren = expr.find_last_of('('); // npos is ok
    auto base_expr = expr.substr(0, paren);

    {
        // RAJA uses capture-by-reference lambdas a lot, and gdb will evaluate
        // some RAJA types as @0xffffabcce038 (a printout of a reference)
        // instead of the info we want. "*&" tricks gdb into giving us the right info.
        auto var = LazyMIVar(mi, base_expr).getVar(); // throws on failure
        if (typeIsReference(var->type)) base_expr = "*&" + base_expr;
    }

    // make request for bounds
    auto bounds_data = std::unique_ptr<char, decltype(&::free)>(
        EvalExpression(mi.getSession(), base_expr.c_str()), ::free
    );
    if (!bounds_data) throw std::runtime_error("EvalExpression failed");

    // some view types have their bounds encoded as template
    // arguments, and some have their bounds as explicit member
    // variables. we have no way to know ahead of time which
    // one we're dealing with, so we arbitrarily try template args
    // first, then member variables.
    auto bounds = ais::RajaView::spanFromTemplateArgs(bounds_data.get());
    if (!bounds.isValid())
        bounds = ais::RajaView::spanFromMembers(bounds_data.get());

    // it's possible that both parses failed and bounds is invalid.
    // we still set the bounds anyway and let the ais printing/parsing
    // logic handle the invalid bounds.
    raja_view->setSpan(std::move(bounds));

    if (ais::RajaView::verboseDebug()) {
        std::ostringstream debug_stream;

        const auto& span = raja_view->getSpan();

        debug_stream << "sizes:\n ";
        for (auto i : span.sizes()) debug_stream << i << ", ";
        debug_stream << "\n";

        debug_stream << "strides:\n ";
        for (auto i : span.strides()) debug_stream << i << ", ";
        debug_stream << "\n";

        debug_stream << "offsets:\n ";
        for (auto i : span.offsets()) debug_stream << i << ", ";
        debug_stream << "\n";

        DVPRINT("%s\n", debug_stream.str().c_str());
    }

    // don't try to do any subarray parsing if our bounds are invalid
    if (!raja_view->getSpan().isValid()) {
        throw std::runtime_error("Failed to determine view bounds");
    }

    // check if we are slicing the view
    return std::unique_ptr<RajaSubArray>(new RajaSubArray(mi, base_expr, expr, meta_info, raja_view));
}

const mdspan& RajaSubArray::getSpan() { return m_view->getSpan(); }
bool RajaSubArray::mayHaveOffset() const { return true; }

RajaSubArray::RajaSubArray(Mi2Aif& mi, const std::string& base_expr, const std::string& expr, std::string_view meta_info, const std::shared_ptr<ais::RajaView> view)
    : SubArray(mi, expr)
    , m_base_expr{base_expr}
    , m_view{view}
{
    DVPRINT("In RajaSubArray::RajaSubArray\n");

    {   // deduce m_rngs from expr
        auto ranges_text = [&]() -> std::vector<std::string> {
            auto rstrings = std::vector<std::string>{};
            auto paren = expr.find_last_of('(');

            if (paren != std::string::npos) {
                // slice contents
                for (auto loc = paren; loc != std::string::npos; ) {
                    auto end = expr.find_first_of(",)", loc+1);
                    if (end != std::string::npos) {
                        rstrings.emplace_back(expr.substr(loc+1,end-loc-1));
                    }
                    loc = end;
                }
            } else if (!meta_info.empty()) {
                // decomposition, get entire contents
                for (auto i = size_t{0}; i < m_view->getSpan().dim(); i++) {
                    const auto low = 0 + m_view->getSpan().offset(i);
                    const auto high = low + m_view->getSpan().size(i) - 1;
                    rstrings.emplace_back(std::to_string(low) + ".." + std::to_string(high));
                }
            }

            return rstrings;
        }();

        getRngs(ranges_text); // sets m_rngs
    }

    {   // fit the user specified ranges into the actual ranges of the view. with offset
        // views, negative indices can be valid.
        if (m_rngs.size() != view->getSpan().dim())
            throw std::runtime_error("Specified slice doesn't match dimension of underlying RAJA::View");

        const auto& offsets = view->getSpan().offsets();
        const auto& sizes = view->getSpan().sizes();
        
        for (size_t i = 0; i < m_rngs.size(); i++) {
            // FIXME: show a warning to the user if truncation happens.
            m_rngs[i].first = std::max(0 + offsets[i], m_rngs[i].first);
            m_rngs[i].first = std::min(int(sizes[i] - 1) + offsets[i], m_rngs[i].first);

            m_rngs[i].second = std::max(0 + offsets[i], m_rngs[i].second);
            m_rngs[i].second = std::min(int(sizes[i] - 1) + offsets[i], m_rngs[i].second);
        }

        for (auto [lo,hi] : m_rngs) DVPRINT("postRng, %d %d\n", lo, hi);
    }

    {   // generate m_type as properly sized and nested ais array
        m_type = view->getElemType();

        // rngs are listed from slowest varying to fastest varying. we need to build
        // the array type from the inside (fastest) out, so we use reverse iterators
        for (auto rit = m_rngs.rbegin(); rit != m_rngs.rend(); ++rit) {
            auto [lower, upper] = *rit;
            if (upper > lower) {
                auto arr = std::make_shared<ais::Array>(m_type, upper-lower+1);
                arr->setBounds(upper-lower+1, 0);
                m_type = arr;
            }
        }
    }
}

std::vector<RajaSubArray::RajaIndex> RajaSubArray::generateIndices(
    const std::vector<std::pair<int, int>>& ranges, const mdspan& bounds)
{
    // iterate through the range space and generate the 1D index into the 1D backing array
    // corresponding to each possible tuple. the output indices can be ordered in two
    // ways: the order in which they should appear as text output after applying the view
    // reordering ("logical position") and the way that they appear in the actual 1D
    // backing array.

    auto result = std::vector<RajaSubArray::RajaIndex>{};
    if (ranges.empty()) return result;

    auto next_position = 0; // for the logical position ordering

    // iterating through (0..2, 0..2, 0..1) would look something like this:
    // 0, 0, 0 "current_tuple" below
    // 0, 0, 1
    // 0, 1, 0 <- referred to as an overflow below
    // 0, 1, 1
    // 0, 2, 0
    // 0, 2, 1
    // 1, 0, 0 <- another overflow
    // ...

    auto current_tuple = std::vector<int>{};
    const auto& offsets = bounds.offsets();
    for (size_t i = 0; i < ranges.size(); i++) {
        current_tuple.push_back(ranges[i].first - offsets[i]);
    }

    while (current_tuple[0] <= ranges[0].second - offsets[0]) {
        int idx = 0;
        for (size_t i = 0; i < current_tuple.size(); i++)
            idx += current_tuple[i] * bounds.strides()[i];
        result.push_back({.position = next_position, .index = idx, .text = nullptr});
        next_position++;

        // increment and overflow if needed
        current_tuple.back() += 1;
        for (size_t i = current_tuple.size() - 1; i > 0; i--) {
            if (current_tuple[i] > ranges[i].second - offsets[i]) {
                current_tuple[i] = ranges[i].first - offsets[i];
                current_tuple[i-1] += 1;
            } else {
                break;
            }
        }
    }

    return result;
}

char* RajaSubArray::getArrayDataString() {
    DVPRINT("RajaSubArray::getArrayDataString()\n");

    // indices in the flat data
    auto flat_indices = generateIndices(m_rngs, m_view->getSpan());
    if (flat_indices.empty()) {
        DVPRINT("generateIndices returned no indices\n");
        return nullptr;
    }

    if (ais::RajaView::verboseDebug()) {
        std::ostringstream debug_stream;

        debug_stream << "flat indices:\n";
        for (auto i : flat_indices) debug_stream << i.index << ", ";
        debug_stream << "\n";

        DVPRINT("%s\n", debug_stream.str().c_str());
    }

    // RAJA supports changing which index is the fastest changing in a view, but it does
    // *not* support changing the order of elements within a single dimension. because of
    // this, we can assume that the first index returned above will always be the smallest,
    // and the last index will always be the biggest.
    auto smallest_index = flat_indices.front().index;
    auto biggest_index = flat_indices.back().index;

    // generate the smallest span of data that will include everything we're interested in
    auto full_data_expr = m_base_expr + ".m_data[" + std::to_string(smallest_index) + "]";
    if (biggest_index - smallest_index >= 1)
        full_data_expr += "@" + std::to_string(biggest_index - smallest_index + 1);

    // get the data from gdb
    auto full_data = std::unique_ptr<char, decltype(&free)>
        (EvalExpression(m_mi.getSession(), full_data_expr.c_str()), free);
    if (!full_data || *full_data == '\0') {
        DVPRINT("Failed to evaluate\n");
        return nullptr;
    }

    // now we link the entries in flat_indices to the text that represents each one

    if (flat_indices.size() == 1) {
        // parse/link a single element
        flat_indices[0].text = full_data.get();
        flat_indices[0].text_len = strlen(full_data.get());
    } else {
        // parse/link a list of items

        // sort entries in the order that they will appear in the gdb output
        std::sort(flat_indices.begin(), flat_indices.end(),
            [](auto l, auto r) { return l.index < r.index; });

        auto input = std::string_view(full_data.get());
        auto current_idx_in_data = smallest_index;
        auto next_flat_idx = int{0}; // index into flat_indices

        // skip opening {. even if the view we are dealing with is >1 dims, we
        // are dumping the data as a 1D array, so we only have to worry about 1
        // level of {
        auto current_char = input.find_first_not_of(", }", 1 /* skip { */);

        while (current_char < input.size()) {
            // find element end
            auto end = input.size();
            if (input[current_char] == '{') {
                // elements are structs or arrays
                end = findExprMatch(input, current_char) + 1;
            } else {
                // elements are primitive types
                end = input.find_first_of(",}", current_char);
            }

            if (ais::RajaView::verboseDebug()) {
                DVPRINT("Current: %d Waiting4: %d text: %.*s\n",
                    current_idx_in_data, flat_indices[next_flat_idx].index,
                    (int)(end - current_char), full_data.get() + current_char);
            }

            // is it an element we're interested in?
            if (current_idx_in_data == flat_indices[next_flat_idx].index) {
                flat_indices[next_flat_idx].text = &input[current_char];
                // end might be string::npos, so truncate it to input.size() just in case
                flat_indices[next_flat_idx].text_len = std::min(end, input.size()) - current_char;
                next_flat_idx++;
            }

            current_char = input.find_first_not_of(", }", end);
            current_idx_in_data++;
        }
    }

    // now reconstruct the parsed data in the order that the view applies

    if (ais::RajaView::verboseDebug()) {
        for (auto e : flat_indices) {
            DVPRINT("order: %d, idx: %d, txt: %4s...\n", e.position, e.index, e.text);
        }
    }

    std::sort(flat_indices.begin(), flat_indices.end(), 
        [](auto l, auto r) { return l.position < r.position; });

    if (ais::RajaView::verboseDebug()) {
        DVPRINT("by logical idx:\n");
        for (auto e : flat_indices) {
            DVPRINT("order: %d, idx: %d, txt: %.4s...\n", e.position, e.index, e.text);
        }
    }

    // even though we always make a text representation of a 1D array here, the parser in
    // m_type will correctly parse it for any dimension.
    //
    // braces should only be printed if the result has more than one value. if the result
    // only has one value, m_type will be the base type and not an array. brackets for the
    // base type would break its parse later on.
    std::ostringstream output;

    if (flat_indices.size() > 1)
        output << "{";

    for (size_t i = 0; i < flat_indices.size(); i++) {
        auto elmt = flat_indices[i];

        if (ais::RajaView::verboseDebug()) DVPRINT("e.text: %.8s...\n", elmt.text);

        output << std::string_view(elmt.text, elmt.text_len);

        if (i < flat_indices.size() - 1) {
            output << ",";
        } else if (flat_indices.size() > 1) {
            output << "}";
        }
    }

    DVPRINT("output: %s\n", output.str().c_str());

    return strdup(output.str().c_str());
}

// Build up the mapping from the ptype result.  Oobs is the
// full line by line output captured by ptype
PTypeFields::Templates::Templates(MIList* oobs) {
    MIListSet(oobs);
    while (auto oob = (MIOOBRecord *)MIListGet(oobs)) {
        auto line = oob->cstring;

        auto s = strstr(line, "[with ");
        if (!s) continue;

        s += 6;

        for (; *s && *s != ']'; ) {
            // template arguments we expect something like:
            // "[with T = int]" or
            // "[with T = int, U = double]"

            // get the template argument name
            auto e = strstr(s+1, " = ");
            if (!e) {
                DVPRINT("could not parse [with]: \"%s\"\n", line);
                break;
            }

            auto targ = std::string{s, std::string::size_type(e-s)};
            s = e+3;
            // and the type it maps to. I'm assuming valid type syntax, and
            // I'm just looking for a separator (,) or terminator (]) that
            // isn't inside a nested <>, () or [].   Which would make for
            // one heck of a challenging RE.
            auto nest = 0;
            for (e=s; *e && (nest || (*e != ',' && *e != ']')); ++e) {
                if (strchr("(<[", *e)) ++nest;
                else if (strchr(")>]", *e)) --nest;
            }

            if (!*e) {
                DVPRINT("could not parse [with]: \"%s\"\n", line);
                break;
            }

            auto tval = std::string{s, std::string::size_type(e-s)};

            // create the regex for doing the mapping
            // there may be some super diabolical counter-exampe, but
            // I'm  just going the restrict the paranoia to just making
            // sure I'm looking for words.
            DVPRINT("got a template mapping: \"%s\"->\"%s\"\n",
                    targ.c_str(), tval.c_str());

            auto tmatch = "\\b" + targ + "\\b";
            m_typeMap.push_back(std::pair{std::regex{tmatch}, tval});

            s = e;
            if (*s == ',') while (*++s && std::isspace(*s)) {}
        }
    }

}
std::string PTypeFields::Templates::operator()(std::string&& field) {
    // expand any template parameters in field
    for ( auto& tm: m_typeMap ) {
        field = std::regex_replace(field, tm.first, tm.second);
    }
    return field;
}

static std::string FixRightShift(std::string type) {
    for (size_t pos=0; (pos=type.find(">>",pos)) != std::string::npos; ++pos) {
        type.replace(pos,2,"> >");
    }
    return type;
}

// given a string with s[0] == '<' return a pointer to the matching '>'
// or the last character in the string
static const char* FindTemplateEnd(const char* type) {
    // this would be a simple up/down count except for boolean template args
    // to something like std::conditional which can create a type like:
    // std::conditional_t<(2u)<(3), unsigned long, unsigned int>
    const char* s = type;
    for (int lev=0; *s; ++s) {
        switch (*s) {
        case '<': ++lev; break;
        case '>': if (!--lev) return s; break;
        case ')': if ( (s[1]=='<' || s[1] == '>') && s[2] == '(') ++s; break;
        default: break;
        }
    }
    return s-1;
}

// get the length of the template argument including the <>
size_t FindTemplateLength(const std::string& str, size_t pos) {
    auto beg = str.c_str()+pos;
    auto end = FindTemplateEnd(beg); // should point to >
    return (end-beg)+1;
}

PTypeFields::PTypeFields(MICommand& sentCmd, bool isFortran)
    : m_typeMapper(sentCmd.output->oobs)
{
    if (isFortran) {
        // CLIGetPtypeFieldList does not handle fortran output
        initFortranFields(sentCmd);
        return;
    }
    MIList* field_list = CLIGetPtypeFieldList(&sentCmd);

    // populate fields
    for (MIListSet(field_list); char* field = (char *)MIListGet(field_list); ) {
        DVPRINT("PTypeFields: field is %s\n", field);
        auto fieldStr = m_typeMapper(field);
        m_fields.push_back(fieldStr);
    }
    MIListFree(field_list, (void (*)()) free);

    // return a pointer to the first letter of the first inherited base class name
    // starting from `from` or nullptr of there are no more
    auto nextBaseClass = [](const char* from) {
        // input string looks something like
        // "type = class MultiDerived : public LeftBase, protected RightBase {\n..."
        auto tokens = {
            // first listed base class is prepended by a colon
            ": public", ": protected", ": private",
            // subsequent base classes are prepended by a comma
            ", public", ", protected", ", private"
        };
        auto lineEnd = strchr(from, '\n');
        if (!lineEnd) {
            // last line has no \n
            lineEnd = from + strlen(from);
        }

        auto earliest = static_cast<const char*>(nullptr);

        for (const auto* token : tokens) {
            auto match = strstr(from, token);
            if (match) {
                match += strlen(token) + 1; // move to beginning of class name
                if (match < lineEnd && (earliest == nullptr || match < earliest)) {
                    earliest = match;
                }
            }
        }

        auto alen = ANONYMOUS_NAMESPACE.size();
        if (earliest && strncmp(earliest, ANONYMOUS_NAMESPACE.c_str(), alen) == 0) {
            earliest += alen;
        }

        DVPRINT("nextBaseClass returning %s\n", (earliest ? earliest : "null"));
        return earliest;
    };

    // populate parent classes
    auto* info = CLIGetPtypeInfo(&sentCmd);
    const auto* end = info;

    while (auto* start = nextBaseClass(end)) {
        // found the start of parent class name, now find the end
        end = start;
        // scan until we hit a space or a comma outside of a template (or the end of the line)
        for (; *end != ' ' && *end != ',' && *end != '\n'; ++end) {
            if (*end == '<') end = FindTemplateEnd(end); // skip to matching >
        }
        DVPRINT("parsed to end: %s\n", std::string(start, end).c_str());
        auto parent = m_typeMapper(std::string(start, end));
        // gdb can't properly parse the closure of nested templates (">>")
        // without a space in between ("> >")
        parent = FixRightShift(parent);
        DVPRINT("found parent: %s\n", parent.c_str());
        m_parentClasses.push_back(parent);
    }
    free(info);
}

// parse the output from a ptype command on a fortran type
// (CLIGetTypeFieldList doesn't handle the format, and it's
//  10 times easier to do in C++)
void PTypeFields::initFortranFields(MICommand& cmd) {
    if (!cmd.completed || !cmd.output || !cmd.output->oobs)
		return;

    // Looking for type :: field   and somehow mi or libmi has replaced the
    // newline with two characters "[backslash]n".
    auto re = std::regex(R"(\s*(.*\S)\s*::\s*(\w+)\s*(\\n)?)");

    auto oobs = cmd.output->oobs;
    for (MIListSet(oobs); auto oob = (MIOOBRecord *)MIListGet(oobs); ) {
        auto s = std::string{oob->cstring};
        auto match = std::smatch{};
        if (std::regex_match(s, match, re)) {
            std::ostringstream os;
            os << match[1] << " " << match[2];
            DVPRINT("got fortran field \"%s\"\n", os.str().c_str());
            m_fields.push_back(os.str()) ;
        }
    }
}

// break a data member string into its name and
// type describe the current class/struct, fieldStr is the description of a member
std::pair<std::string, std::string>
PTypeFields::parseField(const std::string&, const std::string& fieldStr) {
    using RetType = std::pair<std::string, std::string>;

    // the field is type followed by name
    auto namePos = fieldStr.rfind(' ');
    if (namePos == std::string::npos) return RetType{};

    namePos = fieldStr.find_first_not_of("*&",namePos+1);
    if (namePos == std::string::npos) return RetType{};

    auto isStatic = false;
    size_t t0 = 0;
    if (fieldStr.rfind("static ",0) == 0) {
        t0 = 7;
        isStatic = true;
    }
    auto fieldTypeStr = fieldStr.substr(t0,namePos-t0);
    auto fieldName = fieldStr.substr(namePos);
    
    // gdb only use the class member notion of static. Here we are changing
    // the member name from name to "static name" which *works* but does
    // rely on the debugger printing *exactly* "static name = value" for the
    // static member.   This path will need to be smarter if the convention changes.
    if (isStatic) fieldName = "static " + fieldName;

    DVPRINT("got field type \"%s\" name \"%s\"%s\n",
            fieldTypeStr.c_str(), fieldName.c_str(), isStatic ? " STATIC" : "");

    return RetType{fieldName, fieldTypeStr};
}


/*
 * Create a MI variable in gdb and get its MIVar type.
 * Returns null on error.
 */
std::shared_ptr<MIVar> Mi2Aif::createMIVar(const char *taggedExpr) {
    auto [expr, metaInfo] = extractMetaInfo(taggedExpr);
    if (!metaInfo.empty()) {
        return createMIVarWithMetaInfo(expr.c_str(), metaInfo);
    }

    auto session = m_session;
    auto delVar = [session](MIVar* mivar) { DeleteMIVar(session,mivar); };

    std::shared_ptr<MIVar> var(nullptr, delVar);

    MICommand* cmd = MIVarCreate("-", "*", quoteId(expr).c_str());
    sendMICommand(cmd);
    // If this fails, it means the symbol doesn't exist.
    if (!MICommandResultOK(cmd)) {
        dbg_error.reset(NewErrorEvent(DBGERR_NOSYM, "%s", expr.c_str()));
        MICommandFree(cmd);
        return var;
    }

    MIVar* mivar = MIGetVarCreateInfo(cmd);
    MICommandFree(cmd);

    if (mivar->exp == NULL) {
        mivar->exp = strdup(expr.c_str());
    }

    mivar->expr = strdup(expr.c_str());

    // remove newline
    if (expr.back() == '\n') {
        mivar->expr[expr.size() - 1] = '\0';
    }

    // Remove whitespace and the final '&' for reference types.
    // ptype breaks with '&'.
    char* p = &(mivar->type)[strlen(mivar->type) - 1];

    while ((p >= mivar->type) && isspace(*p)) {
        *p-- = '\0';
    }
    
    var.reset(mivar, delVar);
    return var;
}

// parse `expr` for meta info ("[[...]]") and return the separated meta info
// with the rest of the original string, in that order. If no meta info is
// present, the meta info portion of the returned tuple will be an empty string.
std::pair<std::string, std::string> Mi2Aif::extractMetaInfo(const char* expr) {
    // Taking a cue from C++ annotations to use [[...]] to pass meta-data
    // to the back-end.   So far, this is just being used to pass length
    // information for raw C/C++ arrays.   I'd expect to add chunking info
    // via this channel in the future.
    auto e = std::string{expr};
    auto meta = std::string{};
    
    while (e.size() >= 4 && e[0] == '[' && e[1] == '[') {
        size_t i = 2;
        for (int depth = 2; depth && i < e.size(); ++i) {
            if (e[i] == '[') ++depth;
            else if (e[i] == ']') --depth;
        }

        if (!meta.empty()) meta += ',';
        meta += e.substr(2,i-4);
        e = e.substr(i);
    }

    return std::make_pair(std::move(e), std::move(meta));
}

// Create an expression to evaluate based on additional meta-data for context
std::shared_ptr<MIVar>
Mi2Aif::createMIVarWithMetaInfo(const char* expr, const std::string& metaInfo)
{
    DVPRINT("createMiVarWithMetaInfo \"%s\" + \"%s\"\n", expr, metaInfo.c_str());
    std::shared_ptr<MIVar> rawVal = createMIVar(expr);

    if (rawVal && rawVal->type && rawVal->type[strlen(rawVal->type)-1] == '*'
        && !metaInfo.empty() && metaInfo[0] == '@') {
        // we have C pointer with additional length info
        std::ostringstream os;
        os << "(" << expr << ")[0]" << metaInfo;
        return createMIVar(os.str().c_str());
    }

    return rawVal;
    
}

int Mi2Aif::getChildren(MIVar *var) {
    if (m_session == NULL || var == NULL) return 1;

    auto cmd = MIVarListChildren(quoteId(var->name).c_str());
    sendMICommand(cmd);

    if (!MICommandResultOK(cmd)) {
        if (MICommandResultClass(cmd) == MIResultRecordERROR) {
            char *err = MICommandResultErrorMessage(cmd);
            const char* errMsg = ( err ? err : "Error without message!");
            DVPRINT("MIVarGetChildren: Debugger error: %s\n", errMsg);
            dbg_error.reset(NewErrorEvent(DBGERR_DEBUGGER, "%s", errMsg));
            free(err);
        } else {
            DVPRINT("MIVarGetChildren: Bad response from GDB.\n");
            dbg_error.reset(NewErrorEvent(DBGERR_UNKNOWN, "Bad response from GDB."));
        }
        MICommandFree(cmd);
        return 1;
    }
    MIGetVarListChildrenInfo(cmd, var);
    MICommandFree(cmd);

    return 0;
}


// Find the more fundamental description of a type.   This is for when the
// application does something like "typedef int MyInt;
// So this function will map "MyInt" to "int"
std::string Mi2Aif::getPTypeAlias(const std::string& type, bool isExpression, bool strict) {
    // gdb can't properly parse the closure of nested templates (">>")
    // without a space in between ("> >")
    auto fixedType = (isExpression ? type : FixRightShift(type));

    // '/mt' is a modifier for ptype to not print methods or typedefs
    // prepending to the arguments is minor hack since CLIPtype doesn't care about
    // spaces in the argument.
    fixedType = "/mt " + fixedType;

    // I tried using "whatis" for the same role, but it doesn't include enum info,
    // but /mt at least cuts down the extra clutter in the debug files
    MICommand* cmd = CLIPtype(fixedType.c_str());
    sendMICommand(cmd);

    if (!MICommandResultOK(cmd)) {
        DVPRINT("GetStructInfo: CLIPtype call failed.\n");
        MICommandFree(cmd);
        return (strict ? "" : type);
    }

    auto retType = type;
    if (char* pinfo = CLIGetPtypeInfo(cmd)) {
        // info ends with a \n
        const char* info = pinfo;
        if (strncmp(info, "const ", 6) == 0) info += 6;

        auto len = strlen(info);
        auto bracket = strchr(info,'{');
        if (len > 1 && !bracket) {
            auto end = strchr(info, '\n');
            retType = std::string{info, end ? end-info : std::string::npos};
            DVPRINT("Get CLIPTypeInfo %s\n", retType.c_str());
        } else if (strncmp(info, "enum ", 5) == 0 && bracket) {
            retType = std::string{info, bracket};
            DVPRINT("Got enum alias %s\n", retType.c_str());
        } else if (strncmp(info, "union ", 6) == 0 && bracket) {
            retType = std::string{info, bracket};
            DVPRINT("Got union alias %s\n", retType.c_str());
        } else if (strncmp(info, "class ", 6) == 0 && bracket) {
            auto s = std::string{info+6, bracket};

            if (s.rfind(ANONYMOUS_NAMESPACE, 0) == 0) s.erase(0, ANONYMOUS_NAMESPACE.size());

            auto isWd = [](char c) { return isalnum(c) || c == '_' ||  c == ':'; };
            
            if (auto end = std::find_if_not(s.begin(), s.end(),isWd); end != s.end()) {
                if (*end == '<') end += FindTemplateLength(s, end-s.begin());
                s.erase(end, s.end());
            }
            if (s.empty()) { /* anonymous class */ }  
            else if (auto endBracket = findExprMatch(bracket)) {
                s += std::string{endBracket+1, strspn(endBracket+1, " *")};
            }

            retType = std::move(s);
            DVPRINT("got class alias %s\n", s.c_str());
        } else if (strict) {
            retType = "";
        }
                                                                
        free(pinfo);
    }
        
    MICommandFree(cmd);
    return retType;
}

// Get the fields of a struct from a gdb 'ptype" query
PTypeFields Mi2Aif::getPTypeFields(const std::string& type, bool isFortran)
{
    // gdb can't properly parse the closure of nested templates (">>")
    // without a space in between ("> >")
    auto fixedType = FixRightShift(type);
    fixedType = "/mtr " + fixedType;

    MICommand* cmd = CLIPtype(fixedType.c_str());
    sendMICommand(cmd);

    if (!MICommandResultOK(cmd)) {
        DVPRINT("GetStructInfo: CLIPtype call failed.\n");
        MICommandFree(cmd);
        return PTypeFields();
    }

    auto fields = PTypeFields(*cmd, isFortran);
    MICommandFree(cmd);
    return fields;
}

// Translate a gdb output into socket form
void Mi2Aif::parse(std::shared_ptr<ais::Type> type, const char* gdbRes, int refIdx) {

    auto eval = GdbResult{gdbRes ? gdbRes : ""};
    auto any = std::make_unique<ais::Any>(type, refIdx);
    DVPRINT("About to call any->parse\n");
    try {
        any->parse(eval, m_writer);
    } catch (std::exception& err) {
        DVPRINT("parsing failed with %s\n", err.what());
    }
}
    

// find a type object if the type has already been seen
std::shared_ptr<ais::Type> Mi2Aif::getKnownType(const std::string& name) {
    if (auto loc = m_types.find(name); loc != m_types.end()) return loc->second;
    else return std::shared_ptr<ais::Type>{};
}

// cache the type object for a processed type
std::shared_ptr<ais::Type> Mi2Aif::addKnownType(const std::string& name,
                                                std::shared_ptr<ais::Type> type) {
    m_types.insert(std::make_pair(name,type));
    return type;
}

// Map a pointer value to a reference index.  If the index is a new one,
// potentially put the object onto the queue of objects to be traversed.
// This allows a data structure to be aggegated independently of the
// specific pointer value which may not be consistent across ranks.
size_t Mi2Aif::swizzlePointer(const std::string& type, void* p)
{
    // special case for null
    if (!p) return 0;

    if (m_refMap.empty() && m_refs.size() == 1) {
        // we put off indexing the first block until we know we are dealing with pointers      
        auto top = m_refs[0]->getVar();
        DVPRINT("Top reference %s type %s value=%s\n", top->expr, top->type, top->value);
        void* addr_loc{};
        if (top->value && top->value[0]=='0') {
            // the top level is a pointer and we already have the pointer value                
            addr_loc = reinterpret_cast<void*>(strtoll(m_refs[0]->getVar()->value, nullptr, 0));
        } else {
            DVPRINT("Trying to get address of top object");
            // the top level is a value type, use & to try to get an address                   
            auto addrExpr = std::ostringstream{};
            addrExpr << "\"&(" << top->expr << ")\"";
            if (auto addrVar = createMIVar(addrExpr.str().c_str())) {
                auto addrStr = getVarValue(addrVar->name);

                DVPRINT("Got value %s\n", addrStr);

                addr_loc = reinterpret_cast<void*>(strtoll(addrStr, nullptr, 0));
                free(addrStr);
            } else {
                DVPRINT("createMIVar failed\n");
            }
        }
        m_refMap.insert(std::make_pair(addr_loc, 1));
    }

    DVPRINT("type=\"%s\"\n", type.c_str());

    size_t idx = m_refs.size()+1;
    if (auto [loc, added] = m_refMap.insert(std::make_pair(p,idx)); !added) {
        idx = loc->second;
    } else if ( type == "void") {
        DVPRINT("Not trying to evaluate a void*\n");
        // add to m_refs to keep the mapping consistent
        m_refs.emplace_back(std::shared_ptr<LazyMIVar>{});
    } else if ( type.find("decltype(") == 0) {
        // anonymous class type
        size_t end = 9, paren=1;
        for (size_t n=type.size(); end<n; ++end) {
            if (auto c = type[end]; c == '(') ++paren;
            else if (c == ')' && !--paren) break;
        }
        auto derefExpr = type.substr(9,end-9);
        DVPRINT("Creating new anonymous MIVar with %s\n", derefExpr.c_str());
        m_refs.emplace_back(std::make_shared<LazyMIVar>(*this, createMIVar(derefExpr.c_str())));
        
    } else {
        // regular pointer
        auto derefExpr = std::ostringstream{};
        derefExpr << "\"*(" << type << "*)" << p << "\"";
        DVPRINT("Creating new MIVar with %s\n", derefExpr.str().c_str());
        m_refs.emplace_back(std::make_shared<LazyMIVar>(*this, createMIVar(derefExpr.str().c_str())));
    }
    DVPRINT("Returning index %d\n", int(idx));
    return idx;
}

char* EvalExpression(MISession* session, const char* expression)
{
    MICommand *cmd = MIDataEvaluateExpression(quoteId(expression).c_str());
    SendMICommand(session, cmd);
    if (!MICommandResultOK(cmd)) {
        DVPRINT("EvalExpression got error from %s\n", expression);
        MICommandFree(cmd);
        return nullptr;
    }

    char* res = MIGetDataEvaluateExpressionInfo(cmd);
    MICommandFree(cmd);
    return res;
}

// get the raw gdb result f or evaluating an expression
char *Mi2Aif::getVarValue(char *var) {
    char *res;
    MICommand *cmd;

    DVPRINT("GetVarValue: Evaluate %s.\n", var);

    DBEPERFLOG(SetTimerStart("GetVarValue: -data-evaluate-expression"));
    cmd = MIVarEvaluateExpression(quoteId(var).c_str());
    DBEPERFLOG(SetTimerStopAndLog("GetVarValue: -data-evaluate-expression"));

    sendMICommand(cmd);
    if (!MICommandResultOK(cmd)) {
        DVPRINT("GetVarValue: Error\n");
        MICommandFree(cmd);
        throw NewErrorEvent(DBGERR_INTERNAL, "gdb expression evaluation failed");
    }
    res = MIGetVarEvaluateExpressionInfo(cmd);
    MICommandFree(cmd);

    // Check to see if the value was optimized out
    if (strcmp(res, "<value optimized out>") == 0) {
        free(res);
        throw NewErrorEvent(DBGERR_OPT, NULL);
    }

    return res;
}

namespace {
    // small helper RAII object to track when we are evaluating the original expression
    class DepthTracker {
        size_t& m_depth;
    public:
        DepthTracker(size_t& depth) : m_depth(depth) { ++m_depth; }
        ~DepthTracker() { --m_depth; }
        bool isTop() const { return (m_depth == 1); }
    };
}

char *LazyMIVar::getArrayDataString() {

    if (m_subArray) return m_subArray->getArrayDataString();

    auto var = getVar();
    auto session = m_mi.getSession();
    
    if (var->numchild == 0 && var->value) {
        DVPRINT("In GetArrayDataString  var value is %s\n", var->value);
        return strdup(var->value);
    } else if (var->expr) {
        DVPRINT("In GetArrayDataString %s\n", var->expr);
        DBEPERFLOG(SetTimerStart("GetArrayAIF: -data-evaluate-expression"));

        bool last_resort = false;
        auto expr = quoteId(var->expr);
        if (typeIsReference(var->type)) {
            expr = "*&" + expr;
            DVPRINT("Type is a reference, using %s\n", expr.c_str());
        }
        auto cmd = MIDataEvaluateExpression(expr.c_str());
        SendMICommand(session, cmd);
        while (!MICommandResultOK(cmd)) {
            if (MICommandResultClass(cmd) != MIResultRecordERROR) {
                DVPRINT("GetArrayAIF: Error from GDB, but no message\n");
                dbg_error.reset(NewErrorEvent(DBGERR_UNKNOWN,
                                              "Error without message!"));
            } else if (auto buf = MICommandResultErrorMessage(cmd)) {
                DVPRINT("GetArrayAIF: Debugger error: %s\n", buf);
                dbg_error.reset(NewErrorEvent(DBGERR_DEBUGGER, "%s", buf));
                free(buf);
                // try to resolve the expression as a last resort and try again
                if (!last_resort) {
                    last_resort = true;
                    MICommandFree(cmd);
                    cmd = MIVarInfoPathExpression(var->expr);
                    SendMICommand(session, cmd);
                    if (MICommandResultOK(cmd)) {
                        // reset the error message
                        dbg_error.reset();
                        auto buf2 = MIGetVarInfoPathExpression(cmd);
                        MICommandFree(cmd);
                        cmd = MIDataEvaluateExpression(buf2);
                        SendMICommand(session, cmd);
                        free(buf2);
                        continue;
                    }
                }
                DVPRINT("GetArrayAIF: last resort failed\n");
                MICommandFree(cmd);
                return nullptr;
            } else {
                DVPRINT("GetArrayAIF: Bad response from GDB\n");
                dbg_error.reset(NewErrorEvent(DBGERR_UNKNOWN, "Bad response from GDB"));
            }
            MICommandFree(cmd);

            return nullptr;
        }
        DBEPERFLOG(SetTimerStopAndLog("GetArrayAIF: -data-evaluate-expression"));

        auto buf = MIGetDataEvaluateExpressionInfo(cmd);
        MICommandFree(cmd);
        return buf;

    } else if (var->numchild > 0) {
        DVPRINT("GetArrayDataString passed a null expression with children children=%p\n",
                var->children);

        Mi2Aif mi2(session);
        mi2.getChildren(var.get());

        std::ostringstream os;
        os << "{";

        if (var->children) {
            for (int i=0; i<var->numchild; ++i) {
                if (i) os << ",";
                auto child = var->children[i];
                if (child->value) {
                    os << child->value;
                } else if (auto val = mi2.getVarValue(child->name)) {
                    os << val;
                    free(val);
                } else {
                    os << "<unavailable>";
                }
            }
        }

        os << "}";
        
        return strdup(os.str().c_str());

    } else {
        DVPRINT("GetArrayDataString passed a null expression\n");
        dbg_error.reset(NewErrorEvent(DBGERR_UNKNOWN, "null expression while evaluationg an array"));
        return nullptr;
    }
}

// Class to manage breaking the MI query for an array into smaller pieces.
// The calling sequence is simple:
// ArrayChunk chunker(session, val, shape);
// while (char* buf = chunker.getNext()) {
//    ... process buf ...
// }
// The class is not omniscient, we currently only chunk on the slowest changing
// dimension, so the actual block size could be larger than the nominal limit.
// Also, since we are plugging into the front end of gdb/mi (paging is disabled in this
// mode), we are limited to the set of types that can be split using a gdb expression.
class Chunker {
public:
    Chunker(Mi2Aif& mi, std::shared_ptr<MIVar> var, std::shared_ptr<ais::Type> varType);
    Chunker(Mi2Aif& mi, std::string fullExpr);
    ~Chunker();

    // evaluate the next chunk of the variable result.
    std::tuple<std::shared_ptr<ais::Type>, char*, bool> getNext();
    std::shared_ptr<LazyMIVar> getVar() { return m_var; }

    const static size_t DEFAULT_BLOCK_SIZE = 1000000; // one million

private:
    void determineChunking();
    size_t determineBlockSize();

    Chunker(const Chunker&) = delete;
    Chunker& operator=(const Chunker&) = delete;

    Mi2Aif& m_mi;               // Parent Mi2Aif

    std::shared_ptr<LazyMIVar> m_var; // MI variable of interest

    char* m_buf;                // allocated array for the result (contolled by this)

    size_t m_blockSize;         // maximum number of elements in each query
    std::string m_subExpr;      // encode expression to use for querying a chunk
    long m_subIdx;              // index for the current chunk
    long m_maxSubIdx;           // index for the highest chunk to return
    long m_chunk;               // stride for the chunking index.
};

Chunker::Chunker(Mi2Aif& mi, std::shared_ptr<MIVar> var, std::shared_ptr<ais::Type>)
    : m_mi{mi}, m_var(std::make_shared<LazyMIVar>(mi, var))
    , m_buf(nullptr), m_blockSize(determineBlockSize())
    , m_subIdx(0), m_maxSubIdx(0), m_chunk(0)
{
    if (m_blockSize) {
        determineChunking();
    }
}

Chunker::Chunker(Mi2Aif& mi, std::string fullExpr)
    : m_mi{mi}, m_var(std::make_shared<LazyMIVar>(mi, std::move(fullExpr)))
    , m_buf(nullptr), m_blockSize(determineBlockSize())
    , m_subIdx(0), m_maxSubIdx(0), m_chunk(0)
{
    if (m_blockSize) {
        determineChunking();
    }
}

Chunker::~Chunker()
{
    free(m_buf);
}

// Paging and the like is unavailable in MI mode.   Instead, this class and this function
// specifically examines the data type and determines how it can be extracted in multiple
// data print calls that each pull out a portion of the data structure
void Chunker::determineChunking()
{
    DVPRINT("in Chunker::determineChunking blocksize if %zu\n", m_blockSize);
    
    if (!m_blockSize)
        return;

    std::vector<int> arrayLens;
    for (const ais::Type* v = m_var->getType().get(); auto arr = dynamic_cast<const ais::Array*>(v); v=arr->getElemType()) {
        auto len = arr->getLen();
        if (auto vec = dynamic_cast<const ais::Vector*>(arr); vec && !len) {
            std::string sizeStr = std::string("(") + m_var->getExpr() + ").size()";
            char* sizeRes = EvalExpression(m_mi.getSession(), sizeStr.c_str());
            if (!sizeRes) {
                DVPRINT("could not get vector length\n");
                return;
            }

            len = std::max(atoi(sizeRes), 0);
            DVPRINT("Chunking vector got a size %d\n", len);
            free(sizeRes);
        }

        
        DVPRINT("adding length %d\n", len);
        arrayLens.push_back(len);
    }

    if (arrayLens.empty()) {
        return;
    }

    // C/C++ array, this may be real or partially artificial
    int dim = arrayLens.size();
    size_t subArrayLen  = 1;
    for (int i=1; i<dim; ++i)
        subArrayLen *= arrayLens[i];
    size_t numElems = subArrayLen * arrayLens[0];

    DVPRINT("In Chunker:: numElems is %zu\n", numElems);
    
    if (numElems <= m_blockSize)
        return;             // no need to divide

    // we handle at least the simplest  artificial arrays
    m_subExpr = m_var->getExpr();
    size_t atLoc = m_subExpr.rfind("[0]@");
    if (atLoc != std::string::npos) {
        m_subExpr.erase(atLoc);
    }
    
    // transform into a series of artificial arrays for the sub-sets
    m_subExpr += "[$$from$$]@$$len$$";
    m_subIdx = 0;
    m_maxSubIdx = arrayLens[0]-1;
    m_chunk = std::max(1, int(m_blockSize / subArrayLen));
    
    DVPRINT("Chunking: C array %s %ld to %ld by %ld\n",
            m_subExpr.c_str(), m_subIdx, m_maxSubIdx, m_chunk);
}

size_t Chunker::determineBlockSize() {
    static const char* blockSizeStr = ::getenv("GDB4HPC_BLOCK_ELEMENTS");
    if (blockSizeStr) {
        long lval = atol(blockSizeStr);
        if (lval >= 0)
            return lval;
    }

    return DEFAULT_BLOCK_SIZE;
}

// Return either the next chunk of data, or nullptr if there is no more data to be
// had.
// The returned array is controlled by Chunker and should not be freed by the
// caller.
std::tuple<std::shared_ptr<ais::Type>, char*, bool> Chunker::getNext()
{
    DVPRINT("In Chunker::getNext\n");

    bool isFinal = true;
    char* array = nullptr;

    if (m_subIdx > m_maxSubIdx) {
        return std::tuple(m_var->getType(), array, isFinal);
    }

    auto retType = m_var->getType();

    if (m_chunk == 0 && !m_buf) {
        DVPRINT("In Chunker::getNext not big enough to split\n");
        // splitting is not enabled, the array is not big enough to split,
        // or it's a form the splitter doesn't recognize, just return it
        // all
        array = m_buf = m_var->getArrayDataString();
        ++m_subIdx;
    } else {
        DVPRINT("In Chunker::getNext get next chunk\n");
        // get the next chunk of data
        std::swap(m_buf, array);
        if (array) {
            free(array);
            array = nullptr;
        }

        long maxIdx = std::min(m_maxSubIdx, m_subIdx+m_chunk-1);
        long len = maxIdx - m_subIdx + 1;

        std::string subExpr = m_subExpr;
        std::string patterns[3] = { "$$from$$", "$$to$$", "$$len$$" };
        long vals[3] = { m_subIdx, maxIdx, len };
        for (int i=0; i<3; ++i) {
            size_t loc = subExpr.find(patterns[i]);
            if (loc != std::string::npos)
                subExpr.replace(loc, patterns[i].size(), std::to_string(vals[i]));
        }

        DVPRINT("Evaluating sub-expression %s\n", subExpr.c_str());

        array = EvalExpression(m_mi.getSession(),const_cast<char*>(subExpr.c_str()));
        if (array) {
            // success!
            DVPRINT("Succeeded getting sub-array value: %.400s\n", array);

            // update the array type to contain just the length of the chunk
            auto subArrayType = std::static_pointer_cast<ais::Array>(m_var->getType());
            subArrayType = subArrayType->makeTyped(subArrayType->getElemTypePtr());
            subArrayType->setBounds(len, subArrayType->getIndex0() + m_subIdx);

            retType = std::make_shared<ais::ArrayChunk>(subArrayType, m_subIdx == 0,
                                                        maxIdx == m_maxSubIdx);
            
            m_subIdx = maxIdx+1;

        } else {
            // failure!  - fall back to just evaluating the whole kielbasa
            array = m_var->getArrayDataString();
            m_subIdx = m_maxSubIdx + 1;

            DVPRINT("Failed getting sub-array value, full result: %s\n", array);
        }
        m_buf = array;
    }

    isFinal = (m_subIdx > m_maxSubIdx);
    return std::tuple(retType, array, isFinal);
}

void Mi2Aif::sendMICommand(MICommand* command) {
    if (m_session) SendMICommand(m_session, command);
}

void WriteBuffer::writeInt(int val) { m_out << " " << val; }
void WriteBuffer::writeLong(long val) { m_out << " " << val; }
void WriteBuffer::writeLongLong(long long val) { m_out << " " << val; }
void WriteBuffer::writeFloat(float val) {
    m_out << " " << std::setprecision(std::numeric_limits<float>::max_digits10) << val;
}
void WriteBuffer::writeDouble(double val) {
    m_out << " " << std::setprecision(std::numeric_limits<double>::max_digits10) << val;
}
void WriteBuffer::writeLongDouble(long double val) {
    m_out << " " << std::setprecision(std::numeric_limits<long double>::max_digits10) << val;
}
void WriteBuffer::writeString(const std::string& val) {
    m_out  << " " << std::quoted(val);
}
void WriteBuffer::writeError(const std::string& val) {
    m_out  << " @" << std::quoted(val);
}

void WriteBuffer::writePointer(const std::string& type, void*) {
    // base does not support pointers
    m_out << " " << std::quoted(type) << " 0";
}

std::string WriteBuffer::exportResult() {
    auto res = m_out.str();
    m_out.str("");
    return res;
}

void MIWriteBuffer::writePointer(const std::string& type, void* val) {
    int idx = m_mi.swizzlePointer(type, val);
    m_out << " " << std::quoted(type) << " " << idx;
}

Mi2Aif::Mi2Aif(MISession* session) : m_session(session), m_depth(0),
                                     m_writeBuffer(std::make_shared<MIWriteBuffer>(*this)),
                                     m_writer(m_writeBuffer) {}



// Create the top level Chunker for an expression.   The Chunker
// will potentially break the expression into multiple evaluations if
// var is a large container
std::unique_ptr<Chunker> Mi2Aif::makeChunker(const std::string& var) {
    return std::make_unique<Chunker>(*this, var);
}

// Create an Chunker for a secondary reference.
// Note: chunking of chain of values hasn't really been tested..
std::unique_ptr<Chunker> Mi2Aif::makeChunker(std::shared_ptr<MIVar> mivar) {
    auto origType = TypeParser::getType(*this, mivar.get(), nullptr);
    return std::make_unique<Chunker>(*this, mivar, origType);
}

// Special handling for the original expression passed by the user.
// This comes into play if a tree/graph/list structure might contain a
// reference to the initial value
void Mi2Aif::initTopRef(std::shared_ptr<LazyMIVar> lvar) {
    if (!lvar) return;
    if (auto ptrT = std::dynamic_pointer_cast<ais::Pointer>(lvar->getType()); !ptrT) {
        // Save the input expression - if it contains pointers we will need
        // to index the address of this object to resolve any back-pointers in
        // the structure.
        DVPRINT("Pushing top expression %s, type %s\n", 
            lvar->getExpr().c_str(), lvar->getType()->getTypeName().c_str());
        m_refs.push_back(lvar);
    }
}

// Get the next chunk of data from in socket protocol form.
// Node: currently, we are transmitting simple ascii text, if we switch to
// something more specialized and (hopefully) performant the return type
// would not be a string.
std::string Mi2Aif::nextChunk(Chunker& chunker, bool& isFinal) {
    DVPRINT("nextChunk\n");

    if (m_refs.size() == 0) {
        initTopRef(chunker.getVar());
    }
    auto initNumRefs = m_refs.size();

    auto [ type, gdbArr, chunkIsFinal ] = chunker.getNext();
    isFinal = chunkIsFinal;

    if (!gdbArr) {
        if (!dbg_error) dbg_error.reset(NewErrorEvent(DBGERR_INTERNAL, "Getting array data failed."));
        throw dbg_error.release();
    }

    parse(type, gdbArr, 0);

    // add the reference elements as members of the struct - note that
    // the current val is deleted on the first iteration of this loop
    auto refLimit = (clientPrintElements > 0 ?
                     static_cast<size_t>(clientPrintElements) :
                     std::numeric_limits<size_t>::max());

    // create the elements, note that m_refs can grow inside the loop
    for (size_t i=initNumRefs; i<m_refs.size() && i<refLimit ; ++i) {
        if (m_refs[i]) writeVal(m_refs[i]->getVar(), i+1);
    }

    m_writer.resetTypes();
    return m_writeBuffer->exportResult();
}

// translate a variable into socket form - the date is pumped
// through via m_writer
void Mi2Aif::writeVal(std::shared_ptr<MIVar> var, int refIdx) {
    auto chunker = makeChunker(var);
    auto [ type, arr, isFinal ] = chunker->getNext();
    parse(type, arr, refIdx);
}

// AIFStream adapter to evaluate and parse a value on demand.
EvalChunks::EvalChunks(MISession* session, const std::string& var)
    : m_mi2aif(session), m_var(var) {}

std::string EvalChunks::getNextChunk(bool& isFinal) {
    if (!m_chunker) initChunker();

    // Mi2Aif does the work and returns a chunk of socket protocol
    // data
    return m_mi2aif.nextChunk(*m_chunker, isFinal);
}

void EvalChunks::initChunker() {
    if (!m_chunker) m_chunker = m_mi2aif.makeChunker(m_var);
}

AIFStream* AIFStream::fromMultiThreadResult(const char* val) {
    auto mtr = std::make_shared<ais::MultiThreadResult>();
    auto any = ais::Any(mtr, 0);
    auto buff = std::make_shared<WriteBuffer>();
    auto out = ais::Writer{buff};

    // instead of faking MultiThreadResult::parse, unroll ais::Any::parse
    // to get the right ais data.   It's not ideal, but makes sense
    // as long as this is the only weirdo.
    out.writeType(any);
    out.writeInt(0);   // index id
    out.writeString(val);

    return new AIFStream(buff->exportResult(), false);
}    

AIFStream* AIFStream::fromPybeResult(const std::string& str) {
    // the goal of this function is to pack up `str` *exactly* as it comes in.
    // ais::Literal is the most fitting for this, but we still can't do a
    // nice any.parse(...) call because:
    //
    // 1. ais::Literal stops parsing `str` at the first whitespace. if `str` has
    //    spaces in it, it will be truncated.
    // 2. if `str` begins with quotes, ais::Literal will remove them. we want
    //    to keep quotes in this case for printing strings etc. i.e. we want
    //    to keep the formatting that python gave us exactly.
    // 3. python often prints things surrounded by </>, e.g. built-in modules
    //    look like "<module 'sys' (built-in)>" when printed. ais::Type::parse
    //    assumes anything that begins with '<' is an error and treats it
    //    differently. in particular any rank information is lost.
    //
    // maybe this is calling for a new ais::PybeResult type or something similar
    // that fixes all of these problems, but for now I'm just going to make the
    // tests pass with a hack until it becomes a problem.

    auto lit = std::make_shared<ais::Literal>();
    auto any = ais::Any(lit, 0);
    auto buf = std::make_shared<WriteBuffer>();
    auto writer = ais::Writer{buf};

    writer.writeType(any);
    writer.writeInt(0); // index id
    writer.writeString(str);

    return new AIFStream(buf->exportResult(), false);
}

TEST_CASE("RajaSubArray::generateIndices: 1x4 slice of a 4x4 view") {
    auto bounds = mdspan{{4, 4}, {4, 1} };

    // "p view(0, 0..3)"
    const auto ranges = std::vector<std::pair<int, int>>{{0, 0}, {0, 3}};

    const auto indices = RajaSubArray::Tester::generateIndices(ranges, bounds);

    const auto position_truth = std::array{0, 1, 2, 3};
    const auto index_truth = std::array{0, 1, 2, 3};

    REQUIRE(indices.size() == position_truth.size());
    REQUIRE(indices.size() == index_truth.size());

    for (size_t i = 0; i < indices.size(); i++) {
        CHECK_MESSAGE(indices[i].position == position_truth[i], i);
        CHECK_MESSAGE(indices[i].index == index_truth[i], i);
    }
}

TEST_CASE("RajaSubArray::generateIndices: 1x4 slice of a transposed 4x4 view") {
    auto bounds = mdspan{{4, 4}, {1, 4}};

    // "p view(0, 0..3)"
    const auto ranges = std::vector<std::pair<int, int>>{{0, 0}, {0, 3}};

    const auto indices = RajaSubArray::Tester::generateIndices(ranges, bounds);

    const auto position_truth = std::array{0, 1, 2, 3};
    const auto index_truth = std::array{0, 4, 8, 12};

    REQUIRE(indices.size() == position_truth.size());
    REQUIRE(indices.size() == index_truth.size());

    for (size_t i = 0; i < indices.size(); i++) {
        CHECK_MESSAGE(indices[i].position == position_truth[i], i);
        CHECK_MESSAGE(indices[i].index == index_truth[i], i);
    }
}

TEST_CASE("RajaSubArray::generateIndices: 4x1 slice of a 4x4 view") {
    auto bounds = mdspan{{4, 4}, {4, 1}};

    // "p view(0..3, 3)"
    const auto ranges = std::vector<std::pair<int, int>>{{0, 3}, {3, 3}};

    const auto indices = RajaSubArray::Tester::generateIndices(ranges, bounds);

    const auto position_truth = std::array{0, 1, 2, 3};
    const auto index_truth = std::array{3, 7, 11, 15};

    REQUIRE(indices.size() == position_truth.size());
    REQUIRE(indices.size() == index_truth.size());

    for (size_t i = 0; i < indices.size(); i++) {
        CHECK_MESSAGE(indices[i].position == position_truth[i], i);
        CHECK_MESSAGE(indices[i].index == index_truth[i], i);
    }
}

TEST_CASE("RajaSubArray::generateIndices: 4x1 slice of a transposed 4x4 view") {
    auto bounds = mdspan{{4, 4}, {1, 4}};

    // "p view(0..3, 1)"
    const auto ranges = std::vector<std::pair<int, int>>{{0, 3}, {1, 1}};

    const auto indices = RajaSubArray::Tester::generateIndices(ranges, bounds);

    const auto position_truth = std::array{0, 1, 2, 3};
    const auto index_truth = std::array{4, 5, 6, 7};

    REQUIRE(indices.size() == position_truth.size());
    REQUIRE(indices.size() == index_truth.size());

    for (size_t i = 0; i < indices.size(); i++) {
        CHECK_MESSAGE(indices[i].position == position_truth[i], i);
        CHECK_MESSAGE(indices[i].index == index_truth[i], i);
    }
}

TEST_CASE("RajaSubArray::generateIndices: 2x4 slice of a 4x4 view") {
    auto bounds = mdspan{{4, 4}, {4, 1}};

    // "p view(1..2, 0..3)"
    const auto ranges = std::vector<std::pair<int, int>>{{1, 2}, {0, 3}};

    const auto indices = RajaSubArray::Tester::generateIndices(ranges, bounds);

    const auto position_truth = std::array{0, 1, 2, 3, 4, 5, 6, 7};
    const auto index_truth = std::array{4, 5, 6, 7, 8, 9, 10, 11};

    REQUIRE(indices.size() == position_truth.size());
    REQUIRE(indices.size() == index_truth.size());

    for (size_t i = 0; i < indices.size(); i++) {
        CHECK_MESSAGE(indices[i].position == position_truth[i], i);
        CHECK_MESSAGE(indices[i].index == index_truth[i], i);
    }
}

TEST_CASE("RajaSubArray::generateIndices: 3x5x2 slice of a 3x5x2 view") {
    auto bounds = mdspan{{3, 5, 2}, {10, 2, 1}};

    // "p view(0..2, 0..4, 0..1)"
    const auto ranges = std::vector<std::pair<int, int>>{{0, 2}, {0, 4}, {0, 1}};

    const auto indices = RajaSubArray::Tester::generateIndices(ranges, bounds);

    const auto position_truth = std::array{
        0,  1,  2,  3,  4,  5,  6,  7,  8,  9,
        10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
        20, 21, 22, 23, 24, 25, 26, 27, 28, 29
    };
    const auto index_truth = std::array{
        0,  1,  2,  3,  4,  5,  6,  7,  8,  9,
        10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
        20, 21, 22, 23, 24, 25, 26, 27, 28, 29
    };

    REQUIRE(indices.size() == position_truth.size());
    REQUIRE(indices.size() == index_truth.size());

    for (size_t i = 0; i < indices.size(); i++) {
        CHECK_MESSAGE(indices[i].position == position_truth[i], i);
        CHECK_MESSAGE(indices[i].index == index_truth[i], i);
    }
}

TEST_CASE("RajaSubArray::generateIndices: 1x5x2 slice of a 3x5x2 view") {
    auto bounds = mdspan{{3, 5, 2}, {10, 2, 1}};

    // "p view(1, 0..4, 0..1)"
    const auto ranges = std::vector<std::pair<int, int>>{{1, 1}, {0, 4}, {0, 1}};

    const auto indices = RajaSubArray::Tester::generateIndices(ranges, bounds);

    const auto position_truth = std::array{
        0,  1,  2,  3,  4,  5,  6,  7,  8,  9
    };
    const auto index_truth = std::array{
        10, 11, 12, 13, 14, 15, 16, 17, 18, 19
    };

    REQUIRE(indices.size() == position_truth.size());
    REQUIRE(indices.size() == index_truth.size());

    for (size_t i = 0; i < indices.size(); i++) {
        CHECK_MESSAGE(indices[i].position == position_truth[i], i);
        CHECK_MESSAGE(indices[i].index == index_truth[i], i);
    }
}

TEST_CASE("RajaSubArray::generateIndices: 2x4 slice of a transposed 4x4 view") {
    auto bounds = mdspan{{4, 4}, {1, 4}};

    // "p view(1..2, 0..3)"
    const auto ranges = std::vector<std::pair<int, int>>{{1, 2}, {0, 3}};

    const auto indices = RajaSubArray::Tester::generateIndices(ranges, bounds);

    const auto position_truth = std::array{0, 1, 2, 3, 4, 5, 6, 7};
    const auto index_truth = std::array{1, 5, 9, 13, 2, 6, 10, 14};

    REQUIRE(indices.size() == position_truth.size());
    REQUIRE(indices.size() == index_truth.size());

    for (size_t i = 0; i < indices.size(); i++) {
        CHECK_MESSAGE(indices[i].position == position_truth[i], i);
        CHECK_MESSAGE(indices[i].index == index_truth[i], i);
    }
}

TEST_CASE("RajaSubArray::generateIndices: 3x3 slice of a 5x5 view with an offset") {
    auto bounds = mdspan{{5, 5}, {5, 1}, {-1, -1}};

    // "p view(0..2, 0..2)"
    const auto ranges = std::vector<std::pair<int, int>>{{0, 2}, {0, 2}};

    const auto indices = RajaSubArray::Tester::generateIndices(ranges, bounds);

    // logical data layout with given offset:
    // | (-1, -1), (-1, 0), (-1, 1), (-1, 2), (-1, 3) |
    // |                                              |
    // | (0,  -1), (0,  0), (0,  1), (0,  2), (0,  3) |
    // |                                              |
    // | (1,  -1), (1,  0), (1,  1), (1,  2), (1,  3) |
    // |                                              |
    // | (2,  -1), (2,  0), (2,  1), (2,  2), (2,  3) |
    // |                                              |
    // | (3,  -1), (3,  0), (3,  1), (3,  2), (3,  3) |

    const auto position_truth = std::array{0, 1, 2, 3, 4, 5, 6, 7, 8};
    const auto index_truth = std::array{6, 7, 8, 11, 12, 13, 16, 17, 18};

    REQUIRE(indices.size() == position_truth.size());
    REQUIRE(indices.size() == index_truth.size());

    for (size_t i = 0; i < indices.size(); i++) {
        CHECK_MESSAGE(indices[i].position == position_truth[i], i);
        CHECK_MESSAGE(indices[i].index == index_truth[i], i);
    }
}

TEST_CASE("RajaSubArray::generateIndices: 3x3 slice of a 5x5 view with an offset using negative indices") {
    auto bounds = mdspan{{5, 5}, {5, 1}, {-1, -1}};

    // "p view(0..2, 0..2)"
    const auto ranges = std::vector<std::pair<int, int>>{{-1, 1}, {-1, 1}};

    const auto indices = RajaSubArray::Tester::generateIndices(ranges, bounds);

    // logical data layout with given offset:
    // | (-1, -1), (-1, 0), (-1, 1), (-1, 2), (-1, 3) |
    // |                                              |
    // | (0,  -1), (0,  0), (0,  1), (0,  2), (0,  3) |
    // |                                              |
    // | (1,  -1), (1,  0), (1,  1), (1,  2), (1,  3) |
    // |                                              |
    // | (2,  -1), (2,  0), (2,  1), (2,  2), (2,  3) |
    // |                                              |
    // | (3,  -1), (3,  0), (3,  1), (3,  2), (3,  3) |

    const auto position_truth = std::array{0, 1, 2, 3, 4, 5, 6, 7, 8};
    const auto index_truth = std::array{0, 1, 2, 5, 6, 7, 10, 11, 12};

    REQUIRE(indices.size() == position_truth.size());
    REQUIRE(indices.size() == index_truth.size());

    for (size_t i = 0; i < indices.size(); i++) {
        CHECK_MESSAGE(indices[i].position == position_truth[i], i);
        CHECK_MESSAGE(indices[i].index == index_truth[i], i);
    }
}
