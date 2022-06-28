/*********************************************************************************\
 * Expression handling routines.
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

#include <cmath>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <sstream>

#include "client_settings.h"
#include "dbg_client.h"
#include "defs.h"
#include "gram.h"
#include "useful.h"
#include "aifCompat.h"

// Global variables with external exposure - This is used in expr.y.in
const char **ExprStr;
// This function is created by the yacc parser - see expr.y.in
extern int expr_parse(void);

// Static global variables
static expr_t *OPList;
static list_t *_expr_list = NULL;  // used in expr.y.in
static list_t *_elt_list = NULL;

// Static function prototypes
static expr_t *do_parse(const char **);
static val_t *evaluate_expr_imm(expr_t *, rec_set_t *, func_t *);
static val_t *evaluate_func_imm(hentry_t *, expr_t *, rec_set_t *, func_t *);
static val_t *read_var_imm(expr_t *, rec_set_t *, func_t *);
static char *apply_var_op(expr_elt *, expr_t *);
static val_t *apply_op(expr_op, val_t *, val_t *, val_t *, rec_set_t *,
                       val_t **);
static int assign_expr_imm(val_t *, val_t *, rec_set_t *);
static var_t *ReadVar_BlockMap(expr_t *, rec_set_t *, CMD_MODE);
static val_t *ReadThreadProcSet(expr_t*, rec_set_t*);
static expr_elt* check_pop(expr_t*, expr_op);

static void add_to_reply(rep_set_t& repSet, dbginvoke_t* di, std::unique_ptr<Bitset> &&group,
                         AIFStream* aif);
// Only called directly with debug - but keep around to be able to call from gdb
#ifdef DEBUG
void DumpExpr(expr_t *);
#endif

expr_t *ParseExpr(const char *str) { return do_parse(&str); }

static expr_t *do_parse(const char **str) {
    OPList = (expr_t *)Alloc(sizeof(expr_t) + sizeof(expr_elt) * 10);
    OPList->e_len = 10;
    OPList->e_lval = 0;
    OPList->e_eval = 0;

    ExprStr = str;

    if (expr_parse()) {
        FreeExpr(OPList);
        return (expr_t *)NULL;
    }

#ifdef DEBUG
    DumpExpr(OPList);
#endif

    return OPList;
}

void FreeExpr(expr_t *e) {
    int p;

    /*
    ** Free any strings
    */
    for (p = 0; p < e->e_lval; p++) {
        switch (e->e_expr[p].e_op) {
            case EXPR_STRING:
                if (e->e_expr[p].e_string != NULL) Free(e->e_expr[p].e_string);
                break;

            case EXPR_SYM:
            case EXPR_TYPE_SPEC:
            case EXPR_TYPE_IDENT:
                if (e->e_expr[p].e_sym_str != NULL)
                    Free(e->e_expr[p].e_sym_str);
                break;

            default:
                // do nothing
                break;
        }
    }

    Free(e);
}

/*
** This function is used to save the top expr_elt from the expression stack. It
** is used by the parser.
*/
void SaveTopElt(void) {
    expr_elt *top_elt;
    int elt_i;

    top_elt = (expr_elt *)Alloc(sizeof(expr_elt));

    // Get the current depth. Note that this is off by one.
    elt_i = OPList->e_lval - 1;

    top_elt->e_op = OPList->e_expr[elt_i].e_op;

    DPRINT("SaveTopElt: Top op is %s.\n", getExprOpStr(top_elt->e_op));

    // zero out the union
    memset(&top_elt->e_val, 0, sizeof(top_elt->e_val));

    // now assign it
    top_elt->e_val = OPList->e_expr[elt_i].e_val;

    if (_elt_list == (list_t *)NULL) _elt_list = NewList();

    AddFirst(_elt_list, (void *)top_elt);

    // Now we need to remove the top elt from the expr stack if it is a known
    // symbol. This is because the parser is going to re-add it after it does
    // its thing. This is to ensure the correct expr_op gets added if the symbol
    // cooresponds to a blockmap, function, etc. Here we just need to decrement
    // the e_lval since the elt's are not individually malloc'ed. We have a copy
    // of the elt in the elt_list so that is what we restore it from.
    // FIXME: andrewg: This is all messed up IMO....
    // UPDATE: This is necessary because of the way we read the variables.
    // The readvar function depends on if it is inside a function or not. If its
    // in a blockmap, a certain type of readvar function is called.
    // There is no way to fix this since we need to know if we are going to be
    // passing the val into a blockmap. A fix would be to merge all the
    // function, which would require a fundamental rewrite. This only works with
    // DSYM's. If the user isn't passing us a dsym, then they are probably
    // trying to call a function. Add this in sometime!
    if (top_elt->e_op == EXPR_DSYM) {
        // if any of these return true, we need to remove it. Fix this later..
        if (CheckBlockMap(top_elt->e_sym)) {
            DPRINT("SaveTopElt: Found BlockMap! Removing elt from OPList.\n");
            OPList->e_lval--;
        } else if (CheckFunc(top_elt->e_sym)) {
            DPRINT("SaveTopElt: Found Func! Removing elt from OPList.\n");
            OPList->e_lval--;
        }
    }
}

/*
** This function is used to get the expr_op from the saved top expr_elt. It
** is used by the parser.
*/
expr_op GetTopEltOp(void) {
    expr_elt *top_elt;

    SetList(_elt_list);

    if ((top_elt = (expr_elt *)GetListElement(_elt_list)) == (expr_elt *)NULL)
        return EXPR_UNKNOWN;

    DPRINT("GetTopElt: Top op is %s.\n", getExprOpStr(top_elt->e_op));

    return top_elt->e_op;
}

/*
** This function is used to get the expr_val for a SYM from the saved
** top expr_elt. It is used by the parser.
*/
hentry_t *GetTopEltSymVal(void) {
    expr_elt *top_elt;

    SetList(_elt_list);

    if ((top_elt = (expr_elt *)GetListElement(_elt_list)) == (expr_elt *)NULL)
        return NULL;

    if (top_elt->e_op != EXPR_DSYM) return NULL;

    DPRINT("GetTopEltSymVal: e_sym value is \"%s\"\n",
           SymLookup(top_elt->e_sym));

    return top_elt->e_sym;
}

/*
** This function is used to remove the saved expr_elt. It is used by the parser.
*/
void RemoveTopElt(void) {
    expr_elt *top_elt;

    if ((top_elt = (expr_elt *)RemoveFirst(_elt_list)) != (expr_elt *)NULL) {
        Free(top_elt);
    }

    if (EmptyList(_elt_list)) {
        DestroyList(_elt_list, (void (*)(void *))NULL);
        _elt_list = (list_t *)NULL;
    }
}

expr_t *AddExpr(expr_op op, ...) {
    int len;
    int v;
    va_list args;

    DPRINT("AddExpr: adding %s to expr stack.\n", getExprOpStr(op));

    /*
    ** Check if there is enough space for (possibly) two items
    */
    if (OPList->e_lval >= OPList->e_len) {
        len = OPList->e_len * 2;
        OPList = (expr_t *)Resize((void *)OPList,
                                  sizeof(expr_t) + sizeof(expr_elt) * len);
        OPList->e_len = len;
    }

    v = OPList->e_lval++;

    OPList->e_expr[v].e_op = op;

    va_start(args, op);

    switch (op) {
        case EXPR_INT:
            OPList->e_expr[v].e_int = va_arg(args, int);
            DPRINT("AddExpr: e_int is %d\n", OPList->e_expr[v].e_int);
            break;

        case EXPR_FLOAT:
            OPList->e_expr[v].e_float = va_arg(args, double);
            DPRINT("AddExpr: e_float is %g\n", OPList->e_expr[v].e_float);
            break;

        case EXPR_SYM:
        case EXPR_TYPE_SPEC:
        case EXPR_TYPE_IDENT:
            // Ensure this get's free'ed
            OPList->e_expr[v].e_sym_str = va_arg(args, char *);
            DPRINT("AddExpr: e_sym_str is \"%s\"\n",
                   OPList->e_expr[v].e_sym_str == NULL
                       ? "NULL"
                       : OPList->e_expr[v].e_sym_str);
            break;

        case EXPR_DSYM:
        case EXPR_FUNC:
        case EXPR_BLOCKMAP:
            OPList->e_expr[v].e_sym = va_arg(args, hentry_t *);
            DPRINT("AddExpr: e_sym value is \"%s\"\n",
                   SymLookup(OPList->e_expr[v].e_sym));
            break;

        case EXPR_STRING:
            // Ensure this get's free'ed
            OPList->e_expr[v].e_string = va_arg(args, char *);
            DPRINT("AddExpr: e_string is \"%s\"\n",
                   OPList->e_expr[v].e_string == NULL
                       ? "NULL"
                       : OPList->e_expr[v].e_string);
            break;

        default:
            memset(&OPList->e_expr[v].e_val, 0,
                   sizeof(OPList->e_expr[v].e_val));
            break;
    }

    va_end(args);

    return OPList;
}

int EvalCompare(val_t **res, val_t **diff, expr_t *e, rec_set_t *rec_set) {
    expr_elt *ep;
    val_t *lval = (val_t *)NULL;
    val_t *rval = (val_t *)NULL;
    val_t *val;

    DPRINT("In EvalCompare.\n");

    if (e->e_lval == 0) {
        SetError("Parsed expression returned 0 lval. Nothing to compare.");
        return -1;
    }

#ifdef DEBUG
    PrintOutError("EvalCompare: DumpExpr(e) as the following\n");
    DumpExpr(e);
#endif

    RESET(e);

    ep = POP(e);
    DPRINT("EvalCompare: Popped expr_op %s\n", getExprOpStr(ep->e_op));

    // The outermost token *must* be an operator. The parser will always place
    // the operator for a comparison as the first token.
    // We do not allow unary operators for the outermost operator. We require
    // a lval and rval in order to conduct the comparison.
    // We only allow equality operators to be used for the outermost operator.
    switch (ep->e_op) {
        case EXPR_OPEQUAL:
        case EXPR_OPNOTEQUAL:
        case EXPR_OPLEQ:
        case EXPR_OPGEQ:
        case EXPR_OPLT:
        case EXPR_OPGT:
            // The expression tree has the second operand first, followed by the
            // first operand.
            DPRINT("EvalCompare: Evaluating rval\n");
            if ((rval = evaluate_expr_imm(e, rec_set, NULL)) == (val_t *)NULL) {
                return -1;
            }
            DPRINT("EvalCompare: Have rval\n");

            DPRINT("EvalCompare: Evaluating lval\n");
            if ((lval = evaluate_expr_imm(e, rec_set, NULL)) == (val_t *)NULL) {
                DestroyVal(rval);
                return -1;
            }
            DPRINT("EvalCompare: Have lval\n");
            break;

        default:
            SetError("Tried to use an invalid equality operator.");
            return -1;
    }

    // Make sure diff gets initialized if it was provided
    if (diff != NULL) {
        *diff = (val_t *)NULL;
    }

    /*
    ** Remember to use correct ordering. The val will be an aif int result
    ** of the equality operator. If it is 1, or true, then there is no
    ** difference. If it is 0, or false, then there is a difference.
    */
    if ((val = apply_op(ep->e_op, lval, rval, NULL, rec_set, diff)) ==
        (val_t *)NULL) {
        if (rval != (val_t *)NULL) DestroyVal(rval);
        if (lval != (val_t *)NULL) DestroyVal(lval);
        if (diff != NULL && *diff != NULL) DestroyVal(*diff);
        return -1;
    }

    // ensure what came back has an rval of reply or aif
    if (RVAL_TYPE(val) != rval_aif && RVAL_TYPE(val) != rval_reply) {
        if (rval != (val_t *)NULL) DestroyVal(rval);
        if (lval != (val_t *)NULL) DestroyVal(lval);
        if (diff != NULL && *diff != NULL) DestroyVal(*diff);
        SetError("EvalCompare: invalid return val from apply_op.");
        return -1;
    }

    // set the result
    *res = val;

    // TODO: Add in error tolerance for floating point values using AIFEPS.

    // cleanup
    if (rval != (val_t *)NULL) DestroyVal(rval);

    if (lval != (val_t *)NULL) DestroyVal(lval);

    return 0;
}

int EvalExpr(val_t **res, expr_t *e, rec_set_t *rec_set) {
    if (e->e_lval == 0) {
        SetError("Parsed expression returned 0 lval. Nothing to compare.");
        return -1;
    }

#ifdef DEBUG
    PrintOutMsg("EvalExpr: DumpExpr(e) as the following\n");
    DumpExpr(e);
#endif

    RESET(e);

    if ((*res = evaluate_expr_imm(e, rec_set, (func_t *)NULL)) == (val_t *)NULL)
        return -1;

    return 0;
}

int EvalExprList(list_t **res, expr_t *e, rec_set_t *rec_set) {
    val_t *v;
    expr_elt *ep;
    list_t *l;

    DPRINT("In EvalExprList\n");

    if (e->e_lval == 0) {
        SetError("Nothing to do!");
        return -1;
    }

    RESET(e);

    ep = TOP(e);

    l = NewList();

    // We want to evaluate the comma seperated list an element at a time.
    // So lets evaluate any elements seperated by commas and leave one.
    while (ep->e_op == EXPR_OPCOMMA) {
        DPRINT("EvalExprList: Found EXPR_OPCOMMA!\n");

        (void)POP(e);

        if ((v = evaluate_expr_imm(e, rec_set, (func_t *)NULL)) ==
            (val_t *)NULL) {
            DestroyList(l, (void (*)(void *))DestroyVal);
            return -1;
        }

        AddFirst(l, (void *)v);

        ep = TOP(e);

        DPRINT("EvalExprList: Adding val to list\n");
    }

    DPRINT("EvalExprList: Eval final element\n");

    // Evaluate the final element
    if ((v = evaluate_expr_imm(e, rec_set, (func_t *)NULL)) == (val_t *)NULL) {
        DPRINT("EvalExprList: Failed on final element\n");
        DestroyList(l, (void (*)(void *))DestroyVal);
        return -1;
    }

    AddFirst(l, (void *)v);

    *res = l;

    return 0;
}

int EvalFunc(val_t **res, func_t *fp, list_t *args, rec_set_t *rec_set) {
    val_t *val;

    if (fp->f_builtin != (int (*)(list_t *, val_t **))NULL)
        return fp->f_builtin(args, res);

    fp->f_actual = args;

    /*
    ** Apply function to its arguments.
    */
    RESET(fp->f_def);

    if ((val = evaluate_expr_imm(fp->f_def, rec_set, fp)) == (val_t *)NULL)
        return -1;

    *res = val;

    return 0;
}

