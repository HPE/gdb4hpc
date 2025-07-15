// Copyright 2023-2025 Hewlett Packard Enterprise Development LP.

#include "ais.h"
#include <string>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
#include <stdexcept>
#include <future>
#include <mutex>
#include <regex>
#include <cstring>
#include <condition_variable>
#include <iomanip>
#include <variant>
#include <charconv>
#include <cuchar>
#include "dbgevent.h"
#include "parse.h"

#include "unittest_macros.h"

//===========================================================================

// duplicating function from str to make ais "detachable"
static const char* skipspace(const char* s) {
    while (*s && isspace(*s)) ++s;
    return s;
}

static std::unique_ptr<ais::Writer> makeNullWriter();

void ais::Type::printIndex(int, std::ostream&) const {}

int ais::Type::getTypeId(ais::Writer& writer) const
{
    return writer.getTypeId(getTypeName());
}

void ais::Type::parse(InterpretStream& in, Writer& out) const {
    auto s = skipspace(in.get());
    if (*s != '<')
        return parseL(in,out);

    if (auto end = strchr(s+1, '>')) {
        auto error = std::string{s, size_t(end-s+1)};
        parseError(error, out);
        in.setPos(end+1);
    } else {
        return parseL(in,out);
    }
}

void ais::Type::parseError(const std::string& error, Writer& out) const {
    out.writeError(error);
}

void ais::Type::apply(Reader& reader, Visitor& f) const {
    try {
        applyL(reader, f);
    } catch (GdbError& err) {
        f(err);
    }
}

bool ais::Type::resetTruncation() const { return true; }
    
ais::Type::~Type() {}

void ais::Bool::parseL(InterpretStream& in, Writer& out) const
{
    const char* s = skipspace(in.get());

    bool val = false;
    if (strncmp(s, "true", 4) == 0) {
        val = true;
        s += 4;
    } else if (strncmp(s, "false", 5) == 0) {
        val = false;
        s += 5;
    } else {
        throw ais::Error("unrecognized bool value");
    }
    in.setPos(s);
    out.writeInt(val);
}

void ais::Bool::applyL(Reader& reader, Visitor& f) const {
    auto v = readVal(reader);
    f(v);
}

bool ais::Bool::readVal(ais::Reader& reader) const {
    return reader.readInt();
}

std::string ais::Bool::getTypeName() const {
    return "bool";
}

void ais::Char::parseL(InterpretStream& in, Writer& out) const
{
    const char* str = in.get();

    // these are print as integer value potentially followed by a 'char'
    char* next = nullptr;
    char val = strtol(str, &next, 10);
    bool gotVal = (next != str);

    // parse a single character surrounded by single quotes

    next = const_cast<char*>(skipspace(next));

    if (*next == '\'') {
        // get the value from the character, use std::quoted which
        // understands escape sequences
        auto is = std::istringstream{next};
        auto s = std::string{};
        is >> std::quoted(s,'\'');
        if (!gotVal) val = (s.size() ? int{s[0]} : 0);

        for (char c; (c=*++next) && c != '\''; ) {
            if (c == '\\' && next[1]) ++next;
        }
        if (!*next) throw ais::Error("no end quote in character constant");
        
        in.setPos(next+1);
    } else if (gotVal) {
        // it just printed in integer
        in.setPos(next);
    } else {
        throw ais::Error("expected value after a character");
    }
    
    out.writeInt(val);
}

namespace {
    // Helper to print out the human readable form of a character value
    // (which might or might not be printable, and be in use as a tiny
    // int instead of an ASCII value)
    struct CharLiteral {
        CharLiteral(int x) : val(x) {}
        unsigned char val;
    };
    std::ostream& operator<<(std::ostream& os, CharLiteral x) {
        if (isprint(x.val)) {
            return os << "'" << x.val << "'";
        } else {
            char hex[10];
            sprintf(hex, "%02X", static_cast<unsigned short>(x.val));
            return os << "'\\x" << hex << "'";
        }
    }
}

std::unique_ptr<ais::Printer> ais::Char::makePrinter(Printer& parent) const {
    struct TPrinter : public Printer {
        TPrinter(Printer& parent) : Printer(parent) {}
        using Printer::operator();
        void operator()(char val) override { out << format() << static_cast<int>(val) << " " << CharLiteral{val}; }
    };
    return std::make_unique<TPrinter>(parent);
}

void ais::Char::applyL(Reader& reader, Visitor& f) const {
    auto v = readVal(reader);
    f(v);
}

char ais::Char::readVal(ais::Reader& reader) const {
    return reader.readInt();
}

std::string ais::Char::getTypeName() const {
    return "char";
}

std::unique_ptr<ais::Printer> ais::SignedChar::makePrinter(Printer& parent) const {
    struct TPrinter : public Printer {
        TPrinter(Printer& parent) : Printer(parent) {}
        using Printer::operator();
        void operator()(char val) override {
            out << format() << static_cast<int>(static_cast<signed char>(val))
                << " " << CharLiteral(val); }
    };
    return std::make_unique<TPrinter>(parent);
}

std::string ais::SignedChar::getTypeName() const {
    return "signed char";
}

std::unique_ptr<ais::Printer> ais::UnsignedChar::makePrinter(Printer& parent) const {
    struct TPrinter : public Printer {
        TPrinter(Printer& parent) : Printer(parent) {}
        using Printer::operator();
        void operator()(char val) override {
            auto v = static_cast<unsigned int>(static_cast<unsigned char>(val));
            out << format() << v << " " << CharLiteral(val);
        }
    };
    return std::make_unique<TPrinter>(parent);
}

std::string ais::UnsignedChar::getTypeName() const {
    return "unsigned char";
}

void ais::Short::parseL(InterpretStream& in, Writer& out) const
{
    const char* str = in.get();
    char* next = nullptr;
    auto val = strtol(str, &next, 10);
    in.setPos(next);

    if (str == next) {
        out.writeError("could not evaluate");
    } else {
        out.writeInt(val);
    }
}

void ais::Short::applyL(Reader& reader, Visitor& f) const {
    auto v = readVal(reader);
    f(v);
}

short ais::Short::readVal(ais::Reader& reader) const {
    return reader.readInt();
}

std::string ais::Short::getTypeName() const {
    return "short";
}

std::unique_ptr<ais::Printer>
ais::UnsignedShort::makePrinter(ais::Printer& parent) const {
    struct TPrinter : public Printer {
        TPrinter(Printer& parent) : Printer(parent) {}
        using Printer::operator();
        void operator()(int val) override { out << format() << static_cast<unsigned short>(val); }
    };
    return std::make_unique<TPrinter>(parent);
}

std::string ais::UnsignedShort::getTypeName() const {
    return "unsigned short";
}

void ais::Utf32Char::parseL(InterpretStream& in, Writer& out) const
{
    const char* str = in.get();

    // these are print as integer value potentially followed by a 'char'
    char* next = nullptr;
    int val = strtol(str, &next, 10);
    bool gotVal = (next != str);

    // parse a single character surrounded by single quotes
    next = const_cast<char*>(skipspace(next));
    
    if (*next == '\'') {
        // matches the char on next.
        const char* end = findExprMatch(next);
        if (!*end) throw ais::Error("no end quote in character constant");
        in.setPos(end+1);
    } else if (gotVal) {
        // it just printed in integer
        in.setPos(next);
    } else {
        throw ais::Error("expected value after a character");
    }
    
    out.writeInt(val);
}

// this function will be included in RustString when that is created.
std::string to_utf8(char32_t codepoint)
{
    // MB_LEN_MAX is the maximum number of bytes needed to represent a single wide character in any supported locale.
    char arr[MB_LEN_MAX+1] = {0};
    
    // query the current locale
    const char *prev_loc = std::setlocale(LC_ALL, nullptr);
    
    std::setlocale(LC_ALL, "en_US.utf8");
    std::mbstate_t state{};
    std::c32rtomb(arr, codepoint, &state);
    std::setlocale(LC_ALL, prev_loc);
    return std::string(arr);
}

std::unique_ptr<ais::Printer> ais::Utf32Char::makePrinter(Printer& parent) const {
    struct TPrinter : public Printer {
        TPrinter(Printer& parent) : Printer(parent) {}
        using Printer::operator();
        void operator()(int val) { 
            if (val > 256) {
                out << std::dec << val << " " << "\'\\u{" << std::hex << std::setw(6) << std::setfill('0') << static_cast<int>(val) << "}\'"; 
            }
            else {
                char c = static_cast<char>(val);
                std::string escaped;
                switch (c) {
                    case '\'': escaped = "\\'"; break;
                    case '\"': escaped = "\\\""; break;
                    case '\\': escaped = "\\\\"; break;
                    case '\a': escaped = "\\a"; break;
                    case '\b': escaped = "\\b"; break;
                    case '\f': escaped = "\\f"; break;
                    case '\n': escaped = "\\n"; break;
                    case '\r': escaped = "\\r"; break;
                    case '\t': escaped = "\\t"; break;
                    case '\v': escaped = "\\v"; break;
                    case '\0': escaped = "\\0"; break;
                    default: escaped = std::string(1, c); break;
                }
                out << std::dec << val << " '" << escaped << "'"; 
            }
        }
    };
    return std::make_unique<TPrinter>(parent);
}

std::string ais::Utf32Char::getTypeName() const {
    return "Utf32Char";
}

void ais::Int::parseL(InterpretStream& in, Writer& out) const
{
    const char* str = in.get();

    char* next = nullptr;
    int val = strtol(str, &next, 10);
    in.setPos(next);

    if (next == str) {
        out.writeError("could not evaluate");
    } else {
        out.writeInt(val);
    }
}

void ais::Int::applyL(Reader& reader, Visitor& f) const {
    auto v = readVal(reader);
    f(v);
}

int ais::Int::readVal(ais::Reader& reader) const {
    return reader.readInt();
}

std::string ais::Int::getTypeName() const {
    return "int";
}

std::unique_ptr<ais::Printer> ais::UnsignedInt::makePrinter(Printer& parent) const {
    struct TPrinter : public Printer {
        TPrinter(Printer& parent) : Printer(parent) {}
        using Printer::operator();
        void operator()(int val) { out << format() << static_cast<unsigned int>(val); }
    };
    return std::make_unique<TPrinter>(parent);
}

std::string ais::UnsignedInt::getTypeName() const {
    return "unsigned int";
}

void ais::Long::parseL(InterpretStream& in, Writer& out) const
{
    const char* str = in.get();

    char* next = nullptr;
    long val = strtol(str, &next, 10);
    in.setPos(next);

    if (next == str) {
        out.writeError("could not evaluate");
    } else {
        out.writeLong(val);
    }
}


void ais::Long::applyL(Reader& reader, Visitor& f) const {
    auto v = readVal(reader);
    f(v);
}

long ais::Long::readVal(ais::Reader& reader) const {
    return reader.readLong();
}

std::string ais::Long::getTypeName() const {
    return "long";
}

std::unique_ptr<ais::Printer> ais::UnsignedLong::makePrinter(Printer& parent) const {
    struct  TPrinter : public Printer {
        TPrinter(Printer& parent) : Printer(parent) {}
        using Printer::operator();
        void operator()(long val) override { out << format() << static_cast<unsigned long>(val); }
    };
    return std::make_unique<TPrinter>(parent);
}

std::string ais::UnsignedLong::getTypeName() const {
    return "unsigned long";
}

void ais::LongLong::parseL(InterpretStream& in, Writer& out) const
{
    const char* str = in.get();

    char* next = nullptr;
    long long val = strtoll(str, &next, 10);
    in.setPos(next);

    if (next == str) {
        out.writeError("could not evaluate");
    } else {
        out.writeLongLong(val);
    }
}

void ais::LongLong::applyL(Reader& reader, Visitor& f) const {
    auto v = readVal(reader);
    f(v);
}

long long ais::LongLong::readVal(ais::Reader& reader) const {
    return reader.readLongLong();
}

std::string ais::LongLong::getTypeName() const {
    return "long long";
}

std::unique_ptr<ais::Printer> ais::UnsignedLongLong::makePrinter(Printer& parent) const {
    struct TPrinter : public Printer {
        TPrinter(Printer& parent) : Printer(parent) {}
        using Printer::operator();
        void operator()(long long val) override { out << format() << static_cast<unsigned long long>(val); }
    };
    return std::make_unique<TPrinter>(parent);
}

std::string ais::UnsignedLongLong::getTypeName() const {
    return "unsigned long long";
}

void ais::Double::parseL(InterpretStream& in, Writer& out) const
{
    const char* str = in.get();

    char* next = nullptr;
    auto val = strtod(str, &next);
    in.setPos(next);

    if (next == str) {
        out.writeError("could not evaluate");
    } else {
        out.writeDouble(val);
    }
}

void ais::Double::applyL(Reader& reader, Visitor& f) const {
    auto v = readVal(reader);
    f(v);
}


double ais::Double::readVal(ais::Reader& reader) const {
    return reader.readDouble();
}

std::string ais::Double::getTypeName() const {
    return "double";
}

void ais::Float::parseL(InterpretStream& in, Writer& out) const
{
    const char* str = in.get();

    char* next = nullptr;
    auto val = strtof(str, &next);
    in.setPos(next);

    if (next == str) {
        out.writeError("could not evaluate");
    } else {
        out.writeFloat(val);
    }
}

void ais::Float::applyL(Reader& reader, Visitor& f) const {
    auto v = readVal(reader);
    f(v);
}

float ais::Float::readVal(ais::Reader& reader) const {
    return reader.readFloat();
}

std::string ais::Float::getTypeName() const {
    return "float";
}

std::unique_ptr<ais::Printer> ais::Float16::makePrinter(Printer& parent) const {
    struct TPrinter : public Printer {
        TPrinter(Printer& parent) : Printer(parent) {}
        using Printer::operator();
        void operator()(float val) override { out << std::setprecision(4) << val; }
    };
    return std::make_unique<TPrinter>(parent);
}

std::string ais::Float16::getTypeName() const {
    return "float16";
}

std::unique_ptr<ais::Printer> ais::BFloat16::makePrinter(Printer& parent) const {
    struct TPrinter : public Printer {
        TPrinter(Printer& parent) : Printer(parent) {}
        using Printer::operator();
        void operator()(float val) override { out << std::setprecision(3) << val; }
    };
    return std::make_unique<TPrinter>(parent);
}

std::string ais::BFloat16::getTypeName() const {
    return "bfloat16";
}

void ais::LongDouble::parseL(InterpretStream& in, Writer& out) const
{
    const char* str = in.get();
    char* next = nullptr;
    auto val = strtold(str, &next);
    in.setPos(next);

    if (next == str) {
        out.writeError("could not evaluate");
    } else {
        out.writeLongDouble(val);
    }
}

void ais::LongDouble::applyL(Reader& reader, Visitor& f) const {
    auto v = readVal(reader);
    f(v);
}

long double ais::LongDouble::readVal(ais::Reader& reader) const {
    return reader.readLongDouble();
}

std::string ais::LongDouble::getTypeName() const {
    return "long double";
}

ais::Complex::Complex(std::shared_ptr<ais::Type> elem) : m_elemType(elem) {}

void ais::Complex::parseL(InterpretStream& in, Writer& out) const
{
    const char* str = skipspace(in.get());
    if (*str != '(') throw ais::Error("expected paren for complex");
    in.setPos(++str);

    m_elemType->parse(in, out);

    str = skipspace(in.get());
    if (*str != ',') throw ais::Error("expected comma in complex");

    in.setPos(++str);
    m_elemType->parse(in, out);

    str = skipspace(in.get());
    if (*str != ')') throw ais::Error("expected close paren in complex");
    in.setPos(++str);
}

