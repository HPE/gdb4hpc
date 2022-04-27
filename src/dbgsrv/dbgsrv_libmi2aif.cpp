/******************************************************************************
 * libmi to aif conversion routines
 *
 * Copyright 2011-2022 Hewlett Packard Enterprise Development LP.
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
#include <mutex>
#include <condition_variable>
#include <thread>
#include <regex>

#include "dbgevent.h"
#include "dbgfn.h"
#include "dbgsrv_defs.h"
#include "dbgsrv_libmi2aif.h"
#include "dbgsrv_util.h"
#include "defs.h"
#include "useful.h"
#include "ais.h"

/* The following is used to turn on debug printing for the CreateSimpleAIF func.
** It generates a ton of output for large arrays, which is why we want to
** normally leave it off.
*/
//#define CreateSimpleAIF_PRINT

#ifdef CreateSimpleAIF_PRINT
#define CSAPRINT(...) fprintf(stderr, __VA_ARGS__)
#else /* CreateSimpleAIF_PRINT */
#define CSAPRINT(...)
#endif /* CreateSimpleAIF_PRINT */

// Used to fix infinite loop issue for recursive calls to whatis
#define LOOP_PREVENTION 20
extern int clientPrintElements;
static char *GetArrayDataString(MISession* session, MIVar *var);
static char* EvalExpression(MISession* session, const char* expression);

// external prototypes
extern void SendMICommand(MISession *, MICommand *);

/*
** Static prototypes
*/
namespace {

class WriteBuffer; struct TypeName; class MISlices;

class Mi2Aif {
private:
    MISession* m_session;
    size_t m_depth;
    std::vector<std::shared_ptr<MIVar>> m_refs;
    std::unordered_map<void*,size_t> m_refMap;

    std::shared_ptr<WriteBuffer> m_writeBuffer;
    ais::Writer m_writer;

    std::map<std::string, std::shared_ptr<ais::Type>> m_types;

public:
    Mi2Aif(MISession* session);

    std::shared_ptr<MIVar> createMIVar(const char *);
    //AIFStream* getAIF(std::shared_ptr<MIVar>);
    int getChildren(MIVar *);
    char* getVarValue(char *);
    MISession* getSession() const { return m_session; }
    std::string getPTypeAlias(const std::string& type);
    std::vector<std::string> getPTypeFields(const std::string& type);

    std::shared_ptr<ais::Type> getKnownType(const std::string& name);
    std::shared_ptr<ais::Type> addKnownType(const std::string& name,
                                            std::shared_ptr<ais::Type> type);

    size_t swizzlePointer(const std::string& type, void* val);
    void writeVal(std::shared_ptr<MIVar> var, int refIdx,
                  const std::function<void(bool)>& endOfSlice);

    std::string getSlice();

private:
    void sendMICommand(MICommand* command);
    std::shared_ptr<MIVar> createMIVarWithMetaInfo(const char* expr, const std::string& metaInfo);
    //void writeBackPointerAIF(size_t idx, const char* type);
};

// Implementation for translating primitive data objects into a data to be sent
// across the pipe.
// Note that the current implementation is just based on ostringstream, which
// is great for debugging but is definitely non-optimal.   It is expected this
// will be replaced with a cap'n proto based implementation at some point in the
// future.
class WriteBuffer : public ais::WriteBuffer {
public:
    WriteBuffer(Mi2Aif& mi) : m_mi(mi) {}
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
private:
    Mi2Aif& m_mi;
    std::ostringstream m_out;
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
    // Top level parsing function
    static std::shared_ptr<ais::Type> getType(Mi2Aif& mi, MIVar* var, const char* typeStr);

    static std::shared_ptr<ais::Type> wrap(std::shared_ptr<ais::Type> elemType, TypeName t);
    static std::shared_ptr<ais::Type> ftnWrap(std::shared_ptr<ais::Type> elemType, TypeName t);
    static std::shared_ptr<ais::Type> charWrap(std::shared_ptr<ais::Type> elemType, TypeName t);

    static std::tuple<std::string, std::string, std::string> getContainerTypes(TypeName t);
    static std::shared_ptr<ais::Type> stlContainer(Mi2Aif& mi, const ais::Array& contType,
                                                   TypeName t);
    static std::shared_ptr<ais::Type> stdArray(Mi2Aif& mi, TypeName t);
    static std::shared_ptr<ais::Type> stdString(Mi2Aif& mi, TypeName t);
    static std::shared_ptr<ais::Type> smartPointer(Mi2Aif& mi, TypeName t);
    static std::shared_ptr<ais::Type> wrapVoidFunc(Mi2Aif&, TypeName t);
    static std::shared_ptr<ais::Type> ftnCharacter(Mi2Aif& mi, TypeName t);
    static std::shared_ptr<ais::Type> unimplemented(Mi2Aif&, TypeName type);

    static std::string sanitizeType(const std::string& type);

    using Func = std::function<std::shared_ptr<ais::Type>(Mi2Aif& mi, TypeName t)>;
    using Prim = std::pair<std::string, Func>;

    template<class T>
    static Func PrimWrap() { return [](Mi2Aif&, TypeName t) {
                                        return wrap(std::make_shared<T>(),std::move(t)); } ;}

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

