#pragma once

#include "Op.h"
class Value;

//! Base class for operators that input a single value.
class UnaryOp : public Op
{
public:
    UnaryOp(const std::string& name);

    virtual void apply(const Value& val) = 0;

};