/*
** Evaluate an immediate expression.
*/
static val_t *evaluate_expr_imm(expr_t *e, rec_set_t *rec_set, func_t *f) {
    // hentry_t *	sym;
    // arg_t *		a;
    val_t *v1 = (val_t *)NULL;
    val_t *v2 = (val_t *)NULL;
    val_t *v3 = (val_t *)NULL;
    val_t *val;
    expr_elt *ep;
    g_block_map_t *bm = NULL;

    DPRINT("In evaluate_expr_imm\n");

    ep = POP(e);
    DPRINT("evaluate_expr_imm: Popped expr_op %s\n", getExprOpStr(ep->e_op));

    switch (ep->e_op) {
        case EXPR_OPCOND:
            if ((v1 = evaluate_expr_imm(e, rec_set, f)) == (val_t *)NULL)
                return (val_t *)NULL;

            /* drop through... */

        case EXPR_OPADD:
        case EXPR_OPSUB:
        case EXPR_OPMUL:
        case EXPR_OPDIV:
        case EXPR_OPREM:  // TODO: Broken to fix derived fortran types
        case EXPR_OPEQUAL:
        case EXPR_OPNOTEQUAL:
        case EXPR_OPLEQ:
        case EXPR_OPGEQ:
        case EXPR_OPLT:
        case EXPR_OPGT:
        case EXPR_OPAND:
        case EXPR_OPOR:
            if ((v2 = evaluate_expr_imm(e, rec_set, f)) == (val_t *)NULL)
                return (val_t *)NULL;

            /* drop through... */

        case EXPR_OPPOS:
        case EXPR_OPNEG:
            if ((v3 = evaluate_expr_imm(e, rec_set, f)) == (val_t *)NULL)
                return (val_t *)NULL;

            /*
            ** Reverse arguments to get correct ordering.
            */
            if ((val = apply_op(ep->e_op, v3, v2, v1, rec_set, NULL)) ==
                (val_t *)NULL)
                return (val_t *)NULL;

            DestroyVal(v3);

            if (v2 != (val_t *)NULL) DestroyVal(v2);

            if (v1 != (val_t *)NULL) DestroyVal(v1);

            return val;

        case EXPR_INT:
            v1 = NewVal(lval_none, rval_aif, AIFStream::fromInt(ep->e_int));
            DPRINT("evaluate_expr_imm: v1 is int %d\n", ep->e_int);
            return v1;

        case EXPR_FLOAT:
            v1 = NewVal(lval_none, rval_aif, AIFStream::fromDouble(ep->e_float));
            DPRINT("evaluate_expr_imm: v1 is float %g\n", ep->e_float);
            return v1;

        case EXPR_DSYM:
            if ((val = GetVal(ep->e_sym)) != (val_t *)NULL) {
                DPRINT(
                    "evaluate_expr_imm: Found previous val for dsym \"%s\"\n",
                    SymLookup(ep->e_sym));
                return val;
            }

            /*
            ** Create new debugger var.
            */
            val = NewVal(lval_none, rval_none);

            if (AssignVal(LVAL_SYM_VAL(ep->e_sym), val, rec_set) < 0) {
                DestroyVal(val);
                return (val_t *)NULL;
            }

            DestroyVal(val);

            DPRINT("evaluate_expr_imm: Assigned rval_none to dsym \"%s\"\n",
                   SymLookup(ep->e_sym));

            return GetVal(ep->e_sym);

        case EXPR_FUNC:
            return evaluate_func_imm(ep->e_sym, e, rec_set, f);

        case EXPR_BLOCKMAP:
            PUSH(e);
            bm = EvaluateBlockMap(e, rec_set, CMD_MODE_INTERACTIVE);
            if (bm == NULL) {
                return NULL;
            }
            // FIXME: I think var is messed up, integrate it into val.
            val = NewVal(lval_blockmap, bm->m_actual->v_procset,
                         bm->m_actual->v_var, bm,
                         bm->m_actual->v_procset->s_nproc);

            DPRINT("evaluate_expr_imm EXPR_BLOCKMAP val[%p]\n", val);

            return val;

        case EXPR_PROC_THREAD:
            val = ReadThreadProcSet(e, rec_set);
            return val;

    case EXPR_SYM:
            /*
            ** FIXME: This is broken with the symbol table update.
            sym = ep->e_sym;

            if ( f != (func_t *)NULL )
            {
                    if ( FindArg(&a, sym, f) < 0 )
                            return (val_t *)NULL;

                    if ( a != (arg_t *)NULL )
                    {
                            DPRINT("evaluate_expr_imm: Found previous val for
            sym \"%s\"\n", SymLookup(ep->e_sym)); return CopyVal(a->a_val);
                    }
                    // Must be a program var. Drop through...
            } */
        case EXPR_OPSUBSCR:
        case EXPR_OPDREF:
        case EXPR_OPADDROF:
        case EXPR_OPELEMENTREF:
        case EXPR_OPELEMENTPTR:
        case EXPR_OPELEMENTACC:
        case EXPR_OPCAST:
        case EXPR_STRING:
        case EXPR_VAR:
        case EXPR_REPEAT:
            PUSH(e);
            DPRINT("evaluate_expr_imm: Pushed expr_op %s\n",
                   getExprOpStr(*(TOP(e).e_op)));
            return read_var_imm(e, rec_set, f);

        default:
            SetError("Tried to evaluate an unknown expression: %s",
                     getExprOpStr(ep->e_op));
            return (val_t *)NULL;
    }
}

/*
** Evaluate a function. Collect arguments to function and
** apply a function to them.
*/
static val_t *evaluate_func_imm(hentry_t *sym, expr_t *e, rec_set_t *rec_set,
                                func_t *f) {
    int n;
    arg_t *a;
    val_t *val;
    func_t *fp;
    list_t *aarg;
    list_t *sarg;
    expr_elt *ep;

    if ((fp = FindFunc(sym)) == (func_t *)NULL) {
        SetError("Function \"%s\" not defined", SymLookup(sym));
        return (val_t *)NULL;
    }

    ep = POP(e);

    /*
    ** We first expect to find a list start token.
    */
    if (ep->e_op != EXPR_LIST_START) {
        SetError("Internal error: expected EXPR_LIST_START");
        return (val_t *)NULL;
    }

    ep = POP(e);

    /*
    ** Evaluate the arguments for the function. The number of
    ** arguments follows...
    */

    if (ep->e_op != EXPR_INT) {
        SetError("Internal error: expected EXPR_INT");
        return (val_t *)NULL;
    }

    n = ep->e_int;

    aarg = NewList();

    while (n-- > 0) {
        ep = POP(e);

        if (ep->e_op != EXPR_LIST) {
            SetError("Internal error: expected EXPR_LIST");
            DestroyList(aarg, (void (*)(void *))DestroyArg);
            return (val_t *)NULL;
        }

        if ((val = evaluate_expr_imm(e, rec_set, f)) == (val_t *)NULL) {
            DestroyList(aarg, (void (*)(void *))DestroyArg);
            return (val_t *)NULL;
        }

        /*
        ** Save the argument for when the function is
        ** evaluated.
        */

        a = NewArg(ARG_VAL, val);
        AddFirst(aarg, (void *)a);
    }

    if (fp->f_builtin != (int (*)(list_t *, val_t **))NULL) {
        if (fp->f_builtin(aarg, &val) < 0) {
            DestroyList(aarg, (void (*)(void *))DestroyArg);
            return (val_t *)NULL;
        }

        DestroyList(aarg, (void (*)(void *))DestroyArg);

        return val;
    }

    /*
    ** Save actual args for recursive calls.
    */
    sarg = fp->f_actual;
    fp->f_actual = aarg;

    /*
    ** Apply function to its arguments.
    */
    RESET(fp->f_def);

    if ((val = evaluate_expr_imm(fp->f_def, rec_set, fp)) == (val_t *)NULL) {
        fp->f_actual = sarg;
        DestroyList(aarg, (void (*)(void *))DestroyArg);
        return (val_t *)NULL;
    }

    /*
    ** Restore args.
    */
    fp->f_actual = sarg;
    DestroyList(aarg, (void (*)(void *))DestroyArg);

    return val;
}

namespace {
// helper to translate an parsed expression back into a string
struct ExprToArgList {
    static std::string str(expr_t* e);
    static std::string binOp(expr_t* e, const std::string& op, bool wrap=true);
};
}
    
// string-ize an expression binary operation.   If wrap is true, the result is
// parenthesized
std::string ExprToArgList::binOp(expr_t* e, const std::string& op, bool wrap) {
    auto right = str(e);
    auto left = str(e);
    if (wrap) {
        return "(" + left + op + right + ")";
    } else {
        return left + op + right;
    }
}

// recursively translate the parsed expression e into a string
std::string ExprToArgList::str(expr_t* e) {
    auto ep = POP(e);
    switch (ep->e_op) {
    case EXPR_OPADD: return binOp(e,"+");
    case EXPR_OPSUB: return binOp(e,"-");
    case EXPR_OPMUL: return binOp(e,"*");
    case EXPR_OPDIV: return binOp(e,"/");
    case EXPR_OPPOS: return str(e); 
    case EXPR_OPNEG: return "-" + str(e);
    case EXPR_OPAND: return binOp(e,"&&");
    case EXPR_OPOR:  return binOp(e,"||");
    case EXPR_OPEQUAL: return binOp(e,"=="); 
    case EXPR_OPNOTEQUAL: return binOp(e,"!=");
    case EXPR_OPLEQ: return binOp(e,"<=");
    case EXPR_OPGEQ: return binOp(e,">=");
    case EXPR_OPLT: return binOp(e,"<");
    case EXPR_OPGT: return binOp(e,">");
    case EXPR_OPCOND: {
        auto a3 = str(e);
        auto a2 = str(e);
        auto a1 = str(e);
        return "(" + a1 + "?" + a2 + ":" + a3 + ")";
    }
    case EXPR_OPCOMMA: return binOp(e,",");
    case EXPR_OPDREF: return "*" + str(e);
    case EXPR_OPADDROF: return "&" + str(e);
    case EXPR_OPELEMENTREF: return binOp(e,".");
    case EXPR_OPELEMENTPTR: return binOp(e,"->");
    case EXPR_OPELEMENTACC: return binOp(e,"%");  // fortran element access
    case EXPR_OPCAST: {
        auto a2 = str(e);
        auto a1 = str(e);
        return "((" + a1 + ")(" + a2 + "))";
    }        
    case EXPR_TYPE: return "(" + str(e) + ")";
    case EXPR_SLICE: return binOp(e, ":", false);   // probably fortran slice
    case EXPR_INT: return std::to_string(ep->e_val.e_v_int);
    case EXPR_FLOAT: return std::to_string(ep->e_val.e_v_float);
    case EXPR_DSYM: return SymLookup(ep->e_val.e_v_sym);
    case EXPR_SYM: return ep->e_val.e_v_sym_str;
    case EXPR_STRING: return ep->e_val.e_v_string;
    case EXPR_STAR: return "*";  // backend will need to deal with it

    case EXPR_LIST:  
    case EXPR_OPSUBSCR: {
        // a gamut of list like things end up here:
        // (expr,expr,expr)    function calls and fortran array references/slices
        // [expr][expr]        C/C++ array subsripts
        auto arglist = std::string{""};

        ep = TOP(e);
        if (ep->e_op == EXPR_LIST_START) {
            // Pop the list start token
            ep = POP(e);
            // Pop the length
            ep = POP(e);
        } else if (ep->e_op != EXPR_LIST) {
            arglist = str(e);
        }

        // note that expression list is ordered from last to first
        // the ordering works the same for C multi-dimension array references (verified emprically)
        while ((ep=TOP(e))->e_op == EXPR_LIST || ep->e_op == EXPR_OPSUBSCR) {
            ep = POP(e);
            auto elem = str(e);
            if (arglist.empty()) {
                arglist = elem;
            } else {
                arglist = elem + "," + arglist;
            }
        }
        return "(" + arglist + ")";
    }

    default:
        throw std::invalid_argument(std::string{"unrecognized subexpression of type: "} +
                                    getExprOpStr(ep->e_op));
    }
}

// translate an expression back into a string.  
static std::string makeArgumentList(expr_t* e) {
    try {
        return ExprToArgList::str(e);
    } catch (std::exception& e) {
        fprintf(stderr, "could not unparse string: %s\n", e.what());
        return "";
    }
}

