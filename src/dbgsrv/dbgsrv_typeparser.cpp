/******************************************************************************
 * routines the find the ais::Type of an expression
 *
 * Copyright 2023 Hewlett Packard Enterprise Development LP.
 ******************************************************************************/

#include "dbgsrv_libmi2aif.h"
#include "dbgsrv_defs.h"
#include "unittest_macros.h"
#include "parse.h"
#include <regex>

extern void SendMICommand(MISession *, MICommand *);
extern size_t FindTemplateLength(const std::string& str, size_t pos);

namespace TypeParser {

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


// Top level parsing function. Invoking it will detect which language the current
// frame is in and then call one of the sub-parsers (getCpp/FtnType)
std::shared_ptr<ais::Type> getType(Mi2Aif& mi, MIVar* var, const char* typeStr);

std::string sanitizeType(const std::string& type);

std::shared_ptr<ais::Type> getCppType(Mi2Aif& mi, MIVar* var, const char* typeStr);
std::shared_ptr<ais::Type> getFtnType(Mi2Aif& mi, const char* typeStr);

// Supported languages parsable by getType
enum class Lang {
                 Unknown,
                 Cpp,
                 Ftn,
};

std::shared_ptr<ais::Type> cppWrap(std::shared_ptr<ais::Type> elemType, TypeName t);
std::shared_ptr<ais::Type> ftnWrap(std::shared_ptr<ais::Type> elemType, TypeName t);
std::shared_ptr<ais::Type> charWrap(std::shared_ptr<ais::Type> elemType, TypeName t);

std::tuple<std::string, std::string, std::string> getContainerTypes(TypeName t);
std::shared_ptr<ais::Type> stlContainer(Mi2Aif& mi, const ais::Array& contType,
                                        TypeName t);
std::shared_ptr<ais::Type> stdArray(Mi2Aif& mi, TypeName t);
std::shared_ptr<ais::Type> stdPair(Mi2Aif& mi, TypeName t);
std::shared_ptr<ais::Type> stdTuple(Mi2Aif& mi, TypeName t);
std::shared_ptr<ais::Type> kokkosView(Mi2Aif& mi, TypeName t);
std::shared_ptr<ais::Type> rajaView(Mi2Aif& mi, TypeName t);
std::shared_ptr<ais::Type> stdConditional(Mi2Aif& mi, TypeName t);
std::shared_ptr<ais::Type> stdString(Mi2Aif& mi, TypeName t);
std::shared_ptr<ais::Type> smartPointer(Mi2Aif& mi, TypeName t);
std::shared_ptr<ais::Type> wrapVoidFunc(Mi2Aif&, TypeName t);
std::shared_ptr<ais::Type> wrapEnum(Mi2Aif&, TypeName t);
std::shared_ptr<ais::Type> wrapUnion(Mi2Aif&, TypeName t);
std::shared_ptr<ais::Type> ftnCharacter(Mi2Aif& mi, TypeName t);
std::shared_ptr<ais::Type> unimplemented(Mi2Aif&, TypeName type);


// Query gdbmi for the current lang setting
Lang queryCurrentLang(MISession* mi);

using Func = std::function<std::shared_ptr<ais::Type>(Mi2Aif& mi, TypeName t)>;
using Prim = std::pair<std::string, Func>;

template<class T>
Func PrimWrap() { return [](Mi2Aif&, TypeName t) {
                             return cppWrap(std::make_shared<T>(),std::move(t)); } ;}

template<class T>
Func FtnWrap() { return [](Mi2Aif&, TypeName t) {
                            return ftnWrap(std::make_shared<T>(), std::move(t)); } ;}

template<class T>
Func ComplexWrap() {
    return [](Mi2Aif&, TypeName t) {
               return ftnWrap(std::make_shared<ais::Complex>(std::make_shared<T>()),
                              std::move(t)); } ;}

template<class T>
Func CharWrap() {
    return [](Mi2Aif&, TypeName t) {
               return charWrap(std::make_shared<T>(),std::move(t)); } ;}

template<class T>
Func ContainerWrap() {
    return [](Mi2Aif& mi, TypeName t) { return stlContainer(mi, T(), std::move(t)); }; }

static auto //std::tuple<TypeName, std::shared_ptr<ais::Type>, std::shared_ptr<ais::Type>>
getMapTypes(Mi2Aif& mi, TypeName t) {
    auto [keyTypeName, templateArgs, decoration] = getContainerTypes(t);

    auto comma = templateArgs.find(','), lt = templateArgs.find('<');
    if (lt != std::string::npos && lt < comma) {
        comma = templateArgs.find(',', findExprMatch(templateArgs,lt));
    }
    auto beg = size_t{0};
    while (beg < comma && std::isspace(templateArgs[beg])) ++beg;
    auto valueTypeName = templateArgs.substr(0, comma);
    DVPRINT("have std::map with key \"%s\" and value \"%s\"\n", keyTypeName.c_str(),
            valueTypeName.c_str());

    auto t2 = TypeName{t.type + "<" + keyTypeName + "," + valueTypeName + ">",
                       std::move(decoration)};
    auto keyType = getType(mi, nullptr, keyTypeName.c_str());
    auto valueType = getType(mi, nullptr, valueTypeName.c_str());

    return std::make_tuple(t2, keyType, valueType);
}
    

template<class T>
Func MapWrap() {
    return [](Mi2Aif& mi, TypeName t) {
               auto [t2, keyType, valueType] = getMapTypes(mi, t);
               auto vtype = std::make_shared<T>(keyType, valueType);
               return cppWrap(vtype, t2);
           };
}
        
Func SmartPointerWrap() {
    return [](Mi2Aif& mi, TypeName t) { return smartPointer(mi, std::move(t)); }; }

void addStructMembers(Mi2Aif& mi, ais::Struct& stype, MIVar* var,
                      const std::string& type);

inline bool comparePrims(const Prim& a, const Prim& b) { return a.first < b.first; }

Func m_logical = FtnWrap<ais::Logical>();
Func m_int1 = FtnWrap<ais::SignedChar>();
Func m_int2 = FtnWrap<ais::Short>();
Func m_int4 = FtnWrap<ais::Int>();
Func m_int8 = FtnWrap<ais::Long>();
Func m_int16 = FtnWrap<ais::LongLong>();
Func m_real4 = FtnWrap<ais::Float>();
Func m_real8 = FtnWrap<ais::Double>();
Func m_real16 = FtnWrap<ais::LongDouble>();
Func m_complex4 = ComplexWrap<ais::Float>();
Func m_complex8 = ComplexWrap<ais::Double>();
Func m_complex16 = ComplexWrap<ais::LongDouble>();

std::vector<Prim> m_cppPrims =
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

std::vector<Prim> m_cppStlPrims =
{
 {"std::array", stdArray},

 {"std::conditional_t", stdConditional},
 {"std::deque", ContainerWrap<ais::Deque>()},
 {"std::stack", ContainerWrap<ais::Stack>()},
 {"std::vector", ContainerWrap<ais::Vector>()},
 {"std::list", ContainerWrap<ais::List>()},
 {"std::__cxx11::list", ContainerWrap<ais::List>()},
 {"std::priority_queue", ContainerWrap<ais::PriorityQueue>()},
 {"std::queue", ContainerWrap<ais::Queue>()},
 {"std::shared_ptr", SmartPointerWrap()},
 {"std::unique_ptr", SmartPointerWrap()},
 {"std::map", MapWrap<ais::Map>()},
 {"std::multimap", MapWrap<ais::Multimap>()},
 {"std::unordered_map", MapWrap<ais::UnorderedMap>()},
 {"std::unordered_multimap", MapWrap<ais::UnorderedMultimap>()},
 {"std::pair", stdPair},
 {"std::tuple", stdTuple},
 {"Kokkos::View", kokkosView},
 {"RAJA::internal::ViewBase", rajaView},      // "ptype RAJA::View<...>" will turn into this
 {"RAJA::internal::TypedViewBase", rajaView}, // or this
};

std::vector<Prim> m_fortranPrims =
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

std::vector<std::string> m_ignore = { "const", "volatile", "shared",
                                                  "SHARED", "pointer,", "POINTER," };

std::string sanitizeType(const std::string& type) {
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
        DVPRINT("got derived class type: \"%s\"\n", sanType.c_str());
    }

