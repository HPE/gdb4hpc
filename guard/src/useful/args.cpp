/*********************************************************************************\
 * Argument routines
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * Copyright 2010-2013, 2020-2021 Hewlett Packard Enterprise Development LP.
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
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "alloc.h"
#include "args.h"

#define INITIAL_MAXARGC 8

char **Str2Argv(const char *s) {
    int squote = 0;
    int dquote = 0;
    int bsquote = 0;
    int argc = 0;
    int maxargc = 0;
    char *arg;
    char *copybuf;
    char **argv = NULL;
    char **nargv;

    if (s == NULL) return argv;

    copybuf = (char *)Alloc(strlen(s) + 1);

    do {
        while (isspace((int)*s)) s++;

        if ((maxargc == 0) || (argc >= (maxargc - 1))) {
            if (argv == NULL) {
                maxargc = INITIAL_MAXARGC;
                nargv = (char **)Alloc(maxargc * sizeof(char *));
            } else {
                maxargc *= 2;
                nargv = (char **)Resize(argv, maxargc * sizeof(char *));
            }

            argv = nargv;
            argv[argc] = NULL;
        }

        arg = copybuf;

        while (*s != '\0') {
            if (isspace((int)*s) && !squote && !dquote && !bsquote) break;

            if (bsquote) {
                bsquote = 0;
            } else if (*s == '\\') {
                bsquote = 1;
            } else if (squote) {
                if (*s == '\'') squote = 0;
            } else if (dquote) {
                if (*s == '"') dquote = 0;
            } else {
                if (*s == '\'')
                    squote = 1;
                else if (*s == '"')
                    dquote = 1;
            }

            *arg++ = *s;

            s++;
        }

        *arg = '\0';

        if ((argv[argc] = strdup(copybuf)) == NULL) {
            Free(copybuf);
            FreeArgv(argv);
            argv = NULL;
            return argv;
        }

        argc++;
        argv[argc] = NULL;

        while (isspace((int)*s)) s++;

    } while (*s != '\0');

    Free(copybuf);

    return argv;
}

void FreeArgv(char **args) {
    char **ap;

    if (args == (char **)NULL) return;

    for (ap = args; *ap != NULL; ap++) Free(*ap);

    Free(args);
}

char **NewArgv(const char *a0, ...) {
    int n = 1;
    char **ap;
    char **app;
    va_list args;

    if (a0 == NULL) {
        ap = (char **)Alloc(sizeof(char *));
        *ap = NULL;
        return ap;
    }

    va_start(args, a0);

    while (va_arg(args, char *) != NULL) n++;

    va_end(args);

    app = ap = (char **)Alloc((n + 1) * sizeof(char *));
    *app++ = strdup(a0);

    va_start(args, a0);

    while (--n > 0) *app++ = strdup(va_arg(args, char *));

    va_end(args);

    *app = NULL;

    return ap;
}

char **CopyArgv(char **args) {
    char **ap;
    int len = 0;
    int i = 0;
    char **res;

    if (args == NULL || *args == NULL) {
        res = (char **)Alloc(sizeof(char *));
        *res = NULL;
        return res;
    }

    for (ap = args; *ap != NULL; ++ap) {
        ++len;
    }

    res = (char **)Alloc((len + 1) * sizeof(char *));

    for (ap = args; *ap != NULL; ++ap) {
        res[i++] = strdup(*ap);
    }

    // set final entry
    res[i] = NULL;

    return res;
}

char **AppendArgv(char **ap, char **ep) {
    int n = 0;
    char **na;
    char **nap;
    char **app;

    if (ep == NULL || *ep == NULL) return ap;

    for (app = ap; *app != NULL; app++) n++;

    for (app = ep; *app != NULL; app++) n++;

    nap = na = (char **)Alloc((n + 1) * sizeof(char *));

    for (app = ap; *app != NULL; app++, nap++) *nap = strdup(*app);

    for (app = ep; *app != NULL; app++, nap++) *nap = strdup(*app);

    *nap = NULL;

    FreeArgv(ap);

    return na;
}

char **AppendArgs(char **ap, ...) {
    int n = 1;
    char *a;
    char **na;
    char **nap;
    char **app;
    va_list args;

    for (app = ap; *app != NULL; app++) n++;

    va_start(args, ap);

    while (va_arg(args, char *) != NULL) n++;

    va_end(args);

    nap = na = (char **)Alloc((n + 1) * sizeof(char *));

    for (app = ap; *app != NULL; app++, nap++) *nap = strdup(*app);

    va_start(args, ap);

    while ((a = va_arg(args, char *)) != NULL) *nap++ = strdup(a);

    va_end(args);

    *nap = NULL;

    FreeArgv(ap);

    return na;
}

/*
** Convert arguments into a string.
*/
char *ArgsToBuf(char *argv[], int argc) {
    int i;
    char *buf_1 = NULL;
    char *buf_2 = NULL;

    if (argc < 1) {
        return NULL;
    }

    // create a string containing arg 0
    asprintf(&buf_1, "%s", argv[0]);

    for (i = 1; i < argc; ++i) {
        // concat the previous arg with the next one
        asprintf(&buf_2, "%s %s", buf_1, argv[i]);
        // cleanup for this iteration
        Free(buf_1);
        buf_1 = buf_2;
        buf_2 = NULL;
    }

    // return the concat'ed result
    return buf_1;
}


std::string ArgsToString(int argc, char* argv[]) {
    std::string res;
    for (int i=0; i<argc; ++i) {
        if (i)
            res += " ";
        res += argv[i];
    }
    return res;
}

ArgvStream& operator<<(ArgvStream& ab, const std::string &str) {
    ab.m_argv.push_back(str);
    return ab;
}

ArgvStream& operator<<(ArgvStream& ab, const char *cstr) {
    ab.m_argv.push_back(std::string{cstr});
    return ab;
}

ArgvStream& operator<<(ArgvStream& ab, char *cstr) {
    ab.m_argv.push_back(std::string{cstr});
    return ab;
}

// argv must end with a nullptr
ArgvStream& operator<<(ArgvStream& ab, const char *argv[]) {
    for (int i = 0; argv != nullptr && argv[i] != nullptr; i++) {
        ab.m_argv.push_back(std::string{argv[i]});
    }
    return ab;
}

// argv must end with a nullptr
ArgvStream& operator<<(ArgvStream& ab, char *argv[]) {
    for (int i = 0; argv != nullptr && argv[i] != nullptr; i++) {
        ab.m_argv.push_back(std::string{argv[i]});
    }
    return ab;
}

ArgvStream& operator<<(ArgvStream& ab, const int &i) {
    ab.m_argv.push_back(std::to_string(i));
    return ab;
}
