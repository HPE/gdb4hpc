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
template<> int ExprHndl::popExpr<int>() const ;
template<> std::string ExprHndl::popExpr<std::string>() const;

// structs
template<> ProgLocExpr ExprHndl::popExpr<ProgLocExpr>() const;
template<> ProgVarExpr ExprHndl::popExpr<ProgVarExpr>() const;
template<> SubscriptExpr ExprHndl::popExpr<SubscriptExpr>() const;

namespace {

class ExprTransaction {
    // RAII device to save the state of an expr_t's stack and restore it on
    // scope exit. use commit() to disable state reset and commit any
    // changes made to the expr_t's stack.
    //
    // this *only* saves the stack position. if the expr_t grows and/or is moved in
    // memory, this will break.
public:
    ExprTransaction(const expr_t& expr) : m_expression{expr}, m_index{expr.e_eval} {}
    ~ExprTransaction() {
        if (m_revert_on_exit)
            m_expression.e_eval = m_index;
    }

    void commit() { m_revert_on_exit = false; }

private:
    const expr_t& m_expression;
    int m_index;
    bool m_revert_on_exit = true;
};

} // anonymous namespace

static void printExpr(const expr_t* expr) {
    // useful in gdb
    if (!expr) {
        fprintf(stderr, "(nullptr)\n");
        return;
    }

    fprintf(stderr, "(BOTTOM)\n");
    for (int i = 0; i < expr->e_lval; i++) {
        // print arrow
        std::string arrow = i == expr->e_eval ? "->" : "  ";
        fprintf(stderr, "%s ", arrow.c_str());
        // print type
        fprintf(stderr, "%s ", getExprOpStr(expr->e_expr[i].e_op));
        // print value
        switch (expr->e_expr[i].e_op) {
        case EXPR_INT:    fprintf(stderr, "%d\n", expr->e_expr[i].e_val.e_v_int);         break;
        case EXPR_SYM:    fprintf(stderr, "%s\n", expr->e_expr[i].e_val.e_v_sym_str);     break;
        case EXPR_DSYM:   fprintf(stderr, "$%s\n", expr->e_expr[i].e_val.e_v_sym->h_key); break;
        case EXPR_STRING: fprintf(stderr, "%s\n" , expr->e_expr[i].e_val.e_v_string);     break;
        default:          fprintf(stderr, "\n"); break;
        }
    }
    fprintf(stderr, "(TOP)\n");
}

void ExpressionHandle::printExpr() const {
    ::printExpr(m_expr.get());
}

ExpressionHandle::ExpressionHandle(std::string str)
    : m_buf{std::move(str)}
    , m_expr(ParseExpr(m_buf.c_str()), FreeExpr)
{
    if (!m_expr) {
        // ParseExpr uses error.h interface to report errors
        throw ParseError("Parsing of \"" + m_buf + "\" failed: " + GetError());
    }
    resetExprIdx();
}

const expr_t& ExpressionHandle::raw() const {
    resetExprIdx();
    return *m_expr;
}

