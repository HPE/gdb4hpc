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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include <string>
#include <unordered_map>
#include <vector>
#include <iomanip>
#include <regex>

#include "dbgevent.h"
#include "dbgfn.h"
#include "dbgsrv_defs.h"
#include "dbgsrv_libmi2aif.h"
#include "dbgsrv_util.h"
#include "defs.h"
#include "useful.h"
#include "ais.h"

#include "unittest_macros.h"

// Used to fix infinite loop issue for recursive calls to whatis
extern int clientPrintElements;
static char *GetArrayDataString(MISession* session, MIVar *var);
static char* EvalExpression(MISession* session, const char* expression);
static std::string FixRightShift(std::string type);
static const char* FindTemplateEnd(const char* type);
static size_t FindTemplateLength(const std::string& type, size_t pos);

// external prototypes
extern void SendMICommand(MISession *, MICommand *);

/*
** Static prototypes
*/
namespace {

class Mi2Aif;

class LazyMIVar {
    // handle for an MIVar which defers var, type, and expression calculations
private:
    Mi2Aif& m_mi;

    std::shared_ptr<MIVar> m_var;
    std::shared_ptr<ais::Type> m_type;
    std::string m_expr;

public:
    LazyMIVar(Mi2Aif& mi, std::string expr);
    LazyMIVar(Mi2Aif& mi, std::shared_ptr<MIVar> var);

    std::shared_ptr<MIVar> getVar();
    std::shared_ptr<ais::Type> getType();
    std::string getExpr();

private:
    std::string arrayToFnCall(const std::string& expr);
};

class MIWriteBuffer; class WriteBuffer; struct TypeName; class ArraySlicer;

// Class to handle templated types.   For templated classes, gdb will report some
// types in terms of template arguments, e.g. std::shared_ptr<T> where it
// reported the template mapping earlier with "[with T = int]"
class TypeMapper {
public:
    TypeMapper(MIList* oobs);
    TypeMapper() = default; // construct empty

    TypeMapper(const TypeMapper&) = delete;
    TypeMapper(TypeMapper&& other) = default;
    TypeMapper& operator=(const TypeMapper&) = delete;

    std::string operator()(std::string&& type);
private:
    std::vector<std::pair<std::regex,std::string>> typeMap;
};

class PTypeFields {
    // derive class fields and parent classes from the result of a ptype command
public:
    PTypeFields(MICommand& sentCmd, bool isFortran);
    PTypeFields() = default; // construct empty

    const std::vector<std::string>& parentClasses() const { return m_parentClasses; }
    const std::vector<std::string>& fields() const { return m_fields; }

    // name, type
    std::pair<std::string, std::string>
    parseField(const std::string& type, const std::string& fieldStr);

private:
    void initFortranFields(MICommand& sentCmd);

    std::vector<std::string> m_parentClasses;
    std::vector<std::string> m_fields;

    TypeMapper m_typeMapper;
};

class Mi2Aif {
private:
    MISession* m_session;
    size_t m_depth;
    std::vector<std::shared_ptr<LazyMIVar>> m_refs;
    std::unordered_map<void*,size_t> m_refMap;

    std::shared_ptr<MIWriteBuffer> m_writeBuffer;
    ais::Writer m_writer;

    std::map<std::string, std::shared_ptr<ais::Type>> m_types;

public:
    Mi2Aif(MISession* session);

    std::shared_ptr<MIVar> createMIVar(const char *);
    AIFStream* getAIF(std::shared_ptr<MIVar>);
    int getChildren(MIVar *);
    char* getVarValue(char *);
    MISession* getSession() const { return m_session; }
    std::string getPTypeAlias(const std::string& type, bool strict);
    PTypeFields getPTypeFields(const std::string& type, bool isFortran);

    std::string nextSlice(ArraySlicer& slicer, bool& isFinal);
    std::unique_ptr<ArraySlicer> makeSlicer(const std::string& var);
    std::unique_ptr<ArraySlicer> makeSlicer(std::shared_ptr<MIVar> var);
    void initTopRef(std::shared_ptr<LazyMIVar> var);
    std::shared_ptr<ais::Type> getKnownType(const std::string& name);
    std::shared_ptr<ais::Type> addKnownType(const std::string& name,
                                            std::shared_ptr<ais::Type> type);

    size_t swizzlePointer(const std::string& type, void* val);

    // parse `expr` for meta info ("[[...]]") and return the separated meta info
    // with the rest of the original string, in that order. If no meta info is
    // present, the meta info portion of the returned tuple will be an empty string.
    std::tuple<std::string, std::string> extractMetaInfo(const char* expr);

private:
    void writeVal(std::shared_ptr<MIVar> var, int refIdx);
    void sendMICommand(MICommand* command);
    std::shared_ptr<MIVar> createMIVarWithMetaInfo(const char* expr, const std::string& metaInfo);
    void parse(std::shared_ptr<ais::Type> type, const char* gdbRes, int refIdx);
};

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
    
// Utility class for passing type string around.  type is the portion that has been
// processed, and decoration is the part left to process.
struct TypeName {
    TypeName(const std::string& s, size_t pos) : type(s.substr(0,pos)), decoration(s.substr(pos)) {}
    TypeName(const TypeName& t, size_t pos) :
        type(t.type + t.decoration.substr(0,pos)), decoration(t.decoration.substr(pos)) {}
    TypeName(std::string&& t, std::string&& d) : type(std::move(t)), decoration(std::move(d)) {}
    TypeName() = default;
    std::string type;
    std::string decoration;
};

// Functions for creating an ais::Type from the string reported by gdb
class TypeParser {
public:
    // Top level parsing function. Invoking it will detect which language the current
    // frame is in and then call one of the sub-parsers (getCpp/FtnType)
    static std::shared_ptr<ais::Type> getType(Mi2Aif& mi, MIVar* var, const char* typeStr);
    static std::shared_ptr<ais::Type> getCppType(Mi2Aif& mi, MIVar* var, const char* typeStr);
    static std::shared_ptr<ais::Type> getFtnType(Mi2Aif& mi, const char* typeStr);

    static std::shared_ptr<ais::Type> cppWrap(std::shared_ptr<ais::Type> elemType, TypeName t);
    static std::shared_ptr<ais::Type> ftnWrap(std::shared_ptr<ais::Type> elemType, TypeName t);
    static std::shared_ptr<ais::Type> charWrap(std::shared_ptr<ais::Type> elemType, TypeName t);

    static std::tuple<std::string, std::string, std::string> getContainerTypes(TypeName t);
    static std::shared_ptr<ais::Type> stlContainer(Mi2Aif& mi, const ais::Array& contType,
                                                   TypeName t);
    static std::shared_ptr<ais::Type> stdArray(Mi2Aif& mi, TypeName t);
    static std::shared_ptr<ais::Type> stdConditional(Mi2Aif& mi, TypeName t);
    static std::shared_ptr<ais::Type> stdString(Mi2Aif& mi, TypeName t);
    static std::shared_ptr<ais::Type> smartPointer(Mi2Aif& mi, TypeName t);
    static std::shared_ptr<ais::Type> wrapVoidFunc(Mi2Aif&, TypeName t);
    static std::shared_ptr<ais::Type> wrapEnum(Mi2Aif&, TypeName t);
    static std::shared_ptr<ais::Type> wrapUnion(Mi2Aif&, TypeName t);
    static std::shared_ptr<ais::Type> ftnCharacter(Mi2Aif& mi, TypeName t);
    static std::shared_ptr<ais::Type> unimplemented(Mi2Aif&, TypeName type);

    static std::string sanitizeType(const std::string& type);

    // Supported languages parsable by getType
    enum class Lang {
        Unknown,
        Cpp,
        Ftn,
    };

    // Query gdbmi for the current lang setting
    static Lang queryCurrentLang(MISession* mi);

    using Func = std::function<std::shared_ptr<ais::Type>(Mi2Aif& mi, TypeName t)>;
    using Prim = std::pair<std::string, Func>;

    template<class T>
    static Func PrimWrap() { return [](Mi2Aif&, TypeName t) {
                                        return cppWrap(std::make_shared<T>(),std::move(t)); } ;}

    template<class T>
    static Func FtnWrap() { return [](Mi2Aif&, TypeName t) {
                                       return ftnWrap(std::make_shared<T>(), std::move(t)); } ;}

    template<class T>
    static Func ComplexWrap() {
        return [](Mi2Aif&, TypeName t) {
                   return ftnWrap(std::make_shared<ais::Complex>(std::make_shared<T>()),
                                  std::move(t)); } ;}

    template<class T>
    static Func CharWrap() {
        return [](Mi2Aif&, TypeName t) {
                   return charWrap(std::make_shared<T>(),std::move(t)); } ;}

    template<class T>
    static Func ContainerWrap() {
        return [](Mi2Aif& mi, TypeName t) { return stlContainer(mi, T(), std::move(t)); }; }

    static Func SmartPointerWrap() {
        return [](Mi2Aif& mi, TypeName t) { return smartPointer(mi, std::move(t)); }; }

    static void addStructMembers(Mi2Aif& mi, ais::Struct& stype, MIVar* var,
                                 const std::string& type);

    static std::vector<Prim> m_cppPrims;
    static std::vector<Prim> m_cppStlPrims;
    static std::vector<Prim> m_fortranPrims;
    static std::vector<std::string> m_ignore;
    static inline bool comparePrims(const Prim& a, const Prim& b) { return a.first < b.first; }


