/*******************************************************************************
 * Copyright 2022 Hewlett Packard Enterprise Development LP.
 ******************************************************************************/
#include "expression_handle.h"

#include "error.h"
#include "expr.h"
#include "gram.h"
#include "slice.h"
#include "onscopeexit.h"

#include <stdexcept>

using namespace ExpressionHandleShorthands;
using namespace std::string_literals;

// basic types
template<> int ExprHndl::popExpr<int>();
template<> std::string ExprHndl::popExpr<std::string>();

// structs
template<> CompOpExpr ExprHndl::popExpr<CompOpExpr>();
template<> ProgLocExpr ExprHndl::popExpr<ProgLocExpr>();
template<> ProgVarExpr ExprHndl::popExpr<ProgVarExpr>();
template<> SubscriptExpr ExprHndl::popExpr<SubscriptExpr>();

void ExpressionHandle::printExpr() const {
    // useful in gdb
    fprintf(stderr, "(BOTTOM)\n");
    for (int i = 0; i < m_expr->e_lval; i++) {
        // print type
        fprintf(stderr, "%s ", getExprOpStr(m_expr->e_expr[i].e_op));
        // print value
        switch (m_expr->e_expr[i].e_op) {
        case EXPR_INT: fprintf(stderr, "%d", m_expr->e_expr[i].e_val.e_v_int); break;
        default: break;
        }
        // print arrow
        std::string arrow = i == m_expr->e_eval ? "<-" : "";
        fprintf(stderr, "%s\n", arrow.c_str());
    }
    fprintf(stderr, "(TOP)\n");
}

ExpressionHandle::ExpressionHandle(std::string&& str)
    : m_buf{std::move(str)}
    , m_expr(ParseExpr(m_buf.c_str()), FreeExpr)
{
    if (!m_expr) {
        // ParseExpr uses error.h interface to report errors
        throw ParseError("Parsing of \"" + m_buf + "\" failed: " + GetError());
    }
    resetExprIdx();
}

const expr_elt* ExpressionHandle::popExpr() {
    auto r = &m_expr->e_expr[m_expr->e_eval--];
    if (!r) {
        incExprIdx(); // stack should appear unchanged
        throw ParseError("Out of symbols");
    }
    return r;
}

const expr_elt* ExpressionHandle::topExpr() const {
    return &m_expr->e_expr[m_expr->e_eval];
}

expr_op ExpressionHandle::topExprOp() const {
    return topExpr()->e_op;
}

const expr_elt* ExpressionHandle::lastExpr() const {
    return &m_expr->e_expr[m_expr->e_lval - 1];
}

void ExpressionHandle::incExprIdx() {
    m_expr->e_eval++;
}

void ExpressionHandle::resetExprIdx() {
    m_expr->e_eval = m_expr->e_lval - 1;
}

bool ExpressionHandle::exprIsEmpty() const {
    return m_expr->e_eval < 0;
}

void ExpressionHandle::pushStackCheckpoint() {
    m_stackCheckpoints.push_back(m_expr->e_eval);
}

void ExpressionHandle::restoreStackCheckpoint() {
    if (m_stackCheckpoints.empty()) {
        throw ParseError("Empty checkpoint stack");
    }

    m_expr->e_eval = m_stackCheckpoints.back();
    m_stackCheckpoints.pop_back();
}

void ExpressionHandle::removeStackCheckpoint() {
    if (m_stackCheckpoints.empty()) {
        throw ParseError("Empty checkpoint stack");
    }
    m_stackCheckpoints.pop_back();
}

// popExpr specializations, private to ExpressionHandle ////////////////////////

template<>
int ExprHndl::popExpr<int>() {
    int factor = 1;
    int stackChanged = 0;

    if (topExprOp() == EXPR_OPNEG) {
        popExpr(); // pop <neg>
        stackChanged++;
        factor = -1;
    }

    auto elt = popExpr(); // won't return nullptr
    stackChanged++;

    if (elt->e_op != EXPR_INT) {
        for (int i = 0; i < stackChanged; i++) incExprIdx(); // stack should appear unchanged
        throw ParseError("Tried to parse an int but found a "s + getExprOpStr(elt->e_op));
    }

    return elt->e_val.e_v_int * factor;
}