std::unique_ptr<ais::Printer> ais::Complex::makePrinter(Printer& parent) const {
    struct TPrinter : public Printer {
        TPrinter(Printer& parent) : Printer(parent) { }
        void start(const Type* t) override { Printer::start(t); out << "("; }
        void startChild() override { if (m_idx++) out << ","; }
        void end() override { out << ")"; m_idx = 0; Printer::end(); }
        int m_idx = 0;
    };
    return std::make_unique<TPrinter>(parent);
}

void ais::Complex::printIndex(int idx, std::ostream& out) const {
    out << (idx ? "i" : "r");
}

void ais::Complex::applyL(Reader& reader, Visitor& f) const {
    f.nest(m_elemType.get());
    f.start(m_elemType.get());
    m_elemType->apply(reader, f);
    f.end();

    f.start(m_elemType.get());
    m_elemType->apply(reader, f);
    f.end();
    f.unnest();
}

std::string ais::Complex::getTypeName() const {
    return "^complex-" + m_elemType->getTypeName();
}

const ais::Type* ais::Complex::getElemType() const {
    return m_elemType.get();
}

void ais::Logical::parseL(InterpretStream& in, Writer& out) const
{
    const char* s = in.get();

    while (s[0] && isspace(s[0])) ++s;
    bool val = false;
    if (strncmp(s, ".TRUE.", 6) == 0) {
        val = true;
        s += 6;
    } else if (strncmp(s, ".FALSE.", 7) == 0) {
        val = false;
        s += 7;
    } else {
        throw ais::Error("unrecognized logical value");
    }
    in.setPos(s);
    out.writeInt(val);
}

std::unique_ptr<ais::Printer> ais::Logical::makePrinter(Printer& parent) const {
    struct TPrinter : public Printer {
        TPrinter(Printer& parent) : Printer(parent) { }
        using Printer::operator();
        void operator()(bool val) override { out << (val ? ".TRUE." : ".FALSE."); }
    };
    return std::make_unique<TPrinter>(parent);
}

void ais::Logical::applyL(Reader& reader, Visitor& f) const {
    f(readVal(reader));
}


bool ais::Logical::readVal(ais::Reader& reader) const {
    return reader.readInt() != 0;
}

std::string ais::Logical::getTypeName() const {
    return "^logical";
}


void ais::String::parseL(InterpretStream& in, Writer& out) const {
    const char* str = skipspace(in.get());
    if (str[0] == '{') {
        // gdb has printed the contents of the templated object which has the
        // actual string contents in there somewhere.
        int brace = 1;
        const char* beg{}, *end{};
        const char* s = str+1;
        for ( ; *s && !beg; ++s) {
            if (*s == '"') beg = s+1;
            else if (*s == '{') ++brace;
            else if (*s == '}') --brace;
        }
        if (beg) end = findExprMatch(beg-1);
        if (end) {
            for (s=end+1; *s && brace > 0; ++s) {
                if (*s == '{') ++brace;
                else if (*s == '}') --brace;
            }
        }
        
        if (beg && end) {
            out.writeString(std::string{beg, size_t(end-beg)});
        }
        in.setPos(s);
        return;
    } else if (str[0] == '"') {
        auto end = findExprMatch(str);
        out.writeString(std::string{str+1, end});
        in.setPos(end+1);
        return;
    }
             
    out.writeString(std::string{"unrecognized-string-result:"} + in.get());
}

void ais::String::applyL(Reader& reader, Visitor& f) const {
    f(readVal(reader));
}

std::string ais::String::getTypeName() const {
    return "std::string";
}

std::string ais::String::readVal(Reader& reader) const {
    return reader.readString();
}

void ais::FtnString::parseL(InterpretStream& in, Writer& out) const {
    const char* str = skipspace(in.get());
    const char* strEnd{};
    if (str[0] == '\'' && (strEnd=strchr(str+1,'\''))) {
        out.writeString(std::string{str+1, size_t(strEnd-str-1)});
        in.setPos(strEnd+1);
    } else {
        out.writeString(std::string{"unrecognized-fortran-string-result:"} + in.get());
    }
}

void ais::FtnString::applyL(Reader& reader, Visitor& f) const {
    String::applyL(reader, f);
}

std::unique_ptr<ais::Printer> ais::FtnString::makePrinter(Printer& parent) const {
    struct TPrinter : public Printer {
        TPrinter(Printer& parent) : Printer(parent) {}
        using Printer::operator();
        // Literal doesn't quote the result
        void operator()(const std::string& val) override { out << '\'' << val << '\''; }
    };
    return std::make_unique<TPrinter>(parent);
}


std::string ais::FtnString::getTypeName() const {
    return "^FtnString";
}

void ais::Literal::parseL(InterpretStream& in, Writer& out) const {
    const char* str = skipspace(in.get());
    if (str[0] == '{' || str[0] == '"') return ais::String::parseL(in,out);

    auto end = str;
    while (*end && !isspace(*end)) ++end;
    out.writeString(std::string{str, size_t(end-str)});
}

std::string ais::Literal::getTypeName() const {
    return "^literal";
}

std::unique_ptr<ais::Printer> ais::Literal::makePrinter(Printer& parent) const {
    struct TPrinter : public Printer {
        TPrinter(Printer& parent) : Printer(parent) {}
        using Printer::operator();
        // Literal doesn't quote the result
        void operator()(const std::string& val) override { out << val; }
    };
    return std::make_unique<TPrinter>(parent);
}

ais::Enum::Enum() {}
ais::Enum::Enum(const std::string& name) : m_name(name) {}

void ais::Enum::parseL(InterpretStream& in, Writer& out) const
{
    const char* s = skipspace(in.get());
    const char* end{};
    for (end=s; *end && (isalnum(*end) || *end == '_' || *end == ':'); ++end) {}
    out.writeString(std::string{s, end});
    in.setPos(end);
}

std::string ais::Enum::getTypeName() const { return "enum"; };

std::unique_ptr<ais::Printer>
ais::MultiThreadResult::makePrinter(Printer& parent) const {
    struct TPrinter : public Printer {
        TPrinter(Printer& parent) : Printer{parent}, m_label{parent.getLabel()} {}
        using Printer::operator();
        void operator()(const std::string& val) override {
            for (ThreadMsgParser parser{val}; auto res = parser.nextResult();) {
                auto [thread, val] = *res;

                // translate from gdb form to procset form
                for (size_t i = 0; (i = thread.find('-', i)) != std::string::npos; ++i)
                    thread.replace(i, 1, "..");
                for (size_t i = 0; (i = thread.find(' ', i)) != std::string::npos; ++i)
                    thread.replace(i, 1, ",");

                if (!thread.empty()) thread.insert(0,":");
                auto tlabel = m_label;
                if (auto p = tlabel.find_last_of('}'); p != std::string::npos) {
                    tlabel.insert(p, thread);
                }

                auto is = std::istringstream{std::move(val)};
                auto line = std::string{};
                while (std::getline(is, line)) {
                    out << tlabel << line << "\n";
                }
            }
        }
        std::string m_label;
    };
    return std::make_unique<TPrinter>(parent);
}

void ais::MultiThreadResult::applyL(Reader& reader, Visitor& f) const {
    String::applyL(reader, f);
}


std::string ais::MultiThreadResult::getTypeName() const {
    return "^MultiThreadResult";
}

ais::Function::Function() {}

void ais::Function::parseL(InterpretStream& in, Writer& out) const {
    // expecting: {type signature} pointer <name> or
    // pointer <name> or 0x0
    const char* s = skipspace(in.get());

    if (strncmp(s, "0x0", 3) == 0 && !isxdigit(s[3])) {
        out.writeString("0x0");
        in.setPos(s+3);
        return;
    }

    std::ostringstream ss;
    static const char* toggles = "{}<K>k"; // <K and >k keep the delim
    static const char* toggles2 = "<k>k";
    bool keep = false;

    for (const char* tp = (*s == '0' ? toggles2 : toggles); *s && *tp; ++s) {
        if (*s == *tp) {
            if (*++tp == 'K') {
                ss << " " << *s;
                ++tp;
            } else if (*tp == 'k') {
                ss << *s;
                ++tp;
            }
            keep = !keep;
        } else if (keep) {
            ss << *s;
        }
    }

    out.writeString(ss.str());
    in.setPos(s);
}

std::string ais::Function::getTypeName() const {
    return "^function";
}

//===========================================================================

ais::Any::Any() : m_idx(0) {}
ais::Any::Any(std::shared_ptr<Type> obj, int idx) : m_obj(obj), m_idx(idx) {}

void ais::Any::parseL(InterpretStream& in, ais::Writer& out) const {
    out.writeType(*this);
    out.writeInt(m_idx);
    m_obj->parse(in, out);
}

void ais::Any::applyL(Reader& reader, Visitor& f) const {
    const auto type = reader.readType();
    int refIdx = reader.readInt();
    f.nest(type.get(), refIdx);
    f.start(type.get());
    type->apply(reader, f);
    f.end();
    f.unnest();
}

std::string ais::Any::getTypeName() const {
    return "!"; 
}

int ais::Any::getTypeId(ais::Writer& writer) const {
    return m_obj->getTypeId(writer);
}

ais::Reader::~Reader() {}

// Every primitive must be included in this list.  This is the master
// list that is checked first when new types are read.   The protocol
// does not use fixed indices for these types, they are simply assigned
// an index as they are encountered.
// Note that on the reader side, Type objects are never modified, so
// these shared_ptr's can be passed around freely.
std::vector<std::shared_ptr<ais::Type>> ais::Reader::m_primitives {
    std::make_shared<ais::Bool>(),
    std::make_shared<ais::Utf32Char>(),
    std::make_shared<ais::Char>(),
    std::make_shared<ais::CharArray>(),
    std::make_shared<ais::SignedChar>(),
    std::make_shared<ais::UnsignedChar>(),
    std::make_shared<ais::Short>(),
    std::make_shared<ais::UnsignedShort>(),
    std::make_shared<ais::Int>(),
    std::make_shared<ais::UnsignedInt>(),
    std::make_shared<ais::Long>(),
    std::make_shared<ais::UnsignedLong>(),
    std::make_shared<ais::LongLong>(),
    std::make_shared<ais::UnsignedLongLong>(),
    std::make_shared<ais::Float>(),
    std::make_shared<ais::Float16>(),
    std::make_shared<ais::BFloat16>(),
    std::make_shared<ais::Double>(),
    std::make_shared<ais::LongDouble>(),
    std::make_shared<ais::Pointer>(""),
    std::make_shared<ais::SmartPointer>("",""),
    std::make_shared<ais::Logical>(),
    std::make_shared<ais::String>(),
    std::make_shared<ais::FtnString>(),
    std::make_shared<ais::Literal>(),
    std::make_shared<ais::Enum>(),
    std::make_shared<ais::Function>(),
    std::make_shared<ais::Complex>(std::make_shared<ais::Float>()),
    std::make_shared<ais::Complex>(std::make_shared<ais::Double>()),
    std::make_shared<ais::Complex>(std::make_shared<ais::LongDouble>()),                     
    std::make_shared<ais::MultiThreadResult>(),
    std::make_shared<ais::KokkosView>(),
    std::make_shared<ais::RajaView>(),
    std::make_shared<ais::Map>(),
    std::make_shared<ais::UnorderedMap>(),
    std::make_shared<ais::Multimap>(),
    std::make_shared<ais::UnorderedMultimap>(),
    std::make_shared<ais::ArrayChunk>()
};

std::vector<std::shared_ptr<ais::Array>> ais::Reader::m_arrayTypes =
{
 std::make_shared<ais::Array>(),
 std::make_shared<ais::RustVector>(),
 std::make_shared<ais::RustArray>(),
 std::make_shared<ais::FtnArray>(),
 std::make_shared<ais::Vector>(),
 std::make_shared<ais::Deque>(),
 std::make_shared<ais::Stack>(),
 std::make_shared<ais::StdArray>(),
 std::make_shared<ais::PriorityQueue>(),
 std::make_shared<ais::Queue>(),
 std::make_shared<ais::List>()
};
  
//===========================================================================

ais::Unimplemented::Unimplemented(const std::string& typeName) : m_typeName(typeName) {}

void ais::Unimplemented::parseL(InterpretStream& in, Writer& out) const {
    out.writeString(std::string{"unimplemented-"} + m_typeName + "-parse" + in.get());
}


void ais::Unimplemented::applyL(Reader& reader, Visitor& f) const {
    std::cerr << "in Unimplemented::apply\n";
    f(reader.readString());
}

std::string ais::Unimplemented::getTypeName() const {
    return "^unimplemented::" + m_typeName;
}

//===========================================================================

ais::Array::Array() : m_numElems(0) {}

ais::Array::Array(std::shared_ptr<Type> elemType, int len, int idx0)
    : m_elemType(elemType), m_numElems(len), m_idx0(idx0) {}

void ais::Array::parseL(InterpretStream& in, Writer& out) const {
    const char* s = in.get();
    auto& truncater = in.truncater();
    auto mayBeTruncated = truncater.isActive();
    auto isTruncated = false;

    out.writeInt(m_numElems);
    out.writeInt(m_idx0);
    out.writeInt(mayBeTruncated);

    auto outptr = &out;
    auto locOut = std::unique_ptr<Writer>{};

    if (m_numElems<0) return;

    // assuming something like {val,val,val}
    for (int i=0; i<m_numElems; ++i) {
        s = in.get();
        if (s==NULL) break;
        for ( ;*s && isspace(*s); ++s) {}
        if (i ? (*s == ',') : (*s == '(' || *s == '{' || *s == '[')) ++s;
        else if (i && (*s == ')' || *s == '}' || *s == ']') && !*(s+1)) {
            // either an array is truncated or large array has been split into chunks,
            // ultimately this will need to trigger a send operation and a wait to restart
            // parsing
            in.setPos(s+1);

            if (mayBeTruncated) {
                out.writeInt(false); // indicate no more elements
                return;
            }
        } else {
            fprintf(stderr, "unexpected character %c\n", *s);
        }

        in.setPos(s);

        if (!isTruncated && truncater.truncate(*m_elemType)) {
            // we've hit the set print elements count
            // gdb currently can't truncate multi-d arrays in the middle
            // of a row, so we'll need continue parsing and dropping elements
            isTruncated = true;
            out.writeInt(false); // there is not a another element
            locOut = makeNullWriter();
            outptr = locOut.get();
        }

        auto eout = getOut(*outptr); // give filtered array a chance to toggle 

        if (mayBeTruncated) {
            eout.writeInt(true); // there is another element
            truncater.push(*m_elemType);  // start new counter if element is a struct
        }

        m_elemType->parse(in, eout);

        if (mayBeTruncated) truncater.pop(*m_elemType);
    }
    s = in.get();
    for ( ;*s && isspace(*s); ++s) {}
    if (*s == ')' || *s == '}' || *s == ']') ++s;
    else if (m_numElems) fprintf(stdout, "unexpected array end %c\n", *s);
    in.setPos(s);
}

void ais::RustVector::parseL(InterpretStream& in, Writer& out) const {
    const char* s = skipspace(in. get());
    if(strncmp(s, "Vec(size=", 9) != 0) {
        throw ais::Error("could not parse rust vector");
    }
    s += 9;

    char *end = nullptr;
    const_cast<RustVector*>(this)->m_numElems = strtol(s, &end, 10);

    if (end == s || *end != ')') {
        throw ais::Error("could not parse rust vector length");
    }

    s = end+1;
    if (m_numElems) {
        // expecting "= { array }"
        if (auto brace = strchr(s,'{')) s = brace;
    }

    in.setPos(s);
    Array::parseL(in, out);
}

