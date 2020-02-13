#pragma once

#include "Op.h"
#include <string>

class IStream; class OStream;

class VariableEval : public Op
{
public:
    VariableEval(const std::string& variableName,
                 std::shared_ptr<OStream> commandStream);

    const std::string& getVariableName() const;
    bool isSource() const override;
    bool start() const override;
private:
    std::string m_variableName;
    std::shared_ptr<OStream> m_commandStream;
};
