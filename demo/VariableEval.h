#pragma once

#include "Op.h"
#include "StreamReader.h"
#include <string>

class IStream; class OStream;

//! Operator for accessing the value of a program variable
/*! A program variable value is just an operator that acts
  as a "source" pumping out the values of an array one by one
  so they can be processed in sequence.
*/
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
