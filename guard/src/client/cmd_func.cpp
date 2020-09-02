/*********************************************************************************\
 * Internal functions.
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * Copyright 2010-2020 Hewlett Packard Enterprise Development LP.
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
            PrintFunc(Client.cli_out_stream, f);
            break;
        }

        PrintFunc(Client.cli_out_stream, f);
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

/**********************************
 * Built-in functions start here
 **********************************/
// FIXME: These are all invalid. They were never updated. They need to be
// looked at and verified.

/*
** Compute rank of an array. Expecting one arg which must be an
** array type.
*
int
rankfunc(list_t *args, val_t **res)
{
        int		rank;
        arg_t *		a;
        g_map_t *	m;
        AIF *		ra;

        if ( SizeOfList(args) != 1 )
        {
                PrintOutError("Expected 1 argument to builtin function rank,
found %d\n", SizeOfList(args)); return -1;
        }

        SetList(args);
        a = (arg_t *)GetListElement(args);

        switch ( a->a_type )
        {
        case ARG_VAL:
                if ( (rank = AIFArrayRank(VAL_VAL(a->a_val))) < 0 )
                        return -1;

                break;

        case ARG_SYM:
                if ( (m = FindMap(SymLookup(a->a_sym))) == (g_map_t *)NULL )
                {
                        PrintOutError("no such map: \"%s\"\n",
SymLookup(a->a_sym)); return -1;
                }

                if ( !m->m_current )
                {
                        PrintOutError("map is not valid\n");
                        return -1;
                }

                rank = m->m_ix->i_rank;
                break;

        default:
                PrintOutError("Incorrect argument type to builtin function
rank\n"); PrintOutError("Expected a symbol or value.\n"); return -1;
        }

        if ( (ra = IntToAIF(rank)) == (AIF *)NULL )
        {
                PrintOutError("AIF Error: %s\n", AIFErrorStr());
                return -1;
        }

        *res = NewVal(lval_none, rval_aif, ra);

        return 0;
}

*
** Compute number of elements of an array. Expecting one arg which
** must be an array type.
*
int
nelfunc(list_t *args, val_t **res)
{
        int		nel;
        arg_t *		a;
        g_map_t *	m;
        AIF *		ra;

        if ( SizeOfList(args) != 1 )
        {
                PrintOutError("Expected 1 argument to builtin function nel,
found %d\n", SizeOfList(args)); return -1;
        }

        SetList(args);
        a = (arg_t *)GetListElement(args);

        nel = 1;

        switch ( a->a_type )
        {
        case ARG_VAL:
                if ( (nel = AIFArraySize(VAL_VAL(a->a_val))) < 0 )
                        return -1;

                break;

        case ARG_SYM:
                if ( (m = FindMap(SymLookup(a->a_sym))) == (g_map_t *)NULL )
                {
                        PrintOutError("no such map: \"%s\"\n",
SymLookup(a->a_sym)); return -1;
                }

                if ( !m->m_current )
                {
                        PrintOutError("map is not valid\n");
                        return -1;
                }

                nel = m->m_ix->i_nel;
                break;

        default:
                PrintOutError("Incorrect argument type to builtin function
nel\n"); PrintOutError("Expected a symbol or value.\n"); return -1;
        }

        if ( (ra = IntToAIF(nel)) == (AIF *)NULL )
        {
                PrintOutError("AIF Error: %s\n", AIFErrorStr());
                return -1;
        }

        *res = NewVal(lval_none, rval_aif, ra);

        return 0;
}

*
** The upper function returns the upper bound of the specified array index.
**
** Expects two arguments: the name of the array and the index number
** (which are numbered from 0).
*
int
upperfunc(list_t *args, val_t **res)
{
        int		n;
        int		upper;
        arg_t *		a;
        arg_t *		a1;
        g_map_t *	m;
        val_t *		v;
        AIF *		ra;

        if ( SizeOfList(args) != 2 )
        {
                PrintOutError("Expected 2 arguments to builtin function upper,
found %d\n", SizeOfList(args)); return -1;
        }

        SetList(args);
        a = (arg_t *)GetListElement(args);
        a1 = (arg_t *)GetListElement(args);

        if ( a1->a_type != ARG_VAL || RVAL_TYPE(a1->a_val) != rval_aif )
        {
                PrintOutError("Incorrect argument type to builtin function
upper\n"); PrintOutError("Expected a value.\n"); return -1;
        }

        v = a1->a_val;

        if ( AIFToInt(VAL_VAL(v), &n) < 0 )
        {
                PrintOutError("AIF Error: %s\n", AIFErrorStr());
                return -1;
        }

        switch ( a->a_type )
        {
        case ARG_VAL:
                if ( (upper = AIFArrayMaxIndex(VAL_VAL(a->a_val), n)) < 0 )
                {
                        PrintOutError("invalid index passed to upper\n");
                        return -1;
                }

                break;

        case ARG_SYM:
                if ( (m = FindMap(SymLookup(a->a_sym))) == (g_map_t *)NULL )
                {
                        PrintOutError("no such map: \"%s\"\n",
SymLookup(a->a_sym)); return -1;
                }

                if ( !m->m_current )
                {
                        PrintOutError("map is not valid\n");
                        return -1;
                }

                if ( n < 0 || n >= m->m_ix->i_rank )
                {
                        PrintOutError("invalid index passed to upper\n");
                        return -1;
                }

                upper = m->m_ix->i_max[n];
                break;

        default:
                PrintOutError("Incorrect argument type to builtin function
upper\n"); PrintOutError("Expected a symbol or value.\n"); return -1;
        }

        if ( (ra = IntToAIF(upper)) == (AIF *)NULL )
        {
                PrintOutError("AIF Error: %s\n", AIFErrorStr());
                return -1;
        }

        *res = NewVal(lval_none, rval_aif, ra);

        return 0;
}
*/

/*
 * This function is used to initialize the above functions inside of main.c
 */
void InitBuiltinFuncs(void) {
    //	void *		s;
    //	list_t *	arg1;
    //	list_t *	arg2;
    //	list_t *	arg3;

    InitFunc();

    /*
            TODO: These are all invalid now
            arg1 = NewList();

            if ( (s = AddSym("a")) == (void *)NULL )
            {
                    PrintError();
                    return;
            }

            AddToList(arg1, s);

            if
            (
                    (s = AddSym("rank")) == (void *)NULL
                    ||
                    NewFunc(s, arg1, (expr_t *)NULL, rankfunc, "builtin
       function", 1) == (func_t *)NULL
            )
            {
                    PrintError();
                    return;
            }

            arg2 = NewList();

            if ( (s = AddSym("a")) == (void *)NULL )
            {
                    PrintError();
                    return;
            }

            AddToList(arg1, s);

            if
            (
                    (s = AddSym("nel")) == (void *)NULL
                    ||
                    NewFunc(s, arg2, (expr_t *)NULL, nelfunc, "builtin
       function", 1) == (func_t *)NULL
            )
            {
                    PrintError();
                    return;
            }

            arg3 = NewList();

            if ( (s = AddSym("a")) == (void *)NULL )
            {
                    PrintError();
                    return;
            }

            AddToList(arg3, s);

            if ( (s = AddSym("n")) == (void *)NULL )
            {
                    PrintError();
                    return;
            }

            AddToList(arg3, s);

            if
            (
                    (s = AddSym("upper")) == (void *)NULL
                    ||
                    NewFunc(s, arg3, (expr_t *)NULL, upperfunc, "builtin
       function", 1) == (func_t *)NULL
            )
            {
                    PrintError();
                    return;
            }
    */
}
