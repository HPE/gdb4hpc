#pragma once

#include "UnaryOp.h"
#include "Value.h"

class OpBuilder;

class Gt : public UnaryOp
{
public:
    Gt(std::shared_ptr<IStream> in, std::shared_ptr<OStream> out, Value limit);

    static std::shared_ptr<Op> make(OpBuilder& builder);

    virtual void apply();
private:
    Value m_limit;
};
