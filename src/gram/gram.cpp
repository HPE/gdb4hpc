/*******************************************************************************
 * Copyright 2022-2023 Hewlett Packard Enterprise Development LP.
 ******************************************************************************/

#include "gram.h"
#include <iostream>
#include <sstream>

gram::Expression::~Expression() {}
void gram::Expression::indent(int level) const {
    for (int i=0; i<level; ++i) { std::cout << "  "; }
}

void gram::Expression::print(int level) const {
    indent(level);
    printL(level);
}

void gram::Expression::setProcSet() {}

std::vector<const gram::AssertExpr*> gram::Expression::getAssertions() const {
    return std::vector<const gram::AssertExpr*>{};
}

gram::Expression* gram::Expression::negate() {
    return new UnaryOp("-", this);
}

gram::Int::Int(int val) : m_val(val) {}

void gram::Int::printL(int level) const {
    std::cout << "Int: " << m_val << "\n";
}

std::string gram::Int::toString() const {
    return std::to_string(m_val);
}

gram::Expression* gram::Int::negate() {
    m_val = -m_val;
    return this;
}

gram::Float::Float(double val) : m_val(val) {}

void gram::Float::printL(int level) const {
    std::cout << "Float: " << m_val << "\n";
}

std::string gram::Float::toString() const {
    return std::to_string(m_val);
}

gram::Expression* gram::Float::negate() {
    m_val = -m_val;
    return this;
}

gram::String::String(const std::string& val) : m_val(val) {}

void gram::String::printL(int level) const {
    std::cout << "String: \"" << m_val << "\"\n";
}

std::string gram::String::toString() const {
    return '"' + m_val + '"';
}

gram::Sym::Sym(const std::string& sym) : m_sym(sym) {}

void gram::Sym::printL(int level) const {
    std::cout << "Sym: \"" << m_sym << "\"\n";
}

std::string gram::Sym::toString() const {
    return m_sym;
}

gram::DSym::DSym(const std::string& sym) : m_sym(sym) {}

void gram::DSym::printL(int level) const {
    std::cout << "DSym: \"" << m_sym << "\"\n";
}

std::string gram::DSym::toString() const {
    return '$' + m_sym;
}

gram::DSymArray::DSymArray(const std::string& sym, Expression* idcs) : DSym(sym), m_idcs(idcs) {}
gram::DSymArray::~DSymArray() { delete m_idcs; }
void gram::DSymArray::printL(int level) const {
    std::cout << "DSymArray: \"" << m_sym << "\"\n";
    m_idcs->print(level+1);
}

std::string gram::DSymArray::toString() const {
    return '$' + m_sym + '{' + m_idcs->toString() + '}';
}

std::vector<const gram::AssertExpr*> gram::DSymArray::getAssertions() const {
    return m_idcs->getAssertions();
}

gram::ScopedVal::ScopedVal(Expression* scope, Expression* val) :
    m_scope(scope), m_val(val) {}

gram::ScopedVal::~ScopedVal() {
    delete m_scope;
    delete m_val;
}

void gram::ScopedVal::printL(int level) const {
    std::cout << "::\n";
    m_scope->print(level+1);
    m_val->print(level+1);
}

std::string gram::ScopedVal::toString() const {
    return m_scope->toString() + "::" + m_val->toString();
}

gram::UnaryOp::UnaryOp(std::string op, Expression* a) :
    m_op(std::move(op)), m_a(a) {}

std::vector<const gram::AssertExpr*> gram::UnaryOp::getAssertions() const {
    return m_a->getAssertions();
}

void gram::UnaryOp::printL(int level) const {
    std::cout << m_op << "\n";
    m_a->print(level+1);
}

std::string gram::UnaryOp::toString() const {
    return m_op + m_a->toString();
}
gram::UnaryOp::~UnaryOp() { delete m_a; }

gram::Binop::Binop(Expression* a, std::string op, Expression* b) :
    m_op(std::move(op)), m_a(a), m_b(b) {}

void gram::Binop::printL(int level) const {
    std::cout << m_op << "\n";
    m_a->print(level+1);
    m_b->print(level+1);
}

std::string gram::Binop::toString() const {
    return m_a->toString() + m_op + m_b->toString();
}

std::vector<const gram::AssertExpr*> gram::Binop::getAssertions() const {
    auto a = m_a->getAssertions();
    auto b = m_b->getAssertions();
    a.insert(a.end(), b.begin(), b.end());
    return a;
}

gram::Binop::~Binop() { delete m_a; delete m_b; }

gram::Range::Range(Expression* a, Expression* b) : m_procSet(false), m_a(a), m_b(b) {}
gram::Range::~Range() { delete m_a; delete m_b; }

void gram::Range::printL(int level) const {
    std::cout << "..\n";
    m_a->print(level+1);
    m_b->print(level+1);
}

void gram::Range::setProcSet() { m_procSet = true; }

std::string gram::Range::toString() const {
    std::ostringstream os;
    os << m_a->toString() << ".." << m_b->toString();
    /*
    auto ia = dynamic_cast<Int*>(m_a), ib = dynamic_cast<Int*>(m_b);
    if (m_procSet) {
        os << m_a->toString() << ".." << m_b->toString();
    } else if (ia && ib) {
        auto A = ia->getVal(), B = ib->getVal();
        if (B < A) throw EvalError("upper bound of .. range is less than lower bound");
        else os << "[" << A << "]@" << B-A+1;
    } else {
        auto a = m_a->toString();
        auto b = m_b->toString();
        os <<  "[" << a << "]@(" << b <<  "- " << a << "+1)";
    }
    */
    return os.str();
}