/*
** Read a variable from a program and return in res. The supplied expression
** must be in the form:
**
**	[ EXPR_LOC EXPR_INT EXPR_STRING ]
**	[ EXPR_OPSUBSCR EXPR_SLICE [<expr>] [<expr>] ] ...
**	EXPR_SYM | EXPR_STRING
**	[ EXPR_DSYM EXPR_INT <expr> [<expr> ...]]
**
** If f has been supplied, then argument substitution is performed
** on each EXPR_SYM.
**
** A variable can be either an internal debugger variable or a gdb variable.
** Internal variables are created if they don't already exist.
*/
static val_t *read_var_imm(expr_t *e, rec_set_t *rec_set, func_t *f) {
    hentry_t *sym = nullptr;
    char *var_expr = nullptr;
    val_t *val = nullptr;
    val_t *ps_val = nullptr;
    val_t *v1 = nullptr;
    int repeat = 0;
    slice_t *slice = nullptr;
    expr_elt *ep;
    expr_op stype;

    DPRINT("In read_var_imm\n");

    ep = POP(e);

    DPRINT("read_var_imm: Popped expr_op %s\n", getExprOpStr(ep->e_op));

    // Ensure a location wasn't provided.
    if (ep->e_op == EXPR_LOC) {
        SetError("Can't specify location in immediate mode");
        return nullptr;
    }

    // Remember repeat ('@n')
    if (ep->e_op == EXPR_REPEAT) {
        auto reps_elt = POP(e);
        repeat = reps_elt->e_val.e_v_int;
        ep = POP(e);
    }

    // This now works for multi-dimensional fortran arrays. They are represented
    // as a list.
    if (ep->e_op == EXPR_OPSUBSCR) {
        PUSH(e);
        auto argList = makeArgumentList(e);
        slice = new slice_t;
        slice->setArgs(argList.c_str());
        ep = POP(e);
    }
    
    switch (stype = ep->e_op) {
        case EXPR_SYM:
            var_expr = strdup(ep->e_sym_str);
            DPRINT("read_var_imm: sym var_expr is \"%s\"\n", var_expr);
            break;

        case EXPR_DSYM:
            sym = ep->e_sym;
            var_expr = strdup(SymLookup(sym));
            DPRINT("read_var_imm: dsym var_expr is \"%s\"\n", var_expr);
            break;

        case EXPR_STRING:
            var_expr = strdup(ep->e_string);
            DPRINT("read_var_imm: string var_expr is \"%s\"\n", var_expr);
            break;

        case EXPR_VAR:
            //
            // An EXPR_VAR means a process targeted command has been specified
            //
            // First evaluate the first part into a val, we expect this to be an
            // lval_gdb type, otherwise we don't want to do anything since we
            // can only query the backends for lval_gdb types.
            if ((val = evaluate_expr_imm(e, rec_set, f)) == nullptr) goto bad_var;

            // Now evaluate the second part into a val, this is the procset
            if ((ps_val = evaluate_expr_imm(e, rec_set, f)) == nullptr)
                goto bad_var;

            // Ensure that the type is rval_proc, otherwise there was an error.
            if (RVAL_ACTUAL_TYPE(ps_val) != rval_proc) {
                // Fix for BUG 816870
                if (LVAL_SYM(ps_val) == nullptr) {
                    SetError("Invalid process set syntax");
                } else {
                    SetError("No such process \"%s\"",
                             SymLookup(LVAL_SYM(ps_val)));
                }
                goto bad_var;
            }

            // Ensure that the procset actually has a procset assigned to it
            if (RVAL_ACTUAL_PROCSET(ps_val) == nullptr) {
                SetError("Invalid process set");
                goto bad_var;
            }

            break;

        case EXPR_OPDREF:
        case EXPR_OPADDROF:
        case EXPR_OPELEMENTREF:
        case EXPR_OPELEMENTPTR:
        case EXPR_OPELEMENTACC:
        case EXPR_OPCAST:
            var_expr = apply_var_op(ep, e);
            if (var_expr == nullptr) goto bad_var;
            DPRINT("read_var_imm: var_expr op is %s\n", var_expr);
            break;

        default:
            SetError("Symbol expected while evaluating variable");
            goto bad_var;
    }

    DPRINT("read_var_imm: stype is %s\n", getExprOpStr(stype));

    switch (stype) {
        case EXPR_SYM:
            /*
            ** FIXME: This is broken with the symbol table update.
            if ( f != (func_t *)nullptr )
            {
                    DPRINT("read_var_imm: f is not null\n");
                    if ( FindArg(&ap, sym, f) < 0 )
                            goto bad_var;

                    if ( ap != (arg_t *)nullptr )
                    {
                            DPRINT("read_var_imm: Found previous val for sym
            \"%s\"\n", SymLookup(sym)); val = CopyVal(ap->a_val); break;
                    }
            }*/

            /* drop through */

        case EXPR_OPDREF:
        case EXPR_OPADDROF:
        case EXPR_OPELEMENTREF:
        case EXPR_OPELEMENTPTR:
        case EXPR_OPELEMENTACC:
        case EXPR_OPCAST:
        case EXPR_STRING:

            // FIXME: This is a copy-paste hack. We should not be using BUFSIZ,
            // its incredibly dangerous.
            // FIXME: This should be a formal expression operator that is passed
            // to the backend instead of hacking together a random string.
            if (slice) {
                auto sliceexpr = slice->getExpr(var_expr);
                Free(var_expr);
                var_expr = strdup(sliceexpr.c_str());
            }

            if (repeat > 0) {
                // append "@{repeat}"
                auto extra_bytes = 1 + (static_cast<int>(::log10(repeat)) + 1) + 1; // '@' + digits of repeat + \0
                var_expr = static_cast<char *>(realloc(var_expr, strlen(var_expr) + extra_bytes));
                sprintf(var_expr + strlen(var_expr), "@%d", repeat);
            }

            // Always have procset be null here, the EXPR_VAR case above takes
            // care of modifying this if need be.
            val = NewVal(lval_gdb, nullptr, var_expr);
            DPRINT("read_var_imm: Created new val for %s\n", var_expr);

            break;

        case EXPR_DSYM:
            
            if ((val = GetVal(sym)) != nullptr) {
                DPRINT("read_var_imm: Found previous val for dsym \"%s\"\n",
                       SymLookup(sym));

                if (slice && slice->getRank() && slice->getExpr("").find('*') != std::string::npos){
                    DPRINT("assigning the size to a * in procset\n");
                    delete slice;
                    slice = new slice_t;
                    slice->addSlice(0, val->rval_u.rval_procset->s_nproc-1);
                }

                // if slice information is set, we need to create a new
                // reference.
                if (slice != nullptr) {
                    DPRINT(
                        "read_var_imm: Slicing information provided, creating "
                        "new reference to existing val.\n");
                    val = NewVal(lval_internal, LVAL_SYM(val), rval_ref, slice,
                                 RVAL_TYPE(val),
                                 CopyRval(RVAL_TYPE(val), RVAL_VAL(val)));
                }

                break;
            }

            /*
            ** Create new debugger var.
            */
            DPRINT("read_var_imm: Creating new debugger var\n");
            if (slice != nullptr) {
                DPRINT("read_var_imm: Slice is present\n");

                v1 = NewVal(lval_none, rval_none);
                val = NewVal(lval_none, rval_ref, slice, RVAL_TYPE(v1),
                             CopyRval(RVAL_TYPE(v1), RVAL_VAL(v1)));
                DestroyVal(v1);
            } else {
                DPRINT("read_var_imm: Creating rval_none val\n");
                val = NewVal(lval_none, rval_none);
            }

            DPRINT("read_var_imm: Assigning val to sym \"%s\"\n",
                   SymLookup(sym));
            if (AssignVal(LVAL_SYM_VAL(sym), val, rec_set) < 0) goto bad_var;

            DestroyVal(val);

            val = GetVal(sym);
            break;

        case EXPR_VAR:
            // Set the procset if the lval type is lval_gdb
            if (LVAL_TYPE(val) == lval_gdb) {
                // Check to see if we need to create a new temporary procset
                if (RVAL_TYPE(ps_val) == rval_ref) {
                    // Create a temporary proc_set
                    LVAL_GDB_PROCSET(val) = CreateTempProcSetSlice(
                        RVAL_ACTUAL_PROCSET(ps_val), RVAL_REF_SLICE(ps_val));

                    if (LVAL_GDB_PROCSET(val) == nullptr) {
                        PrintOutError(
                            "Failed to create temporary procset for "
                            "expression!\n");
                        goto bad_var;
                    }

                    if (LVAL_GDB_PROCSET(val)->s_nproc == 0) {
                        PrintOutError("Rank selection \"%s\" is invalid.\n", 
                            RVAL_REF_SLICE(ps_val)->getArgs());
                        FreeProcSet(LVAL_GDB_PROCSET(val));
                        goto bad_var;
                    }

                } else {
                    // Set the procset directly for the val
                    LVAL_GDB_PROCSET(val) = RVAL_ACTUAL_PROCSET(ps_val);
                }
            }
            break;

        default:
            break;
    }

    free(var_expr);

    return val;

bad_var:

    free(var_expr);

    delete slice;

    return nullptr;
}

/*
** Apply var operators handled by the backend
*/
static char *apply_var_op(expr_elt *ep, expr_t *e) {
    expr_elt *ep_next = NULL;
    char *v1 = NULL;
    char *v2 = NULL;
    char *v3 = NULL;
    char *v4 = NULL;
    char *v_res = NULL;
    int len, tmp, i;

    DPRINT("In apply_var_op\n");

    switch (ep->e_op) {
        case EXPR_DSYM: /* Not sure if DSYM will actually work or not... */
            v_res = strdup(SymLookup(ep->e_sym));
            break;

        case EXPR_SYM:
        case EXPR_TYPE_SPEC:
        case EXPR_TYPE_IDENT:
            v_res = strdup(ep->e_sym_str);
            break;

        case EXPR_TYPE_PTR:
            // There is an extra star op that we need to pop
            ep_next = POP(e);
            DPRINT("apply_var_op: popped expr_op %s\n",
                   getExprOpStr(ep_next->e_op));
            // ensure we found a star, if not push it back on
            if (ep_next->e_op != EXPR_STAR) {
                DPRINT("apply_var_op: Found something other than star!\n");
                PUSH(e);
            }
            v_res = strdup("*");
            break;

        case EXPR_INT:
            tmp = ep->e_int;
            // figure out the number of digits in the int
            // always a minimum of 1 digit
            len = 1;
            while (tmp /= 10) ++len;
            // don't forget the null term
            v_res = (char *)Alloc(len + 1);
            snprintf(v_res, len + 1, "%d", ep->e_int);
            break;

            /* FIXME: I am not handling float. Would it even be possible for a
               user to use a float as a gdb expression???
            */

        case EXPR_OPSUBSCR:
            ep_next = POP(e);
            DPRINT("apply_var_op: popped expr_op %s\n",
                   getExprOpStr(ep_next->e_op));

            // Handle fortran array syntax
            if (ep_next->e_op == EXPR_LIST_START) {
                // next element must be the length of the list
                ep_next = POP(e);
                DPRINT("apply_var_op: popped expr_op %s\n",
                       getExprOpStr(ep_next->e_op));
                if (ep_next->e_op != EXPR_INT) {
                    SetError("apply_var_op: Unknown Symbol %s encountered",
                             getExprOpStr(ep->e_op));
                    return NULL;
                }

                tmp = ep_next->e_int;

                // Handle the first element in the list
                if (tmp == 0) {
                    // there are no elements in the list, create the
                    // empty content
                    len = 1;
                    v1 = (char *)Alloc(len);
                    snprintf(v1, len, "%s", "");
                } else {
                    // length is at least one, so create the first element
                    ep_next = POP(e);
                    DPRINT("apply_var_op: popped expr_op %s\n",
                           getExprOpStr(ep_next->e_op));
                    v1 = apply_var_op(ep_next, e);
                    if (v1 == NULL) return NULL;
                }

                // Handle the rest of the elements
                for (i = 1; i < tmp; ++i) {
                    // Remember that this is in LALR notation
                    ep_next = POP(e);
                    DPRINT("apply_var_op: popped expr_op %s\n",
                           getExprOpStr(ep_next->e_op));
                    v2 = apply_var_op(ep_next, e);
                    if (v2 == NULL) return NULL;
                    len = strlen(v2) + strlen(",") + strlen(v1) + 1;
                    v3 = (char *)Alloc(len);
                    snprintf(v3, len, "%s,%s", v2, v1);
                    Free(v1);
                    Free(v2);
                    v1 = v3;
                }

                // Create the final array string
                len = strlen("(") + strlen(v1) + strlen(")") + 1;
                v2 = (char *)Alloc(len);
                snprintf(v2, len, "(%s)", v1);
                Free(v1);
            } else {
                // This is C array syntax
                v1 = apply_var_op(ep_next, e);
                if (v1 == NULL) return NULL;
                len = strlen("[") + strlen(v1) + strlen("]") + 1;
                v2 = (char *)Alloc(len);
                snprintf(v2, len, "%s%s%s", "[", v1, "]");
                Free(v1);
            }

            // get the symbol name
            ep_next = POP(e);
            DPRINT("apply_var_op: popped expr_op %s\n",
                   getExprOpStr(ep_next->e_op));
            v1 = apply_var_op(ep_next, e);
            if (v1 == NULL) {
                Free(v2);
                return NULL;
            }

            // Create the final array string
            len = strlen(v1) + strlen(v2) + 1;
            v_res = (char *)Alloc(len);
            snprintf(v_res, len, "%s%s", v1, v2);
            Free(v1);
            Free(v2);
            break;

        /* Note that for cast operations, we only really support the expression
           types that this function supports. You cannot do funky things with
           this (like slicing).
        */
        case EXPR_OPCAST:
            // first get the var(s)
            ep_next = POP(e);
            DPRINT("apply_var_op: popped expr_op %s\n",
                   getExprOpStr(ep_next->e_op));
            v2 = apply_var_op(ep_next, e);
            if (v2 == NULL) return NULL;
            // next get the type
            ep_next = POP(e);
            DPRINT("apply_var_op: popped expr_op %s\n",
                   getExprOpStr(ep_next->e_op));
            if (ep_next->e_op == EXPR_TYPE) {
                ep_next = POP(e);
                DPRINT("apply_var_op: popped expr_op %s\n",
                       getExprOpStr(ep_next->e_op));
            } else {
                Free(v2);
                SetError("apply_var_op: Unknown EXPR %s encountered",
                         getExprOpStr(ep_next->e_op));
                return NULL;
            }
            do {
                v1 = apply_var_op(ep_next, e);
                if (v1 == NULL) {
                    Free(v2);
                    return NULL;
                }
                if (v3 != NULL) {
                    len = strlen(v1) + strlen(v3) + 2;
                    v4 = (char *)Alloc(len);
                    snprintf(v4, len, "%s %s", v1, v3);
                    v3 = v4;
                } else {
                    v3 = v1;
                }
                ep_next = POP(e);
                DPRINT("apply_var_op: popped expr_op %s\n",
                       getExprOpStr(ep_next->e_op));
            } while (ep_next->e_op != EXPR_TYPE);
            // reset v1 to v3
            v1 = v3;

            len = strlen("(") + strlen(v1) + strlen(")") + strlen(v2) + 1;
            v_res = (char *)Alloc(len);
            snprintf(v_res, len, "%s%s%s%s", "(", v1, ")", v2);
            Free(v1);
            Free(v2);
            break;

        case EXPR_OPDREF:
            ep_next = POP(e);
            DPRINT("apply_var_op: popped expr_op %s\n",
                   getExprOpStr(ep_next->e_op));
            v1 = apply_var_op(ep_next, e);
            if (v1 == NULL) return NULL;
            // There is an extra star op that we need to pop
            ep_next = POP(e);
            DPRINT("apply_var_op: popped expr_op %s\n",
                   getExprOpStr(ep_next->e_op));
            // ensure we found a star, if not push it back on
            if (ep_next->e_op != EXPR_STAR) {
                DPRINT("apply_var_op: Found something other than star!\n");
                PUSH(e);
            }
            len = strlen(v1) + strlen("*") + 1;
            v_res = (char *)Alloc(len);
            snprintf(v_res, len, "%s%s", "*", v1);
            Free(v1);
            break;

        case EXPR_OPADDROF:
            ep_next = POP(e);
            DPRINT("apply_var_op: popped expr_op %s\n",
                   getExprOpStr(ep_next->e_op));
            v1 = apply_var_op(ep_next, e);
            if (v1 == NULL) return NULL;
            len = strlen(v1) + strlen("&") + 1;
            v_res = (char *)Alloc(len);
            snprintf(v_res, len, "%s%s", "&", v1);
            Free(v1);
            break;

        case EXPR_OPELEMENTREF:
            ep_next = POP(e);
            DPRINT("apply_var_op: popped expr_op %s\n",
                   getExprOpStr(ep_next->e_op));
            v2 = apply_var_op(ep_next, e);
            if (v2 == NULL) return NULL;
            ep_next = POP(e);
            DPRINT("apply_var_op: popped expr_op %s\n",
                   getExprOpStr(ep_next->e_op));
            v1 = apply_var_op(ep_next, e);
            if (v1 == NULL) {
                Free(v2);
                return NULL;
            }
            len = strlen(v1) + strlen(".") + strlen(v2) + 1;
            v_res = (char *)Alloc(len);
            snprintf(v_res, len, "%s%s%s", v1, ".", v2);
            Free(v1);
            Free(v2);
            break;

        case EXPR_OPELEMENTPTR:
            ep_next = POP(e);
            DPRINT("apply_var_op: popped expr_op %s\n",
                   getExprOpStr(ep_next->e_op));
            v2 = apply_var_op(ep_next, e);
            if (v2 == NULL) return NULL;
            ep_next = POP(e);
            DPRINT("apply_var_op: popped expr_op %s\n",
                   getExprOpStr(ep_next->e_op));
            v1 = apply_var_op(ep_next, e);
            if (v1 == NULL) {
                Free(v2);
                return NULL;
            }
            len = strlen(v1) + strlen("->") + strlen(v2) + 1;
            v_res = (char *)Alloc(len);
            snprintf(v_res, len, "%s%s%s", v1, "->", v2);
            Free(v1);
            Free(v2);
            break;

        case EXPR_OPELEMENTACC:
            ep_next = POP(e);
            DPRINT("apply_var_op: popped expr_op %s\n",
                   getExprOpStr(ep_next->e_op));
            v2 = apply_var_op(ep_next, e);
            if (v2 == NULL) return NULL;
            ep_next = POP(e);
            DPRINT("apply_var_op: popped expr_op %s\n",
                   getExprOpStr(ep_next->e_op));
            v1 = apply_var_op(ep_next, e);
            if (v1 == NULL) {
                Free(v2);
                return NULL;
            }
            len = strlen(v1) + strlen("%") + strlen(v2) + 1;
            v_res = (char *)Alloc(len);
            snprintf(v_res, len, "%s%s%s", v1, "%", v2);
            Free(v1);
            Free(v2);
            break;

        case EXPR_LIST:
            // list element, ignore this token.
            ep_next = POP(e);
            DPRINT("apply_var_op: popped expr_op %s\n",
                   getExprOpStr(ep_next->e_op));
            v_res = apply_var_op(ep_next, e);
            break;

        default:
            SetError("apply_var_op: Unknown Symbol %s encountered",
                     getExprOpStr(ep->e_op));
            return NULL;
    }

    DPRINT("apply_var_op: returning v_res %s\n", v_res);
    return v_res;
}

