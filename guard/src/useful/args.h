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

class ArgvStream {
public:
    // declared to suppress warning when using variadic argument constructor with no args
    ArgvStream() = default;
    
    template <typename... Args>
    ArgvStream(Args&&... args) { (*this << ... << args); };

    // returns a vector of const char *, passable to cti and others by calling .data()
    // on the resulting vector
    const std::vector<const char *> c_strs() const {
        std::vector<const char *> c_strs;
        std::transform(m_argv.begin(), m_argv.end(), 
            std::back_inserter(c_strs), [](const std::string &s){return s.c_str();});
        c_strs.push_back(nullptr);
        return c_strs;
    };

    // parses a string as a list of arguments separated by spaces and adds them
    // to the current list of arguments
    void parseAndAppendArgs(const std::string args) {
        std::stringstream arg;

        for (size_t i = 0; i < args.size(); i++) {
            if (isspace(args[i])) continue;
            
            arg.str("");
            arg.clear();
            
            bool single_quote = false;
            bool double_quote = false;
            bool backslash = false;

            for (; i < args.size(); i++) {
                if (isspace(args[i]) && !single_quote && !double_quote && !backslash) break;

                if (backslash) {
                    backslash = false;
                } else if (args[i] == '\\') {
                    backslash = true;
                } else if (single_quote && args[i] == '\'') {
                    single_quote = false;
                } else if (double_quote && args[i] == '"') {
                    double_quote = false;
                } else {
                    if (args[i] == '\'')
                        single_quote = true;
                    else if (args[i] == '"')
                        double_quote = true;
                }

                arg << args[i];
            }

            if (arg.tellp() != 0) {
                m_argv.push_back(arg.str());
            }
        }
    }

    // returns arguments separated by spaces as a string
    std::string asString(size_t start = 0, size_t stop_after = ULONG_MAX) const {
        std::stringstream ss;
        std::string space = "";

        for (size_t i = start; i < m_argv.size() && stop_after > 0; i++, stop_after--) {
            ss << space << m_argv[i];
            space = " ";
        }

        return ss.str();
    }

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

    ArgvStream& operator<<(const std::string &str) {
        m_argv.push_back(str);
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