    // gdb prints (anonymous namespace)::
    for (size_t pos=0; (pos=sanType.find(ANONYMOUS_NAMESPACE,pos)) != std::string::npos; ) {
        sanType.erase(pos, ANONYMOUS_NAMESPACE.size());
    }

    // remove leading and trailing whitespace
    if (auto p = sanType.find_first_not_of(' ')) sanType.erase(0,p);
    if (auto p = sanType.find_last_not_of(' '); p != std::string::npos) {
        sanType.erase(p+1);
    }

    return sanType;
}

Lang parseShowLangResult(const char* result) {
    // Examples:
    //   The current source language is "auto; currently c++".
    //   The current source language is "fortran".

    if (!result) return Lang::Unknown;

    auto sv = std::string_view(result);
    auto npos = std::string_view::npos;

    if (sv.find("fortran") != npos) return Lang::Ftn;
    if (sv.find("c++") != npos) return Lang::Cpp;
    if (sv.find("\"c\"") != npos || sv.find("currently c\"") != npos)
        return Lang::Cpp;

    return Lang::Unknown;
}

Lang queryCurrentLang(MISession* mi) {
    if (!mi) return Lang::Unknown;

    const char* argv[] = {"show", "lang"};
    MICommand* cmd = CLIBypass(2, const_cast<char **>(argv));
    SendMICommand(mi, cmd);

    auto result = Lang::Unknown;

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


std::shared_ptr<ais::Type> getType(Mi2Aif& mi, MIVar* var, const char* inType) {
    switch (queryCurrentLang(mi.getSession())) {
        case Lang::Unknown:
            DVPRINT("WARNING: failed to detect current language type. Trying cpp.\n");
            // fall through
        case Lang::Cpp:
            return getCppType(mi, var, inType);

        case Lang::Ftn:
            return getFtnType(mi, inType ? inType : var->type);

        default:
            // getting here means that queryCurrentLang somehow returned a value outside
            // of the valid values for Lang
            return std::make_shared<ais::Unimplemented>("language-detection-error");
    }
}

std::shared_ptr<ais::Type> getCppType(Mi2Aif& mi, MIVar* var, const char* inType) {
    static bool isSorted = false;
    if (!isSorted) {
        std::sort(m_cppPrims.begin(), m_cppPrims.end(), comparePrims);
        std::sort(m_cppStlPrims.begin(), m_cppStlPrims.end(), comparePrims);
        isSorted = true;
    }

    auto isId = [](int c) { return std::isalnum(c) || c == '_'; };

    DVPRINT("getCppType var=\"%s\" inType=\"%s\"\n", 
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

    // TODO see if this can be put into cppWrap
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
    auto aliasName = mi.getPTypeAlias(type, false, false);
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
addStructMembers(Mi2Aif& mi, ais::Struct& stype,
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

        mi.getChildren(var);
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

std::shared_ptr<ais::Type> getFtnType(Mi2Aif& mi, const char* typeStr) {
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
std::shared_ptr<ais::Type> cppWrap(std::shared_ptr<ais::Type> elemType, TypeName t) {
    const auto beg = t.decoration.find_first_not_of(' ');
    if (beg == std::string::npos) {
        return elemType;
    }

    DVPRINT("wrap decoration is \"%s\"\n", t.decoration.c_str());

    auto c = t.decoration[beg];
    DVPRINT("c == %c\n", c);

    if (c == '*') {
        return cppWrap(std::make_shared<ais::Pointer>(t.type), TypeName(t,beg+1));
    } else if (c == '&') {
        DVPRINT("ignoring reference");
        return cppWrap(elemType, TypeName(t,beg+1));
    } else if (c == '[') {
        // for multi-dimension arrays, the last value is changing the fastest
        auto brackets_end = beg;
        for (auto loc = brackets_end;
             (loc=t.decoration.find(']', loc+1)) != std::string::npos
                 && (brackets_end=loc)+1 != t.decoration.size()
                 && t.decoration[brackets_end+1] == '['; ) {}

        if (t.decoration[brackets_end] != ']')
            return std::make_shared<ais::Unimplemented>("unknown-c-decoration-" + t.decoration);

        auto wrapper = elemType;

        for (auto loc = brackets_end+1; loc != beg; ) {
            auto close = loc-1;
            loc = t.decoration.rfind('[', loc-1);
            int len = std::stoi(t.decoration.substr(loc+1,close-loc-1));

            wrapper.reset(new ais::Array(wrapper, len));
        }

        return cppWrap(wrapper, TypeName(t,brackets_end+1));
    } else if (std::string_view(t.decoration).substr(beg, 3) == "(&)"
            || std::string_view(t.decoration).substr(beg, 4) == "(&&)")
    {
        // reference to an array (e.g. "int (&)[4]") or reference to a
        // function (e.g. void (&)(void)). ignore the reference symbol and
        // let the next parse handle the rest
        const auto first_paren_end = t.decoration.find(')', beg);
        return cppWrap(elemType, TypeName(t,first_paren_end+1));
    } else if (std::string_view(t.decoration).substr(beg, 2) == "(*") {
        // a pointer type like int *(**)[2] or int (*)(int, int). when
        // reading the type as a human, the asterisks in the parenthesis
        // take precedence over everything else. int *(**)[3] is "a double
        // pointer to an array of 3 int pointers". the int would be
        // dereferenced like *(**array)[0].
        //
        // these precedence rules present a problem for this function, which
        // parses things strictly left to right. to handle it, we transform
        // the input type and put any asterisks in parenthesis onto the end
        // of the type and remove the parenthesis. e.g. int *(**)[2] turns
        // into int *[2]**. note that this function builds up the type from
        // the innermost type outwards, so handling the parenthesis
        // asterisks last is exactly what we want.
        //
        // gdb treats this new notation identically to the parenthesis
        // notation when printing values. "whatis int *[2]**" returns "int *(**)[2]".
        const auto paren_end = findExprMatch(t.decoration, beg);

        t.decoration.erase(beg, paren_end - beg + 1);
        // (paren_end - beg + 1) - 2
        const auto num_asterisks = paren_end - beg - 1;
        t.decoration.append(num_asterisks, '*');

        // the removal of (*...*) shifted the remaining string left, so parse again at beg
        return cppWrap(elemType, TypeName(t,beg));
    } else if (c == '(') {
        // else assume a function.
        const auto paren_end = findExprMatch(t.decoration, beg);
        return cppWrap(std::make_shared<ais::Function>(), TypeName(t,paren_end+1));
    } else if (c == '@') {
        // gpu register tag - like @local
        auto word_end = beg;
        while ( ::isalpha(t.decoration[word_end+1])) ++word_end;
        return cppWrap(elemType, TypeName(t,word_end+1));
    } else if (t.decoration.find("const", beg) == beg) {
        const auto const_end = beg + 4;
        return cppWrap(elemType, TypeName(t,const_end+1));
    } else {
        return std::make_shared<ais::Unimplemented>("unknown-decoration-" + t.decoration.substr(beg));
    }
}

std::shared_ptr<ais::Type> ftnWrap(std::shared_ptr<ais::Type> elemType,
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
getContainerTypes(TypeName t) {
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

std::shared_ptr<ais::Type> stlContainer(Mi2Aif& mi, const ais::Array& ctype, TypeName t) {
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

std::shared_ptr<ais::Type> stdArray(Mi2Aif& mi, TypeName t) {
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

std::shared_ptr<ais::Type> stdPair(Mi2Aif& mi, TypeName t) {
    DVPRINT("parsing stdPair container type %s\n", t.decoration.c_str());

    auto [first, second, decoration] = getContainerTypes(t);
    if (first.empty() || second.empty()) {
        return unimplemented(mi,t);
    }

    DVPRINT("Got types: \"%s\", \"%s\", \"%s\"\n",
            first.c_str(), second.c_str(), decoration.c_str());


    auto t2 = TypeName{t.type  + "<" + first + "," + second + ">",
                       std::move(decoration)};

    auto firstType = getType(mi, nullptr, first.c_str());
    auto secondType = getType(mi, nullptr, second.c_str());

    auto vtype = std::make_shared<ais::Pair>(first, firstType, second, secondType);
    return cppWrap(vtype, t2);
}

std::shared_ptr<ais::Type> stdTuple(Mi2Aif& mi, TypeName t) {
    DVPRINT("parsing stdTuple container type %s\n", t.decoration.c_str());

    // expecting a decoration like: <int, double, int, int>

    auto elems = std::vector<std::pair<std::string, std::shared_ptr<ais::Type>>>{};

    auto beg = size_t{1}, end = beg;
    const auto& d = t.decoration;
    for ( ; end<d.size(); ++end) {
        auto c = d[end];
        if (c == ',' || c == '>') {
            auto typeStr = d.substr(beg, end-beg);
            auto type = getType(mi, nullptr, typeStr.c_str());
            elems.push_back(std::make_pair(typeStr, type));
            if (c == '>') break;
            else if ((beg = d.find_first_not_of(" ",end+1)) != std::string::npos) end=beg;
        } else if (c == '<' || c == '(') {
            end = findExprMatch(d, end);
        }
    }

    auto t2 = TypeName{t, end+1};
    auto vtype = std::make_shared<ais::Tuple>(elems);
    return cppWrap(vtype, t2);
}


std::shared_ptr<ais::Type> stdString(Mi2Aif& mi, TypeName t) {
    DVPRINT("parsing stdString container type %s\n", t.decoration.c_str());

    auto [containedType, templateArgs, decoration] = getContainerTypes(t);

    DVPRINT("Got types: \"%s\", \"%s\", \"%s\"\n",
            containedType.c_str(), templateArgs.c_str(), decoration.c_str());

    auto t2 = TypeName{t.type  + "<" + containedType +">", std::move(decoration)};
    return cppWrap(std::make_shared<ais::String>(), t2);
}

std::shared_ptr<ais::Type> stdConditional(Mi2Aif& mi, TypeName t) {
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

std::shared_ptr<ais::Type> ftnCharacter(Mi2Aif& mi, TypeName t) {
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
    

std::shared_ptr<ais::Type> smartPointer(Mi2Aif& mi, TypeName t) {
    DVPRINT("parsing smartPointer type %s\n", t.decoration.c_str());

    auto [containedType, templateArgs, decoration] = getContainerTypes(t);

    DVPRINT("Got types: \"%s\", \"%s\", \"%s\"\n",
            containedType.c_str(), templateArgs.c_str(), decoration.c_str());

    auto t2 = TypeName{t.type  + "<" + containedType +">", std::move(decoration)};
    return cppWrap(std::make_shared<ais::SmartPointer>(t.type, containedType), t2);
}


std::shared_ptr<ais::Type> charWrap(std::shared_ptr<ais::Type> elemType,
                                                TypeName t) {

    DVPRINT("in charWrap \"%s\" + \"%s\"\n", t.type.c_str(), t.decoration.c_str());

    auto beg = t.decoration.find_first_not_of(' ');
    if (beg == std::string::npos) {
        return elemType;
    } else if (t.decoration[beg] == '*') {
        return cppWrap(std::make_shared<ais::CharArray>(), TypeName{t, beg+1});
    } else if (t.decoration[beg] == '&') {
        // ignore reference
        return charWrap(elemType, TypeName{t, beg+1});
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
    } else if (std::string_view(t.decoration).substr(beg, 3) == "(&)"
            || std::string_view(t.decoration).substr(beg, 4) == "(&&)")
    {
        // reference to an array (e.g. char (&)[4]). ignore the reference symbol
        // and let the next parse handle array size
        auto end = t.decoration.find(')', beg);
        return charWrap(elemType, TypeName{t, end + 1});
    } else if (std::string_view(t.decoration).substr(beg, 2) == "(*") {
        // see explanation of (* case in cppWrap
        return cppWrap(elemType, t);
    } else if (t.decoration[beg] == '(') {
        // assume function that returns char
        return cppWrap(elemType, t);
    } else {
        return std::make_shared<ais::Unimplemented>(t.type + t.decoration);
    }
}

std::shared_ptr<ais::Type> wrapVoidFunc(Mi2Aif& mi, TypeName t) {
    auto end = t.decoration.find_last_not_of(' ');
    if (end == std::string::npos) {
        return std::make_shared<ais::Literal>();
    }

    if (t.decoration[end] != ')') {
        return unimplemented(mi, t);
    }

    return std::make_shared<ais::Function>();
}

std::shared_ptr<ais::Type> wrapEnum(Mi2Aif& mi, TypeName t) {
    DVPRINT("enum decoration is \"%s\"\n", t.decoration.c_str());

    auto namePos = t.decoration.find_first_not_of(' ');
    if (namePos == std::string::npos) {
        return unimplemented(mi, t);
    }

    auto name = t.decoration.substr(namePos);
    return std::make_shared<ais::Enum>(name);
}

std::shared_ptr<ais::Type> wrapUnion(Mi2Aif& mi, TypeName t) {
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

std::shared_ptr<ais::Type> kokkosView(Mi2Aif& mi, TypeName t) {
    DVPRINT("parsing kokkosView type %s\n", t.decoration.c_str());

    auto [containedType, templateArgs, decoration] = getContainerTypes(t);
    if (containedType.empty()) {
        return unimplemented(mi,t);
    }

    DVPRINT("Got types: \"%s\", \"%s\", \"%s\"\n",
            containedType.c_str(), templateArgs.c_str(), decoration.c_str());

    int dim = std::count_if(containedType.begin(), containedType.end(),
                            [](auto c) { return c == '*' || c == '['; });

    auto etypeEnd = containedType.find_first_of("*[");
    auto etypeStr = containedType.substr(0,etypeEnd);

    auto elemType = getType(mi, nullptr, etypeStr.c_str());

    return std::make_shared<ais::KokkosView>(elemType, dim);
}

std::shared_ptr<ais::Type> rajaView(Mi2Aif& mi, TypeName t) {
    DVPRINT("parsing RAJA::view type %s\n", t.decoration.c_str());

    // All internal RAJA::View types will start like an stl container type with
    // their contained type as their first argument. e.g.
    // RAJA::internal::ViewBase<int, ...>
    // RAJA::internal::TypedViewBase<int, ...>

    auto [elem_type_str, template_args, decoration] = getContainerTypes(t);

    DVPRINT("Got types: \"%s\", \"%s\", \"%s\"\n",
        elem_type_str.c_str(), template_args.c_str(), decoration.c_str());
    if (elem_type_str.empty()) {
        return unimplemented(mi, t);
    }

    // get the element type
    auto elem_type = getType(mi, nullptr, elem_type_str.c_str());

    // Now get the dimension.
    // We expect this function to be called with some kind of internal RAJA::View type like:
    //   RAJA::internal::ViewBase<double, double*, RAJA::detail::LayoutBase_impl<camp::int_seq<long, 0l, 1l>, int, -1l> >
    //
    // The internal RAJA view types are all templatized in a pattern like:
    //   RAJA::internal::ViewBase<ValueType, PointerType, LayoutType>
    //
    // The LayoutType is where we can look to figure out the dimensions of the view. They
    // all look something like:
    //   RAJA::internal::LayoutTypeName<IndexType, RangeInts, Sizes, Strides, void>
    // Where RangeInts, Sizes, and Strides are camp::int_seq<IndexType, ...>. All of the
    // integer sequences will have the same length, and the length is equal to dimension + 1.
    // Since the camp::int_seq has the extra IdxLin term in the front, we can simply count
    // the number of commas in the int_seq's template arguments to get the number of dimensions.
    // Note that in some types the IndexType is not explicitly present and the template
    // arguments start with camp::int_seq's.
    //
    // Some fully expanded examples:
    //
    // a 2d view of doubles created via RAJA::View<double, RAJA::Layout<2, int>> has type:
    //   RAJA::internal::ViewBase<double, double*, RAJA::detail::LayoutBase_impl<camp::int_seq<long, 0l, 1l>, int, -1l> >
    //
    // a 3d view of doubles via RAJA::View<double, RAJA::Layout<3, int>> has type:
    //   RAJA::internal::ViewBase<int, int*, RAJA::detail::LayoutBase_impl<camp::int_seq<long, 0l, 1l, 2l>, int, -1l> >
    //
    // An offset layout is simpler. An offset layout is a wrapper around a normal layout and has its own type.
    // A 2d view of ints via RAJA::View<int, RAJA::OffsetLayout<2, int>> has type:
    //   RAJA::internal::ViewBase<int, int*, RAJA::OffsetLayout<2ul, int> >
    // with the dimension as the first argument to OffsetLayout.

    // we keep layout_token short to match both RAJA::View::LayoutBase_impl and
    // RAJA::View::StaticLayoutBase_impl
    const auto layout_token = "LayoutBase_impl";
    const auto offset_token = "RAJA::OffsetLayout";

    if (auto layout_idx = template_args.find(layout_token); layout_idx != std::string::npos) {
        auto layout_type = template_args.substr(layout_idx);

        // find camp::int_seq
        const auto int_seq_token = "camp::int_seq";
        auto int_seq_idx = layout_type.find(int_seq_token);
        if (int_seq_idx == std::string::npos)
            return unimplemented(mi, t);

        // the number of dimensions is equal to the number of commas in
        // camp::int_seq<long, 0l, ..., nl> (the first 'long' term makes dimensions ==
        // commas instead of more common/expected dimensions == commas+1)
        if (layout_type.size() < int_seq_idx + strlen(int_seq_token))
            return unimplemented(mi, t);

        // point `opening` to the '<' in camp::int_seq<...>
        const auto* opening = &layout_type[int_seq_idx + strlen(int_seq_token)];
        const auto* closing = findExprMatch(opening);
        if (closing == nullptr)
            return unimplemented(mi, t);

        auto commas = std::count(opening, closing, ',');

        return std::make_shared<ais::RajaView>(elem_type, commas);

    } else if (auto offset_idx = template_args.find(offset_token); offset_idx != std::string::npos) {
        auto offset_template = template_args.substr(offset_idx);
        auto [dims_str, rest, dec] = getContainerTypes({template_args, strlen(offset_token)});
        DVPRINT("Got inner types: \"%s\", \"%s\", \"%s\"\n", dims_str.c_str(), rest.c_str(), dec.c_str());
        if (dims_str.empty())
            return unimplemented(mi, t);

        try {
            auto dims = std::stoi(dims_str);
            return std::make_shared<ais::RajaView>(elem_type, dims);
        } catch (std::logic_error &e) {
            return unimplemented(mi, t);
        }
    }

    return unimplemented(mi, t);
}

std::shared_ptr<ais::Type> unimplemented(Mi2Aif& mi, TypeName t) {
    return std::make_unique<ais::Unimplemented>(t.type + t.decoration);
}

} // namespace TypeParser

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

    // cpp keywords
    CHECK(st("const char") == std::string("char"));
    // "signed" has an exception for chars
    CHECK(st("signed char") == std::string("signed char"));
    CHECK(st("signed int") == std::string("int"));
    CHECK(st("volatile char") == std::string("char"));

    // c/cpp arrays
    CHECK(st("char[8]") == std::string("char[8]"));
    CHECK(st("char [8]") == std::string("char [8]"));
    CHECK(st("char[8][8]") == std::string("char[8][8]"));
    CHECK(st("char [8] [8]") == std::string("char [8] [8]"));
    CHECK(st("signed int[8]") == std::string("int[8]"));
    CHECK(st("signed int [8]") == std::string("int [8]"));
    CHECK(st("signed int[8][8]") == std::string("int[8][8]"));
    CHECK(st("signed int [8] [8]") == std::string("int [8] [8]"));

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
        basicTypeTest<ais::Pointer>(TypeParser::Lang::Cpp, "int (*) (int, int, int)");
        basicTypeTest<ais::Pointer>(TypeParser::Lang::Cpp, "int (*) ()");
        basicTypeTest<ais::Pointer>(TypeParser::Lang::Cpp, "float (*) (int, int, int)");
        basicTypeTest<ais::Pointer>(TypeParser::Lang::Cpp, "float (*) ()");
        basicTypeTest<ais::Pointer>(TypeParser::Lang::Cpp, "char (*) (int, int, int)");
        basicTypeTest<ais::Pointer>(TypeParser::Lang::Cpp, "char (*) ()");

        // void pointers are explicitly not followed by swizzlePointer, so they are
        // directly resolved as functions (not pointers to functions) at the type parser level
        basicTypeTest<ais::Function>(TypeParser::Lang::Cpp, "void (*) (int, int)");
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

TEST_CASE("TypeParser::getType: RAJA::View") {
    SUBCASE("Basic 2D view") {
        // 2d view of doubles
        auto t = basicTypeTest<ais::RajaView>(TypeParser::Lang::Cpp,
            "RAJA::internal::ViewBase<double, double*, RAJA::detail::LayoutBase_impl<camp::int_seq<long, 0l, 1l>, int, -1l> >"
        );

        auto raja_view = std::dynamic_pointer_cast<ais::RajaView>(t);
        REQUIRE(raja_view != nullptr);
        CHECK(raja_view->dim() == 2);

        auto inner = std::dynamic_pointer_cast<ais::Double>(raja_view->getElemType());
        CHECK_MESSAGE(inner != nullptr, "Incorrect inner type: ", raja_view->getElemType()->getTypeName());
    }

    SUBCASE("Basic 3D view") {
        // 3d view of ints
        auto t = basicTypeTest<ais::RajaView>(TypeParser::Lang::Cpp,
            "RAJA::internal::ViewBase<int, int*, RAJA::detail::LayoutBase_impl<camp::int_seq<long, 0l, 1l, 2l>, int, -1l> >"
        );

        auto raja_view = std::dynamic_pointer_cast<ais::RajaView>(t);
        REQUIRE(raja_view != nullptr);
        CHECK(raja_view->dim() == 3);

        auto inner = std::dynamic_pointer_cast<ais::Int>(raja_view->getElemType());
        CHECK_MESSAGE(inner != nullptr, "Incorrect inner type: ", raja_view->getElemType()->getTypeName());
    }

    SUBCASE("Offset view") {
        // 2d offset view of ints
        auto t = basicTypeTest<ais::RajaView>(TypeParser::Lang::Cpp,
            "RAJA::internal::ViewBase<int, int*, RAJA::OffsetLayout<2ul, int> >"
        );

        auto raja_view = std::dynamic_pointer_cast<ais::RajaView>(t);
        REQUIRE(raja_view != nullptr);
        CHECK(raja_view->dim() == 2);

        auto inner = std::dynamic_pointer_cast<ais::Int>(raja_view->getElemType());
        CHECK_MESSAGE(inner != nullptr, "Incorrect inner type: ", raja_view->getElemType()->getTypeName());
    }

    SUBCASE("Static layout view") {
        // 2d static layout view of floats
        auto t = basicTypeTest<ais::RajaView>(TypeParser::Lang::Cpp,
            "RAJA::internal::TypedViewBase<float, float*, RAJA::detail::StaticLayoutBase_impl<long, camp::int_seq<long, 0l, 1l>, camp::int_seq<long, 16l, 16l>, camp::int_seq<long, 16l, 1l>, void>, camp::list<long, long> >"
        );

        auto raja_view = std::dynamic_pointer_cast<ais::RajaView>(t);
        REQUIRE(raja_view != nullptr);
        CHECK(raja_view->dim() == 2);

        auto inner = std::dynamic_pointer_cast<ais::Float>(raja_view->getElemType());
        CHECK_MESSAGE(inner != nullptr, "Incorrect inner type: ", raja_view->getElemType()->getTypeName());
    }
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
            SUBCASE("chunk") {
                auto t = basicTypeTest<ais::FtnArray>(TypeParser::Lang::Ftn, "INTEGER*4, (0:9)");
                t = containerMatch<ais::FtnArray, ais::Int>(t, 10);
            }
            SUBCASE("2d chunk") {
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
    CHECK(TypeParser::parseShowLangResult("The current source language is \"auto; currently fortran\".") == TypeParser::Lang::Ftn);
    CHECK(TypeParser::parseShowLangResult("The current source language is \"auto; currently c\".") == TypeParser::Lang::Cpp);
    CHECK(TypeParser::parseShowLangResult("The current source language is \"auto; currently c++\".") == TypeParser::Lang::Cpp);
    CHECK(TypeParser::parseShowLangResult("The current source language is \"auto; currently COBOL\".") == TypeParser::Lang::Unknown);

    CHECK(TypeParser::parseShowLangResult("The current source language is \"fortran\".") == TypeParser::Lang::Ftn);
    CHECK(TypeParser::parseShowLangResult("The current source language is \"c\".") == TypeParser::Lang::Cpp);
    CHECK(TypeParser::parseShowLangResult("The current source language is \"c++\".") == TypeParser::Lang::Cpp);
    CHECK(TypeParser::parseShowLangResult("The current source language is \"COBOL\".") == TypeParser::Lang::Unknown);

    CHECK(TypeParser::parseShowLangResult("junk") == TypeParser::Lang::Unknown);
    CHECK(TypeParser::parseShowLangResult("") == TypeParser::Lang::Unknown);
    CHECK(TypeParser::parseShowLangResult(nullptr) == TypeParser::Lang::Unknown);
}

} // namespace unittest
