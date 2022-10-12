/*********************************************************************************\
 * Argument definitions.
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * Copyright 2010-2021 Hewlett Packard Enterprise Development LP.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Hewlett Packard Enterprise Development LP.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *
 *********************************************************************************/

#pragma once

#include <string.h>

#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <type_traits>
#include <climits>
#include <iomanip>

class ArgvStream {
public:
    // declared to suppress warning when using variadic argument constructor with no args
    ArgvStream() = default;
   
    template <typename... Args>
    ArgvStream(Args&&... args) { (*this << ... << args); };

    ArgvStream(int argc, const char * const * argv) {
        for (int i=0; i<argc; ++i) *this << argv[i];
    }

    ArgvStream(int argc, const char** argv) {
        for (int i=0; i<argc; ++i) *this << argv[i];
    }

    ArgvStream(int argc, char** argv){
        for (int i=0; i<argc; ++i) *this << argv[i];
    }

    // returns a vector of const char *, passable to cti and others by calling .data()
    // on the resulting vector
    const std::vector<const char *> c_strs(size_t start) const;

    const std::vector<const char*> c_strs() const {
        return c_strs(0);
    }

    // parses a string as a list of arguments separated by spaces and adds them
    // to the current list of arguments
    void parseAndAppendArgs(const std::string_view& args);

    // returns arguments separated by delimiter as a string. if a stored argument
    // contains delimiter, it will be surrounded with double quotes.
    std::string asString(size_t start = 0, size_t stop_after = ULONG_MAX, char delim=' ') const;

    // allocates a buffer containing a copy of the arguments in the
    // ArgvStream, separated by spaces. 
    // the caller is responsible for freeing the returned buffer.
    char * createBuffer(size_t start = 0, size_t stop_after = ULONG_MAX) const {
        return strdup(asString(start, stop_after).c_str());
    }

    void clear() {m_argv.clear();}

    size_t argc() const {return m_argv.size();};

    std::string& operator[](size_t idx) {
        return m_argv[idx];
    }

    ArgvStream& operator<<(const ArgvStream &b) {
        for (const auto &arg : b.m_argv) {
            *this << arg;
        }
        return *this;
    }

    ArgvStream& operator<<(std::string str) {
        m_argv.push_back(str);
        return *this;
    }

    ArgvStream& operator<<(const std::string_view &view) {
        m_argv.push_back(std::string{view});
        return *this;
    }

    ArgvStream& operator<<(const char *cstr) {
        m_argv.push_back(std::string{cstr});
        return *this;
    }

    // argv must end with a nullptr
    ArgvStream& operator<<(const char * const * argv) {
        for (int i = 0; argv != nullptr && argv[i] != nullptr; i++) {
            m_argv.push_back(std::string{argv[i]});
        }
        return *this;
    }

    // call to_string for arithmetic types
    template <
        typename T,
        typename = std::enable_if_t<std::is_arithmetic<T>::value, T>
    >
    ArgvStream& operator<<(const T &t) {
        m_argv.push_back(std::to_string(t));
        return *this;
    };
private:

    std::vector<std::string> m_argv;
};

// allocates a buffer containing a copy of the arguments in argv, separated by spaces. 
// the caller is responsible for freeing the returned buffer.
char *ArgsToBuf(const char * const * argv, int argc);

// returns arguments separated by spaces as a string
std::string ArgsToString(const char * const * argv, int argc);