const expr_elt* ExpressionHandle::popExpr() const {
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

void ExpressionHandle::incExprIdx() const {
    m_expr->e_eval++;
}

void ExpressionHandle::resetExprIdx(int offset) const {
    if (offset >= m_expr->e_lval) {
        throw ParseError("Offset out of bounds");
    }
    //                   top of stack
    m_expr->e_eval = (m_expr->e_lval - 1) - offset;
}

bool ExpressionHandle::exprIsEmpty() const {
    return m_expr->e_eval < 0;
}

// popExpr specializations, private to ExpressionHandle ////////////////////////

template<>
int ExprHndl::popExpr<int>() const {
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
std::string ExprHndl::popExpr<std::string>() const {
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
ProgLocExpr ExprHndl::popExpr<ProgLocExpr>() const {
    auto t = ExprTransaction(*m_expr);
    auto r = ProgLocExpr{};

    // pop operations will throw on error

    popExpr(); // pop <loc> token
    r.m_lineNumber = popExpr<int>();
    r.m_name = popExpr<std::string>();

    t.commit();
    return r;
}

template<>
SubscriptExpr ExprHndl::popExpr<SubscriptExpr>() const {
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

    auto r = SubscriptExpr{}; // slice_t
    auto t = ExprTransaction(*m_expr);

    popExpr(); // pop <subscr>

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

    t.commit();
    return r;
}

template<>
ProgVarExpr ExprHndl::popExpr<ProgVarExpr>() const {
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
    // <sym> (and its optional <subscript>) might be preceded by <repeat>, the '@' operator.
    // e.g. intarray@3
    //
    //    ...
    //    <int> (3)
    //    <repeat> ('@')
    //    ... <loc>, <subscr> stuff ...
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
    auto t = ExprTransaction(*m_expr);

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
            if (topExprOp() == EXPR_REPEAT) {
                popExpr(); // pop <repeat>
                if (auto num = popExpr<int>(); num >= 0) { // pop <int>
                    r.m_repeat = num;
                } else {
                    throw ParseError("Can't repeat a negative amount of times: " + std::string{"@"} + std::to_string(num));
                }
            }
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

    t.commit();
    return r;
}

ExpressionHandle::View ExpressionHandle::view() const {
    return ExpressionHandle::View(*this);
}

ExpressionHandle::View::View(const ExpressionHandle& parent)
    : m_parent{parent}
    , m_expr(nullptr, &::free)
{
    // XXX mimicking legacy behavior from expr.cpp::do_parse...
    m_expr.reset((expr_t*)malloc(sizeof(expr_t) + sizeof(expr_elt) * parent.raw().e_len));
    m_expr->e_len = parent.raw().e_len;
    m_expr->e_lval = parent.raw().e_lval;
    m_expr->e_eval = parent.raw().e_eval;

    // copy tokens
    for (int i = 0; i < parent.raw().e_lval; ++i) {
        m_expr->e_expr[i] = parent.raw().e_expr[i];
    }
}

const expr_t& ExpressionHandle::View::raw() const {
    // reset stack pointer
    m_expr->e_eval = m_expr->e_lval - 1;
    return *m_expr;
}

// assemble specializations, public ////////////////////////////////////////////

template<>
ProgVarExpr ExprHndl::assemble<ProgVarExpr>() const {
    // no transaction commit, only used to reset stack state after assembly
    auto t = ExprTransaction(*m_expr);
    return popExpr<ProgVarExpr>();
}

// filterOut specializations, public ///////////////////////////////////////////

template <>
void ExprHndl::View::filterOut<ProgLocExpr>() {
    // ProgLocExpr shows up on the stack as:
    //   <string>
    //   <int>
    //   <loc> (top)

    // clear all ProgLocExpr patterns
    auto top = m_expr->e_lval - 1;
    for (int i = top; i >= 2; --i) {
        if (m_expr->e_expr[i].e_op == EXPR_LOC &&
            m_expr->e_expr[i - 1].e_op == EXPR_INT &&
            m_expr->e_expr[i - 2].e_op == EXPR_STRING)
        {
            m_expr->e_expr[i--].e_op = EXPR_UNKNOWN;
            m_expr->e_expr[i--].e_op = EXPR_UNKNOWN;
            m_expr->e_expr[i--].e_op = EXPR_UNKNOWN;
        };
    }

    // collapse
    {
        int write = 0;
        for (int read = 0; read < m_expr->e_lval; ++read) {
            // skip EXPR_UNKNOWNS
            while (m_expr->e_expr[read].e_op == EXPR_UNKNOWN &&
                   read < m_expr->e_lval)
                ++read;
            m_expr->e_expr[write++] = m_expr->e_expr[read];
        }
        m_expr->e_lval = write;
    }

    // reset stack pointer
    m_expr->e_eval = m_expr->e_lval - 1;
}