template<>
std::string ExprHndl::popExpr<std::string>() {
    auto elt = popExpr(); // won't return nullptr

    switch (elt->e_op) {
        case EXPR_STRING:
            return std::string{elt->e_val.e_v_string};
        case EXPR_SYM:
            return std::string{elt->e_val.e_v_sym_str};
        case EXPR_DSYM:
        case EXPR_BLOCKMAP:
            return std::string{elt->e_val.e_v_sym->h_key};
        default:
            incExprIdx(); // stack should appear unchanged
            throw ParseError("Tried to parse a string but found a "s + getExprOpStr(elt->e_op));
    };
}

template<>
CompOpExpr ExprHndl::popExpr<CompOpExpr>() {
    auto elt = popExpr(); // won't return nullptr

    switch (elt->e_op) {
        case EXPR_OPEQUAL:
            return CompOpExpr::Equal;
        case EXPR_OPNOTEQUAL:
            return CompOpExpr::NotEqual;
        case EXPR_OPLT:
            return CompOpExpr::LessThan;
        case EXPR_OPLEQ:
            return CompOpExpr::LessThanEqual;
        case EXPR_OPGT:
            return CompOpExpr::GreaterThan;
        case EXPR_OPGEQ:
            return CompOpExpr::GreaterThanEqual;
        default:
            incExprIdx(); // stack should appear unchanged
            throw ParseError("Tried to parse a comparison op but found a "s + getExprOpStr(elt->e_op));
    }
}

template<>
ProgLocExpr ExprHndl::popExpr<ProgLocExpr>() {
    auto r = ProgLocExpr{};

    pushStackCheckpoint();

    popExpr(); // pop <loc> token

    try {
        r.m_lineNumber = popExpr<int>();
        r.m_name = popExpr<std::string>();
    } catch (const ParseError &e) {
        // fix stack and propagate error
        restoreStackCheckpoint();
        throw e;
    }

    removeStackCheckpoint();
    return r;
}

template<>
SubscriptExpr ExprHndl::popExpr<SubscriptExpr>() {
    // there are a few variations of how subscripts can appear:
    //
    // procset subscripts (e.g. $a{0}) look like this:
    //    <dsym> (e.g. "a")
    //    <int> (index, e.g. 0)
    //    [<neg>]
    //    <list element>
    //    <int> (list length)
    //    <list>
    //    <subscr> (top)
    //
    // ranged procset subscripts (e.g. $a{0..2}) look like this:
    //    <dsym> (e.g. "a")
    //    <int> (first index, e.g. 0)
    //    [<neg>]
    //    <int> (second index, e.g. 2)
    //    [<neg>]
    //    <slice>
    //    <list element>
    //    <int> (list length)
    //    <list>
    //    <subscr> (top)
    //
    // array subscripts (e.g. myArray[7]) look like this:
    //    <sym> (e.g. "myArray")
    //    <int> (the index, e.g. 7)
    //    [<neg>]
    //    <subscr> (top)
    //
    // array subscripts with slices (e.g. myArray[0..9]) look like this:
    //    <sym> (e.g. "myArray")
    //    <int> (first index, e.g. 0)
    //    [<neg>]
    //    <int> (second index, e.g. 9)
    //    [<neg>]
    //    <slice>
    //    <subscr> (top)
    //
    // in all of these cases we want to map the information to a SubscriptExpr.
    // <sym> and <dsym> were left in the above examples for explanatory purposes,
    // but we don't actually pop them off the stack in this function.

    pushStackCheckpoint();

    auto r = SubscriptExpr{}; // slice_t

    popExpr(); // pop <subscr>

    try {
        switch (topExprOp()) {
            case EXPR_LIST_START: { // found a procset subscript, maybe with slice
                popExpr(); // pop <list>
                auto subscriptLen = popExpr<int>(); // pop <int> (list length)

                for (int i = 0; i < subscriptLen; i++) {
                    popExpr(); // pop <list element>
                    
                    if (topExprOp() == EXPR_INT || topExprOp() == EXPR_OPNEG) {
                        // single numbered subscript (not a slice)
                        auto idx = popExpr<int>(); // pop <int>, and <neg> if present
                        r.addSlice(idx, idx);
                    } else if (topExprOp() == EXPR_SLICE) {
                        // slice
                        popExpr(); // pop <slice>
                        auto high = popExpr<int>(); // pop <int>, and <neg> if present
                        auto low = popExpr<int>();  // pop <int>, and <neg> if present
                        r.addSlice(low, high);
                    } else {
                        throw ParseError("Tried to parse a subscript but found a "s + getExprOpStr(topExprOp()));
                    }
                }

                break; // success
            }

            case EXPR_OPNEG:
            case EXPR_INT: { // found an array subscript
                auto idx = popExpr<int>(); // pop <int>, and <neg> if present
                r.addSlice(idx, idx);
                break; // success
            }

            case EXPR_SLICE: { // found a sliced array subscript
                popExpr(); // pop <slice>
                auto high = popExpr<int>(); // pop <int>, and <neg> if present
                auto low = popExpr<int>();  // pop <int>, and <neg> if present
                r.addSlice(low, high);
                break; // success
            }

            default: {
                throw ParseError("Tried to parse a subscript but found a "s + getExprOpStr(topExprOp()));
            }
        }
    } catch (const ParseError &e) {
        // fix stack, propagate error
        restoreStackCheckpoint();
        throw e;
    }

    removeStackCheckpoint();
    return r;
}

