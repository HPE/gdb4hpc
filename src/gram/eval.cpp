/*******************************************************************************
 * Copyright 2022-2023 Hewlett Packard Enterprise Development LP.
 ******************************************************************************/

#include <iostream>
#include <sstream>

#include "gram.h"
#include "eval.h"
#include "dbg_client.h"
#include "blockmap_client.h"
#include "procset.h"

std::unique_ptr<Value>
gram::Expression::eval(rec_set_t*) const {
    std::cout << "evaluator unimplemented\n";
    print();
    throw EvalError{"unimplemented evaluator"};
}

std::unique_ptr<Value>
gram::Expression::evalTop(rec_set_t* rec_set) const {
    return eval(rec_set);
}

std::unique_ptr<ComparisonValue>
gram::Expression::evalCompare(rec_set_t*) const {
    throw EvalError{"Comparison expression is not a binary operator"};
}

std::unique_ptr<Value> gram::Int::eval(rec_set_t* rec_set) const {

    return std::make_unique<AIFValue>(rec_set, std::to_string(m_val),
                                      AIFStream::fromInt(m_val));
}

std::unique_ptr<Value> gram::Float::eval(rec_set_t* rec_set) const {
    // lookout for accuracy
    return std::make_unique<AIFValue>(rec_set, std::to_string(m_val),
                                      AIFStream::fromDouble(m_val));

}

std::unique_ptr<Value> gram::String::eval(rec_set_t* rec_set) const {
    // we could make this an AIFValue but with "" used for a gdb escape
    // there isn't really a way to access it.
    return std::make_unique<ReplyValue>(rec_set, '"' + m_val + '"');
}

std::unique_ptr<Value> gram::String::evalTop(rec_set_t* rec_set) const {
    return std::make_unique<ReplyValue>(rec_set, m_val);
}

std::unique_ptr<Value> gram::Sym::eval(rec_set_t* rec_set) const {

    if (!rec_set) {
        throw EvalError("cannot evaluate " + m_sym + " without a process");
    }
    return std::make_unique<ReplyValue>(rec_set, m_sym);
}

std::unique_ptr<Value> gram::DSym::eval(rec_set_t* rec_set) const {
    auto name = toString();
    if (FindBlockMap(name)) {
        return std::make_unique<BlockmapValue>(rec_set,name);
    } else if (auto [procSet, isTmp] = IdMap::getProcset(name); procSet) {
        auto locRecSet = CreateRecSetForProcset(procSet);
        auto threads = std::string{procSet->s_threads ? procSet->s_threads : ""};
        return std::make_unique<ProcsetValue>(locRecSet, isTmp ? procSet : nullptr,
                                              threads);
    } else {
        throw EvalError("not debug variable: $" + m_sym);
    }
}

std::unique_ptr<Value> gram::DSymArray::eval(rec_set_t* rec_set) const {
    if (auto bm = IdMap::getBlockmapForName("$" + m_sym)) {
        auto val = m_idcs->eval(rec_set);

        auto os = std::ostringstream{};
        os << "[[@" << bm->getElementsPerRank() << "]]";
        val->addMetaInfo(os.str());
        
        if (bm->isDegen()) {
            return std::make_unique<DistributedBlockMapValue>(rec_set, bm, std::move(val));
        } else {
            return std::make_unique<DecompValue>(rec_set, bm, std::move(val));
        }
    } else {
        m_idcs->setProcSet();   // range values need context
        return gram::DSym::eval(rec_set);
    }
}
    
std::unique_ptr<Value> gram::ScopedVal::eval(rec_set_t* rec_set) const {
    auto scopeVal = m_scope->eval(rec_set);

    if (auto psVal =  dynamic_cast<ProcsetValue*>(scopeVal.get())) {
        // using evalTop to support $a::"...."
        auto val = m_val->evalTop(rec_set);
        if (auto repVal = dynamic_cast<ReplyValue*>(val.get())) {
            // $psSet::value
            if (psVal->getRecSet() != repVal->getRecSet() &&
                repVal->getRecSet() != rec_set) {
                throw EvalError("procset scopes cannot be nested");
            }

            return std::make_unique<ReplyValue>(psVal->getRecSet(),
                                                repVal->getExpr());
        }
    } else if (auto scopeId = dynamic_cast<ReplyValue*>(scopeVal.get())) {
        // namespace::value; these will be passed to the back end
        auto val = m_val->eval(rec_set);
        if (auto repVal = dynamic_cast<ReplyValue*>(val.get())) {
            if (scopeId->getRecSet() != repVal->getRecSet()) {
                throw EvalError("procset scopes cannot be nested");
            }
            return std::make_unique<ReplyValue>(scopeId->getRecSet(),
                                                scopeId->getExpr() + "::" + repVal->getExpr());
        }
    }

    throw EvalError("unable to evaluate :: type"); 
}

std::unique_ptr<Value> gram::UnaryOp::eval(rec_set_t* rec_set) const {
    auto A = m_a->eval(rec_set);
    if (ReplyValue* ra = dynamic_cast<ReplyValue*>(A.get())) {
        return std::make_unique<ReplyValue>(ra->getRecSet(), m_op + ra->getExpr());
    } else {
        throw EvalError("cannot apply " + m_op + " to a value");
    }
}

