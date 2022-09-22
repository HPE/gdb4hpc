/*******************************************************************************
 * Copyright 2022 Hewlett Packard Enterprise Development LP.
 ******************************************************************************/
#pragma once

// for parsing expressions and extracting structs/classes of basic types out of
// them. the goal of these classes is to merely parse text and organize it into
// a programmer friendly arrangement of more "text" (or primitive types). an
// example of this philosophy is the fact that, when parsing a procset
// ($a::{0..2}), we simply store the name and the subscript data and leave it to
// the user of the class to actually lookup the dbgprocset_t* and apply the
// subscript themselves.

#include <string>
#include <vector>
#include <memory>
#include <stdexcept>

#include "expr.h"
#include "slice.h"

// declared in gram.h, but avoiding large dbg_client.h include
extern void FreeExpr(expr_t *);

class ExpressionHandle {
    // a handle for an internal expr_t. after construction with an expression
    // string, ParseExpr() will be called internally and an expr_t will be
    // created and stored. from there, assemble<T> can be called to attempt to
    // construct an instance of T from the internal expr_t.

public:
    class ParseError : public std::runtime_error {
    public:
        ParseError(const std::string& msg) : std::runtime_error(msg) {};
    };

    // Read only, filterable view into an ExpressionHandle
    class View;

    // output types produced by assemble<T>

    struct ProgVarExpr; // program variable expression

    // "building block" types produced by popExpr<T>. output types are made of these

    struct ProgLocExpr;            // program location expression
    using SubscriptExpr = slice_t; // subscript expression

private:
    // used to flag that assemble<T>, filterOut<T>, or popExpr<T> is unimplemented for a type
    template <typename T> struct Unimplemented : std::false_type {};

public:
    // parse expr into internal tokens, ready to be assembled into output types
    // via assemble<T>. If a parsing error occurs, ExpressionHandle::ParseError is thrown
    ExpressionHandle(std::string expr);
    ExpressionHandle(ExpressionHandle&& other) = default;

    // Create an ExpressionHandle::View into this ExpressionHandle. The view is only
    // valid as long as the original ExpressionHandle is alive.
    View view() const;

    // assemble internal tokens into T starting from wherever the top of the internal
    // token stack happens to be. throws ExpressionHandle::ParseError on failure.
    template <typename T>
    T assemble() const;

    // assemble internal tokens into T, starting from an offset from the top of
    // the internal stack. throws ExpressionHandle::ParseError on failure.
    template <typename T>
    T assembleFromOffset(int offset) const;

    // access to inner expr_t
    const expr_t& raw() const;

    const std::string& string() const { return m_buf; }

private:
    // manipulators for underlying expr_t. none of these actually modify the underlying
    // expr_t *data*, only its stack top index, so they can be considered const.

    const expr_elt* popExpr() const;
    const expr_elt* topExpr() const;
    expr_op topExprOp() const;
    const expr_elt* lastExpr() const;
    void incExprIdx() const;
    // reset internal expression stack index to an offset from the top of the
    // internal expressions' stack
    void resetExprIdx(int offset = 0) const;
    bool exprIsEmpty() const;

    // return building block type T made out of token(s) on the top of the
    // internal expr_t's stack. on success, T is returned and the stack tokens
    // are consumed. upon failure, it resets the internal stack to the state it
    // was before the function was called and throws ExpressionHandle::ParseError.
    template <typename T>
    T popExpr() const {
        static_assert(Unimplemented<T>::value, "popExpr not implemented for T");
        return T{}; // suppress warning
    }

    // prints internal expr_t's stack and an arrow pointing to the current top.
    // useful in gdb.
    void printExpr() const;

    // original expression string
    const std::string m_buf;

    // internal expr_t. this should not be modified, with the exception of the
    // expr's internal stack pointer.
    // XXX: expr_t still uses legacy FreeExpr for destruction, which makes
    // declaring this as unique_ptr<const expr_t> impossible right now.
    std::unique_ptr<expr_t, decltype(&FreeExpr)> m_expr;
};

