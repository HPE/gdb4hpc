#pragma once

#include <string>
#include <vector>

class IStream; class OStream; class Op; class FrontEnd;

class OpBuilder
{
public:
    OpBuilder(const FrontEnd& frontEnd);

    bool parse(const std::string& expr);

    std::shared_ptr<OStream> getCommandStreamForProcSet(const std::string& procSet) const;
    std::shared_ptr<IStream> getResultStreamForProcSet(const std::string& procSet) const;

    int getNumIStreams() const;

    const std::string& getOpArgs() { return m_opArgs; }

    std::shared_ptr<IStream> getNthIStream(int n) const;
    std::shared_ptr<OStream> makeOStream();

    void addDefaultTerminator();
    void evaluate();

private:
    void addOp(std::shared_ptr<Op> op);

    const FrontEnd& m_frontEnd;
    std::string m_expression;
    std::string m_opArgs;
    std::shared_ptr<IStream> m_from;
    std::shared_ptr<IStream> m_nextFrom;
    std::vector<std::shared_ptr<Op>> m_ops;
};