std::unique_ptr<Value> gram::Binop::eval(rec_set_t* rec_set) const {
    auto A = m_a->eval(rec_set);
    auto B = m_b->eval(rec_set);

    if (rec_set && A->getRecSet() == rec_set && B->getRecSet() == rec_set) {
        return std::make_unique<ReplyValue>(rec_set, A->getExpr() + m_op + B->getExpr());
    } else {
        return std::make_unique<BinaryFuncValue>(std::move(A), m_op, std::move(B));
    }
}

std::unique_ptr<ComparisonValue> gram::Binop::evalCompare(rec_set_t* rec_set) const {
    static auto validOps = { "=", "==", "!=", "<", "<=", ">", ">=" };
    if (std::find(validOps.begin(), validOps.end(), m_op) == validOps.end()) {
        throw EvalError{m_op + " is not a comparison"};
    }

    auto op = (m_op == "=" ? "==" : m_op);

    auto A = m_a->eval(rec_set);
    auto B = m_b->eval(rec_set);

    return std::make_unique<ComparisonValue>(std::move(A), op, std::move(B));
}


std::unique_ptr<Value> gram::Range::eval(rec_set_t* rec_set) const {
    auto A = (m_a ? m_a->eval(rec_set) : nullptr);
    auto B = (m_b ? m_b->eval(rec_set) : nullptr);;
    if ((A && A->getRecSet() != rec_set) || (B && B->getRecSet() != rec_set)) {
        throw EvalError("evaluating an range with procset indices");
    }

    auto aexpr = (A ? A->getExpr() : "");
    auto bexpr = (B ? B->getExpr() : "");

    return std::make_unique<ReplyValue>(rec_set, aexpr + ".." + bexpr);
}

std::unique_ptr<Value> gram::Conditional::eval(rec_set_t* rec_set) const {
    auto A = m_a->eval(rec_set);
    auto B = m_b->eval(rec_set);
    auto C = m_c->eval(rec_set);
    if (A->getRecSet() != rec_set ||
        B->getRecSet() != rec_set ||
        C->getRecSet() != rec_set) {
        throw EvalError("evaluating a conditional with procset is not supported");
    }

    return std::make_unique<ReplyValue>(rec_set, A->getExpr() + "?" + B->getExpr() + ":" + C->getExpr());
}

std::unique_ptr<Value> gram::ArrayRef::eval(rec_set_t* rec_set) const {
    auto A = m_arr->eval(rec_set);
    auto B = m_idx->eval(rec_set);

    if (auto decomp = dynamic_cast<DecompValue*>(A.get())) {
        auto idx = '[' + B->getExpr() + ']';
        decomp->addIndex(idx);
        return A;
    } else if (B->getRecSet() != rec_set) {
        throw EvalError("evaluating an array with procset indices");
    }
    return std::make_unique<ReplyValue>(rec_set, A->getExpr() + "[" + B->getExpr() + "]");
}

std::unique_ptr<Value> gram::FuncCall::eval(rec_set_t* rec_set) const {
    if (!m_args) {
        return std::make_unique<ReplyValue>(rec_set, m_func->toString() + "()");
    }

    auto A = m_args->eval(rec_set);

    if (A->getRecSet() != rec_set) {
        throw EvalError("procset arguments of a function is not supported");
    }

    auto F = m_func->eval(rec_set);
    if (auto decomp = dynamic_cast<DecompValue*>(F.get())) {
        auto idx = '(' + A->getExpr() + ')';
        decomp->addIndex(idx);
        return F;
    } else {
        return std::make_unique<ReplyValue>(rec_set, F->getExpr() + "(" + A->getExpr() + ")");
    }
}

std::unique_ptr<Value> gram::List::eval(rec_set_t* rec_set) const {

    int n = 0;
    std::ostringstream os;

    for ( auto& elem : m_elems) {
        auto val = elem->eval(rec_set);
        if (val->getRecSet() != rec_set) {
            throw EvalError("evaluating a list with procset is not supported");
        }
        os << (n++ ? "," : "") << val->getExpr();
    }
    return std::make_unique<ReplyValue>(rec_set, os.str());
}


std::unique_ptr<Value> gram::SubExpr::eval(rec_set_t* rec_set) const {
    auto A = m_a->eval(rec_set);
    if (ReplyValue* ra = dynamic_cast<ReplyValue*>(A.get())) {
        return std::make_unique<ReplyValue>(ra->getRecSet(), "(" + ra->getExpr() + ")");
    } else {
        return A;
    }
}

std::unique_ptr<Value> gram::CastExpr::eval(rec_set_t* rec_set) const {
    auto A = m_val->eval(rec_set);
    if (rec_set && A->getRecSet() == rec_set) {
        return std::make_unique<ReplyValue>(rec_set, toString());
    } else {
        throw EvalError("cannot cast a procset value");
    }
}

std::unique_ptr<Value> gram::ThreadedProcsetExpr::eval(rec_set_t*) const {
    // This one just used toString() since the procset does all the processing.
    throw EvalError("Procset expression does have a value type");
}


