/*******************************************************************************
 * Copyright 2022 Hewlett Packard Enterprise Development LP.
 ******************************************************************************/

#include "socstream.h"
#include "unittest_macros.h"

#include <cstring>
#include <limits>

namespace soc {

TEST_CASE("OSocStream/ISocStream") {
    auto ostr = OSocStream(DBGCMD_INVOKE);

    SUBCASE("Basic data types") {
        ostr << std::numeric_limits<int>::min()
             << std::numeric_limits<int>::max()
             << std::numeric_limits<unsigned int>::min()
             << std::numeric_limits<unsigned int>::max()
             << std::numeric_limits<long>::min()
             << std::numeric_limits<long>::max()
             << std::numeric_limits<size_t>::min()
             << std::numeric_limits<size_t>::max()
             << true << false
             << "scooby" << std::string("doo")
        ;

        auto wire = ostr.str();
        auto istr = ISocStream(wire);

        CHECK(istr.getCmd() == DBGCMD_INVOKE);

        int imin = 1;
        int imax = 1;
        unsigned int uimin = 1;
        unsigned int uimax = 1;
        size_t lmin = 1; // items packed as long are extracted as size_t
        size_t lmax = 1;
        size_t stmin = 1;
        size_t stmax = 1;
        bool t = false;
        bool f = true;
        std::string cstr; // items packed as char* are extracted as std::string
        std::string str;

        istr.get(imin, imax, uimin, uimax, lmin, lmax, stmin, stmax, t, f, cstr, str);

        CHECK(imin == std::numeric_limits<int>::min());
        CHECK(imax == std::numeric_limits<int>::max());
        CHECK(uimin == std::numeric_limits<unsigned int>::min());
        CHECK(uimax == std::numeric_limits<unsigned int>::max());
        CHECK(lmin == std::numeric_limits<long>::min());
        CHECK(lmax == std::numeric_limits<long>::max());
        CHECK(stmin == std::numeric_limits<size_t>::min());
        CHECK(stmax == std::numeric_limits<size_t>::max());
        CHECK(t == true);
        CHECK(f == false);
        CHECK(cstr == "scooby");
        CHECK(str == "doo");
    }

    SUBCASE("Optional strings") {
        char* here = strdup("here");
        char* notHere = nullptr;

        ostr << here << notHere;

        auto wire = ostr.str();
        auto istr = ISocStream(wire);

        OptionalString hereStr;
        OptionalString notHereStr;

        istr.get(hereStr, notHereStr);

        CHECK(hereStr.str == "here");
        CHECK(strcmp(static_cast<const char*>(hereStr), "here") == 0);
        CHECK(notHereStr.str == "");
        CHECK(static_cast<const char*>(notHereStr) == nullptr);

        free(here);
    }

    SUBCASE("Args") {
        char* three = strdup("3");
        char* apples = strdup("apples");
        char* minus = strdup("minus");
        char* five = strdup("5");
        char* is = strdup("is");
        char* negtwo = strdup("-2");

        char* argv[] = {three, apples, minus, five, apples, is, negtwo, apples};
        int argc = 8;

        ostr << std::make_pair(argc, argv);

        auto wire = ostr.str();
        auto istr = ISocStream(wire);

        auto cmdargs = CmdArgs();
        istr.get(cmdargs);

        CHECK(cmdargs.argc == 8);
        CHECK(std::string(cmdargs.argv[0]) == "3");
        CHECK(std::string(cmdargs.argv[1]) == "apples");
        CHECK(std::string(cmdargs.argv[2]) == "minus");
        CHECK(std::string(cmdargs.argv[3]) == "5");
        CHECK(std::string(cmdargs.argv[4]) == "apples");
        CHECK(std::string(cmdargs.argv[5]) == "is");
        CHECK(std::string(cmdargs.argv[6]) == "-2");
        CHECK(std::string(cmdargs.argv[7]) == "apples");

        free(three);
        free(apples);
        free(minus);
        free(five);
        free(is);
        free(negtwo);
    }
}

OSocStream::OSocStream(dbgcmd cmd) : m_cmd(cmd) {}

OSocStream& OSocStream::operator<<(int i) { m_str << " " << i; return *this; }
OSocStream& OSocStream::operator<<(unsigned int i) { m_str << " " << i; return *this; }
OSocStream& OSocStream::operator<<(long i) { m_str << " " << i; return *this; }
OSocStream& OSocStream::operator<<(size_t i) { m_str << " " << i; return *this; }


OSocStream& OSocStream::operator<<(const char*s) {
    if (s) {
        m_str << " " << strlen(s) << " " << s;
    } else {
        m_str << " 0 ";
    }
    return *this;
}
OSocStream& OSocStream::operator<<(const std::string& s) {
    m_str << " " << s.size() << " " << s;
    return *this;
}
OSocStream& OSocStream::operator<<(const std::pair<int,const char* const*>& args) {
    int argc = args.first;
    *this << argc;
    for (int i=0; i<argc; ++i) {
        *this << args.second[i];
    }
    return *this;
}

std::string OSocStream::str() {
    auto s = m_str.str();
    m_str.str("");
    m_str << m_cmd;
    if (!s.empty()) {
        // the commands are/were sent like a single string; that is preceded by the
        // length as usual.   I don't quite want to call the normal << operator because s has
        // an extra space on the front.
        m_str << " " << s.size()-1 << s;
    }
    // else, commands with no arguments just send the command id.

    return m_str.str();
}

ISocStream::ISocStream(const std::string& str) : m_str(str) {
    int cmdId;
    m_str >> cmdId;
    m_cmd = static_cast<dbgcmd>(cmdId);

    // the protocol has put the arguments into a string -- if there are any.   We're not currently
    // using that to skip over the result, so here we'll just read the arg len and ignore it.
    int len;
    m_str >> len;  // this command will fail for commands with no arguments, but it's harmless
}

ISocStream& ISocStream::operator>>(int& i) { m_str >> i; return *this; }
ISocStream& ISocStream::operator>>(unsigned int& ui) { m_str >> ui; return *this; }
ISocStream& ISocStream::operator>>(bool& b) { int i; m_str >> i; b = i; return *this; }
ISocStream& ISocStream::operator>>(size_t& sz) { m_str >> sz; return *this; }
ISocStream& ISocStream::operator>>(std::string& s) {
    size_t len = 0;
    m_str >> len;
    
    char c = '\0';
    m_str.get(c); // the space between the length an the string
    s.resize(len);
    if (len) m_str.read(&s[0], len);

    return *this;
}

ISocStream& ISocStream::operator>>(OptionalString& s) {
    return *this >> s.str;
}

ISocStream& ISocStream::operator>>(CmdArgs& args) {
    int n;
    *this >> n;
    args.argc = n;

    if (n <= 0) {
        args.argv = nullptr;
        return *this;
    }
    args.argv = (char**)malloc(n * sizeof(char*));

    for (int i=0; i<n; ++i) {
        std::string s;
        *this >> s;
        args.argv[i] = strdup(s.c_str());
    }

    return *this;
}

CmdArgs::~CmdArgs()
{
    for (int i=0; i<argc; ++i) free(argv[i]);
    free(argv);
}

} // namespace soc