    static Func m_logical;
    static Func m_int1;
    static Func m_int2;
    static Func m_int4;
    static Func m_int8;
    static Func m_int16;
    static Func m_real4;
    static Func m_real8;
    static Func m_real16;
    static Func m_complex4;
    static Func m_complex8;
    static Func m_complex16;
};

TypeParser::Func TypeParser::m_logical = FtnWrap<ais::Logical>();
TypeParser::Func TypeParser::m_int1 = FtnWrap<ais::SignedChar>();
TypeParser::Func TypeParser::m_int2 = FtnWrap<ais::Short>();
TypeParser::Func TypeParser::m_int4 = FtnWrap<ais::Int>();
TypeParser::Func TypeParser::m_int8 = FtnWrap<ais::Long>();
TypeParser::Func TypeParser::m_int16 = FtnWrap<ais::LongLong>();
TypeParser::Func TypeParser::m_real4 = FtnWrap<ais::Float>();
TypeParser::Func TypeParser::m_real8 = FtnWrap<ais::Double>();
TypeParser::Func TypeParser::m_real16 = FtnWrap<ais::LongDouble>();
TypeParser::Func TypeParser::m_complex4 = ComplexWrap<ais::Float>();
TypeParser::Func TypeParser::m_complex8 = ComplexWrap<ais::Double>();
TypeParser::Func TypeParser::m_complex16 = ComplexWrap<ais::LongDouble>();

std::vector<TypeParser::Prim> TypeParser::m_cppPrims =
{
 {"bool",PrimWrap<ais::Bool>()},
 {"char", CharWrap<ais::Char>()},
 {"double",PrimWrap<ais::Double>()},
 {"enum", wrapEnum},
 {"float",PrimWrap<ais::Float>()},
 {"int",PrimWrap<ais::Int>()},
 {"int8_t",PrimWrap<ais::Int>()},
 {"long",PrimWrap<ais::Long>()},
 {"long int",PrimWrap<ais::Long>()},
 {"long long",PrimWrap<ais::LongLong>()},
 {"long double",PrimWrap<ais::LongDouble>()},
 {"std::size_t",PrimWrap<ais::Long>()},
 {"size_t",PrimWrap<ais::UnsignedLong>()},
 {"short",PrimWrap<ais::Short>()},
 {"short int",PrimWrap<ais::Short>()},
 {"signed char",CharWrap<ais::SignedChar>()},
 {"uint8_t",PrimWrap<ais::UnsignedChar>()},
 {"uint16_t",PrimWrap<ais::UnsignedShort>()},
 {"uint32_t",PrimWrap<ais::UnsignedInt>()},
 {"uint64_t",PrimWrap<ais::UnsignedLong>()},
 {"union", wrapUnion},
 {"unsigned char",CharWrap<ais::UnsignedChar>()},
 {"unsigned int",PrimWrap<ais::UnsignedInt>()},
 {"unsigned long",PrimWrap<ais::UnsignedLong>()},
 {"unsigned long int",PrimWrap<ais::UnsignedLong>()},
 {"unsigned long long",PrimWrap<ais::UnsignedLongLong>()},
 {"unsigned short",PrimWrap<ais::UnsignedShort>()},
 {"unsigned short int",PrimWrap<ais::UnsignedShort>()},
 {"void", wrapVoidFunc}
};

std::vector<TypeParser::Prim> TypeParser::m_cppStlPrims =
{
 {"std::array", stdArray},
 {"std::conditional_t", stdConditional},
 {"std::deque", ContainerWrap<ais::Deque>()},
 {"std::stack", ContainerWrap<ais::Stack>()},
 {"std::vector", ContainerWrap<ais::Vector>()},
 {"std::priority_queue", ContainerWrap<ais::PriorityQueue>()},
 {"std::queue", ContainerWrap<ais::Queue>()},
 {"std::shared_ptr", SmartPointerWrap()},
 {"std::unique_ptr", SmartPointerWrap()},
};

std::vector<TypeParser::Prim> TypeParser::m_fortranPrims =
{
 {"CHARACTER",ftnCharacter},
 {"COMPLEX",m_complex4},
 {"COMPLEX(4)",m_complex4},
 {"COMPLEX(8)",m_complex8},
 {"COMPLEX(16)",m_complex16},
 {"COMPLEX(KIND=4)",m_complex4},
 {"COMPLEX(KIND=8)",m_complex8},
 {"COMPLEX(KIND=16)",m_complex16},
 {"COMPLEX*8",m_complex4},
 {"COMPLEX*16",m_complex8},
 {"COMPLEX*32",m_complex16},
 {"DOUBLE COMPLEX", m_complex8},
 {"QUAD COMPLEX", m_complex16},
 {"INT", m_int16}, // this can show up as the result of sizeof(...) commands, even in ftn.
                   // reporting it as int16 since it's essentially a size_t.
 {"INTEGER",m_int4},
 {"INTEGER(1)",m_int1},
 {"INTEGER(2)",m_int2},
 {"INTEGER(4)",m_int4},
 {"INTEGER(8)",m_int8},
 {"INTEGER(16)",m_int16},
 {"INTEGER(KIND=1)",m_int1},
 {"INTEGER(KIND=2)",m_int2},
 {"INTEGER(KIND=4)",m_int4},
 {"INTEGER(KIND=8)",m_int8},
 {"INTEGER(KIND=16)",m_int16},
 {"INTEGER*1",m_int1},
 {"INTEGER*2",m_int2},
 {"INTEGER*4",m_int4},
 {"INTEGER*8",m_int8},
 {"INTEGER*16",m_int16},
 {"BYTE", m_int1},
 {"LOGICAL", m_logical},
 {"LOGICAL(1)",m_logical},
 {"LOGICAL(2)",m_logical},
 {"LOGICAL(4)",m_logical},
 {"LOGICAL(8)",m_logical},
 {"LOGICAL(16)",m_logical},
 {"LOGICAL*1",m_logical},
 {"LOGICAL*2",m_logical},
 {"LOGICAL*4",m_logical},
 {"LOGICAL*8",m_logical},
 {"LOGICAL*16",m_logical},
 {"LOGICAL(KIND=1)",m_logical},
 {"LOGICAL(KIND=2)",m_logical},
 {"LOGICAL(KIND=4)",m_logical},
 {"LOGICAL(KIND=8)",m_logical},
 {"LOGICAL(KIND=16)",m_logical},
 {"REAL",m_real4},
 {"REAL(4)",m_real4},
 {"REAL(8)",m_real8},
 {"REAL(16)",m_real16},
 {"REAL(KIND=4)",m_real4},
 {"REAL(KIND=8)",m_real8},
 {"REAL(KIND=16)",m_real16},
 {"REAL*4",m_real4},
 {"REAL*8",m_real8},
 {"REAL*16",m_real16},
 {"DOUBLE PRECISION", m_real8},
 {"STRING",unimplemented}
};

std::vector<std::string> TypeParser::m_ignore = { "const", "volatile", "shared",
                                                  "SHARED", "pointer,", "POINTER," };
class ArraySlicer;

// Data result reported by gdb.
// In the basic form, this is just the string reported on gdb on the
// "print expression", but large arrays be broken into multiple
// "bite-sized" pieces via the ArraySlicer object.
class GdbResult : public ais::InterpretStream {
public:
    GdbResult(const char* buf) : m_buffer{buf}, m_pos{buf} {}
    ~GdbResult() = default;
    const char* get() override { return m_pos; }
    void setPos(const char* pos) override { m_pos = pos; }
    bool mayBeTruncated() override;

