#pragma once

#include "UnaryOp.h"
#include "Value.h"

class OpBuilder;

//! Operator to compute the min-max range of a set of data
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
