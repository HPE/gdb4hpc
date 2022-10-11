/*******************************************************************************
 * Copyright 2022 Hewlett Packard Enterprise Development LP.
 ******************************************************************************/
#pragma once

// SocProto stream classes for use in converting things to and from the protocol
// used to communicate between the client and the server.
//
// The main functions to convert things to/from SocProto are declared in
// dbgformat.h - SocProtoToAif, SocProtoToEvent, etc. The classes here are
// used in the implementations of those functions to do the work.

#include <sstream>

#include "dbgcmd.h"

namespace soc {

// String assembler class for CommandToSocProto -- it just needs to define an
// overrider for every type used as a command argument. Prefer using functions
// in dbgformat.h to using this class directly.
class OSocStream {
private:
    dbgcmd m_cmd;
    std::ostringstream m_str;

public:
    OSocStream(dbgcmd id);

    OSocStream& operator<<(int i);
    OSocStream& operator<<(unsigned int i);
    OSocStream& operator<<(long i);
    OSocStream& operator<<(size_t i);
    OSocStream& operator<<(const char*s);
    OSocStream& operator<<(const std::string& s);
    OSocStream& operator<<(const std::pair<int,char**>& args);

    // retrieve the result string
    std::string str();
};

// Holder struct for argc/argv.
// (Doing a one-off as the stuff in useful/args.h doesn't manage memory)
struct CmdArgs {
    int argc;
    char** argv;
    CmdArgs() : argc(0), argv(nullptr) {}
    ~CmdArgs();
    CmdArgs(const CmdArgs&) = delete;
    CmdArgs& operator=(const CmdArgs&) = delete;
};

struct OptionalString {
    std::string str;
    operator const char*() { return str.empty() ? nullptr : str.c_str(); }
};

// Socket prototype string reader.
class ISocStream {
    dbgcmd m_cmd;
    std::istringstream m_str;
public:
    // Prepare to read the socket string
    ISocStream(const std::string& str);

    // Get the index of the command
    dbgcmd getCmd() { return m_cmd; }

    // Get the rest of the commands; the caller just has to
    // declare the arguments with the correct type, and it will
    // fill it in.
    // Fixme - add error checking here.
    template<typename ...Targs>
    void get(Targs&&... args) {
        (void) (*this >> ... >> std::forward<Targs>(args));
    };

private:
    // read typed arguments from the string.  When a new type of data is
    // passed through the socket, we need to add the appopriate data type here.
    ISocStream& operator>>(int& i);
    ISocStream& operator>>(unsigned int& ui);
    ISocStream& operator>>(bool& b);
    ISocStream& operator>>(size_t& sz);
    ISocStream& operator>>(std::string& s);
    ISocStream& operator>>(OptionalString& s);
    ISocStream& operator>>(CmdArgs& args);
};

} // namespace soc