/*
** Apply unary, binary or tertiary operator on actual AIF data
** NOTE - This is a helper function for apply_op and should not be
** called directly.
*/
static AIFStream *apply_op_aif(expr_op op, AIFStream *a1, AIFStream *a2,
                               AIFStream::Difference* diffArg) {
    int r1;
    int r2;
    AIFStream *ar = NULL;
    AIFStream::Difference localDiff, &diff = (diffArg ? *diffArg : localDiff);
    std::string errMsg;
    DPRINT("In apply_op_aif\n");

    switch (op) {
        case EXPR_OPADD:
            if (!(ar = AIFStream::add(a1, a2))) {
                SetError("%s", AIFErrorStr());
                return NULL;
            }

            break;

        case EXPR_OPSUB:
            if (!(ar = AIFStream::sub(a1, a2))) {
                SetError("%s", AIFErrorStr());
                return NULL;
            }

            break;

        case EXPR_OPMUL:
            if (!(ar = AIFStream::mul(a1, a2))) {
                SetError("%s", AIFErrorStr());
                return NULL;
            }

            break;

        case EXPR_OPDIV:
            if (!(ar = AIFStream::div(a1, a2))) {
                SetError("%s", AIFErrorStr());
                return NULL;
            }

            break;

        // TODO: Broken to fix derived fortran types
        case EXPR_OPREM:
            if (!(ar = AIFStream::rem(a1, a2))) {
                SetError("%s", AIFErrorStr());
                return NULL;
            }

            break;

        case EXPR_OPEQUAL:
            // TODO: Add epsilon here
            if (AIFStream::compare(a1, a2, &r1, diff, &Client.cmp_lower_eps,
                                   &Client.cmp_upper_eps, errMsg) < 0) {
                SetError("%s", errMsg.c_str());
                return NULL;
            }

            ar = AIFStream::fromInt(r1);
            break;

        case EXPR_OPNOTEQUAL:
            if (AIFStream::compare(a1, a2, &r1, diff, &Client.cmp_lower_eps,
                                   &Client.cmp_upper_eps, errMsg) < 0) {
                SetError("%s", errMsg.c_str());
                return NULL;
            } else if (r1 == 3) {
                SetError("Operator not supported for comparison.");
                return NULL;
            }

            ar = AIFStream::fromInt(r1 == 0);
            break;

        case EXPR_OPLEQ:
            if (AIFStream::compare(a1, a2, &r1, diff, &Client.cmp_lower_eps,
                                   &Client.cmp_upper_eps, errMsg) < 0) {
                SetError("%s", errMsg.c_str());
                return NULL;
            } else if (r1 == 3) {
                SetError("Operator not supported for comparison.");
                return NULL;
            }

            ar = AIFStream::fromInt(r1 > 0);  // note a value of 0 means the condition passes
            break;

        case EXPR_OPGEQ:
            if (AIFStream::compare(a1, a2, &r1, diff, &Client.cmp_lower_eps,
                                   &Client.cmp_upper_eps, errMsg) < 0) {
                SetError("%s", errMsg.c_str());
                return NULL;
            } else if (r1 == 3) {
                SetError("Operator not supported for comparison.");
                return NULL;
            }

            ar = AIFStream::fromInt(r1 <
                          0);  // note a value of 0 means the condition passes
            break;

        case EXPR_OPLT:
            if (AIFStream::compare(a1, a2, &r1, diff, &Client.cmp_lower_eps,
                                   &Client.cmp_upper_eps, errMsg) < 0) {
                SetError("%s", errMsg.c_str());
                return NULL;
            } else if (r1 == 3) {
                SetError("Operator not supported for comparison.");
                return NULL;
            }

            ar = AIFStream::fromInt(r1 >= 0);  // note: a value of 0 means the condition passes
            break;

        case EXPR_OPGT:
            if (AIFStream::compare(a1, a2, &r1, diff, &Client.cmp_lower_eps,
                                   &Client.cmp_upper_eps, errMsg) < 0) {
                SetError("%s", errMsg.c_str());
                return NULL;
            } else if (r1 == 3) {
                SetError("Operator not supported for comparison.");
                return NULL;
            }

            ar = AIFStream::fromInt(r1 <= 0);  // note: a value of 0 means the condition passes
            break;

        case EXPR_OPAND:
            if (a1->isZero(r1) < 0 || a2->isZero(r2) < 0) {
                SetError("%s", AIFErrorStr());
                return NULL;
            }

            ar = AIFStream::fromInt((int)(!r1 && !r2));
            break;

        case EXPR_OPOR:
            if (a1->isZero(r1) < 0 || a2->isZero(r2)) {
                SetError("%s", AIFErrorStr());
                return NULL;
            }

            ar = AIFStream::fromInt((int)(!r1 || !r2));
            break;

        case EXPR_OPPOS:
            ar = a1->clone();
            break;

        case EXPR_OPNEG:
            if (!(ar = AIFStream::neg(a1))) {
                SetError("%s", AIFErrorStr());
                return NULL;
            }

            break;

        default:
            return NULL;
    }

    return ar;
}

static val_t *apply_opcond(val_t *v1, val_t *v2, val_t *v3, rec_set_t *rec_set) {
    int r1;
    AIFStream *a1;
    rep_set_t *rep_set;
    group_reply_t *reply;
    rep_set_t *rep_set2;
    group_reply_t *reply2;
    rep_set_t *new_rep_set;
    group_reply_t *new_reply;
    val_t *tmp;
    dbgevent_t *ev;

    if (v1 == NULL) {
        SetError("apply_op(): ternary conditional val is null!");
        return (val_t *)NULL;
    }

    if (v2 == NULL || v3 == NULL) {
        SetError("apply_op(): ternary val is null!");
        return (val_t *)NULL;
    }

    // Apply the ternary based on the val type
    switch (LVAL_TYPE(v1)) {
    case lval_blockmap:
    case lval_gdb:
        // fetch the rval
        if (VAL_RETRIEVE(v1, rec_set)) {
            return (val_t *)NULL;
        }
        // drop through
    case lval_none:
    case lval_internal:
        // switch on the rval type
        switch (RVAL_TYPE(v1)) {
        case rval_aif:
            // can directly access the aif data
            a1 = RVAL_AIF(v1);

        apply_op_aif_direct:
            if (a1->isZero(r1) < 0) {
                SetError("%s", AIFErrorStr());
                return (val_t *)NULL;
            }

            // point at the return val
            tmp = !r1 ? v2 : v3;
            // switch on the return val
            switch (RVAL_TYPE(tmp)) {
            case rval_none:
                return NewVal(lval_none, rval_none);
            case rval_aif:
                // Make sure the rval contents get refreshed
                if (VAL_RETRIEVE(tmp, rec_set)) {
                    return (val_t *)NULL;
                }
                return NewVal(lval_none, rval_aif,
                              RVAL_AIF(tmp));
            case rval_blockmap:
                return NewVal(lval_none, rval_blockmap,
                              RVAL_BLOCKMAP(tmp));
            case rval_reply:
                // Make sure the rval contents get refreshed
                if (VAL_RETRIEVE(tmp, rec_set)) {
                    return (val_t *)NULL;
                }
                return NewVal(lval_none, rval_reply,
                              CopyRepSet(RVAL_REPLY(tmp)));
            case rval_proc:
                return NewVal(lval_none, rval_proc,
                              RVAL_PROCSET(tmp));
            case rval_ref:
                // FIXME: How to support this?
                SetError(
                    "Reference val not supported in "
                    "ternary operator.");
                return (val_t *)NULL;
            }

            // Shouldn't get here
            SetError(
                "apply_op(): Reached end of control "
                "statement!");
            return (val_t *)NULL;

        case rval_reply:
            // point at the reply set
            rep_set = RVAL_REPLY(v1);

            // If there is only one reply in the set, we can
            // treat this as an aif type and do the same thing
            // as we did above
            if (GetNumberOfReply(rep_set) == 1) {
                reply = rep_set->getFirstReply();
                // point directly at the AIF data
                a1 = EVENT_DATA(reply->rep_event);
                // goto the code above
                goto apply_op_aif_direct;
            }

            // Need to loop over the reply, create a new rep_set
            // that will contain the bitset of the v1 reply
            // along with the data of either v2 or v3 depending
            // on true/false value
            new_rep_set = NewRepSet();

            for (auto&& reply : rep_set) {
                // point at the data
                a1 = EVENT_DATA(reply->rep_event);
                if (a1->isZero(r1) < 0) {
                    SetError("%s", AIFErrorStr());
                    // Make sure cleanup happens
                    FreeRepSet(new_rep_set);
                    return (val_t *)NULL;
                }

                // point at the return val
                tmp = !r1 ? v2 : v3;
                // switch on the return val
                switch (RVAL_TYPE(tmp)) {
                    // These rval types are invalid when the
                    // conditional is a reply set. We cannot
                    // have anything other than data events in
                    // the reply
                case rval_none:
                case rval_blockmap:
                case rval_proc:
                case rval_ref:
                    SetError(
                        "Specified value not supported "
                        "when conditional evaluates to "
                        "reply set.");
                    // Make sure cleanup happens
                    FreeRepSet(new_rep_set);
                    return (val_t *)NULL;

                case rval_aif:
                    // Make sure the rval contents get
                    // refreshed
                    if (VAL_RETRIEVE(tmp, rec_set)) {
                        // Make sure cleanup happens
                        FreeRepSet(new_rep_set);
                        return (val_t *)NULL;
                    }
                    // Make a new event out of the AIF data,
                    // create a new reply based on the
                    // original replies bitset and this new
                    // event, and finally add it to the new
                    // reply set
                    ev = NewEvent(DBGEV_DATA);
                    ev->ev_data = RVAL_AIF(tmp)->clone();
                    new_reply = NewGroupReply(
                        reply->rep_di,
                        reply->rep_group->clone(), ev);
                    AddReplyToSet(new_rep_set, new_reply);
                    break;

                case rval_reply:
                    // Make sure the rval contents get
                    // refreshed
                    if (VAL_RETRIEVE(tmp, rec_set)) {
                        // Make sure cleanup happens
                        FreeRepSet(new_rep_set);
                        return (val_t *)NULL;
                    }

                    // need to loop over the reply set and
                    // OR the individual group replies with
                    // the bitset from v1. If the bitset is
                    // empty, we don't add it to the new
                    // reply set. If it isn't empty we add
                    // it.
                    rep_set2 = RVAL_REPLY(tmp);

                    // If there is only one reply in the
                    // set, we can assign directly without
                    // any checks on the bitset
                    if (GetNumberOfReply(rep_set) == 1) {
                        reply2 = rep_set2->getFirstReply();
                        new_reply = NewGroupReply(
                            reply->rep_di,
                            reply->rep_group->clone(),
                            DupEvent(reply2->rep_event));
                        AddReplyToSet(new_rep_set,
                                      new_reply);
                        break;
                    } else {
                        // check to make sure the lval and
                        // rval bitset sizes match for the
                        // replies, otherwise throw an error
                        reply2 = rep_set2->getFirstReply();
                        if (reply->rep_group->sizeInBits() != reply2->rep_group->sizeInBits()) {
                            SetError(
                                "Attempted to apply "
                                "ternary operator to two "
                                "sets with different "
                                "sizes.");
                            // Make sure cleanup happens
                            FreeRepSet(new_rep_set);
                            return (val_t *)NULL;
                        }
                    }

                    // loop over this reply
                    for (auto&& reply2 : rep_set2) {
                        // compute the new set
                        auto tset = bitset::fromAnd(*reply->rep_group, *reply2->rep_group);

                        // check to see if this is empty, if
                        // so continue
                        if (tset->isEmpty()) {
                            continue;
                        }

                        // create the new reply and add it
                        // to the set
                        new_reply = NewGroupReply(
                            reply2->rep_di, std::move(tset),
                            DupEvent(reply2->rep_event));
                        AddReplyToSet(new_rep_set, new_reply);
                    }
                    break;
                }
            }

            // Return the new reply
            return NewVal(lval_none, rval_reply, new_rep_set);

            // These are all invalid conditionals for ternary
            // operations
        case rval_ref:
        case rval_none:
        case rval_blockmap:
        case rval_proc:
            SetError(
                "Conditional val is invalid to apply ternary "
                "operator.");
            return (val_t *)NULL;
        }
    }
    // Shouldn't get here
    SetError("apply_op(): Reached end of control statement!");
    return (val_t *)NULL;
}

// check that a value type is usuable for an operation
static bool op_val_is_ok(val_t* v, rec_set_t* rec_set, const char* message)
{
    switch (LVAL_TYPE(v)) {
    case lval_blockmap:
    case lval_gdb:
        // fetch the rval
        if (VAL_RETRIEVE(v, rec_set)) {
            return false;
        }
    case lval_none:
    case lval_internal:
        switch (RVAL_TYPE(v)) {
        case rval_aif:
        case rval_reply:
            break;

            // These are all invalid for binary operations
        case rval_ref:
        case rval_none:
        case rval_blockmap:
        case rval_proc:
            SetError(message);
            return false;
        }
    }
    return true;
}

namespace {
    // little struct to apply common processing for rval_aif and rval_reply values
    struct mini_reply {
        mini_reply(AIFStream* aaif, const Bitset* agroup) : aif(aaif), group(agroup) {}
        AIFStream* aif;
        const Bitset* group;
    };
}

