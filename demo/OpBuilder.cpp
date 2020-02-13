#include "OpBuilder.h"
#include "OpFactory.h"
#include "VariableEval.h"
#include "Connection.h"
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

int OpBuilder::getNumIStreams() const
{
    return m_from ? 1 : 0;
}

std::shared_ptr<IStream> OpBuilder::getNthIStream(int n) const
{
    if (n <= 1)
        return m_from;
    else
        return nullptr;
}
std::shared_ptr<OStream> OpBuilder::makeOStream()
{
    auto connect = Connection::make();
    m_nextFrom = connect->getReader();
    return connect->getWriter();
}

std::shared_ptr<OStream> OpBuilder::getCommandStreamForProcSet(const std::string& procSet) const
{
    return m_frontEnd.getCommandStreamForProcSet(procSet);
}

std::shared_ptr<IStream> OpBuilder::getResultStreamForProcSet(const std::string& procSet) const
{
    return m_frontEnd.getResultStreamForProcSet(procSet);
}

bool OpBuilder::parse(const std::string& expression)
{
    std::cerr << "expression is: \"" << expression << "\"\n";

    std::regex pipeRE("\\s*(.*)\\$\\|\\s*(.*)");
    std::regex funcRE("\\s*\\$([a-z]+)\\((.*)\\)\\s*");
    std::regex varRE("\\s*\\$([ab])::a\\s*");
    std::regex diffRE("\\s*([$a-b:]+)\\s*-\\s*([$a-b:]+)\\s*");

    std::shared_ptr<IStream> nostream;
    std::shared_ptr<Op> noop;
    
    std::smatch match;
    if (std::regex_match(expression, match, pipeRE)) {
        std::cerr << "matched $|\n";
        
        if (!parse(match[1]))
            return false;

        if (!parse(match[2]))
            return false;

        return true;

    } else if (std::regex_match(expression, match, funcRE)) {
        std::string func = match[1];
        std::string args = match[2];

        std::cerr << "matched a function\n";
        std::cerr << "function: " << func << "\n";
        std::cerr << "arguments: " << args << "\n";

        m_opArgs = args;
        auto op = OpFactory::make(func, *this);

        if (op) {
            addOp(op);
            return true;
        }

    } else if (std::regex_match(expression, match, varRE)) {
        std::cerr << "got a variable reference on procset " << match[1] << "\n";

        std::string procSet = match[1];
        std::shared_ptr<OStream> commandStream = getCommandStreamForProcSet(procSet);
        std::shared_ptr<IStream> resultStream = getResultStreamForProcSet(procSet);
        
        auto op = std::make_shared<VariableEval>(procSet + "::a", commandStream);
        
        m_nextFrom = resultStream;
        if (op) {
            addOp(op);
            return true;
        }
    } else if (std::regex_match(expression, match, diffRE)) {
        std::string var1 = match[1];
        std::string var2 = match[2];
        std::cerr << "got a difference " << var1 << " and " << var2 << "\n";
        
        if (!parse(var1))
            return false;

        auto in1 = getNthIStream(0);
        
        if (!parse(var2))
            return false;

        auto in2 = getNthIStream(1);

        if (!in1 || !in2) {
            std::cerr << "using a terminator for input to difference\n";
        }
        
        auto conn = Connection::make();
        auto op = std::make_shared<Compare>(in1, in2, conn->getWriter());
        m_nextFrom = conn->getReader();

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
    
    if (!m_nextFrom && !op->isTerminator()) {
        throw Error("no input stream for next operation");
    }
    if (m_nextFrom && op->isTerminator()) {
        throw  Error("output stram from a terminating operation");
    }

    m_ops.push_back(op);
    m_from = m_nextFrom;
    m_nextFrom = nullptr;
}

void OpBuilder::addDefaultTerminator()
{
    if (m_from) {
        // we have a dangling result stream -- add a dumper for output
        auto op = std::make_shared<More>(m_from,0);
        addOp(op);
    }
}

void OpBuilder::evaluate()
{
    bool didStart = false;
    for (auto op : m_ops) {
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
