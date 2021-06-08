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

#include <vector>
#include <string>
#include <algorithm>
#include <type_traits>

class ArgvStream {
public:
    ArgvStream() = default;
    template <typename... Args>
    ArgvStream(Args&&... args) { push(args...); };

    const std::vector<const char *> c_strs() const {
        std::vector<const char *> c_strs;
        std::transform(m_argv.begin(), m_argv.end(), 
            std::back_inserter(c_strs), [](const std::string &s){return s.c_str();});
        c_strs.push_back(nullptr);
        return c_strs;
    };

    void clear() {m_argv.clear();}

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
    template <typename... Args> void push(Args&&... args) {
        (*this << ... << args);
    };

    std::vector<std::string> m_argv;
};

#ifdef __cplusplus
extern std::string ArgsToString(int argc, char** argv);

extern "C" {
#endif

extern char **Str2Argv(const char *);
extern char **NewArgv(const char *, ...);
extern char **CopyArgv(char **);
extern char **AppendArgv(char **, char **);
extern char **AppendArgs(char **, ...);
extern char *ArgsToBuf(char **, int);
extern void FreeArgv(char **);

#ifdef __cplusplus
}
#endif