namespace ais {
    struct ArrayPrinter : public Printer {
        ArrayPrinter(Printer& parent,
                     const std::string& aopen,
                     const std::string& asep,
                     const std::string& aclose) : Printer(parent),
                                                  open(aopen), sep(asep), close(aclose) {}
        void addMetaInfo(const Type*, const MetaInfo& info) {
            if (auto arr = std::get_if<ArrayMetaInfo>(&info)) {
                m_idx0 = arr->idx0;
                m_len = arr->len;
            }
        }
        void start(const Type* t) override { Printer::start(t); out << open; }
        void startChild() override { if (m_idx++) out << sep; }
        void end() override {
            if ( m_len > 0 && m_idx < m_len) out << "...";
            out << close; m_idx = 0; Printer::end();
        }
        int m_idx = 0;
        int m_idx0 = 0;
        int m_len = 0;
        std::string open, sep, close;
    };
}

std::unique_ptr<ais::Printer> ais::Array::makePrinter(Printer& parent) const {
    static const char* arr[3] = { "{", ",", "}" };  // open, separator, close
    static const char* one[3] = { "", "\n", "" };   // one per line
    static const char* csv[3] = { "", ",", "" };    // comma separated values
    static const char* csv2[3] = { "", "L", "" };   // csv rows - L stands for "label"
    auto& fmt = parent.format();
    const char** f;
    if (fmt.onePerLine) f = one;
    else if (!fmt.csv) f = arr;
    else if (dynamic_cast<ais::Array*>(m_elemType.get())) f = csv2;
    else f = csv;
    auto sep = (f[1][0] != 'L') ? std::string{f[1]} : "\n" + parent.getLabel();
    return std::make_unique<ArrayPrinter>(parent,f[0],sep,f[2]);
}

void ais::Array::applyL(Reader& reader, Visitor& f) const {
    // to keep Array flexible, it supports fixed size and non-fixed sized modes.
    // i.e. mayBeTruncated, which in practice is possible when 'set print elements' is
    // set.   For multi-dimension arrays the actual number of values output gets
    // super complicated, so instead we just write a 1 before every element and a
    // 0 at the end.

    // note - this header information is also read (and discarded) in applyOne, so
    // if a field is added, make sure to update there
    auto len = reader.readInt();
    auto idx0 = reader.readInt();
    auto mayBeTruncated = static_cast<bool>(reader.readInt());
    f.addMetaInfo(this, ArrayMetaInfo{idx0,len});
    f.nest(m_elemType.get());
    for (int i=0; i<len; ++i) {
        if (mayBeTruncated && !reader.readInt()) break;
        f.start(m_elemType.get());
        m_elemType->apply(reader, f);
        f.end();
    }
    f.unnest();
}

void ais::Array::printIndex(int idx, std::ostream& out) const {
    out << "[" << idx << "]";
}

std::string ais::Array::getTypeName() const {
    return m_elemType ? "[" + m_elemType->getTypeName() : "[";
}

int ais::Array::getTypeId(ais::Writer& writer) const {
    auto elemIdx = getElemTypeId(writer);
    auto name = "[" + std::to_string(elemIdx);
    return writer.getTypeId(name);
}

int ais::Array::getElemTypeId(ais::Writer& writer) const {
    return m_elemType->getTypeId(writer);
}

std::shared_ptr<ais::Array> ais::Array::makeTyped(std::shared_ptr<ais::Type> elemType) const {
    return std::make_shared<ais::Array>(elemType);
}

// Visit the next element in the sequence.  ApplyOneIdx gives the location
// within the structure, and is update on output.
// -- this form is used when processing decomposition arrays --
void ais::Array::applyOne(Reader& reader, std::vector<ApplyOneIdx>& location, int level,
                          const std::function<void(const Type&,Reader&)>& f) {
    if (int(location.size()) <= level) {
        if (!location.empty() && location.back().mayBeTruncated && !reader.readInt()) {
            throw ais::Error("array result truncated (possibly via set print elements)");
        }

        // nesting into another array
        // logic duplicated from Array::apply
        auto len = reader.readInt();
        [[maybe_unused]] auto idx0 = reader.readInt(); // unused in decomposition
        auto mayBeTruncated = static_cast<bool>(reader.readInt());

        if (len <= 0) return;
        location.push_back(ApplyOneIdx{0, len, mayBeTruncated}); 
    }
    if (auto subArray = dynamic_cast<ais::Array*>(m_elemType.get())) {
        // not at the deepest level yet, recurse
        if (location[level].idx < location[level].len) {
            subArray->applyOne(reader, location, level+1, f);
        }
    } else if (location[level].mayBeTruncated && !reader.readInt()) {
        throw ais::Error("array result truncated (possibly via set print elements)");
    } else {
        // at the deepst level, process the next element
        f(*m_elemType, reader);
    }

    if (int(location.size()) == level+1 && ++location[level].idx >= location[level].len) {
        // end of the array, unnest
        location.pop_back();
    }
}

bool ais::Array::isFortran() const { return false; }

bool ais::Array::resetTruncation() const { return false; }

std::shared_ptr<ais::Type> ais::Array::getAtomicType() const {
    auto arr = this;
    for (auto next=this; (next=dynamic_cast<const Array*>(arr->getElemType()));
         arr=next) {}
    return arr->getElemTypePtr();
}

void ais::Array::setOutput(std::unique_ptr<Output>&& out) {
    Array* innermost = this;
    for (Array* next; (next=dynamic_cast<Array*>(innermost->getElemTypePtr().get()));
         innermost = next) {};

    innermost->m_output = std::move(out);
}

ais::FilteredArray::Filter::Filter() {}
ais::FilteredArray::Filter::~Filter() {}

ais::FtnArray::FtnArray() { m_idx0 = 1; }
ais::RustVector::RustVector() { m_idx0 = 1; }
ais::RustArray::RustArray() { m_numElems = 0; }

ais::FtnArray::FtnArray(std::shared_ptr<Type> elemType, int len) :
    ais::Array(elemType, len, 1) {}

ais::RustVector::RustVector(std::shared_ptr<ais::Type> elemType) :
    ais::Array(elemType) {}

ais::RustArray::RustArray(std::shared_ptr<ais::Type> elemType, int len, int idx0) :
    ais::Array(elemType, len, idx0) {}


std::unique_ptr<ais::Printer> ais::RustVector::makePrinter(Printer& parent) const {
    struct TPrinter: public ArrayPrinter {
        TPrinter(Printer& parent) : ArrayPrinter(parent, "", ", ", "}") {}
        virtual void nest(const Type* t, int refIdx) override {
            ArrayPrinter::nest(t, refIdx);
            out << "Vec(size=" << m_len << ") = {";
        }
    };
        
    auto& fmt = parent.format();
    if (fmt.csv || fmt.onePerLine) return ais::Array::makePrinter(parent);
    else {
        return std::make_unique<TPrinter>(parent);
    }
}
std::unique_ptr<ais::Printer> ais::RustArray::makePrinter(Printer& parent) const {
    auto& fmt = parent.format();
    if (fmt.csv || fmt.onePerLine) return ais::Array::makePrinter(parent);
    else {
        return std::make_unique<ArrayPrinter>(parent, "[", ", ", "]");
    }
}

std::string ais::FtnArray::getTypeName() const {
    return m_elemType ? "^fortran[" + m_elemType->getTypeName() : "^fortran[";
}

int ais::FtnArray::getTypeId(ais::Writer& writer) const {
    auto elemIdx = getElemTypeId(writer);
    auto name = "^fortran[" + std::to_string(elemIdx);
    return writer.getTypeId(name);
}

std::shared_ptr<ais::Array> ais::FtnArray::makeTyped(std::shared_ptr<ais::Type> elemType) const {
    auto arr = std::make_shared<ais::FtnArray>(elemType);
    arr->setBounds(0,m_idx0);
    return arr;
}

std::unique_ptr<ais::Printer> ais::FtnArray::makePrinter(Printer& parent) const {
    auto& fmt = parent.format();
    if (fmt.csv || fmt.onePerLine) return ais::Array::makePrinter(parent);
    else return std::make_unique<ArrayPrinter>(parent, "(", ", ", ")");
}

void ais::FtnArray::printIndex(int idx, std::ostream& os) const {
    // TrackingVisitor catches the addMetaInfo, so idx already reflects the
    // initial index
    os << "(" << idx << ")";
}

bool ais::FtnArray::isFortran() const { return true; }

//===========================================================================

ais::ArrayChunk::ArrayChunk(std::shared_ptr<ais::Array> array, bool isBegin, bool isEnd) :
    m_array(array), m_isBegin(isBegin), m_isEnd(isEnd) {}

ais::ArrayChunk::ArrayChunk() : m_isBegin(false), m_isEnd(false) {}

void ais::ArrayChunk::parseL(InterpretStream& in, Writer& out) const {
    out.writeType(*m_array);
    out.writeInt(m_isBegin);
    out.writeInt(m_isEnd);
    
    m_array->parseL(in, out);
}

void ais::ArrayChunk::applyL(Reader& reader, Visitor& f) const {
    // ArrayChunk acts as adapter like ais::Pointer.   So the array
    // itself is a member along with where we are in the chunk train.
    auto array = reader.readType();
    bool isBegin = (reader.readInt() != 0);
    bool isEnd = (reader.readInt() != 0);
    f.addMetaInfo(array.get(), ChunkMetaInfo{isBegin, isEnd});
    if (array) array->apply(reader, f);
}

std::string ais::ArrayChunk::getTypeName() const {
    return "^ArrayChunk";
}

namespace ais {
    // This is basically the motivation for ArrayChunk, we need special handling
    // to avoid an extra "},{" between the chunks.
    struct ArrayChunkPrinter : public ArrayPrinter {
        ArrayChunkPrinter(Printer& parent) :
            ArrayPrinter(parent, "{", ",", ","), onePerLine(false), csv(false) {
            auto& fmt = parent.format();
            if ((onePerLine = fmt.onePerLine)) {
                open = ""; sep = "\n"; close = "";
            } else if ((csv = fmt.csv)) {
                open = close = ""; sep = ",";
            }
        }
        void addMetaInfo(const Type*, const MetaInfo& metaInfo) override {
            if (auto chunk = std::get_if<ChunkMetaInfo>(&metaInfo)) {
                if (onePerLine) {
                    if (!chunk->isBegin) open = "\n";
                } else if (csv) {
                    if (!chunk->isBegin) open = ",";
                } else {
                    if (!chunk->isBegin) open = "";
                    if (chunk->isEnd) close = "}";
                }
            }
        }
        void start(const Type* t) override { Printer::start(t); }
        void startChild() override { if (m_idx++) out << sep; else out << open; }
        void resetLabel(bool) override { parent->resetLabel(true); }

        bool onePerLine = false;
        bool csv = false;

    };
}

std::unique_ptr<ais::Printer> ais::ArrayChunk::makePrinter(Printer& parent) const {
    return std::make_unique<ArrayChunkPrinter>(parent);
}
    
//===========================================================================

ais::Truncater::Truncater(size_t limit) : m_limit(limit)
{
    if (limit) m_rem.push_back(limit);
}

void ais::Truncater::push(const ais::Type& nestedType) {
    if (m_limit && nestedType.resetTruncation()) {
        m_rem.push_back(m_limit);
    }
}
void ais::Truncater::pop(const ais::Type& nestedType) {
    if (m_limit && nestedType.resetTruncation()) {
        m_rem.pop_back();
    }
}
bool ais::Truncater::truncate(const ais::Type& nestedType) {
    if (!m_limit) return false;

    auto& n = m_rem.back();
    if (n == 0) return true;

    // only update the count on the innermost dimension
    if (nestedType.resetTruncation()) --n;
    return false;
}
 
//===========================================================================

ais::CharArray::CharArray() {}

const::std::string ais::CharArray::nullString{"<<<null>>>"};

namespace {
    // A little one-off to parse char arrays when gdb print the result
    // as an array of characters
    class CharArrayBuffer : public ais::WriteBuffer {
    public:
        void writeLong(long) override {}
        void writeLongLong(long long) override {}
        void writeFloat(float) override {}
        void writeDouble(double) override {}
        void writeLongDouble(long double) override {}
        void writeString(const std::string&) override {}
        void writePointer(const std::string&, void*) override {}
        void writeInt(int val) override { str << char(val); }
        void writeError(const std::string& str) override { if (err.empty()) err = str; }

        std::ostringstream str;
        std::string err;
    };
}



void ais::CharArray::parseL(InterpretStream& in, Writer& out) const {
    const char* str = in.get();

    printf("In CharArray::parse, value is <%s>\n", str);
    str = skipspace(str);

    // gdb may return this as an array; currently the main case we've seen
    // is when the value is optimized out
    if (str[0] == '{') {
        in.setPos(str+1);
        auto ctype = ais::Char{};
        auto b = std::make_shared<CharArrayBuffer>();
        auto w = ais::Writer{b};
        do {
            ctype.parse(in, w);
            str = skipspace(in.get());
            if (*str)
                in.setPos(str+1);
        } while (*str == ',');

        if (*str != '}') {
            out.writeError("no closing brace in result\n");
        } else if (!b->err.empty()) {
            out.writeError(b->err);
        } else {
            out.writeString(b->str.str());
        }
        return;
    }
    
    // depending on the context, gdb may print out the pointer value
    // first - skip to the string value.
    // lookat - this won't properly handle a null pointer..
    if (str[0] == '0' && str[1] == 'x') {
        if (str[2] == '0' && !isalnum(str[3])) {
            // using a reserved value and hoping the application doesn't
            // happen to use it is a little weak, but pushing the equivalent
            // of std::optional<std::string> through the operators
            // seems like overkill.
            out.writeString(nullString);
            in.setPos(str+3);
            return;
        }
        auto q = strchr(str, '"'); // contents of the string
        auto e = strchr(str, '<'); // error message

        if (q && (!e || q < e)) str = q;
        else if (e && (!q || e < q)) str = e;
    }

    if (str[0] == '"') {
        auto beg = str+1;
        auto end = beg;
        for ( ; *end && *end != '"'; ++end) {
            if (end[0] == '\\' && end[1]) ++end;
        }
        out.writeString(std::string{beg,end});

        if (*end) {
            // values truncated by set print elements will add ... (from gdb13)
            for (; *++end == '.'; ) {}
        }
        in.setPos(end);
        return;
    }  else if (str[0] == '<') {
        // error message
        out.writeError("<cannot access memory at address>");
        if (auto end = strchr(str,'>')) in.setPos(end+1);
        return;
    }
        
    out.writeError(std::string{"unexpected-CharArray-value"} + in.get());
}

void ais::CharArray::applyL(Reader& reader, Visitor& f) const {
    f(readVal(reader));
}

std::string ais::CharArray::getTypeName() const {
    return "^CharArray";
}

std::string ais::CharArray::readVal(Reader& reader) const {
    return reader.readString();
}

std::unique_ptr<ais::Printer> ais::CharArray::makePrinter(Printer& parent) const {
    struct TPrinter : public Printer {
        TPrinter(Printer& parent) : Printer(parent) {}
        using Printer::operator();
        void operator()(const std::string& val) override {
            if (val == nullString) {
                out << "(char*) 0x0";
            } else {
                Printer::operator()(val);
            }
        }
    };
    return std::make_unique<TPrinter>(parent);
}

//===========================================================================
// Special type for dealing with an array decomposed across multiple ranks.
ais::DecompArray::DecompArray(std::shared_ptr<ais::Array> array,
                              std::shared_ptr<ais::DecompIterator> decompIter)
    : m_array(array), m_decompIter(decompIter), m_openChar('{'), m_closeChar('}') {
    // The decomposition may have higher dimension than the input arrays, so
    // for 2d and higher we *synthesize* the out types.   The main property we
    // need to preserve is the 'fortran-ness' though here we distinguish the
    // array-of-arrays because the can matter for printing.
    auto dim = decompIter->getDim();
    if (dim > 1) {
        m_arrayOfArray = m_array->makeTyped(m_array);
    } else if (dim == 1) {
        m_arrayOfArray = m_array;
    } else {
        m_arrayOfArray = m_array->getElemTypePtr();
    }
}