// get the next slice of the result reply from an aif or reply value object and
// store the replies in \a slice.   Return false if there are are no more slices to
// process.   The reply argument hold the reply slice on return and its lifetime controls
// the lifetime of the object in \a slice.
static bool rval_next_slice(val_t* v, std::unique_ptr<rep_set_t>& reply, std::vector<mini_reply>& slice) {
    switch (RVAL_TYPE(v)) {
    default:
        break;
    case rval_aif:
        if (slice.empty()) {
            slice.push_back( mini_reply(RVAL_AIF(v), nullptr));
            return true;
        }
        // return false after the first time: there isn't anything new to process
        break;

    case rval_reply:
        slice.clear();
        auto repSet = RVAL_REPLY(v);
        if ((reply = repSet->getNextSlice())) {
            for (auto&& r : reply.get()) {
                slice.push_back(mini_reply( EVENT_DATA(r->rep_event), r->rep_group.get()));
            }
            return true;
        }
        break;
    }
    return false;
}

// compute the intersection of two bitsets, treating a null-ptr as all 1's
static std::unique_ptr<Bitset> intersect_groups(const Bitset* a, const Bitset* b) {
    if (a && b)
        return bitset::fromAnd(*a, *b);
    else if (a)
        return a->clone();
    else if (b)
        return b->clone();
    else
        return nullptr;
}

// create a group reply by wrapping a single AIF data object.   Takes ownership of
// group and aif.
static group_reply_t* make_reply_from_aif(dbginvoke_t* di, std::unique_ptr<Bitset> &&group, AIFStream* aif) {
    if (aif) {
        auto ev = NewEvent(DBGEV_DATA);
        ev->ev_data = aif;
        return NewGroupReply(di, std::move(group), ev);
    } else {
        return nullptr;
    }
}

// determine the correct result from a comparison operation over the union of
// two independent sets.
// \param oldVal is the comparison result of the array thus far
// \param addVal is the comparison result of the next slice
// \param isNeq is true if the comparison operation is "!=" which changes
//    the sense of the combination
// \return the new comparison result.
static int combine_compare_vals(int oldVal, int addVal, bool isNeq)
{
    // note that the value for true is 0.
    if (isNeq) {
        return ((!oldVal || !addVal) ? 0 : 1);
    }

    // comparison values are 0=true, other value priorties are:
    // (-1,1)->1, (-2,2)->2, 3, 0
    switch (addVal) {
    case -1:
    case 1:
        return 1;
        break;
    case -2:
    case 2:
        if (oldVal!=1)
            return 2;
        break;
    case 3:
        if(oldVal!=1 && oldVal !=2)
            return 3;
        break;
    }

    return oldVal;
}

// merge the comparison result on an array slice into the full result.  The comparison result
// is a AIF integer with results 0,1,2,3 for true, false, warn error respectively.   Here we've
// done the comparison on a slice, but since this need not match the comparison result of any
// other slice, we need a reduction step to get a final answer.
// \param repSet is the comparison result thus far
// \param group is the set of processes with the same comparison result on a slice.   The
//            function takes ownership of \a group.
// \param aif it the aif integer comparison result on the slice.   The function takes ownership
//       of \a aif
// \param op is the operation being applied.
static void add_to_compare_reply(rep_set_t& repSet, std::unique_ptr<Bitset> &&group, AIFStream*&& aif, expr_op op) {
    repSet.writeTransformedReply(std::move(group), std::move(aif), [op](AIFStream* oldData, AIFStream* newData) -> AIFStream* {
        // XXX: params should be passed as const
        if (!oldData || !newData) return nullptr;

        int addVal;
        if (newData->toInt(addVal)) {
            DPRINT("not a compare result\n");
            return nullptr;
        }

        int oldVal;
        oldData->toInt(oldVal);
        int newVal = combine_compare_vals(oldVal, addVal, op == EXPR_OPNOTEQUAL);

        return AIFStream::fromInt(newVal);
    });
}

// create a group reply containing the content in \a aif and add it to \a repSet
// note : takes ownership of \a group and \a aif
static void add_to_reply(rep_set_t& repSet, dbginvoke_t* di, std::unique_ptr<Bitset> &&group, AIFStream* aif)
{
    if (auto rep = make_reply_from_aif(di, std::move(group), aif)) {
        repSet.addReply(rep);
    }
}

// apply a binary operation when both operands are just AIF types
static val_t* apply_binary_op_aif(expr_op op, AIFStream* a1, AIFStream* a2, bool check_diff,
                                  val_t **diff) {
    AIFStream::Difference aifDiff, *darg = (check_diff ? &aifDiff : nullptr);
    if (AIFStream* res = apply_op_aif(op, a1, a2, darg)) {
        if (darg && diff)
            *diff = NewVal(lval_none, rval_aif, darg->getResultAIF());
        return NewVal(lval_none, rval_aif, res);
    } else {
        return nullptr;
    }
}

// apply a binary operation to two values and return the result
// if check_diff is true, also return the delta array via \a diff
static val_t* apply_binary_op(expr_op op, val_t *v1, val_t *v2, rec_set_t *rec_set, bool check_diff,
                              val_t **diff) {
    if (diff) {
        *diff = nullptr;
    }

    // First check to make sure the supplied vals are safe to operate on

    if (!v1 || !v2) {
        SetError("apply_op(): binary operator called with missing val!");
        return nullptr;
    }
    if (!op_val_is_ok(v1, rec_set, "First val is invalid to apply binary operator."))
        return nullptr;
    if (!op_val_is_ok(v2, rec_set, "Second val is invalid to apply binary operator."))
        return nullptr;

    bool isCompare = false;
    if (op >= EXPR_OPEQUAL && op <= EXPR_OPGT) isCompare = true;

    // boil down aif and replies to a common workable form - note that if the r-value type
    // is AIF mini_reply.group == nullptr

    std::vector<std::unique_ptr<rep_set_t>> newSlices, diffSlices;

    //-----------------------------------------------------------------------
    // retrieve the first slice
    //-----------------------------------------------------------------------
    // we do extra processing on the first slice to determine the type we are working with
    std::unique_ptr<rep_set_t> reply1,reply2;
    std::vector<mini_reply> reps1, reps2;
    bool toProc1 = rval_next_slice(v1,reply1,reps1);
    bool toProc2 = rval_next_slice(v2,reply2,reps2);

    bool resultIsRepSet = true;

    if (reps1.size() >= 1 && reps2.size() >= 1) {
        // check that the bitset sizes match - and determine if our result should be a repset
        auto &r1 = reps1[0], &r2 = reps2[0];
        if (r1.group && r2.group && r1.group->sizeInBits() != r2.group->sizeInBits()) {
            if (reps1.size() == 1 && reps2.size() == 1) {
                // special case = we go forward just treating these as AIFs
                // FIXME: Alert the user somehow that this is a special set.
                r1.group = r2.group = nullptr;
            } else {
                SetError("Attempted to apply binary operator to two val sets with "
                         "different sizes.");
                return nullptr;
            }
        }

        // determine if the result is a rep_set - note that only a rep_set can lead to an
        // *empty* reps set - so we'll treat that like a rep_set to match old behavior
        if (!r1.group && !r2.group)
            resultIsRepSet = false;
    }

    if (!resultIsRepSet) {
        if (reps1.size() == 1 && reps2.size() == 1) {
            return apply_binary_op_aif(op, reps1[0].aif, reps2[0].aif, check_diff, diff);
        } else {
            SetError("Attempted to apply binary operator to a null AIF");
            return nullptr;
        }
    }

    // storage for the return values.   
    std::unique_ptr<rep_set_t> newRepSet;
    if (isCompare) {
        newRepSet.reset(new rep_set_t);
    }

    std::shared_ptr<Bitset> diffGroup;
    // we need track the involved process set for setSparseValues (otherwise
    // an process for which every element passes would not be in the result)
    for (auto&& reply : reps1) {
        if (!diffGroup)
            diffGroup = reply.group->clone();
        else
            *diffGroup |= *reply.group;
    }
    
    //-----------------------------------------------------------------------
    // Process each slice -- add the result to the reply
    //-----------------------------------------------------------------------
    for (int sliceCnt=0; toProc1 || toProc2;
         ++sliceCnt, toProc1=rval_next_slice(v1,reply1,reps1), toProc2 = rval_next_slice(v2,reply2,reps2)) {

        std::unique_ptr<rep_set_t> newRepSlice, diffRepSlice;
        
        if (!sliceCnt || !isCompare)
            newRepSlice.reset(NewRepSet());
        if (check_diff)
            diffRepSlice.reset(NewRepSet());

        //-----------------------------------------------------------------------
        // Main comparison operation
        //-----------------------------------------------------------------------
        // Apply the op to all pairwise results that share a group. We make an
        // exception to the "share a group" rule when the full set of replies is
        // only one rank in size on each side.
        // we could be a smarter about finding overlapping replies, but the previous version
        // was just n-squared so I'll just repeat that for now
        bool isFirst = true;
        for (auto&& r1 : reps1) {
            for (auto&& r2 : reps2) {
                bool is_single = false;
                if (r1.group && r2.group && !r1.group->hasCommonBits(*r2.group)) {
                    if (reps1.size() == 1 && reps2.size() == 1 && 
                        r1.group->numSet() == 1 && r2.group->numSet() == 1)
                        is_single = true;
                    else
                        continue;
                }

                AIFStream::Difference diff, *darg = (check_diff ? &diff : nullptr);
                AIFStream* a1 = r1.aif, *a2 = r2.aif;
                AIFStream* res = apply_op_aif(op, a1, a2, darg);
                if (!res) {
                    return nullptr; // error
                }

                // intersect if we have groups larger than 1 rank in size, 
                // otherwise it is fine to union them
                auto tset = is_single ? r1.group->clone() : intersect_groups(r1.group, r2.group);
                
                if (isCompare) {
                    // consumes res
                    add_to_compare_reply(*newRepSet, tset->clone(), std::move(res), op);
                } else {
                    // we intentionally set the di to null rather than picking an arbitrary one
                    add_to_reply(*newRepSlice, nullptr, tset->clone(), res);
                }

                if (diffRepSlice && darg) {
                    add_to_reply(*diffRepSlice, nullptr, std::move(tset), darg->getResultAIF());
                }

                // we need to track the array size because the diff is only sparsely
                // defined
                if (isFirst && check_diff) {
                    isFirst = false;
                }
            } ///for (auto&& r2 : reps2)
        } // for (auto&& r1 : reps1)

        // combine the slice result with the previous result
        if (!isCompare)
            newSlices.push_back(move(newRepSlice));
        if (diffRepSlice)
            diffSlices.push_back(move(diffRepSlice));
    } // for (each slice)

    // create the reply sets
    if (!newRepSet) {
        if (newSlices.size() == 1) {
            newRepSet = move(newSlices[0]);
            newSlices.clear();
        } else if (newSlices.size() > 1) {
            newRepSet.reset(new rep_set_t(newSlices, nullptr));
        }
    }

    rep_set_t* diffRepSet = nullptr;
    if (check_diff) {
        diffRepSet = new rep_set_t(diffSlices, diffGroup.get());
    }
        

    // create the value result for return and possibly diff.
    val_t* retVal = nullptr;

    if (newRepSet) {
        retVal = NewVal(lval_none, rval_reply, newRepSet.release());
    } 

    if (diff && diffRepSet) {
        *diff = NewVal(lval_none, rval_reply, diffRepSet);
    }
        
    return retVal;
}
            

// apply a unary operator to a value \a v1 and return the new result
static val_t* apply_unary_op(expr_op op, val_t *v1, rec_set_t *rec_set) {
    AIFStream *a1;
    rep_set_t *rep_set;
    rep_set_t *new_rep_set;

    if (v1 == NULL) {
        SetError("apply_op(): unary operator called with missing val!");
        return nullptr;
    }

    if (!op_val_is_ok(v1, rec_set, "Val is invalid to apply unary operator.")) {
        return nullptr;
    }

    // operate on the val
    switch (RVAL_TYPE(v1)) {
    case rval_aif:
        // can directly access the aif data
        a1 = RVAL_AIF(v1);

        // return the result
        if (AIFStream* aif_res = apply_op_aif(op, a1, nullptr, nullptr)) {
            return NewVal(lval_none, rval_aif, aif_res);
        } else {
            return nullptr;
        }


    case rval_reply:
        // point at the reply set
        rep_set = RVAL_REPLY(v1);

        // we always want to recreate the set for unary operations

        // Need to loop over the reply, create a new rep_set that
        // will contain the bitset of the v1 reply
        new_rep_set = NewRepSet();

        for (auto&& reply : rep_set) {
            // point at the data
            a1 = EVENT_DATA(reply->rep_event);

            // apply the operation
            if (AIFStream* aif_res = apply_op_aif(op, a1, nullptr, nullptr)) {
                // Make a new event out of the AIF data, use the replies
                // bitset
                add_to_reply(*new_rep_set, reply->rep_di, reply->rep_group->clone(),
                             aif_res);
            } else {
                FreeRepSet(new_rep_set);
                return nullptr;
            }
        }

        // Return the new reply
        return NewVal(lval_none, rval_reply, new_rep_set);

        // Shouldn't get here
    default:
        SetError("apply_op(): Reached end of control statement!");
        return (val_t *)NULL;
    }

    // Shouldn't get here
    SetError("apply_op(): Reached end of control statement!");
    return (val_t *)NULL;
}
    
/*
** Apply unary, binary or tertiary operator to a val.
   The result will have no dbginvoke since v1 and v2 (v3) potentially come from
   different trees.  The print will just not include procset info.
*/
static val_t *apply_op(expr_op op, val_t *v1, val_t *v2, val_t *v3,
                       rec_set_t *rec_set, val_t **diff) {
    bool check_diff = false;

    DPRINT("In apply_op\n");

    switch (op) {
        case EXPR_OPCOND:
            return apply_opcond(v1, v2, v3, rec_set);

        // All comparison operators *MUST* go here in order for diff to work
        // properly
        case EXPR_OPEQUAL:
        case EXPR_OPNOTEQUAL:
        case EXPR_OPLEQ:
        case EXPR_OPGEQ:
        case EXPR_OPLT:
        case EXPR_OPGT:
            // check to see if we should calculate the different result in situ.
            if (diff != NULL) check_diff = true;

        case EXPR_OPADD:
        case EXPR_OPSUB:
        case EXPR_OPMUL:
        case EXPR_OPDIV:
        // TODO: Broken to fix derived fortran types
        case EXPR_OPREM:
        case EXPR_OPAND:
        case EXPR_OPOR:
            return apply_binary_op(op, v1, v2, rec_set, check_diff, diff);
            
        case EXPR_OPPOS:
        case EXPR_OPNEG:
            // First check to make sure the supplied val is safe to operate on
            return apply_unary_op(op, v1, rec_set);
            
        default:
            SetError("Invalid operation");
            return (val_t *)NULL;
    }
}


