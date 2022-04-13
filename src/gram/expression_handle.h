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

    // output types produced by assemble<T>

    struct AssertExpr;             // assert expression

    // "building block" types produced by popExpr<T>. output types are made of these

    enum class CompOpExpr;         // comparison operator expression
    struct ProgVarExpr;            // program variable expression
    struct ProgLocExpr;            // program location expression
    using SubscriptExpr = slice_t; // subscript expression

private:
    // used to flag that assemble<T> or popExpr<T> is unimplemented for a type
    template <typename T> struct Unimplemented : std::false_type {};

public:
    // parse expr into internal tokens, ready to be assembled into output types 
    // via assemble<T>. If a parsing error occurs, ExpressionHandle::ParseError is thrown
    ExpressionHandle(std::string&& expr);

    // assemble internal tokens into T. throws ExpressionHandle::ParseError on failure
    template <typename T>
    T assemble() {
        static_assert(Unimplemented<T>::value, "Assemble not implemented for T");
        return T{}; // suppress "control reaches end of non-void function" warning
    }

    // access to inner expr_t
    expr_t* raw() { return m_expr.get(); }

private:
    // manipulators for underlying expr_t

    const expr_elt* popExpr();
    const expr_elt* topExpr() const;
    expr_op topExprOp() const;
    const expr_elt* lastExpr() const;
    void incExprIdx();
    void resetExprIdx();
    bool exprIsEmpty() const;

    // return building block type T made out of token(s) on the top of the 
    // internal expr_t's stack. on success, T is returned and the stack tokens 
    // are consumed. upon failure, it resets the internal stack to the state it 
    // was before the function was called and throws ExpressionHandle::ParseError.
    template <typename T>
    T popExpr() {
        static_assert(Unimplemented<T>::value, "popExpr not implemented for T");
        return T{}; // suppress warning
    }

    // helper functions to help with resetting the internal expr_t's stack upon 
    // error in popExpr<T>. stores checkpoints on a stack so these functions can
    // be used in recursive functions.

    void pushStackCheckpoint();
    void restoreStackCheckpoint();
    void removeStackCheckpoint();

    // prints internal expr_t's stack and an arrow pointing to the current top.
    // useful in gdb.
    void printExpr() const;

    // original expression string
    const std::string m_buf;
    // internal expr_t
    std::unique_ptr<expr_t, decltype(&FreeExpr)> m_expr;
    // stack of checkpoint indices used by *StackCheckpoint()
    std::vector<int> m_stackCheckpoints;
};

// for easy access to ExpressionHandle types 
// via "using namespace ExpressionHandleShorthands"
namespace ExpressionHandleShorthands {
    using ExprHndl = ExpressionHandle;
    using CompOpExpr = ExpressionHandle::CompOpExpr;
    using ProgVarExpr = ExpressionHandle::ProgVarExpr;
    using ProgLocExpr = ExpressionHandle::ProgLocExpr;
    using SubscriptExpr = ExpressionHandle::SubscriptExpr;
    using AssertExpr = ExpressionHandle::AssertExpr;
    using ParseError = ExpressionHandle::ParseError;
} // namespace

enum class ExpressionHandle::CompOpExpr {
    // binary comparison operator
    // from e.g. '==', '!=', '<', etc...
    None,
    Equal, NotEqual,
    LessThan, LessThanEqual,
    GreaterThan, GreaterThanEqual
};

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

class ExpressionHandle::AssertExpr {
    // assertion expression. a binary comparison operator with a left-hand side
    // (lhs) and a right-hand side (rhs).
    // from e.g. 'assert $a::...@...:... == $b::...@...:...'
public:
    const auto& compareOp() const { return m_op; }
    const auto& rightHandSide() const { return m_rhs; }
    const auto& leftHandSide() const { return m_lhs; }
private:
    friend ExpressionHandle;

    CompOpExpr m_op;
    ProgVarExpr m_rhs;
    ProgVarExpr m_lhs;
};

template<>
ExpressionHandle::AssertExpr
ExpressionHandle::assemble<ExpressionHandle::AssertExpr>();