    static std::vector<Prim> m_cppPrims;
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
 {"signed char",PrimWrap<ais::SignedChar>()},
 {"std::array", stdArray},
 {"std::deque", ContainerWrap<ais::Deque>()},
 {"std::stack", ContainerWrap<ais::Stack>()},
 {"std::vector", ContainerWrap<ais::Vector>()},
 {"std::priority_queue", ContainerWrap<ais::PriorityQueue>()},
 {"std::queue", ContainerWrap<ais::Queue>()},
 {"std::shared_ptr", SmartPointerWrap()},
 {"std::unique_ptr", SmartPointerWrap()},
 {"uint8_t",PrimWrap<ais::UnsignedChar>()},
 {"uint16_t",PrimWrap<ais::UnsignedShort>()},
 {"uint32_t",PrimWrap<ais::UnsignedInt>()},
 {"uint64_t",PrimWrap<ais::UnsignedLong>()},
 {"unsigned char",CharWrap<ais::UnsignedChar>()},
 {"unsigned int",PrimWrap<ais::UnsignedInt>()},
 {"unsigned long",PrimWrap<ais::UnsignedLong>()},
 {"unsigned long int",PrimWrap<ais::UnsignedLong>()},
 {"unsigned long long",PrimWrap<ais::UnsignedLongLong>()},
 {"unsigned short",PrimWrap<ais::UnsignedShort>()},
 {"unsigned short int",PrimWrap<ais::UnsignedShort>()},
 {"void", wrapVoidFunc}
};

std::vector<TypeParser::Prim> TypeParser::m_fortranPrims =
{
 {"CHARACTER",ftnCharacter},
 {"COMPLEX",m_complex8},
 {"COMPLEX(4)",m_complex4},
 {"COMPLEX(8)",m_complex8},
 {"COMPLEX(16)",m_complex16},
 {"COMPLEX(KIND=4)",m_complex4},
 {"COMPLEX(KIND=8)",m_complex8},
 {"COMPLEX(KIND=16)",m_complex16},
 {"COMPLEX*8",m_complex4},
 {"COMPLEX*16",m_complex8},
 {"COMPLEX*32",m_complex16},
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
 {"REAL",m_real8},
 {"REAL(4)",m_real4},
 {"REAL(8)",m_real8},
 {"REAL(16)",m_real16},
 {"REAL(KIND=4)",m_real4},
 {"REAL(KIND=8)",m_real8},
 {"REAL(KIND=16)",m_real16},
 {"REAL*4",m_real4},
 {"REAL*8",m_real8},
 {"REAL*16",m_real16},
 {"STRING",unimplemented}
};

std::vector<std::string> TypeParser::m_ignore = { "const", "signed", "volatile", "shared",
                                                  "SHARED" };
class ArraySlicer;

// Data result reported by gdb.
// In the basic form, this is just the string reported on gdb on the
// "print expression", but large arrays be broken into multiple
// "bite-sized" pieces via the ArraySlicer object.
class GdbResult : public ais::InterpretStream {
public:
    GdbResult(Mi2Aif& mi, std::shared_ptr<MIVar> var, std::shared_ptr<ais::Type> varType);
    ~GdbResult() = default;
    const char* get() override;
    void setPos(const char* pos) override;
    bool isContinued() override;
    bool mayBeTruncated() override;
    void flush(bool isFinal);

    std::pair<std::shared_ptr<ais::Type>,bool> nextSlice();

    GdbResult(const GdbResult&) = delete;
    GdbResult& operator=(const GdbResult&) = delete;
    
private:
    Mi2Aif& m_mi;
    std::shared_ptr<MIVar> m_var;
    std::unique_ptr<ArraySlicer> m_slicer;
    std::shared_ptr<ais::Type> m_type;

    char* m_buffer;
    bool m_finalSlice = true;
    const char* m_pos;
};

} //namespace

const char *_aif_ignored_type_specifiers[] = {"const", "restrict", NULL};

// quote typename with spaces to pass to MI
static std::string quoteId(std::string var)
{
    if (var.find(' ') != std::string::npos && var.find('"') == std::string::npos) {
        var = "\"" + var + "\"";
    }
    return var;
}

std::unique_ptr<dbgevent_t, decltype(&FreeEvent)> dbg_error{nullptr, FreeEvent};

namespace {
// Class to define an AIFStream that can transmit one section at a time.
// This class perform the evaluation on a separate thread which allows
// the system to work on evaluating the next slice of the data structure (i.e.
// mainly readining from the gdb/mi socket) while it is still transmitting the
// current slice across the MRNet system.
class MISlices : public AIFStream::Slices {
public:
    MISlices(MISession* session) : m_mi2aif{std::make_unique<Mi2Aif>(session)} {}

    // Retrieve the next chunk of data to send across the pipe
    virtual std::string getNextSlice(bool& isFinal) override;

    // begin evaluating an expression.   Note that the evaluation
    // itself will occur in the background
    std::pair<dbgevent_t*,std::string> startEval(const char* var);

    
private:
    void eval(); 

    void flushOutput(bool isFinal);
    void reportError(dbgevent_t* evt);
    