static var_t *ReadVar_BlockMap(expr_t *e, rec_set_t *rec_set, CMD_MODE mode) {
    int smn;
    int smx;
    val_t *val;
    var_t *var = (var_t *)NULL;
    expr_elt *ep;

    DPRINT("In ReadVar_Blockmap. cmd_mode[%d]\n", mode);

    var = NewVar();

    ep = POP(e);

    DPRINT("ReadVar_Blockmap: popped expr_op %s\n", getExprOpStr(ep->e_op));

    if (mode == CMD_MODE_DEFERRED) {
        if (ep->e_op != EXPR_LOC) {
            SetError(
                "Must specify location information in declarative mode. (i.e. "
                "myVar@\"foo.c\":10)");
            goto bad_var;
        }

        ep = POP(e);

        DPRINT("ReadVar_Blockmap: popped expr_op %s\n", getExprOpStr(ep->e_op));

        if (ep->e_op != EXPR_INT) {
            SetError("Line number expected. (i.e. \"foo.c\":10)");
            goto bad_var;
        }

        var->v_lno = ep->e_int;

        DPRINT("ReadVar_Blockmap: line number is %d\n", ep->e_int);

        ep = POP(e);

        DPRINT("ReadVar_Blockmap: popped expr_op %s\n", getExprOpStr(ep->e_op));

        if (ep->e_op != EXPR_STRING) {
            SetError(
                "Filename enclosed with quotes expected. (i.e. \"foo.c\")");
            goto bad_var;
        }

        var->v_fname = strdup(ep->e_string);

        DPRINT("ReadVar_Blockmap: filename is %s\n", ep->e_string);

        ep = POP(e);

        DPRINT("ReadVar_Blockmap: popped expr_op %s\n", getExprOpStr(ep->e_op));
    }

    while (ep->e_op == EXPR_OPSUBSCR || ep->e_op == EXPR_LIST) {
        DPRINT("ReadVar_Blockmap: In EXPR_OPSUBSCR loop\n");

        ep = TOP(e);
        DPRINT("ReadVar_Blockmap: ep points to %s\n", getExprOpStr(ep->e_op));

        // if we are pointing to the start of the list, we want to pop the token
        // and its length. We can parse recursively without knowing the length.
        if (ep->e_op == EXPR_LIST_START) {
            DPRINT(
                "ReadVar_Blockmap: At start of multi-dimensional array "
                "list.\n");
            // Pop the list start token
            ep = POP(e);
            // Pop the length
            ep = POP(e);
            // Pop the first elements list token
            ep = POP(e);
            // Point at the start of the next token
            ep = TOP(e);
            DPRINT("ReadVar_Blockmap: ep points to %s\n",
                   getExprOpStr(ep->e_op));
        }

        if (!var->v_slice) var->v_slice = new slice_t;

        if (ep->e_op == EXPR_SLICE) {
            (void)POP(e);  // pop the EXPR_SLICE off

            smx = -1;
            smn = -1;

            if ((val = evaluate_expr_imm(e, rec_set, (func_t *)NULL)) ==
                (val_t *)NULL)
                goto bad_var;

            if (!VAL_IS_LOCAL_CONST(val)) {
                SetError("Expression evaluated to bad type");
                DestroyVal(val);
                goto bad_var;
            }

            if (RVAL_AIF(val)->toInt(smx) < 0) {
                SetError("%s", AIFErrorStr());
                DestroyVal(val);
                goto bad_var;
            }

            DestroyVal(val);

            if ((val = evaluate_expr_imm(e, rec_set, (func_t *)NULL)) ==
                (val_t *)NULL)
                goto bad_var;

            if (!VAL_IS_LOCAL_CONST(val)) {
                SetError("Expression evaluated to bad type");
                DestroyVal(val);
                goto bad_var;
            }

            if (RVAL_AIF(val)->toInt(smn) < 0) {
                SetError("%s", AIFErrorStr());
                DestroyVal(val);
                goto bad_var;
            }

            DestroyVal(val);

            if (smn < 0 || smx < 0) {
                SetError("Negative indices not allowed");
                goto bad_var;
            }

            var->v_slice->addSlice(smn, smx);

        } else {
            if ((val = evaluate_expr_imm(e, rec_set, (func_t *)NULL)) ==
                (val_t *)NULL)
                goto bad_var;

            if (!VAL_IS_LOCAL_CONST(val)) {
                SetError("Expression evaluated to bad type");
                DestroyVal(val);
                goto bad_var;
            }

            if (RVAL_AIF(val)->toInt(smx) < 0) {
                SetError("%s", AIFErrorStr());
                DestroyVal(val);
                goto bad_var;
            }

            DestroyVal(val);

            if (smx < 0) {
                SetError("Negative indices not allowed");
                goto bad_var;
            }

            smn = smx;

            var->v_slice->addSlice(smn, smx);
        }

        ep = POP(e);
    }

    if (var->v_slice) var->v_slice->reverse();

    switch (ep->e_op) {
        case EXPR_SYM:
            var->v_var = strdup(ep->e_sym_str);
            DPRINT("ReadVarDecl: sym is \"%s\"\n", var->v_var);
            break;

        case EXPR_DSYM:
            var->v_var = strdup(SymLookup(ep->e_sym));
            var->v_internal = 1;
            DPRINT("ReadVarDecl: dsym is \"%s\"\n", var->v_var);
            break;

        case EXPR_STRING:
            var->v_var = strdup(ep->e_string);
            DPRINT("ReadVarDecl: string is \"%s\"\n", var->v_var);
            break;

        case EXPR_OPDREF:
        case EXPR_OPADDROF:
        case EXPR_OPELEMENTREF:
        case EXPR_OPELEMENTPTR:
        case EXPR_OPELEMENTACC:
        case EXPR_OPCAST:
            var->v_var = apply_var_op(ep, e);
            DPRINT("ReadVarDecl: var operation is %s\n", var->v_var);
            break;

        case EXPR_OPSUB:
            ep = POP(e);
            DPRINT("ReadVarDecl: popped expr_op %s\n", getExprOpStr(ep->e_op));
            if (ep->e_op != EXPR_SYM) {
                SetError("Symbol expected while evaluating variable");
                goto bad_var;
            }
            var->v_var = strdup(ep->e_sym_str);
            DPRINT("ReadVarDecl: sym is \"%s\"\n", var->v_var);

            strcat(var->v_var, " - ");  // just subtraction operation for now

            ep = POP(e);
            DPRINT("ReadVarDecl: popped expr_op %s\n", getExprOpStr(ep->e_op));
            if (ep->e_op != EXPR_SYM) {
                SetError("Symbol expected while evaluating variable");
                goto bad_var;
            }
            strcat(var->v_var, ep->e_sym_str);
            DPRINT("ReadVarDecl: sym is \"%s\"\n", var->v_var);
            break;

        default:
            SetError("Symbol expected while evaluating variable");
            goto bad_var;
    }

    ep = TOP(e);

    if ((val = evaluate_expr_imm(e, rec_set, (func_t *)NULL)) == (val_t *)NULL)
        goto bad_var;

    if (RVAL_ACTUAL_TYPE(val) != rval_proc) {
        SetError("No such process \"%s\"", LVAL_GDB_EXPR(val));
        goto bad_var;
    }

    // assign the procset directly
    if ((var->v_procset = RVAL_ACTUAL_PROCSET(val)) == NULL) {
        SetError("Invalid process set");
        goto bad_var;
    }

    // FIXME: This should support a range now that it is using a slice_t.
    // The var should be changed to hold a slice_t structure representing
    // the subset info. Give it a better name too.
    if (RVAL_TYPE(val) == rval_ref) {
        PrintOutError(
            "Warning: Range use detected in procset for command. Only "
            "operating on rank %d.\n",
            RVAL_REF_SLICE(val)->getLow(0));
        var->v_indiv = RVAL_REF_SLICE(val)->getLow(0);
    }

    return var;

bad_var:
    if (var != (var_t *)NULL) DestroyVar(var);

    return (var_t *)NULL;
}

g_block_map_t *EvaluateBlockMap(expr_t *e, rec_set_t *rec_set, CMD_MODE mode) {
    var_t *v;
    g_block_map_t *m;
    expr_elt *ep;

    ep = POP(e);

    if ((m = FindBlockMap(SymLookup(ep->e_sym))) == (g_block_map_t *)NULL) {
        SetError("Block Map \"%s\" not defined", SymLookup(ep->e_sym));
        return (g_block_map_t *)NULL;
    }

    ep = POP(e);

    /*
    ** We first expect to find a list start token.
    */
    if (ep->e_op != EXPR_LIST_START) {
        SetError("Internal error: expected EXPR_LIST_START");
        return (g_block_map_t *)NULL;
    }

    ep = POP(e);

    /*
    ** Evaluate the argument for the map. Maps are only allowed
    ** one argument.
    */

    if (ep->e_op != EXPR_INT) {
        SetError("Internal error: expected EXPR_INT");
        return (g_block_map_t *)NULL;
    }

    if (ep->e_int > 1) {
        SetError("Too many arguments for block map");
        return (g_block_map_t *)NULL;
    }

    ep = POP(e);

    if (ep->e_op != EXPR_LIST) {
        SetError("Internal error: expected EXPR_LIST");
        return (g_block_map_t *)NULL;
    }

    ep = POP(e);

    if (ep->e_op != EXPR_VAR) {
        SetError("Internal error: expected EXPR_VAR");
        return (g_block_map_t *)NULL;
    }

    /*
    ** Evaluate the supplied argument.
    */

    if ((v = ReadVar_BlockMap(e, rec_set, mode)) == (var_t *)NULL)
        return (g_block_map_t *)NULL;

    m->m_actual = v;

    return m;
}

// Create a procset value from the expression stack e.   The epression stack should have
// the thread info as a list that contains integers, integer ranges, strings (for regexps), and
// possibly the special '*' for all.   These will be on top of the usual proc set definition.
static val_t* ReadThreadProcSet(expr_t* e, rec_set_t* rec_set)
{
    // We expect a list start for the thread list
    expr_elt *ep, *ep2;
    if (!check_pop(e, EXPR_LIST_START)) return nullptr;

    // then the length of the thread list
    int threadLen = 0;
    if ((ep = check_pop(e, EXPR_INT))) {
        threadLen = ep->e_int;
    } else {
        return nullptr;
    }

    // process the thread list
    std::vector<std::string> threadElems;

    for (int i=0; i<threadLen; ++i) {
        if (!check_pop(e, EXPR_LIST)) return nullptr;

        if (!(ep = check_pop(e, EXPR_UNKNOWN))) return nullptr;

        switch (ep->e_op) {
        case EXPR_INT:
            threadElems.push_back(std::to_string(ep->e_int));
            break;
        case EXPR_STRING:
            threadElems.push_back(std::string{"\""} + ep->e_string + "\"");
            break;
        case EXPR_SLICE:
            if ((ep=check_pop(e, EXPR_INT)) &&
                (ep2=check_pop(e,EXPR_INT))) {
                threadElems.push_back( std::to_string(ep2->e_int) + ".." + std::to_string(ep->e_int));
            } else {
                SetError("unexpected type in thread range");
                return nullptr;
            }
            break;
        case EXPR_STAR:
            threadElems.push_back(std::string{"*"});
            break;
        default:
            SetError("unexpected type in thread list %s", getExprOpStr(ep->e_op));
            return nullptr;
        }
    }

    // the expression parsing happens back to front, to I need to reverse the list elements
    std::string threadDef;
    for (auto it = threadElems.rbegin(); it != threadElems.rend(); ++it) {
        if (!threadDef.empty())
            threadDef += ",";
        threadDef += *it;
    }
    

    val_t* procVal = read_var_imm(e, rec_set, nullptr);

    if (procVal) {
        slice_t* slice = RVAL_REF_SLICE(procVal);
        slice->setThreads(threadDef.c_str());
    }

    return procVal;
}