void ais::DecompArray::parseL(InterpretStream&, Writer&) const {
    // DecompArray is a client side feature, the parsing should happen as
    // normal array (or vector) types on the back end.
    std::cerr << "DecompArray::parse";
}

std::unique_ptr<ais::Printer> ais::DecompArray::makePrinter(Printer& parent) const {
    return m_arrayOfArray->makePrinter(parent);
}
void ais::DecompArray::printIndex(int idx, std::ostream& out) const {
    m_array->printIndex(idx,out);
}
    
void ais::DecompArray::applyL(Reader& reader, Visitor& f) const {        
    auto distReader = dynamic_cast<DecompReader*>(&reader);
    if (!distReader) {
        throw ais::Error("expected a decomp reader inside a block map");
    }

    // determine the underlying array and element types
    const ais::Type* arrayOfArrayType = m_arrayOfArray.get();
    const ais::Type* arrayType = m_array.get();
    const ais::Type* elemType = arrayType;
    while (auto arr = dynamic_cast<const Array*>(elemType)) {
        elemType = arr->getElemType();
    }

    // array types are largely synthetic for the output side of decompositions, but
    // we do need to disinguish the innermost array type for makePrinter to work right.
    auto getNestType = [elemType,arrayType,arrayOfArrayType](int level, int startLevel) {
        if (level+1 == startLevel) return elemType;
        else if (level+2 == startLevel) return static_cast<const ais::Type*>(arrayType);
        else return arrayOfArrayType;
    };
    

    // DecompArray::apply is going to make it look to visitor like we are
    // processing the unified array.   m_decomIter defines the rank where the next
    // element is coming from, and we call applyOne on that ranks subreader forwarding
    // the result to 'f'.

    // Since we are breaking up the usual traversal of the sub-readers, we need to
    // externalize the current location in inputLocs
    std::vector<std::vector<ais::Array::ApplyOneIdx>> inputLocs;

    // applyOp forwards the action to f
    auto applyOp = [&f,&reader](const Type& t, Reader&) { t.apply(reader, f); };

    // now iterator over the unified array 
    bool first = true;
    bool pend = false;
    int outputLevel = 0;
    bool isFortran = m_array->isFortran();
    int index0 = m_array->getIndex0();

    auto& it = *m_decompIter;
    it.begin(isFortran, index0);
    while (it.next()) {
        if (pend) f.end();

        // handle any nest/start before writing the next value
        for (int i=outputLevel; i < it.startLevel; ++i) {
            // similar logic to the previous, the caller call the first start
            if (first) first = false;
            else f.start(arrayType);
            if (isFortran) f.addMetaInfo(this, ArrayMetaInfo{1,0});
            f.nest(getNestType(i,it.startLevel));
        }
        outputLevel = it.startLevel;
        
        distReader->setSubIdx(it.streamIdx);

        inputLocs.resize(std::max(inputLocs.size(), size_t(it.streamIdx+1)));
        auto& locs = inputLocs[it.streamIdx];
        
        for (int i=0, n=it.len; i < n; ++i) {
            f.start(m_array.get());
            m_array->applyOne(reader, locs, 0, applyOp);
            f.end();
        };

        // unnest when at the end of arrays
        pend = false;
        for (int i=outputLevel; i > it.endLevel; --i) {
            if (pend) f.end();
            f.unnest();
            // this is the natural location for f.end(), but
            // since the caller any::apply calls one, I have to forego
            // the final f.end() call.   Hence, we use "pend" to do it
            // in arrears
            pend = true;
        }
        outputLevel = it.endLevel;
    }
}

std::string ais::DecompArray::getTypeName() const {
    return "^Decomp" + m_array->getTypeName();
}

ais::Array1::Array1(std::shared_ptr<Type> elemType) : Array(elemType,1) {}

void ais::Array1::applyL(Reader& reader, Visitor& f) const {
    f.nest(m_elemType.get());
    f.start(m_elemType.get());
    m_elemType->apply(reader, f);
    f.end();
    f.unnest();
}

void ais::Array1::applyOne(Reader& reader, std::vector<ApplyOneIdx>& location, int,
                           const std::function<void(const Type&,Reader&)>& f) {
    location.push_back(ApplyOneIdx{0, 1, false});
    f(*m_elemType, reader);
    location.pop_back();
}

//===========================================================================

namespace {
// dummy writer to drop output into the aether
class NullBuffer : public ais::WriteBuffer {
public:
    void writeInt(int) override {}
    void writeLong(long) override {}
    void writeLongLong(long long) override {}
    void writeFloat(float) override {}
    void writeDouble(double) override {}
    void writeLongDouble(long double) override {}
    void writeString(const std::string&) override {}
    void writePointer(const std::string&, void*) override {}
    void writeError(const std::string&) override {}

};
}

// toggle between the real and null output streams for filtering
class ais::FilteredArray::Output : public ais::Array::Output {
public:
    Output(std::shared_ptr<Filter> filter, ais::Writer& liveOut)
        : m_filter(filter), m_liveOut(liveOut) {}
    Writer& getOut(ais::Writer& out) {
        return m_filter->doWrite() ? m_liveOut : out; // out is null-writer
    }
private:
    std::shared_ptr<Filter> m_filter;
    ais::Writer& m_liveOut;
};

ais::FilteredArray::FilteredArray(std::shared_ptr<Array> baseType,
                                  std::shared_ptr<Filter> filter)
    : Array(baseType->getAtomicType()), m_baseType(baseType), m_filter(filter) {
    if (baseType->isFortran()) {
        m_ftn = std::make_unique<FtnArray>(baseType->getAtomicType());
    }
}

void ais::FilteredArray::parseL(InterpretStream& in, Writer& out) const {
    auto nullBuffer = std::make_shared<NullBuffer>();
    auto nullWriter = ais::Writer{nullBuffer};

    m_baseType->setOutput(std::make_unique<Output>(m_filter, out));
    
    // write the array header info 
    out.writeInt(m_filter->getLen());
    out.writeInt(0); // idx0
    out.writeInt(false); // mayBeTruncated

    // and write the array, note that all the control overhead is sent to
    // the nullWriter, we switch back to the real output for writing the
    // single elements
    m_baseType->parseL(in, nullWriter);
}
std::string ais::FilteredArray::getTypeName() const {
    return m_ftn ? m_ftn->getTypeName() : Array::getTypeName();
}
int ais::FilteredArray::getTypeId(Writer& writer) const {
    return m_ftn ? m_ftn->getTypeId(writer) : Array::getTypeId(writer);
}

static std::unique_ptr<ais::Writer> makeNullWriter() {
    auto nullBuffer = std::make_shared<NullBuffer>();
    return std::make_unique<ais::Writer>(nullBuffer);
}
 
//===========================================================================

ais::Vector::Vector() : m_capacity(0) {}

ais::Vector::Vector(std::shared_ptr<Type> elemType, int len, int capacity)
    : Array(elemType, len), m_capacity(capacity) {}

void ais::Vector::parseL(InterpretStream& in, Writer& out) const {
    const char* s = skipspace(in.get());

    if (s[0] == '{') {
        out.writeInt(0); // capacity
        return Array::parseL(in, out);
    }

    int len, capacity, chars;
    int n = sscanf(s, "std::vector of length %d, capacity %d%n",
                   &len, &capacity, &chars);

    if (n != 2) {
        fprintf(stderr, "could not parse %s\n", s);
        out.writeString("vector-parse-failed");
        return;
    }

    const_cast<Vector*>(this)->m_numElems = len;
    out.writeInt(capacity);

    if (len <= 0) {
        // duplicate the header for a 0 length array since
        // ArrayParse is looking for "{}"
        in.setPos(s+chars);
        out.writeInt(0);     // numElems
        out.writeInt(0);     // idx0
        out.writeInt(false); // mayBeTruncated
    } else if (len > 0) {
        s = skipspace(s+chars);
        if (*s == '=') {
            s = skipspace(s+1);
            in.setPos(s);
            Array::parseL(in, out);
        } else {
            fprintf(stderr, "did not find '=' in \"%s\"\n", s);
            out.writeString("vector-parse-failed");
        }
    }
}

void ais::Vector::applyL(Reader& reader, Visitor& f) const {
    [[maybe_unused]] auto capacity = reader.readInt();

    Array::applyL(reader, f);
}

std::string ais::Vector::getTypeName() const {
    return "std::vector[" + (m_elemType ? m_elemType->getTypeName() : "");
}
std::string ais::RustVector::getTypeName() const {
    return "alloc::vec::Vec[" + (m_elemType ? m_elemType->getTypeName() : "");
}
std::string ais::RustArray::getTypeName() const {
    return "std::array[" + (m_elemType ? m_elemType->getTypeName() : "");
}

int ais::Vector::getTypeId(Writer& writer) const {
    auto elemIdx = getElemTypeId(writer);
    auto name = "std::vector[" + std::to_string(elemIdx);
    return writer.getTypeId(name);
}
int ais::RustVector::getTypeId(Writer& writer) const {
    auto elemIdx = getElemTypeId(writer);
    auto name = "alloc::vec::Vec[" + std::to_string(elemIdx);
    return writer.getTypeId(name);
}
int ais::RustArray::getTypeId(Writer& writer) const {
    auto elemIdx = getElemTypeId(writer);
    auto name = "std::array[" + std::to_string(elemIdx);
    return writer.getTypeId(name);
}

// generate a typed vector from a factory instance
std::shared_ptr<ais::Array> ais::Vector::makeTyped(std::shared_ptr<ais::Type> elemType) const {
    return std::make_shared<ais::Vector>(elemType);
}
std::shared_ptr<ais::Array> ais::RustVector::makeTyped(std::shared_ptr<ais::Type> elemType) const {
    return std::make_shared<ais::RustVector>(elemType);
}
std::shared_ptr<ais::Array> ais::RustArray::makeTyped(std::shared_ptr<ais::Type> elemType) const {
    return std::make_shared<ais::RustArray>(elemType);
}

void ais::Vector::applyOne(Reader& reader, std::vector<ApplyOneIdx>& location, int level,
                           const std::function<void(const Type&,Reader&)>& f) {
    if (int(location.size()) <= level) {
        [[maybe_unused]] auto capacity = reader.readInt();
    }
    Array::applyOne(reader, location, level, f);
}

//===========================================================================

namespace {
class ListElem : public ais::Type {
public:
    ListElem(std::shared_ptr<ais::Type> elem) : m_elem(elem) {}
    std::string getTypeName() const override { return m_elem->getTypeName(); }
    bool resetTruncation() const override { return m_elem->resetTruncation(); }
    void applyL(ais::Reader& reader, ais::Visitor& f) const override { m_elem->apply(reader, f); }
    void parseL(ais::InterpretStream& in, ais::Writer& out) const override {
        const char* s = in.get();
        // skip the [idx] = part
        if (auto eq = strchr(s,'=')) in.setPos(eq+1);
        m_elem->parse(in,out);
    }
private:
    std::shared_ptr<ais::Type> m_elem;
};
}

ais::List::List() {}

ais::List::List(std::shared_ptr<Type> elemType)
    : Array(elemType) {}

void ais::List::parseL(InterpretStream& in, Writer& out) const {
    const char* s = skipspace(in.get());

    if (strncmp(s,"empty ", 6) == 0) {
        // expecting "empty std::__cxx11::list"
        if (auto t = strstr(s,"::list")) {
            s = t+6;
            for (int i=0; i<3; ++i) out.writeInt(0);
            in.setPos(s);
            return;
        }
    }

    auto lbrk = strchr(s,'{');
    if (!lbrk) {
        out.writeError("<cannot parse list>");
        return;
    }

    // find the number of elements in the list
    int len = 0;
    for (auto p = lbrk; *p && *p != '}'; ++len) {
        p = skipspace(++p);
        if (*p != '[') break;
        p = findExprMatch(p);
        if (!(p=strchr(p,'='))) break;

        // skip over the value
        for ( ; *p && *p != ',' && *p != '}'; ++p) {
            if (*p == '{' || *p == '(' || *p == '"' || *p == '\'')
                p = findExprMatch(p);
        }
    }

    auto le = ListElem{m_elemType};

    Array arr(std::make_shared<ListElem>(m_elemType), len);
    in.setPos(lbrk);
    arr.parseL(in, out);
}
            

std::string ais::List::getTypeName() const {
    return "std::list[" + (m_elemType ? m_elemType->getTypeName() : "");
}

int ais::List::getTypeId(Writer& writer) const {
    auto elemIdx = getElemTypeId(writer);
    auto name = "std::list[" + std::to_string(elemIdx);
    return writer.getTypeId(name);
}

// generate a typed vector from a factory instance
std::shared_ptr<ais::Array> ais::List::makeTyped(std::shared_ptr<ais::Type> elemType) const {
    return std::make_shared<ais::List>(elemType);
}


//=============================

ais::Deque::Deque() {}

ais::Deque::Deque(std::shared_ptr<Type> elemType) : ais::Array(elemType) {}

void ais::Deque::parseL(InterpretStream& in, Writer& out) const {
    const char* s = in.get();

    int len,chars;
    int n = sscanf(s, "std::deque with %d elements = %n", &len, &chars);

    if (n != 1) {
        fprintf(stderr, "could not parse %s\n", s);
        out.writeString("deque-parse-failed");
        return;
    }
        
    const_cast<Deque*>(this)->m_numElems = len;

    in.setPos(s+chars);

    Array::parseL(in, out);
}

// generate a typed vector from a factory instance
std::shared_ptr<ais::Array> ais::Deque::makeTyped(std::shared_ptr<ais::Type> elemType) const {
    return std::make_shared<ais::Deque>(elemType);
}

std::string ais::Deque::getTypeName() const {
    return "std::deque[" + (m_elemType ? m_elemType->getTypeName() : "");
}

int ais::Deque::getTypeId(Writer& writer) const {
    auto elemIdx = getElemTypeId(writer);
    auto name = "std::deque[" + std::to_string(elemIdx);
    return writer.getTypeId(name);
}

//=============================

ais::Queue::Queue() {}

ais::Queue::Queue(std::shared_ptr<Type> elemType) : ais::Deque(elemType) {}

void ais::Queue::parseL(InterpretStream& in, Writer& out) const {
    const char* s = in.get();

    const auto wr_tag = "wrapping:";
    if (auto w = strstr(s, wr_tag)) {
        s = skipspace(w+strlen(wr_tag));
        in.setPos(s);
        Deque::parseL(in, out);
    } else {
        fprintf(stderr, "could not parse %s\n", s);
        out.writeString("priority_queue-parse-failed");
        return;
    }
}

// generate a typed vector from a factory instance
std::shared_ptr<ais::Array> ais::Queue::makeTyped(std::shared_ptr<ais::Type> elemType) const {
    return std::make_shared<ais::Queue>(elemType);
}

std::string ais::Queue::getTypeName() const {
    return "std::queue[" + (m_elemType ? m_elemType->getTypeName() : "");
}

int ais::Queue::getTypeId(Writer& writer) const {
    auto elemIdx = getElemTypeId(writer);
    auto name = "std::queue[" + std::to_string(elemIdx);
    return writer.getTypeId(name);
}


//===========================================================================

ais::Stack::Stack() {}

ais::Stack::Stack(std::shared_ptr<Type> elemType) : ais::Array(elemType) {}

