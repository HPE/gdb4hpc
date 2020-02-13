#include "VariableEval.h"
#include "OStream.h"

VariableEval::VariableEval(const std::string& variableName,
                           std::shared_ptr<OStream> commandStream)
    : Op("VariableEval"),
      m_variableName(variableName),
      m_commandStream(commandStream)
{
}

const std::string& VariableEval::getVariableName() const {
    return m_variableName;
}

bool VariableEval::isSource() const {
    return true;
}

bool VariableEval::start() const {
    m_commandStream->putInt(12);
    return true;
}


