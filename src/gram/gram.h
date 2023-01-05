/*******************************************************************************
 * Copyright 2022 Hewlett Packard Enterprise Development LP.
 ******************************************************************************/

#pragma once

#include <string>
#include <vector>
#include <memory>

#include "eval.h"


namespace gram {
    class AssertExpr;

    class Expression {
    public:
        virtual ~Expression();
        virtual std::unique_ptr<Value> eval(rec_set_t* rec_set) const = 0;
        virtual std::unique_ptr<Value> evalTop(rec_set_t* rec_set) const;
        virtual std::unique_ptr<ComparisonValue> evalCompare(rec_set_t* rec_set) const;
        void print(int level=0) const;
        virtual std::string toString() const = 0;
        virtual void setProcSet();
        virtual std::vector<const gram::AssertExpr*> getAssertions() const;
        virtual Expression* negate();

    protected:
        void indent(int level) const;
        virtual void printL(int level) const = 0;
    };

    class Int : public Expression {
    public:
        Int(int val);
        int getVal() const { return m_val; }
    private:
        void printL(int level) const override;
        std::string toString() const override;
        std::unique_ptr<Value> eval(rec_set_t* rec_set) const override;
        Expression* negate() override;
        int m_val;
    };

    class Float : public Expression {
    public:
        Float(double val);
    private:
        void printL(int level) const override;
        std::string toString() const override;
        std::unique_ptr<Value> eval(rec_set_t* rec_set) const override;
        Expression* negate() override;
        double  m_val;
    };

    class String : public Expression {
    public:
        String(const std::string& val);
    private:
        void printL(int level) const override;
        std::string toString() const override;
        std::unique_ptr<Value> eval(rec_set_t* rec_set) const override;
        std::unique_ptr<Value> evalTop(rec_set_t* rec_set) const override;
        std::string m_val;
    };

    class Sym : public Expression {
    public:
        Sym(const std::string& val);
    private:
        void printL(int level) const override;
        std::string toString() const override;
        std::unique_ptr<Value> eval(rec_set_t* rec_set) const override;
        std::string m_sym;
    };

    class DSym : public Expression {
    public:
        DSym(const std::string& val);

        void printL(int level) const override;
        std::string toString() const override;
        std::unique_ptr<Value> eval(rec_set_t* rec_set) const override;
    protected:
        std::string m_sym;
    };

    class DSymArray : public DSym {
    public:
        DSymArray(const std::string& sym, Expression* indcs);
        ~DSymArray();
        const Expression* getIdcs() const { return m_idcs; }

        std::string toString() const override;
        std::unique_ptr<Value> eval(rec_set_t* rec_set) const override;
        virtual std::vector<const gram::AssertExpr*> getAssertions() const;
    private:
        void printL(int level) const override;
        Expression* m_idcs;
    };

    class ScopedVal : public Expression {
    public:
        ScopedVal(Expression* scope, Expression* val);
        ~ScopedVal() override;

        const Expression* getScope() const { return m_scope; }
        const Expression* getVal() const { return m_val; }
        
        void printL(int level) const override;
        std::string toString() const override;
        std::unique_ptr<Value> eval(rec_set_t* rec_set) const override;

    private:

        Expression *m_scope;
        Expression *m_val;
    };

    class UnaryOp : public Expression {
    public:
        UnaryOp(std::string op, Expression* a);
        ~UnaryOp() override;

        std::string toString() const override;
        std::unique_ptr<Value> eval(rec_set_t* rec_set) const override;
        std::vector<const gram::AssertExpr*> getAssertions() const override;

    protected:
        void printL(int level) const override;
        std::string m_op;
        Expression* m_a;
    };

    class Binop : public Expression {
    public:
        Binop(Expression *a, std::string op, Expression* b);
        ~Binop() override;

        const Expression* getA() const { return m_a; };
        const Expression* getB() const { return m_b; };
        const std::string& getOp() const { return m_op; };

        std::string toString() const override;
        std::unique_ptr<Value> eval(rec_set_t* rec_set) const override;
        std::unique_ptr<ComparisonValue> evalCompare(rec_set_t* rec_set) const override;
        std::vector<const gram::AssertExpr*> getAssertions() const override;

    protected:
        void printL(int level) const override;
        std::string m_op;
        Expression* m_a;
        Expression* m_b;
    };

    class Range : public Expression {
    public:
        Range(Expression *a, Expression* b);
        ~Range() override;

        void printL(int level) const override;
        std::string toString() const override;
        std::unique_ptr<Value> eval(rec_set_t* rec_set) const override;
        void setProcSet() override;
    private:
        bool m_procSet;
        Expression* m_a;
        Expression* m_b;
    };

    class Conditional : public Expression {
    public:
        Conditional(Expression* a, Expression *b, Expression* c);
        ~Conditional() override;

        std::string toString() const override;
        std::unique_ptr<Value> eval(rec_set_t* rec_set) const override;

    private:
        void printL(int level) const override;
        Expression *m_a, *m_b, *m_c;
    };

    class ArrayRef : public Expression {
    public:
        ArrayRef(Expression* arr, Expression* idx);
        ~ArrayRef() override;

        std::string toString() const override;
        std::unique_ptr<Value> eval(rec_set_t* rec_set) const override;

    private:
        void printL(int level) const override;
        Expression* m_arr;
        Expression* m_idx;
    };

    class FuncCall : public Expression {
    public:
        FuncCall(const std::string& func, Expression* args);
        ~FuncCall() override;
    private:
        void printL(int level) const override;
        std::string toString() const override;
        std::unique_ptr<Value> eval(rec_set_t* rec_set) const override;
        std::string  m_func;
        Expression* m_args;
    };

    class List : public Expression {
    public:
        List(Expression* a, Expression* b);
        ~List() override;
        const std::vector<Expression*>& getElems() const { return m_elems; }

        std::string toString() const override;
        std::unique_ptr<Value> eval(rec_set_t* rec_set) const override;

    private:
        void printL(int level) const override;
        void setProcSet() override;
        std::vector<Expression*> m_elems;
    };

    class SubExpr : public Expression {
    public:
        SubExpr(Expression* a);
        ~SubExpr() override;

        std::string toString() const override;
        std::unique_ptr<Value> eval(rec_set_t* rec_set) const override;        

    private:
        void printL(int level) const override;
        Expression* m_a;
    };

    class CastExpr : public Expression {
    public:
        CastExpr(const std::string& type, Expression* val);
        ~CastExpr();

        std::string toString() const override;
        std::unique_ptr<Value> eval(rec_set_t* rec_set) const override;
    private:
        void printL(int level) const override;
        std::string m_type;
        Expression* m_val;
    };

    class AssertExpr : public Expression {
    public:
        AssertExpr(Expression* a, const std::string& file, int lineno);
        ~AssertExpr() override;

        std::pair<std::string, int> getLocation() const;
        const Expression* getExpr() const { return m_expr; }
        
        void printL(int level) const override;
        std::string toString() const override;
        std::unique_ptr<Value> eval(rec_set_t* rec_set) const override;
        std::vector<const gram::AssertExpr*> getAssertions() const override;

    private:
        Expression* m_expr;
        std::string m_file;
        int m_lineno;
    };
}

extern std::unique_ptr<gram::Expression> ParseString(const std::string& expr);

