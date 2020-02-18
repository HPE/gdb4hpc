#pragma once

#include "UnaryOp.h"
#include "Value.h"

class OpBuilder;

class Range : public UnaryOp
{
public:
    Range();

    static std::shared_ptr<Op> make(OpBuilder& builder);

    void apply(const Value& val) override;
    void done() override;
    
private:
    Value m_min;
    Value m_max;
};