void ais::Stack::parseL(InterpretStream& in, Writer& out) const {
    const char* s = in.get();

    int len = -1;
    auto ep = strstr(s, "elements");
    if (ep) {
        auto e = ep;
        for ( ; e != s && (isdigit(e[-1]) || isspace(e[-1])); --e) {}
        char* e_end;
        len = strtol(e, &e_end, 10);
        if (e == e_end) len = -1;
    }

    auto eq = strchr(ep, '=');

    if (!eq || len < 0) {
        fprintf(stderr, "could not parse %s\n", s);
        out.writeString("stack-parse-failed");
        return;
    }
        
    const_cast<Stack*>(this)->m_numElems = len;

    s = skipspace(eq+1);
    in.setPos(s);

    Array::parseL(in, out);
}

// generate a typed vector from a factory instance
std::shared_ptr<ais::Array> ais::Stack::makeTyped(std::shared_ptr<ais::Type> elemType) const {
    return std::make_shared<ais::Stack>(elemType);
}

std::string ais::Stack::getTypeName() const {
    return "std::stack[" + (m_elemType ? m_elemType->getTypeName() : "");
}

int ais::Stack::getTypeId(Writer& writer) const {
    auto elemIdx = getElemTypeId(writer);
    auto name = "std::stack[" + std::to_string(elemIdx);
    return writer.getTypeId(name);
}

//===========================================================================
ais::StdArray::StdArray() {}

ais::StdArray::StdArray(std::shared_ptr<Type> elemType, int len) : ais::Array(elemType,len) {}

void ais::StdArray::parseL(InterpretStream& in, Writer& out) const {
    const char* s = in.get();

    if (auto eq = strchr(s, '=')) {
        s = skipspace(eq+1);
        in.setPos(s);
        Array::parseL(in, out);
    } else {
        fprintf(stderr, "could not parse %s\n", s);
        out.writeString("std::array-parse-failed");
        return;
    }
}

// generate a typed vector from a factory instance
std::shared_ptr<ais::Array> ais::StdArray::makeTyped(std::shared_ptr<ais::Type> elemType) const {
    return std::make_shared<ais::StdArray>(elemType);
}

std::string ais::StdArray::getTypeName() const {
    return "std::array[" + (m_elemType ? m_elemType->getTypeName() : "");
}

int ais::StdArray::getTypeId(Writer& writer) const {
    auto elemIdx = getElemTypeId(writer);
    auto name = "std::array[" + std::to_string(elemIdx);
    return writer.getTypeId(name);
}


//===========================================================================
ais::PriorityQueue::PriorityQueue() {}

ais::PriorityQueue::PriorityQueue(std::shared_ptr<Type> elemType) : ais::Vector(elemType) {}

void ais::PriorityQueue::parseL(InterpretStream& in, Writer& out) const {
    // std::priority_queue is a wrapper around an std::vector
    
    const char* s = in.get();

    const auto wr_tag = "wrapping:";
    if (auto w = strstr(s, wr_tag)) {
        s = skipspace(w+strlen(wr_tag));
        in.setPos(s);
        Vector::parseL(in, out);
    } else {
        fprintf(stderr, "could not parse %s\n", s);
        out.writeString("priority_queue-parse-failed");
        return;
    }
}

// generate a typed vector from a factory instance
std::shared_ptr<ais::Array> ais::PriorityQueue::makeTyped(std::shared_ptr<ais::Type> elemType) const {
    return std::make_shared<ais::PriorityQueue>(elemType);
}

std::string ais::PriorityQueue::getTypeName() const {
    return "std::priority_queue[" + (m_elemType ? m_elemType->getTypeName() : "");
}

int ais::PriorityQueue::getTypeId(Writer& writer) const {
    auto elemIdx = getElemTypeId(writer);
    auto name = "std::priority_queue[" + std::to_string(elemIdx);
    return writer.getTypeId(name);
}

//===========================================================================

ais::Map::Map() {}

ais::Map::Map(std::shared_ptr<Type> keyType, std::shared_ptr<Type> valType)
    : m_keyType(keyType), m_valType(valType) {}

void ais::Map::parseL(InterpretStream& in, Writer& out) const {
    const char* s = in.get();

    // expecting:
    // std::map with 3 elements = {[0] = 0, [2] = 7, [3] = 4}

    static auto elemRE = std::regex{R"(with\s+(\d+)\s+elements?)"};
    auto match = std::cmatch{};
    if (!regex_search(s, match, elemRE)) throw ais::Error("could not parse map size");

    auto n = std::stoi(match[1].str());
    s = match.suffix().first;

    const char* s2 = s;
    if (n > 0) {
        if ((s2 = strchr(s, '{'))) s = s2+1;
        else throw ais::Error("could not parse std::map contents");
    }

    out.writeInt(m_keyType->getTypeId(out));
    out.writeInt(m_valType->getTypeId(out));
    out.writeInt(n);

    int i = 0;
    for (int i=0; i<n; ++i) {
        if ((s2=strchr(s,'['))) s=s2+1; else break;
        in.setPos(s);
        m_keyType->parse(in, out);
        s = in.get();

        if ((s2=strchr(s,']'))) s=s2+1; else break;
        if ((s2=strchr(s,'='))) s=s2+1; else break;

        in.setPos(s);
        m_valType->parse(in,out);
        s = in.get();

        s += strspn(s," ,}");
    }

    if (i != n) throw ais::Error("could not parse std::map contents");
    
    in.setPos(s);
}

void ais::Map::applyL(Reader& reader, Visitor& f) const {
    auto keyTypeIdx = reader.readInt();
    auto valueTypeIdx = reader.readInt();
    auto n = reader.readInt();

    auto keyType = reader.getType(keyTypeIdx);
    auto valueType = reader.getType(valueTypeIdx);

    if (!n) f.nest(keyType.get());

    for (int i=0; i<n; ++i) {
        f.nest(keyType.get());
        f.start(keyType.get());
        keyType->apply(reader, f);
        f.end();
        f.unnest();

        f.nest(valueType.get());
        f.start(valueType.get());
        valueType->apply(reader, f);
        f.end();
        f.unnest();
    }

    if (!n) f.unnest();
}

std::string ais::Map::getTypeName() const {
    return "std::map";
}

int ais::Map::getTypeId(Writer& writer) const {
    m_keyType->getTypeId(writer);
    m_valType->getTypeId(writer);
    return ais::Type::getTypeId(writer);
}

std::unique_ptr<ais::Printer> ais::Map::makePrinter(Printer& parent) const {
    struct TPrinter : public Printer {
        TPrinter(Printer& parent, std::string&& mapType) :
            Printer(parent), m_type(std::move(mapType)) {}
        void start(const Type* t) override {
            Printer::start(t);
            out << m_type << " = {"; }
        void startChild() override {
            // values are written as [key] = value
            if (m_idx % 2) out << "] = ";
            else if (m_idx) out << ", [";
            else out << "[";
            ++m_idx;
        }
        void end() override { Printer::end(); out << "}"; }
        int m_idx = 0;
        std::string m_type;
    };
    return std::make_unique<TPrinter>(parent, getTypeName());
}

ais::UnorderedMap::UnorderedMap() {}
ais::UnorderedMap::UnorderedMap(std::shared_ptr<Type> keyType,
                                std::shared_ptr<Type> valType) : Map(keyType, valType) {}

std::string ais::UnorderedMap::getTypeName() const {
    return "std::unordered_map";
}

ais::Multimap::Multimap() {}
ais::Multimap::Multimap(std::shared_ptr<Type> keyType,
                        std::shared_ptr<Type> valType) : Map(keyType, valType) {}

std::string ais::Multimap::getTypeName() const {
    return "std::multimap";
}

ais::UnorderedMultimap::UnorderedMultimap() {}
ais::UnorderedMultimap::UnorderedMultimap(std::shared_ptr<Type> keyType,
                        std::shared_ptr<Type> valType) : Map(keyType, valType) {}

std::string ais::UnorderedMultimap::getTypeName() const {
    return "std::unordered_multimap";
}

//===========================================================================

ais::Struct::Struct(const std::string& name) : m_name(name) {}

    
void ais::Struct::addMember(const std::string& memberName, std::shared_ptr<Type> memberType) {
    m_members.push_back(std::make_pair(memberName, memberType));
}

// skip over a vtable entry in the class/struct value in C++
static const char* skipVTable(const char* s) {
    if (strncmp(s, "_vptr$", 6) == 0) {
        // virtual function table - ignore
        // expecting: "_vptr$Class = 0x2013a0 <vtable for Class+16>, 
        auto t = s;
        if (!(t=strchr(t,'=')) || !(t=strchr(t,'<'))) return s;
        if (*(t = findExprMatch(t))) ++t;
        if (*t == ',') t = skipspace(t+1);
        return t;
    } else if (strncmp(s, "_vptr.", 6) == 0) {
        // gcc, expecting _vptr.Class = 0x[\x]+,
        if (auto t = strchr(s, ',')) {
            t = skipspace(t+1);
            return t;
        }
    }
    return s;
}

void ais::Struct::parseL(InterpretStream& in, ais::Writer& writer) const {
    auto s = in.get();
    s = skipspace(s);

    char endBracket = '}';
    
    if (*s == '{') {
        ++s;    // C/C++ struct/class
    } else if (*s == '(') {
        ++s; endBracket = ')';  // fortran defined type
    } else {
        static const std::regex refRE{R"(^@0x[[:xdigit:]]+)"};
        auto match = std::cmatch{};
        if (std::regex_search(s, match, refRE)) {
            fprintf(stderr, "got a reference for a struct\n");
            writer.writeError("<reference>");
            in.setPos(s+match.length(0));
            return;
        } else {
            fprintf(stderr, "expected opening bracket\n");
        }
    }

    auto numMembers = m_members.size();

    for (auto& [name, elem] : m_members) {
        s = skipspace(s);
        s = skipVTable(s);
        
        if (strncmp(s, name.c_str(), name.size()) == 0) {
            s += name.size();
        } else if (*s == '<') {
            // member is actually a base class
            --numMembers;
            for (auto ltgt = int{1}; *++s && ltgt; ) {
                if (*s == '<') ++ltgt;
                else if (*s == '>') --ltgt;
            }
        } else {
            fprintf(stderr, "expected \"%s\" got \"%s\"\n", name.c_str(), s);
        }

        s = skipspace(s);
        if (*s == '=') ++s;
        else fprintf(stderr, "expected \"=\" got \"%c\"\n", *s);
        s = skipspace(s);
        in.setPos(s);

        fprintf(stderr, "calling element parse \"%.40s...\"\n", s);

        elem->parse(in, writer);

        s = in.get();

        fprintf(stderr, "after element parse \"%.40s...\"\n", s);
        s = skipspace(s);
        if (*s == ',') ++s;
        in.setPos(s);
    }

    if (!numMembers) {
        const auto ndf = "<No data fields>";
        auto n_ndf = strlen(ndf);
        s = skipspace(s);
        s = skipVTable(s);
        if (strncmp(s, ndf, n_ndf) == 0) s += n_ndf;
        else fprintf(stderr, "did not find <No data fields\n");
    }

    s = skipspace(s);
    if (*s == endBracket) ++s;
    else fprintf(stderr, "expected \'%c\'\n", endBracket);
    
    in.setPos(s);
}

void ais::Struct::parseError(const std::string& err, Writer& out) const {
    // If the struct has no members, I can send and error because the
    // client won't read anything
    if (!m_members.empty()) Type::parseError(err, out);
}

int  ais::Struct::getTypeId(ais::Writer& writer) const {
    auto typeName = "{" + m_name + ";";

    for (auto& [name, elem] : m_members) {
        typeName += name + ";" + std::to_string(elem->getTypeId(writer)) + ";";
    }
    return writer.getTypeId(typeName);
}

// inverse operation of getTypeId.   Here we take the string created by
// ais::Struct::getTypeId and use it to recreate the Struct definition on the
// receiver side.
ais::Struct::Struct(ais::Reader& reader, const std::string& defString) {
    auto pos = defString.find(';',1);
    if (pos == std::string::npos) {
        throw ais::Error("no name for a struct");
    }
    m_name = defString.substr(1,pos-1);
    ++pos;
    // the defString is set up a member-name;typeid;
    for (size_t sep1, sep2;
         ((sep1=defString.find(';',pos)) != std::string::npos &&
          (sep2=defString.find(';',sep1+1)) != std::string::npos); pos = sep2+1) {
        auto mname = defString.substr(pos,sep1-pos);
        auto melemIdStr = defString.substr(sep1+1, sep2-sep1-1);
        auto melemId = std::stoi(melemIdStr);
        auto melem = reader.getType(melemId);

        m_members.push_back(make_pair(mname, melem));
    }
}

std::unique_ptr<ais::Printer> ais::Struct::makePrinter(Printer& parent) const {
    using Members = decltype(m_members);
    // Default printer, print structure on a single line
    struct TPrinter : public Printer {
        TPrinter(Printer& parent, Members members) : Printer(parent), m_members(members) {}
        void start(const Type* t) override { Printer::start(t); m_idx = 0; out << "{"; }
        void startChild() override {
            if (m_idx) out << ", ";
            if (m_idx < int(m_members.size())) out << m_members[m_idx].first << " = ";
            ++m_idx;
        }
        void end() override { out << "}"; }
        int m_idx = 0;
        Members m_members;
    };

    // "Pretty" printer, puts members on separate indented lines
    struct PPrinter : public Printer {
        PPrinter(Printer& parent, Members members) : Printer(parent), m_members(members) {}
        void start(const Type* t) override { Printer::start(t); m_idx = 0; out << "{"; }
        void startChild() override {
            out << "\n";
            indent();
            if (m_idx < int(m_members.size())) out << m_members[m_idx].first << " = ";
            ++m_idx;
        }
        void end() override { out << "\n"; parent->indent(); out << "}"; }
        void indent() override { if (parent) parent->indent(); out << "  "; }
        int m_idx = 0;
        Members m_members;
    };

    if (parent.format().pretty) {
        return std::make_unique<PPrinter>(parent,m_members);
    } else {
        return std::make_unique<TPrinter>(parent,m_members);
    }
}

void ais::Struct::printIndex(int idx, std::ostream& out) const {
    if (idx >= 0 && idx < int(m_members.size())) {
        out << "{" << m_members[idx].first << "}";
    }
}

void ais::Struct::applyL(Reader& reader, Visitor& f) const {
    for ( auto [ name, elem] : m_members) {
        f.nest(elem.get());
        f.start(elem.get());
        elem->apply(reader, f);
        f.end();
        f.unnest();
    }
}

std::string ais::Struct::getTypeName() const {
    return m_name;
}

ais::Union::Union(const std::string& name) : Struct(name) {}

namespace {
    // BufferWriter modified to not traverse pointers, but to treat each one
    // kind of like <optimized out>
    class UnionWriteBuffer : public ais::WriteBuffer {
    public:
        UnionWriteBuffer(std::shared_ptr<ais::WriteBuffer> fwd) : f(fwd) {}
        void writeInt(int val) override { f->writeInt(val); }
        void writeLong(long val) override { f->writeLong(val); }
        void writeLongLong(long long val) override { f->writeLongLong(val); }
        void writeFloat(float val) override { f->writeFloat(val); }
        void writeDouble(double val) override { f->writeDouble(val); }
        void writeLongDouble(long double val) override { f->writeLongDouble(val); }
        void writeString(const std::string& str) override { f->writeString(str); }
        void writePointer(const std::string&, void*) override { suppress(); }
        void writeError(const std::string& str) override { f->writeError(str); }
    private:
        void suppress();
        std::shared_ptr<ais::WriteBuffer> f;
    };

    void UnionWriteBuffer::suppress() { f->writeError("<suppressed in a union>"); }
}

