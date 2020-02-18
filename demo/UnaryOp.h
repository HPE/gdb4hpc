#pragma once

#include "Op.h"
class Value;

class UnaryOp : public Op
{
public:
    UnaryOp(const std::string& name);

    virtual void apply(const Value& val) = 0;

};