/*
** Evaluate a variable in declarative mode. Return details in var_t.
*/
var_t *ReadVarDecl(expr_t *e, rec_set_t *rec_set) {
    int smn;
    int smx;
    val_t *val;
    var_t *var = (var_t *)NULL;
    expr_elt *ep;

    DPRINT("In ReadVarDecl.\n");

    var = NewVar();

    ep = POP(e);

    DPRINT("ReadVarDecl: popped expr_op %s\n", getExprOpStr(ep->e_op));

    if (ep->e_op != EXPR_LOC) {
        SetError(
            "Must specify location information in declarative mode. (i.e. "
            "myVar@\"foo.c\":10)");
        goto bad_var;
    }

    ep = POP(e);

    DPRINT("ReadVarDecl: popped expr_op %s\n", getExprOpStr(ep->e_op));

    if (ep->e_op != EXPR_INT) {
        SetError("Line number expected. (i.e. \"foo.c\":10)");
        goto bad_var;
    }

    var->v_lno = ep->e_int;

    DPRINT("ReadVarDecl: line number is %d\n", ep->e_int);

    ep = POP(e);

    DPRINT("ReadVarDecl: popped expr_op %s\n", getExprOpStr(ep->e_op));

    if (ep->e_op != EXPR_STRING) {
        SetError("Filename enclosed with quotes expected. (i.e. \"foo.c\")");
        goto bad_var;
    }

    var->v_fname = strdup(ep->e_string);

    DPRINT("ReadVarDecl: filename is %s\n", ep->e_string);

    ep = POP(e);

    DPRINT("ReadVarDecl: popped expr_op %s\n", getExprOpStr(ep->e_op));

    while (ep->e_op == EXPR_OPSUBSCR || ep->e_op == EXPR_LIST) {
        DPRINT("ReadVarDecl: In EXPR_OPSUBSCR loop\n");

        ep = TOP(e);
        DPRINT("ReadVarDecl: ep points to %s\n", getExprOpStr(ep->e_op));

        // if we are pointing to the start of the list, we want to pop the token
        // and its length. We can parse recursively without knowing the length.
        if (ep->e_op == EXPR_LIST_START) {
            DPRINT("ReadVarDecl: At start of multi-dimensional array list.\n");
            // Pop the list start token
            ep = POP(e);
            // Pop the length
            ep = POP(e);
            // Pop the first elements list token
            ep = POP(e);
            // Point at the start of the next token
            ep = TOP(e);
            DPRINT("ReadVarDecl: ep points to %s\n", getExprOpStr(ep->e_op));
        }

        if (!var->v_slice) var->v_slice = new slice_t;

        if (ep->e_op == EXPR_SLICE) {
            (void)POP(e);  // pop the EXPR_SLICE off

            smx = -1;
            smn = -1;

            if ((val = evaluate_expr_imm(e, rec_set, (func_t *)NULL)) ==
                (val_t *)NULL)
                goto bad_var;

            if (!VAL_IS_LOCAL_CONST(val)) {
                SetError("Expression evaluated to bad type");
                DestroyVal(val);
                goto bad_var;
            }

            if (RVAL_AIF(val)->toInt(smx) < 0) {
                SetError("%s", AIFErrorStr());
                DestroyVal(val);
                goto bad_var;
            }

            DestroyVal(val);

            if ((val = evaluate_expr_imm(e, rec_set, (func_t *)NULL)) ==
                (val_t *)NULL)
                goto bad_var;

            if (!VAL_IS_LOCAL_CONST(val)) {
                SetError("Expression evaluated to bad type");
                DestroyVal(val);
                goto bad_var;
            }

            if (RVAL_AIF(val)->toInt(smn) < 0) {
                SetError("%s", AIFErrorStr());
                DestroyVal(val);
                goto bad_var;
            }

            DestroyVal(val);

            if (smn < 0 || smx < 0) {
                SetError("Negative indices not allowed");
                goto bad_var;
            }

            var->v_slice->addSlice(smn, smx);

        } else {
            if ((val = evaluate_expr_imm(e, rec_set, (func_t *)NULL)) ==
                (val_t *)NULL)
                goto bad_var;

            if (!VAL_IS_LOCAL_CONST(val)) {
                SetError("Expression evaluated to bad type");
                DestroyVal(val);
                goto bad_var;
            }

            if (RVAL_AIF(val)->toInt(smx) < 0) {
                SetError("%s", AIFErrorStr());
                DestroyVal(val);
                goto bad_var;
            }

            DestroyVal(val);

            if (smx < 0) {
                SetError("Negative indices not allowed");
                goto bad_var;
            }

            smn = smx;

            var->v_slice->addSlice(smn, smx);
        }

        ep = POP(e);
    }

    if (var->v_slice) var->v_slice->reverse();

    switch (ep->e_op) {
        case EXPR_SYM:
            var->v_var = strdup(ep->e_sym_str);
            DPRINT("ReadVarDecl: sym is \"%s\"\n", var->v_var);
            break;

        case EXPR_DSYM:
            var->v_var = strdup(SymLookup(ep->e_sym));
            var->v_internal = 1;
            DPRINT("ReadVarDecl: dsym is \"%s\"\n", var->v_var);
            break;

        case EXPR_STRING:
            var->v_var = strdup(ep->e_string);
            DPRINT("ReadVarDecl: string is \"%s\"\n", var->v_var);
            break;

        case EXPR_OPDREF:
        case EXPR_OPADDROF:
        case EXPR_OPELEMENTREF:
        case EXPR_OPELEMENTPTR:
        case EXPR_OPELEMENTACC:
        case EXPR_OPCAST:
            var->v_var = apply_var_op(ep, e);
            DPRINT("ReadVarDecl: var operation is %s\n", var->v_var);
            break;

        case EXPR_OPSUB:
            ep = POP(e);
            DPRINT("ReadVarDecl: popped expr_op %s\n", getExprOpStr(ep->e_op));
            if (ep->e_op != EXPR_SYM) {
                SetError("Symbol expected while evaluating variable");
                goto bad_var;
            }
            var->v_var = strdup(ep->e_sym_str);
            DPRINT("ReadVarDecl: sym is \"%s\"\n", var->v_var);

            strcat(var->v_var, " - ");  // just subtraction operation for now

            ep = POP(e);
            DPRINT("ReadVarDecl: popped expr_op %s\n", getExprOpStr(ep->e_op));
            if (ep->e_op != EXPR_SYM) {
                SetError("Symbol expected while evaluating variable");
                goto bad_var;
            }
            strcat(var->v_var, ep->e_sym_str);
            DPRINT("ReadVarDecl: sym is \"%s\"\n", var->v_var);
            break;

        default:
            SetError("Symbol expected while evaluating variable");
            goto bad_var;
    }

    ep = TOP(e);

    if ((val = evaluate_expr_imm(e, rec_set, (func_t *)NULL)) == (val_t *)NULL)
        goto bad_var;

    if (RVAL_ACTUAL_TYPE(val) != rval_proc) {
        SetError("No such process \"%s\"", SymLookup(LVAL_SYM(val)));
        goto bad_var;
    }

    // assign the procset directly
    if ((var->v_procset = RVAL_ACTUAL_PROCSET(val)) == NULL) {
        SetError("Invalid process set");
        goto bad_var;
    }

    // FIXME: This should support a range now that we keep the slice_t info.
    // The var should be changed to hold a slice_t structure representing
    // the subset info. Give it a better name too.
    if (RVAL_TYPE(val) == rval_ref) {
        // Only output a warning if the slice provided is more than one rank in size
        const auto* slice = RVAL_REF_SLICE(val);
        const auto* procset = RVAL_ACTUAL_PROCSET(val);
        if (slice->getRank() > 1 || (slice->getLow(0) != slice->getHigh(0))) {
            PrintOutError(
                "Warning: Range use detected in procset for command ($%s{%d..%d}). "
                "Only operating on rank %d.\n",
                procset->s_name, slice->getLow(0), slice->getHigh(0),
                slice->getLow(0));
        }

        var->v_indiv = slice->getLow(0);
        
        // check that v_indiv is actually in the procset
        bool found = false;

        SetList(procset->s_over_set_list);
        while (auto os = (dbg_over_set_t *)GetListElement(procset->s_over_set_list)) {
            if (os->s_di_bs->test(os->s_ps_offset + var->v_indiv)) {
                found = true;
                break;
            }
        }

        if (!found) {
            SetError(
                "Error: rank %d not in procset $%s.",
                var->v_indiv, procset->s_name
            );
            goto bad_var;
        }
    } else {
        // XXX: When ReadVarDecl is used for parsing assert commands, the
        //      particulars on how the procset is specified are important.
        //      -   valid: $proc{0}::var...
        //      -   valid: $decomp{$proc::var...}
        //      - invalid: $proc::var...
        //      The invalid form causes graph build errors because COMPARE nodes
        //      only accept TK_DATA as input, but the invalid form generates
        //      nodes that output TK_DATA_SET.
        const auto* procset = RVAL_ACTUAL_PROCSET(val);
        SetError(
            "Error: Invalid procset ($%s::). Specify a single rank "
            "(e.g. $%s{0}::var...) or use a decomposition (e.g. $dec{$%s::var...}).",
            procset->s_name, procset->s_name, procset->s_name
        );
        goto bad_var;
    }

    return var;

bad_var:
    if (var != (var_t *)NULL) DestroyVar(var);

    return (var_t *)NULL;
}

/*
** Assign expression (e2) to variable (e1).
*/
int AssignExpr(expr_t *e1, expr_t *e2, rec_set_t *rec_set) {
    int ret;
    val_t *lval;
    val_t *rval;

    if (e1->e_lval == 0 || e2->e_lval == 0) {
        SetError("Nothing to do!");
        return -1;
    }

    RESET(e1);
    RESET(e2);

    if ((lval = evaluate_expr_imm(e1, rec_set, (func_t *)NULL)) ==
        (val_t *)NULL)
        return -1;

    if ((rval = evaluate_expr_imm(e2, rec_set, (func_t *)NULL)) ==
        (val_t *)NULL)
        return -1;

    ret = assign_expr_imm(lval, rval, rec_set);

    DestroyVal(lval);
    DestroyVal(rval);

    return ret;
}

// Used by assign_expr_imm to send the actual command
static int SendSetVarCmd(rec_set_t *rec_set, char *var, AIFStream *val) {
    rep_set_t *rep_set;

    if ((rep_set = DbgSetVar(rec_set, var, val)) != NULL) {
        ProcessErrorInRepSet(rep_set);
        FreeRepSet(rep_set);
        return -1;
    }

    return 0;
}

