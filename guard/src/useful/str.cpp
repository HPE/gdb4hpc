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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "alloc.h"
#include "defs.h"
#include "str.h"

char *StripQuotes(char *s) {
    char *e;

    if (s == NULL || *s != '"') return s;

    s++;
    e = &s[strlen(s) - 1];

    if (*e == '"') *e = '\0';

    return s;
}

char *skipspace(char *s) {
    if (s == NULL) {
        return NULL;
    }

    while (isspace((int)*s)) {
        s++;
    }

    return s;
}

char *SkipTo(char *str, char c) {
    if (str == NULL) return NULL;

    return strchr(str, c);
}

char *SkipNum(char *str) {
    if (str == NULL) return NULL;

    while (*str != '\0' && isdigit((int)*str)) str++;

    return str;
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

int IsInteger(char *s) {
    if (s == NULL) return 0;

    for (; *s != '\0'; s++)
        if (!isdigit((int)*s)) return 0;

    return 1;
}
