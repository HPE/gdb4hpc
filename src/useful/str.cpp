/*********************************************************************************\
 * String support routines
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * Copyright 2010-2013, 2020 Hewlett Packard Enterprise Development LP.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Hewlett Packard Enterprise Development LP.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *
 * $HeadURL$
 * $Date$
 * $Rev$
 * $Author$
 *
 *********************************************************************************/

#include "config.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string_view>

#include "alloc.h"
#include "defs.h"
#include "str.h"

#include "unittest_macros.h"

std::string StripQuotes(const std::string_view& input) {
    if (input.size() <= 1 || input.front() != '"' || input.back() != '"') return std::string{input};
    return std::string{input.substr(1, input.size() - 2)};
}

TEST_CASE("StripQuotes") {
    using namespace std::string_literals;

    CHECK(StripQuotes("hello world") == "hello world"s);
    CHECK(StripQuotes("\"hello quoted world\"") == "hello quoted world"s);

    // edge cases
    CHECK(StripQuotes("") == ""s);
    CHECK(StripQuotes("\"") == "\""s);
    CHECK(StripQuotes("\"\"") == ""s);
    CHECK(StripQuotes("\"half quoted") == "\"half quoted"s);
    CHECK(StripQuotes("other half quoted\"") == "other half quoted\""s);
}

TEST_CASE("stripped") {
    using namespace std::literals::string_view_literals;

    CHECK(stripped("  hello\nworld  \t\r") == "hello\nworld"sv);
    CHECK(stripped("\n\t\r") == ""sv);
    CHECK(stripped("\n") == ""sv);
    CHECK(stripped("") == ""sv);
}

/*
** Convert an ascii number to a number. NOTE: silently truncates
** the number to fit in sizeof int.
*/

int StrToNum(char *str) {
    char c;
    char *p;
    int n;

    p = SkipNum(str);

    if (*p == '\0') return strtoul(str, NULL, 10);

    c = *p;
    *p = '\0';
    n = strtoul(str, NULL, 10);
    *p = c;

    return n;
}

char *TrimStr(char *str) {
    // Trim spaces and tabs from beginning:
    int i = 0, j;
    while ((str[i] == ' ') || (str[i] == '\t')) i++;

    if (i > 0) {
        for (j = 0; j < int(strlen(str)); j++) str[j] = str[j + i];
        str[j] = '\0';
    }

    // Trim spaces and tabs from end:
    i = strlen(str) - 1;
    while ((str[i] == ' ') || (str[i] == '\t')) {
        i--;
    }
    if (i < int(strlen(str) - 1)) {
        str[i + 1] = '\0';
    }
    return str;
}

char *TrimStrWithTarget(char *str, char target) {
    // Trim spaces and tabs from beginning:
    int i = 0, j;

    if (target != '\0') {
        while ((str[i] == ' ') || (str[i] == '\t') || (str[i] == target)) i++;
    } else {
        while ((str[i] == ' ') || (str[i] == '\t')) i++;
    }
    if (i > 0) {
        for (j = 0; j < int(strlen(str)); j++) str[j] = str[j + i];
        str[j] = '\0';
    }

    // Trim spaces and tabs from end:
    i = strlen(str) - 1;
    if (target != '\0') {
        while ((str[i] == ' ') || (str[i] == '\t') || (str[i] == target)) i--;
    } else {
        while ((str[i] == ' ') || (str[i] == '\t') || (str[i] == target)) i--;
    }
    if (i < int(strlen(str) - 1)) {
        str[i + 1] = '\0';
    }
    return str;
}

int CountOccurence(char val, char *str) {
    int i, result = 0;
    for (i = 0; i < int(strlen(str)); i++)
        if (str[i] == val) result++;
    return result;
}

char *ReplaceStr(const char *str, const char *oldstr, const char *newstr) {
    char *tmp = (char *)str;
    char *result;
    int found = 0;
    int length, reslen;
    int oldlen = strlen(oldstr);
    int newlen = strlen(newstr);
    int limit;

    while ((tmp = strstr(tmp, oldstr)) != NULL) found++, tmp += oldlen;

    length = strlen(str) + (found * (newlen - oldlen));
    result = (char *)Alloc(length + 1);
    tmp = (char *)str;
    limit = found;  // Countdown
    reslen = 0;     // Length of current result

    // Replace each oldstr found with newstr
    while ((limit-- > 0) && (tmp = strstr(tmp, oldstr)) != NULL) {
        length = (tmp - str);  // Number of chars to keep untouched
        strncpy(result + reslen, str, length);        // Original part kept
        strcpy(result + (reslen += length), newstr);  // Insert newstr
        reslen += newlen;
        tmp += oldlen;
        str = tmp;
    }
    strcpy(result + reslen, str);  // Copies last part and ends with '\0' char
    return result;
}

// Used to format lines of source code returned from GDBMI
void formatSourceLine(std::string &line) {
    // Strings come back with tabs newlines escaped. 
    // We want add those back in without messing with any escaped 
    // characters that might actually be in the contents of the line.

    // un-escape any \t, unless it's in a string literal
    bool inStringLiteral = false;
    for (size_t i = 0; i+1 < line.size(); i++) {
        if (line[i] == '\\') {
            if (line[++i] == 't' && !inStringLiteral) {
                line.replace(--i, 2, "\t");
            } 
        } else if (line[i] == '"') {
            inStringLiteral = !inStringLiteral;
        }
    }

    // remove the trailing \n, if present
    if (line.size() >= 2 && line.find("\\n", line.size() - 2) != std::string::npos) {
        line.replace(line.size() - 2, 2, "");
    }
};

int IsInteger(const char *s) {
    if (s == NULL) return 0;

    for (; *s != '\0'; s++)
        if (!isdigit((int)*s)) return 0;

    return 1;
}

std::string_view stripped(std::string_view sv) {
    const auto whitespace = std::string_view{" \n\r\t"};

    if (auto first = sv.find_first_not_of(whitespace); first != std::string_view::npos) {
        sv.remove_prefix(first);
    } else {
        // sv is only whitespace
        return "";
    }

    // if we get here, we know there is at least one non-whitespace character
    auto last = sv.find_last_not_of(whitespace);
    sv.remove_suffix(sv.size() - (last + 1));

    return sv;
}

MallocedCharPtr::MallocedCharPtr(char* ptr)
    : m_ptr(ptr, std::free)
{}

MallocedCharPtr::MallocedCharPtr(const char* fmt, ...)
    : m_ptr(nullptr, std::free)
{
    va_list args;
    va_start(args, fmt);

    char *cstr = nullptr;
    if (::vasprintf(&cstr, fmt, args) != -1) {
        m_ptr.reset(cstr);
    }

    va_end(args);
}

MallocedCharPtr::MallocedCharPtr(const char* fmt, va_list args)
    : m_ptr(nullptr, std::free)
{
    char *cstr = nullptr;
    if (::vasprintf(&cstr, fmt, args) != -1) {
        m_ptr.reset(cstr);
    }
}

char* MallocedCharPtr::release() {
    return m_ptr.release();
}

std::string_view MallocedCharPtr::get_view() const {
    if (m_ptr) return std::string_view(m_ptr.get());
    return std::string_view{};
}

std::string MallocedCharPtr::get_string() const {
    if (m_ptr) return std::string(m_ptr.get());
    return std::string{};
}
