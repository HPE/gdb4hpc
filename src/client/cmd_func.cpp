/*********************************************************************************\
 * Internal functions.
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * Copyright 2010-2022 Hewlett Packard Enterprise Development LP.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Hewlett Packard Enterprise Development LP.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *
 *********************************************************************************/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <signal.h>
#include <stdio.h>
#include <string.h>

#include "client.h"
#include "useful.h"

/*
** Parse a function definition.
*/
static func_t *ParseFuncDef(const char *str) {
    list_t *args;
    func_t *f;
    expr_t *e;
    void *sym;
    ct_type t;
    ct_val cv;

    if (ScanString(&cv, &str) != CT_SYM) {
        PrintOutError("identifier expected");
        return (func_t *)NULL;
    }

    if ((f = FindFunc((hentry_t *)(sym = cv.cv_sym))) != (func_t *)NULL)
        FreeFunc(f);

    if (ScanString(&cv, &str) != CT_LPAREN) {
        PrintOutError("'(' expected");
        return (func_t *)NULL;
    }

    args = NewList();

    for (;;) {
        if (ScanString(&cv, &str) != CT_SYM) {
            PrintOutError("argument expected");
            DestroyList(args, (void (*)(void *))NULL);
            return (func_t *)NULL;
        }

        if (InList(args, cv.cv_sym)) {
            PrintOutError("duplicate argument");
            DestroyList(args, (void (*)(void *))NULL);
            return (func_t *)NULL;
        }

        AddToList(args, cv.cv_sym);

        if ((t = ScanString(&cv, &str)) == CT_RPAREN) break;

        if (t != CT_COMMA) {
            PrintOutError("'(' or ',' expected");
            DestroyList(args, (void (*)(void *))NULL);
            return (func_t *)NULL;
        }
    }

    if ((t = ScanString(&cv, &str)) != CT_EQUAL) {
        PrintOutError("'=' expected");
        return (func_t *)NULL;
    }

    if ((e = ParseExpr(str)) == (expr_t *)NULL) {
        PrintError();
        return (func_t *)NULL;
    }

    if ((f = NewFunc((hentry_t *)sym, args, e,
                     (int (*)(list_t *, val_t **))NULL, str, 1)) ==
        (func_t *)NULL) {
        PrintError();
        return (func_t *)NULL;
    }

    return f;
}

static int class_set_subcmd_function_cmd(rec_set_t *rec_set, int argc,
                                         char **argv) {
    char *buf;

    if (argc < 2) {
        PrintOutError("Not enough command arguments.\n");
        PrintOutError("Expected: function func_name(func_args) = expr\n");
        return 0;
    }

    buf = ArgsToBuf(&argv[1], argc - 1);

    if (ParseFuncDef(buf) == (func_t *)NULL) {
        PrintError();
        Free(buf);
        return 0;
    }

    Free(buf);

    return 1;
}

static int class_show_subcmd_function_cmd(rec_set_t *rec_set, int argc,
                                          char **argv) {
    func_t *f;

    SetFunc();

    while ((f = GetFunc()) != (func_t *)NULL) {
        if (argc > 1 && strcmp(argv[1], SymLookup(f->f_sym)) == 0) {
            PrintFunc(Client.cli_out, f);
            break;
        }

        PrintFunc(Client.cli_out, f);
    }

    return 1;
}

void InitCmd_func(void) {
    AddSetCommand("function", proc_ignore, 0,
                  class_set_subcmd_function_cmd,
                  class_set_subcmd_function_help, "Define a function.");
    AddShowCommand("function", proc_ignore, 0,
                   class_show_subcmd_function_cmd,
                   class_show_subcmd_function_help,
                   "Show information about defined functions.");
}

/*
 * This function is used to initialize the above functions inside of main.c
 */
void InitBuiltinFuncs(void) {
    //	void *		s;
    //	list_t *	arg1;
    //	list_t *	arg2;
    //	list_t *	arg3;

    InitFunc();
}