    std::unique_ptr<Mi2Aif> m_mi2aif;
    std::shared_ptr<MIVar> m_mivar;
    std::string m_slice;
    bool m_isFinal = false;
    bool m_hasSlice = false;
    dbgevent_t* m_error = nullptr;
    std::mutex m_mutex;
    std::condition_variable m_cv;
};

std::string MISlices::getNextSlice(bool& isFinal) {
    // This side is called from the primary thread to get
    // evaluator thread result.
    // we need to wait for the evaluation thread to post
    // a result
    std::unique_lock l(m_mutex);
    m_cv.wait(l, [this] { return this->m_hasSlice; });

    if (m_error) throw m_error;

    isFinal = m_isFinal;
    auto retVal = std::move(m_slice);
    m_hasSlice = false;

    if (!isFinal) { m_cv.notify_one(); }
    return retVal;
}

std::pair<dbgevent_t*,std::string> MISlices::startEval(const char* var) {
    // reset the error var
    dbg_error.reset();
    
    DVPRINT("MISlices::startEval: Resolving variable %s.\n", var);

    // Create the mivar objecgt to interact with gdb/mi
    m_mivar = m_mi2aif->createMIVar(var);

    if (!m_mivar && strchr(var,'[')) {
        // the frontend can't tell if "(12)" is a C function call or a
        // fortran array index.
        DVPRINT("retrying as a function call\n");
        auto cvar = std::string(var);
        for (size_t p; (p=cvar.find("][")) != std::string::npos; ) {
            cvar.replace(p,2,1,',');
        }
        std::replace(cvar.begin(), cvar.end(), '[', '(');
        std::replace(cvar.begin(), cvar.end(), ']', ')');
        m_mivar = m_mi2aif->createMIVar(cvar.c_str());
    }

    if (!m_mivar) {
        DVPRINT("GetAIFVar: CreateMIVar returned null.\n");
        if (!dbg_error) {
            dbg_error.reset(NewErrorEvent(DBGERR_INTERNAL, "CreateMIVar failed."));
        }
        return std::make_pair(dbg_error.release(),std::string{""});
    }

    // Performng the evaluation happens on a separate thread.  For large
    // data objects, the primary thread is communicating with the front end,
    // sending along the next slice of data when the front end is ready to
    // receive it.
    auto evalThread = std::thread{ [this]() { this->eval(); }};
    evalThread.detach();

    return std::make_pair(nullptr,m_mivar->type);
}

// primary evaluator thread  function
void MISlices::eval() {
    DVPRINT("starting evaluator thread\n");
    try {
        // MI2Aif does the interaction with gdb/mi.   The lambda function we're
        // passing here tells it what do at the end of every slice
        m_mi2aif->writeVal(m_mivar,0,[this](bool isFinal) { this->flushOutput(isFinal); });
    } catch (std::exception& err) {
        reportError(NewErrorEvent(DBGERR_INTERNAL, err.what()));
    } catch (dbgevent_t* evt) {
        reportError(evt);
    }
    flushOutput(true);
    DVPRINT("exit evaluator thread\n");
};

// This is the interaction point between the evaluator thread and the
// primary thread.
void MISlices::flushOutput(bool isFinal) {
    std::unique_lock l(m_mutex);
    if (m_isFinal) return;
    m_cv.wait(l, [this] { return !this->m_hasSlice; });
    m_slice = m_mi2aif->getSlice();
    m_isFinal = isFinal;
    m_hasSlice = true;
    m_cv.notify_one();
}

// report an error from the evaluator thread
void MISlices::reportError(dbgevent_t* evt) {
    DVPRINT("in MISlices::reportError\n");
    std::unique_lock l(m_mutex);
    m_cv.wait(l, [this] { return !this->m_hasSlice; });
    m_slice = "";
    m_isFinal = true;
    m_error = evt;
    m_hasSlice = true;
    m_cv.notify_one();
}
}
    