/*
** Assign the result of evaluating e to variable lval. Action depends on
** type of lval. For internal variables, the data resulting from e replaces
** the contents. For program variables, the data resulting from e is written
** to the program.
** FIXME: Currently the gdbmi server will take the AIF data and convert it to
** string and pass that into gdb. This is dangerous and doesn't always work if
** the AIF string convention doesn't exactly match gdb input. Change this to
** add a SetVar command that works on a string expression for setting variable
** names directly inside the rank, and another SetVar command that operates by
** directly writing binary data into the inferior.
*/
static int assign_expr_imm(val_t *lval, val_t *rval, rec_set_t *rec_set) {
    AIFStream *na;
    rec_set_t *lval_rec_set = NULL;
    rep_set_t *rep_set;
    group_reply_t *reply;
    dbg_over_set_t *over_set;
    int ret;
    rec_set_t *target_recset;

    DPRINT("In assign_expr_imm.\n");

    switch (LVAL_TYPE(lval)) {
        case lval_blockmap:
            SetError(
                "lval_blockmap is not supposed to be processed within "
                "assign_expr_imm()!\n");
            return -1;

        case lval_internal:
            if (RVAL_TYPE(lval) == rval_proc) {
                SetError("Cannot assign process set to debug variable.");
                return -1;
            }

            if (AssignVal(lval, rval, rec_set) < 0) {
                return -1;
            }

            return 0;

        case lval_gdb:

            // Make sure the lval gdb variable actually exists by asking for its
            // type. We need to use the LVAL_GDB_PROCSET if there is one.
            if (LVAL_GDB_PROCSET(lval) != NULL) {
                // Ensure that there is only a single di in the procset,
                // othwerwise this will fail
                // TODO: We might be able to work around this by iterating over
                // each di in the procset, but I am not implementing that right
                // now.
                if (LVAL_GDB_PROCSET(lval)->s_num_sets != 1) {
                    // too many di in procset
                    SetError(
                        "Cannot process procsets with multiple launch/attach "
                        "handles!");
                    return -1;
                }

                // create the rec_set
                lval_rec_set = NewRecSet();

                SetOverlaySetForProcSet(LVAL_GDB_PROCSET(lval));
                while ((over_set = GetNextOverlaySetForProcSet(
                            LVAL_GDB_PROCSET(lval))) != NULL) {
                    // create a group_recv_t for the over_set and add it to the
                    // res
                    AddReceiverToSet(
                        lval_rec_set,
                        NewGroupReceiver(over_set->s_di, over_set->s_di_bs.get()));
                }

                // get the type
                rep_set = DbgGetType(lval_rec_set, LVAL_GDB_EXPR(lval), 0);
            } else {
                // need to use the passed in rec_set here
                rep_set = DbgGetType(rec_set, LVAL_GDB_EXPR(lval), 0);
            }

            // process errors
            if (ProcessErrorInRepSet(rep_set)) {
                if (lval_rec_set != NULL) {
                    FreeRecSet(lval_rec_set);
                }
                FreeRepSet(rep_set);
                return -1;
            }

            // ensure there was at least one reply
            if (GetNumberOfReply(rep_set) == 0) {
                SetError("Missing definition for type %s", LVAL_GDB_EXPR(lval));
                if (lval_rec_set != NULL) {
                    FreeRecSet(lval_rec_set);
                }
                FreeRepSet(rep_set);
                return -1;
            }

            // Free the repset
            FreeRepSet(rep_set);

            // If this is a local constant, we can assign this directly without
            // issue, otherwise we need to make sure that there is either
            // exactly one reply in the rep_set for the gdb var, or we need to
            // ensure that the number of ranks in the gdb lval matches the
            // number of ranks in the gdb rval
            if (VAL_IS_LOCAL_CONST(rval)) {
                // Send the command using the rval aif data
                if (lval_rec_set != NULL) {
                    ret = SendSetVarCmd(lval_rec_set, LVAL_GDB_EXPR(lval),
                                        RVAL_AIF(rval));
                    FreeRecSet(lval_rec_set);
                    return ret;
                } else {
                    return SendSetVarCmd(rec_set, LVAL_GDB_EXPR(lval),
                                         RVAL_AIF(rval));
                }
            }

            // If this is a local rep_set, we can assign directly. We should
            // already be holding onto the rep_set in the val. This is only
            // allowed for previous assigns that went into a debugger variable
            // like $tmp that resulted in a reply type.
            if (VAL_IS_LOCAL_REPLY(rval)) {
                // Grab the result of evaluating the rval gdb expression
                // DO NOT FREE THIS!!! This will get free'ed up when the val
                // gets destroyed.
                rep_set = RVAL_REPLY(rval);

                // check for error
                if (rep_set == NULL) {
                    SetError("Requested rval contained a null reply set!");
                    if (lval_rec_set != NULL) {
                        FreeRecSet(lval_rec_set);
                    }
                    return -1;
                }


                // Check to see if there is only one reply in the rep_set, if so
                // we can assign the value directly since it is a constant
                // across every rank
                if (GetNumberOfReply(rep_set) == 1) {
                    // grab the result
                    reply = rep_set->getFirstReply();
                    // point at the AIF data
                    na = EVENT_DATA(reply->rep_event);

                    // Send the command using the event data
                    if (lval_rec_set != NULL) {
                        ret = SendSetVarCmd(lval_rec_set, LVAL_GDB_EXPR(lval),
                                            na);
                        FreeRecSet(lval_rec_set);
                        return ret;
                    } else {
                        ret = SendSetVarCmd(rec_set, LVAL_GDB_EXPR(lval), na);
                        return ret;
                    }
                }

                // More than one reply, we can still handle this as long as
                // the number of ranks in the lval gdb val match the number of
                // ranks in the rval reply.

                // temporarily point at the first reply, make sure to reset the
                // list when finished. Need to check one of the bitsets in the
                // reply set to see if its the same size of the lval overset.
                reply = rep_set->getFirstReply();

                if (lval_rec_set != NULL) {
                    // We already enforced the one di rule with the lval, so
                    // lets just operate on its procset directly.
                    SetOverlaySetForProcSet(LVAL_GDB_PROCSET(lval));
                    over_set =
                        GetNextOverlaySetForProcSet(LVAL_GDB_PROCSET(lval));

                    // Check if the size of the lval overset bitset matches the
                    // size of the rep_set bitset
                    if (over_set->s_di_bs->sizeInBits() !=
                        reply->rep_group->sizeInBits()) {
                        // The sizes do not match, we cannot complete the
                        // request
                        SetError(
                            "Rank mismatch between requested assignment of "
                            "application variable and reply.");
                        FreeRecSet(lval_rec_set);
                        return -1;
                    }

                    // reset ret
                    ret = 0;

                    // We can assign the rval to the lval since the number of
                    // ranks match loop over each reply
                    for (auto&& reply : rep_set) {
                        // Create the rec_set for this reply from the lval
                        // overset using the bitset from the reply
                        target_recset = NewRecSet();
                        AddReceiverToSet(
                            target_recset,
                            NewGroupReceiver(over_set->s_di, reply->rep_group.get()));
                        // point at the AIF data
                        na = EVENT_DATA(reply->rep_event);
                        // Send the command using the target_recset and the
                        // event data Only set ret if this returns non-zero
                        if (SendSetVarCmd(target_recset, LVAL_GDB_EXPR(lval),
                                          na)) {
                            // error occured
                            ret = -1;
                            SetError(
                                "Warning: Assign partial failure, some data "
                                "may have been assigned to the ranks.");
                        }
                        // cleanup recset
                        FreeRecSet(target_recset);
                    }

                    // cleanup
                    FreeRecSet(lval_rec_set);

                    return ret;

                } else {
                    // Here the user provided something like assign ii $tmp
                    // where $tmp corresponds to a previous assignment of a
                    // debugger variable. The lval_rec_set is NULL.

                    // reset ret
                    ret = 0;

                    // loop over each reply
                    for (auto&& reply : rep_set) {
                        // Loop over each di in the rec_set
                        for (auto&& recv_ptr : rec_set) {
                            // Check if the size of the di bitset matches the
                            // size of the rep_set bitset
                            if (recv_ptr->rec_group->sizeInBits() !=
                                reply->rep_group->sizeInBits()) {
                                // The sizes do not match, we cannot complete
                                // the request
                                SetError(
                                    "Rank mismatch between requested "
                                    "assignment of application variable and "
                                    "reply.");
                                return -1;
                            }

                            // Create the rec_set for this reply from the
                            // rec_set and using the bitset from the reply
                            target_recset = NewRecSet();
                            AddReceiverToSet(
                                target_recset,
                                NewGroupReceiver(recv_ptr->rec_di,
                                                 reply->rep_group.get()));
                            // point at the AIF data
                            na = EVENT_DATA(reply->rep_event);
                            // Send the command using the target_recset and the
                            // event data Only set ret if this returns non-zero
                            if (SendSetVarCmd(target_recset,
                                              LVAL_GDB_EXPR(lval), na)) {
                                // error occured
                                ret = -1;
                                SetError(
                                    "Warning: Assign partial failure, some "
                                    "data may have been assigned to the "
                                    "ranks.");
                            }
                            // cleanup recset
                            FreeRecSet(target_recset);
                        }
                    }

                    return ret;
                }
            }

            // Make sure the rval is a gdb variable

            // TODO: Maybe allow for blockmap to be used here at some point in
            // the future. This will require a lot of work, but might be
            // dooable. Need to figure out which blocks go where and send them
            // off. Probably best implemented with P2P

            if (LVAL_TYPE(rval) != lval_gdb) {
                SetError(
                    "Cannot assign requested type to application variable.");
                if (lval_rec_set != NULL) {
                    FreeRecSet(lval_rec_set);
                }
                return -1;
            }

            // Check to see if the lval and rval doesn't have procset
            // information
            if (lval_rec_set == NULL && LVAL_GDB_PROCSET(rval) == NULL) {
                // We can assign the gdb_name of the rval directly inside
                // each rank. The caller requested something like assign ii jj
                // where the default focus is used.
                na = AIFStream::fromString(LVAL_GDB_EXPR(rval));
                // Send the command using the created AIF string
                ret = SendSetVarCmd(rec_set, LVAL_GDB_EXPR(lval), na);
                // Make sure to free the AIF string before returning
                delete na;
                return ret;
            }

            // Fetch the contents of the rval, note that this might use the rval
            // procset if there is one.
            if (VAL_RETRIEVE(rval, rec_set)) {
                if (lval_rec_set != NULL) {
                    FreeRecSet(lval_rec_set);
                }
                return -1;
            }

            // Grab the result of evaluating the rval gdb expression
            // DO NOT FREE THIS!!! This will get free'ed up when the val gets
            // destroyed.
            rep_set = RVAL_REPLY(rval);

            // check for error
            if (rep_set == NULL) {
                if (lval_rec_set != NULL) {
                    FreeRecSet(lval_rec_set);
                }
                return -1;
            }

            // Check to see if there is only one reply in the rep_set, if so
            // we can assign the value directly since it is a constant across
            // every rank
            if (GetNumberOfReply(rep_set) == 1) {
                // grab the result
                reply = rep_set->getFirstReply();
                // point at the AIF data
                na = EVENT_DATA(reply->rep_event);

                // Send the command using the event data
                if (lval_rec_set != NULL) {
                    ret = SendSetVarCmd(lval_rec_set, LVAL_GDB_EXPR(lval), na);
                    FreeRecSet(lval_rec_set);
                    return ret;
                } else {
                    ret = SendSetVarCmd(rec_set, LVAL_GDB_EXPR(lval), na);
                    return ret;
                }
            }

            // More than one reply, we can still handle this as long as
            // the number of ranks in the lval gdb val match the number of
            // ranks in the rval gdb val.

            // If there was an rval procset, we need to enforce the single di
            // rule.
            if (LVAL_GDB_PROCSET(rval) != NULL) {
                // Ensure that there is only a single di in the procset,
                // othwerwise this will fail
                // TODO: We might be able to work around this by iterating over
                // each di in the procset, but I am not implementing that right
                // now.
                if (LVAL_GDB_PROCSET(rval)->s_num_sets != 1) {
                    // too many di in procset
                    SetError(
                        "Cannot process procsets with multiple launch/attach "
                        "handles!");
                    if (lval_rec_set != NULL) {
                        FreeRecSet(lval_rec_set);
                    }
                    return -1;
                }
            }

            // Need to check one of the bitsets in the reply set
            // to see if its the same size of the lval overset.
            reply = rep_set->getFirstReply();

            if (lval_rec_set != NULL) {
                // We already enforced the one di rule with the lval, so lets
                // just operate on its procset directly.
                SetOverlaySetForProcSet(LVAL_GDB_PROCSET(lval));
                over_set = GetNextOverlaySetForProcSet(LVAL_GDB_PROCSET(lval));

                // Check if the size of the lval overset bitset matches the size
                // of the rep_set bitset
                if (over_set->s_di_bs->sizeInBits() !=
                    reply->rep_group->sizeInBits()) {
                    // The sizes do not match, we cannot complete the request
                    SetError(
                        "Rank mismatch between requested assignment. The "
                        "application variable has multiple values.");
                    FreeRecSet(lval_rec_set);
                    return -1;
                }

                // reset ret
                ret = 0;

                // We can assign the rval to the lval since the number of ranks
                // match loop over each reply
                for (auto&& reply : rep_set) {
                    // Create the rec_set for this reply from the lval overset
                    // using the bitset from the reply
                    target_recset = NewRecSet();
                    AddReceiverToSet(
                        target_recset,
                        NewGroupReceiver(over_set->s_di, reply->rep_group.get()));
                    // point at the AIF data
                    na = EVENT_DATA(reply->rep_event);
                    // Send the command using the target_recset and the event
                    // data Only set ret if this returns non-zero
                    if (SendSetVarCmd(target_recset, LVAL_GDB_EXPR(lval), na)) {
                        // error occured
                        ret = -1;
                        SetError(
                            "Warning: Assign partial failure, some data may "
                            "have been assigned to the ranks.");
                    }
                    // cleanup recset
                    FreeRecSet(target_recset);
                }

                // cleanup
                FreeRecSet(lval_rec_set);

                return ret;

            } else {
                // Here the user provided something like assign ii $a{..}::jj
                // We already enforced the one di rule on the rval procset if
                // there is one. The lval_rec_set is NULL.

                // reset ret
                ret = 0;

                // loop over each reply
                for (auto&& reply : rep_set) {
                    // Loop over each di in the rec_set
                    for (auto&& recv_ptr : rec_set) {
                        // Check if the size of the di bitset matches the size
                        // of the rep_set bitset
                        if (recv_ptr->rec_group->sizeInBits() !=
                            reply->rep_group->sizeInBits()) {
                            // The sizes do not match, we cannot complete the
                            // request
                            SetError(
                                "Rank mismatch between requested assignment. "
                                "The application variable has multiple "
                                "values.");
                            return -1;
                        }

                        // Create the rec_set for this reply from the rec_set
                        // and using the bitset from the reply
                        target_recset = NewRecSet();
                        AddReceiverToSet(target_recset,
                                         NewGroupReceiver(recv_ptr->rec_di,
                                                          reply->rep_group.get()));
                        // point at the AIF data
                        na = EVENT_DATA(reply->rep_event);
                        // Send the command using the target_recset and the
                        // event data Only set ret if this returns non-zero
                        if (SendSetVarCmd(target_recset, LVAL_GDB_EXPR(lval),
                                          na)) {
                            // error occured
                            ret = -1;
                            SetError(
                                "Warning: Assign partial failure, some data "
                                "may have been assigned to the ranks.");
                        }
                        // cleanup recset
                        FreeRecSet(target_recset);
                    }
                }

                return ret;
            }

            // Should never get here
            SetError("assign_expr_imm: reached end of control statement!");
            break;

        case lval_none:
            SetError("Left operand of assignment is not an lvalue");
            break;
    }

    return -1;
}

const char *getExprOpStr(expr_op op) {
    switch (op) {
        case EXPR_OPADD:
            return "<add>";
        case EXPR_OPSUB:
            return "<sub>";
        case EXPR_OPMUL:
            return "<mul>";
        case EXPR_OPDIV:
            return "<div>";
        // TODO: Broken to fix derived fortran types
        case EXPR_OPREM:
            return "<rem>";
        case EXPR_OPPOS:
            return "<pos>";
        case EXPR_OPNEG:
            return "<neg>";
        case EXPR_OPAND:
            return "<and>";
        case EXPR_OPOR:
            return "<or>";
        case EXPR_OPEQUAL:
            return "<eq>";
        case EXPR_OPNOTEQUAL:
            return "<neq>";
        case EXPR_OPLEQ:
            return "<leq>";
        case EXPR_OPGEQ:
            return "<geq>";
        case EXPR_OPLT:
            return "<lt>";
        case EXPR_OPGT:
            return "<gt>";
        case EXPR_OPCOND:
            return "<cond>";
        case EXPR_OPSUBSCR:
            return "<subscr>";
        case EXPR_OPCOMMA:
            return "<comma>";
        case EXPR_OPDREF:
            return "<deref>";
        case EXPR_OPADDROF:
            return "<addr-of>";
        case EXPR_OPELEMENTREF:
            return "<(.) element>";
        case EXPR_OPELEMENTPTR:
            return "<(->) element>";
        case EXPR_OPELEMENTACC:
            return "<(%) element>";
        case EXPR_OPCAST:
            return "<cast>";
        case EXPR_TYPE:
            return "<type>";
        case EXPR_TYPE_SPEC:
            return "<type_specifier>";
        case EXPR_TYPE_IDENT:
            return "<type_identifier>";
        case EXPR_TYPE_PTR:
            return "<type_pointer>";
        case EXPR_LOC:
            return "<loc>";
        case EXPR_SLICE:
            return "<slice>";
        case EXPR_PROC:
            return "<proc>";
        case EXPR_VAR:
            return "<var>";
        case EXPR_GDBVAR:
            return "<gdb_var>";
        case EXPR_INT:
            return "<int>";
        case EXPR_FLOAT:
            return "<float>";
        case EXPR_DSYM:
            return "<dsym>";
        case EXPR_SYM:
            return "<sym>";
        case EXPR_STRING:
            return "<string>";
        case EXPR_STAR:
            return "<star>";
        case EXPR_FUNC:
            return "<func>";
        case EXPR_LIST_START:
            return "<list>";
        case EXPR_LIST:
            return "<list element>";
        case EXPR_BLOCKMAP:
            return "<blockmap>";
        case EXPR_ASSERT:
            return "<assert>";
       case EXPR_PROC_THREAD:
           return "<proc_thread>";
        case EXPR_REPEAT:
            return "<repeat>";
        default:
            return "<UNKNOWN>";
    }

    // should never get here.
    return "<ERROR>";
}

// Only used when debug is turned on; export the symbol for calling from gdb
void DumpExpr(expr_t *e) {
    int i;
    expr_elt *ep;

    PrintOutMsg("Parsed expression is: ");

    for (i = e->e_lval - 1; i >= 0; i--) {
        ep = &e->e_expr[i];

        PrintOutMsg("%s ", getExprOpStr(ep->e_op));

        // print more information for certain ops
        switch (ep->e_op) {
            case EXPR_FUNC:
                PrintOutMsg("{func_name: %s} ", SymLookup(ep->e_sym));
                break;
            case EXPR_BLOCKMAP:
                PrintOutMsg("{blockmap_name: %s} ", SymLookup(ep->e_sym));
                break;
            case EXPR_DSYM:
                PrintOutMsg("{dsym_name: %s} ", SymLookup(ep->e_sym));
                break;
            case EXPR_SYM:
                PrintOutMsg("{sym_name: %s} ", ep->e_sym_str);
                break;
            case EXPR_TYPE_SPEC:
                PrintOutMsg("{type_spec_name: %s} ", ep->e_sym_str);
                break;
            case EXPR_TYPE_IDENT:
                PrintOutMsg("{type_ident_name: %s} ", ep->e_sym_str);
                break;
            case EXPR_INT:
                PrintOutMsg("{int_value: %d} ", ep->e_int);
                break;
            case EXPR_FLOAT:
                PrintOutMsg("{float_value: %g} ", ep->e_float);
                break;
            case EXPR_STRING:
                PrintOutMsg("{string_value: \"%s\"} ", ep->e_string);
                break;
            default:
                // do nothing
                break;
        }
    }

    PrintOutMsg("\n");
}

// XXX: The following functions are used in expr.y.in.

/*
** Start counting elements of a list
*/
void StartExprList(void) {
    int *cnt;

    cnt = (int *)Alloc(sizeof(int));
    *cnt = 0;

    if (_expr_list == (list_t *)NULL) _expr_list = NewList();

    AddFirst(_expr_list, (void *)cnt);
}

/*
** Increment number of arguments.
*/
void IncExprList(void) {
    int *cnt;

    SetList(_expr_list);

    if ((cnt = (int *)GetListElement(_expr_list)) == (int *)NULL) return;

    (*cnt)++;

    DPRINT("IncExprList: Count is %d.\n", *cnt);
}

/*
** Set number of arguments.
*/
void SetExprList(int n) {
    int *cnt;

    SetList(_expr_list);

    if ((cnt = (int *)GetListElement(_expr_list)) == (int *)NULL) return;

    *cnt = n;

    DPRINT("SetExprList: Count is %d.\n", *cnt);
}

/*
** Return number of arguments to function call.
*/
int EndExprList(void) {
    int n;
    int *cnt;

    if ((cnt = (int *)RemoveFirst(_expr_list)) == (int *)NULL) return -1;

    n = *cnt;
    Free(cnt);

    if (EmptyList(_expr_list)) {
        DestroyList(_expr_list, (void (*)(void *))NULL);
        _expr_list = (list_t *)NULL;
    }

    DPRINT("EndExprList: Final list length is %d.\n", n);

    return n;
}

// pop the expression stack of e and verify that the result is an expected type.
// op==EXPR_UNKNOWN pops without checking the type of the result.
// return nullptr if the stack is empty, or the type of the top element is wrong.
static expr_elt* check_pop(expr_t* e, expr_op op)
{
    if (EMPTY(e)) {
        SetError("Internal error: exected a list element");
        return nullptr;
    } 
        
    expr_elt* ep = POP(e);
    if (op != EXPR_UNKNOWN && ep->e_op != op) {
        SetError("Internal error: expected a %s got %s", getExprOpStr(op),
                 getExprOpStr(ep->e_op));
        return nullptr;
    }

    return ep;
}
