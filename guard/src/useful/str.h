/*********************************************************************************\
 * String support defs
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * Copyright 2010-2020-2021 Hewlett Packard Enterprise Development LP.
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
#include <string.h>
#include <ctype.h>
#include <type_traits>

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

char *StripQuotes(char *);

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
int IsInteger(char *);

inline char* StrdupOpt(const char* s) { return (s ? strdup(s) : nullptr); };
