#pragma once

#include "Op.h"
#include "StreamReader.h"
#include <string>

class IStream; class OStream;

class VariableEval : public Op
{
public:
    VariableEval(const std::string& variableName,
                 std::shared_ptr<OStream> commandStream,
                 std::shared_ptr<IStream> resultStream);

    const std::string& getVariableName() const;
    bool isSource() const override;
    bool start() const override;
    void pipeTo(std::shared_ptr<UnaryOp> dest) override;
private:
    std::string m_variableName;
    std::shared_ptr<OStream> m_commandStream;
    StreamReader m_reader;
};