void ais::Union::parseL(InterpretStream& in, Writer& out) const
{
    // This is the only override needed for a union since they are handled identically
    // on the client side.   But it still needs to parse the usual output from gdb,
    // so we just shunt the pointers aside at the writer level.   This also means
    // that pointers nested arbitrarily deeply in structs inside the union are still
    // suppressed.
    fprintf(stderr, "in ais::Union::parseL\n");
    auto sbuff = std::make_shared<UnionWriteBuffer>(out.getBuffer());
    auto sout = ais::Writer{sbuff};
    Struct::parseL(in, sout);
}

//===========================================================================

ais::Pair::Pair(const std::string& first, std::shared_ptr<Type> firstType,
                const std::string& second, std::shared_ptr<Type> secondType) :
    ais::Struct("std::pair<" + first + "," + second + ">") {
    addMember("first", firstType);
    addMember("second", secondType);
}

void ais::Pair::parseL(InterpretStream& in, Writer& out) const {
    // skip over the empty base class
    auto s = in.get();
    static auto firstRE = std::regex{R"(\bfirst\s*=)"};
    auto match = std::cmatch{};
    
    if (std::regex_search(s, match, firstRE)) in.setPos(s += match.prefix().length());
    Struct::parseL(in, out);
}

//===========================================================================
ais::Tuple::Tuple(const std::vector<std::pair<std::string, std::shared_ptr<Type>>>& elems) :
    ais::Struct(makeTypeName(elems)) {
    int idx = 0;
    for (const auto& [name, type] : elems) {
        addMember("[" + std::to_string(++idx) + "]", type);
    }
}

void ais::Tuple::parseL(InterpretStream& in, Writer& out) const {
    // skip over the empty base class
    auto s = in.get();

    if (auto t = strchr(s,'{')) in.setPos(s=t);
    Struct::parseL(in, out);
}

std::string
ais::Tuple::makeTypeName(const std::vector<std::pair<std::string,std::shared_ptr<Type>>>& elems) {
    auto os = std::ostringstream{};
    os << "std::tuple<";
    auto i = 0;
    for (const auto& [name, type] : elems) os << (i++ ? "," : "") << name;
    os << ">";
    return os.str();
}
                

//===========================================================================

ais::Pointer::Pointer(std::string derefType) : m_derefType(std::move(derefType)) {}

void ais::Pointer::parseL(InterpretStream& in, Writer& out) const {
    fprintf(stderr, "in Pointer::parse %s\n", m_derefType.c_str());
    fprintf(stderr, "in Pointer::parse \"%s\"\n", in.get());

    const char* s = in.get();
    void* p; int chars;
    int n = sscanf(s, "%p%n", &p, &chars);
    if (n != 1) {
        fprintf(stderr, "could not parse %s for a pointer\n", s);
        return;
    }
    in.setPos(s + chars);

    out.writePointer(m_derefType, p);
}


std::unique_ptr<ais::Printer> ais::Pointer::makePrinter(Printer& parent) const {
    struct TPrinter : public Printer {
        TPrinter(Printer& parent) : Printer(parent) {}
        using Printer::operator();
        void operator()(const std::string& val) override {
            // apply sends the dereference type first
            out << "(" << val << "*) ";
        }

        void operator()(int idx) override {
            // and then the swizzled pointer
            if (idx > 0)
                out << "<" << idx << ">";
            else
                out << "0x0";
        }
    };
    return std::make_unique<TPrinter>(parent);
}

void ais::Pointer::applyL(Reader& reader, Visitor& f) const {
    auto derefType = reader.readString();
    f(derefType);
    
    auto idx = reader.readInt();
    f(idx);
}

std::string ais::Pointer::getTypeName() const {
    return "*";
}

ais::SmartPointer::SmartPointer(std::string pointerType, std::string derefType) :
    m_pointerType(std::move(pointerType)), m_derefType(std::move(derefType)) {}

void ais::SmartPointer::parseL(InterpretStream& in, Writer& out) const {
    fprintf(stderr, "in SmartPointer::parseL %s<%s> \"%s\"\n",
            m_pointerType.c_str(), m_derefType.c_str(), in.get());

    const char* s = in.get();

    void* p = nullptr; bool parsed = false;
    if (s) {
        // looks like "std::shared_ptr (count 1, weak 0) = {get() = 0x3bfc6f0}"
        // {_M_t = {<std::__uniq_ptr_impl<int, std::default_delete<int> >> =
        //     {_M_t = std::tuple containing = {[1] = 0x1871950,
        //      [2] = {<std::default_delete<int>> = {<No data fields>},
        // <No data fields>}}}, <No data fields>}}
        s = skipspace(s);

        if (strncmp(s,"std::shared_ptr",15) == 0) {
            if (auto s2 = strchr(s+15,')'))
                s = skipspace(s2+1);
        } else if (strncmp(s,"std::unique_ptr",15) == 0) {
            if (auto s2 = strstr(s+15,"> containing ")) {
                s = skipspace(s2+13);
            }
        }
        int chars, n = sscanf(s, "%p%n", &p, &chars);
        if (n == 1) {
            parsed = true;
            in.setPos(s+chars);
        }                
    }

    if (parsed) {
        out.writeString(m_pointerType);
        out.writeInt(1); // indicating success
        out.writePointer(m_derefType, p);
    } else {
        // evaluation failed
        out.writeString(m_pointerType);
        out.writeInt(0); // indicating failure
        out.writeString(m_derefType);
        return;
    }
}

void ais::SmartPointer::applyL(Reader& reader, Visitor& f) const {
    auto ptrType = reader.readString();
    auto gotValue = reader.readInt();
    auto refType = reader.readString();
    // I'm not sure if ptrType and refType should be treated as meta-data
    // or not.
    f(ptrType);
    f(refType);
    if (gotValue) {
        auto idx = reader.readInt();
        f(idx);
    } else {
        f(std::string{"<unavailable>"});
    }
}

std::unique_ptr<ais::Printer> ais::SmartPointer::makePrinter(Printer& parent) const {
    struct TPrinter : public Printer {
        TPrinter(Printer& parent) : Printer(parent) {}
        using Printer::operator();
        void operator()(int idx) override {
            if (idx > 0)
                out << "(<" << idx << ">)";
            else
                out << "0x0";
        }
        void operator()(const std::string& val) override {
            if (!cnt) out << val;   // smart pointer type
            else if (cnt == 1) out << "<" << val << ">"; // contained type
            // higher numbers are part of an array and it gets awfully
            // cluttered to show the type every time
            ++cnt;
        }
        int cnt = 0;
    };
    return std::make_unique<TPrinter>(parent);
}

std::string ais::SmartPointer::getTypeName() const {
    return "<*>";
}

ais::KokkosView::KokkosView(std::shared_ptr<ais::Type> elemType, int dim)
    : m_elemType(elemType), m_dim(dim) {}

ais::KokkosView::KokkosView() {}

std::string ais::KokkosView::getTypeName() const {
    return "Kokkos::View";
}

void ais::KokkosView::parseL(InterpretStream& in, Writer& out) const {
    // Currently, the KokkosView type is use to just return the header of
    // information about the view.   All extraction is done via element or
    // range (from..to) access.
    const char* s = in.get();
    while (*s) ++s;
    in.setPos(s);

    auto desc = std::ostringstream{};
    const auto& lens = m_span.sizes();

    desc << "Kokkos::View use ";
    for (size_t i=0; i<lens.size(); ++i) desc << (!i ? '(' : ',') << "0.." << lens[i]-1;
    desc << ") for full contents";

    out.writeString(desc.str());
}

void ais::KokkosView::applyL(Reader& reader, Visitor& f) const {
    Literal::applyL(reader, f);
}

std::unique_ptr<ais::Printer> ais::KokkosView::makePrinter(Printer& parent) const {
    return Literal::makePrinter(parent);
}

ais::RajaView::RajaView(std::shared_ptr<ais::Type> elem_type, int dim)
    : m_elem_type{elem_type}
    , m_dim{dim}
{}

ais::RajaView::RajaView()
    : m_elem_type{nullptr}
    , m_dim{-1}
{}

std::string ais::RajaView::getTypeName() const {
    return "RAJA::View";
}

int ais::RajaView::dim() const {
    return m_dim;
}

void ais::RajaView::setSpan(mdspan&& span) {
    m_span = std::move(span);
}

void ais::RajaView::parseL(InterpretStream& in, Writer& out) const {
    const char* s = in.get();
    while (*s) s++;
    in.setPos(s);

    auto msg = std::ostringstream{};
    if (m_span.isValid()) {
        msg << "RAJA::View; Use ";
        for (int i = 0; i < dim(); i++) {
            msg << (!i ? "(" : ", ")
                << 0 + m_span.offsets()[i]
                << ".."
                << (m_span.sizes()[i] - 1) + m_span.offsets()[i]
            ;
        }
        msg << ") for full contents.";
    } else {
        msg << "Raja::View; Failed to determine view dimensions. Try gdbmode.";
    }

    out.writeString(msg.str());
}

bool ais::RajaView::verboseDebug() {
    static const auto _enabled = [] {
        return (::getenv("GDB4HPC_RAJA_VERBOSE_DEBUG") != nullptr);
    }();
    return _enabled;
}

mdspan ais::RajaView::spanFromMembers(const std::string_view& input) {
    // expecting an input like the following (ellipses are abbreviations, not actually expected):
    // {m_data = 0x7ff102500000, m_layout = ... = {..., sizes = {7, 6}, strides = {6, 1}, ..., offsets = {-1, -1}}, ... }}
    //
    // the strategy is to skip delimiters from the input string and parse the
    // interesting numbers as we see them come up in the stream. we know sizes, strides,
    // and offsets will always be 1D list, so we don't need any special handling for {.

    std::vector<int> sizes, strides, offsets;

    std::vector<int>* current_number_bucket = nullptr;
    const char* delims = "{=, <>}";

    auto token_idx = input.find_first_not_of(delims);
    while (token_idx < input.size()) {
        auto end_idx = input.find_first_of(delims, token_idx);
        auto token = input.substr(token_idx, end_idx - token_idx);

        if (!std::isdigit(token.front()) && token.front() != '-') {
            // found a key, switch bucket
            if (token == "sizes") current_number_bucket = &sizes;
            else if (token == "strides") current_number_bucket = &strides;
            else if (token == "offsets") current_number_bucket = &offsets;
            else current_number_bucket = nullptr;
        } else if (current_number_bucket) {
            // found a number, add to current bucket
            auto num = int{0};
            auto [_, error] = std::from_chars(token.begin(), token.end(), num);
            if (error != std::errc())
                throw std::runtime_error(std::string{"Failed to parse number: "} + std::string{token});

            current_number_bucket->push_back(num);
        }

        token_idx = input.find_first_not_of(delims, end_idx);
    }

    auto ssizes = std::vector<size_t>{sizes.begin(), sizes.end()};
    auto sstrides = std::vector<size_t>{strides.begin(), strides.end()};
    return mdspan{ssizes, sstrides, offsets};
}

mdspan ais::RajaView::spanFromTemplateArgs(const std::string_view& input) {
    // expecting an input where the sizes and strides are in template arguments. the
    // template arguments will contain multiple camp::int_seq types describing the sizes
    // and strides of the view. these aren't labled in any way, so we have to assume an
    // order. the most common order is:
    //   camp::int_seq<Permutation Data> (not used here),
    //   camp::int_seq<Size data>,
    //   camp::int_seq<Stride data>
    //
    // a full input looks something like:
    //   RAJA::internal::TypedViewBase<int, int*, RAJA::detail::StaticLayoutBase_impl<long, camp::int_seq<long, 0l, 1l>, camp::int_seq<long, 16l, 16l>, camp::int_seq<long, 16l, 1l>, void>, camp::list<long, long> >
    // this is a 16x16 view.

    auto int_seq_token = "camp::int_seq";
    auto int_seq_idxs = std::vector<size_t>{};
    for (auto found = input.find(int_seq_token); found < input.size(); found = input.find(int_seq_token, found + strlen(int_seq_token))) {
        int_seq_idxs.push_back(found);
    }

    // assert 3 int_seqs
    if (int_seq_idxs.size() != 3) {
        return mdspan{};
    }

    auto parseIntSeq = [](const std::string_view& full_input, size_t int_seq_start) {
        // input is camp::int_seq<IdxType, size0, ..., sizen>.
        // extract <...>
        auto output = std::vector<size_t>{};
        auto int_seq_end = findExprMatch(full_input, int_seq_start + strlen("camp::int_seq")) + 1;
        auto int_seq = full_input.substr(int_seq_start, int_seq_end - int_seq_start);

        // note that cursor is relative to int_seq, not full_input.
        // start by jumping from int_seq right into the first comma in the template args,
        // skipping IdxType
        auto cursor = int_seq.find(',');
        while (cursor < int_seq.size()) {
            cursor = int_seq.find_first_not_of(", ", cursor); // skip whitespace
            auto cursor_end = int_seq.find(',', cursor);

            auto num =  int{0};
            // when passing the end char* here, if cursor_end == int_seq.size(),
            // then technically the [] has undefined behavior and will trip
            // sanitizers/asserts. std::from_chars's inut range is [start, end), (not [start, end])
            // so it's fine to pass the address of int_seq.size(), we just can't get it with [].
            auto [_, error] = std::from_chars(&int_seq[cursor], int_seq.data() + cursor_end, num);
            // default constructed errc means "no error"
            if (error != std::errc())
                return output;

            output.push_back(num);
            cursor = cursor_end;
        }
        return output;
    };

    // first int_seq is not related to bounds so it's skipped.
    auto sizes = parseIntSeq(input, int_seq_idxs[1]);
    auto strides = parseIntSeq(input, int_seq_idxs[2]);
    return mdspan{sizes, strides};
}

ais::RustStruct::RustStruct(const std::string& name) : ais::Struct(name) {}

void ais::RustStruct::parseL(InterpretStream& in, Writer& out) const {
    // The rust struct printer puts the name of the struct first
    auto s = in.get();
    if (s = strchr(s, '{'); !s) {
        fprintf(stderr, "did not find { in rust struct\n");
        return;
    }
    ++s;
    for (auto& [name, elem] : m_members) {
        s = skipspace(s);
        if (strncmp(s, name.c_str(), name.size()) == 0) {
            s += name.size();
        }
        if (*s == ':') ++s;
        else fprintf(stderr, "expected \":\" got \"%c\"\n", *s);

        s = skipspace(s);
        in.setPos(s);

        elem->parse(in, out);
        s = in.get();

        if (*s == ',') ++s;
    }
    s = skipspace(s);
    if (*s == '}') ++s;
    in.setPos(s);
}

//===========================================================================

ais::ReadBuffer::ReadBuffer() {}
ais::ReadBuffer::~ReadBuffer() {}

ais::WriteBuffer::WriteBuffer() {}
ais::WriteBuffer::~WriteBuffer() {}


//===========================================================================
void ais::Writer::writeInt(int val) { m_buffer->writeInt(val); }
void ais::Writer::writeLong(long val) { m_buffer->writeLong(val); }
void ais::Writer::writeLongLong(long long val) { m_buffer->writeLongLong(val); }
void ais::Writer::writeFloat(float val) { m_buffer->writeFloat(val); }
void ais::Writer::writeDouble(double val) { m_buffer->writeDouble(val); }
void ais::Writer::writeLongDouble(long double val) { m_buffer->writeLongDouble(val); }
void ais::Writer::writeString(const std::string& val) { m_buffer->writeString(val); }
void ais::Writer::writePointer(const std::string& type, void* val) {
    m_buffer->writePointer(type, val);
}
void ais::Writer::writeError(const std::string& err) {
    m_buffer->writeError(err);
}

void ais::Writer::writeType(const ais::Type& obj) {
    writeInt(obj.getTypeId(*this));
}

