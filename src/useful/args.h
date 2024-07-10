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
#include <optional>

class ArgvStream {
public:
    // declared to suppress warning when using variadic argument constructor with no args
    ArgvStream() = default;

    template <typename... Args>
    explicit ArgvStream(Args&&... args) { (*this << ... << args); };

    explicit ArgvStream(int argc, const char * const * argv) {
        for (int i=0; i<argc; ++i) *this << argv[i];
    }

    explicit ArgvStream(int argc, const char** argv) {
        for (int i=0; i<argc; ++i) *this << argv[i];
    }

    explicit ArgvStream(int argc, char** argv){
        for (int i=0; i<argc; ++i) *this << argv[i];
    }

    // returns contained arguments in vector of const char * with a nullptr as
    // the last entry. the result is passable to cti and others by calling
    // .data() on it.
    std::vector<const char *> c_strs(size_t start) const;

    std::vector<const char*> c_strs() const {
        return c_strs(0);
    }

    // parses a string as a list of arguments separated by spaces and adds them
    // to the current list of arguments
    void parseAndAppendArgs(std::string_view args);

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

    const std::string& operator[](size_t idx) const {
        return m_argv[idx];
    }

    ArgvStream& operator<<(const ArgvStream &b) {
        for (const auto &arg : b.m_argv) {
            *this << arg;
        }
        return *this;
    }

    ArgvStream& operator<<(std::string_view view) {
        m_argv.push_back(std::string{view});
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

// simply append with spaces without adding and quote or quote escapes
std::string ArgsToStringUnquoted(const char * const * argv, int argc);

enum class OptstringScanningMode {
    PermuteArgv,
    PosixlyCorrect,
    HandleAll,
};

// take an array of getopt_long `options` and return a string to use as the
// optstring argument of the getopt_long call that uses them.
// `options` must end with the usual {0, 0, 0, 0} entry.
//
// `mode` picks one of the available getopt_long scanning modes:
//
// - `PermuteArgv` means getopt will permute argv as it scans options so that
//   all non-options end up on the end of argv. this is the default behavior of getopt.
// - `PosixlyCorrect` means getopt will stop on the first non-option argument. the
//   returned string will begin with "+".
// - `HandleAll` means getopt will handle each non-option argument as if it was the
//   argument to an option with the character code 1. the returned string will begin
//   with "-".
//
// `differentiate_missing_args` means that getopt will return ':' when it encounters flag that was
// valid but missing an argument instead of the usual '?' it uses for all errors.
// the returned string will begin with ":" (or "+:", "-:" depending on the `mode`.).
//
// returns the corresponding optstring on success. throws on failure. the
// function can fail in certain edge cases that relate to the underlying getopt
// implementation:
//
// - multiple options having the same long or short name as another entry
// - "W:" in the optstring is a special flag meaning "-W foo" will be treated as "--foo".
//   this means that a long option with a required argument and character code W is not
//   allowed and will result in a nullopt result.
std::string optstring_from_longopts(const struct option options[], OptstringScanningMode mode, bool differentiate_missing_args);
