/*********************************************************************************\
 * String support defs
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * Copyright 2010-2025 Hewlett Packard Enterprise Development LP.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Hewlett Packard Enterprise Development LP.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *
 *********************************************************************************/

#pragma once

#include <string>
#include <string_view>
#include <string.h>
#include <ctype.h>
#include <type_traits>
#include <memory>

/* 
 * Most of these string processing functions are truly const, but sometimes it is
 * useful to use them with char * that you intend to eventually modify. To handle
 * both cases without duplicating code, we make these functions templates, but
 * explicitly only allow them to be used with char * or const char *.
*/

template <typename T>
T* skipspace(T* s) {
    static_assert(std::is_same<char, std::decay_t<T>>::value, "Only char * or const char * allowed.");
    
    if (s == NULL) {
        return NULL;
    }

    while (isspace((int)*s)) {
        s++;
    }

    return s;
}

template <typename T>
T* skipstring(T* res_buf) {
    static_assert(std::is_same<char, std::decay_t<T>>::value, "Only char * or const char * allowed.");
    
    char *endptr = NULL;

    res_buf = skipspace(res_buf);

    // get the length of the string - Note that this can be 0
    int len = (int)strtol(res_buf, &endptr, 10);
    // set res_buf past string
    // + 1 for the space in between the size and string
    res_buf = endptr + len + 1;

    return res_buf;
}

std::string StripQuotes(const std::string_view& input);

template <typename T>
T* SkipTo(T* str, char c) {
    static_assert(std::is_same<char, std::decay_t<T>>::value, "Only char * or const char * allowed.");
    
    if (str == NULL) return NULL;

    return strchr(str, c);
}

template <typename T>
T* SkipNum(T *str) {
    static_assert(std::is_same<char, std::decay_t<T>>::value, "Only char * or const char * allowed.");
    
    if (str == NULL) return NULL;

    while (*str != '\0' && isdigit((int)*str)) str++;

    return str;
}

int StrToNum(char *);
char *TrimStr(char *);
char *TrimStrWithTarget(char *, char);
int CountOccurence(char, char *);
char *ReplaceStr(const char *, const char *, const char *);
void formatSourceLine(std::string &line);
int IsInteger(const char *);

inline char* StrdupOpt(const char* s) { return (s ? strdup(s) : nullptr); };

// return sv with leading and trailing whitespace removed
std::string_view stripped(std::string_view sv);

struct MallocedCharPtr {
    MallocedCharPtr(char* ptr = nullptr);
    // These constructors create the string with ::vasprintf
    explicit MallocedCharPtr(const char* fmt, ...) __attribute__((format (printf,2,3)));
    explicit MallocedCharPtr(const char* fmt, va_list args);

    MallocedCharPtr(MallocedCharPtr&& other) = default;
    MallocedCharPtr& operator=(MallocedCharPtr&& other) = default;

    ~MallocedCharPtr() = default;

    // if contianed pointer is nullptr, returns empty string view.
    std::string_view get_view() const;
    // if container pointer is nullptr, returns empty string.
    std::string get_string() const;

    // `ptr` must be allocated with malloc!
    void reset(char* ptr = nullptr) { m_ptr.reset(ptr); }
    char* get() { return m_ptr.get(); }
    const char* get() const { return m_ptr.get(); }
    char* release();

    explicit operator bool() const noexcept { return static_cast<bool>(m_ptr); }

    // lazily filling out the rest of the wrapper functions as needed...

    std::unique_ptr<char, decltype(&std::free)> m_ptr;
};