int ais::Writer::getTypeId(const std::string& typeName) {
    int idx = m_typeNameToIdx.size();
    auto [ loc, added ] = m_typeNameToIdx.insert( std::make_pair(typeName, idx));
    if ( added ) {
        writeInt(idx);
        writeString(typeName);
    } else {
        idx = loc->second;
    }
    return idx;
}

void ais::Writer::resetTypes() {
    m_typeNameToIdx.clear();
}

std::shared_ptr<ais::WriteBuffer> ais::Writer::getBuffer() const {
    return m_buffer;
}

//===========================================================================

ais::BufferReader::BufferReader(std::shared_ptr<ais::ReadBuffer> buffer) : m_buffer(buffer) {
    for (auto prim : m_primitives) {
        m_primitiveMap.insert(std::make_pair(prim->getTypeName(), prim));
    }
    for (auto arr : m_arrayTypes) {
        m_arrayTypeMap.insert(std::make_pair(arr->getTypeName(), arr));
    }
}

int ais::BufferReader::readInt() { return m_buffer->readInt(); }
long ais::BufferReader::readLong() { return m_buffer->readLong(); }
long long ais::BufferReader::readLongLong() { return m_buffer->readLongLong(); }
float ais::BufferReader::readFloat() { return m_buffer->readFloat(); }
double ais::BufferReader::readDouble() { return m_buffer->readDouble(); }
long double ais::BufferReader::readLongDouble() { return m_buffer->readLongDouble(); }
std::string ais::BufferReader::readString() { return m_buffer->readString(); }
bool ais::BufferReader::hasMoreData() { return m_buffer->hasMoreData(); }

std::shared_ptr<ais::Array>
ais::BufferReader::getArrayType(const std::string& typeName) const {
    if (auto p = typeName.find('['); p != std::string::npos) {
        if (auto pos = m_arrayTypeMap.find(typeName.substr(0,p+1));
            pos != m_arrayTypeMap.end()) {

            auto arrayType = pos->second;

            auto elemTypeIdx = std::stoi( typeName.substr(p+1));
            if (elemTypeIdx < 0 || elemTypeIdx >= int(m_types.size())) {
                throw ais::Error("array of unknown type");
            }

            return arrayType->makeTyped(m_types[elemTypeIdx]);
        }
    }
    return std::shared_ptr<ais::Array>{};
}

std::shared_ptr<ais::Type> ais::BufferReader::readType() {
    // At the point in stream we are will get either a type index of something
    // we already know about or a definition for a new type.   We keep reading
    // definitions until we get to a defined object type
    while (true) {
        auto idx = readInt();
        if (idx >= 0 && idx < int(m_types.size())) {
            return m_types[idx];
        } else if (idx != int(m_types.size())) {
            // the definition are expected to show up in order
            throw ais::Error("expected a type index");
        }

        // defining a new type
        auto typeName = readString();

        if (auto typeLoc = m_primitiveMap.find(typeName); typeLoc != m_primitiveMap.end()) {
            m_types.push_back(typeLoc->second);
        } else if (typeName[0] == '{') {
            // a struct or class
            m_types.push_back( std::make_shared<ais::Struct>(*this, typeName));
        } else if (auto arrayType = getArrayType(typeName)) {
            m_types.push_back(arrayType);
        } else {
            throw ais::Error("could not map a type name ");
        } 
    }
}
std::shared_ptr<ais::Type> ais::BufferReader::getType(int idx) {
    if (idx < 0 || idx >= int(m_types.size())) {
        throw ais::Error("invalid type index");
    }
    return m_types[idx];
}

// Specialized reader for making a decomposed array look like a single array.
ais::DecompReader::DecompReader(std::vector<std::unique_ptr<Reader>>&& subReaders,
                                std::shared_ptr<DecompIterator> iter) :
    m_subReaders(std::move(subReaders)), m_subIdx(0), m_prologue(true),
    m_decompIter(iter)
{}

std::shared_ptr<ais::Type> ais::DecompReader::readType() {
    if (!m_prologue) {
        return m_subReaders[m_subIdx]->readType();
    }

    // m_prologue means we are not getting the type stored on the stream
    // but the type we're presenting as the unified type of the
    // combined arrays.

    // not resetting m_prologue to false here because we still need to
    // provide the refIdx.

    // Read the type of the array fragments reported by each sub-reader
    std::shared_ptr<Type> subType;
    std::string subTypeName;
    for (auto&& s : m_subReaders) {
        // these are all supposed to be equivalent and an array type
        auto t = s->readType();
        [[maybe_unused]] auto refIdx = s->readInt();
        auto tn = t->getTypeName();
        if (!subType) {
            subType = t;
            subTypeName = std::move(tn);
        } else {
            if (subTypeName != tn) {
                throw ais::Error("ranks did not return same type for decomposed array");
            }
        }
    }

    // build the type for the combined array, there's more trickery coming so,
    // so we have the specialized type 'ais::DecompArray' to make the 'apply'
    // *look* like it's processing a single big array.
    auto arrayType = std::dynamic_pointer_cast<Array>(subType);
    if (!arrayType) {
        arrayType = std::make_shared<ais::Array1>(subType);
    }
    return std::make_shared<ais::DecompArray>(arrayType, m_decompIter);
}

int ais::DecompReader::readInt() {
    if (m_prologue) {
        // We are still in the part of the read cycle where we need to synthesize
        // the results to look like a standard stream.   Specifically, we are at
        // this line in ais::Any::apply:
        // int refIdx = reader.readInt();
        // actually every sub-reader has a reference index (and they should be equivalent)
        // We could cache the value we [[maybe_unused] back in DecompReader::readType
        // but since we only support top level decomposition, we can assume 0.
        m_prologue = false;
        return 0;
    } else {
        return m_subReaders[m_subIdx]->readInt();
    }
}


long ais::DecompReader::readLong() { return m_subReaders[m_subIdx]->readLong(); }
long long ais::DecompReader::readLongLong() { return m_subReaders[m_subIdx]->readLongLong(); }
float ais::DecompReader::readFloat() { return m_subReaders[m_subIdx]->readFloat(); }
double ais::DecompReader::readDouble() { return m_subReaders[m_subIdx]->readDouble(); }
long double ais::DecompReader::readLongDouble() { return m_subReaders[m_subIdx]->readLongDouble(); }
std::string ais::DecompReader::readString() { return m_subReaders[m_subIdx]->readString(); }

std::shared_ptr<ais::Type> ais::DecompReader::getType(int idx) {
    return m_subReaders[m_subIdx]->getType(idx);
}

bool ais::DecompReader::hasMoreData() {
    // Currently when doing a slicing, the read process isn't reading data from
    // ranks that had no data.   It really should be ammended to match, but
    // for now we can assume that there is not more data.
    return m_prologue;
    //return std::any_of(m_subReaders.begin(), m_subReaders.end(),
    //                 [](auto& s) { return s->hasMoreData(); });
}

void ais::DecompReader::setSubIdx(int idx) {
    if (idx < 0 || idx >= int(m_subReaders.size())) {
        throw ais::Error("stream index out of bounds");
    }
    m_subIdx = idx;
}

//===========================================================================

ais::ReaderStream::ReaderStream(std::unique_ptr<ais::Reader>&& rdr) :
    m_rdr(std::move(rdr)) {}

void ais::ReaderStream::apply(ais::Visitor& v) {
    while (m_rdr->hasMoreData()) {
        ais::Any t;
        t.apply(*m_rdr, v);
    }
    v.eof();
}


//===========================================================================

namespace ais {


    // When printing the data object, the data is handled by the most deeply nested
    // data type, i.e. the top of the Printer stack.   But we need a single object to
    // be passed around by 'apply', so PrinterMaster is a little class that forwards 
    // data elements to the top of the stack, and manages the stack itself by
    // intercepting the nest and unnest callbacks.
    class PrinterMaster : public Printer {
    public:
        PrinterMaster(const std::string& label, std::ostream& aout,
                      const Formatter& formatter) : Printer(aout), m_label(label),
                                                    m_formatter(formatter)
        {}
        void setTop(Printer* p) {
            if (p == this) {
                // nest/unnest is unbalanced.  I could throw an exception here, but
                // instead we'll just prevent an infinite loop, and maybe get some
                // weird output
                m_top = nullptr;
            } else {
                m_top = p;
            }
        }
        void operator()(bool val) override { m_top ? (*m_top)(val) : Printer::operator()(val); }
        void operator()(char val) override { m_top ? (*m_top)(val) : Printer::operator()(val); }
        void operator()(int val) override { m_top ? (*m_top)(val) : Printer::operator()(val); }
        void operator()(long val) override { m_top ? (*m_top)(val) : Printer::operator()(val); }
        void operator()(long long val) override { m_top ? (*m_top)(val) : Printer::operator()(val); }
        void operator()(float val) override { m_top ? (*m_top)(val) : Printer::operator()(val); }
        void operator()(double val) override { m_top ? (*m_top)(val) : Printer::operator()(val); }
        void operator()(long double val) override { m_top ? (*m_top)(val) : Printer::operator()(val); }
        void operator()(const std::string& val) override { m_top ? (*m_top)(val) : Printer::operator()(val); }
        void operator()(const ais::Error& val) override { m_top ? (*m_top)(val) : Printer::operator()(val); }
        void nest(const Type* t, int refIdx) override {
            if (m_top) m_top->nest(t, refIdx);
            else {
                if(!m_formatter.onePerLine) out << m_label;
                if (refIdx > 0) out << "\n  <" << refIdx << ">:";
                Printer::nest(t,-1);

                if (m_top) m_top->resetLabel(false);
            }
        }
        void unnest() override {
            if (m_top) m_top->unnest();
        }
        void addMetaInfo(const Type* t, const MetaInfo& info) {
            if (m_top) m_top->addMetaInfo(t, info);
        }

        void start(const Type* t) override { if (m_top) m_top->start(t); }
        void end() override { if (m_top) m_top->end(); }
        void startChild() override {}
        void endChild() override {}
        void resetLabel(bool force) override { if (force || !m_formatter.csv) m_label.clear(); }
        std::string getLabel() override { return m_label; }

        Formatter& getFormatter() { return m_formatter; }
            
    private:
        std::string m_label;
        Printer* m_top = nullptr;
        Formatter m_formatter;
    };
}

void ais::print(VStream& val, const std::string& label, const std::string& format, std::ostream& out) {
    try {
        PrinterMaster printer(label, out, Formatter(format));
        val.apply(printer);
    } catch (std::exception& err) {
        out << "caught error printing result " << err.what();
    }
}

//===========================================================================

void ais::TrackingVisitor::nest(const ais::Type* t, int refIdx) {
    m_idcs.push_back(std::make_pair(t,0));
    if (m_wrapped) m_wrapped->nest(t,refIdx);
}

void ais::TrackingVisitor::start(const ais::Type* t) {
    if (m_wrapped) m_wrapped->start(t);
}

void ais::TrackingVisitor::addMetaInfo(const ais::Type* t, const MetaInfo& info) {
    if (auto arr = std::get_if<ArrayMetaInfo>(&info)) {
        if (!m_idcs.empty()) m_idcs.back().second = arr->idx0;
    }
    if (m_wrapped) m_wrapped->addMetaInfo(t, info);
}
void ais::TrackingVisitor::unnest() {
    if (!m_idcs.empty()) m_idcs.pop_back();
    if (m_wrapped) m_wrapped->unnest();
    // incIdx();
}
void ais::TrackingVisitor::end() {
    incIdx();
    if (m_wrapped) m_wrapped->end();
}
void ais::TrackingVisitor::eof() {
    if (m_wrapped) m_wrapped->eof();
}

void ais::TrackingVisitor::incIdx() {
    // we are actually advancing along the level one
    // up from the leaf
    if (m_idcs.size() >= 2) ++((m_idcs.end()-2)->second);
    if (!m_idcs.empty()) m_idcs.back().second = 0;
}

std::string ais::TrackingVisitor::getIdx() const {
    std::ostringstream os;
    for (auto& [t, i] : m_idcs) {
        t->printIndex(i, os);
    }
    auto s = os.str();

    // handle fortran multi-dimension indices which need be to reveresed and
    // changed to commas, so: (-8)(4)(5) => (5,4,-8)
    if (s.find(")(") != std::string::npos) {
        static auto re = std::regex{R"(\(([0-9,-]+)\)\(([0-9]+)\))"};
        for (size_t prevLen = 0; s.size() != prevLen; ) {
            prevLen = s.size();
            s = std::regex_replace(s, re, "($2,$1)", std::regex_constants::match_default);
        }
    }
    
    return s;
}

void ais::Visitor::operator()(bool val) { operator()(int(val)); }
void ais::Visitor::operator()(char val) { operator()(int(val)); }
void ais::Visitor::operator()(int  val) { operator()(long(val)); }
void ais::Visitor::operator()(long val) { operator()(static_cast<long long>(val)); }
void ais::Visitor::operator()(float val) { operator()(double(val)); }
void ais::Visitor::operator()(double val) { operator()(static_cast<long double>(val)); }
void ais::Visitor::start(const Type*) {}
void ais::Visitor::addMetaInfo(const Type*, const MetaInfo&) {}
void ais::Visitor::end() {}
void ais::Visitor::nest(const Type*, int) {}
void ais::Visitor::unnest() {}
void ais::Visitor::eof() {}

namespace ais {

// cache type for working with binary ops.   
enum NoValType { NO_VAL };
    using PrimV = std::variant<NoValType,bool,char,int,long,long long, float, double, long double, std::string, ais::Error>;

// Class calling a binary function.
// BinaryOp is a template function taking two arguments *of the same type*
// Given arguments of potentially different type, this template tries to
// find a promotion using the normal C type promotions.
// Note: there is a proposed addition to C++ that would make this easy,
// but there isn't a non-homely approach supported in C++17
template<class BinaryOp>
class BinaryOpCaller
{
    template<class T>
    bool tryCall(T lhs, PrimV rvar, const std::string& idx) {
        if (T* rhs = std::get_if<T>(&rvar)) {
            m_op(lhs, *rhs);
            return true;
        } else {
            return false;
        }
    }
private:
    BinaryOp m_op;

public:
    BinaryOpCaller(BinaryOp op) : m_op(op) {}

    template<class T>
    void operator()(T lhs, PrimV rvar) {
        if (T* rhs = std::get_if<T>(&rvar)) {
            // matching types
            m_op(lhs, *rhs);
        } else {
            // find a common type via promotion
            std::visit([this,lhs](auto rhs) {
                   if constexpr (std::is_integral<T>::value) {
                       if constexpr (std::is_integral<decltype(rhs)>::value) {
                          if constexpr (sizeof(T) <= sizeof(decltype(rhs))) {
                              m_op(static_cast<decltype(rhs)>(lhs), rhs);
                          } else {
                              m_op(lhs, static_cast<T>(rhs));
                          }
                       } else if constexpr (std::is_floating_point<decltype(rhs)>::value) {
                          m_op(static_cast<double>(lhs), static_cast<double>(rhs));
                       } else {
                           throw ais::Error("unconvertible types for operation");
                       }
                   } else if constexpr (std::is_floating_point<T>::value) {
                       if constexpr (std::is_floating_point<decltype(rhs)>::value) {
                          if constexpr (sizeof(T) <= sizeof(decltype(rhs))) {
                              m_op(static_cast<decltype(rhs)>(lhs), rhs);
                          } else {
                              m_op(lhs, static_cast<T>(rhs));
                          }
                       } else if constexpr (std::is_integral<decltype(rhs)>::value) {
                          m_op(static_cast<double>(lhs), static_cast<double>(rhs));
                       } else {
                          throw ais::Error("unconvertible types for operation");
                       }
                   } else {
                       throw ais::Error("unconvertible types for operation");
                   }
            }, rvar);
        }
    }
};

// Single element queue connecting the lhs and rhs processing of a binary operation
// It stores one value as a std::variant with the available processing types
class BinaryVal {
public:
    // Get the value when it is available.  Returns PrimV{} as an EOF marker if
    // the other side has finished
    PrimV get(bool isLast) {
        std::unique_lock l(mutex);
        cv.wait(l, [this] { return this->hasVal || this->isDone; });

        if (isLast) isDone = true;

        if (hasVal) {
            auto v = val;
            hasVal = false;
            cv.notify_one();
            return v;
        } else {
            return PrimV{};
        }
    }