// This is the "main" function that should be called in this interface...
// It returns NULL on success, otherwise it returns an error event.
dbgevent_t *GetAIFVar(MISession *session, const char *var, AIFStream **val,
                      char **type) {

    auto slices = std::make_shared<MISlices>(session);
    auto [errEvent, typeStr] = slices->startEval(var);
    if (errEvent) return errEvent;
    if (type) *type = strdup(typeStr.c_str()); // believed to be unused
    *val = new AIFStream(slices);
    return nullptr;
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


/*
 * Create a MI variable in gdb and get its MIVar type.
 * Returns null on error.
 */
std::shared_ptr<MIVar> Mi2Aif::createMIVar(const char *expr) {
    // Taking a cue from C++ annotations to use [[...]] to pass meta-data
    // to the back-end.   So far, this is just being used to pass length
    // information for raw C/C++ arrays.   I'd expect to add slicing info
    // via this channel in the future.
    if (expr && expr[0] == '[' && expr[1] == '[') {
        if (auto close = strstr(expr,"]]")) {
            auto metaInfo = std::string{expr+2, size_t(close-expr-2)};
            DVPRINT("got metaInfo \"%s\"\n", metaInfo.c_str());
            return createMIVarWithMetaInfo(close+2, metaInfo);
        }
    }


    auto session = m_session;
    auto delVar = [session](MIVar* mivar) { DeleteMIVar(session,mivar); };

    std::shared_ptr<MIVar> var(nullptr, delVar);

            /*    std::shared_ptr<MIVar> var(nullptr, [session](MIVar* mivar) {
                  if (mivar) { DeleteMIVar(session,mivar->name);  MIVarFree(mivar); }}); */

    MICommand* cmd = MIVarCreate("-", "*", quoteId(expr).c_str());
    sendMICommand(cmd);
    // If this fails, it means the symbol doesn't exist.
    if (!MICommandResultOK(cmd)) {
        dbg_error.reset(NewErrorEvent(DBGERR_NOSYM, expr));
        MICommandFree(cmd);
        return var;
    }

    MIVar* mivar = MIGetVarCreateInfo(cmd);
    MICommandFree(cmd);

    if (mivar->exp == NULL) {
        mivar->exp = strdup(expr);
    }

    mivar->expr = strdup(expr);

    // remove newline
    if (mivar->expr[strlen(expr) - 1] == '\n') {
        mivar->expr[strlen(expr) - 1] = '\0';
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
            dbg_error.reset(NewErrorEvent(DBGERR_DEBUGGER, errMsg));
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

inline bool fieldIsFunction(const char* str) {
    auto paren = strchr(str, '(');
    return (paren && paren[1] != '*');   // "(*" is a pointer to a function
}

// Find the more fundamental description of a type.   This is for when the
// application does something like "typedef int MyInt;
// So this function will "MyInt" -> "int"
std::string Mi2Aif::getPTypeAlias(const std::string& type) {
    MICommand* cmd = CLIPtype(type.c_str());
    sendMICommand(cmd);

    if (!MICommandResultOK(cmd)) {
        DVPRINT("GetStructInfo: CLIPtype call failed.\n");
        MICommandFree(cmd);
        return type;
    }

    auto retType = type;
    if (char* info = CLIGetPtypeInfo(cmd)) {
        // info ends with a \n
        if (auto len = strlen(info); len > 1 && !strchr(info,'{')) {
            retType = std::string{info, len-1};
            DVPRINT("Get CLIPTypeInfo %s\n", retType.c_str());
        }
        free(info);
    }
        
    MICommandFree(cmd);
    return retType;
}

// Get the fields of a struct from a gdb 'ptype" query
std::vector<std::string> Mi2Aif::getPTypeFields(const std::string& type)
{
    auto fields = std::vector<std::string>{};
    MICommand* cmd = CLIPtype(type.c_str());
    sendMICommand(cmd);

    if (!MICommandResultOK(cmd)) {
        DVPRINT("GetStructInfo: CLIPtype call failed.\n");
        MICommandFree(cmd);
        return fields;
    }

    MIList* field_list = CLIGetPtypeFieldList(cmd);
    MICommandFree(cmd);

    for (MIListSet(field_list); char* field = (char *)MIListGet(field_list); ) {
        DVPRINT("GetStructInfo: field is %s\n", field);
        if (!fieldIsFunction(field)) fields.push_back(field);
    }
    return fields;
}

std::shared_ptr<ais::Type> Mi2Aif::getKnownType(const std::string& name) {
    if (auto loc = m_types.find(name); loc != m_types.end()) return loc->second;
    else return std::shared_ptr<ais::Type>{};
}
std::shared_ptr<ais::Type> Mi2Aif::addKnownType(const std::string& name,
                                                std::shared_ptr<ais::Type> type) {
    m_types.insert(std::make_pair(name,type));
    return type;
}

// Map a pointer value to a reference index.  If the index is a new one,
// potentiall put the object onto the queue of objects to be traversed.
// This allows a data structure to be aggegated independently of the
// specific pointer value which may not be consistent across ranks.
size_t Mi2Aif::swizzlePointer(const std::string& type, void* p)
{
    // special case for null
    if (!p) return 0;

    if (m_refMap.empty() && m_refs.size() == 1) {
        // we put off indexing the first block until we know we are dealing with pointers      
        auto top = m_refs[0];
        DVPRINT("Top reference %s type %s value=%s\n", top->expr, top->type, top->value);
        void* addr_loc{};
        if (top->value && top->value[0]=='0') {
            // the top level is a pointer and we already have the pointer value                
            addr_loc = reinterpret_cast<void*>(strtoll(m_refs[0]->value, nullptr, 0));
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

    size_t idx = m_refs.size()+1;
    if (auto [loc, added] = m_refMap.insert(std::make_pair(p,idx)); added) {
        auto derefExpr = std::ostringstream{};
        derefExpr << "\"*(" << type << "*)" << p << "\"";
        DVPRINT("Creating new MIVar with %s\n", derefExpr.str().c_str());
        
        m_refs.push_back(createMIVar(derefExpr.str().c_str()));
    } else {
        idx = loc->second;
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

// Evalate the value of 'var' and write the result to the m_writer
// member.   Call the function endOfSlice with the argument 'isFinal'
// when a slice is ready to be transmitted.  Typically, this is a single
// slice, but large arrays can be sent in multiple slices.
void Mi2Aif::writeVal(std::shared_ptr<MIVar> var, int refIdx,
                      const std::function<void(bool)>& endOfSlice) {
    DepthTracker depth(m_depth);
    if (depth.isTop() && var->type && var->type[strlen(var->type)-1] != '*') {
        // Save the input expression - if it contains pointers we will need
        // to index the address of this object to resolve any back-pointers in
        // the structure.
        DVPRINT("Pushing top expression %s, type %s\n", var->expr, var->type);
        m_refs.push_back(var);
    }
    auto initNumRefs = m_refs.size();

    if (strcmp(var->type, "<text variable, no debug info>") == 0) {
        DVPRINT("GetAIF: No symbols\n");
        throw NewErrorEvent(DBGERR_NOSYMS, nullptr);
    }

    DVPRINT(
        "writeVal: MIVar: expr %s, name %s, exp %s, numchild %d, type %s, "
        "value %s, thread_id %d\n",
        var->expr, var->name, var->exp, var->numchild, var->type, var->value,
        var->thread_id);

    auto obj = TypeParser::getType(*this, var.get(), nullptr);
    auto eval = GdbResult{*this, var, obj};

    DVPRINT("About to call any->parse\n");

    try {

        while (true) {
            if (auto [ sliceType, isFinal ] = eval.nextSlice(); sliceType) {
                // wrapping the object with ais::any sends the type info for the top
                // level object.
                auto any = std::make_unique<ais::Any>(sliceType, refIdx);
                any->parse(eval, m_writer);
                if (!isFinal) {
                    endOfSlice(isFinal);
                    m_writer.resetTypes();
                }
            } else {
                break;
            }
        }
    } catch (std::exception& err) {
        DVPRINT("parsing failed with %s\n", err.what());
    }

    DVPRINT("Done with any->parse\n");

    // types based multiple blocks connected via pointers or references.  For comparative
    // debugging especially, we have to observe the whole state of the data structure.
    if (depth.isTop() && m_refs.size() > initNumRefs) {

        // add the reference elements as members of the struct - note that
        // the current val is deleted on the first iteration of this loop
        auto refLimit = (clientPrintElements > 0 ?
                         static_cast<size_t>(clientPrintElements) :
                         std::numeric_limits<size_t>::max());

        // create the elements, note that m_refs can grow inside the loop
        for (size_t i=initNumRefs; i<m_refs.size() && i<refLimit ; ++i) {
            if (m_refs[i]) {
                writeVal(m_refs[i], i+1, endOfSlice);
            }
        }
    }
}

std::string Mi2Aif::getSlice() {
    return m_writeBuffer->exportResult();
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

    if (var->expr) {
        DVPRINT("In GetArrayDataString %s\n", var->expr);
        DBEPERFLOG(SetTimerStart("GetArrayAIF: -data-evaluate-expression"));

        bool last_resort = false;
        auto expr = quoteId(var->expr);
        if (typeIsReference(var->type)) {
            expr = "*&" + expr;
            DVPRINT("Type is a reference, using %s\n", expr.c_str());
        } else if (strstr(var->type, "std::unique_ptr<") == var->type) {
            expr += ".get()";
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
                dbg_error.reset(NewErrorEvent(DBGERR_DEBUGGER, buf));
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
    ArraySlicer(MISession* session, std::shared_ptr<MIVar> var, std::shared_ptr<ais::Type> varType);
    ~ArraySlicer();

    // evaluate the next slice of the variable result.
    std::tuple<std::shared_ptr<ais::Type>, char*, bool> getNext();

    const static size_t DEFAULT_BLOCK_SIZE = 1000000; // one million

private:
    void determineSlicing(const ais::Type& varType);

    ArraySlicer(const ArraySlicer&) = delete;
    ArraySlicer& operator=(const ArraySlicer&) = delete;

    MISession* m_session;       // session to connect to gdb/MI
    
    std::shared_ptr<MIVar> m_var;  // MI variable of interest
    std::shared_ptr<ais::Type> m_type;   // type of the full variable
    char* m_buf;                // allocated array for the result (contolled by this)
    char* m_array;              // interpretable region of m_buf

    size_t m_blockSize;         // maximum number of elements in each query
    std::string m_subExpr;      // encode expression to use for querying a chunk
    long m_subIdx;              // index for the current chunk
    long m_maxSubIdx;           // index for the highest chunk to return
    long m_slice;               // stride for the chunking index.
};

ArraySlicer::ArraySlicer(MISession* session, std::shared_ptr<MIVar> var,
                         std::shared_ptr<ais::Type> varType)
    : m_session(session), m_var(var), m_type(varType),
      m_buf(nullptr), m_array(nullptr), m_blockSize(DEFAULT_BLOCK_SIZE),
      m_subIdx(0), m_maxSubIdx(0), m_slice(0)
{
    static const char* blockSizeStr = ::getenv("GDB4HPC_BLOCK_ELEMENTS");
    if (blockSizeStr) {
        long lval = atol(blockSizeStr);
        if (lval >= 0)
            m_blockSize = lval;
    }

    if (m_blockSize) {
        determineSlicing(*varType);
    }
}

ArraySlicer::~ArraySlicer()
{
    free(m_buf);
}

// Paging and the like is unavailable in MI mode.   Instead, this class and this function
// specifically examines the data type and determines how it can be extracted in multiple
// data print calls that each pull out a portion of the data structure
void ArraySlicer::determineSlicing(const ais::Type& varType)
{
    DVPRINT("in ArraySlicer::determineSlicing blocksize if %zu\n", m_blockSize);
    
    if (!m_blockSize)
        return;

    std::vector<int> arrayLens;
    for (auto v = &varType; auto arr = dynamic_cast<const ais::Array*>(v); v=arr->getElemType()) {
        auto len = arr->getLen();
        if (auto vec = dynamic_cast<const ais::Vector*>(arr); vec && !len) {
            std::string sizeStr = std::string("(") + m_var->exp + ").size()";
            char* sizeRes = EvalExpression(m_session, sizeStr.c_str());
            if (!sizeRes) {
                DVPRINT("could not get vector length\n");
                return;
            }

            len = atoi(sizeRes);
            DVPRINT("Slicing vector got a size %d\n", len);
            free(sizeRes);
        }

        
        DVPRINT("adding length %d\n", len);
        arrayLens.push_back(len);
    }

    if (arrayLens.empty()) {
        return;
    }

    if (auto vec = dynamic_cast<const ais::Vector*>(&varType); vec && !vec->getLen()) {
        std::string sizeStr = std::string("(") + m_var->exp + ").size()";
        char* sizeRes = EvalExpression(m_session, sizeStr.c_str());
        if (!sizeRes) {
            DVPRINT("could not get vector length\n");
            return;
        }

        auto size = arrayLens[0] = atoi(sizeRes);
        DVPRINT("Slicing vector got a size %d\n", size);
        free(sizeRes);

        if (size <= ptrdiff_t(m_blockSize))
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
    m_subExpr = m_var->expr;
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
        return std::tuple(m_type, array, isFinal);
    }

    auto retType = m_type;

    if (m_array) {
        DVPRINT("In ArraySlicer::getNext no size information\n");
        // we were unable to get the size information for variable, so we
        // have already gotten the full data
        array = m_array;
        m_array = nullptr;
        ++m_subIdx;
    } else if (m_slice == 0 && !m_buf) {
        DVPRINT("In ArraySlicer::getNext not big enough to split\n");
        // splitting is not enabled, the array is not big enough to split,
        // or it's a form the splitter doesn't recognize, just return it
        // all
        array = m_buf = GetArrayDataString(m_session, m_var.get());
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

        array = EvalExpression(m_session,const_cast<char*>(subExpr.c_str()));
        if (array) {
            // success!
            DVPRINT("Succeeded getting sub-array value: %.400s\n", array);

            // update the array type to contain just the length of the slice
            auto subArrayType = std::static_pointer_cast<ais::Array>(m_type);
            subArrayType = subArrayType->makeTyped(subArrayType->getElemTypePtr());
            subArrayType->setBounds(len, m_subIdx);
            retType = subArrayType;
            
            m_subIdx = maxIdx+1;

        } else {
            // failure!  - fall back to just evaluating the whole kielbasa
            array = GetArrayDataString(m_session, m_var.get());
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
        m_out  << "@" << std::quoted(val);
    }

    void WriteBuffer::writePointer(const std::string& type, void* val) {
        int idx = m_mi.swizzlePointer(type, val);
        m_out << " " << idx;
    }

    std::string WriteBuffer::exportResult() {
        auto res = m_out.str();
        m_out.str("");
        return res;
    }
}
    

Mi2Aif::Mi2Aif(MISession* session) : m_session(session), m_depth(0),
                                     m_writeBuffer(std::make_shared<WriteBuffer>(*this)),
                                     m_writer(m_writeBuffer) {}

std::string TypeParser::sanitizeType(const std::string& type) {
    // remove "const", "signed", and "volatile" from C++
    // "shared" and "SHARED" from fortran
    // "@[a-zA-z]* from nvidia registers
    auto sanType = type;
    bool inW = false;
    for (int i=0, word=0; i<int(sanType.size()); ++i) {
        auto c = sanType[i];
        auto cIsW = (::isalnum(c) || c == '_' || c == '@' || c == ':');
        if (cIsW && !inW) {
            word = i;
        } else if (inW && !cIsW &&
                   (sanType[word] == '@' || std::find(m_ignore.begin(), m_ignore.end(),
                                                      sanType.substr(word,i-word)) != m_ignore.end())) {
            if (c == ' ' && (!word || sanType[word-1] == ' ')) ++i;
            sanType.erase(word, i-word);
            i = word-1;
        }
        inW = cIsW;
    }

    // remove leading and trailing whitespace
    if (auto p = sanType.find_first_not_of(' ')) sanType.erase(0,p);
    if (auto p = sanType.find_last_not_of(" &"); p != std::string::npos) {
        sanType.erase(p+1);
    }
    return sanType;
}

std::shared_ptr<ais::Type> TypeParser::getType(Mi2Aif& mi, MIVar* var, const char* inType) {
    static bool isSorted = false;
    if (!isSorted) {
        std::sort(m_cppPrims.begin(), m_cppPrims.end(), comparePrims);
        std::sort(m_fortranPrims.begin(), m_fortranPrims.end(), comparePrims);
        isSorted = true;
    }

    auto isId = [](int c) { return std::isalnum(c) || c == '_'; };

    DVPRINT("TypeParser::getType var=\"%s\" inType=\"%s\"\n", 
            var ? var->type : "", inType ? inType : "");


    const char* typeStr = inType ? inType : var->type;
    auto type = sanitizeType(typeStr);

    DVPRINT("looking up type for %s (%s)\n", typeStr, type.c_str());

    if (auto knownType = mi.getKnownType(type)) {
        DVPRINT("found known type for %s\n", type.c_str());
        return knownType;
    }

    // pointer types
    if (type.back() == '*') {
        DVPRINT("Found a pointer type %s\n", type.c_str());
        auto derefType = type.substr(0,type.find_last_not_of(' ', type.size()-2)+1);
        if (derefType == "char") {
            return mi.addKnownType(type, std::make_shared<ais::CharArray>());
        } else {
            return mi.addKnownType(type, std::make_shared<ais::Pointer>(derefType));
        }
    }

    // check for C++ types
    // we need to use upper_bound and work backwards to be sure be get the maximal
    // match, e.g. "long" v. "long double"
    auto cpp = Prim{type,0};
    auto loc = std::upper_bound(m_cppPrims.begin(), m_cppPrims.end(), cpp, comparePrims);
    while (loc != m_cppPrims.begin() && (--loc)->first[0] == type[0]) {
        const auto& match = loc->first;
        if (match == type.substr(0, match.size()) &&
            (type.size() == match.size() || !isId(type[match.size()]))) {
            DVPRINT("returning type %s %d\n", type.c_str(), static_cast<int>(match.size()));

            return mi.addKnownType(type, loc->second(mi, TypeName{type,match.size()}));
        }
    }

    // check for a fortran primitive, upcase since fortran is case insensitive
    auto uptype = type;
    std::transform(uptype.begin(), uptype.end(), uptype.begin(), ::toupper);

    // we need to use upper_bound and work backwards to be sure be get the maximal
    // match, e.g. "INTEGER" v. "INTEGER*16"
    auto ftn = Prim{uptype,0};
    loc = std::upper_bound(m_fortranPrims.begin(), m_fortranPrims.end(), ftn, comparePrims);
    
    while (loc != m_fortranPrims.begin() && (--loc)->first[0] == uptype[0]) {
        const auto& match = loc->first;
        if (match == uptype.substr(0, match.size())) {
            DVPRINT("got fortran type %s\n", match.c_str());
            return mi.addKnownType(type, loc->second(mi, TypeName{uptype,match.size()}));
        }
    }

    static std::regex stdStringRE(R"((std::[A-Za-z0-9_:]*string)<.*)");
    static std::regex stdStringRE2(R"((std::[A-Za-z0-9_:]*string))");
    std::smatch match;
    if (std::regex_match(type, match, stdStringRE) ||
        std::regex_match(type, match, stdStringRE2)) {
        DVPRINT("matched on a string type %s\n", match[1].str().c_str());
        return mi.addKnownType(type, stdString(mi, TypeName{type,match[1].str().size()}));
    }

    // possible typedef for a more fundamental type
    auto aliasName = mi.getPTypeAlias(type);
    if (aliasName != type) {
        DVPRINT("got an alias \"%s\"\n", aliasName.c_str());
        return getType(mi, var, aliasName.c_str());
    }

    // compound types - (always struct or class? - not sure)
    // extract the base type - * or [ begins the decoration.
    auto ty = type.begin();
    auto templateLevel = int{0};
    for ( ; ty != type.end(); ++ty) {
        if (*ty == '<') ++templateLevel;
        else if (*ty == '>') --templateLevel;
        else if (!templateLevel && (*ty == '*' || *ty == '[')) break;
    }
    for ( ; ty != type.begin() && isspace(*(ty-1)); --ty) {}

    auto typeLen = ty - type.begin();
    auto baseType = type.substr(0,typeLen);
    auto decoration = type.substr(typeLen);
    auto stype = std::make_shared<ais::Struct>(baseType);

    DVPRINT("get baseType %s decoration %s\n", baseType.c_str(), decoration.c_str());

    // register the type here so a class/struct with a pointer to its own type
    // (like a linked list or graph) doesn't get into infinite recursion
    mi.addKnownType(baseType, stype);

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
            if (var->type && var->type != baseType && var->numchild) {
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
                stype->addMember(getMemberName(child1), getType(mi,child1,nullptr));
            } else if (!mi.getChildren(child1)) {
                // child is public|protected|private - walk down another level
                for (int j=0; j<child1->numchild; ++j) {
                    auto child2 = child1->children[j];
                    child2->expr = strdup(child2->name);
                    stype->addMember(getMemberName(child2), getType(mi,child2,nullptr));
                }
            } else {
                DVPRINT("getting children failed\n");
            }
        }
        DVPRINT("done processing members\n");
    } else {
        auto fieldStrs = mi.getPTypeFields(type);
        for (auto&& fieldStr : fieldStrs) {
            // the field is type followed by name
            auto namePos = fieldStr.rfind(' ');
            if (namePos == std::string::npos) continue;

            while (++namePos < fieldStr.size() && fieldStr[namePos] == '*') {}
            auto fieldTypeStr = fieldStr.substr(0,namePos);
            auto fieldName = fieldStr.substr(namePos);
            DVPRINT("got field type \"%s\" name \"%s\"\n",
                    fieldTypeStr.c_str(), fieldName.c_str());

            stype->addMember(fieldName, getType(mi,nullptr,fieldTypeStr.c_str()));
        }
    }

    DVPRINT("calling addKnownType %s decoration %s\n", baseType.c_str(),
            decoration.c_str());

    mi.addKnownType(baseType, stype);
    return wrap(stype, TypeName{type,static_cast<size_t>(typeLen)});
}

// Handle decorations of the base type.  Like *,&,[], etc.
std::shared_ptr<ais::Type> TypeParser::wrap(std::shared_ptr<ais::Type> elemType, TypeName t) {
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
    } else {
        w.reset(new ais::Unimplemented{"unknown-decoration-" + t.decoration});
        end = r.size()-1;
    }

    return wrap(w, TypeName(t,end+1));
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
            int len;
            if (is.peek() == ':') {
                is.ignore();
                DVPRINT("Unallocated array\n");
                result = std::make_shared<ais::FtnArray>(result,0);
            } else if (is >> len) {
                DVPRINT("got len %d\n", len);
                result = std::make_shared<ais::FtnArray>(result, len);
            } 
        } else if (inSize && c == ')') {
            inSize = false;
        } else if (c == '*') {
            int len;
            if (is >> len) {
                DVPRINT("got len %d\n", len);
                result = std::make_shared<ais::FtnArray>(result, len);
            }
        } else if (!isspace(c) && !strchr(",ALOCTBE", c)) {
            // notice we are ignoring ", ALLOCATABLE "
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
    return wrap(vtype, t2);
}

std::shared_ptr<ais::Type> TypeParser::stdArray(Mi2Aif& mi, TypeName t) {
    DVPRINT("parsing stdArray container type %s\n", t.decoration.c_str());

    auto [containedType, templateArgs, decoration] = getContainerTypes(t);
    if (containedType.empty()) {
        return unimplemented(mi,t);
    }

    DVPRINT("Got types: \"%s\", \"%s\", \"%s\"\n",
            containedType.c_str(), templateArgs.c_str(), decoration.c_str());

    auto vt = std::string{"::value_type"};
    if (decoration.find(vt) == 0) {
        DVPRINT("got std_array::value_type\n");
        auto elemType = getType(mi, nullptr, containedType.c_str());
        auto t2 = TypeName{std::move(containedType), decoration.substr(vt.size())};
        return wrap(elemType, t2);
    }
        

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
    return wrap(vtype, t2);
}

std::shared_ptr<ais::Type> TypeParser::stdString(Mi2Aif& mi, TypeName t) {
    DVPRINT("parsing stdString container type %s\n", t.decoration.c_str());

    auto [containedType, templateArgs, decoration] = getContainerTypes(t);

    DVPRINT("Got types: \"%s\", \"%s\", \"%s\"\n",
            containedType.c_str(), templateArgs.c_str(), decoration.c_str());

    auto t2 = TypeName{t.type  + "<" + containedType +">", std::move(decoration)};
    return wrap(std::make_shared<ais::String>(), t2);
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
    return wrap(std::make_shared<ais::SmartPointer>(t.type, containedType), t2);
}


std::shared_ptr<ais::Type> TypeParser::charWrap(std::shared_ptr<ais::Type> elemType,
                                                TypeName t) {

    DVPRINT("in charWrap \"%s\" + \"%s\"\n", t.type.c_str(), t.decoration.c_str());

    auto beg = t.decoration.find_first_not_of(' ');
    if (beg == std::string::npos) {
        return elemType;
    } else if (t.decoration[beg] == '*') {
        return wrap(std::make_shared<ais::CharArray>(), TypeName{t, beg+1});
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
        return wrap(std::make_shared<ais::CharArray>(), t2);

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
    
std::shared_ptr<ais::Type> TypeParser::unimplemented(Mi2Aif& mi, TypeName t) {
    return std::make_unique<ais::Unimplemented>(t.type + t.decoration);
}

GdbResult::GdbResult(Mi2Aif& mi, std::shared_ptr<MIVar> var, std::shared_ptr<ais::Type> varType)
    : m_mi(mi), m_var(var), m_type(varType), m_buffer(nullptr), m_pos(nullptr) {
    DVPRINT("Creating GdbResult\n");
    m_slicer.reset(new ArraySlicer(m_mi.getSession(), var, varType));
}

const char* GdbResult::get() { return m_pos; }

std::pair<std::shared_ptr<ais::Type>,bool> GdbResult::nextSlice() {
    
    if (auto [ type, arr, isFinal ] = m_slicer->getNext(); arr) {
        m_pos = m_buffer = arr;
        m_finalSlice = true;
        return std::pair{type, isFinal};
    } else {
        m_pos = m_buffer = nullptr;
        return std::pair{std::shared_ptr<ais::Type>{}, true};
    }
}

void GdbResult::setPos(const char* pos) {
    m_pos = pos;
    for (auto *p = pos; *p; ++p) {
        if (!::isspace(*p))
            return;
    }
    m_buffer = nullptr;
}

bool GdbResult::isContinued() {
    if (m_finalSlice) return false;
    for (auto *p = m_pos; *p; ++p) {
        if (!::isspace(*p))
            return false;
    }
    return true;
}

bool GdbResult::mayBeTruncated() {
    return (clientPrintElements > 0);
}