template<>
ProgVarExpr ExprHndl::popExpr<ProgVarExpr>() {
    // program variables show up starting with <var>, e.g.:
    //
    //    <dsym> (procset name, e.g. "a") 
    //    <sym> (variable name, e.g. "myVar")
    //    <string>
    //    <int>
    //    <loc>
    //    <var> (top)
    //
    // <sym> and <dsym> can both optionally be preceded by a <subscript>, and
    // the <subscript> token itself has many arrangements (see popExpr<SubscriptExpr>).
    // e.g.:
    //
    //    <sym> (e.g. "myArray")
    //    ... many possibilities handled by popExpr<SubscriptExpr> ...
    //    <subscr>
    //    ... <loc> stuff ...
    //    <loc>
    //    <var> (top)
    //
    // the entire <var> might be wrapped in a <blockmap>, e.g.:
    //
    //    ...
    //    <var>
    //    <list element>
    //    <int>
    //    <list>
    //    <blockmap> (top)

    auto r = ProgVarExpr{};

    pushStackCheckpoint();

    try {
        switch (topExprOp()) {
            case EXPR_BLOCKMAP: {
                r.m_blockmapName = popExpr<std::string>();
                popExpr(); // pop <list>
                if (popExpr<int>() > 1) { // pop <int> (list length)
                    throw ParseError("Multi-part blockmap");
                }
                popExpr(); // pop <list element>

                if (topExprOp() != EXPR_VAR) {
                    throw ParseError("Blockmap without var");
                }
                // success, fall through to parse rest of <var>
            }

            case EXPR_VAR: {
                popExpr(); // pop <var> token
                r.m_location = popExpr<ProgLocExpr>();
                if (topExprOp() == EXPR_OPSUBSCR) {
                    r.m_symbolSubscript = popExpr<SubscriptExpr>();
                } // else keep default initialization
                r.m_symbolName = popExpr<std::string>();
                if (topExprOp() == EXPR_OPSUBSCR) {
                    r.m_procsetSubscript = popExpr<SubscriptExpr>();
                } // else keep default initialization
                r.m_procsetName = popExpr<std::string>();

                break; // success
            }

            default: {
                throw ParseError("Tried to parse a variable but found a "s + getExprOpStr(topExprOp()));
            }
        } 
    } catch (const ParseError& e) {
        // fix stack and propagate error
        restoreStackCheckpoint();
        throw e;
    }

    removeStackCheckpoint();
    return r;
}

// assemble specializations, public ////////////////////////////////////////////

template<>
AssertExpr ExprHndl::assemble<AssertExpr>() {
    auto r = AssertExpr{};
    r.m_op = popExpr<CompOpExpr>();
    // rhs appears first in the stack
    r.m_rhs = popExpr<ProgVarExpr>();
    r.m_lhs = popExpr<ProgVarExpr>();
    return r;
}