    // Set the value of the cache once it's empty.
    // v = PrimV{} is an EOF value that indicates that no additional values will be
    // set
    void set(PrimV v) {
        std::unique_lock l(mutex);
        cv.wait(l, [this] { return !this->hasVal || this->isDone; });
        val = v;
        if (v.index())
            hasVal = true;
        else
            isDone = true;
        cv.notify_one();
    }

private:
    std::mutex mutex;
    std::condition_variable cv;
    PrimV val;
    bool hasVal = false;
    bool isDone = false;
};

// Visitor for the rhs value in a binary operation
class BinaryRVal : public ais::FVisitor<BinaryRVal&> {
public:
    using Base = FVisitor<BinaryRVal&>;

    BinaryRVal(ais::BinaryVal& val) : Base(*this), m_val(val) {}
    void apply(auto v) { m_val.set(v); }
    void eof() override { m_val.set(ais::PrimV{}); }
private:
    ais::BinaryVal& m_val;
};

// Visitor for the lhs value in a binary operation    
class BinaryLVal : public ais::FVisitor<BinaryLVal&> {
public:
    using Base = ais::FVisitor<BinaryLVal&>;
    BinaryLVal(ais::BinaryVal& val, ais::BinaryOperator& op, ais::Visitor& out) :
        Base(*this), m_val(val), m_op(op), m_out(out) {}

    void apply(auto lval) {
        // call the binary operator when the rval is available
        auto rval = m_val.get(false);
        if (!rval.index()) throw ais::Error("Second operand is truncated");

        // note that the tracking is associated with the same operand that
        // generates the call so we know the tracking is looking at the same
        // location
        m_op(lval, rval);
    };

    void nest(const ais::Type* t, int refIdx=-1) override { m_out.nest(t,refIdx); }
    void start(const ais::Type* t) override { m_out.start(t); }
    void addMetaInfo(const ais::Type* t, const MetaInfo& info) override {
        m_out.addMetaInfo(t,info); }
    void end() override { m_out.end(); }
    void unnest() override { m_out.unnest(); }
    void eof() override {
        // if they're the same size, we should get the EOF marker from the rval
        auto val = m_val.get(true);
        if (val.index()) throw ais::Error("First operand is truncated");
        m_out.eof();
    }


private:
    ais::BinaryVal& m_val;
    ais::BinaryOpCaller<ais::BinaryOperator&> m_op;
    ais::Visitor& m_out;
};
    
void applyBinary(ais::VStream& val1, ais::VStream& val2, ais::BinaryOperator& op, ais::Visitor& out) {

    op.setOutput(&out);
    ais::BinaryVal val;
    BinaryRVal rhs{val};
    BinaryLVal lhs{val, op, out};

    // Thread operators for applying on each side.
    // Note that we're doing finally { eof() } so we don't have a thread
    // waiting for a result if its partner croaks.
    // (BinaryVal handles extra eof's gracefully)
    auto res1 = std::async([&lhs,&val1]() {
                               try { val1.apply(lhs); lhs.eof();
                               } catch (...) { lhs.eof(); throw; }});

    auto res2 = std::async([&rhs,&val2]() {
                               try { val2.apply(rhs); rhs.eof(); }
                               catch (...) { rhs.eof(); throw; }});

    res1.wait();
    res2.wait();

    // note: since the threads finished, we know noone is waiting
    // on val's condition variable.
    res1.get();
    res2.get();
}
    
}

namespace {

class PrimCompare;
struct PrimCompareOp {
    PrimCompareOp(PrimCompare& primCompare) : m_primCompare(primCompare) {}
    template<class T> void operator()(T a, T b);
    PrimCompare& m_primCompare;
};
    
// Binary operator to compare two streams of data
class PrimCompare : public ais::FBinaryOperator<PrimCompareOp, PrimCompareOp> {
public:

    // Any values that fail are passed along to \a diff to manage
    PrimCompare(double eqTol, double warnTol, ais::Difference& diff);

    // result is the bit mask for the 6 possible binary results that
    // have been seen
    std::bitset<6> getResult() const { return m_state; }

    // primary comparison operator
    template<class T>
    void apply(T a, T b) {
        using namespace ais;

        if constexpr (std::is_floating_point<T>::value) {
            auto d = a-b;
            auto s = EQ;
            if (d < -m_warnTol) s = LT;     
            else if (d < -m_eqTol) s = WARN_LT;
            else if (d > m_warnTol) s = GT;
            else if (d > m_eqTol) s = WARN_GT;
            m_state.set(s);
            if (s != EQ) m_diff.add(getIdx(),static_cast<long double>(a),
                                    static_cast<long double>(b));
        } else if constexpr (std::is_same<T,ais::Error>::value) {
          throw a;
        } else if (a == b) {
            m_state.set(EQ);
        } else if constexpr (std::is_arithmetic<T>::value) {
            m_diff.add(getIdx(), static_cast<long int>(a), static_cast<long int>(b));
            m_state.set(a < b ? LT : GT);
        } else {
            m_diff.add(getIdx(), a, b);
            m_state.set(a < b ? LT : GT);
        }
    }

    std::string getIdx() { return m_tracker.getIdx(); }
    ais::TrackingVisitor& getTracker() { return m_tracker; }

private:
    std::bitset<6> m_state;
    double m_eqTol;
    double m_warnTol;
    ais::Difference& m_diff;
    ais::TrackingVisitor m_tracker;
};

template<class T> inline void PrimCompareOp::operator()(T a, T b) {
    m_primCompare.apply(a,b);
}    
}


ais::Error::Error(const std::string& msg) : std::runtime_error(msg.c_str()) {}

ais::Printer::Printer(Printer& p) : parent(&p), master(p.master), out(p.out) {}
ais::Printer::Printer(std::ostream& o) :
    parent(nullptr), master(static_cast<PrinterMaster&>(*this)), out(o) {}

void ais::Printer::operator()(bool val) { out << (val ? "true" : "false"); }
void ais::Printer::operator()(char val) { out << val; }
void ais::Printer::operator()(int val) {  out << format() << val; }
void ais::Printer::operator()(long val) { out << format() << val; }
void ais::Printer::operator()(long long val) { out << format() << val; }
void ais::Printer::operator()(float val) {
    out << std::setprecision(std::numeric_limits<float>::digits10) << val;
}
void ais::Printer::operator()(double val) {
    out << std::setprecision(std::numeric_limits<double>::digits10) << val; }
void ais::Printer::operator()(long double val) {
    // aarch misreports the accuracy of a long double as 33 digits, but
    // empirically it show the same precision as x86_64, 18 digits
    //out << std::setprecision(std::numeric_limits<long double>::digits10) << val;
    out << std::setprecision(18) << val;
}
void ais::Printer::operator()(const std::string& val) {
        out << '"' << val << '"';
}
void ais::Printer::operator()(const ais::Error& val) {
    out << val.what();
}


void ais::Printer::nest(const ais::Type* t, int) {
    child = t->makePrinter(*this);
    master.setTop(child.get());
}
void ais::Printer::start(const ais::Type*) { if (parent) parent->startChild(); }
void ais::Printer::addMetaInfo(const ais::Type*, const MetaInfo&) {}
void ais::Printer::end() { if (parent) parent->endChild(); }
void ais::Printer::unnest() {
    if (parent) {
        master.setTop(parent);
        parent->child.reset(); // pop self off the top (and self-destruct)
    }
}
void ais::Printer::startChild() {}
void ais::Printer::endChild() {}

    
std::string ais::Printer::getLabel() {
    return ((parent == &master) ? master.getLabel() : "");
}

void ais::Printer::indent() {}

ais::Formatter& ais::Printer::format() { return master.getFormatter(); }

std::unique_ptr<ais::Printer> ais::Type::makePrinter(Printer& parent) const {
    return std::make_unique<ais::Printer>(parent);
}

ais::Formatter::Formatter(const std::string& format) {
    if (format.find('x') != std::string::npos) form = HEX;
    else if (format.find('o') != std::string::npos) form = OCT;
    else if (format.find('t') != std::string::npos) form = BIN;
    else if (format.find('z') != std::string::npos) form = PADDED_BIN;

    if (format.find('1') != std::string::npos) onePerLine = true;
    if (format.find('p') != std::string::npos) pretty = true;
    if (format.find("csv") != std::string::npos) csv = true;
}

ais::Formatter& operator<<(std::ostream& s, ais::Formatter& f) {
    f.os = &s;
    return f;
}

PrimCompare::PrimCompare(double eqTol, double warnTol, ais::Difference& diff) :
    ais::FBinaryOperator<PrimCompareOp,PrimCompareOp>(*this,*this),
    m_eqTol(eqTol), m_warnTol(warnTol), m_diff(diff), m_tracker(nullptr) {
 }

namespace ais {
    class BinaryOpStream : public VStream {
    public:
        BinaryOpStream(std::shared_ptr<ais::VStream> a,
                       std::shared_ptr<ais::VStream> b,
                       std::unique_ptr<ais::BinaryOperator>&& op) :
            m_a(a), m_b(b), m_op(std::move(op)) {}
        void apply(Visitor& v) override {
            applyBinary(*m_a, *m_b, *m_op, v);
        }
    private:
        std::shared_ptr<ais::VStream> m_a;
        std::shared_ptr<ais::VStream> m_b;
        std::unique_ptr<ais::BinaryOperator> m_op;
    };
}
            
std::shared_ptr<ais::VStream>
ais::makeBinaryOpStream(
    std::shared_ptr<ais::VStream> a,
    std::shared_ptr<ais::VStream> b,
    std::unique_ptr<ais::BinaryOperator>&& op) {
    return std::make_shared<ais::BinaryOpStream>(a,b,std::move(op));
}

// compare the contents of two data streams
std::bitset<6> ais::compare(VStream& a, VStream& b, Difference& diff,
                            const double eqTol, const double warnTol)
{
    PrimCompare res(eqTol, warnTol, diff);
    applyBinary(a, b, res, res.getTracker());
    return res.getResult();
}

ais::BinaryOperator::BinaryOperator() {}
void ais::BinaryOperator::operator()(bool a, bool b) {
    (*this)(char{a}, char{b});
}
void ais::BinaryOperator::operator()(char a, char b) {
    (*this)(int{a}, int{b});
}
void ais::BinaryOperator::operator()(int a, int b) {
    (*this)(long{a}, long{b});
}
void ais::BinaryOperator::operator()(long a, long b) {
    (*this)(static_cast<long long>(a), static_cast<long long>(b));
}
void ais::BinaryOperator::operator()(float a, float b) {
    (*this)(double{a}, double{b});
}
void ais::BinaryOperator::operator()(double a, double b) {
    (*this)(static_cast<long double>(a), static_cast<long double>(b));
}

TEST_CASE("RajaView::Bounds: Standard view") {
    const char* to_parse =
        "{"
          "m_data = 0x2273f0, "
          "m_layout = {"
              "static n_dims = 2, "
              "static limit = 2147483647, "
              "static stride_one_dim = -1, "
              "sizes = {4, 4}, "
              "strides = {4, 1}, "
              "inv_strides = {4, 1}, "
              "inv_mods = {4, 4}"
          "}"
        "}"
    ;

    auto bounds = ais::RajaView::spanFromMembers(to_parse);

    CHECK(bounds.isValid());

    REQUIRE(bounds.sizes().size() == 2);
    CHECK(bounds.sizes()[0] == 4);
    CHECK(bounds.sizes()[1] == 4);

    REQUIRE(bounds.strides().size() == 2);
    CHECK(bounds.strides()[0] == 4);
    CHECK(bounds.strides()[1] == 1);

    CHECK(bounds.offsets().size() == 2);
    CHECK(bounds.offsets()[0] == 0);
    CHECK(bounds.offsets()[1] == 0);
}

TEST_CASE("RajaView::Bounds: Offset view") {
    const char* to_parse =
        "{"
          "m_data = 0x7ff102500000, "
          "m_layout = {<RAJA::internal::OffsetLayout_impl<camp::int_seq<long, 0l, 1l>, int>> = {"
            "base_ = {"
              "static n_dims = 2, "
              "static limit = 2147483647, "
              "static stride_one_dim = -1, "
              "sizes = {7, 6}, "
              "strides = {6, 1}, "
              "inv_strides = {6, 1}, "
              "inv_mods = {7, 6}"
            "}, "
            "static stride_one_dim = -1, "
            "static n_dims = 2, "
            "offsets = {-1, -1}"
        "}, <No data fields>}}"
    ;

    auto bounds = ais::RajaView::spanFromMembers(to_parse);

    CHECK(bounds.isValid());

    REQUIRE(bounds.sizes().size() == 2);
    CHECK(bounds.size(0) == 7);
    CHECK(bounds.size(1) == 6);

    REQUIRE(bounds.strides().size() == 2);
    CHECK(bounds.stride(0) == 6);
    CHECK(bounds.stride(1) == 1);

    REQUIRE(bounds.offsets().size() == 2);
    CHECK(bounds.offset(0) == -1);
    CHECK(bounds.offset(1) == -1);
}

TEST_CASE("RajaView::Bounds: Permuted view") {
    const char* to_parse =
        "{"
          "static n_dims = 2, "
          "static limit = 2147483647, "
          "static stride_one_dim = -1, "
          "sizes = {3, 5}, "
          "strides = {1, 3}, "
          "inv_strides = {1, 3}, "
          "inv_mods = {3, 5}"
        "}"
    ;

    auto bounds = ais::RajaView::spanFromMembers(to_parse);

    CHECK(bounds.isValid());

    REQUIRE(bounds.sizes().size() == 2);
    CHECK(bounds.size(0) == 3);
    CHECK(bounds.size(1) == 5);

    REQUIRE(bounds.strides().size() == 2);
    CHECK(bounds.stride(0) == 1);
    CHECK(bounds.stride(1) == 3);

    REQUIRE(bounds.offsets().size() == 2);
    CHECK(bounds.offset(0) == 0);
    CHECK(bounds.offset(1) == 0);
}

TEST_CASE("RajaView::Bounds: TypedViewBase") {
    const char *to_parse =
      "{"
        "<RAJA::internal::ViewBase<int, int *,"
                                  "RAJA::detail::StaticLayoutBase_impl<"
                                      "long, camp::int_seq<long, 0l, 1l>,"
                                      "camp::int_seq<long, 16l, 16l>,"
                                      "camp::int_seq<long, 16l, 1l>, void>>> ="
            "{"
                "m_data = 0x7ffcf6292f50,"
                "m_layout = {static stride_one_dim = <optimized out>,"
                            "static n_dims = 2, static s_size = <optimized out>,"
                            "static s_size_noproj = <optimized out>}"
            "},"
        "static n_dims = <optimized out>"
      "}"
    ;

    auto bounds = ais::RajaView::spanFromTemplateArgs(to_parse);

    CHECK(bounds.isValid());

    REQUIRE(bounds.sizes().size() == 2);
    CHECK(bounds.size(0) == 16);
    CHECK(bounds.size(1) == 16);

    REQUIRE(bounds.strides().size() == 2);
    CHECK(bounds.stride(0) == 16);
    CHECK(bounds.stride(1) == 1);

    REQUIRE(bounds.offsets().size() == 2);
    CHECK(bounds.offset(0) == 0);
    CHECK(bounds.offset(1) == 0);
}
