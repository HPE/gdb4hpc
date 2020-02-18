#pragma once

#include "UnaryOp.h"
#include "Value.h"

class OpBuilder;

class Gt : public UnaryOp
{
public:
    Gt(const Value& limit);
    static std::shared_ptr<Op> make(OpBuilder& builder);
    void apply(const Value& val);
private:
    Value m_limit;
};