    GdbResult(const GdbResult&) = delete;
    GdbResult& operator=(const GdbResult&) = delete;
    
private:
    const char* m_buffer;
    const char* m_pos;
};

bool GdbResult::mayBeTruncated() {
    return (clientPrintElements > 0);
}

// Slices implementation where the data is created on demand by
// using gdb/mi to evaluate an expression.
class EvalSlices : public AIFStream::Slices {
public:
    EvalSlices(MISession* session, const std::string& var);
    std::string getNextSlice(bool& isFinal) override;
private:
    void initSlicer();
    Mi2Aif m_mi2aif;
    std::string m_var;
    std::unique_ptr<ArraySlicer> m_slicer;
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
    // the next (i.e. first) slice.
    if (!var) throw NewErrorEvent(DBGERR_INTERNAL, "null expression");
    auto slices = std::make_shared<EvalSlices>(session, var);
    return new AIFStream(slices);
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

std::shared_ptr<ais::Type> LazyMIVar::getType() {
    static std::regex anonymousRE(R"((const)? *class)");

    if (m_type) return m_type;

    if (m_var) {
        m_type = TypeParser::getType(m_mi, m_var.get(), nullptr);
    } else {
        if (m_expr.empty()) {
            throwDbgError("No expression to create variable with.");
        }

        // Use whatis commands to determine the gdb type of `exp`. Accounts
        // for the frontend's confusion with ftn array indices and c function calls.
        // If no type is found, returns an empty string.
        auto getGDBType = [this](const std::string& exp) {
            // Send a "whatis `exp`" command to gdb, returns "" on failure
            auto SendWhatIs = [this](const std::string& exp) {
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
            };

            try {
                return SendWhatIs(exp);
            } catch (...) {
                // the frontend can't tell if "(12)" is a C function call or a
                // fortran array index and thus sends function calls to us
                // as "myFunc[12]" instead of "myFunc(12)"
                if (auto fnCallExpr = arrayToFnCall(exp); !fnCallExpr.empty()) {
                    return SendWhatIs(fnCallExpr);
                } else {
                    throw;
                }
            }
        };

        // strip meta info if present
        auto [strippedExpr, metaInfo] = m_mi.extractMetaInfo(m_expr.c_str());

        auto strippedExprType = std::string{};
        try {
            strippedExprType = getGDBType(strippedExpr);
        } catch (std::string err) {
            if (!dbg_error) {
                if (err.find("No symbol") != std::string::npos) {
                    DVPRINT("Whatis %s failed; no symbol.\n", strippedExpr.c_str());
                    dbg_error.reset(NewErrorEvent(DBGERR_NOSYM, "%s", strippedExpr.c_str()));
                } else {
                    DVPRINT("Whatis %s failed: %s.\n", strippedExpr.c_str(), err.c_str());
                    dbg_error.reset(NewErrorEvent(DBGERR_DEBUGGER, "%s: %s",
                                                  strippedExpr.c_str(), err.c_str()));
                }
            }
            throw dbg_error.release();
        }

        if (std::regex_match(strippedExprType, anonymousRE) && getVar()) {
            // unnamed C++ class type, I'll have to work from the expression
            return getType();
        }

        m_type = TypeParser::getType(m_mi, nullptr, strippedExprType.c_str());
        DVPRINT("Got type for %s: %s\n", m_expr.c_str(), m_type->getTypeName().c_str());

        // if we have a pointer with length meta info, we can apply it now
        if (auto ptrType = std::dynamic_pointer_cast<ais::Pointer>(m_type);
            ptrType && !metaInfo.empty() && metaInfo[0] == '@')
        {
            DVPRINT("Found metainfo to apply: %s\n", metaInfo.c_str());
            auto lengthened = std::string{"("} + strippedExpr + ")[0]" + metaInfo;
            try {
                auto appliedType = getGDBType(lengthened);
                if (!appliedType.empty()) {
                    m_type = TypeParser::getType(m_mi, nullptr, appliedType.c_str());
                }
            } catch (...) {}
        }
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

// Build up the mapping from the ptype result.  Oobs is the
// full line by line output captured by ptype
TypeMapper::TypeMapper(MIList* oobs) {
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
            typeMap.push_back(std::pair{std::regex{tmatch}, tval});

            s = e;
            if (*s == ',') while (*++s && std::isspace(*s)) {}
        }
    }

}
std::string TypeMapper::operator()(std::string&& field) {
    // expand any template parameters in field
    for ( auto& tm: typeMap ) {
        field = std::regex_replace(field, tm.first, tm.second);
    }
    return field;
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

        DVPRINT("nextBaseClass returning %s\n", earliest);
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

std::tuple<std::string, std::string> Mi2Aif::extractMetaInfo(const char* expr) {
    // Taking a cue from C++ annotations to use [[...]] to pass meta-data
    // to the back-end.   So far, this is just being used to pass length
    // information for raw C/C++ arrays.   I'd expect to add slicing info
    // via this channel in the future.
    if (expr && expr[0] == '[' && expr[1] == '[') {
        if (auto close = strstr(expr,"]]")) {
            auto metaInfo = std::string{expr+2, size_t(close-expr-2)};
            DVPRINT("got metaInfo \"%s\"\n", metaInfo.c_str());
            return {std::string(close + 2), metaInfo};
        }
    }

    // XXX copying the unchanged input for no reason? a little wasteful...
    return {std::string{expr}, ""};
}

// Create an expression to evaluate based on additional meta-data for context
std::shared_ptr<MIVar>
Mi2Aif::createMIVarWithMetaInfo(const char* expr, const std::string& metaInfo)
{
    DVPRINT("createMiVarWithMetaInfo \"%s\" + \"%s\"\n", expr, metaInfo.c_str());
    std::shared_ptr<MIVar> rawVal = createMIVar(expr);

    if (rawVal && rawVal->type && rawVal->type[strlen(rawVal->type)-1] == '*'
        && !metaInfo.empty() && metaInfo[0] == '@') {
        // we have C pointerwith additional length info
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
std::string Mi2Aif::getPTypeAlias(const std::string& type, bool strict) {
    // gdb can't properly parse the closure of nested templates (">>")
    // without a space in between ("> >")
    auto fixedType = FixRightShift(type);

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
    if (char* info = CLIGetPtypeInfo(cmd)) {
        // info ends with a \n
        auto len = strlen(info);
        auto bracket = strchr(info,'{');
        if (len > 1 && !bracket) {
            retType = std::string{info, len-1};
            DVPRINT("Get CLIPTypeInfo %s\n", retType.c_str());
        } else if (strncmp(info, "enum ", 5) == 0 && bracket) {
            retType = std::string{info, bracket};
            DVPRINT("Got enum alias %s\n", retType.c_str());
        } else if (strncmp(info, "union ", 6) == 0 && bracket) {
            retType = std::string{info, bracket};
             DVPRINT("Got union alias %s\n", retType.c_str());
        }
        free(info);
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

static char* EvalExpression(MISession* session, const char* expression)
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

static bool typeIsReference(const char* str) {
    if (!str) return false;
    for (auto s = str+strlen(str); s != str; ) {
        if (auto c = *--s; c == '&') return true;
        else if (!std::isspace(c)) break;
    }
    return false;
}

static char *GetArrayDataString(MISession* session, MIVar *var) {

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
        mi2.getChildren(var);

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

namespace {
// Class to manage breaking the MI query for an array into smaller pieces.
// The calling sequence is simple:
// ArraySlice slicer(session, val, shape);
// while (char* buf = slicer.getNext()) {
//    ... process buf ...
// }
// The class is not omniscient, we currently only slice on the slowest changing
// dimension, so the actual block size could be larger than the nominal limit.
// Also, since we are plugging into the front end of gdb/mi (paging is disabled in this
// mode), we are limited to the set of types that can be split using a gdb expression.
class ArraySlicer {
public:
    ArraySlicer(Mi2Aif& mi, std::shared_ptr<MIVar> var, std::shared_ptr<ais::Type> varType);
    ArraySlicer(Mi2Aif& mi, std::string fullExpr);
    ~ArraySlicer();

    // evaluate the next slice of the variable result.
    std::tuple<std::shared_ptr<ais::Type>, char*, bool> getNext();
    std::shared_ptr<LazyMIVar> getVar() { return m_var; }

    const static size_t DEFAULT_BLOCK_SIZE = 1000000; // one million

private:
    void determineSlicing();
    size_t determineBlockSize();

    ArraySlicer(const ArraySlicer&) = delete;
    ArraySlicer& operator=(const ArraySlicer&) = delete;

    Mi2Aif& m_mi;               // Parent Mi2Aif

    std::shared_ptr<LazyMIVar> m_var; // MI variable of interest

    char* m_buf;                // allocated array for the result (contolled by this)

    size_t m_blockSize;         // maximum number of elements in each query
    std::string m_subExpr;      // encode expression to use for querying a chunk
    long m_subIdx;              // index for the current chunk
    long m_maxSubIdx;           // index for the highest chunk to return
    long m_slice;               // stride for the chunking index.
};

ArraySlicer::ArraySlicer(Mi2Aif& mi, std::shared_ptr<MIVar> var,
                         std::shared_ptr<ais::Type> varType)
    : m_mi{mi}, m_var(std::make_shared<LazyMIVar>(mi, var))
    , m_buf(nullptr), m_blockSize(determineBlockSize())
    , m_subIdx(0), m_maxSubIdx(0), m_slice(0)
{
    if (m_blockSize) {
        determineSlicing();
    }
}

ArraySlicer::ArraySlicer(Mi2Aif& mi, std::string fullExpr)
    : m_mi{mi}, m_var(std::make_shared<LazyMIVar>(mi, std::move(fullExpr)))
    , m_buf(nullptr), m_blockSize(determineBlockSize())
    , m_subIdx(0), m_maxSubIdx(0), m_slice(0)
{
    if (m_blockSize) {
        determineSlicing();
    }
}

ArraySlicer::~ArraySlicer()
{
    free(m_buf);
}

// Paging and the like is unavailable in MI mode.   Instead, this class and this function
// specifically examines the data type and determines how it can be extracted in multiple
// data print calls that each pull out a portion of the data structure
void ArraySlicer::determineSlicing()
{
    DVPRINT("in ArraySlicer::determineSlicing blocksize if %zu\n", m_blockSize);
    
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
            DVPRINT("Slicing vector got a size %d\n", len);
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

    DVPRINT("In ArraySlicer:: numElems is %zu\n", numElems);
    
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
    m_slice = std::max(1, int(m_blockSize / subArrayLen));
    
    DVPRINT("Slicing: C array %s %ld to %ld by %ld\n",
            m_subExpr.c_str(), m_subIdx, m_maxSubIdx, m_slice);
}

size_t ArraySlicer::determineBlockSize() {
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
// The returned array is controlled by ArraySlicer and should not be freed by the
// caller.
std::tuple<std::shared_ptr<ais::Type>, char*, bool> ArraySlicer::getNext()
{
    DVPRINT("In ArraySlicer::getNext\n");

    bool isFinal = true;
    char* array = nullptr;

    if (m_subIdx > m_maxSubIdx) {
        return std::tuple(m_var->getType(), array, isFinal);
    }

    auto retType = m_var->getType();

    if (m_slice == 0 && !m_buf) {
        DVPRINT("In ArraySlicer::getNext not big enough to split\n");
        // splitting is not enabled, the array is not big enough to split,
        // or it's a form the splitter doesn't recognize, just return it
        // all
        array = m_buf = GetArrayDataString(m_mi.getSession(), m_var->getVar().get());
        ++m_subIdx;
    } else {
        DVPRINT("In ArraySlicer::getNext get next chunk\n");
        // get the next chunk of data
        std::swap(m_buf, array);
        if (array) {
            free(array);
            array = nullptr;
        }

        long maxIdx = std::min(m_maxSubIdx, m_subIdx+m_slice-1);
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

            // update the array type to contain just the length of the slice
            auto subArrayType = std::static_pointer_cast<ais::Array>(m_var->getType());
            subArrayType = subArrayType->makeTyped(subArrayType->getElemTypePtr());
            subArrayType->setBounds(len, m_subIdx);
            retType = subArrayType;
            
            m_subIdx = maxIdx+1;

        } else {
            // failure!  - fall back to just evaluating the whole kielbasa
            array = GetArrayDataString(m_mi.getSession(), m_var->getVar().get());
            m_subIdx = m_maxSubIdx + 1;

            DVPRINT("Failed getting sub-array value, full result: %s\n", array);
        }
        m_buf = array;
    }

    isFinal = (m_subIdx > m_maxSubIdx);
    return std::tuple(retType, array, isFinal);
}

} // namespace


void Mi2Aif::sendMICommand(MICommand* command) {
    if (m_session) SendMICommand(m_session, command);
}

namespace {

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

    void WriteBuffer::writePointer(const std::string& type, void* val) {
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

}
    

Mi2Aif::Mi2Aif(MISession* session) : m_session(session), m_depth(0),
                                     m_writeBuffer(std::make_shared<MIWriteBuffer>(*this)),
                                     m_writer(m_writeBuffer) {}

std::string TypeParser::sanitizeType(const std::string& type) {
    // remove "const", "signed", and "volatile" from C++
    // "shared", "SHARED", "pointer," and "POINTER," from fortran
    // "@[a-zA-z]* from nvidia registers
    auto sanType = type;
    bool inW = false;
    for (int i=0, word=0; i<int(sanType.size()); ++i) {
        auto c = sanType[i];
        auto cIsW = (::isalnum(c) || c == '_' || c == '@' || c == ':' || c == ',');
        if (inW == cIsW) continue;
        if (cIsW && !inW) {
            word = i;
            inW = true;
            continue;
        }
        // inW && !cIsW
        inW = false;
        auto token = sanType.substr(word,i-word);
        if (token[0] == '@'
            || std::find(m_ignore.begin(), m_ignore.end(), token) != m_ignore.end()
            || (token == "signed" && sanType.substr(i+1,4) != "char")) {
            if (c == ' ' && (!word || sanType[word-1] == ' ')) ++i;
            sanType.erase(word, i-word);
            i = word-1;
        } 
    }

    // gdb prints the type as /* real type = DerivedType * */ when it
    // gets the type from the vtable.
    static std::regex virtualTypeRE(R"(/\*\s*real type\s*=\s*(.*\S)\s*\*/)");
    auto match = std::smatch{};
    if (std::regex_match(sanType, match, virtualTypeRE)) {
        sanType = match[1];
        DVPRINT("got devied class type: \"%s\"\n", sanType.c_str());
    }     

    // remove leading and trailing whitespace
    if (auto p = sanType.find_first_not_of(' ')) sanType.erase(0,p);
    if (auto p = sanType.find_last_not_of(" &"); p != std::string::npos) {
        sanType.erase(p+1);
    }
    return sanType;
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
static size_t FindTemplateLength(const std::string& str, size_t pos) {
    auto beg = str.c_str()+pos;
    auto end = FindTemplateEnd(beg); // should point to >
    return (end-beg)+1;
}

static TypeParser::Lang parseShowLangResult(const char* result) {
    // Examples:
    //   The current source language is "auto; currently c++".
    //   The current source language is "fortran".

    if (!result) return TypeParser::Lang::Unknown;

    auto sv = std::string_view(result);
    auto npos = std::string_view::npos;

    if (sv.find("fortran") != npos) return TypeParser::Lang::Ftn;
    if (sv.find("c++") != npos) return TypeParser::Lang::Cpp;
    if (sv.find("\"c\"") != npos || sv.find("currently c\"") != npos)
        return TypeParser::Lang::Cpp;

    return TypeParser::Lang::Unknown;
}

TypeParser::Lang TypeParser::queryCurrentLang(MISession* mi) {
    if (!mi) return TypeParser::Lang::Unknown;

    const char* argv[] = {"show", "lang"};
    MICommand* cmd = CLIBypass(2, const_cast<char **>(argv));
    SendMICommand(mi, cmd);

    auto result = TypeParser::Lang::Unknown;

    if (MICommandResultOK(cmd)) {
        auto result_str = MICommandConsoleOOBMessages(cmd);
        if (result_str) {
            result = parseShowLangResult(result_str);
        }
        free(result_str);
    }

    MICommandFree(cmd);

    return result;
}

// break a data member string into its name and
// type describe the current class/struct, fieldStr is the description of a member
std::pair<std::string, std::string>
PTypeFields::parseField(const std::string& type, const std::string& fieldStr) {
    using RetType = std::pair<std::string, std::string>;

    // the field is type followed by name
    auto namePos = fieldStr.rfind(' ');
    if (namePos == std::string::npos) return RetType{};
    
    while (++namePos < fieldStr.size() && fieldStr[namePos] == '*') {}
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

std::shared_ptr<ais::Type> TypeParser::getType(Mi2Aif& mi, MIVar* var, const char* inType) {
    switch (TypeParser::queryCurrentLang(mi.getSession())) {
        case TypeParser::Lang::Unknown:
            DVPRINT("WARNING: failed to detect current language type. Trying cpp.\n");
            // fall through
        case TypeParser::Lang::Cpp:
            return getCppType(mi, var, inType);

        case TypeParser::Lang::Ftn:
            return getFtnType(mi, inType ? inType : var->type);

        default:
            // getting here means that queryCurrentLang somehow returned a value outside
            // of the valid values for TypeParser::Lang
            return std::make_shared<ais::Unimplemented>("language-detection-error");
    }
}

std::shared_ptr<ais::Type> TypeParser::getCppType(Mi2Aif& mi, MIVar* var, const char* inType) {
    static bool isSorted = false;
    if (!isSorted) {
        std::sort(m_cppPrims.begin(), m_cppPrims.end(), comparePrims);
        std::sort(m_cppStlPrims.begin(), m_cppStlPrims.end(), comparePrims);
        isSorted = true;
    }

    auto isId = [](int c) { return std::isalnum(c) || c == '_'; };

    DVPRINT("TypeParser::getCppType var=\"%s\" inType=\"%s\"\n", 
            var ? var->type : "", inType ? inType : "");
    if (var) {
        DVPRINT("expr %s exp %s\n", var->expr ? var->expr : "",
                var->exp ? var->exp : "");
    }

    const char* typeStr = inType ? inType : var->type;
    auto type = sanitizeType(typeStr);

    DVPRINT("looking up type for %s (%s)\n", typeStr, type.c_str());

    if (auto knownType = mi.getKnownType(type)) {
        DVPRINT("found known type for %s\n", type.c_str());
        return knownType;
    }

    // pointer types
    if (type.back() == '*') {
        auto derefType = type.substr(0,type.find_last_not_of(' ', type.size()-2)+1);
        if (derefType == "char") {
            DVPRINT("Found a char pointer type %s\n", type.c_str());
            return mi.addKnownType(type, std::make_shared<ais::CharArray>());
        } else if (var && var->expr && derefType == "class {...}") {
            DVPRINT("Found a pointer to an anonymous class type %s\n", type.c_str());
            derefType = std::string{"decltype(*"} + var->expr + ")";
            return mi.addKnownType(derefType,
                                   std::make_shared<ais::Pointer>(derefType));
        } else {
            DVPRINT("Found a pointer type %s\n", type.c_str());
            return mi.addKnownType(type, std::make_shared<ais::Pointer>(derefType));
        }
    }

    // check for C++ primitive types
    // we need to use upper_bound and work backwards to be sure be get the maximal
    // match, e.g. "long" v. "long double"
    auto cpp = Prim{type,0};
    auto loc = std::upper_bound(m_cppPrims.begin(), m_cppPrims.end(), cpp, comparePrims);
    while (loc != m_cppPrims.begin() && (--loc)->first[0] == type[0]) {
        const auto& match = loc->first;
        if (match == type.substr(0, match.size()) &&
            (type.size() == match.size() || !isId(type[match.size()]))) {
            DVPRINT("returning c++ primitive type %s %d\n", type.c_str(), static_cast<int>(match.size()));
            return mi.addKnownType(type, loc->second(mi, TypeName{type,match.size()}));
        }
    }

    // check for C++ stl types
    loc = std::upper_bound(m_cppStlPrims.begin(), m_cppStlPrims.end(), cpp, comparePrims);
    while (loc != m_cppStlPrims.begin() && (--loc)->first[0] == type[0]) {
        const auto& match = loc->first;
        if (match == type.substr(0, match.size()) &&
            (type.size() == match.size() || !isId(type[match.size()]))) {
            // make sure our real type isn't a scoped member of the match,
            // e.g. a std::array<int, 10>::value_type
            auto scoped = false;
            for (auto i = match.size(); type.size() > 0 && i < type.size() - 1; i++) {
                if (type[i] == '<') {
                    i += FindTemplateLength(type,i)-1;
                } else if (type[i] == ':' && type[i+1] == ':') {
                    DVPRINT("matched with %s but found further scoping. continuing.\n", match.c_str());
                    scoped = true;
                    break;
                }
            }

            if (scoped) continue;

            DVPRINT("returning c++ stl type %s %d\n", type.c_str(), static_cast<int>(match.size()));
            return mi.addKnownType(type, loc->second(mi, TypeName{type,match.size()}));
        }
    }

    // check for a std::string or one of its variants
    static std::regex stdStringRE(R"((std::[A-Za-z0-9_:]*string)<.*>\s*)");
    static std::regex stdStringRE2(R"((std::[A-Za-z0-9_:]*string)\s*)");
    std::smatch match;
    if (std::regex_match(type, match, stdStringRE) ||
        std::regex_match(type, match, stdStringRE2)) {
        DVPRINT("matched on a string type %s\n", match[1].str().c_str());
        return mi.addKnownType(type, stdString(mi, TypeName{type,match[1].str().size()}));
    }

    // possible typedef for a more fundamental type
    auto aliasName = mi.getPTypeAlias(type, false);
    if (aliasName != type) {
        DVPRINT("got an alias \"%s\"\n", aliasName.c_str());
        return getCppType(mi, var, aliasName.c_str());
    }

    // compound types - (always struct or class? - not sure)
    // extract the base type - * or [ begins the decoration.
    auto ty = type.begin();
    for ( ; ty != type.end(); ++ty) {
        if (*ty == '<') ty += FindTemplateLength(type, ty-type.begin())-1;
        else if (*ty == '*' || *ty == '[') break;
    }
    for ( ; ty != type.begin() && isspace(*(ty-1)); --ty) {}

    auto typeLen = ty - type.begin();
    auto baseType = type.substr(0,typeLen);
    auto decoration = type.substr(typeLen);
    auto stype = std::make_shared<ais::Struct>(baseType);

    // register the type here so a class/struct with a pointer to its own type
    // (like a linked list or graph) doesn't get into infinite recursion
    DVPRINT("get baseType %s decoration %s\n", baseType.c_str(), decoration.c_str());
    mi.addKnownType(baseType, stype);

    addStructMembers(mi, *stype, var, var ? baseType : type);

    return cppWrap(stype, TypeName{type,static_cast<size_t>(typeLen)});
}

void
TypeParser::addStructMembers(Mi2Aif& mi, ais::Struct& stype,
                             MIVar* var, const std::string& type) {
    if (var) {
        auto getMemberName = [] (MIVar* var) {
           DVPRINT("var expr:'%s' name:'%s' exp:'%s\n",
                   var->expr ? var->expr : "NULL",
                   var->name ? var->name : "NULL",
                   var->exp ? var->exp : "NULL");
           auto memberName = std::string{var->expr ? var->expr : var->exp};
           if (auto dot = memberName.rfind('.'); dot != std::string::npos) {
               memberName.erase(0, dot+1);
           }
           return memberName;
        };

        while (true) {
            DVPRINT("Gettting children of type %s\n", var->type);
            mi.getChildren(var);
            if (var->type && var->type != type && var->numchild) {
                var = var->children[0];
            } else {
                break;
            }
        }
        DVPRINT("Got %d children\n", var->numchild);

        // The members are in the children of MIVar
        for (int i=0; i<var->numchild; ++i) {
            if (auto child1 = var->children[i]; child1->type) {
                // normal child
                DVPRINT("adding a child\n");
                DVPRINT("member name is %s\n", getMemberName(child1).c_str());
                stype.addMember(getMemberName(child1), getCppType(mi,child1,nullptr));
            } else if (!mi.getChildren(child1)) {
                // child is public|protected|private - walk down another level
                for (int j=0; j<child1->numchild; ++j) {
                    auto child2 = child1->children[j];
                    child2->expr = strdup(child2->name);
                    stype.addMember(getMemberName(child2), getCppType(mi,child2,nullptr));
                }
            } else {
                DVPRINT("getting children failed\n");
            }
        }
        DVPRINT("done processing members\n");
    } else {
        auto ptfields = mi.getPTypeFields(type, /* true if fortran */ false);
        // parent classes
        for (const auto& parent : ptfields.parentClasses()) {
            stype.addMember(parent, getCppType(mi, nullptr, parent.c_str()));
        }
        // class members
        for (auto&& fieldStr : ptfields.fields()) {
            auto [fieldName, fieldTypeStr] = ptfields.parseField(type, fieldStr);
            if (fieldName.empty()) continue;

            stype.addMember(fieldName, getCppType(mi,nullptr,fieldTypeStr.c_str()));
        }
    }
}

std::shared_ptr<ais::Type> TypeParser::getFtnType(Mi2Aif& mi, const char* typeStr) {
    static bool isSorted = false;
    if (!isSorted) {
        std::sort(m_fortranPrims.begin(), m_fortranPrims.end(), comparePrims);
        isSorted = true;
    }

    // fortran is case insensitive, so we uppercase the type to match strings in m_fortranPrims.
    auto type = sanitizeType(typeStr);
    std::transform(type.begin(), type.end(), type.begin(), ::toupper);

    if (auto knownType = mi.getKnownType(type)) {
        DVPRINT("found known type for %s\n", type.c_str());
        return knownType;
    }

    // we need to use upper_bound and work backwards to be sure be get the maximal
    // match, e.g. "INTEGER" v. "INTEGER*16"
    auto ftn = Prim{type,0};
    auto loc = std::upper_bound(m_fortranPrims.begin(), m_fortranPrims.end(), ftn, comparePrims);

    while (loc != m_fortranPrims.begin() && (--loc)->first[0] == type[0]) {
        const auto& match = loc->first;
        if (match == type.substr(0, match.size())) {
            DVPRINT("got fortran type %s\n", match.c_str());
            return mi.addKnownType(type, loc->second(mi, TypeName{type,match.size()}));
        }
    }

    // compound types - these are reported as "Type typename"
    if (std::string_view(type).substr(0,5) == "TYPE ") {
        type = type.substr(5);
        DVPRINT("fortran compound type! using %s\n", type.c_str());
    }

    // split into typename and decoration
    auto ty = type.begin();
    for ( ; ty != type.end() && *ty != '*' && *ty != '(' && *ty != ','; ++ty) {}
    for ( ; ty != type.begin() && isspace(*(ty-1)); --ty) {}

    auto typeLen = ty - type.begin();
    auto baseType = type.substr(0,typeLen);
    auto decoration = type.substr(typeLen);
    auto stype = std::make_shared<ais::Struct>(baseType);

    DVPRINT("get baseType %s decoration %s\n", baseType.c_str(), decoration.c_str());

    // register the type here so a class/struct with a pointer to its own type
    // (like a linked list or graph) doesn't get into infinite recursion
    mi.addKnownType(baseType, stype);

    auto ptfields = mi.getPTypeFields(baseType, /* true if fortran */ true);
    // parent classes
    for (const auto& parent : ptfields.parentClasses()) {
        stype->addMember(parent, getFtnType(mi, parent.c_str()));
    }
    // class members
    for (auto&& fieldStr : ptfields.fields()) {
        auto [fieldName, fieldTypeStr] = ptfields.parseField(type, fieldStr);
        if (fieldName.empty()) continue;

        stype->addMember(fieldName, getFtnType(mi,fieldTypeStr.c_str()));
    }

    DVPRINT("calling addKnownType %s decoration %s\n", baseType.c_str(),
            decoration.c_str());

    mi.addKnownType(baseType, stype);
    return ftnWrap(stype, TypeName{type,static_cast<size_t>(typeLen)});
}

// Handle decorations of the base type.  Like *,&,[], etc.
std::shared_ptr<ais::Type> TypeParser::cppWrap(std::shared_ptr<ais::Type> elemType, TypeName t) {
    auto beg = t.decoration.find_first_not_of(' ');
    if (beg == std::string::npos) {
        return std::move(elemType);
    }

    DVPRINT("TypeParser::wrap decoration is \"%s\"\n", t.decoration.c_str());

    auto c = t.decoration[beg];
    auto w = std::shared_ptr<ais::Type>{};
    auto end = beg;
    const std::string& r{t.decoration};

    DVPRINT("c == %c\n", c);
    
    if (c == '*') {
        w.reset(new ais::Pointer(t.type));
    } else if (c == '&') {
        DVPRINT("ignoring reference");
        w = elemType;
    } else if (c == '[') {
        // for multi-dimension arrays, the last value is changing
        // the fastest
        for (auto loc = end;
             (loc=r.find(']', loc+1)) != std::string::npos
                 && (end=loc)+1 != r.size()
                 && r[end+1] == '['; ) {}

        if (r[end] != ']') return std::make_shared<ais::Unimplemented>("unknown-c-decoration-" + r);

        w = elemType;

        for (auto loc = end+1; loc != beg; ) {
            auto close = loc-1;
            loc = r.rfind('[', loc-1);
            int len = std::stoi(r.substr(loc+1,close-loc-1));

            w.reset(new ais::Array(w, len));
        }
    } else if (c == '(') {
        for (int level=1; level && ++end != r.size(); ) {
            switch (r[end]) {
            case '(': ++level; break;
            case ')': --level; break;
            default: break;
            }
        }
        w.reset(new ais::Function);
    } else if (c == '@') {
        // gpu register tag - like @local
        while ( ::isalpha(r[end+1])) ++end;
        w = elemType;
    } else if (r.find("const", beg) == beg) {
        w = elemType;
        end = beg + 4;
    } else {
        w.reset(new ais::Unimplemented{"unknown-decoration-" + r.substr(beg)});
        end = r.size()-1;
    }

    return cppWrap(w, TypeName(t,end+1));
}


std::shared_ptr<ais::Type> TypeParser::ftnWrap(std::shared_ptr<ais::Type> elemType,
                                               TypeName t) {
    if (t.decoration.empty()) return elemType;

    auto result = elemType;
    bool inSize = false;

    DVPRINT("In ftnWrap decoration=%s\n", t.decoration.c_str());
    char c;
    auto is = std::istringstream{t.decoration};
    while (is >> c) {
        if (c == '(' || (inSize && (c == ',' || c == ':'))) {
            inSize = true;
            int len = 0, idx0 = 1;
            if (is.peek() == ':') {
                is.ignore();
                DVPRINT("Unallocated array\n");
            } else if (is >> len) {
                if (is.peek() == ':') {
                    is.ignore();
                    idx0 = len;
                    is >> len;
                    len = len-idx0+1;
                }
                DVPRINT("got len %d idx0 %d\n", len, idx0);
            }
                    
            auto arr = std::make_shared<ais::FtnArray>(result);
            arr->setBounds(len, idx0);
            result = arr;

        } else if (inSize && c == ')') {
            inSize = false;
        } else if (c == '*') {
            int len;
            if (is >> len) {
                DVPRINT("got len %d\n", len);
                result = std::make_shared<ais::FtnArray>(result, len);
            }
        } else if (!isspace(c) && !strchr(",ALOCTBEaloctbe", c)) {
            // notice we are ignoring ", ALLOCATABLE " and ", allocatable"
            DVPRINT("got unexpected character '%c'\n", c);
            return std::make_shared<ais::Unimplemented>("unknown-fortran-decoration-" + t.decoration);
        }
    }

    return result;
}

// parse the template argument part of a template class declaration into
// <A, B> C.    A is the contained type, B is the rest of the template arguments
// (it may be empty or contain ,'s) and the rest of the decoration.
std::tuple<std::string, std::string, std::string>
TypeParser::getContainerTypes(TypeName t) {
    // find the contained type
    bool parseError = false;
    std::size_t firstLt = t.decoration.find('<'), lastLt = std::string::npos, comma = std::string::npos;
    if (firstLt == std::string::npos) parseError = true;
    
    int level = 1;
    for (auto i = firstLt+1; level && i < t.decoration.size(); ++i) {
        switch (t.decoration[i]) {
        case '<': ++level; break;
        case '>': if (!--level) lastLt = i; break;
        case ',': if (level == 1 && comma == std::string::npos) comma = i; break;
        }
    }
    if (lastLt == std::string::npos) parseError = true;
    if (parseError) {
        DVPRINT("could not parse container type\n");
        return std::tuple<std::string,std::string,std::string>("","","");
    }

    if (comma == std::string::npos) comma = lastLt;
    auto containedType = t.decoration.substr(firstLt+1, comma-firstLt-1);
    auto templateArgs = (comma == lastLt ? "" : t.decoration.substr(comma+1, lastLt-comma-1));
    auto decoration = t.decoration.substr(lastLt+1);

    return std::make_tuple(containedType, templateArgs, decoration);
}

std::shared_ptr<ais::Type> TypeParser::stlContainer(Mi2Aif& mi, const ais::Array& ctype, TypeName t) {
    DVPRINT("parsing STL container type %s\n", t.decoration.c_str());

    auto [containedType, templateArgs, decoration] = getContainerTypes(t);
    if (containedType.empty()) {
        return unimplemented(mi,t);
    }

    DVPRINT("Got types: \"%s\", \"%s\", \"%s\"\n",
            containedType.c_str(), templateArgs.c_str(), decoration.c_str());

    auto t2 = TypeName{t.type  + "<" + containedType + "," + templateArgs + ">",
                       std::move(decoration)};

    auto elemType = getType(mi, nullptr, containedType.c_str());
    auto vtype = ctype.makeTyped(elemType);
    return cppWrap(vtype, t2);
}

std::shared_ptr<ais::Type> TypeParser::stdArray(Mi2Aif& mi, TypeName t) {
    DVPRINT("parsing stdArray container type %s\n", t.decoration.c_str());

    auto [containedType, templateArgs, decoration] = getContainerTypes(t);
    if (containedType.empty()) {
        return unimplemented(mi,t);
    }

    DVPRINT("Got types: \"%s\", \"%s\", \"%s\"\n",
            containedType.c_str(), templateArgs.c_str(), decoration.c_str());

    int len = 0;
    try {
        len = std::stoi(templateArgs);
    } catch (std::exception&) {
        DVPRINT("could not get length from \"%s\"\n", templateArgs.c_str());
        return unimplemented(mi,t);
    }

    auto t2 = TypeName{t.type  + "<" + containedType + "," + templateArgs + ">",
                       std::move(decoration)};

    auto elemType = getType(mi, nullptr, containedType.c_str());
    auto vtype = std::make_shared<ais::StdArray>(elemType,len);
    return cppWrap(vtype, t2);
}

std::shared_ptr<ais::Type> TypeParser::stdString(Mi2Aif& mi, TypeName t) {
    DVPRINT("parsing stdString container type %s\n", t.decoration.c_str());

    auto [containedType, templateArgs, decoration] = getContainerTypes(t);

    DVPRINT("Got types: \"%s\", \"%s\", \"%s\"\n",
            containedType.c_str(), templateArgs.c_str(), decoration.c_str());

    auto t2 = TypeName{t.type  + "<" + containedType +">", std::move(decoration)};
    return cppWrap(std::make_shared<ais::String>(), t2);
}

std::shared_ptr<ais::Type> TypeParser::stdConditional(Mi2Aif& mi, TypeName t) {
    DVPRINT("parsing std::conditional %s\n", t.decoration.c_str());

    const std::string& d = t.decoration;
    std::vector<std::string> args;

    auto i = d.find('<'), beg=i;
    if (i != std::string::npos) ++i;

    for (int level=1; level && i < d.size(); ++i) {
        switch (d[i]) {
        case '<': ++level; break;
        case '>': if (!--level) args.push_back(d.substr(beg+1, i-beg-1)); break;
        case ')': if (i+2 < d.size() && (d[i+1]=='<' || d[i+1]=='>') && d[i+2]=='(') ++i; break;
        case ',': if (level == 1) { args.push_back(d.substr(beg+1, i-beg-1)); beg = i; } break;
        default: break;
        }
    }

    if (args.size() != 3) {
        DVPRINT("Expected 3 args but got %d\n", int(args.size()));
        return unimplemented(mi,t);
    }

    DVPRINT("Got arguments: \"%s\", \"%s\", \"%s\"\n", args[0].c_str(), args[1].c_str(), args[2].c_str());

    auto t2 = TypeName{t.type  + d.substr(0,i), d.substr(i)};

    auto condResult = EvalExpression(mi.getSession(), args[0].c_str());

    auto theType = ((!condResult || condResult[0]=='t' || condResult[0]=='1') ? args[1] : args[2]);

    DVPRINT("condition result is %s using type %s\n", condResult ? condResult : "NULL", theType.c_str());

    free(condResult);
    
    auto vtype = getType(mi, nullptr, theType.c_str());

    return cppWrap(vtype, t2);
}

std::shared_ptr<ais::Type> TypeParser::ftnCharacter(Mi2Aif& mi, TypeName t) {
    DVPRINT("parsing fortran CHARACTER type %s\n", t.decoration.c_str());

    if (auto star = t.decoration.find('*'); star != std::string::npos) {
        try {
            int len = std::stoi(t.decoration.substr(star+1));
            if (len > 1) {
                return std::make_shared<ais::FtnString>(len);
            }
        } catch (std::exception&) {
            DVPRINT("could not get a length\n");
        }
    }
            
    return std::make_shared<ais::Char>();
}
    

std::shared_ptr<ais::Type> TypeParser::smartPointer(Mi2Aif& mi, TypeName t) {
    DVPRINT("parsing smartPointer type %s\n", t.decoration.c_str());

    auto [containedType, templateArgs, decoration] = getContainerTypes(t);

    DVPRINT("Got types: \"%s\", \"%s\", \"%s\"\n",
            containedType.c_str(), templateArgs.c_str(), decoration.c_str());

    auto t2 = TypeName{t.type  + "<" + containedType +">", std::move(decoration)};
    return cppWrap(std::make_shared<ais::SmartPointer>(t.type, containedType), t2);
}


std::shared_ptr<ais::Type> TypeParser::charWrap(std::shared_ptr<ais::Type> elemType,
                                                TypeName t) {

    DVPRINT("in charWrap \"%s\" + \"%s\"\n", t.type.c_str(), t.decoration.c_str());

    auto beg = t.decoration.find_first_not_of(' ');
    if (beg == std::string::npos) {
        return elemType;
    } else if (t.decoration[beg] == '*') {
        return cppWrap(std::make_shared<ais::CharArray>(), TypeName{t, beg+1});
    } else if (t.decoration[beg] == '[') {
        DVPRINT("processing '['\n");
        
        auto d = t.decoration;
        auto begLast = beg;
        auto end = beg;
        while ((end=d.find(']', begLast+1)) != std::string::npos
               && end+1 != d.size()
               && d[end+1] == '[') {
            begLast = end+1;
        }


        DVPRINT("begLast=%d end=%d\n", int(begLast), int(end));

        auto t2 = TypeName{t.type + "*", d.substr(0,begLast)};
        if (end != std::string::npos) t2.decoration += d.substr(end+1);

        DVPRINT("remainder is %s\n", t2.decoration.c_str());
        return cppWrap(std::make_shared<ais::CharArray>(), t2);

    } else {
        return std::make_shared<ais::Unimplemented>(t.type + t.decoration);
    }
}

std::shared_ptr<ais::Type> TypeParser::wrapVoidFunc(Mi2Aif& mi, TypeName t) {
    auto end = t.decoration.find_last_not_of(' ');
    if (end == std::string::npos) {
        return unimplemented(mi, t);
    }

    if (t.decoration[end] != ')') {
        return unimplemented(mi, t);
    }

    return std::make_shared<ais::Function>();
}

std::shared_ptr<ais::Type> TypeParser::wrapEnum(Mi2Aif& mi, TypeName t) {
    DVPRINT("enum decoration is \"%s\"\n", t.decoration.c_str());

    auto namePos = t.decoration.find_first_not_of(' ');
    if (namePos == std::string::npos) {
        return unimplemented(mi, t);
    }

    auto name = t.decoration.substr(namePos);
    return std::make_shared<ais::Enum>(name);
}

std::shared_ptr<ais::Type> TypeParser::wrapUnion(Mi2Aif& mi, TypeName t) {
    DVPRINT("union decoration is \"%s\"\n", t.decoration.c_str());

    const std::string& d = t.decoration;
    auto namePos = d.find_first_not_of(' ');
    if (namePos == std::string::npos) {
        return unimplemented(mi, t);
    }

    char c;
    auto nameEnd = namePos+1;
    while (nameEnd < d.size() &&
           (c=d[nameEnd], (std::isalnum(c) || c == '_' || c == ':'))) {
        ++nameEnd;
    }

    auto name = d.substr(namePos, nameEnd-namePos);
    auto baseType = t.type + d.substr(0,nameEnd);
    auto stype = std::make_shared<ais::Union>(name);
    auto decoration2 = d.substr(nameEnd);

    DVPRINT("adding union type %s decoration %s\n",
            baseType.c_str(), decoration2.c_str());

    mi.addKnownType(baseType, stype);

    addStructMembers(mi, *stype, nullptr, baseType);

    return cppWrap(stype, TypeName{t, nameEnd});
}


std::shared_ptr<ais::Type> TypeParser::unimplemented(Mi2Aif& mi, TypeName t) {
    return std::make_unique<ais::Unimplemented>(t.type + t.decoration);
}

// Create the top level ArraySlicer for an expression.   The ArraySlicer
// will potentially break the expression into multiple evaluations if
// var is a large container
std::unique_ptr<ArraySlicer> Mi2Aif::makeSlicer(const std::string& var) {
    return std::make_unique<ArraySlicer>(*this, var);
}

// Create an ArraySlicer for a secondary reference.
// Note: slicing of chain of values hasn't really been tested..
std::unique_ptr<ArraySlicer> Mi2Aif::makeSlicer(std::shared_ptr<MIVar> mivar) {
    auto origType = TypeParser::getType(*this, mivar.get(), nullptr);
    return std::make_unique<ArraySlicer>(*this, mivar, origType);
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

// Get the next slice of data from in socket protocol form.
// Node: currently, we are transmitting simple ascii text, if we switch to
// something more specialized and (hopefully) performant the return type
// would not be a string.
std::string Mi2Aif::nextSlice(ArraySlicer& slicer, bool& isFinal) {
    DVPRINT("nextSlice\n");

    if (m_refs.size() == 0) {
        initTopRef(slicer.getVar());
    }
    auto initNumRefs = m_refs.size();

    auto [ type, gdbArr, sliceIsFinal ] = slicer.getNext();
    isFinal = sliceIsFinal;

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
    auto slicer = makeSlicer(var);
    auto [ type, arr, isFinal ] = slicer->getNext();
    parse(type, arr, refIdx);
}

// AIFStream adapter to evaluate and parse a value on demand.
EvalSlices::EvalSlices(MISession* session, const std::string& var)
    : m_mi2aif(session), m_var(var) {}

std::string EvalSlices::getNextSlice(bool& isFinal) {
    if (!m_slicer) initSlicer();

    // Mi2Aif does the work and returns a chunk of socket protocol
    // data
    return m_mi2aif.nextSlice(*m_slicer, isFinal);
}

void EvalSlices::initSlicer() {
    if (!m_slicer) m_slicer = m_mi2aif.makeSlicer(m_var);
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

namespace unittest {

template <typename T>
std::shared_ptr<ais::Type> basicTypeTest(TypeParser::Lang lang, const char *input) {
    // parse the `input` string with the TypeParser function according to `lang`
    // and check that type `T` was returned.
    //
    // return the parsed type as a generic pointer (ais::Type).

    auto t = [lang, input]() -> std::shared_ptr<ais::Type> {
        auto mi = Mi2Aif(nullptr);
        switch (lang) {
            case TypeParser::Lang::Cpp:
                return TypeParser::getCppType(mi, nullptr, input);
            case TypeParser::Lang::Ftn:
                return TypeParser::getFtnType(mi, input);
            default:
                FAIL("Specify a valid language to test the type parser");
                return nullptr;
        }
    }();

    REQUIRE_MESSAGE(t != nullptr, "Failed to parse a type from \"", std::string{input}, "\"");
    CHECK_MESSAGE(std::dynamic_pointer_cast<T>(t) != nullptr, "Incorrect parse of \"",
                  t->getTypeName(), "\" from \"", std::string{input}, "\"");

    return t;
}

template <typename Outer, typename Inner>
std::shared_ptr<Inner> containerMatch(std::shared_ptr<ais::Type> outerptr, size_t len) {
    // verify that `outerptr` is of type `Outer` and that its inner type is `Inner`.
    // also verify that the outer type's length matches `len.
    //
    // return the casted inner type for easy chaining of multi-level containers.

    REQUIRE(outerptr != nullptr);

    auto outer = std::dynamic_pointer_cast<Outer>(outerptr);
    REQUIRE_MESSAGE(outer != nullptr,
                    "Incorrect outer container type: ", outerptr->getTypeName());

    // HACK: we throw ais::complex into this function too, but it doesn't have getLen()
    if constexpr (!std::is_same_v<Outer, ais::Complex>) {
        CHECK(outer->getLen() == len);
    }

    auto inner = std::dynamic_pointer_cast<Inner>(outer->getElemTypePtr());
    REQUIRE_MESSAGE(inner != nullptr,
                    "Incorrect inner container type: ", outer->getElemTypePtr()->getTypeName());

    return inner;
}

TEST_CASE("TypeParser::sanitizeType") {
    auto &st = TypeParser::sanitizeType;

    // nothing to sanitize
    CHECK(st("char") == std::string("char"));

    // whitespace
    CHECK(st("  char") == std::string("char"));
    CHECK(st("char  ") == std::string("char"));
    CHECK(st("  char  ") == std::string("char"));

    // cpp
    CHECK(st("const char") == std::string("char"));
    // "signed" has an exception for chars
    CHECK(st("signed char") == std::string("signed char"));
    CHECK(st("signed int") == std::string("int"));
    CHECK(st("volatile char") == std::string("char"));

    // ftn
    CHECK(st("SHARED char") == std::string("char"));
    CHECK(st("SHARED char") == std::string("char"));
    CHECK(st("POINTER, char") == std::string("char"));
    CHECK(st("POINTER, char") == std::string("char"));

    // nvidia
    CHECK(st("@abcd char") == std::string("char"));
}

TEST_CASE("TypeParser::getType: cpp primitive types") {
    SUBCASE("bool") { basicTypeTest<ais::Bool>(TypeParser::Lang::Cpp, "bool"); }

    SUBCASE("char") { basicTypeTest<ais::Char>(TypeParser::Lang::Cpp, "char"); }

    SUBCASE("double") { basicTypeTest<ais::Double>(TypeParser::Lang::Cpp, "double"); }

    SUBCASE("enum") { basicTypeTest<ais::Enum>(TypeParser::Lang::Cpp, "enum dbgcmd"); }

    SUBCASE("float") { basicTypeTest<ais::Float>(TypeParser::Lang::Cpp, "float"); }

    SUBCASE("int") { basicTypeTest<ais::Int>(TypeParser::Lang::Cpp, "int"); }

    SUBCASE("int8_t") { basicTypeTest<ais::Int>(TypeParser::Lang::Cpp, "int8_t"); }

    SUBCASE("long") { basicTypeTest<ais::Long>(TypeParser::Lang::Cpp, "long"); }

    SUBCASE("long int") { basicTypeTest<ais::Long>(TypeParser::Lang::Cpp, "long int"); }

    SUBCASE("long long") { basicTypeTest<ais::LongLong>(TypeParser::Lang::Cpp, "long long"); }

    SUBCASE("long double") { basicTypeTest<ais::LongDouble>(TypeParser::Lang::Cpp, "long double"); }

    SUBCASE("std::size_t") { basicTypeTest<ais::Long>(TypeParser::Lang::Cpp, "std::size_t"); }

    SUBCASE("size_t") { basicTypeTest<ais::UnsignedLong>(TypeParser::Lang::Cpp, "size_t"); }

    SUBCASE("short") { basicTypeTest<ais::Short>(TypeParser::Lang::Cpp, "short"); }

    SUBCASE("short int") { basicTypeTest<ais::Short>(TypeParser::Lang::Cpp, "short int"); }

    SUBCASE("signed char") { basicTypeTest<ais::SignedChar>(TypeParser::Lang::Cpp, "signed char"); }

    SUBCASE("uint8_t") { basicTypeTest<ais::UnsignedChar>(TypeParser::Lang::Cpp, "uint8_t"); }

    SUBCASE("uint16_t") { basicTypeTest<ais::UnsignedShort>(TypeParser::Lang::Cpp, "uint16_t"); }

    SUBCASE("uint32_t") { basicTypeTest<ais::UnsignedInt>(TypeParser::Lang::Cpp, "uint32_t"); }

    SUBCASE("uint64_t") { basicTypeTest<ais::UnsignedLong>(TypeParser::Lang::Cpp, "uint64_t"); }

    SUBCASE("unsigned char") { basicTypeTest<ais::UnsignedChar>(TypeParser::Lang::Cpp, "unsigned char"); }

    SUBCASE("unsigned int") { basicTypeTest<ais::UnsignedInt>(TypeParser::Lang::Cpp, "unsigned int"); }

    SUBCASE("unsigned long") { basicTypeTest<ais::UnsignedLong>(TypeParser::Lang::Cpp, "unsigned long"); }

    SUBCASE("unsigned long int") {
        basicTypeTest<ais::UnsignedLong>(TypeParser::Lang::Cpp, "unsigned long int");
    }

    SUBCASE("unsigned long long") {
        basicTypeTest<ais::UnsignedLongLong>(TypeParser::Lang::Cpp, "unsigned long long");
    }

    SUBCASE("unsigned short") { basicTypeTest<ais::UnsignedShort>(TypeParser::Lang::Cpp, "unsigned short"); }

    SUBCASE("unsigned short int") {
        basicTypeTest<ais::UnsignedShort>(TypeParser::Lang::Cpp, "unsigned short int");
    }

    SUBCASE("void function pointer") {
        basicTypeTest<ais::Function>(TypeParser::Lang::Cpp, "void(*)(int, int)");
    }
}

TEST_CASE("TypeParser::getType: cpp pointer types") {
    SUBCASE("primitive type pointers") {
        // TODO: verify the inner type of the pointer
        SUBCASE("int") { basicTypeTest<ais::Pointer>(TypeParser::Lang::Cpp, "int*"); }
        SUBCASE("void") { basicTypeTest<ais::Pointer>(TypeParser::Lang::Cpp, "void*"); }
        SUBCASE("float") { basicTypeTest<ais::Pointer>(TypeParser::Lang::Cpp, "float*"); }
    }

    SUBCASE("char pointer") { basicTypeTest<ais::CharArray>(TypeParser::Lang::Cpp, "char*"); }
    SUBCASE("anonymous class pointer") { basicTypeTest<ais::Pointer>(TypeParser::Lang::Cpp, "class {...}*"); }
}

TEST_CASE("TypeParser::getType: cpp decorated types") {
    SUBCASE("reference &") {
        basicTypeTest<ais::Int>(TypeParser::Lang::Cpp, "int&");
        basicTypeTest<ais::Float>(TypeParser::Lang::Cpp, "float&");
    }

    SUBCASE("arrays []") {
        SUBCASE("1d") {
            auto t = basicTypeTest<ais::Array>(TypeParser::Lang::Cpp, "int[10]");
            containerMatch<ais::Array, ais::Int>(t, 10);
        }
        SUBCASE("2d") {
            auto t = basicTypeTest<ais::Array>(TypeParser::Lang::Cpp, "int[10][20]");
            t = containerMatch<ais::Array, ais::Array>(t, 10);
            containerMatch<ais::Array, ais::Int>(t, 20);
        }
        SUBCASE("3d") {
            auto t = basicTypeTest<ais::Array>(TypeParser::Lang::Cpp, "int[10][20][30]");
            t = containerMatch<ais::Array, ais::Array>(t, 10);
            t = containerMatch<ais::Array, ais::Array>(t, 20);
            containerMatch<ais::Array, ais::Int>(t, 30);
        }
        SUBCASE("4d") {
            auto t = basicTypeTest<ais::Array>(TypeParser::Lang::Cpp, "int[10][20][30][40]");
            t = containerMatch<ais::Array, ais::Array>(t, 10);
            t = containerMatch<ais::Array, ais::Array>(t, 20);
            t = containerMatch<ais::Array, ais::Array>(t, 30);
            containerMatch<ais::Array, ais::Int>(t, 40);
        }
    }

    SUBCASE("array references []&") {
        SUBCASE("1d") {
            auto t = basicTypeTest<ais::Array>(TypeParser::Lang::Cpp, "int[10]&");
            containerMatch<ais::Array, ais::Int>(t, 10);
        }
        SUBCASE("2d") {
            auto t = basicTypeTest<ais::Array>(TypeParser::Lang::Cpp, "int[10][20]&");
            t = containerMatch<ais::Array, ais::Array>(t, 10);
            containerMatch<ais::Array, ais::Int>(t, 20);
        }
        SUBCASE("3d") {
            auto t = basicTypeTest<ais::Array>(TypeParser::Lang::Cpp, "int[10][20][30]&");
            t = containerMatch<ais::Array, ais::Array>(t, 10);
            t = containerMatch<ais::Array, ais::Array>(t, 20);
            containerMatch<ais::Array, ais::Int>(t, 30);
        }
        SUBCASE("4d") {
            auto t = basicTypeTest<ais::Array>(TypeParser::Lang::Cpp, "int[10][20][30][40]&");
            t = containerMatch<ais::Array, ais::Array>(t, 10);
            t = containerMatch<ais::Array, ais::Array>(t, 20);
            t = containerMatch<ais::Array, ais::Array>(t, 30);
            containerMatch<ais::Array, ais::Int>(t, 40);
        }
    }

    SUBCASE("function pointers ()") {
        basicTypeTest<ais::Function>(TypeParser::Lang::Cpp, "int (*) (int, int, int)");
        basicTypeTest<ais::Function>(TypeParser::Lang::Cpp, "void (*) (int, int)");
        basicTypeTest<ais::Function>(TypeParser::Lang::Cpp, "int (*) ()");
        basicTypeTest<ais::Function>(TypeParser::Lang::Cpp, "void (*) ()");
    }
}

TEST_CASE("TypeParser::getType: cpp stl types") {
    SUBCASE("std::array") {
        auto t = basicTypeTest<ais::StdArray>(TypeParser::Lang::Cpp, "std::array<int, 13>");
        containerMatch<ais::StdArray, ais::Int>(t, 13);
    }
    SUBCASE("std::deque") {
        auto t = basicTypeTest<ais::Array>(TypeParser::Lang::Cpp, "std::deque<float>");
        containerMatch<ais::Array, ais::Float>(t, 0);
    }
    SUBCASE("std::stack") {
        auto t = basicTypeTest<ais::Array>(TypeParser::Lang::Cpp, "std::stack<unsigned short>");
        containerMatch<ais::Array, ais::UnsignedShort>(t, 0);
    }
    SUBCASE("std::vector") {
        auto t = basicTypeTest<ais::Array>(TypeParser::Lang::Cpp, "std::vector<std::vector<int>>");
        t = containerMatch<ais::Array, ais::Array>(t, 0);
        containerMatch<ais::Array, ais::Int>(t, 0);
    }
    SUBCASE("std::priority_queue") {
        auto t = basicTypeTest<ais::Array>(TypeParser::Lang::Cpp, "std::priority_queue<double>");
        containerMatch<ais::Array, ais::Double>(t, 0);
    }
    SUBCASE("std::queue") {
        auto t = basicTypeTest<ais::Array>(TypeParser::Lang::Cpp, "std::queue<char>");
        containerMatch<ais::Array, ais::Char>(t, 0);
    }
    SUBCASE("std::shared_ptr") {
        basicTypeTest<ais::SmartPointer>(TypeParser::Lang::Cpp, "std::shared_ptr<int>");
    }
    SUBCASE("std::unique_ptr") {
        basicTypeTest<ais::SmartPointer>(TypeParser::Lang::Cpp, "std::unique_ptr<float>");
    }
}

TEST_CASE("TypeParser::getType: cpp string types") {
    basicTypeTest<ais::String>(TypeParser::Lang::Cpp, "std::string");
    basicTypeTest<ais::String>(TypeParser::Lang::Cpp, "std::basic_string");
    basicTypeTest<ais::String>(TypeParser::Lang::Cpp, "std::string<std::allocator>");
    basicTypeTest<ais::String>(TypeParser::Lang::Cpp, "std::basic_string<std::basic_allocator>");
}

TEST_CASE("TypeParser::getType: ftn \"double precision\"") {
    // some ftn compilers report doubles as "double precision" instead of "REAL*8"

    SUBCASE("double precision") { basicTypeTest<ais::Double>(TypeParser::Lang::Ftn, "double precision"); }

    SUBCASE("double precision (arrays)") {
        SUBCASE("1d") {
            auto t = basicTypeTest<ais::FtnArray>(TypeParser::Lang::Ftn, "double precision, allocatable (64)");
            t = containerMatch<ais::FtnArray, ais::Double>(t, 64);
        }
        SUBCASE("2d") {
            auto t =
                basicTypeTest<ais::FtnArray>(TypeParser::Lang::Ftn, "double precision, allocatable (64,63)");
            t = containerMatch<ais::FtnArray, ais::FtnArray>(t, 63);
            containerMatch<ais::FtnArray, ais::Double>(t, 64);
        }
        SUBCASE("3d") {
            auto t =
                basicTypeTest<ais::FtnArray>(TypeParser::Lang::Ftn, "double precision, allocatable (64,63,62)");
            t = containerMatch<ais::FtnArray, ais::FtnArray>(t, 62);
            t = containerMatch<ais::FtnArray, ais::FtnArray>(t, 63);
            containerMatch<ais::FtnArray, ais::Double>(t, 64);
        }
    }
}

TEST_CASE("TypeParser::getType: ftn primitive types") {
    SUBCASE("CHARACTER") {
        basicTypeTest<ais::Char>(TypeParser::Lang::Ftn, "character");
        basicTypeTest<ais::Char>(TypeParser::Lang::Ftn, "CHARACTER");
    }

    SUBCASE("COMPLEX") {
        {
            auto t = basicTypeTest<ais::Complex>(TypeParser::Lang::Ftn, "complex");
            containerMatch<ais::Complex, ais::Float>(t, 0);
        }
        {
            auto t = basicTypeTest<ais::Complex>(TypeParser::Lang::Ftn, "COMPLEX");
            containerMatch<ais::Complex, ais::Float>(t, 0);
        }
    }

    SUBCASE("COMPLEX(4)") {
        auto t = basicTypeTest<ais::Complex>(TypeParser::Lang::Ftn, "COMPLEX(4)");
        containerMatch<ais::Complex, ais::Float>(t, 0);
    }

    SUBCASE("COMPLEX(8)") {
        auto t = basicTypeTest<ais::Complex>(TypeParser::Lang::Ftn, "COMPLEX(8)");
        containerMatch<ais::Complex, ais::Double>(t, 0);
    }

    SUBCASE("double complex") {
        auto t = basicTypeTest<ais::Complex>(TypeParser::Lang::Ftn, "double complex");
        containerMatch<ais::Complex, ais::Double>(t, 0);
    }

    SUBCASE("COMPLEX(16)") {
        auto t = basicTypeTest<ais::Complex>(TypeParser::Lang::Ftn, "COMPLEX(16)");
        containerMatch<ais::Complex, ais::LongDouble>(t, 0);
    }

    SUBCASE("quad complex") {
        auto t = basicTypeTest<ais::Complex>(TypeParser::Lang::Ftn, "quad complex");
        containerMatch<ais::Complex, ais::LongDouble>(t, 0);
    }

    SUBCASE("COMPLEX(KIND=4)") {
        auto t = basicTypeTest<ais::Complex>(TypeParser::Lang::Ftn, "COMPLEX(KIND=4)");
        containerMatch<ais::Complex, ais::Float>(t, 0);
    }

    SUBCASE("COMPLEX(KIND=8)") {
        auto t = basicTypeTest<ais::Complex>(TypeParser::Lang::Ftn, "COMPLEX(KIND=8)");
        containerMatch<ais::Complex, ais::Double>(t, 0);
    }

    SUBCASE("COMPLEX(KIND=16)") {
        auto t = basicTypeTest<ais::Complex>(TypeParser::Lang::Ftn, "COMPLEX(KIND=16)");
        containerMatch<ais::Complex, ais::LongDouble>(t, 0);
    }

    SUBCASE("COMPLEX*8") {
        auto t = basicTypeTest<ais::Complex>(TypeParser::Lang::Ftn, "COMPLEX*8");
        containerMatch<ais::Complex, ais::Float>(t, 0);
    }

    SUBCASE("COMPLEX*16") {
        auto t = basicTypeTest<ais::Complex>(TypeParser::Lang::Ftn, "COMPLEX*16");
        containerMatch<ais::Complex, ais::Double>(t, 0);
    }

    SUBCASE("COMPLEX*32") {
        auto t = basicTypeTest<ais::Complex>(TypeParser::Lang::Ftn, "COMPLEX*32");
        containerMatch<ais::Complex, ais::LongDouble>(t, 0);
    }

    SUBCASE("INTEGER") {
        basicTypeTest<ais::Int>(TypeParser::Lang::Ftn, "INTEGER");
        basicTypeTest<ais::Int>(TypeParser::Lang::Ftn, "integer");
        basicTypeTest<ais::LongLong>(TypeParser::Lang::Ftn, "INT");
        basicTypeTest<ais::LongLong>(TypeParser::Lang::Ftn, "int");
    }

    SUBCASE("byte") { basicTypeTest<ais::SignedChar>(TypeParser::Lang::Ftn, "byte"); }

    SUBCASE("INTEGER(1)") { basicTypeTest<ais::SignedChar>(TypeParser::Lang::Ftn, "INTEGER(1)"); }

    SUBCASE("INTEGER(2)") { basicTypeTest<ais::Short>(TypeParser::Lang::Ftn, "INTEGER(2)"); }

    SUBCASE("INTEGER(4)") { basicTypeTest<ais::Int>(TypeParser::Lang::Ftn, "INTEGER(4)"); }

    SUBCASE("INTEGER(8)") { basicTypeTest<ais::Long>(TypeParser::Lang::Ftn, "INTEGER(8)"); }

    SUBCASE("INTEGER(16)") { basicTypeTest<ais::LongLong>(TypeParser::Lang::Ftn, "INTEGER(16)"); }

    SUBCASE("INTEGER(KIND=1)") { basicTypeTest<ais::SignedChar>(TypeParser::Lang::Ftn, "INTEGER(KIND=1)"); }

    SUBCASE("INTEGER(KIND=2)") { basicTypeTest<ais::Short>(TypeParser::Lang::Ftn, "INTEGER(KIND=2)"); }

    SUBCASE("INTEGER(KIND=4)") { basicTypeTest<ais::Int>(TypeParser::Lang::Ftn, "INTEGER(KIND=4)"); }

    SUBCASE("INTEGER(KIND=8)") { basicTypeTest<ais::Long>(TypeParser::Lang::Ftn, "INTEGER(KIND=8)"); }

    SUBCASE("INTEGER(KIND=16)") { basicTypeTest<ais::LongLong>(TypeParser::Lang::Ftn, "INTEGER(KIND=16)"); }

    SUBCASE("INTEGER*1") { basicTypeTest<ais::SignedChar>(TypeParser::Lang::Ftn, "INTEGER*1"); }

    SUBCASE("INTEGER*2") { basicTypeTest<ais::Short>(TypeParser::Lang::Ftn, "INTEGER*2"); }

    SUBCASE("INTEGER*4") { basicTypeTest<ais::Int>(TypeParser::Lang::Ftn, "INTEGER*4"); }

    SUBCASE("INTEGER*8") { basicTypeTest<ais::Long>(TypeParser::Lang::Ftn, "INTEGER*8"); }

    SUBCASE("INTEGER*16") { basicTypeTest<ais::LongLong>(TypeParser::Lang::Ftn, "INTEGER*16"); }

    SUBCASE("LOGICAL") {
        basicTypeTest<ais::Logical>(TypeParser::Lang::Ftn, "LOGICAL");
        basicTypeTest<ais::Logical>(TypeParser::Lang::Ftn, "logical");
    }

    SUBCASE("LOGICAL(1)") { basicTypeTest<ais::Logical>(TypeParser::Lang::Ftn, "LOGICAL(1)"); }

    SUBCASE("LOGICAL(2)") { basicTypeTest<ais::Logical>(TypeParser::Lang::Ftn, "LOGICAL(2)"); }

    SUBCASE("LOGICAL(4)") { basicTypeTest<ais::Logical>(TypeParser::Lang::Ftn, "LOGICAL(4)"); }

    SUBCASE("LOGICAL(8)") { basicTypeTest<ais::Logical>(TypeParser::Lang::Ftn, "LOGICAL(8)"); }

    SUBCASE("LOGICAL(16)") { basicTypeTest<ais::Logical>(TypeParser::Lang::Ftn, "LOGICAL(16)"); }

    SUBCASE("LOGICAL*1") { basicTypeTest<ais::Logical>(TypeParser::Lang::Ftn, "LOGICAL*1"); }

    SUBCASE("LOGICAL*2") { basicTypeTest<ais::Logical>(TypeParser::Lang::Ftn, "LOGICAL*2"); }

    SUBCASE("LOGICAL*4") { basicTypeTest<ais::Logical>(TypeParser::Lang::Ftn, "LOGICAL*4"); }

    SUBCASE("LOGICAL*8") { basicTypeTest<ais::Logical>(TypeParser::Lang::Ftn, "LOGICAL*8"); }

    SUBCASE("LOGICAL*16") { basicTypeTest<ais::Logical>(TypeParser::Lang::Ftn, "LOGICAL*16"); }

    SUBCASE("LOGICAL(KIND=1)") { basicTypeTest<ais::Logical>(TypeParser::Lang::Ftn, "LOGICAL(KIND=1)"); }

    SUBCASE("LOGICAL(KIND=2)") { basicTypeTest<ais::Logical>(TypeParser::Lang::Ftn, "LOGICAL(KIND=2)"); }

    SUBCASE("LOGICAL(KIND=4)") { basicTypeTest<ais::Logical>(TypeParser::Lang::Ftn, "LOGICAL(KIND=4)"); }

    SUBCASE("LOGICAL(KIND=8)") { basicTypeTest<ais::Logical>(TypeParser::Lang::Ftn, "LOGICAL(KIND=8)"); }

    SUBCASE("LOGICAL(KIND=16)") { basicTypeTest<ais::Logical>(TypeParser::Lang::Ftn, "LOGICAL(KIND=16)"); }

    SUBCASE("REAL") {
        basicTypeTest<ais::Float>(TypeParser::Lang::Ftn, "real");
        basicTypeTest<ais::Float>(TypeParser::Lang::Ftn, "REAL");
    }

    SUBCASE("REAL(4)") { basicTypeTest<ais::Float>(TypeParser::Lang::Ftn, "REAL(4)"); }

    SUBCASE("REAL(8)") { basicTypeTest<ais::Double>(TypeParser::Lang::Ftn, "REAL(8)"); }

    SUBCASE("REAL(16)") { basicTypeTest<ais::LongDouble>(TypeParser::Lang::Ftn, "REAL(16)"); }

    SUBCASE("REAL(KIND=4)") { basicTypeTest<ais::Float>(TypeParser::Lang::Ftn, "REAL(KIND=4)"); }

    SUBCASE("REAL(KIND=8)") { basicTypeTest<ais::Double>(TypeParser::Lang::Ftn, "REAL(KIND=8)"); }

    SUBCASE("REAL(KIND=16)") { basicTypeTest<ais::LongDouble>(TypeParser::Lang::Ftn, "REAL(KIND=16)"); }

    SUBCASE("REAL*4") { basicTypeTest<ais::Float>(TypeParser::Lang::Ftn, "REAL*4"); }

    SUBCASE("REAL*8") { basicTypeTest<ais::Double>(TypeParser::Lang::Ftn, "REAL*8"); }

    SUBCASE("REAL*16") { basicTypeTest<ais::LongDouble>(TypeParser::Lang::Ftn, "REAL*16"); }

    SUBCASE("STRING") {
        // STRING is unimplemented. Once it's implemented, update this test.
        // In practice a reported type of character*n is more common, which is parsed as
        // ais::FtnString and tested in the decorated types test.
        basicTypeTest<ais::Unimplemented>(TypeParser::Lang::Ftn, "STRING");
    }
}

TEST_CASE("TypeParser::getType: ftn decorated types") {
    SUBCASE("arrays ()") {
        // different compilers can report these types slightly differently, hence the
        // variations.

        SUBCASE("integer") {
            SUBCASE("1d") {
                auto t = basicTypeTest<ais::FtnArray>(TypeParser::Lang::Ftn, "integer, allocatable (64)");
                containerMatch<ais::FtnArray, ais::Int>(t, 64);
            }
            SUBCASE("2d") {
                auto t = basicTypeTest<ais::FtnArray>(TypeParser::Lang::Ftn, "integer, allocatable (8,17)");
                t = containerMatch<ais::FtnArray, ais::FtnArray>(t, 17);
                containerMatch<ais::FtnArray, ais::Int>(t, 8);
            }
            SUBCASE("3d") {
                auto t = basicTypeTest<ais::FtnArray>(TypeParser::Lang::Ftn, "integer, allocatable (2,5,7)");
                t = containerMatch<ais::FtnArray, ais::FtnArray>(t, 7);
                t = containerMatch<ais::FtnArray, ais::FtnArray>(t, 5);
                containerMatch<ais::FtnArray, ais::Int>(t, 2);
            }
        }

        SUBCASE("pointer, INTEGER*4") {
            SUBCASE("1d") {
                auto t =
                    basicTypeTest<ais::FtnArray>(TypeParser::Lang::Ftn, "pointer, INTEGER*4, allocatable (64)");
                t = containerMatch<ais::FtnArray, ais::Int>(t, 64);
            }
            SUBCASE("2d") {
                auto t = basicTypeTest<ais::FtnArray>(TypeParser::Lang::Ftn,
                    "pointer, INTEGER*4, allocatable (8,17)");
                t = containerMatch<ais::FtnArray, ais::FtnArray>(t, 17);
                containerMatch<ais::FtnArray, ais::Int>(t, 8);
            }
            SUBCASE("3d") {
                auto t = basicTypeTest<ais::FtnArray>(TypeParser::Lang::Ftn,
                    "pointer, INTEGER*4, allocatable (2,5,7)");
                t = containerMatch<ais::FtnArray, ais::FtnArray>(t, 7);
                t = containerMatch<ais::FtnArray, ais::FtnArray>(t, 5);
                containerMatch<ais::FtnArray, ais::Int>(t, 2);
            }
            SUBCASE("slice") {
                auto t = basicTypeTest<ais::FtnArray>(TypeParser::Lang::Ftn, "INTEGER*4, (0:9)");
                t = containerMatch<ais::FtnArray, ais::Int>(t, 10);
            }
            SUBCASE("2d slice") {
                auto t = basicTypeTest<ais::FtnArray>(TypeParser::Lang::Ftn, "INTEGER*4, (-10:10,-5:13)");
                t = containerMatch<ais::FtnArray, ais::FtnArray>(t, 19);
                containerMatch<ais::FtnArray, ais::Int>(t, 21);
            }
        }

        SUBCASE("integer*8") {
            SUBCASE("1d") {
                auto t = basicTypeTest<ais::FtnArray>(TypeParser::Lang::Ftn, "integer*8, allocatable (64)");
                containerMatch<ais::FtnArray, ais::Long>(t, 64);
            }
            SUBCASE("2d") {
                auto t = basicTypeTest<ais::FtnArray>(TypeParser::Lang::Ftn, "integer*8, allocatable (64,128)");
                t = containerMatch<ais::FtnArray, ais::FtnArray>(t, 128);
                containerMatch<ais::FtnArray, ais::Long>(t, 64);
            }
            SUBCASE("3d") {
                auto t =
                    basicTypeTest<ais::FtnArray>(TypeParser::Lang::Ftn, "integer*8, allocatable (64,128,256)");
                t = containerMatch<ais::FtnArray, ais::FtnArray>(t, 256);
                t = containerMatch<ais::FtnArray, ais::FtnArray>(t, 128);
                containerMatch<ais::FtnArray, ais::Long>(t, 64);
            }
        }

        SUBCASE("pointer, INTEGER*8") {
            SUBCASE("1d") {
                auto t =
                    basicTypeTest<ais::FtnArray>(TypeParser::Lang::Ftn, "pointer, INTEGER*8, allocatable (64)");
                t = containerMatch<ais::FtnArray, ais::Long>(t, 64);
            }
            SUBCASE("2d") {
                auto t = basicTypeTest<ais::FtnArray>(TypeParser::Lang::Ftn,
                    "pointer, INTEGER*8, allocatable (64,128)");
                t = containerMatch<ais::FtnArray, ais::FtnArray>(t, 128);
                containerMatch<ais::FtnArray, ais::Long>(t, 64);
            }
            SUBCASE("3d") {
                auto t = basicTypeTest<ais::FtnArray>(TypeParser::Lang::Ftn,
                    "pointer, INTEGER*8, allocatable (64,32,16)");
                t = containerMatch<ais::FtnArray, ais::FtnArray>(t, 16);
                t = containerMatch<ais::FtnArray, ais::FtnArray>(t, 32);
                containerMatch<ais::FtnArray, ais::Long>(t, 64);
            }
        }

        SUBCASE("real") {
            SUBCASE("1d") {
                auto t = basicTypeTest<ais::FtnArray>(TypeParser::Lang::Ftn, "real, allocatable (64)");
                containerMatch<ais::FtnArray, ais::Float>(t, 64);
            }
            SUBCASE("2d") {
                auto t = basicTypeTest<ais::FtnArray>(TypeParser::Lang::Ftn, "real, allocatable (4,44)");
                t = containerMatch<ais::FtnArray, ais::FtnArray>(t, 44);
                containerMatch<ais::FtnArray, ais::Float>(t, 4);
            }
            SUBCASE("3d") {
                auto t = basicTypeTest<ais::FtnArray>(TypeParser::Lang::Ftn, "real, allocatable (5,55,555)");
                t = containerMatch<ais::FtnArray, ais::FtnArray>(t, 555);
                t = containerMatch<ais::FtnArray, ais::FtnArray>(t, 55);
                containerMatch<ais::FtnArray, ais::Float>(t, 5);
            }
        }

        SUBCASE("pointer, REAL*4") {
            SUBCASE("1d") {
                auto t =
                    basicTypeTest<ais::FtnArray>(TypeParser::Lang::Ftn, "pointer, REAL*4, allocatable (12)");
                containerMatch<ais::FtnArray, ais::Float>(t, 12);
            }
            SUBCASE("2d") {
                auto t =
                    basicTypeTest<ais::FtnArray>(TypeParser::Lang::Ftn, "pointer, REAL*4, allocatable (12,24)");
                t = containerMatch<ais::FtnArray, ais::FtnArray>(t, 24);
                containerMatch<ais::FtnArray, ais::Float>(t, 12);
            }
            SUBCASE("3d") {
                auto t = basicTypeTest<ais::FtnArray>(TypeParser::Lang::Ftn,
                    "pointer, REAL*4, allocatable (12,24,48)");
                t = containerMatch<ais::FtnArray, ais::FtnArray>(t, 48);
                t = containerMatch<ais::FtnArray, ais::FtnArray>(t, 24);
                containerMatch<ais::FtnArray, ais::Float>(t, 12);
            }
        }

        SUBCASE("REAL*8") {
            SUBCASE("1d") {
                auto t =
                    basicTypeTest<ais::FtnArray>(TypeParser::Lang::Ftn, "pointer, REAL*8, allocatable (64)");
                t = containerMatch<ais::FtnArray, ais::Double>(t, 64);
            }
            SUBCASE("2d") {
                auto t =
                    basicTypeTest<ais::FtnArray>(TypeParser::Lang::Ftn, "pointer, REAL*8, allocatable (64,63)");
                t = containerMatch<ais::FtnArray, ais::FtnArray>(t, 63);
                containerMatch<ais::FtnArray, ais::Double>(t, 64);
            }
            SUBCASE("3d") {
                auto t = basicTypeTest<ais::FtnArray>(TypeParser::Lang::Ftn,
                    "pointer, REAL*8, allocatable (64,63,62)");
                t = containerMatch<ais::FtnArray, ais::FtnArray>(t, 62);
                t = containerMatch<ais::FtnArray, ais::FtnArray>(t, 63);
                containerMatch<ais::FtnArray, ais::Double>(t, 64);
            }
        }

        SUBCASE("complex") {
            SUBCASE("1d") {
                auto t = basicTypeTest<ais::FtnArray>(TypeParser::Lang::Ftn, "complex, allocatable (64)");
                t = containerMatch<ais::FtnArray, ais::Complex>(t, 64);
                containerMatch<ais::Complex, ais::Float>(t, 0);
            }
            SUBCASE("2d") {
                auto t = basicTypeTest<ais::FtnArray>(TypeParser::Lang::Ftn, "complex, allocatable (11,12)");
                t = containerMatch<ais::FtnArray, ais::FtnArray>(t, 12);
                t = containerMatch<ais::FtnArray, ais::Complex>(t, 11);
                containerMatch<ais::Complex, ais::Float>(t, 0);
            }
            SUBCASE("3d") {
                auto t = basicTypeTest<ais::FtnArray>(TypeParser::Lang::Ftn, "complex, allocatable (4,3,2)");
                t = containerMatch<ais::FtnArray, ais::FtnArray>(t, 2);
                t = containerMatch<ais::FtnArray, ais::FtnArray>(t, 3);
                t = containerMatch<ais::FtnArray, ais::Complex>(t, 4);
                containerMatch<ais::Complex, ais::Float>(t, 0);
            }
        }

        SUBCASE("pointer, COMPLEX*8") {
            SUBCASE("1d") {
                auto t =
                    basicTypeTest<ais::FtnArray>(TypeParser::Lang::Ftn, "pointer, COMPLEX*8, allocatable (64)");
                t = containerMatch<ais::FtnArray, ais::Complex>(t, 64);
                containerMatch<ais::Complex, ais::Float>(t, 0);
            }
            SUBCASE("2d") {
                auto t = basicTypeTest<ais::FtnArray>(TypeParser::Lang::Ftn,
                    "pointer, COMPLEX*8, allocatable (11,12)");
                t = containerMatch<ais::FtnArray, ais::FtnArray>(t, 12);
                t = containerMatch<ais::FtnArray, ais::Complex>(t, 11);
                containerMatch<ais::Complex, ais::Float>(t, 0);
            }
            SUBCASE("3d") {
                auto t = basicTypeTest<ais::FtnArray>(TypeParser::Lang::Ftn,
                    "pointer, COMPLEX*8, allocatable (4,3,2)");
                t = containerMatch<ais::FtnArray, ais::FtnArray>(t, 2);
                t = containerMatch<ais::FtnArray, ais::FtnArray>(t, 3);
                t = containerMatch<ais::FtnArray, ais::Complex>(t, 4);
                containerMatch<ais::Complex, ais::Float>(t, 0);
            }
        }

        SUBCASE("logical") {
            SUBCASE("1d") {
                auto t = basicTypeTest<ais::FtnArray>(TypeParser::Lang::Ftn, "logical, allocatable (64)");
                t = containerMatch<ais::FtnArray, ais::Logical>(t, 64);
            }
            SUBCASE("2d") {
                auto t = basicTypeTest<ais::FtnArray>(TypeParser::Lang::Ftn, "logical, allocatable (11,12)");
                t = containerMatch<ais::FtnArray, ais::FtnArray>(t, 12);
                containerMatch<ais::FtnArray, ais::Logical>(t, 11);
            }
            SUBCASE("3d") {
                auto t = basicTypeTest<ais::FtnArray>(TypeParser::Lang::Ftn, "logical, allocatable (4,3,2)");
                t = containerMatch<ais::FtnArray, ais::FtnArray>(t, 2);
                t = containerMatch<ais::FtnArray, ais::FtnArray>(t, 3);
                containerMatch<ais::FtnArray, ais::Logical>(t, 4);
            }
        }

        SUBCASE("pointer, LOGICAL*4") {
            SUBCASE("1d") {
                auto t =
                    basicTypeTest<ais::FtnArray>(TypeParser::Lang::Ftn, "pointer, LOGICAL*4, allocatable (64)");
                t = containerMatch<ais::FtnArray, ais::Logical>(t, 64);
            }
            SUBCASE("2d") {
                auto t = basicTypeTest<ais::FtnArray>(TypeParser::Lang::Ftn,
                    "pointer, LOGICAL*4, allocatable (11,12)");
                t = containerMatch<ais::FtnArray, ais::FtnArray>(t, 12);
                containerMatch<ais::FtnArray, ais::Logical>(t, 11);
            }
            SUBCASE("3d") {
                auto t = basicTypeTest<ais::FtnArray>(TypeParser::Lang::Ftn,
                    "pointer, LOGICAL*4, allocatable (4,3,2)");
                t = containerMatch<ais::FtnArray, ais::FtnArray>(t, 2);
                t = containerMatch<ais::FtnArray, ais::FtnArray>(t, 3);
                containerMatch<ais::FtnArray, ais::Logical>(t, 4);
            }
        }
    }

    SUBCASE("fortran string (character*n)") {
        auto t = basicTypeTest<ais::FtnString>(TypeParser::Lang::Ftn, "character*20");
        auto str_t = std::dynamic_pointer_cast<ais::FtnString>(t);
        REQUIRE(str_t != nullptr);
        CHECK(str_t->getLen() == 20);
    }
}

TEST_CASE("Mi2Aif caches types after they are parsed the first time") {
    auto mi = Mi2Aif(nullptr);
    auto t = TypeParser::getType(mi, nullptr, "bool");
    auto t2 = TypeParser::getType(mi, nullptr, "bool");
    auto t3 = TypeParser::getType(mi, nullptr, "float");
    REQUIRE(std::dynamic_pointer_cast<ais::Bool>(t) != nullptr);
    REQUIRE(std::dynamic_pointer_cast<ais::Bool>(t2) != nullptr);
    CHECK(t.get() == t2.get());
    CHECK(t.get() != t3.get());
}

TEST_CASE("TypeParser parse result of \"show lang\"") {
    CHECK(parseShowLangResult("The current source language is \"auto; currently fortran\".") == TypeParser::Lang::Ftn);
    CHECK(parseShowLangResult("The current source language is \"auto; currently c\".") == TypeParser::Lang::Cpp);
    CHECK(parseShowLangResult("The current source language is \"auto; currently c++\".") == TypeParser::Lang::Cpp);
    CHECK(parseShowLangResult("The current source language is \"auto; currently COBOL\".") == TypeParser::Lang::Unknown);

    CHECK(parseShowLangResult("The current source language is \"fortran\".") == TypeParser::Lang::Ftn);
    CHECK(parseShowLangResult("The current source language is \"c\".") == TypeParser::Lang::Cpp);
    CHECK(parseShowLangResult("The current source language is \"c++\".") == TypeParser::Lang::Cpp);
    CHECK(parseShowLangResult("The current source language is \"COBOL\".") == TypeParser::Lang::Unknown);

    CHECK(parseShowLangResult("junk") == TypeParser::Lang::Unknown);
    CHECK(parseShowLangResult("") == TypeParser::Lang::Unknown);
    CHECK(parseShowLangResult(nullptr) == TypeParser::Lang::Unknown);
}

} // namespace unittest
