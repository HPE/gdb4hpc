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

    friend ArgvStream& operator<<(ArgvStream& ab, const std::string &str);
    friend ArgvStream& operator<<(ArgvStream& ab, const char *cstr);
    friend ArgvStream& operator<<(ArgvStream& ab, char *cstr);
    friend ArgvStream& operator<<(ArgvStream& ab, const char *argv[]);
    friend ArgvStream& operator<<(ArgvStream& ab, char *argv[]);
    friend ArgvStream& operator<<(ArgvStream& ab, const int &i);
private:
    template <typename T, typename... Args> void push(const T &t, Args&&... args) {
        *this << t;
        push(args...);
    };
    void push() { /* end variadic arg recursion */ };

    std::vector<std::string> m_argv;
};

ArgvStream& operator<<(ArgvStream& ab, const std::string &str);
ArgvStream& operator<<(ArgvStream& ab, const char *cstr);
ArgvStream& operator<<(ArgvStream& ab, char *cstr);
// argv must end with a nullptr
ArgvStream& operator<<(ArgvStream& ab, const char *argv[]);
// argv must end with a nullptr
ArgvStream& operator<<(ArgvStream& ab, char *argv[]);
ArgvStream& operator<<(ArgvStream& ab, const int &i);

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
