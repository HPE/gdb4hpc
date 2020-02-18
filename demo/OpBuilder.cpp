#include "OpBuilder.h"
#include "OpFactory.h"
#include "VariableEval.h"
#include "FrontEnd.h"
#include "More.h"
#include "Compare.h"
#include "Error.h"
#include <regex>
#include <iostream>

OpBuilder::OpBuilder(const FrontEnd& frontEnd)
    : m_frontEnd(frontEnd)
{
}

std::shared_ptr<OStream> OpBuilder::getCommandStreamForProcSet(const std::string& procSet) const
{
    return m_frontEnd.getCommandStreamForProcSet(procSet);
}

std::shared_ptr<IStream> OpBuilder::getResultStreamForProcSet(const std::string& procSet) const
{
    return m_frontEnd.getResultStreamForProcSet(procSet);
}

bool OpBuilder::parse(const std::string& expression, std::shared_ptr<UnaryOp>& head,
                      std::shared_ptr<Op>& tail)
{
    std::cerr << "expression is: \"" << expression << "\"\n";

    std::regex pipeRE("\\s*(.*)\\$\\|\\s*(.*)");
    std::regex funcRE("\\s*\\$([a-z]+)\\((.*)\\)\\s*");
    std::regex varRE("\\s*\\$([ab])::a\\s*");
    std::regex diffRE("\\s*([$a-b:]+)\\s*-\\s*([$a-b:]+)\\s*");

    std::shared_ptr<IStream> nostream;
    std::shared_ptr<UnaryOp> noop;
    
    std::smatch match;
    if (std::regex_match(expression, match, pipeRE)) {
        std::cerr << "matched $|\n";

        std::shared_ptr<UnaryOp> head1, head2;
        std::shared_ptr<Op> tail1, tail2;

        if (!parse(match[1], head1, tail1))
            return false;

        if (!parse(match[2], head2, tail2))
            return false;

        tail1->pipeTo(head2);
        head = head1;
        tail = tail2;

        return true; 

    } else if (std::regex_match(expression, match, funcRE)) {
        std::string func = match[1];
        std::string args = match[2];

        std::cerr << "matched a function\n";
        std::cerr << "function: " << func << "\n";
        std::cerr << "arguments: " << args << "\n";

        m_opArgs = args;
        auto op = OpFactory::make(func, *this);
        if (!op) {
            std::cerr << "did not recognize function " << func;
            return false;
        }

        auto uop = std::dynamic_pointer_cast<UnaryOp>(op);
        
        if (!uop) {
            std::cerr << "not handling non unary funcs yet";
            return false;
        }

        tail = head = uop;
        addOp(op);
        return true;

    } else if (std::regex_match(expression, match, varRE)) {
        std::cerr << "got a variable reference on procset " << match[1] << "\n";

        std::string procSet = match[1];
        std::shared_ptr<OStream> commandStream = getCommandStreamForProcSet(procSet);
        std::shared_ptr<IStream> resultStream = getResultStreamForProcSet(procSet);
        
        auto op = std::make_shared<VariableEval>(procSet + "::a", commandStream,
                                                 resultStream);
        
        if (op) {
            head = noop;
            tail = op;
            addOp(op);
            return true;
        }

    } else if (std::regex_match(expression, match, diffRE)) {
        std::string var1 = match[1];
        std::string var2 = match[2];
        std::cerr << "got a difference " << var1 << " and " << var2 << "\n";
        
        std::shared_ptr<UnaryOp> head1, head2;
        std::shared_ptr<Op> tail1, tail2;

        if (!parse(var1, head1, tail1))
            return false;

        if (!parse(var2, head2, tail2))
            return false;

        auto op = std::make_shared<Compare>();
        tail1->pipeTo(op->getLHS());
        tail2->pipeTo(op->getRHS());

        head = noop;
        tail = op;
        addOp(op);

        return true;
    }

    return false;
}

void OpBuilder::addOp(std::shared_ptr<Op> op)
{
    if (!op) {
        throw Error("passing a null op to addOp");
    }
    
    m_ops.push_back(op);
}

void OpBuilder::addDefaultTerminator()
{
    if (!m_ops.empty() && !m_ops.back()->isTerminator()) {
        // we have a dangling result stream -- add a dumper for output
        auto op = std::make_shared<More>(0);
        m_ops.back()->pipeTo(op);
        addOp(op);
    }
}

void OpBuilder::evaluate()
{
    bool didStart = false;
    for (auto& op : m_ops) {
        if (op->start()) {
            // only does something for source operations
            didStart = true;
        }
    }

    if (didStart) {
        UnaryTerminator* term = dynamic_cast<UnaryTerminator*>(m_ops.back().get());
        if (term) 
            term->wait();
        else
            throw Error("expeced last operation to be a terminator");
    }
}