class ExpressionHandle::View {
    // An ExpressionHandle::View is a view into an ExpressionHandle that doesn't
    // own any of the underlying data. Its lifetime is the same as the lifetime of the
    // underlying ExpressionHandle. Views can be filtered, removing tokens or patterns of
    // tokens from the view, and they can be used as raw expr_t for expressions.
public:
    View(const ExpressionHandle& parent);

    const ExpressionHandle& parent() const;
    // access to inner expr_t
    const expr_t& raw() const;

    // filter out T from the expression. after filtering, there is no guarantee that the
    // expression is still semantically correct.
    template <typename T>
    void filterOut();

private:
    const ExpressionHandle& m_parent;

    std::unique_ptr<expr_t, decltype(&::free)> m_expr;
};

// for easy access to ExpressionHandle types 
// via "using namespace ExpressionHandleShorthands"
namespace ExpressionHandleShorthands {
    using ExprHndl = ExpressionHandle;
    using ProgVarExpr = ExpressionHandle::ProgVarExpr;
    using ProgLocExpr = ExpressionHandle::ProgLocExpr;
    using SubscriptExpr = ExpressionHandle::SubscriptExpr;
    using ParseError = ExpressionHandle::ParseError;
} // namespace

class ExpressionHandle::ProgLocExpr {
    // program location expression. a line number and a location name. the name
    // can be either a file name or a function name.
    // from e.g. 'c_type_test.x:123'
public:
    const auto& lineNumber() const { return m_lineNumber; }
    const auto& name() const { return m_name; }
private:
    friend ExpressionHandle;

    int m_lineNumber;
    std::string m_name;
};

class ExpressionHandle::ProgVarExpr {
    // program variable expression. a (possibly subscripted) (possibly repeated
    // with '@') symbol name located at a location inside of a (possibly
    // subscripted) procset name, all possibly wrapped in a blockmap name from
    // e.g. '$b{$a{0..2}::myArray[3..4]@2@"c_type_test.x":123}'
   public:
    // returns "" if no blockmap present
    const auto& blockmapName() const {return m_blockmapName; }
    const auto& location() const { return m_location; }
    const auto& symbolName() const { return m_symbolName; }
    // if no subscript, returned slice_t will have a getRank() of 0
    const auto& symbolSubscript() const { return m_symbolSubscript; }
    const auto& procsetName() const { return m_procsetName; }
    // if no subscript, returned slice_t will have a getRank() of 0
    const auto& procsetSubscript() const { return m_procsetSubscript; }
    // if no repeat, returns 0
    const auto& repeat() const { return m_repeat; }
private:
    friend ExpressionHandle;

    std::string m_blockmapName;
    ProgLocExpr m_location;
    std::string m_symbolName;
    SubscriptExpr m_symbolSubscript;
    std::string m_procsetName;
    SubscriptExpr m_procsetSubscript;
    unsigned int m_repeat;
};

template <typename T>
T ExpressionHandle::assemble() const {
    static_assert(Unimplemented<T>::value,
                  "ExpressionHandle::Assemble not implemented for T");
    return T{}; // suppress "control reaches end of non-void function" warning
}

template <typename T>
T ExpressionHandle::assembleFromOffset(int offset) const {
    resetExprIdx(offset);
    return assemble<T>();
}

template <typename T>
void ExpressionHandle::View::filterOut() {
    static_assert(ExpressionHandle::Unimplemented<T>::value,
                  "ExpressionHandle::View::FilterOut not implemented for T");
}

// available assemble specializations

template <>
ExpressionHandle::ProgVarExpr ExpressionHandle::assemble<ExpressionHandle::ProgVarExpr>() const;

// available filterOut specializations

template <>
void ExpressionHandle::View::filterOut<ExpressionHandle::ProgLocExpr>();
