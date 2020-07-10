#include <tuple>
#include <string>
#include <iostream>
#include <sstream>
#include <string.h>

// mocking just enough commands to be interesting
enum dbgcmd {
    DBGCMD_INVOKE,
    DBGCMD_STARTUP,
    DBGCMD_SETLINEBREAK
};

//---------------------------------------------------------------------------
// This is nearly all the stuff need to replace CommandToSocProto; I originally considered pulling
// in tuples, but a variadic template function seems be be less mental overhead because we never
// need that kind of collection
//   - not that we can't just use a stringstream because the protocol puts the length before any string.
class OSocStream {
private:
    std::ostringstream m_str;

public:
    OSocStream(dbgcmd id);
    OSocStream& operator<<(int i);
    OSocStream& operator<<(double d);
    OSocStream& operator<<(const char*s);

    std::string str();
};

OSocStream::OSocStream(dbgcmd cmd) {
    m_str << cmd;
}

OSocStream& OSocStream::operator<<(int i) { m_str << " " << i; return *this; }
OSocStream& OSocStream::operator<<(double d) { m_str << " " << d; return *this; }
OSocStream& OSocStream::operator<<(const char*s) { m_str << " " << strlen(s) << " " << s; return *this; }

std::string OSocStream::str() { return m_str.str(); }


template<typename ...Targs>
std::string CommandToSocProto(dbgcmd cmd, Targs&&... args) {
    OSocStream str(cmd);
    (void) (str << ... << std::forward<Targs>(args));
    return str.str();
}

// And the rough equivalent for SocProtoToCommand.   This is where gdb4hpc create a big dbgcommand object with
// 30 sub-types nestled in a union with destructors and accessors etc.   And all is really has to do
// pull arguments out of the string and call a function.
class ISocStream {
private:
    std::istringstream m_str;
public:
    ISocStream(const std::string& str);

    ISocStream& operator>>(dbgcmd& cmd);
    ISocStream& operator>>(int& i);
    ISocStream& operator>>(double& d);
    ISocStream& operator>>(std::string& s);
};

ISocStream::ISocStream(const std::string& str) : m_str(str) {}

ISocStream& ISocStream::operator>>(dbgcmd& cmd) { int i; m_str >> i; cmd = static_cast<dbgcmd>(i); return *this; }
ISocStream& ISocStream::operator>>(int& i) { m_str >> i; return *this; }
ISocStream& ISocStream::operator>>(double& d) { m_str >> d; return *this; }
ISocStream& ISocStream::operator>>(std::string& s) {
    int len;
    m_str >> len;
    std::cout << "len = " << len << "\n";
    char c;
    m_str.get(c); // the space between the length an the string
    s.resize(len);
    m_str.read(&s[0], len);
    return *this;
}

template<typename ...Targs>
void SocProtoToArgs(ISocStream& str, Targs&&... args) {
    (str >> ... >> std::forward<Targs>(args));
}

//---------------------------------------------------------------------------
// mocking up the server side function that would be in dbgsrv
// -- and yes the arguments don't match the real functions
//---------------------------------------------------------------------------
static void invoke(const char* name, int degrees, double temp) {
    std::cout << "in invoke name=" << name << " degrees=" << degrees << " temp=" << temp << "\n";
}
static void startup(const char* name) {
    std::cout << "in startup name=" << name << "\n";
}
static void setlinebreak() {
    std::cout << "in setlinebreak\n";
}

//---------------------------------------------------------------------------
// A basic structure of dbgsrv_dispatch_cmd.   I haven't tried to fix the big ugly switch statement,
// but this would at least be the *only* big ugly switch statement.
// There isn't any error checking, but there isn't any error checking in the current form either...
// Note that we have to use explicit variable typing all over this function because the compiler
// doesn't know the types - it's only implicit in the protocol.
//---------------------------------------------------------------------------
static void dispatch(const std::string& socstring) {
    ISocStream str(socstring);
    dbgcmd cmd;
    str >> cmd;
    switch (cmd) {
    case DBGCMD_INVOKE:
        {
            std::string name; int degrees; double temp;
            SocProtoToArgs(str, name, degrees, temp);
            invoke(name.c_str(), degrees, temp);
        }
        break;

    case DBGCMD_STARTUP:
        {
            std::string name;
            SocProtoToArgs(str, name);
            startup(name.c_str());
        }
        break;
    case DBGCMD_SETLINEBREAK:
        setlinebreak();
        break;
    }
}            
    

int main(int argc, char* argv[]) 
{
    auto name = "sunshine";
    auto degrees = 94;
    auto temp = 98.6;

    // this would be the client side
    auto res = CommandToSocProto(DBGCMD_INVOKE, name, degrees, temp);
    std::cout << res << "\n"; // the pipe
    dispatch(res);  // the server side

    res = CommandToSocProto(DBGCMD_STARTUP, name);
    std::cout << res << "\n";
    dispatch(res);

    res = CommandToSocProto(DBGCMD_SETLINEBREAK);
    std::cout << res << "\n";
    dispatch(res);
    
    return 0;
}

/* the test output
./test
0 8 sunshine 94 98.6
len = 8
in invoke name=sunshine degrees=94 temp=98.6
1 8 sunshine
len = 8
in startup name=sunshine
2
in setlinebreak
*/
