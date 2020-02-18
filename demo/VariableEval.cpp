#include "VariableEval.h"
#include "StreamReader.h"
#include "OStream.h"

VariableEval::VariableEval(const std::string& variableName,
                           std::shared_ptr<OStream> commandStream,
                           std::shared_ptr<IStream> resultStream)
    : Op("VariableEval"),
      m_variableName(variableName),
      m_commandStream(commandStream),
      m_reader(resultStream)
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
    m_reader.start();
    return true;
}

void VariableEval::pipeTo(std::shared_ptr<UnaryOp> dest)
{
    Op::pipeTo(dest);
    m_reader.pipeTo(dest);
}