gram::Conditional::Conditional(Expression* a, Expression* b, Expression* c) :
    m_a(a), m_b(b), m_c(c) {}

void gram::Conditional::printL(int level) const {
    std::cout << "?:" << "\n";
    m_a->print(level+1);
    m_b->print(level+1);
    m_c->print(level+1);
}

std::string gram::Conditional::toString() const {
    return m_a->toString() + "?" + m_b->toString() + ":" + m_c->toString();
}
gram::Conditional::~Conditional() { delete m_a; delete m_b; delete m_c; }

gram::ArrayRef::ArrayRef(Expression* arr, Expression* idx) :
    m_arr(arr), m_idx(idx)
{}

gram::ArrayRef::~ArrayRef() { delete m_arr; delete m_idx; }

void gram::ArrayRef::printL(int level) const {
    std::cout << "[]" << "\n";
    m_arr->print(level+1);
    m_idx->print(level+1);
}

std::string gram::ArrayRef::toString() const {
    return m_arr->toString() + '[' + m_idx->toString() + ']';
}

gram::FuncCall::FuncCall(Expression* func, Expression* args) : m_func(func), m_args(args) {}
gram::FuncCall::~FuncCall() { delete m_func; delete m_args; }
void gram::FuncCall::printL(int level) const {
    std::cout << "func-call\n";
    if (m_func) m_func->print(level+1);
    if (m_args) m_args->print(level+1);
}

std::string gram::FuncCall::toString() const {
    if (m_args) return m_func->toString() + "(" + m_args->toString() + ")";
    else return m_func->toString() + "()";
}

gram::List::List(Expression* a, Expression* b) {
    List* la = dynamic_cast<List*>(a);
    List* lb = dynamic_cast<List*>(b);
    if (la) {
        m_elems = std::move(la->m_elems);
        delete a;
    } else {
        m_elems.push_back(a);
    }

    if (lb) {
        std::copy(lb->m_elems.begin(), lb->m_elems.end(), std::back_inserter(m_elems));
        lb->m_elems.clear();
        delete b;
    } else {
        m_elems.push_back(b);
    }
}

gram::List::~List() { for (auto elem : m_elems) delete elem; }

void gram::List::printL(int level) const {
    std::cout << "List\n";
    for (auto& elem : m_elems) elem->print(level+1);
}

void gram::List::setProcSet() { 
    for (auto& elem : m_elems) {
        elem->setProcSet();
    }
}

std::string gram::List::toString() const {
    std::ostringstream os;
    int n = 0;
    for (auto& elem : m_elems) {
        os << (n++ ? "," : "") << elem->toString();
    }
    return os.str();
}

gram::SubExpr::SubExpr(Expression *a) : m_a(a) {}
gram::SubExpr::~SubExpr() { delete m_a; }
void gram::SubExpr::printL(int level) const {
    std::cout << "()\n";
    m_a->print(level+1);
}
std::string gram::SubExpr::toString() const {
    return '(' + m_a->toString() + ')';
}

gram::CastExpr::CastExpr(const std::string& type, Expression* val) :
    m_type(type), m_val(val) {}

gram::CastExpr::~CastExpr() {
    delete m_val;
}

std::string gram::CastExpr::toString() const {
    return '(' + m_type + ')' + m_val->toString();
}

void gram::CastExpr::printL(int level) const {
    std::cout << "(" << m_type << ")\n";
    m_val->print(level+1);
}

gram::AssertExpr::AssertExpr(Expression* a, const std::string& file, int lineno)
    : m_expr(a), m_file(file), m_lineno(lineno) {
}

gram::AssertExpr::~AssertExpr() { delete m_expr; }

std::pair<std::string, int> gram::AssertExpr::getLocation() const {
    return std::make_pair(m_file, m_lineno);
}

void gram::AssertExpr::printL(int level) const {
    std::cout << "Assertion at " << m_file << ":" << m_lineno << "\n";
    m_expr->print(level+1);
}

std::string gram::AssertExpr::toString() const {
    return m_expr->toString() + "@" + m_file + ":" + std::to_string(m_lineno);
}

std::unique_ptr<Value> gram::AssertExpr::eval(rec_set_t* rec_set) const {
    // we just assume we're evaluating at the right location
    return m_expr->eval(rec_set);
}

std::vector<const gram::AssertExpr*> gram::AssertExpr::getAssertions() const {
    return std::vector<const gram::AssertExpr*>{this};
}

gram::ThreadedProcsetExpr::ThreadedProcsetExpr(Expression* procs, Expression* threads) :
    m_procs(procs), m_threads(threads) {
    if (procs) procs->setProcSet();
    threads->setProcSet();
}

gram::ThreadedProcsetExpr::~ThreadedProcsetExpr() {
    delete m_procs;
    delete m_threads;
}

void gram::ThreadedProcsetExpr::printL(int level) const {
    std::cout << "Procset\n";
    if (m_procs) {
        m_procs->print(level+1);
    } else {
        indent(level+1);
        std::cout << "*\n";
    }
    m_threads->print(level+1);
}

std::string gram::ThreadedProcsetExpr::toString() const {
    auto ps = m_procs ? m_procs->toString() : "*";
    ps += ":";
    ps += m_threads->toString();
    return ps;
}
        
